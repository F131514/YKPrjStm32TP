/*****************************************************************************
 *   rtc.c:  Realtime clock C file for NXP LPC23xx/34xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.12  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "global.h"
#include "rtc.h"

#include "stm32f0xx_rtc.h"
#include "stm32f0xx_exti.h"


/*********************************************************************************
 *   Function name:    RTCHandler
 *   Describtion: the Tick initializtion of Rtc Clock for LPC2378
 *   Author:      Zhangwei
 *   Data:        2007.11.26
 *   Parameters:  NULL
 *   Return :     NULL
 *   History:
 *           2007.11.26 Prelimnary version,first Release
**********************************************************************************/
void RTC_IRQHandler(void)
{
  if(SET == RTC_GetITStatus(RTC_IT_TS))
	 {
	  RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
		 
	  //Flag_Second_Change = TRUE;
	  //elapsed_time++;
	 }   
}

void RTC_INT_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  /* RTC Alarm A Interrupt Configuration */
  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

void Set_RTC_Date(Uint16 year,Uint8 month,Uint8 day)
{
  RTC_DateTypeDef RTC_DateStructure;

  RTC_DateStructure.RTC_Year=year;
  RTC_DateStructure.RTC_Month=month;
  RTC_DateStructure.RTC_Date=day; 
 	RTC_DateStructure.RTC_WeekDay=0; 

while(RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure)==ERROR){};
}
////////////////////////////////////////////////////////////////////
//RTC 时间调整
////////////////////////////////////////////////////////////////////
void RTC_TimeRegulate(void)
{
  RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_TimeTypeDef  RTC_TimeStructure; 
	
	
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 8; 
	RTC_TimeStructure.RTC_Minutes = 8;
	RTC_TimeStructure.RTC_Seconds = 8;
  /* Configure the RTC time register */
  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
  {
  } 
  else
  {
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32f0);
  }

  /* Disable the Alarm A */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 8;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 8;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 13;
  /* Set the Alarm A */
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

  /* Configure the RTC Alarm A register */
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
 
  /* Enable the RTC Alarm A Interrupt */
  RTC_ITConfig(RTC_IT_TS, ENABLE);
   
  /* Enable the alarm  A */
  //RTC_AlarmCmd(RTC_IT_TS, ENABLE);
}

void Set_RTC_Time(Uint8 hour,Uint8 minute,Uint8 second)
{
 RTC_TimeTypeDef RTC_TimeStructure;

 RTC_TimeStructure.RTC_Hours=hour; 
 RTC_TimeStructure.RTC_Minutes=minute; 
 RTC_TimeStructure.RTC_Seconds=second; 

 while(RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure)==ERROR ){}; 
}


void RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	
	 /* 使能 PWR 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* 允许访问RTC */
  PWR_BackupAccessCmd(ENABLE);
    
  RCC_LSICmd(ENABLE);

  /* 等待到 LSI 预备*/  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* 把RTC 时钟源配置为LSI */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
   /* 定襎同步分频值和异步分频值 */
  //SynchPrediv = 0x18F;
  //AsynchPrediv = 0x63;

  /* 使能RTC时钟 */
  RCC_RTCCLKCmd(ENABLE);

  /* 等待 RTC APB 寄存器同步 */
  RTC_WaitForSynchro();
	
	 RTC_InitStructure.RTC_AsynchPrediv = 0x63;
   RTC_InitStructure.RTC_SynchPrediv = 0x18F;
   RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   
    /* 检查是否已经初始化 */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
		{
		
		}
	
   /*设置寄存器 */
    RTC_TimeRegulate(); 
		
	RTC_INT_Config();
	
 

}


/*****************************************************************************
**                            End Of File
******************************************************************************/