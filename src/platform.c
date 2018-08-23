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
	usart->CR3 |= USART_CR3_DMAT;

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



void enableUart1Dma(uint32_t addressFrom, uint16_t bytesNb){
	RCC->AHBENR = RCC_AHBENR_DMA1EN;
	DMA1_Channel4->CMAR = addressFrom;
	DMA1_Channel4->CPAR =   0x40013804;
	DMA1_Channel4->CNDTR = bytesNb;
	DMA1_Channel4->CCR |= DMA_CCR1_DIR;
	DMA1_Channel4->CCR |= DMA_CCR1_MINC;
	DMA1_Channel4->CCR &= ~DMA_CCR1_MSIZE;
	DMA1_Channel4->CCR &= ~DMA_CCR1_PSIZE;
//	DMA1_Channel4->CCR |= DMA_CCR1_CIRC;
//	DMA1_Channel4->CCR |= DMA_CCR1_MEM2MEM;
	DMA1_Channel4->CCR |= DMA_CCR1_EN;


}



