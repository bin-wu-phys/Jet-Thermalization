#!/usr/bin/gnuplot
reset
set term pdf color enh dashed dl 3.5 size 6,6
set out "eLoss.pdf"
set size ratio 1.0

set key font "Helvetica,28"
set tics font "Helvetica,28"
set xlabel font "Helvetica,28"
set ylabel font "Helvetica,28"
set key reverse
set key spacing 4 samplen 6
set key width 10

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "#008000"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"

set key reverse
set key bottom left at 4.5,2.0

#set xrange [1e-2:30]
#set yrange [0:0.019]

set xlabel "t/t_{rel}" offset 0,-0.5
set ylabel "E_{loss}" offset -3

E=25.0

plot E*(1-exp(-2.5*x**2/E)) w l ls 3 ti "Perfect sink",\
"fpMax30np600p01E25zMax20nz400v4.1.1tot.dat" u 1:6 every 3 w l ls 1 ti "QGP"
#pause -1
reset
set term x11
