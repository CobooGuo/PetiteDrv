/**
 * @file            
 * @brief           
 * @author          wujique
 * @date            2018��1��6�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��1��6�� ������
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

	�����������ǣ���ʱ��ع��ܹ���

*/
#include "mcu.h"
#include "board_sysconf.h"
#include "log.h"

#include "stimer.h"

volatile uint32_t LocalTime = 0; 
uint32_t timingdelay;

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}


/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  ����������ϵͳ�δ���ִ�У�ͨ���δ���1ms
  ��board_mcu_preinit�У����û����RTOS�����ʼ��һ���㱼�õĵδ�ʱ����
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}


uint32_t Stime_get_localtime(void)
{
	return LocalTime;
}


