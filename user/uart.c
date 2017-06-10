//**********************************************************************
//文件名:uart.c
//说明:
//**********************************************************************
#include "global.h"
#include "uart.h"

//机器型号对应表
const Uint8 *machine_name[3][12] = {
{"SY103","SY113","SY123","SY153","SY163","SY183","SY203","SY213","SY223","SY263","SY303","SY503"},
{"SY104","SY114","SY124","SY154","SY164","SY184","SY204","SY214","SY224","SY264","SY304","SY504"},
{"SY105","SY115","SY125","SY155","SY165","SY185","SY205","SY215","SY225","SY265","SY305","SY505"}
};
//内校天平
const Uint8 *machine_nameC[3][12] = {
{"SY103C","SY113C","SY123C","SY153C","SY163C","SY183C","SY203C","SY213C","SY223C","SY263C","SY303C","SY503C"},
{"SY104C","SY114C","SY124C","SY154C","SY164C","SY184C","SY204C","SY214C","SY224C","SY264C","SY304C","SY504C"},
{"SY105C","SY115C","SY125C","SY155C","SY165C","SY185C","SY205C","SY215C","SY225C","SY265C","SY305C","SY505C"}
};

void USART_Configuration(void)//串口初始化函数
  {  

        GPIO_InitTypeDef  GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
                
        RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );
                
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);        
        /*
        *  USART1_TX -> PA9 , USART1_RX ->        PA10
        */                                
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;                 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOA, &GPIO_InitStructure);        
        
		    /*
		    switch(uart_format_type)
				{
					case    1:
					case    5:
					case    9:
					case 0x0d:
						        USART_InitStructure.USART_BaudRate = 1200;//设置串口波特率
					          break;
					case    2:
					case    6:
					case 0x0a:
					case 0x0e:
						        USART_InitStructure.USART_BaudRate = 2400;//设置串口波特率
					          break;	
					case    3:
					case    7:
					case 0x0b:
					case 0x0f:
						        USART_InitStructure.USART_BaudRate = 4800;//设置串口波特率
					          break;		
					case    4:
					case    8:
					case 0x0c:
					case 0x10:
						        USART_InitStructure.USART_BaudRate = 9600;//设置串口波特率
					          break;		
				}
        */
		    switch(uart_format_type)
				{
					case    1: USART_InitStructure.USART_BaudRate = 1200;break;
					case    2: USART_InitStructure.USART_BaudRate = 2400;break;
					case    3: USART_InitStructure.USART_BaudRate = 4800;break;
					case    4: USART_InitStructure.USART_BaudRate = 9600;break;
          case    5:					
					default:
						         USART_InitStructure.USART_BaudRate = 9600;   //设置串口波特率
 				             break;
				}	
        //USART_InitStructure.USART_BaudRate = 9600;//设置串口波特率
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位
        USART_InitStructure.USART_StopBits   = USART_StopBits_1;//设置停止位
        USART_InitStructure.USART_Parity     = USART_Parity_No;//设置效验位
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置流控制
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置工作模式
        USART_Init(USART1, &USART_InitStructure); //配置入结构体

        USART_Cmd(USART1, ENABLE);//使能串口1
		}			

void UART_PrintChar(uint8_t byte) //发送1字节数据
{
 while(!((USART1->ISR)&(1<<7)));
 USART1->TDR=byte;	
}		

void UART_CommStr(Uint8 *str, Uint8 length)
{
	Uint8 i;
  for(i=0;i<length;i++)
       UART_PrintChar(*str++);
}
 
 void  UART_PrintStr(char *str)
{
 while(*str)
 {
   UART_PrintChar(*str++);
 }
}

uint8_t UART_Recive(void)
{	
	while(!(USART1->ISR & (1<<5)));//等待接收到数据
	return(USART1->RDR);			 //读出数据
}
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////


/*
**************************************************************************************************************
*                                         PRINT STRING
*
* Description: This function is used to print a string
*
* Arguments  : str    Pointer to the string
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_SendData (void)
{
  Uint16  check_value;
  Uint32  i,tmp;
  char send_tmp[50];
  //////////////////////////////
  check_value = 0;
  i = 0;
  
  send_tmp[i++] = FRAME_START_CHAR1;
  send_tmp[i++] = FRAME_START_CHAR2;
  #if SDJ_SOFT  //????? ?????? 
  send_tmp[i++] = 22;             //?? ?? 4+6x 
  // ph_negative_flag ph_data;
  // mv_negative_flag mv_data;
  // temp_float_ptc
  send_tmp[i++] = INSTRU_CODE_BALANCE;   //PH   ????
  /////////////////////////////////////////////////////////////
  //??PH??
  send_tmp[i++] = PARA_CODE_PH;       //PH? 
  tmp = (uint32)(net_weight*10000);
  send_tmp[i++] =  tmp&0xff;
  send_tmp[i++] = (tmp>>8)&0xff;
  send_tmp[i++] = (tmp>>16)&0xff;
  send_tmp[i++] = (tmp>>24)&0xff;
  //??
  if(FALSE == positive_flag)
  send_tmp[i++] = DATA_EXPAND_1000_N; //??
  else
  send_tmp[i++] = DATA_EXPAND_1000;   //??
  ////////////////////////////////////////////////////////////
  //??MV??
  send_tmp[i++] = PARA_CODE_MV;       //MV? 
  tmp = (Uint32)(net_weight*1000);
  send_tmp[i++] =  tmp&0xff;
  send_tmp[i++] = (tmp>>8)&0xff;
  send_tmp[i++] = (tmp>>16)&0xff;
  send_tmp[i++] = (tmp>>24)&0xff;
  //??
  send_tmp[i++] = DATA_EXPAND_1000;   //??
  ////////////////////////////////////////////////////////////
  //??????
  send_tmp[i++] = PARA_CODE_TEMP;     //MV? 
  tmp = (uint32)(temp_float_ptc*10);
  send_tmp[i++] =  tmp&0xff;
  send_tmp[i++] = (tmp>>8)&0xff;
  send_tmp[i++] = (tmp>>16)&0xff;
  send_tmp[i++] = (tmp>>24)&0xff;
  //????
  send_tmp[i++] = DATA_EXPAND_10;    //??
 
  #else
  send_tmp[i++] = INSTRU_CODE_DDLV;   //PH   ????
  send_tmp[i++] = PARA_CODE_COND; //COND 
  ////////////////////////////////////////////////////////////
  //??????
  
  send_tmp[i++] = 0;
  send_tmp[i++] = 0;
  send_tmp[i++] = 0;
  send_tmp[i++] = 0;
  //????
  send_tmp[i++] = 0x02; //??
  #endif 
  send_tmp[i++] = 0x00;
  send_tmp[i++] = 0x00;
  send_tmp[i++] = FRAME_END_CHAR1;
  send_tmp[i++] = FRAME_END_CHAR2;
  for(tmp=2;tmp<(i-2);tmp++)
   check_value += send_tmp[tmp]; //???
  send_tmp[i++] = check_value&0xff;
  send_tmp[i++] = (check_value>>8)&0xff;
 
  UART_CommStr((char *)send_tmp,i);                                                    

}

void  UART_SendData_NJ_format1(void)
{
  char send_tmp[16];
  //////////////////////////////
  if(positive_flag)
	send_tmp[0]  = 0x2b;
	else
	send_tmp[0]  = 0x2d;
	
	send_tmp[1]  = 0x20;
	send_tmp[2]  = 0x20;
	send_tmp[3]  = 0x20;
	send_tmp[4]  = 0x20;
	send_tmp[5]  = 0x2e;
	send_tmp[6]  = 0x20;
	send_tmp[7]  = 0x20;
	send_tmp[8]  = 0x20;
	send_tmp[9]  = 0x20;
	
	sprintf(&send_tmp[2],"%8.4f", net_weight);
	
	send_tmp[10] = 0x20;
	send_tmp[11] = 0x67;
	send_tmp[12] = 0x20;
	send_tmp[13] = 0x20;
	send_tmp[14] = 0x0d;
	send_tmp[15] = 0x0a;
  UART_CommStr((char *)send_tmp,16);  
	
}

void  UART_SendData_NJ_format2(void)
{
   char send_tmp[16];
  //////////////////////////////
	send_tmp[0]  = 0x41;
	send_tmp[1]  = 0x20;
	send_tmp[2]  = 0x20;
	
	if(positive_flag)
	send_tmp[3]  = 0x2b;
	else
	send_tmp[3]  = 0x2d;
	
	send_tmp[4]  = 0x20;
	send_tmp[5]  = 0x20;
	send_tmp[6]  = 0x20;
	send_tmp[7]  = 0x2e;
	send_tmp[8]  = 0x20;
	send_tmp[9]  = 0x20;	
	send_tmp[10] = 0x20;
	send_tmp[11] = 0x20;	
	sprintf(&send_tmp[4],"%8.4f", net_weight);
	/*
	switch(current_unit)
	{
		case UNIT_G:   send_tmp[12] = 0x20;  send_tmp[13] = 'g';    break;
	  case UNIT_OZ:  send_tmp[12] = 'o';   send_tmp[13] = 'z';    break;
		case UNIT_CT:  send_tmp[12] = 'c';   send_tmp[13] = 't';    break;
		default:	     send_tmp[12] = 0x20;  send_tmp[13] = 0x20;   break;
	}	
	*/
	send_tmp[12] = 0x20;
	send_tmp[13] = 0x67;
	send_tmp[14] = 0x0d;
	send_tmp[15] = 0x0a;
  UART_CommStr((char *)send_tmp,16);                                                
                                        
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static void  Printer_enter(void)
  {
   //UART_PrintChar(0x0d);
   UART_PrintChar(0x0a);
  }
static void  Printer_CH_MODE(void)
  {
   UART_PrintChar(0x1C);
   UART_PrintChar(0x26);  //??????
  }
static void  Printer_EN_MODE(void)
  { 
   UART_PrintChar(0x1C);
   UART_PrintChar(0x2E);  //??????
  }

void UART_Print_Char1(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x36);  //选择字符集1
 }  
void UART_Print_Char2(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x37);  //选择字符集2
 }    
//////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////  
static void  Printer_TITLE(void)
{
  Uint8  i;
	Uint16 tmp;
  Uint8  send_tmp[20];
	
  i = 0;
  /////////////////////////
  UART_PrintChar(0x1b);
  UART_PrintChar(0x56);  
  UART_PrintChar(0x02);  //横向纵向都放大 一倍
  /////////////////////////////////////////////
  Printer_EN_MODE();
 
	tmp = (model_id>>8)&0xffff;
	UART_PrintChar(tmp/100 +0x30);
  UART_PrintChar((tmp%100)/10 +0x30);
  UART_PrintChar('0');
  UART_PrintChar(3 + dot_position +0x30);	
 
	/*
  if(CHINESE == flag_language)
	 {
    Printer_CH_MODE();
    UART_PrintStr("分析天平");
   }
  else*/
	
  UART_PrintStr("ELECTRONIC BALANCE");
  Printer_enter();
  Printer_EN_MODE();
}

static void  Printer_TABLE(void)
{
  //Uint32 temp_tmp;
  //Uint8  send_tmp[20];
  /*///////////////////////////////////////???? 
  if(CHINESE == flag_language)
  {
   Printer_CH_MODE();
   UART_PrintStr("当前模式");
	 Printer_EN_MODE();
   send_tmp[0]  = ':';
   send_tmp[1]  = ' ';
	 Printer_CN_MODE();
	 switch(current_mode)
	   {
		  case STAT_PCS:         UART_PrintStr("计数");         break;
			case STAT_DENSITY:     UART_PrintStr("密度");         break;
			case STAT_CHECK:       UART_PrintStr("检重");         break;
			case STAT_100:         UART_PrintStr("百分比");       break;
			case STAT_ANIMAL:      UART_PrintStr("动物称重");     break;
			default:	             UART_PrintStr("称重");         break;
	   }		  
  
	} 
  else
	{*/
		UART_PrintStr("CURR_MODE: ");
   	switch(current_mode)
	   {
		  case STAT_PCS:         UART_PrintStr("COUNT_MODE");       break;
			case STAT_DENSITY:     UART_PrintStr("DENSITY_MODE");     break;
			case STAT_CHECK:       UART_PrintStr("CHECK_MODE");       break;
			case STAT_100:         UART_PrintStr("PERCENTAGE_MODE");  break;
			case STAT_ANIMAL:      UART_PrintStr("ANIMAL_MODE");      break;
			default:	             UART_PrintStr("WEIGHT");             break;
	   } 
  Printer_enter();
}

static void  Printer_SIGN(void)
{
  Uint8  send_tmp[20];
  Uint8  i;
  /*if(CHINESE == flag_language)
	{
   Printer_CH_MODE();
   UART_PrintStr("签名");
  }
  else*/
	UART_PrintStr("Signature:");
  
  //Printer_EN_MODE();
  //send_tmp[0]  = ':';
  for(i=1;i<20;i++)
  send_tmp[i]  = '_';
  send_tmp[15] = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter(); 
  Printer_enter();
  Printer_enter();
}

static void  Printer_data(void)
{
  Uint8  i;
  Uint8  send_tmp[20];
  i = 0;
  Printer_EN_MODE();
	UART_PrintStr("CURR_DATA: ");
  ///////////////////////////////////////////////////////////////
	if(FALSE == positive_flag)
  send_tmp[i++] = '-';
  else
  send_tmp[i++] = ' ';
  ///////////////////////////////////////////////////////////////
	switch(current_mode)
	{
		case STAT_PCS:      sprintf(&send_tmp[i],"%8d",   pcs_number);      break;
		case STAT_DENSITY:  sprintf(&send_tmp[i],"%8.1f", density_data);    break;
		case STAT_CHECK:    sprintf(&send_tmp[i],"%8.4f", net_weight);      break;
		case STAT_100:      sprintf(&send_tmp[i],"%8.2f", number_100);      break;
		case STAT_ANIMAL:   sprintf(&send_tmp[i],"%8.1f", animal_data);     break;
		default:            sprintf(&send_tmp[i],"%8.4f", net_weight);      break;	
	}
  i+=8;
  send_tmp[i++]  = 0x00;
  UART_PrintStr(send_tmp);
	//////////////////////////////////打印单位
	i = 0;
	switch(current_mode)
	{
		case STAT_PCS:      UART_PrintStr(" pcs");     break;
		case STAT_DENSITY:  UART_PrintStr(" g/m3");    break;
		case STAT_100:      UART_PrintStr(" % ");      break;
		case STAT_CHECK:    
		case STAT_ANIMAL:   
		default:            switch(current_unit)
		                     {
			                    case UNIT_MG: UART_PrintStr(" mg");  break;
													case UNIT_OZ: UART_PrintStr(" oz");  break;
                          case UNIT_CT: UART_PrintStr(" ct");  break;
                          default:      UART_PrintStr(" g ");  break;
                                        break;													 
												 }
			      break;	
	}
  Printer_enter();
}  


void  Printer_Init(void)
  {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x40);  //??????
    
    UART_PrintChar(0x1b);
    UART_PrintChar(0x63);
    UART_PrintChar(0x00);  //??????
    
    UART_Print_Char1();
  }   
    
	
	
void  UART_PrintData(void)
{
	Printer_Init();
  Printer_TITLE();
  Printer_TABLE();
  Printer_data(); 
  Printer_SIGN();
  print_busy = 0; 
 }
 

