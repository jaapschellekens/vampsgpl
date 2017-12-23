vamps vols.ini
vsel -p theta -n 30  vols.out > vols_30.prn
vsel -p theta -n 7  vols.out > vols_7.prn
vsel -p theta -n 3  vols.out > vols_3.prn
vsel -p transpiration  vols.out > vols_trans.prn
vsel -p interception  vols.out > vols_int.prn
vsel -p soilevaporation  vols.out > vols_soilevap.prn
vsel -p precipitation  vols.out > vols_prec.prn
vsel -p rootextract  vols.out > vols_rot.prn
vsel -p SMD  vols.out > vols_smd.prn
#gnuplot -e "plot 'vols_3.prn' using 1:2 with lines, 'vols_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot 'vols_7.prn' using 1:2 with lines, 'vols_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot 'vols_30.prn' using 1:2 with lines, 'vols_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
