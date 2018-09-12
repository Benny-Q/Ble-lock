#include "Sleep_system.h"

u8 sleepFlg=0;
u8 sleepBuff=0;
extern volatile unsigned char B3TimeFlg;
extern void IWDG_Feed(void);

void Wake_up_init(FunctionalState NewState)
{
	//GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	//EXTI_InitTypeDef EXTI_InitStructure;

	
	APT_EXTI_init();//touch key interrupt init		//exit0
	//Fingerprint_GPIO();

	EXTI_ClearITPendingBit(EXTI_Line2);//����ⲿ�жϱ�ʶλ
	EXTI_ClearITPendingBit(EXTI_PR_PR2);

}


void sleepPins(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIODEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = LED_9_PIN| LED_10_PIN| GPIO_Pin_6| GPIO_Pin_7| SPEAKER_BUSY_PIN| MCU_UART1_TX_PIN| MCU_UART1_RX_PIN| PF_TOUCH_OUT_PIN| PF_POWERON_PIN| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_12_PIN| LED_11_PIN| TOUCHKEY_INT_PIN| RFID_EXTI_PIN| RFID_RST_PIN| RFID_POWER_PIN| GPIO_Pin_6| GPIO_Pin_7| LED_4_PIN| LED_3_PIN| TOUCHKEY_SCL_PIN| TOUCHKEY_SDA_PIN| RFID_SPI_CS_PIN| RFID_SPI_CLK_PIN| RFID_SPI_MISO_PIN| RFID_SPI_MOSI_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| BAT_ADC_PIN| ZIGBEE_EXTI_PIN| HALL_PIN| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| SPEAKER_RST_PIN| SPEAKER_DATA_PIN| LED_8_PIN| LED_7_PIN| LED_6_PIN| MOTOR_PWR_EN_PIN|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_5_PIN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = MOTO1_IN2_PIN| MOTO1_IN1_PIN| LED_1_PIN| LED_2_PIN| Config_PIN| GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIODEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN,DISABLE);

}
void sleep1Pins(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
#if 1
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIODEN,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN,ENABLE);
#endif

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = /*BT_POWER_PIN | BT_RSTN_PIN | USART2_TX_PIN | USART2_RX_PIN |*/
								  LED_9_PIN | LED_10_PIN | /*BT_CONTROL_PIN |*/ GPIO_Pin_7 | 
								  SPEAKER_BUSY_PIN | MCU_UART1_TX_PIN | MCU_UART1_RX_PIN | PF_POWERON_PIN |
								  PF_TOUCH_OUT_PIN| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_12_PIN | LED_11_PIN /*| TOUCHKEY_INT_PIN*/ | RFID_EXTI_PIN |
								  RFID_RST_PIN | RFID_POWER_PIN | GPIO_Pin_6 | GPIO_Pin_7 |
								  LED_4_PIN | LED_3_PIN | TOUCHKEY_SCL_PIN | TOUCHKEY_SDA_PIN |
								  RFID_SPI_CS_PIN | RFID_SPI_CLK_PIN | RFID_SPI_MISO_PIN | RFID_SPI_MOSI_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | BAT_ADC_PIN | ZIGBEE_EXTI_PIN |
								  HALL_PIN | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
								  SPEAKER_RST_PIN | SPEAKER_DATA_PIN | LED_8_PIN | LED_7_PIN | 
								  LED_6_PIN | MOTOR_PWR_EN_PIN | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED_5_PIN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = MOTO1_IN2_PIN | MOTO1_IN1_PIN | LED_1_PIN | LED_2_PIN |
								  Config_PIN | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

#if 1	
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIODEN,DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN,DISABLE);
#endif	
}


void sleepSys(void)
{
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);//����ͣ��
    SYSCLKConfig_STOP();
}


void sleep_ctrl_2(void)
{
    //����10s�Ӻ����������˯��(���ҵ��ֹͣ��wifi����û������)
    /*******************************************************************************
     ʱ�䣬һ���ָ�û�а��£����ֹͣ״̬�������������꣬���˱���
     �����ŵ�״̬�Ѿ��غû�����ʾ���Ѳ���
     û��ָ��ָ�����ڴ������Ҫ����û��wifi�ڴ������Ҫ����
     *******************************************************************************/
	//EXTI->IMR&=~EXTI_Line4;
	//BT_OFF;
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);//��ֹ��ʱ���ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//��ֹ��ʱ���ж�

	USART_Cmd(USART1, DISABLE);
	//USART_Cmd(USART2, DISABLE);
	ADC_Cmd(ADC1,DISABLE);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
		
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update );  
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
  	//RTC_ITConfig(RTC_IT_ALRA, DISABLE);//��ֹRTC���ж�

  	Fingerprint_Disable();
	Motor_GPIO_Deinit();
	RFIDSleep();
	APT_Close();
 /****************���ж˿�����Ϊģ������*****************************/
 	sleepPins(); 
	Hall_GPIO_Init();
	//Config_Key_Init();
}



void sleep_ctrl(void)
{
	//����10s�Ӻ����������˯��(���ҵ��ֹͣ��wifi����û������)
	/*******************************************************************************
	 ʱ�䣬һ���ָ�û�а��£����ֹͣ״̬�������������꣬���˱���
	 �����ŵ�״̬�Ѿ��غû�����ʾ���Ѳ���
	 û��ָ��ָ�����ڴ������Ҫ����û��wifi�ڴ������Ҫ����
	 *******************************************************************************/
	//EXTI->IMR&=~EXTI_Line4;
	//BT_OFF;
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);//��ֹ��ʱ���ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//��ֹ��ʱ���ж�

	USART_Cmd(USART1, DISABLE);
	USART_Cmd(USART2, DISABLE);
	ADC_Cmd(ADC1,DISABLE);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearFlag(USART2,USART_FLAG_RXNE);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update );  
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RTC_ITConfig(RTC_IT_ALRA, DISABLE);//��ֹRTC���ж�

	Usart1_Disable();
	Usart2_init();
	Fingerprint_Disable();
	Motor_GPIO_Deinit();
	RFIDSleep();
	APT_low_power();
	Bluetooth_CONTROL_ON;
	Bluetooth_Exit_init();
 /****************���ж˿�����Ϊģ������*****************************/
	sleep1Pins();
	
 	//sleepSys();

}


extern void batInitCheck(void);
extern void bluetooth_init(void);
void wake_up(void)
{
//	uint8_t touch_key_status;
//	uint8_t Flag_Status = 0;
//	int RFID_Result = 0 ,FPRT_Result = 0;
	Usart2_init();
	SystemInit();
	/*----------??-----------*/	
	TIM2_Configuration();
	TIM3_Configuration();
	
	RTC_Initializes();
	/*------����------*/
	AudioInitialize();
#ifdef __PREVENT__
	Prevent_lock_Monitor();
#endif
	/*---------OLED��Ļ--------*/	
	Motor_GPIO_Init();
	LED_Config();	
	/*------����------*/	
	#ifdef __TOUCH_KEY_SUPPORT__
	APT_Init();
	#endif
	/*-------ָ��--------*/ 
	Fingerprint_init();
	figcntRxd = 0;
	ADC1_Init();
	battery_check(ADC_Value_Buff);
	batInitCheck();
	Config_Key_Init();
	LED_drive();
	Bluetooth_init();
}

/**
 * @brief  ͣ�����Ѻ�����ϵͳʱ��: ʹ�� HSE, PLL
 *         ����ѡ��PLL��Ϊϵͳʱ��.
 * @param  None
 * @retval None
 */
void SYSCLKConfig_STOP(void)
{
    //ErrorStatus HSEStartUpStatus;
    /* ʹ�� HSE */
    //RCC_HSEConfig(RCC_HSE_ON);
    /* �ȴ� HSE ׼������ */
    //HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //if(HSEStartUpStatus == SUCCESS)
    {
    	
        /* ʹ�� PLL */
        RCC_PLLCmd(ENABLE);

        /* �ȴ� PLL ׼������ */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY ) == RESET)
        {
        }

        /* ѡ��PLL��Ϊϵͳʱ��Դ */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK );

        /* �ȴ�PLL��ѡ��Ϊϵͳʱ��Դ */
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}
u8 errorBuff=0;
extern u8 errorTouchCnt;
extern u8 errorFingerCnt;
extern u8 errorRFIDCnt;

void ErroPro()
{
	if(B3TimeFlg&T_1000MS)
	{
		if(errorTouchCnt >= 5)
		{
			errorBuff ++;
			if(errorBuff > 30)
			{
				errorBuff = 0;
				errorTouchCnt = 0;
			}
		}
		else if(errorFingerCnt >= 5)
		{
			errorBuff ++;
			if(errorBuff > 30)
			{
				errorBuff = 0;
				errorFingerCnt = 0;
			}
		}
		else if(errorRFIDCnt >= 5)
		{
			errorBuff ++;
			if(errorBuff > 30)
			{
				errorBuff = 0;
				errorRFIDCnt = 0;
			}
		}
	}

}

extern u8 sysFlg;
extern u8 keySetupBuff;
u8 hallFlg=1;
void sleepPro(void)
{
	if(sleepFlg&SLEEP_READY)
	{
		#ifdef	__DEBUG2__
		printf("sleepPro SLEEP_READY\r\n");
		#endif
		#ifdef __HALL_SUPPORT__
		if(GET_PORT_HALL==0)//!
		{
			sleep_ctrl_2();  // 
			hallFlg = 0;
		}
		else
		{
			sleep_ctrl();
			hallFlg = 1;
		}
		#else
			sleep_ctrl();
			hallFlg = 1;
		#endif
		//sleepSys();
		//IWDG_Feed();
		sleepFlg = SLEEP_START;
	}
	else if(sleepFlg&SLEEP_WAKE_READY)
	{
		sleepBuff = 0;
		sleepFlg = 0;
		
		#ifdef	__DEBUG2__
		delay_ms(50);
		printf("sleepPro SLEEP_WAKE_READY\r\n");
		#endif
	}
	else if(sleepFlg&SLEEP_START)
	{
		while(sleepFlg&SLEEP_START)
		{
			sleepSys();
			//IWDG_Feed();
			if(errorBuff < 250)
			{
				errorBuff ++;
			}
		}
		//Wake_up_init(ENABLE);
		wake_up();
		hallFlg = 1;
		keySetupBuff = 0;
		sleepBuff = 0;
		#ifdef	__DEBUG2__
		delay_ms(1);
		printf("sleepPro SLEEP_START\r\n");
		#endif
	}
	else
	{
		if(B3TimeFlg&T_1000MS)
		{
			#ifdef	__DEBUG2__
			delay_ms(50);
			printf("sleepPro START UP\r\n");
			#endif
			sleepBuff ++;
			if(sleepBuff > 12)
			{
				sleepBuff = 0;
				sleepFlg = SLEEP_READY;
				if(sysFlg&SYS_SETUP)
				{
					sysFlg = 0;
					AudioPlay(AUDIO_PROMPT_SYS_EXIT); //ϵͳ�˳�
				}
				clean_input_key_value();
			}
		}
	}
}



