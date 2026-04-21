TARGETS = devicequery max_blocks_per_sm mat_vec_gpu

GNUPLOT = gnuplot
CC = gcc
COPT = 
NVCC = nvcc
NVPROF = nvprof -m global_hit_rate,tex_cache_hit_rate,l2_tex_read_hit_rate,dram_read_throughput,dram_write_throughput
NVCOPT = -arch=sm_60 -Xptxas -dlcm=ca

POLICIES = 2 3
MATVEC_VEC_SIZE = 8192
MATVEC_BLOCK_SIZES = 1 2 4 8 16 32 64 128 256 512 1024
MATVEC_DATA := outputs/mat_vec_gpu.1.1.perf $(foreach policy,$(POLICIES),$(foreach size, $(MATVEC_BLOCK_SIZES), outputs/mat_vec_gpu.$(policy).$(size).perf))
MATVEC_PROFILE := $(foreach policy,$(POLICIES),$(foreach size, $(MATVEC_BLOCK_SIZES), outputs/mat_vec_gpu.$(policy).$(size).prof))
MATMUL_BLOCK_SIZES = 1 2 4 8 16 32 
MATMUL_DATA := $(foreach policy,$(POLICIES),$(foreach size, $(MATMUL_BLOCK_SIZES), outputs/mat_mul_gpu.$(policy).$(size).perf))
PLOTS := MatVecTime.pdf MatVecProfile.pdf MatMulTime.pdf MatMulTimeZoomIn.pdf

all: build plots
build: $(TARGETS)
#plots: $(MATVEC_DATA) $(MATMUL_DATA) $(MATVEC_PROFILE) $(PLOTS)
plots: $(MATVEC_DATA) $(MATVEC_PROFILE) $(PLOTS)

MatVecTime.pdf: MatVecTime.dat generate_mat_vec_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_mat_vec_plot.plt

MatVecTime.dat: $(MATVEC_DATA) generate_plot.py
	python generate_plot.py -i outputs -p mat_vec_gpu -o $@

MatVecProfile.pdf: MatVecProfile.dat generate_mat_vec_profile_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_mat_vec_profile_plot.plt

MatVecProfile.dat: $(MATVEC_PROFILE) generate_mat_vec_profile_plot.py
	python generate_mat_vec_profile_plot.py -i outputs -p mat_vec_gpu -o $@

MatMulTime.pdf: MatMulTime.dat generate_mat_mul_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_mat_mul_plot.plt

MatMulTimeZoomIn.pdf: MatMulTime.dat generate_mat_mul_zoomin_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_mat_mul_zoomin_plot.plt

MatMulTime.dat: $(MATMUL_DATA) generate_plot.py
	python generate_plot.py -i outputs -p mat_mul_gpu -o $@

%: %.c
	$(CC) $(COPT) $< -o $@

%: %.cu
	$(NVCC) $(NVCOPT) $< -o $@

# Measure CPU performance for given size
outputs/mat_vec_gpu.1.1.perf: mat_vec_gpu
	./mat_vec_gpu $(MATVEC_VEC_SIZE) 1 1 nodebug > $@

# Measure GPU performance for given size with different thread block sizes
define matvec_rules
outputs/mat_vec_gpu.$(1).$(2).perf: mat_vec_gpu
	./mat_vec_gpu $(MATVEC_VEC_SIZE) $(2) $(1) nodebug > $$@
outputs/mat_vec_gpu.$(1).$(2).prof: mat_vec_gpu
	$(NVPROF) ./mat_vec_gpu $(MATVEC_VEC_SIZE) $(2) $(1) nodebug > /dev/null 2> $$@
endef
$(foreach policy,2 3,$(foreach size, $(MATVEC_BLOCK_SIZES), $(eval $(call matvec_rules,$(policy),$(size)))))

outputs/mat_mul_gpu.1.perf: mat_mul_gpu
	./mat_mul_gpu 1024 1 1 nodebug > $@

define matmul_cpu_rules
outputs/mat_mul_gpu.1.$(1).perf: outputs/mat_mul_gpu.1.perf
	cp $$< $$@
endef

$(foreach size, $(MATMUL_BLOCK_SIZES), $(eval $(call matmul_cpu_rules,$(size))))

define matmul_rules
outputs/mat_mul_gpu.$(1).$(2).perf: mat_mul_gpu
	./mat_mul_gpu 1024 $(2) $(1) nodebug > $$@
endef

$(foreach policy,2 3,$(foreach size, $(MATMUL_BLOCK_SIZES), $(eval $(call matmul_rules,$(policy),$(size)))))

clean:
	rm -f $(TARGETS) *.pdf *.dat outputs/*.perf outputs/*.prof
