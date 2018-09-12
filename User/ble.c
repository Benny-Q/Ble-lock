

#include "ble.h"
#include "bsp_usart2.h"
#include "bsp_key.h"


//#include "cardOper.h"
//操作类型：
//0.开门              1.增加用户     2.修改用户    3.删除用户    4.增加蓝牙密码 
//5.修改蓝牙密码      6.删除蓝牙密码 7增加数字密码 8修改数字密码 9删除数字密码 
//10增加nfc           11修改nfc      12删除nfc     13增加身份证  14修改身份证 
//15删除身份证        16增加指纹     17修改指纹    18删除指纹

//USER  :11(手机)+6管理者密码+5开始时间+5结束时间+3权限
//SECRET:1(编号)+1类型+10(密码)+5开始时间+5结束时间+1次数

#ifdef __BLUETOOTH_SUPPORT__

/***********宏定义数据***************************************/
#define BLE_OPER_ADD			1
#define BLE_OPER_MODIFY			2
#define BLE_OPER_DEL			3

/*******************变量定义****************************************/
uint8_t bleFlg=0;
uint8_t bleCnt=0;
uint8_t bleBuff=0;
CommunicateType gBle;

unsigned char sendUartData[70]={0};
uint8_t xOrCnt=0;
uint8_t btMac[9];
uint8_t uartDataLengthContinut=0;
u16 uartDataTemp1Length=0;
uint8_t uartDataXor=0;

//厂商ID 前4位+ 设备ID 后2位
static u8 ManufacturerId[6]={0x30,0x30,0x31,0x31,0x30,0x35};
static u8 defaultBleMac[14]={0x35,0x35,0x31,0x33,0x31,0x34,0x41,0x45,0x46,0x44,0x41,0x34,0xFF,0xFF};
//static u8 defaultBleMac[14]={0x35,0x35,0x34,0x34,0x31,0x34,0x41,0x45,0x46,0x44,0x41,0x34,0xFF,0xFF};

u8 gBleMac[12]={0};


/*******全局变量引用**************************************/
extern u8 getData[64];
extern uint8_t  uStatus;
extern UserType currUser;
extern UserType bleUserInfo[10];
extern SecretType bleSecretInfo[10];
//extern u8 Admin_Flag[BT_Admin_Max];
extern u8 RFIDData[4];
extern u8 Battery_Percent;






void btSend(uint8_t *data,uint8_t length)
{
	u8 i=0;

	for(i=0;i<length;i++)
	{
		Usart2_SendByte(*(data+i));
	}
}


void btSendPack(uint8_t *uData,uint8_t uLength)
{
	u8 i=0;
	u8 sendUartData[128]={0};
	u8 crc=0;

	//先将数据放到 sendUartData
	
	for (i=0; i<uLength; i++)
	{
		sendUartData[i] = (*(uData+i));
	}
	
	//数据包加密
	for (i=0; i<sendUartData[11];i++)
	{
		sendUartData[12+i] = xOrCnt^sendUartData[12+i];
	}
	//sendUartData[3]=xOrcnt;

	//进行整包进行 XOR

	for (i=0; i<uLength-1; i++)
	{
		crc = crc^sendUartData[i];
	}

	sendUartData[uLength-1]=crc;
	btSend(sendUartData,uLength);
}

void getBleMac2ASCII(void)
{
	u8 mac[6] = {0x12,0x13,0x14,0xAE,0xFD,0xA4};
	u8 tmpmac[14]={0xFF};
	u8 tmp_macH =0;
	u8 tmp_macL =0;
	u8 i=0;
	for (i=0;i<6;i++)
	{
		tmp_macH = (mac[i]&0xf0)>>4;
		tmp_macL = mac[i]&0x0f;
		tmpmac[i*2]=tmp_macH;
		tmpmac[i*2+1]=tmp_macL;
	}

	for(i=0;i<14;i++)
	{
		tmpmac[i]+=0x30;
		printf("tmpmac[%d]=%x\r\n",i,tmpmac[i]);
	}
	
}

uint8_t btGetHead(void)
{
	u8 i=0,j=0;
	//u8 tmp_data[70]={0};
	if (gBle.uDataLen >= 6)
	{
		//printf("data=%d\r\n",gBle.uDataLen);
		for(i=0;i<gBle.uDataLen;i++)
		{
			printf("rev-data[%d]=%x\r\n",i,gBle.CommunicateUartData[i]);
		}
		
		for(i=0;i<gBle.uDataLen;i++)
		{
			//当数据包有特殊字符需过滤
			if (gBle.CommunicateUartData[i]==0x89 && gBle.CommunicateUartData[i+1]==0x89)
			{
				for (j=0;j<gBle.uDataLen-i;j++)
				{
					gBle.CommunicateUartData[j]=gBle.CommunicateUartData[i+j];
					//printf("UartData[%d]=%x\r\n",j,gBle.CommunicateUartData[j]);
				}
				gBle.uDataLen -=i;
			
				return 1;
			}
		}
			return 1;
	}
	return 0;
}

void bleGetRuoChanPro(void)
{
	uint8_t i=0;

	for(i=0;i<gBle.uDataLen;i++)
	{
		//gBle.CommunicateValidUartData[i] = gBle.CommunicateUartData[12+i];
		gBle.CommunicateValidUartData[i] = gBle.CommunicateUartData[12+i];
		printf("CommunicateValidUartData[%d]=%x\r\n",i,gBle.CommunicateValidUartData[i]);
	}

	gBle.uDataValidLen = gBle.CommunicateUartData[11];

}


void bleGetDevice()
{
	uint8_t i=0;
	
	printf("getDevie\r\n");
	//RTC_GetDate(RTC_Format_BIN,&RTC_DataStructure);	
	//RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
	
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_GET_DEVICE;
	gBle.SendUartData[3]=0x0; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*数据  使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=0x11;

	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[13+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[19+i]=defaultBleMac[i];
	}
	//电量百分比
	gBle.SendUartData[33] = Battery_Percent;//  80%
	//工作模式
	gBle.SendUartData[34] = 0x2;
	// XOR
	gBle.SendUartData[35] = 0x0;
	
	btSendPack(gBle.SendUartData,36);

}

u8 addBtAdmin(void)
{
	u8 admin_exist=0;
	//u8 i=0;
	
	printf("flag=%d,pwd=%x\r\n",Admin_Flag[BT_Admin],bleUserInfo[0].secret[0]);
	
	//判断是否已有管理员
	if ((Admin_Flag[BT_Admin]>0) && (Admin_Flag[BT_Admin]!=0xFF))
	{
		//已有管理员
		admin_exist =0x0;
		AudioPlay(AUDIO_PROMPT_RECORD_EXIST);
	}
	else
	{
		admin_exist = saveAdminSecret(gBle.CommunicateValidUartData,gBle.uDataValidLen);
		AudioPlay(AUDIO_PROMPT_SET_SUCCESS);
	}

	return admin_exist;
}
void bleRegisterDevice(void)
{
	uint8_t i=0;
	u8 admin_exist=0;

	admin_exist = addBtAdmin();
	//发送协议
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_REGISTER_DEVICE;
	gBle.SendUartData[3]=0x0; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*数据  使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=BLE_RESPONSE_REGISTER_DEVICE;
	gBle.SendUartData[13]=admin_exist;
	gBle.SendUartData[14]=0x50;

	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=defaultBleMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x1;
	btSendPack(gBle.SendUartData,36);
}
extern SecretType currSecret;
uint8_t openCheckPro(uint8_t *uData)
{
	uint8_t userNumber=0;
	//userNumber = saveModifyOrDelFindUser(uData+1,17);
	if(userNumber)
	{
		currSecret.type = *(uData+28);
		//currSecret.number = userNumber;
		memcpy(currSecret.secret,uData+18,10);
		
		//userNumber = saveModifyOrDelFindAll(currSecret);
		if(userNumber)
		{
			return 1;
		}
		
	}
	return 0;
}


void bleOpen()
{
	uint8_t i=0;
	u8 uTemp[10];
	u8 isOpen=0;
	u8 cmpResult=0;
	
	//发送协议
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPEN;
	gBle.SendUartData[3]=0x0; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*数据  使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=BLE_RESPONSE_OPEN;
	gBle.SendUartData[13]=0x0;
	gBle.SendUartData[14]=0x50;

	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=defaultBleMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;
	

	for (i=0; i<10; i++)
	{
		uTemp[i]=gBle.CommunicateValidUartData[18+i];
		printf("uTemp[%d]=%x\r\n",i,uTemp[i]);
	}
	//蓝牙开门密码进行比较
	printf("openadmin=%x\r\n",Admin_Flag[BT_Admin]);

	for (i=0; i<Admin_Flag[BT_Admin]; i++)
	{
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[0]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[1]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[2]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[3]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[4]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[5]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[6]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[7]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[8]);
			printf("secret[%d]=%x\r\n",i,bleSecretInfo[i].secret[9]);
			printf("secret[%d].type=%x\r\n",i,bleSecretInfo[i].type);
			
			if ((uTemp[0] == bleSecretInfo[i].secret[0]
					 && uTemp[1] == bleSecretInfo[i].secret[1]
					 && uTemp[2] == bleSecretInfo[i].secret[2]
					 && uTemp[3] == bleSecretInfo[i].secret[3]
					 && uTemp[4] == bleSecretInfo[i].secret[4]
					 && uTemp[5] == bleSecretInfo[i].secret[5]
					 && uTemp[6] == bleSecretInfo[i].secret[6]
					 && uTemp[7] == bleSecretInfo[i].secret[7]
					 && uTemp[8] == bleSecretInfo[i].secret[8]
					 && uTemp[9] == bleSecretInfo[i].secret[9])
					 && (bleSecretInfo[i].type == ADMIN_BT_STR)
					 )
			{
				cmpResult=1;
			}
	}
	
	if (cmpResult)
	{
		gBle.SendUartData[13]=0x1;
		isOpen=1;
	}
	btSendPack(gBle.SendUartData,36);
	
	if(isOpen)
	{
		printf("open\r\n");
		open_door();
	}
	else
	{
		printf("fail\r\n");
		AudioPlay(AUDIO_PROMPT_VALIDATION_FAIL);
	}
}
u32 nbPackId;
void bleDefault(void)
{
	uint8_t i=0;

	//判断是否已有管理员
	
	//发送协议
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_RESET_FACTORY;
	gBle.SendUartData[3]=0x0; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x21;
	/*数据  使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=BLE_RESPONSE_RESET_FACTORY;

	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[13+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[19+i]=defaultBleMac[i];
	}
	//电量
	gBle.SendUartData[33]=0x1;
	//手机号码
	for (i=0;i<11;i++)
	{

		gBle.SendUartData[34+i] = gBle.CommunicateValidUartData[i+1];
	}
	// XOR
	gBle.SendUartData[45] = 0x0;

	btSendPack(gBle.SendUartData,46);	

	//clear flash
	Reset_Factory_To_Default();
}

u8 nbUserData[32];
u8 btUserData[12];

void bleOperUser()
{
	u8 i=0;
	switch(/*gBle.CommunicateValidUartData[46]*/1)
	{
		case BLE_OPER_ADD:
			#if 0
			for(i=0;i<30;i++)
			{
				nbUserData[i] = gBle.CommunicateValidUartData[18+i];
			}
			//if(saveAddUser(nbUserData,32))
			{
				gBle.CommunicateValidUartData[1] = 1;
			}
			//else
			{
				gBle.CommunicateValidUartData[1] = 0;
			}
			gBle.CommunicateValidUartData[0] = 0xD5;
			gBle.CommunicateValidUartData[2] = batVData;
			for(i=0;i<20;i++)
			{
				gBle.CommunicateValidUartData[3+i]=getData[ADD_FACTORY_ID0+i];
			}
			btSendPack(gBle.CommunicateValidUartData,23);
			#else
			for (i=0; i<6; i++)
			{
				btUserData[i]=gBle.CommunicateValidUartData[i];
				printf("bt-p[%d]=%d\r\n",i,btUserData[i]);
			}
			
			if (RFIDTouchHave(ADMIN_PSW_STR,btUserData,6))
			{
				AudioPlay(AUDIO_PROMPT_RECORD_EXIST);  //用户信息已存在
				//发送响应
				//btSendPack
			}
			else
			{
				AudioPlay(AUDIO_PROMPT_PSWD_RECORD_SUCCESS);  //密码登记成功
				save(ADMIN_PSW_STR,btUserData,6);
				//发送响应
				//btSendPack
			}
			#endif
			break;
		case BLE_OPER_MODIFY:
		case BLE_OPER_DEL:
			#if 0
			for(i=0;i<30;i++)
			{
				nbUserData[i] = gBle.CommunicateValidUartData[18+i];
			}
			//if(saveModifyOrDelUser(nbUserData,32,gBle.CommunicateValidUartData[48]))
			{
			//	gBle.CommunicateValidUartData[1] = 1;
			}
			//else
			{
				gBle.CommunicateValidUartData[1] = 0;
			}
			gBle.CommunicateValidUartData[0] = 0xD5;
			gBle.CommunicateValidUartData[2] = batVData;
			for(i=0;i<20;i++)
			{
				gBle.CommunicateValidUartData[3+i]=getData[ADD_FACTORY_ID0+i];
			}
			btSendPack(gBle.CommunicateValidUartData,23);
//			saveModifyOrDelUser(&gBle.CommunicateValidUartData[1],32,NB_MODIFY);
		#else
		#endif
			break;
		default:
			break;
	}
}

void bleAddSecret()
{
	u8 i =0;
	u8 exist =0;
	printf("bleAddSecret\r\n");
	exist = saveAddSecret(gBle.CommunicateValidUartData,gBle.uDataValidLen);

	//发送协议
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[3]=xOrCnt; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*数据	使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[13]=exist;
	gBle.SendUartData[14]=0x50;

	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=defaultBleMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;

	//btSend(gBle.SendUartData,36);
	btSendPack(gBle.SendUartData,36);

}
void bleModifySecret()
{
	//saveModifyOrDelSecret(gBle.CommunicateValidUartData,gBle.uDataValidLen,gBle.CommunicateValidUartData[39]);
	gBle.CommunicateValidUartData[0]=0xD6;
	gBle.CommunicateValidUartData[1]=1;
	gBle.CommunicateValidUartData[2]=Battery_Percent;;
	memcpy(&gBle.CommunicateValidUartData[3],&getData[ADD_FACTORY_ID0],20);
	btSendPack(gBle.CommunicateValidUartData,23);
}

void bleDelSecret(void)
{
	u8 reslut=0; /* 0 失败， 1:成功*/
	u8 i=0;
	
	//reslut=saveModifyOrDelSecret(gBle.CommunicateValidUartData,gBle.uDataValidLen);
	//发送协议
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[3]=xOrCnt; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*数据	使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[13]=reslut;
	gBle.SendUartData[14]=0x50;

	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=defaultBleMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;

	btSendPack(gBle.SendUartData,36);
	
}

void bleOperSecret()
{
	printf("bleOperSecret=%d\r\n",gBle.CommunicateValidUartData[39]);
	switch(gBle.CommunicateValidUartData[39])
	{
		case BT_OPEN:
		case BT_ADD_USER:
		case BT_ADD_BLE_SECRET:
		case BT_ADD_NUMBER_SECRET:
		case BT_ADD_ID_CARD_SECRET:
		case BT_ADD_FINGER_SECRET:
		case BT_ADD_NFC_SECRET:
			bleAddSecret();
			break;
		case BT_MODIFY_USER:
		case BT_MODIFY_BLE_SECRET:
		case BT_MODIFY_NUMBER_SECRET:
		case BT_MODIFY_NFC_SECRET:
		case BT_MODIFY_ID_CARD_SECRET:
		case BT_MODIFY_FINGER_SECRET:
			//AudioPlay(AUDIO_PROMPT_DELETED);
			bleModifySecret();
			break;
		case BT_DEL_USER:
		case BT_DEL_BLE_SECRET:
		case BT_DEL_NUMBER_SECRET:
		case BT_DEL_NFC_SECRET:
		case BT_DEL_ID_CARD_SECRET:
		case BT_DEL_FINGER_SECRET:
			//AudioPlay(AUDIO_PROMPT_DELETED);
			bleDelSecret();
			break;
		default:
			break;
	}
			
}

void bleAddNfcCardNum(u8 *uNum,u8 uLength)
{
	u8 uDataTemp[10]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	memcpy(uDataTemp,uNum,uLength);
	gBle.CommunicateValidUartData[0] = 0xAE;
	gBle.CommunicateValidUartData[1] = Battery_Percent;;
	memcpy(&gBle.CommunicateValidUartData[2],&getData[ADD_FACTORY_ID0],20);
	memcpy(&gBle.CommunicateValidUartData[22],uDataTemp,10);
	gBle.CommunicateValidUartData[32] = 2;
	btSendPack(gBle.CommunicateValidUartData,33);	
}

void sendNfcFingerPrintData(u8 data_type, u8 state)
{
	u8 i=0;
	printf("s-nfc-d=%d\r\n",state);
	//发送协议
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONESE_ADD_NFC_FP;
	gBle.SendUartData[3]=0x0; //XOR
	//时间 使用6个字节
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//数据长度 使用2字节
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x21;
	/*数据  使用23个字节*/
	//命令字符
	gBle.SendUartData[12]=BLE_RESPONESE_ADD_NFC_FP;
	//电池电量
	gBle.SendUartData[13]=0x50;
	
	//厂商ID + 设备ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[14+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[20+i]=defaultBleMac[i];
	}

	if (data_type == BLE_ADD_DATA_TYPE_NFC)
	{
		//NFC UUID
		gBle.SendUartData[34] = RFIDData[0];
		gBle.SendUartData[35] = RFIDData[1];
		gBle.SendUartData[36] = RFIDData[2];
		gBle.SendUartData[37] = RFIDData[3];
		gBle.SendUartData[38] = 0xFF;
		gBle.SendUartData[39] = 0xFF;
		gBle.SendUartData[40] = 0xFF;
		gBle.SendUartData[41] = 0xFF;
		gBle.SendUartData[42] = 0xFF;
		gBle.SendUartData[43] = 0xFF;
	}
	else if (data_type == BLE_ADD_DATA_TYPE_FP)
	{
		gBle.SendUartData[34] = Admin_Flag[BT_Admin_FPRT];
		gBle.SendUartData[35] = 0xFF;
		gBle.SendUartData[36] = 0xFF;
		gBle.SendUartData[37] = 0xFF;
		gBle.SendUartData[38] = 0xFF;
		gBle.SendUartData[39] = 0xFF;
		gBle.SendUartData[40] = 0xFF;
		gBle.SendUartData[41] = 0xFF;
		gBle.SendUartData[42] = 0xFF;
		gBle.SendUartData[43] = 0xFF;
	}
	//状态
	gBle.SendUartData[44] = state;
	// XOR
	gBle.SendUartData[45] = 0x1;
	
	btSend(gBle.SendUartData,46);
	
}

void bleAddNfcFingerPrint()
{
	//判断是添加NFC 还是指纹
	printf("bleAddNfc type=%d\r\n",gBle.CommunicateValidUartData[gBle.uDataValidLen-1]);
	if (gBle.CommunicateValidUartData[gBle.uDataValidLen-1] == BLE_ADD_DATA_TYPE_FP)
	{
		//添加指纹
		printf("fp state=%d\r\n",gBle.CommunicateValidUartData[gBle.uDataValidLen-2]);
		//发送已建立添加NFC通讯
		if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_START)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_START);
			bleFlg = 0;
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_ING)
		{
			//响应 APP已准备录入NFC
			bleFlg = BLE_ADD_FP_WAIT;
			sysFlg = SYS_BLE_START_FP_SET;
			bleBuff = 0;
			AudioPlay(AUDIO_PROMPT_SETTING_START); //提示请输入磁卡
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_SUCESS)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_SUCESS);
			bleFlg = 0;
		}
	}
	else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-1] == BLE_ADD_DATA_TYPE_NFC)
	{
		//添加NFC
		printf("state=%d\r\n",gBle.CommunicateValidUartData[gBle.uDataValidLen-2]);
		//发送已建立添加NFC通讯
		if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_START)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_START);
			bleFlg = 0;
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_ING)
		{
			//响应 APP已准备录入NFC
			bleFlg = BLE_ADD_NFC_WAIT;
			sysFlg = SYS_BLE_NFC_SETUP;
			bleBuff = 0;
			AudioPlay(AUDIO_PROMPT_SETTING_START); //提示请输入磁卡
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_SUCESS)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_SUCESS);
			bleFlg = 0;
		}
	}
	else
	{
	}
	
}

void bleIdCard(uint8_t uStatus,u8 uCmd)
{
	u8 i=0;
	gBle.uDataValidLen -= 22;
	for(i=0;i<gBle.uDataValidLen;i++)
	{
		gBle.CommunicateValidUartData[i]=gBle.CommunicateValidUartData[21+i];
	}
	if(uCmd == BLE_ADD_NFC_WAIT)
	{
		gBle.CommunicateValidUartData[0]=0xAE;
	}
	else
	{
		gBle.CommunicateValidUartData[0]=0xAD;
	}
	gBle.CommunicateValidUartData[1]=Battery_Percent;;
	for(i=0;i<20;i++)
	{
		gBle.CommunicateValidUartData[2+i]=getData[ADD_FACTORY_ID0+i];
	}
	gBle.CommunicateValidUartData[22]=0;
	gBle.CommunicateValidUartData[23]=uStatus;
	btSendPack(&gBle.CommunicateValidUartData[0],24);
	if(uStatus == 3)//
	{
	}
	else if(uStatus == 1)
	{
		AudioPlay(AUDIO_PROMPT_BI);		
	}
	else if(uStatus == 0)
	{
		AudioPlay(AUDIO_PROMPT_VALIDATION_FAIL);				
	}
	//TypeB_test();
}
extern unsigned char B3TimeFlg;
u8 sleepCnt;
u8 send_cmd=0;

void Uart2_AT_send(uint8_t* Data,uint8_t len)
{
	uint8_t i = 0;
	for(;i<len;i++)
	{

			USART_SendData(USART2,Data[i]);
	}
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET)
			;
}
void bleEnterInstruct()
{
	//btSend("+++",3);
	#if 0
	u8 at_value[5]= {0x41,0x54,0x2b,0x6F,0x6B};
	//u8 cmd[3]={'+','+','+'};
	u8 temp[5]={0};
	u8 i=0;
	for(i=0;i<5;i++)
	{
		temp[i]=gBle.CommunicateUartData[3+i];
		//printf("temp[%d]=%x\r\n",i,temp[i]);
	}
	printf("temp0=%x\r\n",gBle.CommunicateUartData[0]);
	printf("temp1=%x\r\n",gBle.CommunicateUartData[1]);
	printf("temp2=%x\r\n",gBle.CommunicateUartData[2]);
	printf("temp3=%x\r\n",gBle.CommunicateUartData[3]);
	printf("temp2=%x\r\n",gBle.CommunicateUartData[4]);
	printf("temp4=%x\r\n",gBle.CommunicateUartData[5]);
	printf("temp5=%x\r\n",gBle.CommunicateUartData[6]);
	printf("temp6=%x\r\n",gBle.CommunicateUartData[7]);
	#endif
	//gBle.uDataLen=0;
	//if (memcmpStr(at_value,temp,5))
	//{
	//	send_cmd =1;
	//	printf("success\r\n");
	//}
	//if (send_cmd == 0)
	//
	//{
		while(gBle.CommunicateUartData[0] !=0x41)
		{
			Uart2_AT_send("+++",3);
			soft_delay_ms(100);
			printf("+++\r\n");
		}
	//}
	
		while(gBle.CommunicateUartData[0] !=0x41)
		{
			Uart2_AT_send("AT+getAddr\r\n",12);
			soft_delay_ms(100);
			printf("AT+getAddr\r\n");
		}
	//}
}
void bleReadMac()
{
	//btSend("AT+getAddr\r\n",12);
	Usart2_SendString("AT+getAddr\r\n",12);
	soft_delay_ms(500);
}
void bleReStart()
{
	btSend("AT+reStart\r\n",12);
}

void getBleMac(void)
{
	u8 i=0;
	bleEnterInstruct();
	////bleReadMac();
	//printf("len=%d\r\n",gBle.uDataLen);
	for(i=0;i<12;i++)
	{
		gBleMac[i] = gBle.CommunicateUartData[i+6];
		printf("mac[%d]=%x\r\n",i,gBleMac[i]);
	}
	soft_delay_ms(10);
	//btSend("AT+exit\r\n",9);
	printf("AT+exit\r\n");
}

void blePro(void)
{
  #if 0
	if (gBle.CommunicateUartData[0] == 0x89
			&& gBle.CommunicateUartData[1]==0x89
			&& gBle.uDataLen>0)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_START);
			gBle.uDataLen=0;
		}
	#endif
	if(btGetHead())
	{
		bleGetRuoChanPro();
		bleFlg = gBle.CommunicateUartData[2];
		gBle.uDataLen=0;
		sleepCnt = 0;	
	}

	switch(bleFlg)
	{
		case BLE_GET_DEVICE:
			bleGetDevice();
			bleFlg = 0;
			break;
		case BLE_REGISTER_DEVICE:
			bleRegisterDevice();
			bleFlg = 0;
			break;
		case BLE_OPEN:
			bleOpen();
			bleFlg = 0;
			break;
		case BLE_SETUP_IP:
			bleFlg = 0;
			break;
		case BLE_ID_CARD:
			//bleIdCard();
			bleFlg = BLE_ID_CARD_WAIT;
			bleBuff = 0;
			break;
		case BLE_OPER_USER:
			bleOperUser();
			bleFlg = 0;
			break;
		case BLE_OPER_SECRET:
			bleOperSecret();
			bleFlg = 0;
			break;
		case BLE_ADD_NFC_FP:
			bleAddNfcFingerPrint();
			//bleFlg = 0;
			break;		
		case BLE_ADD_NFC_WAIT:
		case BLE_ID_CARD_WAIT:
			//printf("B3TimeFlg=%d\r\n",B3TimeFlg);
			if(B3TimeFlg&T_1000MS)
			{
				bleBuff ++;
				if(bleBuff > 15)
				{
					bleBuff = 0;
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_FAIL);
					AudioPlay(AUDIO_PROMPT_VALIDATION_TIMEOUT);
					bleFlg = 0;
				}
				//printf("add=0x%x0x%x0x%x0x%x\r\n",RFIDData[0],RFIDData[1],RFIDData[2],RFIDData[3]);
				if (RFIDData[0]!=0
						&& RFIDData[1]!=0
						&& RFIDData[2]!=0
						&& RFIDData[3]!=0)
				{
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_ING);
					bleFlg = 0;
					bleBuff = 0;
				}
			}
			break;
		case BLE_ADD_FP_WAIT:
			printf("111B3TimeFlg=%d\r\n",B3TimeFlg);
			if(B3TimeFlg&T_1000MS)
			{
				bleBuff ++;
				if(bleBuff > 100)
				{
					bleBuff = 0;
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_FAIL);
					AudioPlay(AUDIO_PROMPT_VALIDATION_TIMEOUT);
					bleFlg = 0;
				}
				//printf("add=0x%x0x%x0x%x0x%x\r\n",RFIDData[0],RFIDData[1],RFIDData[2],RFIDData[3]);
				if (Admin_Flag[BT_Admin_FPRT]>0)
				{
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_ING);
					bleFlg = 0;
					bleBuff = 0;
				}
			}
			break;
		case BLE_RESET_FACTORY:
			bleDefault();
			bleFlg = 0;
			break;
		default:
			break;
	}



	
}

#endif

