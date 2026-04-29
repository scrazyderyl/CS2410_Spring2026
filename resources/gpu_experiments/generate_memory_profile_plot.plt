# Plot configurations

set title "Memory profile"
set xlabel "Thread block sizes"
set ylabel "Percentage of Bytes Loaded (%)"
set y2label "DRAM Transfer Rate (GB/s)"

set ytics nomirror
set y2tics 0, 20

set yrange [0:100]
set y2range [*:*]

# Stacked-bar (histogram) styling
set style data histograms
set style histogram rowstacked
set style fill solid 1.0 border -1
set boxwidth 0.7

# Colors for the four memory levels
set style line 1 lt rgb "#4daf4a"   # shared memory  - green
set style line 2 lt rgb "#377eb8"   # L1            - blue
set style line 3 lt rgb "#ff7f00"   # L2            - orange
set style line 4 lt rgb "#e41a1c"   # DRAM          - red
set style line 5 lt rgb "purple" lw 30 ps 7

set key bottom center outside horizontal

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30
set output outputFile

# Columns: 1=BlockSizes, 4=dram_read_throughput, 13=pct_shared,
#          14=pct_l1, 15=pct_l2, 16=pct_dram
plot inputFile using 13:xtic(1) title "Shared Memory" linestyle 1 axis x1y1, \
     ''        using 14         title "L1 Hit"        linestyle 2 axis x1y1, \
     ''        using 15         title "L2 Hit"        linestyle 3 axis x1y1, \
     ''        using 16         title "DRAM"          linestyle 4 axis x1y1, \
     ''        using (column(0)):4 title "DRAM Transfer Rate (GB/s)" \
                  with linespoints linestyle 5 axis x1y2

unset out
