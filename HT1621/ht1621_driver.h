//ht1621_driver.h
#ifndef _HT1621_DRIVER_H
#define _HT1621_DRIVER_H
#include "global.h"

///////////////////////////////////////////////////////////
#define COMMAND 0x80   
#define READ    0xC0   
#define WRITE   0xA0   

#define LCD_OFF 0x02   
#define LCD_ON  0x03   
#define WDT_DIS 0x05   
#define TON_OFF 0x08   
#define IRQ_DIS 0x80   
#define Rosc    0x18   
#define XTAL    0x18//0x14   
#define BIS1_2  0x29//0x10   
#define SYS_EN  0x01   
#define SYS_DIS 0x00   
#define NORMAL  0xE3 
//??HT1622pin
#define HT1621_PORT       GPIOB  //use pb

#define HT1621_CS        GPIO_Pin_5
#define HT1621_CLK       GPIO_Pin_6
#define HT1621_DATA      GPIO_Pin_7

#define WRL     GPIO_ResetBits(HT1621_PORT, HT1621_CLK)
#define WRH       GPIO_SetBits(HT1621_PORT, HT1621_CLK)
#define DATAL   GPIO_ResetBits(HT1621_PORT, HT1621_DATA)
#define DATAH     GPIO_SetBits(HT1621_PORT, HT1621_DATA)
#define CSL     GPIO_ResetBits(HT1621_PORT, HT1621_CS)
#define CSH       GPIO_SetBits(HT1621_PORT, HT1621_CS)
///////////////////////////////////////////////////////////////////////
#define   ADDR_OFFSET  (0x00<<2)

//76543210
//AFEDBGCP
#define   SEG_A             0x80
#define   SEG_F             0x40
#define   SEG_E             0x20
#define   SEG_D             0x10
#define   SEG_B             0x08
#define   SEG_G             0x04
#define   SEG_C             0x02
#define   SEG_P             0x01
//////////////////////////////////////ÎÂ¶È×Ö·û
#define   SEG_P1            0x80
#define   SEG_C1            0x40
#define   SEG_G1            0x20
#define   SEG_B1            0x10
#define   SEG_D1            0x08
#define   SEG_E1            0x04
#define   SEG_F1            0x02
#define   SEG_A1            0x01
//////////////////////////·ûºÅÎ»
#define   FLAG_WENDING_0      0x80
#define   FLAG_NEIJIAO_0      0x40
#define   FLAG_WAIJIAO_0      0x20
#define   FLAG_CAL_OK_0       0x10

#define   FLAG_CAL_ER_0       0x08
#define   FLAG_WENDU_0        0x04
#define   FLAG_LOGO_0         0x02
#define   FLAG_FENGE_0        0x01

#define   FLAG_m_7            0x01

#define   FLAG_t_8            0x80
#define   FLAG_pcs_8          0x40
#define   FLAG_md_8           0x20
#define   FLAG_g_8            0x10
#define   FLAG_oz_8           0x08
#define   FLAG_ct_8           0x04
#define   FLAG_percent_8      0x02
#define   FLAG_oF_8           0x01

#define   FLAG_oC_9           0x80

#define   FLAG_BASIC_12       0x80
#define   FLAG_PCS_12         0x40
#define   FLAG_DENSITY_12     0x20
#define   FLAG_RMPLUS_12      0x01
#define   FLAG_CHECK_12       0x08
#define   FLAG_PERCENT_12     0x04
#define   FLAG_ANIMAL_12      0x02
#define   FLAG_RM_12          0x10

#define   FLAG_UPOVER_13      0x80
#define   FLAG_POS_13         0x40
#define   FLAG_NEG_13         0x20
#define   FLAG_LOWOVER_13     0x10


/////////////////////////////////////////////////////////////////////////
//special char
#define   DISP_C       0x0a
#define   DISP_A       0x0b
#define   DISP_L       0x0c
#define   DISP_P       0x0d
#define   DISP_Q       0x0e
#define   DISP_F       0x0f
#define   DISP_U       0x10
#define   DISP_E       0x11
#define   DISP_r       0x12
#define   DISP_X       0x13  // -
#define   DISP_b       0x14
#define   DISP_o       0x15
#define   DISP_t       0x16
#define   DISP_d       0x17
#define   DISP_N       0x18
#define   DISP_NULL    0x19

//////////////////////////////
#define   DISP1_E      0x0a
#define   DISP1_r      0x0b
#define   DISP1_NULL   0x0c


///////////////////////////////////////////////////////////////////////
extern void Init_1621();
extern void Ht1621_Display_area1(void);
extern void Ht1621_Display_area2(void);
extern void Ht1621_Display_area3(void);	

extern void All_on_Must();
extern void All_off_Must();
extern void All_on();
extern void All_off();

#endif