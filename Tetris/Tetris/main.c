#include <avr/io.h>
#include <avr/interrupt.h> 

#include <stdlib.h> //for malloc()

#include "io.h" //Code for the LCD
#include "Joystick.h"	//Joystick_Frame struct, methods/SM's 
						//relating to its reading, reporting, and conversion
						//to desired logical values;
#include "SoundEffects.h"
#include "LED_Matrix.h"
#include "Gamestate.h"
						
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

unsigned char tmpD = 0x00;
unsigned short myADC = 0x0000;

unsigned char lostYet = 0;


char LCD_msg[33];

Joystick_Frame* currentJoystickFramePtr;
Joystick_Frame* nextJoystickFramePtr;

RGB_8x16_Frame* current_RGB_FramePtr;
RGB_8x16_Frame* next_RGB_FramePtr;

Tetromino* activeTile;
Gameboard* board;

//SoundEffect* Effects[6];

void testDisplayJoystickADC(){
	//I know this code is ugly, its really just a test bench to see 
	//what the X and Y ranges from the joystick and the logic value 
	//from its click.
	
	//display X
	unsigned short tmpADC = currentJoystickFramePtr->raw_x;
	LCD_msg[3] = (tmpADC % 10) + '0';
	tmpADC /= 10;
	LCD_msg[2] = (tmpADC % 10) + '0';
	tmpADC /= 10;
	LCD_msg[1] = (tmpADC % 10) + '0';
	tmpADC /= 10;
	LCD_msg[0] = (tmpADC % 10)+ '0';
	LCD_msg[4] = ' ';
	
	
	//display Y
	tmpADC = currentJoystickFramePtr->raw_y;
	LCD_msg[8] = (tmpADC % 10) + '0';
	tmpADC /= 10;
	LCD_msg[7] = (tmpADC % 10) + '0';
	tmpADC /= 10;
	LCD_msg[6] = (tmpADC % 10) + '0';
	tmpADC /= 10;
	LCD_msg[5] = (tmpADC % 10)+ '0';
	LCD_msg[9] = ' ';
	
	//display click
	LCD_msg[10] = currentJoystickFramePtr->click ? '1' :  '0';
	LCD_msg[11] = '\0';
	
	//Write to LCD
	LCD_DisplayString(1, &LCD_msg);

}

void TimerOn(){
	//AVR timer/counter control	register
	TCCR1B = 0x0B;
	//Bit3 = 0: assigns mode to CTC (clear timer on compare)
	//Bit2Bit1Bit0 = 011: sets pre-scaler to 64
	// 00001011 : 0x0B
	//
	// So 8Mhz clock 8,000,000 ticks/sec divided by 64
	// gives us 125,000 ticks/sec on the TCNT1 register.

	//AVR output compare register 1A:
	OCR1A = 125;
	//Timer interrupt will be generated when TCNT1 == OCR1A
	//We want a 1ms tick, so .0001sec * 125,000 ticks/sec = 125
	//So when TCNT1 == 125, 1ms will have passed.

	//AVR timer interupt mask register
	TIMSK1 = 0x02;
	//bit1 enables compare match interrupt

	//initialize AVR counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	//TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80;

}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

//C programmer will not call this ISR, but TimerISR instead;
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

              
void Joystick_Tick(){
	static int count = 0;
	if (count == 100){
		Joystick_Read(nextJoystickFramePtr);
		Joystick_Process_Raw(nextJoystickFramePtr);
		//COPY POINTER BEFORE SWAPPING
		Joystick_Frame* temp = currentJoystickFramePtr;
		//SWAP BUFFER
		currentJoystickFramePtr = nextJoystickFramePtr;
		nextJoystickFramePtr = temp;
		count = 0;
	}
	else{count++; return;}
	//READ + POPULATE always into the next frame before swapping buffer
}
			                                                                                        
void LED_Tick(){
	static int count = 0;
	//update frame every 8 ms
	if(count == 20){ RGB_8x16_Frame tmpFrame = *current_RGB_FramePtr;
		*current_RGB_FramePtr = *next_RGB_FramePtr;
		*next_RGB_FramePtr = tmpFrame;
		count = 0;
	}
	pulseColumn(count++, *current_RGB_FramePtr);
	
}

	
void Gamestate_Tick(){
	static int count = 0;
	count++;
	if(count == 1000){
		checkDownAndDo(activeTile,board);//move down 1 square every second;
		count = 0;//reset count
	}
	if(!lostYet){
		if(count == 200 
		|| count == 400 
		|| count == 600 
		|| count == 800 
		|| count == 0){
			 //switch based on joystick 
			 //up
				if(currentJoystickFramePtr->Y_direction == UP){
					checkRotateAndDo(activeTile,board);
					checkRotateAndDo(activeTile,board);
					checkRotateAndDo(activeTile,board);
				}
			//down
				else if(currentJoystickFramePtr->Y_direction == DOWN){
					checkRotateAndDo(activeTile,board);
				}
	// 		//right
 				else if(currentJoystickFramePtr->X_direction== RIGHT){checkRightAndDo(activeTile,board);}	
	// 		//left
 				else if(currentJoystickFramePtr->X_direction== LEFT){checkLeftAndDo(activeTile,board);} 
				//moves done
		
				//check that new board isn't an outright loss
				lostYet = checkLoss(board);
				//check to see if the active shape should be made inactive and a new one summoned;
				for(int i = 0; i < 4; i++){
					int y_coord = activeTile->coordinates[i][1] + activeTile->y_coordinate;
					if(y_coord == 15){convertPieceToInactive(activeTile, board);free(activeTile);
						activeTile = createTetromino(rand()%5); break;}
					int x_coord = activeTile->coordinates[i][0] + activeTile->x_coordinate;
					if(board->board[x_coord][y_coord + 1]){convertPieceToInactive(activeTile, board); free(activeTile);
						activeTile = createTetromino(rand()%5); break;}
				}
				//check for a row to be deleted	
				deletedAFilledRowAndSlidDown(board);
				combinePieceAndBoardIntoImage(next_RGB_FramePtr,
				activeTile, board);
				
				
		 }else{ return;}
	}else{//lost
		
	
	}
}



int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRB = 0xFF;
	PORTB = 0x00;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	DDRD = 0xFF;
	PORTD = 0x00;
	
	//Initializations
	ADC_init();
	LCD_init();
	
	currentJoystickFramePtr = (Joystick_Frame*) malloc(sizeof(Joystick_Frame));
	nextJoystickFramePtr = (Joystick_Frame*) malloc(sizeof(Joystick_Frame));
	
	current_RGB_FramePtr = (RGB_8x16_Frame*) malloc(sizeof(RGB_8x16_Frame));
	next_RGB_FramePtr = (RGB_8x16_Frame*) malloc(sizeof(RGB_8x16_Frame));
	
	board = (Gameboard*) malloc(sizeof(Gameboard));
	activeTile = createTetromino(rand()%5);
	
	//TEST CODE
// 	for (int i= 0; i < 8; i++){
// 		for(int j = 0; j < 16; j++){
// 			if((i+j) % 2 == 0){
// 				current_RGB_FramePtr->frame[i][j] = (1 << RGB_BLUE_BIT);
// 			}
// 			else{
// 				current_RGB_FramePtr->frame[i][j] = (1 << RGB_BLUE_BIT);
// 			}
// 		}
// 	}
// 	for (int i= 0; i < 8; i++){
// 		for(int j = 0; j < 16; j++){
// 			if((i+j) % 2 == 0){
// 				next_RGB_FramePtr->frame[i][j] = (1 << RGB_GREEN_BIT);
// 			}
// 			else{
// 				next_RGB_FramePtr->frame[i][j] = (1 << RGB_GREEN_BIT);
// 			}
// 		}
// 	}
	
	//timing
	TimerSet(1);
	TimerOn();
    while (1){
		Joystick_Tick();
		Gamestate_Tick(); //TODO
		//SoundEffect_Tick();
		LED_Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

