#!/usr/bin/gnuplot
#set xrange [-2:2]
#set yrange [-2:2]

set xlabel "z"
set ylabel "p"
set zlabel "f"

set palette defined (0 "#008080ff", 0.2 "yellow", 1 "red")

#splot "fzp.dat" u 1:2:3 index 2 w pm3d
#pause -1
