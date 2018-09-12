#include "mains.h"
#include "bsp_key.h"
#include "FingerPrint.h"


u8 Sys_sleepflag = 1;
u8 PSWD_input_status = 0;
//KeyInf_t KeyInf[20];
u8 __wake_up_flag = 0;
u8 __Config_flag = 0;
u8 __Prevent_lock_flag = 0;
u8 Open_Info_flag_temp[2];
u16 Open_Info_Numb_temp[2]; 
u32 time_info_start=0; 
u32 time_info_end=0; 

extern volatile unsigned char B3TimeFlg;

u8 _get_system_sleep_status(void)
{
	return Sys_sleepflag;
	
}

void _set_system_sleep_status(void)
{
	Sys_sleepflag =0;
	return;
}

void _clear_system_sleep_status(void)
{
	Sys_sleepflag =1;
	return;
}


void WWDG_Feed(void)
{
	if(((WWDG->CR & 0x7F) < WWDG->CFR) && ((WWDG->CR & 0x7F)>0x3F))
    {
      WWDG_SetCounter(0xF0);
    }
}

void WWDG_Config(void)
{
	//IWDG_SetReload

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_WWDGEN,ENABLE);
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{
		RCC_ClearFlag();
	}
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x7F);
	WWDG_Enable(0x7F);
}

void IWDG_Config(void)
{
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		RCC_ClearFlag();
	}
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	IWDG_SetReload(4000);
	IWDG_ReloadCounter();

	IWDG_Enable();
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
u8 tempStr[6]={'F','i','n','g','e','r'};
void batInitCheck(void)
{
	if (BatyLow_flag == 3)
	{
	#ifdef __VOICE_SUPPORT__
		AudioPlay(AUDIO_PROMPT_BATTERY_LOW);//电池电量低，请更换电池
	#endif
	}	
	else if (BatyLow_flag == 2)
	{
	#ifdef __VOICE_SUPPORT__
		AudioPlay(AUDIO_PROMPT_BATTERY_LOW);
	    while(1) 
		{
            time_pro();
		    touchPro();	
		    sleepPro();
		    ErroPro();
		}
	#endif
	}	
	else if (BatyLow_flag == 1)
	{
	#ifdef __VOICE_SUPPORT__
		AudioPlay(AUDIO_PROMPT_BATTERY_LOW);
	     while(1) 
		{
            time_pro();
		    sleepPro();
		    ErroPro();
		}
	#endif
	}	
}
int main(){

	Usart1_init();
	Usart2_init();
	//printf("@@\r\n");
/*----------时钟-----------*/	
	TIM2_Configuration();
	TIM3_Configuration();
	SysTick_Init();
	RTC_Initializes();
	RTC_AlarmEXTI();

/*------语音------*/
	AudioInitialize();
/*------防拆检测------*/
#ifdef __PREVENT__
	Prevent_lock_GPIO_Config();
	Prevent_lock_Monitor();
#endif
	Motor_GPIO_Init();

/*---------OLED屏幕--------*/	
	LED_Config();
/*------滑盖------*/
	#ifdef __HALL_SUPPORT__
	Hall_GPIO_Init();
	#endif
	
/*------键盘------*/	
	#ifdef __TOUCH_KEY_SUPPORT__
	APT_Init();
	#endif

	#ifdef __BLUETOOTH_SUPPORT__
	Bluetooth_init();
	soft_delay_ms(500);
	#endif
/*-------指纹--------*/
	Fingerprint_init();
	
/*----------电量检测-----------*/
	ADC1_Init();
	battery_check(ADC_Value_Buff);
	batInitCheck();
/*-------------------------------------*/ 
	Config_Key_Init();
	figcntRxd = 0;
	
/****************
	test Hardware
****************/	
	check_Admin_info();
	LED_drive();
	__Config_flag = 0;
	#ifdef	__DEBUG2__
	printf("Start init finish\r\n");
	#endif
	AudioPlay(AUDIO_PROMPT_WELCOME);
//	bluetooth_init();
//  getBleMac();
/***************************************************
*														  *	
*				系统初始化完毕				  *	
*														  *
***************************************************/
	while(1)
	{
#ifdef	__DEBUG2__
		if(B3TimeFlg&T_1000MS)
#endif
		//IWDG_Feed();
		time_pro();
		keySetupPro();
		touchPro();	
		RFIDPro();
		fingerPro();
		sleepPro();
		ErroPro();
#ifdef __BLUETOOTH_SUPPORT__
		blePro();
#endif
	}
		
}
