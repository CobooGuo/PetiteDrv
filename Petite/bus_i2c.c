/**
 * @file            mcu_i2c.c
 * @brief           IOģ��I2C
 * @author          test
 * @date            2019��03��30�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:      
 *   ��    ��:      �ݼ�ȸ������
 *   �޸�����:      �����ļ�
    	1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱���������������뱣��WUJIQUE��Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
		8 һ�н���Ȩ���ݼ�ȸ���������С�
*/

#include "mcu.h"
#include "log.h"
#include "p_list.h"
#include "bus_i2c.h"
#include "bus_vi2c.h"
#include "alloc.h"

/*	I2Cģ��ά��һ��������¼�е�ǰ��ʼ����I2C������*/
struct list_head DevI2cGdRoot = {&DevI2cGdRoot, &DevI2cGdRoot};

/**
 *@brief:      bus_i2c_register
 *@details:    ��ʼ��I2C�ӿڣ� �൱��ע��һ��I2C�豸
 *@param[in]   const DevI2c * dev I2C�豸��Ϣ
 *@param[out]  ��
 *@retval:     
 */
s32 bus_i2c_register(const DevI2c * dev)
{
    
	struct list_head *listp;
	DevI2cNode *p;
	
	wjq_log(LOG_INFO, "[register] i2c:%s!\r\n", dev->pnode.name);

	/*	��Ҫ��ѯ��ǰI2C����������ֹ����	*/
	listp = DevI2cGdRoot.next;
	while(1) {
		if (listp == &DevI2cGdRoot) break;

		p = list_entry(listp, DevI2cNode, list);
		//wjq_log(LOG_INFO, "i2c dev name:%s!\r\n", p->dev.name);
		
		if (strcmp(dev->pnode.name, p->dev.pnode.name) == 0) {
			wjq_log(LOG_INFO, "i2c dev name err!\r\n");
			return -1;
		}
		
		listp = listp->next;
	}

	/* ����һ���ڵ�ռ�   ����������     	*/
	p = (DevI2cNode *) wjq_malloc(sizeof(DevI2cNode));
	list_add(&(p->list), &DevI2cGdRoot);
	
	/*	��ʼ���豸�ڵ� 	*/
	memcpy((u8 *)&p->dev, (u8 *)dev, sizeof(DevI2c));
	p->gd = -1;

	if (dev->pnode.type == BUS_I2C_V)
		bus_vi2c_init(&p->dev);
	else if (dev->pnode.type == BUS_I2C_H)
		mcu_hi2c_init(&p->dev);
	
	return 0;
}
/**
 *@brief:      bus_i2c_open
 *@details:    �������ִ�һ��i2c�ӿ�
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     �����豸�ڵ�
 */
DevI2cNode *bus_i2c_open(char *name)
{

	DevI2cNode *node;
	struct list_head *listp;
	
	//I2C_DEBUG(LOG_INFO, "i2c open:%s!\r\n", name);

	listp = DevI2cGdRoot.next;
	node = NULL;
	
	while(1) {
		if (listp == &DevI2cGdRoot) break;

		node = list_entry(listp, DevI2cNode, list);
		//I2C_DEBUG(LOG_INFO, "i2c dev name:%s!\r\n", node->dev.name);
		 
		if (strcmp(name, node->dev.pnode.name) == 0) {
			//I2C_DEBUG(LOG_INFO, "i2c dev open ok!\r\n");
			break;
		}
		
		listp = listp->next;
	}

	if (node != NULL) {
		if (node->gd == 0) {
			node = NULL;
		} else {
			node->gd = 0;
		}
	}
	return node;
}
/**
 *@brief:      mcu_i2c_close
 *@details:    �ر�I2C�ڵ�
 *@param[in]   DevI2cNode *node 
 *@param[out]  ��
 *@retval:     -1 �ر�ʧ�ܣ�0 �رճɹ�
 */
s32 bus_i2c_close(DevI2cNode *node)
{
	if  (node == NULL) return -1;

	if (node->gd != 0) return -1;

	node->gd = -1; 

	return 0;
}

/**
 *@brief:      mcu_i2c_transfer
 *@details:    �м������¿�ʼλ�Ĵ�������
 *@param[in]   DevI2cNode * node  I2C�ڵ�
 			   u8 addr   7λ��ַ
               u8 rw    0 д��1 ��    
               u8* data  
               s32 datalen �������ݳ���
 *@param[out]  ��
 *@retval:     
 */
s32 bus_i2c_transfer(DevI2cNode *node, u8 addr, u8 rw, u8* data, s32 datalen)
{
	if (node == NULL) return -1;

	if(node->dev.pnode.type == BUS_I2C_H)
		return mcu_hi2c_transfer (node, addr, rw, data, datalen);
	else if (node->dev.pnode.type == BUS_I2C_V)	
		return bus_vi2c_transfer(node, addr, rw, data, datalen);
	else {
		wjq_log(LOG_DEBUG, "i2c dev type err\r\n");	
	}	
	return -1;
}


#if 0

void mcu_i2c_example(void)
{
	DevI2cNode *node;
	
	node = mcu_i2c_open("VI2C1");
	if(node == NULL)
	{
		wjq_log(LOG_DEBUG, "open VI2C1 err!\r\n");
		while(1);
	}
	
	u8 data[16];
	mcu_i2c_transfer(node, 0x70, MCU_I2C_MODE_W, data, 8);
	mcu_i2c_transfer(node, 0x70, MCU_I2C_MODE_R, data, 8);

	mcu_i2c_close(node);
}

#endif

