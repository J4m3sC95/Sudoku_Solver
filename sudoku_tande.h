/*
 * Header file for sudoku.c
 *
 */

#ifndef SUDOKU_TANDE_H
#define SUDOKU_TANDE_H

#include <stdio.h>
#include "sudoku.h"
#include "sudoku_examples.h"

#define SUDOKU_TO_SOLVE metro_sudoku_challenging

// function prototypes
unsigned char solvable(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
unsigned char trial_and_error(unsigned start_sudoku[82], unsigned char end_sudoku[82], unsigned char master_possibilities[10][82]);

#endif
