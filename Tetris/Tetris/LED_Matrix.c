#include "LED_Matrix.h"
#include "io.h"

void setRed(RGB_Square* ptr){
	*ptr = (1 << RGB_RED_BIT);
}
void setGreen(RGB_Square* ptr){
	*ptr = (1 << RGB_GREEN_BIT);
}
 void setBlue(RGB_Square* ptr){
	*ptr = (1 << RGB_BLUE_BIT);
}
void clearAll(RGB_Square* ptr){
	*ptr = 0;
	
}


void pulseColumn(unsigned char column, RGB_8x16_Frame frame){
	//static unsigned long pulseCount = 0;
	unsigned short register1 = 0xFFFF;
	unsigned short register2 = 0xFFFF;
	unsigned short register3 = 0x00FF;
	unsigned short register4 = 0x00FF;
	//build each register
	column &= 0x07;
	switch(column){
		case 7:
			//set the single high bit in the upper nibble of both
			//reg3 and reg4 to select which of the 8 columns they 
			//will be igniting.
			register4 |= 0x80FF;
			register3 |= 0x80FF;			
			break;
		case 6: 
			register4 |= 0x40FF;
			register3 |= 0x40FF;
			break;
		case 5:
			register4 |= 0x20FF;
			register3 |= 0x20FF;
			break;
		case 4:
			register4 |= 0x10FF;
			register3 |= 0x10FF;
			break;
		case 3:
			register4 |= 0x08FF;
			register3 |= 0x08FF;
			break;
		case 2:
			register4 |= 0x04FF;
			register3 |= 0x04FF;
			break;
		case 1:
			register4 |= 0x02FF;
			register3 |= 0x02FF;
			break;
		case 0:
			register4 |= 0x01FF;
			register3 |= 0x01FF;
			break;
	}
	//matrix 2
	for(int i = 0; i < 8; i++){
		//check red green blue
		if(frame.frame[column][i+8] & (1 << RGB_RED_BIT)){register2 &= ~(1 << i);}
	
		else if(frame.frame[column][i+8] & (1 << RGB_GREEN_BIT)){register4 &= ~(1 << i); }

		else if(frame.frame[column][i+8] & (1 << RGB_BLUE_BIT)){register2 &= ~(1 << (i+8));}
		
		//matrix 1			
		if(frame.frame[column][i] & (1 << RGB_RED_BIT)){register1 &= ~(1 << i);}
		
		else if(frame.frame[column][i] & (1 << RGB_GREEN_BIT)){register3 &= ~(1 << i); }

		else if(frame.frame[column][i] & (1 << RGB_BLUE_BIT)){register1 &= ~(1 << (i+8));}
	}
	
	

	writetoAll16BitShiftRegisters(register1, register2, register3, register4);
		
	
}

