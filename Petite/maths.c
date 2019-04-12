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
int log2(int value) 
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
*/
int BinarySearch(u16 *pArray, u16 data, int front, int end)
{
    int low, high, mid;
    low = front;
    high = end;
    while(low <= high)
    {

        mid = (low + high) / 2;
        if (*(pArray + mid) < data)
        {
            low = mid + 1;
        }
        else if (*(pArray + mid) > data)
        {
            high = mid - 1;
        }
        else 
        {
            return mid;
        }
    }
    return -1;
}
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


