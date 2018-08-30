/*
 * ThreadHealthMonitor.c
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */
#include <ThreadHealthMonitor.h>
#include "FreeRTOS.h"

void ThreadHealthMonitor ( void * pvParameters )
{
	for (;;) {

		vTaskDelay(1000);
	}
}
