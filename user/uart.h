#ifndef __UART_H
#define	__UART_H

#include "stm32f0xx.h"
/*
**************************************************************************************************************
*                                    send data to PC
*
**************************************************************************************************************
*/
//????
//??????(2???)+ ????LEN(1???)+ ?????(1???) 
//+ ????(n???) + ??????(2???) + ???(2???)
//
//
#define   FRAME_START_CHAR1               0x7e
#define   FRAME_START_CHAR2               0x7f
#define   FRAME_END_CHAR1                 0xea
#define   FRAME_END_CHAR2                 0xeb


#define   INSTRU_CODE_PH                  0x00
#define   INSTRU_CODE_DDLV                0x01
#define   INSTRU_CODE_RJY                 0x02
#define   INSTRU_CODE_LZJ                 0x03
#define   INSTRU_CODE_BALANCE             0x04

#define   PARA_CODE_PH                    0x00
#define   PARA_CODE_MV                    0x01
#define   PARA_CODE_COND                  0x02
#define   PARA_CODE_RES                   0x03
#define   PARA_CODE_TDS                   0x04
#define   PARA_CODE_SAL                   0x05
#define   PARA_CODE_DO1                   0x06
#define   PARA_CODE_DO2                   0x07
#define   PARA_CODE_TEMP                  0x08

#define   DATA_EXPAND_1                   0x00
#define   DATA_EXPAND_10                  0x01
#define   DATA_EXPAND_100                 0x02
#define   DATA_EXPAND_1000                0x03
#define   DATA_EXPAND_10000               0x04
#define   DATA_EXPAND_0_1                 0x05
#define   DATA_EXPAND_0_01                0x06
#define   DATA_EXPAND_0_001               0x07
#define   DATA_EXPAND_0_0001              0x08

#define   DATA_EXPAND_1_N                 0x09
#define   DATA_EXPAND_10_N                0x0A
#define   DATA_EXPAND_100_N               0x0B
#define   DATA_EXPAND_1000_N              0x0C
#define   DATA_EXPAND_10000_N             0x0D
#define   DATA_EXPAND_0_1_N               0x0E
#define   DATA_EXPAND_0_01_N              0x0F
#define   DATA_EXPAND_0_001_N             0x10
#define   DATA_EXPAND_0_0001_N            0x11
extern void USART_Configuration(void);
extern uint8_t UART_Recive(void);
extern void UART_Send(uint8_t *Buffer, uint32_t Length);

#endif /* __LED_H */