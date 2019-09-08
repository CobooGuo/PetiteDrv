/**
 * @file            wujique_sysconf.c
 * @brief           ϵͳ����
 * @author          wujique
 * @date            2018��4��9�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��4��9�� ����һ
 *   ��    ��:         wujique
 *   �޸�����:   �����ļ�
*/
#include "mcu.h"
#include "board_sysconf.h"
#include "log.h"

#include "dev_spiflash.h"
#include "bus_lcd.h"
#include "bus_spi.h"
#include "bus_i2c.h"
#include "dev_lcd.h"


/*
	���ļ���������ϵͳ����Щ�豸
*/

/*
	���󰴼�Ӳ������
	row�������ǰ��
*/
KeyPadIO KeyPadIOList[KEY_PAD_ROW_NUM+KEY_PAD_COL_NUM]=
		{
			/*ROW*/
			{MCU_PORT_E, GPIO_Pin_0},
			{MCU_PORT_E, GPIO_Pin_1},
			{MCU_PORT_E, GPIO_Pin_2},
			{MCU_PORT_E, GPIO_Pin_3},
			/*COL*/
			{MCU_PORT_C, GPIO_Pin_3},
			{MCU_PORT_C, GPIO_Pin_2},
			{MCU_PORT_C, GPIO_Pin_1},
			{MCU_PORT_C, GPIO_Pin_0},
		};

/*---------------֮�£���SPI I2C LCD �ȳ���ıȽϺõ��豸�ͽӿڶ���----------------*/

/*-------------------------------
	I2C������
-------------------------------*/
/*
	IO��ģ���I2C1
*/
const DevI2c DevVi2c1={
		.name = "VI2C1",
		.type = DEV_I2C_V,
		
		.sclport = MCU_PORT_B,
		.sclpin = GPIO_Pin_6,

		.sdaport = MCU_PORT_B,
		.sdapin = GPIO_Pin_7,
		};
		
const DevI2c DevVi2c2={
		.name = "VI2C2",
		.type = DEV_I2C_V,
		
		.sclport = MCU_PORT_B,
		.sclpin = GPIO_Pin_8,

		.sdaport = MCU_PORT_B,
		.sdapin = GPIO_Pin_9,
		};

/*
	Ӳ��I2C������

	δ���ԣ�STM32��I2C��������ô��
*/
const DevI2c DevI2C1={
		.name = "I2C1",
		.type = DEV_I2C_H,
		
		.sclport = MCU_PORT_B,
		.sclpin = GPIO_Pin_6,

		.sdaport = MCU_PORT_B,
		.sdapin = GPIO_Pin_7,
		};	
/*----------------------
	IO��ģ��SPI������
------------------------*/
/*
	VSPI1, ���ϵ�LCD�ӿ��е�4��IOģ��SPI��
	����XPT2046�������������ɶ���д��
*/					
const DevSpi DevVSpi1IO={
		.name = "VSPI1",
		.type = DEV_SPI_V,
		
		/*clk*/
		.clkport = MCU_PORT_B,
		.clkpin = GPIO_Pin_0,
		/*mosi*/
		.mosiport = MCU_PORT_C,
		.mosipin = GPIO_Pin_5,
		/*miso*/
		.misoport = MCU_PORT_C,
		.misopin = GPIO_Pin_4,
	};

/* 
	�����ӿ��ϵ�Ӳ��SPI1���˴���IOģ��ʵ��
*/
const DevSpi DevVSpi2IO={
		.name = "VSPI2",
		.type = DEV_SPI_V,
		
		/*clk*/
		.clkport = MCU_PORT_A,
		.clkpin = GPIO_Pin_5,
		/*mosi*/
		.mosiport = MCU_PORT_A,
		.mosipin = GPIO_Pin_7,
		/*miso*/
		.misoport = MCU_PORT_A,
		.misopin = GPIO_Pin_6,
	};


/*
	Ӳ��SPI��������SPI3
	SPI������ʱ֧��SPI3��
	���������������������޸�mcu_spi.c�е�Ӳ��SPI��������ʼ������
*/
const DevSpi DevSpi3IO={
		.name = "SPI3",
		.type = DEV_SPI_H,
		
		/*clk*/
		.clkport = MCU_PORT_B,
		.clkpin = GPIO_Pin_3,
		
		/*mosi*/
		.mosiport = MCU_PORT_B,
		.mosipin = GPIO_Pin_5,

		/*miso*/
		.misoport = MCU_PORT_B,
		.misopin = GPIO_Pin_4,
	};

/*------------------------ 
	SPIͨ��
-------------------------*/
/*
	����FLASH��
*/
const DevSpiCh DevSpi3CH1={
		.name = "SPI3_CH1",
		.spi = "SPI3",
		
		.csport = MCU_PORT_C,
		.cspin = GPIO_Pin_7,
		
	};		
	

#if 0
/*
	ģ��SPIͨ������CS
	��������û��CS��LCD��Ļ��
	VSPI3����ʵ�������ӿ�SPI����������IOģ��ġ�
*/	
const DevSpiCh DevVSpi3CH1={
		.name = "VSPI3_CH1",
		.spi = "VSPI3",
		
		.csport = MCU_PORT_NULL,
		.cspin = NULL,
		
	};
#endif

/*
	ģ��SPIͨ��
*/	
const DevSpiCh DevVSpi2CH1={
		.name = "VSPI2_CH1",
		.spi = "VSPI2",
		
		.csport = MCU_PORT_A,
		.cspin = GPIO_Pin_4,
	};


/* 
	LCD�����еĴ������ӿ�, IOģ��SPI
*/
const DevSpiCh DevVSpi1CH1={
		.name = "VSPI1_CH1",
		.spi = "VSPI1",
		
		.csport = MCU_PORT_B,
		.cspin = GPIO_Pin_1,
		
	};
#if 0		
/* 
	SPI������������������ͬ�Ŀ�����
*/		
const DevSpiCh DevVSpi1CH2={
		.name = "VSPI1_CH2",
		.spi = "VSPI1",
		
		.csport = MCU_PORT_D,
		.cspin = GPIO_Pin_14,
	};
#endif

/*-------------------------------------
	LCDӲ���ӿ����߶���
	LCDӲ���ӿڰ���һ������ͨ�Žӿ�basebus��A0�ܽţ���λ�ܽţ�����ܽš�
---------------------------------------*/
#if 1
/*
	����LCD�ӿڣ�ʹ��������SPI����
	�����ӿ��е�SPI�ӿ�

	���ԣ� ��ʱ��ģ��SPIʵ��
*/
const DevLcdBus BusLcdSpi1={
	.name = "BusLcdSpi1",
	.type = LCD_BUS_SPI,
	.basebus = "VSPI2_CH1",

	.A0port = MCU_PORT_E,
	.A0pin = GPIO_Pin_4,

	.rstport = MCU_PORT_E,
	.rstpin = GPIO_Pin_5,

	.blport = MCU_PORT_A,
	.blpin = GPIO_Pin_9,

	.staport = MCU_PORT_NULL, 
	.stapin = NULL,
};
#else
/*
	������û��CS��MISO��1.33��LCD��
*/
const DevLcdBus BusLcdVSpi3={
	.name = "BusLcdVSpi3",
	.type = LCD_BUS_SPI,
	.basebus = "VSPI3_CH1",

	.A0port = MCU_PORT_G,
	.A0pin = GPIO_Pin_4,

	.rstport = MCU_PORT_G,
	.rstpin = GPIO_Pin_7,

	.blport = MCU_PORT_G,
	.blpin = GPIO_Pin_9,
};

#endif 
/*
	I2C�ӿڵ�LCD����
*/
const DevLcdBus BusLcdI2C1={
	.name = "BusLcdI2C1",
	.type = LCD_BUS_I2C,
	.basebus = "VI2C1",

	/*I2C�ӿڵ�LCD���ߣ�����Ҫ����IO*/
	.A0port = MCU_PORT_NULL,
	.A0pin = NULL,

	.rstport = MCU_PORT_NULL,
	.rstpin = NULL,
	
	.blport = MCU_PORT_NULL,
	.blpin = NULL,
};
/*
	8080�ӿڵ�LCD����
*/	
const DevLcdBus BusLcd8080={
	.name = "BusLcd8080",
	.type = LCD_BUS_8080,
	.basebus = "8080",//��ʹ���ã�8080����ֱ��Ƕ����LCD BUS������

	/* 8080 ����A0�ţ�����Ҳ�������AO */
	.A0port = MCU_PORT_NULL,
	.A0pin = NULL,

	.rstport = MCU_PORT_A,
	.rstpin = GPIO_Pin_15,
	
	.blport = MCU_PORT_B,
	.blpin = GPIO_Pin_15,

};


/*----------------------------
	FLASH�豸����
-----------------------------*/
const DevSpiFlash DevSpiFlashBoard={
	"core_spiflash",  
	"SPI3_CH1", 
	NULL
};

/*--------------------------------
	lcd�豸������
	ָ��ϵͳ�ж��ٸ�LCD�豸�������ĸ�LCD�����ϡ�
---------------------------------*/

/*I2C�ӿڵ� OLED*/

const DevLcd DevLcdOled1={
	.name = "i2coledlcd",  
	.buslcd = "BusLcdI2C1",  
	.id = 0X1315, 
	.width = 64, 
	.height = 128
	};

/*SPI�ӿڵ� OLED*/
/*
DevLcd DevLcdSpiOled	=	{
	.name = "spioledlcd", 	
	.buslcd = "BusLcdSpi3", 	
	.id = 0X1315, 
	.width = 64, 
	.height = 128};
*/
/*SPI�ӿڵ� COG LCD*/
const DevLcd DevLcdCOG1	=	{
	.name = "spicoglcd", 
	//.buslcd = "BusLcdVSpi2CH1", 
	.buslcd = "BusLcdSpi1",
	.id = 0X7565, 
	.width = 64, 
	.height = 128};

/*fsmc�ӿڵ� tft lcd*/
const DevLcd DevLcdtTFT	=	{"tftlcd", 		"BusLcd8080", 	NULL, 240, 320};
//const DevLcd DevLcdtTFT	=	{"tftlcd", 		"BusLcd8080", 	0x9325, 240, 320};
//const DevLcd DevLcdtTFT	=	{"tftlcd", 		"BusLcd8080", 	0x9341, 240, 320};
/*1408, 4.0���IPS��Ļ*/
//const DevLcd DevLcdtTFT	=	{"tftlcd", 		"BusLcd8080", 	0x1408, 480, 800};
/*SPI�ӿڵ� tft lcd*/
//const DevLcd DevLcdtTFT	=	{"spitftlcd", 		"BusLcdSpi3", 	0x9342, 240, 320};
//const DevLcd DevLcdtTFT	=	{"spitftlcd", 		"BusLcdVSpi1CH2", 	0x9342, 240, 320};
/*1.44�� �о�԰*/
//const DevLcd DevSpiLcdtTFT	=	{"spitftlcd",   "BusLcdSpi3", 	0x7735, 128, 128};

/* 1.3�磬IPS���о�԰��ֻ��SCL&SDA��SPI�ӿ�LCD*/
//const DevLcd DevLcdVSPITFT =	{"vspitftlcd",		"BusLcdVSpi3",	0x7789, 240, 240};

/* spi �ӿڵ���ɫīˮ�� */
//const DevLcd DevLcdSPIEPaper =	{"spiE-Paper",		"BusLcdSpi3",	0x9187, 176, 264};
/* spi �ӿ� �ڰ�īˮ�� 1.54�� GDEH154D27*/
//const DevLcd DevLcdSPIEPaper =	{"spiE-Paper",		"BusLcdSpi3",	0x3820, 200, 200};

/*
	ϵͳ�豸ע��
	ͨ���������ֲ㼶��������ϵ��
	�������Կ���ʵ�ֿɼ��ַ���ʾ���豸��
*/
s32 sys_dev_register(void)
{
	wjq_log(LOG_INFO, "[----------register---------------]\r\n");
	/*ע��I2C����*/
	mcu_i2c_register(&DevVi2c1);
			dev_lcdbus_register(&BusLcdI2C1);
					dev_lcd_register(&DevLcdOled1);
					
	//mcu_i2c_register(&DevVi2c2);
	
	mcu_spi_register(&DevSpi3IO);
			mcu_spich_register(&DevSpi3CH1);
					dev_spiflash_register(&DevSpiFlashBoard);

	mcu_spi_register(&DevVSpi2IO);
		mcu_spich_register(&DevVSpi2CH1);
			dev_lcdbus_register(&BusLcdSpi1);
					dev_lcd_register(&DevLcdCOG1);				
					
	mcu_spi_register(&DevVSpi1IO);
			mcu_spich_register(&DevVSpi1CH1);//8080�ӿڵĴ�����
	
	dev_lcdbus_register(&BusLcd8080);
			dev_lcd_register(&DevLcdtTFT);
			
	wjq_log(LOG_INFO, "[---------- register finish ---------]\r\n");
	return 0;
}

/*

Ӳ���������ñ��뷨������LINUX���豸��
ʵ��Ӧ���У��޸Ľӿ�IO�Ŀ�����Ӧ�ò���
�޸�����Ŀ����Խϴ�
������ݶ��LCD��������ʱ�򣬻����LCD��
�ܶ�LCD�޷��Զ�ʶ�����ԣ�����Ӳ�������ļ���á�

��ʽ��
[�㼶]����:����
	{
		����
	}

���磺	
{
	[0]cpu:stm32f407
		{}
		[1]VI2C:VI2C1
			{	
				
			}
			[2]LCDBUS:BusLcdI2C1
				{
					
				}
				[3]LCD:i2coledlcd
					{
						
					}

}


*/

/*

	�����ֿ�ֻʹ�ö�����ֵ����ֿ����������ɵĵ����ֿ⣬
	ʹ��ģʽ�����ɣ�Ҳ�����ݿ⡣

*/
struct fbcon_font_desc font_songti_16x16 = {
	"songti16",
	"mtd1/songti1616.DZK",
	16,
	16,
	32
};
	
struct fbcon_font_desc font_songti_12x12 = {
	"songti12",
	"mtd1/songti1212.DZK",
	12,
	12,
	24
};

struct fbcon_font_desc font_siyuan_16x16 = {
	"siyuan16",
	"mtd1/shscn1616.DZK",
	16,
	16,
	32
};
	
struct fbcon_font_desc font_siyuan_12x12 = {
	"siyuan12",
	"mtd1/shscn1212.DZK",
	12,
	12,
	24
};

struct fbcon_font_desc *FontList[FONT_LIST_MAX]=
	{
		&font_songti_16x16,
		&font_songti_12x12,
		&font_siyuan_16x16,
		&font_siyuan_12x12
	};



