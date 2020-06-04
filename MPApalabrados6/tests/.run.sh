touch tests//.timeout
CMD="valgrind --leak-check=full  valgrind --leak-check=full /home/yesenia/Escritorio/MP6/MPApalabrados/MPApalabrados6/dist/Debug/GNU-Linux/mpapalabrados6  -open data/ES_100_0.match -save tests/.out.match < data/ES_100_0_156.data 1> tests//.out6 2>&1"
eval $CMD
rm tests//.timeout
