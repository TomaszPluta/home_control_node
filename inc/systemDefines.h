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

#define MAX_CMD_SIZE				(32)
#define MAX_LOG_SIZE				(32)


#define BUF_SIZE_TX					(128)
#define BUF_SIZE_RX					(128)

#define CMD_TIMEOUT_MS				(4000)

#define CENTER_ADDR					{0xE7,0xE7,0xE7,0xE7,0xE7}

#define BALCONY_LIGHT_TOPIC 				"Balcony/Light"
#define BALCONY_TEMP_TOPIC 					"Balcony/Temp"

typedef enum {
	lightLevel

}msg_type_t;



typedef struct{
	uint8_t  data[DATA_SIZE];
	uint8_t  dataLen;
	msg_type_t  type;
}msgDataInt_t;



typedef struct{
	uint8_t  data[DATA_SIZE];
	uint8_t  dataLen;
	msg_type_t  type;
	uint32_t timeStamp;
}msgDataExt_t;


#endif /* SYSTEMDEFINES_H_ */
