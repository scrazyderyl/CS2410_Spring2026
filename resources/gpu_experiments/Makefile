TARGETS = devicequery mat_vec_gpu mat_mul_gpu

GNUPLOT = gnuplot
CC = gcc
COPT = 
NVCC = nvcc
NVPROF = nvprof -m flop_count_sp,achieved_occupancy,eligible_warps_per_cycle,sm_efficiency,warp_execution_efficiency,global_hit_rate,tex_cache_hit_rate,l2_tex_read_hit_rate,dram_read_throughput,dram_write_throughput
NVCOPT = -arch=sm_60 -Xptxas -dlcm=ca

MATVEC_DIMENSION_SIZE = 8192
MATVEC_BLOCK_SIZES = 1 2 4 8 16 32 64 128 256 512 1024
MATVEC_DATA := outputs/mat_vec_gpu.1.1.perf $(foreach policy,2 3,$(foreach size, $(MATVEC_BLOCK_SIZES), outputs/mat_vec_gpu.$(policy).$(size).perf))
MATVEC_PROFILE := $(foreach policy,2 3,$(foreach size, $(MATVEC_BLOCK_SIZES), outputs/mat_vec_gpu.$(policy).$(size).prof))
MATVEC_PLOTS := plots/MatVecTime.pdf plots/MatVecMemoryProfile.2.pdf plots/MatVecMemoryProfile.3.pdf plots/MatVecUtilizationProfile.2.pdf plots/MatVecUtilizationProfile.3.pdf

MATMUL_DIMENSION_SIZE = 1024
MATMUL_BLOCK_SIZES = 1 2 4 8 16 32 
MATMUL_DATA := outputs/mat_mul_gpu.1.1.perf $(foreach policy,2 3,$(foreach size, $(MATMUL_BLOCK_SIZES), outputs/mat_mul_gpu.$(policy).$(size).perf))
MATMUL_PROFILE := $(foreach policy,2 3,$(foreach size, $(MATMUL_BLOCK_SIZES), outputs/mat_mul_gpu.$(policy).$(size).prof))
MATMUL_PLOTS := plots/MatMulTime.pdf plots/MatMulMemoryProfile.2.pdf plots/MatMulMemoryProfile.3.pdf plots/MatMulUtilizationProfile.2.pdf plots/MatMulUtilizationProfile.3.pdf

MATMUL_OUTPUTS := $(foreach policy,1 2 3,$(foreach size, $(MATMUL_BLOCK_SIZES), outputs/mat_mul_gpu.$(policy).$(size).out))
MATMUL_DIFFS := $(foreach policy,2 3,$(foreach size, $(MATMUL_BLOCK_SIZES), diffs/mat_mul_gpu.$(policy).$(size).diff))

PLOTS := $(MATVEC_PLOTS) $(MATMUL_PLOTS)

.PRECIOUS: data/MatVecProfile.%.dat data/MatMulProfile.%.dat

all: build diffs
build: $(TARGETS)
diffs: $(MATMUL_DIFFS)
plots: $(MATVEC_DATA) $(MATVEC_PROFILE) $(MATMUL_DATA) $(MATMUL_PROFILE) $(PLOTS)

plots/MatVecTime.pdf: data/MatVecTime.dat generate_mat_vec_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_mat_vec_plot.plt

data/MatVecTime.dat: $(MATVEC_DATA) generate_timing_data.py
	python generate_timing_data.py -i outputs -p mat_vec_gpu -o $@

plots/MatVecMemoryProfile.%.pdf: data/MatVecProfile.%.dat generate_memory_profile_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_memory_profile_plot.plt

plots/MatVecUtilizationProfile.%.pdf: data/MatVecProfile.%.dat generate_utilization_profile_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_utilization_profile_plot.plt

data/MatVecProfile.%.dat: $(MATVEC_PROFILE) $(MATVEC_DATA) generate_profile_data.py
	python generate_profile_data.py -i outputs -t mat_vec_gpu -p $* -o $@

plots/MatMulTime.pdf: data/MatMulTime.dat generate_mat_mul_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_mat_mul_plot.plt

data/MatMulTime.dat: $(MATMUL_DATA) generate_timing_data.py
	python generate_timing_data.py -i outputs -p mat_mul_gpu -o $@

plots/MatMulMemoryProfile.%.pdf: data/MatMulProfile.%.dat generate_memory_profile_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_memory_profile_plot.plt

plots/MatMulUtilizationProfile.%.pdf: data/MatMulProfile.%.dat generate_utilization_profile_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_utilization_profile_plot.plt

data/MatMulProfile.%.dat: $(MATMUL_PROFILE) $(MATMUL_DATA) generate_profile_data.py
	python generate_profile_data.py -i outputs -t mat_mul_gpu -p $* -o $@

%: %.c
	$(CC) $(COPT) $< -o $@

%: %.cu
	$(NVCC) $(NVCOPT) $< -o $@

# Measure CPU performance for given size
outputs/mat_vec_gpu.1.1.perf: mat_vec_gpu
	./mat_vec_gpu $(MATVEC_DIMENSION_SIZE) 1 1 nodebug > $@

# Measure GPU performance for given size with different thread block sizes
define matvec_rules
outputs/mat_vec_gpu.$(1).$(2).perf: mat_vec_gpu
	./mat_vec_gpu $(MATVEC_DIMENSION_SIZE) $(2) $(1) nodebug > $$@
outputs/mat_vec_gpu.$(1).$(2).prof: mat_vec_gpu
	$(NVPROF) ./mat_vec_gpu $(MATVEC_DIMENSION_SIZE) $(2) $(1) nodebug > /dev/null 2> $$@
endef
$(foreach policy,2 3,$(foreach size, $(MATVEC_BLOCK_SIZES), $(eval $(call matvec_rules,$(policy),$(size)))))

# Measure CPU performance for given size
outputs/mat_mul_gpu.1.1.out: mat_mul_gpu
	./mat_mul_gpu $(MATMUL_DIMENSION_SIZE) 1 1 debug > $@
outputs/mat_mul_gpu.1.1.perf: mat_mul_gpu
	./mat_mul_gpu $(MATMUL_DIMENSION_SIZE) 1 1 nodebug > $@

# Measure GPU performance for given size with different thread block sizes
define matmul_rules
outputs/mat_mul_gpu.$(1).$(2).out: mat_mul_gpu
	./mat_mul_gpu $(MATMUL_DIMENSION_SIZE) $(2) $(1) debug > $$@
diffs/mat_mul_gpu.$(1).$(2).diff: outputs/mat_mul_gpu.1.1.out outputs/mat_mul_gpu.$(1).$(2).out mat_mul_gpu
	-python diff.py outputs/mat_mul_gpu.1.1.out outputs/mat_mul_gpu.$(1).$(2).out > $$@
outputs/mat_mul_gpu.$(1).$(2).perf: mat_mul_gpu
	./mat_mul_gpu $(MATMUL_DIMENSION_SIZE) $(2) $(1) nodebug > $$@
outputs/mat_mul_gpu.$(1).$(2).prof: mat_mul_gpu
	$(NVPROF) ./mat_mul_gpu $(MATMUL_DIMENSION_SIZE) $(2) $(1) nodebug > /dev/null 2> $$@
endef
$(foreach policy,2 3,$(foreach size, $(MATMUL_BLOCK_SIZES), $(eval $(call matmul_rules,$(policy),$(size)))))

clean:
	rm -f $(TARGETS) $(PLOTS) $(MATVEC_DATA) $(MATVEC_PROFILE) $(MATMUL_DATA) $(MATMUL_PROFILE) $(PLOTS) $(MATMUL_OUTPUTS) $(MATMUL_DIFFS) data/*.dat