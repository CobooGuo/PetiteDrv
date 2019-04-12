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

	volatile u32 cnt = 0; 
	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
	/*����RTC�Ĵ�������Ҫʹ�ܱ�����*/
	PWR_BackupAccessCmd(ENABLE); 
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x55aa)	
	{
		wjq_log(LOG_DEBUG, " init rtc\r\n");
		BKP_DeInit();
		/*����LSEʱ��*/
		RCC_LSEConfig(RCC_LSE_ON);
		/*�ȴ�RCC LSEʱ�Ӿ���*/
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	
		{
			cnt++;
			if(cnt>0x2000000)
			{
				wjq_log(LOG_ERR, "lse not rdy\r\n");
				return -1;	
			}
		}		
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��	
	
    	RTC_EnterConfigMode();/// ��������	
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_SetCounter(1000);	//����RTC��������ֵ
		
		RTC_ExitConfigMode(); //�˳�����ģʽ	
	 
		BKP_WriteBackupRegister(BKP_DR1,0x55aa);	//����Ѿ���ʼ������
	} 

	wjq_log(LOG_INFO, " init rtc finish\r\n");	
	return 0;	
}

u8 RTC_Set(u32 sec)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(sec);	//����RTC��������ֵ
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
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

