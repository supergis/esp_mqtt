#ifndef __DHT22_H__
#define __DHT22_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

void DHTInit();
float * DHTRead(void);

//void ICACHE_FLASH_ATTR xc_cmd_CDHT22(uint8_t id, char *pParam);
//void ICACHE_FLASH_ATTR readDHT22( char  *pParam);
void readDHT22( char  *pstrID,  char  *pParam);
#endif
