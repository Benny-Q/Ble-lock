#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__


#include "mains.h"
#include "STM_GPIO_List.h"

#define ACK_SUCCESS        0x00 //执行成功
#define ACK_FAIL           0x01 //执行失败
#define ACK_FULL           0x04 //数据库满
#define ACK_NOUSER         0x05 //没有这个用户
#define ACK_USER_EXIST     0x07 //用户已存在
#define ACK_TIMEOUT        0x08 //图像采集超时
#define ACK_BREAK          0x18 //终止当前指令
#define ACK_IMAGEFAIL      0x84 //图像不合格




#define MANGER_ROLL        1 //管理员
#define COMMON_ROLL        2 //普通用户
#define CALLER_ROLL        3 //访客

#define   USED    1  //有数据未处理时置1
#define   NOUSED  0

extern uint8_t ICcard_int_flag;//IC卡唤醒标识位


typedef enum{
	CMD_TEST=0,            //检查 Target 与 Host 之间是否通讯正常。
	CMD_ENROLL,            //指纹录入
	CMD_IDENTIFY,          //1:N 比对：按下手指和所有已注册的 Template 比对。	
	CMD_CLEAR_TEMPLATE,    //删除指定编号中已注册的 Template。
	CMD_CLEAR_ALLTEMPLATE, //删除已注册的所有 Template。
	CMD_BREAK			//打断命令
}CMD_ENUM;

typedef enum{
	Result_Succes,
	Result_Failed,
	Result_Timeout,
	Result_Exist,
	Result_Full,
	Result_Again,
	Result_Pass,
	Result_Finger_Exist
}Result_Enum;


//定义指纹头结构体消息
typedef struct tagFINGER
{
	uint8_t  usefalg;       //有数据未处理时置1，否则清零
	uint8_t  data[17];                               
}FINGER;

extern FINGER g_finger[6];    //定义指纹接收二级缓存，给处理用
extern FINGER Rev_finger;     //定义指纹接收一级缓存
extern uint8_t figcntRxd;        //接收字节计数
//extern u8 figcntRxdbak;     //接收字节计数备份

extern uint8_t FingPrint_data[17]; //发送给指纹模块的数组

uint8_t FingerPrint_api(CMD_ENUM cmd,uint16_t data,uint8_t roll);
void FPRT_GPIO(void);
void FPRT_EXTI_GPIO(void);
void Fingerprint_init(void);
void Fingerprint_Enable(void);
void Fingerprint_Disable(void);
int Fingerprint_Enroll(uint8_t permissions, uint16_t User_Number);
int Fingerprint_Compare(void);

void FingerPrint_Delete(uint16_t User_Number);
void FingerPrint_Support(void);
void FPRT_Touch_GPIO(void);
void FPRT_EXTI_GPIO(void);
extern void fingerPro(void);


//EventFlg            equ     37h;注意40h以后没有位寻址
#define  SYS_MOTO_P			0x01         //;BIT = 1,CHANGE
#define  SYS_MOTO_WAIT		0x02         //;BIT = 1,CHANGE FULL
#define  SYS_MOTO_N			0x04       // ;BIT = 1,LOW POWER
#define  SYS_POWER_OFF		0x08       // ;BIT = 1,LOW POWER
#define  SYS_FINISH_WAIT	0x10        // ;BIT = 1,LOW POWER
#define  SYS_TIME_FINISH       0x20

#define LOCK_TIME_150MS 5// 3
#define LOCK_TIME_200MS 6// 4
#define LOCK_TIME_300MS 7// 6
extern void bluetooth_init(void);
#endif 
