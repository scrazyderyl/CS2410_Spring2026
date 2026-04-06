# Plot configurations

set style line 1 lt rgb "red" lw 20 ps 5 dt 1 pt 2
set style line 2 lt rgb "red" lw 20 ps 5 dt 2 pt 4
set style line 3 lt rgb "red" lw 20 ps 5 dt 3 pt 6
set style line 4 lt rgb "blue" lw 20 ps 5 dt 1 pt 2
set style line 5 lt rgb "blue" lw 20 ps 5 dt 2 pt 4
set style line 6 lt rgb "blue" lw 20 ps 5 dt 3 pt 6
set title "IPC (Instructions Per Cycle)" 
set xlabel "Size"
set ylabel "IPC"
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set key bottom center outside horizontal
set rmargin 5
set xtics rotate by -45
set logscale x
set logscale y

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

if (!exists("inputFile")) inputFile='IPC.dat'
if (!exists("outputFile")) outputFile='IPC.pdf'
set output outputFile
plot inputFile using 1:2 with linespoints linestyle 1 t "array.bin", \
     inputFile using 1:3 with linespoints linestyle 2 t "array\\_nodata.bin", \
     inputFile using 1:4 with linespoints linestyle 3 t "array\\_optimized.bin", \
     inputFile using 1:5 with linespoints linestyle 4 t "linked-list.bin", \
     inputFile using 1:6 with linespoints linestyle 5 t "linked-list\\_nodata.bin", \
     inputFile using 1:7 with linespoints linestyle 6 t "linked-list\\_optimized.bin"
unset out

