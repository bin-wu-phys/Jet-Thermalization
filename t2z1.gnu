#!/usr/bin/gnuplot
reset
set term pdf color enh dashed dl 3.5 size 8,8
set out "t2z1.pdf"
set size ratio 1.0

set key font "Helvetica,18"
set tics font "Helvetica,28"
set xlabel font "Helvetica,28"
set ylabel font "Helvetica,28"
set key reverse
set key spacing 4 samplen 6

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "#008000"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"


set xrange [-8:8]
#set yrange [0:0.019]


set xlabel "p/T" offset 0,-0.5
set ylabel "f" offset -3


set title "t = 2.0 t_{rel}, z = 1.0 t_{rel}"   font "Helvetica,28"


plot "feqz0.dat" u 2:(0.115*$3) w l ls 4 ti "Thermal",\
"fpMax30np600p01E25zMax20nz400v4.1.1.dat" u 2:3 index 20 every :::210::210 w l ls 2 ti "Numerics"
#pause -1
reset
set term x11
