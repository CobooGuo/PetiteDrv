/**
 * @file            mcu_i2c.c
 * @brief           IOģ��I2C
 * @author          test
 * @date            2017��10��30�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:      2017��10��30�� ����һ
 *   ��    ��:      �ݼ�ȸ������
 *   �޸�����:      �����ļ�
    	1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱���������������뱣��WUJIQUE��Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
		8 һ�н���Ȩ���ݼ�ȸ���������С�
*/

#include "mcu.h"
#include "log.h"
#include "bus_i2c.h"

#define MCU_I2C_DEBUG

#ifdef MCU_I2C_DEBUG
#define I2C_DEBUG	wjq_log 
#else
#define I2C_DEBUG(a, ...)
#endif

/*
	����ͷʹ��SCCB�ӿڣ���ʵ����I2C
	��ST�ٷ�����ͷ��ֲ��ʹ�õ���Ӳ��I2C
*/
#define CAREMA_SCCB_I2C 	I2C2 //ʹ��I2C2������
#define SCCB_TIMEOUT_MAX    10000
	
void SCCB_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStruct;	  
	/****** Configures the I2C1 used for OV9655 camera module configuration *****/
	/* I2C1 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	/* GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 
	
	/* Connect I2C2 pins to AF4 */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2);
	
	/* Configure I2C2 GPIOs */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* Configure I2C2 */
	/* I2C DeInit */
	I2C_DeInit(CAREMA_SCCB_I2C);
	  
	///* Enable the I2C peripheral */
	//I2C_Cmd(CAREMA_SCCB_I2C, ENABLE);
	
	/* Set the I2C structure parameters */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 10000;
	
	/* Initialize the I2C peripheral w/ selected parameters */
	I2C_Init(CAREMA_SCCB_I2C, &I2C_InitStruct);
	
	I2C_Cmd(CAREMA_SCCB_I2C, ENABLE);
	
	I2C_AcknowledgeConfig(CAREMA_SCCB_I2C, ENABLE);

}
	
uint8_t bus_sccb_writereg(uint8_t DeviceAddr, uint16_t Addr, uint8_t Data)
{
	
	uint32_t timeout = SCCB_TIMEOUT_MAX;
	
	/* Generate the Start Condition */
	I2C_GenerateSTART(CAREMA_SCCB_I2C, ENABLE);
	
	/* Test on I2C1 EV5 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}
	 
	/* Send DCMI selected device slave Address for write */
	I2C_Send7bitAddress(CAREMA_SCCB_I2C, DeviceAddr, I2C_Direction_Transmitter);
	
	/* Test on I2C1 EV6 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}
	
	/* Send I2C1 location address LSB */
	I2C_SendData(CAREMA_SCCB_I2C, (uint8_t)(Addr));
	
	/* Test on I2C1 EV8 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}
	
	/* Send Data */
	I2C_SendData(CAREMA_SCCB_I2C, Data);
	
	/* Test on I2C1 EV8 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}  
	
	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP(CAREMA_SCCB_I2C, ENABLE);
	
	/* If operation is OK, return 0 */
	return 0;

}
	
uint8_t bus_sccb_readreg(uint8_t DeviceAddr, uint16_t Addr)
{
  uint32_t timeout = SCCB_TIMEOUT_MAX;
  uint8_t Data = 0;

  /* Generate the Start Condition */
  I2C_GenerateSTART(CAREMA_SCCB_I2C, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }
  
  /* Send DCMI selected device slave Address for write */
  I2C_Send7bitAddress(CAREMA_SCCB_I2C, DeviceAddr, I2C_Direction_Transmitter);
 
  /* Test on I2C1 EV6 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }

  /* Send I2C1 location address LSB */
  I2C_SendData(CAREMA_SCCB_I2C, (uint8_t)(Addr));

  /* Test on I2C1 EV8 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Clear AF flag if arised */
  CAREMA_SCCB_I2C->SR1 |= (uint16_t)0x0400;

  /* Generate the Start Condition */
  I2C_GenerateSTART(CAREMA_SCCB_I2C, ENABLE);
  
  /* Test on I2C1 EV6 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Send DCMI selected device slave Address for write */
  I2C_Send7bitAddress(CAREMA_SCCB_I2C, DeviceAddr, I2C_Direction_Receiver);

  /* Test on I2C1 EV6 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }
 
  /* Prepare an NACK for the next data received */
  I2C_AcknowledgeConfig(CAREMA_SCCB_I2C, DISABLE);

  /* Test on I2C1 EV7 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }

  /* Prepare Stop after receiving data */
  I2C_GenerateSTOP(CAREMA_SCCB_I2C, ENABLE);

  /* Receive the Data */
  Data = I2C_ReceiveData(CAREMA_SCCB_I2C);

  /* return the read data */
  return Data;
}


s32 mcu_hi2c_init(DevI2c * dev)
{
	return -1;
}


s32 mcu_hi2c_transfer(DevI2cNode *node, u8 addr, u8 rw, u8* data, s32 datalen)
{
	if(rw == MCU_I2C_MODE_W)//д
    {

    }
    else if(rw == MCU_I2C_MODE_R)//��
    {
		
    }
	return -1;
}

