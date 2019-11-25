#ifndef SOUNDEFFECTS_H_
#define SOUNDEFFECTS_H_

#include "io.h"


#define WIN_SOUND 0
#define LOSS_SOUND 1
#define ROW_SOUND 2
#define TETRIS_SOUND 3
#define SIDEWAYS_SOUND 4
#define DOWN_SOUND 5

typedef struct SoundEffect{
	//length, pitch
	int length;
	double* pitches_ptr;
} SoundEffect;

//constructor
SoundEffect* create_SoundEffect(int len, double* pitchesArray);





//SoundEffect_Queue
typedef struct SoundEffect_Queue{
		SoundEffect* effects;
		unsigned char capacity;
		unsigned char size;
		unsigned char front;
		unsigned char back;
} SoundEffect_Queue;


SoundEffect_Queue* create_SoundEffect_Queue(int cap);

//void EnqueueSoundEffect(SoundEffect_Queue queue , int);

/*void DequeueAndPlaySoundEffect();*/

int isQueueEmpty();

int isQueueFull();

void SoundEffect_Player_Tick();



#endif /* SOUNDEFFECTS_H_ */