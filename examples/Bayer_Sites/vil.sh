vamps vil.ini
vsel -p theta -n 30  vil.out > vil_30.prn
vsel -p theta -n 7  vil.out > vil_7.prn
vsel -p theta -n 3  vil.out > vil_3.prn
vsel -p transpiration  vil.out > vil_trans.prn
vsel -p interception  vil.out > vil_int.prn
vsel -p soilevaporation  vil.out > vil_soilevap.prn
vsel -p precipitation  vil.out > vil_prec.prn
vsel -p rootextract  vil.out > vil_rot.prn
vsel -p SMD  vil.out > vil_smd.prn
#gnuplot -e "plot 'vil_3.prn' using 1:2 with lines, 'vil_sm_sat.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot 'vil_7.prn' using 1:2 with lines, 'vil_sm_sat.csv' using 1:2 with linesp; pause -1"
#gnuplot -e "plot 'vil_30.prn' using 1:2 with lines, 'vil_soilm.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
