#ifndef _NTFS_MISC_H_
#define _NTFS_MISC_H_

#include <stdlib.h>
#include <pkgconf/fs_ntfs.h>

#include "CYGCFG_FLAGS_DEF.h"

extern void *__ntfs_malloc(size_t size, const char *file, const char* line);
extern void *__ntfs_calloc(size_t size, const char *file, const char* line);

//#define CYGCFG_FS_NTFS_MAXMEM_TRACK

#ifdef CYGCFG_FS_NTFS_MAXMEM_TRACK
#define ntfs_calloc(_size) \
        __ntfs_calloc(_size, __FUNCTION__, __FUNCTION__);   \
   
#define ntfs_malloc(_size) \
        __ntfs_malloc(_size, __FUNCTION__, __FUNCTION__);   \

#else
#define ntfs_calloc(_size) \
        __ntfs_calloc(_size, NULL, 0);   \
   
#define ntfs_malloc(_size) \
        __ntfs_malloc(_size, NULL, 0);   \

#endif


#endif /* _NTFS_MISC_H_ */

