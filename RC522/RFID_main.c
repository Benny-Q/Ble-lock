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

int RFID_AddPICC(uint8_t UserID,uint16_t User_Number)
{
	return 0;
} 


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
}

int RFID_Compare(void)
{
	return 0;

}

u8 RFIDCnt=0;
u8 RFIDData[4]={0};
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
extern SecretType bleSecretInfo[SECRET_INFO_NUM_MAX];

u8 RFIDTouchHave(u8 uType,u8 *uData,u8 uLength)
{
	u8 i=0;

	for (i=0; i<Admin_Flag[BT_Admin];i++)
	{
		//验证NFC
		if ((bleSecretInfo[i].type == ADMIN_RFID_STR))
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
	
	return 0;
}



extern u8 sysFlg;
extern u8 touchTimer;
u8 RFIDCheckPro()
{
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
			sysAddDeviceState = 1;
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



