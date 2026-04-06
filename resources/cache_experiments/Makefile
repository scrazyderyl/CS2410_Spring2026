# Do 'make' on thompson.cs.pitt.edu

TARGETS = array.bin array_nodata.bin array_optimized.bin linked-list.bin linked-list_nodata.bin linked-list_optimized.bin
ASSEMBLIES = array.s array_nodata.s array_optimized.s linked-list.s linked-list_nodata.s linked-list_optimized.s

GNUPLOT = gnuplot
CC = g++
COPT = -O3 -fno-tree-vectorize
LOPT = -O3
SIZES = 100 200 400 600 800 1000 2000 4000 6000 8000 10000 20000 40000 60000 80000 100000 200000 400000 600000 800000 1000000
STAT_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).ipc))
TOPDOWN_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).topdown))
TOPDOWN_PLOTS := $(foreach target,$(TARGETS), plots/Topdown.$(target).pdf)
MEMSTALLS_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).memstalls))
MEMSTALLS_PLOTS := $(foreach target,$(TARGETS), plots/MemStalls.$(target).pdf)
CACHE_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).cache))
CACHE_PLOTS := $(foreach target,$(TARGETS), plots/CacheStatsIncludingKernel.$(target).pdf)
CACHE_USERSPACE_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).ucache))
CACHE_USERSPACE_PLOTS := $(foreach target,$(TARGETS), plots/CacheStats.$(target).pdf)

# Memstalls plot is inaccurate due to x86 PMU multiplexing so don't include it.
#PLOTS := plots/IPC.pdf $(TOPDOWN_PLOTS) $(MEMSTALLS_PLOTS) $(CACHE_USERSPACE_PLOTS)
PLOTS := plots/IPC.pdf $(TOPDOWN_PLOTS) $(CACHE_USERSPACE_PLOTS)

.PRECIOUS: $(STAT_DATA) $(TOPDOWN_DATA) $(MEMSTALLS_DATA) $(CACHE_USERSPACE_DATA)

all: $(TARGETS) $(ASSEMBLIES) $(PLOTS)

build: $(TARGETS)

%.o: %.c
	$(CC) $(COPT) -c $<

%.bin: %.o
	$(CC) $(LOPT) $^ -o $@

%.s: %.c
	$(CC) $(COPT) -S -g -fverbose-asm -Wa,-adhln $< | c++filt > $@

$(foreach target,$(TARGETS), $(eval $(call link_rules,$(target))))

plots/IPC.pdf: data/IPC.dat generate_ipc_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_ipc_plot.plt

data/IPC.dat: $(STAT_DATA) generate_ipc_plot.py
	python generate_ipc_plot.py -i outputs -o $@

define plot_rules
plots/Topdown.$(1).pdf: data/Topdown.$(1).dat generate_topdown_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_topdown_plot.plt
data/Topdown.$(1).dat: $(TOPDOWN_DATA) generate_topdown_plot.py
	python generate_topdown_plot.py -t $(1) -i outputs -o $$@
plots/MemStalls.$(1).pdf: data/MemStalls.$(1).dat generate_memstalls_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_memstalls_plot.plt
data/MemStalls.$(1).dat: $(MEMSTALLS_DATA) generate_memstalls_plot.py
	python generate_memstalls_plot.py -t $(1) -i outputs -o $$@
plots/CacheStatsIncludingKernel.$(1).pdf: data/CacheStatsIncludingKernel.$(1).dat generate_cachestats_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_cachestats_plot.plt
data/CacheStatsIncludingKernel.$(1).dat: $(CACHE_DATA) generate_cachestats_plot.py
	python generate_cachestats_plot.py -t $(1) -e cache -i outputs -o $$@
plots/CacheStats.$(1).pdf: data/CacheStats.$(1).dat generate_cachestats_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_cachestats_plot.plt
data/CacheStats.$(1).dat: $(CACHE_USERSPACE_DATA) generate_cachestats_plot.py
	python generate_cachestats_plot.py -t $(1) -e ucache -i outputs -o $$@
endef
$(foreach target,$(TARGETS), $(eval $(call plot_rules,$(target))))

define stat_rules
outputs/$(1).$(2).ipc: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e cycles:u,instructions:u ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).topdown: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 --topdown ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).memstalls: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e cycles:u,cycle_activity.stalls_mem_any:u,cycle_activity.stalls_l1d_miss:u,cycle_activity.stalls_l2_miss:u,cycle_activity.stalls_l3_miss:u -a ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).cache: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e mem_inst_retired.all_loads,mem_load_retired.l1_hit,mem_load_retired.l1_miss,mem_load_retired.l2_hit,mem_load_retired.l2_miss,mem_load_retired.l3_hit,mem_load_retired.l3_miss ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).ucache: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e mem_inst_retired.all_loads,mem_load_retired.l1_hit:u,mem_load_retired.l1_miss:u,mem_load_retired.l2_hit:u,mem_load_retired.l2_miss:u,mem_load_retired.l3_hit:u,mem_load_retired.l3_miss:u ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
endef
$(foreach target,$(TARGETS),$(foreach size, $(SIZES), $(eval $(call stat_rules,$(target),$(size)))))

clean:
	rm -f $(TARGETS) $(ASSEMBLIES) $(STAT_DATA) $(TOPDOWN_DATA) $(MEMSTALLS_DATA) $(CACHE_DATA) $(CACHE_USERSPACE_DATA) data/*.dat $(PLOTS) *.o perf_ctl
