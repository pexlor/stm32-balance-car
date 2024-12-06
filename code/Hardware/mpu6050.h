#ifndef __MPU6050_H
#define __MPU6050_H
#include "My_I2C.h"
#include "system.h"

struct mpu6050_data{
	short acc_x;
	short acc_y;
	short acc_z;

	short gyro_x;
	short gyro_y;
	short gyro_z;
	
	float pitch;
	float roll;
	float yaw;
	
};
extern struct mpu6050_data outMpu;                                                                                                            
#define MPU_IIC_Init			IIC_GPIO_Init
#define MPU_IIC_Start			IIC_Start
#define MPU_IIC_Stop			IIC_Stop
#define MPU_IIC_Send_Byte		IIC_Send_Byte
#define MPU_IIC_Read_Byte		IIC_Read_Byte
#define MPU_IIC_Wait_Ack		IIC_Wait_Ack

//#define MPU_ACCEL_OFFS_REG      0X06    // 加速度计偏移寄存器（未使用，保留）
//#define MPU_PROD_ID_REG         0X0C    // 产品ID寄存器（未使用，保留）
#define MPU_SELF_TESTX_REG      0X0D    // X轴自检寄存器
#define MPU_SELF_TESTY_REG      0X0E    // Y轴自检寄存器
#define MPU_SELF_TESTZ_REG      0X0F    // Z轴自检寄存器
#define MPU_SELF_TESTA_REG      0X10    // 加速度计自检寄存器
#define MPU_SAMPLE_RATE_REG     0X19    // 采样率分频寄存器
#define MPU_CFG_REG             0X1A    // 配置寄存器（低通滤波器、外部同步设置）
#define MPU_GYRO_CFG_REG        0X1B    // 陀螺仪配置寄存器（全量程范围设置）
#define MPU_ACCEL_CFG_REG       0X1C    // 加速度计配置寄存器（全量程范围设置）
#define MPU_MOTION_DET_REG      0X1F    // 运动检测阈值寄存器
#define MPU_FIFO_EN_REG         0X23    // FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG     0X24    // I2C主控制寄存器
#define MPU_I2CSLV0_ADDR_REG    0X25    // I2C从设备0地址寄存器
#define MPU_I2CSLV0_REG         0X26    // I2C从设备0数据寄存器
#define MPU_I2CSLV0_CTRL_REG    0X27    // I2C从设备0控制寄存器
#define MPU_I2CSLV1_ADDR_REG    0X28    // I2C从设备1地址寄存器
#define MPU_I2CSLV1_REG         0X29    // I2C从设备1数据寄存器
#define MPU_I2CSLV1_CTRL_REG    0X2A    // I2C从设备1控制寄存器
#define MPU_I2CSLV2_ADDR_REG    0X2B    // I2C从设备2地址寄存器
#define MPU_I2CSLV2_REG         0X2C    // I2C从设备2数据寄存器
#define MPU_I2CSLV2_CTRL_REG    0X2D    // I2C从设备2控制寄存器
#define MPU_I2CSLV3_ADDR_REG    0X2E    // I2C从设备3地址寄存器
#define MPU_I2CSLV3_REG         0X2F    // I2C从设备3数据寄存器
#define MPU_I2CSLV3_CTRL_REG    0X30    // I2C从设备3控制寄存器
#define MPU_I2CSLV4_ADDR_REG    0X31    // I2C从设备4地址寄存器
#define MPU_I2CSLV4_REG         0X32    // I2C从设备4数据寄存器
#define MPU_I2CSLV4_DO_REG      0X33    // I2C从设备4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG    0X34    // I2C从设备4控制寄存器
#define MPU_I2CSLV4_DI_REG      0X35    // I2C从设备4读数据寄存器
#define MPU_I2CMST_STA_REG      0X36    // I2C主状态寄存器
#define MPU_INTBP_CFG_REG       0X37    // 中断引脚配置寄存器
#define MPU_INT_EN_REG          0X38    // 中断使能寄存器
#define MPU_INT_STA_REG         0X3A    // 中断状态寄存器
#define MPU_ACCEL_XOUTH_REG     0X3B    // 加速度计X轴高8位数据寄存器
#define MPU_ACCEL_XOUTL_REG     0X3C    // 加速度计X轴低8位数据寄存器
#define MPU_ACCEL_YOUTH_REG     0X3D    // 加速度计Y轴高8位数据寄存器
#define MPU_ACCEL_YOUTL_REG     0X3E    // 加速度计Y轴低8位数据寄存器
#define MPU_ACCEL_ZOUTH_REG     0X3F    // 加速度计Z轴高8位数据寄存器
#define MPU_ACCEL_ZOUTL_REG     0X40    // 加速度计Z轴低8位数据寄存器
#define MPU_TEMP_OUTH_REG       0X41    // 温度传感器高8位数据寄存器
#define MPU_TEMP_OUTL_REG       0X42    // 温度传感器低8位数据寄存器
#define MPU_GYRO_XOUTH_REG      0X43    // 陀螺仪X轴高8位数据寄存器
#define MPU_GYRO_XOUTL_REG      0X44    // 陀螺仪X轴低8位数据寄存器
#define MPU_GYRO_YOUTH_REG      0X45    // 陀螺仪Y轴高8位数据寄存器
#define MPU_GYRO_YOUTL_REG      0X46    // 陀螺仪Y轴低8位数据寄存器
#define MPU_GYRO_ZOUTH_REG      0X47    // 陀螺仪Z轴高8位数据寄存器
#define MPU_GYRO_ZOUTL_REG      0X48    // 陀螺仪Z轴低8位数据寄存器
#define MPU_I2CSLV0_DO_REG      0X63    // I2C从设备0写数据寄存器
#define MPU_I2CSLV1_DO_REG      0X64    // I2C从设备1写数据寄存器
#define MPU_I2CSLV2_DO_REG      0X65    // I2C从设备2写数据寄存器
#define MPU_I2CSLV3_DO_REG      0X66    // I2C从设备3写数据寄存器
#define MPU_I2CMST_DELAY_REG    0X67    // I2C主延迟控制寄存器
#define MPU_SIGPATH_RST_REG     0X68    // 信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG    0X69    // 运动检测控制寄存器
#define MPU_USER_CTRL_REG       0X6A    // 用户控制寄存器
#define MPU_PWR_MGMT1_REG       0X6B    // 电源管理1寄存器
#define MPU_PWR_MGMT2_REG       0X6C    // 电源管理2寄存器
#define MPU_FIFO_CNTH_REG       0X72    // FIFO计数高8位寄存器
#define MPU_FIFO_CNTL_REG       0X73    // FIFO计数低8位寄存器
#define MPU_FIFO_RW_REG         0X74    // FIFO读写寄存器
#define MPU_DEVICE_ID_REG       0X75    // 设备ID寄存器


#define MPU_ADDR							0X68

#define MPU_READ    0XD1
#define MPU_WRITE   0XD0
extern struct mpu6050_data outMpu;
uint8_t MPU_Init(void); 								
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf); 
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data);				
uint8_t MPU_Read_Byte(uint8_t reg);						

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_LPF(uint16_t lpf);
uint8_t MPU_Set_Rate(uint16_t rate);
uint8_t MPU_Set_Fifo(uint8_t sens);


short MPU_Get_Temperature(void);
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az);

#endif
