//**********************************************************************
//文件名:filter.c
//函数列表
//1   手动破坏稳定状态  stable_manual_break
//2   滤波初始化        filter_init
//3   排序              filter_sort
//4   取平均值          stable_filter_average
//5   取平均值1         stable_filter_average1
//6   取平均值2         stable_filter_average2
//7   取平均值3         stable_filter_average3
//8   滤波过程          filter(void)
//9   零点自动跟踪      auto_zer0
//10  载荷自动跟踪      stable_auto_zer0_A
//10a 载荷自动跟踪      stable_auto_zer0_B
//11  重复性自动修正    repeat_adjust
//12  返回滤波后的数据  Get_Filter_Data
//
//**********************************************************************
#include "global.h"

#include "filter.h"
#include "ad_acquire.h"
#include "stdlib.h"


 static Uint32   stable_times;                 //稳定计时间
   
 static Uint32   Buf_Total1[AD_AVER_CNT],Buf_Total2[AD_AVER_CNT],Buf_Total3[AD_AVER_CNT];
 
 static Uint32   AD_Buf_Raw1[AD_AVER_CNT1],ad_buf_index1,flag_ad_buf_full1,fast_filter_flag;
 static Uint32   AD_Buf_Raw2[AD_AVER_CNT2],ad_buf_index2,flag_ad_buf_full2;
  
 static Uint32  filter_data_final;
 static Uint32  raw_data,Average_AD_A,Average_AD_B;
        
 Uint8 flag_auto_track_enable,auto_track_cnt;  //在这种情况下都不允许自动跟踪和载荷跟踪
 Uint8 flag_load_track_enable,load_track_cnt; 
/////////////////////////////////////////////////////////////
//1 此函数在校准的时候调用，人为破坏平衡
/////////////////////////////////////////////////////////////
void  stable_manual_break(void)
{
 stable_times  = 0;
 stable_flag = FALSE; 
}   
//////////////////////////////////
//2 滤波初始化
//////////////////////////////////////     
void  filter_init(void)
{
 Uint32 i;
 //for(i=0;i<AD_SORT_BUF_CNT;i++)
 // Buf_Sort1[i] = 0;
 for(i=0;i<AD_AVER_CNT;i++)
   {
    Buf_Total1[i] = 0;
    Buf_Total2[i] = 0;
    Buf_Total3[i] = 0;
   } 
   
 for(i=0;i<AD_AVER_CNT1;i++)
  AD_Buf_Raw1[i] = 0;
 for(i=0;i<AD_AVER_CNT2;i++)
  AD_Buf_Raw2[i] = 0;

  ad_buf_index1     = 0;
  flag_ad_buf_full1 = 0;
  ad_buf_index2     = 0;
  flag_ad_buf_full2 = 0;

}

//********************************************************
Uint32  get_buf_data(void)
        { //当前缓存已满，则取除了最大最小值后的平均值
           //如果没有满  ，则取 所有数据的平均值
           Uint32  buf[AD_AVER_CNT1];
           Uint32  sum;
           Uint8   pass,i;
           if(1 == flag_ad_buf_full1)
            {
                for(i=0;i<AD_AVER_CNT1;i++)
                 buf[i] = AD_Buf_Raw1[i];
                
                for(pass = 0;pass < AD_AVER_CNT1 - 1 ;pass++)
	              {
		            for( i = 0;i < (AD_AVER_CNT1 - 1 - pass);i++)
		             {
			           if(buf[i] > buf[i+1])
			             {
			               sum = buf[i];
		   	               buf[i] = buf[i+1];
			               buf[i+1] = sum ;
			             }
		             }
	              }
	         sum = 0;  
	         for(i=4;i<12;i++)   
	         sum += buf[i];
	         return(sum/8);
	        }
           else
            {
              sum = 0;
              for(i=0;i<ad_buf_index1;i++)
               sum += AD_Buf_Raw1[i];         
 	          return(sum/ad_buf_index1); 
            }
         }     
             
/////////
//********************************************************
Uint32  get_buf_data2(void) //
        {
        
           Uint32  buf[AD_AVER_CNT];
           Uint32  sum;
           Uint8   pass,i;
           
            for(i=0;i<AD_AVER_CNT;i++)
              buf[i] = Buf_Total3[i];
                 
            for(pass = 0;pass < AD_AVER_CNT - 1 ;pass++)
	              {
		            for( i = 0;i < (AD_AVER_CNT - 1 - pass);i++)
		             {
			           if(buf[i] > buf[i+1])
			             {
			               sum = buf[i];
		   	               buf[i] = buf[i+1];
			               buf[i+1] = sum ;
			             }
		             }
	              }
	         sum = 0;  
	         for(i=4;i<12;i++)   
	         sum += buf[i];
	         return(sum/8);
	      
         } 
//////////////////////////////////////     
static Uint32  filter_Average2(Uint32 raw1)
 { 
  //把RAW 数据放在缓存中，并取平均值
  Uint32 i,sum;
   
  AD_Buf_Raw2[ad_buf_index2++] = raw1;
  
  if(AD_AVER_CNT2 == ad_buf_index2)
     {
      flag_ad_buf_full2 = 1;
      ad_buf_index2 = 0;
     }
  ///////////////////////////////////////返回
  if(1 == flag_ad_buf_full2)
    {  
     sum = 0;
     for(i=0;i<AD_AVER_CNT2;i++)
     sum+=AD_Buf_Raw2[i];
     return(sum/AD_AVER_CNT2);
    }
  else
    {
     sum = 0;
     for(i=0;i<ad_buf_index2;i++)
     sum+=AD_Buf_Raw2[i];
     return(sum/ad_buf_index2); 
    }   
 }     
/*****************************************************************
*5  取平均值1
*队列长度为16
/*****************************************************************/  
static Uint32  filter_total1(Uint32 data1)
  {
  
    static Uint32   Buf_data1_count = 0;
	static Uint32   Buf_data1_count_H = 0;
	static Uint32   Buf_index1 = 0;
	       Uint32   data1_tmp;
	  
      //////////////////////////////////////////////取最近16次数据之和
       Buf_data1_count = Buf_data1_count + data1; //累加本次新数据
       if(Buf_data1_count < data1) //溢出
        {       
         Buf_data1_count_H++;
        }
                                                   
       data1_tmp = Buf_data1_count;
       Buf_data1_count = Buf_data1_count - Buf_Total1[Buf_index1]; //减去上次数据
       if(Buf_data1_count > data1_tmp)
         {       
          Buf_data1_count_H--;
         }
     
       Buf_Total1[Buf_index1++] = data1;    //新数据保存
       if(AD_AVER_CNT == Buf_index1)
       Buf_index1 = 0;      
 
     /////////////////////////////第一次滑动滤波
	 data1_tmp = ((Buf_data1_count_H&0xff)<<24)|((Buf_data1_count&0xffffff00)>>8); //*16除以256 相当于除以16
     return(data1_tmp);
  }    
/*****************************************************************
*5  取平均值1
*队列长度为16
/*****************************************************************/  
static Uint32  filter_total2(Uint32 data2)
  {
      static Uint32   Buf_data2_count = 0;
      static Uint32   Buf_index2 = 0;
	  
      //////////////////////////////////////////////取最近16次数据之和
      Buf_data2_count = Buf_data2_count + data2; //累加本次新数据
                                                 //减去上次数据  
      Buf_data2_count = Buf_data2_count - Buf_Total2[Buf_index2];  
      
      Buf_Total2[Buf_index2++] = data2;    //新数据保存
      if(AD_AVER_CNT == Buf_index2)
      Buf_index2 = 0;      
     /////////////////////////////第二次滑动滤波 
     return(Buf_data2_count/AD_AVER_CNT);    
  } 
 /*****************************************************************
*5 排序后 取中间8个数据的平均值
*  队列长度为16
/*****************************************************************/  
static Uint32  filter_total3(Uint32 data3)
  {
             Uint32   total_tmp;
      static Uint32   Buf_index3 = 0;
      static Uint32   flag_full  = 0;
	  
	  Buf_Total3[Buf_index3++] = data3;
	  if(AD_AVER_CNT == Buf_index3)
        {
         Buf_index3 = 0;
         flag_full = 1;
        }
      //////////////////////////////////
      if(0==flag_full)
      return(data3);
      else
      return(get_buf_data2());     
  }       
//////////////////////////////////////////////////////  
//8B 新的滤波算法 
//信号输出过程：
//开始阶段，超一个方向快速上升或下降
//沿一个方向过冲后再回调，然后是围绕某个区间上下振荡
//
///////////////// 
#define NOT_VALID_VALUE                     8     //6个字
#define NOVALID_ACCEP_TIME                  2      //连续朝某个方向变化2次后才认可是有效数据

/////////////////////////////////////////////////////////////////////////////////////////
//根据数据变化率情况，返回本次新数据的修正值
////////////////////////////////////////////////////////////////////////////////////////
static Uint32  filter0_proc(void)
{
  static Uint32 raw_data1_last,raw_data2_last;
  static Uint32 direct_add_cnt,direct_sub_cnt;  //变化方向
  static Uint32 filter0_stable_cnt = 0;         //稳定次数
         Uint32 data_tmp;
  
         Uint32 differ_value;
  
  data_tmp = Get_Raw();
  //使用最原始的数据判断不稳定，使用最后滤波的数据判断 稳定
  if(abs(raw_data1_last-data_tmp) > 50000)
   {
       stable_flag        = 0;
       stable_times       = 0;
	   filter0_stable_cnt = 0;
	   //这些变量 起加快显示的作用
	   fast_filter_flag    = 1;	  
	   ad_buf_index1       = 0;
	   flag_ad_buf_full1   = 0;  
	   ad_buf_index2       = 0;
	   flag_ad_buf_full2   = 0;
	   //破坏缓存数据，重新开始建立 
       //以下变量对数据处理起作用 零点跟踪 和 载荷跟踪 	 
       flag_auto_track_enable = 0;  //在这种情况下都不允许自动跟踪和载荷跟踪
       auto_track_cnt         = 0;
       flag_load_track_enable = 0;
       load_track_cnt         = 0; 
   }
  
  raw_data1_last = data_tmp;
  
  data_tmp   = filter_total1(data_tmp);      //直接得到需要处理的原始数据 除以16
  data_tmp   = filter_total2(data_tmp);       //再取最近16次的滑动平均值
  raw_data   = filter_total3(data_tmp);       //16次数据中间8位数据的平均值
    
  differ_value = abs(raw_data - raw_data2_last); 
  /////////////////////////////////////////////////////
  if(differ_value > NOT_VALID_VALUE*inner_code_step)   //变化数据大于xx个数字
    {  
       stable_flag        = 0;
       stable_times       = 0;
	   filter0_stable_cnt = 0;
	   //这些变量 起加快显示的作用
	   fast_filter_flag    = 1;	  
	   ad_buf_index1       = 0;
	   flag_ad_buf_full1   = 0;  
	   ad_buf_index2       = 0;
	   flag_ad_buf_full2   = 0;
	   //破坏缓存数据，重新开始建立 
       //以下变量对数据处理起作用 零点跟踪 和 载荷跟踪 	 
       flag_auto_track_enable = 0;  //在这种情况下都不允许自动跟踪和载荷跟踪
       auto_track_cnt         = 0;
       flag_load_track_enable = 0;
       load_track_cnt         = 0; 
       
       raw_data2_last = raw_data;
     }
   else
     { //当变化小于一定数值时候
       filter0_stable_cnt++;
        raw_data2_last = raw_data;
       ///////////////////////////////////在稳定后使用相邻两次值的权作为最终数据  
       if(filter0_stable_cnt < 4)       //此参数可以调整
            {
             raw_data2_last = raw_data;
            }
       else if(filter0_stable_cnt < 16)
            {
             raw_data2_last = (raw_data*9+raw_data2_last)/10; 
            } 
       else if(filter0_stable_cnt < 24)
            {
             raw_data2_last = (raw_data*8+raw_data2_last*2)/10;    //更新
            } 
       
       //根据稳定时间 置标志
       if(filter0_stable_cnt > 2)
            {
             direct_add_cnt   = 0;
	         direct_sub_cnt   = 0;    
             fast_filter_flag = 0;  //滤波缓存开始起作用
            }        
      }
     return raw_data2_last;
   } 
     
     
///////////////////////////////////////////////////////////////////
//         滤波算法
//
//(1) 取渐进平均值             1/16
//(2) 取渐进平均值             1/16
//(3) 取排序后的平均值 8个取中间4个
//(4) 取平均值                  1/8
//
/*////////////////////////////////////////////////////////////////////
void  filter_a(void)
{     
      static Uint32 ad_stable_cnt;
      Uint32 ad_sample_dat;
      
      ad_sample_dat = filter0_proc();      //如果处于快速变化阶段
      ///////////////////////////////////////////////////////////
      if(1==fast_filter_flag)
         {
          stable_flag    = FALSE;
          ad_stable_cnt  = 0;
          Average_AD_A   = ad_sample_dat;  //快速变化阶段
          Average_AD_B   = Average_AD_A;
          filter_data_final = Average_AD_B;         
 	     }     
 	  else
 	   {
 	     /////////////////////////////////////////////////
 	     if(0==flag_ad_buf_full1) //初次建立缓存还没有填充满
 	         {
 	          AD_Buf_Raw1[ad_buf_index1++] = ad_sample_dat;////save 
             }
          else
             {
              ad_stable_cnt++;
           
              // 填充满后 再开始稳定计时间 根据时间来取舍数据的权重 
              if(ad_stable_cnt<4)
                 {
                   if(0 == ad_buf_index1)
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*9+AD_Buf_Raw1[AD_AVER_CNT1-1])/10;
 	               else
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*9+AD_Buf_Raw1[ad_buf_index1-1])/10;     
                 }
              else //if(ad_stable_cnt<6) 
                 {
                   if(0 == ad_buf_index1)
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*8+AD_Buf_Raw1[AD_AVER_CNT1-1]*2)/10;
 	               else
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*8+AD_Buf_Raw1[ad_buf_index1-1]*2)/10;  
                 }
             
 	          }
 	      	  ////////////////////////////////////得到新数据  
 	          if(AD_AVER_CNT1 == ad_buf_index1)
 	           {
 	            flag_ad_buf_full1 = 1; 
                ad_buf_index1 = 0;
               }   
 	          Average_AD_A = get_buf_data();   //获取排序后的中间数据的平均值
 	          /////////////////////////////////////////////////////////////////////
 	          ////////////////////////////////////////////////////然后再取一次平均值
 	          Average_AD_B = filter_Average2(Average_AD_A);
 	          ////////////////////////////////////////////////判断稳定以及 开启自动跟踪功能 
              if(abs(filter_data_final-Average_AD_B) < 2*inner_code_step)
 	           {
 	             stable_times++;
 	             if((FALSE==point10_cal_ok)||(TRUE==point2_cal_start))
 	               {
 	                 if(stable_times > 14)
 	                    stable_flag = TRUE;   
 	               }
 	             else  
 	               {
 	                if(stable_times > 14)
 	                   stable_flag = TRUE;
 	               } 
 	           }
              else
               {
                 stable_times = 0;
               }
               filter_data_final = Average_AD_B;
              //////////////////////////////////////////////////////// 
              if(TRUE == stable_flag)       //稳定后开启自动跟踪功能
                 flag_auto_track_enable = 1;
              else
                {
                 flag_auto_track_enable = 0;
                 auto_track_cnt         = 0;
                } 
 	          //载荷跟踪的条件(1) 稳定
 	          //              (2) 有一定的重量存在 
 	          //              (3) 不在用户校准过程中
 	          //              (4) 线性校准数据起作用的时候
              if((stable_flag==TRUE)&&(TRUE==point10_cal_ok)&&(FALSE==point2_cal_start)) //稳定一段时间且不在任何校准状态
                flag_load_track_enable = 1;
              else
               {
                flag_load_track_enable = 0;
                load_track_cnt         = 0;
               }              
         }         
 }
*/ 
///////////////////////////////////////////////////////////////////////////////
void  filter(void)
{     
  static Uint32 raw_data1_last,raw_data2_last;
  static Uint32 direct_cnt1,direct_cnt2;      
  static Uint32 filter0_stable_cnt = 0;         //稳定次数
         Uint32 data_tmp;
  
         Uint32 differ_value;
  
  data_tmp = Get_Raw();
  //使用最原始的数据判断不稳定，使用最后滤波的数据判断 稳定
  if(abs(raw_data1_last-data_tmp) > 50000)
   {
       stable_flag        = 0;
       stable_times       = 0;
       direct_cnt1 = 0;
       direct_cnt2 = 0;
       ///////////////////////////////////
       flag_ad_buf_full2 = 0;
       ad_buf_index2 = 0;
	   //破坏缓存数据，重新开始建立 
       //以下变量对数据处理起作用 零点跟踪 和 载荷跟踪 	 
       flag_auto_track_enable = 0;  //在这种情况下都不允许自动跟踪和载荷跟踪
       auto_track_cnt         = 0;
       flag_load_track_enable = 0;
       load_track_cnt         = 0; 
   }
  
  raw_data1_last = data_tmp;
  data_tmp   = filter_total1(raw_data1_last);
  direct_cnt1++;
  if(direct_cnt1>15)
   {
    direct_cnt2++;
    Average_AD_A = data_tmp;
   } 
  else
   {
    direct_cnt2 = 0; 
    //Average_AD_A = raw_data1_last/16;
    Average_AD_A = data_tmp;
   }
  
  data_tmp   = filter_total2(Average_AD_A);  //再取最近16次的滑动平均值
  
  if((direct_cnt2>15)||(abs(filter_data_final-Average_AD_B)<10*inner_code_step))
  Average_AD_B = data_tmp;
  else
  Average_AD_B = Average_AD_A;
  ////////////////////////////////增加平均值滤波
  data_tmp = filter_Average2(Average_AD_B);
  
  ////////////////////////////////////////////////////////////// 
  if(abs(filter_data_final-data_tmp) < inner_code_step)
    {
 	   stable_times++;
 	   if(current_mode==STAT_CAL_INNER)
 	      {
 	       if(stable_times > 20)
 	         stable_flag = TRUE;   
 	      }
 	   else  
 	     {
 	      if(stable_times > 10)
 	         stable_flag = TRUE;
 	     } 
 	  }
   else
    {
     stable_times = 0;
    }
   //////////////////////////////////////////
    filter_data_final = data_tmp;
   //////////////////////////////////////////////////////// 
       if(TRUE == stable_flag)       //稳定后开启自动跟踪功能
            flag_auto_track_enable = 1;
       else
           {
            flag_auto_track_enable = 0;
            auto_track_cnt         = 0;
           } 
    
        ////////////////////////////////////////////////判断稳定以及 开启自动跟踪功能 
             
 	          //载荷跟踪的条件(1) 稳定
 	          //              (2) 有一定的重量存在 
 	          //              (3) 不在用户校准过程中
 	          //              (4) 线性校准数据起作用的时候
					  //               repetion_func is 1
        if((stable_flag==TRUE)&&(TRUE==point10_cal_ok)&&(FALSE==point2_cal_start)&&(1==repetion_func)) //稳定一段时间且不在任何校准状态
                flag_load_track_enable = 1;
              else
               {
                flag_load_track_enable = 0;
                load_track_cnt         = 0;
               }                       
 } 
///////////////////////////////////////////
//9  零点自动跟踪 修正后的零点数据
//////////////////////////////////////////
void   auto_zer0(Uint32 weight1)
{
  Uint32 x;
  
  auto_track_cnt++;
  if(2 == auto_track_cnt)
     {
        auto_track_cnt = 0;
        //if(data_new < AUTO_0_SETUP * inner_code_step)
        x = abs(weight1-zer0_data);
        if(x < 7*inner_code_step)
          {
           auto_zer0_valid = TRUE;   //自动跟踪生效  
           zer0_data = weight1;      //这个零点数据是否需要更新    
          } 
       /* else if(x < AUTO_0_SETUP*inner_code_step)
          {
            if(weight1>zer0_data)                             
              zer0_data = zer0_data + inner_code_step;      //使用线性校准前的数据作活动零点
            else
              zer0_data = zer0_data - inner_code_step;      //使用线性校准前的数据作活动零点
          }*/
     }
}
///////////////////////////////////////////
//10  载荷稳定后的零点自动跟踪,
//来保证稳定后的变化量施加给零点
//////////////////////////////////////////
void auto_loadtrack(Uint32 weight2)
{
    static Uint32 trackcnt = 0; //
	  static Uint32 lock_ad_last = 0;
	  Uint32 weigh_tmp;
	  if((stable_flag==TRUE)&&(weight2 > 100*inner_code_step)&&(TRUE==point10_cal_ok)&&(FALSE==point2_cal_start)){
				trackcnt++;
				if(trackcnt > 8) {
				 	  trackcnt = 0;
					  weigh_tmp = abs(weight2-lock_ad_last);   //比较重量漂移了多少 
            if(weigh_tmp > (2*inner_code_step+inner_code_step/2)) {  //重量变化量太大了，认为是真实重量
                lock_ad_last = weight2;
            } else {
                if(weight2 > lock_ad_last) 
                   zer0_data = zer0_data + weigh_tmp;
                else
                   zer0_data = zer0_data - weigh_tmp;
								
								lock_ad_last = weight2;
				    } 	
				}else {
			      lock_ad_last = weight2;
			  }
    } else {
		    trackcnt = 0;
		}				
}
Uint32  auto_repetioncheck(Uint32 weight2)
{	
    static Uint32 lock_ad_last     ; //上次锁定的重量(纯重量内码)
    
    if((weight2 > 100*inner_code_step)&&(0 == load_track_cnt)) {
        if(abs(weight2-lock_ad_last) > (3 * inner_code_step)) {
  			    //更新此次重量
            lock_ad_last = weight2;
        }else {
            lock_ad_last = (lock_ad_last*3+weight2*7)/10;        //小于3个取平均值
           }  
         load_track_cnt++;
     } else {
		     lock_ad_last = weight2;
		 }
	     return(lock_ad_last);
}
	
	
	
	
///////////////////////////////////////////////////////////
//
const  float ntc_table[]=
{ //-5     0     5      10     15
 41.033,29.884,25.121,19.805,15.693,
 //20     25    30      35     40
 12.496,10.000,8.0413,6.4972,5.2743,
 //45     50    55      60     65
 4.3013,3.4970,2.8993,2.3959,1.9882,
 //70     75    80      85     90
 1.6567,1.3850,1.1641,0.9814,0.8304,
 //95    100    105
 0.7052,0.5840,0.5140,0.0001
};

const  Uint8 ntc_start[]=
{0,//无效数据
 0 ,5, 10,15,20,25,30,35,
 40,45,50,55,60,65,70,75,
 80,85,90,95,100,105
};

static Uint16 ntc_buf[16];

/////////////////////////////////////////////////////////////////////
void filter_init_lpc2478ad(void)
 {
  Uint8 i;
  for(i=0;i<16;i++)
     ntc_buf[i] = 0;
    
 }
 
 //////////////////////////////////////////
//********************************************************
float  filter_ad_NTC(Uint16 ntc_data)
         { //当前缓存已满，则取除了最大最小值后的平均值
           //如果没有满  ，则取 所有数据的平均值
           Uint32  sum;
           float   x;
           Uint8   pass,i;
           static  Uint8 index = 0;
           ntc_buf[index++] = ntc_data;
           if(4 == index)
           index = 0;
           ////////////////////////////////////////
	       sum = 0;
	       for(i=0;i<4;i++)
	        sum+=ntc_buf[i];  
	       sum = sum/4;
	       /////////////////////////////////根据此数据倒推出电阻式多大
	       x = (10*sum)/(4096.00001-sum);
	       ////////////////////////////////根据此电阻值计算出温度
	       if(x>=ntc_table[0]) //温度太小
	         {
	          //return(11.1);
	          return(TEMP_SENSOR_NC_VALUE);
	         }
	       else if(x<=ntc_table[22]) 
	         {
	          //return(22.2);
	          return(TEMP_SENSOR_NC_VALUE);
	         } 
	       else 
	         {
	          i = 1;
	          while(1)
	           {
	            if(x>ntc_table[i])
	               break;
	            else
	             i++;   
	           }
	          //////////////////计算温度 
	          x =ntc_start[i]-(x-ntc_table[i])/(ntc_table[i-1]-ntc_table[i])*5;
	          //return(33.3);
	          return(x); 
	          } 
	 }        
///////////////////////////////////////////////////////////////////
	
	
//////////////////////////////////////////////
//12  返回自动跟踪后的数据
//////////////////////////////////////////////
Uint32 Get_Raw1_Data(void)
{
return(raw_data);
}
Uint32 Get_Ave1_Data(void)
{
 return(Average_AD_A);
}
Uint32 Get_Ave2_Data(void)
{
 return(Average_AD_B);
}

Uint32 Get_Filter_Data(void)
{
 return(filter_data_final);
}
