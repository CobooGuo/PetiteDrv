/**
 * @file            font.c
 * @brief           �ֿ����
 * @author          wujique
 * @date            2018��3��2�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��3��2�� ������
 *   ��    ��:       �ݼ�ȸ������
 *   �޸�����:   �����ļ�
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
#include "vfs.h"
#include "board_sysconf.h"
#include "font.h"
/*
	��õķ�����ʵ�����ֿ��Ծ٣�Ҳ�������ֿ��ͷ�������ֿ���Ϣ��
	��ʱ������ô�����ˡ�
*/

/*

	asc�ַ�����ʹ�ú�⡣
	ASC�ֿ�������Դ�����С�
	FontAscList�Ķ���Ҫ�����ֿⶨ������һ��
*/
const struct fbcon_font_desc *FontAscList[FONT_LIST_MAX]=
		{
			&font_vga_8x16,
			&font_vga_6x12,
			&font_vga_8x16,
			&font_vga_6x12
		};

s32 FontInit = -1;		

int FontFd[FONT_LIST_MAX];

u8 font_find_index(char *font)
{
	u8 i =0;

	while(1)
	{
		if(0 == strcmp(FontList[i]->name, font))
        {
        	return i;
		}
		
		i++;
		if(i>=FONT_LIST_MAX)
			break;
	}
	return i;
	
}
/**
 *@brief:      font_check_hzfont
 *@details:    ����ֿ⣬��Ҫ�Ǻ��ֿ�
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 font_hzfont_init(void)
{
	u8 i;

	i = 0;
	while(1)
	{
		if(i>= FONT_LIST_MAX)
			break;

		wjq_log(LOG_INFO, "font:%s , path:%s,", FontList[i]->name, FontList[i]->path);
		
		FontFd[i] = vfs_open(FontList[i]->path, O_RDONLY);
		
		if(FontFd[i] == NULL)
		{
			wjq_log(LOG_INFO, "err\r\n");
		}
		else
		{
			wjq_log(LOG_INFO, "ok!\r\n");	
		}
		
		i++;
	}

	FontInit = 0;
	return 0;
}

/**
 *@brief:      font_get_hz
 *@details:    ��ȡ���ֵ���
 *@param[in]   FontType type  
               char *ch       
               char *buf      
 *@param[out]  ��
 *@retval:     
 */
s32 font_get_hz(char *font, u8 *ch, u8 *buf)
{
	unsigned int len;
	u32 addr;
	u8 hcode,lcode;
	int res;
	u8 i = 0;

	if(FontInit == -1)
		font_hzfont_init();

	i = font_find_index(font);
	
	if(i >= FONT_LIST_MAX)
		return -1;
	
	hcode = *ch;
	lcode = *(ch+1);
	
	if((hcode < 0x81) 
		|| (hcode > 0xfe)
		)
	{
		//uart_printf("no china hz\r\n");
		return -1;
	}

	//uart_printf("hz code:%02x, %02x\r\n", hcode, lcode);

	addr = (hcode-0x81)*190;
	if(lcode<0x7f)
	{
		addr = addr+lcode-0x40;	
	}
	else
	{
		addr = addr+lcode-0x41;	
	}
	addr = addr*FontList[i]->size;
	//uart_printf("adr:%08x\r\n", addr);
	
	vfs_lseek(FontFd[i], addr, SEEK_SET);
	res = vfs_read(FontFd[i], (const void *)buf, FontList[i]->size);
	

	if(res != FontList[i]->size)
	{
		//uart_printf("font read err\r\n");
		return -1;
	}
	
	return 0;
	
}
/**
 *@brief:      font_get_asc
 *@details:    ��ȡASC�ַ���������
 *@param[in]   FontType type  
               char *ch       
               char *buf      
 *@param[out]  ��
 *@retval:     
 */
s32 font_get_asc(char *font, u8 *ch, u8 *buf)
{
	u8* fp;
	u8 i;
	
	if(*ch >= 0x80)
		return -1;
	
	if(FontInit == -1)
			font_hzfont_init();
	
	i = font_find_index(font);
	
	if(i >= FONT_LIST_MAX)
		return -1;

	fp = (u8*)FontAscList[i]->path + (*ch)*FontAscList[i]->size;
	//wjq_log(LOG_DEBUG, "dot data\r\n");
	//PrintFormat(fp, 16);
	
	memcpy(buf, fp, FontAscList[i]->size);

	return 0;
}
/**
 *@brief:      font_get_hw
 *@details:    ��ȡ���峤��
 *@param[in]   FontType type  
               u8 *h       
               u8 *w      
 *@param[out]  ��
 *@retval:     
 			���ص��ǵ����ַ�����Ҳ���Ƕ�Ӧ��ASC��ȣ������������ַ����
 */

s32 font_get_hw(char *font, u16 *h, u16 *w)
{
	u8 i;
	
	if(FontInit == -1)
		font_hzfont_init();

	i = font_find_index(font);
	
	if(i >= FONT_LIST_MAX)
		return -1;

	*w = FontAscList[i]->width;
	*h = FontAscList[i]->height;

	return 0;
}

