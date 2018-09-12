#ifndef  __DELAY_H__
#define __DELAY_H__


#include "mains.h"

#define US_DEALY_DIVISION		6


void SysTick_Init(void);
void delay_us(__IO uint32_t nTime);
#define delay_ms(x) delay_us(100*x)	 //µ¥Î»ms


extern void soft_delay_ms(uint32_t _ndelay);










#endif

