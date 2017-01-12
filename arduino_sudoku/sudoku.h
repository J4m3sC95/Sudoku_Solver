/*
 * Header file for sudoku.c
 *
 */

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <ctype.h>

//if compiling for arduino
#ifdef ARDUINO
#include <Arduino.h>
#endif

/***** FUNCTION PROTOTYPES ******/
unsigned char is_num(unsigned char data[10], unsigned char num);
unsigned char where_num(unsigned char data[10], unsigned char num);
unsigned char *extract_square(unsigned char square, unsigned char sudoku[82]);
unsigned char *extract_row(unsigned char row, unsigned char sudoku[82]);
unsigned char *extract_col(unsigned char col, unsigned char sudoku[82]);
void square_print(unsigned char data[10]);
void sudoku_print(unsigned char sudoku[82]);
void sudoku_file_print(unsigned char sudoku[82], FILE *output_file);
void build_master_possibilities(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
unsigned char count_gaps(unsigned char master_possibilities[10][82]);
unsigned int count_possibilties(unsigned char master_possibilities[10][82]);
void update_master_possibilities(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
void extreme_update(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
void extreme_test(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
void enter_number(unsigned char num, unsigned char position, unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
void possibility_solve(unsigned char sudoku[82], unsigned char master_possibilities[10][82]);
void sudoku_verify(unsigned char sudoku[82]);
unsigned char sudoku_solve(unsigned char start_sudoku[82], unsigned char end_sudoku[82], unsigned char master_possibilities[10][82]);
#endif
