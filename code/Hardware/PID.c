#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "stdio.h"
#include "mpu6050.h" 
#include "Motor.h" 
#include "Encoder.h"
#include "PID.h"
#define Pwm_Limit 8000

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
	.Balance_Kp=-350,//-230 245
	.Balance_Ki=-400,
	.Balance_Kd=-1.5,//-0.16 0.50
	.Velocity_Kp=-150,//-48 70 100
	.Velocity_Ki=-0.95,//-0.24 0.21 0.325
	.Turn_Kp = 70,
	.Turn_Kd = 0.5,
};

int Vertical_Ring_PD(float pitch,float gyro_y,int mechanicla_balance){//直立环
	
	float Bias;
	static float Bias_i=0,temp=0;
	int Data;
	Bias=pitch-mechanicla_balance;//误差值
	Data=ARG.Balance_Kp*Bias+ARG.Balance_Kd*gyro_y;
	return Data;
}

int Vertical_Speed_PI(float pitch,int encoder_left,int encoder_right,int movement){//速度环
	static float  Speed_I=0,Speed_last=0;
	float Data,Speed_P;
	Speed_P=encoder_left+encoder_right-0;//误差值
	
	Speed_last*=0.8f;
	Speed_last+=Speed_P*0.2f;//一阶低通滤波
	
	Speed_I+=Speed_last;
	Speed_last-=movement;
	if(Speed_I>Pwm_Limit) Speed_I=Pwm_Limit;
	if(Speed_I<-Pwm_Limit) Speed_I=-Pwm_Limit;//积分限制
	Data=ARG.Velocity_Kp*Speed_P+ARG.Velocity_Ki*Speed_I;
	if(Turn_off(pitch)){
		Speed_I=0;
	}	
	return Data;
}

int Vertical_Turn_PD(u8 CCD,float yaw){//转向环
	float Turn;
	float Bias;
	Bias=CCD-64;
	Turn=-Bias*ARG.Turn_Kp-yaw*ARG.Turn_Kd;
	return Turn;
}

int Turn_off(float pitch){//跌落保护
	if(pitch>60||pitch<-60){
		SetMotorASpeed(0);
		SetMotorBSpeed(0);
		return 1;
	}else{
		return 0;
	}
}

void PWM_Limiting(int *motor1,int *motor2)//速度限制
{
	if(*motor1<-Pwm_Limit) *motor1=-Pwm_Limit;	
	if(*motor1>Pwm_Limit)  *motor1=Pwm_Limit;	
	if(*motor2<-Pwm_Limit) *motor2=-Pwm_Limit;	
	if(*motor2>Pwm_Limit)  *motor2=Pwm_Limit;		
}
void Get_PID_Data(float pitch,float yaw,float gyro_y,int encoder_left,int encoder_right,int Contrl_Turn,int *Motor_A,int *Motor_B){
	
	int Balance_PWm =Vertical_Ring_PD(pitch,gyro_y,Mechanicla_balance);//直立环
	
	int Velocity_Pwm = Vertical_Speed_PI(pitch,encoder_left,encoder_right,0);//速度环
	
	int Turn_Pwm = Vertical_Turn_PD(Contrl_Turn,yaw);
	
	*Motor_A = Balance_PWm+Velocity_Pwm+Turn_Pwm;
	
	*Motor_B = Balance_PWm+Velocity_Pwm-Turn_Pwm;
	
	PWM_Limiting(Motor_A,Motor_B);//pwm限幅
	
	if(Turn_off(pitch)){//跌落保护
		*Motor_A = 0;
		*Motor_B = 0;
	}
	
}
