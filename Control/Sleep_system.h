#ifndef __WAKE_UP_H__
#define __WAKE_UP_H__


#include "mains.h"

void Wake_up_init(FunctionalState NewState);
extern uint8_t Guide_key(void);
extern void wake_up(void);
extern void sleep_ctrl(void);
extern void sleep_ctrl_2(void);
extern void SYSCLKConfig_STOP(void);
#define SLEEP_WAKE_READY	4
#define SLEEP_READY			2
#define SLEEP_START			1
extern void sleepPro(void);
extern void ErroPro(void);
#endif

