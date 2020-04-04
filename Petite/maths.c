/**
 * @file            
 * @brief           ��ѧ����
 * @author          wujique
 * @date            2018��06��26�� ���ڶ�
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:      
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
#include "log.h"

/*

�ǵݹ��ж�һ������2�Ķ��ٴη�

*/
int math_log2(int value) 
{ 
  int x=0; 
  while(value>1) 
  { 
	value>>=1; 
	x++; 
  } 
  return x; 
} 

/*
	�������U16 ����ϣ������
*/
void ShellSort(u16 *pSrc, s32 Len)
{
    u32 d;//����
    s32 j, k;

    u16 temp;

    //uart_printf("shell:%d\r\n", Len);

    for(d = Len/2; d >0; d=d/2)
    {
        for(j = d; j < Len; j++)
        {
            temp = *(pSrc + j);

            for(k = j - d; (k >=0 && temp <(*(pSrc + k))); k -=d)
            {
                *(pSrc + k + d) = *(pSrc + k);
            }

            *(pSrc + k + d) = temp;
        }
    }
}

/*
	��������: 
	��������: ���ַ�����u16
	��ڲ���: 
	�� �� ֵ: ����-1û�ҵ�������Ϊ����ֵ
	���������ǽ���
*/
int BinarySearch(u16 *pArray, u16 data, u16 len)
{
    s16 s, e, mid;
	
    s = 0;
    e = len - 1;
	wjq_log(LOG_INFO,"find: %d---", data);
	
    while(s <= e)
    {

        mid = (s + e) / 2;

        if (*(pArray + mid) > data)
        {
            s = mid + 1;
        }
        else if (*(pArray + mid) < data)
        {
            e = mid - 1;
        }
        else 
        {
        	wjq_log(LOG_INFO,"yes %d %d %d\r\n", s,e,mid);
            return mid;
        }
    }
	
	mid = s;
	s = e;
	e = mid;
	
	wjq_log(LOG_INFO,"no %d %d\r\n", s,e);
    return -1;
}
/*
	��������������
*/
int BinarySearchD(u16 *pArray, u16 data, u16 len)
{
    s16 s, e, mid;
	
    
	wjq_log(LOG_INFO,"find: %d---", data);

	s = 0;
    e = len - 1;
	
    while(1)
    {

        mid = (s + e) / 2;

        if (*(pArray + mid) < data)
        {
            s = mid;
        }
        else if (*(pArray + mid) > data)
        {
            e = mid;
        }
        else 
        {
        	wjq_log(LOG_INFO,"yes %d %d %d\r\n", s,e,mid);
            return mid;
        }
		
		if(s == e)
			break;
    }
	
	wjq_log(LOG_INFO,"no %d %d\r\n", s,e);
    return -1;
}

#if 0
//const u16 test1[10] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
const u16 test1[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};


const u16 test2[30] = {101, 100,99, 
						91, 90, 89, 
						81, 80, 79,
						71, 70, 69, 
						61, 60, 59,
						51, 50, 49, 
						41, 40, 39, 
						31, 30, 29, 
						21, 20, 19,
						11, 10, 9};

void test_bs(void)
{
	u16 i=0;

	while(i<30)
	{
		BinarySearchD(test1, test2[i], 10);	
		i++;
	}

	while(1);
}

#endif

/*
	��������: 
	��������: ascתU32
	��ڲ���: 
	�� �� ֵ: ����-1û�ҵ�������Ϊ����ֵ
*/
u32 ascii_2_u32(u8 *pb,u8 asciidatalen)
{
    u32 i;
    u32 d;
    u8 tmpvalue;

    d=0;
    for(i=0; i<asciidatalen&&i<8; i++)
    {
        if((*(pb+i))>='a'&&(*(pb+i))<='f')
        {
            tmpvalue = 0x0a + (*(pb+i)) - 'a';
        }
        else if((*(pb+i))>='A'&&(*(pb+i))<='F')
        {
            tmpvalue = 0x0a + (*(pb+i)) - 'A';
        }
        else if((*(pb+i))>='0'&&(*(pb+i))<='9')
        {
            tmpvalue = (*(pb+i));
        }
        else
        {
            return 0;
        }

        d = d<<4;
        d += (tmpvalue & 0x0f);
    }

    return d;
}

/*
	��������: 
	��������: ��ָ��  pData ��ʼλ�ò���һ��[...]�ַ���������������֮��
                    �����ݿ�����seg�У�len˵��seg buf�ĳ��ȣ��ҵ��󷵻��ַ�������
                    ͬʱ�����Ѿ���ѯ�ĳ���index
	��ڲ���: 
	�� �� ֵ: 0 �ҵ���-1 û�ҵ�
*/
s32 vfs_find_seg(const u8 *pData, u32 DataLen,u8 *seg, u32 *len ,u32 *ScanLen)
{
    u32 index = 0;
    u32 seg_buf_len = *len;
    u32 str_len = 0;
    s32 res = -1;

    while(index < DataLen)
    {
        if(*(pData+index) == '[')
            break;
        index++;
    }

    index++;

    while((index < DataLen) && (str_len < seg_buf_len))
    {
        if(*(pData+index) == ']')
        {
            res = 0;
            break;
        }

        *(seg+str_len) = *(pData+index);
        index++;
        str_len++;
    }

    if(res == 0)
        *len = str_len;
    else
        *len = 0;

    *ScanLen = index;
    return res;
}


