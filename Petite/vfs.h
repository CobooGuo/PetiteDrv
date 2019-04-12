
#ifndef _FS_VS_H_
#define _FS_VS_H_

#define SEEK_SET 0 //��ƫ��������Ϊ�����ļ�offset���ֽڴ�
#define SEEK_CUR 1 //��ǰƫ������offset,offset��Ϊ����
#define SEEK_END 2 //����Ϊ�ļ����ȼ�offset,offset��Ϊ����

#define VFS_DIR_LEN  8//Ŀ¼���ȣ�һ�㶼��mtd0 mtd1
#define VFS_NAME_LEN 30//�ļ�������

typedef enum 
{
	FS_TYPE_FATFS = 1,
	FS_TYPE_SPIFFS,
	FS_TYPE_LITTLEFS,
	FS_TYPE_CONSTFS
}FS_TYPE;
/*
	vfs Ŀ¼��
	Ҳ����vfs mount��Ҫ����Ϣ
*/
typedef struct _strMtdPra
{
    u8 name[8];     //�̷���
    FS_TYPE type;       //�ļ�ϵͳ����
    
}VFSDIR;

#define O_RDONLY 	1
#define O_WRONLY	2
#define O_RDWR		3
#define O_APPEND	4
#define O_CREAT		5
#define O_EXTC		6
#define O_NOBLOCK	7
#define O_TRUNC		8

extern void vfs_init(void);
extern int vfs_mount(const VFSDIR *Mtd);
extern int vfs_creat(const char *filename, int mode);
extern int vfs_open(const char *pathname, int oflags);
extern int vfs_read(int fd, const void *buf, size_t length);
extern int vfs_write(int fd, const void *buf, size_t length);
extern int vfs_lseek(int fd, int offset, int whence);
extern int vfs_close(int fd);

#endif

