/*
 * messages.c
 *
 *  Created on: 04.08.2018
 *      Author: tomek
 */

#include "systemDefines.h

message_t CreateMessage(uint8 * data, uint8_t dataLen, uint8_t type){
	message_t message;
	message.data = data;
	message.type = type;
	message.dataLen = dataLen;
	message.timeStamp = getCurrentTime;
}
