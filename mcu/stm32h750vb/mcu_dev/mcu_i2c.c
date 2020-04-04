/**
 * @file            mcu_i2c.c
 * @brief           
 * @author          test
 * @date            2017��10��30�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:      2017��10��30�� ����һ
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

#define MCU_I2C_DEBUG

#ifdef MCU_I2C_DEBUG
#define I2C_DEBUG	wjq_log 
#else
#define I2C_DEBUG(a, ...)
#endif


#define SCCB_TIMEOUT_MAX    10000
	
void SCCB_GPIO_Config(I2C_TypeDef* I2CDef)
{
	


}
	
uint8_t bus_sccb_writereg(I2C_TypeDef* I2CDef, uint8_t DeviceAddr, uint16_t Addr, uint8_t Data)
{
	
	
	
	/* If operation is OK, return 0 */
	return 0;

}
	
uint8_t bus_sccb_readreg(I2C_TypeDef* I2CDef, uint8_t DeviceAddr, uint16_t Addr)
{
 
  return 0;
}

s32 mcu_hi2c_init(DevI2c * dev)
{

	
	return 0;
}


s32 mcu_hi2c_transfer(DevI2cNode *node, u8 addr, u8 rw, u8* data, s32 datalen)
{
	if(rw == MCU_I2C_MODE_W)//д
    {

    }
    else if(rw == MCU_I2C_MODE_R)//��
    {
		
    }
	return -1;
}

