/*
 * gpio.c
 *
 *  Created on: 13.08.2018
 *      Author: tomek
 */

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include "gpio.h"


#define PIN_MODE_MSK			(0x3)



void EnableGpioClk (GPIO_TypeDef * gpioPort){
	if (gpioPort == GPIOA){
		RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
	} else if (gpioPort == GPIOB){
		RCC->APB2ENR = RCC_APB2ENR_IOPBEN;
	} else if (gpioPort == GPIOC){
		RCC->APB2ENR = RCC_APB2ENR_IOPCEN;
	} else if (gpioPort == GPIOD){
		RCC->APB2ENR = RCC_APB2ENR_IOPDEN;
	}
}

void SetGpioAsInput (GPIO_TypeDef * gpioPort, uint8_t pinNb){
	if (pinNb < 8){
		gpioPort->CRL &= ~(PIN_MODE_MSK<<pinNb);
	} else {
		gpioPort->CRH &= ~(PIN_MODE_MSK<<(pinNb-8)*4);
	}
}

void SetGpioAsOutput(GPIO_TypeDef * gpioPort, uint8_t pinNb){
	if (pinNb < 8){
		gpioPort->CRL |= (PIN_MODE_MSK<<pinNb);
	} else {
		gpioPort->CRH |= (PIN_MODE_MSK<<((pinNb-8)*4));
	}
}


bool IsGpioHigh(GPIO_TypeDef * gpioPort, uint8_t pinNb){
	return (gpioPort->IDR & (1<<pinNb));
}

