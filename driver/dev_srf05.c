/**
 * @file            dev_srf05.c
 * @brief           ����������������
 * @author          wujique
 * @date            2019��03��23�� ���ڶ�
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:    
 *   ��    ��:      �ݼ�ȸ������
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
	���������ģ��HY-SRF05
	ԭ��
	1 VCC ��5V���磬ʵ��3.3VҲ�ܹ���
	2 Trig���������10us�ߵ�ƽ������������
	3 ģ���Զ����ͳ����������롣
	4 ������ɹ�������ECHO�ŷ���һ���ߵ�ƽ����ƽ����ʱ����ǳ��������䷵��ʱ��
		����  mm = 340*100*10/(1*1000*1000)/2*T
		����T��λus����ʽ�򻯣�Xmm = Tus*17/100

		������Լ������60ms��

	������ƣ�
	1 ������ʱʹ��Ӳ��ʱ��
	2 ���ռ�ʱ�ö�ʱ������
*/
#include "mcu.h"
#include "log.h"
#include "dev_srf05.h"


s32 Srf05Gd = -2;
/*
	���������ʹ��PF8 ���в���PF8�Ƕ�ʱ��13��ͨ��1 
*/

#define SRF05_TRIG_PIN MCU_IO_9
#define SRF05_TRIG_PORT MCU_PORT_B

#define SRF05_ECHO_PIN	MCU_IO_8
#define SRF05_ECHO_PORT	MCU_PORT_B

/**
 *@brief:      dev_srf05_trig
 *@details:    ����������10us���ϸߵ�ƽ
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
static s32 dev_srf05_trig(void)
{
	mcu_io_output_setbit(SRF05_TRIG_PORT, SRF05_TRIG_PIN);
	Delay(1);
	mcu_io_output_resetbit(SRF05_TRIG_PORT, SRF05_TRIG_PIN);
	return 0;
}


s32 dev_srf05_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	mcu_io_config_out(SRF05_TRIG_PORT, SRF05_TRIG_PIN);
	mcu_io_output_resetbit(SRF05_TRIG_PORT, SRF05_TRIG_PIN);

	mcu_io_config_timer(SRF05_ECHO_PORT, SRF05_ECHO_PIN, MCU_TIMER_5);
	
	Srf05Gd = -1;
	return 0;
}

s32 dev_srf05_open(void)
{
	if(Srf05Gd != -1)
		return -1;
	Srf05Gd = 0;
	return 0;
}

s32 dev_srf05_close(void)
{
	Srf05Gd = -1;
	return 0;
}
/**
 *@brief:      dev_touchkey_read
 *@details:    ���豸����ȡ�����¼�
 *@param[in]   u8 *buf    
               u32 count  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_srf05_read(u8 *buf, u32 count)
{
	u32 capr,capf;
	u32 len;
	
	if(Srf05Gd != 0)
		return -1;
	
	dev_srf05_trig();
	mcu_timer_cap_init(MCU_TIMER_4, MCU_TIMER_1US, MCU_TIMER_CH1, MCU_TIMER_CAP_RISING);
	mcu_timer_start(MCU_TIMER_4);
	wjq_log(LOG_DEBUG, "1 ");
	capr = mcu_timer_get_cap(MCU_TIMER_4, MCU_TIMER_CH1);
	wjq_log(LOG_DEBUG, "2 ");
	mcu_timer_cap_init(MCU_TIMER_4, MCU_TIMER_1US, MCU_TIMER_CH1, MCU_TIMER_CAP_FALLING);
	mcu_timer_start(MCU_TIMER_4);
	wjq_log(LOG_DEBUG, "3 ");
	capf = mcu_timer_get_cap(MCU_TIMER_4, MCU_TIMER_CH1);
	wjq_log(LOG_DEBUG, "4 ");
	len = capf*17/100;
	wjq_log(LOG_DEBUG, "\r\n%d-%d--len:%d-\r\n", capr, capf, len);
	
	return 0;

}

s32 dev_srf05_write(void)
{
	
	return 0;
}


s32 dev_srf05_test(void)
{
	dev_srf05_init();
	dev_srf05_open();
	
	while(1)
	{
		dev_srf05_read(0,0);
		wjq_log(LOG_DEBUG, "srf05 run\r\n");
		Delay(1000);
	}
}




