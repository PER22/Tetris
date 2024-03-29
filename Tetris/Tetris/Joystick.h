#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef enum Direction{NONE, LEFT, RIGHT, UP, DOWN} Direction;
	
#define JOYSTICK_LIMIT_LEFT 100
#define JOYSTICK_LIMIT_RIGHT 1000
#define JOYSTICK_LIMIT_UP 100
#define JOYSTICK_LIMIT_DOWN 1000


typedef struct Joystick_Frame {
	
	unsigned short raw_x;
	unsigned short raw_y;
	unsigned char click;
	
	//These will be filled by 2 calls to Joystick_Process_Raw();
	Direction X_direction;
	Direction Y_direction;
} Joystick_Frame;


void Joystick_Process_Raw(Joystick_Frame* frame);
void Joystick_Read(Joystick_Frame* frame);
void Joystick_Tick();



#endif /* JOYSTICK_H_ */