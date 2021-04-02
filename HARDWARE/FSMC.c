/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               .c
** Descriptions:           SPI1 hardware driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:             	 lyd
** Created date:            2015-05-01
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "FSMC.h"
#include "ecat_def.h"
#include "el9800hw.h"


#define Bank1_SRAM4_ADDR    ((u32)( 0x6c000000))	
/*******************************************************************************
* Function Name  : SPI1_Init
* Description    : SPI1 initialize
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void FSMC_GPIO_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
	FSMC_NORSRAMTimingInitTypeDef  WriteTiming; 
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB |
                         RCC_AHB1Periph_GPIOG, ENABLE);

  /* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
  
/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+------------------+------------------+
 | PD0  <-> FSMC_D2  | 										| PG12 <-> FSMC_NE4  |            
 | PD1  <-> FSMC_D3  | 									  | PB7  <-> FSMC_NL   |  
 | PD4  <-> FSMC_NOE | 									  |                    | 
 | PD5  <-> FSMC_NWE | 										|                    | 
 | PD8  <-> FSMC_D13 |									  |                    | 
 | PD9  <-> FSMC_D14 | 									  |                    |  
 | PD10 <-> FSMC_D15 | 									  |                    |  
 | 									 | PE7  <-> FSMC_D4   |                    |
 |  								 | PE8  <-> FSMC_D5   |                    |
 |                   | PE9  <-> FSMC_D6   |                    |
 | PD14 <-> FSMC_D0  | PE10 <-> FSMC_D7   |------------------+
 | PD15 <-> FSMC_D1  | PE11 <-> FSMC_D8   |
 +-------------------| PE12 <-> FSMC_D9   |
                     | PE13 <-> FSMC_D10  |
                     | PE14 <-> FSMC_D11  |
                     | PE15 <-> FSMC_D12  |
                     +--------------------+
*/
/*
 +-------------------+--------------------+------------------+------------------+
 | PD0  <-> FSMC_D2  | 										| PG10 <-> FSMC_NE3  |            
 | PD1  <-> FSMC_D3  | 									  | PB7  <-> FSMC_NL   |  
 | PD4  <-> FSMC_NOE | 									  |                    | 
 | PD5  <-> FSMC_NWE | 										|                    | 
 | PD8  <-> FSMC_D13 |									  |                    | 
 | PD9  <-> FSMC_D14 | 									  |                    |  
 | PD10 <-> FSMC_D15 | 									  |                    |  
 | 									 | PE7  <-> FSMC_D4   |                    |
 |  								 | PE8  <-> FSMC_D5   |                    |
 |                   | PE9  <-> FSMC_D6   |                    |
 | PD14 <-> FSMC_D0  | PE10 <-> FSMC_D7   |------------------+
 | PD15 <-> FSMC_D1  | PE11 <-> FSMC_D8   |
 +-------------------| PE12 <-> FSMC_D9   |
                     | PE13 <-> FSMC_D10  |
                     | PE14 <-> FSMC_D11  |
                     | PE15 <-> FSMC_D12  |
                     +--------------------+
*/

  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5 
                               |GPIO_Pin_8  | GPIO_Pin_9  |  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);


  /* GPIOB configuration */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_FSMC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* GPIOG configuration */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;  
  GPIO_Init(GPIOG, &GPIO_InitStructure);

/*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_AddressSetupTime =  15;//
  p.FSMC_AddressHoldTime = 15;//;
  p.FSMC_DataSetupTime = 15;
  p.FSMC_BusTurnAroundDuration = 15;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode =0;
	
	
	/*
	WriteTiming.FSMC_AddressSetupTime = 15;	 //地址建立时间（ADDSET）为1个HCLK 1/168M=6ns
  WriteTiming.FSMC_AddressHoldTime = 15;	 //地址保持时间（ADDHLD）模式A未用到	
  WriteTiming.FSMC_DataSetupTime = 60;		 ////数据保持时间（DATAST）为9个HCLK 6*9=54ns	 	 
  WriteTiming.FSMC_BusTurnAroundDuration = 15;
  WriteTiming.FSMC_CLKDivision = 0;
  WriteTiming.FSMC_DataLatency = 0;
  WriteTiming.FSMC_AccessMode = 0;	 */
    
	
	
	

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;/////////////////////////////
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;  
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /*!< Enable FSMC Bank1_SRAM2 Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}



void SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  for (; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM4_ADDR + WriteAddr) = *pBuffer++;

    /* Increment the address*/
    WriteAddr += 2;
  }
}

/**
  * @brief  Reads a block of data from the FSMC SRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the
  *         SRAM memory.
  * @param  ReadAddr : SRAM memory internal address to read from.
  * @param  NumHalfwordToRead : number of half-words to read.
  * @retval None
  */
void SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
  for (; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(__IO uint16_t*) (Bank1_SRAM4_ADDR + ReadAddr);

    /* Increment the address*/
    ReadAddr += 2;
  }
}



	


void PMPReadWord (UINT16 Address, UINT16 *Buffer, UINT16 Count)
{
   //for (; Count != 0; Count--) /* while there is data to read */
  //{
    /* Read a half-word from the memory */
    *Buffer++ = *(__IO uint16_t*) (Bank1_SRAM4_ADDR + Address);

    /* Increment the address*/
  //  Address += 2;
 // }
}

UINT32 PMPReadDWord (UINT16 Address)
{
    
    UINT32_VAL res;
	  
		//res.Val = (UINT32)(*(volatile UINT32 *)(Bank1_SRAM4_ADDR+(Address)));
	  res.w[0]= *(__IO uint16_t*) (Bank1_SRAM4_ADDR + Address);
    /* Increment the address*/
    Address += 2;
	 res.w[1]= *(__IO uint16_t*) (Bank1_SRAM4_ADDR + Address);
	

    return res.Val;
}





void PMPWriteWord (UINT16 Address, UINT16 *Buffer, UINT16 Count)
{
    
	//for (; Count != 0; Count--) /* while there is data to write */
	//	{
       /* Transfer data to the memory */
        *(uint16_t *) (Bank1_SRAM4_ADDR + Address) = *Buffer++;

      /* Increment the address*/
 //      Address += 2;
	//	}
    
}

void PMPWriteDWord (UINT16 Address, UINT32 Val)
{
   
	UINT32_VAL res;
	res.Val=Val;
	
	/* Transfer data to the memory */
	
   // *(uint32_t *) (Bank1_SRAM4_ADDR + Address) = Val;
	*(uint16_t *) (Bank1_SRAM4_ADDR + Address) = res.w[0];

    /* Increment the address*/
    Address += 2;
	*(uint16_t *) (Bank1_SRAM4_ADDR + Address) = res.w[1];

  
}


void PMPReadRegUsingCSR(UINT8 *ReadBuffer, UINT16 Address, UINT8 Count)
{
	UINT32_VAL param32_1 = {0};
  UINT8 i = 0;
	UINT16_VAL wAddr;
    wAddr.Val = Address;

    param32_1.v[0] = wAddr.byte.LB;
    param32_1.v[1] = wAddr.byte.HB;
    param32_1.v[2] = Count;
    param32_1.v[3] = ESC_READ_BYTE;

    PMPWriteDWord (CSR_CMD_REG, param32_1.Val);

    do
    {
        param32_1.Val = PMPReadDWord (ESC_CSR_CMD_REG);
	

    }while(param32_1.v[3] & ESC_CSR_BUSY);


    param32_1.Val = PMPReadDWord (ESC_CSR_DATA_REG);

    
     for(i=0;i<Count;i++)
         ReadBuffer[i] = param32_1.v[i];
   
    return;
}

void PMPWriteRegUsingCSR( UINT8 *WriteBuffer, UINT16 Address, UINT8 Count)
{
    UINT32_VAL param32_1 = {0};
	  UINT16_VAL wAddr;
    UINT8 i = 0;
		
    for(i=0;i<Count;i++)
         param32_1.v[i] = WriteBuffer[i];
		
    PMPWriteDWord (ESC_CSR_DATA_REG, param32_1.Val);
		
		wAddr.Val = Address;

    param32_1.v[0] = wAddr.byte.LB;
    param32_1.v[1] = wAddr.byte.HB;
    param32_1.v[2] = Count;
    param32_1.v[3] = ESC_WRITE_BYTE;

    PMPWriteDWord (0x304, param32_1.Val);
   do
    {
       
			param32_1.Val = PMPReadDWord (0x304);
			
    }while(param32_1.v[3] & ESC_CSR_BUSY);

    return;
}

void PMPReadPDRamRegister(UINT8 *ReadBuffer, UINT16 Address, UINT16 Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0,nlength, nBytePosition;
    UINT8 nReadSpaceAvblCount;
    UINT16 RefAddr = Address;

    /*Reset/Abort any previous commands.*/
    param32_1.Val = PRAM_RW_ABORT_MASK;                                                 


        PMPWriteDWord (PRAM_READ_CMD_REG, param32_1.Val);
   
        /*The host should not modify this field unless the PRAM Read Busy
        (PRAM_READ_BUSY) bit is a 0.*/
        do
        {

  
            param32_1.Val = PMPReadDWord (PRAM_READ_CMD_REG);
  
        }while((param32_1.v[3] & PRAM_RW_BUSY_8B));

    /*Write address and length in the EtherCAT Process RAM Read Address and
     * Length Register (ECAT_PRAM_RD_ADDR_LEN)*/
    param32_1.w[0] = Address;
    param32_1.w[1] = Count;



        PMPWriteDWord (PRAM_READ_ADDR_LEN_REG, param32_1.Val);
   

        param32_1.Val = PMPReadDWord (HBI_INDEXED_DATA2_REG );
        /*Set PRAM Read Busy (PRAM_READ_BUSY) bit(-EtherCAT Process RAM Read Command Register)
         *  to start read operatrion*/

        param32_1.Val = PRAM_RW_BUSY_32B; /*TODO:replace with #defines*/

        PMPWriteDWord (PRAM_READ_CMD_REG, param32_1.Val);
 
        /*Read PRAM Read Data Available (PRAM_READ_AVAIL) bit is set*/
        do
        {
            
                    param32_1.Val = PMPReadDWord (PRAM_READ_CMD_REG);
          
        }while(!(param32_1.v[0] & IS_PRAM_SPACE_AVBL_MASK));

    nReadSpaceAvblCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Fifo registers are aliased address. In indexed it will read indexed data reg 0x04, but it will point to reg 0
     In other modes read 0x04 FIFO register since all registers are aliased*/

    
        param32_1.Val = PMPReadDWord (PRAM_READ_FIFO_REG);
  
    nReadSpaceAvblCount--;
    nBytePosition = (Address & 0x03);
    nlength = (4-nBytePosition) > Count ? Count:(4-nBytePosition);
    memcpy(ReadBuffer+i ,&param32_1.v[nBytePosition],nlength);
    Count-=nlength;
    i+=nlength;

    while(Count && nReadSpaceAvblCount)
    {
       
           param32_1.Val = PMPReadDWord (PRAM_READ_FIFO_REG);
    

        nlength = Count > 4 ? 4: Count;
        memcpy((ReadBuffer+i) ,&param32_1,nlength);

        i+=nlength;
        Count-=nlength;
        nReadSpaceAvblCount --;

        if (!nReadSpaceAvblCount)
        {
           
                    param32_1.Val = PMPReadDWord (PRAM_READ_CMD_REG);
          
            nReadSpaceAvblCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;
        }

    }
   
    return;
}
        

void PMPWritePDRamRegister(UINT8 *WriteBuffer, UINT16 Address, UINT16 Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0,nlength, nBytePosition,nWrtSpcAvlCount;
    UINT16 RefAddr = Address;
    
    /*Reset or Abort any previous commands.*/
    param32_1.Val = PRAM_RW_ABORT_MASK;                                                 /*TODO:replace with #defines*/
  
        PMPWriteDWord (PRAM_WRITE_CMD_REG, param32_1.Val);
   

        /*Make sure there is no previous write is pending
        (PRAM Write Busy) bit is a 0 */
        do
        {

   
            param32_1.Val = PMPReadDWord (PRAM_WRITE_CMD_REG);
 
        }while((param32_1.v[3] & PRAM_RW_BUSY_8B));

        /*Write Address and Length Register (ECAT_PRAM_WR_ADDR_LEN) with the
        starting byte address and length)*/
        param32_1.w[0] = Address;
        param32_1.w[1] = Count;

  
        PMPWriteDWord (PRAM_WRITE_ADDR_LEN_REG, param32_1.Val);
   

    /*write to the EtherCAT Process RAM Write Command Register (ECAT_PRAM_WR_CMD) with the  PRAM Write Busy
    (PRAM_WRITE_BUSY) bit set*/

    param32_1.Val = PRAM_RW_BUSY_32B; 

   
        PMPWriteDWord (PRAM_WRITE_CMD_REG, param32_1.Val);
  

        /*Read PRAM write Data Available (PRAM_READ_AVAIL) bit is set*/
        do
        {
   
            param32_1.Val = PMPReadDWord (PRAM_WRITE_CMD_REG);

        }while(!(param32_1.v[0] & IS_PRAM_SPACE_AVBL_MASK));

    /*Check write data available count*/
    nWrtSpcAvlCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Write data to Write FIFO) */ 
    /*get the byte lenth for first read*/
    nBytePosition = (Address & 0x03);

    nlength = (4-nBytePosition) > Count ? Count:(4-nBytePosition);

    param32_1.Val = 0;
    memcpy(&param32_1.v[nBytePosition],WriteBuffer+i, nlength);

  
        PMPWriteDWord (PRAM_WRITE_FIFO_REG,param32_1.Val);

    nWrtSpcAvlCount--;
    Count-=nlength;
    i+=nlength;


    while(nWrtSpcAvlCount && Count)
    {
        nlength = Count > 4 ? 4: Count;
        param32_1.Val = 0;
        memcpy(&param32_1, (WriteBuffer+i), nlength);

   
        PMPWriteDWord (PRAM_WRITE_FIFO_REG,param32_1.Val);
   
			i+=nlength;
        Count-=nlength;
        nWrtSpcAvlCount--;

        if (!nWrtSpcAvlCount)
        {
            
                    param32_1.Val = PMPReadDWord (PRAM_WRITE_CMD_REG);
           
            /*Check write data available count*/
            nWrtSpcAvlCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;
        }
    }
    return;
}
void PMPReadDRegister(UINT8 *ReadBuffer, UINT16 Address, UINT16 Count)
{
    if (Address >= 0x1000)
    {
         PMPReadPDRamRegister(ReadBuffer, Address,Count);
    }
    else
    {
         PMPReadRegUsingCSR(ReadBuffer, Address,Count);
    }
}
void PMPWriteRegister( UINT8 *WriteBuffer, UINT16 Address, UINT16 Count)
{
   
   if (Address >= 0x1000)
   {
		PMPWritePDRamRegister(WriteBuffer, Address,Count);
   }
   else
   {
		PMPWriteRegUsingCSR(WriteBuffer, Address,Count);
   }
    
}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
