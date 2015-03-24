/* main.c -- MQTT client example
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"

#include "smartconfig.h"

//#include "../dht22/dht22.h"
#include "dht22.h"

#include "hall.h"

MQTT_Client mqttClient;
char  g_pDataBuf[64];
char g_pstrID[10];// = "20201";
 
 static ETSTimer SmSensor;
 static ETSTimer SmSmartconfig;
 static int  nSmartconfigIndex;

static void  PublishData(void *arg)
{
	MQTT_Client* pClient = (MQTT_Client*)arg;
	if( pClient !=0 )
	{
		int8	datalen= 0;
		//readDHT22( g_pstrID, g_pDataBuf ); //"20208"
		//readHALL( g_pstrID, g_pDataBuf ); //"20208"
		readPM( g_pstrID, g_pDataBuf ); 
		datalen = strlen( g_pDataBuf );

		INFO("MQTT data: %s \r\n", g_pDataBuf );

//-----------------
		INFO("ssid: %s \r\n", sysCfg.sta_ssid );
		INFO("pw: %s \r\n", sysCfg.sta_pwd );
//-----------------	

		MQTT_Publish( pClient, "/sm/sensors", g_pDataBuf, datalen, 0, 0);
	}
}

void wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	}
}
void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	//MQTT_Subscribe(&mqttClient, "/sm/sensors");
	MQTT_Subscribe(&mqttClient, "/sm/cmd");
}

void mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	//char  dataBuf[64]; //topicBuf[64],
	MQTT_Client* client = (MQTT_Client*)args;

INFO("MQTT data: %s \r\n", data );

	//os_memcpy(topicBuf, topic, topic_len);
	//topicBuf[topic_len] = 0;

	//os_memcpy(dataBuf, data, data_len);
	//dataBuf[data_len] = 0;

	//INFO("MQTT topic: %s, data: %s \r\n", topicBuf, dataBuf);

	/* Echo back to /echo channel*/
	//MQTT_Publish(client, "/echo", dataBuf, data_len, 0, 0);
	//MQTT_Publish(client, "/sm/sensors", dataBuf, data_len, 0, 0);

	//MQTT_Publish(client, "/echo", data, data_len, 0, 0);

	os_memcpy(g_pDataBuf, data, data_len);
	g_pDataBuf[data_len] = 0;
//-------------------------------------------------------------------
	if( strcmp(g_pDataBuf,"#get#")==0 )
	{// Tengsw 响应外部的命令，发回一个数据吧
		//char      dataTemp[64];
		int8	datalen= 0;
		
		//os_sprintf(temp, "TCPCLIENT TO:%s:%d\r\n", server, port);
		//readDHT22( g_pstrID, g_pDataBuf ); //"20208"
		//readHALL( g_pstrID, g_pDataBuf ); //"20208"
		readPM( g_pstrID, g_pDataBuf ); //"20208"

		//os_sprintf( dataTemp, "OID:20208,H:16.67,T:26.53\r\n" );
		datalen = strlen( g_pDataBuf );

		INFO("MQTT data: %s \r\n", g_pDataBuf );
	
		MQTT_Publish(client, "/sm/sensors", g_pDataBuf, datalen, 0, 0);
	}
}


void ICACHE_FLASH_ATTR
smartconfig_done(void *data)
{
	struct station_config *sta_conf = data;

	//wifi_station_set_config(sta_conf);
	//sysCfg.sta_ssid, sysCfg.sta_pwd
	CFG_Load();
	INFO("ssid: %s \r\n", sta_conf->ssid );
	INFO("pw: %s \r\n", sta_conf->password );
		
	os_sprintf(sysCfg.sta_ssid, "%s", sta_conf->ssid );
	os_sprintf(sysCfg.sta_pwd, "%s", sta_conf->password );  	
	CFG_Save();
//-----------------
//-----------------	
    //uint8 ssid[32];
    //uint8 password[64];

	wifi_station_disconnect();

	nSmartconfigIndex=20;
	//wifi_station_connect();

	//smartconfig_stop();

	//WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
}

static void  SmartConfigure(void *arg)
{
	if( nSmartconfigIndex == 0 )
	{
		INFO("smartconfig_start  \r\n" );
		smartconfig_start(SC_TYPE_ESPTOUCH, smartconfig_done);
		//smartconfig_start(SC_TYPE_AIRKISS, smartconfig_done);
	}
	else if( nSmartconfigIndex == 30 )
	{
		INFO("smartconfig_stop \r\n" );
		os_timer_disarm(&SmSmartconfig);

		smartconfig_stop();

		WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
	}
	nSmartconfigIndex=nSmartconfigIndex+1;
	INFO("INDEX: %d \r\n",  nSmartconfigIndex );
}

void user_init(void)
{
	nSmartconfigIndex = 0;

	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);
	
	CFG_Load();

               INFO("================\r\n" );
	INFO("ssid: %s \r\n", sysCfg.sta_ssid );
	INFO("pw: %s \r\n", sysCfg.sta_pwd );

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

	INFO("\r\nSystem started ...\r\n");

               // Tengsw 每五秒发一个数据吧
	memset( g_pstrID, 0, sizeof(g_pstrID) );
	//os_sprintf( g_pstrID, "%s", "20202" );
	//os_sprintf( g_pstrID, "%08X", system_get_chip_id() );
	os_sprintf( g_pstrID, "%u", system_get_chip_id() );

	os_timer_setfn(&SmSensor, (os_timer_func_t *)PublishData, &mqttClient );
	os_timer_arm(&SmSensor, 5000, 1);

	os_timer_setfn(&SmSmartconfig, (os_timer_func_t *)SmartConfigure, 0  );
	os_timer_arm(&SmSmartconfig, 2000, 1);
}
