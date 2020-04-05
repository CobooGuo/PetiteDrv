/**
 * @file            mcu_bsp_stm32.c
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
#include "mcu.h"
#include "log.h"
#include "mcu_io.h"

const GPIO_TypeDef *Stm32PortList[MCU_PORT_MAX] = {NULL, GPIOA,  GPIOB, GPIOC, GPIOD,
									GPIOE, GPIOF, GPIOG};
/*
	��ʼ������IO��ʱ��
*/
void mcu_io_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

	//��GPIO��ʱ�ӣ��ȴ򿪸��ò����޸ĸ��ù���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  

}


s32 mcu_io_config_timer(MCU_PORT port, MCU_IO pin, McuTimerNum timer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(port == NULL)
		return -1;
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    GPIO_InitStructure.GPIO_Pin = pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //---���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //---�ٶ� 50MHz
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);
	return 0;
}

s32 mcu_io_config_dac(MCU_PORT port, MCU_IO pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//---ģ��ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);//---��ʼ�� GPIO
	return 0;
}

void mcu_io_config_in(MCU_PORT port, MCU_IO pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	if(port == NULL)
		return;
	
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);//��ʼ��  	
}


void mcu_io_config_out(MCU_PORT port, MCU_IO pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(port == NULL)
		return;
	
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ���ģʽ	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure);//��ʼ��
}

/*
	���Ҫ�ģ���Ϊ���ù��ܣ�������������ν��AF��AF��STM32�ĸ���
	Ҫ��Ϊ������ΪPWM��DAC��ADC��ͨ�ù���
*/
void mcu_io_config_af(MCU_PORT port, MCU_IO pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(port == NULL)
		return;
	
	GPIO_InitStructure.GPIO_Pin = pin; //GPIOA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ� 100MHz
	GPIO_Init((GPIO_TypeDef *)Stm32PortList[port], &GPIO_InitStructure); //��ʼ�� PA3
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
		return Bit_SET;
	
	return GPIO_ReadInputData((GPIO_TypeDef *)Stm32PortList[port]);
}



