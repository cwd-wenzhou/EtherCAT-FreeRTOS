#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#define CS PAout(4)


 	    													  
void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
void spi_transfer_block(uint8_t *tx, uint8_t *rx, uint8_t length);
#endif


