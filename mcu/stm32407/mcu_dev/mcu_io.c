/**
 * @file            mcu_io.c
 * @brief           adc����
 * @author          wujique
 * @date            2017��12��8�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2017��12��8�� ������
 *   ��    ��:         wujique
 *   �޸�����:   �����ļ�
       	1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱���������������뱣��WUJIQUE��Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
		8 һ�н���Ȩ���ݼ�ȸ���������С�
*/
/*
	��װIO�������Ա���ֲ������оƬ
*/
#include "stm32f4xx.h"

#include "mcu.h"

#include "log.h"

const GPIO_TypeDef *Stm32PortList[MCU_PORT_MAX] = {NULL, GPIOA,  GPIOB, GPIOC, GPIOD,
									GPIOE, GPIOF, GPIOG, GPIOH,
									GPIOI, GPIOJ, GPIOK};

/*
	��ʼ������IO��ʱ��
*/
void mcu_io_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK, ENABLE);
}

uint8_t GpioTimDef[MCU_TIMER_MAX]=
	{
		NULL,
		GPIO_AF_TIM1,
		GPIO_AF_TIM2,	
		GPIO_AF_TIM3,
		GPIO_AF_TIM4,
		GPIO_AF_TIM5,
		NULL,	
		NULL,
		GPIO_AF_TIM8,
		GPIO_AF_TIM9,
		GPIO_AF_TIM10,	
		GPIO_AF_TIM11,
		GPIO_AF_TIM12,
		GPIO_AF_TIM13,
		GPIO_AF_TIM14,	
	};


s32 mcu_io_config_timer(MCU_PORT port, MCU_IO pin, McuTimerNum timer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t pinsource;
	uint8_t GPIO_AF;
	
	if(port == NULL)
		return -1;

	if(timer == MCU_TIMER_6 || timer == MCU_TIMER_7)
		return -1;
	
	GPIO_AF = GpioTimDef[timer];

	pinsource = log2(pin);
	/*need fix*/
	GPIO_PinAFConfig((GPIO_TypeDef *)Stm32PortList[port], pinsource, GPIO_AF); //---�ܽŸ���Ϊ TIM4����
	
    GPIO_InitStructure.GPIO_Pin = pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //---���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //---�ٶ� 50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //---���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //---����
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);
}

s32 mcu_io_config_dac(MCU_PORT port, MCU_IO pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//---ģ��ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//---����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);//---��ʼ�� GPIO
}
/*
	��IO������Ϊ���룬 ����
*/
void mcu_io_config_in(MCU_PORT port, MCU_IO pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	if(port == NULL)
		return;
	
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);//��ʼ��  	
}


void mcu_io_config_out(MCU_PORT port, MCU_IO pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(port == NULL)
		return;
	
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);//��ʼ��
}

void mcu_io_output_setbit(MCU_PORT port, MCU_IO pin)
{
	if(port == NULL)
		return;
	
	GPIO_SetBits((GPIO_TypeDef *)Stm32PortList[port], pin);
}

void mcu_io_output_resetbit(MCU_PORT port, MCU_IO pin)
{
	if(port == NULL)
		return;
	
	GPIO_ResetBits((GPIO_TypeDef *)Stm32PortList[port], pin);
}		

u8 mcu_io_input_readbit(MCU_PORT port, MCU_IO pin)
{
	if(port == NULL)
		return Bit_SET;
	
	return GPIO_ReadInputDataBit((GPIO_TypeDef *)Stm32PortList[port], pin);
}

u16 mcu_io_input_readport(MCU_PORT port)
{
	if(port == NULL)
		return NULL;
	
	return GPIO_ReadInputData((GPIO_TypeDef *)Stm32PortList[port]);
}



