/*
 * rfm12b.c
 *
 *  Created on: 30.08.2018
 *      Author: tomek
 */
#include "stm32f10x.h"
#include "RFM12B_reg.h"

void RFM12B_GPIO_Init(void) {

	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable SPI1 and GPIOA clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


	//SPI - SCK, MISO, MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//CS hdwr
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);


	GPIO_InitTypeDef PORT;

	// Enable the nRF24L01 GPIO peripherals
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	// Configure CSN pin
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	PORT.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &PORT);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}



uint16_t SpiTransfer(uint16_t data) {

	 // Wait until TX buffer is empty
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	// Send byte to SPI (TXE cleared)
	SPI_I2S_SendData(SPI1, data);
	// Wait while receive buffer is empty
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	// Return received byte
	return SPI_I2S_ReceiveData(SPI1);
}


uint16_t Rfm_xmit(uint16_t data){
 GPIO_ResetBits(GPIOB, GPIO_Pin_1);
 uint16_t spiRxData = SpiTransfer(data);
 GPIO_SetBits(GPIOB, GPIO_Pin_1);
 return spiRxData;
}

void Rfm12bTest (void){
	Rfm_xmit(SW_RESET);

	static uint16_t status;
	do{

		_delay_ms(250);
		status = Rfm_xmit(STATUS_READ);

	}while(!(status & M_POR));


	status=Rfm_xmit(STATUS_READ);


	if(status&M_POR){
		asm volatile ("nop");
	}else{
		asm volatile ("nop"); //("TEST PASS\r\n");
	}

	while(1);
}



