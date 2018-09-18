#ifndef __BLE_H
#define __BLE_H

#include "mains.h"

extern void blePro(void);
extern void btSend(uint8_t *data,uint8_t length);
extern void btSendPack(uint8_t *uData,uint8_t uLength);
extern uint8_t btGetHead(void);

/*命令类型*/ 
typedef enum {	
CMD_TYPE1 = 'i', /*命令类型*/ //0x69 74 63 7A  
CMD_TYPE2 = 't', /*命令类型*/  
CMD_TYPE3 = 'c', /*命令类型*/  
CMD_TYPE4 = 'z', /*命令类型*/  
ACK_TYPE1 = 'i', /*响应类型*/ //0x69 74 61 7A  
ACK_TYPE2 = 't', /*响应类型*/  
ACK_TYPE3 = 'a', /*响应类型*/  
ACK_TYPE4 = 'z', /*响应类型*/  
UNKNOW_TYPE = 0xff, /**/
} CommandClass_t; 
	/*设备和模组命令*/ 
	//部分命令为 GATT 协议或者 SPP 协议专用命令（命令号后有备注专用）。 
	typedef enum {	
	CMD_VERSION 	= 1,  /*（0x01）获取软件版本*/	
	CMD_RESET	   = 2,  /*（0x02）模块复位*/  
	CMD_ORGL	  = 3,	/*（0x03）恢复出厂状态(清除各种设置)*/	
	CMD_MRAD	  = 10,  /*（0x0a）获取最近连接的蓝牙认证设备地址*/  
	CMD_STATE	   = 11,  /*（0x0b）获取蓝牙工作状态*/	
	CMD_DISCOVERABLE	= 13,  /*（0x0d）可发现和广播状态控制*/  
	CMD_DISC	  = 15,  /*（0x0f）命令模组断开与手机的连接*/  
	CMD_SPPDATA 	= 16,  /*（0x10）发送接收SPP数据*///(SPP协议专用)  
	CMD_BATMEAS 	= 18,  /*（0x12）查询电池电量*/  
	CMD_POWERDOWN	 = 19,	/*（0x13）关机*/  
	CMD_READADDR	 = 20,	/*（0x14）读取蓝牙模组的地址*/	
	CMD_ATTDATA 	= 21,  /*（0x15）发送接收LE数据*///(GATT协议专用)  
	CMD_REENDISCOVERABLE  = 22,  /*（0x16）控制断开后是否自动进入可发现状态*/  
	CMD_LEADVPARAMS    = 23,  /*（0x17）设置LE广播间隔时间*///(GATT协议专用)	
	CMD_RSSI	  = 24,  /*（0x18）获取连接信号强度*/  
	CMD_LECONPARAMS    = 25,  /*（0x19）设置LE连接间隔时间*///(GATT协议专用)	
	CMD_UARTBAUD	 = 26,	/*（0x1a）设置UART波特率*/	
	CMD_RENAME	   = 27,  /*（0x1b）修改设备名*/  
	CMD_MODBTADDR	 = 28,	/*（0x1c）修改模组蓝牙地址*/ 
	CMD_MODIFY_SERVICE_NAME = 29,
	CMD_UART_RXFIFO_LEVEL  = 31,   /*（0x1f）串口接收fifo指示*/ 
	CMD_BT_RXFIFO_LEVEL   = 32,  /*（0x20）蓝牙接收fifo指示*/ 
	CMD_POLL_TIME	  = 33,  /*（0x21）报告发送间隔时间(ms)*/ 
	CMD_MOD_IF_CHECKSUM  = 38,	/*（0x26）控制是否使用checksum*/ 
	CMD_IAP2_START		=42,
	CMD_IAP2_FILE		=44,
	CMD_IAP2_FINISH		=46,
	CMD_SET_GPIO	 = 52,	/*设置gpio电平*/ 
	CMD_ADV_DUAL	 = 54,	/*设置是否让android识别为双模模组*/ 
	CMD_LED_STATE	 = 55,	/*设置使能LED状态*/ 
	CMD_TESTMODE	= 62,  /*进入测试模式*/
	CMD_OTAMODE    = 65,  /*进入OTA模式*/
	CMD_LE_SET_SCAN 	= 66,	  /*LE使能扫描周围设备*/
	CMD_LE_CONNECT	   = 67,  /*LE连接特定地址设备*/   
	CMD_LE_EXIT_CONNECT    = 68,  /*LE退出连接特定地址设备*/   
	CMD_LE_DISCONNECT	 = 69,	/*LE断开特定设备连接*/ 
	CMD_LE_FIND_SERVICES	= 70,  /*LE查找服务*/	
	CMD_LE_FIND_CHARA	 = 71,	/*LE查找属性*/	 
	CMD_LE_FIND_DESC	 = 72,	/*LE查找描述*/	 
	CMD_LE_SET_NOTIFY	 = 73,	/*LE设置使能通知功能*/	 
	CMD_LE_XFER 	 = 74,	/*LE收发数据,接收通知数据*/   
	CMD_LE_READ 	 = 75,	/*LE读属性*/ 
	CMD_LE_REMOTEN	   = 76,	/*LE查询当前连接的设备*/ 
	CMD_LE_XFER2H     = 77,
	CMD_READ_NAME	  = 0x801B, /*读设备名*/  
	CMD_UNKNOW	   = 0xffff,  /*未知命令*/ 
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

//添加数据状态
#define BLE_ADD_DATA_START 0x3
#define BLE_ADD_DATA_ING 0x2
#define BLE_ADD_DATA_SUCESS 0x1
#define BLE_ADD_DATA_FAIL 0x0

//添加数据类型
#define BLE_ADD_DATA_TYPE_FP 0x1
#define BLE_ADD_DATA_TYPE_NFC 0x2


extern void bleGetRuoChanPro(void);
extern void bleIdCard(uint8_t uStatus,u8 uCmd);
extern uint8_t openCheckPro(uint8_t *uData);
extern void defaultData(void);
extern void InitializationBTMac(void);

/*
操作类型：0.开门 1.增加用户 2.修改用户 3.删除用户 4.增加蓝牙密码 5.修改蓝牙密码 6.删除蓝牙密码 
			  7.增加数字密码 8.修改数字密码 9.删除数字密码 10.增加nfc 11.修改nfc 12.删除nfc 13.增加身份证 14.修改身份证 
			  15.删除身份证 16.增加指纹 17.修改指纹 18.删除指纹 19.添加协迫指纹

*/
#define BT_OPEN			0  //蓝牙开门密码
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
