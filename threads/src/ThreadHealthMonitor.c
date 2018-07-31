/*
 * ThreadHealthMonitor.c
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */
#include "FreeRTOS.h"
#include "ThreadHealthMonitor.h"

void ThreadHealthMonitor ( void * pvParameters )
{
	for (;;) {

		vTaskDelay(1000);
	}
}
