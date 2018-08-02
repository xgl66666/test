/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

/****************************************************************************

	Abstract:

***************************************************************************/
#ifndef __RTMP_ECOS_TYPE_H_
#define __RTMP_ECOS_TYPE_H_
/* Put platform dependent declaration here */
/* For example, linux type definition */
typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned int			UINT32;
typedef unsigned long long		UINT64;
typedef int					INT32;
typedef short					INT16;
typedef long long 				INT64;
typedef long long    loff_t;
#define RTMP_SYS_IO_READ32(_R, _pV)		\
{										\
	(*_pV = HAL_REG32(_R));				\
}

#define RTMP_SYS_IO_WRITE32(_R, _V)		\
{										\
	HAL_REG32((_R)) = (_V);			\
}
typedef unsigned char			UCHAR;
typedef unsigned short			USHORT;
typedef unsigned int			UINT;
typedef unsigned long			ULONG;
typedef unsigned char *		PUINT8;
typedef unsigned short *		PUINT16;
typedef unsigned int *			PUINT32;
typedef unsigned long long *	PUINT64;
typedef int *					PINT32;
typedef long long * 			PINT64;
/* modified for fixing compile warning on Sigma 8634 platform */
typedef char 					STRING;
typedef signed char			CHAR;
typedef signed short			SHORT;
typedef signed int				INT;
typedef signed long			LONG;
typedef signed long long		LONGLONG;	

typedef unsigned long long		ULONGLONG;
 
typedef unsigned char			BOOLEAN;
//typedef void					VOID;
typedef char *				PSTRING;
typedef void *				PVOID;
typedef CHAR *				PCHAR;
typedef UCHAR * 				PUCHAR;
typedef USHORT *			PUSHORT;
typedef LONG *				PLONG;
typedef ULONG *				PULONG;
typedef UINT *				PUINT;
#if 1
typedef UINT8               __u8;
typedef UINT16              __u16;
typedef UINT32              __u32;
typedef UINT64              __u64;
typedef signed char         __s8;
typedef INT16               __s16;
typedef INT32               __s32;
typedef INT64               __s64;
typedef unsigned char u8;	/* Unsigned types of an exact size */
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;		/* Signed types of an exact size */
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

#endif
#endif /* __RTMP_ECOS_TYPE_H_ */
