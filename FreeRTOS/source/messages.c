/*
 * messages.c
 *
 *  Created on: 04.08.2018
 *      Author: tomek
 */

#include "rtc.h"

#include "systemDefines.h"

msgDataInt_t CreateMessage(uint8_t * data, uint8_t dataLen, uint8_t type){
	msgDataInt_t message;
	memcpy(&message.data, data, dataLen);
	message.type = type;
	message.dataLen = dataLen;
	return message;
}
