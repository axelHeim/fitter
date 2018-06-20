reset

set grid

set ylabel 'deposited Energy in cell'
set xlabel 'distance of cell centre to centre of gravity / mm'


set xrange[-110:110]
set logscale y



plot 'fit_validationY.dat' using 1:2:3 with yerrorbars t 'y-axis data points', 'fit_validationX.dat' using 1:2:3 with yerrorbars t 'x-axis data points', 'fit_validationYfunc.dat' w lines t 'energDep*totalEnergyDeposit y-axis [GeV]', 'fit_validationXfunc.dat' w lines t 'energDep*totalEnergyDeposit x-axis [GeV]'
