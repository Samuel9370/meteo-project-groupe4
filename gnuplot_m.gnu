#!/usr/local/bin/gnuplot -persist
set datafile separator ";"
set xrange [-50:140]
set yrange[-70:60]
unset key
set term pngcairo
set term pngcairo size 1280, 960
set output "graphique.png"

set palette defined (0 "blue", 17 "#00ffff",33 "white",50 "yellow",66 "red",100 "#990000",101 "purple")
set cbrange [80:100]
set view map
set pm3d at b
set dgrid3d 100,100,1
splot "sortie.txt" u 4:3:2

