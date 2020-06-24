#!/usr/bin/gnuplot
reset
set term post portrait color enh dl 3.5 size 8,6
set out "t5.eps"
set size ratio .8

set tics font "Helvetica,16"
set xlabel font "Helvetica,16"
set ylabel font "Helvetica,16"

set style line 2 lw 12 lt 1 ps 1.5 pt 4 lc rgb "red"
set style line 3 lw 12 lt 2 pt 7 ps 1.1 lc rgb "#ff6600"
set style line 4 lw 12 lt 5 pt 7 ps 1.1 lc rgb "dark-green"
set style line 5 lw 12 lt 6 pt 7 ps 1.1 lc rgb "blue"
set style line 1 lw 12 lt 1 pt 7 ps 1.1 lc rgb "#800080"
set style line 6 lw 2 lt 2 pt 7 ps 1.1 lc rgb "black"


#set xrange [0.4:]
#set yrange [1e-3:3]


set xlabel "z/t_{red}"
set ylabel "p/T"
#set zlabel "f"

set palette defined (0 "#008080ff", 0.2 "yellow", 1 "red")

set title "t = 5.0 t_{rel}"

set view 30, 250

set xrange [-2:8]
set yrange [-5:10]

splot "fpMax20np1000p01E15zMax20nz200.dat" u 1:2:3 index 50 w pm3d noti,\
"feqz0.dat" u 1:2:(0.034*$3) w l ls 6 noti
#pause -1
reset
set term x11
