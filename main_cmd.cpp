/*
 * Sudoku main for just command line support
 */

#include <stdio.h>
#include "sudoku.h"
#include "sudoku_tande.h"
#include "sudoku_examples.h"

#define SUDOKU_TO_SOLVE metro_sudoku_easy
unsigned char output_sudoku[82];

int main() {
	sudoku_solve_tande(SUDOKU_TO_SOLVE, output_sudoku);

	// wait till key press (for windows)
	// getchar();

	return 0;
}
