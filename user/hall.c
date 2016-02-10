#include "ets_sys.h"
#include "osapi.h"
#include "c_types.h"
#include "user_interface.h"
#include "gpio.h"
#include "hall.h"

#define MAXTIMINGS 10000
#define BREAKTIME 20
 
int ICACHE_FLASH_ATTR HALLRead(void)
{
      if( GPIO_INPUT_GET(2) == 1 )
      {
             return  1;
      }
      else
      {
             return 0;
      }
}

void ICACHE_FLASH_ATTR HALLInit() {
    //Set GPIO2 to output mode for DHT22
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO2_U);
}

void  readHALL( char  *pstrID, char  *pParam)
{
    HALLInit();
    int nVal = HALLRead();

    os_sprintf(pParam, "OID:%s,HALL:%d\r\n", pstrID, nVal );
}