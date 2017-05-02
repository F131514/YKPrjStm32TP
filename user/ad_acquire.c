//**********************************************************************
//文件名:ad_acquire.c
//函数列表:
//1.初始化定时器1:         Init_Timer1  本程序没有使用
//2.初次化外部中断2:       Init_EINT2
//3.CPLD 原始数据累加函数  epm7064s_filter_raw
//4.快速中断处理:          FIQ_Exception(读取原始数据)
//5.返回累加后的原始数据:  Get_Raw
//**********************************************************************
#include "global.h"
#include "ad_acquire.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_dma.h"

volatile static Uint16  RegularConvData_Tab;      // ADC???  

void ADC1_Init(void)  
{  
  ADC_InitTypeDef     ADC_InitStruct;
  DMA_InitTypeDef     DMA_InitStruct;
	GPIO_InitTypeDef    GPIO_InitStruct;
	
	/* Enable  GPIOA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = ADC1_TEMP_PIN ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  //GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init(ADC1_PORT, &GPIO_InitStruct);
	
 DMA_DeInit(DMA1_Channel1);//????
 DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;//??????

 DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&RegularConvData_Tab;//??????

 DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
 DMA_InitStruct.DMA_BufferSize =4;//??? 4
 DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//???????
 DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//???????
 DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
 DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//???
 DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//????
 DMA_InitStruct.DMA_Priority = DMA_Priority_High;//????
 DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//??????
 DMA_Init(DMA1_Channel1, &DMA_InitStruct);

 DMA_Cmd(DMA1_Channel1, ENABLE);//????
 ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);//?? DMA ????
 /* Enable ADC_DMA */
 ADC_DMACmd(ADC1, ENABLE);

 ADC_StructInit(&ADC_InitStruct);
 /* ?? ADC1 ?????????? 12 bits */
 ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
 ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
 ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
 ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
 ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Backward;
 ADC_Init(ADC1, &ADC_InitStruct);
 /* Convert the ADC1 Vref with 55.5 Cycles as sampling time */
 ADC_ChannelConfig(ADC1, ADC_Channel_14 , ADC_SampleTime_55_5Cycles);
 /* ADC ?? */
 ADC_GetCalibrationFactor(ADC1);
 ADC_DMACmd(ADC1, ENABLE);
 /* ?? ADC1 */
 ADC_Cmd(ADC1, ENABLE);
 /* ?? ADCEN ?? */
 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

 ADC_StartOfConversion(ADC1);
}

Uint16 Get_temp_ad(void)
{
 return(RegularConvData_Tab);
}
