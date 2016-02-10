#ifndef __HALL_H__
#define __HALL_H__

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

void 	HALLInit();
int  	HALLRead(void);
void  	readHALL( char  *pstrID, char  *pParam);
#endif
