/**
 * @file            
 * @brief           
 * @author          wujique
 * @date            2019��3��2�� ������
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:        
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
#include "board_sysconf.h"
#include "vfs.h"
#ifdef SYS_FS_FATFS
#include "ff.h"
#endif
/*
	��������ɶ�10%
	
	vfs��Ŀ���Ƕ��ϲ�����fs�ľ���ʵ�֡�
	���磬���ֿ⣬�п�����fatfs�����SD����Ҳ�п�����littlefs�����SPI FLASH��������û��fs��spi FLASH(��constfs�ӿ�)��
	α���̣�
	0.ͨ���궨�壬����֧��ʲô�ļ�ϵͳ��������ؽӿ�ע�ᵽvfs
	1.�ϵ�ʱ����һ��FSע�ᵽVFS��ע�����ݰ���FS���ͣ�FS�̷���FSӲ����Ϣ�� FSӲ�̲����ӿڡ�
	2.vfsͨ��FS���;������������ļ�ϵͳ�������ӿڣ�����Ӳ����Ϣ����ӿڡ�
	3.�ļ�ϵͳ���ⲿģ�飬�޷���֤���룬�ܿ�����Ҫ��vfs��͹����������⡣

							{
								spiffs�ļ�ϵͳ�ӿ�
							}
							{
								littlefs�ļ�ϵͳ�ӿ�
							}
							{
								fatfs�ļ�ϵͳ�ӿ�
							}
			   ------����	
app-----VFS ----|
			   ------�豸
							{
								mtd0 
								fatfs
								XXX,
								XXXX
							}	
							{	
								//u��
								mtd1
								fatfs
								XXX,
								xxx
							}
							{
								mtd2
								littlefs
								//Ӳ����Ϣ��Ӳ�̽ӿڣ����ṩ���ļ�ϵͳʹ�õģ�
								��ͬ���ļ�ϵͳ����Ϣ��Ҫ�����Щ�����
								spi flash
								spi flash�ӿ�
							}
							������
*/
#define Test_VFS 

#ifdef Test_VFS
#define VfsDebug wjq_log
#else
#define VfsDebug(a,...)
#endif

/*
	������ͬ�ļ�ϵͳ, mountʱ��Ҫ������
*/
union _uFileSysPra
{
	#ifdef SYS_FS_FATFS
    FATFS fatfs;	//fatfs
    #endif
	//CONSTFS constfs;	//const fs
	u8 NA;
};
/*
	�����ļ�ʱ��ͬ���ļ�ϵͳ��Ҫ�����ݽṹ�в��죬
	��union ����
*/
union _uFilePra
{
	#ifdef SYS_FS_FATFS
	FIL fatfd;//fatfs
	#endif
	u8 NA;
};
	
/*
	�ļ�ϵͳ�ڵ�
*/
typedef struct _strVfsNode
{
	const VFSDIR *Mtd;
	union _uFileSysPra pra;
}FsNode;
/*
	�б�
*/
FsNode VfsNodeList[SYS_FS_NUM+1];



typedef struct _strFileNode
{
	union _uFilePra pra;
	FsNode *fsnode;//��¼�ļ������ĸ��ļ�ϵͳ
}FileNode;


/*
**Description:      ��һ��������ַ����ļ�·�����и��1��Ŀ¼���ļ�����
                    ����Ŀ¼�����ļ�����
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
*/
static s32 vfs_get_dir_name(const u8 *lpPath, u8 *lpDir, u8 *lpName)
{
    int index;
    u8 dir[VFS_DIR_LEN];//Ŀ¼��ֻ�жϵ�һ��
    u8 name[VFS_NAME_LEN];//�ļ���
    u8 name_index;    
    u8 str_len;
    u8 flag = 0;

    index = 0;
    name_index = 0;
    memset(name, 0, sizeof(name));
    memset(dir, 0, sizeof(dir));
    
    while(index < strlen(lpPath))
    {
        
        if(*(lpPath+index) == '/'
            && flag == 0)//�ָ���
        {
            str_len = strlen(name);
            if( str_len > VFS_DIR_LEN)
            {
                VfsDebug(LOG_DEBUG, "Ŀ¼��̫��\r\n");
                return -1;
            }
            else if(str_len > 0)
            {
                strcpy(dir, name);//ֻ�ܱ���һ��·��
                name_index = 0;
                memset(name, 0, sizeof(name)); 
                flag = 1;
            }
            
        }
        else
        {
            name[name_index++] = *(lpPath+index);
            if(name_index >= VFS_NAME_LEN)
            {
                VfsDebug(LOG_DEBUG, "�ļ���̫��\r\n");
                return -1;
                break;//�ļ���̫���������˳�
            }
        }

        index++;
    }

    strcpy(lpDir, dir);
    strcpy(lpName, name);

    if(strlen(dir) == 0)
    {
        strcpy(lpDir,"mtd0");
    }
    
    return 0;
}

/*
**Description:      ��ʼ�������ļ�ϵͳ�б�     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
*/
void vfs_init(void)
{
	u8 i = 0;
	while(1)
	{
		if(i>= SYS_FS_NUM)
			break;
		VfsNodeList[i].Mtd = NULL;
		i++;
	}
	
}
/*
**Description:      ��һ��mtd��root������     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
*/
s32 vfs_mount(const VFSDIR *Mtd)
{
	u8 i = 0;
	
    VfsDebug(LOG_DEBUG, "mount %s �� / \r\n", Mtd->name);

    while(1)
	{
		if(i>= SYS_FS_NUM)
		{
			VfsDebug(LOG_DEBUG, "mount err\r\n");
			break;
		}
		
		if(VfsNodeList[i].Mtd == NULL)
		{
			VfsNodeList[i].Mtd = Mtd;

			/*���� �ļ�ϵͳ���͵��ö�Ӧ�ĳ�ʼ��*/
			switch(VfsNodeList[i].Mtd->type)
			{
				case FS_TYPE_FATFS:
					VfsDebug(LOG_DEBUG, "mount FS_TYPE_FATFS\r\n");
					#ifdef SYS_FS_FATFS
					FRESULT res;

					/* Initialises the File System*/
					res = f_mount(&VfsNodeList[i].pra.fatfs, SYS_FS_FATFS_SD, 1);
					if ( res == FR_OK ) 
					{
						wjq_log(LOG_FUN, "> sd File System initialized.\r\n");
						return 0;
					}
					//vfs_explore_disk("1:/", 1);
					
					wjq_log(LOG_FUN, "> sd Cannot initialize File System:%d\r\n", res);
					#endif
					return -1;
					
				case FS_TYPE_SPIFFS:
					VfsDebug(LOG_DEBUG, "mount FS_TYPE_SPIFFS\r\n");
					break;
				
				case FS_TYPE_LITTLEFS:
					VfsDebug(LOG_DEBUG, "mount FS_TYPE_LITTLEFS\r\n");
					break;
				
				case FS_TYPE_CONSTFS:
					VfsDebug(LOG_DEBUG, "mount FS_TYPE_CONSTFS\r\n");
					break;
				
				default:
					VfsDebug(LOG_DEBUG, "mount err: unkown fs\r\n");
					break;
			}

			break;
		}
		
		i++;
	}
	return -1;
}

/*
**Description:      ��ѯ�ļ�ϵͳ������Ŀ¼�ṹָ��     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
*/
static FsNode *vfs_find_dir(u8* dirname)
{
	u8 i = 0;
	
    VfsDebug(LOG_DEBUG, "find dir %s\r\n", dirname);

	while(1)
	{
		if(i>= SYS_FS_NUM)
		{
			VfsDebug(LOG_DEBUG, "scan dir err:no dir %s\r\n", dirname);
			return NULL;
		}
		
		if(VfsNodeList[i].Mtd != NULL)
		{
			if(0 == strcmp(VfsNodeList[i].Mtd->name, dirname))
        	{
            	VfsDebug(LOG_DEBUG, "�ҵ���ӦĿ¼ %s\r\n", dirname);
        	    return &VfsNodeList[i];
       		}	
		}
		i++;
	}
	
}

int vfs_creat(const char *filename, int mode)
{
	return -1;	
}
    

int vfs_open(const char *pathname, int oflags)
{
	u8 dir[VFS_DIR_LEN];
    u8 name[VFS_NAME_LEN];
	FsNode *fsnode;
	FileNode *filenode;
	
	vfs_get_dir_name(pathname, &dir[0], &name[0]);

	fsnode = vfs_find_dir(&dir[0]);
	if(fsnode == NULL)
		return NULL;

	filenode = wjq_malloc(sizeof(FileNode));
	if(filenode == NULL)
		return NULL;
	filenode->fsnode = fsnode;
	
	switch(filenode->fsnode->Mtd->type)
	{
		case FS_TYPE_FATFS:
			VfsDebug(LOG_DEBUG, "open FS_TYPE_FATFS\r\n");
			#ifdef SYS_FS_FATFS
			FRESULT res;
			BYTE mode;
			/*need fix*/
			if(oflags == O_RDONLY)
				mode = FA_READ;
			else if(oflags == O_CREAT)
				mode = FA_CREATE_ALWAYS | FA_WRITE;
			
			res = f_open(&filenode->pra.fatfd, &name[0], mode);
			if(res == FR_OK)
			{
				wjq_log(LOG_INFO, "open file :%s ok!\r\n", name);	
				return (int)filenode;
				
			}
			else
				#endif
			{
				wjq_log(LOG_INFO, "open file:%s, err\r\n", name);	
			}
			return NULL;

			
		case FS_TYPE_SPIFFS:
			VfsDebug(LOG_DEBUG, "open FS_TYPE_SPIFFS\r\n");
			break;
		
		case FS_TYPE_LITTLEFS:
			VfsDebug(LOG_DEBUG, "open FS_TYPE_LITTLEFS\r\n");
			break;
		
		case FS_TYPE_CONSTFS:
			VfsDebug(LOG_DEBUG, "open FS_TYPE_CONSTFS\r\n");
			break;
		
		default:
			VfsDebug(LOG_DEBUG, "open err: unkown fs\r\n");
			break;
	}
	return NULL;
}

int vfs_read(int fd, const void *buf, size_t length)
{
	FileNode *filenode;
	unsigned int len;
		
	if(fd == NULL)
		return -1;
	
	filenode = (FileNode *)fd;
	switch(filenode->fsnode->Mtd->type)
	{
		case FS_TYPE_FATFS:
			VfsDebug(LOG_DEBUG, "vfs_read FS_TYPE_FATFS\r\n");
			
			#ifdef SYS_FS_FATFS
			FRESULT res;
		
			res = f_read(&filenode->pra.fatfd, (void *)buf, length, &len);
			if((res != FR_OK) || (len != length))
			{
				VfsDebug(LOG_DEBUG, "read err\r\n");
				return -1;
			}

			return (int)len;
			#else
			return -1;
			#endif 

			
		case FS_TYPE_SPIFFS:
			VfsDebug(LOG_DEBUG, "vfs_read FS_TYPE_SPIFFS\r\n");
			break;
		
		case FS_TYPE_LITTLEFS:
			VfsDebug(LOG_DEBUG, "vfs_read FS_TYPE_LITTLEFS\r\n");
			break;
		
		case FS_TYPE_CONSTFS:
			VfsDebug(LOG_DEBUG, "vfs_read FS_TYPE_CONSTFS\r\n");
			break;
		
		default:
			VfsDebug(LOG_DEBUG, "vfs_read err: unkown fs\r\n");
			break;
	}
	return NULL;
}

int vfs_write(int fd, const void *buf, size_t length)
{
	/*need fix*/
	return -1;
}

int vfs_lseek(int fd, int offset, int whence)
{
	FileNode *filenode;
	unsigned int len;
		
	if(fd == NULL)
		return -1;
	
	filenode = (FileNode *)fd;
	switch(filenode->fsnode->Mtd->type)
	{
		case FS_TYPE_FATFS:
			VfsDebug(LOG_DEBUG, "vfs_lseek FS_TYPE_FATFS\r\n");
			#ifdef SYS_FS_FATFS
			FRESULT res;
			res = f_lseek(&filenode->pra.fatfd, offset);
			if(res != FR_OK)
			{
				return 0;
			}
			#else 	
			return -1;
			#endif 
			break;
			
		case FS_TYPE_SPIFFS:
			VfsDebug(LOG_DEBUG, "vfs_lseek FS_TYPE_SPIFFS\r\n");
			break;
		
		case FS_TYPE_LITTLEFS:
			VfsDebug(LOG_DEBUG, "vfs_lseek FS_TYPE_LITTLEFS\r\n");
			break;
		
		case FS_TYPE_CONSTFS:
			VfsDebug(LOG_DEBUG, "vfs_lseek FS_TYPE_CONSTFS\r\n");
			break;
		
		default:
			VfsDebug(LOG_DEBUG, "vfs_lseek err: unkown fs\r\n");
			break;
	}
	return NULL;
}

int vfs_close(int fd)
{
	FileNode *filenode;
	unsigned int len;
		
	if(fd == NULL)
		return -1;
	
	filenode = (FileNode *)fd;
	switch(filenode->fsnode->Mtd->type)
	{
		case FS_TYPE_FATFS:
			VfsDebug(LOG_DEBUG, "vfs_close FS_TYPE_FATFS\r\n");
		#ifdef SYS_FS_FATFS
			FRESULT res;
			res = f_close(&filenode->pra.fatfd);
			if(res != FR_OK)
			{
				return 0;
			}
		#else 	
			return -1;
		#endif 
			break;
			
		case FS_TYPE_SPIFFS:
			VfsDebug(LOG_DEBUG, "vfs_close FS_TYPE_SPIFFS\r\n");
			break;
		
		case FS_TYPE_LITTLEFS:
			VfsDebug(LOG_DEBUG, "vfs_close FS_TYPE_LITTLEFS\r\n");
			break;
		
		case FS_TYPE_CONSTFS:
			VfsDebug(LOG_DEBUG, "vfs_close FS_TYPE_CONSTFS\r\n");
			break;
		
		default:
			VfsDebug(LOG_DEBUG, "vfs_close err: unkown fs\r\n");
			break;
	}
	return NULL;
}



