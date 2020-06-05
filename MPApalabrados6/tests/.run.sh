touch tests//.timeout
CMD="valgrind --leak-check=full  valgrind --leak-check=full /home/yesenia/Escritorio/MP6/MPApalabrados/MPApalabrados6/dist/Debug/GNU-Linux/mpapalabrados6  -open data/OPEN_ERROR.data 1> tests//.out9 2>&1"
eval $CMD
rm tests//.timeout
