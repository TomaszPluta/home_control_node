
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stm32f10x.h"


#include "mqtt_client.h"

#include "systemDefines.h"
#include "platform.h"
#include "gpio.h"//
#include "rtc.h"
#include "__rfm12b.h"
#include "ring_buff.h"

volatile uint8_t rxBuff[1024];
volatile uint16_t pos;
volatile bool rx_flag;
volatile uint32_t systickMsIRQ;
volatile ringBuff_t ringBuff;


#define BROKER_ADDR		(1)
#define NODE_ADDR		(2)

 uint32_t GetTickCount(void){
	 return systickMsIRQ;
}


volatile rfm12bObj_t rfm12bObj;


uint8_t client_rec(void * context, uint8_t * buf, uint8_t buf_len);

int mqtt_message_cb(struct _MqttClient *client, MqttMessage *message, byte msg_new, byte msg_done){
	return 1;
}

int mqt_net_connect_cb (void *context, const char* host, word16 port, int timeout_ms){
	return 1;
}

int mqtt_net_read_cb(void *context, byte* buf, int buf_len, int timeout_ms){
	uint32_t enterTimestamp = GetTickCount();
	while (GetTickCount() - enterTimestamp < timeout_ms){
		uint8_t rxNb = RingBufferRead(&ringBuff, (uint8_t *)buf, buf_len);
		if (rxNb >0){
			return rxNb;
		}
	}
	return -1;
}

int mqtt_net_write_cb(void *context, const byte* buff, int buffLen, int timeout_ms){

	rfm12bObj_t * obj = (rfm12bObj_t*) context;
	Rfm12bStartSending(obj, (uint8_t *)buff, buffLen, BROKER_ADDR);
	return buffLen;
}


int mqtt_net_disconnect_cb(void *context){
	return 0;
}




uint8_t radio_receive (rfm12bObj_t* rfm12b, ringBuff_t * ringBuff){
	//here check if address we receive is our address and check who send this message
	uint8_t byteNb = 0;
		byteNb = rfm12b->completedRxBuff.dataNb;
		if (byteNb > 0){
			byteNb = (byteNb < R_BUFF_SIZE) ? byteNb : R_BUFF_SIZE;
			RingBufferWrite(ringBuff,  &rfm12b->completedRxBuff.data, byteNb);
		}
		rfm12b->completedRxBuff.dataNb = 0;
	return byteNb;
}





//
//
//
//uint8_t client_rec(void * context, uint8_t * buf, uint8_t buf_len){
//	rfm12bObj_t * obj = (rfm12bObj_t*) context;
//	uint8_t byteNb = rfm12bObj.completedRxBuff.dataNb;
//	if (byteNb > 0){
//		byteNb = (byteNb < buf_len) ? byteNb : buf_len;
//		memcpy (buf, obj->completedRxBuff.data, byteNb);
//	}
//	obj->completedRxBuff.dataNb = 0;
//	return byteNb;
//}





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



void EXTI9_5_IRQHandler (void){
	EXTI_ClearITPendingBit(EXTI_Line5);
	Rfm12bIrqCallback(&rfm12bObj);
	radio_receive (&rfm12bObj, &ringBuff);

}



void StartSystick(void){
	SysTick->LOAD = 72000 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE | SysTick_CTRL_TICKINT;
}


void SysTick_Handler(void){
	systickMsIRQ++;
}

 int main(){
	 	StartSystick();
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
	 	Rrm12bObjInit (&rfm12bObj, NODE_ADDR);

 		RingBufferInit(&ringBuff);

	 	RTC_Init();
	 	RtcClear();



		const char * infoTxt = "Start Mqtt client";
	 	SendUart1Dma(infoTxt, strlen(infoTxt));


	 		MqttNet net;
	 		MqttClient client;
	 		net.context = &rfm12bObj;
	 		net.connect = mqt_net_connect_cb;
	 		net.read = mqtt_net_read_cb;
	 		net.write = mqtt_net_write_cb;
	 		net.disconnect = mqtt_net_disconnect_cb;

	 		uint8_t tx_buf[BUF_SIZE_TX];
	 		uint8_t rx_buf[BUF_SIZE_RX];
	 		MqttClient_Init(&client, &net, mqtt_message_cb, tx_buf, BUF_SIZE_TX, rx_buf, BUF_SIZE_RX, CMD_TIMEOUT_MS);

	 		MqttConnect mqtt_con;
	 		mqtt_con.clean_session =0;
	 		mqtt_con.client_id = "rt1";
	 		mqtt_con.enable_lwt = 0;
	 		mqtt_con.keep_alive_sec =30;
	 		mqtt_con.stat = MQTT_MSG_BEGIN;
	 		mqtt_con.username ="bedroomTMP1";
	 		mqtt_con.password = "passw0rd";
	 		MqttClient_Connect(&client, &mqtt_con);

	 		const char* test_topic1 = "flat/livingroom/temp/1";
	 		const char* test_topic2 = "flat/bedroom/humidity/2";
	 		MqttTopic topics[2];
	 		topics[0].qos =1;
	 		topics[0].topic_filter = test_topic1;
	 	    topics[1].qos =1;
	 		topics[1].topic_filter = test_topic2;


	 		MqttSubscribe subscribe;
	 		subscribe.packet_id = 1;
	 		uint8_t topic_count = 2;
	 		subscribe.topic_count = topic_count;
	 		subscribe.topics = topics;
	 		subscribe.stat = MQTT_MSG_BEGIN;
	 		MqttClient_Subscribe(&client, &subscribe);






	 	while (1){


	 		 radio_receive (&rfm12bObj, &ringBuff);

	 		if (!(GPIOB->IDR & (1<<11))){
	 			uint8_t buff[] = "helloWorld1helloWorld2helloWorld3";
	 			Rfm12bStartSending(&rfm12bObj, buff, 30, BROKER_ADDR);
	 			_delay_ms(250);
	 		}
	 	}
 }


