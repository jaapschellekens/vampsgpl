vamps gros.ini
vsel -p theta -n 30  gros.out > gros_30.prn
vsel -p theta -n 7  gros.out > gros_7.prn
vsel -p theta -n 3  gros.out > gros_3.prn
vsel -p transpiration  gros.out > gros_trans.prn
vsel -p interception  gros.out > gros_int.prn
vsel -p soilevaporation  gros.out > gros_soilevap.prn
vsel -p precipitation  gros.out > gros_prec.prn
vsel -p rootextract  gros.out > gros_rot.prn
vsel -p SMD  gros.out > gros_smd.prn
#gnuplot -e "plot 'gros_3.prn' using 1:2 with lines, 'gros_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot 'gros_7.prn' using 1:2 with lines, 'gros_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot 'gros_30.prn' using 1:2 with lines, 'gros_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
