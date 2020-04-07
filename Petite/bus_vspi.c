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
#include "mcu_io.h"
#include "log.h"
#include "board_sysconf.h"
#include "bus_spi.h"
#include "mcu_spi.h"
#include "bus_vspi.h"

#define BUS_VSPI_DEBUG

#ifdef BUS_VSPI_DEBUG
#define VSPI_DEBUG	wjq_log 
#else
#define VSPI_DEBUG(a, ...)
#endif


/**
 *@brief:      mcu_vspi_init
 *@details:    ��ʼ������SPI
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 bus_vspi_init(const DevSpi *dev)
{

	wjq_log(LOG_DEBUG, "vspi init:%s\r\n", dev->name);

	mcu_io_config_out(dev->clkport, dev->clkpin);
	mcu_io_output_setbit(dev->clkport,dev->clkpin);

	mcu_io_config_out(dev->mosiport, dev->mosipin);
	mcu_io_output_setbit(dev->mosiport, dev->mosipin);


	mcu_io_config_in(dev->misoport, dev->misopin);
	mcu_io_output_setbit(dev->misoport, dev->misopin);
	
	return 0;
}


/**
 *@brief:      vspi_delay
 *@details:    ����SPIʱ����ʱ
 *@param[in]   u32 delay  
 *@param[out]  ��
 *@retval:     
 */
void vspi_delay(u32 delay)
{
	volatile u32 i=delay;

	while(i>0){
		i--;	
	}

}

u32 VspiDelay = 0;

/**
 *@brief:      mcu_vspi_open
 *@details:    ������SPI
 *@param[in]   SPI_DEV dev    
               SPI_MODE mode  
               u16 pre        
 *@param[out]  ��
 *@retval:     
 */
s32 bus_vspi_open(DevSpiNode *node, SPI_MODE mode, u16 pre)
{

	if(node == NULL)return -1;
	
	if(node->gd != -1){
		//VSPI_DEBUG(LOG_DEBUG, "vspi dev busy\r\n");
		return -1;
	}
	
	//VSPI_DEBUG(LOG_DEBUG, "vo-");
	
	node->clk = pre;
	node->gd = 0;
		
    return 0;
}
/**
 *@brief:      mcu_vspi_close
 *@details:    �ر�����SPI
 *@param[in]   SPI_DEV dev  
 *@param[out]  ��
 *@retval:     
 */
s32 bus_vspi_close(DevSpiNode *node)
{
	if(node->gd != 0)return -1;
	//VSPI_DEBUG(LOG_DEBUG, "vc-");
	node->gd = -1;
	
    return 0;
}
/**
 *@brief:      mcu_vspi_transfer
 *@details:       ����SPIͨ��
 *@param[in]   SPI_DEV dev  
               u8 *snd      
               u8 *rsv      
               s32 len      
 *@param[out]  ��
 *@retval:     

 		node->clk = 0, CLKʱ��1.5M 2018.06.02
 */
s32 bus_vspi_transfer(DevSpiNode *node, u8 *snd, u8 *rsv, s32 len)
{
	u8 i;
	u8 data;
	s32 slen;
	u8 misosta;

	volatile u16 delay;
	
	DevSpi *dev;
	
	if(node == NULL){
		VSPI_DEBUG(LOG_DEBUG, "vspi dev err\r\n");
		return -1;
	}

	if(node->gd != 0){
		VSPI_DEBUG(LOG_DEBUG, "vspi dev no open\r\n");
		return -1;
	}
	
    if( ((snd == NULL) && (rsv == NULL)) || (len < 0) ){
        return -1;
    }

	dev = &(node->dev);

	slen = 0;

	while(1){
		if(slen >= len)	break;

		if(snd == NULL)
			data = 0xff;
		else
			data = *(snd+slen);
		
		for(i=0; i<8; i++){
			mcu_io_output_resetbit(dev->clkport, dev->clkpin);

			delay = node->clk;
			while(delay>0){
				delay--;	
			}
			
			if(data&0x80)
				mcu_io_output_setbit(dev->mosiport, dev->mosipin);
			else
				mcu_io_output_resetbit(dev->mosiport, dev->mosipin);
			
			delay = node->clk;
			while(delay>0){
				delay--;	
			}
			
			data<<=1;
			mcu_io_output_setbit(dev->clkport, dev->clkpin);
			
			delay = node->clk;
			while(delay>0){
				delay--;	
			}
			
			misosta = mcu_io_input_readbit(dev->misoport, dev->misopin);
			if(misosta == MCU_IO_STA_1){
				data |=0x01;
			}else{
				data &=0xfe;
			}
			
			delay = node->clk;
			while(delay>0){
				delay--;	
			}
			
		}

		if(rsv != NULL)
			*(rsv+slen) = data;
		
		slen++;
	}

	return slen;
}



