#ifndef __KEY_H_
#define __KEY_H_


#include "mains.h"
void Hall_GPIO_Init(void);

extern void touchPro(void);

#define TOUCH_DOWN	1
u8 check_System_Lock(void);
extern void keySetupPro(void);
extern void sysPro(void);
extern void save(u8 pswdRfidTouch,u8 *dData,u8 uLength);
extern void touchPro(void);
extern u8 touchCheckPro(void);
extern u8 sysFlg;
extern u8 touchFlg;
extern u8 sysAddDeviceState;


#define SYS_SETUP		0x01
#define SYS_SETUP_EFFI	0x02
#define SYS_LOCK	0x04
#ifdef __BLUETOOTH_SUPPORT__
//#define SYS_BLE_START_PWD_SET 0x08
#define SYS_BLE_PWD_SETUP 0x08
#define SYS_BLE_START_NFC_SET 0x10
#define SYS_BLE_NFC_SETUP 0x20
#define SYS_BLE_START_FP_SET 0x40
#define SYS_BLE_FP_SETUP 0x80
#endif

#define SYS_TIMEOUT_MAX	15;//15S

#ifdef __BLUETOOTH_SUPPORT__
#define communicateUartDataMax 70
#define communicateUartDataMax1 60
typedef struct _CommunicateType{
    volatile BYTE ucGetFinishFlg;
    volatile BYTE CommunicateUartData[communicateUartDataMax];
    u8 CommunicateValidUartData[communicateUartDataMax1];
		u8 SendUartData[communicateUartDataMax1];
    volatile u8 uDataLen;
    //u8 uDataLenBk;
    u8 uDataValidLen;
    u8 mac[9];
    u8 packId[3];
}CommunicateType;

//用户表
typedef struct _UserType{
    u8 phone[11];  //注册用户手机号码
    u8 secret[6];  //密码
    u8 startTime[5];  //其实时间
    u8 endTime[5]; //结束时间
    u8 auth[3];  //权限 
}UserType;

//密码表
typedef struct _SecretType{
    u8 phone[11];
    u8 secret[10];
		u8 type;
    u8 startTime[5];
    u8 endTime[5];
	u8 timers;
}SecretType;


#define BT_ADD_DEFAULT     			0x00
#define ADD_FACTORY_ID0				0x01
#define ADD_FACTORY_ID1				0x02
#define ADD_FACTORY_ID2				0x03
#define ADD_FACTORY_ID3				0x04
#define ADD_DEVICE_TYPE0			0x05
#define ADD_DEVICE_TYPE1			0x06
#define ADD_SELF_MAC0     			0x07
#define ADD_SELF_MAC1     			0x08
#define ADD_SELF_MAC2     			0x09
#define ADD_SELF_MAC3     			0x0A
#define ADD_SELF_MAC4     			0x0B
#define ADD_SELF_MAC5     			0x0C
#define ADD_SELF_MAC6     			0x0D
#define ADD_SELF_MAC7     			0x0E
#define ADD_SELF_MAC8     			0x0F
#define ADD_SELF_MAC9     			0x10
#define ADD_SELF_MAC10     			0x11
#define ADD_SELF_MAC11     			0x12
#define ADD_SELF_MAC12     			0x13
#define ADD_SELF_MAC13     			0x14
//#define ADD_SELF_MAC14     			0x15
#define ADD_ADMIN_PHONE0   			0x16
#define ADD_ADMIN_PHONE1   			0x17
#define ADD_ADMIN_PHONE2   			0x18
#define ADD_ADMIN_PHONE3   			0x19
#define ADD_ADMIN_PHONE4   			0x1A
#define ADD_ADMIN_PHONE5   			0x1B
#define ADD_ADMIN_PHONE6   			0x1C
#define ADD_ADMIN_PHONE7   			0x1D
#define ADD_ADMIN_PHONE8   			0x1E
#define ADD_ADMIN_PHONE9   			0x1F
#define ADD_ADMIN_PHONE10   		0x20
#define ADD_SECRET0     			0x21
#define ADD_SECRET1     			0x22
#define ADD_SECRET2     			0x23
#define ADD_SECRET3     			0x24
#define ADD_SECRET4     			0x25
#define ADD_SECRET5     			0x26
#define ADD_IP0   					0x27
#define ADD_IP1   					0x28
#define ADD_IP2   					0x29
#define ADD_IP3   					0x2A
#define ADD_IP_PORT0				0x2B
#define ADD_IP_PORT1				0x2C

extern u8 saveAddUser(u8 *uData,u8 uLen);
extern u8 saveModifyOrDelUser(u8 *uData,u8 wSize,u8 modifyOrDel);
extern u8 secretUser(u8 *uData,u8 wSize);
extern u8 saveAddSecret(u8 *uData,u8 operType);
extern void openDoor(u8 *phone,u8 uOpenFun);
extern u8 saveModifyOrDelFindUser(u8 *uData,u8 uLength);
extern u8 saveModifyOrDelFindSecret(u8 *uData,u8 uLength);
extern u8 memcmpStr(u8 *uData1,u8 *uData2,u8 uLength);
extern void memsetStr(u8 *uData1,u8 uData2,u8 uLength);
extern u8 findSecret(u8 *uSecret,u8 uLength);
extern void memcpyStr(u8 *uData1,u8 *uData2,u8 uLength);
extern void btSaveFlashData(u8 data_type, SecretType data);
extern void btSaveUserFlashData(UserType data);
extern u8 saveAdminSecret(u8 *uData,u8 wSize);
extern u8 btDelSecretUserInfo(u8 *uData,u8 operType);
#endif /*#ifdef __BLUETOOTH_SUPPORT__*/

#endif

