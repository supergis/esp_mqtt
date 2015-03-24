#include "ets_sys.h"
#include "osapi.h"
#include "c_types.h"
#include "user_interface.h"
#include "gpio.h"
#include "pm.h"

#include "adc.h"

#define MAXTIMINGS 10000
#define BREAKTIME 20
 
int ICACHE_FLASH_ATTR PMRead(void)
{
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(250000);
    GPIO_OUTPUT_SET(2, 0);
    os_delay_us(280);//20000);

    uint16 nadc = adc_read();

    os_delay_us(40);
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(9680);

    GPIO_DIS_OUTPUT(2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);

    //uint16 ICACHE_FLASH_ATTR adc_read(void)
    return  nadc;
      // if( GPIO_INPUT_GET(2) == 1 )
      // {
      //        return  1;
      // }
      // else
      // {
      //        return 0;
      // }

 /*   static float r[2];
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
 */   
}

void ICACHE_FLASH_ATTR PMInit() {
    //Set GPIO2 to output mode for  LED light
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
}

void  readPM( char  *pstrID, char  *pParam)
{
    HALLInit();
    int nVal = PMRead();
    //double VoltageRatio = 0.0048828125; // 5/1024
    double dPM25 = ( 0.0048828125 * nVal * 0.17 - 0.1)*1000;
    int  nInteger=(int) dPM25;
    int  nFractional = (dPM25-nInteger)*100;

    os_sprintf(pParam, "OID:%s,PM2.5:  %d.%d, V:%d\r\n", pstrID, nInteger, nFractional, (int)(0.48828125 * nVal )  );
}