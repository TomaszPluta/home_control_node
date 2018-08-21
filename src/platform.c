/*
 * platform.c
 *
 *  Created on: 13.08.2018
 *      Author: tomek
 */


#include "stm32f10x.h"
#include "platform.h"



void EnableUart (USART_TypeDef * usart){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	usart->BRR = 72000000/115200;
	usart->CR1 |= USART_CR1_UE;
	usart->CR1 |= USART_CR1_TE;
	usart->CR1 |= USART_CR1_RE;
}

uint16_t uart1_receive(void){
	while (!(USART1->SR & USART_SR_RXNE)){
		;
	}
	return (uint16_t) USART1->DR;
}

void uart1_send(uint16_t c){
	 while (!(USART1->SR & USART_SR_TXE)){
	    	;
	 }
	 USART1->DR =c;
}


void SendString (const char* string){
	const uint8_t maxCharToSend = 32;
	uint8_t i;
	while (string[i] != 0)
	{
		uart1_send(string[i]);
		i++;
		if (i == maxCharToSend){
			break;
		}
	}
	uart1_send(0xA);
}





