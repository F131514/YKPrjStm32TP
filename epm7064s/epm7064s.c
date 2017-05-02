#include "global.h"
#include "epm7064s.h"
#include "stm32f0xx_exti.h"
#include "led.h"

static volatile Uint32 Raw_Data     = 0;                       //ԭʼ�����ۼ�TOTAL_TIMESΪһ��������
static volatile Uint32 Raw_Data_Tmp = 0;
static Uint8  raw_data_high = 0;
static Uint32 raw_data_low  = 0; 

static void delay (int length)
	{
	  while (length >0)
    	 length--;
	}
		 
//**************************************************************************************/
//  ������     void Start_SPI(void)
//  �������   ��
//  �������   ��
/***************************************************************************************/
void  EPM7064_PIN_Init(void)
{
	  //����PA0-7 DATA
    Uint8 i;
	  EXTI_InitTypeDef EXTI_InitStruct;
	  NVIC_InitTypeDef NVIC_InitStruct;
    //�ܽ�����
	  GPIO_InitTypeDef GPIO_InitStruct;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //PA
		
    GPIO_InitStruct.GPIO_Pin   = EPM7064_DATA0|EPM7064_DATA1|EPM7064_DATA2|EPM7064_DATA3|EPM7064_DATA4|EPM7064_DATA5|EPM7064_DATA6|EPM7064_DATA7;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(EPM7064_DATA_PORT, &GPIO_InitStruct);
	  ////
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //PC
	  GPIO_InitStruct.GPIO_Pin   = EPM7064_DATA_HIG_PIN|EPM7064_DATA_LOW_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(EPM7064_CON_PORT, &GPIO_InitStruct);
	  
    GPIO_InitStruct.GPIO_Pin   = MOTOR_DRIVE_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStruct); 
	
	  GPIO_InitStruct.GPIO_Pin   = MOTOR_POSI_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStruct);
	
    ///////////////////////////////////////////////�����ⲿ�жϴ���Դ ģʽ
    /* Configyre P[A|B|C|D|E]0  NIVC  PB1 12*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		
		NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
	
	  /* EXTI line gpio config(PB12) */	
		GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1; 
	  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_Level_2;
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP; // ????
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* EXTI line(PB0) mode config */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
		EXTI_InitStruct.EXTI_Line    = EXTI_Line1;
		EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //?????
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStruct);
}
    
    
Uint32 Read_EPM7064_Low(void) //��ȡEPM7064���ֽ�
{
  Uint32 i;
  EPM7_DATA_LOW_ENABLE;
  delay(5);
  i = GPIO_ReadInputData(EPM7064_DATA_PORT)&0xff;
  EPM7_DATA_LOW_DISABLE;
  return(i);
}	
Uint32 Read_EPM7064_High(void) //��ȡEPM7064���ֽ�
{
  Uint32 i;
  EPM7_DATA_HIG_ENABLE;
  delay(5);
  i = GPIO_ReadInputData(EPM7064_DATA_PORT)&0x1f;  //ȡ5λ
  EPM7_DATA_HIG_DISABLE;
  return(i);
}

//**********************************************************************
//ԭʼ���ݴ���
//(1)T1����8
//(2)�ۼ�����ȥ�����1/512
//(3)�ۼ������ϱ����µ�����
//(4)�ۼ�512�����һ��������
//�ۼ� �˲�
/////////////////////////////////////////       
static void  epm7064s_filter_raw(Uint32 x)
       {
 Sint32 i,j;
 Uint32 k;
 static Uint8  u8,ua,ub;
 static Sint32     a, b;
 i = x << 8;         //extend 256
 i = i - (b >> 8);  //sub a/65536
 j = a >> 1;         //b/512
 j = i - j;
 //////////////////////ext 4.5bytes 
 u8 = ((u8>>4)&0x0f)|((j&0x0f)<<4); 
 j = j >> 4;         //j/16
 //////////////////////////////////
 k = 0;
 k = ua + u8;
 ua = k;   //ȡ��Ͳ���
 if(k>255) //��λ�������1
 a = a + j + 1;
 else
 a = a + j; //ua over
 //////////////////////////////////
 k = 0;
 k = ub + ua;
 ub = k;  //ȡ��Ͳ���
 if(k>255)
 b = b + a + 1;
 else
 b = b + a;
 ////////////ub over
 raw_data_low = raw_data_low + (Uint32)b;
 if(raw_data_low < b)  //������λ
 raw_data_high++;
 Raw_Data = (raw_data_high<<24)|((raw_data_low>>8)&0x00ffffff); 
 }
 
 
//**********************************************************************
//��������:FIQ_Exception()
//��������:�����жϴ������,��ȡAD7190������
//**********************************************************************
////////////////////////////////       
void EXTI0_1_IRQHandler(void)
{ 
    static Uint32 Cycle_times=0;    
    Uint32 x,y;
	 
    ////////////////////////////////////////use 7064s
    x =  Read_EPM7064_Low();
    y = (Read_EPM7064_High()<<8) | x;

    epm7064s_filter_raw(y);
    Cycle_times++;
    if(adc_times == Cycle_times)
        {
				 Cycle_times   = 0;
         raw_data_high = 0;
         raw_data_low  = 0;
         Raw_Data_Tmp  = Raw_Data;
				 flag_new_data = TRUE;
        }
		EXTI_ClearITPendingBit(EXTI_Line1);		
}
/////////////////////////////////////////////////////
///////////////////////////////////////////////////// 
 Uint32 Get_Raw(void)
 {
  return(Raw_Data_Tmp);
 }

