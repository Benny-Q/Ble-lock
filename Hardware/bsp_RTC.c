#include "bsp_RTC.h"
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(Common_Leap[1][(a) - 1])

static uint8_t Common_Leap[2][12]= {
									{31,29,31,30,31,30,31,31,30,31,30,31},    //闰年
									{31,28,31,30,31,30,31,31,30,31,30,31}	  //平年
								   };


RTC_InitTypeDef RTC_InitStructure;
RTC_DateTypeDef RTC_DataStructure;
RTC_TimeTypeDef RTC_TimeStructure;


__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

uint8_t Time_data[12] = {0};
uint8_t Time_data_CNT = 0;

void RTC_Configuration(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	PWR_BackupAccessCmd(ENABLE);	
/**内部时钟**/
#if defined (RTC_CLOCK_SOURCE_LSI)
		RCC_LSICmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		SynchPrediv = 0x18F;
		AsynchPrediv = 0x63;
		
/**外部时钟**/		
#elif defined (RTC_CLOCK_SOURCE_LSE)
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		SynchPrediv = 0xFF;
		AsynchPrediv = 0x7F;
#endif 

		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();
		
}


void RTC_Initializes(void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F0)
	{
		RTC_Configuration();
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		if(RTC_Init(&RTC_InitStructure) == ERROR)
		{
			while(1);
		}
		
		RTC_DataStructure.RTC_Year = 18;
		RTC_DataStructure.RTC_Month = 1;
		RTC_DataStructure.RTC_Date = 1;
		RTC_SetDate(RTC_Format_BIN,&RTC_DataStructure);
		
		RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours = 0;
		RTC_TimeStructure.RTC_Minutes = 0;
		RTC_TimeStructure.RTC_Seconds = 0;
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F0);
	}
	else
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    	PWR_BackupAccessCmd(ENABLE);
		RCC_LSEConfig(RCC_LSE_ON);
		RTC_WaitForSynchro();
	}
}

void RTC_AlarmEXTI(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	NVIC_InitTypeDef  NVIC_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	//RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0x000001ff, RTC_AlarmSubSecondMask_All);
	RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0x000fffff, RTC_AlarmSubSecondMask_All);
	
	/* Clear the RTC Alarm Flag */
	RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);

	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void Clean_Time_Value(void)
{
	uint8_t loop = 0;
	Time_data_CNT = 0;
	for(loop = 0; loop<12 ;loop++)
	{
		Time_data[loop] = 0;
	}
}

uint8_t Calendar_Calculation(void)
{
	uint16_t year, month, day, hour, minute ;
	year = Time_data[0]*1000+ Time_data[1]*100+ Time_data[2]*10+ Time_data[3];
	month = Time_data[4]*10+ Time_data[5];
	day = Time_data[6]*10+ Time_data[7];
	hour = Time_data[8]*10+ Time_data[9];
	minute = Time_data[10]*10 +Time_data[11];
	if(year > 1999 && year < 2100)
	{
		 if(((year%4==0)&&(year%100!=0)) || (year%400==0))
		 {
		 	year = Leap_Year;
		 }
		 else
		 {
			year = Common_year;
		 }
	}
	else
	{
		return Set_Failed;
	}


	if(month > 0 && month < 13)
	{
		if(Common_Leap[year][month-1] >= day && day != 0)
		{
			if(hour < 24)
			{
				if(minute < 60)
				{
					return Set_Success;
				}
				else
				{
					return Set_Failed;
				}
			}
			else
			{
				return Set_Failed;
			}
		}
		else
		{
			return Set_Failed;
		}
	}
	else
	{
		return Set_Failed;
	}
}


void GetTime(u8 *time)
{
	RTC_GetDate(RTC_Format_BIN,&RTC_DataStructure);	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);	

	*time = RTC_DataStructure.RTC_Year;  
	*(time+1) = RTC_DataStructure.RTC_Month;  
	*(time+2) = RTC_DataStructure.RTC_Date;
	
	*(time+3) = RTC_TimeStructure.RTC_Hours; 
	*(time+4) = RTC_TimeStructure.RTC_Minutes;
	*(time+5) = RTC_TimeStructure.RTC_Seconds;
}

#if 1
uint32_t MkTime(void)
{

	uint8_t temp_time[6];
	uint16_t year = 0, month = 0;
	uint16_t day = 0;
	uint8_t i;
	uint32_t data;
	RTC_GetDate(RTC_Format_BIN,&RTC_DataStructure);	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);	

	temp_time[0] = RTC_DataStructure.RTC_Year;  
	temp_time[1] = RTC_DataStructure.RTC_Month;  
	temp_time[2] = RTC_DataStructure.RTC_Date;
	
	temp_time[3] = RTC_TimeStructure.RTC_Hours; 
	temp_time[4] = RTC_TimeStructure.RTC_Minutes;
	temp_time[5] = RTC_TimeStructure.RTC_Seconds;

	year = temp_time[0]+2000;
	if(((year%4==0)&&(year%100!=0)) || (year%400==0))
    {
    	i = Leap_Year;
    }
    else
    {
   		i = Common_year;
    }

	year = temp_time[0] - 16;
	for(month = 0; month < (temp_time[1]-1); month++)
	{
		day = day + Common_Leap[i][month];
	}
	if(year%4 == 0)
	{
		day = day + temp_time[2] - 1;
	}
	else
	{
		day = (year%4)*365 + 1 + day + temp_time[2] - 1;

	}
	data = ((year/4)*1461 + day)*24*3600 + temp_time[3]*3600 + temp_time[4]*60 + temp_time[5];
	return data;

}


int Hour_Cal(uint16_t CNT, uint8_t year_res)
{

	int result;
	uint8_t temp_time[6];
	uint16_t hour;
	uint16_t year = 0, month = 0;
	uint16_t day = 0;
	uint8_t i;
	RTC_GetDate(RTC_Format_BIN,&RTC_DataStructure);	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);	

	
	temp_time[0] = RTC_DataStructure.RTC_Year;  
	temp_time[1] = RTC_DataStructure.RTC_Month;  
	temp_time[2] = RTC_DataStructure.RTC_Date;
	
	temp_time[3] = RTC_TimeStructure.RTC_Hours; 


	year = temp_time[0]+2000;
	if(((year%4==0)&&(year%100!=0)) || (year%400==0))
    {
    	i = Leap_Year;
    }
    else
    {
   		i = Common_year;
    }
	for(month = 0; month < (temp_time[1]-1); month++)
	{
		day = day + Common_Leap[i][month];
	}

	hour = (day+temp_time[2]-1)*24 + temp_time[3];

	if(i == Leap_Year)
	{
		if(year_res)
		{
			if((CNT+8784)>hour)
			{
				result = 0x11;
			}
			else
			{
				result = -1;
			}
		}
		else
		{
			if(CNT>hour)
			{
				result = 0x11;
			}
			else
			{
				result = -1;
			}
		}
	}
	else if(i == Common_year)
	{
		if(year_res)
		{
			if((CNT+8760)>hour)
			{
				result = 0x11;
			}
			else
			{
				result = -1;
			}
		}
		else
		{
			if(CNT>hour)
			{
				result = 0x11;
			}
			else
			{
				result = -1;
			}
		}
	}
	return result;
}



void time_to(uint32_t Second, uint8_t *data)
{
#if 1
	u32    i , month = 0 ,day = 0 ,hms = 0 ;
	


	day = Second / 86400;			/* 有多少天 */
	hms = Second % 86400;			/* 今天的时间，单位s */
	
	/* Hours, minutes, seconds are easy */
	*(data+3) = hms / 3600;
	*(data+4) = (hms % 3600) / 60;
	*(data+5) = (hms % 3600) % 60;

	
	if(day%1461 < 366)
	{
		*data = (day/1461)*4;
		for(i = 0; i < 12; i++)
		{
			month = month+Common_Leap[0][i];
			if( month > (day%1461))
			{
				*(data+1) = i+1;
				*(data+2) = Common_Leap[0][i] - (month - day%1461);
				return;
			}
		}
	}
	else
	{
		*data = (day/1461)*4 + (day%1461 - 366)/365 +1;
		for(i = 0; i < 12; i++)
		{
			month = month+Common_Leap[1][i];
			if( month > ((day%1461)-366)%365)
			{
				*(data+1) = i+1;
				*(data+2) = Common_Leap[0][i] - (month - (((day%1461)-366)%365+1));
				return;
			}
		}
	}
#else

	register u32	i;
	register long	hms, day;

	day = Second / 864000; 		/* 有多少天 */
	hms = Second % 86400; 		/* 今天的时间，单位s */

	/* Hours, minutes, seconds are easy */
	*(data+3) = hms / 3600;
	*(data+4) = (hms % 3600) / 60;
	*(data+5) = (hms % 3600) % 60;

	/* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
	for (i = 2016; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	*data = i;

	/* Number of months in days left */ /*计算当前的月份*/
	if (leapyear(*data)) {
		days_in_month(2) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(2) = 28;
	*(data+1) = i;

	/* Days are what is left over (+1) from all that. *//*计算当前日期*/
	*(data+2)  = day + 1;

	/*
	 * Determine the day of week
	 */
	//GregorianDay(tm);  //不用转换成星期



#endif
}
#endif 
