
/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "ecat_def.h"
#if EL9800_HW
#include "SPI2.h"
#include "ecatslv.h"

#define    _EL9800HW_ 1
#include "el9800hw.h"
#undef    _EL9800HW_
/* ECATCHANGE_START(V5.11) ECAT10*/
/*remove definition of _EL9800HW_ (#ifdef is used in el9800hw.h)*/
/* ECATCHANGE_END(V5.11) ECAT10*/

#include "ecatappl.h"
#include "sys.h"


#include "includes.h"
void mem_test(void);
/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

typedef union
{
    unsigned short    Word;
    unsigned char    Byte[2];
} UBYTETOWORD;

typedef union 
{
    UINT8           Byte[2];
    UINT16          Word;
}
UALEVENT;

/*-----------------------------------------------------------------------------------------
------
------    SPI defines/macros
------
-----------------------------------------------------------------------------------------*/
#define SPI_DEACTIVE                    1
#define SPI_ACTIVE                        0


#if INTERRUPTS_SUPPORTED
/*-----------------------------------------------------------------------------------------
------
------    Global Interrupt setting
------
-----------------------------------------------------------------------------------------*/

#define 	DISABLE_GLOBAL_INT           			  __disable_irq()					
#define 	ENABLE_GLOBAL_INT           		    __enable_irq()				
#define    DISABLE_AL_EVENT_INT          DISABLE_GLOBAL_INT
#define    ENABLE_AL_EVENT_INT           ENABLE_GLOBAL_INT



/*-----------------------------------------------------------------------------------------
------
------    ESC Interrupt
------
-----------------------------------------------------------------------------------------*/
#if AL_EVENT_ENABLED
#define    INIT_ESC_INT           IRQ_EXTI0_Configuration();					
#define    EcatIsr                EXTI0_IRQHandler
#define    ACK_ESC_INT         		EXTI_ClearITPendingBit(EXTI_Line0);  
#define IS_ESC_INT_ACTIVE					 
#endif //#if AL_EVENT_ENABLED


/*-----------------------------------------------------------------------------------------
------
------    SYNC0 Interrupt
------
-----------------------------------------------------------------------------------------*/
#if DC_SUPPORTED && _STM32_IO8
#define    INIT_SYNC0_INT                 SYNC0_EXTI1_Configuration();		
#define    Sync0Isr                       EXTI3_IRQHandler 
#define    DISABLE_SYNC0_INT              NVIC_DisableIRQ(EXTI3_IRQn);	 
#define    ENABLE_SYNC0_INT               NVIC_EnableIRQ(EXTI3_IRQn);	
#define    ACK_SYNC0_INT                  EXTI_ClearITPendingBit(EXTI_Line3);
#define    IS_SYNC0_INT_ACTIVE             

																					
/*ECATCHANGE_START(V5.10) HW3*/

#define    INIT_SYNC1_INT                  SYNC1_EXTI2_Configuration();
#define    Sync1Isr                        EXTI1_IRQHandler
#define    DISABLE_SYNC1_INT               NVIC_DisableIRQ(EXTI1_IRQn);
#define    ENABLE_SYNC1_INT                NVIC_EnableIRQ(EXTI1_IRQn); 
#define    ACK_SYNC1_INT                   EXTI_ClearITPendingBit(EXTI_Line1);
#define    IS_SYNC1_INT_ACTIVE              

/*ECATCHANGE_END(V5.10) HW3*/

#endif //#if DC_SUPPORTED && _STM32_IO8

#endif	//#if INTERRUPTS_SUPPORTED
/*-----------------------------------------------------------------------------------------
------
------    Hardware timer
------
-----------------------------------------------------------------------------------------*/
#if _STM32_IO8
#if ECAT_TIMER_INT
#define ECAT_TIMER_INT_STATE       
#define ECAT_TIMER_ACK_INT        		 	TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);	
#define    TimerIsr                     TIM2_IRQHandler					
#define    ENABLE_ECAT_TIMER_INT        NVIC_EnableIRQ(TIM2_IRQn) ;	
#define    DISABLE_ECAT_TIMER_INT       NVIC_DisableIRQ(TIM2_IRQn) ;

#define INIT_ECAT_TIMER           			TIM_Configuration(10) ;   

#define STOP_ECAT_TIMER            			DISABLE_ECAT_TIMER_INT;/*disable timer interrupt*/ \

#define START_ECAT_TIMER          			ENABLE_ECAT_TIMER_INT


#else    //#if ECAT_TIMER_INT

#define INIT_ECAT_TIMER      					TIM_Configuration(10);

#define STOP_ECAT_TIMER              TIM_Cmd(TIM2, DISABLE);		 

#define START_ECAT_TIMER              TIM_Cmd(TIM2, ENABLE);   			

#endif //#else #if ECAT_TIMER_INT

#elif _STM32_IO4

#if !ECAT_TIMER_INT
#define    ENABLE_ECAT_TIMER_INT       NVIC_EnableIRQ(TIM2_IRQn) ;	
#define    DISABLE_ECAT_TIMER_INT      NVIC_DisableIRQ(TIM2_IRQn) ;
#define INIT_ECAT_TIMER               TIM_Configuration(10) ;	
#define STOP_ECAT_TIMER              	TIM_Cmd(TIM2, DISABLE);	
#define START_ECAT_TIMER           		TIM_Cmd(TIM2, ENABLE);			

#else    //#if !ECAT_TIMER_INT

#warning "define Timer Interrupt Macros"

#endif //#else #if !ECAT_TIMER_INT
#endif //#elif _STM32_IO4

/*-----------------------------------------------------------------------------------------
------
------    Configuration Bits
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    LED defines
------
-----------------------------------------------------------------------------------------*/
#if _STM32_IO8
// EtherCAT Status LEDs -> StateMachine
#define LED_ECATGREEN               
#define LED_ECATRED                   
#endif //_STM32_IO8


/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc

/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/


/*******************************************************************************
  Function:
    void GetInterruptRegister(void)

  Summary:
    The function operates a SPI access without addressing.

  Description:
    The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
    It will be saved in the global "EscALEvent"
  *****************************************************************************/
static void GetInterruptRegister(void)
{
      DISABLE_AL_EVENT_INT;
      HW_EscReadIsr((MEM_ADDR *)&EscALEvent.Word, 0x220, 2);
      ENABLE_AL_EVENT_INT;

}


/*******************************************************************************
  Function:
    void ISR_GetInterruptRegister(void)

  Summary:
    The function operates a SPI access without addressing.
        Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegsiter()"

  Description:
    The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
  *****************************************************************************/

static void ISR_GetInterruptRegister(void)
{
    HW_EscReadIsr((MEM_ADDR *)&EscALEvent.Word, 0x220, 2);
}

/*--------------------------------------------------------------------------------------
------
------    exported hardware access functions
------
--------------------------------------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : init the led and swtich port
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
* 最后修改日期   : 2020.8.8
*******************************************************************************/
void GPIO_Config(void) 
{ 
	 GPIO_InitTypeDef GPIO_InitStructure;
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH, ENABLE);
	
	 /* configration the LED pin */	
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;  
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_ResetBits(GPIOG,GPIO_Pin_8);			
	 GPIO_Init(GPIOG, &GPIO_InitStructure);  //暂时不知道使能哪几个pin，先放在这里。
	
} 


/*
 * 描    述：LAN9252_reset
 * 功    能：复位LAN9252此芯片
 * 入口参数：无
 * 出口参数：无
 * 最后修改日期： 2020.8.8
 */ 
void LAN9252_reset(void)// PF8为#RST
{		
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
 

	//while(1)
	{
		GPIO_WriteBit(GPIOF, GPIO_Pin_8, Bit_RESET);
		Delayms_666M(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_8, Bit_SET);
		Delayms_666M(1000);
	}

} 
/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function intialize the Process Data Interface (PDI) and the host controller.
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 HW_Init(void)
{
	UINT16 intMask;
	UINT32 data;
	/*initialize the led and switch port*/
  GPIO_Config();
//	LAN9252_reset();// PF8为#RST
	/* initialize the SSP registers for the ESC SPI */
	SPI2_GPIO_Init();
	
	//mem_test();// 测试PDI接口
	
	/*initialize ADC configration*/
//	ADC_Configuration();

	do
	{
			intMask = 0x0093;
			HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
			intMask = 0;
			HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
	} while (intMask!= 0x0093);
		

		
		//IRQ enable,IRQ polarity, IRQ buffer type in Interrupt Configuration register.
    //Wrte 0x54 - 0x00000101
    data = 0x00000101;
 
    SPIWriteDWord (0x54,data);
    

    //Write in Interrupt Enable register -->
    //Write 0x5c - 0x00000001
    data = 0x00000001;
    SPIWriteDWord (0x5C, data);
    

    SPIReadDWord(0x58);

		intMask = 0x00;
	  
    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
		
			

#if AL_EVENT_ENABLED



    INIT_ESC_INT;
    ENABLE_ESC_INT();
#endif

#if DC_SUPPORTED&& _STM32_IO8
    INIT_SYNC0_INT
    INIT_SYNC1_INT

    ENABLE_SYNC0_INT;
    ENABLE_SYNC1_INT;
#endif

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;
  
	
#if INTERRUPTS_SUPPORTED
    /* enable all interrupts */
    ENABLE_GLOBAL_INT;
#endif


    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be implemented if hardware resources need to be release
        when the sample application stops
*////////////////////////////////////////////////////////////////////////////////////////
void HW_Release(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}
#if INTERRUPTS_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_GetALEventRegister()"
*////////////////////////////////////////////////////////////////////////////////////////
#if _STM32_IO4  && AL_EVENT_ENABLED
/* the pragma interrupt_level is used to tell the compiler that these functions will not
   be called at the same time from the main function and the interrupt routine */
//#pragma interrupt_level 1
#endif
UINT16 HW_GetALEventRegister_Isr(void)
{
     ISR_GetInterruptRegister();
    return EscALEvent.Word;
}
#endif


#if UC_SET_ECAT_LED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param RunLed            desired EtherCAT Run led state
 \param ErrLed            desired EtherCAT Error led state

  \brief    This function updates the EtherCAT run and error led
*////////////////////////////////////////////////////////////////////////////////////////
void HW_SetLed(UINT8 RunLed,UINT8 ErrLed)
{
#if _STM32_IO8
 //     LED_ECATGREEN = RunLed;
//      LED_ECATRED   = ErrLed;
#endif
}
#endif //#if UC_SET_ECAT_LED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  This function operates the SPI read access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    /* HBu 24.01.06: if the SPI will be read by an interrupt routine too the
                     mailbox reading may be interrupted but an interrupted
                     reading will remain in a SPI transmission fault that will
                     reset the internal Sync Manager status. Therefore the reading
                     will be divided in 1-byte reads with disabled interrupt */
    UINT16 i;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be read */
    while ( Len > 0 )
    {
        if (Address >= 0x1000)
        {
            i = Len;
        }
        else
        {
            i= (Len > 4) ? 4 : Len;

            if(Address & 01)
            {
               i=1;
            }
            else if (Address & 02)
            {
               i= (i&1) ? 1:2;
            }
            else if (i == 03)
            {
                i=1;
            }
        }

        DISABLE_AL_EVENT_INT;

       SPIReadDRegister(pTmpData,Address,i);

      
       ENABLE_AL_EVENT_INT;

        Len -= i;
        pTmpData += i;
        Address += i;
    }


}
#if INTERRUPTS_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

\brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_EscRead()"
*////////////////////////////////////////////////////////////////////////////////////////
#if _STM32_IO4  && AL_EVENT_ENABLED
/* the pragma interrupt_level is used to tell the compiler that these functions will not
   be called at the same time from the main function and the interrupt routine */
//#pragma interrupt_level 1
#endif
 void HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{

   UINT16 i;
   UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */

    /* loop for all bytes to be read */
   while ( Len > 0 )
   {

        if (Address >= 0x1000)
        {
            i = Len;
        }
        else
        {
            i= (Len > 4) ? 4 : Len;

            if(Address & 01)
            {
               i=1;
            }
            else if (Address & 02)
            {
               i= (i&1) ? 1:2;
            }
            else if (i == 03)
            {
                i=1;
            }
        }

        SPIReadDRegister(pTmpData, Address,i);

        Len -= i;
        pTmpData += i;
        Address += i;
    }
   
}
#endif //#if INTERRUPTS_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

  \brief  This function operates the SPI write access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{

    UINT16 i;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be written */
    while ( Len )
    {

        if (Address >= 0x1000)
        {
            i = Len;
        }
        else
        {
            i= (Len > 4) ? 4 : Len;

            if(Address & 01)
            {
               i=1;
            }
            else if (Address & 02)
            {
               i= (i&1) ? 1:2;
            }
            else if (i == 03)
            {
                i=1;
            }
        }

        DISABLE_AL_EVENT_INT;
       
        /* start transmission */

        SPIWriteRegister(pTmpData, Address, i);


        ENABLE_AL_EVENT_INT;

       
   
        /* next address */
        Len -= i;
        pTmpData += i;
        Address += i;

    }

}
#if INTERRUPTS_SUPPORTED
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  The SPI PDI requires an extra ESC write access functions from interrupts service routines.
        The behaviour is equal to "HW_EscWrite()"
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{

    UINT16 i ;
    UINT8 *pTmpData = (UINT8 *)pData;

  
    /* loop for all bytes to be written */
    while ( Len )
    {

        if (Address >= 0x1000)
        {
            i = Len;
        }
        else
        {
            i= (Len > 4) ? 4 : Len;

            if(Address & 01)
            {
               i=1;
            }
            else if (Address & 02)
            {
               i= (i&1) ? 1:2;
            }
            else if (i == 03)
            {
                i=1;
            }
        }
        
       /* start transmission */


       SPIWriteRegister(pTmpData, Address, i);

       
       /* next address */
        Len -= i;
        pTmpData += i;
        Address += i;
    }

}

#endif

/*
 * 描述:  EcatIsr
 * 功能:  IRQ中断服务函数
 * 入口:  无
 * 出口:  无
 */
void  EcatIsr(void)
{
   PDI_Isr();

   /* reset the interrupt flag */
   ACK_ESC_INT;
	

}
#endif     // AL_EVENT_ENABLED



#if DC_SUPPORTED&& _STM32_IO8
/*
 * 描述:  Sync0Isr
 * 功能:  SYNC0中断服务函数
 * 入口:  无
 * 出口:  无
 */
void Sync0Isr(void)
{

   Sync0_Isr();

   ACK_SYNC0_INT;

}
/*ECATCHANGE_START(V5.10) HW3*/
/*
 * 描述:  Sync1Isr
 * 功能:  SYNC1中断服务函数
 * 入口:  无
 * 出口:  无
 */
void Sync1Isr(void)
{	
   DISABLE_ESC_INT();
   Sync1_Isr();
	 ACK_SYNC1_INT;
   ENABLE_ESC_INT();

}
/*ECATCHANGE_END(V5.10) HW3*/
#endif

#if _STM32_IO8 && ECAT_TIMER_INT

/*
 * 描述:  TimerIsr
 * 功能:  timer中断服务函数---定时1ms---2000个ticks
 * 入口:  无
 * 出口:  无
 */
void TimerIsr(void)
{		
		ECAT_CheckTimer();

		ECAT_TIMER_ACK_INT;
	
}

#endif

//#endif //#if EL9800_HW
/** @} */




