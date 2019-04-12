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
		RCC_APB2Periph_TIM1,RCC_APB1Periph_TIM2,RCC_APB1Periph_TIM3,RCC_APB1Periph_TIM4,
		RCC_APB1Periph_TIM5,RCC_APB1Periph_TIM6,RCC_APB1Periph_TIM7,RCC_APB2Periph_TIM8
	};
/*
	����������Կ�����23457��5����ʱ���ж����û�и��ã�
	�򵥵Ķ�ʱ�����������4��
*/	
const uint8_t STM32TimerIRQList[MCU_TIMER_MAX]={
	0,
		TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn,
		TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn
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
	if(TIM_GetITStatus(TIM3, TIM_FLAG_Update) == SET)
	{										
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);

		if(McuTimerCtrlList[MCU_TIMER_3].retype == 1)
			TIM_Cmd(TIM3, DISABLE);//ֹͣ��ʱ��
		if(McuTimerCtrlList[MCU_TIMER_3].Callback != NULL)	
			McuTimerCtrlList[MCU_TIMER_3].Callback();

	}
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
	if(TIM_GetITStatus(TIM5, TIM_FLAG_Update) == SET)
	{										
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);

		if(McuTimerCtrlList[MCU_TIMER_5].retype == 1)
			TIM_Cmd(TIM5, DISABLE);//ֹͣ��ʱ��
		if(McuTimerCtrlList[MCU_TIMER_5].Callback != NULL)	
			McuTimerCtrlList[MCU_TIMER_5].Callback();
 
	}
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
	if(TIM_GetITStatus(TIM7, TIM_FLAG_Update) == SET)
	{										
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		
		if(McuTimerCtrlList[MCU_TIMER_7].retype == 1)
			TIM_Cmd(TIM7, DISABLE);//ֹͣ��ʱ��
		if(McuTimerCtrlList[MCU_TIMER_7].Callback != NULL)	
			McuTimerCtrlList[MCU_TIMER_7].Callback();
		
	}
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
    NVIC_InitTypeDef NVIC_InitStructure;
    

	if(timer == MCU_TIMER_NULL
			|| timer >= MCU_TIMER_MAX)
		return -1;
	
    //�򿪶�ʱ��ʱ��
    if(timer == MCU_TIMER_1 || timer == MCU_TIMER_8)
		RCC_APB2PeriphClockCmd(STM32TimerRccList[timer], ENABLE);	
	else
    	RCC_APB1PeriphClockCmd(STM32TimerRccList[timer], ENABLE);
	
    //��λ��ʱ��
    TIM_Cmd((TIM_TypeDef *)STM32TimerList[timer], DISABLE);
	
    TIM_SetCounter((TIM_TypeDef *)STM32TimerList[timer], 0);
    
	/*�ж����ȼ��ڵײ�̶�*/
	NVIC_InitStructure.NVIC_IRQChannel = STM32TimerIRQList[timer];	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_ITConfig((TIM_TypeDef *)STM32TimerList[timer], TIM_IT_Update, ENABLE);//�򿪶�ʱ���ж�
    
	McuTimerCtrlList[timer].Callback = NULL;
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
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	McuTimerCtrlList[timer].Callback = callback;
	McuTimerCtrlList[timer].retype = type;
	
	//��λ��ʱ��
    TIM_Cmd((TIM_TypeDef *)STM32TimerList[timer], DISABLE);
    TIM_SetCounter((TIM_TypeDef *)STM32TimerList[timer], 0);

	TIM_TimeBaseInitStruct.TIM_Prescaler = STM32TimerTickSet[tickdef]-1;//��Ƶ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseInitStruct.TIM_Period = tick - 1;  //����
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 1;
    TIM_TimeBaseInit((TIM_TypeDef *)STM32TimerList[timer], &TIM_TimeBaseInitStruct);

	TIM_ClearFlag((TIM_TypeDef *)STM32TimerList[timer], TIM_FLAG_Update);
	
	TIM_ITConfig((TIM_TypeDef *)STM32TimerList[timer], TIM_IT_Update, ENABLE);//�򿪶�ʱ���ж�

	TIM_Cmd((TIM_TypeDef *)STM32TimerList[timer], ENABLE);//ʹ�ܶ�ʱ��(����)	
	
	return 0;
}

s32 mcu_timer_start(McuTimerNum timer)
{
	TIM_Cmd((TIM_TypeDef *)STM32TimerList[timer], ENABLE);
	
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
	TIM_Cmd((TIM_TypeDef *)STM32TimerList[timer], DISABLE);
	
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
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    if(timer == MCU_TIMER_1 || timer == MCU_TIMER_8)
		RCC_APB2PeriphClockCmd(STM32TimerRccList[timer], ENABLE);	
	else
    	RCC_APB1PeriphClockCmd(STM32TimerRccList[timer], ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = STM32TimerTickSet[tickdef]-1;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //---���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period= tick - 1; //---�Զ���װ��ֵ
    TIM_TimeBaseInit((TIM_TypeDef *)STM32TimerList[timer], &TIM_TimeBaseStructure);//---��ʼ����ʱ�� 4
    
    //----��ʼ�� TIM4 PWM ģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //---PWM ����ģʽ 1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //---�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //---������Ե�

	/*Ĭ�����õ���ͨ��2*/
	if(ch == MCU_TIMER_CH1)
	{
    	TIM_OC1Init((TIM_TypeDef *)STM32TimerList[timer], &TIM_OCInitStructure);
    	TIM_SetCompare1((TIM_TypeDef *)STM32TimerList[timer], tick*duty/100);
    	TIM_OC1PreloadConfig((TIM_TypeDef *)STM32TimerList[timer], TIM_OCPreload_Enable); 
	}
	else if(ch == MCU_TIMER_CH2)
	{
    	TIM_OC2Init((TIM_TypeDef *)STM32TimerList[timer], &TIM_OCInitStructure);
    	TIM_SetCompare2((TIM_TypeDef *)STM32TimerList[timer], tick*duty/100);
    	TIM_OC2PreloadConfig((TIM_TypeDef *)STM32TimerList[timer], TIM_OCPreload_Enable); 
	}
	else if(ch == MCU_TIMER_CH3)
	{
    	TIM_OC3Init((TIM_TypeDef *)STM32TimerList[timer], &TIM_OCInitStructure);
    	TIM_SetCompare3((TIM_TypeDef *)STM32TimerList[timer], tick*duty/100);
    	TIM_OC3PreloadConfig((TIM_TypeDef *)STM32TimerList[timer], TIM_OCPreload_Enable); 
	}
	else if(ch == MCU_TIMER_CH4)
	{
    	TIM_OC4Init((TIM_TypeDef *)STM32TimerList[timer], &TIM_OCInitStructure);
    	TIM_SetCompare4((TIM_TypeDef *)STM32TimerList[timer], tick*duty/100);
    	TIM_OC4PreloadConfig((TIM_TypeDef *)STM32TimerList[timer], TIM_OCPreload_Enable); 
	}
	else
		return -1;
		
    TIM_ARRPreloadConfig((TIM_TypeDef *)STM32TimerList[timer], ENABLE);

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
	u16 timerch;
	u16 timitcc;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM2_ICInitStructure;

	if(timer == MCU_TIMER_6 || timer == MCU_TIMER_7)
		return -1;

	if(timer == MCU_TIMER_1 || timer == MCU_TIMER_8)
		RCC_APB2PeriphClockCmd(STM32TimerRccList[timer], ENABLE);	
	else
    	RCC_APB1PeriphClockCmd(STM32TimerRccList[timer], ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler =STM32TimerTickSet[tickdef]; //Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
	TIM_TimeBaseInit((TIM_TypeDef *)STM32TimerList[timer], &TIM_TimeBaseStructure); // ��ʼ����ʱ�� 2

	if(ch == MCU_TIMER_CH1)
	{
		timerch = TIM_Channel_1;
		timitcc = TIM_IT_CC1;
	}
	else if(ch == MCU_TIMER_CH2)
	{
		timerch = TIM_Channel_2;
		timitcc = TIM_IT_CC2;
	}
	else if(ch == MCU_TIMER_CH3)
	{
 		timerch = TIM_Channel_3;
		timitcc = TIM_IT_CC3;
	}
	else if(ch == MCU_TIMER_CH4)
	{
		timerch = TIM_Channel_4;
		timitcc = TIM_IT_CC4;
	}
	else
		return -1;

	//��ʼ��ͨ�� 4
	TIM2_ICInitStructure.TIM_Channel = timerch; //ѡ������� IC4 ӳ�䵽 TIM2


	if(edge == MCU_TIMER_CAP_RISING)
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	else if(edge == MCU_TIMER_CAP_FALLING)
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; 
	else if(edge == MCU_TIMER_CAP_BOTHEDGE)
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge; 
		
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���������Ƶ,����Ƶ
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
	TIM_ICInit((TIM_TypeDef *)STM32TimerList[timer], &TIM2_ICInitStructure);//��ʼ�� TIM2 IC4

	TIM_ClearITPendingBit((TIM_TypeDef *)STM32TimerList[timer], timitcc|TIM_IT_Update); //����жϱ�־
	TIM_SetCounter((TIM_TypeDef *)STM32TimerList[timer], 0); 

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
	u16 timitcc;
	u32 value;

	if(timer == MCU_TIMER_6 || timer == MCU_TIMER_7)
		return 0;
	
	if(ch == MCU_TIMER_CH1)
	{
		timitcc = TIM_IT_CC1;
	}
	else if(ch == MCU_TIMER_CH2)
	{
		timitcc = TIM_IT_CC2;
	}
	else if(ch == MCU_TIMER_CH3)
	{
		timitcc = TIM_IT_CC3;
	}
	else if(ch == MCU_TIMER_CH4)
	{
		timitcc = TIM_IT_CC4;
	}
	else
		return 0;


	while(TIM_GetFlagStatus((TIM_TypeDef *)STM32TimerList[timer], timitcc) == RESET)
	{
		if(TIM_GetCounter((TIM_TypeDef *)STM32TimerList[timer]) > 0xffffffff-1000)
			return TIM_GetCounter((TIM_TypeDef *)STM32TimerList[timer]);//��ʱ��,ֱ�ӷ��� CNT ��ֵ
	}
	
	if(ch == MCU_TIMER_CH1)
	{
		value = TIM_GetCapture1((TIM_TypeDef *)STM32TimerList[timer]);
	}
	else if(ch == MCU_TIMER_CH2)
	{
		value = TIM_GetCapture2((TIM_TypeDef *)STM32TimerList[timer]);
	}
	else if(ch == MCU_TIMER_CH3)
	{
		value = TIM_GetCapture3((TIM_TypeDef *)STM32TimerList[timer]);
	}
	else if(ch == MCU_TIMER_CH4)
	{
		value = TIM_GetCapture4((TIM_TypeDef *)STM32TimerList[timer]);
	}
	return value;
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


