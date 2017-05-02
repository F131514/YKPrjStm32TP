/*****************************************************************************
**               AMICCOM Electronics Corporation Document                   **
**          Copyright (c) 2011-2014 AMICCOM Electronics Corporation         **
**                                                                          **
**      A3,1F,No.1, Li-Hsin 1th Road, Science_Based Industrid Park,         **
**                       Hsin_Chu City, 300, Taiwan, ROC.                   **
**                 Tel: 886-3-5785818   Fax: 886-3-5785819                  **
**         E-mail:info@amiccom.com.tw  http: //www.amiccom.com.tw           **
*****************************************************************************/
#ifndef _DEFINE_H_
#define _DEFINE_H_

//#include <stdio.h>
//#include <string.h>
//#include <intrins.h>
//#include <absacc.h>

typedef signed char     Sint8;
typedef short           Sint16;
typedef long            Sint32;
typedef unsigned char   Uint8;
typedef unsigned short  Uint16;
typedef unsigned long   Uint32;

#define NO                  0
#define YES                 1

#define ERROR               0
#define OK                  1

#define CALIBRATE_NUMBER    6

#define LOW                 0
#define HIGH                1

#define OFF                 0
#define ON                  1

#define DISABLE             0
#define ENABLE              1

#define FALSE               0
#define TRUE                1

#define PAIRON   1

#define BIT0   0x01
#define BIT1   0x02
#define BIT2   0x04
#define BIT3   0x08
#define BIT4   0x10
#define BIT5   0x20
#define BIT6   0x40
#define BIT7   0x80

#define BIT0N  0xFE
#define BIT1N  0xFD
#define BIT2N  0xFB
#define BIT3N  0xF7
#define BIT4N  0xEF
#define BIT5N  0xDF
#define BIT6N  0xBF
#define BIT7N  0x7F

#define	BASE_ADDR           ((Uint8 volatile code*)(0))

//201208//====================================================
//201208typedef struct
//201208{
//201208  BYTE  bit0 : 1;
//201208  BYTE  bit1 : 1;
//201208  BYTE  bit2 : 1;
//201208  BYTE  bit3 : 1;
//201208  BYTE  bit4 : 1;
//201208  BYTE  bit5 : 1;
//201208  BYTE  bit6 : 1;
//201208  BYTE  bit7 : 1;
//201208} tpBIT;
//201208//====================================================
//201208#define SetBit(var, bit_no)     {var |= (1 << bit_no);}
//201208#define ClrBit(var, bit_no)     {var &= ~(1 << bit_no);}
//201208#define GetBit(var, bit_def)    ((var&(1 << bit_def))?1:0)
//201208#define DefineBit(var, bitno)   (((tpBIT *)&var)->bitno)
//201208//====================================================
//201208
#endif