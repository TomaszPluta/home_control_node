
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
#include "nrf24.h"//
#include "rtc.h"
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
#include "rfm12b.h"
#include "RFM12B_reg.h"


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

void EXTI9_5_IRQHandler (void){
	EXTI_ClearITPendingBit(EXTI_Line5);




		uint16_t status = RFM12B_RDSTATUS();

		if (status & RFM12_STATUS_FFIT ){
			uint8_t rx = RFM12B_RDFIFO();
				if (pos <1024){
					rxBuff[pos] = rx;
					pos++;
					if (pos==30){
						 GPIOC->ODR &= ~GPIO_Pin_13;
						asm volatile ("nop");
					 	FIFOReset();
					 	pos =0;
					}
			}
		}







}
//
//
//
//	uint16_t status = RFM12B_RDSTATUS();
//
//
//	if (status & RFM12_STATUS_FFIT ){
//		uint8_t rx = RFM12B_RDFIFO();
//		if (rx !=0){
//			if (pos <1024){
//				rxBuff[pos] = rx;
//				pos++;
//				if (pos==100){
//					asm volatile ("nop");
//				}
//			}
//		}
//	}
//
//}
//		}
//		if (status & RFM12_STATUS_FFIT ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_POR ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_RGUR ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_FFOV ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_WKUP ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_EXT ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_LBD ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_FFEM ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_ATS ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_RSSI ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_DQD ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_CRL ){
//			asm volatile ("nop");
//		}
//		if (status & RFM12_STATUS_ATGL ){
//			asm volatile ("nop");
//		}
//
//}




 int main(){





	 	EnableGpioClk(LOG_UART_PORT);
	 	SetGpioAsOutAltPushPUll(LOG_UART_PORT, LOG_UART_PIN_TX);
	 	SetGpioAsInFloating(LOG_UART_PORT, LOG_UART_PIN_RX);
	 	EnableUart(USART1);



	 	RFM12B_GPIO_Init();

	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	 	GPIO_InitTypeDef PORT;
	 	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	 	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	 	PORT.GPIO_Pin = GPIO_Pin_13;
	 	GPIO_Init(GPIOC, &PORT);
	 	GPIOC->ODR |= GPIO_Pin_13;

	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		EnableExti(GPIOB, 5, false, true);
		SetGpioAsInPullUp(GPIOB, 5);


	 	rfInit();
	 	_delay_ms(100);	//wymagane opoznienie

	 	_delay_ms(50);
	 	FIFOReset();



		 writeCmd(0x8299);


	 	  while(1) {
//
//	 		  waitForData();
//
//	 		  uint8_t rx =  rfRecv();
//	 		  if (rx !=0){
//	 			  if (pos <1024){
//	 				  rxBuff[pos] = rx;
//	 				  pos++;
//	 				  if (pos==100){
//	 					 GPIOC->ODR &= ~GPIO_Pin_13;
//	 					  asm volatile ("nop");
//	 				  }
//	 			  }
//
//	 		  }
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
