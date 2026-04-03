# Plot configurations

set style line 1 lt rgb "red" lw 20 ps 5
set style line 2 lt rgb "blue" lw 20 ps 5
set style line 3 lt rgb "green" lw 20 ps 5
set style line 4 lt rgb "purple" lw 20 ps 5
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

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

if (!exists("inputFile")) inputFile='IPC.dat'
if (!exists("outputFile")) outputFile='IPC.pdf'
set output outputFile
plot inputFile using 1:2 with linespoints linestyle 1 t "array.bin", \
     inputFile using 1:3 with linespoints linestyle 2 t "array\\_nodata.bin", \
     inputFile using 1:4 with linespoints linestyle 3 t "linked-list.bin", \
     inputFile using 1:5 with linespoints linestyle 4 t "linked-list\\_nodata.bin"
unset out

