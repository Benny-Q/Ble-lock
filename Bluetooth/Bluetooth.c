#include "bluetooth.h"


extern u8 Bluetooth_Rx_Buf[80] = {0xff};
extern u8 Bluetooth_Rx_Num = 0;


void Bluetooth_POWER_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BT_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

	GPIO_Init(BT_POWER_PORT, &GPIO_InitStructure);
}

void Bluetooth_RSTN_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BT_RSTN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

	GPIO_Init(BT_RSTN_PORT, &GPIO_InitStructure);

}

void Bluetooth_Control_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BT_CONTROL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

	GPIO_Init(BT_CONTROL_PORT, &GPIO_InitStructure);

}

void Bluetooth_Exit_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	
	GPIO_Init(USART2_RX_PORT,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource3);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	#if 0
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority= 1 ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	#endif
	NVIC_Init(&NVIC_InitStructure);


}

void Bluetooth_init(void)
{
	Bluetooth_POWER_GPIO();
	Bluetooth_RSTN_GPIO();
	Bluetooth_Control_GPIO();
	
	Bluetooth_POWER_ON;
	Bluetooth_RSEST_ON;
	Bluetooth_CONTROL_ON;
	//Usart2_Disable();
}



