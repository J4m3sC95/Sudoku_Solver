/*
 * Sudoku solving functions
 */

#include "sudoku.h"

// if compiling for arduino
#ifdef ARDUINO
// buffer to store printf data in for macro to avoid rewriting all printfs to serial prints
char buffer[100];
#define printf(fmt,...) sprintf(buffer, fmt, ##__VA_ARGS__); Serial.print(buffer);
#endif

// function which determines if a number is in a 9 element array (row/col/square)
// 0 = not there, if present, a count will be returned
unsigned char is_num(unsigned char data[10], unsigned char num) {
	unsigned char n, count = 0;
	for (n = 0; n < 9; n++) {
		if (data[n] == num) {
			count += 1;
		}
	}
	return count;
}

// function to find number in 9 element array (row/col/square)
// search direction defined by argument direction: 0 (default) = forwards, 1 = reverse;
// 0 if not there, otherwise returns location
unsigned char where_num(unsigned char data[10], unsigned char num, unsigned char direction) {
	unsigned char n, m;
	for (n = 0; n < 9; n++) {
		m = (direction ? (8 - n) : n);
		if (data[m] == num) {
			return m;
		}
	}
	return 0;
}

// function to extract a square from the overall sudoku matrix
// squares numbered from 0 to 8, left to right, top to bottom
unsigned char *extract_square(unsigned char square, unsigned char sudoku[82]) {
	static unsigned char square_data[10];
	unsigned char row, col, index;
	unsigned char first_element = ((square / 3) * 27) + ((square % 3) * 3);
	for (row = 0; row < 3; row++) {
		for (col = 0; col < 3; col++) {
			index = (row * 9) + col + first_element;
			square_data[(row * 3) + col] = sudoku[index];
		}
	}
	return square_data;
}

// function to extract a row from the overall sudoku matrix
// rows numbered from 0 to 8, top to bottom
unsigned char *extract_row(unsigned char row, unsigned char sudoku[82]) {
	static unsigned char row_data[10];
	unsigned char n;
	for (n = 0; n < 9; n++) {
		row_data[n] = sudoku[(row * 9) + n];
	}
	return row_data;
}

// function to extract a column from the overall sudoku matrix
// columns numbered from 0 to 8, left to right
unsigned char *extract_col(unsigned char col, unsigned char sudoku[82]) {
	static unsigned char col_data[10];
	unsigned char n;
	for (n = 0; n < 9; n++) {
		col_data[n] = sudoku[(n * 9) + col];
	}
	return col_data;
}

// function to print the contents of a square for debug use
void square_print(unsigned char data[10]) {
	unsigned char n,m;
	for(m = 0; m<=6; m+=3){
		printf("\n");
		for (n = m; n < (3 + m); n++) {
			printf("%d, ", data[n]);
		}
	}
	printf("\n");
}

// function to print entire sudoku
void sudoku_print(unsigned char sudoku[82]) {
	unsigned char n, m, p, q;
	for(q = 0; q <= 6; q+=3){
		printf("\n");
		for (n = q; n < (3 + q); n++) {
			for (m = 0; m < 3; m++) {
				for (p = 0; p < 3; p++) {
					printf("%d, ", sudoku[(n * 9) + (m * 3) + p]);
				}
				printf("\t");
			}
			printf("\n");
		}
	}
}

// function to print entire sudoku to file
void sudoku_file_print(unsigned char sudoku[82], FILE *output_file) {
	unsigned char n, m, p, q;
	for(q = 0; q<=6; q+=3){
		fprintf(output_file, "\n");
		for (n = q; n < (3 + q); n++) {
			for (m = 0; m < 3; m++) {
				for (p = 0; p < 3; p++) {
					fprintf(output_file, "%d, ", sudoku[(n * 9) + (m * 3) + p]);
				}
				fprintf(output_file, "\t");
			}
			fprintf(output_file, "\n");
		}
	}

}

// build array of arrays to hold all possibilities
// first array is gap array (1 = gap, 0 = no gap)
// subsequent arrays are possibilities for each number (0 = not possible, 1 = possible)
// unsigned char master_possibilities[10][82];
void build_master_possibilities(unsigned char sudoku[82], unsigned char master_possibilities[10][82]) {
	unsigned char n,m,p;
	// find gaps
	for (n = 0; n < 81; n++) {
		// if gap then p = 1 else = 0
		p = (sudoku[n] == 0) ? 1 : 0;
		for (m = 0; m < 10; m++) {
			master_possibilities[m][n] = p;
		}
	}
}

// function to count the remaining gaps
unsigned char count_gaps(unsigned char master_possibilities[10][82]) {
	unsigned char n, gaps = 0;
	for (n = 0; n < 81; n++) {
		if (master_possibilities[0][n] == 1) {
			gaps += 1;
		}
	}
	return gaps;
}

// function to count the remaining possibilities
unsigned int count_possibilties(unsigned char master_possibilities[10][82]){
	unsigned char n, m, count = 0;
	for(n = 1; n<=9; n++){
		for(m = 0; m<81; m++){
			if(master_possibilities[n][m]){
				count ++;
			}
		}
	}
	return count;
}

// function to update the master_possibilities array
void update_master_possibilities(unsigned char sudoku[82], unsigned char master_possibilities[10][82]) {
	unsigned char n,m, square_num, row, col, num, position, row_line_up, col_line_up, opcode, opcode_num;
	unsigned char position1 = 0, position2 = 0, position3 = 0;
	unsigned char *data, *possibility_data;
	// define a function array for the extract functions
	unsigned char* (*extract[3])(unsigned char, unsigned char [82]) = {&extract_square, &extract_row, &extract_col};
	
	// cycle through numbers to eliminate stuff
	for (num = 1; num <= 9; num++) {
		// eliminate based on squares
		for (square_num = 0; square_num < 9; square_num++) {
			data = extract_square(square_num, sudoku);
			// does it already exist here??
			if (is_num(data, num) == 0) {
				// loop through opcodes 1 and 2 row then col
				for(opcode = 1; opcode < 3; opcode++){
					for (n = 0; n < 3; n++) {
						opcode_num = ((opcode == 1) ? (((square_num / 3) * 3) + n) : (((square_num % 3) * 3) + n));
						// check to see if number in that row and eliminate that row from possibility if so
						if (is_num((*extract[opcode])(opcode_num, sudoku), num) == 1) {
							for (m = 0; m < 3; m++) {
								// if row (opcode = 1): position = square starting column + current row + m
								// if col (opcode = 2): position = square starting row + current column + m time row
								position = ((opcode == 1) ? (((square_num % 3) * 3) + (opcode_num * 9) + m) : (((square_num / 3) * 27) + opcode_num + (m * 9)));
								master_possibilities[num][position] = 0;
							}
						}
					}
				}

				// eliminate possibilities in other squares based on 2 or 3 possibilities lining up in this square
				possibility_data = extract_square(square_num, master_possibilities[num]);
				m = is_num(possibility_data, 1);
				if ((m == 2) | (m == 3)) {
					// where are they
					position1 = where_num(possibility_data, 1);
					for (n = 0; n <= 9; n++) {
						if (possibility_data[n] == 1) {
							position2 = n;
						}
					}
					if (m == 3) {
						for (n = 0; n < position2; n++) {
							if (possibility_data[n] == 1) {
								position3 = n;
							}
						}
					}
					else {
						position3 = position2;
					}
					// do the 2 or 3 possibilities line up?
					row_line_up = ((position1 / 3) == (position2 / 3)) && ((position2 / 3) == (position3 / 3));
					col_line_up = ((position1 % 3) == (position2 % 3)) && ((position2 % 3) == (position3 % 3));
					if (row_line_up) {
						row = (position1 / 3) + (square_num / 3) * 3;
						for (n = 0; n < 9; n++) {
							if (n != (((square_num % 3) * 3) + (n % 3))) {
								master_possibilities[num][(row * 9) + n] = 0;
							}
						}
					}
					else if (col_line_up) {
						col = (position1 % 3) + (square_num % 3) * 3;
						for (n = 0; n < 9; n++) {
							if (n != (((square_num / 3) * 3) + (n % 3))) {
								master_possibilities[num][(n * 9) + col] = 0;
							}
						}
					}
				}
			}
			else {
				// if in this square set all to zero
				for (n = 0; n < 9; n++) {
					// position = square starting col + square starting row + square current col + square current row
					position = ((square_num % 3) * 3) + ((square_num / 3) * 27) + (n % 3) + ((n / 3) * 9);
					master_possibilities[num][position] = 0;
				}
			}
		}
	}
}

// fuction including some radical update methods
void extreme_update(unsigned char sudoku[82], unsigned char master_possibilities[10][82]) {
	unsigned char n, m, square, row, col, p, q, position, num;
	unsigned char count = 0, row_count = 0, col_count = 0, zero_count = 0;
	unsigned char *temp_data;
	unsigned char data[3][10];
	unsigned char squares[4] = { 0,0,0 };
	// loop through numbers
	for (num = 1; num <= 9; num++) {
		/*** ELIMINATE SPACES BASED ON TWO COLUMNS/ROWS ELIMINATED DUE TO POSSIBILITIES FOR TWO SQUARES SHARING SAME ROW/COL POSSIBILITIES ***/
		// loop through squares on diagonal ie: 0,4,8
		for (n = 0; n <= 8; n += 4) {
			// check to see if any squares on the squares row have num in it
			count = 0;
			for (m = 0; m < 3; m++) {
				square = (n - (n % 3)) + m;
				if (!is_num(extract_square(square, sudoku), num)) {
					count++;
				}
			}
			// if so check to see whether 2 rows of 3 zero possibilities line up
			if (count == 3) {
				for (m = 0; m < 3; m++) {
					temp_data = extract_square((n + m - (n % 3)), master_possibilities[num]);
					for (p = 0; p < 10; p++) {
						data[m][p] = temp_data[p];
					}
				}
				// check each row
				for (row = 0; row < 3; row++) {
					row_count = 0;
					// each square
					for (p = 0; p < 3; p++) {
						// each num
						zero_count = 0;
						for (m = 0; m < 3; m++) {
							if (data[p][(row * 3) + m] == 0) {
								zero_count++;
							}
						}
						// if so eliminate these rows from the remaining square
						if (zero_count == 3) {
							squares[row_count] = p;
							row_count++;
							if (row_count == 2) {
								squares[2] = 0;
								while ((squares[2] == squares[0]) || (squares[2] == squares[1])) {
									squares[2] ++;
								}
								for (q = 0; q < 9; q++) {
									if ((q / 3) != row) {
										// position = squares starting row + square starting column + current col + current row
										position = ((n / 3) * 27) + ((squares[2] % 3) * 3) + (q % 3) + (q / 3) * 9;
										master_possibilities[num][position] = 0;
									}
								}
							}
						}
					}
				}
			}

			// check to see if all squares on the squares col have num in it
			count = 0;
			for (m = 0; m < 3; m++) {
				square = (n - ((n / 3) * 3)) + (m * 3);
				if (!is_num(extract_square(square, sudoku), num)) {
					count++;
				}
			}
			// if so check to see whether 2 cols of 3 zero possibilities line up
			if (count == 3) {
				for (m = 0; m < 3; m++) {
					temp_data = extract_square((n + (m * 3) - ((n / 3) * 3)), master_possibilities[num]);
					for (p = 0; p < 10; p++) {
						data[m][p] = temp_data[p];
					}
				}
				// check each col
				for (col = 0; col < 3; col++) {
					col_count = 0;
					// each square
					for (p = 0; p < 3; p++) {
						// each num
						zero_count = 0;
						for (m = 0; m < 3; m++) {
							if (data[p][col + (m*3)] == 0) {
								zero_count++;
							}
						}
						// if so eliminate these cols from the remaining square
						if (zero_count == 3) {
							squares[col_count] = p;
							col_count++;
							if (col_count == 2) {
								squares[2] = 0;
								while ((squares[2] == squares[0]) || (squares[2] == squares[1])) {
									squares[2] ++;
								}
								for (q = 0; q < 9; q++) {
									if ((q % 3) != col) {
										// position = squares starting row + square starting column + current col + current row
										position = (squares[2] * 27) + ((n%3)*3) + (q % 3) + (q / 3) * 9;
										master_possibilities[num][position] = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

// function to test some more methods for extreme_update
void extreme_test(unsigned char sudoku[82], unsigned char master_possibilities[10][82]) {
	/*** IF x (2 OR MORE) NUMBERS SHARE THE SAME x POSSIBILITIES THEN NO OTHER NUMBER CAN BE PUT IN THESE POSSIBILITIES ***/
	// this happens for numbers 3 and 8 in square 3 of the challenging metro sudoku
	unsigned char square, num, n, m, p, match, position;
	unsigned char index = 0, two_poss_count = 0;
	unsigned char *data;
	unsigned char two_poss[10] = { 0,0,0,0,0,0,0,0,0 };
	unsigned char locations[3] = { 0,0 };
	// cycle through squares
	for (square = 0; square < 9; square++) {
		index = 0;
		two_poss_count = 0;
		// cycle through numbers
		for (num = 1; num <= 9; num++) {
			// if 2 possibilities make a note of this
			if (is_num(extract_square(square, master_possibilities[num]), 1) == 2) {
				two_poss[index] = num;
				two_poss_count++;
				index++;
			}
		}
		// if there are two or more numbers with 2 possibilities in this square find out if they match
		if (two_poss_count >= 2) {
			for (n = 0; n < (two_poss_count - 1) ; n++) {
				for (m = 1; m < (two_poss_count - n); m++) {
					match = 1;
					//printf("n = %d, m = %d\n", n, m);
					for (p = 0; p < 9; p++) {
						// position = square starting col + square starting row + current row + current col
						position = ((square % 3) * 3) + ((square / 3) * 27) + ((p / 3) * 9) + (p % 3);
						if (master_possibilities[two_poss[n]][position] != master_possibilities[two_poss[n + m]][position]) {
							match = 0;
						}
					}
					if (match == 1) {
						// find the two locations
						data = extract_square(square, master_possibilities[two_poss[n]]);
						locations[0] = where_num(data, 1);
						for (p = 0; p < 9; p++) {
							if (data[p] == 1) {
								locations[1] = p;
							}
						}
						// remove these as possibilities for other numbers
						for (num = 1; num <= 9; num++) {
							if ((num != two_poss[n]) && (num != two_poss[n + m])) {
								// position = square starting col + square starting row + current row + current col
								position = ((square % 3) * 3) + ((square / 3) * 27) + ((locations[0] / 3) * 9) + (locations[0] % 3);
								master_possibilities[num][position] = 0;
								// position = square starting col + square starting row + current row + current col
								position = ((square % 3) * 3) + ((square / 3) * 27) + ((locations[1] / 3) * 9) + (locations[1] % 3);
								master_possibilities[num][position] = 0;
							}
						}
					}
				}
			}
		}
	}
	/*** PERHAPS EVEN x (2 OR MORE) NUMBERS SHARING x POSSIBILITIES BETWEEN THEM ***/
}

// function to fill in a sudoku number and remove from master_possibility array
void enter_number(unsigned char num, unsigned char position, unsigned char sudoku[82], unsigned char master_possibilities[10][82]) {
	unsigned char n;
	// fill in sudoku
	sudoku[position] = num;
	// remove from the master_possibilities array
	for (n = 0; n < 10; n++) {
		master_possibilities[n][position] = 0;
	}
}

// function to find all places numbers can go and enter them into sudoku
void possibility_solve(unsigned char sudoku[82], unsigned char master_possibilities[10][82]) {
	unsigned char opcode_num, n, num, position, possibilities, opcode;
	unsigned char *data;
	// define a function array for the extract functions
	unsigned char* (*extract[3])(unsigned char, unsigned char [82]) = {&extract_square, &extract_row, &extract_col};
	
	// look through all numbers
	for (num = 1; num <= 9; num++) {
		// loop through opcodes in extract function array (square, row, col)
		for(opcode = 0; opcode < 3; opcode++){
			// look through <opcode>s to find unique positions
			for (opcode_num = 0; opcode_num < 9; opcode_num++) {
				data = extract_square(opcode_num, sudoku);
				if (is_num(data, num) == 0) {
					data = extract_square(opcode_num, master_possibilities[num]);
					if (is_num(data, 1) == 1) {
						n = where_num(data, 1);
						if(opcode == 0){
							// if square: position = square starting col + square starting row + col in square + row in square
							position = ((opcode_num % 3) * 3) + ((opcode_num / 3) * 27) + (n % 3) + ((n / 3) * 9);
						}
						else{
							// if row: position = row + n, if col: position = n row + col
							position = ((opcode == 1) ? ((opcode_num * 9) + n) : ((n * 9) + opcode));
						}
						enter_number(num, position, sudoku, master_possibilities);
					}
				}
			}
		}
	}
	
	// look for positions where only one number can go
	for (position = 0; position < 81; position++) {
		// check to see if gap
		if (master_possibilities[0][position] == 1) {
			possibilities = 0;
			for (num = 1; num <= 9; num++) {
				if (master_possibilities[num][position] == 1) {
					possibilities += 1;
				}
			}
			if (possibilities == 1) {
				for (num = 1; num <= 9; num++) {
					if (master_possibilities[num][position] == 1) {
						enter_number(num, position, sudoku, master_possibilities);
						//printf("Unique position found for number %d in position %d, \n", num, position);
					}
				}
			}
		}
	}
}

// function to verify solver return 0 = correct, 1 = incorrect
void sudoku_verify(unsigned char sudoku[82]) {
	unsigned char num, n, opcode;
	// define a function array for the extract functions
	unsigned char* (*extract[3])(unsigned char, unsigned char [82]) = {&extract_square, &extract_row, &extract_col};
	
	for (num = 1; num <= 9; num++) {
		for(opcode = 0; opcode < 3; opcode++){
			// check rows
			for (n = 0; n < 9; n++) {
				if (is_num((*extract[opcode])(n, sudoku), num) > 1) {
					printf("\nError!! - More than one %d in %s %d\n", num, (opcode ? ((opcode == 1) ? "row" : "col") : "square"), n);
				}
			}
		}
	}
}

// function to solve sudoku
// returns 0 if solved otherwise returns number of gaps
unsigned char sudoku_solve(unsigned char start_sudoku[82], unsigned char end_sudoku[82], unsigned char master_possibilities[10][82]) {
	unsigned int old_poss_count, new_poss_count,n;
	unsigned char iterations = 0, old_gaps = 0, new_gaps = 0;
	unsigned char possibility_matrix[10][82];
	
	for(n = 0; n < 81; n++){
		end_sudoku[n] = start_sudoku[n];
	}

	build_master_possibilities(end_sudoku, master_possibilities);
	new_poss_count = count_possibilties(master_possibilities);
	new_gaps = count_gaps(master_possibilities);
	printf("Starting sudoku:\n");
	sudoku_print(end_sudoku);
	printf("\nStarting gaps = %d\n\n", new_gaps);
	do{
		old_gaps = new_gaps;
		old_poss_count = new_poss_count;
		update_master_possibilities(end_sudoku, master_possibilities);
		extreme_update(end_sudoku, master_possibilities);
		extreme_test(end_sudoku, master_possibilities);
		possibility_solve(end_sudoku, master_possibilities);
		
		new_gaps = count_gaps(master_possibilities);
		new_poss_count = count_possibilties(master_possibilities);
		iterations += 1;
		printf("After Iteration %d, %d gaps and %d possibilities remain\n", iterations, new_gaps, new_poss_count);
	} while (((new_gaps != old_gaps) || (new_poss_count != old_poss_count)) && (new_gaps != 0));

	printf("\nSolver Complete - ");

	if (new_gaps == 0) {
		printf("Sudoku Complete!!\n");
	}
	else {
		printf("Sudoku Incomplete!! - %d gaps remain\n", new_gaps);
	}

	printf("\nFinished sudoku:\n");
	sudoku_print(end_sudoku);
	printf("\n");

	sudoku_verify(end_sudoku);

	return new_gaps;
}
