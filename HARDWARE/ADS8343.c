#include "ads8343.h"
#include "usart.h"

uint16_t spi_readwrite_2byte(uint16_t cmd)
{
   union
   {
      uint8_t b[2];
      uint16_t dw;
   } res;
   res.b[1] = SPI1_ReadWriteByte(cmd >> 8);
   res.b[0] = SPI1_ReadWriteByte(cmd >> 0);
   return(res.dw);
}

uint32_t spi_readwrite_3byte(uint8_t* cmd)
{
   union
   {
      uint8_t b[3];
      uint32_t dw;
   } res;
	 res.b[2] = SPI1_ReadWriteByte(cmd[2]);
   res.b[1] = SPI1_ReadWriteByte(cmd[1]);
   res.b[0] = SPI1_ReadWriteByte(cmd[0]);
   return(res.dw);
}



uint32_t read(int channel, ADC_Mode mode){
	
	union
   {
      uint8_t b[2];
      uint16_t dw;
   } data;
	uint32_t result;
	CS_0();
	setChannelAndModeByte(channel, mode);
//	CS_1();
	//while(HAL_GPIO_ReadPin(ADCBUSY_GPIO_Port,ADCBUSY_Pin) == GPIO_PIN_SET)
//	while (PCin(4))
//		continue;
//	CS_0();
	//HAL_SPI_Receive(&hspi2,data,2,100);
	data.dw=spi_readwrite_2byte(0x00);
	
//	printf("data.dw=%d\r\n",data.dw);
//	result = ((uint32_t)data.b[0] << 24) & 0xff000000;
//	result |= ((uint32_t)data.b[1] << 16);
//	//make into an unsigned int for compatibility with the DAC used on the analog shield.
//	if(result < 0)
//	{
//		result = result >> 16;
//		result &= 0x7FFF;
//	}
//	else
//	{
//		result = result >> 16;
//		result |= 0x8000;
//	}
//	CS_1();
	return data.dw;
}



int signedRead(int channel, bool ADC_Mode){
	return 1;
}

void writeOneChannel(int channel, unsigned int value){
	uint8_t call = 0x10;
	uint8_t data[3];

	if(channel == 1)
		call = 0x12;
	else if(channel == 2)
		call = 0x14;
	else if(channel == 3)
		call = 0x16;
	data[0] = call;
	data[1] = (uint8_t)(value >> 8);
	data[2] = (uint8_t)(value & 0x00FF);
	CS1_0();
	spi_readwrite_3byte(data);
	//HAL_SPI_Transmit(&hspi2,data,3,100);
	CS1_1();
}


void writeTwoChannels(unsigned int value0, unsigned int value1, bool simul){

}
void writeThreeChannels(unsigned int value0, unsigned int value1, unsigned int value2, bool simul){

}
void writeAllChannels(unsigned int value0, unsigned int value1, unsigned int value2, unsigned int value3, bool simul){

}

void setChannelAndModeByte(uint8_t channel, ADC_Mode mode){
	uint8_t control = 0x82; //default to channel 1 '001'
	uint8_t byte;
	if(channel == 3){
		control = control | 0xE0;
	}

	else if(channel == 2){
		control = control | 0xA0;
	}

	else if(channel == 1){
		control = control | 0xD0;
	}

	else if(channel == 0){
		control = control | 0x10;
	}

	//differential mode active
	if(mode == DIFFR){
		control = control & 0xFB;
	}
	else{
		control = control | 0x04;
	}

	SPI1_ReadWriteByte(control);
	//HAL_SPI_Transmit(&hspi2,&control,1,100);

	return;
}

