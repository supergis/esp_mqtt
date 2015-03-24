/*********************************************
 *DHT22 Temprature and Humidity sensor for ESP8266.
 *Note: plseas use 5V power-supply, otherwise got 0.
 *Author: openthings@163.com.
 *copyright&GPL V2.
 *Last modified 2014-12-28.
 **********************************************/

#include "ets_sys.h"
#include "osapi.h"
#include "c_types.h"
#include "user_interface.h"
#include "gpio.h"
#include "dht22.h"

#define MAXTIMINGS 10000
#define BREAKTIME 20
 
float * ICACHE_FLASH_ATTR DHTRead(void)
{
    static float r[2];
    int counter = 0;
    int laststate = 1;
    int i = 0;
    int j = 0;
    int checksum = 0;
    //int bitidx = 0;
    //int bits[250];
    int data[100];
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(250000);
    GPIO_OUTPUT_SET(2, 0);
    os_delay_us(20000);
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(40);
    GPIO_DIS_OUTPUT(2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);

    // wait for pin to drop?
    while (GPIO_INPUT_GET(2) == 1 && i<100000) {
          os_delay_us(1);
          i++;
    }

    if(i==100000)
    	return r;

    // read data
    for (i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while ( GPIO_INPUT_GET(2) == laststate) {
            counter++;
                        os_delay_us(1);
            if (counter == 1000)
                break;
        }
        laststate = GPIO_INPUT_GET(2);
        if (counter == 1000) break;

        //bits[bitidx++] = counter;

        if ((i>3) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > BREAKTIME)
                data[j/8] |= 1;
            j++;
        }
    }

//    for (i=3; i<bitidx; i+=2) {
//        os_printf("bit %d: %d\n", i-3, bits[i]);
//        os_printf("bit %d: %d (%d)\n", i-2, bits[i+1], bits[i+1] > BREAKTIME);
//    }
//    os_printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x\n", j, data[0], data[1], data[2], data[3], data[4]);

    float temp_p, hum_p;
    if (j >= 39) {
        checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
        if (data[4] == checksum) {
            // checksum is valid
            hum_p = data[0] * 256 + data[1];
            hum_p /= 10;
            temp_p = (data[2] & 0x7F)* 256 + data[3];
            temp_p /= 10.0;
            if (data[2] & 0x80)
                temp_p *= -1;
			//os_printf("Temp =  %d *C, Hum = %d %%\n", (int)(temp_p*100), (int)(hum_p*100));
			r[0] = temp_p;
			r[1] = hum_p;
        }
    }
	return r;
}

void ICACHE_FLASH_ATTR DHTInit() {
    //Set GPIO2 to output mode for DHT22
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
}

// void ICACHE_FLASH_ATTR xc_cmd_CDHT22(uint8_t id, char *pParam)
// {
// 	char temp[64];
// 	//float te=25;
// 	//float hu=38;
// 	DHTInit();
// 	float * r = DHTRead();
// 	//console_printf("Temperature: %d *C, Humidity: %d %%\r\n", (int)(r[0]*100), (int)(r[1]*100));

// 	os_sprintf(temp, "#ID=0:T=%d,H=%d\r\n",(int)(r[0]*100), (int)(r[1]*100));
// 	//sendInfoAll(temp);
// }

//void ICACHE_FLASH_ATTR readDHT22( char  *pParam)
void  readDHT22( char  *pstrID, char  *pParam)
{
    DHTInit();
    float * r = DHTRead();

    //char temp[64];
    char* temp = pParam;
   //os_sprintf(temp, "#ID=2:T=%d,H=%d\r\n",(int)(pParam[0]*100), (int)(pParam[1]*100));
    int  t1 = ( int ) r[0];
    int  t2 = 0;
    if( t1 < 0 )
    {
        float ftemp = -r[0];
        t2 = ( int ) ( (r[0]+t1)*100);
    }
    else
    {
        t2 = ( int ) ( (r[0]-t1)*100);        
    }
    int  h1 = ( int ) r[1];
    int  h2 = ( int ) ( (r[1]-h1)*100);    
    //os_sprintf(temp, "OID:20201,H:%d,T:%d\r\n",(int)(pParam[1]*100), (int)(pParam[0]*100) );
    os_sprintf(temp, "OID:%s,H:%d.%d,T:%d.%d\r\n", pstrID, h1,h2,t1,t2);
    //sendInfoAll(temp);
}
