#ifndef __PM25_H__
#define __PM25_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "adc.h"

void 	PMInit();
uint16  PMRead(void);
void 	readPM( char  *pstrID, char  *pParam);
#endif
