#ifndef _INFOCHECK_H_
#define _INFOCHECK_H_

#include "mains.h"
#if 0
extern uint8_t __Error_flag;
extern uint8_t __Correct_flag; 

typedef struct
{
	u8 flag;
	u8 CNT;
	u8 value[3];
	u16	num;
	
}Del_Inf;

typedef enum
{
	Null_info,
	Pswd_info,
	RFID_info,
	FPRT_info,
}Combination_info;

typedef enum
{
	PSWD_Mode,
	FPRT_Mode,	
	RFID_Mode
}Open_Door_Mode;

typedef struct tagkey
{
	uint8_t useflag;     //????
	uint8_t passNum;     //????
	uint8_t password[12]; //12??? //???6?,????????????[0]
}KeyInf_t;

extern KeyInf_t key_info_first;
extern Combination_info Combination_Info;
extern Open_Door_Mode Open_Door_Info;
//extern uint8_t _get_Combination_info(void);
//extern void _set_Combination_info(uint8_t Combination_info);
//extern void _clean_Combination_info(void);

//extern uint8_t _get_Combination_Status(void);
//extern void _set_Combination_Status(uint8_t _status);
//extern void _clean_Combination_Status(void);

//extern void set_PSWD_input_status(uint8_t _status);
//extern uint8_t get_PSWD_input_status(void);

//extern uint8_t scan_one_key_input(uint8_t * key_iteam);
//void Query_User_Space(void);
//extern int verify_PSWD_input_key_serial(uint8_t *PSWD_input_serial);
#endif
#if 0
extern uint8_t Check_Admin(void);
#else
extern void check_Admin_info(void);
#endif

extern void clean_input_key_value(void);
///extern uint8_t Delet_data(void);
//extern void Delet(void);
//extern int Set_Pswd(uint8_t Menu_Type,uint16_t User_Number);
//void Pswd_Delet(uint8_t UserID , uint8_t permissions);
//int save_PSWD_to_Flash(uint8_t PSWD_CNT, uint8_t *PSWD_array, uint8_t permissions, uint16_t User_Number);
#if 1
int Dynamic_Pswd(uint8_t *PSWD_input_serial);
#endif

//extern uint8_t _get_Erro_info(void);
//extern void _set_Error_info(uint8_t i);
//extern void _clean_Error_info(void);
//extern uint8_t _get_Correct_info(void);
//extern void _set_Correct_info(uint8_t i);
//extern void _clean_Correct_info(void);


//void Delet_Information(uint16_t Delet_Number);
//int Add_Information(uint8_t permissions);

//void Save_Open_Door_Info(uint8_t Open_Flag,uint16_t User_Number);
//void Query_Open_Door_Info(void);
extern void defaultDataPro(void);





#endif



