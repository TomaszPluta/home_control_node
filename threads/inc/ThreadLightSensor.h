/*
 * ThreadLightSensor.h
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */

#ifndef THREADLIGHTSENSOR_H_
#define THREADLIGHTSENSOR_H_



typedef enum{
	low =0,
	high =1,
}lightLevel_t;


lightLevel_t GetLightLevel(void);

void ThreadLightSensor ( void * pvParameters );

#endif /* THREADLIGHTSENSOR_H_ */
