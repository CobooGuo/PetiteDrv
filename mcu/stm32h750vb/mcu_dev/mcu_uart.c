/**
 * @file            mcu_uart.c
 * @brief           ��������
 * @author          wujique
 * @date            2017��10��24�� ���ڶ�
 * @version         ����
 * @par             
 * @par History:
 * 1.��    ��:      2017��10��24�� ���ڶ�
 *   ��    ��:     
 *   �޸�����:        �����ļ�
		��Ȩ˵����
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
#include "mcu_uart.h"
#include "log.h"
#include "alloc.h"


#define MCU_UART1_BUF_SIZE       1
#define MCU_UART2_BUF_SIZE       1
#define MCU_UART3_BUF_SIZE       256
/*������Ϣ����*/
#define MCU_UART4_BUF_SIZE       256


#if 0
/*���ڻ��� ��ʱͨ���������飬������Ϊ��̬����*/
u8 McuUart1Buf[MCU_UART1_BUF_SIZE];
u8 McuUart2Buf[MCU_UART2_BUF_SIZE];
u8 McuUart3Buf[MCU_UART3_BUF_SIZE];
#endif
u8 McuUartLogBuf[MCU_UART4_BUF_SIZE];

/*
@bref�������豸
*/
typedef struct  
{	
	/* Ӳ�����*/
	/*
		STM IO ������Ҫ̫�����ݣ�����ֱ���ڴ����ж��壬 �����ú궨��
		����Ǹ���һ���豸����������FLASH ���Ϳ������豸�����ж���һ�����ĸ�SPI�Ķ��塣
	*/

	USART_TypeDef* USARTx;

	/*RAM���*/
	s32 gd;	//�豸��� С�ڵ���0��Ϊδ���豸
	
	u16 size;// buf ��С
	u8 *Buf;//����ָ��
	u16 Head;//ͷ
	u16 End;//β
	u8  OverFg;//�����־	
}_strMcuUart; 

static _strMcuUart McuUart[MCU_UART_MAX];

UART_HandleTypeDef McuHuart4;
UART_HandleTypeDef McuHuart2;

/*------------------------------------------------------*/
/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
/*
	Ҫע�⣬Ӳ����ʼ��������
	void HAL_UART_MspInit(UART_HandleTypeDef* huart)��
	������cube���ɵġ�
*/
static void mcu_MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  McuHuart4.Instance = UART4;
  McuHuart4.Init.BaudRate = 115200;
  McuHuart4.Init.WordLength = UART_WORDLENGTH_8B;
  McuHuart4.Init.StopBits = UART_STOPBITS_1;
  McuHuart4.Init.Parity = UART_PARITY_NONE;
  McuHuart4.Init.Mode = UART_MODE_TX_RX;
  McuHuart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  McuHuart4.Init.OverSampling = UART_OVERSAMPLING_16;
  McuHuart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  McuHuart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  McuHuart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&McuHuart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&McuHuart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&McuHuart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&McuHuart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void mcu_MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  McuHuart2.Instance = USART2;
  McuHuart2.Init.BaudRate = 115200;
  McuHuart2.Init.WordLength = UART_WORDLENGTH_8B;
  McuHuart2.Init.StopBits = UART_STOPBITS_1;
  McuHuart2.Init.Parity = UART_PARITY_NONE;
  McuHuart2.Init.Mode = UART_MODE_TX_RX;
  McuHuart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  McuHuart2.Init.OverSampling = UART_OVERSAMPLING_16;
  McuHuart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  McuHuart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  McuHuart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&McuHuart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&McuHuart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&McuHuart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&McuHuart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
 *@brief:      mcu_uart_init
 *@details:       ��ʼ�������豸
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_init(void)
{
	u8 i;

	for(i = 0; i<MCU_UART_MAX; i++)
	{
		McuUart[i].gd = -1;	
	}

	return 0;	
}

/**
 *@brief:      mcu_uart_open
 *@details:    �򿪴��ڣ�ʵ�ʾ��ǳ�ʼ������
 *@param[in]   s32 comport  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_open(McuUartNum comport)
{

	if(comport >= MCU_UART_MAX)
		return -1;

	if(McuUart[comport].gd >=0)
		return -1;

	if(comport == MCU_UART_1)
	{

	}
	else if(comport == MCU_UART_2)
	{
		McuUart[MCU_UART_2].End = 0;
		McuUart[MCU_UART_2].Head = 0;
		McuUart[MCU_UART_2].OverFg = 0;
		McuUart[MCU_UART_2].size = MCU_UART2_BUF_SIZE;
		McuUart[MCU_UART_2].gd = 0;
		
		McuUart[MCU_UART_2].Buf = (u8 *)wjq_malloc(MCU_UART2_BUF_SIZE);

		mcu_MX_USART2_UART_Init();
		
		
	}
	else if(comport == MCU_UART_4)
	{
		McuUart[MCU_UART_4].End = 0;
		McuUart[MCU_UART_4].Head = 0;
		McuUart[MCU_UART_4].OverFg = 0;
		McuUart[MCU_UART_4].size = MCU_UART4_BUF_SIZE;
		McuUart[MCU_UART_4].gd = 0;
		McuUart[MCU_UART_4].Buf = McuUartLogBuf;//(u8 *)wjq_malloc(MCU_UART3_BUF_SIZE);
		mcu_MX_UART4_Init();
	}
	else
	{
		/* ���ںŲ�֧��*/
		return -1;
	}
	return (0);
}
/**
 *@brief:      mcu_uart_close
 *@details:    �رմ���
 *@param[in]   s32 comport  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_close (McuUartNum comport)
{
	if(comport >= MCU_UART_MAX)
		return -1;

	if(McuUart[comport].gd < 0)
		return 0;

	if(comport == MCU_UART_2)
	{
		

	}
	else if(comport == MCU_UART_4)
	{

	} 
	else
		return -1;
	
	wjq_free(McuUart[comport].Buf);
	
	McuUart[comport].gd = -1;
	
	return(0);
}

/**
 *@brief:      mcu_dev_uart_tcflush
 *@details:    �崮�ڽ��ջ���
 *@param[in]   s32 comport  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_tcflush(McuUartNum comport)
{ 
	if(comport >= MCU_UART_MAX)
		return -1;

	if(McuUart[comport].gd < 0)
		return -1;
	
	McuUart[comport].Head = McuUart[comport].End; 
    return 0;
}
/**
 *@brief:      mcu_dev_uart_set_baud
 *@details:    ���ô��ڲ�����
 *@param[in]   s32 comport   
               s32 baud      
               s32 databits  
               s32 parity    
               s32 stopbits  
               s32 flowctl   
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_set_baud (McuUartNum comport, s32 baud)
{

	if(comport >= MCU_UART_MAX)
		return -1;
	if(McuUart[comport].gd < 0)
		return -1;
	
	return 0;
}

/**
 *@brief:      mcu_uart_read
 *@details:    ���������ݣ��������ݣ�
 *@param[in]   s32 comport  
               u8 *buf      
               s32 len      
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_read (McuUartNum comport, u8 *buf, s32 len)
{
    s32 i;
    
    if(len <= 0) return(-1);
    if(buf == NULL) return(-1);
	if(comport >= MCU_UART_MAX)
		return -1;
	
	if(McuUart[comport].gd < 0)
		return -1;
	
    i = 0;

    //uart_printf("rec index:%d, %d\r\n", UartHead3, rec_end3);
    while(McuUart[comport].Head != McuUart[comport].End)
    {
        *buf = *(McuUart[comport].Buf + McuUart[comport].Head);
		McuUart[comport].Head++;
        if(McuUart[comport].Head >= McuUart[comport].size) 
            McuUart[comport].Head = 0;

        buf ++;
        i ++;
        if(i >= len)
        {
            break;
        }
  }
  return (i);
}
/**
 *@brief:      mcu_dev_uart_write
 *@details:    д��������(��������)
 *@param[in]   s32 comport  
               u8 *buf      
               s32 len      
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_uart_write (McuUartNum comport, u8 *buf, s32 len)
{
	u32 t;
	u16 ch;
  
	if (len <= 0) 
		return(-1);
		
	if(buf == NULL) 
		return(-1);
	
	if(comport >= MCU_UART_MAX)
		return -1;

	if(McuUart[comport].gd < 0)
		return -1;

	if(comport == MCU_UART_2)
	{
		HAL_UART_Transmit(&McuHuart2, buf, len, 100);	

	}
	else if(comport == MCU_UART_4)
	{
		HAL_UART_Transmit(&McuHuart4, buf, len, 100);
	} 
	else
		return -1;
	
	
	return(0);
}
/**
 *@brief:      mcu_dev_uart_test
 *@details:    ���ڲ���
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
void mcu_uart_test(void)
{
    u8 buf[12];
    s32 len;
    s32 res;

	wjq_log(LOG_FUN, "%s,%s,%d,%s\r\n", __FUNCTION__,__FILE__,__LINE__,__DATE__);
	while(1)
	{
	    len =  mcu_uart_read (MCU_UART_4, buf, 10);
		if(len != 0)
		{
	    	wjq_log(LOG_FUN, "mcu_dev_uart_read :%d\r\n", len);
	    	res = mcu_uart_write(MCU_UART_4, buf, len);
	    	wjq_log(LOG_FUN, "mcu_dev_uart_write res: %d\r\n", res);
		}
	}
    
}

