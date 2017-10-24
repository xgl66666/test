
#ifndef __API_MS_MW_COMMON_H__
#define __API_MS_MW_COMMON_H__

typedef void* (*MsMW_Alloc)(int size);
typedef void (*MsMW_Free)(void* pBuf);
typedef unsigned long (*MsMW_VA2PA)(unsigned long);
typedef unsigned long (*MsMW_PA2VA)(unsigned long, int bCached);

#endif // __API_MS_MW_COMMON_H__


