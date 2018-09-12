#ifndef __BSPADC_H_
#define __BSPADC_H_

#include "mains.h"
#include "stm32f0xx_adc.h"

#define ADC_Vref 3.3
#define ADC_DIV_R1 1000
#define ADC_DIV_R2 1000

#if 1
#define Battery_Level_6 5.0
#define Battery_Level_5 4.8
#define Battery_Level_4 4.6
#define Battery_Level_3 4.4
#define Battery_Level_2 4.2
#define Battery_Level_1 3.9
#define Battery_Level_0 3.6
#else
#define Battery_Level_6 2.4
#define Battery_Level_5 2.3
#define Battery_Level_4 2.2
#define Battery_Level_3 2.1
#define Battery_Level_2 2.0
#define Battery_Level_1 1.85
#define Battery_Level_0 1.7
#endif
void ADC1_Init(void);
void ADC_Mode_Config(void);
void ADC2_Mode_Config(void);

void battery_check(uint32_t ADC_PC2_ConvertedValue);
uint32_t ADC_Check(void);

extern uint32_t ADC_Value_Buff;
extern volatile u8  BatyLow_flag;

#endif
