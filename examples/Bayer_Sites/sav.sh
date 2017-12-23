vamps sav.ini
vsel -p theta -n 30  sav.out > sav_30.prn
vsel -p theta -n 7  sav.out > sav_7.prn
vsel -p theta -n 3  sav.out > sav_3.prn
vsel -p transpiration  sav.out > sav_trans.prn
vsel -p interception  sav.out > sav_int.prn
vsel -p soilevaporation  sav.out > sav_soilevap.prn
vsel -p precipitation  sav.out > sav_prec.prn
vsel -p rootextract  sav.out > sav_rot.prn
vsel -p SMD  sav.out > sav_smd.prn
#gnuplot -e "plot 'sav_3.prn' using 1:2 with lines, 'sav_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot 'sav_7.prn' using 1:2 with lines, 'sav_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot 'sav_30.prn' using 1:2 with lines, 'sav_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
