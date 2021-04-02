#include "API.h"
float s[11],k[11],I[11],Y[11];
int adc_delay_time=1000;//默认值1hz，即计时1000ms；

void adc_change_init()
{
	int i;
	for (i=0;i<10;i++)
	{
		Y[i]=(i+1)*1.25;
		I[i]=4+1.5*i;
	}
	for (i=0;i<10;i++)
	{
	  k[i+1]=(Y[i+1]-Y[i])/(I[i+1]-I[i]);
	}
}	

//设置adc采样间隔时间
int ds_para_set(int fs)
{
	adc_delay_time=1000/fs;
	return 1;
}

INT16 A2X(float x)
{
	INT16 y;
	if (x<1.3) y=0x8000;
	else if (x<3) y=0x8fff;
	else if (x<I[1]) y=k[1]*(x-I[0])+Y[0];
	else if (x<I[2]) y=k[2]*(x-I[1])+Y[1];
	else if (x<I[3]) y=k[3]*(x-I[2])+Y[2];
	else if (x<I[4]) y=k[4]*(x-I[3])+Y[3];
	else if (x<I[5]) y=k[5]*(x-I[4])+Y[4];
	else if (x<I[6]) y=k[6]*(x-I[5])+Y[5];
	else if (x<I[7]) y=k[7]*(x-I[6])+Y[6];
	else if (x<I[8]) y=k[8]*(x-I[7])+Y[7];
	else if (x<I[9]) y=k[9]*(x-I[8])+Y[8];
	else if (x<20) y=k[10]*(x-I[0])+Y[9];
	else y=0x7fff;
	return y;
}
