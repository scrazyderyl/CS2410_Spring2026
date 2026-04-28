# Plot configurations

set style line 1 lt rgb "red" lw 20 ps 5
set style line 2 lt rgb "blue" lw 20 ps 5
set style line 3 lt rgb "green" lw 20 ps 5
set style line 4 lt rgb "orange" lw 20 ps 5
set style line 5 lt rgb "purple" lw 20 ps 5
set title "Utilization profile"
set xlabel "Thread block sizes" 
set ylabel "Utilization (%)"
set y2label "GFLOPS"
set ytics nomirror
set y2tics autofreq
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set logscale x
set key bottom center outside horizontal

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

set output outputFile
plot inputFile using 1:8:xticlabels(1) with linespoints linestyle 1 t "SM utilization", \
inputFile using 1:9:xticlabels(1) with linespoints linestyle 2 t "Warp utilization", \
inputFile using 1:($7*100):xticlabels(1) with linespoints linestyle 3 t "Warp occupancy", \
inputFile using 1:($10*100/2):xticlabels(1) with linespoints linestyle 4 t "Avg. eligible warps per scheduler", \
inputFile using 1:6:xticlabels(1) with linespoints linestyle 5 t "Throughput (GFLOPS)" axis x1y2
unset out

