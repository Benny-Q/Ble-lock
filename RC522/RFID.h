#ifndef __RFID_H
#define __RFID_H

#include "stm32f0xx.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>

#include "mains.h"


void IC_test ( void );
uint8_t RFID_Init(uint8_t *uid , uint8_t loop);
int RFID_AddPICC(uint8_t UserID,uint16_t User_Number);
int RFID_Compare(void);
void RFID_Delete (uint16_t UserID , uint8_t permissions);
extern void RFIDPro(void);
extern void RFIDGpioInit(void);
extern u8 RFIDTouchHave(u8 uType,u8 *uData,u8 uLength);
#endif
