#include "bsp_key.h"
#include "APT8L16.h"
#include "STM_Gpio_List.h"

const u8 MANU_CODE_STR[4] = {0,0,0,1};
const u8 DEVICE_TYPE_STR[2] = {0,1};
void btSend(u8 *uData,u8 uLength);

void Hall_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN, ENABLE);
		GPIO_InitStructure.GPIO_Pin = HALL_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(HALL_PORT, &GPIO_InitStructure);
	
		RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource4);
	
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority= 0 ;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
}

u8 isHaveAdmin()
{
	if(Flag_Inf[Admin_Pswd]||Flag_Inf[Admin_RFID]||Flag_Inf[Admin_FPRT])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 sysFlg=0;
u8 sysCnt=0;
u8 sysBuff=0;
u8 sysAddDeviceState=0;


#define KEY_SETUP_STEP	20

u8 keySetupFlg=0;
u8 keySetupBuff=0;
u8 keySetupCnt=0;
extern volatile unsigned char B3TimeFlg; 	

extern SecretType bleUserInfo[USER_INFO_NUM_MAX];
extern SecretType bleSecretInfo[SECRET_INFO_NUM_MAX];



u8 check_System_Lock(void)
{
    u8 i;
    if(sysFlg&SYS_LOCK)
	{
	    time_info_end = MkTime();
		if((time_info_end - time_info_start)>120)
		{
			sysFlg = sysFlg&0x03;
		}else {
			AudioPlay(AUDIO_PROMPT_WARING);
			AudioPlay(AUDIO_PROMPT_SYS_EXIT);
			for(i=0;i<12;i++)
	        {
		       LED_Control(ON,i);
	        }
	 	}
	}
	if(sysFlg&SYS_LOCK)
	{
		return 1;
	}else 
	{
		return 0;
	}
}


void keySetupPro(void)
{
	if(B3TimeFlg&T_50MS)
	{
		if(GET_PORT_SETUP==0)
		{
			#ifdef	__DEBUG2__
			printf("keySetupPro key down=%d\r\n",keySetupBuff);
			#endif
			if(keySetupBuff >= (KEY_SETUP_STEP*6))
			{
				keySetupBuff = 0;
				sysFlg = 0;
				defaultDataPro();
				SaveData_Inf(System_Data, System_Data_0);
				SaveData_Inf(User_Data, USER_Data_0);
				FingerPrint_Delete(300);
				AudioPlay(AUDIO_PROMPT_RECOVER_SETTING); //恢复出厂设置
				soft_delay_ms(2000);
				AudioPlay(AUDIO_PROMPT_RECOVER_SETTING_SUCCESS); //出厂设置恢复成功
			}
			else
			{
				if((keySetupBuff%KEY_SETUP_STEP) == 0)
				{
					AudioPlay(AUDIO_PROMPT_BI);
				}
				keySetupBuff ++;
			}
		}
		else
		{
			if(keySetupBuff)
			{
				#ifdef	__DEBUG2__
				printf("keySetupPro key up=%d\r\n",keySetupBuff);
				#endif
				sysCnt = 0;
				if(isHaveAdmin())
				{
					AudioPlay(AUDIO_PROMPT_GET_SETTING_INFO); //请输入管理员信息
					sysFlg = 0;
					sysFlg |= SYS_SETUP_EFFI;
				}
				else
				{
					AudioPlay(AUDIO_PROMPT_SETTING_START); //注册管理员，请输入指纹、密码或刷卡
					sysFlg = 0;
					sysFlg |= SYS_SETUP;
				}
			}
			keySetupBuff = 0;
		}
	}
}

void sysPro(void)
{
	
	if(sysFlg&SYS_SETUP)
	{
				

	}
	else if(sysFlg&SYS_SETUP_EFFI)
	{
		
	}
	else
	{
		
	}

}


extern u8 touchFlg=0;
u8 touchCnt=0;
u8 touchBuff[2]={0,0};
u8 keyNum[64];
u8 keyCnt=0;
u8 touchtmp[2]={0,0};
u8 touchData[2][12];
u8 touchTimer=0;
u8 touchErrorTimer=0;

void cleanInputKey(void)
{
    u8 i=0;
	for(i=0;i<keyCnt;i++)
	{
		keyNum[i]=0xff;
	}
	keyCnt = 0;
}

u8 touchCmp(void)
{
	u8 i=0;

		for(i=0;i<keyCnt;i++)
		{
			if(touchData[0][i] != touchData[1][i])
			{
				break;
			}
		}
		if(i==keyCnt)
		{
			return 1;
		}
	return 0;
}

void save(u8 pswdRfidTouch,u8 *dData,u8 uLength)
{}

void touchCheckAddAdmin(void)
{
	if(touchCmp())
	{
		AudioPlay(AUDIO_PROMPT_PSWD_RECORD_SUCCESS);  //密码登记成功
		save(ADMIN_PSW_STR,&touchData[0][0],keyCnt);
	}
	else
	{
		AudioPlay(AUDIO_PROMPT_PSWD_RECORD_FAIL);  //密码不一致，登记失败
	}

}

u8 touchCheckIsAdmin(void)
{
	u8 i=0,j=0;
	u8 result=0;
	u8 pwdH=0,pwdL=0;
	u8 tmpPwd[6];
	
	if (Admin_Flag[BT_Admin_Pswd]==0) return 0;

	for (i=0;i<Admin_Flag[BT_Admin];i++)
	{
		if (bleSecretInfo[i].type == ADMIN_PSW_STR)
		{
			//将存储的密码进行转换, 数字密码的长度为6位,数据存储使用三个字节
			for(j=0;j<3;j++)
			{
				pwdH = (bleSecretInfo[i].secret[j]&0xf0)>>4;
				pwdL = bleSecretInfo[i].secret[j]&0x0f;
				tmpPwd[j*2]=pwdH;
				tmpPwd[j*2+1]=pwdL;
			}
			//比较
			for (j=0;j<6;j++)
			{
				//printf("tmp[%d]=%d , touch[%d]=%d\r\n",j,tmpPwd[j],j,touchData[0][j]);
				if (touchData[0][j] != tmpPwd[j])
				{
				  result=0;
					break;  //输入密码不匹配调出循环比较
				}
				else
				{
					result =1; //输入密码匹配
				}
			}

			if (result==1) break;
			
		}
	}

	return result;

}

void timeOut()
{
	touchTimer = 0;
	cleanInputKey();
    sysFlg = 0;
//	AudioPlay(AUDIO_PROMPT_VALIDATION_TIMEOUT);
}


void touchCheckEffi(void)
{
	touchErrorTimer ++;
	if(touchErrorTimer<=5)
	{
		if(touchCheckIsAdmin())
		{
			sysFlg = SYS_SETUP;
			AudioPlay_All(AUDIO_PROMPT_VALIDATION_SUCCESS,AUDIO_PROMPT_SETTING_START,AUDIO_PROMPT_NONE);//验证通过，请输入指纹、密码或刷卡
			
		}
		else
		{
			AudioPlay(AUDIO_PROMPT_WARING); //警报声
		}
	}
	else
	{
		timeOut();
	}
}
u8 errorTouchCnt = 0;

void touchCheckOpen()
{
	if(touchCheckIsAdmin()
	&&(errorTouchCnt < 5)
	)
	{
		open_door();
		//touchTimer = 0;
	}
	else if((Admin_Flag[BT_Admin]==0)
	&&(touchData[0][0] == 1)
	&&(touchData[0][1] == 2)
	&&(touchData[0][2] == 3)
	&&(touchData[0][3] == 4)
	&&(touchData[0][4] == 5)
	&&(touchData[0][5] == 6)
	&&(keyCnt >= 6)
	&&(errorTouchCnt < 5)
	)
	{
		open_door();
	}
	else
	{
		if(errorTouchCnt < 5)
		{
			errorTouchCnt ++;
			AudioPlay(AUDIO_PROMPT_WARING);//请重新输入
		}
		else
		{
			AudioPlay(AUDIO_PROMPT_SYS_EXIT);//系统锁定
			time_info_start = MkTime();
			sysFlg |= SYS_LOCK;
		}
		//delay_ms(50);
		//AudioPlay(AUDIO_PROMPT_TRY_AGAIN);
	}
}
extern u8 sleepBuff;

/*** 按键寄存器值**************************
**** key_1		0x20
**** key_2		0x40
**** key_3		0x400
**** key_4		0x10
**** key_5		0x80
**** key_6		0x800
**** key_7		0x8
**** key_8		0x100
**** key_9		0x1000
**** key_*		0x4
**** key_0		0x200
**** key_#		0x2000
*/
void touchCheck(void)
{
	u8 i=0;
	touchCnt = 0xff;
	if(check_System_Lock())
	{
        return;
	}

	switch(APTTouchKeyreg_value)
	{
	case 0x2000:            
		LED_Control(OFF,11);
		AudioPlay(AUDIO_PROMPT_BI);
		LED_Control(ON,11);
		touchCnt = KEY_SHARP;

		if((touchTimer == 0)
		&&(keyCnt == 0)
		&&(sysFlg)
		)
		{
			AudioPlay(AUDIO_PROMPT_SYS_EXIT); 	//系统退出
			sysFlg = 0;
			sleepBuff = 12;
			return;
		}
		
		//touchBuff[touchTimer] ++;
		if(keyCnt < 6 || keyCnt > 12)
		{
			AudioPlay(AUDIO_PROMPT_RECORD_PSWD_MORE_6_TO_12); //请输入6到12位密码
			cleanInputKey();
			return;
		}

		for(i=0;i<keyCnt;i++)
		{
            touchData[touchTimer][i]=keyNum[i];
		}
		#if 0
		if(sysFlg&SYS_SETUP)
		{
			if(RFIDTouchHave(ADMIN_PSW_STR,&touchData[touchTimer][0],keyCnt))
			{
				AudioPlay(AUDIO_PROMPT_RECORD_EXIST); //用户已存在
				touchTimer = 0;
			}
			else if(touchTimer==0)
			{
				touchTimer = 1;
				AudioPlay(AUDIO_PROMPT_KEYIN_PSWD_AGAIN); //请再输入同一密码
			}
			else if(touchTimer==1)
			{
				touchCheckAddAdmin();
				touchTimer = 0;
			}
		}
		else if(sysFlg&SYS_SETUP_EFFI)
		{
			touchCheckEffi();
		}
		else
		#endif
		{
			touchCheckOpen();			
		}
		cleanInputKey();
		break;
	case 0x0200:
		touchCnt = KEY_0;
		LED_Control(OFF,10);
		break;
	case 0x0004:
		touchCnt = KEY_STAR;
		cleanInputKey();
		LED_Control(OFF,9);
		break;
	case 0x1000:
		touchCnt = KEY_9;
		LED_Control(OFF,8);
		break;
	case 0x0100:
		touchCnt = KEY_8;
		LED_Control(OFF,7);
		break;
	case 0x0008:
		touchCnt = KEY_7;
		LED_Control(OFF,6);
		break;
	case 0x0800:
		touchCnt = KEY_6;
		LED_Control(OFF,5);
		break;
	case 0x0080:
		touchCnt = KEY_5;
		LED_Control(OFF,4);
		break;
	case 0x0010:
		touchCnt = KEY_4;
		LED_Control(OFF,3);
		break;
	case 0x0400:
		touchCnt = KEY_3;
		LED_Control(OFF,2);
		break;
	case 0x0040:
		touchCnt = KEY_2;
		LED_Control(OFF,1);
		break;
	case 0x0020:
		touchCnt = KEY_1;
		LED_Control(OFF,0);		
		break;
	default:
		break;
	}
	if(touchCnt <= KEY_9)
	{
		keyNum[keyCnt] = touchCnt;
		keyCnt ++;
	}
	if(touchCnt<=KEY_STAR)
	{
		AudioPlay(AUDIO_PROMPT_BI);
	}
	for(i=0;i<12;i++)
	{
		LED_Control(ON,i);
	}
	
}
extern u8 temp;
u8 touchCheckPro(void)
{
	if(touchFlg&TOUCH_DOWN)
	{
		touchFlg &= ~TOUCH_DOWN;
		
		APT_ScanTouchKey();
		if(APTTouchKeyreg_value == 32768)
		{
			LED_Control(OFF,11);
			AudioPlay(AUDIO_PROMPT_BI);
			LED_Control(ON,11);
			touchTimer = 0;
			touchBuff[0] = 0;
			touchBuff[1] = 0;
			AudioPlay(AUDIO_PROMPT_WELCOME);
			sysFlg = 0;		
			return 1;
		}
	}	
	return 0;
}


void touchPro(void)
{
	if(touchFlg&TOUCH_DOWN)
	{
		touchFlg &= ~TOUCH_DOWN;
		APT_ScanTouchKey();
#ifdef __DEBUG2__
		printf("key_value = %d\r\n",APTTouchKeyreg_value);
#endif
		touchCheck();
	}

}



#ifdef __BLUETOOTH_SUPPORT__
UserType currUser;

#define BT_IDLE						0x00
#define GET_DEVICE_INFO				0xA4
#define REGISTER_DEVICE				0xB0
#define OPTION_SECRET				0xD2
#define SETUP_IP					0xAC
extern u8 getData[64];


/***********************************************************************
*函数名 getSecretUserIsExist
*功能:  判断用户表中用户是否存在 通过手机号
***********************************************************************/
u8 getUserIsExist(u8 *uData)
{
	u8 i=0;
	u8 phoneNum[11]={0};

	for (i=0;i<11;i++)
	{
		phoneNum[i]=uData[18+i];
		//printf("phoneNum[%d]=%x\r\n",i,phoneNum[i]);
	}

	//比较
	for (i=0;i<USER_INFO_NUM_MAX;i++)
	{
		//printf("del-cmp=%d\r\n",memcmp(phoneNum,bleUserInfo[i].phone,11));
		if (!memcmpStr(phoneNum,bleUserInfo[i].phone,11))
		{

			return 1;
		}
	}
	
	return 0;
}


/***********************************************************************
*函数名 getSecretUserIsExist
*功能:  判断密码表中用户是否存在 通过手机号
***********************************************************************/
u8 getSecretUserIsExist(u8 *uData)
{
	u8 i=0;
	u8 phoneNum[11]={0};

	for (i=0;i<11;i++)
	{
		phoneNum[i]=uData[1+i];
		//printf("phoneNum[%d]=%x\r\n",i,phoneNum[i]);
	}

	//比较
	for (i=0;i<USER_INFO_NUM_MAX;i++)
	{
		//printf("del-cmp=%d\r\n",memcmp(phoneNum,bleSecretInfo[i].phone,11));
		if (!memcmpStr(phoneNum,bleSecretInfo[i].phone,11))
		{

			return 1;
		}
	}
	
	return 0;
}

u8 batCnt=100;

void defaultData()
{
	//BYTE i=0,utemp[6];
	AudioPlay(AUDIO_PROMPT_RECOVER_SETTING);
}
//user bank1到bank10  最大到31
//secret bank 11到30
//record bank 31
u8 saveAddUser(u8 *uData,u8 uLen)
{
	u8 i=0;

	//通过手机号码判断用户是否存在
	if (getUserIsExist(uData)==1) return 0;

	//添加用户
	//获取用户手机号
	for (i=0;i<11;i++)
	{
		currUser.phone[i]=uData[18+i];
	}
	//获取用户密码
	for (i=0;i<6;i++)
		currUser.secret[i]=uData[29+i];
	//获取时间
	//获取权限
	for (i=0;i<3;i++)
		currUser.secret[i]=uData[uLen-5+i];

	btSaveUserFlashData(currUser);
	AudioPlay(AUDIO_PROMPT_RECORD_SAVED);
	printf("save succ\r\n");
	return 1;
}

void saveFindSecretDelFromNum(u8 uNumber)
{
	u8 i=0;
	u8 uBuff[64];
	for(i=0;i<20;i++)
	{
		//McuEepromReadBuffer(uBuff, (11+i)<<6, 64);
		if(*(uBuff+1) == uNumber)
		{
			memsetStr(uBuff,0,32);
			//McuEepromWriteBuffer(uBuff, (11+i)<<6, 64);
		}
		if(*(uBuff+33) == uNumber)
		{
			memsetStr(uBuff+32,0,32);
			//McuEepromWriteBuffer(uBuff, (11+i)<<6, 64);
		}

	}
}
u8 memoffsetStr(u8 *uData,u8 findData,u8 uLength)
{
	u8 i=0;
	for(i=0;i<uLength;i++)
	{
		if((*uData+i)==findData) 
		{
			return i+1;
		}
	}
	return i+1;
}

u8 btDelSecretUserInfo(u8 *uData,u8 operType)
{
	u8 i=0,num=0;
	u8 tmpData[10]={0};
	u8 infoExist=0;

	//通过手机号码判断用户是否存在
	if (getSecretUserIsExist(uData)==0) return 1;
	//判断此用户是否有权限操作
	//判断要删除的内容是否存在
	for (i=0;i<10;i++)
		tmpData[i]=uData[18+i];
		
	for (i=0;i<SECRET_INFO_NUM_MAX;i++)
	{
		if (!memcmpStr(tmpData,bleSecretInfo[i].secret,10))
		{
			num=i;
			infoExist=1;
		}
	}

	//执行删除
	if (infoExist)
	{
		for (i=num;i<SECRET_INFO_NUM_MAX;i++)
		{
			bleSecretInfo[num]=bleSecretInfo[i+1];
		}

		if(operType == BT_DEL_NUMBER_SECRET) Admin_Flag[BT_Admin_Pswd]--;

		if(operType == BT_DEL_NFC_SECRET) Admin_Flag[BT_Admin_RFID]--;

		if(operType == BT_DEL_FINGER_SECRET) Admin_Flag[BT_Admin_FPRT]--;

		Admin_Flag[BT_Admin]--; 

		SaveData_Inf(BT_Secret_Data, BT_Secret_Page);
		SaveData_Inf(BT_System_Data, BT_System_Page);
		
		AudioPlay(AUDIO_PROMPT_DATA_CLEARED);

		return 1;
	}
	return 0;
}


u8 strComp(u8 *uData0,u8 *uData1,u8 uLength)
{
	u8 i=0;
	for(i=0;i<uLength;i++)
	{
		if(*(uData0+i) != *(uData1+i))
		{
			break;
		}
	}
	if(i == uLength)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void memsetStr(u8 *uData1,u8 uData2,u8 uLength)
{
	u8 i=0;
	for(i=0;i<uLength;i++)
	{
		*(uData1+i) = uData2;
	}
}
void memcpyStr(u8 *uData1,u8 *uData2,u8 uLength)
{
	u8 i=0;
	for(i=0;i<uLength;i++)
	{
		*(uData1+i) = *(uData2+i);
	}
}
u8 memcmpStr(u8 *uData1,u8 *uData2,u8 uLength)
{
	u8 i=0;
	for(i=0;i<uLength;i++)
	{
		if(*(uData1+i) != *(uData2+i))
		{
			return 0;
		}
	}
	return 1;
}

//user bank1到bank10  最大到31
//secret bank 11到30
//record bank 31
void saveNumFindPhone(u8 uIndex,u8 *uData)
{
	u8 uBuff[64];
	if(uIndex == 0xf0)
	{
		memcpyStr(uData,&getData[ADD_ADMIN_PHONE0],11);
	}
	else
	{
		//McuEepromReadBuffer(uBuff, (1+(uIndex-1)/2)<<6, 64);
		if((uIndex+1)%2)
		{
			memcpyStr(uData,uBuff,11);
		}
		else
		{
			memcpyStr(uData,uBuff+32,11);
		}
	}
}

u8 saveModifyOrDelFindUser(u8 *uData,u8 uLength)
{
	u8 i=0,j=0;
	u8 uBuff[64];
	//通过手机号码判断用户是否存在
#if 1
	if(strComp(&getData[ADD_ADMIN_PHONE0],uData,17))
	{
		memcpyStr(currUser.phone,&uBuff[0],11);
		memcpyStr(currUser.secret,&uBuff[11],6);
		memcpyStr(currUser.startTime,&uBuff[17],5);
		memcpyStr(currUser.endTime,&uBuff[22],5);
		memcpyStr(currUser.auth,&uBuff[27],3);
		return 0xF0;
	}	
	//McuEepromReadBuffer(uBuff, 0, 64);
	
	for(i=0;i<10;i++)
	{
		//McuEepromReadBuffer(uBuff, (i+1)<<6, 64);
		for(j=0;j<uLength;j++)
		{
			if(*(uBuff+j) != *(uData+j))
			{
				break;
			}
		}
		if(j==uLength)
		{	
			memcpyStr(currUser.phone,&uBuff[0],11);
			memcpyStr(currUser.secret,&uBuff[11],6);
			memcpyStr(currUser.startTime,&uBuff[17],5);
			memcpyStr(currUser.endTime,&uBuff[22],5);
			memcpyStr(currUser.auth,&uBuff[27],3);
			return i*2+1;
		}
		for(j=0;j<uLength;j++)
		{
			if(*(uBuff+32+j) != *(uData+j))
			{
				break;
			}
		}
		if(j==uLength)
		{
			memcpyStr(currUser.phone,&uBuff[32],11);
			memcpyStr(currUser.secret,&uBuff[43],6);
			memcpyStr(currUser.startTime,&uBuff[49],5);
			memcpyStr(currUser.endTime,&uBuff[54],5);
			memcpyStr(currUser.auth,&uBuff[59],3);
			return i*2+2;
		}
	}
	return 0;
	#endif
}
u8 saveModifyOrDelFindSecret(u8 *uData,u8 uLength)
{
	u8 i=0,j=0;
	u8 uBuff[64];
	for(i=0;i<20;i++)
	{
		//McuEepromReadBuffer(uBuff, (11+i)<<6, 64);
		for(j=0;j<uLength;j++)
		{
			if(*(uBuff+2+j) != *(uData+j))
			{
				break;
			}
		}
		if(j==uLength)
		{
			memcpyStr(uData,uBuff,2);
			return i*2+1;
		}
		for(j=0;j<uLength;j++)
		{
			if(*(uBuff+34+j) != *(uData+j))
			{
				break;
			}
		}
		if(j==uLength)
		{
			memcpyStr(uData,uBuff,2);
			return i*2+2;
		}
	}
	return 0;
}

u8 saveModifyOrDelFindAll(SecretType uSecret)
{
	u8 i=0;
	u8 uBuff[64];
	/*if(uSecret.number==0xf0)
	{
		
	}*/
	for(i=0;i<20;i++)
	{
		//McuEepromReadBuffer(uBuff, (11+i)<<6, 64);
		//if((uBuff[1]==uSecret.number)
		//&&(strComp(&uBuff[2],uSecret.secret,10))
		if(strComp(&uBuff[2],uSecret.secret,10))
		{
			return 1;
		}
		/*if((uBuff[1+32]==uSecret.number)
		&&(strComp(&uBuff[2+32],uSecret.secret,10))
		)*/
		if(strComp(&uBuff[2+32],uSecret.secret,10))
		{
			return 1;
		}
	}
	return 0;
}

// SECRET ADD
SecretType currSecret;
u8 saveTemp[64];

u8 saveAdminSecret(u8 *uData,u8 wSize)
{
	u8 i=0;
	//解析新密码
	for(i=0;i<6;i++)
	{
		currSecret.secret[i]= uData[38+i];
		//printf("secret[%d]=%x\r\n",i,currSecret.secret[i]);
	}
	//解析手机号码
	for(i=0;i<11;i++)
	{
		currSecret.phone[i]= uData[21+i];
		//printf("phone[%d]=%x\r\n",i,currSecret.phone[i]);
	}
	
	btSaveFlashData(ADMIN_PSW_STR, currSecret);
	return 1;
}
u8 saveAddSecret(u8 *uData,u8 operType)
{
	u8 i=0;
	u8 result=0;;
	for(i=0;i<10;i++)
	{
		//用于获取数据包中密码、NFC uuid, 指纹信息
		currSecret.secret[i]= uData[18+i];
	}
	
	//手机号处理
	for (i=0;i<11;i++)
		currSecret.phone[i]= uData[1+i];
	
	//操作类型，用于后期判断是，数字密码、NFC、 指纹
	currSecret.type = operType;
	printf("serc-type=%x\r\n",currSecret.type);
	if (currSecret.type == BT_ADD_BLE_SECRET)
	{
		result=1;
		btSaveFlashData(ADMIN_BT_STR, currSecret);
		AudioPlay(AUDIO_PROMPT_PSWD_RECORD_SUCCESS);
	}
	else if (currSecret.type == BT_ADD_NUMBER_SECRET)
	{
		result=1;
		btSaveFlashData(ADMIN_PSW_STR, currSecret);
		AudioPlay(AUDIO_PROMPT_PSWD_RECORD_SUCCESS);
	}
	else if (currSecret.type == BT_ADD_NFC_SECRET)
	{
		result=1;
		btSaveFlashData(ADMIN_RFID_STR, currSecret);
		//AudioPlay(AUDIO_PROMPT_NFC_RECORD_SUCCESS);
	}
	else if (currSecret.type == BT_ADD_FINGER_SECRET)
	{
		result=1;
		btSaveFlashData(ADMIN_FINGER_STR, currSecret);
		//AudioPlay(AUDIO_PROMPT_FGPRT_RECORD_SUCCESS);
	}
	else
	{
		AudioPlay(AUDIO_PROMPT_RECORD_FAIL);
	}
	
	return result;
}

void btSaveFlashData(u8 data_type, SecretType data)
{
	u8 i=0;
	if (data_type == ADMIN_PSW_STR)
	{
			//添加手机号码
			for (i=0; i<11; i++)
				bleSecretInfo[Admin_Flag[BT_Admin]].phone[i] = data.phone[i];
			
			//添加密码
			for (i=0; i<10; i++)
			{
				bleSecretInfo[Admin_Flag[BT_Admin]].secret[i] = data.secret[i];
				printf("secret[%d]=%x\r\n",i,bleSecretInfo[Admin_Flag[BT_Admin]].secret[i]);
			}

			bleSecretInfo[Admin_Flag[BT_Admin]].type = data_type;
			Admin_Flag[BT_Admin_Pswd]++;
			
			printf("save digit-pwd\r\n");
			
	}
	else if (data_type == ADMIN_RFID_STR)
	{
		for(i=0;i<11; i++)
			bleSecretInfo[Admin_Flag[BT_Admin]].phone[i] = data.phone[i];
		
		for (i=0; i<10; i++)
		{
			bleSecretInfo[Admin_Flag[BT_Admin]].secret[i] = data.secret[i];
		}
		bleSecretInfo[Admin_Flag[BT_Admin]].type = data_type;
		Admin_Flag[BT_Admin_RFID]++;
	}
	else if (data_type == ADMIN_FINGER_STR)
	{
		bleSecretInfo[Admin_Flag[BT_Admin]].secret[0] = data.secret[0];
		bleSecretInfo[Admin_Flag[BT_Admin]].type = data_type;
		//Admin_Flag[BT_Admin_FPRT]++;
	}
	else if (data_type == ADMIN_BT_STR)
	{
			for(i=0;i<11; i++)
			{
				//printf("bt-phone[%d]=0x%x\r\n",i,data.phone[i]);
				bleSecretInfo[Admin_Flag[BT_Admin]].phone[i] = data.phone[i];
			}
					
			for (i=0; i<10; i++)
			{
				bleSecretInfo[Admin_Flag[BT_Admin]].secret[i] = data.secret[i];
				printf("save-BT[%d]=0x%x\r\n",i,data.secret[i]);
			}

			bleSecretInfo[Admin_Flag[BT_Admin]].type = data_type;
	}

	//printf("s-user =%d\r\n",Admin_Flag[BT_Admin]);
	SaveData_Inf(BT_Secret_Data, BT_Secret_Page);
	if (data_type)
	{
		Admin_Flag[BT_Admin]++; 
	}
	//printf("s-sys=%d\r\n",Admin_Flag[BT_Admin]);
	SaveData_Inf(BT_System_Data, BT_System_Page);
}

void btSaveUserFlashData(UserType data)
{
	u8 i=0;
	//手机号码
	for (i=0;i<11;i++)
		bleUserInfo[Admin_Flag[BT_User]].phone[i]=data.phone[i];
	
	//用户密码
		for (i=0;i<11;i++)
			bleUserInfo[Admin_Flag[BT_User]].secret[i]=data.secret[i];
	//时间
	
	//权限
	for (i=0;i<3;i++)
		bleUserInfo[Admin_Flag[BT_User]].secret[i]=data.secret[i];

	Admin_Flag[BT_User]++;
	SaveData_Inf(BT_User_Data, BT_User_Page);
	SaveData_Inf(BT_System_Data, BT_System_Page);
}
#endif






