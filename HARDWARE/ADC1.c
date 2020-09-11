/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:              ADC.c
** Descriptions:           ADC channel0 hardware driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:              陈韦达
** Created date:            2020.8.10
** Version:                 v1.0
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
* Function Name  : ADC_GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void ADC_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  //RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 		F107用这个				 				
  /* Configure PA.00 (ADC Channel1) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG, ENABLE); 不确定加不加这句
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
    
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  /* ADC1 configuration ------------------------------------------------------*/
  //ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	  												 /* 独立模式 */
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			  																 /* 连续多通道模式 */
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   												/* 连续转换 */
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //f1用这句
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;  /* 转换不受外界决定 */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		      								 /* 右对齐 */
  //ADC_InitStructure.ADC_NbrOfChannel = 1;					   																			/* 扫描通道数 */
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);

 /* ADC1 regular channel8 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);  /* 通道X,采用时间为55.5周期,1代表规则通道第1个 */	
  ADC_Cmd(ADC1, ENABLE);                 																																							/* Enable ADC1 */                      
  ADC_SoftwareStartConv(ADC1);    																															/* 使能转换开始 */
}

