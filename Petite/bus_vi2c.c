/**
 * @file            mcu_i2c.c
 * @brief           IOģ��I2C
 * @author          test
 * @date            2019��03��26�� ����һ
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
#include "bus_i2c.h"

#define MCU_I2C_TIMEOUT 250

/**
 *@brief:      mcu_i2c_delay
 *@details:    I2C�ź���ʱ����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_vi2c_delay(void)
{
    //Delay(1);//��ʱ��I2Cʱ��
    volatile u32 i = 1;

    for(;i>0;i--);
}

/**
 *@brief:      mcu_vi2c_sda_input
 *@details:    ��I2C sda IO����Ϊ����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
void mcu_vi2c_sda_input(DevI2c *dev)
{
	mcu_io_config_in(dev->sdaport, dev->sdapin);
}
/**
 *@brief:      mcu_i2c_sda_output
 *@details:       ��I2C sda IO����Ϊ���
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     
 */
void mcu_vi2c_sda_output(DevI2c *dev)
{
	mcu_io_config_out(dev->sdaport, dev->sdapin);
}
/**
 *@brief:      mcu_i2c_readsda
 *@details:    ��SDA����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static s32 mcu_vi2c_readsda(DevI2c *dev)
{

    if(MCU_IO_STA_1 == mcu_io_input_readbit(dev->sdaport, dev->sdapin))
        return MCU_IO_STA_1;
    else
        return MCU_IO_STA_0;
}
/**
 *@brief:      mcu_vi2c_sda
 *@details:       SDA����ߵ͵�ƽ
 *@param[in]  u8 sta  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_vi2c_sda(DevI2c *dev, u8 sta)
{

    if(sta == MCU_IO_STA_1)
    {
		mcu_io_output_setbit(dev->sdaport, dev->sdapin);
    }
    else if(sta == MCU_IO_STA_0)
    {
    	mcu_io_output_resetbit(dev->sdaport, dev->sdapin);
    }

}

/**
 *@brief:      mcu_i2c_scl
 *@details:    ʱ��SCL����ߵ͵�ƽ
 *@param[in]   u8 sta  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_vi2c_scl(DevI2c *dev, u8 sta)
{

	if(sta == MCU_IO_STA_1)
    {
		mcu_io_output_setbit(dev->sclport, dev->sclpin);
    }
    else if(sta == MCU_IO_STA_0)
    {
    	mcu_io_output_resetbit(dev->sclport, dev->sclpin);
    }
}
/**
 *@brief:      mcu_i2c_start
 *@details:    ����startʱ��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_vi2c_start(DevI2c *dev)
{
    mcu_vi2c_sda_output(dev);
    
    mcu_vi2c_sda(dev, MCU_IO_STA_1);  
    mcu_vi2c_scl(dev, MCU_IO_STA_1);

    mcu_vi2c_delay();
    mcu_vi2c_sda(dev, MCU_IO_STA_0);

    mcu_vi2c_delay();
    mcu_vi2c_scl(dev, MCU_IO_STA_0);
}
/**
 *@brief:      mcu_i2c_stop
 *@details:    ����I2C STOPʱ��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_vi2c_stop(DevI2c *dev)
{
    mcu_vi2c_sda_output(dev);

    mcu_vi2c_scl(dev, MCU_IO_STA_0);
    mcu_vi2c_sda(dev, MCU_IO_STA_0);   
    mcu_vi2c_delay();
    
    mcu_vi2c_scl(dev, MCU_IO_STA_1);
    mcu_vi2c_delay();
    
    mcu_vi2c_sda(dev, MCU_IO_STA_1);
    mcu_vi2c_delay();
}

/**
 *@brief:      mcu_i2c_wait_ack
 *@details:       �ȴ�ACK�ź�
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     static
 */
static s32 mcu_vi2c_wait_ack(DevI2c *dev)
{
    u8 time_out = 0;
    
    //sdaת����
    mcu_vi2c_sda_input(dev);
    mcu_vi2c_sda(dev, MCU_IO_STA_1);
    mcu_vi2c_delay();
    
    mcu_vi2c_scl(dev, MCU_IO_STA_1);
    mcu_vi2c_delay();
    
    while(1)
    {
        time_out++;
        if(time_out > MCU_I2C_TIMEOUT)
        {
            mcu_vi2c_stop(dev);
            //wjq_log(LOG_ERR, "i2c:wait ack time out!\r\n");
            return 1;
        }

        if(MCU_IO_STA_0 == mcu_vi2c_readsda(dev))
        {   
            break;
        }
    }
    
    mcu_vi2c_scl(dev, MCU_IO_STA_0);
    
    return 0;
}
/**
 *@brief:      mcu_i2c_ack
 *@details:       ����ACK�ź�
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_vi2c_ack(DevI2c *dev)
{
    mcu_vi2c_scl(dev, MCU_IO_STA_0);
    mcu_vi2c_sda_output(dev);
    
    mcu_vi2c_sda(dev, MCU_IO_STA_0);
    mcu_vi2c_delay();
    
    mcu_vi2c_scl(dev, MCU_IO_STA_1);
    mcu_vi2c_delay();
    
    mcu_vi2c_scl(dev, MCU_IO_STA_0);
}
/**
 *@brief:      mcu_i2c_writebyte
 *@details:       I2C����дһ���ֽ�����
 *@param[in]  u8 data  
 *@param[out]  ��
 *@retval:     static
 */
static s32 mcu_vi2c_writebyte(DevI2c *dev, u8 data)
{
    u8 i = 0;

    mcu_vi2c_sda_output(dev);

    mcu_vi2c_scl(dev, MCU_IO_STA_0);
    
    while(i<8)
    {
    
        if((0x80 & data) == 0x80)
        {
            mcu_vi2c_sda(dev, MCU_IO_STA_1);   
        }
        else
        {
            mcu_vi2c_sda(dev, MCU_IO_STA_0);
        }
        
        mcu_vi2c_delay();

        mcu_vi2c_scl(dev, MCU_IO_STA_1);
        mcu_vi2c_delay();
        
        mcu_vi2c_scl(dev, MCU_IO_STA_0);
        mcu_vi2c_delay();
        
        data = data <<1;
        i++;
    }
		return 0;
}
/**
 *@brief:      mcu_i2c_readbyte
 *@details:       I2C���߶�һ���ֽ�����
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     static
 */
static u8 mcu_vi2c_readbyte(DevI2c *dev)
{
    u8 i = 0;
    u8 data = 0;

    mcu_vi2c_sda_input(dev);
    
    while(i<8)
    {
        mcu_vi2c_scl(dev, MCU_IO_STA_0);
        mcu_vi2c_delay();
        
        mcu_vi2c_scl(dev, MCU_IO_STA_1);

        data = (data <<1);

        if(1 == mcu_vi2c_readsda(dev))
        {
            data =data|0x01;    
        }
        else
        {
            data = data & 0xfe;
        }

        mcu_vi2c_delay();
        
        i++;
    }

    return data;
}


s32 mcu_vi2c_init(DevI2c * dev)
{
	/*��ʼ��IO��״̬*/
	mcu_io_config_out(dev->sclport, dev->sclpin);
	mcu_io_config_out(dev->sdaport, dev->sdapin);

	mcu_io_output_setbit(dev->sdaport, dev->sdapin);
	mcu_io_output_setbit(dev->sclport, dev->sclpin); 
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
s32 mcu_vi2c_transfer(DevI2cNode *node, u8 addr, u8 rw, u8* data, s32 datalen)
{
    s32 i;
    u8 ch;
	DevI2c *dev;
	s32 res;
	
    #if 0//����IO���Ƿ���ͨ
    while(1)
    {
        uart_printf("test \r\n");
        mcu_i2c_scl(1);
        mcu_i2c_sda(1); 
        Delay(5);
        mcu_i2c_scl(0);
        mcu_i2c_sda(0); 
        Delay(5);
    }   
    #endif

	if(node == NULL)
		return -1;

	dev = &node->dev;
	
	//I2C_DEBUG(LOG_DEBUG, "i2c trf %s\r\n", dev->name);
    //������ʼ
    mcu_vi2c_start(dev);
    //���͵�ַ+��д��־
    //����ADDR
    if(rw == MCU_I2C_MODE_W)
    {
        addr = ((addr<<1)&0xfe);
        //uart_printf("write\r\n");
    }
    else
    {
        addr = ((addr<<1)|0x01);
        //uart_printf("read\r\n");
    }
    
    mcu_vi2c_writebyte(dev, addr);
	
    res = mcu_vi2c_wait_ack(dev);
	if(res == 1)
		return 1;
	
    i = 0;

    //���ݴ���
    if(rw == MCU_I2C_MODE_W)//д
    {
	    while(i < datalen)
	 	{
            ch = *(data+i);
            mcu_vi2c_writebyte(dev, ch);
            res = mcu_vi2c_wait_ack(dev);
			if(res == 1)
				return 1;
			
			i++;
	    }
    }
    else if(rw == MCU_I2C_MODE_R)//��
    {
       	while(i < datalen)
	 	{
            ch = mcu_vi2c_readbyte(dev);  
            mcu_vi2c_ack(dev);
            *(data+i) = ch;
			i++;
	    }
    }

    //���ͽ���
    mcu_vi2c_stop(dev);
    return 0;
}


