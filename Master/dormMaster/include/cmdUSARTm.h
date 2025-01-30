#ifndef __CMD_USARTM_H
#define __CMD_USARTM_H


#include <Arduino.h>
#include <HardwareSerial.h>

#define cmdUSART_LENGTH 10 

extern uint8_t cmdUSART_TxPacket[cmdUSART_LENGTH];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
extern uint8_t cmdUSART_RxPacket[cmdUSART_LENGTH];				//定义接收数据包数组
extern uint8_t cmdUSART_RxFlag;					//定义接收数据包标志位

void cmdUSART_setMODE();





#endif 