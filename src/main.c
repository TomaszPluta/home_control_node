
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "nrf24_hal.h"
#include "nrf24.h"
#include "bmp280.h"

#define SPI_BUFFER_LEN 64
#define BUFFER_LENGTH	0xFF
#define	SPI_READ	0x80
#define SPI_WRITE	0x7F
#define BMP280_DATA_INDEX	1
#define BMP280_ADDRESS_INDEX	2



struct bmp280_t bmp280;

//
//
//main(int argc, char* argv[])
//{
//
//	uint8_t pBuf[10] = {'a', 'b', 'c', 'd', 'e'};
//
//	 // At this stage the system clock should have already been configured
//	// at high speed.
//	nRF24_GPIO_Init();
//	nRF24_Check();
//	uint8_t ADDR[] = { 'n', 'R', 'F', '0', '0' }; // the TX address
//	nRF24_DisableAA(0xFF); // disable ShockBurst
//	nRF24_SetRFChannel(90); // set RF channel to 2490MHz
//	nRF24_SetDataRate(nRF24_DR_2Mbps); // 2Mbit/s data rate
//	nRF24_SetCRCScheme(nRF24_CRC_1byte); // 1-byte CRC scheme
//	nRF24_SetAddrWidth(5); // address width is 5 bytes
//	nRF24_SetTXPower(nRF24_TXPWR_0dBm); // configure TX power
//	nRF24_SetAddr(nRF24_PIPETX, ADDR); // program TX address
//	nRF24_SetOperationalMode(nRF24_MODE_TX); // switch transceiver to the TX mode
//	nRF24_SetPowerMode(nRF24_PWR_UP); // wake-up transceiver (in case if it sleeping)
//	// the nRF24 is ready for transmission, upload a payload, then pull CE pin to HIGH and it will transmit a packet...
//
//	int8_t status;
//	nRF24_WritePayload(pBuf, 10); // transfer payload data to transceiver
//	nRF24_CE_H(); // assert CE pin (transmission starts)
//	while (1) {
//		status = nRF24_GetStatus();
//		if (status & (nRF24_FLAG_TX_DS | nRF24_FLAG_MAX_RT)) {
//			// transmission ended, exit loop
//			break;
//		}
//	}
//	nRF24_CE_H(); // de-assert CE pin (nRF24 goes to StandBy-I mode)
//	nRF24_ClearIRQFlags(); // clear any pending IRQ flags
//	if (status & nRF24_FLAG_MAX_RT) {
//		// Auto retransmit counter exceeds the programmed maximum limit (payload in FIFO is not removed)
//		// Also the software can flush the TX FIFO here...
//		return 0;
//	}
//	if (status & nRF24_FLAG_TX_DS) {
//		// Successful transmission
//		return 1;
//	}
//	// In fact that should not happen
//	return 0;
//
//
//
//}
//
//


//
//void spi2_init(void) {
//
//	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	/* Enable SPI1 and GPIOA clocks */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//
//	//SPI - SCK, MISO, MOSI
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//
//	//CS hdwr
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//
//	/* SPI1 configuration */
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
//	SPI_SSOutputCmd(SPI2, ENABLE);
//	SPI_Init(SPI2, &SPI_InitStructure);
//
//	/* Enable SPI1  */
//	SPI_Cmd(SPI2, ENABLE);
//
//}
//

#define bmp280_CSN_L()              GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define bmp280_CSN_H()              GPIO_SetBits(GPIOB, GPIO_Pin_12)

//
//uint8_t  spi_read_write_string(uint8_t * read, uint8_t* write, uint8_t cnt){
//	 // Wait until TX buffer is empty
//
//	bmp280_CSN_L();
//	uint8_t i =0;
//	while (i != cnt){
//
//		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
//		// Send byte to SPI (TXE cleared)
//		SPI_I2S_SendData(SPI2, write[i]);
//			// Wait while receive buffer is empty
//		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//
//
//		read[i] = SPI_I2S_ReceiveData(SPI2);
//
//		i++;
//	}
//	bmp280_CSN_H();
//
//}


s8  BMP280_SPI_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 iError=BMP280_INIT_VALUE;
	u8 array[SPI_BUFFER_LEN]={BUFFER_LENGTH};
	u8 stringpos;
	/*	For the SPI mode only 7 bits of register addresses are used.
	The MSB of register address denotes the type of SPI data transfer, whether
	read/write (read as 1/write as 0)*/
	array[BMP280_INIT_VALUE] = reg_addr|SPI_READ;
	spi_read_write_string(array, array, cnt+1);
	/*read routine is initiated by masking register address with 0x80*/
	/*
	* Please take the below function as your reference to
	* read the data using SPI communication
	* " IERROR = SPI_READ_WRITE_STRING(ARRAY, ARRAY, CNT+1)"
	* add your SPI read function here
	* iError is an return value of SPI read function
	* Please select your valid return value
	* In the driver SUCCESS defined as 0
	* and FAILURE defined as -1
	* Note :
	* This is a full duplex operation,
	* The first read data is discarded, for that extra write operation
	* have to be initiated. Thus cnt+1 operation done in the SPI read
	* and write string function
	* For more information please refer the SPI communication in data sheet
	*/
	for (stringpos = BMP280_INIT_VALUE; stringpos < cnt; stringpos++) {
		*(reg_data + stringpos) = array[stringpos+BMP280_DATA_INDEX];
	}
	return (s8)iError;
}

/*	\Brief: The function is used as SPI bus write
 *	\Return : Status of the SPI write
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, where data is to be written
 *	\param reg_data : It is a value held in the array,
 *		which is written in the register
 *	\param cnt : The no of bytes of data to be written
 */
s8  BMP280_SPI_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 iError = BMP280_INIT_VALUE;
	u8 array[SPI_BUFFER_LEN * BMP280_ADDRESS_INDEX];
	u8 stringpos = BMP280_INIT_VALUE;
	u8 index = BMP280_INIT_VALUE;

	for (stringpos = BMP280_INIT_VALUE; stringpos < cnt; stringpos++) {
		/* the operation of (reg_addr++)&0x7F done as per the
		SPI communication protocol specified in the data sheet*/
		index = stringpos * BMP280_ADDRESS_INDEX;
		array[index] = (reg_addr++) & SPI_WRITE;
		array[index + BMP280_DATA_INDEX] = *(reg_data + stringpos);
	}

	uint8_t read_buff[8];
	spi_read_write_string(read_buff, array, cnt*2 );


	/* Please take the below function as your reference
	 * to write the data using SPI communication
	 * add your SPI write function here.
	 * "IERROR = SPI_WRITE_STRING(ARRAY, CNT*2)"
	 * iError is an return value of SPI write function
	 * Please select your valid return value
	 * In the driver SUCCESS is defined as 0 and FAILURE is defined as -1
	 */
	return (s8)iError;
}

/*	Brief : The delay routine
 *	\param : delay in ms
*/
void  BMP280_delay_msek(u32 msek)
{
	/*Here you can write your own delay routine*/
}




s8 SPI_routine(void) {
/*--------------------------------------------------------------------------*
 *  By using bmp280 the following structure parameter can be accessed
 *	Bus write function pointer: BMP280_WR_FUNC_PTR
 *	Bus read function pointer: BMP280_RD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *--------------------------------------------------------------------------*/

	bmp280.bus_write = BMP280_SPI_bus_write;
	bmp280.bus_read = BMP280_SPI_bus_read;
	bmp280.delay_msec = BMP280_delay_msek;

	return BMP280_INIT_VALUE;
}


signed long t_fine;
signed long bmp280_compensate_T_int32(signed long adc_T)
{
	long signed int var1, var2, T;
	var1 = ((((adc_T>>3) - ((long signed int)bmp280.calib_param.dig_T1<<1))) * ((long signed int)bmp280.calib_param.dig_T1)) >> 11;
	var2 = (((((adc_T>>4) - ((long signed int)bmp280.calib_param.dig_T1)) * ((adc_T>>4) - ((long signed int)bmp280.calib_param.dig_T1))) >> 12) *
	((long signed int)bmp280.calib_param.dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}




 double compensate_temperature(signed long adc_T)
{
	double var1;
	double var2;
	double temperature;
	double temperature_min = -40;
	double temperature_max = 85;

	var1 = ((double)adc_T) / 16384.0 - ((double)bmp280.calib_param.dig_T1) / 1024.0;
	var1 = var1 * ((double)bmp280.calib_param.dig_T2);
	var2 = (((double)adc_T) / 131072.0 - ((double)bmp280.calib_param.dig_T1) / 8192.0);
	var2 = (var2 * var2) * ((double)bmp280.calib_param.dig_T1);
	static int32_t t_fine;
	t_fine = (int32_t)(var1 + var2);
	temperature = (var1 + var2) / 5120.0;

	if (temperature < temperature_min)
		temperature = temperature_min;
	else if (temperature > temperature_max)
		temperature = temperature_max;

	return temperature;
}






int
main(int argc, char* argv[])
{



	uint8_t pBuf[10] = {'a', 'b', 'c', 'd', 'e'};


	static uint8_t status;
	static uint32_t lost_pckts;
	static signed long temperature;
	static signed long pressure;



	spi2_init();
	SPI_routine();
	bmp280_init(&bmp280);



static signed long temp_out;
static signed long press_out;


while(1){


	bmp280_read_uncomp_temperature(&temperature);
	temp_out = bmp280_compensate_T_int32(temperature);
	temp_out = bmp280_compensate_temperature_int32(temperature);
	compensate_temperature(temperature);

	bmp280_read_uncomp_pressure(&pressure);
	press_out = bmp280_compensate_pressure_int32(pressure);

}

}

