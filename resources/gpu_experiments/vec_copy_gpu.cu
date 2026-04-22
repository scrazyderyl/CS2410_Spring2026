/////////////////////////////////////////////////////////////////////////
// vector_copy_gpu.cu
//
// A minimal streaming kernel designed to approach the P100's peak DRAM
// bandwidth. Each thread reads one element and writes one element, with
// perfectly coalesced access: thread T in a warp reads in[base+T] and
// writes out[base+T], so a warp's 32 loads collapse into a single
// 128-byte cache line transaction.
//
// Compile:  nvcc -arch=sm_60 -O3 vector_copy_gpu.cu -o vector_copy_gpu
// Profile:  nvprof --metrics dram_read_throughput,dram_write_throughput,\
//                           gld_efficiency,gst_efficiency,achieved_occupancy \
//                  ./vector_copy_gpu <num_elements> <block_size> <iterations>
//
// Intended use: reference point for "what does peak bandwidth look like?"
// Compare results here against mat_vec_gpu to see how algorithmic access
// patterns and coalescing limits real-world kernel performance.
/////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <cuda_runtime.h>

// Pointers to device memory
float* d_in  = NULL;
float* d_out = NULL;

double time_memcpy  = 0;
double time_compute = 0;

/////////////////////////////////////////////////////////////////////////
// Convenience: fetch the current wall-clock time in seconds.
/////////////////////////////////////////////////////////////////////////
static double now_seconds()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

/////////////////////////////////////////////////////////////////////////
// Check a CUDA call and abort with the real error if it failed.
/////////////////////////////////////////////////////////////////////////
#define CUDA_CHECK(call)                                                     \
	do {                                                                     \
		cudaError_t _e = (call);                                             \
		if (_e != cudaSuccess) {                                             \
			fprintf(stderr, "CUDA error at %s:%d: %s\n",                     \
			        __FILE__, __LINE__, cudaGetErrorString(_e));             \
			exit(1);                                                         \
		}                                                                    \
	} while (0)

/////////////////////////////////////////////////////////////////////////
// Simple streaming copy kernel.
//
// Access pattern:
//   thread i reads  in[i]
//   thread i writes out[i]
//
// Within a warp, the 32 consecutive threads hit 32 consecutive floats
// (128 bytes) -> a single coalesced memory transaction per warp.
/////////////////////////////////////////////////////////////////////////
__global__ void vector_copy(float* out, const float* in, int n)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < n) {
		out[i] = in[i];
	}
}

/////////////////////////////////////////////////////////////////////////
// Variant: each thread copies 4 consecutive floats (a float4).
//
// This issues wider loads/stores (16 bytes per thread), which on Pascal
// often gets closer to peak bandwidth than plain float copies because
// the memory system prefers fewer, wider transactions.  Left here as
// a natural follow-up experiment for students.
/////////////////////////////////////////////////////////////////////////
__global__ void vector_copy_float4(float4* out, const float4* in, int n4)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < n4) {
		out[i] = in[i];
	}
}

/////////////////////////////////////////////////////////////////////////
// Allocate device memory and copy input from host.
/////////////////////////////////////////////////////////////////////////
void copy_host_to_device(const float* h_in, int n)
{
	double t0 = now_seconds();

	CUDA_CHECK(cudaMalloc((void**)&d_in,  sizeof(float) * (size_t)n));
	CUDA_CHECK(cudaMalloc((void**)&d_out, sizeof(float) * (size_t)n));
	CUDA_CHECK(cudaMemcpy(d_in, h_in, sizeof(float) * (size_t)n,
	                      cudaMemcpyHostToDevice));

	time_memcpy += now_seconds() - t0;
}

/////////////////////////////////////////////////////////////////////////
// Copy result back and free device memory.
/////////////////////////////////////////////////////////////////////////
void copy_device_to_host(float* h_out, int n)
{
	double t0 = now_seconds();

	CUDA_CHECK(cudaMemcpy(h_out, d_out, sizeof(float) * (size_t)n,
	                      cudaMemcpyDeviceToHost));
	CUDA_CHECK(cudaFree(d_in));
	CUDA_CHECK(cudaFree(d_out));

	time_memcpy += now_seconds() - t0;
}

/////////////////////////////////////////////////////////////////////////
// Program main
/////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	if (argc != 4) {
		printf("Usage: vector_copy_gpu <num_elements> <block_size> <iterations>\n");
		printf("  num_elements   number of float elements to copy\n");
		printf("  block_size     CUDA block size (threads per block)\n");
		printf("  iterations     how many times to launch the kernel\n");
		printf("                 (more iterations amortize launch overhead)\n");
		exit(2);
	}

	int N          = atoi(argv[1]);
	int block_size = atoi(argv[2]);
	int iters      = atoi(argv[3]);

	if (N <= 0 || block_size <= 0 || iters <= 0) {
		fprintf(stderr, "All arguments must be positive integers.\n");
		exit(2);
	}

	printf("N = %d elements (%.2f MB), block_size = %d, iterations = %d\n",
	       N, (double)N * sizeof(float) / (1024.0 * 1024.0),
	       block_size, iters);

	// Host buffers
	float* h_in  = (float*)malloc(sizeof(float) * (size_t)N);
	float* h_out = (float*)malloc(sizeof(float) * (size_t)N);
	if (!h_in || !h_out) {
		fprintf(stderr, "Host malloc failed.\n");
		exit(1);
	}

	srand(2026);
	for (int i = 0; i < N; i++) {
		h_in[i] = rand() / (float)RAND_MAX;
	}

	copy_host_to_device(h_in, N);

	int nblocks = (N + block_size - 1) / block_size;

	// Warm-up launch to exclude one-time driver/JIT overhead from timing.
	vector_copy<<<nblocks, block_size>>>(d_out, d_in, N);
	CUDA_CHECK(cudaDeviceSynchronize());

	// Timed launches.
	double t0 = now_seconds();
	for (int it = 0; it < iters; it++) {
		vector_copy<<<nblocks, block_size>>>(d_out, d_in, N);
	}
	CUDA_CHECK(cudaDeviceSynchronize());
	time_compute = now_seconds() - t0;

	copy_device_to_host(h_out, N);

	// Verify correctness on a few sample elements.
	int errors = 0;
	for (int i = 0; i < N; i += (N / 10 > 0 ? N / 10 : 1)) {
		if (h_out[i] != h_in[i]) {
			errors++;
			if (errors <= 5) {
				fprintf(stderr, "Mismatch at %d: in=%f out=%f\n",
				        i, h_in[i], h_out[i]);
			}
		}
	}

	// Bandwidth: each iteration reads N floats and writes N floats.
	double bytes_per_iter = 2.0 * (double)N * sizeof(float);
	double total_bytes    = bytes_per_iter * iters;
	double bw_gb_per_s    = (total_bytes / time_compute) / 1e9;

	printf("Memcpy Time:   %f seconds\n", time_memcpy);
	printf("Compute Time:  %f seconds (%d iterations)\n", time_compute, iters);
	printf("Per-iteration: %f ms\n",
	       1000.0 * time_compute / (double)iters);
	printf("Bytes moved:   %.2f GB total, %.2f GB per iteration\n",
	       total_bytes / 1e9, bytes_per_iter / 1e9);
	printf("Bandwidth:     %.2f GB/s (read + write combined)\n", bw_gb_per_s);
	printf("P100 peak:     732 GB/s -> %.1f%% of peak\n",
	       100.0 * bw_gb_per_s / 732.0);
	printf("Sample check:  %d mismatches across sampled elements\n", errors);

	free(h_in);
	free(h_out);
	cudaDeviceReset();
	return (errors == 0) ? 0 : 1;
}

