#include <avr/io.h>
#include <avr/interrupt.h> 
#include "io.h" //Code for the LCD
#include "Joystick.h"	//Joystick_Frame struct, methods/SM's 
						//relating to its reading, reporting, and conversion
						//to desired logical values;





volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

unsigned char tmpD = 0x00;
unsigned short myADC = 0x0000;

char LCD_msg[33];

Joystick_Frame* currentJoystickFramePtr;
Joystick_Frame* nextJoystickFramePtr;


void testDisplayJoystickADC(){
	
	//display X
	unsigned char tmpADC = currentJoystickFramePtr->raw_x;
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
	LCD_msg[10] = currentJoystickFramePtr->click ? '1' :  '0';
	
	LCD_msg[11] = '\0';
	
	
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
	//READ + POPULATE always into the next frame before swapping buffer
	Joystick_Read(nextJoystickFramePtr);	
	Joystick_Process_Raw(nextJoystickFramePtr);	
	//COPY POINTER BEFORE SWAPPING
	Joystick_Frame* temp = currentJoystickFramePtr;
	//SWAP BUFFER	
	currentJoystickFramePtr = nextJoystickFramePtr;
	nextJoystickFramePtr = temp;
}

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	//DDRB = 0x______
	//PORTB = 0x________
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	DDRD = 0xFF;
	PORTD = 0x00;
	
	//Initializations
	ADC_init();
	LCD_init();
	currentJoystickFramePtr = (Joystick_Frame*) malloc(sizeof(Joystick_Frame));
	nextJoystickFramePtr = (Joystick_Frame*) malloc(sizeof(Joystick_Frame));
	
	TimerSet(250);
	TimerOn();
    while (1){
		Joystick_Tick();
		testDisplayJoystickADC();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}


