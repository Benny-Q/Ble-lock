#include "delay.h"


//void delay_init(void)
//{
//	SysTick->CTRL |= (1<<2);//ѡ���ڲ�ʱ��Դ(FCLK)
//	SysTick->CTRL &= ~(1<<1);//�ر��ж�
//	SysTick->CTRL &= ~(1<<0);//�ر�ϵͳ�δ�ʱ��
//}

//void delay_us(u32 nus)
//{
//	SysTick->VAL = 0;//�����־λ
//	SysTick->LOAD = nus*8;
//	SysTick->CTRL |= (1<<0);//�����δ�ʱ��
//	while(!(SysTick->CTRL&(1<<16)));//�ȴ�������0
//	SysTick->CTRL &= ~(1<<0);//�ر�ϵͳ�δ�ʱ��
//}

//void delay_ms(u16 nms)
//{
//	SysTick->VAL = 0;//�����־λ
//	SysTick->LOAD = nms*8000;
//	SysTick->CTRL |= (1<<0);//�����δ�ʱ��
//	while(!(SysTick->CTRL&(1<<16)));//�ȴ�������0
//	SysTick->CTRL &= ~(1<<0);//�ر�ϵͳ�δ�ʱ��
//}



/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
  *          ���õ��� 1us 10us 1ms �жϡ�     
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  


static __IO uint32_t TimingDelay;
 
/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init(void)
{

	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
		// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief   us��ʱ����,10usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us13148087
  * @retval  ��
  */                                                                                                                                                                                
void delay_us(__IO uint32_t nTime)
{ 
	#if 1
	uint32_t temp;
	SysTick->LOAD=US_DEALY_DIVISION*nTime;				//us delay divide fact: 8
	SysTick->CTRL=0X01;
	SysTick->VAL=0;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->CTRL=0;
	SysTick->VAL=0;

	#else
	
	TimingDelay = nTime;	
	SysTick->VAL=0x00;        					//��ռ�����
	
	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	
	#ifdef __DEBUG__
	//printf("=== delay_us systick start! ===\r\n");
	#endif
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;

	#endif
}

//��ʱms�ĺ����ú궨�壬��ͷ�ļ�bsp_SysTick.h��
//#define Delay_ms(x) Delay_us(100*x)	 //��λms

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	#if 0
	#ifdef __DEBUG__
	//printf("= TimingDelay=%d =\r\n",TimingDelay);
	#endif
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	#endif
}
void soft_delay_ms(uint32_t _ndelay)
{
	//u32	tmp_delay;
	uint32_t loop_delay;

	#if 1
	for (loop_delay=0; loop_delay < _ndelay; loop_delay++)
	{
		delay_us(1000);
	}
	#else
	for (loop_delay=0; loop_delay < _ndelay; loop_delay++)
	{
		tmp_delay = 7000;
		
		while(tmp_delay)
		{
			tmp_delay--;
		}
	}
	#endif
}
/*********************************************END OF FILE**********************/
