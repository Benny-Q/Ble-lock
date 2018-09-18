#ifndef  __MAINS_H_
#define	 __MAINS_H_


//#define  __S900__
#define  __S500__

#define  __VOICE_SUPPORT__
//#define  __PREVENT__
//#define  __FOR_DEMO__
#define  macSPI1_RC522
#define  __UART1_DEBUG__
//#define  __UART2_DEBUG__
#define __FPRT_BYD__
//#define __FPRT_SET_REG_
#define RTC_CLOCK_SOURCE_LSE
//#define __HALL_SUPPORT__
#define __BLUETOOTH_SUPPORT__


/**********************/
#define  __TOUCH_KEY_SUPPORT__
//触控IC选择
//#define __APT8L16__
#define __APT_GT316L_DEVICE__
/**********************/

#include "common.h"

#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_conf.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "STM_GPIO_List.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_TiMbase.h"
#include "bsp_RTC.h"
#include "bsp_LED.h"
#include "bsp_ADC.h"
#include "bsp_Config.h"
#include "bsp_key.h"

#include "FingerPrint.h"
#include "APT8L16.h"
#include "delay.h"
#include "RFID.h"
#include "Motor.h"
#include "audio.h"
#include "Flash.h"
#include "Prevent_lock.h"
#include "Sleep_system.h"
#include "Information_Check.h"
#include "Config_Menu.h"
#include "Bluetooth.h"
#include "STM_GPIO_List.h"

#include "Ble.h"

//宏常量定义
#define BULETOOTH_MAC_MAX 14    //蓝牙mac地址最大
#define SECRET_INFO_NUM_MAX 30  //密码表信息存储最大个数
#define USER_INFO_NUM_MAX 30  //用户表信息存储最大个数


extern u8 _get_system_sleep_status(void);
extern void _set_system_sleep_status(void);
extern void _clear_system_sleep_status(void);


extern u8 Sys_sleepflag;
extern u8 PSWD_input_status;
extern u8 __wake_up_flag;
extern u8 __Config_flag;
extern u8 __Prevent_lock_flag;
extern u8 Open_Info_flag_temp[2];
extern u16 Open_Info_Numb_temp[2];
extern u32 time_info_start; 
extern u32 time_info_end; 

#define SYS_SLEEP_TIMER_CNT_THD 1000



#endif

