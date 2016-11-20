/*
 * Sudoku solving functions
 */

#include "sudoku.h"

// if compiling for arduino
#ifdef ARDUINO
// buffer to store printf data in for macro to avoid rewriting all printfs to serial prints
char buffer[100];
#define printf(fmt,...) sprintf(buffer, fmt, ##__VA_ARGS__); Serial.print(buffer);
unsigned int old_poss_count, new_poss_count;

//if not compiling for arduino
#else

// easy sudoku from 1st march metro
unsigned char metro_sudoku_easy[82] = {
	5,0,7,	0,8,1,	0,0,0,
	0,1,0,	0,0,6,	7,0,5,
	0,2,0,	7,0,4,	0,3,0,

	0,0,5,	0,2,0,	0,0,9,
	8,0,2,	0,0,0,	1,0,7,
	1,0,0,	5,9,0,	6,0,0,

	0,6,0,	8,0,5,	0,4,0,
	2,8,1,	3,0,0,	0,7,6,
	0,0,0,	0,7,0,	9,0,8
};

// moderate sudoku from 1st march metro
unsigned char metro_sudoku_moderate[82] = {
	5,8,0,	0,0,4,	0,9,0,
	0,0,6,	0,9,0,	0,0,0,
	0,0,9,	0,0,1,	0,0,4,

	0,6,4,	0,5,0,	8,3,0,
	7,0,0,	8,0,3,	0,0,1,
	0,9,8,	0,4,0,	6,0,0,

	6,0,0,	3,0,0,	7,0,9,
	0,0,0,	0,1,0,	3,0,0,
	0,1,0,	9,0,0,	0,5,2
};

// challenging sudoku from 1st march metro
unsigned char metro_sudoku_challenging[82] = {
	0,0,0,	0,0,0,	0,1,0,
	4,0,0,	0,3,0,	9,0,0,
	0,7,0,	9,0,2,	6,0,0,

	2,5,0,	0,0,6,	0,0,3,
	0,9,0,	0,0,0,	0,7,0,
	6,0,0,	5,0,0,	0,9,1,

	0,0,8,	3,0,4,	0,5,0,
	0,0,3,	0,5,0,	0,0,9,
	0,4,0,	0,0,0,	0,0,0
};

// challenging sudoku from 21st April metro
unsigned char metro_sudoku_challenging_2[82] = {
	0,0,0,	0,2,0,	0,0,0,
	0,0,9,	6,0,4,	2,0,0,
	5,0,0,	3,0,8,	0,0,6,
	
	0,9,8,	0,0,0,	7,1,0,
	0,3,0,	0,0,0,	0,9,0,
	0,7,1,	0,0,0,	6,3,0,
	
	8,0,0,	2,0,5,	0,0,7,
	0,0,7,	1,0,6,	4,0,0,
	0,0,0,	0,3,0,	0,0,0,
};

// sudoku from the courier 29th feb
unsigned char courier_sudoku[82] = {
	
	0,2,0,	0,0,8,	0,0,0,
	0,0,7,	0,0,2,	3,0,1,
	0,0,5,	1,0,0,	0,0,0,

	0,9,0,	0,0,0,	0,8,7,
	8,0,2,	0,1,0,	0,0,3,
	4,7,0,	0,0,0,	0,2,0,

	0,0,0,	0,0,3,	7,0,0,
	7,0,6,	4,0,0,	9,0,0,
	0,0,0,	6,0,0,	0,4,0
};

#endif

// function which determines if a number is in a 9 element array (row/col/square)
// 0 = not there, if present, a count will be returned
unsigned char is_num(unsigned char data[10], unsigned char num) {
	unsigned char n;
	unsigned char count = 0;
	for (n = 0; n < 9; n++) {
		if (data[n] == num) {
			count += 1;
		}
	}
	return count;
}

// function to find number in 9 element array (row/col/square)
// 0 if not there, otherwise returns location
unsigned char where_num(unsigned char data[10], unsigned char num) {
	unsigned char n;
	for (n = 0; n < 9; n++) {
		if (data[n] == num) {
			return n;
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
	unsigned char n;
	printf("\n");
	for (n = 0; n < 3; n++) {
		printf("%d, ", data[n]);
	}
	printf("\n");
	for (n = 3; n < 6; n++) {
		printf("%d, ", data[n]);
	}
	printf("\n");
	for (n = 6; n < 9; n++) {
		printf("%d, ", data[n]);
	}
	printf("\n");
}

// function to print entire sudoku
void sudoku_print(unsigned char sudoku[82]) {
	unsigned char n, m, p;
	printf("\n");
	for (n = 0; n < 3; n++) {
		for (m = 0; m < 3; m++) {
			for (p = 0; p < 3; p++) {
				printf("%d, ", sudoku[(n * 9) + (m * 3) + p]);
			}
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
	for (n = 3; n < 6; n++) {
		for (m = 0; m < 3; m++) {
			for (p = 0; p < 3; p++) {
				printf("%d, ", sudoku[(n * 9) + (m * 3) + p]);
			}
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
	for (n = 6; n < 9; n++) {
		for (m = 0; m < 3; m++) {
			for (p = 0; p < 3; p++) {
				printf("%d, ", sudoku[(n * 9) + (m * 3) + p]);
			}
			printf("\t");
		}
		printf("\n");
	}
}

// function to print entire sudoku to file
void sudoku_file_print(unsigned char sudoku[82], FILE *output_file) {
	unsigned char n, m, p;
	fprintf(output_file, "\n");
	for (n = 0; n < 3; n++) {
		for (m = 0; m < 3; m++) {
			for (p = 0; p < 3; p++) {
				fprintf(output_file, "%d, ", sudoku[(n * 9) + (m * 3) + p]);
			}
			fprintf(output_file, "\t");
		}
		fprintf(output_file, "\n");
	}
	fprintf(output_file, "\n");
	for (n = 3; n < 6; n++) {
		for (m = 0; m < 3; m++) {
			for (p = 0; p < 3; p++) {
				fprintf(output_file, "%d, ", sudoku[(n * 9) + (m * 3) + p]);
			}
			fprintf(output_file, "\t");
		}
		fprintf(output_file, "\n");
	}
	fprintf(output_file, "\n");
	for (n = 6; n < 9; n++) {
		for (m = 0; m < 3; m++) {
			for (p = 0; p < 3; p++) {
				fprintf(output_file, "%d, ", sudoku[(n * 9) + (m * 3) + p]);
			}
			fprintf(output_file, "\t");
		}
		fprintf(output_file, "\n");
	}
}

// build array of arrays to hold all possibilities
// first array is gap array (1 = gap, 0 = no gap)
// subsequent arrays are possibilities for each number (0 = not possible, 1 = possible)
unsigned char master_possibilities[10][82];
void build_master_possibilities(unsigned char sudoku[82]) {
	unsigned char n,m;
	for (n = 0; n < 10; n++) {
		for (m = 0; m < 81; m++) {
			master_possibilities[n][m] = 0;
		}
	}
	// find gaps
	for (n = 0; n < 81; n++) {
		// position = square starting column + square starting row + current row + current col
		if (sudoku[n] == 0) {
			for (m = 0; m < 10; m++) {
				master_possibilities[m][n] = 1;
			}
		}
	}
}

// function to count the remaining gaps
unsigned char count_gaps(void) {
	unsigned char n;
	unsigned char gaps = 0;
	for (n = 0; n < 81; n++) {
		if (master_possibilities[0][n] == 1) {
			gaps += 1;
		}
	}
	return gaps;
}

// function to count the remaining possibilities
unsigned int count_possibilties(void){
	unsigned char n, m;
	unsigned int count = 0;
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
void update_master_possibilities(unsigned char sudoku[82]) {
	unsigned char n,m, square_num, row, col, num, position, row_line_up, col_line_up;
	unsigned char position1 = 0;
	unsigned char position2 = 0;
	unsigned char position3 = 0;
	// cycle through numbers to eliminate stuff
	for (num = 1; num <= 9; num++) {
		// eliminate based on squares
		for (square_num = 0; square_num < 9; square_num++) {
			unsigned char *data = extract_square(square_num, sudoku);
			// does it already exist here??
			if (is_num(data, num) == 0) {
				// eliminate possibilities by row
				for (n = 0; n < 3; n++) {
					row = ((square_num / 3) * 3) + n;
					// check to see if number in that row and eliminate that row from possibility if so
					if (is_num(extract_row(row, sudoku), num) == 1) {
						for (m = 0; m < 3; m++) {
							// position = square starting column + current row + m
							position = ((square_num % 3) * 3) + (row * 9) + m;
							master_possibilities[num][position] = 0;
						}
					}
				}
				// eliminate possibilities by column
				for (n = 0; n < 3; n++) {
					col = ((square_num % 3) * 3) + n;
					// check to see if number in that col and eliminate that col from possibility if so
					if (is_num(extract_col(col, sudoku), num) == 1) {
						for (m = 0; m < 3; m++) {
							// position = square starting row + current column + m time row
							position = ((square_num / 3) * 27) + col + (m * 9);
							master_possibilities[num][position] = 0;
						}
					}
				}

				// eliminate possibilities in other squares based on 2 or 3 possibilities lining up in this square
				unsigned char *possibility_data = extract_square(square_num, master_possibilities[num]);
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
					row_line_up = ((position1 / 3) == (position2 / 3)) & ((position2 / 3) == (position3 / 3));
					col_line_up = ((position1 % 3) == (position2 % 3))&((position2 % 3) == (position3 % 3));
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
void extreme_update(unsigned char sudoku[82]) {
	unsigned char n, m, square, row, col, p, q, position, num;
	unsigned char count = 0;
	unsigned char row_count = 0;
	unsigned char col_count = 0;
	unsigned char zero_count = 0;
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
								while ((squares[2] == squares[0]) | (squares[2] == squares[1])) {
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
								while ((squares[2] == squares[0]) | (squares[2] == squares[1])) {
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

// fucntion to test some more methods for extreme_update
void extreme_test(unsigned char sudoku[82]) {
	/*** IF x (2 OR MORE) NUMBERS SHARE THE SAME x POSSIBILITIES THEN NO OTHER NUMBER CAN BE PUT IN THESE POSSIBILITIES ***/
	// this happens for numbers 3 and 8 in square 3 of the challenging metro sudoku
	unsigned char square, num, n, m, p, match, position;
	// cycle through squares
	for (square = 0; square < 9; square++) {
		unsigned char index = 0;
		unsigned char two_poss_count = 0;
		unsigned char two_poss[10] = { 0,0,0,0,0,0,0,0,0 };
		unsigned char locations[3] = { 0,0 };
		unsigned char *data;
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
							if ((num != two_poss[n]) & (num != two_poss[n + m])) {
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
void enter_number(unsigned char num, unsigned char position, unsigned char sudoku[82]) {
	// fill in sudoku
	sudoku[position] = num;
	// remove from the master_possibilities array
	unsigned char n;
	for (n = 0; n < 10; n++) {
		master_possibilities[n][position] = 0;
	}
}

// function to find all places numbers can go and enter them into sudoku
void possibility_solve(unsigned char sudoku[82]) {
	unsigned char square_num, n, num, position, row, col, possibilities;
	unsigned char *data;
	// look through all numbers
	for (num = 1; num <= 9; num++) {
		//printf("possibilities for %d", num);
		//sudoku_print(master_possibilities[num]);
		// look through squares to find unique positions
		for (square_num = 0; square_num < 9; square_num++) {
			data = extract_square(square_num, sudoku);
			if (is_num(data, num) == 0) {
				data = extract_square(square_num, master_possibilities[num]);
				if (is_num(data, 1) == 1) {
					n = where_num(data, 1);
					// position = square starting col + square starting row + col in square + row in square
					position = ((square_num % 3) * 3) + ((square_num / 3) * 27) + (n % 3) + ((n / 3) * 9);
					enter_number(num, position, sudoku);
					//printf("Unique position found for number %d in square %d position %d, global position %d\n", num, square_num, n, position);
				}
			}
		}
		
		// look through rows to find unique positions
		for (row = 0; row < 9; row++) {
			data = extract_row(row, sudoku);
			if (is_num(data, num) == 0) {
				data = extract_row(row, master_possibilities[num]);
				if (is_num(data, 1) == 1) {
					n = where_num(data, 1);
					// position = row + n
					position = (row * 9) + n;
					enter_number(num, position, sudoku);
					//printf("Unique position found for number %d in row %d position %d, global position %d\n", num, row, n, position);
				}
			}
		}
				
		// look through cols to find unique positions
		for (col = 0; col < 9; col++) {
			data = extract_col(col, sudoku);
			if (is_num(data, num) == 0) {
				data = extract_col(col, master_possibilities[num]);
				if (is_num(data, 1) == 1) {
					n = where_num(data, 1);
					// position = n row + col
					position = (n * 9) + col;
					enter_number(num, position, sudoku);
					//printf("Unique position found for number %d in column %d position %d, global position %d\n", num, col, n, position);
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
						enter_number(num, position, sudoku);
						//printf("Unique position found for number %d in position %d, \n", num, position);
					}
				}
			}
		}
	}
}

// function to verify solver return 0 = correct, 1 = incorrect
void sudoku_verify(unsigned char sudoku[82]) {
	unsigned char num, n;
	for (num = 1; num <= 9; num++) {
		// check rows
		for (n = 0; n < 9; n++) {
			if (is_num(extract_row(n, sudoku), num) > 1) {
        #ifdef ARDUINO_AVR_UNO
        printf("Error!!");
        #else
				printf("\nError!! - More than one %d in row %d\n", num, n);
        #endif
			}
		}
		// check columns
		for (n = 0; n < 9; n++) {
			if (is_num(extract_col(n, sudoku), num) > 1) {
        #ifdef ARDUINO_AVR_UNO
				printf("Error!!");
        #else
				printf("\nError!! - More than one %d in col %d\n", num, n);
        #endif
			}
		}
		// check sqaures
		for (n = 0; n < 9; n++) {
			if (is_num(extract_square(n, sudoku), num) > 1) {
        #ifdef ARDUINO_AVR_UNO
				printf("Error!!");
        #else
				printf("\nError!! - More than one %d in square %d\n", num, n);
        #endif
			}
		}
	}
}

// function to solve sudoku
// returns 0 if solved otherwise returns number of gaps
unsigned char sudoku_solve(unsigned char sudoku[82]) {
	unsigned char iterations = 0;
	unsigned char old_gaps = 0;

	build_master_possibilities(sudoku);
	new_poss_count = count_possibilties();
	unsigned char new_gaps = count_gaps();
	printf("Starting sudoku:\n");
	sudoku_print(sudoku);
	printf("\nStarting gaps = %d\n\n", new_gaps);
	while (((new_gaps != old_gaps)|(new_poss_count != old_poss_count)) && (new_gaps != 0)) {
		old_gaps = new_gaps;
		old_poss_count = new_poss_count;
		update_master_possibilities(sudoku);
		extreme_update(sudoku);
		extreme_test(sudoku);
		possibility_solve(sudoku);
		
		new_gaps = count_gaps();
		new_poss_count = count_possibilties();
		iterations += 1;
		printf("After Iteration %d, %d gaps and %d possibilities remain\n", iterations, new_gaps, new_poss_count);
	}

	printf("\nSolver Complete - ");

	if (new_gaps == 0) {
		printf("Sudoku Complete!!\n");
	}
	else {
		printf("Sudoku Incomplete!! - %d gaps remain\n", new_gaps);
	}

	printf("\nFinished sudoku:\n");
	sudoku_print(sudoku);
	printf("\n");

	sudoku_verify(sudoku);

	return new_gaps;
}
