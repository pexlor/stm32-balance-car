#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "Cartask.h"
#include "My_I2C.h"
#include "usart.h"

#define START_TASK_PRIO  1
TaskHandle_t satrt_task_handle;
#define START_TASK_STACK_SIZE 128

#define MOTOR_STACK_SIZE 128//字
#define MOTOR_TASK_PRIO  2
TaskHandle_t motor_task_handle;

#define SHOW_STACK_SIZE 128
#define SHOW_TASK_PRIO  2
TaskHandle_t show_task_handle;

#define GETDATE_STACK_SIZE 128
#define GETDATE_TASK_PRIO  2
TaskHandle_t getdate_task_handle;

// 可调用的全局变量 MotorA,MotorB,outMpu(struct),Encoder_A,Encoder_B
void show(void);
void start_task(void *pvParameters);

int main(void){
	IIC_GPIO_Init();
	USART1_Init(9600);
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	xTaskCreate((TaskFunction_t) start_task,
						 (char *)"start_task",
						 (configSTACK_DEPTH_TYPE)START_TASK_STACK_SIZE,
						 (void *) NULL,
						 (UBaseType_t) START_TASK_PRIO,
						 (TaskHandle_t *)&satrt_task_handle);
	vTaskStartScheduler();
						 
	while(1)
	{
		
	}	
}

void start_task(void *pvParameters){
	taskENTER_CRITICAL();//进入临界区
	xTaskCreate((TaskFunction_t) Motor_task,
						 (char *)"Motor_task",
						 (configSTACK_DEPTH_TYPE)MOTOR_STACK_SIZE,
						 (void *) NULL,
						 (UBaseType_t) MOTOR_TASK_PRIO,
						 (TaskHandle_t *)&motor_task_handle);
	xTaskCreate((TaskFunction_t) Show_task,
						 (char *)"Show_task",
						 (configSTACK_DEPTH_TYPE)SHOW_STACK_SIZE,
						 (void *) NULL,
						 (UBaseType_t) SHOW_TASK_PRIO,
						 (TaskHandle_t *)&show_task_handle);
	xTaskCreate((TaskFunction_t) Get_Date_task,
					 	 (char *)"Get_Date_task",
						 (configSTACK_DEPTH_TYPE)GETDATE_STACK_SIZE,
						 (void *) NULL,
						 (UBaseType_t) GETDATE_TASK_PRIO,
						 (TaskHandle_t *)&getdate_task_handle);
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();//退出临界区
}


