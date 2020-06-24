#!/usr/bin/gnuplot
reset
set term post portrait color enh dl 3.5 size 8,6
set out "eDen.eps"
set size ratio 6.0/8.0

set tics font "Helvetica,18"
set xlabel font "Helvetica,18"
set ylabel font "Helvetica,18"

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "#ff00ff"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"

set key reverse
set key bottom left at 4,0.5
set key spacing 4 samplen 6

set xrange [-3:10]
#set yrange [0:0.019]

set xlabel "z/t_{rel}"
set ylabel "energy desity"


plot "fpMax30np600p01E25zMax20nz400v4.1.1den.dat" u 1:3 index 10 w l ls 1 ti "t=1.0t_{rel}",\
"fpMax30np600p01E25zMax20nz400v4.1.1den.dat" u 1:3 index 20 w l ls 2 ti "t=2.0t_{rel}",\
"fpMax30np600p01E25zMax20nz400v4.1.1den.dat" u 1:3 index 50 w l ls 3 ti "t=5.0t_{rel}",\
"fpMax30np600p01E25zMax20nz400v4.1.1den.dat" u 1:3 index 80 w l ls 4 ti "t=8.0t_{rel}"
#pause -1
reset
set term x11
