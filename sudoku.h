/*
 * Header file for sudoku.c
 *
 */

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

//if compiling for arduino
#ifdef ARDUINO
#include <Arduino.h>
#endif

/***** FUNCTION PROTOTYPES ******/
unsigned char is_num(unsigned char data[10], unsigned char num);
unsigned char where_num(unsigned char data[10], unsigned char num, unsigned char direction = 0);
unsigned char *extract_square(unsigned char square, unsigned char sudoku[82]);
unsigned char *extract_row(unsigned char row, unsigned char sudoku[82]);
unsigned char *extract_col(unsigned char col, unsigned char sudoku[82]);
void square_print(unsigned char data[10]);
void sudoku_print(unsigned char sudoku[82]);
void sudoku_file_print(unsigned char sudoku[82], FILE *output_file);
void build_master_possibilities(unsigned char sudoku[82], unsigned char **master_possibilities);
unsigned char count_gaps(unsigned char **master_possibilities);
unsigned int count_possibilties(unsigned char **master_possibilities);
void update_master_possibilities(unsigned char sudoku[82], unsigned char **master_possibilities);
void extreme_update(unsigned char sudoku[82], unsigned char **master_possibilities);
void extreme_test(unsigned char sudoku[82], unsigned char **master_possibilities);
void enter_number(unsigned char num, unsigned char position, unsigned char sudoku[82], unsigned char **master_possibilities);
void possibility_solve(unsigned char sudoku[82], unsigned char **master_possibilities);
unsigned char sudoku_verify(unsigned char sudoku[82]);
unsigned char sudoku_solve(unsigned char start_sudoku[82], unsigned char end_sudoku[82], unsigned char **master_possibilities);
#endif
