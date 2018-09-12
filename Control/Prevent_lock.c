#include "Prevent_lock.h"

void Prevent_lock_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN,ENABLE);
	
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

		GPIO_InitStructure.GPIO_Pin = Prevent_lock_PIN;
		GPIO_Init(Prevent_lock_PORT,&GPIO_InitStructure);
	
		RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);	
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource7);
	
		EXTI_InitStructure.EXTI_Line = EXTI_Line7;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;	
		NVIC_InitStructure.NVIC_IRQChannelPriority= 0x00 ;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
}

void Prevent_lock_Monitor(void)
{
	uint8_t Flag_Status = 0;
	Flag_Status = Flag_Inf[Audio_mode];
	if(Flag_Status)
	{
		Flag_Inf[Audio_mode] = 0;
	}
	
	if(GPIO_ReadInputDataBit(Prevent_lock_PORT,Prevent_lock_PIN))
	{
		#ifdef	__DEBUG2__
		printf("__PREVENT_status\r\n");
		#endif
		while(1)
		{			
			if(!GPIO_ReadInputDataBit(Prevent_lock_PORT,Prevent_lock_PIN))
			{
				break;
			}
			AudioPlay(AUDIO_PROMPT_BI);
		}
	}
	else if(__Prevent_lock_flag)
	{
		#ifdef	__DEBUG2__
		printf("PREVENT_open\r\n");
		#endif
		EXTI->IMR&=~EXTI_Line7;
		while(1)
		{
			AudioPlay(AUDIO_PROMPT_BI);
		}
	}
	Flag_Inf[Audio_mode] = Flag_Status;
}





