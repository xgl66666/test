#ifndef __NTFS_H__
#define __NTFS_H__

#include "inode.h"

#define UPCASE_LEN 65536
#define NTFS_HASH_TABLE_SIZE 256
#define NTFS_FREE_BIT_NON_INIT (1LL << 63)

typedef enum {
	ATIME_ENABLED,
	ATIME_DISABLED,
	ATIME_RELATIVE
} ntfs_atime_t;

typedef struct{
    u32     num;                            // Number of nodes
    cyg_drv_mutex_t ntfs_ihash_lock;
    ntfs_inode *inodes[NTFS_HASH_TABLE_SIZE]; // Nodes slots
} ntfs_hash_table_t;

typedef struct{
	struct dirent *dent;
	struct CYG_FILE_TAG *fp;
	cyg_uint32	f_EntSize;
} ntfs_fill_context_t;

extern int ntfs_add_ihash (ntfs_hash_table_t *ihash, ntfs_inode *ni,cyg_drv_mutex_t *mutex);

extern bool ntfs_del_ihash(ntfs_hash_table_t *ihash, ntfs_inode *ni,cyg_drv_mutex_t *mutex);

extern ntfs_inode *ntfs_get_ihash(ntfs_hash_table_t *ihash, u64 inum,cyg_drv_mutex_t *mutex);

#endif
