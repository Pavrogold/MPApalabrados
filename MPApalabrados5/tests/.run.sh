touch tests//.timeout
CMD="valgrind --leak-check=full  valgrind --leak-check=full /home/acid/ownCloud/Scrabble/MPalabrados5/dist/Debug/GNU-Linux/mpalabrados5  -l ES -r 100 -w 10 -h 10 -p data/OPEN_ERROR.data 1> tests//.out12 2>&1"
eval $CMD
rm tests//.timeout
