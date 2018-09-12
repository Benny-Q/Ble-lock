#include "Information_Check.h"
#if 0
uint8_t PASSWORD[256] ;
uint8_t Pswd_input_CNT = 0;
uint8_t __Error_flag = 0;
uint8_t __Correct_flag = 0;
uint8_t	_Combination_Status = 0;

Del_Inf DelInf;
Combination_info Combination_Info = Null_info;
KeyInf_t key_info_first;
void set_PSWD_input_status(uint8_t _status)
{
	PSWD_input_status= _status;

	return;
}

uint8_t get_PSWD_input_status(void)
{
	return PSWD_input_status;
}

uint8_t _get_Erro_info(void)
{
	return __Error_flag;
}

void _set_Error_info(uint8_t i)
{
	__Error_flag = i + __Error_flag;
	return;
}

void _clean_Error_info(void)
{
	__Error_flag = 0;
}

uint8_t _get_Correct_info(void)
{
	return __Error_flag;
}

void _set_Correct_info(uint8_t i)
{
	__Correct_flag = i;
	return;
}

void _clean_Correct_info(void)
{
	__Correct_flag = 0;
}

uint8_t _get_Combination_info(void)
{
	return Combination_Info;
	
}

void _set_Combination_info(Combination_info _Combination_info)
{
	Combination_Info = _Combination_info;
	return;
}

void _clean_Combination_info(void)
{
	Combination_Info = Null_info;
	return;
}

uint8_t _get_Combination_Status(void)
{
	return _Combination_Status;
	
}

void _set_Combination_Status(uint8_t _status)
{
	_Combination_Status = _status + _Combination_Status;
	return;
}

void _clean_Combination_Status(void)
{
	_Combination_Status = 0;
	return;
}


void clean_DelInf(void)
{
	DelInf.flag = 0;
	DelInf.num = 0;
	while(DelInf.CNT)
	{
		DelInf.CNT--;
		DelInf.value[DelInf.CNT] = 0;
		
	}

}


uint8_t scan_one_key_input(uint8_t * key_iteam)
{
	uint8_t scan_loop;
	uint8_t scan_cnt;
	//printf("scan_one_key_input\r\n");
	scan_cnt = _get_APT_Touch_Key_Quantity();
	
	for (scan_loop=0;scan_loop<scan_cnt;scan_loop++)
	{
		if (*(key_iteam+2*scan_loop+1)>0)
		{
			return *(key_iteam+2*scan_loop);
		}
	}
	return 0xff;
}


int verify_PSWD_input_key_serial(uint8_t *PSWD_input_serial)
{
	uint16_t input_verify_offset;
	uint8_t loop;
	uint8_t i ,User_Number;
	int ret_value = 0;
	for(i = 0 ;i < 3 ;i++)
	{
		Read_flash_Data(User_Data, i + USER_Data_0);
		for(User_Number = 0; User_Number < 100; User_Number++)
		{
			if(UserInfo[User_Number].Userflag== 0x11 || UserInfo[User_Number].Userflag== 0x12)
			{
				for (loop = 0; loop < 64 - UserInfo[User_Number].User_Info_CNT; loop++)
				{
					ret_value = 0;
					for(input_verify_offset = 0; input_verify_offset < UserInfo[User_Number].User_Info_CNT; input_verify_offset++)
					{
						#ifdef __DEBUG2__
						printf("UserInfo[0].Info[0]=0x%02x\r\n",UserInfo[User_Number].Info[input_verify_offset/2]);
						printf("PSWD_input_serial = 0x%02x\r\n",*(PSWD_input_serial+input_verify_offset));
						#endif 
						if((input_verify_offset%2) == 0)
						{
							if((UserInfo[User_Number].Info[input_verify_offset/2]&0xf0) != *(PSWD_input_serial+input_verify_offset+loop)<<4)
							{
								ret_value = -1;
								break;
							}
						}
						else if((input_verify_offset%2) == 1)
						{
							if((UserInfo[User_Number].Info[input_verify_offset/2]&0x0f) != *(PSWD_input_serial+input_verify_offset+loop))
							{	
								ret_value = -1;
								break;
							}
						}
						
					}
					if (!ret_value)
					{
						if(!__Config_flag)
						{
							Open_Info_flag_temp[0] = UserInfo[User_Number].Userflag;
							Open_Info_Numb_temp[0] = User_Number+i*100+1;
						}
						return UserInfo[User_Number].Userflag; 
					}
				}
			}
			else
			{
				ret_value =-1;
			}
		}
	}
#if 1

	if(ret_value == -1 && !__Config_flag)
	{
		ret_value = Dynamic_Pswd(PSWD_input_serial);
	}
#endif		
	return ret_value;
	
}

int Set_Pswd(uint8_t permissions, uint16_t User_Number)
{	
	uint8_t Set_Pswd_status = 0 ;
	set_PSWD_input_status(1);
	TIM2_Start();

	while(1)
	{		
		Set_Pswd_status = _get_APT_TouchKey_status();
		if (Set_Pswd_status > 0)
		{
			_clear_APT_Touchkey_status();
			clear_TIM2_Count();
		}
		if(__Correct_flag)
		{
			__Correct_flag = 0;
			return 1;
		}
		else if(__Error_flag == 3)
		{
			__Error_flag = 0;
			return -1;
		}

		Config_Key_Resume_To_Default_Setting();
		
		if (!_get_system_sleep_status() 
			#ifdef __HALL_SUPPORT__
			|| !GPIO_ReadInputDataBit(HALL_PORT, HALL_PIN)
			#endif
			)
		{
			return 0;
		}

	}

}

uint8_t Delet_data(void)
{
	uint16_t Delet_value=0;
	uint16_t scan_Delet_value= 0;
	Delet_value = _get_APT_TouchKey_reg();
	
	key_value_remap(Delet_value);
		
	scan_Delet_value = scan_one_key_input((uint8_t *)key_map_status);	

	if(scan_Delet_value == KEY_STAR)
	{
		if(DelInf.CNT)
		{
			DelInf.CNT--;
			DelInf.value[DelInf.CNT]=0;
		}
		else if(DelInf.CNT == 0)
		{
			return 0;
		}
	}
	else if(scan_Delet_value == KEY_SHARP)
	{
		DelInf.num = DelInf.value[0]*100 +  DelInf.value[1]*10 +  DelInf.value[2];
		if(DelInf.num < 300 && DelInf.num != 0)
		{
			if(UserInfo[DelInf.num-1].Userflag == 0x11 && (Flag_Inf[Admin_Pswd]  == 1))
			{
				TIM3_Start();
				return 1;
			}
			Delet_Information(DelInf.num-1);
			AudioPlay(AUDIO_PROMPT_DELETED);
			return 0;
		}
		else
		{
			AudioPlay(AUDIO_PROMPT_TRY_AGAIN);
			TIM3_Start();
			clean_DelInf();
			DelInf.flag++;
			return 1;
		}
		
	}
	else
	{
		DelInf.value[DelInf.CNT]=scan_Delet_value;
		DelInf.CNT++;
	}
	return 1;
}

void Delet(void)
{
	uint8_t Delet_key_status = 0;
	u8 status = 1;
	DelInf.flag = 0;
	DelInf.CNT = 0;
	DelInf.value[0] = 0;
	DelInf.value[1] = 0;
	DelInf.value[2] = 0;
	_clear_APT_Touchkey_status();
	TIM2_Start();
	TIM3_Start();
	while(1)
	{
		
		Delet_key_status = _get_APT_TouchKey_status();
		if(Delet_key_status>0)
		{
			clear_TIM2_Count();
			_clear_APT_Touchkey_status();
			AudioPlay(AUDIO_PROMPT_BI);
			status = Delet_data();
		}	
		if(DelInf.CNT == 0 || DelInf.CNT < 3)
		{
			if(tim3_count_value%2)
			{
				if(DelInf.CNT == 0)
				{
				}
				else if(DelInf.CNT == 1)
				{
				}
				else if(DelInf.CNT == 2)
				{
				}
			}
			else
			{
			}
		}
	
			

		
		if(!status)
		{
			DelInf.flag = 0;
			clean_DelInf();
			TIM3_Stop();
			break;
		}
		if(DelInf.flag == 3)
		{
			DelInf.flag = 0;
			clean_DelInf();
			TIM3_Stop();
			break;
		}

		Config_Key_Resume_To_Default_Setting();
		
		if (!_get_system_sleep_status() 
			#ifdef __HALL_SUPPORT__
			|| !GPIO_ReadInputDataBit(HALL_PORT, HALL_PIN)
			#endif
			)
		{
			TIM3_Stop();
			return;
		}

	}
}


int Dynamic_Pswd(uint8_t *PSWD_input_serial)
{
	RTC_GetDate(RTC_Format_BIN,&RTC_DataStructure);	
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStructure);
	uint8_t i,j,loop;
	uint8_t offset[3],offset_loop ;
	uint8_t key[12] = {2,5,3,7,8,9,7,6,5,8,9,7};
	uint8_t PSWD[12] = {0};
	uint8_t Temp_PSWD[12] = {0};
	int Dynamic_ret = 0;
	uint64_t Temp[3] = {0};
	
	Temp[0] = 200000000000 + RTC_DataStructure.RTC_Year * 100000000+\
 						     RTC_DataStructure.RTC_Month* 1000000+\
 						     RTC_DataStructure.RTC_Date * 10000+\
 						     RTC_TimeStructure.RTC_Hours* 100+\
 						     RTC_TimeStructure.RTC_Minutes;

	Temp[2] = Temp[0]/5 + 1;
	offset[2] = Temp[2]&0x7; 
	
	Temp[1] = Temp[0]/5 - 1;
	offset[1] = Temp[1]&0x7; 
	
	Temp[0] = Temp[0]/5;
	offset[0] = Temp[0]&0x7; 
	
	Read_flash_Data(User_Data, USER_Data_0);
	for(i = 0 ; i < 9 ;i++)
	{
		if(UserInfo[i].Userflag == 0x11)
		{	
			for(j = 0;j < 6;j++)
			{		
				if(UserInfo[i].Info[j] == 0xff)
				{
					UserInfo[i].Info[j] = 0;
				}
				PSWD[j*2] = (UserInfo[i].Info[j]>>4) & 0x0f;
				PSWD[j*2+1] =  UserInfo[i].Info[j] & 0x0f;
			}
			
			
			for(j = 0;j < 12;j++)
			{
				Temp_PSWD[j] = PSWD[j] ^ key[j];
			}
		
			for(offset_loop = 0; offset_loop<3; offset_loop++)
			{
				j = offset[offset_loop];
				Temp[offset_loop] = ((Temp_PSWD[j] & 0x7)<<24) | ((Temp_PSWD[j+1] & 0xf)<<16) | ((Temp_PSWD[j+2] & 0xf)<< 8) | (Temp_PSWD[j+3] & 0xf);	   
				Temp[offset_loop] = Temp[offset_loop] % 1000000;
				Temp_PSWD[0] = Temp[offset_loop] / 100000;
				Temp_PSWD[1] = (Temp[offset_loop] % 100000)/10000;
				Temp_PSWD[2] = (Temp[offset_loop] % 10000)/1000;
				Temp_PSWD[3] = (Temp[offset_loop] % 1000)/100;
				Temp_PSWD[4] = (Temp[offset_loop] % 100)/10;
				Temp_PSWD[5] = Temp[offset_loop] % 10;

			
				for(loop = 0; loop < 5; loop++)
				{
					Dynamic_ret = 0;
					for(j = 0 ;j < 6;j++)
					{
						if(Temp_PSWD[j] != *(PSWD_input_serial + j + loop))
						{
							Dynamic_ret = -1;
							break;
						}
					}
					if(!Dynamic_ret)
					{
						break;
					}
				}
				if(!Dynamic_ret)
				{
					Dynamic_ret = 0x12;
					break;
				}
			}
		}
		else
		{
			Dynamic_ret = -1;
		}
		if(Dynamic_ret == 0x12)
		{
			break;
		}

	}
	return Dynamic_ret;
}


int save_PSWD_to_Flash(uint8_t PSWD_CNT, uint8_t *PSWD_array, uint8_t permissions, uint16_t User_Number)
{
	uint8_t input_PSWD_LOOP;
	uint8_t input_time;
	uint8_t i, Pswd_Info_Loop;
	uint8_t Exist = 0;
	#ifdef __DEBUG2__
	printf("User_Number = 0x%02x\r\n",User_Number);
	#endif

	if (PSWD_CNT<6)
		return -1;
	
	input_time = get_PSWD_input_status();
	if (input_time > 0)
	{
		//save first time password to temp;	 
		
		for(i = 0; i < 3; i++)
		{
			Read_flash_Data(User_Data, i + USER_Data_0);
			for(Pswd_Info_Loop = 0 ; Pswd_Info_Loop < 100 ; Pswd_Info_Loop++)
			{
				if(UserInfo[Pswd_Info_Loop].Userflag== 0x11 || UserInfo[Pswd_Info_Loop].Userflag== 0x12)
				{
					Exist = 0;
					for(input_PSWD_LOOP = 0; input_PSWD_LOOP < PSWD_CNT ; input_PSWD_LOOP++)
					{
						if((input_PSWD_LOOP%2) == 0)
						{
							if((UserInfo[Pswd_Info_Loop].Info[input_PSWD_LOOP/2]&0xf0) != *(PSWD_array+input_PSWD_LOOP)<<4)
							{
								Exist = 1;
								break;
							}
						}
						else if((input_PSWD_LOOP%2) == 1)
						{
							if((UserInfo[Pswd_Info_Loop].Info[input_PSWD_LOOP/2]&0x0f) != *(PSWD_array+input_PSWD_LOOP))
							{	
								Exist = 1;
								break;
							}
						}
					}
					if(!Exist)
					{
						return 3;
					}
				}
			}
		}
		Read_flash_Data(User_Data, User_Number/100 + USER_Data_0);
		key_info_first.passNum =PSWD_CNT ;

		for(input_PSWD_LOOP = 0; input_PSWD_LOOP < PSWD_CNT; input_PSWD_LOOP++)
		{
			key_info_first.password[input_PSWD_LOOP] = *(PSWD_array+input_PSWD_LOOP);
			
		#ifdef __DEBUG2__
			printf("# @@@ # save_PSWD_to_Flash key_info_first.password[%d]=%d ! \r\n",input_PSWD_LOOP,key_info_first.password[input_PSWD_LOOP]);
		#endif
			
		}
	}
	else
	{
		//check the second time password correct, and save to flash
		if(key_info_first.passNum == PSWD_CNT)
		{
			UserInfo[User_Number%100].User_Info_CNT = PSWD_CNT;
			for(input_PSWD_LOOP = 0; input_PSWD_LOOP<PSWD_CNT; input_PSWD_LOOP++)
			{
				if (key_info_first.password[input_PSWD_LOOP] == *(PSWD_array+input_PSWD_LOOP))
				{
					if(input_PSWD_LOOP % 2 == 0)  //0存高位
					{
						UserInfo[User_Number%100].Info[input_PSWD_LOOP/2] = (*(PSWD_array+input_PSWD_LOOP)<<4) & UserInfo[User_Number%100].Info[input_PSWD_LOOP/2];
					}
					else if((input_PSWD_LOOP % 2 == 1))							//1存低位
					{
						UserInfo[User_Number%100].Info[input_PSWD_LOOP/2] = *(PSWD_array+input_PSWD_LOOP) | UserInfo[User_Number%100].Info[input_PSWD_LOOP/2];
						#ifdef __DEBUG2__
						printf("UserInfo[%d].Info[%d] = 0x%02x\r\n", User_Number%100, input_PSWD_LOOP/2,UserInfo[User_Number%100].Info[input_PSWD_LOOP/2]);
						#endif
					}					
				}
				else
				{
					return -1;
				}
			}
			if(permissions == Root)
			{
				UserInfo[User_Number%100].Userflag = 0x11;	
				Flag_Inf[ADMIN] ++;
				Flag_Inf[Admin_Pswd] ++;
			}
			else if(permissions == User)
			{
				UserInfo[User_Number%100].Userflag = 0x12;
				Flag_Inf[USER] ++;
				Flag_Inf[User_Pswd] ++;
			}
			SaveData_Inf(User_Data, User_Number/100 + USER_Data_0);
			SaveData_Inf(System_Data, System_Data_0);
		}
		else
		{
			return -1;
		}
	}
	#ifdef __DEBUG2__
	printf("UserInfo[%d].Userflag = 0x%02x\r\n", User_Number, UserInfo[User_Number].Userflag);
	#endif
	return 0;
	
}

int Add_Information(uint8_t permissions)
{
	int RFID_Read_ret = 0, FPRT_Read_ret = 0, PSWD_Read_ret = 0;
	uint8_t APT_Touch_status = 0;
	uint16_t APT_Touch_value = 0;
	uint8_t i = 0;
	uint16_t User_Num = 0 , Full_status = 0;
	_clear_APT_Touchkey_status();

	if(permissions == Root && Flag_Inf[ADMIN] == 9)
	{
		AudioPlay(AUDIO_PROMPT_STORAGE_FULL);
		return -1;
	}
	else if(permissions == Root)
	{
		Read_flash_Data(User_Data, USER_Data_0);
		for(User_Num = 0; User_Num < 9; User_Num++)
		{
			if(UserInfo[User_Num].Userflag == 0xff)
			{
				break;
			}
		}
	}
	else if(permissions == User)
	{
		for(i = 0;i < 3;i++)
		{
			Read_flash_Data(User_Data, i + USER_Data_0);
			for(User_Num = 0; User_Num<100; User_Num++)
			{
				if(i == 0 && User_Num >= 9)
				{
					if (UserInfo[User_Num].Userflag == 0xff)
					{
						Full_status = 0;
						break;
					}
					if (User_Num == 100)
					{
						Full_status++;
						break;
					}
				} 
				else if(i == 1 || i == 2)
				{
					if (UserInfo[User_Num].Userflag == 0xff)
					{
						Full_status = 0;
						break;
					}
					if (User_Num == 100)
					{
						Full_status++;
						break;
					}	
				}
			}
			
			if(!Full_status)
			{
				User_Num = 100*i+User_Num;
				break;
			}
			else if(Full_status == 3)
			{
				AudioPlay(AUDIO_PROMPT_STORAGE_FULL);
				return -1;
			}
		}	
	}
	
	while(1)
	{
		APT_Touch_status = _get_APT_TouchKey_status();
		APT_Touch_value = _get_APT_TouchKey_reg();
		if(APT_Touch_value == 0x0010)
		{
			AudioPlay(AUDIO_PROMPT_BI);
			return 0;
		}
		if(APT_Touch_status > 0)
		{
			clear_TIM2_Count();
			RFID_Read_ret = RFID_AddPICC(permissions,User_Num);
			if(RFID_Read_ret == -1)
			{
				PSWD_Read_ret = Set_Pswd(permissions,User_Num);
				if(PSWD_Read_ret == 1)
				{
					return 0;
				}
				else
				{
					_clean_APT_TouchKey_reg();
				}

			}
			else if(RFID_Read_ret == 1)
			{
				AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_RECORD_EXIST, AUDIO_PROMPT_NONE);
			}
			else if(RFID_Read_ret == 2)
			{	
				AudioPlay_All(AUDIO_PROMPT_NFC, AUDIO_PROMPT_RECORD_SAVED, AUDIO_PROMPT_NONE);
				return 0;
			}
		}
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
		{
			soft_delay_ms(50);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))	
			{
				TIM2_Stop();
				FPRT_Read_ret = Fingerprint_Enroll(permissions,User_Num);	
				Fingerprint_Disable();
				if(FPRT_Read_ret == 0)
				{
					TIM2_Start();
					return 0;
				}
				else if(FPRT_Read_ret == 3)
				{
					AudioPlay_All(AUDIO_PROMPT_FGPRT, AUDIO_PROMPT_RECORD_EXIST, AUDIO_PROMPT_NONE);
				}
				else
				{
					AudioPlay_All(AUDIO_PROMPT_FGPRT, AUDIO_PROMPT_RECORD_FAIL,AUDIO_PROMPT_TRY_AGAIN);
				}
				TIM2_Start();
			}
		}
		if (!_get_system_sleep_status() || !GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6))
		{
			return 0;
		}
	}
}

void Delet_Information(uint16_t Delet_Number)
{
	uint16_t i;
	Read_flash_Data(User_Data, Delet_Number/100 + USER_Data_0);
	if(UserInfo[Delet_Number].Userflag == 0x21 | UserInfo[Delet_Number%100].Userflag == 0x22)
	{
		FingerPrint_Delete(Delet_Number);
	}
	if((UserInfo[Delet_Number%100].Userflag & 0x0f) == 0x01)
	{
		Flag_Inf[ADMIN]--;
		if((UserInfo[Delet_Number%100].Userflag & 0xf0) == 0x10)
		{
			Flag_Inf[Admin_Pswd]--;
		}
		else if((UserInfo[Delet_Number%100].Userflag & 0xf0) == 0x20)
		{
			Flag_Inf[Admin_FPRT]--;
		}
		else if((UserInfo[Delet_Number%100].Userflag & 0xf0) == 0x30)
		{
			Flag_Inf[Admin_RFID]--;
		}
	}
	else if((UserInfo[Delet_Number%100].Userflag & 0x0f) == 0x02)
	{
		Flag_Inf[USER]--;
		if((UserInfo[Delet_Number%100].Userflag & 0xf0) == 0x10)
		{
			Flag_Inf[User_Pswd]--;	
		}
		else if((UserInfo[Delet_Number%100].Userflag & 0xf0) == 0x20)
		{
			Flag_Inf[User_FPRT]--;
		}
		else if((UserInfo[Delet_Number%100].Userflag & 0xf0) == 0x30)
		{
			Flag_Inf[User_RFID]--;
		}
	}

	
	UserInfo[Delet_Number%100].Userflag = 0xff;
	UserInfo[Delet_Number%100].User_Info_CNT = 0xff;
	for(i = 0; i<6; i++)
	{
		UserInfo[Delet_Number].Info[i] = 0xff;
	}
	SaveData_Inf(User_Data, Delet_Number/100 + USER_Data_0);
	SaveData_Inf(System_Data, System_Data_0);
	

}

void Save_Open_Door_Info(uint8_t Open_Flag, uint16_t User_Number)
{	
	uint8_t Number[3];
	Number[0] = User_Number/100;
	Number[1] = (User_Number%100)/10;
	Number[2] = User_Number%10;


	/*记录条数*/
	if(Flag_Inf[Open_Info_CNT] <375)
	{
		Read_flash_Data(Open_Info_Data, (Flag_Inf[Open_Info_CNT]/125) + Open_Info_Data_0);
	}
	else if(Flag_Inf[Open_Info_CNT] == 375)
	{
		Flag_Inf[Open_Info_CNT] = 0 ;
		Read_flash_Data(Open_Info_Data, Open_Info_Data_0);
	}

	/*具体记录内容*/
	if((Open_Flag&0x0f) == 0x01 && !Flag_Inf[Combination_mode])
	{
		OpenInfo[Flag_Inf[Open_Info_CNT]%125].Open_flag = 0x00001000|(Number[0]<<8)|(Number[1]<<4)|(Number[2]);
	}
	else if((Open_Flag&0x0f) == 0x02 && !Flag_Inf[Combination_mode])
	{
		OpenInfo[Flag_Inf[Open_Info_CNT]%125].Open_flag = 0x00002000|(Number[0]<<8)|(Number[1]<<4)|(Number[2]);
	}
	#if 0
	else if(Flag_Inf[Combination_mode] == 1)
	{
		if(_get_Combination_Status()==1)
		{
			OpenInfo[Flag_Inf[Open_Info_CNT]%125].Open_flag = Open_Flag<<7|(Number[0]<<8)|(Number[1]<<4)|(Number[2])/
		}
		else
		{

		}
	}
	else if(Flag_Inf[Combination_mode] == 2)
	{
	
	}
	else if(Flag_Inf[Combination_mode] == 3)
	{

	}
	#endif
	//MkTime(&OpenInfo[Flag_Inf[Open_Info_CNT]%125].Open_info);
	Flag_Inf[Open_Info_CNT]++;
	SaveData_Inf(System_Data, System_Data_0);
	SaveData_Inf(Open_Info_Data, (Flag_Inf[Open_Info_CNT]/125) + Open_Info_Data_0);
}
#endif

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
	#if 0
	for(i=0;i<ADD_MAX;i++)
	{
		Flag_Inf[i] = 0;
	}
	for(i=0;i<100;i++)
	{
		UserInfo[i].Userflag = 0;
	}
	Flag_Inf[ADD_DEFAULT] = ADD_DEFAULT_STR;
	#endif
	for (i=0; i<BT_Admin_Max; i++)
	{
		Admin_Flag[i] =0;
	}
	
}

void check_Admin_info(void)
{
	#if 0
	Read_flash_Data(System_Data, System_Data_0);
	if(Flag_Inf[ADD_DEFAULT]!=ADD_DEFAULT_STR)
	{
		//printf("Flag_Inf[ADD_DEFAULT] = %d\r\n", Flag_Inf[ADD_DEFAULT]);
		defaultDataPro();
		SaveData_Inf(System_Data, System_Data_0);
		SaveData_Inf(User_Data, USER_Data_0);		
	}
	Read_flash_Data(User_Data, USER_Data_0);
	#ifdef __DEBUG2__
		printf("Flag_Inf[ADMIN] = %d\r\n", Flag_Inf[ADMIN]);
		printf("Flag_Inf[Admin_Pswd] = %d\r\n", Flag_Inf[Admin_Pswd]);
		printf("Flag_Inf[Admin_FPRT] = %d\r\n", Flag_Inf[Admin_FPRT]);
		printf("Flag_Inf[Admin_RFID] = %d\r\n", Flag_Inf[Admin_RFID]);
		printf("Flag_Inf[USER] = %d\r\n", Flag_Inf[USER]);
		printf("Flag_Inf[User_Pswd] = %d\r\n", Flag_Inf[User_Pswd]);
		printf("Flag_Inf[User_FPRT] = %d\r\n", Flag_Inf[User_FPRT]);
		printf("Flag_Inf[User_RFID] = %d\r\n", Flag_Inf[User_RFID]);
		printf("Flag_Inf[Audio_mode] = %d\r\n", Flag_Inf[Audio_mode]);
		printf("Flag_Inf[General_mode] = %d\r\n", Flag_Inf[General_Open_mode]);
		printf("Flag_Inf[Combination_mode] = %d\r\n", Flag_Inf[Combination_mode]);
		printf("Flag_Inf[Open_Info_CNT] = %d\r\n", Flag_Inf[Open_Info_CNT]);
	#endif
	#else
		u8 i=0;
		Read_flash_Data(BT_User_Data,BT_User_Page);
		Read_flash_Data(BT_Secret_Data,BT_Secret_Page);
		Read_flash_Data(BT_System_Data,BT_System_Page);

		printf("Admin_Flag[BT_Admin]=%x\r\n",Admin_Flag[BT_Admin]);

		if (Admin_Flag[BT_Admin] == 0xFF)
		{
			for (i=0; i<BT_Admin_Max; i++)
			{
				Admin_Flag[i] =0;
			}
		}
	#endif
	
}


