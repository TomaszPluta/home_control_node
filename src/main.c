
#include <stdio.h>
#include <stdlib.h>


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
	 RTC_Init();
	 RtcClear(); //not working
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



