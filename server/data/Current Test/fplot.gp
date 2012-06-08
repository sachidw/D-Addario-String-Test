set title "Current Frequency Plot Strings 1,2" 
set font "Times-Roman,40"


set xlabel "Time"
set xdata time
set ytics nomirror
set y2tics
set ylabel "Frequency(Hz) String 1" textcolor lt 1
set y2label "Frequency(Hz) String 2" textcolor lt 2

set timefmt  "%m/%d_%H:%M:%S"
set format x "%H:%M\n%m/%d"
set terminal jpeg large size 1200,900
set output "freqCurrentS1S2.jpeg"
plot "currentFrequency.txt" using 1:3 with linespoints, "currentFrequency.txt" using 1:4 with linespoints axis x1y2

