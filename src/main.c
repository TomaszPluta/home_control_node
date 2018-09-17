
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




volatile rfm12bObj_t rfm12bObj;

uint8_t client_rec(uint8_t * buf, uint8_t buf_len);

int mqtt_message_cb(struct _MqttClient *client, MqttMessage *message, byte msg_new, byte msg_done){
	return 1;
}

int mqt_net_connect_cb (void *context, const char* host, word16 port, int timeout_ms){
	return 1;
}

int mqtt_net_read_cb(void *context, byte* buf, int buf_len, int timeout_ms){

	uint32_t enterTimestamp = xTaskGetTickCount();
	while (xTaskGetTickCount() - enterTimestamp  > timeout_ms){
		uint8_t rxNb = client_rec(buf, buf_len);
		if (rxNb >0){
			return rxNb;
		}
	}
	return -1;
}

int mqtt_net_write_cb(void *context, const byte* buff, int buffLen, int timeout_ms){

	rfm12bObj_t * obj = (rfm12bObj_t*) context;
	Rfm12bStartSending(obj, (uint8_t *)buff, buffLen);
	return buffLen;
}

int mqtt_net_disconnect_cb(void *context){
	return 0;
}




uint8_t client_rec(uint8_t * buf, uint8_t buf_len){
	uint8_t byteNb = rfm12bObj.completedRxBuff.dataNb;
	if (byteNb > 0){
		byteNb = (byteNb < buf_len) ? byteNb : buf_len;
		memcpy (buf, rfm12bObj.completedRxBuff.data, byteNb);
	}
	return byteNb;
}


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



void StartSystick(voi){
	SysTick->LOAD = 72000 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE;
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
	 	Rrm12bObjInit (&rfm12bObj);

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

	 		MqttClient_Subscribe(&client, &subscribe);








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
