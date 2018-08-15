/*
 * rtc.h
 *
 *  Created on: 05.08.2018
 *      Author: tomek
 */

#ifndef RTC_H_
#define RTC_H_

#include "stdint.h"
#include "time.h"

typedef struct tm tmTime_t;


uint32_t getCurrentTime (void);
void RtcClear (void);
void RTC_Init (void);

#endif /* RTC_H_ */
