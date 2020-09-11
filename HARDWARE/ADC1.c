/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:              ADC.c
** Descriptions:           ADC channel0 hardware driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:              ��Τ��
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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  //RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 		F107�����				 				
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
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG, ENABLE); ��ȷ���Ӳ������
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
    
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  /* ADC1 configuration ------------------------------------------------------*/
  //ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	  												 /* ����ģʽ */
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			  																 /* ������ͨ��ģʽ */
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   												/* ����ת�� */
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //f1�����
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;  /* ת������������ */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		      								 /* �Ҷ��� */
  //ADC_InitStructure.ADC_NbrOfChannel = 1;					   																			/* ɨ��ͨ���� */
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);

 /* ADC1 regular channel8 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);  /* ͨ��X,����ʱ��Ϊ55.5����,1�������ͨ����1�� */	
  ADC_Cmd(ADC1, ENABLE);                 																																							/* Enable ADC1 */                      
  ADC_SoftwareStartConv(ADC1);    																															/* ʹ��ת����ʼ */
}

