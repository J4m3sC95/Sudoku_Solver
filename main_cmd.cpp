/*
 * Sudoku main for just command line support
 */

#include <stdio.h>
#include "sudoku.h"
#include "sudoku_examples.h"

#define SUDOKU_TO_SOLVE courier_sudoku

int main() {
	unsigned char n;
	printf("\nJames' Sudoku Solving Program Version 3\n\n");
	
	unsigned char output_sudoku[82];
	unsigned char **possibility_matrix = (unsigned char**) malloc(10*sizeof(unsigned char*));
	// build possibility array
	for(n = 0; n < 10; n++){
		possibility_matrix[n] = (unsigned char*) malloc(82*sizeof(unsigned char));
	}
	build_master_possibilities(SUDOKU_TO_SOLVE, possibility_matrix);
	sudoku_solve(SUDOKU_TO_SOLVE, output_sudoku, possibility_matrix);

	// wait till key press (for windows)
	// getchar();

	return 0;
}
