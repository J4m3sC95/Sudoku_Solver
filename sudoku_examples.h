/*
 * Header file which contains example sudokus to solve
 *
 */

#ifndef SUDOKU_EXAMPLES_H
#define SUDOKU_EXAMPLES_H

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

#endif
