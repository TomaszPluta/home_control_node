/*
 * ThreadServiceMode.c
 *
 *  Created on: 03.08.2018
 *      Author: tomek
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "ThreadServiceMode.h"


extern QueueHandle_t logMsgQueue;



void ThreadCommunication ( void * pvParameters )
{
	commandBuff[MAX_CMD_SIZE];
	if (xQueueReceive(logMsgQueue, &commandBuff, MAX_CMD_SIZE)){
			msgDataExt_t messageExt;
	}
	uint8_t x = 4;
	while (x){
		uint16_t c = uart1_receive();
		uart1_send(c);
	}
}
