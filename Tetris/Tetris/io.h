#ifndef __io_h__
#define __io_h__
#include <avr/io.h>
//Code from
void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string);
void delay_ms(int miliSec);

void ADC_init();
void ADC_channel(unsigned char channel);
unsigned short ADC_read(unsigned char channel);


//Code from lab 9
void set_PWM(double frequency);
void PWM_on();
void PWM_off();

#endif
