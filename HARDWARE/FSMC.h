/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
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
#include "ecat_def.h"


  #define SPIx                           SPI1
  #define SPIx_CLK                       RCC_APB2Periph_SPI1
  #define SPIx_CLK_INIT                  RCC_APB2PeriphClockCmd

  #define SPIx_SCK_PIN                   GPIO_Pin_3
  #define SPIx_SCK_GPIO_PORT             GPIOB
  #define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
  #define SPIx_SCK_SOURCE                GPIO_PinSource3
  #define SPIx_SCK_AF                    GPIO_AF_SPI1

  #define SPIx_MISO_PIN                  GPIO_Pin_4
  #define SPIx_MISO_GPIO_PORT            GPIOB
  #define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
  #define SPIx_MISO_SOURCE               GPIO_PinSource4
  #define SPIx_MISO_AF                   GPIO_AF_SPI1

  #define SPIx_MOSI_PIN                  GPIO_Pin_5
  #define SPIx_MOSI_GPIO_PORT            GPIOB
  #define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
  #define SPIx_MOSI_SOURCE               GPIO_PinSource5
  #define SPIx_MOSI_AF                   GPIO_AF_SPI1

	#define DESELECT_SPI 		 GPIOE->BSRRL = GPIO_Pin_2  //GPIO_SetBits(GPIOE,GPIO_Pin_2) 
	#define SELECT_SPI 			 GPIOE->BSRRH = GPIO_Pin_2  //GPIO_ResetBits(GPIOE,GPIO_Pin_2) 


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


///////////////////////////////////////////////////////////////////////////////////////




/* Private function prototypes -----------------------------------------------*/				
void FSMC_GPIO_Init(void);	
uint8_t WR_CMD (uint8_t cmd)  ;
void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(uint8_t period)	;
void EXTI1_Configuration(void);
void EXTI2_Configuration(void);
void EXTI3_Configuration(void);
void PMPReadDRegister(UINT8 *ReadBuffer, UINT16 Address, UINT16 Count);
void PMPWriteRegister( UINT8 *WriteBuffer, UINT16 Address, UINT16 Count);
UINT32 PMPReadDWord (UINT16 Address);
void PMPWriteDWord (UINT16 Address, UINT32 Val);
void SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);
void PMPReadWord (UINT16 Address, UINT16 *Buffer, UINT16 Count);
void PMPWriteWord (UINT16 Address, UINT16 *Buffer, UINT16 Count);
	
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


