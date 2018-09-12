#include "bsp_Config.h"

void Config_Key_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN,ENABLE);
	
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		GPIO_InitStructure.GPIO_Pin = Config_PIN;
		GPIO_Init(Config_PORT,&GPIO_InitStructure);
	
		RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);	
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource6);
	
		EXTI_InitStructure.EXTI_Line = EXTI_Line6;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;	
		NVIC_InitStructure.NVIC_IRQChannelPriority= 1 ;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		__Config_flag = 0;
}



void Config_Key_Resume_To_Default_Setting(void)
{
	TIM14_Start();
	while(!GPIO_ReadInputDataBit(Config_PORT,Config_PIN))
	{
		if(tim14_count_value == 10)
		{
			AudioInitialize();
			FingerPrint_Delete(300);
			Clear_Flash();
			RTC_WriteBackupRegister(RTC_BKP_DR0, 0xFFFF);
			AudioPlay(AUDIO_PROMPT_DATA_CLEARED);
			__disable_irq();
			NVIC_SystemReset();		//请求单片机重启
		}
	}
	TIM14_Stop();
}

void Reset_Factory_To_Default(void)
{
	AudioInitialize();
	FingerPrint_Delete(300);
	Clear_Flash();
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0xFFFF);
	soft_delay_ms(1000);
	AudioPlay(AUDIO_PROMPT_RECOVER_SETTING_SUCCESS); //出厂设置恢复成功
	__disable_irq();
	NVIC_SystemReset();		//请求单片机重启
}

