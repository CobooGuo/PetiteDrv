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
#ifndef __WUJIQUE_SYSCONF__
#define __WUJIQUE_SYSCONF__

/*���������Ϣ������ں�*/
#define PC_PORT  MCU_UART_3 

/* 	�ú������ЩLCD�����μӱ���*/
#define LCD_DRIVER_ST7565		1
#define LCD_DRIVER_SSD1615		1
#define LCD_DRIVER_9341			1
//#define LCD_DRIVER_9341_8BIT	1
#define LCD_DRIVER_9325			1
//#define LCD_DRIVER_7735			1
//#define LCD_DRIVER_7789			1
//#define LCD_DRIVER_91874		1
//#define LCD_DRIVER_3820			1
#define LCD_DRIVER_NT35510		1


/*
	ѡ��������ⷽ��
*/
#define SYS_USE_TS_IC_CASE		1
//#define SYS_USE_TS_ADC_CASE 	1

#ifdef SYS_USE_TS_IC_CASE
/*
	����xpt 2046ʹ�� ģ��SPI1_ch1
	��Ƶ����Ϊ0
*/
#define XPT2046_SPI "VSPI1_CH1"
#define XPT2046_SPI_PRE	0
/*
	���ʹ��Ӳ��SPI3_CH4,
	��Ƶ��������̫��
*/
//#define XPT2046_SPI "SPI3_CH4"
//#define XPT2046_SPI_PRE	SPI_BaudRatePrescaler_8

#endif

/*xpt2046��������VSPI1*/
#ifdef SYS_USE_TS_IC_CASE
	#define SYS_USE_VSPI1	1
#endif

/*
	RS485�����������豸���á�
*/
//#define SYS_USE_RS485	1
//#define SYS_USE_EXUART	1
#define SYS_USE_USB	1
#define SYS_USE_CAMERA  1

/* 
	�������ӿڽ��й��ܶ���  
*/

/*
	VSPI2 VI2C2 KEYPAD ,3��ֻ����һ��
*/
//#define SYS_USE_VSPI2 1
//#define SYS_USE_VI2C2	1
#define SYS_USE_KEYPAD	1
#ifdef SYS_USE_KEYPAD
#define KEY_PAD_COL_NUM (4)//4��
#define KEY_PAD_ROW_NUM (4)//4��	
#endif

#define BOARD_DEV_HTU21U	1
#ifdef  BOARD_DEV_HTU21U
#define DEV_HTU21D_I2CBUS "VI2C2"
#endif

#define BOARD_DEV_HCOC	1
#ifdef	BOARD_DEV_HCOC
#define DEV_PTHCHO_UART MCU_UART_1
#endif

#if (defined(SYS_USE_EXUART) && defined(SYS_USE_RS485))
 #error "please not define SYS_USE_EXUART & SYS_USE_RS485 sametime!(in wujique_sysconf.h file)"
#endif

#if (defined(SYS_USE_EXUART) && defined(SYS_USE_USB))
 #error "please not define SYS_USE_EXUART & SYS_USE_USB sametime!(in wujique_sysconf.h file)"
#endif

#if (defined(SYS_USE_EXUART) && defined(SYS_USE_CAMERA))
 #error "please not define SYS_USE_EXUART & SYS_USE_CAMERA sametime!(in wujique_sysconf.h file)"
#endif

#if (defined(SYS_USE_RS485) && defined(SYS_USE_USB))
 #error "please not define SYS_USE_RS485 & SYS_USE_USB sametime!(in wujique_sysconf.h file)"
#endif
#if (defined(SYS_USE_RS485) && defined(SYS_USE_CAMERA))
 #error "please not define SYS_USE_RS485 & SYS_USE_CAMERA sametime!(in wujique_sysconf.h file)"
#endif

/*
	����SPI2��ʹ������IO�������󰴼���ͻ����I2C2Ҳ����
*/
#if (defined(SYS_USE_VSPI2) && defined(SYS_USE_KEYPAD))
 #error "please not define SYS_USE_VSPI2 & SYS_USE_KEYPAD sametime!(in wujique_sysconf.h file)"
#endif
#if (defined(SYS_USE_VI2C2) && defined(SYS_USE_KEYPAD))
 #error "please not define SYS_USE_VI2C2 & SYS_USE_KEYPAD sametime!(in wujique_sysconf.h file)"
#endif
#if (defined(SYS_USE_VSPI2) && defined(SYS_USE_VI2C2))
 #error "please not define SYS_USE_VSPI2 & SYS_USE_VI2C2 sametime!(in wujique_sysconf.h file)"
#endif


/*��������ⷽ����xpt2046 ������ADC����ֻ��ѡһ��*/
#if defined(SYS_USE_TS_IC_CASE) && defined(SYS_USE_TS_ADC_CASE)
 #error "please just select one touch device!(in wujique_sysconf.h file)"
#endif

/* �Ƿ�ʹ��RTOS */
#define SYS_USE_RTOS 1

#ifdef SYS_USE_RTOS
#define Wujique407_TASK_STK_SIZE (1024)
#define Wujique407_TASK_PRIO	1

#define START_TASK_STK_SIZE (512)
#define START_TASK_PRIO	3//

#define USB_TASK_STK_SIZE (1024)
#define USB_TASK_PRIO	2
#endif

/*����alloc.c�ڴ��������*/

#define AllocArraySize (80*1024)


/*  ���� �����ļ�ϵͳ */
#define SYS_FS_NUM 5
#define SYS_FS_FATFS 1//ʹ��FATFS
#define VFS_SD_DIR	"mtd0"//sd���ļ�ϵͳ����vfs�е�Ŀ¼��
#define SYS_FS_FATFS_SD "1:/"//sd��Ŀ¼���� ������ָ�diso.c�е�����й�ϵ
#define VFS_USB_DIR	"mtd1"//USB
#define SYS_FS_FATFS_USB "2:/"

/*	�������Ͷ��� */
#define FONT_LIST_MAX 4	
extern struct fbcon_font_desc *FontList[FONT_LIST_MAX];


/* �ж����ȼ�ͳһ����          */
#define NVIC_PRE(x) NVIC_PRE_PRI_##x
#define NVIC_SUB(x) NVIC_SUB_PRI_##x
/*
#define NVIC_PRE_PRI_
#define NVIC_SUB_PRI_
*/
/*
	USB �ж�
	
*/
#define NVIC_PRE_PRI_OTG_FS_IRQn	1
#define NVIC_SUB_PRI_OTG_FS_IRQn	1

#if 0
#define NVIC_PRE_PRI_OTG_HS_EP1_OUT_IRQn	1
#define NVIC_SUB_PRI_OTG_HS_EP1_OUT_IRQn	2
#define NVIC_PRE_PRI_OTG_HS_EP1_IN_IRQn 1
#define NVIC_SUB_PRI_OTG_HS_EP1_IN_IRQn 1
#endif
/*
	SDIO_IRQn 0 0
*/
#define NVIC_PRE_PRI_SDIO_IRQn	0
#define NVIC_SUB_PRI_SDIO_IRQn	0
/*
SD_SDIO_DMA_IRQn 0 1
*/
#define NVIC_PRE_PRI_SD_SDIO_DMA_IRQn	1
#define NVIC_SUB_PRI_SD_SDIO_DMA_IRQn	0


#define NVIC_PRE_PRI_USART1_IRQn	3
#define NVIC_SUB_PRI_USART1_IRQn	3

#define NVIC_PRE_PRI_USART2_IRQn	3
#define NVIC_SUB_PRI_USART2_IRQn	3

#define NVIC_PRE_PRI_USART3_IRQn	3
#define NVIC_SUB_PRI_USART3_IRQn	3

#define NVIC_PRE_PRI_TIM2_IRQn		1
#define NVIC_SUB_PRI_TIM2_IRQn  	1

#define NVIC_PRE_PRI_ADC_IRQn	1
#define NVIC_SUB_PRI_ADC_IRQn	1

#define NVIC_PRE_PRI_CAN1_RX0_IRQn	2
#define NVIC_SUB_PRI_CAN1_RX0_IRQn	0
/*
i2s ����
DMA1_Stream4_IRQn 0 0
*/
#define NVIC_PRE_PRI_DMA1_Stream4_IRQn	0
#define NVIC_SUB_PRI_DMA1_Stream4_IRQn 	0

/*
i2s ¼��
DMA1_Stream3_IRQn	0 0
*/
#define NVIC_PRE_PRI_DMA1_Stream3_IRQn	0
#define NVIC_SUB_PRI_DMA1_Stream3_IRQn	0

/*
���׵δ�ʱ��
TIM5_IRQn 2 2
*/
#define NVIC_PRE_PRI_TIM5_IRQn	2
#define NVIC_SUB_PRI_TIM5_IRQn	2

/*
	dac sound
	TIM3_IRQn 0 0
*/
#define NVIC_PRE_PRI_TIM3_IRQn	0
#define NVIC_SUB_PRI_TIM3_IRQn	0

/* 
adc ��ʽ���������̿���
TIM7_IRQn 1 1
*/
#define NVIC_PRE_PRI_TIM7_IRQn	1
#define NVIC_SUB_PRI_TIM7_IRQn	1

/*
���ڼ��
EXTI15_10_IRQn 1 1
*/
#define NVIC_PRE_PRI_EXTI15_10_IRQn	1
#define NVIC_SUB_PRI_EXTI15_10_IRQn	1

#endif


