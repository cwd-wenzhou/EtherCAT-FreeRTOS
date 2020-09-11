/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:              SPI2.c
** Descriptions:           SPI2 hardware driver
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "SPI2.h"
#include "ecat_def.h"
#include "el9800hw.h"
/*******************************************************************************
* Function Name  : SPI1_Init
* Description    : SPI1 initialize
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void SPI2_Init(void) 
{ 
  SPI_InitTypeDef  SPI_InitStructure;
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  /* DISABLE SPI2 */ 
  SPI_Cmd(SPI2, DISABLE); 
  /* SPI2 Config -------------------------------------------------------------*/ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 
  SPI_Init(SPI2, &SPI_InitStructure); 
  /* Enable SPI2 */ 
  SPI_Cmd(SPI2, ENABLE); 
	//SPI1_ReadWriteByte(0xff);// 发送复位命令??
} 


u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据	
 		    
}
/*******************************************************************************
* Function Name  : SPI2_GPIO_Init
* Description    : SPI2 Port initialize
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
 * 最后修改日期： 2020.8.20
*******************************************************************************/
void SPI2_GPIO_Init(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  | GPIO_Pin_14 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  /* CS */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* IRQ */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13复用为 SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB14复用为 SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15复用为 SPI2
  SPI2_Init(); 
} 



/*******************************************************************************
* Function Name  : WR_CMD
* Description    : Read and Wire data to ET1100 
* Input          : - cmd: the data send to ET1100
* Output         : none
* Return         : temp: the data read from ET1100
* Attention		 : None
*******************************************************************************/
uint8_t WR_CMD (uint8_t cmd)  
{ 
//  SPI_I2S_SendData(SPI1,cmd); 
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
//	temp =  SPI_I2S_ReceiveData(SPI1); 
//	return temp;
//	
		 uint8_t temp; 

  /* Wait for SPI2 Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send SPI2 data */ 
  SPI_I2S_SendData(SPI2,cmd); 
  /* Wait for SPI2 data reception */ 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read SPI2 received data */ 
	temp =  SPI_I2S_ReceiveData(SPI2); 

	return temp;
	
//	SPI1->DR = cmd;        //发送数据
//	while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0x00) ;
//	return SPI1->DR;
//		
} 

 
/*******************************************************************************
* Function Name  : SPIWrite
* Description    : Wire byte data to lan9252
* Input          : - data: the data send to ET1100
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIWrite(UINT8 data)
{
   WR_CMD(data);
}

/*******************************************************************************
* Function Name  : SPIRead
* Description    : read byte data from lan9252
* Input          : none
* Output         : none
* Return         : data: read from lan9252
* Attention		 : None
*******************************************************************************/
UINT8 SPIRead(void)
{
    UINT8 data;
    data = WR_CMD(0);
    return (data);
}


/*******************************************************************************
* Function Name  : SPIReadDWord
* Description    : read word data from lan9252
* Input          : Address：the addreoss to be read from lan9252
* Output         : none
* Return         : data: word data from lan9252
* Attention		 : None
*******************************************************************************/
uint32_t SPIReadDWord (uint16_t Address)
{
    UINT32_VAL dwResult;
    UINT16_VAL wAddr;
		
    wAddr.Val  = Address;
    //Assert CS line
    CSLOW();
    //Write Command
     SPIWriteByte(CMD_FAST_READ);
    //Write Address
    SPIWriteByte(wAddr.byte.HB);
    SPIWriteByte(wAddr.byte.LB);
    
    //Dummy Byte
    SPIWriteByte(CMD_FAST_READ_DUMMY);
    //Read Bytes
    dwResult.byte.LB = SPIReadByte();
    dwResult.byte.HB = SPIReadByte();
    dwResult.byte.UB = SPIReadByte();
    dwResult.byte.MB = SPIReadByte();
    //De-Assert CS line
    CSHIGH();
   
    return dwResult.Val;
}
/*******************************************************************************
* Function Name  : SPISendAddr
* Description    : write address to lan9252
* Input          : Address：the address write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPISendAddr (uint16_t Address)
{
    UINT16_VAL wAddr;

    wAddr.Val  = Address;
    //Write Address
    SPIWriteByte(wAddr.byte.HB);
    SPIWriteByte(wAddr.byte.LB);
}

/*******************************************************************************
* Function Name  : SPIReadBurstMode
* Description    : Read word from lan9252 in burst mode
* Input          : none
* Output         : none
* Return         : word data from lan9252
* Attention		 : None
*******************************************************************************/
UINT32 SPIReadBurstMode ()
{
    UINT32_VAL dwResult;
    //Read Bytes
    dwResult.byte.LB = SPIReadByte();
    dwResult.byte.HB = SPIReadByte();
    dwResult.byte.UB = SPIReadByte();
    dwResult.byte.MB = SPIReadByte();
    
    return dwResult.Val;
}

/*******************************************************************************
* Function Name  : SPIWriteBurstMode
* Description    : write data to lan9252 in burst mode
* Input          : val：the data write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIWriteBurstMode (uint32_t Val)
{
    UINT32_VAL dwData;
    dwData.Val = Val;
    
    //Write Bytes
    SPIWriteByte(dwData.byte.LB);
    SPIWriteByte(dwData.byte.HB);
    SPIWriteByte(dwData.byte.UB);
    SPIWriteByte(dwData.byte.MB);
}
/*******************************************************************************
* Function Name  : SPIWriteDWord
* Description    : write Word lan9252 in 
* Input          : Address the address write to lan9252
										val：the data write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIWriteDWord (uint16_t Address, uint32_t Val)
{
    UINT32_VAL dwData;
    UINT16_VAL wAddr;

    wAddr.Val  = Address;
    dwData.Val = Val;
    //Assert CS line
    CSLOW();
    //Write Command
    SPIWriteByte(CMD_SERIAL_WRITE);
    //Write Address
    SPIWriteByte(wAddr.byte.HB);
    SPIWriteByte(wAddr.byte.LB);
    //Write Bytes
    SPIWriteByte(dwData.byte.LB);
    SPIWriteByte(dwData.byte.HB);
    SPIWriteByte(dwData.byte.UB);
    SPIWriteByte(dwData.byte.MB);

    //De-Assert CS line
    CSHIGH();
}

/*******************************************************************************
* Function Name  : SPIReadRegUsingCSR
* Description    : Read data from lan9252 use CSR
* Input          : ReadBuffer:data buf 
									 Address：the reg address write to lan9252
										Count:the number write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIReadRegUsingCSR(uint8_t *ReadBuffer, uint16_t Address, uint8_t Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0;
    UINT16_VAL wAddr;
    wAddr.Val = Address;

    param32_1.v[0] = wAddr.byte.LB;
    param32_1.v[1] = wAddr.byte.HB;
    param32_1.v[2] = Count;
    param32_1.v[3] = ESC_READ_BYTE;

    SPIWriteDWord (ESC_CSR_CMD_REG, param32_1.Val);

    do
    {
        param32_1.Val = SPIReadDWord (ESC_CSR_CMD_REG);
		
    }while(param32_1.v[3] & ESC_CSR_BUSY);

    param32_1.Val = SPIReadDWord (ESC_CSR_DATA_REG);

    
    for(i=0;i<Count;i++)
         ReadBuffer[i] = param32_1.v[i];
   
    return;
}

/*******************************************************************************
* Function Name  : SPIWriteRegUsingCSR
* Description    : write data to lan9252 use CSR
* Input          : ReadBuffer:data buf 
									 Address：the reg address write to lan9252
										Count:the number write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIWriteRegUsingCSR( uint8_t *WriteBuffer, uint16_t Address, uint8_t Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0;
    UINT16_VAL wAddr;

    for(i=0;i<Count;i++)
         param32_1.v[i] = WriteBuffer[i];

    SPIWriteDWord (ESC_CSR_DATA_REG, param32_1.Val);


    wAddr.Val = Address;

    param32_1.v[0] = wAddr.byte.LB;
    param32_1.v[1] = wAddr.byte.HB;
    param32_1.v[2] = Count;
    param32_1.v[3] = ESC_WRITE_BYTE;

    SPIWriteDWord (0x304, param32_1.Val);
    do
    {
        param32_1.Val = SPIReadDWord (0x304);

    }while(param32_1.v[3] & ESC_CSR_BUSY);

    return;
}


/*******************************************************************************
* Function Name  : SPIReadPDRamRegister
* Description    : read data from lan9252 pd ram
* Input          : ReadBuffer:data buf 
									 Address：the reg address write to lan9252
										Count:the number write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIReadPDRamRegister(uint8_t *ReadBuffer, uint16_t Address, uint16_t Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0,nlength, nBytePosition;
    UINT8 nReadSpaceAvblCount;

//    /*Reset/Abort any previous commands.*/
    param32_1.Val = (unsigned long int)PRAM_RW_ABORT_MASK;                                                 

    SPIWriteDWord (PRAM_READ_CMD_REG, param32_1.Val);

    /*The host should not modify this field unless the PRAM Read Busy
    (PRAM_READ_BUSY) bit is a 0.*/
    do
    {

			param32_1.Val = SPIReadDWord (PRAM_READ_CMD_REG);

    }while((param32_1.v[3] & PRAM_RW_BUSY_8B));

    /*Write address and length in the EtherCAT Process RAM Read Address and
     * Length Register (ECAT_PRAM_RD_ADDR_LEN)*/
    param32_1.w[0] = Address;
    param32_1.w[1] = Count;

    SPIWriteDWord (PRAM_READ_ADDR_LEN_REG, param32_1.Val);


    /*Set PRAM Read Busy (PRAM_READ_BUSY) bit(-EtherCAT Process RAM Read Command Register)
     *  to start read operatrion*/

    param32_1.Val = PRAM_RW_BUSY_32B; /*TODO:replace with #defines*/

    SPIWriteDWord (PRAM_READ_CMD_REG, param32_1.Val);

		

    /*Read PRAM Read Data Available (PRAM_READ_AVAIL) bit is set*/
    do
    {			
      //  param32_1.Val
			param32_1.Val = SPIReadDWord (PRAM_READ_CMD_REG);
		

    }while(!(param32_1.v[0] & IS_PRAM_SPACE_AVBL_MASK));

    nReadSpaceAvblCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Fifo registers are aliased address. In indexed it will read indexed data reg 0x04, but it will point to reg 0
     In other modes read 0x04 FIFO register since all registers are aliased*/

    /*get the UINT8 lenth for first read*/
    //Auto increment is supported in SPIO
    param32_1.Val = SPIReadDWord (PRAM_READ_FIFO_REG);
    nReadSpaceAvblCount--;
    nBytePosition = (Address & 0x03);
    nlength = (4-nBytePosition) > Count ? Count:(4-nBytePosition);
    memcpy(ReadBuffer+i ,&param32_1.v[nBytePosition],nlength);
    Count-=nlength;
    i+=nlength;

    //Lets do it in auto increment mode
    CSLOW();

    //Write Command
    SPIWriteByte(CMD_FAST_READ);

    SPISendAddr(PRAM_READ_FIFO_REG);
    
    //Dummy Byte
    SPIWriteByte(CMD_FAST_READ_DUMMY);

    while(Count)
    {
        param32_1.Val = SPIReadBurstMode();

        nlength = Count > 4 ? 4: Count;
        memcpy((ReadBuffer+i) ,&param32_1,nlength);

        i+=nlength;
        Count-=nlength;
        nReadSpaceAvblCount --;
    }

    CSHIGH();

    return;
}
/*******************************************************************************
* Function Name  : SPIWritePDRamRegister
* Description    : write data from lan9252 pd ram
* Input          : ReadBuffer:data buf 
									 Address：the reg address write to lan9252
										Count:the number write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIWritePDRamRegister(uint8_t *WriteBuffer, uint16_t Address, uint16_t Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0,nlength, nBytePosition,nWrtSpcAvlCount;

//    /*Reset or Abort any previous commands.*/
    param32_1.Val = PRAM_RW_ABORT_MASK;                                                

    SPIWriteDWord (PRAM_WRITE_CMD_REG, param32_1.Val);

    /*Make sure there is no previous write is pending
    (PRAM Write Busy) bit is a 0 */
    do
    {			
        param32_1.Val = SPIReadDWord (PRAM_WRITE_CMD_REG);

    }while((param32_1.v[3] & PRAM_RW_BUSY_8B));

    /*Write Address and Length Register (ECAT_PRAM_WR_ADDR_LEN) with the
    starting UINT8 address and length)*/
    param32_1.w[0] = Address;
    param32_1.w[1] = Count;

    SPIWriteDWord (PRAM_WRITE_ADDR_LEN_REG, param32_1.Val);

    /*write to the EtherCAT Process RAM Write Command Register (ECAT_PRAM_WR_CMD) with the  PRAM Write Busy
    (PRAM_WRITE_BUSY) bit set*/

    param32_1.Val = PRAM_RW_BUSY_32B; /*TODO:replace with #defines*/

    SPIWriteDWord (PRAM_WRITE_CMD_REG, param32_1.Val);
		

    /*Read PRAM write Data Available (PRAM_READ_AVAIL) bit is set*/
    do
    {	
       param32_1.Val = SPIReadDWord (PRAM_WRITE_CMD_REG);

    }while(!(param32_1.v[0] & IS_PRAM_SPACE_AVBL_MASK));

    /*Check write data available count*/
    nWrtSpcAvlCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Write data to Write FIFO) */ 
    /*get the byte lenth for first read*/
    nBytePosition = (Address & 0x03);

    nlength = (4-nBytePosition) > Count ? Count:(4-nBytePosition);

    param32_1.Val = 0;
    memcpy(&param32_1.v[nBytePosition],WriteBuffer+i, nlength);

    SPIWriteDWord (PRAM_WRITE_FIFO_REG,param32_1.Val);

    nWrtSpcAvlCount--;
    Count-=nlength;
    i+=nlength;

    //Auto increment mode
    CSLOW();

    //Write Command
    SPIWriteByte(CMD_SERIAL_WRITE);

    SPISendAddr(PRAM_WRITE_FIFO_REG);

    while(Count)
    {
        nlength = Count > 4 ? 4: Count;
        param32_1.Val = 0;
        memcpy(&param32_1, (WriteBuffer+i), nlength);

        SPIWriteBurstMode (param32_1.Val);
        i+=nlength;
        Count-=nlength;
        nWrtSpcAvlCount--;
    }

    CSHIGH();
    return;
}


/*******************************************************************************
* Function Name  : SPIReadDRegister
* Description    : read reg from lan9252 pd ram
* Input          : ReadBuffer:data buf 
									 Address：the reg address write to lan9252
										Count:the number write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIReadDRegister(uint8_t *ReadBuffer, uint16_t Address, uint16_t Count)
{
    if (Address >= 0x1000)
    {
         SPIReadPDRamRegister(ReadBuffer, Address,Count);
    }
    else
    {
         SPIReadRegUsingCSR(ReadBuffer, Address,Count);
    }
}

/*******************************************************************************
* Function Name  : SPIWriteRegister
* Description    : write reg from lan9252 pd ram
* Input          : ReadBuffer:data buf 
									 Address：the reg address write to lan9252
										Count:the number write to lan9252
* Output         : none
* Return         : none: 
* Attention		 : None
*******************************************************************************/
void SPIWriteRegister( uint8_t *WriteBuffer, uint16_t Address, uint16_t Count)
{
   
   if (Address >= 0x1000)
   {
		SPIWritePDRamRegister(WriteBuffer, Address,Count);
   }
   else
   {
		SPIWriteRegUsingCSR(WriteBuffer, Address,Count);
   }
    
}



/*
 * 描    述：SPIReadDWord_test
 * 功    能：读一个32数据
 * 入口参数：Address---读操作地址
 * 出口参数：读到的数据
 */ 
unsigned long SPIReadDWord_test(unsigned short Address)
{
	UINT32_VAL dwResult;
	UINT16_VAL wAddr;

	wAddr.Val  = Address;
	//Assert CS line
	CSLOW();
	//Write Command
	SPIWriteByte(CMD_FAST_READ);
    //Write Address
	SPIWriteByte(wAddr.byte.HB);
	SPIWriteByte(wAddr.byte.LB);
			
	//Dummy Byte
	SPIWriteByte(CMD_FAST_READ_DUMMY);
    //Read Bytes
    dwResult.byte.LB = SPIReadByte();
    dwResult.byte.HB = SPIReadByte();
    dwResult.byte.UB = SPIReadByte();
    dwResult.byte.MB = SPIReadByte();
    //De-Assert CS line
		CSHIGH();
		return dwResult.Val;
}
/*
 * 描    述：LAN9252_ReadID
 * 功    能：CSR读操作读LAN9252的芯片ID
 * 入口参数：无
 * 出口参数：读到的芯片ID
 */
unsigned long LAN9252_ReadID(void)
{
	UINT8 Temp[10] = {0,0,0,0,0,0,0,0,0,0};	  
	SPIReadRegUsingCSR(Temp, 0x0e02, 2);	    
	return (Temp[0] | ((u32)Temp[1] << 8) |
		((u32)Temp[2] << 16) | ((u32)Temp[3] << 24));	
	
}   
/*
 * 描    述：mem_test
 * 功    能：测试PDI接口
 * 入口参数：无
 * 出口参数：无
 */
void mem_test(void)
{
	//UINT8 *pTmpData=0x02;
	//SPIWriteRegister(pTmpData, 0x0e02, 1);
	unsigned long temp = 0;
	//------------------------------------读物理地址64H=87654321
	temp = SPIReadDWord_test(0x64);
	temp = SPIReadDWord_test(0x64);
//	printf(temp == 0x87654321 ? "\n\r  mem_test:PASSED! const = %x ":"\n\r  test:FAILED! const = %x ", (int)temp);
    
	if(temp != 0x87654321)
	{
//		printf("stop();");
		//while(1);
	}
	
	//------------------------------------读虚拟地址0e02H=9252
	temp = LAN9252_ReadID();
//	printf(temp == 0x9252 ? "   ID = %x\n\r":"\n\r  test:FAILED! ID = %x\n\r", (int)temp);
	
	if(temp != 0x9252)
	{
//		printf("stop();");
		while(1);
	}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/