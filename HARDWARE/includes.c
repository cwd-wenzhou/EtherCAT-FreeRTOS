/*
 * includes.c
 *
 *  Created on: 2020-8-20
 *      Author: CWD
 */
#include <stdio.h>
#include "includes.h"
/*
 * ����:  Delayms_666M
 * ����:  �����ʱ��������ʱN*1ms
 * ���:  ��ʱʱ��
 * ����:  ��
 */
void Delayms_666M(unsigned long time_)
{
	unsigned long _x,_y;
	for(_y = 0; _y < time_; _y++)
		for(_x = 0;_x < 6666; _x++);
}

/*
 * ����:  Delayus_666M
 * ����:  �����ʱ��������ʱN*1Us----��DDR3��������0.1us
 * ���:  ��ʱʱ��
 * ����:  ��
 */
void Delayus_666M(unsigned long time_)
{
	unsigned long _y;
	for(_y = 0; _y < time_; _y++)
	{
		__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();
		// 		asm("NOP;");�����ʱ12ns
	}
}
void user_define()
{

}

/**/
