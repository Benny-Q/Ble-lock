/**
  ******************************************************************************
  * @file    stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mains.h"
#include "stm32f0xx_it.h"
extern volatile unsigned char B3TimeFlg;
/** @addtogroup STM32F0-Discovery_Demo
  * @{
  */

/** @addtogroup STM32F0XX_IT
  * @brief Interrupts driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) == SET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearFlag(EXTI_Line17);
#if 0		
		IWDG_Feed();
		if(_feed_dog_wake_up)
		{
			system_entry_sleep();
		}
#endif		
	}
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update ) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update );        //清中断标志

		tim2_count_value++;
		B3TimeFlg |= 0x01;

#if 0
		RTC_GetDate(RTC_Format_BIN,&RTC_DataStructure);
		printf("the time is : %d年 %d月 %d日 \r\n",2018+RTC_DataStructure.RTC_Year\
													  ,RTC_DataStructure.RTC_Month\
											 		   ,RTC_DataStructure.RTC_Date);

	
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
		printf("the time is : %0.2d时 %0.2d分 %0.2d秒 \r\n",RTC_TimeStructure.RTC_Hours\
														 ,RTC_TimeStructure.RTC_Minutes\
														 ,RTC_TimeStructure.RTC_Seconds);
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F0);
#endif 	
    }

}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update ) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update );        //清中断标志

		tim3_count_value++;
		
		//APT_Key_Long_Press_Scan();
		
	#ifdef __DEBUG__
		printf("TIM3_IRQHandler interrupt!\r\n");
	#endif
    }
}

void TIM14_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM14, TIM_IT_Update ) != RESET)
    {
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update );        //清中断标志
		tim14_count_value++;
    }



}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
void USART1_IRQHandler(void)
{
	
	#ifdef __FPRT_BYD__
	u8 ch,i;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE ) == SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		ch = USART_ReceiveData(USART1 );
		//printf("%x\r\n",ch);
		if (ch == 0xEF && Rev_finger.usefalg == 0)
		{
			Rev_finger.usefalg = 1;
		}
		else if (ch == 0x01 && Rev_finger.usefalg == 1)
		{
			Rev_finger.usefalg = 2;
		}
		else if (ch == 0xFF && Rev_finger.usefalg > 1 && Rev_finger.usefalg < 6)
		{
			Rev_finger.usefalg ++;
		}
		else if (ch == 0x07 && Rev_finger.usefalg == 6)
		{
			Rev_finger.usefalg = 7;
		}
		else if (Rev_finger.usefalg == 7)
		{
			Rev_finger.data[figcntRxd++] = ch;
		}
		else
		{
			
		}
	}
	if (figcntRxd > Rev_finger.data[1]+1 && Rev_finger.usefalg == 7)
	{
		for (i=0; i<6; i++)
		{
			if (g_finger[i].usefalg != 1)
			{
				g_finger[i].usefalg = 1;
				memcpy(g_finger[i].data, Rev_finger.data, 17);
				break;
			}
		}
		figcntRxd=0;
		memset(&Rev_finger.usefalg, 0, 18);
	}
	#else
	unsigned char tmp=0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE ) == SET)
	{
		tmp = USART_ReceiveData(USART1 );
		if ((figcntRxd >= 8)&&(tmp == 0xf5))
		{
			figcntRxd = 0;
			Rev_finger.usefalg=0;
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		Rev_finger.data[figcntRxd++] = tmp ;		//USART_ReceiveData(USART1 );
	}
	
	if (figcntRxd == 7)
	{
		Rev_finger.usefalg=1;
	}
	
	#endif
}
extern CommunicateType gBle;

void USART2_IRQHandler(void)
{
	#ifdef __BLUETOOTH_SUPPORT__
	u8 uTemp;
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET)
	{
		uTemp = USART_ReceiveData(USART2);

		if(gBle.uDataLen < 70)
		{
			gBle.CommunicateUartData[gBle.uDataLen] = uTemp;
			gBle.uDataLen ++;
		}

	}
	#else
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET)
	{
		USART_ReceiveData(USART2);
	}
	#endif
}
void EXTI0_1_IRQHandler(void)
{
	
}
extern u8 sleepFlg;
extern u8 sleepBuff;
//#define SUPPORT_TEST
void EXTI2_3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2 ) != RESET)  //12键触摸
    {
        //EXTI->IMR&=~EXTI_Line2;//屏蔽line2上的中断
		EXTI_ClearITPendingBit(EXTI_Line2);  //清中断标志位 PE4 触摸按键
		_set_APT_Touchkey_status(1);
		touchFlg |= TOUCH_DOWN;
		if((sleepFlg&SLEEP_READY)||(sleepFlg&SLEEP_START))
		{
			sleepFlg = SLEEP_WAKE_READY;
		}
		sleepBuff = 0;
    }

	if(EXTI_GetITStatus(EXTI_Line3 ) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		sleepBuff = 0;
		sleepFlg = SLEEP_WAKE_READY;
	}
}

void EXTI4_15_IRQHandler(void)
{

		

		#ifdef __HALL_SUPPORT__
		if(EXTI_GetITStatus(EXTI_Line4) !=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line4);//滑盖
			#ifndef SUPPORT_TEST
			if(GET_PORT_HALL==0)
			{
				sleepFlg = SLEEP_WAKE_READY;
			}
			else
			{
				sleepFlg = SLEEP_READY;
			}
			sleepBuff = 0;
			#endif
		}	
		#endif

		if(EXTI_GetITStatus(EXTI_Line6) !=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line6);//设置键
			sleepFlg = 0;
			sleepBuff = 0;
			#ifdef	__DEBUG2__
			printf("setup key down\r\n");
			#endif
		}

		#ifdef __PREVENT__
		if(EXTI_GetITStatus(EXTI_Line7) !=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line7);
			soft_delay_ms(50);
			if(GPIO_ReadInputDataBit(Prevent_lock_PORT,Prevent_lock_PIN))
			{
				__Prevent_lock_flag = 1;
			}
			EXTI_ClearFlag(EXTI_Line7);
		}
		#endif
		
		if(EXTI_GetITStatus(EXTI_Line11) !=RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line11);
			#ifndef SUPPORT_TEST
			sleepFlg = 0;
			sleepBuff = 0;
			#endif
		}

		
		
}
