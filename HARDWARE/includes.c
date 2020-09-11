/*
 * includes.c
 *
 *  Created on: 2020-8-20
 *      Author: CWD
 */
#include <stdio.h>
#include "includes.h"
/*
 * 描述:  Delayms_666M
 * 功能:  软件延时函数，延时N*1ms
 * 入口:  延时时间
 * 出口:  无
 */
void Delayms_666M(unsigned long time_)
{
	unsigned long _x,_y;
	for(_y = 0; _y < time_; _y++)
		for(_x = 0;_x < 6666; _x++);
}

/*
 * 描述:  Delayus_666M
 * 功能:  软件延时函数，延时N*1Us----在DDR3中运行是0.1us
 * 入口:  延时时间
 * 出口:  无
 */
void Delayus_666M(unsigned long time_)
{
	unsigned long _y;
	for(_y = 0; _y < time_; _y++)
	{
		__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();
		// 		asm("NOP;");大概延时12ns
	}
}
void user_define()
{

}

/**/
