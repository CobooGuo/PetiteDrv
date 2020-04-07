#ifndef __BUS_I2C_H_
#define __BUS_I2C_H_

#include "p_list.h"
#include "petie_def.h"

typedef enum{
	DEV_I2C_H = 1,//Ӳ��SPI������
	DEV_I2C_V = 2,//IOģ��SPI
}DEV_I2C_TYPE;

/*
	i2c�豸����
*/

typedef struct
{
	/*�豸����*/
	char name[DEV_NAME_SIZE];

	/*�豸���ͣ�IOģ�� or Ӳ��������*/
	DEV_I2C_TYPE type;

	/*�豸��Ҫ����Դ��ģ��I2Cֻ��Ҫ����IO��*/
	MCU_PORT sclport;
	u16 sclpin;

	MCU_PORT sdaport;
	u16 sdapin;
}DevI2c;

/*
	

*/
typedef struct
{
	s32 gd;
	DevI2c dev;	
	struct list_head list;
}DevI2cNode;


#define MCU_I2C_MODE_W 0
#define MCU_I2C_MODE_R 1

extern s32 bus_i2c_register(const DevI2c * dev);
extern DevI2cNode *bus_i2c_open(char *name);
extern s32 bus_i2c_close(DevI2cNode *node);
extern s32 bus_i2c_transfer(DevI2cNode *node, u8 addr, u8 rw, u8* data, s32 datalen);

#endif

