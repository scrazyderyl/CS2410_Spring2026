#include <stdio.h>

int getSPcores(cudaDeviceProp prop) {
    int cores = 0;
    int mp = prop.multiProcessorCount;
    switch (prop.major) {
        case 6: // Pascal
            if (prop.minor == 0) cores = mp * 64;       // P100
            else if (prop.minor == 1) cores = mp * 128; // GP10x consumer
            else if (prop.minor == 2) cores = mp * 128;
            break;
        case 7: // Volta and Turing
            if (prop.minor == 0 || prop.minor == 2) cores = mp * 64;  // V100
            else if (prop.minor == 5) cores = mp * 64;                // Turing
            break;
        case 8: // Ampere
            if (prop.minor == 0) cores = mp * 64;       // A100
            else if (prop.minor == 6) cores = mp * 128; // GA10x consumer
            else if (prop.minor == 9) cores = mp * 128; // Ada
            break;
        case 9: // Hopper
            cores = mp * 128;  // H100
            break;
        default:
            cores = -1;
    }
    return cores;
}

// Print device properties
void printDevProp(cudaDeviceProp prop)
{
    printf("Major revision number:           %d\n",  prop.major);
    printf("Minor revision number:           %d\n",  prop.minor);
    printf("Name:                            %s\n",  prop.name);
    printf("Peak throughput (GFLOPS):        %.2f\n", 2.0 * getSPcores(prop) * prop.clockRate / 1.0e6);
    printf("Global memory bandwidth (GB/s):  %.2f\n", 2.0 * prop.memoryClockRate * (prop.memoryBusWidth / 8) / 1.0e6);
    printf("Warp size:                       %d\n",  prop.warpSize);
    printf("Total global memory:             %zu\n",  prop.totalGlobalMem);
    printf("Maximum shared memory per block: %zu\n",  prop.sharedMemPerBlock);
    printf("Maximum registers per block:     %d\n",  prop.regsPerBlock);
    printf("Maximum threads per block:       %d\n",  prop.maxThreadsPerBlock);
    for (int i = 0; i < 3; ++i)
    printf("Maximum dimension %d of block     %d\n", i, prop.maxThreadsDim[i]);
    for (int i = 0; i < 3; ++i)
    printf("Maximum dimension %d of grid:     %d\n", i, prop.maxGridSize[i]);
    printf("Maximum shared memory per SM:    %zu\n",  prop.sharedMemPerMultiprocessor );
    printf("Maximum registers per SM:        %d\n", prop.regsPerMultiprocessor);
    printf("Maximum threads per SM:          %d\n", prop.maxThreadsPerMultiProcessor);
    printf("Maximum thread blocks per SM:    %d\n", prop.maxBlocksPerMultiProcessor);
    printf("Number of SMs:                   %d\n",  prop.multiProcessorCount);
    printf("Clock rate:                      %d\n",  prop.clockRate);
    return;
}
 
int main()
{
    // Number of CUDA devices
    int devCount;
    cudaGetDeviceCount(&devCount);
    printf("CUDA Device Query...\n");
    printf("There are %d CUDA devices.\n", devCount);
 
    // Iterate through devices
    for (int i = 0; i < devCount; ++i)
    {
        // Get device properties
        printf("\nCUDA Device #%d\n", i);
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);
        printDevProp(devProp);
    }
 
/*
    printf("\nPress any key to exit...");
    char c;
    scanf("%c", &c);
*/
 
    return 0;
}
