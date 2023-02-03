#!/usr/local/bin/gnuplot -persist
set datafile separator ";"
set xrange [5000:90000]
set yrange[-20:65]


unset key
set term pngcairo
set term pngcairo size 1280, 960
set output "graphique.png"

plot "sortie.txt" using 1:3:4 with filledcurve fc rgb "#FFF0F0", "sortie.txt" using 1:2 with lines
