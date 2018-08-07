/*
 * ThreadCommunication.c
 *
 *  Created on: 31.07.2018
 *      Author: tomek
 */


#include "FreeRTOS.h"
#include "ThreadCommunication.h"
#include "queue.h"


#include "mqtt_client.h"
#include "systemDefines.h"


QueueHandle_t externalMsgQueue;

int mqtt_message_cb(struct _MqttClient *client, MqttMessage *message, byte msg_new, byte msg_done){
	return 1;
}

int mqt_net_connect_cb (void *context, const char* host, word16 port, int timeout_ms){
	return 1;
}

int mqtt_net_read_cb(void *context, byte* buf, int buf_len, int timeout_ms){
	return 1;
}

int mqtt_net_write_cb(void *context, const byte* buf, int buf_len, int timeout_ms){
	return 0;
}

int mqtt_net_disconnect_cb(void *context){
	return 0;
}


void ThreadCommunication ( void * pvParameters )
{


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

	MqttClient_Subscribe(&client, &subscribe);
	for (;;) {

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
		vTaskDelay(100);
	}
}
