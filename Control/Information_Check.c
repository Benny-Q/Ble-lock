#include "Information_Check.h"

extern u8 touchBuff[2];
extern u8 touchTimer;

void clean_input_key_value(void)
{
	touchTimer = 0;
	touchBuff[0] = 0;
	touchBuff[1] = 0;
	
}

void defaultDataPro(void)
{
	u8 i=0;
	
	for (i=0; i<BT_Flag_Max; i++)
	{
		Admin_Flag[i] =0;
	}
	
}

void InitFlashData(void)
{
	u8 i=0;
	Read_flash_Data(BT_User_Data,BT_User_Page);
	Read_flash_Data(BT_Secret_Data,BT_Secret_Page);
	Read_flash_Data(BT_System_Data,BT_System_Page);

	printf("Admin_Flag[BT_Admin]=%x\r\n",Admin_Flag[BT_Admin]);

	//³õÊ¼FlagÊý¾Ý
	if (Admin_Flag[BT_Admin] == 0xFF)
	{
		for (i=0; i<BT_Flag_Max; i++)
			Admin_Flag[i] =0;
	}

}


