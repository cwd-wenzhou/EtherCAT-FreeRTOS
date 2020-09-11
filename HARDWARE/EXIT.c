/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:              EXIT.c
** Descriptions:           Exit hardware driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:              陈韦达
** Created date:            2020.8.10
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

#include "stm32f4xx.h"
/*******************************************************************************
* Function Name  :EXTI0_Configuration
* Description    : EXTI0_Configuration初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*最后修改时间  : 2020.8.8
*******************************************************************************/
void IRQ_EXTI0_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure; 
	  EXTI_InitTypeDef EXTI_InitStructure;
	
	  /* Configure PC.00 as  interrupt port -------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO  , ENABLE); 				STM32F107 用这句		 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
    /* Configure PC.00 as  interrupt port -------------------------*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);//PC0 连接到中断线2
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);  STM32F107 用这句
  EXTI_ClearITPendingBit(EXTI_Line0);
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Rising_Falling;// EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 ;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
		  /* Configure NVIC Interrupt  -------------------------*/	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name  : EXTI1_Configuration
* Description    : EXTI1_Configuration 初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*最后修改时间  : 2020.8.8
*******************************************************************************/
void SYNC0_EXTI1_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;   
	  EXTI_InitTypeDef EXTI_InitStructure;
		  /* Configure PB.01 as  input -------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO  , ENABLE); 		STM32F107 用这句				 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /* Configure PB.01 as  interrupt port -------------------------*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);//PC0 连接到中断线2
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);   STM32F107 用这句
  EXTI_ClearITPendingBit(EXTI_Line3);
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_Line =  EXTI_Line3 ;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
		  /* Configure NVIC Interrupt  -------------------------*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}

/*******************************************************************************
* Function Name  : EXTI3_Configuration
* Description    : EXTI3_Configuration 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*最后修改时间  : 2020.8.8
*******************************************************************************/
void SYNC1_EXTI2_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;   
	  EXTI_InitTypeDef EXTI_InitStructure;
	  /* Configure PA.03 as  input -------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO  , ENABLE); 						  STM32F107 用这句 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure PA.03 as  interrupt port -------------------------*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PC0 连接到中断线2
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);     STM32F107 用这句
  EXTI_ClearITPendingBit(EXTI_Line1);
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	  /* Configure NVIC Interrupt  -------------------------*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}
