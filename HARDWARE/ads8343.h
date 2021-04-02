#ifndef __ADS8343_H__
#define __ADS8343_H__

#include <stdbool.h>
#include <stdint.h>
#include "SPI1.h"
#include "sys.h"

#define CS_1() 		PAout(4)=1;
#define CS_0()  	PAout(4)=0;

#define CS1_1() 	PAout(4)=1;
#define CS1_0()  	PAout(4)=0;

typedef enum{SINGLE,DIFFR}ADC_Mode;
void setChannelAndModeByte(uint8_t channel, ADC_Mode mode);
uint32_t read(int channel, ADC_Mode mode);
int signedRead(int channel, bool mode);
void writeOneChannel(int channel, unsigned int value); //any one channel
void writeTwoChannels(unsigned int value0, unsigned int value1, bool simul); //channels 0 and 1
void writeThreeChannels(unsigned int value0, unsigned int value1, unsigned int value2, bool simul); //channels 0-3
void writeAllChannels(unsigned int value0, unsigned int value1, unsigned int value2, unsigned int value3, bool simul); //all channels

uint32_t spi_readwrite_3byte(uint8_t* cmd);
uint16_t spi_readwrite_2byte(uint16_t cmd);
#endif