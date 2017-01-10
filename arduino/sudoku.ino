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

//#define SUDOKU_TO_SOLVE courier_sudoku
#define SUDOKU_TO_SOLVE my_sudoku

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  unsigned long time = millis();
  sudoku_solve(SUDOKU_TO_SOLVE);
  time = millis() - time;
  Serial.print("Run time = ");
  Serial.print(time);
  Serial.println("ms\n");
}

void loop(){}
