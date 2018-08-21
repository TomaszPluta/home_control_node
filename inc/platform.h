/*
 * platform.h
 *
 *  Created on: 13.08.2018
 *      Author: tomek
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_


#include "stm32f10x.h"

#include "gpio.h"

#define Light_SENSOR_PORT    (GPIOB)
#define Light_SENSOR_PIN     (9)

#define LOG_UART_PORT 	(GPIOA)
#define LOG_UART_PIN_TX 	(9)
#define LOG_UART_PIN_RX 	(10)


void EnableUart (USART_TypeDef * usart);
void SendString (const char* string);

#endif /* PLATFORM_H_ */
