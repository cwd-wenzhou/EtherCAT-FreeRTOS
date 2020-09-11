/****************************************Copyright (c)****************************************************
**                                      
**  
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               LTC2486.h
** Descriptions:            
**
**--------------------------------------------------------------------------------------------------------
** Created by:              CWD
** Created date:            2020-8-20
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#ifndef __LTC2486_C__
#define __LTC2486_C__
#include <stdbool.h>
#include <stdint.h>
#include "SPI1.h"
#include "sys.h"
#ifndef EXT_ADC_XFER
   #define EXT_ADC_MISO   PAin(6)
   #define EXT_ADC_MOSI   PAin(7)
   #define EXT_ADC_CLK    PAin(5)
   
   // you can get a faster baud rate if you clock the LTC2486 with an external
   // oscillator.
   //#use spi(MASTER, bits=8, MSB_FIRST, mode=0, BAUD=38000, di=EXT_ADC_MISO, do=EXT_ADC_MOSI, clk=EXT_ADC_CLK, stream=SPI_STREAM_LTC2486)
   
   #define EXT_ADC_XFER(x)   SPI1_ReadWriteByte(x)
   
   //assumes already selected
   #define ext_adc_ready()   EXT_ADC_MISO
#else
   // user has already defined his own xfer routines.  useful if user is sharing
   // spi port or is multiple ltc2486 in his application.
      
   // user needs to point ext_adc_ready() to his own routine
#endif
#define int1 uint16_t
////////////////// begin LTC2486_CONFIG choices //////////////////////////////

// this can be sent alone to ext_adc_read_and_start().
#define LTC2486_KEEP_PREVIOUS 0x8000

// the following groups can be or'd together when sent to 
// ext_adc_read_and_start().
// first group (channel choice) is required by ext_adc_read_and_start(),
// other groups are not required.

// first group, channel choice:
#define LTC2486_CHAN_0_1      0xA080   //0=in+, 1=in-
#define LTC2486_CHAN_2_3      0xA180   //2=in+, 3=in-
#define LTC2486_CHAN_1_0      0xA880   //0=in-, 1=in+
#define LTC2486_CHAN_3_2      0xA980   //2=in-, 3=in+
#define LTC2486_CHAN_0        0xB080   //0=in+, com=in-
#define LTC2486_CHAN_2        0xB180   //2=in+, com=in-
#define LTC2486_CHAN_1        0xB880   //1=in+, com=in-
#define LTC2486_CHAN_3        0xB980   //3=in+, com=in-
#define LTC2486_CHAN_TEMP     0xA0C0   //when using this option, speed and gain is ignored

// second group, reject mode
#define LTC2486_REJECT_5060   0x0000
#define LTC2486_REJECT_50     0x0010
#define LTC2486_REJECT_60     0x0020

// third group, speed
#define LTC2486_SPEED_1x      0x0000
#define LTC2486_SPEED_2x      0x0008

// fourth group, gain
#define LTC2486_GAIN_1x       0x0000
#define LTC2486_GAIN_4x       0x0001
#define LTC2486_GAIN_8x       0x0002
#define LTC2486_GAIN_16x      0x0003
#define LTC2486_GAIN_32x      0x0004
#define LTC2486_GAIN_64x      0x0005
#define LTC2486_GAIN_128x     0x0006
#define LTC2486_GAIN_264x     0x0007

////////////////// end LTC2486_CONFIG choices //////////////////////////////


////////////////// begin library code ///////////////////////////////////////

// provided for compatability with other libraries
#define ext_adc_init()

#define LTC2486_UNDERVOLTAGE  0xFFFFFFFF
#define LTC2486_OVERVOLTAGE   0xFFFFFFFE

uint32_t ext_adc_write24(uint16_t val);
uint16_t ext_adc_chan_to_config(uint8_t chan);
unsigned char ext_adc_read_and_start(int32_t *pRet, uint16_t cmd);
int32_t ext_adc_read(uint8_t chan);
		 
#endif  
