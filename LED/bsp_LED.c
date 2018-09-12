#include "bsp_LED.H"

void LED_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOFEN,ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_Pin = LED_9_PIN | LED_10_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_3_PIN | LED_4_PIN | LED_11_PIN | LED_12_PIN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_6_PIN | LED_7_PIN | LED_8_PIN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_5_PIN;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_1_PIN | LED_2_PIN;
	GPIO_Init(GPIOF,&GPIO_InitStructure);

}

void led_delay(uint32_t tmep)
{
	for(; tmep!= 0; tmep--);
}

void LED_drive()
{
	GPIO_ResetBits(GPIOA, LED_9_PIN | LED_10_PIN);
	GPIO_ResetBits(GPIOB, LED_3_PIN | LED_4_PIN | LED_11_PIN | LED_12_PIN );
	GPIO_ResetBits(GPIOC, LED_6_PIN | LED_7_PIN | LED_8_PIN);
	GPIO_ResetBits(GPIOD, LED_5_PIN);
	GPIO_ResetBits(GPIOF, LED_1_PIN | LED_2_PIN);

}

void LED_Disable()
{
	GPIO_SetBits(GPIOA, LED_9_PIN | LED_10_PIN);
	GPIO_SetBits(GPIOB, LED_3_PIN | LED_4_PIN | LED_11_PIN | LED_12_PIN );
	GPIO_SetBits(GPIOC, LED_6_PIN | LED_7_PIN | LED_8_PIN);
	GPIO_SetBits(GPIOD, LED_5_PIN);
	GPIO_SetBits(GPIOF, LED_1_PIN | LED_2_PIN);
}

void LED_Control(uint8_t state,uint8_t NUM)
{
	if(state == ON)
	{
		switch(NUM)
		{
			case 0:				//1
				LED9_ON;
				break;
			case 1:
				LED10_ON;
				break;
			case 2:
				LED12_ON;
				break;
			case 3:
				LED1_ON;
				break;
			case 4:
				LED2_ON;
				break;
			case 5:
				LED11_ON;
				break;
			case 6:
				LED3_ON;
				break;
			case 7:
				LED5_ON;
				break;
			case 8:
				LED8_ON;
				break;
			case 9:					//*
				LED4_ON;
				break;
			case 10:				//0
				LED6_ON;
				break;
			case 11:			//#
				LED7_ON;
				break;
			default:
				break;
		}
	}
	else
	{
		switch(NUM)
		{
			case 0:				//1
				LED9_OFF;
				break;
			case 1:
				LED10_OFF;
				break;
			case 2:
				LED12_OFF;
				break;
			case 3:
				LED1_OFF;
				break;
			case 4:
				LED2_OFF;
				break;
			case 5:
				LED11_OFF;
				break;
			case 6:
				LED3_OFF;
				break;
			case 7:
				LED5_OFF;
				break;
			case 8:
				LED8_OFF;
				break;
			case 9:					//*
				LED4_OFF;
				break;
			case 10:				//0
				LED6_OFF;
				break;
			case 11:			//#
				LED7_OFF;
				break;
			default:
				break;
		}
	}
}
