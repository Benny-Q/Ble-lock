

#include "ble.h"
#include "bsp_usart2.h"
#include "bsp_key.h"


//#include "cardOper.h"
//�������ͣ�
//0.����              1.�����û�     2.�޸��û�    3.ɾ���û�    4.������������ 
//5.�޸���������      6.ɾ���������� 7������������ 8�޸��������� 9ɾ���������� 
//10����nfc           11�޸�nfc      12ɾ��nfc     13�������֤  14�޸����֤ 
//15ɾ�����֤        16����ָ��     17�޸�ָ��    18ɾ��ָ��

//USER  :11(�ֻ�)+6����������+5��ʼʱ��+5����ʱ��+3Ȩ��
//SECRET:1(���)+1����+10(����)+5��ʼʱ��+5����ʱ��+1����

#ifdef __BLUETOOTH_SUPPORT__

/***********�궨������***************************************/
#define BLE_OPER_ADD			1
#define BLE_OPER_MODIFY			2
#define BLE_OPER_DEL			3

/*******************��������****************************************/
uint8_t bleFlg=0;
uint8_t bleCnt=0;
uint8_t bleBuff=0;
CommunicateType gBle;

unsigned char sendUartData[70]={0};
uint8_t xOrCnt=0;
uint8_t uartDataLengthContinut=0;
u16 uartDataTemp1Length=0;
uint8_t uartDataXor=0;

//����ID ǰ4λ+ �豸ID ��2λ
static u8 ManufacturerId[6]={0x30,0x30,0x31,0x31,0x30,0x35};
//static u8 defaultBleMac[14]={0x35,0x35,0x31,0x33,0x31,0x34,0x41,0x45,0x46,0x44,0x41,0x34,0xFF,0xFF};
//static u8 defaultBleMac[14]={0x35,0x35,0x34,0x34,0x31,0x34,0x41,0x45,0x46,0x44,0x41,0x34,0xFF,0xFF};

u8 gBleMac[14]={0};


/*******ȫ�ֱ�������**************************************/
extern u8 getData[64];
extern uint8_t  uStatus;
extern UserType currUser;
extern UserType bleUserInfo[10];
extern SecretType bleSecretInfo[10];
extern u8 BluetoothMac[BULETOOTH_MAC_MAX];
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

	//�Ƚ����ݷŵ� sendUartData
	
	for (i=0; i<uLength; i++)
	{
		sendUartData[i] = (*(uData+i));
	}
	
	//���ݰ�����
	for (i=0; i<sendUartData[11];i++)
	{
		sendUartData[12+i] = xOrCnt^sendUartData[12+i];
	}
	//sendUartData[3]=xOrcnt;

	//������������ XOR

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
	u8 i=0;
	//u8 tmp_data[70]={0};
	if (gBle.CommunicateUartData[0]==0x89&&gBle.CommunicateUartData[1]==0x89
			&& gBle.uDataLen >= 6 )
	{
		//printf("data=%d\r\n",gBle.uDataLen);
		for(i=0;i<gBle.uDataLen;i++)
		{
			printf("rev-data[%d]=%x\r\n",i,gBle.CommunicateUartData[i]);
		}
		
		return 1;
	}
	return 0;
}

void bleGetRuoChanPro(void)
{
	uint8_t i=0;

	memsetStr(gBle.CommunicateValidUartData,0,communicateUartDataMax1);
	
	gBle.uDataValidLen = gBle.CommunicateUartData[11];
	
	for(i=0;i<gBle.uDataValidLen;i++)
	{
		//������Ч����
		gBle.CommunicateValidUartData[i] = gBle.CommunicateUartData[12+i];
		printf("CommunicateValidUartData[%d]=%x\r\n",i,gBle.CommunicateValidUartData[i]);
	}

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
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*����  ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=0x11;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[13+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[19+i]=BluetoothMac[i];
	}
	//�����ٷֱ�
	gBle.SendUartData[33] = Battery_Percent;//  80%
	//����ģʽ
	gBle.SendUartData[34] = 0x2;
	// XOR
	gBle.SendUartData[35] = 0x0;
	
	btSendPack(gBle.SendUartData,36);

}

u8 addBtAdmin(void)
{
	u8 admin_exist=0;
	//u8 i=0;
	
	//printf("flag=%d,pwd=%x\r\n",Admin_Flag[BT_Admin],bleUserInfo[0].secret[0]);
	
	//�ж��Ƿ����й���Ա
	if ((Admin_Flag[BT_Admin]>0) && (Admin_Flag[BT_Admin]!=0xFF))
	{
		//���й���Ա
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
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_REGISTER_DEVICE;
	gBle.SendUartData[3]=0x0; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*����  ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_REGISTER_DEVICE;
	gBle.SendUartData[13]=admin_exist;
	gBle.SendUartData[14]=0x50;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=BluetoothMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x1;
	btSendPack(gBle.SendUartData,36);
}
void bleOpen()
{
	uint8_t i=0;
	u8 uTemp[10];
	u8 isOpen=0;
	u8 cmpResult=0;
	
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPEN;
	gBle.SendUartData[3]=0x0; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*����  ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_OPEN;
	gBle.SendUartData[13]=0x0;
	gBle.SendUartData[14]=0x50;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=BluetoothMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;
	

	for (i=0; i<10; i++)
	{
		uTemp[i]=gBle.CommunicateValidUartData[18+i];
		//printf("uTemp[%d]=%x\r\n",i,uTemp[i]);
	}
	//��������������бȽ�
	//printf("openadmin=%x\r\n",Admin_Flag[BT_Admin]);

	for (i=0; i<Admin_Flag[BT_Admin]; i++)
	{
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
		//printf("open\r\n");
		open_door();
	}
	else
	{
		//printf("fail\r\n");
		AudioPlay(AUDIO_PROMPT_VALIDATION_FAIL);
	}
}
u32 nbPackId;
void bleDefault(void)
{
	uint8_t i=0;

	//�ж��Ƿ����й���Ա
	
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_RESET_FACTORY;
	gBle.SendUartData[3]=0x0; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x21;
	/*����  ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_RESET_FACTORY;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[13+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[19+i]=BluetoothMac[i];
	}
	//����
	gBle.SendUartData[33]=0x1;
	//�ֻ�����
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

void bleAddUserInfo(void)
{
	u8 i=0;
	u8 state=0;
	
	state = saveAddUser(gBle.CommunicateValidUartData,gBle.uDataValidLen);
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPER_USER;
	gBle.SendUartData[3]=0x0; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*����  ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_OPER_USER;
	gBle.SendUartData[13]=state;
	gBle.SendUartData[14]=0x50;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=BluetoothMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;


	btSendPack(gBle.SendUartData,36);	

}

void bleOperUser()
{
	u8 operType=gBle.CommunicateValidUartData[gBle.uDataValidLen-1];

	printf("operuser=%d\r\n",operType);
	
	switch(operType)
	{
		case BLE_OPER_ADD:
			bleAddUserInfo();
			break;
		case BLE_OPER_MODIFY:
		case BLE_OPER_DEL:
			break;
		default:
			break;
	}
}

void bleAddSecret(u8 operType)
{
	u8 i =0;
	u8 exist =0;
	printf("bleAddSecret\r\n");
	exist = saveAddSecret(gBle.CommunicateValidUartData,operType);

	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[3]=xOrCnt; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*����	ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[13]=exist;
	gBle.SendUartData[14]=0x50;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=BluetoothMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;

	//btSend(gBle.SendUartData,36);
	btSendPack(gBle.SendUartData,36);

}
void bleModifySecret()
{
}

void bleDelSecret(u8 operType)
{
	u8 result=0; /* 0 ʧ�ܣ� 1:�ɹ�*/
	u8 i=0;
	
	result=btDelSecretUserInfo(gBle.CommunicateValidUartData,operType);
	printf("del-result=%d\r\n",result);
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[3]=xOrCnt; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x17;
	/*����	ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_OPER_SECRET;
	gBle.SendUartData[13]=result;
	gBle.SendUartData[14]=0x50;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[15+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[21+i]=BluetoothMac[i];
	}
	// XOR
	gBle.SendUartData[35] = 0x0;

	btSendPack(gBle.SendUartData,36);
	
}

void bleOperSecret()
{
	u8 operType = gBle.CommunicateValidUartData[39];
	printf("bleOperSecret=%d\r\n",operType);
	
	switch(operType)
	{
		case BT_OPEN:
		case BT_ADD_USER:
		case BT_ADD_BLE_SECRET:
		case BT_ADD_NUMBER_SECRET:
		case BT_ADD_ID_CARD_SECRET:
		case BT_ADD_FINGER_SECRET:
		case BT_ADD_NFC_SECRET:
			bleAddSecret(operType);
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
			bleDelSecret(operType);
			break;
		default:
			break;
	}
			
}

void sendNfcFingerPrintData(u8 data_type, u8 state)
{
	u8 i=0;
	printf("s-nfc-d=%d\r\n",state);
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONESE_ADD_NFC_FP;
	gBle.SendUartData[3]=0x0; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];

	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x21;
	/*����  ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONESE_ADD_NFC_FP;
	//��ص���
	gBle.SendUartData[13]=0x50;
	
	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[14+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[20+i]=BluetoothMac[i];
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
	//״̬
	gBle.SendUartData[44] = state;
	// XOR
	gBle.SendUartData[45] = 0x1;
	
	btSend(gBle.SendUartData,46);
	
}

void bleAddNfcFingerPrint()
{
	//�ж������NFC ����ָ��
	//printf("bleAddNfc type=%d\r\n",gBle.CommunicateValidUartData[gBle.uDataValidLen-1]);
	if (gBle.CommunicateValidUartData[gBle.uDataValidLen-1] == BLE_ADD_DATA_TYPE_FP)
	{
		//���ָ��
		//printf("fp state=%d\r\n",gBle.CommunicateValidUartData[gBle.uDataValidLen-2]);
		//�����ѽ������ָ��ͨѶ
		if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_START)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_START);
			bleFlg = 0;
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_ING)
		{
			//��Ӧ APP��׼��¼��ָ��
			bleFlg = BLE_ADD_FP_WAIT;
			sysFlg = SYS_BLE_START_FP_SET;
			bleBuff = 0;
			AudioPlay(AUDIO_PROMPT_SETTING_START); //��ʾ������ſ�
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_SUCESS)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_SUCESS);
			sysFlg=0;//�˳�ϵͳ����
			bleFlg = 0;
		}
	}
	else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-1] == BLE_ADD_DATA_TYPE_NFC)
	{
		//���NFC
		printf("state=%d\r\n",gBle.CommunicateValidUartData[gBle.uDataValidLen-2]);
		//�����ѽ������NFCͨѶ
		if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_START)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_START);
			bleFlg = 0;
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_ING)
		{
			//��Ӧ APP��׼��¼��NFC
			bleFlg = BLE_ADD_NFC_WAIT;
			sysFlg = SYS_BLE_NFC_SETUP;
			bleBuff = 0;
			memsetStr(RFIDData,0x0,sizeof(RFIDData));
			AudioPlay(AUDIO_PROMPT_SETTING_START); //��ʾ������ſ�
		}
		else if (gBle.CommunicateValidUartData[gBle.uDataValidLen-2] == BLE_ADD_DATA_SUCESS)
		{
			sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_SUCESS);
			sysFlg=0;//�˳�ϵͳ����
			bleFlg = 0;
		}
	}
	else
	{
	}
	
}

void bleOpenDoorInfo(void)
{
	u8 i=0;
	
	printf("bleOpenDoorInfo\r\n");
	//����Э��
	gBle.SendUartData[0]=0x89;
	gBle.SendUartData[1]=0x89;
	gBle.SendUartData[2]=BLE_RESPONSE_OPEN_INFO;
	gBle.SendUartData[3]=xOrCnt; //XOR
	//ʱ�� ʹ��6���ֽ�
	gBle.SendUartData[4]=gBle.CommunicateUartData[4];
	gBle.SendUartData[5]=gBle.CommunicateUartData[5];
	gBle.SendUartData[6]=gBle.CommunicateUartData[6];
	gBle.SendUartData[7]=gBle.CommunicateUartData[7];
	gBle.SendUartData[8]=gBle.CommunicateUartData[8];
	gBle.SendUartData[9]=gBle.CommunicateUartData[9];
	//���ݳ��� ʹ��2�ֽ�
	gBle.SendUartData[10]=0x0;
	gBle.SendUartData[11]=0x27;
	/*����	ʹ��23���ֽ�*/
	//�����ַ�
	gBle.SendUartData[12]=BLE_RESPONSE_OPEN_INFO;
	//�û��ֻ���
	for (i=0;i<11;i++)
		gBle.SendUartData[13+i]=0x0;

	//����ʱ��
	for (i=0;i<5;i++)
		gBle.SendUartData[24+i]=0x0;
	//��������
	gBle.SendUartData[29]=0x0;
	//����
	gBle.SendUartData[30]=Battery_Percent;

	//����ID + �豸ID
	for (i=0;i<6;i++)
	{
		gBle.SendUartData[31+i]=ManufacturerId[i];
	}
	// MAC
	for(i=0;i<14;i++)
	{
		gBle.SendUartData[37+i]=BluetoothMac[i];
	}
	// XOR
	gBle.SendUartData[51] = 0x0;

	btSendPack(gBle.SendUartData,52);
	
}


extern unsigned char B3TimeFlg;
//u8 sleepCnt;

void getMacFormUart(void)
{
	u8 i=0,j=0;
	//u8 tmpMac[14]={0};

	for(i=0;i<gBle.uDataLen;i++)
	{
		//printf("uart[%d]=%x\r\n",i,gBle.CommunicateUartData[i]);
		if (i>5)
		{
			if (gBle.CommunicateUartData[i] != 0x3A 
					&& gBle.CommunicateUartData[i] != 0xa 
					&& gBle.CommunicateUartData[i] != 0xd)
			{
				BluetoothMac[j] = gBle.CommunicateUartData[i];
				//printf("mac[%d]=%x\r\n",j,BluetoothMac[j]);
				j++;
			}
		}
	}

	Admin_Flag[BT_Mac]=1;
	SaveData_Inf(BT_System_Data,BT_System_Page);

}

void InitializationBTMac(void)
{
	
	u8 exist=0;

	//for(u8 i=0;i<14;i++)
	//	printf("mac[%d]=%x\r\n",i,BluetoothMac[i]);
		
	if (Admin_Flag[BT_Mac] ==1)
	{
		exist=1;
	}
	//printf("exist=%d\r\n",exist);
	
	//�ж��Ƿ��Ѿ�����mac����
	if (!exist)
	{
		soft_delay_ms(2000);
		Usart2_SendString("+++",3);
		soft_delay_ms(500);
		
		gBle.uDataLen=0;
		Usart2_SendString("AT+getAddr\r\n",12);
		soft_delay_ms(500);
		getMacFormUart();
		
		Usart2_SendString("AT+exit\r\n",9);
		soft_delay_ms(500);
		gBle.uDataLen=0;
	}

}

void blePro(void)
{

	if(btGetHead())
	{
		bleGetRuoChanPro();
		bleFlg = gBle.CommunicateUartData[2];
		gBle.uDataLen=0;
		//sleepCnt = 0;	
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
				if (sysAddDeviceState ==1)
				{
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_NFC,BLE_ADD_DATA_ING);
					memsetStr(RFIDData,0x0,sizeof(RFIDData));
					bleFlg = 0;
					bleBuff = 0;
					sysAddDeviceState=0;
				}
			}
			break;
		case BLE_ADD_FP_WAIT:
			//printf("111B3TimeFlg=%d\r\n",B3TimeFlg);
			if(B3TimeFlg&T_1000MS)
			{
				bleBuff ++;
				if(bleBuff > 15)
				{
					bleBuff = 0;
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_FAIL);
					AudioPlay(AUDIO_PROMPT_VALIDATION_TIMEOUT);
					bleFlg = 0;
					sysFlg=0;//�˳�ϵͳ����
					sysAddDeviceState=0;
				}
				//printf("add=0x%x0x%x0x%x0x%x\r\n",RFIDData[0],RFIDData[1],RFIDData[2],RFIDData[3]);
				if (sysAddDeviceState ==Result_Succes)
				{
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_ING);
					bleFlg = 0;
					bleBuff = 0;
					sysAddDeviceState=0;
				}
				else if (sysAddDeviceState ==Result_Failed || sysAddDeviceState ==Result_Timeout)
				{
					sendNfcFingerPrintData(BLE_ADD_DATA_TYPE_FP,BLE_ADD_DATA_FAIL);
					bleFlg = 0;
					bleBuff = 0;
					sysFlg=0;//�˳�ϵͳ����
					sysAddDeviceState=0;
				}
			}
			break;
		case BLE_RESET_FACTORY:
			bleDefault();
			bleFlg = 0;
			break;
		case BLE_OPEN_INFO:
			bleOpenDoorInfo();
			break;
		default:
			break;
	}



	
}

#endif

