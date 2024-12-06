#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"                  // Device header

#define DeedLine 3500
#define Pwm_Limit 10000

extern int MotorA;
extern int MotorB;
void Motor_Init(void);
void SetMotorASpeed(int Speed);
void SetMotorBSpeed(int Speed);
void SetMotor(int Motor_A,int Motor_B);
#endif
