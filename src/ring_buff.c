/*
 * ring_buff.c
 *
 *  Created on: 19.09.2018
 *      Author: tomek
 */
#include "ring_buff.h"




void RingBufferInit (volatile ringBuff_t * ringBuff){
	memset(ringBuff, 0, sizeof(ringBuff_t));
}


void RingBufferWrite (volatile ringBuff_t * ringBuff, uint8_t * data, uint16_t amount)
{
	while (amount--){
		ringBuff->buffer[ringBuff->head++] = *data++;
		if(ringBuff->head == R_BUFF_SIZE){
			ringBuff->head =0;
		}
	}
}


uint16_t RingBufferRead (volatile ringBuff_t * ringBuff, uint8_t * data, uint16_t amount)
{
	uint16_t just_read = 0;
	uint16_t to_read=0;
	int32_t dist;

	dist = (ringBuff->head - ringBuff->tail);
	if (dist>0){
		to_read = dist;
	}
	else  if (dist < 0){
		to_read = (R_BUFF_END - ringBuff->tail) +  (ringBuff->head - R_BUFF_START);
	}
	if (dist==0){
		return 0;
	}

	if (amount > to_read){
		amount = to_read;
	}

	uint16_t i=0;
	while (amount--){
		data[i++] = ringBuff->buffer[ringBuff->tail++];
		if (ringBuff->tail == R_BUFF_SIZE){
			ringBuff->tail =0;
		}
		just_read++;
	}
	return just_read;
}
