/**
 * @file            dev_keypad.c
 * @brief           ���󰴼�ɨ��
 * @author          wujique
 * @date            2018��3��10�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��3��10�� ������
 *   ��    ��:         �ݼ�ȸ������
 *   �޸�����:   �����ļ�

		��Ȩ˵����
		1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱�������������ұ�����Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
*/
#include "mcu.h"
#include "log.h"
#include "board_sysconf.h"
#include "dev_keypad.h"


#ifdef SYS_USE_KEYPAD

//#define DEV_KEYPAD_DEBUG

#ifdef DEV_KEYPAD_DEBUG
#define KEYPAD_DEBUG	wjq_log 
#else
#define KEYPAD_DEBUG(a, ...)
#endif

/*

	ԭ��
	ɨ�����ֻ�ṩ��λ�����ṩ����
	����������APP�����飬�����̰���������Ҳ��APP�����顣
	4*4���󰴼�����λ���壬���Ͻ�Ϊ1�������ң����ϵ��¡�

	ɨ�跽����
	���������0������IO��״̬��

	��ֵ�ο����̣�ɨ�����䵽����������ǰ���λ��ͨ����Ϣ��

*/
/*�ް�������ʱ����IO״̬��ͬʱҲ��ʶ��״̬��Щλ��Ч*/
#define KEYPAD_INIT_STA_MASK (0X0F)

/*
����ʱ��=��������*scan����ִ�м��*KEY_PAD_ROW_NUM
��ͬ�����������ʱ�䣺
�𶯰�����10ms
���У�40ms-50ms
��е����������100ms
*/
#define KEY_PAD_DEC_TIME (3)//����ɨ�����
/*
	ɨ�����ʹ�õı���
*/
struct _strKeyPadCtrl
{
	u8 dec;//��������
	u8 oldsta;//��һ�ε��ȶ�״̬
	u8 newsta;
};
struct _strKeyPadCtrl KeyPadCtrl[KEY_PAD_ROW_NUM];


/*
	�������壬ɨ�赽�İ���λ�ü�ֵ��ֻ˵��λ�õ�ͨ�ϣ�
	���λΪ0��ʶͨ�����£���Ϊ1�����ʶ�Ͽ���
*/
#define KEYPAD_BUFF_SIZE (32)
static u8 KeyPadBuffW = 0;//д����
static u8 KeyPadBuffR = 0;//������
static u8 KeyPadBuff[KEYPAD_BUFF_SIZE];

static s32 DevKeypadGd = -2;//�豸����

/**
 *@brief:      dev_keypad_init
 *@details:    ��ʼ�����󰴼�IO��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_keypad_init(void)
{
	u8 i;

	/* r */
	i = 0;
	while(1)
	{
		mcu_io_config_out(KeyPadIOList[i].port, KeyPadIOList[i].pin);
		mcu_io_output_setbit(KeyPadIOList[i].port, KeyPadIOList[i].pin);
		i++;
		if(i >= KEY_PAD_ROW_NUM)
			break;
	}

	/* c */
	i = 0;
	while(1)
	{
		mcu_io_config_in(KeyPadIOList[i+KEY_PAD_ROW_NUM].port, KeyPadIOList[i+KEY_PAD_ROW_NUM].pin);
		i++;
		if(i >= KEY_PAD_COL_NUM)
			break;
	}

	
	for(i = 0; i< KEY_PAD_ROW_NUM; i++)
	{
		KeyPadCtrl[i].dec = 0;
		KeyPadCtrl[i].oldsta = KEYPAD_INIT_STA_MASK;
		KeyPadCtrl[i].newsta = KEYPAD_INIT_STA_MASK;
	}
	
	DevKeypadGd = -1;
	
	return 0;
}

/**
 *@brief:      dev_keypad_open
 *@details:    �򿪰�����ֻ�д򿪰�����ɨ��Ż����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_keypad_open(void)
{
	if(DevKeypadGd == -1)
	{
		DevKeypadGd	 = 0;
	}
	else
		return -1;
	
	return 0;
}
/**
 *@brief:      dev_keypad_read
 *@details:    ����ֵ
 *@param[in]   u8 *key  
               u8 len   ��������ֵ����   
 *@param[out]  ��
 *@retval:     ���ض�����ֵ��
 */
s32 dev_keypad_read(u8 *key, u8 len)
{
	u8 i =0;
	
	if(DevKeypadGd != 0)
		return -1;

	while(1)
	{
		if(KeyPadBuffR != KeyPadBuffW)
		{
			*(key+i) = KeyPadBuff[KeyPadBuffR];
			KeyPadBuffR++;
			if(KeyPadBuffR >= KEYPAD_BUFF_SIZE)
				KeyPadBuffR = 0;
		}
		else
			break;

		i++;
		if(i>= len)
			break;
	}

	return i;
}

/**
 *@brief:      dev_keypad_scan
 *@details:    ����ɨ�裬�ڶ�ʱ�����������ж�ʱִ��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_keypad_scan(void)
{
	u16 ColSta;
	u8 chgbit;
	static u8 scanrow = 0;
	u8 keyvalue;
	u8 i;
	
	if(DevKeypadGd != 0)
		return -1;
	
	/*�������״̬�������������IO����ƴ������IO*/
	#if 0
	ColSta = mcu_io_input_readport(KeyPadIOList[0+KEY_PAD_ROW_NUM].port);
	ColSta = (ColSta>>8)&KEYPAD_INIT_STA_MASK;
	#else
	i = 0;
	ColSta = 0x00;
	while(1)
	{
		ColSta = ColSta<<1;
		
		keyvalue = mcu_io_input_readbit(KeyPadIOList[i+KEY_PAD_ROW_NUM].port, KeyPadIOList[i+KEY_PAD_ROW_NUM].pin);
		if(keyvalue == 1)
			ColSta++;
	
		i++;
		if(i >= KEY_PAD_COL_NUM)
			break;
	}
	#endif

	/*��¼��״̬����״̬�����������ȶ����������¼���*/
	if(ColSta != KeyPadCtrl[scanrow].newsta)
	{
		KeyPadCtrl[scanrow].newsta = ColSta;
		KeyPadCtrl[scanrow].dec = 0;
	}

	/*����״̬���״̬�б仯������ɨ���ж�*/
	if(ColSta != KeyPadCtrl[scanrow].oldsta)
	{
		KEYPAD_DEBUG(LOG_DEBUG, " chg--");
		KeyPadCtrl[scanrow].dec++;
		if(KeyPadCtrl[scanrow].dec >= KEY_PAD_DEC_TIME)//���ڷ�������
		{
			/*ȷ���б仯*/
			KeyPadCtrl[scanrow].dec = 0;
			/*�¾ɶԱȣ��ҳ��仯λ*/
			chgbit = KeyPadCtrl[scanrow].oldsta^KeyPadCtrl[scanrow].newsta;
			KEYPAD_DEBUG(LOG_DEBUG, "row:%d, chage bit:%02x\r\n",scanrow,chgbit);

			/*���ݱ仯��λ������仯�İ���λ��*/
			u8 i;
			for(i=0;i<KEY_PAD_COL_NUM;i++)
			{
				if((chgbit & (0x01<<i))!=0)
				{
					keyvalue = 	scanrow*KEY_PAD_COL_NUM+i;
					/*���ͨ�ϣ������ɿ�����־*/
					if((KeyPadCtrl[scanrow].newsta&(0x01<<i)) == 0)
					{
						KEYPAD_DEBUG(LOG_DEBUG, "press\r\n");
					}
					else
					{
						KEYPAD_DEBUG(LOG_DEBUG, "rel\r\n");
						keyvalue += KEYPAD_PR_MASK;
					}
					/**/
					KeyPadBuff[KeyPadBuffW] =keyvalue+1;//+1��������1��ʼ������0��ʼ
					KeyPadBuffW++;
					if(KeyPadBuffW>=KEYPAD_BUFF_SIZE)
						KeyPadBuffW = 0;
				}
			}
			
			KeyPadCtrl[scanrow].oldsta = KeyPadCtrl[scanrow].newsta;
			
		}
	}

	/*����һ�е�IO���0*/
	scanrow++;
	if(scanrow >= KEY_PAD_ROW_NUM)
		scanrow = 0;

	i = 0;
	while(1)
	{
		mcu_io_output_setbit(KeyPadIOList[i].port, KeyPadIOList[i].pin);
		i++;
		if(i >= KEY_PAD_ROW_NUM)
			break;
	}
	mcu_io_output_resetbit(KeyPadIOList[scanrow].port, KeyPadIOList[scanrow].pin);

	return 0;
}
/**
 *@brief:      dev_keypad_test
 *@details:    ����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_keypad_test(void)
{
	u8 key;
	s32 res;
	res = dev_keypad_read(&key, 1);
	if(res == 1)
	{
		wjq_log(LOG_FUN, "get a key:%02x\r\n", key);
	}
	return 0;
}

#endif

