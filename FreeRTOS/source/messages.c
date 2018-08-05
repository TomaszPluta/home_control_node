/*
 * messages.c
 *
 *  Created on: 04.08.2018
 *      Author: tomek
 */

#include "rtc.h"

#include "systemDefines.h"

message_t CreateMessage(uint8_t * data, uint8_t dataLen, uint8_t type){
	message_t message;
	memcpy(&message.data, data, dataLen);
	message.type = type;
	message.dataLen = dataLen;
	message.timeStamp = getCurrentTime;
	return message;
}
