#include "mains.h"


uint8_t Flash_Buffer[1024] = {0xff}; 
u8 Flag_Inf[ADD_MAX]={0};
User_Info UserInfo[100];
Open_Info OpenInfo[125];
u8 getData[ADD_MAX]={0};

UserType bleUserInfo[USER_INFO_NUM_MAX];
SecretType bleSecretInfo[SECRET_INFO_NUM_MAX];
u8 Admin_Flag[BT_Admin_Max]={0};
u8 BluetoothMac[BULETOOTH_MAC_MAX]={0};







void Flash_Init(void)
{
	// 调整flash与时钟速率之间的关系
	FLASH->ACR |= FLASH_ACR_LATENCY;
}

void Flash_Unlock(void)
{
	// FLASH->CR 的第7位为解锁的标志位或者上锁的操作位
	while(FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = FLASH_FKEY1;
		FLASH->KEYR = FLASH_FKEY2;
	}
}

void Flash_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void Flash_Clear_All_Flag(void)
{
	unsigned long flag_temp;
	flag_temp = FLASH->SR;
	FLASH->SR = flag_temp & 0x34;
}
// 因为flash是从0x08000000开始的，总共64k，每1k就是1页
// which_page_temp -- 你所清空页包含的地址（该地址在哪一页的范围就清空哪一页）
void Flash_Earse_Page(unsigned long which_page_temp)
{
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR  = which_page_temp;
    FLASH->CR |= FLASH_CR_STRT; 
    while(FLASH->SR & FLASH_SR_BSY); // 等待BSY清零 
    FLASH->CR &= ~FLASH_CR_PER;
}

void Clear_Flash()
{
	uint8_t i = 0;
	
	Usart1_Disable();
	Usart2_Disable();
	__disable_irq();
		
	for(i = 0;i <= BT_System_Page; i++)
	{
		Flash_Clear_All_Flag();
		Flash_Unlock();
		Flash_Earse_Page(Base_Addr+1024*i);
		Flash_Lock();
	}

	__enable_irq();
	Usart1_init();
	Usart2_init();
	
}
// 因为flash是从0x08000000开始的，总共64k，每1k就是1页
// 我们直接将最后一页当做一个储存空间，即储存的首地址为 (0x08000000 + 1024 * 63)
// 所以 write_dat_temp 的范围 0 -- （1024 / 4 - 1）
// Flash数据长度必须是半字节，其它长度会引起中断
// 为了配合读取的时候是整个一个字，写入的时候也写入一个字
void Flash_Write_Dat(unsigned long write_num_temp,unsigned long write_dat_temp)
{
	Flash_Clear_All_Flag();
	Flash_Unlock();
	//Flash_Earse_Page(0x08000000 + 1024 * 63);
	FLASH->CR |= FLASH_CR_PG;
	*(__IO uint16_t*)(0x08000000 + 1024 * 63 + write_num_temp * 4 + 0) = (write_dat_temp & 0xffff);
	*(__IO uint16_t*)(0x08000000 + 1024 * 63 + write_num_temp * 4 + 2) = ((write_dat_temp & 0xffff0000) >> 16);
	while(FLASH->SR & FLASH_SR_BSY); // 等待BSY清零
	FLASH->CR &= ~FLASH_CR_PG;
	Flash_Lock();
}
// 因为flash是从0x08000000开始的，总共64k，每1k就是1页
// 我们直接将最后一页当做一个储存空间，即储存的首地址为 (0x08000000 + 1024 * 63)
// 所以 read_num_temp 的范围 0 -- （1024 / 4 - 1）
// 读取一个地址的时候，读取的是一个字，4个字节,所以一次需要跳过四个字节
unsigned long Flash_Read(unsigned long read_num_temp)
{
	unsigned long read_dat_temp;
	read_dat_temp = *(__IO uint32_t *)(0x08000000 + 1024 * 63 + read_num_temp * 4);
	while(FLASH->SR & FLASH_SR_BSY); // 等待BSY清零
	return read_dat_temp;
}

void Flash_Write_Page(uint8_t Data_Page, uint8_t *white_data)
{
	uint16_t loop;
	uint16_t buffer_H, buffer_L;
	Flash_Clear_All_Flag();
	Flash_Unlock();
	Flash_Earse_Page(Base_Addr + 1024 * Data_Page);
	FLASH->CR |= FLASH_CR_PG;
	for (loop = 0; loop < 256; loop++)
	{
		memcpy(&buffer_L, white_data+loop*4, 2);
		memcpy(&buffer_H, white_data+loop*4+2, 2);
		*(__IO uint16_t*)(Base_Addr + 1024 * Data_Page + loop * 4 + 0) = buffer_L;
		*(__IO uint16_t*)(Base_Addr + 1024 * Data_Page + loop * 4 + 2) = buffer_H;
		while(FLASH->SR & FLASH_SR_BSY); // 等待BSY清零
	}
	FLASH->CR &= ~FLASH_CR_PG;
	Flash_Lock();
}

void Flash_Read_Page(uint8_t Data_Page, uint8_t *read_data)
{
	uint32_t read_dat_temp;
	uint16_t loop;
	for (loop = 0; loop < 256; loop++)
	{
		read_dat_temp = *(__IO uint32_t *)(Base_Addr + 1024 * Data_Page + loop * 4);
		while(FLASH->SR & FLASH_SR_BSY); // 等待BSY清零
		memcpy(read_data+loop*4, &read_dat_temp, 4);
	}
}

/*
----------------------------------------------------------
----------------------------------------------------------
*/
#if 0
void Check_Flash(void)
{
	uint32_t Rx0_crc;
	uint32_t Rx0_sum;
	uint32_t Rx0_MIRROR_crc;
	uint32_t Rx0_MIRROR_sum;
	
	uint32_t Rx1_crc;
	uint32_t Rx1_sum;
	uint32_t Rx1_MIRROR_crc;
	uint32_t Rx1_MIRROR_sum;

	uint32_t Rx2_crc;
	uint32_t Rx2_sum;
	uint32_t Rx2_MIRROR_crc;
	uint32_t Rx2_MIRROR_sum;

	uint32_t Rx3_crc;
	uint32_t Rx3_sum;
	uint32_t Rx3_MIRROR_crc;
	uint32_t Rx3_MIRROR_sum;

	uint32_t Rx4_crc;
	uint32_t Rx4_sum;
	uint32_t Rx4_MIRROR_crc;
	uint32_t Rx4_MIRROR_sum;

	uint32_t Rx5_crc;
	uint32_t Rx5_sum;
	uint32_t Rx5_MIRROR_crc;
	uint32_t Rx5_MIRROR_sum;

	uint32_t Rx6_crc;
	uint32_t Rx6_sum;
	uint32_t Rx6_MIRROR_crc;
	uint32_t Rx6_MIRROR_sum;

	uint16_t loop;

/************************************************

				用户数据0区

*************************************************/
	Flash_Read_Page(USER_Data_0, Flash_Buffer);
	loop = 800;
	Rx0_crc = 0;
	while(loop--)
	{
		Rx0_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx0_sum, Flash_Buffer+1020, 4); 

	Flash_Read_Page(USER_Data_MIRROR_0, Flash_Buffer);
	loop = 800;
	Rx0_MIRROR_crc = 0;
	while(loop--)
	{
		Rx0_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx0_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx0_crc, Rx0_sum, Rx0_MIRROR_crc, Rx0_MIRROR_sum, USER_Data_0, USER_Data_MIRROR_0);

/************************************************

				用户数据1区

*************************************************/
	Flash_Read_Page(USER_Data_1, Flash_Buffer);
	loop = 800;
	Rx1_crc=0;
	while(loop--)
	{
		Rx1_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx1_sum, Flash_Buffer+1020, 4);	

	Flash_Read_Page(USER_Data_MIRROR_1, Flash_Buffer);
	loop = 800;
	Rx1_MIRROR_crc=0;
	while(loop--)
	{
		Rx1_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx1_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx1_crc, Rx1_sum, Rx1_MIRROR_crc, Rx1_MIRROR_sum, USER_Data_1, USER_Data_MIRROR_1);

/************************************************

				用户数据2区

*************************************************/
	Flash_Read_Page(USER_Data_2, Flash_Buffer);
	loop = 800;
	Rx2_crc=0;
	while(loop--)
	{
		Rx2_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx2_sum, Flash_Buffer+1020, 4);

	Flash_Read_Page(USER_Data_MIRROR_2, Flash_Buffer);
	loop = 800;
	Rx2_MIRROR_crc=0;
	while(loop--)
	{
		Rx2_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx2_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx2_crc, Rx2_sum, Rx2_MIRROR_crc, Rx2_MIRROR_sum, USER_Data_2, USER_Data_MIRROR_2);


/************************************************

				系统数据1区

*************************************************/

	Flash_Read_Page(System_Data_0, Flash_Buffer);
	loop = 40;
	Rx3_crc=0;
	while(loop--)
	{
		Rx3_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx3_sum, Flash_Buffer+1020, 4);


	Flash_Read_Page(System_Data_MIRROR_0, Flash_Buffer);
	loop = 40;
	Rx3_MIRROR_crc=0;
	while(loop--)
	{
		Rx3_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx3_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx3_crc, Rx3_sum, Rx3_MIRROR_crc, Rx3_MIRROR_sum, System_Data_0, System_Data_MIRROR_0);



/************************************************

				开门记录0区

*************************************************/

	Flash_Read_Page(Open_Info_Data_0, Flash_Buffer);
	loop = 1000;
	Rx4_crc=0;
	while(loop--)
	{
		Rx4_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx4_sum, Flash_Buffer+1020, 4);

	Flash_Read_Page(Open_Info_Data_MIRROR_0, Flash_Buffer);
	loop = 1000;
	Rx4_MIRROR_crc=0;
	while(loop--)
	{
		Rx4_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx4_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx4_crc, Rx4_sum, Rx4_MIRROR_crc, Rx4_MIRROR_sum, Open_Info_Data_0, Open_Info_Data_MIRROR_0);



/************************************************

				开门记录1区

*************************************************/

	Flash_Read_Page(Open_Info_Data_1, Flash_Buffer);
	loop = 1000;
	Rx5_crc=0;
	while(loop--)
	{
		Rx5_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx5_sum, Flash_Buffer+1020, 4);

	Flash_Read_Page(Open_Info_Data_MIRROR_1, Flash_Buffer);
	loop = 1000;
	Rx5_MIRROR_crc=0;
	while(loop--)
	{
		Rx5_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx5_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx5_crc, Rx5_sum, Rx5_MIRROR_crc, Rx5_MIRROR_sum, Open_Info_Data_1, Open_Info_Data_MIRROR_1);


/************************************************

				开门记录2区

*************************************************/

	Flash_Read_Page(Open_Info_Data_2, Flash_Buffer);
	loop = 1000;
	Rx6_crc=0;
	while(loop--)
	{
		Rx6_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx6_sum, Flash_Buffer+1020, 4);


	Flash_Read_Page(Open_Info_Data_MIRROR_2, Flash_Buffer);
	loop = 1000;
	Rx6_MIRROR_crc=0;
	while(loop--)
	{
		Rx6_MIRROR_crc+=Flash_Buffer[loop];
	}
	memcpy(&Rx6_MIRROR_sum, Flash_Buffer+1020, 4);
	
	Check_Data_CRC(Rx6_crc, Rx6_sum, Rx6_MIRROR_crc, Rx6_MIRROR_sum, Open_Info_Data_2, Open_Info_Data_MIRROR_2);



}
#endif

void Read_flash_Data(uint8_t Flash_Block ,uint8_t Data_Page)
{	
	Usart1_Disable();
	Usart2_Disable();
	__disable_irq();
	
	Flash_Read_Page(Data_Page, Flash_Buffer);
	#if 0
	if(Flash_Block == User_Data)
	{
		memcpy(UserInfo,Flash_Buffer,800);
	}
	else if(Flash_Block == System_Data)
	{
		memcpy(Flag_Inf,Flash_Buffer,40);
	}
	else if(Flash_Block == Open_Info_Data)
	{
		memcpy(OpenInfo,Flash_Buffer,1000);
	}
	#endif
	if(Flash_Block == BT_User_Data)
	{
		memcpy(bleUserInfo,Flash_Buffer,sizeof(bleUserInfo));
	}
	else if(Flash_Block == BT_Secret_Data)
	{
		memcpy(bleSecretInfo,Flash_Buffer,sizeof(bleSecretInfo));
	}
	else if(Flash_Block == BT_System_Data)
	{
		memcpy(Admin_Flag,Flash_Buffer+SYS_DATA_ADDR,sizeof(Admin_Flag));
		memcpy(BluetoothMac,Flash_Buffer+SYS_DATA_BTMAC_ADDR,sizeof(BluetoothMac));
	}

	__enable_irq();
	Usart1_init();
	Usart2_init();
}

void SaveData_Inf(uint8_t Flash_data,uint8_t Page)
{
	Usart1_Disable();
	Usart2_Disable();
	__disable_irq();

	#if 0
	if(Flash_data == User_Data)
	{
		loop = 800;
		memcpy(Flash_Buffer, UserInfo, 800);
		Rx_crc=0;
		while(loop--)
		{
			Rx_crc+=Flash_Buffer[loop];
		}
		memcpy(Flash_Buffer+1020, &Rx_crc, 4);
	}
	else if(Flash_data == System_Data)
	{
		loop = 40;
		memcpy(Flash_Buffer, Flag_Inf, 40);
		Rx_crc=0;
		while(loop--)
		{
			Rx_crc+=Flash_Buffer[loop];
		}
		memcpy(Flash_Buffer+1020, &Rx_crc, 4);
	}
	else if(Flash_data == Open_Info_Data)
	{
		loop = 1000;
		memcpy(Flash_Buffer, OpenInfo, 1000);
		Rx_crc=0;
		while(loop--)
		{
			Rx_crc+=Flash_Buffer[loop];
		}
		memcpy(Flash_Buffer+1020, &Rx_crc, 4);
	}

	Flash_Write_Page(Page, Flash_Buffer);
	Flash_Write_Page(Page-7, Flash_Buffer);
	#endif
	if(Flash_data == BT_User_Data)
	{
		memcpy(Flash_Buffer, bleUserInfo, sizeof(bleUserInfo));
	}
	else if (Flash_data == BT_Secret_Data)
	{
		memcpy(Flash_Buffer, bleSecretInfo, sizeof(bleSecretInfo));
	}
	else if (Flash_data == BT_System_Data)
	{
		memcpy(Flash_Buffer+SYS_DATA_ADDR, Admin_Flag, sizeof(Admin_Flag));
		memcpy(Flash_Buffer+SYS_DATA_BTMAC_ADDR, BluetoothMac, sizeof(BluetoothMac));
	}
	
	Flash_Write_Page(Page, Flash_Buffer);
	__enable_irq();
	Usart1_init();
	Usart2_init();
}
#if 0
void Check_Data_CRC(uint32_t D_CRC, uint32_t D_CRC_SUM, uint32_t D_CRC_MIRROR, uint32_t D_CRC_MIRROR_SUM, uint8_t DATA, uint8_t DATA_MIRROR)
{
	if(D_CRC != D_CRC_SUM)
	{
		if(D_CRC_MIRROR == D_CRC_MIRROR_SUM)
		{
			Flash_Read_Page(DATA_MIRROR, Flash_Buffer);
			Flash_Write_Page(DATA, Flash_Buffer);
		}
	}
	if(D_CRC_MIRROR != D_CRC_MIRROR_SUM)
	{
		if(D_CRC == D_CRC_SUM)
		{
			Flash_Read_Page(DATA, Flash_Buffer);
			Flash_Write_Page(DATA_MIRROR, Flash_Buffer);
		}
	}
}
#endif

