#!/usr/bin/gnuplot
reset
set term pdf color enh dashed dl 3.5 size 8,6
set out "t2z0.pdf"
set size ratio 6.0/8.0

set key font "Helvetica,18"
set tics font "Helvetica,18"
set xlabel font "Helvetica,18"
set ylabel font "Helvetica,18"
set key reverse
set key spacing 4 samplen 6
set key width 10
set key left bottom at 3,0.05

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "#008000"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"


set xrange [-8:8]
set yrange [0:0.056]


set xlabel "p/T" offset 0,-0.5
set ylabel "f" offset -3

set label "t = 2.0 t_{rel}, z = 0"  font "Helvetica,18" at -7,0.04



plot "feqz0.dat" u 2:(0.053*$3) w l ls 4 ti "Thermal",\
"fpMax30np600p01E25zMax20nz400v4.1.1.dat" u 2:3 index 20 every :::200::200 w l ls 2 ti "Numerics"
#pause -1
reset
set term x11
