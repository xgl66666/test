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
 ***************************************************************************

    Module Name:
	rt_ecos.h

    Abstract:

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
*/

#ifndef __RT_ECOS_H__
#define __RT_ECOS_H__

#define LLTD_SUPPORT 1

#include <stdio.h>
#include <stdlib.h>
#include <cyg/infra/diag.h>     /* For dial_printf */
#include <cyg/hal/hal_if.h>     /* For CYGACC_CALL_IF_DELAY_US */
//#include <cyg/hal/hal_cache.h>  /* For HAL_DCACHE_INVALIDATE */
#include <cyg/kernel/kapi.h>
#include <cyg/io/eth/netdev.h>  /* For struct eth_drv_sc */
#include <cyg/io/eth/eth_drv.h> /* For eth_drv_netdev */
#include <cyg/io/flash.h>       /* For SPI Flash */
#include <sys/mbuf.h>           /* Memory Pool */
//#include <sys/malloc.h>
#include <sys/param.h>          /* For tsleep */
#include <stdarg.h>
#include <sys/sockio.h>
#include <pkgconf/system.h>
#ifdef BRANCH_ADV
#undef CONFIG_BRIDGE
#include <autoconf.h>
#include <cfg_def.h>
#endif
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "rtmp_type.h"

#ifdef RTMP_USB_SUPPORT
//#include <usb/usb.h>
#include <drvUSBHost.h>
#endif
#define IFNAMSIZ            16
#include "os/wireless.h"
/*#include "rtmp_def.h" */


/************************************************
* for Mstar ,the struct urb fields have a different name
************************************************/
//#define completion  stCompletion
#define rtusb_urb_context  pContext
#define rtusb_urb_status   s32Status
#define actual_length		u32ActualLength
#define transfer_dma	 	tTransferDma
#define transfer_flags		u32TransferFlags
#define context				pContext
#define status				s32Status





//typedef struct stCompletion  struct completion;
#undef AP_WSC_INCLUDED
#undef STA_WSC_INCLUDED
#undef WSC_INCLUDED

#ifdef CONFIG_AP_SUPPORT
#ifdef WSC_AP_SUPPORT
#define AP_WSC_INCLUDED
#endif /* WSC_AP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
#ifdef WSC_STA_SUPPORT
#define STA_WSC_INCLUDED
#endif /* WSC_STA_SUPPORT */
#endif /* CONFIG_STA_SUPPORT */

#if defined(WSC_AP_SUPPORT) || defined(WSC_STA_SUPPORT)
#define WSC_INCLUDED
#endif

#ifdef KTHREAD_SUPPORT
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,4)
#error "This kerne version doesn't support kthread!!"
#endif
#endif /* KTHREAD_SUPPORT */

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifdef RTMP_USB_SUPPORT // os abl move
typedef struct usb_device	*PUSB_DEV;
typedef struct urb *purbb_t;
typedef struct usb_ctrlrequest devctrlrequest;
#endif
#ifdef RTMP_USB_SUPPORT
#define RTMP_USB_CONTROL_MSG_ENODEV		(-ENODEV)
#define RTMP_USB_CONTROL_MSG_FAIL		(-EFAULT)
#endif /* RTMP_USB_SUPPORT */
#ifdef RTMP_MAC_USB
#define RTMP_DRV_NAME	"rt2870"
#else
#define RTMP_DRV_NAME	"rt2860"
#endif /* RTMP_MAC_USB */

/***********************************************************************************
 *	Profile related sections
 ***********************************************************************************/
#ifdef CONFIG_AP_SUPPORT
#ifdef RTMP_MAC_USB
#ifdef INF_AMAZON_SE
#define AP_PROFILE_PATH                 "/ramdisk/etc/Wireless/RT2870AP/RT2870AP.dat"
#define AP_RTMP_FIRMWARE_FILE_NAME "/ramdisk/etc/Wireless/RT2870AP/RT2870AP.bin"
#else
#define AP_PROFILE_PATH			"/etc/Wireless/RT2870AP/RT2870AP.dat"
#define AP_RTMP_FIRMWARE_FILE_NAME "/etc/Wireless/RT2870AP/RT2870AP.bin"
#endif
#define AP_NIC_DEVICE_NAME			"RT2870AP"
#define AP_DRIVER_VERSION			"3.0.0.0"
#ifdef MULTIPLE_CARD_SUPPORT
#define CARD_INFO_PATH			"/etc/Wireless/RT2870AP/RT2870APCard.dat"
#endif /* MULTIPLE_CARD_SUPPORT */
#endif /* RTMP_MAC_USB */


#ifdef SINGLE_SKU_V2
#define SINGLE_SKU_TABLE_FILE_NAME	"/etc/Wireless/RT2860STA/SingleSKU.dat"
#endif /* SINGLE_SKU_V2 */

#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT

#ifdef RTMP_MAC_USB
#define STA_PROFILE_PATH			"/etc/Wireless/RT2870STA/RT2870STA.dat"
#define STA_DRIVER_VERSION			"3.0.0.4"
#ifdef MULTIPLE_CARD_SUPPORT
#define CARD_INFO_PATH			"/etc/Wireless/RT2870STA/RT2870STACard.dat"
#endif /* MULTIPLE_CARD_SUPPORT */
#endif /* RTMP_MAC_USB */


extern	const struct iw_handler_def rt28xx_iw_handler_def;

#ifdef SINGLE_SKU_V2
#define SINGLE_SKU_TABLE_FILE_NAME	"/etc/Wireless/RT2870STA/SingleSKU.dat"
#endif /* SINGLE_SKU_V2 */

#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_APSTA_MIXED_SUPPORT
extern	const struct iw_handler_def rt28xx_ap_iw_handler_def;
#endif /* CONFIG_APSTA_MIXED_SUPPORT */

/***********************************************************************************
 *	Compiler related definitions
 ***********************************************************************************/
#undef __inline
#define __inline		static inline
#define IN
#define OUT
#define INOUT
#define NDIS_STATUS		INT


/***********************************************************************************
 *	OS Specific definitions and data structures
 ***********************************************************************************/
#define RTMP_INTERRUPT_INIC  CYGNUM_HAL_INTERRUPT_IP5
typedef unsigned int		ra_dma_addr_t;
typedef struct eth_drv_sc	*PNET_DEV;
typedef void				*PNDIS_PACKET;
typedef char				NDIS_PACKET;
typedef PNDIS_PACKET		*PPNDIS_PACKET;
typedef	ra_dma_addr_t			NDIS_PHYSICAL_ADDRESS;
typedef	ra_dma_addr_t			*PNDIS_PHYSICAL_ADDRESS;
typedef void				*NDIS_HANDLE;
typedef char 				*PNDIS_BUFFER;

typedef struct ifreq		NET_IOCTL;
typedef struct ifreq		* PNET_IOCTL;
typedef int 				RTMP_OS_PID;
typedef cyg_sem_t	        RTMP_OS_SEM;
typedef VOID					NET_DEV_STATS;

#undef CYGARC_UNCACHED_ADDRESS
#undef CYGARC_PHYSICAL_ADDRESS
#undef CYGARC_CACHED_ADDRESS

#define CYGARC_PHYSICAL_ADDRESS(x)  x
#define CYGARC_UNCACHED_ADDRESS(x)  x
#define CYGARC_CACHED_ADDRESS(x)  x

#ifdef RTMP_MAC_PCI
#ifndef PCI_DEVICE
#define PCI_DEVICE(vend,dev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID
#endif /* PCI_DEVICE */
#if WIRELESS_EXT >= 12
/* This function will be called when query /proc */
struct iw_statistics *rt28xx_get_wireless_stats(
    IN struct net_device *net_dev);
#endif
#endif /* RTMP_MAC_PCI */

#define RT_MOD_DEC_USE_COUNT()	do{}while(0)
#define RT_MOD_INC_USE_COUNT()	do{}while(0)

#define RTMP_INC_REF(_A)		0
#define RTMP_DEC_REF(_A)		0
#define RTMP_GET_REF(_A)		0

#define isxdigit(_char)  \
	(('0' <= (_char) && (_char) <= '9') ||\
	 ('a' <= (_char) && (_char) <= 'f') || \
	 ('A' <= (_char) && (_char) <= 'F') \
	)

#define isdigit(ch) \
	((ch >= '0') && (ch <= '9'))


#define tolower(ch) ((ch >= 'A' && (ch) <= 'Z') ? (ch) - ('A' - 'a') : (ch))

#define toupper(ch) ((ch >= 'a' && (ch) <= 'z') ? (ch) + ('A' - 'a') : (ch))

#define IWEVCUSTOM	0x8C02		/* Driver specific ascii string */
#define IW_PRIV_SIZE_MASK       0x07FF  /* Max number of those args */





/* Maximum size of returned data, sync with wireless.h in Linux */
#define IW_SCAN_MAX_DATA        4096    /* In bytes */

/*
 *	Data unalignment handling related definitions and data structure
 */
extern void bad_unaligned_access_length(void) __attribute__((noreturn));

struct UINT64 { UINT64 x __attribute__((packed)); };
struct UINT32 { UINT32 x __attribute__((packed)); };
struct UINT16 { UINT16 x __attribute__((packed)); };

/*
 * Elemental unaligned loads
 */
static inline UINT64 __uldq(const UINT64 *addr)
{
	const struct UINT64 *ptr = (const struct UINT64 *) addr;
	return ptr->x;
}

static inline UINT32 __uldl(const UINT32 *addr)
{
	const struct UINT32 *ptr = (const struct UINT32 *) addr;
	return ptr->x;
}



/*
 * Elemental unaligned stores
 */
static inline void __ustq(UINT64 val, UINT64 *addr)
{
	struct UINT64 *ptr = (struct UINT64 *) addr;
	ptr->x = val;
}

static inline void __ustl(UINT32 val, UINT32 *addr)
{
	struct UINT32 *ptr = (struct UINT32 *) addr;
	ptr->x = val;
}

static inline void __ustw(UINT16 val, UINT16 *addr)
{
	struct UINT16 *ptr = (struct UINT16 *) addr;
	ptr->x = val;
}

static inline USHORT __uldw(const UINT16 *addr)
{
	const struct UINT16 *ptr = (const struct UINT16 *) addr;
	return ptr->x;
}

#define __get_unaligned(ptr, size) ({		\
	const void *__gu_p = ptr;		\
	__typeof__(*(ptr)) val;			\
	switch (size) {				\
	case 1:					\
		val = *(const UINT8 *)__gu_p;	\
		break;				\
	case 2:					\
		val = __uldw(__gu_p);		\
		break;				\
	case 4:					\
		val = __uldl(__gu_p);		\
		break;				\
	case 8:					\
		val = __uldq(__gu_p);		\
		break;				\
	default:				\
		bad_unaligned_access_length();	\
	};					\
	val;					\
})

#define __put_unaligned(val, ptr, size)		\
do {						\
	void *__gu_p = ptr;			\
	switch (size) {				\
	case 1:					\
		*(UINT8 *)__gu_p = val;		\
	        break;				\
	case 2:					\
		__ustw(val, __gu_p);		\
		break;				\
	case 4:					\
		__ustl(val, __gu_p);		\
		break;				\
	case 8:					\
		__ustq(val, __gu_p);		\
		break;				\
	default:				\
	    	bad_unaligned_access_length();	\
	};					\
} while(0)

#define get_unaligned(ptr) \
	__get_unaligned((ptr), sizeof(*(ptr)))
#if 0
#define put_unaligned(x,ptr) \
	__put_unaligned((UINT64)(x), (ptr), sizeof(*(ptr)))
#endif

/***********************************************************************************
 *	Network related constant definitions
 ***********************************************************************************/
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#define ETH_P_IPV6	0x86DD
#define ETH_P_IP		0x0800

#define ETH_LENGTH_OF_ADDRESS		6

#define NDIS_STATUS_SUCCESS			0x00
#define NDIS_STATUS_FAILURE			0x01
#define NDIS_STATUS_INVALID_DATA		0x02
#define NDIS_STATUS_RESOURCES			0x03

#define NDIS_SET_PACKET_STATUS(_p, _status)		do{} while(0)
#define NdisWriteErrorLogEntry(_a, _b, _c, _d)		do{} while(0)

/* statistics counter */
#define STATS_INC_RX_PACKETS(_pAd, _dev)
#define STATS_INC_TX_PACKETS(_pAd, _dev)

#define STATS_INC_RX_BYTESS(_pAd, _dev, len)
#define STATS_INC_TX_BYTESS(_pAd, _dev, len)

#define STATS_INC_RX_ERRORS(_pAd, _dev)
#define STATS_INC_TX_ERRORS(_pAd, _dev)

#define STATS_INC_RX_DROPPED(_pAd, _dev)
#define STATS_INC_TX_DROPPED(_pAd, _dev)


/***********************************************************************************
 *	Ralink Specific network related constant definitions
 ***********************************************************************************/
#define MIN_NET_DEVICE_FOR_MBSSID		0x00		/*0x00,0x10,0x20,0x30 */
#define MIN_NET_DEVICE_FOR_WDS			0x10		/*0x40,0x50,0x60,0x70 */
#define MIN_NET_DEVICE_FOR_APCLI		0x20
#define MIN_NET_DEVICE_FOR_MESH			0x30
#ifdef CONFIG_STA_SUPPORT
#define MIN_NET_DEVICE_FOR_DLS			0x40
#define MIN_NET_DEVICE_FOR_TDLS			0x50
#endif /* CONFIG_STA_SUPPORT */
#ifdef P2P_SUPPORT
#define MIN_NET_DEVICE_FOR_P2P_CLI		(MIN_NET_DEVICE_FOR_TDLS + 0x10)
#define MIN_NET_DEVICE_FOR_P2P_GO			(MIN_NET_DEVICE_FOR_TDLS + 0x20)
#endif /* P2P_SUPPORT */

#define NET_DEVICE_REAL_IDX_MASK		0x0f		/* for each operation mode, we maximum support 15 entities. */

#define RT_DEV_PRIV_FLAGS_GET(__pDev)									\
	((PNET_DEV)__pDev)->priv_flags

#define RT_DEV_PRIV_FLAGS_SET(__pDev, __PrivFlags)						\
	((PNET_DEV)__pDev)->priv_flags = __PrivFlags
#ifdef CONFIG_STA_SUPPORT
#define NDIS_PACKET_TYPE_DIRECTED		0
#define NDIS_PACKET_TYPE_MULTICAST		1
#define NDIS_PACKET_TYPE_BROADCAST		2
#define NDIS_PACKET_TYPE_ALL_MULTICAST	3
#define NDIS_PACKET_TYPE_PROMISCUOUS	4
#endif /* CONFIG_STA_SUPPORT */



/***********************************************************************************
 *	OS signaling related constant definitions
 ***********************************************************************************/
/*EddyTODO */
#ifdef CONFIG_AP_SUPPORT
#ifdef IAPP_SUPPORT
typedef struct _RT_SIGNAL_STRUC {
	unsigned short					Sequence;
	unsigned char					MacAddr[ETH_LENGTH_OF_ADDRESS];
	unsigned char					CurrAPAddr[ETH_LENGTH_OF_ADDRESS];
	unsigned char					Sig;
} RT_SIGNAL_STRUC, *PRT_SIGNAL_STRUC;

/* definition of signal */
#define	SIG_NONE					0
#define SIG_ASSOCIATION				1
#define SIG_REASSOCIATION			2
#endif /* IAPP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

/***********************************************************************************
 *	OS file operation related data structure definitions
 ***********************************************************************************/
/*EddyTODO */
typedef int RTMP_OS_FD;

typedef struct _RTMP_OS_FS_INFO_
{
	int	fsinfo; /* Dummy structure, for cross-platform compatibility */
}RTMP_OS_FS_INFO;

#define IS_FILE_OPEN_ERR(_fd) 	((_fd) < 0)

/***********************************************************************************
 *	OS semaphore related data structure and definitions
 ***********************************************************************************/

typedef cyg_spinlock_t	   OS_NDIS_SPIN_LOCK;
/* */
/*  spin_lock enhanced for Nested spin lock */
/* */
#define OS_NdisAllocateSpinLock(__lock)			\
	cyg_spinlock_init((cyg_spinlock_t *)(__lock), 0);

#define OS_NdisFreeSpinLock(__lock)				\
	cyg_spinlock_destroy((cyg_spinlock_t *)(__lock))

#define OS_SEM_LOCK(__lock)						\
{												\
	cyg_spinlock_spin((cyg_spinlock_t *)(__lock));		\
}

#define OS_SEM_UNLOCK(__lock)					\
{												\
	cyg_spinlock_clear((cyg_spinlock_t *)(__lock)); 	\
}
#define RTMP_Spinlock_LOCK(__lock)	\
	do{\
		cyg_scheduler_lock();\
		}while(0)

#define RTMP_Spinlock_UNLOCK(__lock)	\
	do{\
		cyg_scheduler_unlock();\
		}while(0)


/* sample, use semaphore lock to replace IRQ lock, 2007/11/15 */
#define OS_IRQ_LOCK(__lock, __irqflags)			\
{											\
	__irqflags = 0;							\
	cyg_scheduler_lock();				\
}

#define OS_IRQ_UNLOCK(__lock, __irqflag)			\
{											\
	cyg_scheduler_unlock();				\
}

#define OS_INT_LOCK(__lock, __irqflags)	\
{											\
    HAL_DISABLE_INTERRUPTS(__irqflags);     \
}

#define OS_INT_UNLOCK(__lock, __irqflag)	\
{											\
    HAL_RESTORE_INTERRUPTS(__irqflag);      \
}
#define NDIS_SPIN_LOCK							OS_NDIS_SPIN_LOCK
#define NdisAllocateSpinLock(__pReserved, __pLock)	OS_NdisAllocateSpinLock(__pLock)
#define NdisFreeSpinLock						OS_NdisFreeSpinLock
#define RTMP_IRQ_LOCK							OS_IRQ_LOCK
#define RTMP_IRQ_UNLOCK							OS_IRQ_UNLOCK
#define RTMP_INT_LOCK							OS_INT_LOCK
#define RTMP_INT_UNLOCK							OS_INT_UNLOCK

#define RTMP_SEM_LOCK 			RTMP_Spinlock_LOCK
#define RTMP_SEM_UNLOCK 		RTMP_Spinlock_UNLOCK
#define NdisAcquireSpinLock		RTMP_Spinlock_LOCK
#define NdisReleaseSpinLock		RTMP_Spinlock_UNLOCK
#define RtmpMLMEUp								OS_RTMP_MlmeUp
struct completion
{
	unsigned int done;
	cyg_sem_t    wait;
};

typedef int		wait_queue_head_t;
typedef struct completion RTMP_OS_COMPLETION;
/*
	Following lock/unlock definition used for BBP/RF register read/write.
	Currently we don't use it to protect MAC register access.

	For USB:
			we use binary semaphore to do the protection because all register
			access done in kernel thread and should allow task go sleep when
			in protected status.

	For PCI/PCI-E/RBUS:
			We use interrupt to do the protection because the register may accessed
			in thread/tasklet/timer/inteerupt, so we use interrupt_disable to protect
			the access.
*/
#define RTMP_MCU_RW_LOCK(_pAd, _irqflags)	\
	do{								\
		if (_pAd->infType == RTMP_DEV_INF_USB)	\
		{\
			RTMP_SEM_EVENT_WAIT(&_pAd->McuCmdSem, _irqflags);\
		}\
		else\
		{\
			RTMP_SEM_LOCK(&_pAd->McuCmdLock, _irqflags);\
		}\
	}while(0)

#define RTMP_MCU_RW_UNLOCK(_pAd, _irqflags)	\
	do{				\
		if(_pAd->infType == RTMP_DEV_INF_USB)\
		{	\
			RTMP_SEM_EVENT_UP(&_pAd->McuCmdSem);\
		}		\
		else\
		{\
			RTMP_SEM_UNLOCK(&_pAd->McuCmdLock, _irqflags);\
		}\
	}while(0)
#ifndef wait_event_interruptible_timeout
#define __wait_event_interruptible_timeout(wq, condition, ret) \
do { \
        wait_queue_t __wait; \
        init_waitqueue_entry(&__wait, current); \
        add_wait_queue(&wq, &__wait); \
        for (;;) { \
                set_current_state(TASK_INTERRUPTIBLE); \
                if (condition) \
                        break; \
                if (!signal_pending(current)) { \
                        ret = schedule_timeout(ret); \
                        if (!ret) \
                                break; \
                        continue; \
                } \
                ret = -ERESTARTSYS; \
                break; \
        } \
        current->state = TASK_RUNNING; \
        remove_wait_queue(&wq, &__wait); \
} while (0)

#define wait_event_interruptible_timeout(wq, condition, timeout) \
({ \
        long __ret = timeout; \
        if (!(condition)) \
                __wait_event_interruptible_timeout(wq, condition, __ret); \
        __ret; \
})
#endif

#define RTMP_SEM_EVENT_INIT_LOCKED(_pSema, __pSemaList)	\
	cyg_semaphore_init(_pSema, 1);      \
    cyg_semaphore_wait(_pSema);

#define RTMP_SEM_EVENT_INIT(_pSema, __pSemaList)			\
	cyg_semaphore_init(_pSema, 1);

#define RTMP_SEM_EVENT_DESTORY(_pSema)
#define RTMP_SEM_EVENT_WAIT(_pSema, _status)	\
	(_status) = ((~cyg_semaphore_wait(_pSema)) & 0x01);

#define RTMP_SEM_EVENT_UP(_pSema)			\
	cyg_semaphore_post(_pSema)

#define RTMP_OS_TASK_WAKE_UP(__pTask)						\
	RTMP_SEM_EVENT_UP(&(pTask)->taskSema);

#define RTMP_OS_TASK_GET(__pTask)							\
	(__pTask)

#define RTCMDUp					OS_RTCMDUp

/***********************************************************************************
 *	OS Memory Access related data structure and definitions
 ***********************************************************************************/
//#define GFP_ATOMIC          (0)
#define GFP_DMA             (0)
//#define GFP_KERNEL          (0)
/*
#undef kfree
#undef kmalloc
#define kmalloc(_size, _flags)	malloc((_size))

#define kfree(_ptr)             free((_ptr))

#define vmalloc(_size)          malloc((_size))
#define vfree(_ptr)             free((_ptr))
*/


extern void * MsOS_AllocateMemory (MS_U32 u32Size, MS_S32 s32PoolId);
extern MS_BOOL MsOS_FreeMemory (void *pAddress, MS_S32 s32PoolId);
extern MS_S32 ;
extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;


#undef kfree
#undef kmalloc
#define kmalloc(_size, _flags)	MsOS_AllocateMemory((_size),gs32NonCachedPoolID)

#define kfree(_ptr)             MsOS_FreeMemory((_ptr),gs32NonCachedPoolID)

#define vmalloc(_size)          MsOS_AllocateMemory((_size),gs32NonCachedPoolID)
#define vfree(_ptr)             MsOS_FreeMemory((_ptr),gs32NonCachedPoolID)


#define MEM_ALLOC_FLAG          (GFP_NOIO) /*(GFP_DMA | GFP_ATOMIC) */
#define copy_to_user(x,y,z)     (memcpy(x,y,z) == NULL)
#define copy_from_user(x,y,z)   (memcpy(x,y,z) == NULL)

#define NdisMoveMemory(Destination, Source, Length) memmove(Destination, Source, Length)
#define NdisCopyMemory(Destination, Source, Length) memcpy(Destination, Source, Length)
#define NdisZeroMemory(Destination, Length)         memset(Destination, 0, Length)
#define NdisFillMemory(Destination, Length, Fill)   memset(Destination, Fill, Length)
#define NdisCmpMemory(Destination, Source, Length)  memcmp(Destination, Source, Length)
#define NdisEqualMemory(Source1, Source2, Length)   (!memcmp(Source1, Source2, Length))
#define RTMPEqualMemory(Source1, Source2, Length)	(!memcmp(Source1, Source2, Length))
#define COPY_MAC_ADDR(Addr1, Addr2)                 memcpy((Addr1), (Addr2), MAC_ADDR_LEN)

#define MlmeAllocateMemory(_pAd, _ppVA)		os_alloc_mem(_pAd, _ppVA, MGMT_DMA_BUFFER_SIZE)
#define MlmeFreeMemory(_pAd, _pVA)			os_free_mem(_pAd, _pVA)

#define COPY_MAC_ADDR(Addr1, Addr2)             memcpy((Addr1), (Addr2), MAC_ADDR_LEN)


/***********************************************************************************
 *	OS task related data structure and definitions
 ***********************************************************************************/
/*EddyTODO */
typedef	int	THREAD_PID;
#define RTMP_OS_MGMT_TASK_FLAGS	    0
#define	THREAD_PID_INIT_VALUE	    -1
#define RTMP_GET_OS_PID(_x, _pid)   _x = _pid
#define	GET_PID_NUMBER(_v)	        (_v)
#define CHECK_PID_LEGALITY(_pid)	if ((_pid) >= 0)
#define KILL_THREAD_PID(_A, _B, _C)	kill_proc((_A), (_B), (_C))
#ifdef KTHREAD_SUPPORT
#define RTMP_OS_TASK_LEGALITY(__pTask)						\
	if ((__pTask)->kthread_task != NULL)
#else
#define RTMP_OS_TASK_LEGALITY(__pTask)						\
	CHECK_PID_LEGALITY((__pTask)->taskPID)
#endif /* KTHREAD_SUPPORT */

typedef int (*cast_fn)(void *);
typedef void (*PNETFUNC)(ULONG arg1);
typedef INT (*RTMP_OS_TASK_CALLBACK)(IN ULONG Context);

typedef enum _RTMP_NET_TASK_STATUS_
{
	RTMP_NET_TASK_UNKNOWN = 0,
	RTMP_NET_TASK_INITED = 1,
	RTMP_NET_TASK_RUNNING = 2,
	RTMP_NET_TASK_PENDING = 4,
	RTMP_NET_TASK_STOP = 8,
}RTMP_NET_TASK_STATUS;

typedef struct _RTMP_NET_TASK_STRUCT_
{
	PNETFUNC 				funcPtr;
	unsigned long           data;
	unsigned long			pAd;
	RTMP_NET_TASK_STATUS	taskStatus;
	char                    taskName[8];
}RTMP_NET_TASK_STRUCT, *PRTMP_NET_TASK_STRUCT;

#define RTMP_NET_TASK_INIT(_pNetTask, _pFuncPtr, _pData)	\
    do{														\
		(_pNetTask)->funcPtr = (_pFuncPtr);					\
		(_pNetTask)->data = (unsigned int)(_pData);			\
		(_pNetTask)->pAd = (unsigned int)(_pData);			\
		(_pNetTask)->taskStatus = RTMP_NET_TASK_INITED;		\
	}while(0)
#define RTMP_OS_TASKLET_INIT(__pAd, __pTasklet, __pFunc, __Data)	\
					RTMP_NET_TASK_INIT(__pTasklet, __pFunc, __Data)

#define RTMP_NET_TASK_KILL(_pNetTask)	\
    do{														\
		(_pNetTask)->taskStatus = RTMP_NET_TASK_STOP;		\
	}while(0)

#define RTMP_OS_TASKLET_KILL(__pTasklet)	\
			RTMP_NET_TASK_KILL(__pTasklet)

#ifdef ECOS_NETTASK_SCHDULE_NEW

extern volatile UINT32 nettask_schedule;

#define tasklet_hi_schedule(_pTask)										\
					do{ 												\
						unsigned long _retVal = 0;						\
						HAL_DISABLE_INTERRUPTS(_retVal);			    \
						nettask_schedule |= _pTask; 			 		\
						HAL_RESTORE_INTERRUPTS(_retVal);				\
					}while(0)

#else

extern void spl_debug(void);
extern struct eth_drv_sc *sc_wifi;

#define tasklet_hi_schedule(_pTask)					                        						\
	do{																		                        \
		cyg_bool_t _retVal = 0;											                        \
		POS_COOKIE pOSCookie = (POS_COOKIE) ((PRTMP_ADAPTER) ((_pTask)->pAd))->OS_Cookie;             \
        if (pOSCookie->nettask_handle != 0){  \
    		_retVal = cyg_mbox_tryput(pOSCookie->nettask_handle, (void*) (_pTask)); \
		}                   \
		else 																                        \
        	DBGPRINT_ERR(("NetTask(%s) nettask_handle is null!\n", (char *)((_pTask)->taskName)));	\
		if (_retVal == 1){\
			eth_drv_dsr(0,0,sc_wifi);/*wake up alarm thread*/\
			(_pTask)->taskStatus = RTMP_NET_TASK_PENDING;	\
			}\
		else																                        \
        	DBGPRINT_ERR(("NetTask(%s) add failed!\n", (char *)((_pTask)->taskName)));	\
	}while(0)

#endif		//ECOS_NETTASK_SCHDULE_NEW

#define RTMP_OS_TASKLET_SCHE(__pTasklet)							\
		tasklet_hi_schedule(__pTasklet)


/***********************************************************************************
 * Timer related definitions and data structures.
 **********************************************************************************/
//#define HZ				100
#define OS_HZ			HZ

typedef struct timer_list	OS_NDIS_MINIPORT_TIMER;
typedef struct timer_list	OS_TIMER;

typedef void (*FUNCPTR)(void *);

struct RtmpEcosTimer
{
	cyg_handle_t 	counter_hdl;
	cyg_handle_t	alarm_hdl;
	cyg_alarm	alarm_obj;
    BOOLEAN         Valid; /* Set to True when call RTMPInitTimer */
	PVOID		data;
};

typedef struct RtmpEcosTimer	NDIS_MINIPORT_TIMER;
typedef struct RtmpEcosTimer	RTMP_OS_TIMER;
typedef void (*TIMER_FUNCTION) (cyg_handle_t alarm, cyg_addrword_t data);

/* The unit of _time is ticks instead of ms. */
#define OS_WAIT(_time) {int value; tsleep((caddr_t)&value, PWAIT, "os_wait", (_time*HZ/1000));}

#define typecheck(type,x) \
({      type __dummy; \
        typeof(x) __dummy2; \
        (void)(&__dummy == &__dummy2); \
        1; \
})
#define time_after(a,b)         \
        (typecheck(unsigned long, a) && \
         typecheck(unsigned long, b) && \
         ((long)(b) - (long)(a) < 0))
#define time_before(a,b)        time_after(b,a)

#define RTMP_TIME_BEFORE(a,b)	time_before(b,a)
#define RTMP_TIME_AFTER(a,b) 	time_after(a, b)
#undef mdelay
#define mdelay(_us)		        RTMPusecDelay((_us)*1000)

#define ONE_TICK 1

static inline void NdisGetSystemUpTime(ULONG *time)
{
	*time = cyg_current_time();
}

/***********************************************************************************
 * Memory pool & Network packet Structure
 **********************************************************************************/
typedef struct _ECOS_DESC_BUFFER
{
    struct mbuf             *pMBuf;
    PNDIS_PHYSICAL_ADDRESS  PhysicalAddress;
} ECOS_DESC_BUFFER, *PECOS_DESC_BUFFER;

typedef struct _ECOS_PKT_BUFFER
{
	struct _ECOS_PKT_BUFFER *Next;  /* Don't move, for InsertTailQueueAC use */

	/* Internal information */
	INT             MemPoolType;        /* Record from which mempool */
    struct mbuf     *pHeaderMBuf;       /* Pointer to header mbuf */
    struct mbuf     *pDataMBuf;         /* Pointer to data mbuf */

	/* packet information */
	PNET_DEV        net_dev;
	UCHAR           cb[48];

	PUCHAR          pDataPtr;
	LONG            pktLen;
	LONG            ref_cnt;
} ECOS_PKT_BUFFER, *PECOS_PKT_BUFFER;

/***********************************************************************************
 *	OS specific cookie data structure binding to RTMP_ADAPTER
 ***********************************************************************************/
typedef INT (*pWscMsgCallBack)(PUCHAR pData, UINT32	dataLen);

struct os_cookie {
#ifdef RTMP_MAC_USB
	struct usb_device		*pUsb_Dev;
#ifdef CONFIG_STA_SUPPORT
	struct usb_interface *intf;
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_MAC_USB */
#ifdef RTMP_MAC_PCI
	struct pci_dev          *pci_dev;
	struct pci_dev          *parent_pci_dev;
	USHORT                  DeviceID;
	ra_dma_addr_t              pAd_pa;
#endif /* RTMP_MAC_PCI */

	RTMP_NET_TASK_STRUCT    rx_done_task;
    RTMP_NET_TASK_STRUCT    cmd_rsp_event_task;
	RTMP_NET_TASK_STRUCT    mgmt_dma_done_task;
	RTMP_NET_TASK_STRUCT    ac0_dma_done_task;
	RTMP_NET_TASK_STRUCT    ac1_dma_done_task;
	RTMP_NET_TASK_STRUCT    ac2_dma_done_task;
	RTMP_NET_TASK_STRUCT    ac3_dma_done_task;
	RTMP_NET_TASK_STRUCT    hcca_dma_done_task;
	RTMP_NET_TASK_STRUCT    tbtt_task;
#ifdef RTMP_MAC_PCI
	RTMP_NET_TASK_STRUCT    fifo_statistic_full_task;
#endif /* RTMP_MAC_PCI */

#ifdef CONFIG_AP_SUPPORT
#ifdef UAPSD_SUPPORT
	RTMP_NET_TASK_STRUCT    uapsd_eosp_sent_task;
#endif /* UAPSD_SUPPORT */

#ifdef DFS_SUPPORT
	RTMP_NET_TASK_STRUCT    pulse_radar_detect_task;
	RTMP_NET_TASK_STRUCT    width_radar_detect_task;
#endif /* DFS_SUPPORT */

#ifdef CARRIER_DETECTION_SUPPORT
	RTMP_NET_TASK_STRUCT    carrier_sense_task;
#endif /* CARRIER_DETECTION_SUPPORT */

#ifdef NEW_DFS
	RTMP_NET_TASK_STRUCT    dfs_task;
#endif /* NEW_DFS */
#endif /* CONFIG_AP_SUPPORT */
#ifdef RTMP_MAC_USB
	RTMP_NET_TASK_STRUCT null_frame_complete_task;
	RTMP_NET_TASK_STRUCT pspoll_frame_complete_task;
#endif /* RTMP_MAC_USB */

#ifdef CONFIG_MULTI_CHANNEL
	RTMP_NET_TASK_STRUCT	hcca_null_frame_complete_task;
#endif /* CONFIG_MULTI_CHANNEL */
	RTMP_OS_PID             apd_pid; /*802.1x daemon pid */
	unsigned long			apd_pid_nr;
#ifdef CONFIG_AP_SUPPORT
#ifdef IAPP_SUPPORT
	RT_SIGNAL_STRUC			RTSignal;
	RTMP_OS_PID             IappPid; /*IAPP daemon pid */
	unsigned long			IappPid_nr;
#endif /* IAPP_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
#ifdef WAPI_SUPPORT
	RTMP_OS_PID			wapi_pid; /*wapi daemon pid */
	unsigned long			wapi_pid_nr;
#endif /* WAPI_SUPPORT */
	INT						ioctl_if_type;
	INT 					ioctl_if;

    /* message queue handle & box */
    cyg_handle_t            nettask_handle;
    cyg_mbox                nettask_mbox;

#ifdef WSC_AP_SUPPORT
#ifdef WSC_INCLUDED
	pWscMsgCallBack WscMsgCallBack;
#endif /* WSC_INCLUDED */
#endif /* WSC_AP_SUPPORT */
};

typedef struct os_cookie	* POS_COOKIE;



/***********************************************************************************
 *	OS debugging and printing related definitions and data structure
 ***********************************************************************************/
#define printk      diag_printf
#define printf      diag_printf
#define sprintf     diag_sprintf
#define snprintf    diag_snprintf
#define PRINT_MAC(addr)	\
	addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]

#ifdef DBG
extern ULONG RTDebugLevel;
extern ULONG RTDebugFunc;

#ifdef BRANCH_ADV
extern UCHAR    DbgBuffer[2048];
int printUI(const char *fmt, ...);

#define DBGPRINT_RAW(Level, Fmt)    \
do{                                 \
    if (Level <= RTDebugLevel)      \
    {                               \
        printUI Fmt;                 \
    }                               \
}while(0)
#else /* !BRANCH_ADV */
#define DBGPRINT_RAW(Level, Fmt)    \
do{                                 \
    if (Level <= RTDebugLevel)      \
    {                               \
        printk Fmt;                 \
    }                               \
}while(0)
#endif /* BRANCH_ADV */

#define DBGPRINT(Level, Fmt)    DBGPRINT_RAW(Level, Fmt)

#define DBGPRINT_ERR(Fmt)           \
{                                   \
    printk("ERROR!!! ");            \
    printk Fmt;                     \
}

#define DBGPRINT_S(Status, Fmt)     \
{                                   \
	printk Fmt;                     \
}
#else
#define DBGPRINT(Level, Fmt)
#define DBGPRINT_RAW(Level, Fmt)
#define DBGPRINT_S(Status, Fmt)
#define DBGPRINT_ERR(Fmt)
#endif

#undef  ASSERT
#ifdef VENDOR_FEATURE3_SUPPORT
#define ASSERT(x)
#else
#define ASSERT(x)                                   \
{                                                   \
    if (!(x))                                       \
    {                                               \
	printk("%s", __FILE__);   		                \
	printk(":%d assert " #x "failed\n", __LINE__);  \
    }                                               \
}
#endif /* VENDOR_FEATURE3_SUPPORT */

void hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen);

/***********************************************************************************
 * Device DMA Access related definitions and data structures.
 **********************************************************************************/
/* This defines the direction arg to the DMA mapping routines. */
#define PCI_DMA_BIDIRECTIONAL	0
#define PCI_DMA_TODEVICE	    1
#define PCI_DMA_FROMDEVICE	    2
#define PCI_DMA_NONE		    3

ra_dma_addr_t ecos_pci_map_single(void *handle, void *ptr, size_t size, int sd_idx, int direction);
#ifdef RTMP_MAC_PCI
#define PCI_MAP_SINGLE(_handle, _ptr, _size, _sd_idx, _dir) \
    ecos_pci_map_single(_handle, _ptr, _size, _sd_idx, _dir)

#define PCI_UNMAP_SINGLE(_handle, _ptr, _size, _dir) {}
#endif /* RTMP_MAC_PCI */

/*
 * Input: NDIS_PHYSICAL_ADDRESS  PhysicalAddress
 * Input: ULONG  Value
 */
#define RTMP_GetPhysicalAddressLow(PhysicalAddress)         (PhysicalAddress)
#define RTMP_GetPhysicalAddressHigh(PhysicalAddress)        (0)
#define RTMP_SetPhysicalAddressLow(PhysicalAddress, Value)  PhysicalAddress = Value;
#define RTMP_SetPhysicalAddressHigh(PhysicalAddress, Value)

#define NdisMIndicateStatus(_w, _x, _y, _z)

/***********************************************************************************
 * Device Register I/O Access related definitions and data structures.
 **********************************************************************************/
#ifdef RTMP_MAC_USB
#define RTMP_IO_FORCE_READ32(_A, _R, _pV)								\
	RTUSBReadMACRegister((_A), (_R), (PUINT32) (_pV))

#define RTMP_IO_READ32(_A, _R, _pV)								\
	RTUSBReadMACRegister((_A), (_R), (PUINT32) (_pV))

#define RTMP_IO_FORCE_WRITE32(_A, _R, _V)	\
	do{\
		/*if ((_R) != 0x404)*/ /* TODO:shiang-6590, depends on sw porting guide, don't acccess it now */\
			RTUSBWriteMACRegister((_A), (_R), (UINT32) (_V), FALSE);		\
	}while(0)

#define RTMP_IO_READ8(_A, _R, _pV)								\
{																\
}

#define RTMP_IO_WRITE32(_A, _R, _V)								\
	RTUSBWriteMACRegister((_A), (_R), (UINT32) (_V), FALSE)

#define RTMP_IO_WRITE8(_A, _R, _V)								\
{																\
	USHORT	_Val = _V;											\
	RTUSBSingleWrite((_A), (_R), (USHORT) (_Val), FALSE);		\
}


#define RTMP_IO_WRITE16(_A, _R, _V)								\
{																\
	RTUSBSingleWrite((_A), (_R), (USHORT) (_V), FALSE);			\
}

//#define RTMP_SYS_IO_READ32
//#define RTMP_SYS_IO_WRITE32
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_PCI
#define RTMP_IO_READ32(_A, _R, _pV)								    \
{																    \
    if ((_A)->bPCIclkOff == FALSE)                                  \
    {                                                               \
		(*_pV = HAL_REG32(((_A)->CSRBaseAddress + (_R))));			\
    }                                                               \
    else															\
		*_pV = 0;													\
}

#define RTMP_IO_FORCE_READ32(_A, _R, _pV)							\
{																	\
	(*_pV = HAL_REG32(((_A)->CSRBaseAddress + (_R))));			    \
}

#define RTMP_IO_READ8(_A, _R, _pV)								    \
{																    \
	(*_pV = HAL_REG8(((_A)->CSRBaseAddress + (_R))));				\
}

#ifndef VENDOR_FEATURE3_SUPPORT
#define RTMP_IO_WRITE32(_A, _R, _V)									\
{																	\
    if ((_A)->bPCIclkOff == FALSE)                                  \
    {                                                               \
	HAL_REG32(((_A)->CSRBaseAddress + (_R))) = (_V);		        \
    }                                                               \
}
#endif /* VENDOR_FEATURE3_SUPPORT */

#define RTMP_IO_FORCE_WRITE32(_A, _R, _V)							\
{																	\
	HAL_REG32(((_A)->CSRBaseAddress + (_R))) = (_V);		        \
}

#ifdef RTMP_RBUS_SUPPORT
#ifdef NEW_DFS
#define RTMP_DFS_IO_READ32(_R, _pV)		\
{										\
	(*_pV = HAL_REG32((_R)));			\
}
#endif /* NEW_DFS */

#define RTMP_DFS_IO_WRITE32(_R, _V)		\
{										\
    HAL_REG32((_R)) = (_V);             \
}
#endif /* RTMP_RBUS_SUPPORT */

#if defined(RALINK_2880) || defined(RALINK_3052) || defined(RALINK_2883) || defined(RTMP_RBUS_SUPPORT)
#define RTMP_IO_WRITE8(_A, _R, _V)                                  \
{                                                                   \
	ULONG Val;                                                      \
	UCHAR _i;                                                       \
	_i = ((_R) & 0x3);                                              \
	Val = HAL_REG32(((_A)->CSRBaseAddress + ((_R) - _i)));          \
	Val = Val & (~(0x000000ff << ((_i)*8)));                        \
	Val = Val | ((ULONG)(_V) << ((_i)*8));                          \
	HAL_REG32(((_A)->CSRBaseAddress + ((_R) - _i))) = (Val);	    \
}
#else
#define RTMP_IO_WRITE8(_A, _R, _V)							        \
{															        \
	HAL_REG8(((_A)->CSRBaseAddress + (_R))) = (_V);	                \
}
#endif /* defined(RT2880) || defined(RT3052) || defined(RT2883)  */

#define RTMP_IO_WRITE16(_A, _R, _V)							        \
{															        \
	HAL_REG16(((_A)->CSRBaseAddress + (_R))) = (_V);                \
}
#endif /* RTMP_MAC_PCI */
#define RTMP_USB_URB_DATA_GET(__pUrb)			((purbb_t)__pUrb)->context
#define RTMP_USB_URB_STATUS_GET(__pUrb)			((purbb_t)__pUrb)->status
#define RTMP_USB_URB_LEN_GET(__pUrb)			((purbb_t)__pUrb)->actual_length

/*EddyTODO */


/***********************************************************************************
 *	Network Related data structure and marco definitions
 ***********************************************************************************/
typedef struct _RTMP_VX_INIT_STRING_{
	int phyNum;
	int devNum;
	int infType;
	int irqNum;
	int devType;
	int privSize;
	unsigned int csrAddr;
	unsigned char devName[IFNAMSIZ];
}RTMP_VX_INIT_STRING;

#define PKTSRC_NDIS             0x7f
#define PKTSRC_DRIVER           0x0f

#define RTMP_OS_NETDEV_STATE_RUNNING(_pNetDev)	((_pNetDev)->sc_arpcom.ac_if.if_flags & IFF_UP)
#define RTMP_OS_NETDEV_GET_PRIV(_pNetDev)		((_pNetDev)->driver_private)
#define RTMP_OS_NETDEV_SET_PRIV(_pNetDev, _pPriv)	((_pNetDev)->driver_private = (_pPriv))
#define RTMP_OS_NETDEV_GET_DEVNAME(_pNetDev)	((_pNetDev)->dev_name)
#define RTMP_OS_NETDEV_GET_PHYADDR(_pNetDev)	((_pNetDev)->sc_arpcom.ac_enaddr)
#define RTMP_OS_NETDEV_PHYADDR_LEN(_pNetDev)	(MAC_ADDR_LENGTH)

#define RTMP_OS_NETDEV_START_QUEUE(_pNetDev)	do{}while(0)
#define RTMP_OS_NETDEV_STOP_QUEUE(_pNetDev)		do{}while(0)
#define RTMP_OS_NETDEV_WAKE_QUEUE(_pNetDev)		do{}while(0)
#define RTMP_OS_NETDEV_CARRIER_OFF(_pNetDev)	do{}while(0)


#define QUEUE_ENTRY_TO_PACKET(pEntry)   (PNDIS_PACKET)(pEntry)

#define PACKET_TO_QUEUE_ENTRY(pPacket)  (PQUEUE_ENTRY)(pPacket)


#define GET_SG_LIST_FROM_PACKET(_p, _sc)    rt_get_sg_list_from_packet(_p, _sc)

#define RELEASE_NDIS_PACKET(_pAd, _pPacket, _Status)    \
    RTMPFreeNdisPacket(_pAd, _pPacket);

/*
 * packet helper
 * 	- convert internal rt packet to os packet or
 *             os packet to rt packet
 */
#define RTPKT_TO_OSPKT(_p)		((PECOS_PKT_BUFFER)(_p))
#define OSPKT_TO_RTPKT(_p)		((PNDIS_PACKET)(_p))

#define GET_OS_PKT_DATAPTR(_pkt)    (RTPKT_TO_OSPKT(_pkt)->pDataPtr)
#define SET_OS_PKT_DATAPTR(_pkt, _dataPtr)	\
		(RTPKT_TO_OSPKT(_pkt)->pDataPtr) = (_dataPtr)

#define GET_OS_PKT_LEN(_pkt)        (RTPKT_TO_OSPKT(_pkt)->pktLen)
#define SET_OS_PKT_LEN(_pkt, _len)	\
		(RTPKT_TO_OSPKT(_pkt)->pktLen) = (_len)

#define GET_OS_PKT_DATATAIL(_pkt) \
		((RTPKT_TO_OSPKT(_pkt)->pDataPtr) + (RTPKT_TO_OSPKT(_pkt)->pktLen))
#define SET_OS_PKT_DATATAIL(_pkt, _start, _len)	do {}while(0)

#define GET_OS_PKT_NETDEV(_pkt) \
		(RTPKT_TO_OSPKT(_pkt)->net_dev)
#define SET_OS_PKT_NETDEV(_pkt, _pNetDev)	\
		(RTPKT_TO_OSPKT(_pkt)->net_dev) = (PNET_DEV)(_pNetDev)

#define GET_OS_PKT_TYPE(_pkt)   (RTPKT_TO_OSPKT(_pkt))

#define GET_OS_PKT_NEXT(_pkt)   do {}while(0)

#define OS_PKT_CLONE(_pAd, _pkt, _src, _flag)		\
	_src = skb_clone(RTPKT_TO_OSPKT(_pkt), _flag);

#define OS_PKT_CLONED(_pkt)		(((RTPKT_TO_OSPKT(_pkt))->ref_cnt) > 1)
#define OS_PKT_COPY(_pkt)		skb_copy(RTPKT_TO_OSPKT(_pkt), GFP_ATOMIC)

#define OS_PKT_TAIL_ADJUST(_pkt, _removedTagLen)								\
	SET_OS_PKT_DATATAIL(_pkt, GET_OS_PKT_DATATAIL(_pkt), (-_removedTagLen));	\
	GET_OS_PKT_LEN(_pkt) -= _removedTagLen;

#define OS_PKT_HEAD_BUF_EXTEND(_pkt, _offset)								\
	skb_push(RTPKT_TO_OSPKT(_pkt), _offset)

#define OS_PKT_TAIL_BUF_EXTEND(_pkt, _Len)									\
	skb_put(RTPKT_TO_OSPKT(_pkt), _Len)

#define OS_PKT_RESERVE(_pkt, _Len)											\
	skb_reserve(RTPKT_TO_OSPKT(_pkt), _Len)

#define GET_OS_PKT_TOTAL_LEN(_pkt)  (RTPKT_TO_OSPKT(_pkt)->pktLen)

#define get_unaligned32							get_unaligned
#define get_unalignedlong						get_unaligned
#define OS_NTOHS(_Val)  (ntohs(_Val))
#define OS_HTONS(_Val)  (htons(_Val))
#define OS_NTOHL(_Val)  (ntohl(_Val))
#define OS_HTONL(_Val)  (htonl(_Val))

#define CB_OFF  10

#define GET_OS_PKT_CB(_p)						(RTPKT_TO_OSPKT(_p)->cb)

/* User Priority */
#define RTMP_SET_PACKET_UP(_p, _prio)		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+0] = _prio)
#define RTMP_GET_PACKET_UP(_p)				(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+0])

/* Fragment # */
#define RTMP_SET_PACKET_FRAGMENTS(_p, _num)	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+1] = _num)
#define RTMP_GET_PACKET_FRAGMENTS(_p)		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+1])

/* 0x0 ~0x7f: TX to AP's own BSS which has the specified AID. if AID>127, set bit 7 in RTMP_SET_PACKET_EMACTAB too. */
/*(this value also as MAC(on-chip WCID) table index) */
/* 0x80~0xff: TX to a WDS link. b0~6: WDS index */
#define RTMP_SET_PACKET_WCID(_p, _wdsidx)   (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+2] = _wdsidx)
#define RTMP_GET_PACKET_WCID(_p)          	((UCHAR)(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+2]))

/* 0xff: PKTSRC_NDIS, others: local TX buffer index. This value affects how to a packet */
#define RTMP_SET_PACKET_SOURCE(_p, _pktsrc)	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+3] = _pktsrc)
#define RTMP_GET_PACKET_SOURCE(_p)       	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+3])

/* RTS/CTS-to-self protection method */
#define RTMP_SET_PACKET_RTS(_p, _num)      	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+4] = _num)
#define RTMP_GET_PACKET_RTS(_p)          	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+4])
/* see RTMP_S(G)ET_PACKET_EMACTAB */

/* TX rate index */
#define RTMP_SET_PACKET_TXRATE(_p, _rate)	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+5] = _rate)
#define RTMP_GET_PACKET_TXRATE(_p)		  	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+5])

/* From which Interface */
#define RTMP_SET_PACKET_IF(_p, _ifdx)		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+6] = _ifdx)
#define RTMP_GET_PACKET_IF(_p)		  		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+6])
#define RTMP_SET_PACKET_NET_DEVICE_MBSSID(_p, _bss)		RTMP_SET_PACKET_IF((_p), (_bss))
#define RTMP_SET_PACKET_NET_DEVICE_WDS(_p, _bss)		RTMP_SET_PACKET_IF((_p), ((_bss) + MIN_NET_DEVICE_FOR_WDS))
#define RTMP_SET_PACKET_NET_DEVICE_APCLI(_p, _idx)   	RTMP_SET_PACKET_IF((_p), ((_idx) + MIN_NET_DEVICE_FOR_APCLI))
#define RTMP_SET_PACKET_NET_DEVICE_MESH(_p, _idx)   	RTMP_SET_PACKET_IF((_p), ((_idx) + MIN_NET_DEVICE_FOR_MESH))
#define RTMP_SET_PACKET_NET_DEVICE_P2P(_p, _idx)   		RTMP_SET_PACKET_IF((_p), ((_idx) + MIN_NET_DEVICE_FOR_P2P_GO))
#define RTMP_GET_PACKET_NET_DEVICE_MBSSID(_p)			RTMP_GET_PACKET_IF((_p))
#define RTMP_GET_PACKET_NET_DEVICE(_p)					RTMP_GET_PACKET_IF((_p))

#define RTMP_SET_PACKET_MOREDATA(_p, _morebit)	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+7] = _morebit)
#define RTMP_GET_PACKET_MOREDATA(_p)			(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+7])
#define _GET_PAD_FROM_NET_DEV(_pAd, _net_dev)   (_pAd) = (_net_dev)->priv;
#define GET_PAD_FROM_NET_DEV(_pAd, _net_dev)                        \
     _pAd = RTMP_OS_NETDEV_GET_PRIV(_net_dev);



#ifdef UAPSD_SUPPORT
/* if we queue a U-APSD packet to any software queue, we will set the U-APSD
   flag and its physical queue ID for it */
#define RTMP_SET_PACKET_UAPSD(_p, _flg_uapsd, _que_id) \
                    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+9] = ((_flg_uapsd<<7) | _que_id))

#define RTMP_SET_PACKET_QOS_NULL(_p)     (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+9] = 0xff)
#define RTMP_GET_PACKET_QOS_NULL(_p)	 (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+9])
#define RTMP_SET_PACKET_NON_QOS_NULL(_p) (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+9] = 0x00)
#define RTMP_GET_PACKET_UAPSD_Flag(_p)   (((RTPKT_TO_OSPKT(_p)->cb[CB_OFF+9]) & 0x80) >> 7)
#define RTMP_GET_PACKET_UAPSD_QUE_ID(_p) ((RTPKT_TO_OSPKT(_p)->cb[CB_OFF+9]) & 0x7f)

#define RTMP_SET_PACKET_EOSP(_p, _flg)   (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+10] = _flg)
#define RTMP_GET_PACKET_EOSP(_p)         (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+10])
#endif /* UAPSD_SUPPORT */



/* */
/*	Sepcific Pakcet Type definition */
/* */
#define RTMP_PACKET_SPECIFIC_CB_OFFSET	11

#define RTMP_PACKET_SPECIFIC_DHCP		0x01
#define RTMP_PACKET_SPECIFIC_EAPOL		0x02
#define RTMP_PACKET_SPECIFIC_IPV4		0x04
#define RTMP_PACKET_SPECIFIC_WAI		0x08
#define RTMP_PACKET_SPECIFIC_VLAN		0x10
#define RTMP_PACKET_SPECIFIC_LLCSNAP	0x20
#define RTMP_PACKET_SPECIFIC_TDLS		0x40

/*Specific */
#define RTMP_SET_PACKET_SPECIFIC(_p, _flg)  \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] = _flg)

/*DHCP */
#define RTMP_SET_PACKET_DHCP(_p, _flg)   											\
    do{																				\
        if (_flg)																	\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_DHCP);		\
        else																		\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (!RTMP_PACKET_SPECIFIC_DHCP);	\
    }while(0)
#define RTMP_GET_PACKET_DHCP(_p)    \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_DHCP)

/*EAPOL */
#define RTMP_SET_PACKET_EAPOL(_p, _flg)   											\
    do{																				\
        if (_flg)																	\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_EAPOL);	\
        else																		\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (!RTMP_PACKET_SPECIFIC_EAPOL);	\
    }while(0)
#define RTMP_GET_PACKET_EAPOL(_p)   \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_EAPOL)

/*WAI */
#define RTMP_SET_PACKET_WAI(_p, _flg)   											\
    do{																				\
        if (_flg)																	\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_WAI);		\
        else																		\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (!RTMP_PACKET_SPECIFIC_WAI);	    \
    }while(0)
#define RTMP_GET_PACKET_WAI(_p) \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_WAI)

#define RTMP_GET_PACKET_LOWRATE(_p) \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & (RTMP_PACKET_SPECIFIC_EAPOL | RTMP_PACKET_SPECIFIC_DHCP | RTMP_PACKET_SPECIFIC_WAI))

/*VLAN */
#define RTMP_SET_PACKET_VLAN(_p, _flg)   											\
    do{																				\
        if (_flg)																	\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_VLAN);		\
        else																		\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (!RTMP_PACKET_SPECIFIC_VLAN);	\
    }while(0)
#define RTMP_GET_PACKET_VLAN(_p)    \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_VLAN)

/*LLC/SNAP */
#define RTMP_SET_PACKET_LLCSNAP(_p, _flg)   										\
    do{																				\
        if (_flg)																	\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_LLCSNAP);	\
        else																		\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (~RTMP_PACKET_SPECIFIC_LLCSNAP);	\
    }while(0)

#define RTMP_GET_PACKET_LLCSNAP(_p) \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_LLCSNAP)

/* IP */
#define RTMP_SET_PACKET_IPV4(_p, _flg)												\
    do{																				\
        if (_flg)																	\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_IPV4);		\
        else																		\
            (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (!RTMP_PACKET_SPECIFIC_IPV4);	\
    }while(0)

#define RTMP_GET_PACKET_IPV4(_p)    \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_IPV4)
#define RTMP_SET_PACKET_TDLS(_p, _flg)														\
			do{																				\
				if (_flg)																	\
					(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) |= (RTMP_PACKET_SPECIFIC_TDLS);		\
				else																		\
					(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11]) &= (~RTMP_PACKET_SPECIFIC_TDLS);	\
			}while(0)

#define RTMP_GET_PACKET_TDLS(_p)		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+11] & RTMP_PACKET_SPECIFIC_TDLS)


/* If this flag is set, it indicates that this EAPoL frame MUST be clear. */
#define RTMP_SET_PACKET_CLEAR_EAP_FRAME(_p, _flg)   \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+12] = _flg)
#define RTMP_GET_PACKET_CLEAR_EAP_FRAME(_p) \
    (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+12])

#ifdef DOT11_VHT_AC
#define MAX_PACKETS_IN_QUEUE				1024 /*(512)*/
#else
#define MAX_PACKETS_IN_QUEUE				(128)/*(512)*/
#endif /* DOT11_VHT_AC */


/* use bit3 of cb[CB_OFF+16] */

#define RTMP_SET_PACKET_MGMT_PKT(_p, _flg)  \
        RTPKT_TO_OSPKT(_p)->cb[CB_OFF+16] = (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+16] & 0xF7) | ((_flg & 0x01) << 3);
#define RTMP_GET_PACKET_MGMT_PKT(_p)    \
		((RTPKT_TO_OSPKT(_p)->cb[CB_OFF+16] & 0x08) >> 3)

/* use bit0 of cb[CB_OFF+20] */
#define RTMP_SET_PACKET_MGMT_PKT_DATA_QUE(_p, _flg)	\
        RTPKT_TO_OSPKT(_p)->cb[CB_OFF+20] = (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+20] & 0xFE) | (_flg & 0x01);
#define RTMP_GET_PACKET_MGMT_PKT_DATA_QUE(_p)		\
		(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+20] & 0x01)


#define RTMP_SET_PACKET_5VT(_p, _flg)   (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+22] = _flg)
#define RTMP_GET_PACKET_5VT(_p)         (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+22])

#define RTMP_SET_PACKET_PROTOCOL(_p, _protocol) {   \
	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+23] = (UINT8)((_protocol) & 0x00ff)); \
	(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+24] = (UINT8)(((_protocol) & 0xff00) >> 8)); \
}

#define RTMP_GET_PACKET_PROTOCOL(_p) \
	((((UINT16)(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+24]) & 0x00ff) << 8) \
	| ((UINT16)(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+23]) & 0x00ff))

#ifdef INF_AMAZON_SE
/* [CB_OFF+28], 1B, Iverson patch for WMM A5-T07 ,WirelessStaToWirelessSta do not bulk out aggregate */
#define RTMP_SET_PACKET_NOBULKOUT(_p, _morebit)			(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+28] = _morebit)
#define RTMP_GET_PACKET_NOBULKOUT(_p)					(RTPKT_TO_OSPKT(_p)->cb[CB_OFF+28])
#endif /* INF_AMAZON_SE */


#ifdef P2P_SUPPORT
#define RTMP_SET_PACKET_OPMODE(_p, _flg)   (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+26] = _flg)
#define RTMP_GET_PACKET_OPMODE(_p)         (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+26])
#endif /* P2P_SUPPORT */

#if defined(CONFIG_CSO_SUPPORT) || defined(CONFIG_RX_CSO_SUPPORT)
#define RTMP_SET_TCP_CHKSUM_FAIL(_p, _flg) (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+30] = _flg);
#define RTMP_GET_TCP_CHKSUM_FAIL(_p)  (RTPKT_TO_OSPKT(_p)->cb[CB_OFF+30])
#endif /* defined(CONFIG_CSO_SUPPORT) || defined(CONFIG_RX_CSO_SUPPORT) */


/* Max skb->cb = 48B = [CB_OFF+38] */



/***********************************************************************************
 *	Other function prototypes definitions
 ***********************************************************************************/
void RTMP_GetCurrentSystemTime(LARGE_INTEGER *time);
int rt28xx_packet_xmit(VOID *skb);

#ifdef RTMP_RBUS_SUPPORT
#ifndef CONFIG_RALINK_FLASH_API
void FlashWrite(UCHAR * p, ULONG a, ULONG b);
void FlashRead(UCHAR * p, ULONG a, ULONG b);
#endif /* CONFIG_RALINK_FLASH_API */
#endif /* RTMP_RBUS_SUPPORT */
/*
INT rt28xx_ioctl(
	IN	PNET_DEV		net_dev,
	IN	OUT	struct ifreq	*rq,
	IN	INT			cmd);
*/
extern int ra_mtd_write(int num, loff_t to, size_t len, const u_char *buf);
extern int ra_mtd_read(int num, loff_t from, size_t len, u_char *buf);



#define RTMP_OS_PKT_INIT(__pRxPacket, __pNetDev, __pData, __DataSize)		\
{																			\
	PNDIS_PACKET __pRxPkt;													\
	__pRxPkt = RTPKT_TO_OSPKT(__pRxPacket);									\
	SET_OS_PKT_NETDEV(__pRxPkt, __pNetDev);									\
	SET_OS_PKT_DATAPTR(__pRxPkt, __pData);									\
	SET_OS_PKT_LEN(__pRxPkt, __DataSize);									\
	SET_OS_PKT_DATATAIL(__pRxPkt, __pData, __DataSize);						\
}

typedef struct usb_device_id USB_DEVICE_ID;

#ifdef INF_AMAZON_SE
#define BULKAGGRE_SIZE				30
#else
#define BULKAGGRE_SIZE				60 /* 100 */
#endif /* INF_AMAZON_SE */
#define RTMP_OS_TASK							OS_TASK

#define RTMP_OS_TASK_DATA_GET(__pTask)						\
	((__pTask)->priv)

#define RTMP_OS_TASK_IS_KILLED(__pTask)						\
	((__pTask)->task_killed)

#define get_unaligned32							get_unaligned
#define get_unalignedlong						get_unaligned

typedef VOID									* PPCI_DEV;
/*#ifdef RTMP_USB_SUPPORT */
/******************************************************************************

  	USB related definitions

******************************************************************************/

#define RTMP_USB_PKT_COPY(__pNetDev, __pNetPkt, __Len, __pData)			\
{																		\
	memcpy(skb_put(__pNetPkt, __Len), __pData, __Len);					\
	GET_OS_PKT_NETDEV(__pNetPkt) = __pNetDev;							\
	RTMP_SET_PACKET_SOURCE(OSPKT_TO_RTPKT(__pNetPkt), PKTSRC_NDIS);		\
}


#ifdef INF_AMAZON_SE
#define BULKAGGRE_SIZE				30
#else
#define BULKAGGRE_SIZE				60 /* 100 */
#endif /* INF_AMAZON_SE */

#ifndef OS_ABL_SUPPORT
#define RTUSB_UNLINK_URB(pUrb)		usb_kill_urb(pUrb)
#else
#define RTUSB_UNLINK_URB(pUrb)		rausb_kill_urb(pUrb)
#endif /* OS_ABL_SUPPORT */

#define RT28XX_PUT_DEVICE(dev_p)

#ifndef OS_ABL_SUPPORT
#define RTUSB_ALLOC_URB(iso)                                                usb_alloc_urb(iso)
#define RTUSB_SUBMIT_URB(pUrb)                                              usb_submit_urb(pUrb,GFP_NOIO)
#else
#define RTUSB_ALLOC_URB(iso)                                                rausb_alloc_urb(iso)
#define RTUSB_SUBMIT_URB(pUrb)                                              rausb_submit_urb(pUrb)
#endif /* OS_ABL_SUPPORT */

#define RTUSB_URB_ALLOC_BUFFER(pUsb_Dev, BufSize, pDma_addr)                kmalloc(BufSize, GFP_NOIO)
#define RTUSB_URB_FREE_BUFFER(pUsb_Dev, BufSize, pTransferBuf, Dma_addr)    kfree(pTransferBuf)

#ifndef OS_ABL_SUPPORT
#define RTUSB_FREE_URB(pUrb)	usb_free_urb(pUrb)
#else
#define RTUSB_FREE_URB(pUrb)	rausb_free_urb(pUrb)
#endif /* OS_ABL_SUPPORT */

/* Prototypes of completion funuc. */
#define RtmpUsbBulkOutDataPacketComplete		RTUSBBulkOutDataPacketComplete
#define RtmpUsbBulkOutMLMEPacketComplete		RTUSBBulkOutMLMEPacketComplete
#define RtmpUsbBulkOutNullFrameComplete			RTUSBBulkOutNullFrameComplete
#ifdef CONFIG_MULTI_CHANNEL
#define RtmpUsbBulkOutHCCANullFrameComplete			RTUSBBulkOutHCCANullFrameComplete
#endif /* CONFIG_MULTI_CHANNEL */
#define RtmpUsbBulkOutRTSFrameComplete			RTUSBBulkOutRTSFrameComplete
#define RtmpUsbBulkOutPsPollComplete			RTUSBBulkOutPsPollComplete
#define RtmpUsbBulkRxComplete					RTUSBBulkRxComplete
#define RTUSBBulkCmdRspEventComplete(Status, pURB, pt_regs)	 	 RTUSBBulkCmdRspEventComplete(pURB)

#define RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutDataPacketComplete(pURB)
#define RTUSBBulkOutMLMEPacketComplete(Status, pURB, pt_regs)    RTUSBBulkOutMLMEPacketComplete(pURB)
#define RTUSBBulkOutNullFrameComplete(Status, pURB, pt_regs)     RTUSBBulkOutNullFrameComplete(pURB)
#ifdef CONFIG_MULTI_CHANNEL
#define RTUSBBulkOutHCCANullFrameComplete(Status, pURB, pt_regs)     RTUSBBulkOutHCCANullFrameComplete(pURB)
#endif /* CONFIG_MULTI_CHANNEL */
#define RTUSBBulkOutRTSFrameComplete(Status, pURB, pt_regs)      RTUSBBulkOutRTSFrameComplete(pURB)
#define RTUSBBulkOutPsPollComplete(Status, pURB, pt_regs)        RTUSBBulkOutPsPollComplete(pURB)
#define RTUSBBulkRxComplete(Status, pURB, pt_regs)               RTUSBBulkRxComplete(pURB)
#define RTUSBBulkCmdRspEventComplete(Status, pURB, pt_regs)               RTUSBBulkCmdRspEventComplete(pURB)
#define USBUploadFWComplete(Status, pURB, pt_regs)               USBUploadFWComplete(pURB)
#define USBKickOutCmdComplete(Status, pURB, pt_regs)               USBKickOutCmdComplete(pURB)

/*extern void dump_urb(struct urb *purb); */

#define InterlockedIncrement 	 	atomic_inc
#define NdisInterlockedIncrement 	atomic_inc
#define InterlockedDecrement		atomic_dec
#define NdisInterlockedDecrement 	atomic_dec
#define InterlockedExchange			atomic_set


typedef void USBHST_STATUS;
typedef INT32 URBCompleteStatus;
typedef struct pt_regs pregs;

USBHST_STATUS RTUSBBulkOutDataPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutMLMEPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutNullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
#ifdef CONFIG_MULTI_CHANNEL
USBHST_STATUS RTUSBBulkOutHCCANullFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
#endif /* CONFIG_MULTI_CHANNEL */
USBHST_STATUS RTUSBBulkOutRTSFrameComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkOutPsPollComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkRxComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS RTUSBBulkCmdRspEventComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS USBUploadFWComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);
USBHST_STATUS USBKickOutCmdComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);


/* Fill Bulk URB Macro */

#define RTUSB_FILL_TX_BULK_URB(pUrb,	\
			       pUsb_Dev,	\
			       uEndpointAddress,		\
			       pTransferBuf,			\
			       BufSize,				\
			       Complete,	\
			       pContext,		\
					TransferDma)	\
  			   do{	\
			       		usb_fill_bulk_urb(pUrb, pUsb_Dev, usb_sndbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
				}while(0)


#define RTUSB_FILL_HTTX_BULK_URB(pUrb,	\
				pUsb_Dev,	\
				uEndpointAddress,		\
				pTransferBuf,			\
				BufSize,				\
				Complete,	\
				pContext,				\
				TransferDma)				\
  				do{	\
					usb_fill_bulk_urb(pUrb, pUsb_Dev, usb_sndbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
				}while(0)

#define RTUSB_FILL_RX_BULK_URB(pUrb,	\
				pUsb_Dev,				\
				uEndpointAddress,		\
				pTransferBuf,			\
				BufSize,				\
				Complete,				\
				pContext,				\
				TransferDma)			\
  				do{	\
					usb_fill_bulk_urb(pUrb, pUsb_Dev, usb_rcvbulkpipe(pUsb_Dev, uEndpointAddress),	\
								pTransferBuf, BufSize, Complete, pContext);	\
				}while(0)
/* pRxContext->data_dma + pAd->NextRxBulkInPosition; */

#define RTUSB_URB_DMA_MAPPING(pUrb)	\
	{	\
		pUrb->transfer_dma	= 0;	\
		pUrb->transfer_flags &= (~URB_NO_TRANSFER_DMA_MAP);	\
	}

#define RTUSB_CONTROL_MSG(pUsb_Dev, uEndpointAddress, Request, RequestType, Value,Index, tmpBuf, TransferBufferLength, timeout, ret)	\
  		do{	\
			if (RequestType == DEVICE_VENDOR_REQUEST_OUT)	\
				ret = USB_CONTROL_MSG(pUsb_Dev, usb_sndctrlpipe(pUsb_Dev, uEndpointAddress), Request, RequestType, Value, Index, tmpBuf, TransferBufferLength, timeout);	\
			else if (RequestType == DEVICE_VENDOR_REQUEST_IN)	\
				ret = USB_CONTROL_MSG(pUsb_Dev, usb_rcvctrlpipe(pUsb_Dev, uEndpointAddress), Request, RequestType, Value, Index, tmpBuf, TransferBufferLength, timeout);	\
			else	\
			{	\
				DBGPRINT(RT_DEBUG_ERROR, ("vendor request direction is failed\n"));	\
				ret = -1;	\
			}	\
		}while(0)



#define RTMP_OS_USB_CONTEXT_GET(__pURB)		__pURB->rtusb_urb_context
#define RTMP_OS_USB_STATUS_GET(__pURB)		__pURB->rtusb_urb_status

#ifndef OS_ABL_SUPPORT
#define USB_CONTROL_MSG		usb_control_msg

#else

#define USB_CONTROL_MSG		rausb_control_msg

/*extern int rausb_register(struct usb_driver * new_driver); */
/*extern void rausb_deregister(struct usb_driver * driver); */

extern struct urb *rausb_alloc_urb(int iso_packets);
extern void rausb_free_urb(VOID *urb);

extern void rausb_put_dev(VOID *dev);
extern struct usb_device *rausb_get_dev(VOID *dev);

extern int rausb_submit_urb(VOID *urb);

#ifndef gfp_t
#define gfp_t		INT32
#endif /* gfp_t */

extern void *rausb_buffer_alloc(VOID *dev,
								size_t size,
								ra_dma_addr_t *dma);
extern void rausb_buffer_free(VOID *dev,
								size_t size,
								void *addr,
								ra_dma_addr_t dma);

extern void rausb_kill_urb(VOID *urb);

extern int rausb_control_msg(VOID *dev,
							unsigned int pipe,
							UINT8 request,
							UINT8 requesttype,
							UINT16 value,
							UINT16 index,
							void *data,
							UINT16 size,
							int timeout);

#endif /* OS_ABL_SUPPORT */

/*#endif // RTMP_USB_SUPPORT */
#undef KERN_EMERG
#define KERN_EMERG
#ifdef RALINK_ATE
#ifdef RTMP_MAC_USB
#ifdef CONFIG_AP_SUPPORT
#define EEPROM_BIN_FILE_NAME  "/etc/Wireless/RT2870AP/e2p.bin"
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
#undef EEPROM_BIN_FILE_NAME /* Avoid APSTA mode re-define issue */
#define EEPROM_BIN_FILE_NAME  "/etc/Wireless/RT2870STA/e2p.bin"
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_MAC_USB */
#ifdef RTMP_USB_SUPPORT

/* Prototypes of completion funuc. */
#define ATE_RTUSBBulkOutDataPacketComplete(Status, pURB, pt_regs)    ATE_RTUSBBulkOutDataPacketComplete(pURB)

USBHST_STATUS ATE_RTUSBBulkOutDataPacketComplete(URBCompleteStatus Status, purbb_t pURB, pregs *pt_regs);

#endif /* RTMP_USB_SUPPORT */
#define ate_print printf
#define ATEDBGPRINT DBGPRINT
#ifdef RTMP_MAC_PCI
#define EEPROM_SIZE								0x200
#ifdef CONFIG_AP_SUPPORT
#define EEPROM_BIN_FILE_NAME  "/tgtsvr/pci/e2p_ap.bin"
#endif /* CONFIG_AP_SUPPORT */
#endif /* RTMP_MAC_PCI */
#endif /* RALINK_ATE */

INT rt28xx_ioctl(
	IN	PNET_DEV	endDev,
	IN	caddr_t		data,
	IN	int			cmd);

long simple_strtol(const char *cp,char **endp,unsigned int base);
char *mtk_simple_strsep(register char **stringp, register const char *delim);
VOID rt2860_interrupt(void *dev_instance);

#ifdef RTMP_MAC_PCI
#define TX_RING_SIZE            64 /*64 */
#define MGMT_RING_SIZE          128
#define RX_RING_SIZE            64 /*64 */
#define MAX_TX_PROCESS          TX_RING_SIZE /*8 */
#define MAX_DMA_DONE_PROCESS    TX_RING_SIZE
#define MAX_TX_DONE_PROCESS     TX_RING_SIZE /*8 */
#define LOCAL_TXBUF_SIZE        2
#endif /* RTMP_MAC_PCI */

#define APCLI_IF_UP_CHECK(pAd, ifidx) ((pAd)->ApCfg.ApCliTab[(ifidx)].dev->state & ETH_DRV_STATE_ACTIVE)

#define RTMP_THREAD_PID_KILL(__PID)		KILL_THREAD_PID(__PID, SIGTERM, 1)

#define RTMP_OS_ATOMIC							atomic_t
#define RTMP_OS_ATMOIC_INIT(__pAtomic, __pAtomicList)
#define RTMP_OS_MAX_SCAN_DATA_GET()		IW_SCAN_MAX_DATA
#define OS_PKT_HEAD_BUF_EXTEND(_pkt, _offset)								\
	skb_push(RTPKT_TO_OSPKT(_pkt), _offset)

#include "os/rt_os.h"
#define RTUSBMlmeUp								OS_RTUSBMlmeUp

#define RTMP_NET_TASK_DATA_ASSIGN(__Tasklet, __Data)		\
		(__Tasklet)->data = (unsigned long)__Data

#define in_interrupt()    0
#define RTMP_OS_NETDEV_SET_WDEV		RtmpOsSetNetDevWdev
#define RTMP_OS_NETDEV_GET_WDEV		RtmpOsGetNetDevWdev
#define RTMP_OS_NETDEV_SET_TYPE_MONITOR(_pNetDev)
#define RTMP_OS_NETDEV_GET_TYPE(_pNetDev)
#define RTMP_OS_NETDEV_SET_TYPE(_pNetDev, _type)

extern void if_attach(struct ifnet *);
extern int mt7601_connect_status;


/* undef them to avoid compile errors in rt_symb.c */
#undef EINVAL
#undef EOPNOTSUPP
#undef EFAULT
#undef ENETDOWN
#undef E2BIG
#undef ENOMEM
#undef EAGAIN
#undef ENOTCONN

#define EINVAL					(-RTMP_IO_EINVAL)
#define EOPNOTSUPP      		(-RTMP_IO_EOPNOTSUPP)
#define EFAULT					(-RTMP_IO_EFAULT)
#define ENETDOWN				(-RTMP_IO_ENETDOWN)
#define E2BIG					(-RTMP_IO_E2BIG)
#define ENOMEM					(-RTMP_IO_ENOMEM)
#define EAGAIN					(-RTMP_IO_EAGAIN)
#define ENOTCONN				(-RTMP_IO_ENOTCONN)


#define URB_SHORT_NOT_OK 			MS_FLAG_URB_SHORT_NOT_OK
#define URB_NO_TRANSFER_DMA_MAP 	MS_FLAG_URB_NO_TRANSFER_DMA_MAP
#define URB_NO_SETUP_DMA_MAP  		MS_FLAG_URB_NO_SETUP_DMA_MAP
#define URB_ASYNC_UNLINK			MS_FLAG_URB_ASYNC_UNLINK
#define URB_ZERO_PACKET				MS_FLAG_URB_ZERO_PACKET
#define URB_NO_INTERRUPT			MS_FLAG_URB_NO_INTERRUPT

//#define KERN_EMERG      "<0>"   /* system is unusable                   */
#define KERN_ALERT      "<1>"   /* action must be taken immediately     */
#define KERN_CRIT       "<2>"   /* critical conditions                  */
#define KERN_ERR        "<3>"   /* error conditions                     */
#define KERN_WARNING    "<4>"   /* warning conditions                   */
#define KERN_NOTICE     "<5>"   /* normal but significant condition     */
#define KERN_INFO       "<6>"   /* informational                        */
#define KERN_DEBUG      "<7>"   /* debug-level messages                 */

#define usb_get_dev 		ms_usb_get_dev
#define usb_put_dev 		ms_usb_put_dev
#define usb_register		ms_usb_register
#define usb_deregister		ms_usb_deregister
#define usb_free_urb		ms_usb_free_urb
#define usb_alloc_urb		ms_usb_alloc_urb
#define usb_submit_urb		ms_usb_submit_urb
#define usb_unlink_urb		ms_usb_unlink_urb
#define	usb_control_msg 	ms_usb_control_cmd
#define usb_bulk_msg		ms_usb_bulk_msg
#define usb_fill_bulk_urb	ms_usb_stuff_bulk_urb
#define usb_fill_control_urb	ms_usb_stuff_control_urb
#define usb_set_intfdata	ms_usb_set_intfdata
#define usb_get_intfdata	ms_usb_get_intfdata
#define usb_kill_urb		ms_usb_unlink_urb


#define __cpu_to_le16(x)  (x)
#define __cpu_to_le32(x)  (x)


#endif /* __RT_ECOS_H__ */
