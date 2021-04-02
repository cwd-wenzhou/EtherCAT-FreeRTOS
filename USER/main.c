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
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define ADC0_TASK_PRIO		2
//�����ջ��С	
#define ADC0_STK_SIZE 		128  
//������
TaskHandle_t ADC0Task_Handler;
//������
void adc0_task(void *pvParameters);

//�������ȼ�
#define ECAT_TASK_PRIO		4
//�����ջ��С	
#define ECAT_STK_SIZE 		256  
//������
TaskHandle_t ECATTask_Handler;
//������
void ECAT_task(void *pvParameters);

//�������ȼ�
#define FLOAT_TASK_PRIO		3
//�����ջ��С	
#define FLOAT_STK_SIZE 		512
//������
TaskHandle_t FLOATTask_Handler;
//������
void float_task(void *pvParameters);

//ȫ�ֱ������������ںͶ����ֵ��ͬ��
float CH1_current,CH2_current,CH3_current,CH4_current;
INT16 CH1_AD_Inputs,CH2_AD_Inputs,CH3_AD_Inputs,CH4_AD_Inputs;
//INT16 data_cache[4][100];//����һ��4*100�����ݵĻ�����
//int data_cache_count[4]={0,0,0,0};//��¼��ǰ������ʹ�ã�

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);		//��ʼ����ʱ����
	uart_init(115200);     	//��ʼ������
	LED_Init();		        //��ʼ��LED�˿�
	HW_Init();
  adc_change_init();
  MainInit();
	SPI1_Init();
  bRunApplication = TRUE;
	PAout(4)=1;
//	CH1_current=(float)(read(0,SINGLE))/163.4;
//	printf("ans=%f\r\n",CH1_current);
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}
 
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����adc0_task����
    xTaskCreate((TaskFunction_t )adc0_task,     	
                (const char*    )"adc0_task",   	
                (uint16_t       )ADC0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )ADC0_TASK_PRIO,	
                (TaskHandle_t*  )&ADC0Task_Handler);   
    //����ECAT_task����
    xTaskCreate((TaskFunction_t )ECAT_task,     
                (const char*    )"ECAT_task",   
                (uint16_t       )ECAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ECAT_TASK_PRIO,
                (TaskHandle_t*  )&ECATTask_Handler);        
    //�����������
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}




//����������
void adc0_task(void *pvParameters)
{
	while(1)
	{
		//CH3_current=(float)(read(2,SINGLE))/327;
    vTaskDelay(1000);
	}
}   

//ECAT������
void ECAT_task(void *pvParameters)
{
   
  while (1)  
	{
		MainLoop();
		vTaskDelay(20);
	} 
//while (bRunApplication == TRUE);

    //HW_Release();
//	while (1)
//	{
//		
//		vTaskDelay(20);
//	}
}
//��������
void float_task(void *pvParameters)
{
	//float ans;
  //SPI1_Init();
	while(1)
	{
		//taskENTER_CRITICAL();
		//setChannelAndModeByte(1, SINGLE);
		//ans=(float)(read(1,SINGLE));
		//printf("ans=%f\r\n",ans);
//		CH1_current=(float)(read(0,SINGLE))/163.4;
//		CH2_current=(float)(read(1,SINGLE))/163.6;
//		CH3_current=(float)(read(2,SINGLE))/163.4;
//		CH4_current=(float)(read(3,SINGLE))/165;
		//printf("ans=%f\r\n",CH2_current);
		//taskEXIT_CRITICAL();
    vTaskDelay(500);
	}
}
//configKERNEL_INTERRUPT_PRIORITY

