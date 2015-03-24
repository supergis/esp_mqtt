#ifndef __PM25_H__
#define __PM25_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

void PMInit();
int    PMRead(void);
void  readPM( char  *pstrID, char  *pParam);

#endif
