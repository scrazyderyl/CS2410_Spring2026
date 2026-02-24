# Do 'make' on thompson.cs.pitt.edu

TARGETS = branch branch.s

GNUPLOT = gnuplot
CC = g++
COPT = -O0
LOPT = -O0
TYPES = sort nosort
SIZES = 10 100 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000 11000 12000 13000 14000 15000 16000 17000 18000 19000 20000 30000 40000 50000 60000 70000 80000 90000 100000
STAT_DATA := $(foreach type,$(TYPES),$(foreach size, $(SIZES), outputs/$(type).$(size).stat))
PLOTS := BranchMispredictionRate.pdf IPC.pdf

all: $(TARGETS) $(STAT_DATA) $(PLOTS)

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

define perf_rules
outputs/$(1).$(2).perf: $(TARGETS)
	perf record -F 997 -m ,256M \
		-e intel_pt//u --filter 'filter run @ ./branch' \
		-e cycles,instructions,branches,branch-misses \
		./branch $(2) $(1)
	perf report -i perf.data > $$@
	rm -f perf.data
endef
$(foreach type,$(TYPES),$(foreach size, $(SIZES), $(eval $(call perf_rules,$(type),$(size)))))

define stat_rules
outputs/$(1).$(2).stat: $(TARGETS)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e cycles,instructions,branches,branch-misses ./branch $(2) $(1) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
endef
$(foreach type,$(TYPES),$(foreach size, $(SIZES), $(eval $(call stat_rules,$(type),$(size)))))

clean:
	rm -f $(TARGETS) $(STAT_DATA) *.dat $(PLOTS) *.o
