vamps wurz.ini
vsel -p theta -n 30  wurz.out > 30.prn
vsel -p theta -n 7  wurz.out > 7.prn
vsel -p theta -n 3  wurz.out > 3.prn
vsel -p transpiration  wurz.out > trans.prn
vsel -p interception  wurz.out > int.prn
vsel -p soilevaporation  wurz.out > soilevap.prn
vsel -p precipitation  wurz.out > prec.prn
vsel -p rootextract  wurz.out > rot.prn
vsel -p SMD  wurz.out > smd.prn
#gnuplot -e "plot '3.prn' using 1:2 with lines, 'wurz_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot '7.prn' using 1:2 with lines, 'wurz_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot '30.prn' using 1:2 with lines, 'wurz_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
