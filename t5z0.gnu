#!/usr/bin/gnuplot
reset
set term post portrait color enh dl 3.5 size 8,6
set out "t5z0.eps"
set size ratio 1.0

set tics font "Helvetica,18"
set xlabel font "Helvetica,18"
set ylabel font "Helvetica,18"

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "dark-green"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"


set xrange [-8:8]
#set yrange [0:0.019]


set xlabel "p/T"
set ylabel "f"
#set zlabel "f"

set palette defined (0 "#008080ff", 0.2 "yellow", 1 "red")

set ztics 0,0.05,0.2

set title "t = 2.0 t_{rel}, z = 0"



plot "feqz0.dat" u 2:(0.116*$3) w l ls 2 ti "Thermal",\
"fpMax30np600p01E25zMax20nz400v4.1.1.dat" u 2:3 index 50 every :::200::200 w l ls 4 ti "Numerics"
#pause -1
reset
set term x11
