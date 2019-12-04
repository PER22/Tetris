

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "LED_Matrix.h"

#define TETROMINO_INDEX_SQUARE 0
#define TETROMINO_INDEX_LINE 1
#define TETROMINO_INDEX_L_SHAPE 2
#define TETROMINO_INDEX_TRIANGLE 3
#define TETROMINO_INDEX_BENT 4

typedef struct Tetromino{
	signed char coordinates[4][2];
	unsigned char x_coordinate;
	unsigned char y_coordinate;	
}Tetromino;

typedef struct Gameboard{
	unsigned char board[8][16];
}Gameboard;


//natural behavior
void checkDownAndDo(Tetromino* active, Gameboard* inactive);
//user manipulation 
void checkRotateAndDo(Tetromino* active, Gameboard* inactive);
void checkLeftAndDo(Tetromino* active, Gameboard* inactive);
void checkRightAndDo(Tetromino* active, Gameboard* inactive);
//when at bottom
void convertPieceToInactive(Tetromino* active, Gameboard* inactive);

void combinePieceAndBoardIntoImage(RGB_8x16_Frame* next_frame_ptr, Tetromino* piece, Gameboard* board);
int checkLoss(Gameboard* inactive);

#endif 