/**
 * @file            dev_buzzer.c
 * @brief           PWM����������
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
#include "petite_config.h"

#include "log.h"
#include "mcu_timer.h"
#include "mcu_io.h"
/**
 *@brief:      dev_buzzer_init
 *@details:    ��ʼ��������
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_buzzer_init(void)
{
	wjq_log(LOG_INFO, "init buzzer\r\n");
	/*
		�û����ֲ����֪���ܽźͶ�ʱ��ͨ���Ĺ�ϵ
	*/
	mcu_io_config_timer(MCU_PORT_D, MCU_IO_13, MCU_TIMER_4);
	mcu_timer_pwm_init(MCU_TIMER_4, MCU_TIMER_1US, 250, 50, MCU_TIMER_CH2);
	return 0;
}
/**
 *@brief:      dev_buzzer_open
 *@details:    �򿪷�����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_buzzer_open(void)
{
	wjq_log(LOG_INFO, "open buzzer\r\n");
	
	mcu_io_config_timer(MCU_PORT_D, MCU_IO_13, MCU_TIMER_4);
    //---ʹ�� TIM4 
	mcu_timer_start(MCU_TIMER_4);
	
	return 0;
}
/**
 *@brief:      dev_buzzer_close
 *@details:    �رշ�����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_buzzer_close(void)
{
	wjq_log(LOG_INFO, "close buzzer\r\n");
    mcu_timer_stop(MCU_TIMER_4); //---�رն�ʱ�� TIM4 


	/*�رշ�����ʱ��Ҫ��IO��Ϊ��ͨIO����������͵�ƽ���������������ɴ����*/
	mcu_io_config_out(MCU_PORT_D, MCU_IO_13);
	mcu_io_output_resetbit(MCU_PORT_D, MCU_IO_13);
	
	return 0;
}
/**
 *@brief:      dev_buzzer_test
 *@details:    ���Է�����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
void dev_buzzer_test(void)
{
    
}

