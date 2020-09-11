///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                            LTC2486.C                              ////
////                                                                   ////
////  Library for a Linear LTC2486 16bit ADC                           ////
////                                                                   ////
////  This library assumes the CS pin is held low.  Changing the state ////
////  of the CS pin during a conversion seemed to negatively affect    ////
////  the results of the conversion.                                   ////
////                                                                   ////
////  API                                                              ////
////  ---------------------------------------------------------------- ////
////                                                                   ////
////  int1 suc = ext_adc_read_and_start(signed int32 *pRet, int16 cmd) ////
////                                                                   ////
////     Reads the previous conversion and saves result to pRet,       ////
////     then starts a new conversion using the parameters of          ////
////     cmd.  For a list of valid parameters for cmd, look in         ////
////     this file for the section labeled LTC2486_CONFIG.             ////
////     Many of the parameters for cmd can be ORd together.           ////
////     If you are only performing simple single channel              ////
////     conversion (not differential) then the function               ////
////     ext_adc_chan_to_config() can be used to generated             ////
////     the cmd needed for that channel.                              ////
////                                                                   ////
////     It is important to note that this routine reads the previous  ////
////     conversion result, and then starts a new conversion using     ////
////     the cmd parameters.                                           ////
////                                                                   ////
////     Returns TRUE if:                                              ////
////        the previous conversion is complete.                       ////
////        pRet is updated.                                           ////
////        a new conversion using cmd is started.                     ////
////     Returns FALSE if:                                             ////
////        previous conversion is not finished.                       ////
////        pRet is not updated.                                       ////
////        cmd is ignored (allow previous conversion to fininsh).     ////
////                                                                   ////
////     pRet will be assigned LTC2486_UNDERVOLTAGE if an undervoltage ////
////     error occured.  pRet will be assigned to LTC2486_OVERVOLTAGE  ////
////     if an overvoltage error occurred.                             ////
////                                                                   ////
////                                                                   ////
////  int1 ready = ext_adc_ready()                                     ////
////                                                                   ////
////     Returns TRUE if the previous conversion has finished          ////
////     and is ready to be read with ext_adc_read_and_start().        ////
////     This is done by looking at the SPI MISO line (low means       ////
////     ready).  Under some configuration conditions you may          ////
////     need to provide this macro/callback in your application       ////
////     (see CONFIG section below).                                   ////
////                                                                   ////
////                                                                   ////
////  int16 cmd = ext_adc_chan_to_config(int8 chan)                    ////
////                                                                   ////
////     chan is a single ended (non differential) channel (0-3        ////
////     for voltage channel, 4 for temperature), returns int16 cmd    ////
////     register that can be used for ext_adc_read_and_start().       ////
////                                                                   ////
////                                                                   ////
////  int32 result = ext_adc_read(int8 chan)                           ////
////                                                                   ////
////     Performs a simple read of channel (0-3, single ended)         ////
////     and returns result.  Will deadlock the processor until        ////
////     conversion is started and then read.  The time this           ////
////     takes is 2 conversions (see SPEED section below).             ////
////                                                                   ////
////                                                                   ////
////  INIT                                                             ////
////  --------------------------------------------------------------   ////
////                                                                   ////
////  There is no real init routine.  It is assumed that SPI and       ////
////  SPI pins are already initialized before calling this (use        ////
////  #use spi()).  It is also assumed CS is always low during         ////
////  entire conversion (we've had accuracy problems if CS is          ////
////  modified during conversion).  The first call to                  ////
////  ext_adc_read_and_start() will start the first conversion.        ////
////                                                                   ////
////  ext_adc_init() is provided for backwards compability with        ////
////  other libraries, but this is an empty macro.                     ////
////                                                                   ////
////                                                                   ////
////  SPEED                                                            ////
////  --------------------------------------------------------------   ////
////                                                                   ////
////  When using the internal oscillator of the LTC2486, the           ////
////  conversion speed is limited (6 samples/sec) and the SPI          ////
////  communication speed is limited (38000bps).                       ////
////                                                                   ////
////  Clocking at 2Mhz seemd to produce no negative conversion         ////
////  effects.  Conversion speed is ~48 samples/sec.  SPI comm         ////
////  speed is 4Mbps.                                                  ////
////                                                                   ////
////  PIC PWM can be used to clock to LTC2486                          ////
////                                                                   ////
////  Above conversion speeds are for using 1x speed option.  2x       ////
////  speed option could also be used, and sample rate would           ////
////  be doubled.                                                      ////
////                                                                   ////
////                                                                   ////
////  CONFIG                                                           ////
////  --------------------------------------------------------------   ////
////                                                                   ////
////  If nothing is defined before including this file, then           ////
////  EXT_ADC_MISO, EXT_ADC_MOSI and EXT_ADC_CLK and #use spi()        ////
////  are defined to use SPI1 on most PICs.  EXT_ADC_XFER()            ////
////  is defined for a SPI transfer, and ext_adc_ready() is defined    ////
////  to look for conversion ready signal.                             ////
////                                                                   ////
////  If you define your own EXT_ADC_XFER() macro before including     ////
////  this file, then those config options are ignored.                ////
////  EXT_ADC_XFER() should be a SPI read/write transfer function.     ////
////  ext_adc_ready() will also need to be defined to look at the      ////
////  conversion ready signal.                                         ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2012 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////


#include "LTC2486.h"
uint32_t ext_adc_write24(uint16_t val)
{
   union
   {
      uint8_t b[4];
      uint32_t dw;
   } res;
   
   res.b[3] = 0;
   res.b[2] = EXT_ADC_XFER(val >> 8);
   res.b[1] = EXT_ADC_XFER(val >> 0);
   res.b[0] = EXT_ADC_XFER(0);
   return(res.dw);
}

// see documentation above
uint16_t ext_adc_chan_to_config(uint8_t chan)
{
   uint16_t ret;

   switch(chan)
   {
      default:
      case 0:  ret = LTC2486_CHAN_0;      break;
      case 1:  ret = LTC2486_CHAN_1;      break;
      case 2:  ret = LTC2486_CHAN_2;      break;
      case 3:  ret = LTC2486_CHAN_3;      break;
      case 4:  ret = LTC2486_CHAN_TEMP;   break;
   }

   return(ret);
}

// see documentation above
unsigned char ext_adc_read_and_start(int32_t *pRet, uint16_t cmd)
{
   int32_t res;   //signed
   int1 sig,msb;
   int1 ret = 0;

   if (ext_adc_ready()>0)
   {
      ret = 1;
      
      res = ext_adc_write24(cmd);

      if (pRet)
      {
         //sig = bit_test(res, 21);
         //msb = bit_test(res, 20);
				 sig=res & 0x100000;//0000 0000 0001 0000 0000 0000 0000 0000;
         msb=res & 0x200000;//0000 0000 0010 0000 0000 0000 0000 0000;
         res >>= 4;
         res &= (uint32_t)0x0000FFFF;
      
         //printf("\r\nadc s=%u m=%u %ld ", sig, msb, res);
         
         if (sig && msb)
            res = LTC2486_OVERVOLTAGE;
         else if (!sig && !msb)
            res = LTC2486_UNDERVOLTAGE;
         if ((res!=0) && !sig)
         {
            //non standard sign extension
            res |= (uint32_t)0xFFFF0000;
         }
      
         *pRet = res;
      }
   }
   
   return(ret);
}

int32_t ext_adc_read(uint8_t chan)
{
   int32_t ret;
   
   while(!ext_adc_read_and_start(0, ext_adc_chan_to_config(chan)));
      
   while(!ext_adc_read_and_start(&ret, ext_adc_chan_to_config(chan)));
   
   return(ret);
}


