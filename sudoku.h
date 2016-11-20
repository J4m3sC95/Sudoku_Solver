/*
 * Header file for sudoku.c
 *
 */

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <ctype.h>

unsigned int old_poss_count, new_poss_count;

unsigned char master_possibilities[10][82];

// sudokus
unsigned char metro_sudoku_easy[82];
unsigned char metro_sudoku_moderate[82];
unsigned char metro_sudoku_challenging[82];
unsigned char metro_sudoku_challenging_2[82];
unsigned char courier_sudoku[82];

/***** FUNCTION PROTOTYPES ******/
unsigned char is_num(unsigned char data[10], unsigned char num);
unsigned char where_num(unsigned char data[10], unsigned char num);
unsigned char *extract_square(unsigned char square, unsigned char sudoku[82]);
unsigned char *extract_row(unsigned char row, unsigned char sudoku[82]);
unsigned char *extract_col(unsigned char col, unsigned char sudoku[82]);
void square_print(unsigned char data[10]);
void sudoku_print(unsigned char sudoku[82]);
void sudoku_file_print(unsigned char sudoku[82], FILE *output_file);
void build_master_possibilities(unsigned char sudoku[82]);
unsigned char count_gaps(void);
unsigned int count_possibilties(void);
void update_master_possibilities(unsigned char sudoku[82]);
void extreme_update(unsigned char sudoku[82]);
void extreme_test(unsigned char sudoku[82]);
void enter_number(unsigned char num, unsigned char position, unsigned char sudoku[82]);
void possibility_solve(unsigned char sudoku[82]);
void sudoku_verify(unsigned char sudoku[82]);
unsigned char sudoku_solve(unsigned char sudoku[82]);

#endif
