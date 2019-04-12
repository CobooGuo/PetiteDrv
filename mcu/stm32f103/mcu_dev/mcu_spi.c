/**
 * @file            mcu_spi.c
 * @brief           Ƭ��SPI����������
 * @author          test
 * @date            2017��10��26�� ������
 * @version         ����
 * @par             
 * @par History:
 * 1.��    ��:      2017��10��26�� ������
 *   ��    ��:      test
 *   �޸�����:      �����ļ�
		��Ȩ˵����
		1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱�������������ұ�����Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
		8 һ�н���Ȩ���ݼ�ȸ���������С�
*/
#include "mcu.h"
#include "log.h"
#include "board_sysconf.h"
#include "bus_spi.h"

#define MCU_SPI_DEBUG

#ifdef MCU_SPI_DEBUG
#define SPI_DEBUG	wjq_log 
#else
#define SPI_DEBUG(a, ...)
#endif


#define MCU_SPI_WAIT_TIMEOUT 0x40000

/*
	��λ���ã�һ������ģʽ
*/
typedef struct
{
	u16 CPOL;
	u16 CPHA;	
}_strSpiModeSet;

const _strSpiModeSet SpiModeSet[SPI_MODE_MAX]=
	{
		{SPI_CPOL_Low, SPI_CPHA_1Edge},
		{SPI_CPOL_Low, SPI_CPHA_2Edge},
		{SPI_CPOL_High, SPI_CPHA_1Edge},
		{SPI_CPOL_High, SPI_CPHA_2Edge}
	};


/**
 *@brief:      mcu_spi_init
 *@details:    ��ʼ��SPI����������ʱ֧��SPI3
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_hspi_init(const DevSpi *dev)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	if(strcmp(dev->name, "SPI3") == 0)
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    }
	else if(strcmp(dev->name, "SPI1") == 0)
	{	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	}
	else if(strcmp(dev->name, "SPI2") == 0)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	}
	else
	{
		return -1;
	}

	/*����IO��*/
    GPIO_InitStructure.GPIO_Pin = dev->clkpin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[dev->clkport], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = dev->misopin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[dev->misoport], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = dev->mosipin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)Stm32PortList[dev->mosiport], &GPIO_InitStructure);

    return 0;
}

/**
 *@brief:      mcu_spi_open
 *@details:       ��SPI
 *@param[in]   SPI_DEV dev  ��SPI��
               u8 mode      ģʽ
               u16 pre      Ԥ��Ƶ
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_hspi_open(DevSpiNode *node, SPI_MODE mode, u16 pre)
{
	SPI_InitTypeDef SPI_InitStruct;
	SPI_TypeDef* SPIC;
	
	if(node->gd != -1)
	{
		//SPI_DEBUG(LOG_DEBUG, "spi dev busy\r\n");
		return -1;
	}
	
	if(mode >= SPI_MODE_MAX)
		return -1;

	if(strcmp(node->dev.name, "SPI3") == 0)
    {
		SPIC = SPI3;
    }

	SPI_I2S_DeInit(SPIC);
	SPI_Cmd(SPIC, DISABLE); 
	
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//---˫��˫��ȫ˫��
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//---��ģʽ
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//---8bit֡�ṹ
    SPI_InitStruct.SPI_CPOL = SpiModeSet[mode].CPOL;
    SPI_InitStruct.SPI_CPHA = SpiModeSet[mode].CPHA;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; //---SPI_NSS_Hard; Ƭѡ��Ӳ������SPI������������
    SPI_InitStruct.SPI_BaudRatePrescaler = pre;  //---Ԥ��Ƶ
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//---���ݴ���� MSB λ��ʼ
    SPI_InitStruct.SPI_CRCPolynomial = 7;//---CRC ֵ����Ķ���ʽ

    SPI_Init(SPIC, &SPI_InitStruct);

	SPI_Cmd(SPIC, ENABLE);
	
	node->gd = 0;
		
    return 0;
}
/**
 *@brief:      mcu_spi_close
 *@details:    �ر�SPI ������
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_hspi_close(DevSpiNode *node)
{
    SPI_TypeDef* SPIC;
	
	if(node->gd != 0)
		return -1;
	
	if(strcmp(node->dev.name, "SPI3") == 0)
    {
		SPIC = SPI3;
    }
	
	SPI_Cmd(SPIC, DISABLE);
	node->gd = -1;
    return 0;
}
/**
 *@brief:      mcu_spi_transfer
 *@details:    SPI ����
 *@param[in]   u8 *snd  
               u8 *rsv  
               s32 len  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_hspi_transfer(DevSpiNode *node, u8 *snd, u8 *rsv, s32 len)
{
    s32 i = 0;
    s32 pos = 0;
    u32 time_out = 0;
    u16 ch;
	SPI_TypeDef* SPIC;
	
	if(node == NULL)
		return -1;

	if(node->gd != 0)
	{
		SPI_DEBUG(LOG_DEBUG, "spi dev no open\r\n");
		return -1;
	}
	
    if( ((snd == NULL) && (rsv == NULL)) || (len < 0) )
    {
        return -1;
    }
	
    if(strcmp(node->dev.name, "SPI3") == 0)
    {
		SPIC = SPI3;
    }
    /* æ�ȴ� */
    time_out = 0;
    while(SPI_I2S_GetFlagStatus(SPIC, SPI_I2S_FLAG_BSY) == SET)
    {
        if(time_out++ > MCU_SPI_WAIT_TIMEOUT)
        {
            return(-1);
        }
    }

    /* ���SPI�������ݣ���ֹ�����ϴδ������������� */
    time_out = 0;
    while(SPI_I2S_GetFlagStatus(SPIC, SPI_I2S_FLAG_RXNE) == SET)
    {
        SPI_I2S_ReceiveData(SPIC);
        if(time_out++ > 2)
        {
            return(-1);
        }
    }

    /* ��ʼ���� */
    for(i=0; i < len; )
    {
        // д����
        if(snd == NULL)/*����ָ��ΪNULL��˵�������Ƕ����� */
        {
            SPI_I2S_SendData(SPIC, 0xff);
        }
        else
        {
            ch = (u16)snd[i];
            SPI_I2S_SendData(SPIC, ch);
        }
        i++;
        
        // �ȴ����ս���
        time_out = 0;
        while(SPI_I2S_GetFlagStatus(SPIC, SPI_I2S_FLAG_RXNE) == RESET)
        {
            time_out++;
            if(time_out > MCU_SPI_WAIT_TIMEOUT)
            {
                return -1;
            }    
        }
        // ������
        if(rsv == NULL)/* ����ָ��Ϊ�գ������ݺ��� */
        {
            SPI_I2S_ReceiveData(SPIC);
        }
        else
        {
            ch = SPI_I2S_ReceiveData(SPIC);
            rsv[pos] = (u8)ch;
        } 
        pos++;

    }

    return i;
}


