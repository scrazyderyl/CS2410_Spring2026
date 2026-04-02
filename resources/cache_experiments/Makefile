# Do 'make' on thompson.cs.pitt.edu

TARGETS = array.bin array_nodata.bin linked-list.bin linked-list_nodata.bin

GNUPLOT = gnuplot
CC = g++
COPT = -O0
LOPT = -O0
SIZES = 1 5 10 50 100 500 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000
STAT_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).stat))
TOPDOWN_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).topdown))
TOPDOWN_PLOTS := $(foreach target,$(TARGETS), Topdown.$(target).pdf)
CACHE_DATA := $(foreach target,$(TARGETS),$(foreach size, $(SIZES), outputs/$(target).$(size).cache))
CACHE_PLOTS := $(foreach target,$(TARGETS), $(target).cache.pdf)
PLOTS := IPC.pdf $(TOPDOWN_PLOTS) $(CACHE_PLOTS)

.PRECIOUS: $(foreach target,$(TARGETS), $(target).cache.dat) $(STAT_DATA) $(TOPDOWN_DATA) $(CACHE_DATA)

all: $(TARGETS) $(PLOTS)

build: $(TARGETS)

%.o: %.c
	$(CC) $(COPT) -c $<

%.bin: %.o
	$(CC) $(LOPT) $^ -o $@

%.s: %.c
	$(CC) -S -g -fverbose-asm -Wa,-adhln $< | c++filt > $@

$(foreach target,$(TARGETS), $(eval $(call link_rules,$(target))))

IPC.pdf: IPC.dat generate_ipc_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_ipc_plot.plt

IPC.dat: $(STAT_DATA) generate_ipc_plot.py
	python generate_ipc_plot.py -i outputs -o $@

%.cache.pdf: %.cache.dat generate_cachestats_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_cachestats_plot.plt

%.cache.dat: $(CACHE_DATA) generate_cachestats_plot.py
	python generate_cachestats_plot.py -i outputs -o $@ -t $*

define plot_rules
Topdown.$(1).pdf: Topdown.$(1).dat generate_topdown_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_topdown_plot.plt
Topdown.$(1).dat: $(STAT_DATA) generate_topdown_plot.py
	python generate_topdown_plot.py -t $(1) -i outputs -o $$@
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
outputs/$(1).$(2).cache: $(1)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e L1-dcache-loads,L1-dcache-load-misses,l2_rqsts.demand_data_rd_hit,l2_rqsts.demand_data_rd_miss,LLC-loads,LLC-load-misses ./$(1) $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
endef
$(foreach target,$(TARGETS),$(foreach size, $(SIZES), $(eval $(call stat_rules,$(target),$(size)))))

clean:
	rm -f $(TARGETS) $(STAT_DATA) $(TOPDOWN_DATA) $(CACHE_DATA) *.dat $(PLOTS) *.o perf_ctl
