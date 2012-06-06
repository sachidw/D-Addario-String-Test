set title "FFT"
set ylabel "Magnitude"
set xlabel "Frequency(Hz)"
set xrange [0:614]
#set yrange [0:1000]
plot "FFT.dat" with impulses
pause -1

