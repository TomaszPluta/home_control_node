
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

volatile rfm12bObj_t rfm12bObj;

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
	Rfm12bIrqCallback(&rfm12bObj);

}




 int main(){
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

	 	 NVIC_EnableIRQ(EXTI9_5_IRQn);


	 	Rrm12bObjInit (&rfm12bObj);

		 	while (1){

		 		  if (!(GPIOB->IDR & (1<<11))){
		 			  uint8_t buff[] = "helloWorld1helloWorld2helloWorld3";
		 			  Rfm12bStartSending(&rfm12bObj, buff, 30);
		 			 _delay_ms(250);
		 		  }
		 	}
	}




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
