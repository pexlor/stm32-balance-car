#include "stm32f10x.h"                  // Device header
#include "PID.h"
#define DeedLine 3000
int MotorA=0;
int MotorB=0;
int16_t My_abs(int16_t Speed);

void Motor_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);//TIME3 CH1234
	
	TIM_InternalClockConfig(TIM3);//ʹ���ڲ�ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;//�Զ���1װ��ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;//Ԥ��ƵPSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;//�Ƚϵ�ֵ
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
	TIM_Cmd(TIM3,ENABLE);
}

void SetMotorASpeed(int16_t Speed){
	if(Speed<=0){
		TIM_SetCompare3(TIM3,-Speed+DeedLine);
		TIM_SetCompare1(TIM3,0);
	}else{
		TIM_SetCompare1(TIM3,Speed+DeedLine);
		TIM_SetCompare3(TIM3,0);
	}
}

void SetMotorBSpeed(int16_t Speed){
	if(Speed<=0){
		TIM_SetCompare2(TIM3,-Speed+DeedLine);
		TIM_SetCompare4(TIM3,0);
		
	}else{
		TIM_SetCompare4(TIM3,Speed+DeedLine);
		TIM_SetCompare2(TIM3,0);
	}
}

void SetMotor(int Motor_A,int Motor_B){
	SetMotorASpeed(Motor_A);
	SetMotorBSpeed(Motor_B);
}


