//***********************************************************************
// ��������I2C�ӿ�, ����I2C0����(400K), I2C1����(100K)
//***********************************************************************
#include "global.h"
#include "I2C.h"

////////////////////////////////////////////
/* 1 delay */
////////////////////////////////////////////
static void Wait(Uint8 length)
	{
	 while(length >0)
    	 length--;
	}
/****************************************************************************
* ����SDA �ܽŵķ���
****************************************************************************/	
void SET_I2C_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStruct.GPIO_Pin  = I2C_SDA_PIN ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}
void SET_I2C_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStruct.GPIO_Pin  = I2C_SDA_PIN ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
  GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}
/****************************************************************************
* ������: I2C0_Start
* ����: ��I2C0��ʼ����
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
static void I2C_Start(void)
{
  SET_I2C_OUTPUT();  
        
	SET_I2C_SDA;
	Wait(10);
	SET_I2C_SCL;
	Wait(10);
	CLR_I2C_SDA;
	Wait(10);
	CLR_I2C_SCL;
	Wait(10);
}

/****************************************************************************
* ������: I2C0_Stop
* ����: ��I2C0��������
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
static void I2C_Stop(void)
{
  SET_I2C_OUTPUT(); 
        
	CLR_I2C_SCL;
	Wait(10);
	CLR_I2C_SDA;
	Wait(10);
	SET_I2C_SCL;
	Wait(10);
	SET_I2C_SDA;
	Wait(10);
}
/****************************************************************************
* ������: Init_I2C0
* ����: ��ʼ��I2C0
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
void I2c_Init(void)
{
    //��ʼ�� SCLK�ܽ�
	  GPIO_InitTypeDef GPIO_InitStruct;
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin  = I2C_SCL_PIN ;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
    GPIO_Init(I2C_PORT, &GPIO_InitStruct);
    //////////////////
	  SET_I2C_OUTPUT();  
    Wait(200);
	
    I2C_Start();
    I2C_Stop();
}
/****************************************************************************
* ������: I2C0_Recv_Byte
* ����: I2C0����һ���ֽ�
* ��ڲ���: �Ƿ�ACK(1��,0����)
* ���ڲ���: �ֽ�����
****************************************************************************/
static Uint8 I2C_Recv_Byte(Uint8 ack)
{
        Uint8 i;
        Uint8 rb;
	SET_I2C_INPUT();			//����ģʽ
	for(i=0; i<8; i++)
	{
		rb <<= 1;
		if(READ_I2C_SDA)
		//if(1)
                rb |= 1;
		SET_I2C_SCL;
		Wait(10);
		CLR_I2C_SCL;
		Wait(10);
	}
	SET_I2C_OUTPUT();		//���ģʽ
	if(ack) 
        CLR_I2C_SDA; 
        else
        SET_I2C_SDA;	        //�Ƿ�ACK
	Wait(10);
	SET_I2C_SCL;
	Wait(10);
	CLR_I2C_SCL;
	Wait(10);
	return(rb);
}

/****************************************************************************
* ������: I2C0_Send_Byte
* ����: I2C0����һ���ֽ�
* ��ڲ���: ���͵��ֽ�
* ���ڲ���: �Ƿ��յ�ACK(1�յ�,0δ�յ�)
****************************************************************************/
static Uint8 I2C_Send_Byte(Uint8 wb)
{
       Uint8 i;
       Uint8 ack;
       SET_I2C_OUTPUT();
	for(i=0; i<8; i++)
	{
		if(wb & 0x80)
         SET_I2C_SDA; 
         else
         CLR_I2C_SDA;
		wb <<= 1;
		Wait(10);
		SET_I2C_SCL;
		Wait(10);
		CLR_I2C_SCL;
	}
	SET_I2C_INPUT();			//����ģʽ
	Wait(200);
	if(READ_I2C_SDA==0)
	//if(1)	
         ack = TRUE;
         else 
         ack = FALSE;	//�Ƿ��յ�ACK
	SET_I2C_SCL;
	Wait(10);
	CLR_I2C_SCL;
	Wait(10);
	SET_I2C_OUTPUT();		//���ģʽ
	return(ack);
}

/****************************************************************************
* ������: Read_MainBoard_EEPROM
* ����: ������EEPROM����һ������
* ��ڲ���: addr��EEPROM��ַ(11λ),rdptr��������ָ��,rlen�������ֽ���
* ���ڲ���: ��
****************************************************************************/
void Read_EEPROM(Uint16 addr, void *rdptr, Uint8 rlen)
     {
        Uint8   *rptr;
        Uint16  eepaddr,rl;
	rptr = (Uint8 *)rdptr;
	while(rlen)
	{//δ����
		rl = 256 - (addr & 0xFF);
		if(rl>rlen) rl = rlen;
		eepaddr = AT24C16_ADDR | ((addr >> 7) & 0x0E);
		I2C_Start();
		I2C_Send_Byte(eepaddr);
		I2C_Send_Byte((Uint8)addr);	
		I2C_Start();
		I2C_Send_Byte(eepaddr+1);
		addr += rl;
		rlen -= rl;
		while(rl--) *rptr++ = I2C_Recv_Byte(rl>0);
		I2C_Stop();
	}
}
/****************************************************************************
* ������: Write_MainBoard_EEPROM
* ����: ������EEPROMд��һ������
* ��ڲ���: addr��EEPROM��ַ(11λ),wrptrд������ָ��,wlenд���ֽ���
* ���ڲ���: �����ɹ����(��0��ʾʧ��)
****************************************************************************/
Uint8 Write_EEPROM(Uint16 addr, void *wrptr, Uint8 wlen)
      {
        Uint8   rbuf[16],*wptr,ack,flag;
        Uint16  eepaddr,wl;
        Uint8   i;
	wptr = (Uint8 *)wrptr;
	flag = 0;
	while(wlen)
	{//δд��
		wl = 16 - (addr & 0x0F);	//һ��д�벻�ܳ���16���ֽںͿ��,�������
		if(wl>wlen) wl = wlen;
		//��������ݱȽ�,��ͬ�Ĳ�����д��
		Read_EEPROM(addr, rbuf, wl);	//����ԭ������
		if(memcmp(wptr, rbuf, wl)!=0)
		{//�в�ͬ��Ҫд
			eepaddr = AT24C16_ADDR | ((addr >> 7) & 0x0E);
                        //
			I2C_Start();
			I2C_Send_Byte(eepaddr);
			I2C_Send_Byte((Uint8)addr);
			for(i=0; i<wl; i++) I2C_Send_Byte(wptr[i]);
			I2C_Stop();
			//�ȴ���д���
			for(i=0; i<200; i++)
			{
				I2C_Start();
				ack = I2C_Send_Byte(eepaddr);
				I2C_Stop();
				if(ack) break;
			}

			if(!flag)
			{//��ǰУ�鶼��ȷ����²Ž���У��
				Read_EEPROM(addr, rbuf, wl);	//����д������
				flag = memcmp(wptr, rbuf, wl);
			}
		}
		addr += wl;
		wptr += wl;
		wlen -= wl;
	}
	return(flag);
  }

//end of I2C.c