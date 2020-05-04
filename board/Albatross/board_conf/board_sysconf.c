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
#include "drv_spiflash.h"
#include "bus_lcd.h"
#include "bus_spi.h"
#include "bus_i2c.h"
#include "drv_lcd.h"


const DevI2c DevVi2c1={
		.pnode={
				.name = "VI2C1",
				.type = BUS_I2C_V,
				},
		
		.sclport = MCU_PORT_B,
		.sclpin = MCU_IO_5,

		.sdaport = MCU_PORT_B,
		.sdapin = MCU_IO_4,
		};

/*
	I2C�ӿڵ�LCD����
*/
const DevLcdBus BusLcdI2C1={
	.pnode={
				.name = "BusLcdI2C1",
				.type = BUS_LCD_I2C,
			},
			
	.basebus = "VI2C1",

	/*I2C�ӿڵ�LCD���ߣ�����Ҫ����IO*/
	.A0port = MCU_PORT_NULL,
	.A0pin = NULL,

	.rstport = MCU_PORT_NULL,
	.rstpin = NULL,

	.blport = MCU_PORT_NULL,
	.blpin = NULL,

	.staport = MCU_PORT_NULL, 
	.stapin = NULL,
};
/*I2C�ӿڵ� OLED*/
const DevLcd DevLcdOled1={
	.pnode={
				.name = "i2coledlcd",
				.type = DEV_LCD,
		},
		
	.buslcd = "BusLcdI2C1",  
	.id = 0X1315, 
	.width = 64, 
	.height = 128,
};

/*
	ϵͳ�豸ע��
	ͨ���������ֲ㼶��������ϵ��
	�������Կ���ʵ�ֿɼ��ַ���ʾ���豸��
*/
s32 petite_dev_register(void)
{
	wjq_log(LOG_DEBUG, "[register] petite_dev_register!\r\n");

	/*ע��I2C����*/
	bus_i2c_register(&DevVi2c1);
			dev_lcdbus_register(&BusLcdI2C1);
					dev_lcd_register(&DevLcdOled1);
	return 0;
}


/*

	�����ֿ�ֻʹ�ö�����ֵ����ֿ����������ɵĵ����ֿ⣬
	ʹ��ģʽ�����ɣ�Ҳ�����ݿ⡣

*/
struct fbcon_font_desc font_songti_16x16 = {
	"songti16",
	"mtd0/1:/songti1616.DZK",
	16,
	16,
	32
};
	
struct fbcon_font_desc font_songti_12x12 = {
	"songti12",
	"mtd0/1:/songti1212.DZK",
	12,
	12,
	24
};

struct fbcon_font_desc font_siyuan_16x16 = {
	"siyuan16",
	"mtd0/1:/shscn1616.DZK",
	16,
	16,
	32
};
	
struct fbcon_font_desc font_siyuan_12x12 = {
	"siyuan12",
	"mtd0/1:/shscn1212.DZK",
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



