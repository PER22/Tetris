#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

#define SHIFTREG_RCLK_PIN 5 
#define SHIFTREG_SRCLK_PIN 7
#define SHIFTREG_SCLR_PIN 0
#define SHIFTREG_DATA1_PIN 0
#define SHIFTREG_DATA2_PIN 1
#define SHIFTREG_DATA3_PIN 2
#define SHIFTREG_DATA4_PIN 3

          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);						 
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

 //for 8 Mhz crystal
void delay_ms(int miliSec)
{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC);
}

void ADC_channel(unsigned char channel){
	if(channel < 8 && channel >= 0){
		//CLEAR ADMUX2:0
		ADMUX &= 0xF8;
		//Set ADMUX
		ADMUX |= (channel & 0x07);
	}
}

unsigned short ADC_read(unsigned char channel){
		unsigned short myADC = 0x0000;
		ADC_channel(channel);
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		myADC = ADC;
		return myADC;
} 


void set_PWM(double frequency){
	static double current_frequency;
	if(frequency != current_frequency){
		if(!frequency){TCCR3B &= 0x08;}
		else{TCCR3B |= 0x03;}
		if(frequency < 0.954){OCR3A = 0xFFFF;}
		else if(frequency > 31250){OCR3A = 0x0000;}
		else{OCR3A = (short) (8000000 /(128*frequency)) - 1;}
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char tempB;
unsigned char tempD = 0x00;

void writetoAll16BitShiftRegisters(unsigned short data1, unsigned short data2, unsigned short data3, unsigned short data4){

	//while writing data to register, RCLK is held low, when it goes high values are 
	//latched to the storage register and pins Q7:0.
	PORTB &= ~(1 << SHIFTREG_RCLK_PIN);
		
	//Now serial write cycle begins.
	//When SRCLK goes from 0 to 1, the DATA# 
	//line's value shifts into the shift register. 
	//Looped 16x from MSB to LSB.
			
	for (int i = 15; i >=0; i--){
		
		//1 data bit is written to each shift register and the clock is set high.
		unsigned char bitToWrite =  (unsigned char)((data1 >> i) & 0x0001 );
		if(bitToWrite){PORTB |= (bitToWrite << SHIFTREG_DATA1_PIN);}
		else{PORTB &= ~(1 << SHIFTREG_DATA1_PIN);}
		
		bitToWrite =  (unsigned char)((data2 >> i) & 0x0001 );
		if(bitToWrite){PORTB |= (bitToWrite << SHIFTREG_DATA2_PIN);}
		else{PORTB &= ~(1 << SHIFTREG_DATA2_PIN);}
		
		bitToWrite =  (unsigned char)((data3 >> i) & 0x0001 );
		if(bitToWrite){PORTB |= (bitToWrite << SHIFTREG_DATA3_PIN);}
		else{PORTB &= ~(1 << SHIFTREG_DATA3_PIN);}
		
		bitToWrite =  (unsigned char)((data4 >> i) & 0x0001 );
		if(bitToWrite){PORTB |= (bitToWrite << SHIFTREG_DATA4_PIN);}
		else{PORTB &= ~(1 << SHIFTREG_DATA4_PIN);}
			
		for(int k = 0; k < 10; k++){
			asm("nop");
		}
		
		//Tick Clock, wait, tick other direction
		PORTB |= (1<< SHIFTREG_SRCLK_PIN);
		for(int k = 0; k < 10; k++){
			asm("nop");
		}
		PORTB &= ~(1 << SHIFTREG_SRCLK_PIN);
		for(int k = 0; k < 10; k++){
			asm("nop");
		}
		
		
	}
	
	//latch values from shift register to output pins 
	//after each of the 16 bits has been looped through;
	PORTB |= (1<< SHIFTREG_RCLK_PIN);

}





