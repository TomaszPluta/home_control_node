




#include <stdio.h>
#include <stdlib.h>
#include "nrf24.h"
#include "stm32f10x_gpio.h"



uint8_t big_array[] = "When reading or writing the time and date registers, sec \
ondary (user) buffers are used to prevent errors when the \
internal registers update. When reading the time and date  \
registers, the user buffers are synchronized to the internal \
registers  on  any  START  and  when  the  register  pointer \
rolls over to zero. The time information is read from these \
secondary registers, while the clock continues to run. This \
eliminates  the  need  to  reread  the  registers  in  case  the \
main registers update during a read. \
The  countdown  chain  is  reset  whenever  the  seconds \
register  is  written.  Write  transfers  occur  on  the  acknowl \
edge  from  the  DS3231.  Once  the  countdown  chain  is \
reset,  to  avoid  rollover  issues  the  remaining  time  and \
date  registers  must  be  written  within  1  second.  The  1Hz \
square-wave  output,  if  enabled,  transitions  high  500ms \
after the seconds data transfer, provided the oscillator is \
already running.";



uint8_t big_array[];




volatile static int debug_var;
/* ------------------------------------------------------------------------- */
uint8_t temp;
uint8_t q = 0;
uint8_t data_array_tx[32];
uint8_t data_array_rx[32];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
/* ------------------------------------------------------------------------- */
int main()
{

	uint8_t i;
	for (i =0; i< 30; i++){
		big_array[i] = 'A';
	}

	for (i =30; i< 60; i++){
		big_array[i] = 'B';
	}

	for (i =60; i< 90; i++){
			big_array[i] = 'C';
		}


	for (i =90; i< 120; i++){
			big_array[i] = 'D';
		}

	for (i =120; i< 140; i++){
			big_array[i] = 'E';
		}


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef PORT;
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	PORT.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &PORT);
	GPIOC->ODR |= GPIO_Pin_13;


	PORT.GPIO_Mode =  GPIO_Mode_IPU;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	PORT.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &PORT);


	nrf24_init();
	nRF24_restore_defaults();


    /* Channel #2 , payload length: 4 */
    nrf24_config(2,32);

    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);


    data_array_tx[0] = 0;
    data_array_tx[1] = 'i';
    data_array_tx[2] = 'n';
    data_array_tx[3] = 'g';


    while(1)
    {
        /* Fill the data buffer */


        if ((GPIOB->IDR  & GPIO_IDR_IDR11) == 0){
			/* Automatically goes to TX mode */
		//	478888888888888888nrf24_send(data_array_tx);

			l3_send_packet(100, big_array, 140);

//			data_array_tx[0]++;
//
//			/* Make analysis on last tranmission attempt */
//			temp = nrf24_lastMessageStatus();
//
//
//			if(temp == NRF24_TRANSMISSON_OK)
//			{
//			//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//			//	GPIO_SetBits(GPIOC, GPIO_Pin_13);
//				debug_var++;
//			}
//			else if(temp == NRF24_MESSAGE_LOST)
//			{
//			//  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//				debug_var++;
//			}
//
//			/* Retranmission count indicates the tranmission quality */
//			temp = nrf24_retransmissionCount();
//			debug_var+= temp;
//

			/* Or you might want to power down after TX */
			// nrf24_powerDown();
			nrf24_powerUpRx();
			_delay_ms(400);
		}


	   if(nrf24_dataReady())
		{
			nrf24_getData(data_array_rx); //<<-- to zawiesza
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			//GPIOC->ODR &=  ~GPIO_Pin_13;
			GPIOC->BRR = GPIO_Pin_13;
			//	spi_transfer(FLUSH_RX);

			//nrf24_powerUpRx();


			//_delay_ms(300);




//			_delay_ms(100);
//
//			if (data_array_rx[0] == 4){
//				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//			}
//			if (data_array_rx[0] == 5){
//				GPIO_SetBits(GPIOC, GPIO_Pin_13);
//			}
//			if (data_array_rx[0] == 6){
//				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//			}
//			if (data_array_rx[0] == 7){
//				GPIO_SetBits(GPIOC, GPIO_Pin_13);
//			}
		}

		/* Wait a little ... */
		_delay_ms(10);
    }
}

