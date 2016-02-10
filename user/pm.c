#include "ets_sys.h"
#include "osapi.h"
#include "c_types.h"
#include "user_interface.h"
#include "gpio.h"
#include "adc.h"

#include "pm.h"

uint16 ICACHE_FLASH_ATTR PMRead(void)
{
    GPIO_OUTPUT_SET(2, 0);
    os_delay_us(280);

    uint16 nVolt = adc_read();
    //uint16 nVolt = system_adc_read();

    os_delay_us(40);
    GPIO_OUTPUT_SET(2, 1);
    os_delay_us(9680);

    GPIO_DIS_OUTPUT(2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);

    return  nVolt;
  }

void ICACHE_FLASH_ATTR PMInit() {
    //Set GPIO2 to output mode for  LED light
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
}

void  readPM( char  *pstrID, char  *pParam)
{
    PMInit();
    uint16 nVal = PMRead();
    //double VoltageRatio = 0.0048828125; // 5/1024   ===>Arduino
    //double VoltageRatio = 0.00322265625;  // 3.3 /1024   ===> ESP8266
    double dPM25 = ( 0.00322265625 * nVal * 0.17 - 0.1)*1000;
    int  nInteger=(int) dPM25;
    int  nFractional = (dPM25-nInteger)*100;

    os_sprintf(pParam, "OID:%s,PM:%d.%d,V:%d\r\n", pstrID, nInteger, nFractional, nVal  );
}