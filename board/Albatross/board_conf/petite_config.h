#ifndef __PETITE_CONFIG_H__
#define __PETITE_CONFIG_H__

/* ����PetiteDrv */

/* �豸������ */
#define DEV_NAME_SIZE	16

/* ����Ķ�������Ƿ�����Ӧ������ */

/* �м������ */
#define PANEL_SOUND_MODULE	0
#define PANEL_SOUND_I2S		0
#define PANEL_SOUND_DAC		0
#define PANEL_EMENU_MODULE	1
#define DRV_SDIO_MODULE		0
#define PANEL_FONT_MODULE	0//asc�ַ��Ǳ��䣬��������Ƿ��ú��ֵ���

/* dev */
#define DRV_RS485_MODULE 		0
#define DRV_KEYPAD_MODULE 		0
#define DRV_SPI_FLASH_MODULE 	0
#define DRV_WM8978_MODULE   	0
#define DRV_XPT2046_MODULE		1
#define DRV_HTU21U_MODULE		0
#define DRV_HCOC_MODULE			0

/* ����ͷ���� */
#define DRV_CAMERA_MODULE		0
#define DRV_CAMERA_OV2640 		0
#define DRV_CAMERA_OV9655	 	0

/* 	�ú������ЩLCD�����μӱ���*/
#define LCD_DRIVER_ST7565		1
#define LCD_DRIVER_SSD1615		1
#define LCD_DRIVER_9341			1
#define LCD_DRIVER_9341_8BIT	0
#define LCD_DRIVER_9325			1
#define LCD_DRIVER_7735			0
#define LCD_DRIVER_7789			0
#define LCD_DRIVER_91874		0
#define LCD_DRIVER_3820			0
#define LCD_DRIVER_NT35510		1
#define LCD_DRIVER_R61408		1

/* �Ƿ�ʹ��RTOS */
#define SYS_USE_RTOS 1

/* �ļ�ϵͳ */
#define SYS_USE_VFS		0
#define SYS_FS_FATFS	0
#define SYS_FS_LITTLEFS	0
#define SYS_FS_SPIFS	0

/*����alloc.c�ڴ��������*/
#define AllocArraySize (80*1024)


#endif

