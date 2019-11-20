#ifndef SOUNDEFFECTS_H_
#define SOUNDEFFECTS_H_

#include "io.h"
typedef struct SoundEffect{
	//length, pitch
	int length;
	double* pitches_ptr;
} SoundEffect;
//constructor
SoundEffect* create_SoundEffect(int len, double* pitchesArray);


//SoundEffect_Queue
typedef SoundEffect_Queue 



void SoundEffect_Tick();



#endif /* SOUNDEFFECTS_H_ */