TARGETS = branch

CC = g++
COPT = 
LOPT = 
TYPES = sort nosort
SIZES = 100000 1000000
PERF_DATA := $(foreach type,$(TYPES),$(foreach size, $(SIZES), outputs/$(type).$(size).perf))
STAT_DATA := $(foreach type,$(TYPES),$(foreach size, $(SIZES), outputs/$(type).$(size).stat))

all: $(TARGETS) $(STAT_DATA)

build: $(TARGETS)

%.o: %.c
	$(CC) $(COPT) -c $<

branch: branch.o
	$(CC) $(LOPT) $^ -o $@

$(foreach target,$(TARGETS), $(eval $(call link_rules,$(target))))

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
	rm -f $(TARGETS) $(PERF_DATA) $(STAT_DATA) *.o
