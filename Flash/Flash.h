#ifndef __FLASH_H
#define __FLASH_H

#include "mains.h"

#define Base_Addr 0x0800C800  // 50页


typedef enum
{
	USER_Data_MIRROR_0,
	USER_Data_MIRROR_1,
	USER_Data_MIRROR_2,
	System_Data_MIRROR_0,
	Open_Info_Data_MIRROR_0,
	Open_Info_Data_MIRROR_1,
	Open_Info_Data_MIRROR_2,

	USER_Data_0,
	USER_Data_1,
	USER_Data_2,
	System_Data_0,
	Open_Info_Data_0,
	Open_Info_Data_1,
	Open_Info_Data_2

}Flash_Memory_Save;

typedef enum
{
	System_Data,
	User_Data,
	Open_Info_Data
}Flash_Data;


typedef enum
{
	BT_User_Page,
	BT_Secret_Page,
	BT_System_Page
}BT_Flash_Page;

typedef enum
{
	BT_User_Data,
	BT_Secret_Data,
	BT_System_Data
}BT_Flash_Data;

typedef struct
{
	uint8_t UserCNT;
	uint16_t AdminCNT;
}Inf_CNT;
#define ADD_DEFAULT_STR	0x5a
typedef enum
{
	ADD_DEFAULT,
	Audio_mode ,
	General_Open_mode,
	Combination_mode,
	Time_Set_status,
	FPRT,
	ADMIN,
	Admin_Pswd,
	Admin_RFID,
	Admin_FPRT,
	Admin_BT,
	USER,
	User_Pswd,
	User_RFID,
	User_FPRT,
	User_BT,
	Open_Info_CNT,
	ADD_MAC0,
	ADD_MAC1,
	ADD_MAC2,
	ADD_MAC3,
	ADD_MAC4,
	ADD_MAC5,
	ADD_MAC6,
	ADD_MAC7,
	ADD_MAX
}Flag_Info;

//蓝牙用户使用的Flag
typedef enum
{
	BT_Admin,
	BT_Admin_Pswd,
	BT_Admin_RFID,
	BT_Admin_FPRT,
	BT_User,
	BT_Mac,
	BT_Flag_Max
}BT_Flag_Info;


//数据存储flash每页中的起始地址
#define SYS_DATA_ADDR 0
#define SYS_DATA_BTMAC_ADDR BT_Flag_Max


/************************************************************
*																	
*		Userflag说明
*		第一位表示存储何种信息
*		第二位表示用户权限
*		0x11为管理员密码0x12为用户密码
*		0x21为管理员指纹0x22为用户指纹
*		0x31为管理员磁卡0x32为用户磁卡
*																	
*************************************************************/
#define ADMIN_PSW_STR		0x11
#define ADMIN_RFID_STR		0x21
#define ADMIN_FINGER_STR	0x31
#define ADMIN_BT_STR		0x41

typedef struct
{
	uint8_t Userflag;
	uint8_t User_Info_CNT;
	uint8_t Info[6];
}User_Info;


typedef struct
{
	uint32_t Open_flag;
	uint32_t Open_info;
}Open_Info;


//extern User_Info UserInfo[100];
extern u8 Flag_Inf[ADD_MAX];
extern Flag_Info FlagInfo;
//extern Open_Info OpenInfo[125];

extern u8 Admin_Flag[BT_Flag_Max];



void Flash_Write_Dat(unsigned long write_num_temp,unsigned long write_dat_temp);
unsigned long Flash_Read(unsigned long read_num_temp);
//extern void Check_Flash(void);
extern void SaveData_Inf(uint8_t Flash_data,uint8_t Page);
extern void Clear_Flash(void);
extern void Read_flash_Data(uint8_t Flash_Block ,uint8_t Data_Page);
void Check_Data_CRC(uint32_t D_CRC, uint32_t D_CRC_SUM, uint32_t D_CRC_MIRROR, uint32_t D_CRC_MIRROR_SUM, uint8_t DATA, uint8_t DATA_MIRROR);

#endif
