
/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:             led.H
** Descriptions:          led  hardware driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:              陈韦达
** Created date:            2020.8.8
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
#include "led.h" 
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOG,GPIO_Pin_8);//GPIOF9,F10设置高，灯灭

}






