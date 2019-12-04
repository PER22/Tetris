#include "Gamestate.h"
#include <stdlib.h>

const signed char pieceShapes[5][4][2]={
	
	{{0,0},{1,0},{1,1},{0,1}}, //square
	{{0,0},{0,1},{0,2},{0,3}}, //line
	{{0,0},{0,-1},{0,-2},{1,0}}, //L-shape
	{{0,0},{0,-1},{0,1},{1,0}}, //triangle
	{{0,0},{0,-1},{1,0},{1,1}}
};//bent

void checkDownAndDo(Tetromino* active, Gameboard* inactive){
	signed char tempActive[4][2];
	for(int i = 0; i < 4; i++){
		tempActive[i][0] = active->coordinates[i][0];
		tempActive[i][1] = active->coordinates[i][1] + 1;
		
		if(tempActive[i][0] + active->x_coordinate > 7
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
	//copy
	for(int i = 0; i < 4; i++){
		active->coordinates[i][0] = tempActive[i][0];
		active->coordinates[i][1] = tempActive[i][1];
	}
}
//user manipulation
void checkRotateAndDo(Tetromino* active, Gameboard* inactive){
	//do temp rotate that might be illegal
	signed char tempActive[4][2];
	for(int i = 0; i < 4; i++){
		tempActive[i][0] = -1 * active->coordinates[i][1];
		tempActive[i][1] = active->coordinates[i][0];
		//check perimeter
		if(tempActive[i][0] + active->x_coordinate > 7 
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
	signed char tempActive[4][2];
	for(int i = 0; i < 4; i++){
		tempActive[i][0] = active->coordinates[i][0] - 1;
		tempActive[i][1] = active->coordinates[i][1];
		
		if(tempActive[i][0] + active->x_coordinate > 7
		|| tempActive[i][0] + active->x_coordinate < 0
		|| tempActive[i][1] + active->y_coordinate > 15
		|| tempActive[i][1] + active->y_coordinate < 0){
			return; //fail
		}
		//check gameboard
		else if(inactive->board[tempActive[i][0] + active->x_coordinate][tempActive[i][1]+ active->y_coordinate]){
			return;//fail
		}
	}
	for(int i = 0; i < 4; i++){
		active->coordinates[i][0] = tempActive[i][0];
		active->coordinates[i][1] = tempActive[i][1];
	}	
}
	
void checkRightAndDo(Tetromino* active, Gameboard* inactive){
	signed char tempActive[4][2];
	for(int i = 0; i < 4; i++){
		tempActive[i][0] = active->coordinates[i][0] + 1;
		tempActive[i][1] = active->coordinates[i][1];
			
		if(tempActive[i][0] + active->x_coordinate > 7
		|| tempActive[i][0] + active->x_coordinate < 0
		|| tempActive[i][1] + active->y_coordinate > 15
		|| tempActive[i][1] + active->y_coordinate < 0){
			return; //fail
		}
		//check gameboard
		else if(inactive->board[tempActive[i][0] + active->x_coordinate][tempActive[i][1]+ active->y_coordinate]){
			return;//fail
		}
	}
	for(int i = 0; i < 4; i++){
		active->coordinates[i][0] = tempActive[i][0];
		active->coordinates[i][1] = tempActive[i][1];
	}
	
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


//for display
void combinePieceAndBoardIntoImage(	RGB_8x16_Frame* next_frame_ptr,
									Tetromino* piece, Gameboard* board){
										

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






