/**
 * @file            dev_lcd.c
 * @brief           LCD �м��
 * @author          wujique
 * @date            2018��4��17�� ���ڶ�
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��4��17�� ���ڶ�
 *   ��    ��:         wujique
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
#include "board_sysconf.h"
#include "log.h"
#include "p_list.h"
#include "alloc.h"
#include "vfs.h"

#include "bus_lcd.h"
#include "dev_lcd.h"

#include "dev_ILI9341.h"
#include "dev_str7565.h"
#include "dev_IL91874.h"
#include "dev_IL3820.h"
#include "dev_ILI9325.h"
#include "dev_st7789.h"
#include "dev_st7735r.h"
#include "drv_nt35510.h"

#define DEV_LCD_DEBUG

#ifdef DEV_LCD_DEBUG
#define LCD_DEBUG	wjq_log 
#else
#define LCD_DEBUG(a, ...)
#endif

u16 PenColor = BLACK;
u16 BackColor = BLUE;

/*	LCD�����б� */
_lcd_drv *LcdDrvList[] = {
				/* tft lcd ILI9341 */
				#if( LCD_DRIVER_9341 == 1 )
					&TftLcdILI9341Drv,   
				#endif
				/* tft lcd ILI9325 */
				#if( LCD_DRIVER_9325 == 1 )	
					&TftLcdILI9325Drv,
				#endif
				/* COG LCD  */
				#if( LCD_DRIVER_ST7565 == 1 )
					&CogLcdST7565Drv,
				#endif
				/* oled  */
				#if( LCD_DRIVER_SSD1615 == 1 )
					&OledLcdSSD1615rv,
				#endif
				/* tft lcd, ���ǲ��ڣ�SPI ��I2C ��8BIT  */
				#if( LCD_DRIVER_9341_8BIT == 1 )
					&TftLcdILI9341_8_Drv,
				#endif
				/* 1.44�� �о�԰ tft lcd  */
				#if( LCD_DRIVER_7735 == 1 )
					&TftLcdST7735R_Drv,
				#endif
				/* 1.3�磬IPS���о�԰��ֻ��SCL&SDA��SPI�ӿ�LCD*/
				#if( LCD_DRIVER_7789 == 1 )
					&TftLcdST7789_Drv,
				#endif
				/*91874 �������� 2.7�����ֽ ��ɫ��*/
				#if( LCD_DRIVER_91874 == 1 )
					&TftLcdIL91874Drv,
				#endif
				/*3820 �������� GDEH154D27*/
				#if( LCD_DRIVER_3820 == 1 )	
					&TftLcdIL3820Drv,
				#endif

				/*ȫ�� NT35510 */
				#if( LCD_DRIVER_NT35510 == 1 )	
					&TftLcdNT35510Drv,
				#endif
};
/*	���Զ�ʶ��ID������*/
_lcd_drv *LcdProbDrvList[] = {
				#if( LCD_DRIVER_9341 == 1 )
					&TftLcdILI9341Drv,
				#endif
				#if( LCD_DRIVER_9325 == 1 )	
					&TftLcdILI9325Drv,
				#endif
				
				/*ȫ�� NT35510 */
				#if( LCD_DRIVER_NT35510 == 1 )	
					&TftLcdNT35510Drv,
				#endif
};
/**
 *@brief:      dev_lcd_finddrv
 *@details:    ����ID�����豸����
 *@param[in]   u16 id  
 *@param[out]  ��
 *@retval:     _lcd_drv
 */
static _lcd_drv *dev_lcd_finddrv(u16 id)
{
	u8 i =0;
	
	while(1){
		if(LcdDrvList[i]->id == id){
			return LcdDrvList[i];
		}
		i++;
		if(i>= sizeof(LcdDrvList)/sizeof(_lcd_drv *)){
			return NULL;
		}
	}
}

struct list_head DevLcdRoot = {&DevLcdRoot, &DevLcdRoot};	
/**
 *@brief:      dev_lcd_register
 *@details:    ע��LCD�豸
 *@param[in]   
 *@param[out]  
 *@retval:     
 */
s32 dev_lcd_register(const DevLcd *dev)
{
	struct list_head *listp;
	DevLcdNode *plcdnode;
	s32 ret = -1;
	
	wjq_log(LOG_INFO, "[register] lcd :%s, base on:%s!\r\n", dev->pnode.name, dev->buslcd);

	/*
		��Ҫ��ѯ��ǰ����ֹ����
	*/
	listp = DevLcdRoot.next;
	while(1) {
		if (listp == &DevLcdRoot) break;

		plcdnode = list_entry(listp, DevLcdNode, list);

		if (strcmp(dev->pnode.name, plcdnode->dev.pnode.name) == 0) {
			wjq_log(LOG_INFO, "lcd dev name err!\r\n");
			return -1;
		}

		if (strcmp(dev->buslcd, plcdnode->dev.buslcd) == 0) {
			wjq_log(LOG_INFO, "one lcd bus just for one lcd!\r\n");
			return -1;
		}
		
		listp = listp->next;
	}

	/* 
		����һ���ڵ�ռ� 
	*/
	plcdnode = (DevLcdNode *)wjq_malloc(sizeof(DevLcdNode));
	list_add(&(plcdnode->list), &DevLcdRoot);
	
	/*�����豸��Ϣ*/
	memcpy((u8 *)&plcdnode->dev, (u8 *)dev, sizeof(DevLcd));
	plcdnode->gd = -1;

	/* ��ʼ�� */
	if (dev->id == NULL) {
		LCD_DEBUG(LOG_DEBUG, "prob LCD id\r\n");

		/*�ҵ����������󣬳�ʼ��*/
		u8 j = 0;

		while(1) {
			ret = LcdProbDrvList[j]->init(plcdnode);
			if (ret == 0) {
				LCD_DEBUG(LOG_DEBUG, "lcd drv prob ok!\r\n");	
				plcdnode->drv = LcdProbDrvList[j];
				break;
			} else {
				j++;
				if (j >= sizeof(LcdProbDrvList)/sizeof(_lcd_drv *)) {
					LCD_DEBUG(LOG_DEBUG, "lcd prob err\r\n");
					break;
				}
			}
		}
	}else {
		ret = -1;
		LCD_DEBUG(LOG_DEBUG, "find lcd drv, id:%04x...", dev->id);
		plcdnode->drv = dev_lcd_finddrv(dev->id);
		if (plcdnode->drv != NULL) {
			/*�ҵ����������󣬳�ʼ��*/
			LCD_DEBUG(LOG_DEBUG, "suc!\r\n");
			ret = plcdnode->drv->init(plcdnode);
		} else {
			LCD_DEBUG(LOG_DEBUG, "fail!\r\n");
		}
	}

	if(ret == 0) {
		plcdnode->gd = -1;
		
		plcdnode->dir = H_LCD;
		
		plcdnode->height = plcdnode->dev.height;
		plcdnode->width = plcdnode->dev.width;
		
		dev_lcd_setdir(plcdnode, H_LCD, L2R_U2D);
		
		plcdnode->drv->onoff((plcdnode),1);//����ʾ
		
		plcdnode->drv->color_fill(plcdnode, 0, plcdnode->width, 0, plcdnode->height, BLUE);
		plcdnode->drv->update(plcdnode);
		plcdnode->drv->backlight(plcdnode, 1);

		wjq_log(LOG_INFO, "lcd init OK\r\n");
	} else {
		plcdnode->gd = -2;
		wjq_log(LOG_INFO, "lcd drv init err!\r\n");
	}
	
	return 0;
}


/**
 *@brief:      dev_lcd_open
 *@details:    ��LCD
 *@param[in]   char *name  
 *@param[out]  ��
 *@retval:     DevLcd
 */
DevLcdNode *dev_lcd_open(char *name)
{

	DevLcdNode *node;
	struct list_head *listp;
	
	//LCD_DEBUG(LOG_INFO, "lcd open:%s!\r\n", name);

	listp = DevLcdRoot.next;
	node = NULL;
	
	while(1) {
		if (listp == &DevLcdRoot) break;

		node = list_entry(listp, DevLcdNode, list);
		//LCD_DEBUG(LOG_INFO, "lcd name:%s!\r\n", node->dev.name);
		
		if (strcmp(name, node->dev.pnode.name) == 0) {
			//LCD_DEBUG(LOG_INFO, "lcd dev get ok!\r\n");
			break;
		} else {
			node = NULL;
		}
		
		listp = listp->next;
	}

	if (node != NULL) {
		if (node->gd > (-2)) node->gd++;
		else return NULL;
	}
	
	return node;
}

/**
 *@brief:      dev_lcd_close
 *@details:    �ر�LCD
 *@param[in]   DevLcd *dev  
 *@param[out]  ��
 *@retval:     
 */
s32 dev_lcd_close(DevLcdNode *node)
{
	if(node->gd <0)	return -1;
	else{
		node->gd -= 1;
		return 0;
	}
}
/*
����-1 ������ԭ��ı仯��
��APP�㣬ԭ���ǣ�1��1��������������ƽ���ˡ�

�������ͻ�Ϊ(0,0)�����۳����ǿ������Դ棬���Ǵӣ�0��0����ʼ

*/
s32 dev_lcd_drawpoint(DevLcdNode *lcd, u16 x, u16 y, u16 color)
{
	if (lcd == NULL)	return -1;
	
	return lcd->drv->draw_point(lcd, x-1, y-1, color);
}

s32 dev_lcd_prepare_display(DevLcdNode *lcd, u16 sx, u16 ex, u16 sy, u16 ey)
{
	if(lcd == NULL)	return -1;
	
	return lcd->drv->prepare_display(lcd, sx-1, ex-1, sy-1, ey-1);
}


s32 dev_lcd_fill(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 *color)
{	
	if(lcd == NULL)	return -1;
	
	return lcd->drv->fill(lcd, sx-1,ex-1,sy-1,ey-1,color);
}
s32 dev_lcd_color_fill(DevLcdNode *lcd, u16 sx,u16 ex,u16 sy,u16 ey,u16 color)
{
	if(lcd == NULL)	return -1;
	
	return lcd->drv->color_fill(lcd, sx-1,ex-1,sy-1,ey-1,color);
}
s32 dev_lcd_backlight(DevLcdNode *lcd, u8 sta)
{
	if(lcd == NULL)	return -1;
	
	lcd->drv->backlight(lcd, sta);
	return 0;
}
s32 dev_lcd_display_onoff(DevLcdNode *lcd, u8 sta)
{
	if(lcd == NULL)	return -1;

	return lcd->drv->onoff(lcd, sta);
}

s32 dev_lcd_flush(DevLcdNode *lcd, u16 *color, u32 len)
{
	if(lcd == NULL)	return -1;

	return lcd->drv->flush(lcd, color, len);	
}
s32 dev_lcd_update(DevLcdNode *lcd)
{
	if(lcd == NULL)	return -1;

	return lcd->drv->update(lcd);
}
/**
 *@brief:      dev_lcd_setdir
 *@details:    ���ú�����������ɨ�跽��
 *@param[in]   u8 dir       0 ����1����
               u8 scan_dir  �ο��궨��L2R_U2D       
 *@param[out]  ��
 *@retval:     
 */
s32 dev_lcd_setdir(DevLcdNode *node, u8 dir, u8 scan_dir)
{
	u16 temp;
	u8 scan_dir_tmp;

	if (node == NULL) return -1;

	//�л���Ļ����	
	if (dir != node->dir) {
		
		node->dir = node->dir^0x01;
		temp = node->width;
		node->width = node->height;
		node->height = temp;
		LCD_DEBUG(LOG_DEBUG, "set dir w:%d, h:%d\r\n", node->width, node->height);
	}
	
	//������ɨ�跽��ӳ��ת��
	if(node->dir == W_LCD) {
		/*
			����	 ����
			LR----UD
			RL----DU
			UD----RL
			DU----LR
			UDLR----LRUD
		*/
		scan_dir_tmp = 0;
		if ((scan_dir&LRUD_BIT_MASK) == 0) {
			scan_dir_tmp += LRUD_BIT_MASK;
		}

		if ((scan_dir&LR_BIT_MASK) == LR_BIT_MASK) {
			scan_dir_tmp += UD_BIT_MASK;	
		}

		if ((scan_dir&UD_BIT_MASK) == 0) {
			scan_dir_tmp += LR_BIT_MASK;
		}
	}else {
		scan_dir_tmp = scan_dir;
	}
	
	node->scandir = scan_dir_tmp;
	
	node->drv->set_dir(node, node->scandir);
	
	return 0;
}

/* 

��tslib����һЩ��ʾ����������
��Щ�������Թ�ΪGUI
*/
#include "font.h"

/**
 *@brief:      line
 *@details:    ��һ����
 *@param[in]   int x1           
               int y1           
               int x2           
               int y2           
               unsigned colidx  
 *@param[out]  ��
 *@retval:     
 */
void line (DevLcdNode *lcd, int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (lcd == NULL) return;

	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2){
			dev_lcd_drawpoint(lcd, x1 >> 16, y1, colidx);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			dev_lcd_drawpoint(lcd, x1, y1 >> 16, colidx);
			y1 += dy;
			x1++;
		}
	}
}

/**
 *@brief:     put_cross
 *@details:   ��ʮ��
 *@param[in]  int x            
              int y            
              unsigned colidx  
 *@param[out]  ��
 *@retval:     
 */
void put_cross(DevLcdNode *lcd, int x, int y, unsigned colidx)
{
	if (lcd == NULL) return;
	
	line (lcd, x - 10, y, x - 2, y, colidx);
	line (lcd, x + 2, y, x + 10, y, colidx);
	line (lcd, x, y - 10, x, y - 2, colidx);
	line (lcd, x, y + 2, x, y + 10, colidx);

	line (lcd, x - 6, y - 9, x - 9, y - 9, colidx + 1);
	line (lcd, x - 9, y - 8, x - 9, y - 6, colidx + 1);
	line (lcd, x - 9, y + 6, x - 9, y + 9, colidx + 1);
	line (lcd, x - 8, y + 9, x - 6, y + 9, colidx + 1);
	line (lcd, x + 6, y + 9, x + 9, y + 9, colidx + 1);
	line (lcd, x + 9, y + 8, x + 9, y + 6, colidx + 1);
	line (lcd, x + 9, y - 6, x + 9, y - 9, colidx + 1);
	line (lcd, x + 8, y - 9, x + 6, y - 9, colidx + 1);

}
/**
 *@brief:      put_char
 *@details:    ��ʾһ��Ӣ��
 *@param[in]   int x       
               int y       
               int c       
               int colidx  
 *@param[out]  ��
 *@retval:     
 */
void put_char(DevLcdNode *lcd, int x, int y, int c, int colidx)
{
	int i,j,bits;
	u8* p;
	
	if (lcd == NULL) return;	
	
	p = (u8*)font_vga_8x8.path;//need fix
	for (i = 0; i < font_vga_8x8.height; i++) {
		bits =  p[font_vga_8x8.height * c + i];
		for (j = 0; j < font_vga_8x8.width; j++, bits <<= 1) {
			if (bits & 0x80) {
				lcd->drv->draw_point(lcd, x + j, y + i, colidx);
			}
		}
	}
}
/**
 *@brief:      put_string
 *@details:    ��ʾһ���ַ���
 *@param[in]   int x            
               int y            
               char *s          
               unsigned colidx  
 *@param[out]  ��
 *@retval:     
 */
void put_string(DevLcdNode *lcd, int x, int y, char *s, unsigned colidx)
{
	int i;
	
	if (lcd == NULL)	return;	
	
	for (i = 0; *s; i++, x += font_vga_8x8.width, s++)
		put_char(lcd, x, y, *s, colidx);
}
/**
 *@brief:      put_string_center
 *@details:    ������ʾһ���ַ���
 *@param[in]   int x            
               int y            
               char *s          
               unsigned colidx  
 *@param[out]  ��
 *@retval:     
 */
void put_string_center(DevLcdNode *lcd, int x, int y, char *s, unsigned colidx)
{
	int sl = strlen (s);
	
	if(lcd == NULL)	return;	
	
    put_string (lcd, x - (sl / 2) * font_vga_8x8.width,
                y - font_vga_8x8.height / 2, s, colidx);
}

/**
 *@brief:      rect
 *@details:    ��һ�����ο�
 *@param[in]   int x1           
               int y1           
               int x2           
               int y2           
               unsigned colidx  
 *@param[out]  ��
 *@retval:     
 */
void rect (DevLcdNode *lcd, int x1, int y1, int x2, int y2, unsigned colidx)
{
	if (lcd == NULL) return;

	line (lcd, x1, y1, x2, y1, colidx);
	line (lcd, x2, y1, x2, y2, colidx);
	line (lcd, x2, y2, x1, y2, colidx);
	line (lcd, x1, y2, x1, y1, colidx);
}

/**
 *@brief:      dev_lcd_put_string
 *@details:    ��ʾ�ַ�����֧������
 *@param[in]   ��
 *@param[out]  ��
 *@retval:     	
 */
s32 dev_lcd_put_string(DevLcdNode *lcd, char *font, int x, int y, char *s, unsigned colidx)
{
	u16 slen;
	u16 xlen,ylen;
	u16 *framebuff;//���㻺�壬����L2R_U2D��ʽ���
	u8 *dotbuf;//�ַ����󻺳�
	s32 res;
	u16 sidx;
	u16 i,j;
	u32 xbase;//��ʾ��x��ƫ����
	u16 fontw,fonth;
	
	if (lcd == NULL)	return -1;
	
	/* ͨ��ˢһ���飬�����ʾ�ٶ� */
	slen = strlen(s);
	//uart_printf("str len:%d\r\n", slen);

	font_get_hw(font, &fonth, &fontw);
	/*
		�����ַ������ȼ���ˢ�����򳤿�
	*/
	xlen = slen*fontw;
	ylen = fonth;

	framebuff = (u16*)wjq_malloc(xlen*ylen*sizeof(u16));//���㻺��
	dotbuf = (u8*)wjq_malloc(32);//Ҫ��Ϊ�����ֿ���������
	sidx = 0;

	/*��ȡ���󣬲�ת��ΪLCD����*/
	while(1) {
		//Ӣ����ĸ
		if (*(s+sidx) < 0x81) {
			//uart_printf("eng\r\n");
			u8 ch;
			/*��ȡ����*/
			ch = *(s+sidx);
			
			res = font_get_asc(font, &ch, dotbuf);
			//PrintFormat(dotbuf, 16);
			/*asc�Ǻ��*/
			for (j=0;j<fonth;j++) {

				xbase = xlen*j + sidx*fontw;//��ǰ�ַ�X��ƫ����
				for (i=0;i<fontw;i++) {
					/*��ʱֻ����6*12��8*16��ASC��ÿһ��1���ֽ�*/
					if ((dotbuf[j*1+i/8]&(0x80>>(i%8)))!= 0) {
						//uart_printf("* ");
						framebuff[xbase + i] = colidx;
					} else {
						//uart_printf("- ");
						framebuff[xbase + i] = BackColor;
					}
				}
				//uart_printf("\r\n");
			}	
			
			sidx++;
		}		else {//����
			//uart_printf("ch\r\n");
			res = font_get_hz(font, s+sidx, dotbuf);//��SD����ȡһ��1616���ֵĵ���Ҫ1ms
			//PrintFormat(dotbuf, 32);

			/*����֧���ݿ⣬ȡģ��ʽ2,16*16*/
			for (j=0; j<fonth; j++) {
				xbase = xlen*j + sidx*fontw;//��ǰ�ַ�X��ƫ����
				for (i=0;i<(fontw*2);i++) {//������ASC������
					/*��ʱֻ��1212��1616��ÿһ��2���ֽ�����*/
					if ((dotbuf[i*2+j/8]&(0x80>>(j%8)))!= 0) {
						//uart_printf("* ");
						framebuff[xbase + i] = colidx;
					} else {
						//uart_printf("- ");
						framebuff[xbase + i] = BackColor;
					}
				}
				//uart_printf("\r\n");
			}	
			
			sidx+= 2;
		}

		if (sidx >= slen) {
			//uart_printf("finish");
			break;
		}
	}


	if ( y + ylen > lcd->height) {
		/*��ʾ������Ļ*/
		ylen = lcd->height - y+1;//����height = 240,y = 240, Ҳ����ζ��ֻ��ʾһ��
	}
	
	if (x + xlen >= lcd->width) {
		/*��ʾ������Ļ���*/
		i = lcd->width - x + 1;
		
		/*��������*/
		j = 1;
		while(1) {
			if (j >= ylen) break;
			memcpy(framebuff+j*i, framebuff+ j*xlen, 2*i);
			j++;
		}
		xlen = i;
	}

	dev_lcd_fill(lcd, x, x + xlen-1, y, y + ylen-1, framebuff);

	wjq_free(framebuff);
	wjq_free(dotbuf);

	return 0;	
}
extern void Delay(__IO uint32_t nTime);

#if 1
typedef struct tagBITMAPFILEHEADER  //�ļ�ͷ  14B  
{ 
    u16  bfType;   //0x424d, "BM"
    u32  bfSize;   //�ļ���С�������ļ�ͷ
    u16  bfReserved1;   //�����ֽ�
    u16  bfReserved2;   //�����ֽ�
    u32  bfOffBits;   	//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ��
} BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER  //λͼ��Ϣͷ
{ 
    u32 biSize;   //���ṹ���ȣ�Ҳ����40
    s32 biWidth;  //ͼ����   
    s32 biHeight; //ͼ��߶�    
    u16 biPlanes; //1  
    u16 biBitCount;//1�ڰ׶�ɫͼ��4 16λɫ��8 256ɫ��24 ���ɫ 
    u32 biCompression;   //�Ƿ�ѹ��
    u32 biSizeImage;   //ʵ��λͼ�����ֽ���
    s32 biXPelsPerMeter;  //Ŀ���豸ˮƽ�ֱ��� 
    s32 biYPelsPerMeter;   //Ŀ���豸��ֱ�ֱ���
    u32 biClrUsed;  //ͼ��ʵ���õ���ɫ������Ϊ0�����õ�����ɫ��Ϊ2��biBitCount�η�
    u32 biClrImportant;  //ָ����ͼ������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���е���ɫ������Ҫ��
} BITMAPINFOHEADER;

/*��ɫ��ÿ��Ԫ��*/
typedef struct tagRGBQUAD
{ 
	u8    rgbBlue; //��ɫ����  
	u8    rgbGreen; //��ɫ����    
	u8    rgbRed;   //��ɫ����  
	u8    rgbReserved;    
} RGBQUAD; 


#define WIDTHBYTES(i) ((i+31)/32*4)

/*

	4��ͼƬ��ˢ��ʱ�䣬ͼƬ������SD��

			FSMC	������		SPI�����У���U16��ΪU8����10ms��
	1bit	53ms 	30  	316
	4bit	74	 	50		340
	8bit	79	 	51		344
	24bit	111	 	91		378


	Ҫ���٣������������淽����
	����˫���壬��DMA������DMA�󣬲��ȴ�����ɣ�
	�ͳ���׼����һ�����ݣ�������һ��ǰ����ѯ��һ���Ƿ�����ɡ�
	������ԭ����ʱ�䣺
		����׼��ʱ��+���ݴ���ʱ��
	����ʱ���Ϊ��
		����׼��ʱ��/���ݴ���ʱ�䣬���߽ϳ���Ϊ��Ҫʱ�䡣
*/
s32 dev_lcd_show_bmp(DevLcdNode *lcd, u16 x, u16 y, u16 xlen, u16 ylen, s8 *BmpFileName)
{
	BITMAPFILEHEADER    bf;
    BITMAPINFOHEADER    bi;
	
	int bmpfd;
	
	u32 rlen;
    u16 LineBytes;
	u16 NumColors;
    u32 ImgSize;
	u16 buf[40];

	u32 i,j;
	u8 *palatte;
	volatile u16 color;
	u32 k, m;
	u16 r,g,b;
	u8 c;
	u16 *pcc;
	u8 *pdata;
	u8 linecnt = 20;//һ�ζ����У��ӿ��ٶ�
	u8 l;
	
	wjq_log(LOG_DEBUG, "bmp open file:%s\r\n", BmpFileName);

	bmpfd = vfs_open(BmpFileName, O_RDONLY);

	if (bmpfd == NULL) {
		wjq_log(LOG_DEBUG, "bmp open file err\r\n");
		return -1;
	}
	rlen = vfs_read(bmpfd, (void *)buf, 14);
    
	bf.bfType      = buf[0];
    bf.bfSize      = buf[2];
    bf.bfSize = (bf.bfSize<<16)+buf[1];
    bf.bfReserved1 = buf[3];
    bf.bfReserved2 = buf[4];
    bf.bfOffBits   = buf[6];
    bf.bfOffBits = (bf.bfOffBits<<16)+buf[5];
	
	wjq_log(LOG_DEBUG, "bf.bfType:%x\r\n", bf.bfType);	
	wjq_log(LOG_DEBUG, "bf.bfSize:%d\r\n", bf.bfSize);
	wjq_log(LOG_DEBUG, "bf.bfOffBits:%d\r\n", bf.bfOffBits);

	rlen = vfs_read(bmpfd, (void *)buf, 40);

	bi.biSize           = (unsigned long) buf[0];
    bi.biWidth          = (long) buf[2];
    bi.biHeight         = (long) buf[4];
    bi.biPlanes         = buf[6];
    bi.biBitCount       = buf[7];
    bi.biCompression    = (unsigned long) buf[8];
    bi.biSizeImage      = (unsigned long) buf[10];
    bi.biXPelsPerMeter  = (long) buf[12];
    bi.biYPelsPerMeter  = (long) buf[14];
    bi.biClrUsed        = (unsigned long) buf[16];
    bi.biClrImportant   = (unsigned long) buf[18];

	wjq_log(LOG_DEBUG, "bi.biSize:%d\r\n", bi.biSize);	
	wjq_log(LOG_DEBUG, "bi.biWidth:%d\r\n", bi.biWidth);
	wjq_log(LOG_DEBUG, "bi.biHeight:%d\r\n", bi.biHeight);
	wjq_log(LOG_DEBUG, "bi.biPlanes:%d\r\n", bi.biPlanes);
	wjq_log(LOG_DEBUG, "bi.biBitCount:%d\r\n", bi.biBitCount);
	wjq_log(LOG_DEBUG, "bi.biCompression:%d\r\n", bi.biCompression);
	wjq_log(LOG_DEBUG, "bi.biSizeImage:%d\r\n", bi.biSizeImage);
	wjq_log(LOG_DEBUG, "bi.biXPelsPerMeter:%d\r\n", bi.biXPelsPerMeter);
	wjq_log(LOG_DEBUG, "bi.biYPelsPerMeter:%d\r\n", bi.biYPelsPerMeter);
	wjq_log(LOG_DEBUG, "bi.biClrUsed:%d\r\n", bi.biClrUsed);
	wjq_log(LOG_DEBUG, "bi.biClrImportant:%d\r\n", bi.biClrImportant);

	/*8������ռ��һ���ֽڣ�����һ���ֽڲ���һ���ֽ�*/
	/*��ɫͼƬ���ֽڶ���*/
	LineBytes = WIDTHBYTES(bi.biWidth * bi.biBitCount);
    ImgSize   = (unsigned long) LineBytes  * bi.biHeight;

    wjq_log(LOG_DEBUG, "bmp w:%d,h:%d, bitcount:%d, linebytes:%d\r\n", bi.biWidth, bi.biHeight, bi.biBitCount, LineBytes);
	
	if(bi.biClrUsed!=0)
		NumColors=(unsigned long)bi.biClrUsed;//��� bi.biClrUsed ��Ϊ�㣬���Ǳ�ͼ��ʵ���õ�����ɫ
	else {
	    switch(bi.biBitCount)
	    {
	    case 1:
	        NumColors=2;//�ڰ����õ�������ɫ�壬һ���Ǻ�һ���ǰ�
	        break;
	        
	    case 4:
	        NumColors=16;
	        break;
	        
	    case 8:
	        NumColors=256;
	        break;
	        
	    case 24:
	        NumColors=0;
	        break;
	        
	    default:
			vfs_close(bmpfd);
	        return 2;
	    }
	}

	/* ����ɫ�� */
	if (NumColors != 0) {
		palatte = wjq_malloc(4*NumColors);
		rlen = vfs_read(bmpfd, (void *)palatte, 4*NumColors);

	}

	if (xlen > bi.biWidth) xlen = bi.biWidth;

    if (ylen > bi.biHeight) ylen = bi.biHeight;

	pdata = wjq_malloc(LineBytes*linecnt);
	
	dev_lcd_prepare_display(lcd, x, x+xlen-1, y, y+ylen-1);
			
    switch(bi.biBitCount)
    {
    case 1:
		pcc = wjq_malloc(xlen*sizeof(u16));
		//ͼƬȡģ:����,����ҵ�
		for (j=0; j<ylen;) {
        	if(j+linecnt >= ylen)
				linecnt = ylen-j;

			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			l = 0;
			while (l < linecnt) {
				k = l*LineBytes;
				#if 0//���ó����� ����
				i = 0;				
				while(1)
	            {
					/*
            		һ���ֽ�8�����أ���λ��ǰ
            		��ɫ����256����ɫ
					*/
		            c = pdata[k];
					
					m = 0;
					while(m <8)
					{
						if((c &(0x80>>m)) != 0)
	                		*(pcc+i)  = WHITE;
						else
							*(pcc+i)  = BLACK;	

						i++;
						if(i>= xlen)
							break;
						m++;
					}
					
					if(i>= xlen)
						break;

					k++;
	            }
				#else
				for (i=0; i<xlen; i++) {
		        	/*
		        		һ���ֽ�8�����أ���λ��ǰ
		        		��ɫ����256����ɫ
					*/
		            c = pdata[k+(i/8)]&(0x80>>(i%8));
		            
		            if(c != 0)
						*(pcc+i) = WHITE;
		            else
						*(pcc+i) = BLACK;
		        }
				#endif
				
				dev_lcd_flush(lcd, pcc, xlen);
				l++;
			}

			j += linecnt;
        }
		wjq_free(pcc);

        break;
        
    case 4:
		pcc = wjq_malloc(xlen*sizeof(u16));
		//ͼƬȡģ:����,����ҵ�s
		for (j=0; j<ylen;) {
        	if (j+linecnt >= ylen)
				linecnt = ylen-j;
			
			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);			
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			l = 0;
			while(l < linecnt) {
				k = l*LineBytes;

				#if 0//���ó���,����
				i = 0;
				while(1)
	            {
					/*4��bit 1�����أ�Ҫ���ж�U16��ת��
					rgb565
					#define BLUE         	 0x001F  
					#define GREEN         	 0x07E0
					#define RED           	 0xF800
					*/
					c = *(pdata+k);
					
					m = ((c>>4)&0x0f)*4;
					
					b = (palatte[m++] & 0xF8)>>3;
					g = (palatte[m++] & 0xFC)>>2;
					r = (palatte[m] & 0xF8)>>3;
	                                
	                *(pcc+i) = (r<<11)|(g<<5)|(b<<0);
					
					i++;
					if(i>= xlen)
						break;
					
					m = (c&0x0f)*4;
					
					b = (palatte[m++] & 0xF8)>>3;
					g = (palatte[m++] & 0xFC)>>2;
					r = (palatte[m] & 0xF8)>>3;

	                *(pcc+i) = (r<<11)|(g<<5)|(b<<0);
					
					i++;
					if(i>= xlen)
						break;
					k++;
	            }
				#else
				for (i=0; i < xlen; i++) {
									/*4��bit 1�����أ�Ҫ���ж�U16��ת��
					rgb565
	#define BLUE			 0x001F  
	#define GREEN			 0x07E0
	#define RED 			 0xF800
					*/
					m = *(pdata+k+i/2);
					
					if (i%2 == 0)
						m = ((m>>4)&0x0f);
					else
						m = (m&0x0f);
					
					m = m*4;
					
					r = (palatte[m+2] & 0xF8)>>3;
					g = (palatte[m+1] & 0xFC)>>2;
					b = (palatte[m] & 0xF8)>>3;
					
					*(pcc+i) = (r<<11)|(g<<5)|(b<<0);
				}
				#endif
				
				dev_lcd_flush(lcd, pcc, xlen);
				l++;
			}

			j += linecnt;
        }
		wjq_free(pcc);
        break;

    case 8:
		pcc = wjq_malloc(xlen*sizeof(u16));	
		//ͼƬȡģ:����,����ҵ�
		for (j=0; j<ylen;) {
        	if (j+linecnt >= ylen) linecnt = ylen-j;
			
			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);	
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			
			l = 0;
			while(l < linecnt) {
				k = l*LineBytes;
				
            	for (i=0; i < xlen; i++) {
					/*1���ֽ�1�����أ�Ҫ���ж�U16��ת��
					rgb565
					#define BLUE         	 0x001F  
					#define GREEN         	 0x07E0
					#define RED           	 0xF800
					*/
					m = *(pdata+k);
					k++;
					m = m*4;
					
					r = (palatte[m+2] & 0xF8)>>3;
	                g = (palatte[m+1] & 0xFC)>>2;
	                b = (palatte[m] & 0xF8)>>3;
					
	                *(pcc+i)  = (r<<11)|(g<<5)|(b<<0);
	            }
				
				dev_lcd_flush(lcd, pcc, xlen);
				
				l++;
			}

			j += linecnt;
        }
		wjq_free(pcc);
        break;

	case 16:

		break;
		
    case 24://65K���ɫ		
		pcc = (u16 *)pdata;
		//ͼƬȡģ:����,����ҵ�
        for (j=0; j<ylen;) {
			if(j+linecnt >= ylen)
				linecnt = ylen-j;
			
			rlen = vfs_read(bmpfd, (void *)pdata, LineBytes*linecnt);
			if (rlen != LineBytes*linecnt) {
				wjq_log(LOG_DEBUG, "bmp read data err!\r\n");
			}
			
			l = 0;
			while (l < linecnt) {
				k = l*LineBytes;
				
            	for (i=0; i < xlen; i++) {
	            	/*3���ֽ�1�����أ�Ҫ���ж�U16��ת��
						rgb565
						#define BLUE         	 0x001F  
						#define GREEN         	 0x07E0
						#define RED           	 0xF800
					*/
					b = pdata[k++];
					g = pdata[k++];
					r = pdata[k++];
					
					r = ((r<<8)&0xf800);
					g = ((g<<3)&0x07e0);
					b = ((b>>3)&0x001f);
					*(pcc+i) = r+g+b;

	            }
				
				dev_lcd_flush(lcd, pcc, xlen);
				l++;
			}

			j += linecnt;
        }			
        break;

	case 32:
		break;
		
    default:
        break;
    } 

	dev_lcd_update(lcd);
	
	wjq_free(pdata);
	if (NumColors != 0) {
		wjq_free(palatte);
	}
	vfs_close(bmpfd);
    return 0;
}
#endif
/**
 *@brief:      dev_lcd_test
 *@details:    LCD���Ժ���
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
void dev_lcd_test(void)
{
	DevLcdNode *LcdCog = NULL;
	DevLcdNode *LcdOled = NULL;
	DevLcdNode *LcdOledI2C = NULL;
	DevLcdNode *LcdTft = NULL;

	/*  �������豸 */
	LcdCog = dev_lcd_open("spicoglcd");
	if (LcdCog==NULL)
		wjq_log(LOG_FUN, "open cog lcd err\r\n");

	LcdOled = dev_lcd_open("vspioledlcd");
	if (LcdOled==NULL)
		wjq_log(LOG_FUN, "open oled lcd err\r\n");
	
	LcdTft = dev_lcd_open("tftlcd");
	if (LcdTft==NULL)
		wjq_log(LOG_FUN, "open tft lcd err\r\n");

	LcdOledI2C = dev_lcd_open("i2coledlcd");
	if (LcdOledI2C==NULL)
		wjq_log(LOG_FUN, "open oled i2c lcd err\r\n");
	
	/*�򿪱���*/
	dev_lcd_backlight(LcdCog, 1);
	dev_lcd_backlight(LcdOled, 1);
	dev_lcd_backlight(LcdOledI2C, 1);
	dev_lcd_backlight(LcdTft, 1);

	#if 0/*��֧�ֺ���ʱ*/
	put_string(LcdCog, 5, 5, "spi cog lcd", BLACK);
	put_string(LcdOled, 5, 5, "vspi oled lcd", BLACK);
	put_string(LcdOledI2C, 5, 5, "i2c oled lcd", BLACK);
	put_string(LcdTft, 5, 5, "2.8 tft lcd", BLACK);
	#endif

	#if 1
	dev_lcd_put_string(LcdOled, "songti12", 10,1, "ABC-abc��", BLACK);
	dev_lcd_put_string(LcdOled, "siyuan16", 1, 13, "����oled lcd", BLACK);
	dev_lcd_put_string(LcdOled, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdOled, "siyuan16", 1, 47, "�ݼ�ȸ������", BLACK);
	dev_lcd_update(LcdOled);
	dev_lcd_put_string(LcdCog, "songti12", 10,1, "ABC-abc��", BLACK);
	dev_lcd_put_string(LcdCog, "siyuan16", 1, 13, "����cog lcd", BLACK);
	dev_lcd_put_string(LcdCog, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdCog, "siyuan16", 1, 47, "�ݼ�ȸ������", BLACK);
	dev_lcd_update(LcdCog);
	dev_lcd_put_string(LcdTft, "songti12", 20,30, "ABC-abc��", RED);
	dev_lcd_put_string(LcdTft, "siyuan16", 20,60, "����tft lcd", RED);
	dev_lcd_put_string(LcdTft, "songti12", 20,100, "www.wujique.com", RED);
	dev_lcd_put_string(LcdTft, "siyuan16", 20,150, "�ݼ�ȸ������", RED);
	dev_lcd_update(LcdTft);
	dev_lcd_put_string(LcdOledI2C, "songti12", 10,1, "ABC-abc��", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,13, "����LcdOledI2C", BLACK);
	dev_lcd_put_string(LcdOledI2C, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,47, "�ݼ�ȸ������", BLACK);
	dev_lcd_update(LcdOledI2C);
	#endif
	
	while(1);
}



void dev_i2coledlcd_test(void)
{

	DevLcdNode *LcdOledI2C = NULL;

	LcdOledI2C = dev_lcd_open("i2coledlcd");
	if (LcdOledI2C==NULL)
		wjq_log(LOG_FUN, "open oled i2c lcd err\r\n");
	
	/*�򿪱���*/
	dev_lcd_backlight(LcdOledI2C, 1);

	dev_lcd_put_string(LcdOledI2C, "songti12", 10,1, "ABC-abc��", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,13, "����LcdOledI2C", BLACK);
	dev_lcd_put_string(LcdOledI2C, "songti12", 10,30, "www.wujique.com", BLACK);
	dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,47, "�ݼ�ȸ������", BLACK);
	dev_lcd_update(LcdOledI2C);

	LcdOledI2C = dev_lcd_open("i2coledlcd2");
	if (LcdOledI2C==NULL)
		wjq_log(LOG_FUN, "open oled i2c2 lcd err\r\n");
	
	/*�򿪱���*/
	dev_lcd_backlight(LcdOledI2C, 1);
	while(1) {
		dev_lcd_put_string(LcdOledI2C, "songti12", 10,1, "ABC-abc��", BLACK);
		dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,13, "����LcdOledI2C", BLACK);
		dev_lcd_put_string(LcdOledI2C, "songti12", 10,30, "www.wujique.com", BLACK);
		dev_lcd_put_string(LcdOledI2C, "siyuan16", 1,47, "�ݼ�ȸ������", BLACK);
		dev_lcd_update(LcdOledI2C);
		Delay(1000);
		dev_lcd_color_fill(LcdOledI2C, 1, 1000, 1, 1000, WHITE);
		dev_lcd_update(LcdOledI2C);
		Delay(1000);
	}

}

