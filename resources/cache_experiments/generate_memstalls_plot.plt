# Plot configurations

set style line 1 lt rgb "red" lw 20 ps 5
set style line 2 lt rgb "blue" lw 20 ps 5
set style line 3 lt rgb "green" lw 20 ps 5
set style line 4 lt rgb "purple" lw 20 ps 5
set title "CPU Cycles Breakdown" 
set xlabel "Size"
set ylabel "Percentage of Cycles (%)"
set style data histograms
set style histogram rowstacked
set boxwidth 0.8 relative
set style fill solid 1.0 border -1
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ 0 : 100 ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set key bottom center outside horizontal
set rmargin 5
set xtics rotate by -45

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

if (!exists("inputFile")) inputFile='MemStalls.dat'
if (!exists("outputFile")) outputFile='MemStalls.pdf'
set output outputFile
plot inputFile using 2 t "L1 Bound", \
     '' using 3 t "L2 Bound", \
     '' using 4 t "L3 Bound", \
     '' using 5:xticlabels(1) t "DRAM Bound"
unset out

