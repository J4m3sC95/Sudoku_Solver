CC=g++
CFLAGS=-I.

default:
	@echo "\nBuild Options for Sudoku Solver Program:"
	@echo "- sudoku_cmd:\tsudoku solver for use on command line"
	@echo "- sudoku_gtk:\tsudoku solver with gtk+2.0 GUI"
	@echo "- all:\t\tbuild all of the above"
	@echo "- clean:\tremoves any built files\n"

all: sudoku_gtk sudoku_cmd

sudoku_gtk: sudoku.o sudoku_tande.o main_gtk.cpp
	$(CC) -o $@ sudoku.o sudoku_tande.o main_gtk.cpp $(CFLAGS) `pkg-config --libs --cflags gtk+-2.0` 

sudoku_cmd: sudoku.o sudoku_tande.o main_cmd.cpp
	$(CC) -o $@ sudoku.o sudoku_tande.o main_cmd.cpp $(CFLAGS)

sudoku_tande.o: sudoku_tande.cpp sudoku_tande.h
	$(CC) -c -o $@ sudoku_tande.cpp $(CFLAGS)

sudoku.o: sudoku.h sudoku.cpp
	$(CC) -c -o sudoku.o sudoku.cpp $(CFLAGS)

clean:
	rm -f sudoku_gtk sudoku_cmd sudoku.o sudoku_tande.o
