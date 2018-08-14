
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f10x.h"

/*Config RTOS includes*/
#include "FreeRTOSConfig.h"
/*Kernel includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

#include "systemDefines.h"
#include "ThreadLightSensor.h"
#include "ThreadSupervisor.h"

#include "gpio.h"
#include "nrf24.h"

QueueHandle_t internalMsgQueue;
QueueHandle_t externalMsgQueue;


static gpio_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef PORT;
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	PORT.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &PORT);
	GPIOC->ODR |= GPIO_Pin_13;

	PORT.GPIO_Mode =  GPIO_Mode_IPU;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	PORT.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &PORT);
}



 int main(){



	 RTC_Init();
	 RtcClear();
	 internalMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataInt_t));
	 externalMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataExt_t));

	// 	xTaskCreate( bmp280_read_temp_full,  "bmp280_read_temp_full", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	// 	xTaskCreate( bmp280_read_press_full, "bmp280_read_press_full", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	 	xTaskCreate( ThreadLightSensor, "ThreadLigtSensor", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	 	xTaskCreate( ThreadSupervisor, "Supervisor", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	// 	xTaskCreate( nr24l01_receiver, "rex", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	 	vTaskStartScheduler();









 }



