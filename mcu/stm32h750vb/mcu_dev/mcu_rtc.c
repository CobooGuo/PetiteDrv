/**
 * @file            mcu_rtc.c
 * @brief           stm32Ƭ��RTC����
 * @author          wujique
 * @date            2018��1��24�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��1��24�� ������
 *   ��    ��:         wujique
 *   �޸�����:   �����ļ�
*/
#include <stdarg.h>
#include <stdio.h>
#include "mcu.h"
#include "log.h"
#include "mcu_rtc.h"

/**
 *@brief:      mcu_rtc_init
 *@details:    ��λʱ��ʼ��RTC
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_rtc_init(void)
{

	wjq_log(LOG_INFO, " init rtc finish\r\n");	
	return 0;	
}

u8 RTC_Set(u32 sec)
{

	return 0;	    
}	  
/**
 *@brief:      mcu_rtc_set_time
 *@details:    ����ʱ��
 *@param[in]   u8 hours    
               u8 minutes  
               u8 seconds  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_rtc_set_time(u8 hours, u8 minutes, u8 seconds)
{

	return 0;
}		
/**
 *@brief:      mcu_rtc_set_date
 *@details:    ��������
 *@param[in]   u8 year     
               u8 weekday  
               u8 month    
               u8 date     
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_rtc_set_date(u8 year, u8 weekday, u8 month, u8 date)
{

	return 0;
}

/**
 *@brief:      mcu_rtc_get_date
 *@details:    ��ȡ����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_rtc_get_date(void)
{

	
	return 0;
}
/**
 *@brief:      mcu_rtc_get_time
 *@details:    ��ȡʱ��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_rtc_get_time(void)
{

	return 0;
}
/*

	��ȡUTCʱ��

*/
time_t mcu_rtc_get_utc_time()
{
	time_t time;

	wjq_log(LOG_DEBUG, "%lu\n",time);

	return time;
}

struct tm* gmtime (const time_t *timep)
{
	return 0;
}
/*

	��UTCʱ��ת��Ϊ����ʱ��

*/
struct tm* localtime (const time_t *timep)
{
	return 0;
}

s32 mcu_rtc_test(void)
{
		return 0;
}

