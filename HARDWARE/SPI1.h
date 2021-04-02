#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#define CS PAout(4)


 	    													  
void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
void spi_transfer_block(uint8_t *tx, uint8_t *rx, uint8_t length);
#endif


