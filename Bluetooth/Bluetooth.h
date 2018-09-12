#ifndef __BLUETOOTH_H_
#define __BLUETOOTH_H_

#include "mains.h"

#define Bluetooth_POWER_ON 	GPIO_SetBits(BT_POWER_PORT, BT_POWER_PIN)
#define Bluetooth_POWER_OFF GPIO_ResetBits(BT_POWER_PORT, BT_POWER_PIN)

#define Bluetooth_RSEST_ON 	GPIO_SetBits(BT_RSTN_PORT, BT_RSTN_PIN)
#define Bluetooth_RSEST_OFF GPIO_ResetBits(BT_RSTN_PORT, BT_RSTN_PIN)

#define Bluetooth_CONTROL_ON	GPIO_SetBits(BT_CONTROL_PORT, BT_CONTROL_PIN)
#define Bluetooth_CONTROL_OFF	GPIO_ResetBits(BT_CONTROL_PORT, BT_CONTROL_PIN)

extern u8 Bluetooth_Rx_Buf[80];
extern u8 Bluetooth_Rx_Num;

void Bluetooth_init(void);
void Bluetooth_Exit_init(void);
void blethpro(void);
#endif
