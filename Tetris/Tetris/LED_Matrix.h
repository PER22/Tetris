#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_



#define RGB_RED_BIT 0
#define RGB_BLUE_BIT 1
#define RGB_GREEN_BIT 2



typedef unsigned char RGB_Square;



typedef struct RGB_8x16_Frame{
	RGB_Square frame[8][16];
}RGB_8x16_Frame;


inline void setRed(RGB_Square* );
inline void setGreen(RGB_Square* );
inline void setBlue(RGB_Square* );
inline void clearAll(RGB_Square* );

void pulseColumn(unsigned char column, RGB_8x16_Frame);

RGB_8x16_Frame* create_RGB_Frame();

#endif /* LED_MATRIX_H_ */
