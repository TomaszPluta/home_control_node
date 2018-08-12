/*
 * ThreadLigtSensor.c
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */

#include <stdbool.h>
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "systemDefines.h"
#include "messages.h"
#include "ThreadLightSensor.h"

extern QueueHandle_t internalMsgQueue;

#define PIN_MODE_MSK			(0x3)






void EnableGpioClk (uint8_t  gpioPort){
	RCC->APB2ENR = (1<<gpioPort);
}

void SetGpioAsInput (GPIO_TypeDef * gpioPort, uint8_t pinNb){
	if (pinNb < 8){
		gpioPort->CRL &= ~(PIN_MODE_MSK<<pinNb);
	} else {
		gpioPort->CRH &= ~(PIN_MODE_MSK<<(pinNb-8)*4);
	}
}

void SetGpioAsOutput(GPIO_TypeDef * gpioPort, uint8_t pinNb){
	if (pinNb < 8){
		gpioPort->CRL |= (PIN_MODE_MSK<<pinNb);
	} else {
		gpioPort->CRH |= (PIN_MODE_MSK<<((pinNb-8)*4));
	}
}


bool ReadtGpioPin(GPIO_TypeDef * gpioPort, uint8_t pinNb){
	return (gpioPort->IDR & (1<<pinNb));
}





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
