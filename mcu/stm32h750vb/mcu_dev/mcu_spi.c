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
#include "bus_spi.h"
#include "board_sysconf.h"

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
	/* ע��H750 ��U32*/
	u32 CPOL;
	u32 CPHA;	
}_strSpiModeSet;

const _strSpiModeSet SpiModeSet[SPI_MODE_MAX]=
	{
		{SPI_POLARITY_LOW, SPI_PHASE_1EDGE},
		{SPI_POLARITY_LOW, SPI_PHASE_2EDGE},
		{SPI_POLARITY_HIGH, SPI_PHASE_1EDGE},
		{SPI_POLARITY_HIGH, SPI_PHASE_2EDGE}
	};

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
SPI_HandleTypeDef McuHspi2;
static void mcu_MX_SPI2_Init(void)
{
  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  McuHspi2.Instance = SPI2;
  McuHspi2.Init.Mode = SPI_MODE_MASTER;
  McuHspi2.Init.Direction = SPI_DIRECTION_2LINES;
  McuHspi2.Init.DataSize = SPI_DATASIZE_8BIT;  
  McuHspi2.Init.NSS = SPI_NSS_SOFT;
  McuHspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  McuHspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  McuHspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  McuHspi2.Init.CRCPolynomial = 0x7;
  McuHspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  McuHspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  McuHspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  McuHspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  McuHspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  McuHspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_02CYCLE;
  McuHspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_02CYCLE;
  McuHspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  McuHspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  McuHspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  
  //if (HAL_SPI_Init(&McuHspi2) != HAL_OK)
  //{
	//Error_Handler();
  //}
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
 *@brief:      mcu_spi_init
 *@details:    ��ʼ��SPI����������ʱ֧��SPI3
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_hspi_init(const DevSpi *dev)
{
	SPI_DEBUG(LOG_DEBUG, ">----mcu_hspi_init\r\n");
	mcu_MX_SPI2_Init();
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
s32 mcu_hspi_open(DevSpiNode *node, SPI_MODE mode, u16 KHz)
{

	if (node->gd != -1) {
		SPI_DEBUG(LOG_DEBUG, "spi dev busy\r\n");
		return -1;
	}
	
	if(mode >= SPI_MODE_MAX)
		return -1;
	
	if (strcmp(node->dev.pnode.name, "SPI2") != 0) {
		return -1;
    }

	McuHspi2.Init.CLKPolarity = SpiModeSet[mode].CPOL;
  	McuHspi2.Init.CLKPhase = SpiModeSet[mode].CPHA;

	/* ���ݴ����KHz �����Ƶ*/
	McuHspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	
	if (HAL_SPI_Init(&McuHspi2) != HAL_OK) {
		Error_Handler();
  	}

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
	
	if(node->gd != 0) return -1;
	
	if (strcmp(node->dev.pnode.name, "SPI2") == 0) {

    }
	

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

	HAL_StatusTypeDef res;
	
	if(node == NULL) return -1;

	if (node->gd != 0) {
		SPI_DEBUG(LOG_DEBUG, "spi dev no open\r\n");
		return -1;
	}
	
    if ( ((snd == NULL) && (rsv == NULL)) || (len < 0) ) {
        return -1;
    }
	
	if (strcmp(node->dev.pnode.name, "SPI2") != 0) {
		return -1;
	}
	
	if (snd == NULL)	{
		//SPI_DEBUG(LOG_DEBUG, "r ");
		res = HAL_SPI_Receive(&McuHspi2, rsv, len, 20);//need fix ��ʱҪ�����ٶȺ����ݶ�
	} else if(rsv == NULL) {	
		//SPI_DEBUG(LOG_DEBUG, "t ");
		res = HAL_SPI_Transmit(&McuHspi2, snd, len, 20);
	} else {
		//SPI_DEBUG(LOG_DEBUG, "tr ");
		res = HAL_SPI_TransmitReceive(&McuHspi2, snd, rsv, len, 20);
	}
	//SPI_DEBUG(LOG_DEBUG, "spi tra: %d\r\n", res);
	if(res != 0 )
		return -1;
	
	i = len;
    return i;
}


