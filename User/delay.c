#include "delay.h"


//void delay_init(void)
//{
//	SysTick->CTRL |= (1<<2);//选择内部时钟源(FCLK)
//	SysTick->CTRL &= ~(1<<1);//关闭中断
//	SysTick->CTRL &= ~(1<<0);//关闭系统滴答定时器
//}

//void delay_us(u32 nus)
//{
//	SysTick->VAL = 0;//清除标志位
//	SysTick->LOAD = nus*8;
//	SysTick->CTRL |= (1<<0);//启动滴答定时器
//	while(!(SysTick->CTRL&(1<<16)));//等待计数到0
//	SysTick->CTRL &= ~(1<<0);//关闭系统滴答定时器
//}

//void delay_ms(u16 nms)
//{
//	SysTick->VAL = 0;//清除标志位
//	SysTick->LOAD = nms*8000;
//	SysTick->CTRL |= (1<<0);//启动滴答定时器
//	while(!(SysTick->CTRL&(1<<16)));//等待计数到0
//	SysTick->CTRL &= ~(1<<0);//关闭系统滴答定时器
//}



/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
  *          常用的有 1us 10us 1ms 中断。     
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  


static __IO uint32_t TimingDelay;
 
/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{

	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief   us延时程序,10us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us13148087
  * @retval  无
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
	SysTick->VAL=0x00;        					//清空计数器
	
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	
	#ifdef __DEBUG__
	//printf("=== delay_us systick start! ===\r\n");
	#endif
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;

	#endif
}

//延时ms的函数用宏定义，在头文件bsp_SysTick.h里
//#define Delay_ms(x) Delay_us(100*x)	 //单位ms

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
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
