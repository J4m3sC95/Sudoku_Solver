/*
 * Header file for sudoku_tande.cpp
 *
 */

#ifndef SUDOKU_TANDE_H
#define SUDOKU_TANDE_H

#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
//#include "sudoku_examples.h"

//#define SUDOKU_TO_SOLVE very_difficult

//#define PRINT_ALL_OUTPUT

// function prototypes
//unsigned char solvable(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
unsigned char find_global_position(unsigned char local_position, unsigned char local_feature, unsigned char local_feature_num);
unsigned char trial_and_error(unsigned start_sudoku[82], unsigned char end_sudoku[82], unsigned char master_possibilities[10][82]);
unsigned char sudoku_solve_tande(unsigned char input_sudoku[82], unsigned char output_sudoku[82]);

#endif
