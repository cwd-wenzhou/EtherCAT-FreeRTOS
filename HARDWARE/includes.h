/*
 * includes.h
 *
 *  Created on: 2012-3-18
 *      Author: wsy
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_
#include <stdio.h>
 
 
#define stop() while(1)
#define BIT(x) (1 << (x))
#define REG(x) (*((volatile unsigned long *)(x)))
#define REG_U16(x) (*((volatile unsigned short int*)(x)))

#define TEST_OUT_0	REG(XPAR_AXI_GPIO_0_BASEADDR) = 0
#define TEST_OUT_1	REG(XPAR_AXI_GPIO_0_BASEADDR) = 0x1


 
 

void Delayus_666M(unsigned long time_);//�����ʱn*us
void Delayms_666M(unsigned long time_);//�����ʱ
   

//==================================================

//(*(int(*)())0)(); //�����λ  ��������ͼĴ���


#endif /* INCLUDES_H_ */
