/*
 * rtc.c
 *
 *  Created on: 05.08.2018
 *      Author: tomek
 */
#include "rtc.h"
#include "time.h"
#include "stm32f10x.h"


typedef struct tm tmTime_t;


void RTC_Init (tmTime_t time){
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
	PWR->CR |= PWR_CR_DBP;

	while (RTC->CRL & RTC_CRL_RTOFF);
	RTC->CRL |= RTC_CRL_CNF;

	RTC->CRL &= RTC_CRL_CNF;
	while (RTC->CRL & RTC_CRL_RTOFF);

}



uint32_t getCurrentTime (void){
	return 1;
}
