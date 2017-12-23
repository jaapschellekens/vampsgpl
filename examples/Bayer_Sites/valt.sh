#vamps valt.ini
vsel -p theta -n 30  valt.out > 30.prn
vsel -p theta -n 100  valt.out > 100.prn
vsel -p theta -n 199  valt.out > 200.prn
vsel -p theta -n 7  valt.out > 7.prn
vsel -p theta -n 3  valt.out > 3.prn
vsel -p transpiration  valt.out > trans.prn
vsel -p interception  valt.out > int.prn
vsel -p soilevaporation  valt.out > soilevap.prn
vsel -p precipitation  valt.out > prec.prn
vsel -p rootextract  valt.out > rot.prn
vsel -p SMD  valt.out > smd.prn
#gnuplot -e "plot '3.prn' using 1:2 with lines, 'valt_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot '7.prn' using 1:2 with lines, 'valt_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot '100.prn' using 1:2 with lines, 'valt_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot '200.prn' using 1:2 with lines, 'valt_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot '30.prn' using 1:2 with lines, 'valt_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
