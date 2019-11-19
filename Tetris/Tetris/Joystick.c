#include <avr/io.h>
#include "Joystick.h"
#include "io.h"


void Joystick_Process_Raw(Joystick_Frame* frame){
	
	
	
}

void Joystick_Read(Joystick_Frame* frame){
	
	//We will read the Vx and Vy from the 2-potentiometer Joystick;
	
	frame->raw_x = ADC_Read(0);		//read 10 bit ADC value on ADC0	
	frame->raw_y = ADC_Read(1);		//read ADC1 as well
	frame->click = (~PINA & 0x08) ? 1 : 0; //read the click
}
