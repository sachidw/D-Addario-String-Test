set title "Current Frequency and Displacement Plot"
set ylabel "Frequency(Hz) "
set y2label "Displacement(inch)"
set xlabel "Time"
set xdata time
set boxwidth 100
set y2tics nomirror

#set xtics ("<awk '{print $1}' currentInch")


#set y2range [0:1]
set timefmt  "%m/%d_%H:%M:%S"
set format x "%H:%M\n%m/%d"
set terminal jpeg large size 1200,900
set output "freqdisCurrent.jpeg"
plot "fcurrent" using 1:2 with linespoints, "currentInch" using 1:2 with boxes axis x1y2 

