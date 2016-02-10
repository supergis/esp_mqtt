#ifndef __DHT22_H__
#define __DHT22_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

void DHTInit();
float * DHTRead(void);
int DHTRead11(uint8_t pin);
void readDHT11( char  *pstrID,  char  *pParam);
#endif
