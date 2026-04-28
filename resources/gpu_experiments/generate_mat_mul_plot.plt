# Plot configurations

set border 3 front lt black linewidth 1.000 dashtype solid
set boxwidth 0.8 absolute
set style fill   solid 1.00 noborder
set grid nopolar
set grid noxtics nomxtics ytics nomytics noztics nomztics nortics nomrtics \
 nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics
set grid layerdefault   lt 0 linecolor 0 linewidth 0.500,  lt 0 linecolor 0 linewidth 0.500
set style histogram rowstacked title textcolor lt -1 offset character 2, 0.25
set datafile missing '-'
set style data histograms
set xtics border in scale 0,0 nomirror rotate by -45  autojustify
set xtics  norangelimit  font ",8"
set xtics   ()
set ytics border in scale 0,0 mirror norotate  autojustify
set ytics  norangelimit autofreq font ",8"
set ztics border in scale 0,0 nomirror norotate  autojustify
set cbtics border in scale 0,0 mirror norotate  autojustify
set rtics axis in scale 0,0 nomirror norotate  autojustify
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback

# Render plot
set terminal pdf transparent enhanced font "arial,10" fontscale 5.0 size 45, 30

if (!exists("inputFile")) filename='TotalTime.dat'
if (!exists("outputFile")) filename='TotalTime.pdf'
set output outputFile

bm = 0.15
lm = 0.12
rm = 0.95
gap = 0.03
size = 0.75
kk = 0.7 # relative height of bottom plot

set multiplot
set border 1+2+8
set xtics nomirror
set ytics nomirror
set lmargin at screen lm
set rmargin at screen rm
set bmargin at screen bm
set tmargin at screen bm + size * kk

set xlabel "Thread block sizes" 
set ylabel "Total Time (seconds)" 
set key top left invert noreverse

set yrange [0:2]
plot newhistogram "CPU", inputFile u "Compute.1":xtic(1) notitle lc rgb "blue", '' u "Memcpy.1" notitle lc rgb "orange", \
newhistogram "GPU", '' u "Compute.2":xtic(1) notitle lc rgb "blue", '' u "Memcpy.2" notitle lc rgb "orange", \
newhistogram "GPU w/ shared memory", '' u "Compute.3":xtic(1) notitle lc rgb "blue", '' u "Memcpy.3" notitle lc rgb "orange"

unset xtics
unset xlabel
unset ylabel
set ytics norangelimit autofreq font ",8"
set border 2+4+8
set bmargin at screen bm + size * kk + gap
set tmargin at screen bm + size + gap

set title "Execution time of mat-mat multiply on CPU and GPU"

set yrange [10:15]
plot newhistogram '', inputFile u "Compute.1":xtic(1) notitle lc rgb "blue", '' u "Memcpy.1" notitle lc rgb "orange", \
newhistogram '', '' u "Compute.2":xtic(1) notitle lc rgb "blue", '' u "Memcpy.2" notitle lc rgb "orange", \
newhistogram '', '' u "Compute.3":xtic(1) notitle lc rgb "blue", '' u "Memcpy.3" notitle lc rgb "orange"

unset multiplot
unset out

