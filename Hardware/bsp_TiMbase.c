#include "bsp_TiMbase.h" 

volatile unsigned char B3TimeFlg=0; 		  
unsigned char Time2MsBuff=0;		 
unsigned char EventFlg=0;			 
unsigned char Time50MsBuff=0;		 
unsigned char Time80MsBuff=0;		 
unsigned char Time110MsBuff=0;		 
unsigned char Time1000MsBuff=0; 	 
unsigned char Time2000MsBuff=0; 	 
unsigned char BeepFlg=0;							
unsigned char BeepDelayBuff=0;							
unsigned char BeepLedRunMaxBuff=0;							

u8 tim3_count_value;
//TIMER2 for system sleep
u8 tim2_count_value;
u8 tim14_count_value;



/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--63 
 * 中断周期为 = 1/(64MHZ /64) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */

void clear_TIM3_Count(void)
{
	tim3_count_value =0;
}
uint32_t get_TIM3_Count(void)
{
	return tim3_count_value;
}
void TIM3_Start(void)
{
	#ifdef __DEBUG__
	printf("TIM3 is start!!!!\r\n");
	#endif
	clear_TIM3_Count();
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);	
}

void TIM3_Stop(void)
{
	clear_TIM3_Count();
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	
	TIM_Cmd(TIM3, DISABLE);	

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);

	
	#ifdef __DEBUG__
	printf("++++ TIM3 is stopped!  tim3_count_value =%d \r\n",tim3_count_value);
	#endif
}


void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
		
		/* 设置TIM2CLK 为 64MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN, ENABLE);
    
    //TIM_DeInit(TIM2);
	  /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period=299; //999+1=1000		//50mS 
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	  /* 时钟预分频数为64 */
    TIM_TimeBaseStructure.TIM_Prescaler= 47999;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM3, ENABLE);																		
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/
    
    //使能TIM2中断通道,TIM2中断优先级配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    

	TIM3_Stop();
	
}


void clear_TIM2_Count(void)
{
	tim2_count_value =0;
}
uint32_t get_TIM2_Count(void)
{
	return tim2_count_value;
}

void TIM2_Start(void)
{
	
	clear_TIM2_Count();
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);	
}

void TIM2_Stop(void)
{
	clear_TIM2_Count();
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	
	TIM_Cmd(TIM2, DISABLE);	

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

	
	#ifdef __DEBUG__
	printf("++++ TIM2 is stopped!  tim2_count_value =%d \r\n",tim2_count_value);
	printf("++++ system entry sleep! \r\n");
	#endif
}



void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
		
		/* 设置TIM2CLK 为 64MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    //TIM_DeInit(TIM2);
	  /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period=49; //999+1=1000		//50mS 
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	  /* 时钟预分频数为64 */
    TIM_TimeBaseStructure.TIM_Prescaler= 47999;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/
    
    //使能TIM2中断通道,TIM2中断优先级配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    

 	
}


void clear_TIM14_Count(void)
{
	tim14_count_value =0;
}
uint32_t get_TIM14_Count(void)
{
	return tim14_count_value;
}

void TIM14_Start(void)
{
	
	clear_TIM14_Count();
	TIM14_Configuration();
	TIM_ClearFlag(TIM14, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM14, ENABLE);	
}

void TIM14_Stop(void)
{
	clear_TIM14_Count();
    TIM_ITConfig(TIM14,TIM_IT_Update,DISABLE);
	
	TIM_Cmd(TIM14, DISABLE);	

	TIM_ClearFlag(TIM14, TIM_FLAG_Update);

}



void TIM14_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
		
		/* 设置TIM2CLK 为 64MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    
    //TIM_DeInit(TIM2);
	  /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period=999; //999+1=1000		//50mS 
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	  /* 时钟预分频数为64 */
    TIM_TimeBaseStructure.TIM_Prescaler= 47999;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM14, ENABLE);																		

    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    

 	TIM14_Stop();
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--63 
 * 中断周期为 = 1/(64MHZ /64) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */

void time_pro(void)
{
	if(B3TimeFlg&T_2MS)
	{
		B3TimeFlg&=~T_2MS;
	}
	if(B3TimeFlg&T_50MS)
	{
		B3TimeFlg&=~T_50MS;
	}
	if(B3TimeFlg&T_80MS)
	{
		B3TimeFlg&=~T_80MS;
	}
	if(B3TimeFlg&T_110MS)
	{
		B3TimeFlg&=~T_110MS;
	}
	if(B3TimeFlg&T_1000MS)
	{
		B3TimeFlg&=~T_1000MS;
	}
	if(B3TimeFlg&T_2000MS)
	{
		B3TimeFlg&=~T_2000MS;
	}
	if(B3TimeFlg&T_5000MS)
	{
		B3TimeFlg&=~T_5000MS;
	}
	if((B3TimeFlg&T_F_2MS)==0)
	{
		return;
	}
	B3TimeFlg|=T_2MS;
	B3TimeFlg&=~T_F_2MS;
	B3TimeFlg |= T_50MS;
	B3TimeFlg |= T_80MS;
	Time110MsBuff ++;
	if(Time110MsBuff >= 2)
	{
		Time110MsBuff = 0;
		B3TimeFlg |= T_110MS;
	}
	if(B3TimeFlg&T_50MS)
	{
		Time50MsBuff ++;
		if(Time50MsBuff >= 20)
		{
			Time50MsBuff = 0;
			B3TimeFlg |= T_1000MS;
		}
	}	
	if(B3TimeFlg&T_50MS)
	{
		Time1000MsBuff ++;
		if(Time1000MsBuff >= 40)
		{
			Time1000MsBuff = 0;
			B3TimeFlg |= T_2000MS;
		}
	}	
	if(B3TimeFlg&T_50MS)
	{
		Time2000MsBuff ++;
		if(Time2000MsBuff >= 100)
		{
			Time2000MsBuff = 0;
			B3TimeFlg |= T_5000MS;
		}
	}	
	
}
/*********************************************END OF FILE**********************/

