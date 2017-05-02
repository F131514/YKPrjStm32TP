/*****************************************************************************
 *   rtc.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __RTC_H 
#define __RTC_H

#include  "global.h"
/////////////////////////////////
extern void        RTC_Config(void);
extern void        RTC_IRQHandler(void);

#endif /* end __RTC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
