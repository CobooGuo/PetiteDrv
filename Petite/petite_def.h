#ifndef __PETITE_DEF__
#define __PETITE_DEF__


/* ����PetiteDrv */

/* �豸������ */
#define DEV_NAME_SIZE	16

/* 	�ú������ЩLCD�����μӱ���*/
#define TFT_LCD_DRIVER_COG12864
#define TFT_LCD_DRIVER_SSD1615
#define TFT_LCD_DRIVER_9341
#define TFT_LCD_DRIVER_9341_8BIT
#define TFT_LCD_DRIVER_9325
#define TFT_LCD_DRIVER_7735
#define TFT_LCD_DRIVER_7789
#define TFT_LCD_DRIVER_91874
#define TFT_LCD_DRIVER_3820

/*�豸���Ͷ���*/
typedef enum{
	DEV_NULL = 0,
	BUS_LCD_SPI,
	BUS_LCD_I2C,
	BUS_LCD_8080,
	BUS_LCD_MAX,

	BUS_I2C_H = 0x11,//Ӳ��SPI������
	BUS_I2C_V = 0x12,//IOģ��SPI

	BUS_SPI_H = 0x21,//Ӳ��SPI������
	BUS_SPI_V = 0x22,//IOģ��SPI
	BUS_SPI_CH = 0x23,//spi ͨ��

	DEV_SPIFLASH = 0x31,
	DEV_LCD = 0x32,
}PetiteDevType;

typedef struct
{
	const char name[DEV_NAME_SIZE];
	PetiteDevType type;
}PetiteNode;


#endif
