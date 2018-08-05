/*
 * supervisor.c
 *
 *  Created on: 03.08.2018
 *      Author: tomek
 */
#include "FreeRTOS.h"
#include "queue.h"

#include "systemDefines.h"

extern QueueHandle_t OutputQueue;




void ThreadSupervisor ( void * pvParameters )
{

	for (;;) {

		message_t message;
		xQueueReceive(OutputQueue, &message, 0);
		switch (message.type){
		case lightLevel:

			break;
		}
		vTaskDelay(1000);
	}

}
