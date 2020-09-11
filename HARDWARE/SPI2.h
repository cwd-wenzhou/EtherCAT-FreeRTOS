/****************************************Copyright (c)****************************************************
**                                      
**  
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               SPI2.h
** Descriptions:            SPI2 connects ESC and STM32F407
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

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#define DESELECT_SPI 		 GPIO_SetBits(GPIOB,GPIO_Pin_12) 
	#define SELECT_SPI 			GPIO_ResetBits(GPIOB,GPIO_Pin_12)  

  #define CSLOW()      SELECT_SPI
	#define CSHIGH()     DESELECT_SPI

	#define SPIWriteByte SPIWrite
	#define SPIReadByte() SPIRead()



	// *****************************************************************************
	// *****************************************************************************
	// Section: File Scope or Global Data Types
	// *****************************************************************************
	// *****************************************************************************
	#define CMD_SERIAL_READ 0x03
	#define CMD_FAST_READ 0x0B
	#define CMD_DUAL_OP_READ 0x3B
	#define CMD_DUAL_IO_READ 0xBB
	#define CMD_QUAD_OP_READ 0x6B
	#define CMD_QUAD_IO_READ 0xEB
	#define CMD_SERIAL_WRITE 0x02
	#define CMD_DUAL_DATA_WRITE 0x32
	#define CMD_DUAL_ADDR_DATA_WRITE 0xB2
	#define CMD_QUAD_DATA_WRITE 0x62
	#define CMD_QUAD_ADDR_DARA_WRITE 0xE2

	#define CMD_SERIAL_READ_DUMMY 0
	#define CMD_FAST_READ_DUMMY 1
	#define CMD_DUAL_OP_READ_DUMMY 1
	#define CMD_DUAL_IO_READ_DUMMY 2
	#define CMD_QUAD_OP_READ_DUMMY 1
	#define CMD_QUAD_IO_READ_DUMMY 4
	#define CMD_SERIAL_WRITE_DUMMY 0
	#define CMD_DUAL_DATA_WRITE_DUMMY 0
	#define CMD_DUAL_ADDR_DATA_WRITE_DUMMY 0
	#define CMD_QUAD_DATA_WRITE_DUMMY 0
	#define CMD_QUAD_ADDR_DARA_WRITE_DUMMY 0

	#define ESC_CSR_CMD_REG		0x304
	#define ESC_CSR_DATA_REG	0x300
	#define ESC_WRITE_BYTE 		0x80
	#define ESC_READ_BYTE 		0xC0
	#define ESC_CSR_BUSY		0x80


	/////////////////////////////////////////////////////////////////////////////////
	
	
typedef union
{
	uint32_t Val;
	uint8_t v[4];
	uint16_t w[2];
	struct
	{
		uint8_t LB;
		uint8_t HB;	
		uint8_t UB;
		uint8_t MB;
	}byte;
}UINT32_VAL;


typedef union
{
	uint16_t Val;
	struct
	{
		uint8_t LB;
		uint8_t HB;	
	}byte;
}UINT16_VAL;

/* Private function prototypes -----------------------------------------------*/				
void SPI2_GPIO_Init(void);	
void SPIReadDRegister(uint8_t *ReadBuffer, uint16_t Address, uint16_t Count);
void SPIWriteRegister( uint8_t *WriteBuffer, uint16_t Address, uint16_t Count);
void SPIWriteDWord (uint16_t Address, uint32_t Val);
uint32_t SPIReadDWord (uint16_t Address);
uint8_t WR_CMD (uint8_t cmd)  ;
void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(uint8_t period)	;
void IRQ_EXTI0_Configuration(void);
void SYNC0_EXTI1_Configuration(void);
void SYNC1_EXTI2_Configuration(void);
u8 SPI2_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


