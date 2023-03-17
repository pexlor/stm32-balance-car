#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "delay.h"
#include "mpu6050.h"
#include "OLED.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "PID.h"
#include "motor.h"
#include "Encoder.h"

// MotorA,MotorB,outMpu(struct),Encoder_A,Encoder_B
void show(void);

int main(void){//Å·À­½Ç
	delay_init();
	MPU_Init();
	IIC_GPIO_Init();
	OLED_Init();
	Motor_Init();
	Encoder_Init();
	OLED_Fill(0x00);
	while(mpu_dmp_init());
	TIM_Cmd(TIM1, ENABLE);
	while(1)
	{
			//while(mpu_dmp_get_data(&outMpu.pitch,&outMpu.roll,&outMpu.yaw));
			//Get_PID_Data(outMpu.pitch,outMpu.yaw,outMpu.gyro_y,Encoder_A,Encoder_B,64,&MotorA,&MotorB);
			//SetMotor(MotorA,MotorB);
			show();
	}	
}
	
void show(void){
		if(MotorA<0) 
		{
			OLED_ShowChar(46,1,'-',2);
			OLED_ShowFloat(50,1,-MotorA,6,2);
		}
		else 
		{
			OLED_ShowChar(46,1,' ',2);
			OLED_ShowFloat(50,1,MotorA,6,2);
		}
		
		if(Encoder_B<0) 
		{
			OLED_ShowChar(46,3,'-',2);
			OLED_ShowNum(50,3,-Encoder_B,4,2);
		}
		else 
		{
			OLED_ShowChar(46,3,'+',2);
			OLED_ShowNum(50,3,Encoder_B,4,2);
		}
		
		if(Encoder_A<0) 
		{
			OLED_ShowChar(46,5,'-',2);
			OLED_ShowNum(50,5,-Encoder_A,4,2);
		}
		else 
		{
			OLED_ShowChar(46,5,'+',2);
			OLED_ShowNum(50	,5,Encoder_A,4,2);
		}
}
uint16_t time1=0,time2=0;
void TIM1_UP_IRQHandler(void){//ÖÐ¶Ïº¯Êý
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET){
		Encoder_A=Encoder_Get_A();
		Encoder_B=Encoder_Get_B();
		while(mpu_dmp_get_data(&outMpu.pitch,&outMpu.roll,&outMpu.yaw));
		Get_PID_Data(outMpu.pitch,outMpu.yaw,outMpu.gyro_y,Encoder_A,Encoder_B,64,&MotorA,&MotorB);
		SetMotor(MotorA,MotorB);
		if(time2==40){
			show();
			time2=0;
		}
		time2++;
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}
