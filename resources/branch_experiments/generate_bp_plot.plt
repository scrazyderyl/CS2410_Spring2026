# Plot configurations

set style line 1 lt rgb "red" lw 20 ps 5
set style line 2 lt rgb "blue" lw 20 ps 5
set style line 3 lt rgb "green" lw 20 ps 5
set style line 4 lt rgb "purple" lw 20 ps 5
set title "Branch Misprediction Rate(%)" 
set xlabel "Data structure size"
set ylabel "Misprediction (%)"
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
# set logscale x
set key at graph 1.0, 0.85

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

if (!exists("inputFile")) inputFile='BranchMisprediction.dat'
if (!exists("outputFile")) outputFile='BranchMisprediction.pdf'
set output outputFile
plot inputFile using 1:2 with linespoints linestyle 1 t "nosort", inputFile using 1:3 with linespoints linestyle 2 t "sort"
#plot inputFile using 1:2 with linespoints linestyle 1 t "nosort", \
#     ""        using 1:2:(sprintf("%.2f", $2)) with labels offset char 0,1 notitle, \
#     ""        using 1:3 with linespoints linestyle 2 t "sort", \
#     ""        using 1:3:(sprintf("%.2f", $3)) with labels offset char 0,1 notitle
unset out

