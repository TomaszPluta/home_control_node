
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ThreadCommunication.h>
#include <ThreadLightSensor.h>
#include <ThreadServiceMode.h>
#include <ThreadSupervisor.h>
#include "stm32f10x.h"

/*Config RTOS includes*/
#include "FreeRTOSConfig.h"
/*Kernel includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

#include "systemDefines.h"
#include "platform.h"
#include "gpio.h"//
#include "rtc.h"
#include "__rfm12b.h"

QueueHandle_t internalMsgQueue;
QueueHandle_t externalMsgQueue;
QueueHandle_t logMsgQueue;



void  gpio_init(void){
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
}



#define RFM12_STATUS_RGIT 	0x8000
#define RFM12_STATUS_FFIT 	0x8000
#define RFM12_STATUS_POR 	0x4000
#define RFM12_STATUS_RGUR 	0x2000
#define RFM12_STATUS_FFOV 	0x2000
#define RFM12_STATUS_WKUP 	0x1000
#define RFM12_STATUS_EXT 	0x0800
#define RFM12_STATUS_LBD 	0x0400
#define RFM12_STATUS_FFEM 	0x0200
#define RFM12_STATUS_ATS 	0x0100
#define RFM12_STATUS_RSSI 	0x0100
#define RFM12_STATUS_DQD 	0x0080
#define RFM12_STATUS_CRL 	0x0040
#define RFM12_STATUS_ATGL	 0x0020



//
//void _delay_ms(int n) {
//
//	int i, j;
//	j= n*1000;
//	while(j--) {
//		i=2;
//		while(i--);
//	}
//}


volatile uint8_t rxBuff[1024];
volatile uint16_t pos;
volatile bool rx_flag;

void EXTI9_5_IRQHandler (void){
	EXTI_ClearITPendingBit(EXTI_Line5);




		uint16_t status = Rfm12bWriteCmd(0x0000);

		if (status & RFM12_STATUS_FFIT ){
			uint8_t rx = rfm12bReadFifo();
				if (pos <1024){
					rxBuff[pos] = rx;
					pos++;
					if (pos==30){
						 GPIOC->ODR ^= GPIO_Pin_13;
						asm volatile ("nop");
						rfm12bFifoReset();
					 	pos =0;
					 	rx_flag = true;
					}
			}
		}
}




 int main(){

//		RCC->APB2RSTR |= RCC_APB2RSTR_AFIORST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_IOPARST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_IOPBRST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_IOPCRST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_IOPDRST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_IOPERST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
//
//		RCC->APB2RSTR |= RCC_APB2RSTR_AFIORST;
//		RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;

	//    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;

	 	EnableGpioClk(LOG_UART_PORT);
	 	SetGpioAsOutAltPushPUll(LOG_UART_PORT, LOG_UART_PIN_TX);
	 	SetGpioAsInFloating(LOG_UART_PORT, LOG_UART_PIN_RX);
	 	EnableUart(USART1);




	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	 	GPIO_InitTypeDef PORT;
	 	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	 	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	 	PORT.GPIO_Pin = GPIO_Pin_13;
	 	GPIO_Init(GPIOC, &PORT);
	 	GPIOC->ODR |= GPIO_Pin_13;




	 	Rfm12bInit();
	 	_delay_ms(1000);	//wymagane opoznienie
	 	  Rfm12bWriteCmd(0x0000);
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 	EnableExti(GPIOB, 5, false, true);
	 	SetGpioAsInPullUp(GPIOB, 5);
		SetGpioAsInPullUp(GPIOB, 11);

	 	rfm12bFifoReset();
	 	rfm12bSwitchRx();


	 	  while(1) {
	 		  if (rx_flag){

	 			  NVIC_DisableIRQ(EXTI9_5_IRQn);
	 			  rfm12bSwitchTx();

	 			  _delay_ms(50);

	 			  uint8_t buff[] = "helloWorld1helloWorld2helloWorld3";
	 			  //  Rfm12bSendBuff(buff, 30);
	 			  RF12_TXPACKET(buff, 30);
	 			  NVIC_EnableIRQ(EXTI9_5_IRQn);
	 			  _delay_ms(250);
	 			  rfm12bSwitchRx();
	 			  _delay_ms(20);

//	 			//  rfm12bSwitchTx();
//	 		//	  _delay_ms(50);
//
//	 			  uint8_t buff[] = "goodyFive!1helloWorld2helloWorld3";
//	 			  //Rfm12bSendBuff(buff, 30);
//	 			 RF12_TXPACKET(buff, 30);
//
//	 			  _delay_ms(250);
//	 			// rfm12bFifoReset();
//	 			  rfm12bSwitchRx();
//	 			  _delay_ms(20);


	 			  rx_flag = false;

	 		  }



	 		 if (!(GPIOB->IDR & (1<<11))){

	 			 NVIC_DisableIRQ(EXTI9_5_IRQn);
	 			 rfm12bSwitchTx();

	 			 _delay_ms(50);

	 			 uint8_t buff[] = "helloWorld1helloWorld2helloWorld3";
	 			 //  Rfm12bSendBuff(buff, 30);
	 			 RF12_TXPACKET(buff, 30);
	 			 _delay_ms(250);
	 			 NVIC_EnableIRQ(EXTI9_5_IRQn);
	 			 rfm12bSwitchRx();
	 			 _delay_ms(20);




	 		 }

	 	  }

 }











//
//
//	 	while (1){

//			uint16_t status = RFM12B_RDSTATUS();
//
//
//			if (status & RFM12_STATUS_RGIT ){
//	 		uint8_t rx = RFM12B_RDFIFO();
//	 		if (rx !=0){
//	 			rxBuff[pos] = rx;
//	 			pos++;
//	 			if (pos==100){
//	 				asm volatile ("nop");
//	 			}
//	 		}
//
//			}
//			if (status & RFM12_STATUS_FFIT ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_POR ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_RGUR ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_FFOV ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_WKUP ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_EXT ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_LBD ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_FFEM ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_ATS ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_RSSI ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_DQD ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_CRL ){
//				asm volatile ("nop");
//			}
//			if (status & RFM12_STATUS_ATGL ){
//				asm volatile ("nop");
//			}
//
//
//
//
//
//

//			_delay_ms(100);
//
//
//	 	}

//	 	RFM12B_GPIO_Init();
//
//	 	Rfm_xmit(SW_RESET);
//	 	uint16_t status;
//	 	_delay_ms(250);
//	 	status = Rfm_xmit(STATUS_READ);
//	 	status = Rfm_xmit(STATUS_READ);
//
//
//
//	 	Rfm_init();



 //https://github.com/das-labor/librfm12/blob/master/src/rfm12.c

//	 	Rfm12bTest();
//	 	Rfm12bRx();
//
//
//	 RTC_Init();
//	 RtcClear();
//	 internalMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataInt_t));
//	 externalMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataExt_t));
//	 logMsgQueue = xQueueCreate(OUTPUT_QUEUE_SIZE, sizeof(msgDataExt_t));
//
//	 //	xTaskCreate( ThreadLightSensor, "ThreadLigtSensor", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
//	 	xTaskCreate( ThreadSupervisor, "Supervisor", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
//	 	xTaskCreate( ThreadCommunication, "Communication", 2048, NULL, tskIDLE_PRIORITY + 1, NULL);
//		xTaskCreate( ThreadServiceMode, "ServiceMode", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
//	 	vTaskStartScheduler();
//








// }
//
//
//
