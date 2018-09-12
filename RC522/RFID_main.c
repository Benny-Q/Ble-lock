#include "RFID.h"
#include "mains.h"

void IC_test ( void )
{
 	u8 ucArray_ID [ 4 ];                                                                                             //????IC?????UID(IC????)
	u8 ucStatusReturn;   
 	bool bFind = false;
	RC522_Init();     //RC522????????????
	
	PcdReset();
	M500PcdConfigISOType('A');//??????

	#ifdef __DEBUG2__
	printf ("IC-TEST\r\n") ;
	#endif
 	while (1)
 	{ 
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //??
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 //???????
		
		if ( ucStatusReturn != MI_OK )                                                                                 //??????
			bFind = false;		
		
		if ( ( ucStatusReturn == MI_OK ) && ( bFind == false ) )
		{
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //???(???????????????,?????????????????)
			{
				bFind = true;
				#ifdef __DEBUG2__
				printf ( "The Card ID is: " );   
				printf ( "%02X%02X%02X%02X\n", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );  //??IC??UID(IC????) 
				#endif	
			}
			
		}

 	}
}

void RFIDGpioInit(void)
{

		GPIO_InitTypeDef GPIO_InitStructure;
	
		
		/*开启GPIOB的外设时钟*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PD2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}


uint8_t RFID_Init( uint8_t *uid , uint8_t loop )
{	
 	u8 ucArray_ID [ 4 ];                                                                                             //????IC?????UID(IC????)
	u8 ucStatusReturn;   
 	bool bFind = false;

	RC522_Init();     //RC522????????????
	
	PcdReset();
	soft_delay_ms(20);
	M500PcdConfigISOType('A');
	//while ( loop-- )
 	{ 
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //??
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 //???????
		
		if ( ucStatusReturn != MI_OK )                                                                                 //??????
			bFind = false;		
		
		if ( ( ucStatusReturn == MI_OK ) && ( bFind == false ) )
		{
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //???(???????????????,?????????????????)
			{
				bFind = true;
				#ifdef __DEBUG2__
				printf ( "The Card ID is: " );   
				printf ( "%02X%02X%02X%02X\n", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );  //??IC??UID(IC????) 
				#endif	
				memcpy(uid , ucArray_ID , 4);
				return 0;
			}
			
		}

		/*if (loop)
		{
			soft_delay_ms(50);
		}*/

 	}
	return 1;
	
}

#if 0

int RFID_AddPICC(uint8_t UserID)
{
	uint8_t Admin_CNT = 0, User_CNT = 0;
	unsigned char tmp[4];
	char Numstr[3];
	uint8_t i,status,remap,IC_CNT;
	i = Check_Admin();
	if(UserID == Root && i == 1)
	{
		AudioPlay(AUDIO_PROMPT_STORAGE_FULL);
		return -1;
	}
	
	for (IC_CNT = 0; IC_CNT<50 ;IC_CNT++)
	{
		if (ICcardInf[IC_CNT].useflag== 255 || ICcardInf[IC_CNT].useflag==0)
		{
			break;
		}
	}	
	if (IC_CNT == 50)
	{
	#ifdef __DEBUG2__
		printf("RFID Cards Full");
	#endif
		AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_STORAGE_FULL, AUDIO_PROMPT_NONE);
		return -1;
	}

	remap = Flag_Inf[Admin_RFID] + Flag_Inf[User_RFID];

	if(permissions == Root)
	{
		for(i = 0;i <= IC_CNT;i++)
		{
			if(ICcardInf[i].useflag == Root)
			{
				Admin_CNT++;
			}
		}
		OLED_EnterInfo(Admin_CNT+1 ,Root, remap);
	}
	if(permissions == User)
	{
		for(i = 0;i <= IC_CNT;i++)
		{
			if(ICcardInf[i].useflag == User)
			{
				User_CNT++;
			}
		}
		OLED_EnterInfo(User_CNT+1, User, remap);
	}
	
	
	remap = IC_CNT;
	AudioPlay(AUDIO_PROMPT_INPUT_NFC);
	soft_delay_ms(50);
	AudioPlay(AUDIO_PROMPT_BI);
	i=3;
	while(i--)
	{
		status = RFID_Init( tmp, 50);
		if (!status)
			break;
		AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_VALIDATION_FAIL, AUDIO_PROMPT_TRY_AGAIN);
	}
	
	i=0;
	if (!status)
	{
		AudioPlay(AUDIO_PROMPT_BI);
		while ( i<50 )
		{
			if (ICcardInf[i].uid[0]==tmp[0] &&\
				ICcardInf[i].uid[1]==tmp[1] &&\
				ICcardInf[i].uid[2]==tmp[2] &&\
				ICcardInf[i].uid[3]==tmp[3] )
			{				
				OLED_Failed();
				#ifdef __S900__
				AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_RECORD_EXIST, AUDIO_PROMPT_NONE);
				#else
				AudioPlay(AUDIO_PROMPT_RECORD_EXIST);
				#endif
				#ifdef __DEBUG__
				printf("This PICC is exist!\r\n");
				#endif
				APT_Init();
				return 0;
			}
			i++;
		}		
		ICcardInf[remap].useflag=UserID;
		ICcardInf[remap].uid[0]=tmp[0];
		ICcardInf[remap].uid[1]=tmp[1];
		ICcardInf[remap].uid[2]=tmp[2];
		ICcardInf[remap].uid[3]=tmp[3];
		sprintf(Numstr, " T%d", remap+1);		
		OLED_EnterResult(0);
		AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_RECORD_SAVED, AUDIO_PROMPT_NONE);
		SaveData_Inf();
		#ifdef __DEBUG__
		printf("Nfc Add PICC is Success\r\n");
		#endif
	}
	else
	{
		#ifdef __DEBUG__
		printf("This PICC Add failed!\r\n");
		#endif
		OLED_EnterResult(1);
		#ifdef __S900__
		AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_RECORD_FAIL, AUDIO_PROMPT_NONE);
		#endif
		APT_Init();
		return -1;
	}
	
	APT_Init();
	return status;
}
#else
int RFID_AddPICC(uint8_t UserID,uint16_t User_Number)
{
	unsigned char tmp[4];
	//char Numstr[3];
	uint8_t i,status,IC_CNT = 0;
	
	status = RFID_Init(tmp,1);
	if(status)
	{
		return -1;
	}
	
	if (!status)
	{
		AudioPlay(AUDIO_PROMPT_BI);
		for(i = 0;i < 3; i++)
		{
			Read_flash_Data(User_Data, i + USER_Data_0);
			for(IC_CNT = 0; IC_CNT<100 ;IC_CNT++)
			{
				if(UserInfo[IC_CNT].Userflag == 0x31 || UserInfo[IC_CNT].Userflag == 0x32)
				{
					if (UserInfo[IC_CNT].Info[0] == tmp[0] &&\
						UserInfo[IC_CNT].Info[1] == tmp[1] &&\
						UserInfo[IC_CNT].Info[2] == tmp[2] &&\
						UserInfo[IC_CNT].Info[3] == tmp[3])
					{				
						APT_Init();
						return 1;
					}
				}
			}
		}
		Read_flash_Data(User_Data, User_Number/100 + USER_Data_0);
		if(UserID == Root)
		{
			UserInfo[User_Number%100].Userflag = 0x31;
			Flag_Inf[ADMIN] ++;
			Flag_Inf[Admin_RFID] ++;
		}
		else if(UserID == User)
		{
			UserInfo[User_Number%100].Userflag = 0x32;
			Flag_Inf[USER] ++;
			Flag_Inf[User_RFID] ++;
		}
		UserInfo[User_Number%100].Info[0] =  tmp[0];
		UserInfo[User_Number%100].Info[1] =  tmp[1];
		UserInfo[User_Number%100].Info[2] =  tmp[2];
		UserInfo[User_Number%100].Info[3] =  tmp[3];
		//sprintf(Numstr, " T%d", remap+1);		
		SaveData_Inf(System_Data, System_Data_0);
		SaveData_Inf(User_Data, User_Number/100 + USER_Data_0);
		status = 2;
	}
	APT_Init();
	return status;
} 
#endif

u8 RFIDRead(u8 *tmp)
{
	uint8_t status;
	status = RFID_Init(tmp, 1);
	//APT_Init();
	if (status)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void RFIDDelete(uint8_t Delet_Number)
{
    u8 i=0;
	Read_flash_Data(User_Data, USER_Data_0);
	Flag_Inf[ADMIN]--;
	Flag_Inf[Admin_RFID]--;
	UserInfo[Delet_Number%100].Userflag = 0xff;
	UserInfo[Delet_Number%100].User_Info_CNT = 0xff;
	for(i = 0; i<6; i++)
	{
		UserInfo[Delet_Number].Info[i] = 0xff;
	}
	SaveData_Inf(User_Data, USER_Data_0);
	SaveData_Inf(System_Data, System_Data_0);
}

int RFID_Compare(void)
{
	unsigned char tmp[4];
	uint8_t j = 0, i = 0;
	uint8_t status;
	status = RFID_Init(tmp, 1);
	APT_Init();
	if (status)
	{
		return -1;
	}
	
	/*if (Flag_Inf[General_Open_mode] == 0)
	{
		if(__Config_flag == 0)
		{
			return 0x31;
		}
	}	*/
	AudioPlay(AUDIO_PROMPT_BI);
	for(i = 0;i < 3;i++)
	{
		Read_flash_Data(User_Data, i + USER_Data_0);
		while ( j<100 )
		{
			if(UserInfo[j].Userflag == 0x31 || UserInfo[j].Userflag == 0x32)
			{
				if (UserInfo[j].Info[0] == tmp[0] &&\
					UserInfo[j].Info[1] == tmp[1] &&\
					UserInfo[j].Info[2] == tmp[2] &&\
					UserInfo[j].Info[3] == tmp[3])
				{				
					APT_Init();
					if(!__Config_flag)
					{
						Open_Info_flag_temp[0] =  UserInfo[j].Userflag;
						Open_Info_Numb_temp[0] =  j+i*100+1;
					}
					return UserInfo[j].Userflag;
				}
			}
			j++;
		}
	}
	return -2;

}

u8 RFIDCnt=0;
u8 RFIDData[4];
//ADMIN_RFID_STR

void hexToBCD(u8 *uData1,u8 *uData2,u8 uLength)
{
	u8 i;
	for(i=0;i<uLength;i++)
	{
		*(uData2+i*2) = (*(uData1+i))>>4;
		*(uData2+i*2+1) = (*(uData1+i))&0x0f;
	}
}

u8 uTemp[12];
u8 RFIDTouchHave(u8 uType,u8 *uData,u8 uLength)
{
	u8 i=0;
	#if 0
	for(i=0;i<Flag_Inf[ADMIN];i++)
	{
		if((UserInfo[i].Userflag == uType)
		&&(uType == ADMIN_RFID_STR)	
		)
		{
			if((UserInfo[i].Info[0] == RFIDData[0])
				&&(UserInfo[i].Info[1] == RFIDData[1])
				&&(UserInfo[i].Info[2] == RFIDData[2])
				&&(UserInfo[i].Info[3] == RFIDData[3])
			)
			{
				return i+1;
			}
		}
		else if((UserInfo[i].Userflag == uType)
		&&(uType == ADMIN_PSW_STR) 
		)
		{
			hexToBCD(UserInfo[i].Info,uTemp,UserInfo[i].User_Info_CNT);
			if((UserInfo[i].User_Info_CNT == uLength)
				&&memcmpStr(uData,uTemp,uLength)
			)
			{
				return i+1;
			}
		}
	}
	#else
	for (i=0; i<Admin_Flag[BT_Admin];i++)
	{
		//验证NFC
		if ((bleSecretInfo[i].type == BT_ADD_NFC_SECRET)
				|| (bleSecretInfo[i].timers == BT_MODIFY_NFC_SECRET))
		{
			printf("RFID1111=0x%x0x%x0x%x0x%x\r\n",RFIDData[0],RFIDData[1],RFIDData[2],RFIDData[3]);
			if ((bleSecretInfo[i].secret[0] == RFIDData[0])
					&& bleSecretInfo[i].secret[1] == RFIDData[1]
					&& bleSecretInfo[i].secret[2] == RFIDData[2]
					&& bleSecretInfo[i].secret[3] == RFIDData[3])
			{
				return i+1;
			}
		}
	}
	#endif
	return 0;
}



extern u8 sysFlg;
extern u8 touchTimer;
u8 RFIDCheckPro()
{
	u8 i=0;
	for(i=0;i<Flag_Inf[ADMIN];i++)
	{
		if(UserInfo[i].Userflag == ADMIN_RFID_STR)
		{
			if((UserInfo[i].Info[0] == RFIDData[0])
				&&(UserInfo[i].Info[1] == RFIDData[1])
				&&(UserInfo[i].Info[2] == RFIDData[2])
				&&(UserInfo[i].Info[3] == RFIDData[3])
			)
			{
				return 1;
			}
		}
	}
	return 0;
}
u8 errorRFIDCnt = 0;
void RFIDPro(void)
{
	u8 returnFlg;//,i;
	
	//RFID_Read_ret = RFID_Compare();
	//if(touchFlg&TOUCH_DOWN)
	{
		returnFlg=RFIDRead(RFIDData);

		//printf("returnFlg=%d\r\n",returnFlg);
		if(returnFlg==0)
		{
			return;		
		}
		if(sysFlg&SYS_SETUP)
		{
			returnFlg = RFIDTouchHave(ADMIN_RFID_STR,0,0);
			if(returnFlg)
			{
			  //RFIDDelete(returnFlg - 1);
				//AudioPlay(AUDIO_PROMPT_NFC_DELETED); //卡用户删除
				AudioPlay(AUDIO_PROMPT_RECORD_EXIST); // 磁卡已存在
			}
			else
			{
				AudioPlay(AUDIO_PROMPT_NFC_RECORD_SUCCESS); //卡登记成功
				save(ADMIN_RFID_STR,RFIDData,4);
			}
			
		}
		else if(sysFlg&SYS_SETUP_EFFI)
		{
			if(RFIDTouchHave(ADMIN_RFID_STR,0,0))
			{
				sysFlg = SYS_SETUP;
				AudioPlay_All(AUDIO_PROMPT_VALIDATION_SUCCESS,AUDIO_PROMPT_SETTING_START,AUDIO_PROMPT_NONE);//AUDIO_PROMPT_INIT_SETTING
				touchTimer = 0;
			}
			else
			{
				AudioPlay(AUDIO_PROMPT_WARING);
			}
			
		}
		#ifdef __BLUETOOTH_SUPPORT__
		else if (sysFlg&SYS_BLE_NFC_SETUP)
		{
			AudioPlay(AUDIO_PROMPT_NFC_RECORD_SUCCESS); //卡登记成功
				//printf("RFIDData=0x%x0x%x0x%x0x%x\r\n",RFIDData[0],RFIDData[1],RFIDData[2],RFIDData[3]);
			touchTimer = 0;
			sysFlg = 0;
		}
		#endif
		else
		{

		    if(check_System_Lock())
	        {
                return;
	        }
			if((RFIDTouchHave(ADMIN_RFID_STR,0,0))
			&&(errorRFIDCnt < 5)
			)
			{
				open_door();
			}
			else
			{
				if(errorRFIDCnt < 5)
				{
					errorRFIDCnt ++;
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
/*	int RFID_Read_ret = 0;	
	RFID_Read_ret = RFID_Compare();
	if(RFID_Read_ret == -1)
	{
		
	
	}
	else if(RFID_Read_ret == 0x31 || RFID_Read_ret == 0x32)
	{	
	#ifdef	__DEBUG2__
		printf("RFID_Read_ret 0x31= %d\r\n",RFID_Read_ret);
	#endif
		
	}
	else if(RFID_Read_ret == -2)
	{
	#ifdef	__DEBUG2__
		printf("RFID_Read_ret -2= %d\r\n",RFID_Read_ret);
	#endif
		
	}
	*/

}



