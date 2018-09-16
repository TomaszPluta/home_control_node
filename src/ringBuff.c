/*
 * ringBuff.c
 *
 *  Created on: 16.09.2018
 *      Author: tomek
 */
//
//#include <stdint.h>
//
//
//volatile ring_buff_t r_buff;
////ring_buffer_init(&r_buff);
//
//
//
//void ring_buffer_write (volatile ring_buff_t * r_buff, uint8_t * data, uint16_t amount)
//{
//	while (amount--){
//		r_buff->buffer[r_buff->head++] = *data;
//		if(r_buff->head == R_BUFF_SIZE){
//			r_buff->head =0;
//		}
//	}
//}
//
//
//uint16_t ring_buffer_read (volatile ring_buff_t * r_buff, uint8_t * data, uint16_t amount)
//{
//	uint16_t just_read = 0;
//	uint16_t to_read=0;
//	int32_t dist;
//
//	dist = (r_buff->head - r_buff->tail);
//	if (dist>0){
//		to_read = dist;
//	}
//	else  if (dist < 0){
//		to_read = (R_BUFF_END - r_buff->tail) +  (r_buff->head - R_BUFF_START);
//	}
//	if (dist==0){
//		return 0;
//	}
//
//	if (amount > to_read){
//		amount = to_read;
//	}
//
//	uint16_t i=0;
//	while (amount--){
//		data[i++] = r_buff->buffer[r_buff->tail++];
//		if (r_buff->tail == R_BUFF_SIZE){
//			r_buff->tail =0;
//		}
//		just_read++;
//	}
//	return just_read;
//}
