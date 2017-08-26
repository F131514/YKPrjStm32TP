#ifndef	_I2C_H_
#define	_I2C_H_

#include "stm32f0xx.h"
//定义I2C0的时钟线和数据线
#define I2C_PORT		        GPIOB
#define I2C_SCL_PIN	        GPIO_Pin_10
#define I2C_SDA_PIN         GPIO_Pin_11

//定义I2C0基本操作			
#define SET_I2C_SDA	      	GPIO_SetBits(  I2C_PORT, I2C_SDA_PIN )
#define SET_I2C_SCL	    	  GPIO_SetBits(  I2C_PORT, I2C_SCL_PIN )
#define CLR_I2C_SDA	    	  GPIO_ResetBits(I2C_PORT, I2C_SDA_PIN )
#define CLR_I2C_SCL	    	  GPIO_ResetBits(I2C_PORT, I2C_SCL_PIN )
#define READ_I2C_SDA	     	GPIO_ReadInputDataBit(I2C_PORT,I2C_SDA_PIN)

////////////////////////////////////////以下是存储空间定义分配
#define AT24C16_ADDR	                    0xA0

//clr_data 线性校准零点
#define    EEP_WEIZERO_L1_ADDR            0x00    //线性校准零点
#define    EEP_WEIZERO_L1_ADDR_BAK        0x04    //备份
//zer0_data 用户校准零点
#define    EEP_WEIZERO_U1_ADDR            0x08    //用户校准零点
#define    EEP_WEIZERO_U1_ADDR_BAK        0x0C    //备份

#define    EEP_WEIFULL1_ADDR              0x10    //满量程内码
#define    EEP_WEIFULL1_ADDR_BAK          0x14    //备份


//////////////////////////////////////////////////////
#define    EEP_INNER_CAL1_ADDR            0xE0    //调试使用 内校准
#define    EEP_INNER_CAL2_ADDR            0xE8    //用户     内校准
//线性校准
#define    EEP_LINCAL1_ADDR01             0x20    //零点
#define    EEP_LINCAL1_ADDR02             0x24    //1/10
#define    EEP_LINCAL1_ADDR03             0x28    //2/10
#define    EEP_LINCAL1_ADDR04             0x2C    //3/10
#define    EEP_LINCAL1_ADDR05             0x30    //4/10
#define    EEP_LINCAL1_ADDR06             0x34    //5/10
#define    EEP_LINCAL1_ADDR07             0x38    //6/10
#define    EEP_LINCAL1_ADDR08             0x3C    //7/10
#define    EEP_LINCAL1_ADDR09             0x40    //8/10
#define    EEP_LINCAL1_ADDR10             0x44    //9/10
#define    EEP_LINCAL1_ADDR11             0x48    //满量程
#define    EEP_LINCAL1_CHECK              0x4C    //

//用户配置 参数 0x50
#define   EEP_USR_FUN1_ADDR               0x50  //   称重单位
#define   EEP_USR_FUN2_ADDR               0x54  //
#define   EEP_USR_FUN3_ADDR               0x58  //
#define   EEP_USR_FUN4_ADDR               0x5C  //
#define   EEP_USR_FUN5_ADDR               0x60  // 存放数据通讯格式
#define   EEP_USR_FUN6_ADDR               0x64  // 存放串口数据目的地
#define   EEP_USR_FUN7_ADDR               0x68  //
#define   EEP_USR_FUN8_ADDR               0x6C  //
//系统配置参数 0x60-0x80
#define   EEP_SYS_FULL_ADDR               0x70  //
#define   EEP_SYS_DIV_ADDR                0x74  // 
#define   EEP_SYS_LOGO_ADDR               0x78  //
#define   EEP_SYS_LANG_ADDR               0x7C  //
#define   EEP_SYS_REP_ADDR                0x80  //
#define   EEP_SYS_INT_ADDR                0x84  //
#define   EEP_SYS_LINE_ADDR               0x88  //
#define   EEP_SYS_LOCK_ADDR               0x8C  //
#define   EEP_SYS_ASD_ADDR                0x90  //
#define   EEP_SYS_REALKG_ADDR             0x94  //

#define   EEP_SYS_DISDIV_ADDR             0x98  // 显示分辨率功能 20170211
//////////////////////////////////////////////////////////////////////////////////
//0x90-0x9f   暂时保留
#define   EEP_UART_ADDR                   0x9C  //配置为打印口，还是连接上位机口
#define   EEP_MODESEL_ADDR                0xF0  //MODE SELECTION

//////////////////////////////////////////////////////////////////////////////////
/////内校准天平使用另外一块存储空间
//////////////////////////////////////////////////////////////////////////////////
//clr_data 线性校准零点
#define    EEP_WEIZERO_L2_ADDR            0xA0    //线性校准 0 点
#define    EEP_WEIZERO_L2_ADDR_BAK        0xA4    //
#define    EEP_WEIZERO_U2_ADDR            0xA8    //zer0_data 用户校准零点
#define    EEP_WEIZERO_U2_ADDR_BAK        0xAC   

#define    EEP_WEIFULL2_ADDR              0xB0   
#define    EEP_WEIFULL2_ADDR_BAK          0xB4    
//线性校准
#define    EEP_LINCAL2_ADDR01             0xC0    //??
#define    EEP_LINCAL2_ADDR02             0xC4    //1/10
#define    EEP_LINCAL2_ADDR03             0xC8    //2/10
#define    EEP_LINCAL2_ADDR04             0xCC    //3/10
#define    EEP_LINCAL2_ADDR05             0xD0    //4/10
#define    EEP_LINCAL2_ADDR06             0xD4    //5/10
#define    EEP_LINCAL2_ADDR07             0xD8    //6/10
#define    EEP_LINCAL2_ADDR08             0xDC    //7/10
#define    EEP_LINCAL2_ADDR09             0xE0    //8/10
#define    EEP_LINCAL2_ADDR10             0xE4    //9/10
#define    EEP_LINCAL2_ADDR11             0xE8    //???
#define    EEP_LINCAL2_CHECK              0xEC    //
//测试地址
#define    EEP_TEST_ADDR                  0xFC    //EEPROM TEST
//??????????????
//AT24C01 128Bytes   AT24C08 1024Bytes   AT24C64   8192Bytes   
//AT24C02 256Bytes   AT24C16 2048Bytes   AT24C128 16384Bytes
//AT24C04 512Bytes   AT24C32 4096Bytes   AT24C256 32768Bytes
//???? 100*16=1600BYTES ????24C16 ???????16BYTE
//????24C32??? ??????,????????
#define    EEP_REC_TOTAL_ADDR             0x100     //??????
#define    EEP_REC_TOTAL_ADDR_BAK         0x104     //??????   
#define    EEP_REC_START_ADDR             0x110     //??????????

#define    EEP_BYTE_PER_REC               0x10      //???????????
#define    MAX_REC_NUM                    100       //????????
//#define    RECNUM_PER_PAGE              1         //???10???

extern  void    I2c_Init(void);
extern  Uint8   Write_EEPROM(Uint16 addr, void *wrptr, Uint8 wlen);
extern  void    Read_EEPROM(Uint16 addr, void *rdptr, Uint8 rlen);
#endif
/*_I2C_H_*/