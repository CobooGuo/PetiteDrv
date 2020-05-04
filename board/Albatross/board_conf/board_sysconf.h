/**
 * @file                wujique_sysconf.h
 * @brief           ϵͳ����
 * @author          wujique
 * @date            2018��3��30�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��3��30�� ������
 *   ��    ��:         wujique
 *   �޸�����:   �����ļ�
*/
#ifndef __BOARD_SYSCONF__
#define __BOARD_SYSCONF__

#define PC_PORT  MCU_UART_4 

/* 
	���ļ�������豸�����
	�Ƕ�board_sysconf.c�Ĳ���0	
*/

/*-----------------------------------------------------------*/
/*	ѡ��������ⷽ��: ʹ�ô���IC ��     �ڲ�ADCת��*/
#define SYS_USE_TS_IC_CASE		1
//#define SYS_USE_TS_ADC_CASE 	0

/*��������ⷽ����xpt2046 ������ADC����ֻ��ѡһ��*/
#if defined(SYS_USE_TS_IC_CASE) && defined(SYS_USE_TS_ADC_CASE)
 #error "please just select one touch device!"
#endif

/*	����xpt 2046ʹ�� ģ��SPI1_ch1
	��Ƶ����Ϊ0      */
#define SYS_USE_VSPI1	1	
#define XPT2046_SPI "VSPI1_CH1"
#define XPT2046_SPI_KHZ	  (2000)

#if 0
/*	���ʹ��Ӳ��SPI3_CH4,
	��Ƶ��������̫��*/
//#define XPT2046_SPI "SPI3_CH4"
//#define XPT2046_SPI_PRE	SPI_BaudRatePrescaler_8
#endif
/*-----------------------------------------------------------*/
#define ALBATROSS_TASK_STK_SIZE (1024)
#define ALBATROSS_TASK_PRIO	1

#define START_TASK_STK_SIZE (512)
#define START_TASK_PRIO	3//


/*-----------------------------------------------------------*/

#define SYS_FS_NUM 0

#define VFS_SD_DIR	"mtd0"//sd���ļ�ϵͳ����vfs�е�Ŀ¼��
#define SYS_FS_FATFS_SD "1:/"//sd��Ŀ¼���� ������ָ�diso.c�е�����й�ϵ

#define VFS_USB_DIR	"mtd1"//USB
#define SYS_FS_FATFS_USB "2:/"
/*-----------------------------------------------------------*/

/*	�������Ͷ��� */
#define FONT_LIST_MAX 4	
extern struct fbcon_font_desc *FontList[FONT_LIST_MAX];


#endif


