#ifndef __BUS_SPI_H_
#define __BUS_SPI_H_

#include "p_list.h"
#include "petie_def.h"

typedef enum{
	DEV_SPI_H = 1,//Ӳ��SPI������
	DEV_SPI_V = 2,//IOģ��SPI
}DEV_SPI_TYPE;

/*
	SPI �����㣬
	1����SPI��������������CS
	2����SPIͨ�����ɿ�����+CS���
	
*/
/*

	SPI �豸����

*/
typedef struct
{
	/*�豸����*/
	char name[DEV_NAME_SIZE];
	
	/*�豸���ͣ�IOģ�� or Ӳ��������*/
	DEV_SPI_TYPE type;
	
	MCU_PORT clkport;
	u16 clkpin;

	MCU_PORT mosiport;
	u16 mosipin;

	MCU_PORT misoport;
	u16 misopin;

}DevSpi;

/*

	SPI�������豸�ڵ�
	
*/
typedef struct
{
	/*���������Ϊ-1����Ϊ0��spi�����������ظ���*/
	s32 gd;
	/*������Ӳ����Ϣ����ʼ��������ʱ�����豸������Ϣ����*/
	DevSpi dev;	
	
	/*ģ��SPI��ʱ�ӷ�Ƶ����*/
	u16 clk;
	/*����*/
	struct list_head list;
}DevSpiNode;

/*
	SPI ͨ������
	һ��SPIͨ������һ��SPI������+һ��CS�������

*/
typedef struct
{
	/*ͨ�����ƣ��൱���豸����*/
	char name[DEV_NAME_SIZE];
	/*SPI����������*/
	char spi[DEV_NAME_SIZE];

	/*cs��*/
	MCU_PORT csport;
	u16 cspin;
}DevSpiCh;

/*SPIͨ���ڵ�*/
typedef struct
{
	/**/
	s32 gd;
	
	DevSpiCh dev;
	
	DevSpiNode *spi;//�������ڵ�ָ��
	
	struct list_head list;
}DevSpiChNode;


extern s32 bus_spi_register(const DevSpi *dev);
extern s32 bus_spich_register(const DevSpiCh *dev);

extern DevSpiChNode *bus_spich_open(char *name, SPI_MODE mode, u16 pre);
extern s32 bus_spich_close(DevSpiChNode * node);
extern s32 bus_spich_transfer(DevSpiChNode * node, u8 *snd, u8 *rsv, s32 len);
extern s32 bus_spich_cs(DevSpiChNode * node, u8 sta);


#endif

