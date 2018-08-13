/*
 * ThreadLightSensor.h
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */

#ifndef THREADLIGHTSENSOR_H_
#define THREADLIGHTSENSOR_H_



typedef enum{
	lightLow =0,
	lightHigh =1,
}lightLevel_t;


lightLevel_t GetLightLevel(void);

void ThreadLightSensor ( void * pvParameters );

#endif /* THREADLIGHTSENSOR_H_ */
