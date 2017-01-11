/*
 * Sudoku main for just command line support
 */

#include <stdio.h>
#include "sudoku.h"

#define SUDOKU_TO_SOLVE courier_sudoku

int main() {
	printf("\nJames' Sudoku Solving Program Version 3\n\n");

	sudoku_solve(SUDOKU_TO_SOLVE);

	// wait till key press (for windows)
	// getchar();

	return 0;
}
