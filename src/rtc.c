/*
 * rtc.c
 *
 *  Created on: 05.08.2018
 *      Author: tomek
 */
#include "rtc.h"

#include "stm32f10x.h"




void RtcClear (void){

	PWR->CR |= PWR_CR_DBP;
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	RCC->BDCR |= RCC_BDCR_LSEON;
	while (!(RCC->BDCR & RCC_BDCR_LSERDY));
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
	RCC->BDCR |= RCC_BDCR_RTCEN;
	PWR->CR &= ~PWR_CR_DBP;

}


void RTC_Init (void){
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
	PWR->CR |= PWR_CR_DBP;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

	RCC->BDCR |= RCC_BDCR_LSEON;
	while (!(RCC->BDCR & RCC_BDCR_LSERDY));
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
	RCC->BDCR |= RCC_BDCR_RTCEN;

	while (!(RTC->CRL & RTC_CRL_RTOFF));
	RTC->CRL |= RTC_CRL_CNF;

	RTC->PRLL = 0x7FFF;
	RTC->CNTH =0;
	RTC->CNTL =0;

	RTC->CRL &= RTC_CRL_CNF;
	while (!(RTC->CRL & RTC_CRL_RTOFF));
	PWR->CR &= ~PWR_CR_DBP;
}



uint32_t getCurrentTime (void){
	return 1;
}
