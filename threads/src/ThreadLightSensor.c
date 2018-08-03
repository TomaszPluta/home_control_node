/*
 * ThreadLigtSensor.c
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */
#include "FreeRTOS.h"
#include "ThreadLightSensor.h"
#include "queue.h"

extern QueueHandle_t OutputQueue;


void LightSensorInit(void){

}


void ThreadLightSensor ( void * pvParameters )
{



	for (;;) {
		lightLevel_t lightLevel = GetLightLevel();
		message_t message = CreateMessage((uint8_t*) &lightLevel, sizeof(lightLevel_t), MSG_TYPE_LIGHT);
		xQueueSend(OutputQueue, &message_t message, 0);
		vTaskDelay(1000);
	}
}
