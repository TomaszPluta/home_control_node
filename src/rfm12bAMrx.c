/*
 * rfm12badammruk.c
 *
 *  Created on: 01.09.2018
 *      Author: tomek
 */



#include "stdint.h"
#include "stm32f10x.h"


#define RFM12B_NSEL_L()              GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define RFM12B_NSEL_H()              GPIO_SetBits(GPIOB, GPIO_Pin_1);

#define NSEL_RFM12_DOWN				 RFM12B_NSEL_L()
#define NSEL_RFM12_UP				 RFM12B_NSEL_H()



uint16_t writeCmd( uint16_t CMD )
{


	NSEL_RFM12_DOWN;
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, CMD);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	uint16_t rec = SPI_I2S_ReceiveData(SPI1);

	NSEL_RFM12_UP;

	return rec;

}



void FIFOReset() {
  writeCmd(0xCA81);
  writeCmd(0xCA83);
}

void waitForData() {
  unsigned int status;
  while(1) {
    status = writeCmd(0x0000);
    if ( (status&0x8000) ) {
      return;
    }
  }
}

void rfInit() {
  writeCmd(0x0000);
  writeCmd(0x80E7); //EL,EF,868band,12.0pF
  //writeCmd(0x8299); //er,!ebb,ET,ES,EX,!eb,!ew,DC
  writeCmd(8008);
  writeCmd(0xA640); //freq select
  writeCmd(0xC647); //4.8kbps
  writeCmd(0x94A0); //VDI,FAST,134kHz,0dBm,-103dBm
  writeCmd(0xC2AC); //AL,!ml,DIG,DQD4
  writeCmd(0xCA81); //FIFO8,SYNC,!ff,DR
  writeCmd(0xCED4); //SYNC=2DD4;
  writeCmd(0xC483); //@PWR,NO RSTRIC,!st,!fi,OE,EN
  writeCmd(0x9850); //!mp,90kHz,MAX OUT
  writeCmd(0xCC17); //!OB1,!OB0, LPX,!ddy,DDIT,BW0
  writeCmd(0xE000); //NOT USE
  writeCmd(0xC800); //NOT USE
  writeCmd(0xC040); //1.66MHz,2.2V
}

/*
unsigned char rfRecv() {
  unsigned int data;
  writeCmd(0x0000);
  data = writeCmd(0xB000);
  return (data&0x00FF);
}
*/

unsigned char rfRecv() {
  unsigned int data;
  while(1) {
    data = writeCmd(0x0000);
    if ( (data&0x8000) ) {
      data = writeCmd(0xB000);
      return (data&0x00FF);
    }
  }
}
