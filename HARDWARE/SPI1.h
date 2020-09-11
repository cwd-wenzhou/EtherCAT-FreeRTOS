/****************************************Copyright (c)****************************************************
**                                      
**  
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               SPI1.h
** Descriptions:            SPI1 init the as1~4
**
**--------------------------------------------------------------------------------------------------------
** Created by:              CWD
** Created date:            2020-8-20
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

// first group, channel choice:
#define LTC2486_CHAN_0_1      0xA080   //0=in+, 1=in-
#define LTC2486_CHAN_2_3      0xA180   //2=in+, 3=in-
#define LTC2486_CHAN_1_0      0xA880   //0=in-, 1=in+
#define LTC2486_CHAN_3_2      0xA980   //2=in-, 3=in+
#define LTC2486_CHAN_0        0xB080   //0=in+, com=in-
#define LTC2486_CHAN_2        0xB180   //2=in+, com=in-
#define LTC2486_CHAN_1        0xB880   //1=in+, com=in-
#define LTC2486_CHAN_3        0xB980   //3=in+, com=in-
#define LTC2486_CHAN_TEMP     0xA0C0   //when using this option, speed and gain is ignored

// second group, reject mode
#define LTC2486_REJECT_5060   0x0000
#define LTC2486_REJECT_50     0x0010
#define LTC2486_REJECT_60     0x0020

// third group, speed
#define LTC2486_SPEED_1x      0x0000
#define LTC2486_SPEED_2x      0x0008

// fourth group, gain
#define LTC2486_GAIN_1x       0x0000
#define LTC2486_GAIN_4x       0x0001
#define LTC2486_GAIN_8x       0x0002
#define LTC2486_GAIN_16x      0x0003
#define LTC2486_GAIN_32x      0x0004
#define LTC2486_GAIN_64x      0x0005
#define LTC2486_GAIN_128x     0x0006
#define LTC2486_GAIN_264x     0x0007

#define DESELECT_SPI 		 GPIO_SetBits(GPIOA,GPIO_Pin_4) 
#define SELECT_SPI 			GPIO_ResetBits(GPIOA,GPIO_Pin_4)  

#define CSLOW()      SELECT_SPI
#define CSHIGH()     DESELECT_SPI

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
u16 SPI1_ReadWriteByte_16bit(u16 TxData);
u16 U16_AD_LTC2486(unsigned char channel);//SPI1总线从LTC2486读取数据
		 
#endif

