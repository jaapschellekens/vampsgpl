vamps trem.ini
vsel -p theta -n 30  trem.out > 30.prn
vsel -p theta -n 7  trem.out > 7.prn
vsel -p theta -n 3  trem.out > 3.prn
vsel -p transpiration  trem.out > trans.prn
vsel -p interception  trem.out > int.prn
vsel -p soilevaporation  trem.out > soilevap.prn
vsel -p precipitation  trem.out > prec.prn
vsel -p rootextract  trem.out > rot.prn
vsel -p SMD  trem.out > smd.prn
gnuplot -e "plot '3.prn' using 1:2 with lines, 'trem_soilm.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot '7.prn' using 1:2 with lines, 'trem_soilm.csv' using 1:2 with linesp; pause -1"
gnuplot -e "plot '30.prn' using 1:2 with lines, 'trem_weather.csv' using 1:8 with linesp axis x1y2; pause -1"
#gnuplot -e 'plot "prec.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "trans.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "rot.prn" using 2 with linesp; pause -1'
#gnuplot -e 'plot "int.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "soilevap.prn" using 2 with linesp smooth  cumulative; pause -1'
#gnuplot -e 'plot "smd.prn" using 2 with linesp; pause -1'
