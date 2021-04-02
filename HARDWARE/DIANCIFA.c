#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
/*
本程序为电磁阀驱动程序
*/
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//初始化
void Diancifa_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//RELAY1,RELAY2,RELAY3,RELAY4对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//FBO1,FBO2,FBO3,FBO4对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
	


}