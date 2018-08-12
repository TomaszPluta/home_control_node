/*
 * ThreadLightSensor.h
 *
 *  Created on: 25.07.2018
 *      Author: tomek
 */

#ifndef THREADLIGHTSENSOR_H_
#define THREADLIGHTSENSOR_H_


#define	APB2_PBA 			 2
#define	APB2_PBB 			 3
#define	APB2_PBC 			 4
#define APB2_PBD  			 5


typedef enum{
	low =0,
	high =1,
}lightLevel_t;


lightLevel_t GetLightLevel(void);

void ThreadLightSensor ( void * pvParameters );

#endif /* THREADLIGHTSENSOR_H_ */
