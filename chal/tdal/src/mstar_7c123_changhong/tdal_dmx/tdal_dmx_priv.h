/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DMX
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_dmx/tdal_dmx_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DMX private header
 *
 *****************************************************************************/

#ifndef _TDAL_DMX_PRIV_H_
#define _TDAL_DMX_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "MsCommon.h"
#include "apiDMX.h"

#ifndef ST_OSLINUX
#ifndef MODULE
#define MODULE
#endif
#endif

/*******************************************************
 *                      Defines                        *
 *******************************************************/

#define kTDAL_DMX_MAX_TASK_NAME         31      /* max length of a task name */
#define kTDAL_DMX_TASK_FLAG             0
#if defined(USE_TDAL_TS_SOURCE_SINK)
#define kTDAL_DMX_NB_DEMUX              2
#define kTDAL_DMXi_MAX_NB_OF_FILTERS    44
#else
#define kTDAL_DMX_NB_DEMUX              1
#define kTDAL_DMXi_MAX_NB_OF_FILTERS    48
#endif
#define kTDAL_DMX_DEPTH_SIZE            16      /* depth of the filter */

#define kTDAL_DMXi_MAX_NB_OF_CHANNELS   kTDAL_DMXi_MAX_NB_OF_FILTERS

#define kTDAL_DMX_ONE_KILO_SIZE         1024
#define kTDAL_DMX_FOUR_KILO_SIZE        (4 * kTDAL_DMX_ONE_KILO_SIZE)

/* size of memory reserved to sections */
#if defined(LOADER_EMBEDDED)
#define kTDAL_DMX_SECTIONS_MEM_SIZE     ((size_t)1024*1024)
#else
#ifdef EIT_FULL_CACHING_ON
#define kTDAL_DMX_SECTIONS_MEM_SIZE     ((size_t)512*1024)
#else
#define kTDAL_DMX_SECTIONS_MEM_SIZE     ((size_t)8*1024*5)
#endif
#endif

#define kTDAL_DMX_MSG_Q_MAX_MSG         (kTDAL_DMXi_MAX_NB_OF_CHANNELS * kTDAL_DMX_NB_DEMUX)    /* message queue limitation */


#define kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT 2

/*******************************************************
 *                 Macros                              *
 *******************************************************/
#define mLockAccess(lockKey)            (TDAL_LockMutex(lockKey))
#define mUnLockAccess(lockKey)          (TDAL_UnlockMutex(lockKey))


/*******************************************************
 *              Typedefs                               *
 *******************************************************/
/* Channel structure */
typedef struct
{
    tTDAL_DMX_DemuxId DemuxId;                      /* demux id */
    tTDAL_DMX_ChannelId ChannelId;                  /* channel id */

    tTDAL_DMX_ChannelStream ChannelStream;          /* channel type */
    tTDAL_DMX_ChannelOutput ChannelOutput;          /* channel output */
    tTDAL_DMX_ChannelMemorySize ChannelMemorySize;  /* channel memory size */

    bool bUsed;                                     /* channel allocated */
    bool bEnable;                                   /* channel enabled */
    bool bWaitingForDeletion;                       /* channel to be deleted */

    tTDAL_DMX_ChannelCallbackFct Callback;          /* callback function to signal */
    /* new section filter data */

    int32_t ChannelPid;                             /* Pid of this Channel */

    uint32_t DemuxHandle;                           /* handle of the demux */
    uint32_t StreamHandle;                          /* handle of the stream=channel */
    //STPTI_Buffer_t BufferHandle;                  /* STPTI handle on a buffer */
    uint32_t BufferSize;                            /* size of the SPTI buffer */
    //STPTI_Signal_t SignalHandle;                  /* STPTI handle on a signal */

    //task_t *taskId;                               /* Task id of this channel */
    uint8_t *ptrBufferSection;                      /* Adr of section buffer received */
    void *pStack;                                   /* stack pointer of the task */
    //tdesc_t *tdescTask;                           /* task descriptor of the task */
} tTDAL_DMX_Channel_t;

/* Filter structure */
typedef struct
{
    tTDAL_DMX_FilterId FilterId;                    /* filter identificator */
    tTDAL_DMX_ChannelId AssociatedChannelId;        /* associated channel id */

    bool bUsed;                                     /* filter allocated */
    bool bEnable;                                   /* filter enabled (started,stopped) */
    bool bWaitingEnable;                            /* filter waiting state */
    bool bSet;                                      /* filter set up */
    bool bMultimediaDisabled;
    /* EDO */
    bool bStptiAssociated;                          /* filter set up */
    uint32_t Tag;                                   /* set by client */

    uint8_t *Filter;                                /* value of the hardware filter */
    uint8_t *PositiveMask;                          /* mask of the hardware filter */
    uint8_t *NegativeMask;                          /* mode of the hardware filter (positive or negative) */
    //STPTI_FilterData_t filterData;                /* specific data for ST */
    tTDAL_DMX_FilterCRCMode efilterCRCMode;         /* CRC mode: check or not */
    MS_U8 FilterHandle;                             /* MApi handle on filter */
    DMX_FILTER_TYPE FilterType;
    DMX_Flt_info FilterInfo;

    bool AllocateHWFilter;
    void *SectionBufferPtr;
} tTDAL_DMX_Filter_t;

typedef struct
{
    tTDAL_DMX_DemuxId Id;                           /* Id used by application */
    MS_U8 Handle;                                   /* handle given by hardware */
} tTDAL_DMX_Demux_t;


typedef struct
{
    TDAL_task_id TaskHandle;
    unsigned char TaskStack[0x4000];

    volatile MS_BOOL ShouldExit;
    volatile MS_BOOL Finished;
} tTDAL_DMXi_SectionTask;

/*******************************************************
 *      Variables   Declarations   (IMPORT)            *
 *******************************************************/

/** those variables are defined in tdal_dmx.c **/
/* see tdal_dmx.c to get information about them */
IMPORT bool TDAL_DMXi_initialized;
IMPORT tTDAL_DMX_Channel_t TDAL_DMXi_pstChannel[kTDAL_DMXi_MAX_NB_OF_CHANNELS];
IMPORT tTDAL_DMX_Filter_t TDAL_DMXi_pstFilter[kTDAL_DMXi_MAX_NB_OF_FILTERS];
IMPORT tTDAL_DMX_Demux_t TDAL_DMXi_pstDemux[kTDAL_DMX_NB_DEMUX];
IMPORT uint8_t TDAL_DMXi_usedPTI[kTDAL_DMX_NB_DEMUX];
IMPORT TDAL_mutex_id TDAL_DMXi_pSectionTableAccess;
#if defined(TDAL_DMX_CHANNEL_DELETION_BY_TASK)
IMPORT STOS_MessageQueue_t * TDAL_DMXi_deletionMngtMsgQ;
IMPORT task_t * TDAL_DMXi_deleteMngtTaskId;
#endif
IMPORT tTDAL_DMX_DemuxId TDAL_DMXi_pstDemuxId[kTDAL_DMX_NB_DEMUX];
IMPORT MS_S32 TDAL_DMXi_sections_partition;
IMPORT unsigned char TDAL_DMXi_sections_memory[kTDAL_DMX_SECTIONS_MEM_SIZE];
//IMPORT STPTI_Capability_t TDAL_DMXi_PTICapability[kTDAL_DMX_NB_DEMUX];

IMPORT TDAL_queue_id TDAL_DMXi_SectionQueue;

IMPORT tTDAL_DMXi_SectionTask TDAL_DMXi_SectionTaskArr[kTDAL_DMXi_SECTION_RECEIVE_THREAD_COUNT];



/*******************************************************
 *      Internal   Functions   Declarations            *
 *******************************************************/
tTDAL_DMX_Error TDAL_DMXi_Control_Filter(tTDAL_DMX_FilterId FilterId, tTDAL_DMX_Ctrl Ctrl);
/* allocate memory in section-reserved partition */
void *TDAL_DMXi_Section_Malloc(unsigned long memorySize);

/* de-allocate memory in ... */
void TDAL_DMXi_Section_Free(void *buffer);

#if defined(TDAL_DMX_CHANNEL_DELETION_BY_TASK)
/* deletion task entry point */
void TDAL_DMXi_taskChannelDeletionMngt(void *param);
#else
void TDAL_DMXi_ChannelDeletionMngt(tTDAL_DMX_ChannelId channelForDeletion);
#endif
/* reception task entry point */
void TDAL_DMXi_taskSectionReceived(void *param);

/* reception task for PES entry point */
void TDAL_DMXi_taskPESReceived(void *param);

/* free all resources needed by a channel */
tTDAL_DMX_Error TDAL_DMXi_FreeChannelAndAssociatedRessources(tTDAL_DMX_ChannelId channelId);

/* free all resources needed by a filter */
tTDAL_DMX_Error TDAL_DMXi_FreeFilterAndAssociatedRessources(tTDAL_DMX_FilterId FilterId);

/* look for a free channel Id */
tTDAL_DMX_ChannelId TDAL_DMXi_LookForFreeChannel(void);

/* look for a free filter Id */
tTDAL_DMX_FilterId TDAL_DMXi_LookForFreeFilter(void);

/* modify filter setup */
tTDAL_DMX_Error TDAL_DMXi_FilterActivate(tTDAL_DMX_DemuxId DemuxId, tTDAL_DMX_ChannelId ChannelId, tTDAL_DMX_FilterId FilterId);

void TDAL_DMXi_NotifySectCallback(MS_U8 u8DmxId, DMX_EVENT enEvent, MS_U32 NotifyParam);

void TDAL_DMXi_SectionTask(void *argv);

#if 0   /* GS: use TBOX_GetDriverErrorText() instead */
#ifdef   __DEBUG_TRACE__
/* error conversion function */
char *TDAL_DMXi_TranslateTdalErrorInString(int errorCode);
#endif
#endif

#if defined(__DEBUG_TRACE__) && defined(TDAL_DMX_DBG_TRC_PCK_CNT)
void TDAL_DMXi_DebugPacketCount(tTDAL_DMX_ChannelId ChannelId);
#endif

#ifdef __cplusplus
}
#endif

#endif   /*_TDAL_DMX_PRIV_H_*/
