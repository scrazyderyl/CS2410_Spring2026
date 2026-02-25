# Plot configurations

set style line 1 lt rgb "red" lw 20 ps 5
set style line 2 lt rgb "blue" lw 20 ps 5
set style line 3 lt rgb "green" lw 20 ps 5
set style line 4 lt rgb "purple" lw 20 ps 5
set title "CPU Cycles Breakdown" 
set xlabel "Array size"
set ylabel "Percentage of Cycles (%)"
set style data histograms
set style histogram rowstacked
set boxwidth 0.8 relative
set style fill solid 1.0 border -1
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set key outside top right
set xtics rotate by -45

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

if (!exists("inputFile")) inputFile='Topdown.dat'
if (!exists("outputFile")) outputFile='Topdown.pdf'
set output outputFile
#plot inputFile using 1:2 with linespoints linestyle 1 t "ipc"
plot inputFile using 3 t "Retiring", \
     '' using 2 t "Frontend Bound", \
     '' using 4 t "Backend Bound", \
     '' using 5:xticlabels(1) t "Bad Speculation"
unset out

