#!/usr/local/bin/gnuplot -persist
set datafile separator ";"
set xrange [2009:2024]
set yrange[-30:60]
unset key
set term pngcairo
set term pngcairo size 1280, 960
set output "graphique.png"

plot "sortie.txt" using 1:2 with lines
unset key
