/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               Timer.c
** Descriptions:          timer2  hardware driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:               cwd
** Created date:            2020-8-25
** Version:                 	v1.0
** Descriptions:           The original version
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
* Function Name  : TIM_Configuration
* Description    : TIM_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM_Configuration(uint8_t period)		//100us
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructure.TIM_Period= period*200 ; //200;		 					/* 自动重装载寄存器周期的值(计数值) */
																	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
  TIM_TimeBaseStructure.TIM_Prescaler= (36 - 1);				    /* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* 采样分频 */
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* 向上计数模式 */
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);											/* 开启时钟 */
	#if _STM32_IO8
#if ECAT_TIMER_INT
				  /* Configure NVIC Interrupt  -------------------------*/	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	#endif
	#endif
}
