#ifndef __PID_H_
#define __PID_H_
float Vertical_Ring_PD(float pitch,short gyro_y,int mechanicla_balance);
float Vertical_Speed_PI(int encoder_left,int encoder_right,int movement);
float Vertical_Turn_PD(u8 CCD,float yaw);
int Turn_off(float pitch);
void Get_PID_Data(float pitch,float yaw,short gyro_y,int encoder_left,int encoder_right,int Contrl_Turn,int *Motor_A,int *Motor_B);
#endif
