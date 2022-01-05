# FreeRTOS-EtherCAT-Project
程序概要说明书
##1 软件总体说明
该软件在FreeRTOS的框架下，实现EtherCAT网络控制，传感器的电流信号采集与处理，处理完成后发送给应用层，应用层利用这些数据，在主站实时显示。主站可设置采样频率等参数，通过EthercCAT网络传给从站。
##2 软件架构说明
本系统软件的设计基本思想是在FreeRTOS的轻量级实时操作系统下进行任务调度。主要有三个任务：
	ECAT_task：进行EtherCAT的主循环，完成Ethercat网络功能。
	ADC_task：进行ADC采样任务。
	Control_task：（仅针对B板）进行电磁阀控制任务。
其外增加外部触发中断EXIT.c，exit.h文件，由Lan9252芯片的IRQ（500Hz）信号来触发，以达到同步采样。
##3各任务的说明
###3.1 ECAT_task
该任务调用EtherCAT协议栈的主循环函数接口MainLoop()；每20ms运行一次。完成状态机的处理以及数据的传输。
###3.2 ADC_task
该任务调用ADC芯片的驱动文件的read函数，一定频率下（由主站设置）采样传感器的电流值。并通过浮点运算计算AD转换出电流值。通过设置全局变量的方式，等待ECAT_task任务将数据发送给主站。
###3.3 Control_task：
该任务实现根据主站发送的数据控制电磁阀的开合。仅针对B板调试使用，在中断中实现以达到同步效果。
##4通讯协议模块说明
###4.1初始化函数
序号	函数名称	函数说明	函数所属源文件
1	void LED_Init (void)	初始化工作指示灯；
包括初始化时钟，输出模式等	led.c
2	UINT8 HW_Init(void)	初始化stm32芯片与网卡芯片LAN9252的FSMC通讯问题，如无误返回0。	ecatappl.c
3	void adc_change_init()	电流值转化为位移值的参数初始化。参考的技术要求给定的默认值，如果修改默认值，需要重新烧写程序	API.c
4	UINT16 MainInit(void)	初始化EtherCAT网络	ecatapp.c
5	void ECAT_Init(void)	在MainInit()中被调用，初始化从站的接口函数。	ecatslv.c
6	void COE_ObjInit(void)	在MainInit()中被调用，初始化从站的对象。	Coeappl.c
7	void SPI1_Init(void)	初始化STM32芯片与采样芯片ADS8343的SPI通信	SPI1.c
###4.2EtherCAT协议栈重要函数
序号	函数名称	函数说明	函数所属源文件
1	void MainLoop(void)	周期性的处理应用层事件和状态机	ecatapp.c
2	void ECAT_Application(void)	协议栈版本问题，该函数仅作为调用：
APPL_Application()的封装。处理中断产生后的输入映射。	ecatappl.c
3	void APPL_Application(void)	被同步中断调用或者MainLoop ()中调用。
在该函数中，处理对象字典的数据映射。	el9800appl.c
4	void COE_Main(void)	在MainLoop()中被调用。负责低优先级的方法。 	coeappl.c
5	void ECAT_Main(void)	在MainLoop()中被调用。必须被周期性的调用。
负责Al和邮箱事件处理。	ecatslv.c
6	void COE_ObjInit(void)	初始化对象词典	coeapp.c
7	UINT16 COE_AddObjectToDic(TOBJECT OBJMEM * pNewObjEntry)	添加对象词典	coeapp.c
8	UINT16 COE_ObjDictionaryInit(void)	初始化对象词典	coeapp.c
9	void COE_ClearObjDictionary(void)	清除对象词典	coeapp.c
10	void HandleBusCycleCalculation(void)	SM模式下用于计算总线时间	ecatapp.c
11	UINT8 COE_ServiceInd(TCOEMBX MBXMEM *pCoeMbx)	接收coe服务	ecatcoe.c
12	UINT8    CheckSmSettings(UINT8 maxChannel)	检查sm配置	ecatslv.c
13	UINT16 StartInputHandler(void)	检查sm区域，预运行到安全运行调用	ecatslv.c
14	UINT16 StartOutputHandler(void)	检查接收到的输出 安全到运行调用	ecatslv.c
15	Void AL_ControlInd(UINT8 alControl, UINT16 alStatusCode)	处理EtherCAT状态机	ecatslv.c
16	void AL_ControlRes(void)	状态转换暂停	ecatslv.c
17	void DC_CheckWatchdog(void)	检查sync状态和标志位	ecatslv.c
18	void ECAT_StateChange(UINT8 alStatus, UINT16 alStatusCode)	状态处理	ecatslv.c


###4.3EtherCAT协议栈重要文件功能
Ecatslv:  处理EtherCAT状态机模块。状态机转换请求由主站发起，主站将请求状态写入A1Control寄存器中，从站采用查询的方式获取当前该状态转换的事件，将寄存器值作为参数传入AL_ControlInd()函数中，该函数作为核心函数来处理状态机的转换，根据主站请求的状态配置SM通道的开启或关闭，检查SM通道参数是否配置正确等。
 Ecatappl:  EtherCAT从站应用层接口，整个协议栈运行的核心模块，EtherCAT从站状态机和过程数据接口。Main()函数的定义，输入输出程数据对象的映射处理，ESC与处理器本地内存的输入输出过程数据的交换等;
Mailbox:  处理EtherCAT邮箱服务模块。包括邮箱通信接口的初始化，邮箱通道的参数配置，根据当前状态机来开启或关闭邮箱服务，邮箱通信失败后的邮箱重复发送请求，邮箱数据的读写以及根据主站请求的不同服务类型调用相应服务函数来处理；Ecatcoe:    CoE ( CANopen overEtherCAT)处理函数定义模块。包括CoE接口的初始化，对当前请求的CoE服务的处理以及对保存而未处理过的CoE服务的处理。
Sdoserv:    SDO服务处理模块。处理所有SDO及SDO信息服务，包括对主站发送的上传、下载SDO服务请求的处理，处理完成后SDO响应数据由主站来主动读取。
El9800appl: 访问CoE对象字典模块。读写对象字典，获得对象字典的入口；定义对象字典。以及对象字典的具体处理函数由该模块实现。
Coeappl:   CoE服务的应用层接口模块。对CoE服务实际应用的处理以及CoE对象字典的处理。包括对象字典的初始化，添加对象到对象字典，移除对象字典中的某一条目以及清除对象字典等处理函数的实现。

##5重要API()说明
函数1 主函数main()
 完成各种初始化函数的调用，创建开始任务，在FreeRTOS框架下开始任务调度。
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	LED_Init();		        //初始化LED端口
	HW_Init();
  	adc_change_init();
  	MainInit();
	SPI1_Init();
  	bRunApplication = TRUE;
	PAout(4)=1;
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
函数2 ADC采样函数read()：
调用更底层的setChannelAndModeByte()设置采样的通道和方式，使用spi读取。
uint32_t read(int channel, ADC_Mode mode){
	union
   {
      uint8_t b[2];
      uint16_t dw;
   } data;
	uint32_t result;
	CS_0();
	setChannelAndModeByte(channel, mode);
	data.dw=spi_readwrite_2byte(0x00);
	return data.dw;
}

函数3 adc转化参数初始化函数void adc_change_init()
该函数使用循环给X[],Y[].K[],I[]赋默认值，如果需要修改，需要在该函数二次开发
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
函数4位移值和电流值转化函数INT16 A2X(float x)
该函数使用技术要求中的公式，将电流值转化为位移值。
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
函数5中断服务函数：
A版：
int count1=0,count2=0,count3=0,count4=0;//全局变量，用于计数控制采样频率。
void  EcatIsr(void)
{		 
		PDI_Isr();
	
		count1++;
		if (count1>500/AD_Switch_Status_OUT.CH1_Frequency){
			CH1_current=(float)(read(0,SINGLE))/163.4;
			count1=0;
		}
		
		count2++;
		if (count2>500/AD_Switch_Status_OUT.CH2_Frequency){
			CH2_current=(float)(read(1,SINGLE))/163.6;
			count2=0;
		}
		
		count3++;
		if (count3>500/AD_Switch_Status_OUT.CH3_Frequency){
			CH3_current=(float)(read(2,SINGLE))/163.4;
			count3=0;
		}
		
		count4++;
		if (count4>500/AD_Switch_Status_OUT.CH4_Frequency){
			CH4_current=(float)(read(3,SINGLE))/165;
			count4=0;
		}
   /* reset the interrupt flag */
   ACK_ESC_INT;//重置中断位
}
B版：
int count1=0,count2=0,count3=0,count4=0;
int second_count=0;
int diancifa_time1=0,diancifa_time2=0,diancifa_time3=0,diancifa_time4=0;//定义用于计数的全局变量。
int diancifa_state1=0,diancifa_state2=0,diancifa_state3=0,diancifa_state4=0;
extern float CH1_current,CH2_current,CH3_current,CH4_current;
extern TOBJ6010 AD_Switch_Status_OUT;
extern TOBJ6000 AD_Inputs;
void  EcatIsr(void)
{		 

   PDI_Isr();
	 count1++;
		if (count1>1000/AD_Switch_Status_OUT.CH1_Frequency){
			float temp = (float)(read(0,SINGLE))/163.4;//调用read函数进行ad采样，下同
			if (temp<20) CH1_current=temp;//做一下范围检查
			AD_Inputs.CH1=A2X(CH1_current);//调用A2X函数进行采样电流值到位移值的转换，下同。
			count1=0;
		}
		
		count2++;
		if (count2>1000/AD_Switch_Status_OUT.CH2_Frequency){
			float temp = (float)(read(1,SINGLE))/163.6;
			if (temp<20) CH2_current=temp;//做一下范围检查
			AD_Inputs.CH2=A2X(CH2_current);
			count2=0;
		}
		
		count3++;
		if (count3>1000/AD_Switch_Status_OUT.CH3_Frequency){
			float temp = (float)(read(2,SINGLE))/163.4;
			if (temp<20) CH3_current=temp;//做一下范围检查
			AD_Inputs.CH3=A2X(CH3_current);
			count3=0;
		}
		
		count4++;
		if (count4>1000/AD_Switch_Status_OUT.CH4_Frequency){
			float temp = (float)(read(1,SINGLE))/165;
			if (temp<20) CH4_current=temp;//做一下范围检查
			AD_Inputs.CH4=A2X(CH4_current);
			count4=0;
		}
   /* reset the interrupt flag */
		
	  second_count++;
		if (second_count>1000){
			//每一秒钟进入该循环查询更新一次电磁阀状态。
			second_count=0;
			Diancifa_STATUS_6030.diancifa1_Time++; //过了一秒钟，即每个电磁阀离上次动作增加一秒，修改该值。下同。
			Diancifa_STATUS_6030.diancifa2_Time++;
			Diancifa_STATUS_6030.diancifa3_Time++;
			Diancifa_STATUS_6030.diancifa4_Time++;
		}
		
		if (Diancifa_STATUS_6030.diancifa1_Time>Diancifa_cmd.diancifa1_time){
			if (diancifa_state1!=Diancifa_cmd.diancifa1){
				diancifa_state1=Diancifa_cmd.diancifa1;
				RELAY1=Diancifa_cmd.diancifa1;
				Diancifa_STATUS_6030.diancifa1_Time=0;
			}
		}
		
		if (Diancifa_STATUS_6030.diancifa2_Time>Diancifa_cmd.diancifa2_time){
			if (diancifa_state2!=Diancifa_cmd.diancifa2){
				diancifa_state2=Diancifa_cmd.diancifa2;
				RELAY2=Diancifa_cmd.diancifa2;
				Diancifa_STATUS_6030.diancifa2_Time=0;
			}
		}
		if (Diancifa_STATUS_6030.diancifa3_Time>Diancifa_cmd.diancifa3_time){
			if (diancifa_state3!=Diancifa_cmd.diancifa3){
				diancifa_state3=Diancifa_cmd.diancifa3;
				RELAY3=Diancifa_cmd.diancifa3;
				Diancifa_STATUS_6030.diancifa3_Time=0;
			}
		}
		if (Diancifa_STATUS_6030.diancifa4_Time>Diancifa_cmd.diancifa4_time){
			if (diancifa_state4!=Diancifa_cmd.diancifa4){
				diancifa_state4=Diancifa_cmd.diancifa4;
				RELAY4=Diancifa_cmd.diancifa4;
				Diancifa_STATUS_6030.diancifa4_Time=0;
			}
		}
		
		Diancifa_STATUS_6030.diancifa1_STATE=FBO1; //电磁阀动作与否的反馈，即读取对应同都的电磁阀是否动作，下同。
		Diancifa_STATUS_6030.diancifa2_STATE=FBO2;
		Diancifa_STATUS_6030.diancifa3_STATE=FBO3;
		Diancifa_STATUS_6030.diancifa4_STATE=FBO4;
   ACK_ESC_INT;//重置中断位
}
