#include "SoundEffects.h"
#include <stdio.h>

SoundEffect_Queue* create_SoundEffect_Queue(int cap){
	SoundEffect_Queue* ptr = (SoundEffect_Queue*) malloc(sizeof(SoundEffect_Queue)); 
	return ptr;
}

// void EnqueueSoundEffect(SoundEffect_Queue* queue , int soundEffectSelector){
// 	
// 	if(!isQueueFull(queue) && soundEffectSelector < 6 && soundEffectSelector >= 0){
// 			queue->size += 1;
// 			queue->back = (queue->back + 1) % queue->capacity;
// 		//	queue->effects[queue->back] = soundEffectSelector;
// 	}
// }

// void PlaySoundEffectThenDequeue(SoundEffect_Queue* queue){
// 	if(!isQueueEmpty(queue)){}
// }


int isQueueEmpty(SoundEffect_Queue* queue){
	return (queue->size == 0);
}

int isQueueFull(SoundEffect_Queue* queue){
	return (queue->size == queue->capacity);
}


	
void SoundEffect_Player_Tick(){}

