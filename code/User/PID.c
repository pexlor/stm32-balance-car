#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "stdio.h"
#include "mpu6050.h" 
#include "Motor.h" 
#include "Encoder.h"
#include "PID.h"

float Mechanicla_balance=0;
int Movement=0;

struct PID_Arg{
	float Balance_Kp;
	float Balance_Ki;
	float Balance_Kd;
	float Velocity_Kp;
	float Velocity_Ki;
	float Velocity_Kd;
	float  Turn_Kp;
	float  Turn_Ki;
	float  Turn_Kd;
};

struct PID_Arg ARG = {
	.Balance_Kp=700,//-230 245
	.Balance_Ki=0,
	.Balance_Kd=1.2,//-0.16 0.50
	.Velocity_Kp=200,//-48 70 100
	.Velocity_Ki=0,//-0.24 0.21 0.325
	.Turn_Kp = 0,
	.Turn_Kd = 0,
};

int Balance_Pwm; //直立环
int Velocity_Pwm; //速度环
int Turn_Pwm; //转向环
static float  Speed_I = 0,Speed= 0; //速度环积分和滤波值

float Vertical_Ring_PD(float pitch,short gyro_y,int mechanicla_balance){
	
	float Bias;
	float Data;
	
	Bias = pitch - mechanicla_balance; //计算偏差
	
	Data = ARG.Balance_Kp * Bias + ARG.Balance_Kd * gyro_y; //PD计算
	return Data;
}

float Vertical_Speed_PI(int encoder_left,int encoder_right,int movement){
	
	float Data,Speed_P;
	Speed_P = encoder_left + encoder_right - movement; //速度偏差=测量速度（左右编码器之和）-目标速度（此处为零）
	
	Speed *= 0.8f;
	Speed += Speed_P*0.2f; //速度低通滤波
	
	Speed_I += Speed; //速度积分
	Speed_I -= movement;
	
	//积分限幅
	if(Speed_I > 10000) 	Speed_I = 10000;
	if(Speed_I < -10000) 	Speed_I = -10000;
	
	Data = ARG.Velocity_Kp*Speed + ARG.Velocity_Ki*Speed_I; //PI环
	
	return Data;
}

float Vertical_Turn_PD(u8 target_angle,float yaw){
	float Turn;
	float Bias;
	
	Bias=target_angle-yaw;
	Turn = -Bias*ARG.Turn_Kp - yaw*ARG.Turn_Kd; //PD环

	return Turn;
}

int Turn_off(float pitch){
	if(pitch>60||pitch<-60){
		return 1;
	}else{
		return 0;
	}
}



void Get_PID_Data(float pitch,float yaw,short gyro_y,int encoder_left,int encoder_right,int Contrl_Turn,int *Motor_A,int *Motor_B){
	
	if(Turn_off(pitch)){
		*Motor_A = 0;
		*Motor_B = 0;
		Speed_I = 0; //积分清零
		Speed= 0; //速度清零
		return;
	}
	
	Balance_Pwm 	= (int)Vertical_Ring_PD(pitch,gyro_y,Mechanicla_balance);
	
	Velocity_Pwm 	= (int)Vertical_Speed_PI(encoder_left,encoder_right,0);
	
	Turn_Pwm 			= (int)Vertical_Turn_PD(Contrl_Turn,yaw);
	
	*Motor_A = Balance_Pwm + Velocity_Pwm + Turn_Pwm;
	
	*Motor_B = Balance_Pwm + Velocity_Pwm - Turn_Pwm;
	
	//printf("%.2f %.2f %d %d %d\n",pitch,gyro_y,Balance_Pwm,Velocity_Pwm,Turn_Pwm);
	
}
