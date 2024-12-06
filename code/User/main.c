#include "stm32f10x.h"                  // Device header
#include "system.h"
#include "My_I2C.h"
#include "usart.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"
#include "mpu6050.h"
#include "OLED.h"
#include "Timer.h"
#include "Kalman.h"
#include "math.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	USART3_Init(115200); //串口初始化
	IIC_GPIO_Init(); //I2C接口初始化
	OLED_Init(); //OLED初始化
	OLED_ShowString(1,1,"INIT ING");
	MPU_Init(); //mpu6050初始化
	Motor_Init(); //电机驱动初始
	Encoder_Init(); //编码器初始化
	while(mpu_dmp_init());
	OLED_ShowString(1,1,"START");
	
	Timer_Init(5000,72); //5ms定时器
	while(1)
	{
		
	}
}

//五毫秒定时，所有任务在这里面执行
void TIM1_UP_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
			Encoder_A=Encoder_Get_A();
			Encoder_B=Encoder_Get_B();
			// 输出角度
			while(mpu_dmp_get_data(&outMpu.pitch,&outMpu.roll,&outMpu.yaw));
			MPU_Get_Gyroscope(&outMpu.gyro_x,&outMpu.gyro_y,&outMpu.gyro_z);
			Get_PID_Data(outMpu.pitch,outMpu.roll,outMpu.gyro_y,Encoder_A,Encoder_B,0,&MotorA,&MotorB);
			SetMotor(MotorA,MotorB);
			OLED_ShowSignedNum(2,0,Encoder_A,8);
			OLED_ShowSignedNum(3,0,Encoder_B,8);
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
    }
}



