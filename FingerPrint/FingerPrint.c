#include "FingerPrint.h"
#include "mains.h"

/****************************************************
NBTS-063 一体化半导体指纹模块
*****************************************************/

/********************/
//volatile u8 Sys_sleepflag;

uint8_t figcntRxd=0;        //接收字节计数

uint8_t FingPrint_data[17]={0}; //发送给指纹模块的数组

FINGER g_finger[6];    //定义指纹接收二级缓存，给处理用
FINGER Rev_finger;     //定义指纹接收一级缓存

/*********************************************************************
 @brief  指纹模块初始化
 @param  无
 @retval 无
**********************************************************************/
void FPRT_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIOB的外设时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PF_POWERON_PIN; //PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(PF_POWERON_PORT, &GPIO_InitStructure);	
}


void FPRT_EXTI_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = PF_TOUCH_OUT_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PF_TOUCH_OUT_PORT, &GPIO_InitStructure);	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource11);

	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void Fingerprint_init(void)
{

	FPRT_GPIO();
	FPRT_EXTI_GPIO();
	soft_delay_ms(10);
	
}

void Fingerprint_Enable(void)
{
	
	GPIO_SetBits(PF_POWERON_PORT, PF_POWERON_PIN);
	soft_delay_ms(40);
	Usart1_init();
}
/*********************************************************************
 @brief  指纹模块使能
 @param  无
 @retval 无
**********************************************************************/
void Fingerprint_Disable(void)
{
	Usart1_Disable();
	soft_delay_ms(100);
	GPIO_ResetBits(PF_POWERON_PORT, PF_POWERON_PIN);
}



/*********************************************************************
 @brief  发送指纹控制命令命令
 @param  @命令类型
         @用户ID
         @用户权限：1 管理员 2 普通用户 3 访客
 @retval 无
**********************************************************************/

uint8_t FingerPrint_api(CMD_ENUM cmd,uint16_t data,uint8_t roll)
{
	uint8_t  result;
	uint8_t  i;
	uint16_t crc;
	uint8_t  dataL;  //注册指纹用到，删除指纹用到
	uint8_t  dataH;
	result=0;
	dataH=(data>>8)&0xff;
	dataL=data&0xff;
	switch(cmd)
	{
		case CMD_TEST: //0
			result=0;
			break;			     
		case CMD_ENROLL: //1，注册指纹
			FingPrint_data[8]=0x08;
			FingPrint_data[9]=0x31;
			FingPrint_data[10]=dataH;
			FingPrint_data[11]=dataL;
			FingPrint_data[12]=0x03;
			FingPrint_data[13]= 0x00;
			FingPrint_data[14]= 0x00;
			result=1;
			break;
		case CMD_IDENTIFY:  //2，1：N对比指纹
			FingPrint_data[8]=0x08;
			FingPrint_data[9]=0x32;
			FingPrint_data[10]=0x02;
			FingPrint_data[11]=0xFF;
			FingPrint_data[12]=0xFF;
			FingPrint_data[13]= 0x00;
			FingPrint_data[14]= 0x00;
			result=1;
			break;
		case CMD_CLEAR_TEMPLATE: //4,删除指定用户
			FingPrint_data[8]=0x07;
			FingPrint_data[9]=0x0C;
			FingPrint_data[10]=dataH;
			FingPrint_data[11]=dataL;
			FingPrint_data[12]=0x00;
			FingPrint_data[13]=0x01;
			result=1;
			break;
		case CMD_CLEAR_ALLTEMPLATE: //5,删除所有用户
			FingPrint_data[8]=0x03;
			FingPrint_data[9]=0x0D;
			result=1;
			break;
		case CMD_BREAK://
			FingPrint_data[8]=0x03;
			FingPrint_data[9]=0x30;
			result=1;
			break;
		default:
	        result=0;
	        break;
	}
	
	if(result)
	{
		FingPrint_data[0]=0xEF;
		FingPrint_data[1]=0x01;
		FingPrint_data[2]=0xFF;
		FingPrint_data[3]=0xFF;
		FingPrint_data[4]=0xFF;
		FingPrint_data[5]=0xFF;
		FingPrint_data[6]=0x01;
		FingPrint_data[7]=0x00;
		crc=0;
		for(i=6; i<FingPrint_data[8]+7; i++)
		{
			crc += FingPrint_data[i];
		}
		#ifdef __DEBUG2__
		printf("CRC= 0x%04x\r\n", crc);
		#endif
		FingPrint_data[FingPrint_data[8]+7]=(crc>>8)&0xff;
		FingPrint_data[FingPrint_data[8]+8]=crc&0xff;
		Usart1_SendString(FingPrint_data,FingPrint_data[8]+9);
	}
	
	return result;
}

#ifdef __FPRT_BYD__


static void FPRT_Respond_Clean(u8 type)
{
	if (type)
	{
		memcpy(&g_finger[0], &g_finger[1], 18);
		memcpy(&g_finger[1], &g_finger[2], 18);
		memcpy(&g_finger[2], &g_finger[3], 18);
		memcpy(&g_finger[3], &g_finger[4], 18);
		memcpy(&g_finger[4], &g_finger[5], 18);
		memset(&g_finger[5], 0, 18);
	}
	else
		memset(g_finger, 0, 108);
}

u8 FPRT_Respond_Check(CMD_ENUM mode, u16* id)
{
	u8 result;
	switch (mode)
	{
		case CMD_ENROLL:
			if (g_finger[0].data[2] == 0x01)
			{
				result = Result_Failed;
			}
			else if (g_finger[0].data[2] == 0x27 || g_finger[0].data[2] == 0x22)
			{
				result = Result_Exist;
			}
			else if (g_finger[0].data[2] == 0x26)
			{
				result = Result_Timeout;
			}
			else if (g_finger[0].data[2] == 0x1F)
			{
				result = Result_Full;
			}
			else if (g_finger[0].data[2] == 0x00)
			{
				if (g_finger[0].data[3] == 0x06 && g_finger[0].data[4] == 0xF2)
					result = Result_Succes;
				else if (g_finger[0].data[3] == 0x02 && (g_finger[0].data[4] == 0x01 || g_finger[0].data[4] == 0x02))
					result = Result_Again;
				else
					result = Result_Pass;
			}
			else
			{
				result = Result_Failed;
			}
			break;

		case CMD_IDENTIFY:
			if (g_finger[0].data[2] == 0x00)
			{
				if (g_finger[0].data[3] == 5)
				{
					*id = g_finger[0].data[5];
					*id |= g_finger[0].data[4]<<8;
					result = Result_Succes;
				}
				else
					result = Result_Pass;
			}
			else
			{
				result = Result_Failed;
			}
			break;
		case CMD_CLEAR_TEMPLATE:
		case CMD_CLEAR_ALLTEMPLATE:
		case CMD_BREAK:
			if (g_finger[0].data[2] == 0x00)
			{
				result = Result_Succes;
			}
			else
			{
				result = Result_Failed;
			}
			break;
	}
	
	FPRT_Respond_Clean(1);

	return result;
}
#endif

void FingerPrint_Delete(uint16_t User_Number)
{
	Fingerprint_Enable();
	soft_delay_ms(100);
	
#ifdef	__DEBUG2__
	printf("FingerPrint_Delete\r\n");
#endif
	FingerPrint_api(CMD_BREAK,1,1);
	soft_delay_ms(50);
	if (User_Number == 300)
	{
		FingerPrint_api(CMD_CLEAR_ALLTEMPLATE, 0, 0);	
	}
	else
	{		
		FingerPrint_api(CMD_CLEAR_TEMPLATE, User_Number, 1);
	}
	soft_delay_ms(50);
	Fingerprint_Disable();
}


int Fingerprint_Enroll(uint8_t permissions, uint16_t User_Number)
{
	u8 i = 0;
	u8 Exist = 0;
	u8 status;
	Fingerprint_Enable();
#ifdef	__DEBUG2__
		printf("Fingerprint_Enroll\r\n");
#endif
	FingerPrint_api(CMD_BREAK,1,1);
	soft_delay_ms(100);
	if(Flag_Inf[FPRT] == 1)
	{
		Exist = Fingerprint_Compare();
		if(Exist == 0x21||Exist == 0x22)
		{
			return Result_Exist;
		}
	}
	Read_flash_Data(User_Data, User_Number/100 + USER_Data_0);
	FingerPrint_api(CMD_CLEAR_TEMPLATE, User_Number, 0);
	soft_delay_ms(100);
	FPRT_Respond_Clean(0);
	i=250;

	FingerPrint_api(CMD_ENROLL,User_Number,0);
	#ifdef __DEBUG2__
	printf("User_Number = %d\r\n",User_Number);
	#endif
	while(i--)
	{
		if(touchCheckPro())
		{
			return Result_Finger_Exist;
		}
		if (g_finger[0].usefalg == 1)
		{
			status=FPRT_Respond_Check(CMD_ENROLL, 0);
			switch (status)
			{
				case Result_Pass:
					break;
				case Result_Again:
					AudioPlay(AUDIO_PROMPT_KEYIN_FGPRT_AGAIN); //请再输入同一指纹
					break;
				case Result_Failed: 
					AudioPlay(AUDIO_PROMPT_RECORD_FAIL);
					i=0;
					return -1;
				case Result_Succes:
					#ifdef __DEBUG2__
					printf("FPRT Success\r\n");
					#endif
					AudioPlay(AUDIO_PROMPT_FGPRT_RECORD_SUCCESS);//指纹登记成功
					#ifdef __BLUETOOTH_SUPPORT__
					Admin_Flag[BT_Admin_FPRT]++;
					#else
					Exist = (u8)Flag_Inf[Admin_FPRT];
					save(ADMIN_FINGER_STR,&Exist,1);
					#endif
					return 0;
				case Result_Exist:					
					return Result_Exist;
				case Result_Timeout:
					i=0;
					return -1;
			}
		}
		soft_delay_ms(50);
	}
	#ifdef __DEBUG__
	printf("Enroll failed!\r\n");
	#endif	
	#ifdef __S800_V1_1__
	AudioPlay_All(AUDIO_PROMPT_FGPRT, AUDIO_PROMPT_RECORD_FAIL, AUDIO_PROMPT_NONE);
	#endif
	return -1;
}

int Fingerprint_Compare(void)
{
	uint8_t i;
	uint8_t status;
	u16 finger_id;
	
	Fingerprint_Enable();	
	soft_delay_ms(100);
#ifdef	__DEBUG2__
	printf("Fingerprint_Compare\r\n");
#endif
	
	FingerPrint_api(CMD_BREAK,1,1);	
	soft_delay_ms(50);
	FPRT_Respond_Clean(0);
	FingerPrint_api(CMD_IDENTIFY,1,1);	
	Rev_finger.usefalg=0;
	i= 150;
	while(i--)
	{
		if (g_finger[0].usefalg == 1)
		{
			status=FPRT_Respond_Check(CMD_IDENTIFY, &finger_id);
			#ifdef __DEBUG2__
			printf("finger_id = %d\r\n",finger_id);
			printf("status = %d\r\n",status);
			#endif
			switch (status)
			{
				case Result_Pass:
					break;
				case Result_Failed:	
					return Result_Failed;
				case Result_Succes:	
					#ifdef __BLUETOOTH_SUPPORT__
					#else
					Read_flash_Data(User_Data, finger_id/100 + USER_Data_0);
					if(!__Config_flag)
					{
						Open_Info_flag_temp[0] =  UserInfo[finger_id%100].Userflag;
						Open_Info_Numb_temp[0] = finger_id+1;
					}
					#endif
					return Result_Succes;//UserInfo[finger_id%100].Userflag;
				case Result_Timeout:
					return Result_Timeout;
			}
		}
		soft_delay_ms(50);
	}
	
	//printf("FPRT TimeOut\r\n");
	//AudioPlay_All(AUDIO_PROMPT_FGPRT, AUDIO_PROMPT_VALIDATION_TIMEOUT,AUDIO_PROMPT_NONE);
	return Result_Timeout;
}

#define FINGER_IDLE				0
#define FINGER_DOWN				1
#define FINGER_DOWN1			2
#define FINGER_DOWN2			3
#define FINGER_START_UNMOTOR	4
#define FINGER_START_UNMOTOR_WAIT	5
#define FINGER_START_FINISH		6

u8 fingerOpenFlg=0;
//FingerPrint_api(CMD_IDENTIFY,1,1);


#define FINGER_DOWN_DOWN	0x01
u8 fingerDownFlg=0;
u8 fingerDownBuff=0;
u8 fingerDownPro()
{
	if((fingerDownFlg&FINGER_DOWN)==0)
	{
		if(GPIO_ReadInputDataBit(PF_TOUCH_OUT_PORT, PF_TOUCH_OUT_PIN))
		{
			fingerDownFlg |= FINGER_DOWN_DOWN;
			fingerDownBuff = 0;
			return 1;
		}
	}
	else
	{
		if(GPIO_ReadInputDataBit(PF_TOUCH_OUT_PORT, PF_TOUCH_OUT_PIN)==0)
		{
			fingerDownBuff ++;
			if(fingerDownBuff > 1)
			{
				fingerDownBuff = 0;
				fingerDownFlg &= ~FINGER_DOWN_DOWN;
			}
		}
		else
		{
			fingerDownBuff = 0;
		}
	}
	return 0;
}

u16 finger_id=0;
u8 fingerGetDataPro()
{
	u8 status;
	if(Rev_finger.usefalg)
	{
		Rev_finger.usefalg = 0;
		status=FPRT_Respond_Check(CMD_IDENTIFY, &finger_id);
		#ifdef __DEBUG2__
		printf("finger_id = %d\r\n",finger_id);
		#endif
		switch (status)
		{
			case Result_Pass:
				break;
			case Result_Failed: 
				AudioPlay(AUDIO_PROMPT_VALIDATION_FAIL);
				return Result_Failed;
			case Result_Succes: 		
				open_door();
				Read_flash_Data(User_Data, USER_Data_0);
				return Result_Succes;
			case Result_Timeout:
				AudioPlay(AUDIO_PROMPT_VALIDATION_TIMEOUT);
				return Result_Timeout;
			default:
				break;
		}
	}
	return Result_Pass;
}
u8 fingerSetupGetDataPro()
{
	u8 status;
	if(Rev_finger.usefalg)
	{
		Rev_finger.usefalg = 0;
		status=FPRT_Respond_Check(CMD_ENROLL, &finger_id);
		#ifdef __DEBUG2__
		printf("finger_id = %d\r\n",finger_id);
		#endif
		switch (status)
		{
			case Result_Pass:
				break;
			case Result_Failed: 
				AudioPlay(AUDIO_PROMPT_VALIDATION_FAIL);
				return Result_Failed;
			case Result_Succes: 		
				open_door();
				Read_flash_Data(User_Data, USER_Data_0);
				return Result_Succes;
			case Result_Timeout:
				AudioPlay(AUDIO_PROMPT_VALIDATION_TIMEOUT);
				return Result_Timeout;
			default:
				break;
		}
	}
	return Result_Pass;
}

//普通唤醒模式
u8 errorFingerCnt = 0;
void fingerOpenPro(void)
{
	int FPRT_Read_ret;
	if(fingerDownPro())
	{
		#ifdef	__DEBUG2__
		printf("fingerOpenPro\r\n");
		#endif
		if(check_System_Lock())
	    {
            return;
	    }
		FPRT_Read_ret = Fingerprint_Compare();
		Fingerprint_Disable();		
		#ifdef	__DEBUG2__
		printf("fingerOpenProEnd FPRT_Read_ret=%d\r\n",FPRT_Read_ret);
		#endif

		#ifdef __BLUETOOTH_SUPPORT__
		if(((FPRT_Read_ret==Result_Succes))
		#else
		if(((FPRT_Read_ret==Result_Succes)||(Flag_Inf[ADMIN]==0))
		#endif
		&&(errorFingerCnt < 5)
		)
		{
			//AudioPlay(AUDIO_PROMPT_COME_IN);
			open_door();
		}
		else
		{
			
			if(errorFingerCnt < 5)
			{
				errorFingerCnt ++;
				AudioPlay(AUDIO_PROMPT_WARING);
			}
			else
			{
				AudioPlay(AUDIO_PROMPT_SYS_EXIT);
				time_info_start = MkTime();
				sysFlg |= SYS_LOCK;
			}
		}
	}
}

u8 fingerSetupFlg=0;

void fingerSetupPro(void)
{
	int FPRT_Read_ret;
	if(fingerDownPro())
	{
		TIM2_Stop();
		permissions = Root;
		#ifdef __BLUETOOTH_SUPPORT__
		FPRT_Read_ret = Fingerprint_Enroll(Root,Admin_Flag[BT_Admin_FPRT]);	
		#else
		FPRT_Read_ret = Fingerprint_Enroll(Root,Flag_Inf[Admin_FPRT]);	
		#endif
		Fingerprint_Disable();
		switch(FPRT_Read_ret)
		{
			case Result_Succes:
				TIM2_Start();
				break;
			case Result_Exist:
				AudioPlay(AUDIO_PROMPT_RECORD_EXIST); //用户已存在
				break;
			case Result_Finger_Exist:
				break;
			default:
				AudioPlay(AUDIO_PROMPT_RECORD_FAIL);
				break;
		}		
		TIM2_Start();
	}

}
extern u8 sysFlg;
void fingerSetupEffiPro(void)
{
	int FPRT_Read_ret;
	if(fingerDownPro())
	{
		//touchTimer = 0;
		fingerOpenFlg = FINGER_DOWN;
		#ifdef	__DEBUG2__
		printf("fingerOpenPro\r\n");
		#endif
		FPRT_Read_ret = Fingerprint_Compare();
		Fingerprint_Disable();
		switch(FPRT_Read_ret)
		{
			case Result_Succes:
				#ifdef __BLUETOOTH_SUPPORT__
				sysFlg = SYS_BLE_FP_SETUP;
				#else
				sysFlg = SYS_SETUP;
				#endif
				//AudioPlay(AUDIO_PROMPT_INIT_SETTING);
				AudioPlay_All(AUDIO_PROMPT_VALIDATION_SUCCESS,AUDIO_PROMPT_SETTING_START,AUDIO_PROMPT_NONE);//验证通过，请输入指纹、密码或刷卡
				break;
			case Result_Exist:
				AudioPlay(AUDIO_PROMPT_RECORD_EXIST);
				break;
			default:
				AudioPlay(AUDIO_PROMPT_WARING);
				break;
		}
		
	}
}


u8 temp=0;
void fingerPro(void)
{
	#ifdef __BLUETOOTH_SUPPORT__
	if(sysFlg&SYS_BLE_FP_SETUP)
	{	
		fingerSetupPro();		
	}
	else if(sysFlg&SYS_BLE_START_FP_SET)
	{
		fingerSetupEffiPro();		
	}
	#else
	if(sysFlg&SYS_SETUP)
	{	
		fingerSetupPro();		
	}
	else if(sysFlg&SYS_SETUP_EFFI)
	{
		fingerSetupEffiPro();		
	}
	#endif
	else
	{
		fingerOpenPro();		
	}
}



