#!/usr/bin/gnuplot
reset
set term post portrait color enh dl 3.5 size 8,6

set tics font "Helvetica,14"
set xlabel font "Helvetica,16"
set ylabel font "Helvetica,16"

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "dark-green"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"



set xlabel "z/t_{rel}"
set ylabel "p/T" offset -4
set zlabel "|p|f/T"

set palette defined (0 "#008080ff", 0.2 "yellow", 1 "red")

set view 30, 30

set xrange [-3:10]
set yrange [-5:30]

set ztics 0,1.0,4.0
set size ratio .8

set out "energy1.eps"

set colorbox user origin 0.17,0.5 size 0.025,0.3

set label "t = 1.0 t_{rel}" at 3,33,0.3
splot "fpMax30np600p01E25zMax20nz400v4.1.dat" u 1:2:(abs($2)*$3) index 10 w pm3d noti

unset colorbox

#set out "energy2.eps"

#set label "t = 2.0 t_{rel}" at 3,33,0.3
#splot "fpMax30np600p01E25zMax20nz400v4.1.dat" u 1:2:(abs($2)*$3) index 20 w pm3d noti

#set out "energy5.eps"

#set label "t = t_{br}(E)=5 t_{rel}" at 3,33,0.3
#splot "fpMax30np600p01E25zMax20nz400v4.1.dat" u 1:2:(abs($2)*$3) index 50 w pm3d noti

#set out "energy8.eps"

#set label "t = 8.0 t_{rel}" at 3,33,0.3
#splot "fpMax30np600p01E25zMax20nz400v4.1.dat" u 1:2:(abs($2)*$3) index 80 w pm3d noti

reset
set term x11
