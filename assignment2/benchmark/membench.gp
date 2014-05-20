set logscale x 2
set xtics (4,16,64,256,"1K" 1024,"4K" 4096,"16K" 16384,"64K" 65536,"256K" 262144,"1M" 1048576)
set ylabel "Time (nsec)"
set xlabel "Stride (bytes)"
set title ""
set yrange [0:600]
set term postscript color
plot "< awk '{if ($2 == 4096) print $4,$6}' membench.out" title "4KB" with linespoints, \
     "< awk '{if ($2 == 8192) print $4,$6}' membench.out" title "8KB" with linespoints, \
     "< awk '{if ($2 == 16384) print $4,$6}' membench.out" title "16KB" with linespoints, \
     "< awk '{if ($2 == 32768) print $4,$6}' membench.out" title "32KB" with linespoints, \
     "< awk '{if ($2 == 65536) print $4,$6}' membench.out" title "64KB" with linespoints, \
     "< awk '{if ($2 == 131072) print $4,$6}' membench.out" title "128KB" with linespoints, \
     "< awk '{if ($2 == 262144) print $4,$6}' membench.out" title "256KB" with linespoints, \
     "< awk '{if ($2 == 524288) print $4,$6}' membench.out" title "512KB" with linespoints, \
     "< awk '{if ($2 == 1048576) print $4,$6}' membench.out" title "1MB" with linespoints, \
     "< awk '{if ($2 == 2097152) print $4,$6}' membench.out" title "2MB" with linespoints, \
     "< awk '{if ($2 == 4194304) print $4,$6}' membench.out" title "4MB" with linespoints
