#ifndef __TIMEBASE_H
#define __TIMEBASE_H

#include "mains.h"


extern void TIM3_Configuration(void);
extern void TIM3_Start(void);
extern void TIM3_Stop(void);
extern u32 get_TIM3_Count(void);
extern void clear_TIM3_Count(void);
extern u8 tim3_count_value;

extern void TIM2_Configuration(void);
extern void TIM2_Start(void);
extern void TIM2_Stop(void);
extern u32 get_TIM2_Count(void);
extern void clear_TIM2_Count(void);
extern u8 tim2_count_value;

extern void TIM14_Configuration(void);
extern void TIM14_Start(void);
extern void TIM14_Stop(void);
extern u32 get_TIM14_Count(void);
extern void clear_TIM14_Count(void);
extern u8 tim14_count_value;

extern void time_pro(void);

#define 	T_F_2MS     0x01
#define     T_2MS       0x02
#define     T_50MS     0x04
#define     T_80MS     0x08
#define     T_110MS   0x10
#define     T_1000MS 0x20
#define     T_2000MS 0x40
#define     T_5000MS 0x80

#endif	/* TIMEBASE_H */
