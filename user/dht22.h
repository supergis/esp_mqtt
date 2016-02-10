#ifndef __DHT22_H__
#define __DHT22_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

void DHTInit();
float * DHTRead(void);
void readDHT22( char  *pstrID,  char  *pParam);
#endif
