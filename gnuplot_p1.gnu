#!/usr/local/bin/gnuplot -persist
set datafile separator ";"

set xrange [10:100000]
set yrange[80000:120000]

unset key
set term pngcairo
set term pngcairo size 1280, 960
set output "graphique.png"

plot "sortie.txt" using 1:3:4 with filledcurve fc rgb "#FFF0F0", "sortie.txt" using 1:2 with lines
