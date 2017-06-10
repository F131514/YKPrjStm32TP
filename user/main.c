/******************** (C) COPYRIGHT 2012 ������ ********************
 * �ļ���  ��main
 * ����    ��         
 * ʵ��ƽ̨�����stm32f030������
 * ����    ��LED��˸
 * ����    �����
 * ����    ��qfv5.taobao.com
**********************************************************************/

#include "global.h"
#include "ht1621_driver.h"
#include "timer.h"
#include "rtc.h"
#include "led.h"
#include "key.h"
#include "I2C.h"
#include "filter.h"
#include "EPM7064s.h"
#include "ad_acquire.h"

const Uint8 send_buf[] = "hello,world";
/////////////////////////////////////////
Uint32 const machine_speed[4][3] = {
{112*3,      448,       112*4},
{112*3,      400,       112*4},
{112*3,      360,       112*4},
{112*3,      320,       112*4}
};


///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
void Factory_Pro(void)
 {	
	static  Uint32  sample_temp_time = 0;
  	      Uint32  cycle_times_500ms,cycle_times_10ms;
	 
	Factory_Inncal_step = 0;
	 
  while(1)
  {	 
	 if (TRUE == flag_new_data) //�����ݴ���
				    {
             flag_new_data = FALSE;
             filter();
             update_new_data();
            }
									
       if (flag_10ms_ok)    //10ms???
            {
             flag_10ms_ok = FALSE;
             cycle_times_10ms++;
             //key scan
             Key_F_Pro(Get_Key_Code());  //����ģʽ�µİ������� 							 
            }  
      
       if (flag_100ms_ok)
            {//ˢ��������
             flag_100ms_ok = FALSE;  
						 Ht1621_F_Display_area1();
						
					   Update_Display();	
            }    
         
       if (flag_300ms_ok)
            {
             flag_300ms_ok = FALSE;
         		 ///////////////
						 Ht1621_F_Display_area3();
            						
						}
       if (flag_500ms_ok)
            {                   //500ms
             flag_500ms_ok = FALSE;
             cycle_times_500ms++;
						 ////////////////////////////////	
             if(cycle_times_500ms&0x01)							
               LED_Close(); 
						 else
							 LED_Open();						
						}
    }
	} 
///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
void Normal_Pro(void)
 {	
	static  Uint32  sample_temp_time = 0;
  	      Uint32  cycle_times_500ms,cycle_times_10ms;
	        Uint8   buf[16];
  while(1)
  {	 
	 if (TRUE == flag_new_data) //�����ݴ���
				    {
             flag_new_data = FALSE;
             filter();
             update_new_data();
            }
       if (flag_10ms_ok)    //10ms???
            {
             flag_10ms_ok = FALSE;
             cycle_times_10ms++;
             /////////////////30ms 
             if(0==(cycle_times_10ms%3))
                {
								 //read temp	
                 //while(!(ADDR3&0x80000000));
                 //ad_tmp = (ADDR3>>6)&0x3ff;
                 //temp_float = filter_ad_NTC(ad_tmp) - 5; ///1024.0001 *3300;
                }
             //key scan
             Key_Pro(Get_Key_Code());							 
             }  
      
       if (flag_100ms_ok)
            {//ˢ��������
             flag_100ms_ok = FALSE;
             if(FALSE == sleepmode)
						   {
							  Ht1621_Display_area1();
					      Update_Display();
						   }	 
            }    
         
       if (flag_300ms_ok)
            {//ˢ�´ν���
             flag_300ms_ok = FALSE;
          	 temp_float = filter_ad_NTC(Get_temp_ad()) - 7;
						 if(FALSE == sleepmode)	
						  {
							 Ht1621_Display_area2();
						   Ht1621_Display_area3();
							}				
						}
       if (flag_500ms_ok)
            {                   //500ms
             flag_500ms_ok = FALSE;
             cycle_times_500ms++;
             if(cycle_times_500ms&0x01)							
               LED_Close(); 
						 else
							 LED_Open();
            }
            ////////////////////////????????,?????????    
       if (flag_1s_ok)     
             ////////////////////////////////?????????
            {
             flag_1s_ok = FALSE;
					   sample_temp_time++;
						 if((abs(temp_cmp_point-temp_float) > TEMP_WAR_RANGE)&&(TRUE==temp_cmp_flag))
							 temp_error_flag = TRUE;
             else
							 temp_error_flag = FALSE;
             /////////////////////////�������ȶ�ʱ���жϣ������ȶ�����40S��ǿ�ƿ���							
						 if((TRUE==power_on_flag)&&(sample_temp_time>40))
							   power_on_flag = FALSE;  
						 
		         if((1 == Uart_Printer_Flag)&&(TRUE == stable_flag)) {
						     if((uart_format_type<UART_TYPE_MANU_F1_1200))
						     {
							     if(uart_format_type < UART_TYPE_AUTO_F2_1200)     //ѡ��1 2 3 4
							     UART_SendData_NJ_format1();
							     else
							     UART_SendData_NJ_format2();	 
						 	   } else {
								   if(uart_format_type < UART_TYPE_MANU_F2_1200)     //ѡ��1 2 3 4
							       UART_SendData_NJ_format1();
							      else
							       UART_SendData_NJ_format2();	 
						       }
								 } 
					
             if(sample_temp_time > 1800)
                 {
                  temp_cmp_flag = TRUE;
                  temp_cmp_point = temp_float;             //?????? ??????????
                 }
            }
    }
	} 

	
/**********************************************/
/* �������ܣ�������                           */
/* ��ڲ�������                               */
/**********************************************/
int main(void)
{
	Uint8 i,j;
	Uint8 buf[4];
	
	Uint32 k;
		
	TIM3_Init(200);
  RTC_Config();
	
	Init_1621();
	
	CPU_LED_Init();
	BL_LED_Init();
	SPEAKER_PIN_Init();
	Key_Init();
	I2c_Init();
	
	//��UART���������� �Ա���ʼ��UART
	Init_UARTCONFIG_Para();
	USART_Configuration();
	//Printer_Init();        //���ô�ӡ��
	
	EPM7064_PIN_Init(); 
	///////////////////////////////////////////////��CPLD�����������ȶ�����
	Read_EEPROM(EEP_SYS_DIV_ADDR, buf, 4);
  for(i=0;i<200;i++)	{;}	             //delay
	if((CHECK_DATA==buf[3])&&(buf[2]==buf[1]+buf[0]))
    dot_position = buf[0];
	else
		dot_position = WAN_DIVISION;
	for(i=0;i<4;i++)  buf[i] = 0;
	Read_EEPROM(EEP_SYS_ASD_ADDR, buf, 4);
  for(i=0;i<200;i++)	{;}	             //delay
	if((CHECK_DATA==buf[3])&&(buf[2]==buf[1]+buf[0])) //ASD ����
		  asd_para = buf[0]; 
  else
		  asd_para = 0;	
	
 	adc_times = machine_speed[asd_para][dot_position]; //���ȸ����ֵ ��������Ĵ˲���
	/////////////////////////////////////////////////////////////////////////
	ADC1_Init();
	////////////////////////�˴��ж��Ƿ�����Ԥ��ʱ�� �� ��ʾ Ԥ��ʱ�䵹��ʱ	
  //��У׼���� ����Ѱ��λ��
	for(k=0;k<12345;k++)
	;
	#if(INNER_CAL_VERSION)
			{	
				i = MOTOR_POSITION;  i++;
				motor_err_flag = motor_run_proc(i&0x01);
				for(k=0;k<12345;k++);
				i = MOTOR_POSITION;  i++;
				motor_err_flag = motor_run_proc(i&0x01);
				for(k=0;k<12345;k++);
				i = MOTOR_POSITION;  i++;
				motor_err_flag = motor_run_proc(i&0x01);
				for(k=0;k<12345;k++);
				
				if(LOAD_POSITION == MOTOR_POSITION)	
			  motor_err_flag = motor_run_proc(UNLOAD_POSITION);
				/*
				if(1 == motor_err_flag)   //������⵽���λ�ô���
			    {
           while(1){ Speaker_Time = 99;}					
				  }
				*/
			}
	#endif		
	////////////////////////////	  
	BL_Open();
	prehot_time = PREHOT_TIME_CNT;
	do
		{
		 Display_Prehot_time();
     if(prehot_time == (PREHOT_TIME_CNT-8))
			  BL_Close();
			i = Get_Key_Code();		
	  }
	while((0!=prehot_time)&&((KEY_LONG+KEY_ON_OFF)!=i));
		
	if(0 == prehot_time)
	{
	 //�ȴ�����
	 while(1)
	 {	 
	  i = Get_Key_Code();
		if(i==KEY_ON_OFF)
		{
			Speaker_Time = KEY_TIME_NORMAL; 
      break;
	  }		
	 	if(i==(KEY_ON_OFF+KEY_LONG))
	 	  break;	
	 }
	} 
	//////////////////////////////////////////////////////////////////////////
	
	
	
	BL_Open(); 
	Displaybuf_All_on();  //��LOGO�� ȫ����ʾ ʱ���ɳ�ʼ������ȷ��
	//���Ȱ� LOGO ��Ϣ �Ƿ���ʾ��ȡ����
  Init_SYSLOGO_Para();
  if(FALSE == flag_LOGO)
	   display_buffer[0] &= (~FLAG_LOGO_0);
	Update_Display();
	///////////////////////////////////////////////////////////////////////////			
  i = System_Init();
	Displaybuf_All_off(); 
	if(MACHINE_FACTORY_MODE == i)
	   {
		  Factory_Pro();
		 }
	else
	   {
					
	    Display_Model_Info();    //���� �ͺ���ʾ����
	    Update_Display();
	    i =20;
	    do
	    {if(flag_100ms_ok)
			 {flag_100ms_ok = FALSE; i--;}
	    }
	    while(i!=0);      
	    /////////////////////*/
			
	    Displaybuf_All_off();
		
      Normal_Pro();
		 }		
	while(1){;}
}
