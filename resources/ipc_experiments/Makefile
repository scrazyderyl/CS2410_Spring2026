# Do 'make' on thompson.cs.pitt.edu

TARGETS = ipc ipc.s

GNUPLOT = gnuplot
CC = g++
COPT = -O0
LOPT = -O0
TYPES = ipc
SIZES = 1 2 3 4 5 6 7 8 9
STAT_DATA := $(foreach type,$(TYPES),$(foreach size, $(SIZES), outputs/$(type).$(size).stat))
PLOTS := IPC.pdf

all: $(TARGETS) $(STAT_DATA) $(PLOTS)

build: $(TARGETS)

%.o: %.c
	$(CC) $(COPT) -c $<

ipc: ipc.o
	$(CC) $(LOPT) $^ -o $@

ipc.s: ipc.c
	$(CC) -S -g -fverbose-asm -Wa,-adhln $< | c++filt > $@

$(foreach target,$(TARGETS), $(eval $(call link_rules,$(target))))

IPC.pdf: IPC.dat generate_ipc_plot.plt
	$(GNUPLOT) -e "inputFile='$<'" -e "outputFile='$@'" generate_ipc_plot.plt

IPC.dat: $(STAT_DATA) generate_ipc_plot.py
	python generate_ipc_plot.py -i outputs -o $@

define stat_rules
outputs/$(1).$(2).stat: $(TARGETS)
	mkfifo perf_ctl
	perf stat -D -1 --control fd:3 -e cycles,instructions ./ipc $(2) 3<>perf_ctl > $$@ 2>&1
	rm -f perf_ctl
endef
$(foreach type,$(TYPES),$(foreach size, $(SIZES), $(eval $(call stat_rules,$(type),$(size)))))

clean:
	rm -f $(TARGETS) $(STAT_DATA) *.dat $(PLOTS) *.o
