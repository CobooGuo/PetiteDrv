/**
 * @file                wujique_stm407.c
 * @brief           �ݼ�ȸSTM32F407������Ӳ�����Գ���
 * @author          wujique
 * @date            2018��1��29�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        2018��1��29�� ����һ
 *   ��    ��:         wujique
 *   �޸�����:   �����ļ�
*/
#include "mcu.h"
#include "board_sysconf.h"

#include "log.h"
#include "font.h"
#include "emenu.h"


extern u16 PenColor;
extern u16 BackColor;

#define TEST_FONG "songti12"


DevLcdNode * WJQTestLcd;

s32 wjq_wait_key(u8 key)
{

	return 0;
}

s32 wjq_test_showstr(char *s)
{
	wjq_log(LOG_DEBUG, "test:%s", s);
	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, BackColor);
	
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, s, PenColor);
	dev_lcd_update(WJQTestLcd);
	wjq_wait_key(0);
	
	return 0;
}	

s32 wjq_test(void)
{
	wjq_test_showstr((char *)__FUNCTION__);
	return 0;
}


#include "tslib.h"
extern struct tsdev *ts_open_module(void);
s32 test_tp_calibrate(void)
{
	DevLcdNode *lcd;

	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	lcd = dev_lcd_open("tftlcd");
	if(lcd == NULL)
	{
		wjq_test_showstr("open lcd err!");	
	}
	else
	{
		dev_lcd_setdir(lcd, H_LCD, L2R_U2D);
		dev_touchscreen_open();
		//ts_calibrate(lcd);
		dev_touchscreen_close();
	}
	
	dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
	dev_lcd_update(lcd);
	dev_lcd_close(lcd);
	
	return 0;
}


s32 test_tp_test(void)
{
	DevLcdNode *lcd;

	dev_lcd_color_fill(WJQTestLcd, 1, 1000, 1, 1000, WHITE);
	/*���о�����ʾ���˵�*/
	dev_lcd_put_string(WJQTestLcd, TEST_FONG, 1, 32, (char *)__FUNCTION__, BLACK);
	dev_lcd_update(WJQTestLcd);
	lcd = dev_lcd_open("tftlcd");
	if(lcd == NULL)
	{
		wjq_test_showstr("open lcd err!");	
	}
	else
	{
		dev_lcd_setdir(lcd, H_LCD, L2R_U2D);
		dev_touchscreen_open();	
	
		struct tsdev *ts;
		//ts = ts_open_module();

		struct ts_sample samp[10];
		int ret;
		u8 i =0;	
		while(1)
		{
			//ret = ts_read(ts, samp, 10);
			if(ret != 0)
			{
				//uart_printf("pre:%d, x:%d, y:%d\r\n", samp[0].pressure, samp[0].x, samp[0].y);
						
				i = 0;
				
				while(1)
				{
					if(i>= ret)
						break;
					
					if(samp[i].pressure != 0 )
					{
						//uart_printf("pre:%d, x:%d, y:%d\r\n", samp.pressure, samp.x, samp.y);
						dev_lcd_drawpoint(lcd, samp[i].x, samp[i].y, RED); 
					}
					i++;
				}
			}

			u8 keyvalue;
			s32 res;
			
			//res = dev_keypad_read(&keyvalue, 1);
			if (res == 1) {
				if (keyvalue == 8) {
					dev_lcd_color_fill(lcd, 1, 1000, 1, 1000, BLUE);
					dev_lcd_update(lcd);
				} else if(keyvalue == 12) {
					break;
				}
			}
		}

		dev_touchscreen_close();
	}
	return 0;
}



const MENU WJQTestList[]=
{
	MENU_L_0,//�˵��ȼ�
	"���Գ���",//����
	"test",	//Ӣ��
	MENU_TYPE_KEY_2COL,//�˵�����
	NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"LCD",//����
		"LCD",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			/*
			MENU_L_2,//�˵��ȼ�
			"VSPI OLED",//����
			"VSPI OLED",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_vspi_oled_display,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			*/
			MENU_L_2,//�˵��ȼ�
			"I2C OLED",//����
			"I2C OLED",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			/*
			MENU_L_2,//�˵��ȼ�
			"SPI COG",//����
			"SPI COG",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_spi_cog_display,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			*/
			MENU_L_2,//�˵��ȼ�
			"SPI tft",//����
			"SPI tft",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			//test_lcd_spi_128128,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			wjq_test,
			
			MENU_L_2,//�˵��ȼ�
			"tft",//����
			"tft",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
	
			MENU_L_2,//�˵��ȼ�
			"ͼƬ����",//����
			"test BMP",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"�ֿ����",//����
			"test Font",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"����",//����
		"sound",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"������",//����
			"buzzer",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"DAC����",//����
			"DAC music",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"����",//����
			"FM",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"I2S����",//����
			"I2S Music",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"¼��",//����
			"rec",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"������",//����
		"tp",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"У׼",//����
			"calibrate",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_tp_calibrate,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"����",//����
			"test",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			test_tp_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
		MENU_L_1,//�˵��ȼ�
		"����",//����
		"KEY",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"���İ尴��",//����
			"core KEY",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			/*��������*/
			MENU_L_2,//�˵��ȼ�
			"��������",//����
			"touch key",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			/*���Բ��⣬�������Գ����Ҫʹ�þ��󰴼�*/
			MENU_L_2,//�˵��ȼ�
			"���󰴼�",//����
			"keypad",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
		MENU_L_1,//�˵��ȼ�
		"����",//����
		"camera",	//Ӣ��
		MENU_TYPE_FUN,//�˵�����
		wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
		MENU_L_1,//�˵��ȼ�
		"SPI FLASH",//����
		"SPI FLASH",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"���İ�FLASH",//����
			"core FLASH",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"�װ� FLASH",//����
			"board FLASH",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
		MENU_L_1,//�˵��ȼ�
		"ͨ��",//����
		"con",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"485",//����
			"485",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"485 ����",//����
				"485 rec",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"485 ����",//����
				"485 snd",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
			MENU_L_2,//�˵��ȼ�
			"CAN",//����
			"CAN",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"CAN ����",//����
				"CAN rec",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				
				MENU_L_3,//�˵��ȼ�
				"CAN ����",//����
				"CAN snd",	//Ӣ��
				MENU_TYPE_FUN,//�˵�����
				wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			
		
		
			MENU_L_2,//�˵��ȼ�
			"����",//����
			"uart",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"����",//����
			"eth",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"OTG",//����
			"OTG",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"HOST",//����
				"HOST",	//Ӣ��
				MENU_TYPE_LIST,//�˵�����
				NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
				MENU_L_3,//�˵��ȼ�
				"Device",//����
				"Device",	//Ӣ��
				MENU_TYPE_LIST,//�˵�����
				NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		MENU_L_1,//�˵��ȼ�
		"ģ��",//����
		"mod",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
			/*�����ӿڲ���*/
			MENU_L_2,//�˵��ȼ�
			"RF24L01",//����
			"RF24L01",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			MENU_L_2,//�˵��ȼ�
			"MPU6050",//����
			"MPU6050",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

			/*�ô�������8266ģ��*/
			MENU_L_2,//�˵��ȼ�
			"wifi",//����
			"wifi",	//Ӣ��
			MENU_TYPE_FUN,//�˵�����
			wjq_test,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		

		MENU_L_1,//�˵��ȼ�
		"test",//����
		"test",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
		MENU_L_1,//�˵��ȼ�
		"test1",//����
		"test1",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test2",//����
		"test2",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test3",//����
		"test3",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test4",//����
		"test4",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test5",//����
		"test5",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test6",//����
		"test6",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test7",//����
		"test7",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test8",//����
		"test8",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test9",//����
		"test9",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��

		MENU_L_1,//�˵��ȼ�
		"test10",//����
		"test10",	//Ӣ��
		MENU_TYPE_LIST,//�˵�����
		NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
		
			MENU_L_2,//�˵��ȼ�
			"t10-1",//����
			"t10-1",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-2",//����
			"t10-2",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-3",//����
			"t10-3",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-4",//����
			"t10-4",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-5",//����
			"t10-5",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-6",//����
			"t10-6",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-7",//����
			"t10-7",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-8",//����
			"t10-8",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-9",//����
			"t10-9",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-10",//����
			"t10-10",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-11",//����
			"t10-11",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
			MENU_L_2,//�˵��ȼ�
			"t10-12",//����
			"t10-12",	//Ӣ��
			MENU_TYPE_LIST,//�˵�����
			NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
	
	/*���Ĳ˵��ǽ����˵���������*/			
	MENU_L_0,//�˵��ȼ�
	"END",//����
	"END",	//Ӣ��
	MENU_TYPE_NULL,//�˵�����
	NULL,//�˵����������ܲ˵��Ż�ִ�У����Ӳ˵��Ĳ���ִ��
};


void albatross_750_test(void)
{
	wjq_log(LOG_DEBUG,"run app\r\n");

	//WJQTestLcd = dev_lcd_open("tftlcd");
	//WJQTestLcd = dev_lcd_open("spicoglcd");
	WJQTestLcd = dev_lcd_open("i2coledlcd");
	if (WJQTestLcd == NULL) {
		wjq_log(LOG_DEBUG, "emenu open lcd err\r\n");
	}

	//dev_keypad_open();
	//emenu_run(WJQTestLcd, (MENU *)&WJQTestList[0], sizeof(WJQTestList)/sizeof(MENU), TEST_FONG, 1, MENU_LANG_ENG);	

	/*�򿪱���*/
	dev_lcd_backlight(WJQTestLcd, 1);

	//dev_lcd_put_string(WJQTestLcd, "songti12", 10,1, "ABC-abc", BLACK);
	//dev_lcd_put_string(WJQTestLcd, "siyuan16", 1,13, "����LcdOledI2C", BLACK);
	//dev_lcd_put_string(WJQTestLcd, "songti12", 10,30, "www.wujique.com", BLACK);
	//dev_lcd_put_string(WJQTestLcd, "siyuan16", 1,47, "�ݼ�ȸ������", BLACK);
	
	dev_lcd_put_string(WJQTestLcd, "songti12", 10,1, "ABC-abc", BLACK);
	//dev_lcd_put_string(WJQTestLcd, "siyuan16", 1,13, "LcdOledI2C", BLACK);
	dev_lcd_put_string(WJQTestLcd, "songti12", 10,30, "www.wujique.com", BLACK);
	//dev_lcd_put_string(WJQTestLcd, "siyuan16", 1,47, "Albatross", BLACK);
	dev_lcd_update(WJQTestLcd);
	
	while(1) {
		wjq_log(LOG_DEBUG, "Albatross_750_test run \r\n");
		Delay(1000);
	}
}

#include "FreeRtos.h"

TaskHandle_t  CanaryTaskHandle;

s32 albatross_750test_init(void)
{
	#if 0
	xTaskCreate(	(TaskFunction_t) albatross_750_test,
					(const char *)"canary stm103 test task",		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
					(const configSTACK_DEPTH_TYPE) ALBATROSS_TASK_STK_SIZE,
					(void *) NULL,
					(UBaseType_t) ALBATROSS_TASK_PRIO,
					(TaskHandle_t *) &CanaryTaskHandle );	
	#else
	albatross_750_test();
	#endif				
	return 0;
}



