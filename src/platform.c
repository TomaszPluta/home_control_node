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

	NVIC_EnableIRQ(USART1_IRQn);
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



void SendUart1Dma(uint32_t sourceAddr, uint16_t bytesNb){
	RCC->AHBENR = RCC_AHBENR_DMA1EN;
	DMA1_Channel4->CMAR = sourceAddr;
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




//
//void ring_buffer_write (volatile ring_buff_t * r_buff, uint8_t * data, uint16_t amount)
//{
//	while (amount--){
//		r_buff->buffer[r_buff->head++] = *data;
//		if(r_buff->head == R_BUFF_SIZE){
//			r_buff->head =0;
//		}
//	}
//}
//
//
//uint16_t ring_buffer_read (volatile ring_buff_t * r_buff, uint8_t * data, uint16_t amount)
//{
//	uint16_t just_read = 0;
//	uint16_t to_read=0;
//	int32_t dist;
//
//	dist = (r_buff->head - r_buff->tail);
//	if (dist>0){
//		to_read = dist;
//	}
//	else  if (dist < 0){
//		to_read = (R_BUFF_END - r_buff->tail) +  (r_buff->head - R_BUFF_START);
//	}
//	if (dist==0){
//		return 0;
//	}
//
//	if (amount > to_read){
//		amount = to_read;
//	}
//
//	uint16_t i=0;
//	while (amount--){
//		data[i++] = r_buff->buffer[r_buff->tail++];
//		if (r_buff->tail == R_BUFF_SIZE){
//			r_buff->tail =0;
//		}
//		just_read++;
//	}
//	return just_read;
//}
//
//
//uint16_t uart_rec(void){
//	if(USART1->SR & USART_SR_RXNE)
//		return USART1->DR;
//	else {
//		return UART_NO_DATA;
//	}
//}
//
//
//void uart_send(uint16_t c)
//{
//	while (!(USART1->SR & USART_SR_TXE)){
//		;
//	}
//	USART1->DR = c;
//}
//
//
//void USART1_IRQHandler(void)
//{
//	if((USART1->SR & USART_IT_RXNE) != RESET){
//		char c = USART1->DR;
//		ring_buffer_write(&r_buff, (uint8_t*)&c, 1);
//	}
//}
//
//
//void ring_buffer_init(volatile ring_buff_t * r_buff)
//{
//	memset(r_buff, 0, sizeof(ring_buff_t));
//}

