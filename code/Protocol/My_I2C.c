#include "stm32f10x.h"                  // Device header
#include "My_I2C.h"
#include "system.h"

#define GPIO_PORT_IIC     GPIOB                       
#define IIC_SCL_PIN       GPIO_Pin_8                   
#define IIC_SDA_PIN       GPIO_Pin_9                 
#define IIC_DELAY_TIME		0

/*引脚配置*/
#define IIC_W_SCL(x)		GPIO_WriteBit(GPIO_PORT_IIC, IIC_SCL_PIN, (BitAction)(x))
#define IIC_W_SDA(x)		GPIO_WriteBit(GPIO_PORT_IIC, IIC_SDA_PIN, (BitAction)(x))

#define IIC_R_SDA()  GPIO_ReadInputDataBit(GPIO_PORT_IIC, IIC_SDA_PIN)	

void IIC_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
    IIC_W_SCL(1);
		IIC_W_SDA(1);
}

static void IIC_Delay(void)
{
   delay_us(IIC_DELAY_TIME);
}

/**
  * @brief  IIC开始
  * @param  无
  * @retval 无
  */
void IIC_Start(void)
{
	IIC_W_SDA(1);
	
	
	IIC_W_SCL(1);
	
	
	IIC_W_SDA(0);
	
	
	IIC_W_SCL(0);
	
}

/**
  * @brief  IIC停止
  * @param  无
  * @retval 无
  */
void IIC_Stop(void)
{
	IIC_W_SDA(0);
	
	
	IIC_W_SCL(1);
	
	
	IIC_W_SDA(1);
	
}

uint8_t IIC_Wait_Ack(void)
{
    uint8_t re;

    IIC_W_SDA(1);	
		
    IIC_W_SCL(1);	
    
	
    if (IIC_R_SDA())
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC_W_SCL(0);
		
    return re;
}


void IIC_Ack(void)
{
    IIC_W_SDA(0);	
    
	
    IIC_W_SCL(1);	
    
	
    IIC_W_SCL(0);
    

}

void IIC_NAck(void)
{
    IIC_W_SDA(1);	
    
	
    IIC_W_SCL(1);	
    
	
    IIC_W_SCL(0);
    
}

/**
  * @brief  IIC发送一个字节
  * @param  byte 要发送的一个字节
  * @retval 无
  */
void IIC_Send_Byte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		IIC_W_SDA(byte & (0x80 >> i));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
	//IIC_Wait_Ack();
}


uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value = 0x00;
		IIC_W_SDA(1);
	
    for (i = 0; i < 8; i++)
    {
        IIC_W_SCL(1);
				
        
				if (IIC_R_SDA())
        {
            value |= (0x80 >> i);
        }
				
        IIC_W_SCL(0);
				
    }
		if(ack){
			IIC_Ack();
		}else{
			IIC_NAck();
		}
		
    return value;
}

uint8_t IIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    IIC_GPIO_Init();		

    IIC_Start();		

    IIC_Send_Byte(_Address|IIC_WR);
    ucAck = IIC_Wait_Ack();
    IIC_Stop();		

    return ucAck;
}
