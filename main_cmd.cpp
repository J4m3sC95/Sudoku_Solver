/*
 * Sudoku main for just command line support
 */

#include <stdio.h>
#include "sudoku.h"

#define SUDOKU_TO_SOLVE courier_sudoku

int main() {
	printf("\nJames' Sudoku Solving Program Version 3\n\n");
	
	unsigned char output_sudoku[82];
	unsigned char possibility_matrix[10][82];

	sudoku_solve(SUDOKU_TO_SOLVE, output_sudoku, possibility_matrix);

	// wait till key press (for windows)
	// getchar();

	return 0;
}
