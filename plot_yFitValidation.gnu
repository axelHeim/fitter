reset

set grid

set ylabel 'ratio of total deposited Energy'
set xlabel 'distance of cell centre to centre of gravity / mm'


set xrange[-110:110]
set yrange[-0.1:1]



plot 'fit_validationY.dat' using 1:2:3 with yerrorbars t 'y-axis data points', 'fit_validationYfunc.dat' w lines t 'function along y-axis', 'fit_validationXfunc.dat' w lines t 'function along x-axis','fit_validationX.dat' using 1:2:3 with yerrorbars t 'x-axis data points'
