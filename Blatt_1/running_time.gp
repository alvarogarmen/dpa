set xlabel 'Number of Points'
set ylabel 'Running Time (seconds)'
set title 'Running Time vs. Number of Points'
set grid
set terminal png
set output 'running_time.png'
plot 'running_time.csv' using 1:2 with linespoints pointtype 7 pointsize 1.5 title 'Running Time'
quit
