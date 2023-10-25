#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "motor.h"
#include "Encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "PID.h"
#include "mpu6050.h"
#include "OLED.h"
void vTaskDelay( const TickType_t xTicksToDelay );
void show(void);


void Motor_task(void *pvParameters){
	taskENTER_CRITICAL();//挂起所以任务
	Motor_Init();
	Encoder_Init();
	taskEXIT_CRITICAL();
	for(;;){
		Encoder_A=Encoder_Get_A();
		Encoder_B=Encoder_Get_B();
		Get_PID_Data(outMpu.pitch,outMpu.yaw,outMpu.gyro_y,Encoder_A,Encoder_B,64,&MotorA,&MotorB);
		SetMotor(MotorA,MotorB);
		vTaskDelay(10);
	}
}

void Get_Date_task(void *pvParameters){
	taskENTER_CRITICAL();
	MPU_Init();
	while(mpu_dmp_init());
	taskEXIT_CRITICAL();
	for(;;){
		while(mpu_dmp_get_data(&outMpu.pitch,&outMpu.roll,&outMpu.yaw));
		vTaskDelay(5);
	}
}

void Show_task(void *pvParameters){
	vTaskSuspendAll();
	IIC_GPIO_Init();
	OLED_Init();
	OLED_ShowStr (1,1,"pitch:",1);
	OLED_ShowStr (1,3,"roll:",1);
	OLED_ShowStr (1,5,"yaw:",1);
	xTaskResumeAll();
	for(;;){
		show();
		vTaskDelay(200);
	}
}


void show(void){
		if(outMpu.pitch<0) 
		{
			OLED_ShowChar(46,1,'-',2);
			OLED_ShowFloat(50,1,-outMpu.pitch,3,2);
		}
		else 
		{
			OLED_ShowChar(46,1,' ',2);
			OLED_ShowFloat(50,1,outMpu.pitch,3,2);
		}
		
		if(outMpu.roll<0) 
		{
			OLED_ShowChar(46,3,'-',2);
			OLED_ShowFloat(50,3,-outMpu.roll,3,2);
		}
		else 
		{
			OLED_ShowChar(46,3,' ',2);
			OLED_ShowFloat(50,3,outMpu.roll,3,2);
		}
		
		if(outMpu.yaw<0) 
		{
			OLED_ShowChar(46,5,'-',2);
			OLED_ShowFloat(50,5,-outMpu.yaw,3,2);
		}
		else 
		{
			OLED_ShowChar(46,5,' ',2);
			OLED_ShowFloat(50,5,outMpu.yaw,3,2);
		}
}
