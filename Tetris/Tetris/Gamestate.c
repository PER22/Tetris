#include "Gamestate.h"
#include <stdlib.h>

const signed char pieceShapes[5][4][2]={
	
	{{0,0},{1,0},{1,1},{0,1}}, //square
	{{0,-1},{0,0},{0,1},{0,2}}, //line
	{{0,0},{0,-1},{0,-2},{1,0}}, //L-shape
	{{0,0},{0,-1},{0,1},{1,0}}, //triangle
	{{0,0},{0,-1},{1,0},{1,1}}//bent
};

void checkDownAndDo(Tetromino* active, Gameboard* inactive){
	for(int i = 0; i < 4; i++){
		if(
		   (active->coordinates[i][0] + active->x_coordinate )     > 7
		|| (active->coordinates[i][0] + active->x_coordinate )     < 0
		|| (active->coordinates[i][1] + active->y_coordinate + 1)  > 15
		|| (active->coordinates[i][1] + active->y_coordinate + 1)  < 0 ){
			return; //fail
		}
		//check gameboard
		else if(inactive->board[active->coordinates[i][0] + active->x_coordinate][active->coordinates[i][1] + active->y_coordinate + 1]){
			return;//fail
		}
	}
	//copy
	active->y_coordinate++;
}
//user manipulation
void checkRotateAndDo(Tetromino* active, Gameboard* inactive){
	//do temp rotate that might be illegal
	signed char tempActive[4][2];
	for(int i = 0; i < 4; i++){
		tempActive[i][0] =  active->coordinates[i][1];
		tempActive[i][1] = -1 * active->coordinates[i][0];
		//check perimeter
		if(tempActive[i][0] + active->x_coordinate > 7//fix 
		|| tempActive[i][0] + active->x_coordinate < 0 
		|| tempActive[i][1] + active->y_coordinate > 15  
		|| tempActive[i][1] + active->y_coordinate < 0){
			return; //fail
		} 
		//check gameboard
		else if(inactive->board[tempActive[i][0]+ active->x_coordinate][tempActive[i][1]+ active->y_coordinate]){
			return;//fail
		}
	}
	//passed checks, go ahead and	
	//assign new rotated coordinates.
	for(int i = 0; i < 4; i++){
		active->coordinates[i][0] = tempActive[i][0];
		active->coordinates[i][1] = tempActive[i][1];
	}
	
}

void checkLeftAndDo(Tetromino* active, Gameboard* inactive){
		for(int i = 0; i < 4; i++){
			if(
			   (active->coordinates[i][0] + active->x_coordinate -1)> 7
			|| (active->coordinates[i][0] + active->x_coordinate -1)< 0
			|| (active->coordinates[i][1] + active->y_coordinate )  > 15
			|| (active->coordinates[i][1] + active->y_coordinate )  < 0 ){
				return; //fail
			}
			//check gameboard
			else if(inactive->board[active->coordinates[i][0] + active->x_coordinate - 1][active->coordinates[i][1] + active->y_coordinate]){
				return;//fail
			}
		}
		//copy
		active->x_coordinate--;
}
	
void checkRightAndDo(Tetromino* active, Gameboard* inactive){
		for(int i = 0; i < 4; i++){
			if(
			(active->coordinates[i][0] + active->x_coordinate + 1)    > 7
			|| (active->coordinates[i][0] + active->x_coordinate + 1) < 0
			|| (active->coordinates[i][1] + active->y_coordinate)  > 15
			|| (active->coordinates[i][1] + active->y_coordinate)  < 0 ){
				return; //fail
			}
			//check gameboard
			else if(inactive->board[active->coordinates[i][0] + active->x_coordinate + 1][active->coordinates[i][1] + active->y_coordinate]){
				return;//fail
			}
		}
		//copy
		active->x_coordinate++;
}

int checkLoss(Gameboard* inactive){
	for(int i = 0; i < 8; i++){
		if(inactive->board[i][0]){return 1;}
	}
	return 0;
	
}

//once touched
void convertPieceToInactive(Tetromino* active, Gameboard* inactive){
	for(int i = 0; i < 4; i ++){
		inactive->board[active->coordinates[i][0]+ active->x_coordinate][active->coordinates[i][1]+ active->y_coordinate] = 1;
	}
}

int deletedAFilledRowAndSlidDown(Gameboard* inactive){
	unsigned char rowDeleteFlag;
	for (int i = 15; i>= 0; i--){
			rowDeleteFlag = 1;
			for(int j = 0; j < 8; j++){
				if(!(inactive->board[j][i])){rowDeleteFlag = 0;}
			}
			if(rowDeleteFlag){
				for(int k = i; k > 0; k--){
					for(int m = 0; m < 8; m++){
						inactive->board[m][k]=inactive->board[m][k-1];
					}
				}
				for(int k = 0; k < 8; k++){
					inactive->board[k][0] = 0;	
				}
				return 1;
			}
	}
	return 0;
}

//for display
void combinePieceAndBoardIntoImage(	RGB_8x16_Frame* next_frame_ptr,
									Tetromino* piece, Gameboard* board){
	//clear frame
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 16; j++){
			next_frame_ptr->frame[i][j] = 0;
		}	
	}
	//put 4 active tetromino squares in using GREEN
	for (int i = 0; i <4; i++){
		next_frame_ptr->frame[piece->coordinates[i][0] + piece->x_coordinate][piece->coordinates[i][1] + piece->y_coordinate] |= (1 << RGB_GREEN_BIT);		
	}
	//put all 8*16 inactive values in using BLUE
	for (int i = 0; i < 8; i++){
		for(int j = 0; j < 16; j++){
			if(board->board[i][j]){next_frame_ptr->frame[i][j] |= (1 << RGB_BLUE_BIT);}
		}
	}
}


Tetromino* createTetromino(unsigned char type){
	Tetromino* tet = (Tetromino*)malloc(sizeof(Tetromino));
	for(int i =0; i< 4;i++ ){
		for(int j =0; j< 2;j++ ){
			tet->coordinates[i][j] = pieceShapes[type][i][j];
		}
	}
	switch(type){
		case 0:
			tet->x_coordinate = 3;
			tet->y_coordinate = 0;
			break;
		
		case 1:
			tet->x_coordinate = 3;
			tet->y_coordinate = 2;
		break;
		
		case 2:
		tet->x_coordinate = 3;
		tet->y_coordinate = 2;
		break;
		
		case 3:
		tet->x_coordinate = 3;
		tet->y_coordinate = 1;
		break;
		
		case 4:
		tet->x_coordinate = 3;
		tet->y_coordinate = 1;
		break;
		
		default:
		tet->x_coordinate = 2;
		tet->y_coordinate = 3;
		break;
		
		
	}
	return tet;
}

void losingDisplay(RGB_8x16_Frame* next_frame_ptr){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 16; j++){
			next_frame_ptr->frame[i][j] = (1 << RGB_RED_BIT);
		}
	}
}






