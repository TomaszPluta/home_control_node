/*
 * ThreadLigtSensor.c
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */

#include <stdbool.h>
#include <ThreadLightSensor.h>


#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "platform.h"
#include "systemDefines.h"
#include "messages.h"

extern QueueHandle_t internalMsgQueue;
extern QueueHandle_t logMsgQueue;

void LightSensorInit(void){
	EnableGpioClk(Light_SENSOR_PORT);
	SetGpioAsInput(Light_SENSOR_PORT, Light_SENSOR_PIN);
}


lightLevel_t GetLightLevel(void){
	bool pinStateHigh = IsGpioHigh(Light_SENSOR_PORT, Light_SENSOR_PIN);
	if (pinStateHigh){
		return lightLow;
	}
	return lightHigh;
}



void ThreadLightSensor ( void * pvParameters )
{
//	xQueueSend(logMsgQueue, "LightSensor started", 0);

	LightSensorInit();
	//GetThreadConfig();
	for (;;) {
		lightLevel_t lightLevel = GetLightLevel();
		msgDataInt_t message = CreateMessage((uint8_t*) &lightLevel, sizeof(lightLevel_t), lightLevel);
		xQueueSend(internalMsgQueue, &message, 0);
		vTaskDelay(1000);
	}
}
