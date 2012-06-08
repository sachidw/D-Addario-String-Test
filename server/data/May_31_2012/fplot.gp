set title "Current Frequency Plot"
set ylabel "Frequency(Hz) "
set xlabel "Time"
set xdata time
#set y2range [0:1]
set timefmt  "%m/%d_%H:%M:%S"
#set format x "%H:%M\n%m/%d"
#set terminal jpeg large size 1200,900
#set output "freqCurrent.jpeg"
plot "fcurrent" using 1:2 with linespoints
pause -1
