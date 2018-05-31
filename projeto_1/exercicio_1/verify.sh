# !/bin/bash

gcc -o sudoku.out sudoku.c -lpthread


for i in $(ls "sudokus"); do
	echo "Arquivo :"$i
       	./sudoku.out < "sudokus/"$i
done

rm sudoku.out 
