/**
 * @file            mcu_timer.c
 * @brief           CPUƬ�϶�ʱ������
 * @author          test
 * @date            2017��10��25�� ������
 * @version         ����
 * @par             
 * @par History:
 * 1.��    ��:      2017��10��25�� ������
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
#include "mcu_timer.h"

/*

	�Զ�ʱ�����г��󣬳���Ľӿ��ṩ���ϲ�����ʹ�á�
	1. ��ʱ������ʱ����Ӳ��ʱ����
	2. Ӳ��ʱ����3�ֹ��ܣ�PWN�����񣬶�ʱ��
	3. ��ʱ���ṩ�ӿ�

*/

#define MCU_TIME_DEBUG

#ifdef MCU_TIME_DEBUG
#define TIME_DEBUG	wjq_log 
#else
#define TIME_DEBUG(a, ...)
#endif

typedef struct _strTimerCtrl
{
	void (*Callback)(void);	
	u8 retype;
}McuTimerCtrl;

McuTimerCtrl McuTimerCtrlList[MCU_TIMER_MAX];
/*STM32��ز�������*/
const TIM_TypeDef *STM32TimerList[MCU_TIMER_MAX]={
	0,
		TIM1,TIM2,TIM3,TIM4,
		TIM5,TIM6,TIM7,TIM8

	};
const uint32_t STM32TimerRccList[MCU_TIMER_MAX]={
	0,
		1,2,3,4,
		5,6,7,8
	};
/*
	����������Կ�����23457��5����ʱ���ж����û�и��ã�
	�򵥵Ķ�ʱ�����������4��
*/	
const uint8_t STM32TimerIRQList[MCU_TIMER_MAX]={
	0,
		0, 1, 2, 3,
		4, 5, 6, 7
	};
/*
    ��ʱ��ʱ��Ϊ84M,
    Tout=((SYSTEM_CLK_PERIOD)*(SYSTEM_CLK_PRESCALER))/Ft us.

	Ԥ��Ƶ,8400��ʱ�ӲŴ���һ�ζ�ʱ������ 
	��ôһ����ʱ��������ʱ�����(1/84M)*8400 = 100us	  
*/	
const u16  STM32TimerTickSet[MCU_TIMER_DEF_MAX] =
	{
		0,
		84,//1us
		840,//840Ԥ��Ƶ��һ��tick=10us
		8400,//8400Ԥ��Ƶ��һ��tick=100us
		42000,//Ԥ��Ƶ��һ��tick=500us
		8,//Լ����100ns��ͨ�����ڲ�����������Ƕ�ʱ
	}; 
/**
 *@brief:	   mcu_tim3_IRQhandler
 *@details:    ��ʱ���жϴ�����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:	   
 */
void mcu_tim3_IRQhandler(void)
{

}

/**
 *@brief:	   mcu_tim5_IRQhandler
 *@details:    ��ʱ���жϴ�����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:	   
 */
void mcu_tim5_IRQhandler(void)
{
}

/**
 *@brief:	   mcu_tim7_IRQhandler
 *@details:    ��ʱ���жϴ�����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:	   
 */
void mcu_tim7_IRQhandler(void)
{

}

/**
 *@brief:	   
 *@details:    
 *@param[in]	 
 *@param[out]  
 *@retval:	   
 */
s32 mcu_timer_init(McuTimerNum timer)
{

	return 0;
}  
/**
 *@brief:      
 *@details:    
 *@param[in]     
 *@param[out]  
 *@retval:     
 */

s32 mcu_timer_config(McuTimerNum timer, McuTimerTickDef tickdef, u32 tick, void (*callback)(void), u8 type)
{

	
	return 0;
}

s32 mcu_timer_start(McuTimerNum timer)
{
	
	return 0;
}

/**
 *@brief:      
 *@details:    
 *@param[in]     
 *@param[out]  
 *@retval:     
 */
s32 mcu_timer_stop(McuTimerNum timer)
{

	return 0;
}

/**
 *@brief:      mcu_timer_pwm_init
 *@details:    
 *@param[in]   
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_timer_pwm_init(McuTimerNum timer, McuTimerTickDef tickdef, u32 tick, u8 duty, McuTimerCh ch)
{



	return 0;
}
/*
	��ʱ������
*/
/**
 *@brief:      mcu_timer_cap_init
 *@details:    ��ʼ����ʱ�����񣬲�ʹ���ж�
 *@param[in]     
               
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_timer_cap_init(McuTimerNum timer, McuTimerTickDef tickdef, McuTimerCh ch, McuTimerCapEdge edge)
{


	return 0;

}
/**
 *@brief:      mcu_timer_get_cap
 *@details:    ��ѯ��ȡ��ʱȥ����ֵ
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     ����ֵ����ʱ�򷵻����ֵ	
 */
u32 mcu_timer_get_cap(McuTimerNum timer, McuTimerCh ch)
{ 


	return 0;
}

/*


*/
/**
 *@brief:	   mcu_tim5_test
 *@details:    ��ʱ������
 *@param[in]   void  
 *@param[out]  ��
 *@retval:	   
 */
void mcu_tim5_test(void)
{
	static u8 i = 0;

	i++;
	if(1 == i)
	{
		TIME_DEBUG(LOG_DEBUG, "tim int 1\r\n");    
	}
	else if(2 == i)
	{
		TIME_DEBUG(LOG_DEBUG, "tim int 2\r\n");
	}
	else if(3 == i)
	{
		TIME_DEBUG(LOG_DEBUG, "tim int 3\r\n");
	}
	else
	{
		TIME_DEBUG(LOG_DEBUG, "tim int 4\r\n");
		i = 0;	 
	}
}
void mcu_tim7_test(void)
{

	TIME_DEBUG(LOG_DEBUG, "tim7\r\n");    

}

s32 mcu_timer_test(void)
{
	mcu_timer_init(MCU_TIMER_5);
	mcu_timer_config(MCU_TIMER_5, MCU_TIMER_500US, 2000, mcu_tim5_test, MCU_TIMER_RE);

	//mcu_timer_init(MCU_TIMER_7);
	//mcu_timer_config(MCU_TIMER_7, MCU_TIMER_10US, 50000, mcu_tim7_test, MCU_TIMER_RE);
	
	mcu_timer_start(MCU_TIMER_5);
	//mcu_timer_start(MCU_TIMER_7);
	
	while(1)
	{
		
	}
}


