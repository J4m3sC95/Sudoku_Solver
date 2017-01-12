/*
 * Trial and error solving functionality
 */

#include "sudoku_tande.h"

// Function to find all places where two numbers share possibilities (return a vector?)

// Function to determine whether it is still possible to solve the sudoku
unsigned char solvable(unsigned char sudoku[82], unsigned char master_possibilities[10][82]){
	return 0;
}

// Function to solve a sudoku by trial and error
unsigned char trial_and_error(unsigned char start_sudoku[82], unsigned char end_sudoku[82], unsigned char master_possibilities[10][82]){
	unsigned char n, num, opcode, sub_num;
	unsigned char *data;
	unsigned char location[] = {0,0};
	// define a function array for the extract functions
	unsigned char* (*extract[3])(unsigned char, unsigned char [82]) = {&extract_square, &extract_row, &extract_col};
	
	// load end_sudoku with start sudoku values
	for(n = 0; n < 81; n++){
		end_sudoku[n] = start_sudoku[n];
	}
	// find a pair of numbers which share the same possibilities in a row/col/square
	// loop through square -> row -> col
	for(opcode = 0; opcode < 3; opcode++){
		// loop through all rows/cols/squares
		for(n = 0; n < 9; n++){
			// loop through all numbers
			for(num = 1; num<=8; num++){
				data = (*extract[opcode])(n, master_possibilities[num]);
				// find out if current number has two possibilities in current location
				if(is_num(data, 1) == 2){
					location[0] = where_num(data, 1);
					location[1] = where_num(data, 1, 1);
					// now we've found a number with two possibilities in this location can we find another with the same
					for(sub_num = num + 1; sub_num <=9; sub_num++){
						data = (*extract[opcode])(n, master_possibilities[sub_num]);
						// find out if current number has two possibilities in current location
						if(is_num(data, 1) == 2){
							if((where_num(data,1) == location[0]) && (where_num(data,1,1) == location[1])){
								printf("Number %d has 2 possibilities in %s %d at locations %d and %d and so does number %d\n", num, (opcode ? ((opcode == 1) ? "row" : "col") : "square"), n, location[0], location[1], sub_num);
								goto found_match;
							}
						}
					}
				}
			}
		}
	}
	// program will continue to this point if a match is found
	// (could perhaps avoid this and store all options for future use if this doesnt work?)
	found_match:
	
	// attempt to solve the sudokus with the numbers one way around and then the other
	// if can't solve and it still possible to complete then either run this function again (recursively?) with two new pairs or try a different pair?
	return 0;
}

int main() {
	printf("\nJames' Sudoku Solving Program Version 3\n\n");
	
	unsigned char output_sudoku[82];
	
	unsigned char possibility_matrix[10][82];

	sudoku_solve(SUDOKU_TO_SOLVE, output_sudoku, possibility_matrix);
	
	trial_and_error(output_sudoku, output_sudoku, possibility_matrix);

	// wait till key press (for windows)
	// getchar();

	return 0;
}
