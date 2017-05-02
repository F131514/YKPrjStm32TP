/******************** (C) COPYRIGHT 2011 青风电子 ********************
 * 文件名  ：led.c
 * 描述    ：         
 * 实验平台：青风stm32f051开发板
 * 描述    ：led驱动函数
 * 作者    ：青风
 * 店铺    ：qfv5.taobao.com
**********************************************************************************/

#include "led.h"
/**********************************************/
/* 函数功能； led灯 初始化                     */
/* 入口参数：无                               */
/**********************************************/
void CPU_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStruct.GPIO_Pin = CPU_LED_PIN ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init(CPU_LED_PORT, &GPIO_InitStruct);
	GPIO_SetBits(CPU_LED_PORT, CPU_LED_PIN );
}

void BL_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStruct.GPIO_Pin = BL_LED_PIN ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init(BL_LED_PORT, &GPIO_InitStruct);
	//GPIO_SetBits(BL_LED_PORT, BL_LED_PIN );
}

void SPEAKER_PIN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStruct.GPIO_Pin = SPEAKER_PIN ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init(SPEAKER_PORT, &GPIO_InitStruct);
	GPIO_ResetBits(SPEAKER_PORT, SPEAKER_PIN );
}
/**********************************************/
/* 函数功能；打开led灯                        */
/* 入口参数：无                               */
/**********************************************/
void LED_Open(void)
  {
	 GPIO_ResetBits(CPU_LED_PORT, CPU_LED_PIN );
	}
	
void BL_Open(void)
  {
	 GPIO_SetBits(BL_LED_PORT, BL_LED_PIN );
	}	

void SPEAKER_Open(void)
  {
	 GPIO_SetBits(SPEAKER_PORT, SPEAKER_PIN );
	}		
/**********************************************/
/* 函数功能； 关掉led灯                        */
/* 入口参数：无                               */
/**********************************************/
void LED_Close(void)
{
		GPIO_SetBits(CPU_LED_PORT, CPU_LED_PIN );
}

void BL_Close(void)
{
		GPIO_ResetBits(BL_LED_PORT, BL_LED_PIN );
}
void SPEAKER_Close(void)
  {
	 GPIO_ResetBits(SPEAKER_PORT, SPEAKER_PIN );
	}	
