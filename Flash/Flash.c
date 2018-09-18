#include "mains.h"


uint8_t Flash_Buffer[1024] = {0xff}; 
u8 Flag_Inf[ADD_MAX]={0};
//User_Info UserInfo[100];
//Open_Info OpenInfo[125];
u8 getData[ADD_MAX]={0};

UserType bleUserInfo[USER_INFO_NUM_MAX];
SecretType bleSecretInfo[SECRET_INFO_NUM_MAX];
u8 Admin_Flag[BT_Flag_Max]={0};
u8 BluetoothMac[BULETOOTH_MAC_MAX]={0};



void Flash_Init(void)
{
	// ����flash��ʱ������֮��Ĺ�ϵ
	FLASH->ACR |= FLASH_ACR_LATENCY;
}

void Flash_Unlock(void)
{
	// FLASH->CR �ĵ�7λΪ�����ı�־λ���������Ĳ���λ
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
// ��Ϊflash�Ǵ�0x08000000��ʼ�ģ��ܹ�64k��ÿ1k����1ҳ
// which_page_temp -- �������ҳ�����ĵ�ַ���õ�ַ����һҳ�ķ�Χ�������һҳ��
void Flash_Earse_Page(unsigned long which_page_temp)
{
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR  = which_page_temp;
    FLASH->CR |= FLASH_CR_STRT; 
    while(FLASH->SR & FLASH_SR_BSY); // �ȴ�BSY���� 
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
// ��Ϊflash�Ǵ�0x08000000��ʼ�ģ��ܹ�64k��ÿ1k����1ҳ
// ����ֱ�ӽ����һҳ����һ������ռ䣬��������׵�ַΪ (0x08000000 + 1024 * 63)
// ���� write_dat_temp �ķ�Χ 0 -- ��1024 / 4 - 1��
// Flash���ݳ��ȱ����ǰ��ֽڣ��������Ȼ������ж�
// Ϊ����϶�ȡ��ʱ��������һ���֣�д���ʱ��Ҳд��һ����
void Flash_Write_Dat(unsigned long write_num_temp,unsigned long write_dat_temp)
{
	Flash_Clear_All_Flag();
	Flash_Unlock();
	//Flash_Earse_Page(0x08000000 + 1024 * 63);
	FLASH->CR |= FLASH_CR_PG;
	*(__IO uint16_t*)(0x08000000 + 1024 * 63 + write_num_temp * 4 + 0) = (write_dat_temp & 0xffff);
	*(__IO uint16_t*)(0x08000000 + 1024 * 63 + write_num_temp * 4 + 2) = ((write_dat_temp & 0xffff0000) >> 16);
	while(FLASH->SR & FLASH_SR_BSY); // �ȴ�BSY����
	FLASH->CR &= ~FLASH_CR_PG;
	Flash_Lock();
}
// ��Ϊflash�Ǵ�0x08000000��ʼ�ģ��ܹ�64k��ÿ1k����1ҳ
// ����ֱ�ӽ����һҳ����һ������ռ䣬��������׵�ַΪ (0x08000000 + 1024 * 63)
// ���� read_num_temp �ķ�Χ 0 -- ��1024 / 4 - 1��
// ��ȡһ����ַ��ʱ�򣬶�ȡ����һ���֣�4���ֽ�,����һ����Ҫ�����ĸ��ֽ�
unsigned long Flash_Read(unsigned long read_num_temp)
{
	unsigned long read_dat_temp;
	read_dat_temp = *(__IO uint32_t *)(0x08000000 + 1024 * 63 + read_num_temp * 4);
	while(FLASH->SR & FLASH_SR_BSY); // �ȴ�BSY����
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
		while(FLASH->SR & FLASH_SR_BSY); // �ȴ�BSY����
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
		while(FLASH->SR & FLASH_SR_BSY); // �ȴ�BSY����
		memcpy(read_data+loop*4, &read_dat_temp, 4);
	}
}

/*
----------------------------------------------------------
----------------------------------------------------------
*/

void Read_flash_Data(uint8_t Flash_Block ,uint8_t Data_Page)
{	
	Usart1_Disable();
	Usart2_Disable();
	__disable_irq();
	
	Flash_Read_Page(Data_Page, Flash_Buffer);

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

