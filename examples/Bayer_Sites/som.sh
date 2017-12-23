vamps som.ini
vsel -p theta -n 30  som.out > som_30.prn
vsel -p theta -n 7  som.out > som_7.prn
vsel -p theta -n 3  som.out > som_3.prn
vsel -p transpiration  som.out > som_trans.prn
vsel -p interception  som.out > som_int.prn
vsel -p soilevaporation  som.out > som_soilevap.prn
vsel -p precipitation  som.out > som_prec.prn
vsel -p rootextract  som.out > som_rot.prn
vsel -p SMD  som.out > som_smd.prn
#gnuplot -e "plot 'som_3.prn' using 1:2 with lines, 'som_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot 'som_7.prn' using 1:2 with lines, 'som_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot 'som_30.prn' using 1:2 with lines, 'som_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
