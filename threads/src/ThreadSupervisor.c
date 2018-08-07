/*
 * supervisor.c
 *
 *  Created on: 03.08.2018
 *      Author: tomek
 */
#include "FreeRTOS.h"
#include "queue.h"

#include "systemDefines.h"
#include "mqtt_client.h"

extern QueueHandle_t internalMsgQueue;
extern QueueHandle_t externalMsgQueue;



void ConverIntMsgToExtMsg(msgDataInt_t * msgDataInt, msgDataExt_t * msgDataExt){
	memcpy(&msgDataExt, msgDataInt, sizeof(msgDataInt_t));
	msgDataExt->timeStamp = getCurrentTime();

}


void ThreadSupervisor ( void * pvParameters )
{


	for (;;) {
		//measurements scheduler here
		// jedna kolejka do supervizora czy osobne od kazdego tasku?


		//parse and send internal data
		msgDataInt_t messageInt;
		xQueueReceive(internalMsgQueue, &messageInt, 0);
		msgDataExt_t messageExt;
		ConverIntMsgToExtMsg(&messageInt, &messageExt);
		xQueueSend(externalMsgQueue, &messageExt, 0);


		vTaskDelay(1000);
	}

}
