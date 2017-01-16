/*
 * Trial and error solving functionality
 */

#include "sudoku_tande.h"
/*
// Function to determine whether it is still possible to solve the sudoku (not needed for all sudokus tried)
unsigned char solvable(unsigned char sudoku[82], unsigned char **master_possibilities){
	return 0;
}
* */

// function which returns global position based on a local position
unsigned char find_global_position(unsigned char local_position, unsigned char local_feature, unsigned char local_feature_num){
	if(local_feature == 0){
		// if square: position = square start row * 27 + square start col * 3 + square row * 9 + square col
		return ((local_feature_num/3) * 27) + ((local_feature_num % 3) * 3) + ((local_position/3) * 9) + (local_position % 3);
	}
	else if(local_feature == 1){
		// if row: position = row number * 9 + position
		return (local_feature_num * 9) + local_position;
	}
	else if(local_feature == 2){
		// if col: position = col start pos + position * 9
		return (local_feature_num) + (local_position * 9);
	}
	return 0xFF;
}

// Function to solve a sudoku by trial and error
unsigned char trial_and_error(unsigned char start_sudoku[82], unsigned char end_sudoku[82], unsigned char **master_possibilities){
	unsigned char n, m, p, num, opcode, sub_num, attempt1, attempt2;
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
								printf("Trial and error solver: ");
								#ifdef PRINT_ALL_OUTPUT
								printf("\nNumber %d has 2 possibilities in %s %d at locations %d and %d and so does number %d\n", num, (opcode ? ((opcode == 1) ? "row" : "col") : "square"), n, location[0], location[1], sub_num);
								#endif
								goto found_match;
							}
						}
					}
				}
			}
		}
	}
	printf("Error!! - no possible locations\n\n");
	return 0xFF;
	// program will continue to this point if a match is found
	// (could perhaps avoid this and store all options for future use if this doesnt work?)
	found_match:
	
	// calculate global positions from the local positions		
	location[0] = find_global_position(location[0], opcode, n);
	location[1] = find_global_position(location[1], opcode, n);
	#ifdef PRINT_ALL_OUTPUT
	printf("This equates to global positions %d and %d\n", location[0], location[1]); 
	#endif
	
	// build some new arrays to house the master_possibilities for the new solving
	unsigned char **master_possibilities_attempt1 = (unsigned char**)malloc(10*sizeof(unsigned char*));
	unsigned char **master_possibilities_attempt2 = (unsigned char**)malloc(10*sizeof(unsigned char*));
	for(p = 0; p < 10; p++){
		master_possibilities_attempt1[p] = (unsigned char*)malloc(82*sizeof(unsigned char));
		master_possibilities_attempt2[p] = (unsigned char*)malloc(82*sizeof(unsigned char));
		for(m = 0; m<81; m++){
			master_possibilities_attempt1[p][m] = master_possibilities[p][m];
			master_possibilities_attempt2[p][m] = master_possibilities[p][m];
		}
	}
	// build some new arrays for the sudokus
	unsigned char *sudoku_attempt1 = (unsigned char*) malloc(82*sizeof(unsigned char));
	unsigned char *sudoku_attempt2 = (unsigned char*) malloc(82*sizeof(unsigned char));
	for(m = 0; m < 81; m++){
		sudoku_attempt1[m] = end_sudoku[m];
		sudoku_attempt2[m] = end_sudoku[m];
	}

	// attempt to solve the sudokus with the numbers one way around and then the other
	#ifdef PRINT_ALL_OUTPUT
	printf("\nSolving sudoku with %d in %s %d position %d and %d in position %d...\n", num, (opcode ? ((opcode == 1) ? "row" : "col") : "square"), n, location[0], sub_num, location[1]);
	#endif
	enter_number(num, location[0], sudoku_attempt1, master_possibilities_attempt1);
	enter_number(sub_num, location[1], sudoku_attempt1, master_possibilities_attempt1);
	attempt1 = sudoku_solve(sudoku_attempt1, sudoku_attempt1, master_possibilities_attempt1);
	
	if(attempt1 == 0){
		for(m = 0; m < 81; m++){
			end_sudoku[m] = sudoku_attempt1[m];
			
		}
		#ifdef PRINT_ALL_OUTPUT
		sudoku_print(sudoku_attempt1);
		#endif
		return 0;
	}
	else{
		#ifdef PRINT_ALL_OUTPUT
		printf("\nSolving sudoku with %d in %s %d position %d and %d in position %d...\n", num, (opcode ? ((opcode == 1) ? "row" : "col") : "square"), n, location[1], sub_num, location[0]);
		#endif
		enter_number(num, location[1], sudoku_attempt2, master_possibilities_attempt2);
		enter_number(sub_num, location[0], sudoku_attempt2, master_possibilities_attempt2);
		attempt2 = sudoku_solve(sudoku_attempt2, sudoku_attempt2, master_possibilities_attempt2);
		if(attempt2 == 0){
			for(m = 0; m < 81; m++){
				end_sudoku[m] = sudoku_attempt2[m];
				
			}
			#ifdef PRINT_ALL_OUTPUT
			sudoku_print(sudoku_attempt2);
			#endif
			return 0;
		}
	}
	
	// need to determine if still possible to solve if neither works
	if(attempt1 && attempt2){
		if(attempt1 == 0xFF){
			for(m = 0; m < 81; m++){
				end_sudoku[m] = sudoku_attempt2[m];
				for(p = 0; p <10; p++){
					master_possibilities[p][m] = master_possibilities_attempt2[p][m];
				}
			}
			return attempt2;
		}
		else if(attempt2 == 0xFF){
			for(m = 0; m < 81; m++){
				end_sudoku[m] = sudoku_attempt1[m];
				for(p = 0; p <10; p++){
					master_possibilities[p][m] = master_possibilities_attempt1[p][m];
				}
			}
			return attempt1;
		}
		return 0xFF;
	}
	

		
	// if can't solve and it still possible to complete then either run this function again (recursively?) with two new pairs or try a different pair?
	return 0;
}

unsigned char sudoku_solve_tande(unsigned char input_sudoku[82], unsigned char output_sudoku[82]) {
	unsigned char n, old_gaps, new_gaps;
	#ifdef PRINT_ALL_OUTPUT
	printf("\nJames' Sudoku Solving Program Version 3\n\n");
	#endif
	
	// unsigned char output_sudoku[82];	
	unsigned char **possibility_matrix = (unsigned char**) malloc(10*sizeof(unsigned char*));
	for(n = 0; n < 10; n++){
		possibility_matrix[n] = (unsigned char*) malloc(82*sizeof(unsigned char));
	}
	
	build_master_possibilities(input_sudoku, possibility_matrix);
	#ifdef PRINT_ALL_OUTPUT
	printf("Starting sudoku:\n");
	sudoku_print(input_sudoku);	
	printf("\nStarting standard solver...\n");
	#endif
	new_gaps = sudoku_solve(input_sudoku, output_sudoku, possibility_matrix);
	if(new_gaps){
		#ifdef PRINT_ALL_OUTPUT
		printf("\n");
		#endif
		//trial_and_error(output_sudoku, output_sudoku, possibility_matrix);
		
		do{
			old_gaps = new_gaps;
			new_gaps = trial_and_error(output_sudoku, output_sudoku, possibility_matrix);
			if(new_gaps == 0xFF){
				return 0xFF;
			}
		} while((old_gaps != new_gaps) && (new_gaps != 0));
		
	}
	#ifdef PRINT_ALL_OUTPUT
	printf("\nFinal sudoku:\n");
	sudoku_print(output_sudoku);
	printf("\n");
	#endif

	// wait till key press (for windows)
	// getchar();

	return 0;
}
