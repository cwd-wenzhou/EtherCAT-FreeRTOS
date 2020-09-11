/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:              SPI1.c
** Descriptions:           SPI1 hardware driver 
													SPI1负责读取外部adc，即AD1~4通道输入的信号
**
**--------------------------------------------------------------------------------------------------------
** Created by:             	陈韦达
** Created date:            2020-8-24
** Version:                 		v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#include "SPI1.h"
//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  /* CS */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* IRQ */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PA5复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PA5复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PA7复用为 SPI1
	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
	
  /* SPI1 Config -------------------------------------------------------------*/ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 
  SPI_Init(SPI1, &SPI_InitStructure); 
  /* Enable SPI1 */ 

	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

}   
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}


u16 SPI1_ReadWriteByte_16bit(u16 TxData)
{
	u8 Tx_low,Tx_high,Rx_low,Rx_high;
	u16 ans;
	Tx_high=(TxData>>8)&0xff;
	Tx_low=(TxData&0xff);
	Rx_low=SPI1_ReadWriteByte(Tx_low);
	Rx_high=SPI1_ReadWriteByte(Tx_high);
	
	
	ans=(Rx_high<<8)|Rx_low;
	return ans;
}



void LTC2486_INIT()
{
	
	u16 TxData = 0;
	//Assert CS line
	CSLOW();
	//TxData= 0xB980; //Temperature Input, 60Hz Rejection
	TxData= 0xA0E0; //Temperature Input, 60Hz Rejection
	SPI1_ReadWriteByte_16bit(TxData); 
}

u16 U16_AD_LTC2486(unsigned char channel)
{

	u16 TxData = 0;
	TxData= 0xFFFF; //Temperature Input, 60Hz Rejection
	return SPI1_ReadWriteByte_16bit(TxData); 
	
}







