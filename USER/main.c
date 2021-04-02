#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "el9800hw.h"
#include "SPI1.h"
#include "ecat_def.h"
#include "applInterface.h"
#include "el9800appl.h"
#include "API.h"
#include "ads8343.h"
#include "DIANCIFA.h"
//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define ADC_TASK_PRIO		2
//任务堆栈大小	
#define ADC_STK_SIZE 		128  
//任务句柄
TaskHandle_t ADCTask_Handler;
//任务函数
void adc_task(void *pvParameters);

//任务优先级
#define ECAT_TASK_PRIO		4
//任务堆栈大小	
#define ECAT_STK_SIZE 		256  
//任务句柄
TaskHandle_t ECATTask_Handler;
//任务函数
void ECAT_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		3
//任务堆栈大小	
#define FLOAT_STK_SIZE 		512
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);

//全局变量声明，用于和对象字典的同步
float CH1_current,CH2_current,CH3_current,CH4_current;
INT16 CH1_AD_Inputs,CH2_AD_Inputs,CH3_AD_Inputs,CH4_AD_Inputs;
INT16 data_cache[4][100];//开辟一个4*100个数据的缓存区
int data_cache_count[4]={0,0,0,0};//记录当前缓存区使用；

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	uart_init(115200);     	//初始化串口
	LED_Init();		        //初始化LED端口
	HW_Init();
	Diancifa_Init();
  adc_change_init();
  MainInit();
	SPI1_Init();
  bRunApplication = TRUE;
	PAout(4)=1;
//	CH1_current=(float)(read(0,SINGLE))/163.4;
//	printf("ans=%f\r\n",CH1_current);
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
 
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建adc0_task任务
    xTaskCreate((TaskFunction_t )adc_task,     	
                (const char*    )"adc_task",   	
                (uint16_t       )ADC_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )ADC_TASK_PRIO,	
                (TaskHandle_t*  )&ADCTask_Handler);   
    //创建ECAT_task任务
    xTaskCreate((TaskFunction_t )ECAT_task,     
                (const char*    )"ECAT_task",   
                (uint16_t       )ECAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ECAT_TASK_PRIO,
                (TaskHandle_t*  )&ECATTask_Handler);        
    //浮点测试任务
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}




//采样任务
void adc_task(void *pvParameters)
{
	while(1)
	{
//	  CH1_current=(float)(read(0,SINGLE))/163.4;
//		CH2_current=(float)(read(1,SINGLE))/163.6;
//		CH3_current=(float)(read(2,SINGLE))/163.4;
//		CH4_current=(float)(read(3,SINGLE))/165;
		//printf("ans=%f\r\n",CH2_current);
		//taskEXIT_CRITICAL();
    vTaskDelay(1000);
	}
}   

//ECAT任务函数
void ECAT_task(void *pvParameters)
{
   
  while (1)  
	{
		MainLoop();
		vTaskDelay(20);
	} 

}
//采样任务
void float_task(void *pvParameters)
{
	//float ans;
  //SPI1_Init();
	while(1)
	{
//		if (Diancifa_STATUS_6030.diancifa1_Time>Diancifa_cmd.diancifa1_time){
//			if (diancifa_state1!=Diancifa_cmd.diancifa1){
//				diancifa_state1=Diancifa_cmd.diancifa1;
//				RELAY1=Diancifa_cmd.diancifa1;
//				Diancifa_STATUS_6030.diancifa1_Time=0;
//			}
//		}
//		
//		if (Diancifa_STATUS_6030.diancifa2_Time>Diancifa_cmd.diancifa2_time){
//			if (diancifa_state2!=Diancifa_cmd.diancifa2){
//				diancifa_state2=Diancifa_cmd.diancifa2;
//				RELAY2=Diancifa_cmd.diancifa2;
//				Diancifa_STATUS_6030.diancifa2_Time=0;
//			}
//		}
//		if (Diancifa_STATUS_6030.diancifa3_Time>Diancifa_cmd.diancifa3_time){
//			if (diancifa_state3!=Diancifa_cmd.diancifa3){
//				diancifa_state3=Diancifa_cmd.diancifa3;
//				RELAY3=Diancifa_cmd.diancifa3;
//				Diancifa_STATUS_6030.diancifa3_Time=0;
//			}
//		}
//		if (Diancifa_STATUS_6030.diancifa4_Time>Diancifa_cmd.diancifa4_time){
//			if (diancifa_state4!=Diancifa_cmd.diancifa4){
//				diancifa_state4=Diancifa_cmd.diancifa4;
//				RELAY4=Diancifa_cmd.diancifa4;
//				Diancifa_STATUS_6030.diancifa4_Time=0;
//			}
//		}
//		
//		Diancifa_STATUS_6030.diancifa1_STATE=FBO1;
//		Diancifa_STATUS_6030.diancifa2_STATE=FBO2;
//		Diancifa_STATUS_6030.diancifa3_STATE=FBO3;
//		Diancifa_STATUS_6030.diancifa4_STATE=FBO4;
    vTaskDelay(500);
	}
}
//configKERNEL_INTERRUPT_PRIORITY

