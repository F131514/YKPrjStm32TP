/******************** (C) COPYRIGHT 2011 ������ ********************
 * �ļ���  ��led.c
 * ����    ��         
 * ʵ��ƽ̨�����stm32f051������
 * ����    ��led��������
 * ����    �����
 * ����    ��qfv5.taobao.com
**********************************************************************************/

#include "led.h"
/**********************************************/
/* �������ܣ� led�� ��ʼ��                     */
/* ��ڲ�������                               */
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
/* �������ܣ���led��                        */
/* ��ڲ�������                               */
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
/* �������ܣ� �ص�led��                        */
/* ��ڲ�������                               */
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
