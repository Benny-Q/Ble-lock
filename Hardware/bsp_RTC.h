#ifndef __RTC_H__
#define __RTC_H__


#include "mains.h"

extern RTC_InitTypeDef RTC_InitStructure;
extern RTC_DateTypeDef RTC_DataStructure;
extern RTC_TimeTypeDef RTC_TimeStructure;

extern uint8_t Time_data[12];

void RTC_Configuration(void);
void RTC_Initializes(void);
void RTC_AlarmEXTI(void);
uint8_t Calendar_Calculation(void);
void GetTime(u8 *time);
uint32_t MkTime(void);
void time_to(uint32_t Second, uint8_t *data);
int Hour_Cal(uint16_t CNT, uint8_t year_res);


typedef enum{
	Set_Success = 1,
	Set_Failed,
	Set_Over
}Time_Result_Enum;

typedef enum{
	Leap_Year,
	Common_year
}Year_Enum;


#endif 

