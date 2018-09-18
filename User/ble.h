#ifndef __BLE_H
#define __BLE_H

#include "mains.h"

extern void blePro(void);
extern void btSend(uint8_t *data,uint8_t length);
extern void btSendPack(uint8_t *uData,uint8_t uLength);
extern uint8_t btGetHead(void);

/*��������*/ 
typedef enum {	
CMD_TYPE1 = 'i', /*��������*/ //0x69 74 63 7A  
CMD_TYPE2 = 't', /*��������*/  
CMD_TYPE3 = 'c', /*��������*/  
CMD_TYPE4 = 'z', /*��������*/  
ACK_TYPE1 = 'i', /*��Ӧ����*/ //0x69 74 61 7A  
ACK_TYPE2 = 't', /*��Ӧ����*/  
ACK_TYPE3 = 'a', /*��Ӧ����*/  
ACK_TYPE4 = 'z', /*��Ӧ����*/  
UNKNOW_TYPE = 0xff, /**/
} CommandClass_t; 
	/*�豸��ģ������*/ 
	//��������Ϊ GATT Э����� SPP Э��ר���������ź��б�ער�ã��� 
	typedef enum {	
	CMD_VERSION 	= 1,  /*��0x01����ȡ����汾*/	
	CMD_RESET	   = 2,  /*��0x02��ģ�鸴λ*/  
	CMD_ORGL	  = 3,	/*��0x03���ָ�����״̬(�����������)*/	
	CMD_MRAD	  = 10,  /*��0x0a����ȡ������ӵ�������֤�豸��ַ*/  
	CMD_STATE	   = 11,  /*��0x0b����ȡ��������״̬*/	
	CMD_DISCOVERABLE	= 13,  /*��0x0d���ɷ��ֺ͹㲥״̬����*/  
	CMD_DISC	  = 15,  /*��0x0f������ģ��Ͽ����ֻ�������*/  
	CMD_SPPDATA 	= 16,  /*��0x10�����ͽ���SPP����*///(SPPЭ��ר��)  
	CMD_BATMEAS 	= 18,  /*��0x12����ѯ��ص���*/  
	CMD_POWERDOWN	 = 19,	/*��0x13���ػ�*/  
	CMD_READADDR	 = 20,	/*��0x14����ȡ����ģ��ĵ�ַ*/	
	CMD_ATTDATA 	= 21,  /*��0x15�����ͽ���LE����*///(GATTЭ��ר��)  
	CMD_REENDISCOVERABLE  = 22,  /*��0x16�����ƶϿ����Ƿ��Զ�����ɷ���״̬*/  
	CMD_LEADVPARAMS    = 23,  /*��0x17������LE�㲥���ʱ��*///(GATTЭ��ר��)	
	CMD_RSSI	  = 24,  /*��0x18����ȡ�����ź�ǿ��*/  
	CMD_LECONPARAMS    = 25,  /*��0x19������LE���Ӽ��ʱ��*///(GATTЭ��ר��)	
	CMD_UARTBAUD	 = 26,	/*��0x1a������UART������*/	
	CMD_RENAME	   = 27,  /*��0x1b���޸��豸��*/  
	CMD_MODBTADDR	 = 28,	/*��0x1c���޸�ģ��������ַ*/ 
	CMD_MODIFY_SERVICE_NAME = 29,
	CMD_UART_RXFIFO_LEVEL  = 31,   /*��0x1f�����ڽ���fifoָʾ*/ 
	CMD_BT_RXFIFO_LEVEL   = 32,  /*��0x20����������fifoָʾ*/ 
	CMD_POLL_TIME	  = 33,  /*��0x21�����淢�ͼ��ʱ��(ms)*/ 
	CMD_MOD_IF_CHECKSUM  = 38,	/*��0x26�������Ƿ�ʹ��checksum*/ 
	CMD_IAP2_START		=42,
	CMD_IAP2_FILE		=44,
	CMD_IAP2_FINISH		=46,
	CMD_SET_GPIO	 = 52,	/*����gpio��ƽ*/ 
	CMD_ADV_DUAL	 = 54,	/*�����Ƿ���androidʶ��Ϊ˫ģģ��*/ 
	CMD_LED_STATE	 = 55,	/*����ʹ��LED״̬*/ 
	CMD_TESTMODE	= 62,  /*�������ģʽ*/
	CMD_OTAMODE    = 65,  /*����OTAģʽ*/
	CMD_LE_SET_SCAN 	= 66,	  /*LEʹ��ɨ����Χ�豸*/
	CMD_LE_CONNECT	   = 67,  /*LE�����ض���ַ�豸*/   
	CMD_LE_EXIT_CONNECT    = 68,  /*LE�˳������ض���ַ�豸*/   
	CMD_LE_DISCONNECT	 = 69,	/*LE�Ͽ��ض��豸����*/ 
	CMD_LE_FIND_SERVICES	= 70,  /*LE���ҷ���*/	
	CMD_LE_FIND_CHARA	 = 71,	/*LE��������*/	 
	CMD_LE_FIND_DESC	 = 72,	/*LE��������*/	 
	CMD_LE_SET_NOTIFY	 = 73,	/*LE����ʹ��֪ͨ����*/	 
	CMD_LE_XFER 	 = 74,	/*LE�շ�����,����֪ͨ����*/   
	CMD_LE_READ 	 = 75,	/*LE������*/ 
	CMD_LE_REMOTEN	   = 76,	/*LE��ѯ��ǰ���ӵ��豸*/ 
	CMD_LE_XFER2H     = 77,
	CMD_READ_NAME	  = 0x801B, /*���豸��*/  
	CMD_UNKNOW	   = 0xffff,  /*δ֪����*/ 
	} Command_t;  


#define BLE_GET_DEVICE		0x21
#define BLE_RESPONSE_GET_DEVICE 0x11
#define BLE_REGISTER_DEVICE	0x23
#define BLE_RESPONSE_REGISTER_DEVICE	0x13
#define BLE_OPEN_INFO		0x24
#define BLE_RESPONSE_OPEN_INFO		0x14
#define	BLE_WARM_INFO		0x25
#define	BLE_RESPONSE_WARM_INFO		0x15
#define BLE_RESET_FACTORY			0x26
#define BLE_RESPONSE_RESET_FACTORY			0x16
#define BLE_OPEN			0x41
#define BLE_RESPONSE_OPEN			0x31
#define BLE_OPER_USER		0x42
#define BLE_RESPONSE_OPER_USER		0x32
#define BLE_OPER_SECRET		0x43
#define BLE_RESPONSE_OPER_SECRET		0x33
#define BLE_ADD_NFC_FP			0x29
#define BLE_RESPONESE_ADD_NFC_FP 0x19
#define BLE_SETUP_IP		0xAC
#define BLE_ID_CARD			0xAD
#define BLE_ID_CARD_WAIT	0xF0
#define BLE_ID_CARD_FINISH	0xF1
#define BLE_ADD_NFC_WAIT	0xF7
#define BLE_ADD_FP_WAIT	0xF8

//�������״̬
#define BLE_ADD_DATA_START 0x3
#define BLE_ADD_DATA_ING 0x2
#define BLE_ADD_DATA_SUCESS 0x1
#define BLE_ADD_DATA_FAIL 0x0

//�����������
#define BLE_ADD_DATA_TYPE_FP 0x1
#define BLE_ADD_DATA_TYPE_NFC 0x2


extern void bleGetRuoChanPro(void);
extern void bleIdCard(uint8_t uStatus,u8 uCmd);
extern uint8_t openCheckPro(uint8_t *uData);
extern void defaultData(void);
extern void InitializationBTMac(void);

/*
�������ͣ�0.���� 1.�����û� 2.�޸��û� 3.ɾ���û� 4.������������ 5.�޸��������� 6.ɾ���������� 
			  7.������������ 8.�޸��������� 9.ɾ���������� 10.����nfc 11.�޸�nfc 12.ɾ��nfc 13.�������֤ 14.�޸����֤ 
			  15.ɾ�����֤ 16.����ָ�� 17.�޸�ָ�� 18.ɾ��ָ�� 19.���Э��ָ��

*/
#define BT_OPEN			0  //������������
#define BT_ADD_USER					1
#define BT_MODIFY_USER				2
#define BT_DEL_USER					3
#define BT_ADD_BLE_SECRET		4
#define BT_MODIFY_BLE_SECRET	5
#define BT_DEL_BLE_SECRET		6
#define BT_ADD_NUMBER_SECRET	7
#define BT_MODIFY_NUMBER_SECRET	8
#define BT_DEL_NUMBER_SECRET	9
#define BT_ADD_NFC_SECRET		10
#define BT_MODIFY_NFC_SECRET	11
#define BT_DEL_NFC_SECRET		12
#define BT_ADD_ID_CARD_SECRET		13
#define BT_MODIFY_ID_CARD_SECRET	14
#define BT_DEL_ID_CARD_SECRET		15
#define BT_ADD_FINGER_SECRET		16
#define BT_MODIFY_FINGER_SECRET		17
#define BT_DEL_FINGER_SECRET		18






#endif
