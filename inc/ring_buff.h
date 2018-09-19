/*
 * ring_buff.h
 *
 *  Created on: 19.09.2018
 *      Author: tomek
 */

#ifndef RING_BUFF_H_
#define RING_BUFF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define R_BUFF_SIZE		(512)
#define R_BUFF_START    (0)
#define R_BUFF_END     (R_BUFF_SIZE)

typedef struct {
	uint16_t head;
	uint16_t tail;
	uint16_t buffer[R_BUFF_SIZE];
}ringBuff_t;


void RingBufferInit (volatile ringBuff_t * r_buff);
void ring_buffer_write (volatile ringBuff_t * r_buff, uint8_t * data, uint16_t amount);
uint16_t ring_buffer_read (volatile ringBuff_t * r_buff, uint8_t * data, uint16_t amount);


#ifdef __cplusplus
}
#endif

#endif /* RING_BUFF_H_ */
