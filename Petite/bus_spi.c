/**
 * @file            bus_spi.c
 * @brief           SPI���߹���
 * @author          test
 * @date            2019��03��26�� ������
 * @version         ����
 * @par             
 * @par History:
 * 1.��    ��:      
 *   ��    ��:      test
 *   �޸�����:      �����ļ�
		��Ȩ˵����
		1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱�������������ұ�����Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
		8 һ�н���Ȩ���ݼ�ȸ���������С�
*/
#include "mcu.h"
#include "log.h"
#include "p_list.h"
#include "board_sysconf.h"

#include "bus_spi.h"
#include "bus_vspi.h"

#define BUS_SPI_DEBUG

#ifdef BUS_SPI_DEBUG
#define BUSSPI_DEBUG	wjq_log 
#else
#define BUSSPI_DEBUG(a, ...)
#endif

/*


	����SPIͳһ����ӿ�


*/
struct list_head DevSpiRoot = {&DevSpiRoot, &DevSpiRoot};
/**
 *@brief:      mcu_spi_register
 *@details:    ע��SPI�������豸
 *@param[in]   DevSpi *dev      
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_spi_register(const DevSpi *dev)
{

	struct list_head *listp;
	DevSpiNode *p;
	
	wjq_log(LOG_INFO, "[register] spi :%s!\r\n", dev->name);

	/*
		��Ҫ��ѯ��ǰ����ֹ����
	*/
	listp = DevSpiRoot.next;
	while(1)
	{
		if(listp == &DevSpiRoot)
			break;

		p = list_entry(listp, DevSpiNode, list);

		if(strcmp(dev->name, p->dev.name) == 0)
		{
			wjq_log(LOG_INFO, "spi dev name err!\r\n");
			return -1;
		}
		
		listp = listp->next;
	}

	/* 
		����һ���ڵ�ռ� 
		
	*/
	p = (DevSpiNode *)wjq_malloc(sizeof(DevSpiNode));
	list_add(&(p->list), &DevSpiRoot);

	memcpy((u8 *)&p->dev, (u8 *)dev, sizeof(DevSpi));
	

	/*��ʼ��*/
	if(dev->type == DEV_SPI_V)
		mcu_vspi_init(dev);
	else if(dev->type == DEV_SPI_H)
		mcu_hspi_init(dev);
	
	p->gd = -1;
	
	return 0;
}

struct list_head DevSpiChRoot = {&DevSpiChRoot, &DevSpiChRoot};
/**
 *@brief:      mcu_spich_register
 *@details:    ע��SPIͨ��
 *@param[in]   DevSpiCh *dev     
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_spich_register(const DevSpiCh *dev)
{
	struct list_head *listp;
	DevSpiChNode *p;
	DevSpiNode *p_spi;
	
	wjq_log(LOG_INFO, "[register] spi ch :%s!\r\n", dev->name);

	/*
		��Ҫ��ѯ��ǰ����ֹ����
	*/
	listp = DevSpiChRoot.next;
	while(1)
	{
		if(listp == &DevSpiChRoot)
			break;

		p = list_entry(listp, DevSpiChNode, list);
		
		if(strcmp(dev->name, p->dev.name) == 0)
		{
			wjq_log(LOG_INFO, ">--------------------spi ch dev name err!\r\n");
			return -1;
		}
		
		listp = listp->next;
	}

	/* Ѱ��SPI������*/
	listp = DevSpiRoot.next;
	while(1)
	{
		if(listp == &DevSpiRoot)
		{
			wjq_log(LOG_INFO, ">---------------------spi ch reg err:no spi!\r\n");
			return -1;
		}


		p_spi = list_entry(listp, DevSpiNode, list);

		if(strcmp(dev->spi, p_spi->dev.name) == 0)
		{
			//wjq_log(LOG_INFO, "spi ch find spi!\r\n");
			break;
		}
		
		listp = listp->next;
	}
	/* 
		����һ���ڵ�ռ� 
		
	*/
	p = (DevSpiChNode *)wjq_malloc(sizeof(DevSpiChNode));
	list_add(&(p->list), &DevSpiChRoot);
	memcpy((u8 *)&p->dev, (u8 *)dev, sizeof(DevSpiCh));
	p->gd = -1;
	p->spi = p_spi;

	/* ��ʼ���ܽ� */
	mcu_io_config_out(dev->csport,dev->cspin);
	mcu_io_output_setbit(dev->csport,dev->cspin);

	return 0;
}


/**
 *@brief:      mcu_spi_open
 *@details:    ��SPIͨ��
 *@param[in]   DevSpiChNode * node
               u8 mode      ģʽ
               u16 pre      Ԥ��Ƶ
 *@param[out]  ��
 *@retval:     
 			   ��һ��SPI����F407�ϴ��Ҫ2us
 */
DevSpiChNode *mcu_spi_open(char *name, SPI_MODE mode, u16 pre)
{

	s32 res;
	DevSpiChNode *node;
	struct list_head *listp;
	
	//BUSSPI_DEBUG(LOG_INFO, "spi ch open:%s!\r\n", name);

	listp = DevSpiChRoot.next;
	node = NULL;
	
	while(1)
	{
		if(listp == &DevSpiChRoot)
			break;

		node = list_entry(listp, DevSpiChNode, list);
		//BUSSPI_DEBUG(LOG_INFO, "spi ch name%s!\r\n", node->dev.name);
		
		if(strcmp(name, node->dev.name) == 0)
		{
			//BUSSPI_DEBUG(LOG_INFO, "spi ch dev get ok!\r\n");
			break;
		}
		else
		{
			node = NULL;
		}
		
		listp = listp->next;
	}

	if(node != NULL)
	{
		
		if(node->gd == 0)
		{
			//BUSSPI_DEBUG(LOG_INFO, "spi ch open err:using!\r\n");
			node = NULL;
		}
		else
		{
			/*��SPI������*/
			if(node->spi->dev.type == DEV_SPI_H)
			{
				res = mcu_hspi_open(node->spi, mode, pre);	
			}
			else if(node->spi->dev.type == DEV_SPI_V)
			{
				res = mcu_vspi_open(node->spi, mode, pre);	
			}

			if(res == 0)
			{
				node->gd = 0;
				//BUSSPI_DEBUG(LOG_INFO, "spi dev open ok: %s!\r\n", name);
				mcu_io_output_resetbit(node->dev.csport, node->dev.cspin);
			}
			else
			{
				//BUSSPI_DEBUG(LOG_INFO, "spi dev open err!\r\n");
				node = NULL;
			}
		}
	}
	else
	{
		BUSSPI_DEBUG(LOG_INFO, ">-------spi ch no exist!\r\n");	
	}
	
	return node;
}


/**
 *@brief:      mcu_spi_close
 *@details:    �ر�SPI ͨ��
 *@param[in]   DevSpiChNode * node  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_spi_close(DevSpiChNode * node)
{
	if(node->spi->dev.type == DEV_SPI_H)
	{
		mcu_hspi_close(node->spi);
	}
	else
		mcu_vspi_close(node->spi);
	
	/*����CS*/
	mcu_io_output_setbit(node->dev.csport, node->dev.cspin);
	node->gd = -1;
 
	return 0;
}
/**
 *@brief:      mcu_spi_transfer
 *@details:    SPI ����
 *@param[in]   DevSpiChNode * node
 			   u8 *snd  
               u8 *rsv  
               s32 len  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_spi_transfer(DevSpiChNode * node, u8 *snd, u8 *rsv, s32 len)
{
	if(node == NULL)
		return -1;

	if(node->spi->dev.type == DEV_SPI_H)
		return mcu_hspi_transfer(node->spi, snd, rsv, len);
	else if(node->spi->dev.type == DEV_SPI_V)	
		return mcu_vspi_transfer(node->spi, snd, rsv, len);
	else
	{
		BUSSPI_DEBUG(LOG_DEBUG, "spi dev type err\r\n");	
	}
	return -1;
}
/**
 *@brief:      mcu_spi_cs
 *@details:    �ٿض�ӦSPI��CS
 *@param[in]   DevSpiChNode * node  
               u8 sta   1 �ߵ�ƽ��0 �͵�ƽ     
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_spi_cs(DevSpiChNode * node, u8 sta)
{
	switch(sta)
	{
		case 1:
			mcu_io_output_setbit(node->dev.csport, node->dev.cspin);
			break;
			
		case 0:
			mcu_io_output_resetbit(node->dev.csport, node->dev.cspin);
			break;
			
		default:
			return -1;

	}

	return 0;
}

#if 0

void spi_example(void)
{
	DevSpiChNode *spichnode;
	u8 src[16];
	u8 rsv[16];
	
	/*��SPIͨ��*/
	spichnode = mcu_spi_open("VSPI1_CH1", SPI_MODE_1, 4);
	if(spichnode == NULL)
	{
		while(1);
	}
	/*��10������*/
	mcu_spi_transfer(spichnode, NULL, rsv, 10);
	/*д10������*/
	mcu_spi_transfer(spichnode, src, NULL, 10);
	/*��д10������*/
	mcu_spi_transfer(spichnode, src, rsv, 10);

	mcu_spi_close(spichnode);
}

#endif

