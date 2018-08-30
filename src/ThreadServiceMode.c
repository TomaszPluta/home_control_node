/*
 * ThreadServiceMode.c
 *
 *  Created on: 03.08.2018
 *      Author: tomek
 */
#include <ThreadServiceMode.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "systemDefines.h"

extern QueueHandle_t logMsgQueue;



void ThreadServiceMode ( void * pvParameters )
{
	xQueueSend(logMsgQueue, "serviceMode started", 0);

	for (;;) {
		char commandBuff[MAX_CMD_SIZE];
		if (xQueueReceive(logMsgQueue, &commandBuff, MAX_CMD_SIZE)){
			SendString(commandBuff);
		}
	}
//	uint8_t x = 4;
//	while (x){
//		uint16_t c = uart1_receive();
//		uart1_send(c);
//	}
}
