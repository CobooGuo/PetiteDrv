#ifndef _DEV_LCDBUS_H_
#define _DEV_LCDBUS_H_

#include "p_list.h"
#include "petite_def.h"


/*LCD ���߶��� */
typedef struct
{
	PetiteNode pnode;
	
	/*������������*/
	char basebus[DEV_NAME_SIZE];

	/*	3���ߣ�A0-�������ݣ�rst-��λ��bl-����
		I2C���ߵ�LCD����Ҫ��������    	*/

	MCU_PORT A0port;
	u16 A0pin;

	MCU_PORT rstport;
	u16 rstpin;

	MCU_PORT blport;
	u16 blpin;

	/* ����ֽ 91874 ��Ҫһ��busy״̬��*/
	MCU_PORT staport;
	u16 stapin;
}DevLcdBus;
/* �豸�ڵ㶨�� */
typedef struct
{
	
	DevLcdBus dev;

	s32 gd;	
	void *basenode;	
	
	struct list_head list;
}DevLcdBusNode;



extern s32 bus_lcd_bl(DevLcdBusNode *node, u8 sta);
extern s32 bus_lcd_rst(DevLcdBusNode *node, u8 sta);
extern DevLcdBusNode *bus_lcd_open(char *name);
extern s32 bus_lcd_close(DevLcdBusNode *node);
extern s32 bus_lcd_write_data(DevLcdBusNode *node, u8 *data, u32 len);
extern s32 bus_lcd_flush_data(DevLcdBusNode *node, u8 *data, u32 len);
extern s32 bus_lcd_flush_wait(DevLcdBusNode *node);
extern s32 bus_lcd_read_data(DevLcdBusNode *node, u8 *data, u32 len);
extern s32 bus_lcd_write_cmd(DevLcdBusNode *node, u8 cmd);
extern s32 dev_lcdbus_register(const DevLcdBus *dev);


#endif

