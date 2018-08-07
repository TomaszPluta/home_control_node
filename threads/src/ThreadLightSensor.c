/*
 * ThreadLigtSensor.c
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "systemDefines.h"
#include "messages.h"
#include "ThreadLightSensor.h"

extern QueueHandle_t internalMsgQueue;


void LightSensorInit(void){

}


lightLevel_t GetLightLevel(void){
	return 1;
}



void ThreadLightSensor ( void * pvParameters )
{

	for (;;) {
		lightLevel_t lightLevel = GetLightLevel();
		msgDataInt_t message = CreateMessage((uint8_t*) &lightLevel, sizeof(lightLevel_t), lightLevel);
		xQueueSend(internalMsgQueue, &message, 0);
		vTaskDelay(1000);
	}
}
