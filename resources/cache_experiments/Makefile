# Do 'make' on thompson.cs.pitt.edu

TARGETS = array.bin array_nodata.bin linked-list.bin linked-list_nodata.bin
ASSEMBLIES = array.s array_nodata.s linked-list.s linked-list_nodata.s

GNUPLOT = gnuplot
CC = g++
COPT = -O3
LOPT = -O3
SIZES = 100 200 300 400 500 600 700 800 900 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000 50000 100000 500000 1000000 5000000 10000000
STAT_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).stat))
TOPDOWN_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).topdown))
TOPDOWN_PLOTS := $(foreach target,$(TARGETS), plots/Topdown.$(target).pdf)
MEMSTALLS_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).memstalls))
MEMSTALLS_PLOTS := $(foreach target,$(TARGETS), plots/MemStalls.$(target).pdf)
CACHE_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).cache))
CACHE_PLOTS := $(foreach target,$(TARGETS), plots/CacheStats.$(target).pdf)
CACHE_USERSPACE_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).ucache))
CACHE_USERSPACE_PLOTS := $(foreach target,$(TARGETS), plots/CacheUserSpaceStats.$(target).pdf)
#PLOTS := plots/IPC.pdf $(TOPDOWN_PLOTS) $(MEMSTALLS_PLOTS) $(CACHE_PLOTS)
PLOTS := plots/IPC.pdf $(MEMSTALLS_PLOTS) $(CACHE_PLOTS) $(CACHE_USERSPACE_PLOTS)

.PRECIOUS: $(STAT_DATA) $(TOPDOWN_DATA) $(MEMSTALLS_DATA) $(CACHE_DATA) $(CACHE_USERSPACE_DATA)

all: $(TARGETS) $(ASSEMBLIES) $(PLOTS)

build: $(TARGETS)

%.o: %.c
	$(CC) $(COPT) -c $<

%.bin: %.o
	$(CC) $(LOPT) $^ -o $@

%.s: %.c
	$(CC) $(COPT) -S -g -fverbose-asm -Wa,-adhln $< | c++filt > $@

$(foreach target,$(TARGETS), $(eval $(call link_rules,$(target))))

plots/IPC.pdf: plots/IPC.dat generate_ipc_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_ipc_plot.plt

plots/IPC.dat: $(STAT_DATA) generate_ipc_plot.py
	python generate_ipc_plot.py -i outputs -o $@

define plot_rules
plots/Topdown.$(1).pdf: plots/Topdown.$(1).dat generate_topdown_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_topdown_plot.plt
plots/Topdown.$(1).dat: $(TOPDOWN_DATA) generate_topdown_plot.py
	python generate_topdown_plot.py -t $(1) -i outputs -o $$@
plots/MemStalls.$(1).pdf: plots/MemStalls.$(1).dat generate_memstalls_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_memstalls_plot.plt
plots/MemStalls.$(1).dat: $(MEMSTALLS_DATA) generate_memstalls_plot.py
	python generate_memstalls_plot.py -t $(1) -i outputs -o $$@
plots/CacheStats.$(1).pdf: plots/CacheStats.$(1).dat generate_cachestats_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_cachestats_plot.plt
plots/CacheStats.$(1).dat: $(CACHE_DATA) generate_cachestats_plot.py
	python generate_cachestats_plot.py -t $(1) -e cache -i outputs -o $$@
plots/CacheUserSpaceStats.$(1).pdf: plots/CacheUserSpaceStats.$(1).dat generate_cachestats_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_cachestats_plot.plt
plots/CacheUserSpaceStats.$(1).dat: $(CACHE_USERSPACE_DATA) generate_cachestats_plot.py
	python generate_cachestats_plot.py -t $(1) -e ucache -i outputs -o $$@
endef
$(foreach target,$(TARGETS), $(eval $(call plot_rules,$(target))))

define stat_rules
outputs/$(1).$(2).stat: $(TARGETS)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e cycles,instructions ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).topdown: $(TARGETS)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 --topdown ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).memstalls: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -M tma_memory_bound,tma_l1_bound,tma_l2_bound,tma_l3_bound,tma_dram_bound,tma_store_bound -a ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).cache: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e mem_load_retired.l1_hit,mem_load_retired.l1_miss,mem_load_retired.l2_hit,mem_load_retired.l2_miss,mem_load_retired.l3_hit,mem_load_retired.l3_miss ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).ucache: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e mem_load_retired.l1_hit:u,mem_load_retired.l1_miss:u,mem_load_retired.l2_hit:u,mem_load_retired.l2_miss:u,mem_load_retired.l3_hit:u,mem_load_retired.l3_miss:u ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
endef
$(foreach target,$(TARGETS),$(foreach size, $(SIZES), $(eval $(call stat_rules,$(target),$(size)))))

clean:
	rm -f $(TARGETS) $(ASSEMBLIES) $(STAT_DATA) $(TOPDOWN_DATA) $(MEMSTALLS_DATA) $(CACHE_DATA) $(CACHE_USERSPACE_DATA) plots/*.dat $(PLOTS) *.o perf_ctl
