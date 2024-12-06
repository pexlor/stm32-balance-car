#include "stm32f10x.h"                  // Device header
#include "mpu6050.h"
#include "system.h"
struct mpu6050_data outMpu;

uint8_t MPU_Init(void)
{
    uint8_t res;
		
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//重置设备
    delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒设备
    MPU_Set_Gyro_Fsr(3); //设置陀螺仪量程
    MPU_Set_Accel_Fsr(0);	//设置加速度计量程
    MPU_Set_Rate(50);	//设置采样率
    MPU_Write_Byte(MPU_INT_EN_REG,0X00);	// 禁用所有中断
    MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	// 禁用 I2C 主模式
    MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	// 禁用 FIFO 缓存
    //MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80); // 配置 INT 引脚为推挽输出，低电平有效
    res=MPU_Read_Byte(MPU_DEVICE_ID_REG); // 读取设备 ID
		
    if(res==MPU_ADDR)
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);
        MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	
        MPU_Set_Rate(50);						
    } else return 1;
    return 0;
}


uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);
}


uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);
}


uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);
}

uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	
    return MPU_Set_LPF(rate/2);	
}


short MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    short raw;
    float temp;
    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}


uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((uint16_t)buf[0]<<8)|buf[1];
        *gy=((uint16_t)buf[2]<<8)|buf[3];
        *gz=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;;
}

uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;;
}

uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|0);
    if(MPU_IIC_Wait_Ack())	
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);
    MPU_IIC_Wait_Ack();		
    for(i=0; i<len; i++)
    {
        MPU_IIC_Send_Byte(buf[i]);	
        if(MPU_IIC_Wait_Ack())	
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Stop();
    return 0;
}


uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|0);
    if(MPU_IIC_Wait_Ack())	
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	
    MPU_IIC_Wait_Ack();		
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|1);
    MPU_IIC_Wait_Ack();		
		for(int i =0 ;i < len;i++)
		{
			if(i == len-1){
				buf[i] = MPU_IIC_Read_Byte(0);
			}else{
				buf[i] = MPU_IIC_Read_Byte(1);
			}
		}
    MPU_IIC_Stop();	
    return 0;
}

uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);
    if(MPU_IIC_Wait_Ack())	
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	
    MPU_IIC_Wait_Ack();		
    MPU_IIC_Send_Byte(data);
    if(MPU_IIC_Wait_Ack())	
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}

uint8_t MPU_Read_Byte(uint8_t reg)
{
    uint8_t res;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);
    MPU_IIC_Wait_Ack();		
    MPU_IIC_Send_Byte(reg);	
    MPU_IIC_Wait_Ack();		
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);
    MPU_IIC_Wait_Ack();		
    res=MPU_IIC_Read_Byte(0);
    MPU_IIC_Stop();			
    return res;
}
