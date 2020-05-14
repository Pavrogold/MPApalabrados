touch tests//.timeout
CMD="valgrind --leak-check=full  valgrind --leak-check=full /home/yesenia/Escritorio/MP/Entrega5/MPApalabrados/MPApalabrados5/dist/Debug/GNU-Linux/mpapalabrados5  -open data/ES_2020_155_A.match -p data/ES_2020_155_B.data  1> tests//.out7 2>&1"
eval $CMD
rm tests//.timeout
