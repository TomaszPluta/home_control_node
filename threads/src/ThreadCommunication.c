/*
 * ThreadCommunication.c
 *
 *  Created on: 31.07.2018
 *      Author: tomek
 */

#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "ThreadCommunication.h"
#include "queue.h"

#include "nrf24.h"

#include "mqtt_client.h"
#include "systemDefines.h"


int client_rec(byte* buf){
	if(nrf24_dataReady())
	{
		if(nrf24_dataReady())
		{	uint8_t rxBytesNb;
			uint8_t data_array[32];
			nrf24_getData(data_array);
			rxBytesNb = l3_receive_packet(data_array, buf);
			if (rxBytesNb){

				GPIOC->BRR = GPIO_Pin_13;
				return rxBytesNb;
			}
		}
//		_delay_ms(10);
	}
	return 0;
}


QueueHandle_t externalMsgQueue;

int mqtt_message_cb(struct _MqttClient *client, MqttMessage *message, byte msg_new, byte msg_done){
	return 1;
}

int mqt_net_connect_cb (void *context, const char* host, word16 port, int timeout_ms){
	return 1;
}

int mqtt_net_read_cb(void *context, byte* buf, int buf_len, int timeout_ms){
	client_rec(buf);
}

int mqtt_net_write_cb(void *context, const byte* buf, int buf_len, int timeout_ms){
	l3_send_packet(0, (uint8_t*) buf, buf_len);
}

int mqtt_net_disconnect_cb(void *context){
	return 0;
}



void ThreadCommunication ( void * pvParameters )
{
	vPortEnterCritical();
	nrf24_init();
	nRF24_restore_defaults();
	vPortExitCritical();
	gpio_init();

	/* Channel #2 , payload length: 4 */
	const uint8_t channel = 2;
	const uint8_t payload_len = 32;
	nrf24_config(channel, payload_len);
	uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
	uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	/* Set the device addresses */
	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);




//		while (1) {
//			if (!(IsGpioHigh(GPIOB, 11))){
//				GPIOC->ODR ^= GPIO_Pin_13;
//				uint8_t data_array[32] = {4, 5, 6};
//			  	nrf24_send(data_array);
//			    _delay_ms(400);
//			}
//
//			if (nrf24_dataReady() == 1){
//				GPIOC->ODR &= ~GPIO_Pin_13;
//			}
//		}




	MqttNet net;
	MqttClient client;
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

	//MqttClient_Subscribe(&client, &subscribe);
	for (;;) {


//
//		if (!(IsGpioHigh(GPIOB, 11))){
//			MqttClient_Connect(&client, &mqtt_con);
//			GPIOC->ODR ^= GPIO_Pin_13;
//		}

		MqttClient_WaitMessage(&client,100);





		msgDataExt_t messageExt;
		if (xQueueReceive(externalMsgQueue, &messageExt, 0)){
			MqttPublish publishPckt;
			switch (messageExt.type){
			case lightLevel:
				publishPckt.topic_name = test_topic1;
				publishPckt.topic_name_len = sizeof(test_topic1);
			}


			 MqttClient_Publish(&client, &publishPckt);
		}

		//poll sevrer b ping and inform supervisor about broken connection
		vTaskDelay(1000);
	}
}
