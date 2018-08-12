
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



QueueHandle_t internalMsgQueue;
QueueHandle_t externalMsgQueue;




 int main(){
	 EnableGpioClk(APB2_PBB);
	 SetGpioAsOutput(GPIOB, 9);
	 SetGpioAsInput(GPIOB, 9);

	 while(1){
		bool on =  ReadtGpioPin(GPIOB, 9);
		 on =  ReadtGpioPin(GPIOB, 9);
		 reversed ligh sensor logic
	 }
	 RTC_Init();
	 RtcClear();
	 internalMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataInt_t));
	 externalMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataExt_t));

	// 	xTaskCreate( bmp280_read_temp_full,  "bmp280_read_temp_full", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	// 	xTaskCreate( bmp280_read_press_full, "bmp280_read_press_full", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	 	xTaskCreate( ThreadLightSensor, "ThreadLigtSensor", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	// 	xTaskCreate( nr24l01_receiver, "rex", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	 	vTaskStartScheduler();


   while (1) {
 }

 }



