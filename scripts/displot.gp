set title "Current Displacent Plot"
set ylabel "Displacemnt (inch) "
set xlabel "Time"
set xdata time
set timefmt  "%m/%d_%H:%M:%S"
set format x "%H:%M\n%m/%d"
set terminal jpeg large size 1200,900
set output "disCurrent.jpeg"
set boxwidth 50
plot "currentInch" using 1:4 with linespoints, "currentInch" using 1:5 with linespoints
