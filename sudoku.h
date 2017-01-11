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

// easy sudoku from 1st march metro
static unsigned char metro_sudoku_easy[82] = {
  5,0,7,  0,8,1,  0,0,0,
  0,1,0,  0,0,6,  7,0,5,
  0,2,0,  7,0,4,  0,3,0,

  0,0,5,  0,2,0,  0,0,9,
  8,0,2,  0,0,0,  1,0,7,
  1,0,0,  5,9,0,  6,0,0,

  0,6,0,  8,0,5,  0,4,0,
  2,8,1,  3,0,0,  0,7,6,
  0,0,0,  0,7,0,  9,0,8
};

// moderate sudoku from 1st march metro
static unsigned char metro_sudoku_moderate[82] = {
  5,8,0,  0,0,4,  0,9,0,
  0,0,6,  0,9,0,  0,0,0,
  0,0,9,  0,0,1,  0,0,4,

  0,6,4,  0,5,0,  8,3,0,
  7,0,0,  8,0,3,  0,0,1,
  0,9,8,  0,4,0,  6,0,0,

  6,0,0,  3,0,0,  7,0,9,
  0,0,0,  0,1,0,  3,0,0,
  0,1,0,  9,0,0,  0,5,2
};

// challenging sudoku from 1st march metro
static unsigned char metro_sudoku_challenging[82] = {
  0,0,0,  0,0,0,  0,1,0,
  4,0,0,  0,3,0,  9,0,0,
  0,7,0,  9,0,2,  6,0,0,

  2,5,0,  0,0,6,  0,0,3,
  0,9,0,  0,0,0,  0,7,0,
  6,0,0,  5,0,0,  0,9,1,

  0,0,8,  3,0,4,  0,5,0,
  0,0,3,  0,5,0,  0,0,9,
  0,4,0,  0,0,0,  0,0,0
};

// challenging sudoku from 21st April metro
static unsigned char metro_sudoku_challenging_2[82] = {
  0,0,0,  0,2,0,  0,0,0,
  0,0,9,  6,0,4,  2,0,0,
  5,0,0,  3,0,8,  0,0,6,
  
  0,9,8,  0,0,0,  7,1,0,
  0,3,0,  0,0,0,  0,9,0,
  0,7,1,  0,0,0,  6,3,0,
  
  8,0,0,  2,0,5,  0,0,7,
  0,0,7,  1,0,6,  4,0,0,
  0,0,0,  0,3,0,  0,0,0,
};

// sudoku from the courier 29th feb
static unsigned char courier_sudoku[82] = {
  
  0,2,0,  0,0,8,  0,0,0,
  0,0,7,  0,0,2,  3,0,1,
  0,0,5,  1,0,0,  0,0,0,

  0,9,0,  0,0,0,  0,8,7,
  8,0,2,  0,1,0,  0,0,3,
  4,7,0,  0,0,0,  0,2,0,

  0,0,0,  0,0,3,  7,0,0,
  7,0,6,  4,0,0,  9,0,0,
  0,0,0,  6,0,0,  0,4,0
};

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
