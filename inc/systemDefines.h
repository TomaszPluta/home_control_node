/*
 * systemDefines.h
 *
 *  Created on: 04.08.2018
 *      Author: tomek
 */

#ifndef SYSTEMDEFINES_H_
#define SYSTEMDEFINES_H_

#include <stdint.h>

#define OUTPUT_QUEUE_SIZE			(16)
#define DATA_SIZE					(24)


#define MSG_TYPE_LIGHT				(1)

typedef struct{
	uint8_t  data[DATA_SIZE];
	uint8_t  dataLen;
	uint8_t  type;
	uint32_t timeStamp;
}message_t;


#endif /* SYSTEMDEFINES_H_ */
