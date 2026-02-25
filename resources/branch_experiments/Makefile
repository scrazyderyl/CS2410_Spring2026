# Do 'make' on thompson.cs.pitt.edu

TARGETS = branch branch.s

GNUPLOT = gnuplot
CC = g++
COPT = -O0
LOPT = -O0
TYPES = sort nosort
SIZES = 10 100 200 300 400 500 600 700 800 900 1000 10000 20000 30000 40000 50000 60000 70000 80000 90000 100000
STAT_DATA := $(foreach type,$(TYPES),$(foreach size, $(SIZES), outputs/$(type).$(size).stat))
TOPDOWN_DATA := $(foreach type,$(TYPES),$(foreach size, $(SIZES), outputs/$(type).$(size).topdown))
PLOTS := BranchMispredictionRate.pdf IPC.pdf Topdown.sort.pdf Topdown.nosort.pdf

all: $(TARGETS) $(STAT_DATA) $(TOPDOWN_DATA) $(PLOTS)

build: $(TARGETS)

%.o: %.c
	$(CC) $(COPT) -c $<

branch: branch.o
	$(CC) $(LOPT) $^ -o $@

branch.s: branch.c
	$(CC) -S -g -fverbose-asm -Wa,-adhln $< | c++filt > $@

$(foreach target,$(TARGETS), $(eval $(call link_rules,$(target))))

BranchMispredictionRate.pdf: BranchMispredictionRate.dat generate_bp_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_bp_plot.plt

BranchMispredictionRate.dat: $(STAT_DATA) generate_bp_plot.py
	python generate_bp_plot.py -i outputs -o $@

IPC.pdf: IPC.dat generate_ipc_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_ipc_plot.plt

IPC.dat: $(STAT_DATA) generate_ipc_plot.py
	python generate_ipc_plot.py -i outputs -o $@

define topdown_rules
Topdown.$(1).pdf: Topdown.$(1).dat generate_topdown_plot.plt
	$(GNUPLOT) -e "inputFile='$$<'" -e "outputFile='$$@'" generate_topdown_plot.plt

Topdown.$(1).dat: $(STAT_DATA) generate_topdown_plot.py
	python generate_topdown_plot.py -t $(1) -i outputs -o $$@
endef
$(foreach type,$(TYPES), $(eval $(call topdown_rules,$(type))))

define stat_rules
outputs/$(1).$(2).stat: $(TARGETS)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e cycles,instructions,branches,branch-misses ./branch $(2) $(1) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
outputs/$(1).$(2).topdown: $(TARGETS)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 --topdown ./branch $(2) $(1) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
endef
$(foreach type,$(TYPES),$(foreach size, $(SIZES), $(eval $(call stat_rules,$(type),$(size)))))

clean:
	rm -f $(TARGETS) $(STAT_DATA) $(TOPDOWN_DATA) *.dat $(PLOTS) *.o perf_ctl
