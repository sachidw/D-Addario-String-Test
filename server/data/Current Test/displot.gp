set title "Current Displacent Plot String 1,2"

set xlabel "Time"
set xdata time
set ytics nomirror
set y2tics
set ylabel "Displacemnt (inch) String 1" textcolor lt 1
set y2label "Displacemnt (inch) String 2" textcolor lt 2




set timefmt  "%m/%d_%H:%M:%S"
set format x "%H:%M\n%m/%d"
set terminal jpeg large size 1200,900
set output "currentDisplacementS1S2.jpeg"
set boxwidth 50
plot "currentDisplacementInch.txt" using 1:6 with linespoints, "currentDisplacementInch.txt" using 1:7 with linespoints axis x1y2
