#include <avr/io.h>
#include "Joystick.h"
#include "io.h"


void Joystick_Process_Raw(Joystick_Frame* frame){
	
	//Set X
	if(frame->raw_x < JOYSTICK_LIMIT_LEFT){frame->X_direction = LEFT;}
	else if(frame->raw_x > JOYSTICK_LIMIT_RIGHT){frame->X_direction = RIGHT;}
	else{frame->X_direction = NONE;}
	//Set Y
	if(frame->raw_y < JOYSTICK_LIMIT_UP){frame->Y_direction = UP;}
	else if(frame->raw_y > JOYSTICK_LIMIT_DOWN){frame->Y_direction = DOWN;}
	else{frame->Y_direction = NONE;}
	//Decide what to do on ambiguous inputs:
	//I have decided to ignore the vertical input as it isn't 
	//time critical to move the block downward
	if(frame->X_direction != NONE && frame->Y_direction != NONE){
		frame->Y_direction = NONE;
	} 
}

void Joystick_Read(Joystick_Frame* frame){
	
	//We will read the Vx and Vy from the 2-potentiometer Joystick;
	
	frame->raw_x = ADC_read(0);		//read 10 bit ADC value on ADC0	
	frame->raw_y = ADC_read(1);		//read ADC1 as well
	frame->click = (~PINA & 0x08) ? 1 : 0; //read the click
}
