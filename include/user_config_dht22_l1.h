/*
条件编译配置文件。
设备类型: DHT22，温湿度传感器
网络服务：云服务总线
*/

#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

#define CFG_HOLDER	0x00FF55A4
#define CFG_LOCATION	0x3C	/* Please don't change or if you know what you doing */

/*DEFAULT CONFIGURATIONS*/

//#define MQTT_HOST			"192.168.12.67"     // "mqtt.yourdomain.com" //or "192.168.11.1"
//#define MQTT_HOST			"192.168.12.25"
#define MQTT_HOST			"192.168.8.180"       //Lan

#define MQTT_PORT			1883 //8440
#define MQTT_BUF_SIZE		1024
#define MQTT_KEEPALIVE		120	 /*second*/

#define MQTT_CLIENT_ID		"DVES_%08X"
#define MQTT_USER			"DVES_USER"
#define MQTT_PASS			"DVES_PASS"


#define STA_SSID  "OpenThings" 		//"DVES_HOME"
#define STA_PASS  "supermap"  		//"wifipassword"
#define STA_TYPE AUTH_WPA2_PSK

#define MQTT_RECONNECT_TIMEOUT 	5	/*second*/

#define CLIENT_SSL_ENABLE


#define SM_SC_TYPE			SC_TYPE_ESPTOUCH // 
//#define SM_SC_TYPE		SC_TYPE_AIRKISS

#define SM_SENSOR_TYPE		1 	//  1---DHT22; 2--HALL; 3--PM2.5

#endif
