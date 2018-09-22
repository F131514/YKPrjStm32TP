//ht1621_driver.c
#include "global.h"
#include "ht1621_driver.h"
#include "I2C.h"


 Uint8 const display_code[]=
                          {
                           SEG_A+SEG_B+SEG_C+SEG_D+SEG_E+SEG_F,      //0
                           SEG_B+SEG_C,                              //1  
                           SEG_A+SEG_B+SEG_D+SEG_E+SEG_G,            //2
                           SEG_A+SEG_B+SEG_C+SEG_D+SEG_G,            //3
                           SEG_B+SEG_C+SEG_F+SEG_G,                  //4
                           SEG_A+SEG_C+SEG_D+SEG_F+SEG_G,            //5
                           SEG_A+SEG_C+SEG_D+SEG_E+SEG_F+SEG_G,      //6
                           SEG_A+SEG_B+SEG_C,                        //7
                           SEG_A+SEG_B+SEG_C+SEG_D+SEG_E+SEG_F+SEG_G,//8
                           SEG_A+SEG_B+SEG_C+SEG_D+SEG_F+SEG_G,      //9
						               SEG_A+SEG_D+SEG_E+SEG_F,                  //C    0x0a
                           SEG_A+SEG_B+SEG_C+SEG_E+SEG_F+SEG_G,      //A	  0x0b
						               SEG_D+SEG_E+SEG_F,						             //L    0x0c
													 SEG_A+SEG_B+SEG_E+SEG_F+SEG_G,            //P	  0x0d
						               SEG_A+SEG_B+SEG_C+SEG_F+SEG_G,			       //Q    0x0e	
						               SEG_A+SEG_E+SEG_F+SEG_G,                  //F    0x0f
						               SEG_B+SEG_C+SEG_D+SEG_E+SEG_F,            //U    0x10
                           SEG_A+SEG_D+SEG_E+SEG_F+SEG_G,            //E    0x11                      
                           SEG_E+SEG_F+SEG_G,                        //r    0x12
													 SEG_G,                                    //-	  0x13
													 SEG_C+SEG_D+SEG_E+SEG_F+SEG_G,            //b..  0x14 
													 SEG_C+SEG_D+SEG_E+SEG_G,                  //o    0x15
                           SEG_D+SEG_E+SEG_F+SEG_G,                  //t  	0x16		
													 SEG_B+SEG_C+SEG_D+SEG_E+SEG_G,            //d  	0x17
													 SEG_A+SEG_B+SEG_C+SEG_E+SEG_F,            //N    0x18
                           0                                         //NULL 0x19
                          };
 Uint8 const display_code1[]=
                          {
                           SEG_A1+SEG_B1+SEG_C1+SEG_D1+SEG_E1+SEG_F1,        //0
                           SEG_B1+SEG_C1,                                    //1  
                           SEG_A1+SEG_B1+SEG_D1+SEG_E1+SEG_G1,               //2
                           SEG_A1+SEG_B1+SEG_C1+SEG_D1+SEG_G1,               //3
                           SEG_B1+SEG_C1+SEG_F1+SEG_G1,                      //4
                           SEG_A1+SEG_C1+SEG_D1+SEG_F1+SEG_G1,               //5
                           SEG_A1+SEG_C1+SEG_D1+SEG_E1+SEG_F1+SEG_G1,        //6
                           SEG_A1+SEG_B1+SEG_C1,                             //7
                           SEG_A1+SEG_B1+SEG_C1+SEG_D1+SEG_E1+SEG_F1+SEG_G1, //8
                           SEG_A1+SEG_B1+SEG_C1+SEG_D1+SEG_F1+SEG_G1,        //9 
													 SEG_A1+SEG_D1+SEG_E1+SEG_F1+SEG_G1,               //E    0x0a                       
                           SEG_E1+SEG_F1+SEG_G1,                             //r    0x0b	
                           0                               									 //     0x0c					
                          };													
/////////////////////////////////////////////////正常显示菜单
Uint8 const display_FULL_OVER[]  = {DISP_NULL,DISP_NULL,DISP_F,DISP_U,DISP_L,DISP_L,DISP_NULL}; //上溢出	
/////////////////////////////////////////////////校准显示菜单
Uint8 const display_CAL_INFO1[]  = {DISP_C,DISP_A,DISP_L,DISP_X,DISP_X, 0,DISP_X};   //校准
Uint8 const display_CAL_INFO2[]  = {DISP_C,DISP_A,DISP_L,DISP_X,     2, 0,     0};   //校准
Uint8 const display_CAL_INFO3[]  = {DISP_X,DISP_X,DISP_X,DISP_X, DISP_X, DISP_X,DISP_X};   //内校准

Uint8 const display_CALL_INFO[]  = {DISP_L,     1,DISP_N,DISP_X,     0,   0,   0};   //线性校准											
Uint8 const display_ERR_INFO[]   = {DISP_NULL,DISP_E,DISP_r,DISP_r,DISP_NULL,0,1};   //错误提示
//////////////////////////////////////////////////配置显示菜单
Uint8 const display_CFG_INFO[]   = {DISP_U, DISP_C,     0,1,DISP_NULL,       0,1}; //用户配置
Uint8 const display_F_CFG_INFO[] = {DISP_F, DISP_C,     0,1,DISP_NULL,       0,1}; //系统配置
Uint8 const display_F_CFG2_INFO[] = {DISP_r, DISP_r,    0,1,DISP_NULL,DISP_NULL,1}; //系统配置2
//////////////////////////////////////////////////配置启动菜单
Uint8 const display_BOOT_INFO[]  = {DISP_b, DISP_o,DISP_o,DISP_t,DISP_NULL,DISP_NULL,DISP_NULL};
Uint8 const display_MODEL_INFO[] = {DISP_NULL, DISP_X, 0, 0, 0, 0,  DISP_X};
////////////////////////////////PCS 采集阶段显示
Uint8 const display_PCS_INFO[]   = {DISP_P, DISP_C,     5,DISP_X,   0,  0,  0};
//DENSITY MODE 
Uint8 const display_DEN_MODE[4][7] =
{
 {DISP_NULL,      5,       0, DISP_L,         1,   DISP_d, DISP_NULL},
 {DISP_NULL, DISP_L,       1, DISP_Q,    DISP_U,        1, DISP_d},
 {        5, DISP_X,       5, DISP_d, DISP_NULL,        0, 4},
 {        5, DISP_X,  DISP_L, DISP_d, DISP_NULL,        0, 4}
};
//DENSITY STEP
Uint8 const display_DENSITY_STEP1[]  = {     5, DISP_o,  DISP_L,      1, DISP_d,   DISP_X,  1}; //固体
Uint8 const display_DENSITY_STEP2[]  = {DISP_L,      1,  DISP_Q, DISP_U, DISP_d,   DISP_X,  1}; //液体

//ANIMAL TIMESTEP
Uint8 const display_ANIMAL_TIME[]    = {DISP_NULL,      5, DISP_E,DISP_L,DISP_X,0,  0}; //SEL-10 
//
Uint8 const display_REC_ERROR[]={DISP_NULL, DISP_NULL,DISP_E,DISP_r,DISP_r,DISP_NULL,DISP_NULL}; //Err 
///////////////////////////////////
//ram0   ram1   ram2   ram3   ram4   ram5   
//flag    万     千     百     十     个    
//
////////////////////////////////////////////
/* 1 delay */
////////////////////////////////////////////
static void delay(Uint32 length)
	{
	  while (length >0)
    	         length--;
	 }

////////////////////////////////
////////////////////////////
//HT1621 管脚初始化
///////////////////////////

//-----------------------------------------------------------------------------   
//传送Length位数据给1621   
static void Send_LBit(Uint8 Sdata,Uint8 Length)   
{   
    Uint8 i=Length;   
    while(i--)   
    {   
        WRL;      //nWR=0;   
        if(Sdata&0x80)   
            DATAH;//DATA=1;   
        else   
            DATAL;//DATA=0;   
        Sdata<<=1;   
        WRH;      //nWR=1;   
        delay(1);   
    }      
} 
//-----------------------------------------------------------------------------   
//送控制指令   
static void Send_Com(Uint8 con,Uint8 sdata)   
{   
    CSL;//nCS=0;   
    Send_LBit(con,3);   
    Send_LBit(sdata,8);   
    Send_LBit(0x00,2);   
    CSH;//nCS=1;   
}   
//-----------------------------------------------------------------------------   
//初始化   
void Init_1621(void)   
{   
    Uint8 i;
    //管脚配置
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStruct.GPIO_Pin = HT1621_CS | HT1621_CLK | HT1621_DATA ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init( HT1621_PORT, &GPIO_InitStruct);
	GPIO_SetBits(HT1621_PORT, HT1621_CS | HT1621_CLK | HT1621_DATA);
	
	
		 
    Send_Com(COMMAND,SYS_EN);   
    Send_Com(COMMAND,Rosc);   
    Send_Com(COMMAND,BIS1_2);   
    //Send_Com(COMMAND,NORMAL);        
    Send_Com(COMMAND,LCD_ON);   
    
    for(i=0;i<6;i++)
      display_buffer[i] = 0;
} 
/////////////////////////////////////////////////////

void  Update_Display(void)
   {
    Uint8  i;

    CSL;
    Send_LBit(WRITE,3);
    Send_LBit(ADDR_OFFSET,6);                  //地址  
    for(i=0;i<28;i++)
    {
     if(i%2 == 0)
     Send_LBit(display_buffer[i>>1]&0xf0,4);      //先送高4位
     else
     Send_LBit((display_buffer[i>>1]<<4)&0xf0,4); //再送低4位  
    }
    CSH;
   }

//显示某个数字
void Displaybuf_All_off(void)   
{   
    Uint8 i;   
    for(i=0;i<14;i++)     
        display_buffer[i]=0x00;
}

void Displaybuf_All_on(void)   
{   
    Uint8 i;   
    for(i=0;i<14;i++)     
        display_buffer[i]=0xff;
}  


/////////////////////////////////////////////////////////////////////
//
//以下为增加的显示相关的函数
/////////////////////////////////////////////////////////////////////
void Display_Boot_Info(void)
 {
    //
	  Uint8 i;
		for(i=1;i<8;i++)	 
    display_buffer[i] =  display_code[display_BOOT_INFO[i-1]];
 	  for(i=4;i<7;i++)	 
	  display_buffer[i] |= SEG_P;
 }

void Display_Model_Info(void)
 {
    //
	  Uint8    i;
	  Uint16 tmp; 
	  tmp = (model_id>>8)&0xffff;
	 
		for(i=1;i<8;i++)	 
    display_buffer[i] =  display_code[display_MODEL_INFO[i-1]];
 	  	 
	  display_buffer[3] =  display_code[tmp/100];
	  display_buffer[4] =  display_code[(tmp%100)/10];
	  display_buffer[6] =  display_code[3 + dot_position];
 }
  
//////////////////////////////////////////////////////////////////////
void update_area1_buf(Uint8 *buf,Uint8 dot)
{
  	Uint8  i;	//////////////////////传入缓存
	  Uint8  *p;
	  p = buf;
		for(i=1;i<8;i++)	 
      {
			 display_buffer[i] =  display_code[*p-0x30];
       p++;
	    }
    /////////////////
	  for(i=1;i<dot;i++)
	  {  
		 if(display_buffer[i] == display_code[0])
		   display_buffer[i]  = display_code[DISP_NULL];
		 else
			 break;
		}		 
		if(7 != dot)
		display_buffer[dot] |= SEG_P; 
    //////////////////////////////////////////////
		//消隐条件： 称重 检重 动物
		if(STAT_DISP_REC==current_mode)
		  {
				if((TRUE==REC_flag)&&(UNIT_MG==REC_current_unit)&&(STAT_PCS!=REC_current_mode)&&(STAT_100!=REC_current_mode))//pcs % 两种模式不显示 m
	   	  	display_buffer[7] |= FLAG_m_7;
		  }
		else if(del_last_one&0x01)
		  {	
		   if((STAT_WEIGH==current_mode)||(STAT_WEIGH==current_mode)||(STAT_WEIGH==current_mode))
			    { 
				   display_buffer[7] = display_code[DISP_NULL];
           if(UNIT_MG == current_unit)
           display_buffer[6] &= (~SEG_P); 
		      }
		 }				
}	
////////////////////////////////////////////////////////////////////////
//当显示记录数据时候，也调用此函数，所以采用参数传递的方式
////////////////////////////////////////////////////////////////////////
void  Display_weight_Data(float w_data,Uint8 w_unit)
			 {
				  Uint8  i,dot_pos,tmp[10];
				  float  x;
                switch(w_unit)     
                 			 {
												 case UNIT_MG: 
													             x = w_data*G_TO_MG;   
												               sprintf(tmp,"%08.1f", x);
												               dot_pos = 6;
												               tmp[6] = tmp[7];
												               break;
												 case UNIT_OZ: 
													             x = w_data*G_TO_OZ; 
												               sprintf(tmp,"%08.6f", x);
												               dot_pos = 1;
												               for(i=1;i<7;i++)
												                 tmp[i] = tmp[i+1];
												               break;		
												 case UNIT_OZT: 
													             x = w_data*G_TO_OZT; 
												               sprintf(tmp,"%08.6f", x);
												               dot_pos = 1;
												               for(i=1;i<7;i++)
												                 tmp[i] = tmp[i+1];
												               break;		
												 
											   default:	     
													             if(w_unit == UNIT_CT)
																				 x = w_data*G_TO_CT;
													             else
																			   x = w_data;     
        												       sprintf(tmp,"%08.4f", x);
												               dot_pos = 3;
												               for(i=3;i<7;i++)
												               tmp[i] = tmp[i+1];
												               break;												 
											 }
			     update_area1_buf(tmp,dot_pos);
			}
			 
void  Display_weight_Data_Special(float w_data,Uint8 w_unit)
			 {
				  Uint8  i,dot_pos,tmp[10];
				  float  x;
                switch(w_unit)     
                 			 {
												 case UNIT_MG: 
													             x = w_data*G_TO_MG;   
												               sprintf(tmp,"%08.1f", x);
												               dot_pos = 6;
												               tmp[6] = tmp[7];
												               //
												               dot_pos = 7;
												               for(i=6;i!=0;i--)
												                  tmp[i] = tmp[i-1];
												               tmp[0] = 0x30;
												               break;
											   case UNIT_G:	     
													             if(w_unit == UNIT_CT)
																				 x = w_data*G_TO_CT;
													             else
																			   x = w_data;     
        												       sprintf(tmp,"%08.4f", x);
												               dot_pos = 3;
												               for(i=3;i<7;i++)
												               tmp[i] = tmp[i+1];
																			 //
																			 dot_pos =4;
																			 for(i=6;i!=0;i--)
												                  tmp[i] = tmp[i-1];
												               tmp[0] = 0x30;
												               break;	
												 default:break;						 
											 }
											 
			     update_area1_buf(tmp,dot_pos);
			}			
/////////////////////////////////////////////////////////////////////////
void  Display_Prehot_time(void)
     {	
  	   display_buffer[1]  =  display_code[DISP_NULL]; 
       display_buffer[2]  =  display_code[DISP_NULL];
		   display_buffer[3]  =  display_code[prehot_time/600];
		   display_buffer[4]  =  display_code[(prehot_time/60)%10] | SEG_P; 
       display_buffer[5]  =  display_code[(prehot_time%60)/10];
		   display_buffer[6]  =  display_code[(prehot_time%60)%10];
		   display_buffer[7]  =  display_code[DISP_NULL]; 
			 Update_Display();
      }		
/////////////////////////////////////////////////////////////////////////			
void  Display_Inner_code(void)
     {	
			 Uint8  tmp[10];
			 Uint32 tmp1,tmp2;
  		 tmp1 = Get_Filter_Data();
		   ////////////////////////////////
		   display_buffer[11] =  display_code1[DISP1_NULL]; 
       display_buffer[10] =  display_code1[DISP1_NULL];
		   display_buffer[9]  =  display_code1[DISP1_NULL];
			 if(tmp1 > 9999999)
			 {
				 //借用温度显示的一位
				 tmp2 = tmp1/10;
				 display_buffer[9] = display_code1[tmp1%10];
			 }
       else
        tmp2 = tmp1;
			  //显示高7位
  		  sprintf(tmp,"%07d", tmp2);
			  update_area1_buf(tmp,7); 
		 }			
////////////////////////////////////////////////////////////////////////
void  Display_density_mode(void)
{
	Uint8 i;
  for(i=1;i<8;i++)
	display_buffer[i] = display_code[display_DEN_MODE[density_mode_index-1][i-1]];
	if(3==density_mode_index)
	display_buffer[7] = display_code[solid_sel_index];
	if(4==density_mode_index)
	display_buffer[7] = display_code[liquid_sel_index];
}
////////////////////////////////////////////////////////////////////////
void  Display_animal_time(void)
{
  Uint8 i;
  for(i=1;i<8;i++)
	display_buffer[i] = display_code[display_ANIMAL_TIME[i-1]];
  switch(animal_time_index)
  {
		case 1:display_buffer[6]=display_code[1];display_buffer[7]=display_code[0];break;
	  case 2:display_buffer[6]=display_code[1];display_buffer[7]=display_code[5];break;
	  case 3:display_buffer[6]=display_code[2];display_buffer[7]=display_code[0];break;
		case 4:display_buffer[6]=display_code[3];display_buffer[7]=display_code[0];break;
	  default:break;
	}		
}
////////////////////////////////////////////////////////////////////////
//考虑两次显示 步骤
const Uint8 display_step[] = {0,1,1,2,2,3,3,4,4};
void  Display_density_step(void)
{
 Uint8 i;
 if(0x01 == density_mode_index)	
  {//固体密度模式
    for(i=1;i<8;i++)
		display_buffer[i] = display_code[display_DENSITY_STEP1[i-1]];
	  display_buffer[7] = display_code[display_step[density_step]];
  }
 else if(0x02 == density_mode_index)
	{//液体密度模式
    for(i=1;i<8;i++)
		display_buffer[i] = display_code[display_DENSITY_STEP2[i-1]];
	  display_buffer[7] = display_code[display_step[density_step]];	
	}
 else if(0x03 == density_mode_index)
  {
  }
 else 
  {
	}	 
}
////////////////////////////////////////////////////////////////////////

void Display_normal_main(void)
 {
	 static Uint8  cycle_times = 0;
	 static Uint8  animal_ok   = 0;
	        Uint8  i,tmp[10];
	  //
	  if(FALSE == point2_cal_ok)
		  {
			 Display_Inner_code();
		  }
		else 
		  { 
	     switch(current_mode)
		     {
			    case STAT_WEIGH: 
                          //net_weight = 123.4567;
					                if((disdiv_para&0x01)&&((UNIT_MG==current_unit)||(UNIT_G==current_unit))&&(net_weight>(full_range/2)))
													Display_weight_Data_Special(net_weight,current_unit);
													else	
					                Display_weight_Data(net_weight,current_unit);
		  							  	  break;			
	  	    case STAT_PCS:
						            if(TRUE == sample_pcs_enter) //采集样品数目阶段
												   {
														for(i=1;i<8;i++)
														display_buffer[i] = display_code[display_PCS_INFO[i-1]]; 
														switch(sample_pcs_index)
														{
															case 0:                                     display_buffer[7] = display_code[5];break;
															case 1: display_buffer[6] = display_code[0];display_buffer[7] = display_code[5];break;
                              case 2: display_buffer[6] = display_code[1];display_buffer[7] = display_code[0];break;
															case 3: display_buffer[6] = display_code[2];display_buffer[7] = display_code[0];break;
															case 4: display_buffer[6] = display_code[5];display_buffer[7] = display_code[0];break;
															case 5: display_buffer[5] = display_code[1];
																      display_buffer[6] = display_code[0];display_buffer[7] = display_code[0];break;
                          		default:break;	
														}															
													 }
												else //认为 只要 weight_per_pcsa 该变量在范围内就显示PCS
											   	 {
														if((weight_per_pcs>WPP_MIN)&&(weight_per_pcs<WPP_MAX))
														  {
															 sample_pcs_ok  = TRUE;	
															 pcs_number = net_weight/weight_per_pcs + PCS_ADJUST_COEF; //????
														   if(pcs_number > PCS_MAX)
												         {
												          for(i=1;i<8;i++) 
												       	   display_buffer[i] = display_code[display_FULL_OVER[i-1]];
												         }
											         else
														     { 
															    sprintf(tmp,"%07d", pcs_number);
												          update_area1_buf(tmp,7);    //只保留一个0 即可	
															   }	
															}
														else	
														  {
														   sample_pcs_ok  = FALSE;	
														   Display_weight_Data(net_weight,current_unit);
													    }
													 }
                          break;
	  	   case STAT_CHECK://根据参数 确定是否检重 周期 100ms 
						              check_flag = TRUE;
				                  cycle_times++; 
				                  if(cycle_times > 6)
														 cycle_times = 0;
					                if((limit1_weight>CHECK_LIMIT_MAX)||(limit1_weight<CHECK_LIMIT_MIN))
														 check_flag = FALSE;
												  if((limit2_weight>CHECK_LIMIT_MAX)||(limit2_weight<CHECK_LIMIT_MIN))
														 check_flag = FALSE;
												  if(limit1_weight>limit2_weight)
														 check_flag = FALSE;
													
						              if((net_weight<limit1_weight)||(net_weight>limit2_weight))
													   {
														  check_valid = TRUE;
														  Speaker_Time = 10;   //蜂鸣器叫
														 } 
													else
													    check_valid = FALSE;
													if((TRUE==check_flag)&&(TRUE==check_valid)&&(cycle_times>3))
													    {//消隐藏
                               for(i=1;i<8;i++)
                							 display_buffer[i] = display_code[DISP_NULL];									
															} 
                       	  else
												       Display_weight_Data(net_weight,current_unit); //正常显示重量
													break;
          case STAT_100:
						              //if((weight_compare_100>COMPARE_100_MIN)&&(weight_compare_100<COMPARE_100_MAX))
                          if(weight_compare_100>COMPARE_100_MIN)													
													{
																compare_100_flag = TRUE;
																number_100 = (net_weight/weight_compare_100)*100; 
															  sprintf(tmp,"%08.2f", number_100);
																tmp[5] = tmp[6];
																tmp[6] = tmp[7];
																update_area1_buf(tmp,5);  //display xxxxx.xx
															 }
													else	
													     {
                                compare_100_flag = FALSE;																 
						                    Display_weight_Data(net_weight,current_unit);
					                     }
 												  break;
			    case STAT_DENSITY:
						                 if(0 == density_mode_index)	    //还没有选择 密度测量模式					
                        	   Display_weight_Data(net_weight,current_unit);
                             else if((density_mode_index<5)&&(0==density_step))	//正在选择 密度测量模式
														 {
															 cycle_times = 0;
														   Display_density_mode();
														 }
														 else if(0!=density_step)	//进入正常测量流程														 
														 {//在采集重量阶段
															 if(6==density_step) //采集完毕
															   {
																	 sprintf(tmp,"%08.3f",density_data);
																	 tmp[4] = tmp[5];
																	 tmp[5] = tmp[6];
																   tmp[6] = tmp[7];
												           update_area1_buf(tmp,4);
																 }	 
															 else if((1==density_step)||(3==density_step)||(5==density_step))
															  {
																 cycle_times++;
																 if(20 == cycle_times)
																   {cycle_times = 0;
																		density_step++;
																	 }
																 else
																   Display_density_step();
														   }
															 else
																   Display_weight_Data(net_weight,current_unit);
													   }	 
														 else{;}
                              break;														 
          case STAT_ANIMAL:		
						                 if(0 == animal_time_index) //还没有选择 时间
                         	     Display_weight_Data(net_weight,current_unit);
														 else if((0!=animal_time_index)&&(FALSE==animal_start))
														 {
														   Display_animal_time();
															 animal_ok = FALSE;
														 }
														 else if(TRUE == animal_start)	//进入正常测量流程														 
														 {
															 //动物模式开启后 在设定时间内正常显示，时间到后 显示稳定值
														   if((0==animal_run_time)&&(TRUE==stable_flag)&&(FALSE==animal_ok))
															     {
																		 animal_data = net_weight;
																		 animal_ok = TRUE;
																	 } 
															 ////////////////////////////////保存
															 if(TRUE == animal_ok)
															    Display_weight_Data(animal_data,current_unit);
															 else
																  Display_weight_Data(net_weight,current_unit);
													   }
														 else{;} 
			    default:        break;	
		     }
			 }
		//////////
		if(((STAT_CONFIG==current_mode)||(STAT_WEIGH==current_mode))&&(UNIT_MG == current_unit)) //只有在重量模式下或者配置模式下且单位为毫克时候才显示
			display_buffer[7] |=  FLAG_m_7; 
 }
 
void Display_CALL_Pro(void)
 {//100ms * 20 = 2s
	Uint8    i;
	Uint16 tmp; 
 	 for(i=1;i<8;i++)
 	   display_buffer[i] = display_code[display_CALL_INFO[i-1]]; //载入标准格式 
	 
	 if(11 == point10_cal_step)
		 {display_buffer[5]=display_code[8];display_buffer[6]=display_code[8];display_buffer[7]=display_code[8];}
   else	if(12 == point10_cal_step)
		 {display_buffer[5]=display_code[9];display_buffer[6]=display_code[9];display_buffer[7]=display_code[9];}
   else		
	   {
      tmp = line_cal_para*point10_cal_step/10;			 
      display_buffer[5]=display_code[tmp/100];
      display_buffer[6]=display_code[(tmp%100)/10];
	    display_buffer[7]=display_code[tmp%10];
     }
		/*
	 if(10 == point10_cal_step)
	   {display_buffer[6]=display_code[1];display_buffer[7]=display_code[0];}
	 else if(11 == point10_cal_step)
		 {display_buffer[6]=display_code[1];display_buffer[7]=display_code[1];}
   else	if(12 == point10_cal_step)
		 {display_buffer[6]=display_code[8];display_buffer[7]=display_code[8];}
   else		 
	    display_buffer[7] = display_code[point10_cal_step];
  */  		 
 }	

////////////////////////////////////////////////
void Display_CAL_Pro(void)
{ //100ms * 20 = 2s
  static Uint8 flash_status;
	static Uint8 cycle_time1 = 0;
	static Uint8 cycle_time2 = 0;
	static Uint8 cycle_time3 = 0;
	Uint8 i;
  
	if(USER_CAL_ZER0_FLASH == point2_cal_step)
	  {
		 cycle_time2 = 0;	
		 cycle_time1++;
     if(5 == cycle_time1)
		  {
		   cycle_time1 = 0;
			 flash_status++;
			 if(flash_status&0x01)
			  { 
			  for(i=1;i<8;i++)
 	        display_buffer[i] = display_code[display_CAL_INFO1[i-1]]; //载入标准格式 
		    }
			 else
			  { 
			  for(i=1;i<8;i++)
 	        display_buffer[i] = 0; //载入标准格式 
		    }
		  } 
	  }
	else if(USER_CAL_FULL_FLASH == point2_cal_step)
	  {
		 cycle_time1 = 0;	
		 cycle_time2++;
     if(5 == cycle_time2)
		  {
		   cycle_time2 = 0;
			 flash_status++;
			 if(flash_status&0x01)
			  { 
			  for(i=1;i<8;i++)
 	        display_buffer[i] = display_code[display_CAL_INFO2[i-1]]; //载入标准格式 CAL-200
				 
					display_buffer[5]=display_code[ user_cal_para/100];
		      display_buffer[6]=display_code[(user_cal_para%100)/10];
          display_buffer[7]=display_code[ user_cal_para%10];				
		    }
			 else
			  { 
			  for(i=1;i<8;i++)
 	        display_buffer[i] = 0; 
		    }
		  } 	
		}
	else if(USER_CAL_FULL==point2_cal_step)
	  {
		 cycle_time2 = 0;
		 cycle_time3 = 0;
		 for(i=1;i<8;i++)
 	        display_buffer[i] = display_code[display_CAL_INFO2[i-1]]; //载入标准格式 CAL-200
				 
					display_buffer[5]=display_code[ user_cal_para/100];
		      display_buffer[6]=display_code[(user_cal_para%100)/10];
          display_buffer[7]=display_code[ user_cal_para%10];				 
		}			
	else if(USER_CAL_OK_FLAG == point2_cal_step)
	  {
		  cycle_time3++;  //
		  if(20 == cycle_time3)
			  {
					cycle_time3 = 0;
				  point2_cal_step++;
				}	
		}		
	else if(USER_CAL_OVER == point2_cal_step)
	  {
		 ;
		}  		
	else
	  {
		 ;
	  }		
}	
 //////////////////////////////////////////////////
void Display_CAL_INNER_Pro(void)
{
     //100ms * 20 = 2s
     static Uint8 flash_status;
	   static Uint8 cycle_time1 = 0;
	   Uint8 i;
 
		 cycle_time1++;
	  
     if(cycle_time1 > 5)
		 {
			 cycle_time1 = 0;
			 flash_status++;
			 if(flash_status&0x01)
			  { 
			  for(i=1;i<8;i++)
 	        display_buffer[i] = display_code[display_CAL_INFO3[i-1]]; //载入标准格式 
		    }
			 else
			  { 
			  for(i=1;i<8;i++)
 	        display_buffer[i] = 0; //载入标准格式 
		    }
		 }
}

void Display_REC_Pro(void)
{
 //根据存储的模式 及数据 显示
  static Uint8  tmp=0xff;
	un_rec_data   rec_data;
        Uint8   i,check_data,buf[16];
 if(disp_rec_index != tmp)
     {
		   tmp = disp_rec_index;
			 for(i=0;i<16;i++)
			 buf[i] = 0x00;
		   //////////////////////////read & display
	     Read_EEPROM(EEP_REC_START_ADDR+(tmp-1)*EEP_BYTE_PER_REC,buf, 16);	 
	     for(i=0;i<200;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += buf[i];
				 
       if((buf[0]==CHECK_DATA)&&(buf[1]==CHECK_DATA)&&(buf[15]==check_data))
       { //合法数据	
				 REC_flag          = TRUE;
				 REC_current_mode  = buf[7]; //存放称量模式
				 REC_current_unit  = buf[8];
		     rec_data.rec_uint32=(buf[9]<<24)+(buf[10]<<16)+(buf[11]<<8)+buf[12];
				 if(STAT_100 == REC_current_mode)
				   {
						sprintf(buf,"%08.2f", rec_data.rec_float);
					  buf[5] = buf[6];
						buf[6] = buf[7];
						update_area1_buf(buf,5);
					 }						 
				 else if(STAT_PCS == REC_current_mode)
					 {
						sprintf(buf,"%07d", rec_data.rec_uint32);
	          update_area1_buf(buf,7);    //只保留一个0 即可	
					 }	 
         else 					 
				 Display_weight_Data(rec_data.rec_float,REC_current_unit);
			 }
       else
       {//非法数据
				REC_flag = FALSE;
				for(i=1;i<8;i++)
         display_buffer[i] = display_code[display_REC_ERROR[i-1]]; 
       }				 
		 }
}
////////////////////////////////////////////////
void Display_CFG_Pro(void)
{
	Uint8 i;
	for(i=1;i<8;i++)
 	 display_buffer[i] = display_code[display_CFG_INFO[i-1]]; //载入标准格式 
  switch(current_menu)
	{
	  case MENU_CONFIG_MODE:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[1];
			                     switch(current_mode_bak)
													 {
														 case STAT_WEIGH:   display_buffer[7] = display_code[1];  break;
														 case STAT_PCS:     display_buffer[7] = display_code[2];  break;
                             case STAT_DENSITY: display_buffer[7] = display_code[3];  break;
														 case STAT_CHECK:   display_buffer[7] = display_code[4];  break;
                             case STAT_100:     display_buffer[7] = display_code[5];  break;
														 case STAT_ANIMAL:  display_buffer[7] = display_code[6];  break;
                           
														 default:     display_buffer[6] = display_code[0x0f];
															            display_buffer[7] = display_code[0x10];break;
													 }  
													 
                      		 break;
   		case MENU_CONFIG_UNIT:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[2];
		                       switch(current_unit)
													 {
														 case UNIT_G:  display_buffer[7] = display_code[1];   break;
														 case UNIT_MG: display_buffer[7] = display_code[2] | FLAG_m_7;   break;
                             case UNIT_CT: display_buffer[7] = display_code[3];   break;
                             case UNIT_OZ: display_buffer[7] = display_code[4];   break;
														 case UNIT_OZT:display_buffer[7] = display_code[5];   break;
														 default:      display_buffer[6] = display_code[0x0f];
															             display_buffer[7] = display_code[0x10];break;
													 }
													 ////////////////////////////////////////////
													 break;										 
   	  case MENU_CONFIG_TEMP:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[3];
			                     if(temp_unit&0x01)
														  display_buffer[7] = display_code[2];
													 else
													    display_buffer[7] = display_code[1]; 
													 break;
	  	case MENU_CONFIG_DIV:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[4];
			                     if(del_last_one&0x01)
														  display_buffer[7] = display_code[2];
													 else
													    display_buffer[7] = display_code[1]; 
													 break;
													 
		  case MENU_CONFIG_BAUD:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[5];
			                     //
			                     display_buffer[6] = display_code[uart_format_type/10];
													 display_buffer[7] = display_code[uart_format_type%10]; 
													 break;													 
	  	case MENU_CONFIG_DEL:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[6];
			                     
													 display_buffer[6] = display_code[4];
													 display_buffer[7] = display_code[4]; 
													 break;
			case MENU_CONFIG_DEF:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[7];
			                     //
			                     display_buffer[6] = display_code[6];
													 display_buffer[7] = display_code[6]; 
													 break;	
			case MENU_CONFIG_COMM:
			                     display_buffer[3] = display_code[0];display_buffer[4] = display_code[8];
			                     //
			                     display_buffer[6] = display_code[0];
													 display_buffer[7] = display_code[Uart_Printer_Flag]; 
													 break;	
      default:
			                     break;
                           			
	}	
}
////////////////////////////////////////////////
void Ht1621_Display_area1(void)	//主显示
   {
		Uint8 i; 
		switch(current_mode)
		{
			case STAT_CONFIG:      Display_CFG_Pro();        break;
			case STAT_CAL_LINE:    Display_CALL_Pro();       break;
			
			case STAT_CAL:         Display_CAL_Pro();        break;
			case STAT_CAL_INNER:   Display_CAL_INNER_Pro();  break;
			case STAT_DISP_REC:    Display_REC_Pro();        break;
			default:
 		           if(TRUE == power_on_flag) //开机不稳定阶段
							    Display_Boot_Info();
		           else	if(TRUE == full_flag)
							  {
								 for(i=1;i<8;i++)	 
                  display_buffer[i] = display_code[display_FULL_OVER[i-1]]; 
								 if(UNIT_MG == current_unit)	
									 display_buffer[7] |= FLAG_m_7;
								}
							 else	
  		            Display_normal_main();	
               break;							 
	 }
  }
void Ht1621_Display_area2(void)	//次显示
   {
   	Uint8 tmp[8],temp_dot;
		float tmp1;
		temp_dot = 2; //默认为 
		///////////////////////////////////////开机未稳定阶段不显示 
		if((TRUE == power_on_flag)||(STAT_CAL_LINE==current_mode))
		  {
			 return;	
			 /*
			 tmp[0] = DISP1_NULL + 0x30;
		 	 tmp[1] = DISP1_NULL + 0x30;
       tmp[2] = DISP1_NULL + 0x30;
   		 temp_dot = 0;*/		
		  }
		/////////////////////////////////////////////////////////	
		if(TRUE == display_flag_mp)
		{
		 //当前总记录数
			sprintf(tmp,"%03d", (Uint32)rec_total_num);
 			temp_dot = 0;	
		}			
		else if(STAT_DISP_REC == current_mode)
		 {
		  //当前索引号
			sprintf(tmp,"%03d", disp_rec_index);
 			temp_dot = 0;	 
		 }			
    else
		 {//正常温度显示	
			 if((temp_float>TEMP_MIN_RANGE)&&(temp_float<TEMP_MAX_RANGE))	
			   {
					temp_error_flag  = FALSE;
			    if(TEMP_UNIT_OC == (temp_unit&0x01))
					   {
							sprintf(tmp,"%4.1f", temp_float); 
					 	  tmp[2] = tmp[3];
						 }	 
					else
					 {
						 tmp1 = temp_float*9/5+32.5;
						 if(tmp1 > 99.999)
						   {
								sprintf(tmp,"%03d", (Uint32)tmp1);
								temp_dot = 0;
							 }	 
						 else
						 	 {
								sprintf(tmp,"%4.1f", tmp1);
						    tmp[2] = tmp[3];
							 }	 
					 }
        }
        else
        {
					temp_error_flag  = TRUE;
					tmp[0] = DISP1_E + 0x30;
		 	    tmp[1] = DISP1_r + 0x30;
          tmp[2] = DISP1_r + 0x30;
   		    temp_dot = 0;
				}					
      }			
    if(0x20 == tmp[0])
			display_buffer[11] =  display_code1[DISP1_NULL];
		else
		  display_buffer[11] =  display_code1[tmp[0]-0x30];
		
    display_buffer[10] =  display_code1[tmp[1]-0x30];
		display_buffer[9]  =  display_code1[tmp[2]-0x30];
		if(2 == temp_dot)
			 display_buffer[10] |= SEG_P1;
		///温度单位
		//1保存记录成功后需要短暂熄灭
		//2在查看已保存记录时候需要熄灭
		if((TEMP_UNIT_OC==(temp_unit&0x01))&&(FALSE==display_flag_mp)&&(STAT_DISP_REC!=current_mode)) 
	 		 display_buffer[9]  |= FLAG_oC_9;	
   }		 
//////////////////////////////////////////////////////////////////////////////////////	 
//
///////////////////////
	void   Display_weight_unit(Uint8 tmp)
		          {
							 switch(tmp)	
			            { 
									 case UNIT_MG:	
				           case UNIT_G :  display_buffer[8] |= FLAG_g_8;            break;
									 case UNIT_CT:  display_buffer[8] |= FLAG_ct_8;           break;
                   case UNIT_OZ:  display_buffer[8] |= FLAG_oz_8;           break;
									 case UNIT_OZT: display_buffer[8] |= (FLAG_oz_8+FLAG_t_8);break;
                   
                   default:break;						
			            }
			         }	 
//	 
void Ht1621_Display_area3(void)              	//符号位显示
   { //300ms * 6 = 1.8s 
		static Uint8 cycle_times=0; 
   	Uint8 tmp;
	  if(TRUE == power_on_flag)
       return;
		///////////////////////////////////////////////////////
		display_buffer[0]  = 0;
	  display_buffer[0] |= FLAG_FENGE_0; 	
		if(1 == (flag_LOGO&0x01))
    display_buffer[0] |= FLAG_LOGO_0;
		if(TRUE == stable_flag)
		 display_buffer[0] |= FLAG_WENDING_0;
		
		if(STAT_CAL_LINE==current_mode)
		 {
			 display_buffer[8] |= FLAG_g_8;
			 return;
		 }
	  if(TRUE == temp_error_flag)
		display_buffer[0] |= FLAG_WENDU_0;
		//
		if(STAT_CAL_INNER == current_mode)
		 display_buffer[0] |= FLAG_NEIJIAO_0;
		if(STAT_CAL == current_mode)
		 display_buffer[0] |= FLAG_WAIJIAO_0;
		
		////////////////////////////////////////////////////三种情况下此标识灯亮
		if(USER_CAL_OK_FLAG == point2_cal_step)
		 display_buffer[0] |= FLAG_CAL_OK_0;
		//当用户配置删除数据 和 恢复出厂设置成功后 ，借用此标志位显示
		if((STAT_CONFIG==current_mode)&&(MENU_CONFIG_DEL==current_menu)&&(1==done_ok_flag))
		   display_buffer[0] |= FLAG_CAL_OK_0;
		if((STAT_CONFIG==current_mode)&&(MENU_CONFIG_DEF==current_menu)&&(1==done_ok_flag))	
    	 display_buffer[0] |= FLAG_CAL_OK_0;
		
		if(USER_CAL_ER == point2_cal_step)
		 display_buffer[0] |= FLAG_CAL_ER_0;
		
		#if(INNER_CAL_VERSION)
		if(TRUE == motor_err_flag)
		 display_buffer[0] |= (FLAG_NEIJIAO_0 + FLAG_CAL_ER_0);
		#endif
		
    ///////////////////////////////////////////////////////
		display_buffer[8] = 0;
		///温度单位
		//1保存记录成功后需要短暂熄灭
		//2在查看已保存记录时候需要熄灭
		if((TEMP_UNIT_OF==(temp_unit&0x01))&&(FALSE==display_flag_mp)&&(STAT_DISP_REC!=current_mode)) 
			 display_buffer[8] |= FLAG_oF_8;
		
		if((STAT_DISP_REC==current_mode)&&(TRUE==REC_flag))
		{//显示数据记录时候 的 单位处理
		       switch(REC_current_mode)
					     {
						    case STAT_WEIGH:
								case STAT_CHECK:	
			          case STAT_ANIMAL:	 
								   			          Display_weight_unit(REC_current_unit); break;	        	
 		            case STAT_PCS:    display_buffer[8] |= FLAG_pcs_8;       break;
                case STAT_100:		display_buffer[8] |= FLAG_percent_8;	 break;					
								case STAT_DENSITY: 
								default:	        break;
							 }		        
	
		}
    else
		{	
		switch(current_mode)
		{
			case STAT_CONFIG:
				               if(MENU_CONFIG_UNIT == current_menu)
													  Display_weight_unit(current_unit);
												break;
		  
		  case STAT_WEIGH: 
      case STAT_CHECK:	 
			case STAT_ANIMAL:
                 				Display_weight_unit(current_unit);  
			                  break;							
			case STAT_PCS: //只有在单件重有效的情况下 才显示PCS
				             if(TRUE == sample_pcs_ok)
				                display_buffer[8] |= FLAG_pcs_8;
                     else
											  Display_weight_unit(current_unit);
										    break;
  		case STAT_100: //只有在基准点有效的情况下 才显示百分比符号
				             if(TRUE == compare_100_flag)
				                display_buffer[8] |= FLAG_percent_8;
										 else
											  Display_weight_unit(current_unit);
			                  break;	
  		case STAT_DENSITY: //
				              if(6 == density_step)//当得到密度数据时候应该显示 g/m3
								{
								display_buffer[8] |= FLAG_md_8;	
								} else
				                Display_weight_unit(current_unit); 
				                break;	
			default:          break;
	  }
	}
  ///////////////////////////////////////////////////////////////////
  	display_buffer[12] = 0;
	  if((STAT_DISP_REC==current_mode)&&(TRUE==REC_flag))
		 tmp = REC_current_mode;	
		else if((STAT_CONFIG==current_mode)&&(MENU_CONFIG_MODE==current_menu))
		 tmp = current_mode_bak;	
		else
		 tmp = current_mode;	
		switch(tmp)
		{
			case STAT_WEIGH:    display_buffer[12] |= FLAG_BASIC_12;    break;							
			case STAT_PCS:      display_buffer[12] |= FLAG_PCS_12;      break;
  		    case STAT_DENSITY:  display_buffer[12] |= FLAG_DENSITY_12;  break;	
			case STAT_CHECK:    display_buffer[12] |= FLAG_CHECK_12;    break;							
			case STAT_100:      display_buffer[12] |= FLAG_PERCENT_12;  break;
  		    case STAT_ANIMAL:   display_buffer[12] |= FLAG_ANIMAL_12;   break;	
			default:
			       break;
	  }
		if(STAT_DISP_REC == current_mode)
       display_buffer[12] |= FLAG_RMPLUS_12;
		if(TRUE == display_flag_mp)
		  {
			 display_buffer[12] |= FLAG_RM_12;	
			 cycle_times++;
       if(6 == cycle_times)
			   {
				  cycle_times = 0;
					display_flag_mp = FALSE; 
				 }				 
			}	
  /////////////////////////////////////////////////////////////////////////////////////	
			display_buffer[13] = 0;
			if((STAT_CAL==current_mode||STAT_CAL_INNER==current_mode))
			{;}
			else if(FALSE == positive_flag)
			  display_buffer[13] |= FLAG_NEG_13;
			//////////////////////////////////////检重模式下允许这两个符号都显示
			if(STAT_CHECK==current_mode)
			   {
					if(1==sample_limit_step)
						 display_buffer[13] |= FLAG_LOWOVER_13;
					else if(2==sample_limit_step)
		  			 display_buffer[13] |= FLAG_UPOVER_13;
			    else if(TRUE == check_flag)
					   display_buffer[13] |= (FLAG_LOWOVER_13+FLAG_UPOVER_13);
					else
					{ }						
				 }
			//////////////////////////////////////////////	 
			if((TRUE==full_flag)&&(TRUE==positive_flag))
      	display_buffer[13] |= FLAG_UPOVER_13;
			if((TRUE==full_flag)&&(FALSE==positive_flag))
      	display_buffer[13] |= FLAG_LOWOVER_13;
			
	}	
	 
/////////////////////////////////////////////////////////////////////////////////////////////
//
//显示 工厂模式
/////////////////////////////////////////////////////////////////////////////////////////////
void Display_F_CFG_Pro(void)
{
 // 显示配置的信息
 	Uint16 i;
	Uint8 tmp[10];
	for(i=1;i<8;i++)
 	display_buffer[i] = display_code[display_F_CFG_INFO[i-1]]; //载入标准格式 
	////////当前哪个配置项 配置数值
	display_buffer[3] = display_code[0];
	display_buffer[4] = display_code[current_menu+1];
	if(MENU_F_CONFIG_TESTKEY == current_menu)
		display_buffer[4] = display_code[20];
	
	i = Get_F_tmp();
	if(MENU_F_CONFIG_FULL == current_menu)
	{
	 display_buffer[5] =  display_code[i/100];
   display_buffer[6] =  display_code[(i%100)/10];
   display_buffer[7] =  display_code[0];	        //最后一位显示 180  	
   display_buffer[8] |= FLAG_g_8;
  }else if(MENU_F_CONFIG_TESTKEY == current_menu)
	{
	 display_buffer[5] =  display_code[i/100];
   display_buffer[6] =  display_code[(i%100)/10];
   display_buffer[7] =  display_code[i%10];	        //
  	
	} else if(MENU_F_CONFIG_REALKG == current_menu)
  {
	  sprintf(tmp,"%8.4f", inner_real_g.rec_float);
	  display_buffer[1] = display_code[tmp[0]-0x30];
		display_buffer[2] = display_code[tmp[1]-0x30];
		display_buffer[3] = display_code[tmp[2]-0x30] | SEG_P;
		display_buffer[4] = display_code[tmp[4]-0x30];
		display_buffer[5] = display_code[tmp[5]-0x30];
		display_buffer[6] = display_code[tmp[6]-0x30];
		display_buffer[7] = display_code[tmp[7]-0x30];
	} else
  {
		display_buffer[8] &= (~FLAG_g_8);
	  display_buffer[7]  = display_code[i]; 
	}		
}

void Display_F_CFG2_Pro(void)
{
 // 显示配置的信息
 	Uint16 i;
	Uint8 tmp[10];
	for(i=1;i<8;i++)
 	display_buffer[i] = display_code[display_F_CFG2_INFO[i-1]]; //载入标准格式 
	////////当前哪个配置项 配置数值
	display_buffer[4] = display_code[current_menu];
	i = Get_F_tmp();
  display_buffer[7] = display_code[i];
				
}
////////////////////////////////////////////////////////////////////////////////////////////
void  Display_inner_cal_sub(void)
   {
    Uint32 i;
    Uint32 tmp;
		tmp =  abs(factory_inner_cal_full-user_inner_cal_full);
		 if(tmp>999)
		   {
				display_buffer[11] =  display_code1[9]; 
				display_buffer[10] =  display_code1[9];
		    display_buffer[9]  =  display_code1[9];
			 }
		else
	  	 {
			  display_buffer[11] =  display_code1[tmp/100]; 
				display_buffer[10] =  display_code1[(tmp%100)/10];
		    display_buffer[9]  =  display_code1[tmp%10];
			 }			
   }

void Ht1621_F_Display_area1(void)	
   {
		Uint8 i; 
		switch(current_mode)
		{
			case STAT_WEIGH:   Display_weight_Data(net_weight,current_unit);
			                   if(INNER_CAL_VERSION)
												 {
													Display_inner_cal_sub(); 
												 }
			                                                                break;
			case STAT_INNER:   Display_Inner_code();                        break;        
			case STAT_CONFIG:  Display_F_CFG_Pro();                         break;
			case STAT_CONFIG2: Display_F_CFG2_Pro();                        break;									 
			default:break;						 
	  }
  }

	
void Ht1621_F_Display_area3(void)              	//符号位显示
   { //300ms * 6 = 1.8s 
		static Uint8 cycle_times=0; 
   	Uint8 tmp;
		///////////////////////////////////////////////////////
		display_buffer[0]  = 0;
		display_buffer[8]  = 0; 
		switch(current_mode)
		{
			case STAT_WEIGH:
				               display_buffer[8]|=  FLAG_g_8 ;
				               break;
			case STAT_INNER:
				               if(motor_pos_flag)
                       display_buffer[0]|=  FLAG_NEIJIAO_0;   												 
				               else
											 display_buffer[0]&=  (~FLAG_NEIJIAO_0); 
											 break;
			case STAT_CONFIG:
				               if(MENU_F_CONFIG_FULL==current_menu)
											 display_buffer[8]|=  FLAG_g_8 ; 
				               break;
			case STAT_CONFIG2:
				               display_buffer[12] |=  FLAG_BASIC_12 ;
			
			                 if(selemode&MODE_PCS)
												 display_buffer[12] |=  FLAG_PCS_12 ;
											 else
												 display_buffer[12] &=  ~(FLAG_PCS_12) ;
			
											 if(selemode&MODE_DENSITY)
												 display_buffer[12] |=  FLAG_DENSITY_12 ;
											 else
												 display_buffer[12] &=  ~(FLAG_DENSITY_12) ;

											 if(selemode&MODE_CHECK)
												 display_buffer[12] |=  FLAG_CHECK_12 ;
											 else
												 display_buffer[12] &=  ~(FLAG_CHECK_12) ;
											  
											 if(selemode&MODE_100)
												 display_buffer[12] |=  FLAG_PERCENT_12 ;
											 else
												 display_buffer[12] &=  ~(FLAG_PERCENT_12) ;
											 
											 if(selemode&MODE_ANIMAL)
												 display_buffer[12] |=  FLAG_ANIMAL_12 ;
											 else
												 display_buffer[12] &=  ~(FLAG_ANIMAL_12) ;
				               break;
											 
			default:	
				               break;
		}		

		display_buffer[13] = 0;
		if(FALSE == positive_flag)
			display_buffer[13] |= FLAG_NEG_13;		
	}
    
    