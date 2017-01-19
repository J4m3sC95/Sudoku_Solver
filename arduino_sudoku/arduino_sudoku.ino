/*
 * This sketch has solved a sudoku in 214ms if serial baudrate is set to 230400 on arduino mega
 * - the desktop version runs the same sudoku in about 5ms therefore it is 42.8 times slower on arduino
 * 
 * This sketch will solve "mysudoku"
  */

// enter your own sudoku here:
static unsigned char my_sudoku[82] = {
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

#include "sudoku.h"

#define SUDOKU_TO_SOLVE my_sudoku

unsigned char master_possibilities[10][82];
unsigned char *master_possibilities_ptr[] = {master_possibilities[0], master_possibilities[1], master_possibilities[2], master_possibilities[3],
master_possibilities[4],master_possibilities[5], master_possibilities[6], master_possibilities[7], master_possibilities[8], master_possibilities[9]
};

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  unsigned long time = millis();
  sudoku_solve(SUDOKU_TO_SOLVE, SUDOKU_TO_SOLVE, master_possibilities_ptr);
  time = millis() - time;
  Serial.print("Run time = ");
  Serial.print(time);
  Serial.println("ms\n");
}

void loop(){}
