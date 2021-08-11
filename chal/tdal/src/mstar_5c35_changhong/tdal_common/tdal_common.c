/******************************************************************************
 *                   COPYRIGHT 2004 IWEDIA TECHNOLOGIES                   *
 ******************************************************************************
 *
 * MODULE NAME: TDAL Common
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_common/tdal_common.c $
 *          $Rev: 673 $
 *          $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/********************************************************
   *        Includes                         *
   ********************************************************/
#include<stdio.h>

/*   Necessary   for   stgfb   FB   enable/disable*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>


#include"crules.h"
#include"tdal_common.h"
#include"tbox.h"
#include"flash_cfg.h"
#include"tdal_common_priv.h"
#include "tdal_ssu_priv.h"

#include "MsCommon.h"
#include "MsOS.h"
#include "SysInit.h"
#include "drvUART.h"
#include "drvWDT.h"

#if defined (MSOS_TYPE_ECOS)
#include <cyg/hal/hal_cache.h>
#endif


#ifdef   DIAGNOSTICS_ENABLE
#include"diagnostics_cfg.h"
#endif
/********************************************************
   *        Defines                         *
   ********************************************************/
/*   Register   needed   for   TDAL_Reset   */


/*   WatchDog   Registers   */

/********************************************************
   *        Macros                         *
   ********************************************************/
#define UARTBUF_SIZE        512
#define XOFF_THRASHOLD ( UARTBUF_SIZE - 64 )
#define XON_THRASHOLD  ( UARTBUF_SIZE - 128 )
#define XON  17
#define XOFF 19
#define TDAL_NB_QUEUES 20
/********************************************************
*        Global   Variables   (GLOBAL/IMPORT)        *
********************************************************/

#define   kTDAL_MEMORY_SIZE   0x140000

#define WATCHDOG_TIMEOUT    3 /* in sec */
#define WATCHDOG_FEED_TIME  1 /* in sec */

/* MSTAR globals, currently not sure why we need them! */
MS_S32          gs32SystemPoolID = 0;           /* for MSOS_ECOS*/

/* TDAL globals */
/********************************************************
   *        Local   Module   Variables   (MODULE)        *
   ********************************************************/
static MS_U8    TDAL_memory[kTDAL_MEMORY_SIZE]; /* Memory buffer for TDAL memory pool */
static MS_S32	TDAL_MemoryPool = -1;
IMPORT  void*   RT_Task(void *arg);
static MS_U8   UartBuffer[UARTBUF_SIZE] ;
static MS_BOOL DisableUart = FALSE  ;
static MS_U32  UartDisableCounter = 0 ;
static MS_BOOL UartStop = FALSE ;
static MS_U32  u32Read = 0 , u32Write = 0 , CharNum = 0;

static MS_BOOL WatchdogStarted = FALSE;
static MS_BOOL WatchdogAllowFeed = TRUE;
static MS_S32  WatchdogTimer = -1;

#ifdef NEVER /* should be removed */
static MS_U8 UartBuf[64];
#endif

MS_U8 UartMatch[] = {'0','0','1','1','2','2','3','3'};

/* QUEUE management */
typedef struct {
   void        *queue;       /* the queue */
   int         entry;        /* number of elements in the queue */
   int         head;         /* head of the queue */
   int         tail;         /* tail of the queue */
   int         count;        /* maximum number of messages in the queue */
   int         msgsize;
   MS_S32      sema;
}TDAL_queue_t;

static int          TDAL_mInitQueue = 0;             /* 0 if not initialized */
static int          TDAL_mNbQueue = 0;              /* Nb created queues */
static TDAL_queue_t TDAL_mQueue[40];   /* Queue management */
static MS_S32  TDAL_mQueueMutex = -1;
static MS_S32  TDAL_COMMONi_MallocMutex = -1;
/********************************************************
   *        Local   File   Variables   (LOCAL)          *
   ********************************************************/
mTBOX_SET_MODULE(eTDAL_COMMON);

/********************************************************
   *        Module   functions                   *
   ********************************************************/


/********************************************************
 *          Local functions                         *
 ********************************************************/

LOCAL void TDALi_StartWatchdog(void);

void XONOFF_FlowControl(void)
{

    MS_U32 size ;
    if (u32Read > u32Write)
    {
        size = UARTBUF_SIZE - (u32Read - u32Write );
    }
    else
    {
        size = u32Write - u32Read ;
    }
    if ((size > XOFF_THRASHOLD ) && (!UartStop))
    {
        CharNum++;
        UartStop = TRUE ;
        MDrv_UART_PutChar((MS_U8)XOFF);
    }
    else if ((size <= XON_THRASHOLD ) && (UartStop))
    {
        UartStop = FALSE ;
        MDrv_UART_PutChar((MS_U8)XON);
    }
    return ;
}

void UartDisableCheck(MS_U8 InCh)
{
    // Check with Uart Match Table //
    if ( UartMatch[UartDisableCounter] == InCh )
    {
            UartDisableCounter++ ;
    }
    else
    {
        if (InCh != UartMatch[0])
            UartDisableCounter = 0 ;
        else
            UartDisableCounter = 1 ;
    }
    if (UartDisableCounter == 8)
    {
        DisableUart = ~DisableUart ;
        UartDisableCounter = 0 ;
    }
}

void UartCB(int c)
{
    MS_U8 InCh;

    InCh = MDrv_UART_GetChar();

    if (InCh != (MS_U8)c)
    {
        return;
    }

    UartBuffer[u32Write] = InCh ;
    u32Write = (u32Write + 1 ) % UARTBUF_SIZE ;
    XONOFF_FlowControl();

    UartDisableCheck(InCh);

    return ;
}

void cyg_uart_init(void)
{
    *(volatile MS_U16*)(0xBF203C00+(0x53<<2)) |= 0x0004;        // CHIPTOP: select piu_uart
    *(volatile MS_U16*)(0xBF201600+(0x13<<2)) &= ~0x1F00;       // CLKGEN0: ckg_uart [12:8]
    *(volatile MS_U16*)(0xBF201600+(0x13<<2)) |= 0x0C00;        //          [12:8] UART_CLK : 123Mhz [0] wired UART_CLK
    *(volatile MS_U16*)(0xBF001C00+(0x09<<2)) |= 0x0800;        // PM_SLEEP: reg_uart_enable

    MDrv_UART_Init(E_UART_PIU_UART0, 115200);
}

void cyg_uart_putchar(unsigned char c)
{
//#ifndef CAK_MERLIN

//#if defined(__TRACE_DCU__)

//  if(!DisableUart)
    MDrv_UART_PutChar((MS_U8)c);

//#endif
}

unsigned char cyg_uart_getchar(void)
{
    MS_U8 tmp ;

    while(1)
    {
        if(u32Read != u32Write)
        {
            tmp =  UartBuffer[u32Read];
            u32Read = (u32Read + 1) % UARTBUF_SIZE;

            XONOFF_FlowControl();

            return  tmp;
        }
        MsOS_DelayTask(1);
    }
}

void __cxa_atexit(void (*arg1)(void*), void* arg2, void* arg3)
{

}

#include <cyg/kernel/kapi.h>

void cyg_exception_handler_fn(
    cyg_addrword_t data,
    cyg_code_t   exception_number,
    cyg_addrword_t info
)
{
    int i;
    HAL_SavedRegisters * pRegisters = (HAL_SavedRegisters *) info;
 
    printf("Program crash: PC = 0x%x, exception number = %d, info = 0x%x\n",pRegisters->pc, exception_number, info);
    for (i=0; i<32; i++)
    {
        printf("r%d 0x%x\n", i, pRegisters->d[i]);
    }
    printf("badva 0x%x\n", pRegisters->badvr);
 
    WatchdogAllowFeed = FALSE;
}

/***********************************************************************
   *   Function   Name   :   TDAL_Init
   *
   *   Description   :   Initialise   driver   dependant   source   code
   *
   *   Side   effects   :
   *
   *   Comment      :
*
   **********************************************************************/
GLOBAL   void   TDAL_Init(void)
{
   
    /* TDAL_poolID = MsOS_CreateMemoryPool(kTDAL_MEMORY_SIZE, 16, (void*)&TDAL_memory, E_MSOS_FIFO, "TdalMemoryPool");  
     * TDAL_poolID is imported from SysInit module and belongs to cached memory pool
     * it can be seen from mstar examples that av, pvr, mm are using non-cached, and other is using cached 
     * so we need to take same consideration/exploring for this non-cached/cached issue.
     * */

	cyg_exception_set_handler(0xFFFFFFFF, cyg_exception_handler_fn, (cyg_addrword_t) NULL, NULL, NULL);

	TDAL_COMMONi_MallocMutex = MsOS_CreateMutex(E_MSOS_FIFO, NULL, 0);
	mTBOX_ASSERTm(TDAL_COMMONi_MallocMutex > 0, "TDAL_COMMON mutex not created\n");

	TDAL_MemoryPool = MsOS_CreateMemoryPool(
				sizeof(TDAL_memory),
				0x0, /* Min allocation */
				(void*)&TDAL_memory,
				E_MSOS_FIFO,
				"TDAL Memory pool");

	mTBOX_ASSERTm(TDAL_MemoryPool > 0, "TDAL memory pool could not be created\n");

	TDALm_WorkerThreadsInitalize();
	//TDAL_FP_Init();/*for fp test*/

#ifndef __DEBUG__
	TDALi_StartWatchdog();
#endif

	TDALm_CRC32_Init();

    return;
}

/***********************************************************************
   *   Function   Name   :   TDAL_Term
   *
   *   Description   :   Terminate   driver   dependant   source   code
   *
   *   Side   effects   :
   *
   *   Comment      :
*
   **********************************************************************/
GLOBAL   void   TDAL_Term(void)
{
    bool bfRet = FALSE;
	if (TDAL_COMMONi_MallocMutex > 0)
	{
		bfRet = MsOS_DeleteMutex(TDAL_COMMONi_MallocMutex);
		mTBOX_ASSERT( TRUE == bfRet);
		TDAL_COMMONi_MallocMutex = -1;
	}

	if (TDAL_MemoryPool > 0)
	{
		bfRet = MsOS_DeleteMemoryPool(TDAL_MemoryPool);
		mTBOX_ASSERT( TRUE == bfRet);
		TDAL_MemoryPool = -1;
	}

	TDALm_WorkerThreadsDeinitalize();
}

/***********************************************************************
   *   Function   Name   :   TDAL_EnterLowPowerTimeout
   *
   *   Description   :   Initiate   the   low   power   (a.k   a.   idle   state).
   *
   *   Side   effects   :
   *
   *   Comment      :   if   timeout   is   0, the   TV   set   will   never   auto   power   on.
   *              If   timeout   is   less   than   kTDAL_AUTO_POWER_ON_OFFSET,
   *              the   TV   set   will   auto   power   on   immediately.
   *
   **********************************************************************/
void   TDAL_EnterLowPowerTimeout(uint32_t   timeout)
{
   return;
}

/***********************************************************************
   *   Function   Name   :   TDAL_GetPowerOnStatus
   *
   *   Description   :   Get   the   status   of   the   low   power:
   *              -   the   cause   of   the   termination;
   *              -   the   duration   since   its   initiation.
   *
   *   Side   effects   :
   *
   *   Comment      :   If   TDAL_GetPowerOnStatus   returns   false,
   *              the   power   on   status   and   the   duration   are   not   accurate.
*
   **********************************************************************/
bool   TDAL_GetPowerOnStatus(tTDAL_PowerOnStatus   *status,
                   uint32_t          *lowPowerDuration,
                   uint32_t          ulStandbyReference)
{
   bool              result = false;
   return(result);
}

/***********************************************************************
   *   Function   Name   :   TDAL_Reset
   *
   *   Description   :   Reset   the   Hardware   platform.
   *
   *   Side   effects   :
   *
   *   Comment      :
*
   **********************************************************************/
GLOBAL   void   TDAL_Reset(void)
{
    WDT_Result efRet = E_WDT_FAIL;
	mTBOX_FCT_ENTER("TDAL_Reset");

    printf("TDAL_Reset\n");

    TDALi_StartWatchdog();

    WatchdogAllowFeed = FALSE;
    efRet = MDrv_WDT_SetTimer_ms(E_WDT_DBGLV_ALL, 1);
    mTBOX_ASSERT( E_WDT_OK == efRet );
    MsOS_DelayTask(2 * WATCHDOG_TIMEOUT * 1000);
    mTBOX_ASSERTm(FALSE, "The system should not reach this point, but should restart\n");

	mTBOX_RETURN;
}

/***********************************************************************
   *   Function   Name   :   TDAL_Jump
   *
   *   Description   :   Jump   to   the   bootloader   or   the   application.
   *
   *   Side   effects   :
   *
   *   Comment      :
*
   **********************************************************************/
GLOBAL   void   TDAL_Jump(tTDAL_JUMP_Type   eJumpType)
{
    WDT_Result efRet = E_WDT_FAIL;
   /* STB reset */
    MDrv_WDT_Init(E_WDT_DBGLV_ALL);
    mTBOX_ASSERT( E_WDT_OK == efRet );
    MDrv_WDT_SetTimer(E_WDT_DBGLV_ALL, 1);
    mTBOX_ASSERT( E_WDT_OK == efRet );

    MsOS_DelayTask(2000);
}


/***********************************************************************
   *   Function   Name   :   TDAL_GetUpTime
   *
   *   Description   :   Get   how   long   the   system   has   been   running.
   *
   *   Side   effects   :
   *
   *   Comment      :   If   TDAL_GetUpTime   returns   false, the   uptime   is   not
   *              accurate.
   *
   **********************************************************************/
bool   TDAL_GetUpTime(uint32_t   *upTime)
{
   bool        result = false;
   *upTime = 0;
   return(result);
}


/***********************************************************************
   *   Function   Name   :   TDAL_GetTime
   *
   *   Description   :   Get   the   time   since   the   Epoch   (00:00:00   UTC, January   1,
   *              1970), measured   in   seconds.
   *
   *   Side   effects   :
   *
   *   Comment      :   If   TDAL_GetTime   returns   false, the   time   is   not
   *              accurate.
   *
   **********************************************************************/
bool   TDAL_GetTime(uint32_t   *time)
{
   bool      result = false;
   *time = 0;
   return(result);
}


/***********************************************************************
   *   Function   Name   :   TDAL_SetTime
   *
   *   Description   :   Set   the   time   since   the   Epoch   (00:00:00   UTC, January   1,
   *              1970), measured   in   seconds.
   *
   *   Side   effects   :
   *
   *   Comment      :   If   TDAL_SetTime   returns   false, the   time   is   not   changed.
   *
   **********************************************************************/
bool   TDAL_SetTime(uint32_t   time)
{
   bool      result = false;
   if   (time)   {};
   result = true;
   return(result);
}

/***********************************************************************
   *   Function   Name   :   TDAL_SetMemoryPartition
   *
   *   Description   :   Set   memory   partition   which   will   be   used   by   TDAL   Component
   *
   *   Side   effects   :
   *
   *   Comment      :
*
   **********************************************************************/
GLOBAL   void   TDAL_SetMemoryPartition(void*   pPartition)
{
    return ;
}

#if 0
void   TDAL_DebugPartitionStatus(partition_t   *partition)
{
}
#endif
/***********************************************************************
   *   Function   Name   :   TDAL_Malloc
   *   scope      :   private   to   TDAL
   *
   *   Description   :   Allocate   memory   in   specific   system   partition
   *
   *   Side   effects   :
   *
   *   Comment      :
*
   **********************************************************************/
GLOBAL   void*   TDAL_Malloc(uint32_t   memorySize)
{
	void   *ptr = NULL;
	mTBOX_FCT_ENTER("TDAL_Malloc");
    MS_BOOL bfRet = FALSE;

	bfRet = MsOS_ObtainMutex(TDAL_COMMONi_MallocMutex, -1);
	mTBOX_ASSERT(bfRet == TRUE);
	ptr = MsOS_AllocateMemory(memorySize, TDAL_MemoryPool);
	MsOS_ReleaseMutex(TDAL_COMMONi_MallocMutex);
	mTBOX_ASSERT(bfRet == TRUE);

	if (ptr == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Allocation failed mempoolID %d, memory_size = %d\n", TDAL_MemoryPool, memorySize));
	}
	mTBOX_RETURN(ptr);
}

/***********************************************************************
   *   Function   Name   :   TDAL_Free
   *   scope      :   private   to   TDAL
   *
   *   Description   :   Free   memory   previously   allocated   in   specific   system   partition
   *
   *   Side   effects   :
   *
   *   Comment      :
   *
   **********************************************************************/
GLOBAL   void   TDAL_Free(void *buffer)
{
	MS_BOOL bRet;
    MS_BOOL bfRet = FALSE;

	if (buffer)
	{
		bfRet = MsOS_ObtainMutex(TDAL_COMMONi_MallocMutex, -1);
        mTBOX_ASSERT(bfRet == TRUE);
		bRet = MsOS_FreeMemory(buffer, (MS_S32)TDAL_MemoryPool);
		bfRet = MsOS_ReleaseMutex(TDAL_COMMONi_MallocMutex);
        mTBOX_ASSERT(bfRet == TRUE);

		mTBOX_ASSERTm(bRet == TRUE, "MsOS_FreeMemory failed. This could mean deallocation of memory which is not allocated\n");
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Failed - buffer address is NULL\n"));
	}
	
    return;
}


void   TDAL_CreateMutex(TDAL_mutex_id * MutexID)
{
	MS_S32 mutex = MsOS_CreateMutex(E_MSOS_FIFO, NULL, 0);
    mTBOX_ASSERT(mutex >=0);
	if (mutex == -1)
	{
	    *MutexID = NULL;
	}
	else
	{
		*MutexID = (TDAL_mutex_id) TDAL_Malloc(sizeof(TDAL_mutex_t));
		if (*MutexID != NULL)
		{
			((TDAL_mutex_t *)*MutexID)->mutex_id = mutex;
		}
	}

}

void   TDAL_DeleteMutex(TDAL_mutex_id   MutexID)
{
    MS_BOOL bfRet = FALSE;
	TDAL_mutex_t *mutex = (TDAL_mutex_t*) MutexID;

	bfRet = MsOS_DeleteMutex((MS_S32)mutex->mutex_id);
    mTBOX_ASSERT( TRUE == bfRet);

	TDAL_Free(mutex);

}


void   TDAL_LockMutex(TDAL_mutex_id   MutexID)
{
	TDAL_mutex_t *mutex = (TDAL_mutex_t*) MutexID;
	uint32_t owner = MsOS_GetOSThreadID();

	if (mutex->task_id != owner)
	{
		MsOS_ObtainMutex((MS_S32)mutex->mutex_id, -1);
		mutex->task_id = owner;
		mutex->count = 1;
	}
	else
	{
		mutex->count ++;
	}
}


void   TDAL_UnlockMutex(TDAL_mutex_id   MutexID)
{
	TDAL_mutex_t *mutex = (TDAL_mutex_t*) MutexID;
    MS_BOOL bfRet = FALSE;

	if (mutex->count > 1)
	{
		mutex->count --;
	}
	else
	{
		mutex->task_id = 0;
		mutex->count = 0;
		bfRet = MsOS_ReleaseMutex((MS_S32)mutex->mutex_id);
		mTBOX_ASSERT(TRUE == bfRet);
	}

}

TDAL_task_id TDAL_CreateTask(TDAL_TaskPriority priority,
        char *taskName,
        unsigned char * stack,
        uint32_t stackSize,
        void (*entry) (void*),
        void *arg
        )
{
    pthread_attr_t attr;
    pthread_t threadID;
    int ret;
    struct sched_param schedparam;

    TDAL_task_id returnValue;

    mTBOX_FCT_ENTER("TDAL_CreateTask");

    if ((entry == NULL) || (stack == NULL) || (stackSize <= 0))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Incorrect arguments\n"));
        mTBOX_RETURN(NULL);
    }

    ret = pthread_attr_init( &attr );
    mTBOX_ASSERT(ret == 0);
    ret = pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
    mTBOX_ASSERT(ret == 0);
    ret = pthread_attr_setstackaddr( &attr, &stack[stackSize] );
    mTBOX_ASSERT(ret == 0);
    ret = pthread_attr_setstacksize( &attr, stackSize );
    mTBOX_ASSERT(ret == 0);
    ret = pthread_attr_setschedpolicy( &attr, SCHED_RR); //SCHED_RR
    mTBOX_ASSERT(ret == 0);

    switch(priority)
    {
    case eTDAL_PRIORITY_REALTIME:
        schedparam.sched_priority = 25;
    	break;
    case eTDAL_PRIORITY_HIGH:
        schedparam.sched_priority = 20;
    	break;
    case eTDAL_PRIORITY_LOW:
        schedparam.sched_priority = 10;
        break;
    case eTDAL_PRIORITY_NORMAL:
    default:
        schedparam.sched_priority = 16;
    	break;
    }

    mTBOX_ASSERT((schedparam.sched_priority > sched_get_priority_min(SCHED_RR))
            && (schedparam.sched_priority < sched_get_priority_max(SCHED_RR)));

    ret = pthread_attr_setschedparam( &attr, &schedparam );
    mTBOX_ASSERT(ret == 0);

    if (pthread_create( &threadID, &attr,(void* (*) (void *)) entry, arg ) != 0)
    {
    	returnValue = NULL;
    }
    else
    {
    	returnValue = (TDAL_task_id) threadID;
    }

    pthread_attr_destroy(&attr);
    mTBOX_RETURN(returnValue);
}

void TDAL_DeleteTask(TDAL_task_id taskId)
{
	int ret;

	if (taskId == NULL)
	{
		return;
	}

	//ret = pthread_cancel(taskId);
	ret = pthread_join(taskId, NULL);
}

void TDAL_DelayTask(uint32_t timeInMs)
{
    unsigned int timeout = timeInMs;
    struct timespec timeout0;
    struct timespec timeout1;
    struct timespec* tmp;
    struct timespec* t0 = &timeout0;
    struct timespec* t1 = &timeout1;

    mTBOX_FCT_ENTER("TDAL_DelayTask");

    /* usleep can't be used for duration < 1sec - POSIX.1 -2001 declares this function has obsolete, use nanosleep instead */
    t0->tv_sec = (timeout) / 1000; /* Seconds */
    t0->tv_nsec = (timeout % 1000) * 1000000; /* nanoseconds */
    if (t0->tv_nsec >= 1000000000)
    {
        t0->tv_nsec -= 1000000000;
        t0->tv_sec++;
    }

    while (-1 == nanosleep(t0, t1))
    {
        if (errno == EINTR)
        {
            tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
        else
        {
            mTBOX_RETURN;
        }
    }

    mTBOX_RETURN;
}

static int TDALi_AddFifo(int index, unsigned short len, unsigned char *msg)
{
    MS_BOOL bfRet = FALSE;
    bfRet = MsOS_ObtainMutex(TDAL_mQueueMutex, -1);
    mTBOX_ASSERT(TRUE == bfRet);
    if (TDAL_mQueue[index].entry >= TDAL_mQueue[index].count)   /* full fifo : Error */
    {
        bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
        mTBOX_ASSERT(TRUE == bfRet);
        mTBOX_TRACE((kTBOX_NIV_WARNING, "AddFifo : fifo is full\n"));
        return (-1);
    }

    if (MsOS_ReleaseSemaphore(TDAL_mQueue[index].sema) == FALSE)
    {
        bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
        mTBOX_ASSERT(TRUE == bfRet);
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Semaphore error\n"));
        return (-1);
    }

    if (len > TDAL_mQueue[index].msgsize)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "AddFifo : message to enqueue is too big and will be truncated\n"));
    }

    TDAL_mQueue[index].entry++;   /* one more element   */

    /* element's copy in fifo */
    memcpy((unsigned char *) (TDAL_mQueue[index].queue) + (TDAL_mQueue[index].msgsize * TDAL_mQueue[index].tail), msg, len);

    TDAL_mQueue[index].tail++;   /* tail is updated   */
    if (TDAL_mQueue[index].tail >= TDAL_mQueue[index].count)
	{
        TDAL_mQueue[index].tail = 0;
	}

    bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
    mTBOX_ASSERT(TRUE == bfRet);

    return 0;   /* Success */
}

/*=============================================================================
 *
 * Function : GetFifo
 *
 * Parameters:
 *      FIFO index
 *      restored message
 *
 * Description:
 *      Restore a message from Fifo
 *
 * Returns:
 *      0 : OK
 *      -1 : error
 *
 *===========================================================================*/
static int TDALi_GetFifo(int index, unsigned char *msg)
{
    MS_BOOL bfRet = FALSE;
    bfRet = MsOS_ObtainMutex(TDAL_mQueueMutex, -1);
    mTBOX_ASSERT(TRUE == bfRet);

    if (TDAL_mQueue[index].entry <= 0)   /* empty : Error */
    {
        bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
        mTBOX_ASSERT(TRUE == bfRet);
        mTBOX_TRACE((kTBOX_NIV_WARNING, "GetFifo : fifo is empty\n"));
        return (-1);
    }

    TDAL_mQueue[index].entry--;

    /* element's copy out fifo */
    memcpy(msg, (unsigned char *) (TDAL_mQueue[index].queue) + (TDAL_mQueue[index].msgsize * TDAL_mQueue[index].head), TDAL_mQueue[index].msgsize);

    TDAL_mQueue[index].head++;   /* head is updated   */
    if (TDAL_mQueue[index].head >= TDAL_mQueue[index].count)
	{
		TDAL_mQueue[index].head = 0;
	}

    bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
    mTBOX_ASSERT(TRUE == bfRet);

    return 0;   /* Success */
}

#if MSTAR_QUEUE == 0
/********************************************************/
/*          Functions Definitions (GLOBAL)        */
/********************************************************/
bool TDAL_CreateQueue(TDAL_qdepth maxNumber,
                  TDAL_qwidth msgSize,
                  TDAL_queue_id *qid)
{
   int index;
    MS_BOOL bfRet = FALSE;

    if ((!maxNumber) || (!msgSize) || (!qid))
    {
       return false;
    }

   /* first time initialization */
   if (TDAL_mInitQueue == 0)
   {
       TDAL_mInitQueue = 1;
       TDAL_mQueueMutex = MsOS_CreateMutex(E_MSOS_FIFO, NULL, 0);
       mTBOX_ASSERTm(TDAL_mQueueMutex > 0, "TDAL_mQueueMutex mutex not created\n");
	bfRet = MsOS_ObtainMutex(TDAL_mQueueMutex, -1);
        mTBOX_ASSERT(TRUE == bfRet);

       TDAL_mNbQueue = 0;
       for (index = 0; index < TDAL_NB_QUEUES; index++)
       {
		TDAL_mQueue[index].queue = NULL;
		TDAL_mQueue[index].entry = 0;
		TDAL_mQueue[index].head = 0;
		TDAL_mQueue[index].tail = 0;
		TDAL_mQueue[index].count = 0;
		TDAL_mQueue[index].msgsize = 0;
		TDAL_mQueue[index].sema = NULL;
       }
   }
   else
   {
	bfRet = MsOS_ObtainMutex(TDAL_mQueueMutex, -1);
        mTBOX_ASSERT(TRUE == bfRet);
   }

   for (index = 0; index < TDAL_NB_QUEUES; index++)
   {
	if (TDAL_mQueue[index].queue == NULL)
	{
           /* found a free slot */
           /* Create a FIFO */
           TDAL_mQueue[index].queue = TDAL_Malloc(maxNumber * msgSize);

           /* create a semaphore */
	if (TDAL_mQueue[index].queue)
	{
               TDAL_mQueue[index].sema = MsOS_CreateSemaphore(1,E_MSOS_FIFO,"TDAL_mQueue_Sem");
               if (TDAL_mQueue[index].sema < 0)
               {
                   TDAL_Free(TDAL_mQueue[index].queue);
                   TDAL_mQueue[index].queue = NULL;
               }
        }

        if (TDAL_mQueue[index].queue) 
        {
               TDAL_mQueue[index].count = maxNumber;
               TDAL_mQueue[index].head = 0;
               TDAL_mQueue[index].tail = 0;
               TDAL_mQueue[index].entry = 0;
               TDAL_mQueue[index].msgsize = msgSize;
               TDAL_mNbQueue++;
               *qid = index + 1;   /* queue identifier = index of TDAL_mQueue[] + 1 (never 0) */
	}
	else
	{
		bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
                mTBOX_ASSERT(TRUE == bfRet);
               return false;
           }
           break;
       }
   }/* end for loop */

	bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
    mTBOX_ASSERT(TRUE == bfRet);

	if (index >= TDAL_NB_QUEUES)
	{
             return false;
        }

   return true;
}

/* ======================================================================== */
bool TDAL_DeleteQueue(TDAL_queue_id qid)
{
    bool ret = true;
    int index = qid - 1;
    MS_BOOL bfRet = FALSE;

    if ((index < 0) || (index > TDAL_NB_QUEUES))
	{
        return false;
    }

    if (!TDAL_mNbQueue)
	{
        return false;
    }

    bfRet = MsOS_ObtainMutex(TDAL_mQueueMutex, -1);
    mTBOX_ASSERT(TRUE == bfRet);

    if (TDAL_mQueue[index].queue)
	{
        if (MsOS_DeleteSemaphore(TDAL_mQueue[index].sema) == FALSE)
	{
            ret = false;   /* the mutex associated to this queue is still kept by an Enqueue/Dequeue function */
            mTBOX_ASSERT(TRUE == ret);
	}
        else
        {
            TDAL_Free(TDAL_mQueue[index].queue);   /* Delete the FIFO */
            TDAL_mQueue[index].queue = 0;
            TDAL_mQueue[index].head = 0;
            TDAL_mQueue[index].tail = 0;
            TDAL_mQueue[index].entry = 0;
            TDAL_mQueue[index].count = 0;
            TDAL_mQueue[index].msgsize = 0;
            TDAL_mQueue[index].sema = NULL;

            TDAL_mNbQueue--;
            ret = true;
        }
    }
    else
	{
            ret = false;
        }

	bfRet = MsOS_ReleaseMutex(TDAL_mQueueMutex);
    mTBOX_ASSERT(TRUE == bfRet);
   return ret;
}

/* ======================================================================== */
bool TDAL_Enqueue(TDAL_queue_id qid,void *msg)
{
   int index = qid - 1;


   if (!TDAL_mNbQueue)
   {
       return false;
   }

   if ((index < 0) || (index >= TDAL_NB_QUEUES) || (!msg))
   {
       return false;
   }
   else if (!TDAL_mQueue[index].queue)
   {
       return false;
   }

   if (TDALi_AddFifo(index, TDAL_mQueue[index].msgsize,(unsigned char *) msg))
   {
       return false;
   }

   return true;
}

/* ======================================================================== */
bool TDAL_Dequeue(TDAL_queue_id qid,void *msg_buf)
{
    int      index = qid - 1;

    if (!TDAL_mNbQueue)
    {
        return false;
    }

    if ((index < 0) || (index >= TDAL_NB_QUEUES))
    {
        return false;
    }
    else if (!TDAL_mQueue[index].queue)
    {
        return false;
    }

    if (MsOS_ObtainSemaphore(TDAL_mQueue[index].sema, MSOS_WAIT_FOREVER) == TRUE)
    {
        if (!TDALi_GetFifo(index, (unsigned char *) msg_buf))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
#endif


LOCAL void TDALi_WathchdogTimerFeeder (MS_U32 u32StTimer, MS_U32 u32TimerID)  ///< Timer callback function  u32StTimer: not used; u32TimerID: Timer ID
{
    WDT_Result efRet;
    if (WatchdogAllowFeed)
    {
        efRet = MDrv_WDT_Clear();
        mTBOX_ASSERT(E_WDT_OK == efRet);
    }
}

LOCAL void TDALi_StartWatchdog(void)
{
    WDT_Result efRet;
    if (!WatchdogStarted)
    {
        WatchdogAllowFeed = TRUE;

        efRet = MDrv_WDT_Init(E_WDT_DBGLV_ALL);
        mTBOX_ASSERT(E_WDT_OK == efRet);
        efRet = MDrv_WDT_SetTimer(E_WDT_DBGLV_ALL, WATCHDOG_TIMEOUT);
        mTBOX_ASSERT(E_WDT_OK == efRet);

        WatchdogTimer = MsOS_CreateTimer(TDALi_WathchdogTimerFeeder, 0, WATCHDOG_FEED_TIME * 1000, TRUE, "WatchdogTimerFeeder");

        WatchdogStarted = TRUE;
    }
}

GLOBAL tTDAL_OTA_ErrorCode TDAL_Diag_GetInfo(tTDAL_Diag_InfoType diagInfoType, tTDAL_Diag_Handle pProperStructure)
{
    tTDAL_Diag_HWInfo *pHardwareInfo = NULL;
    tTDAL_Diag_SWInfo *pSoftwareInfo = NULL;
    unsigned char hwconfigData[1024+4] = {0};
    unsigned int stbSerialNumber = 0;
    unsigned char SerialNumber[4] = {0};
    int ret = 0;
	CH_LoaderInfo_t rpstru_LoaderInfo={0};
	tTDAL_OTA_ErrorCode	enm_ChComRet;
	CH_SystemInfo_t gstru_SystemInfo={0};

    printf("[%s %d]enter diagInfoType=%d \n",__FUNCTION__,__LINE__,diagInfoType);

    if ((diagInfoType < eTDAL_DIAG_HW_INFO) || (diagInfoType > eTDAL_DIAG_SW_INFO))
    {
        return eTDAL_OTA_STATUS_ERROR;
    }

    if (pProperStructure == NULL)
    {
        return eTDAL_OTA_STATUS_ERROR;
    }
	enm_ChComRet = CH_COM_GetLoaderInfo(&rpstru_LoaderInfo);
	enm_ChComRet |= CH_COM_GetSystemInfo( &gstru_SystemInfo );
	if(enm_ChComRet != eTDAL_OTA_STATUS_NO_ERROR)
	{
		U8 TempBuf[20] = {"0"};

		memset(&gstru_SystemInfo,0,sizeof(gstru_SystemInfo));

		strcpy((char*)gstru_SystemInfo.uc_name,"System Info");
		gstru_SystemInfo.i_length = sizeof(gstru_SystemInfo) - 16;


		memcpy(gstru_SystemInfo.uc_STBSN,TempBuf,15 );
		printf("\n ================CH_COM_GetSystemInfo fail\n");

		//CH_COM_SetSystemInfo(&gstru_SystemInfo);
	}
	gstru_SystemInfo.uc_STBSN[16] = 0;

    memset(&hwconfigData,0,sizeof(hwconfigData));
    memset(hwconfigData,0,sizeof(hwconfigData));
    memset(SerialNumber,0,sizeof(SerialNumber));

    if (diagInfoType == eTDAL_DIAG_HW_INFO)
    {
        pHardwareInfo = (tTDAL_Diag_HWInfo *)pProperStructure;

        sprintf(pHardwareInfo->cHardwareVersion,"%x",(unsigned char*)rpstru_LoaderInfo.ui_Hardware);
        pHardwareInfo->uiHardwareVersionId = (uint16_t)HARDWARE_VERSION_ID;

        strcpy(pHardwareInfo->cHardwareModel,(unsigned char*)HARDWARE_MODEL);
        pHardwareInfo->uiHardwareModelId = (uint16_t)HARDWARE_MODEL_ID;

        strcpy(pHardwareInfo->cHardwareDeviceName,(unsigned char*)HARDWARE_DEVICE_NAME);
        pHardwareInfo->uiHardwareDeviceNameId = HARDWARE_DEVICE_NAME_ID;

        strcpy(pHardwareInfo->cHardwareManufacturer,(unsigned char*)HARDWARE_MANUFACTURER);
        pHardwareInfo->uiHardwareManufacturerId = HARDWARE_MANUFACTURER_ID;

        strcpy(pHardwareInfo->cOUI,(unsigned char*)HARDWARE_MANUFACTURER);
        pHardwareInfo->uiOUIId = HARDWARE_MANUFACTURER_ID;

        /*read the stb serial number info from flash 0x80070*/
//        ret = MDrv_SERFLASH_Read(HWCONFIG_FALG_PARTITION_ADDR+HWCONFIG_AREA_STB_SERIAL_NUMBER,4,SerialNumber);
//        if ( ret != TRUE)
//        {
//            return eTDAL_OTA_STATUS_ERROR;
//        }

        stbSerialNumber = 76667;//(SerialNumber[0]|(SerialNumber[1]<<8)|(SerialNumber[2]<<16)|(SerialNumber[3]<<24));
        sprintf(pHardwareInfo->cHardwareSTBSerialNumber,"%s",gstru_SystemInfo.uc_STBSN);//020236150000400
        printf("[%s %d]stb sn=[%s][%s] \n",__FUNCTION__,__LINE__,pHardwareInfo->cHardwareSTBSerialNumber,pHardwareInfo->cHardwareVersion);

        return eTDAL_OTA_STATUS_NO_ERROR;

    }
    else //eTDAL_DIAG_SW_INFO
    {
        pSoftwareInfo = (tTDAL_Diag_SWInfo *)pProperStructure;
        sprintf(pSoftwareInfo->cDriverVersion,"%02d.%02d.%02d.%02d",(rpstru_LoaderInfo.ui_Software & 0xff000000) >>24,
		((rpstru_LoaderInfo.ui_Software & 0x00ff0000) >>16),
		((rpstru_LoaderInfo.ui_Software & 0x0000ff00) >>8),
		(rpstru_LoaderInfo.ui_Software & 0x000000ff)
		);
        pSoftwareInfo->uiDriverVersionId = rpstru_LoaderInfo.ui_Software;

        strcpy(pSoftwareInfo->cLoaderVersion,(unsigned char*)LOADER_VERSION);
        pSoftwareInfo->uiLoaderVersionId = LOADER_VERSION_ID;

        strcpy(pSoftwareInfo->cLauncherVersion,(unsigned char*)LAUNCHER_VERSION);
        pSoftwareInfo->uiLauncherVersionId = LAUNCHER_VERSION_ID;
        printf("[%s %d]stb sn=[%s] \n",__FUNCTION__,__LINE__,pSoftwareInfo->cDriverVersion);

        return eTDAL_OTA_STATUS_NO_ERROR;
    }
}
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_GetParameters(tTDAL_OTA_StatusParameters *otaParam)
{
	CH_LoaderInfo_t rpstru_LoaderInfo={0};
	tTDAL_OTA_ErrorCode	enm_ChComRet;
	
    char                    cStatus[10]={0};   /* Success, Failed, Suspended */
    char                    cErrorCode[5] = {0}; /* Error code format: EXXX */
    if (otaParam == NULL)
    {
        return eTDAL_OTA_STATUS_ERROR;
    }
	enm_ChComRet = CH_COM_GetLoaderInfo(&rpstru_LoaderInfo);

	if(enm_ChComRet != eTDAL_OTA_STATUS_NO_ERROR)
	{
		printf("\n ============CH_COM_GetLoaderInfo fail\n");
	}
	otaParam->bInStandBy  = !rpstru_LoaderInfo.ui_TimeCode;
	if(rpstru_LoaderInfo.ui_DateCode > 0xFFF)
	{
		rpstru_LoaderInfo.ui_DateCode = rpstru_LoaderInfo.ui_DateCode%0xfff;
	}
	if(rpstru_LoaderInfo.ui_DateCode == 0 )
	{
		otaParam->eErrorStatus = eTDAL_OTA_STATUS_NO_ERROR;
		sprintf(otaParam->cStatus,"%s","Success");
		sprintf(otaParam->cErrorCode,"%d",0);
	}
	else
	{
		otaParam->eErrorStatus = eTDAL_OTA_STATUS_ERROR;
		sprintf(otaParam->cStatus,"%s","Failed");
		sprintf(otaParam->cErrorCode,"E%03X",rpstru_LoaderInfo.ui_DateCode);
	}
	return eTDAL_OTA_STATUS_NO_ERROR;
}

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
tTDAL_OTA_ErrorCode TDAL_GetHWParameters(tTDAL_HWDataType hwDataType, tTDAL_HW_Handle pProperStructure)
{
    mTBOX_FCT_ENTER("TDAL_GetHWParameters");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_OTA_GetDiagnosticStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_OTA_STATUS_NO_ERROR;
}

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_Initiate(tTDAL_OTAType otaType, tTDAL_OTAStandbyState otaStandbyState)
{
    mTBOX_FCT_ENTER("TDAL_OTA_Initiate");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_OTA_GetDiagnosticStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_OTA_STATUS_NO_ERROR;
}

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
void TDAL_GetLoadVersion(unsigned char *pcLoadVersion)
{
    mTBOX_FCT_ENTER("TDAL_GetLoadVersion");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_GetLoadVersion NOT IMPLEMENTED\n"));
    mTBOX_RETURN;
}

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/

tTDAL_OTA_ErrorCode TDAL_OTA_GetDiagnosticStatus(tTDAL_DiagnosticOTAStatus *pOTAStatus)
{
    mTBOX_FCT_ENTER("TDAL_OTA_GetDiagnosticStatus");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_OTA_GetDiagnosticStatus NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_OTA_STATUS_NO_ERROR;
}

/*
* \depricated function is going to be removed from the API, as it belongs to older API version.
*/
tTDAL_OTAStandbyState TDAL_OTA_GetStandbyState(void)
{
    mTBOX_FCT_ENTER("TDAL_OTA_GetStandbyState");
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_OTA_GetStandbyState NOT IMPLEMENTED\n"));
    mTBOX_RETURN eTDAL_STANDBY_OFF;
}
/*************************globle variable define*************************/
#ifdef USE_TDAL_OTA
//#if 1
const CH_COM_Params_t gstru_fact_SaveParams[CH_COM_INFOTYPE_MAX] =
{
	/*0:CH_COM_INFOTYPE_LOADER*/
	{
		CH_COM_INFOTYPE_LOADER,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X101000,
			0X80,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X781000,
			0X80,
		}
	},
	/*2:CH_COM_INFOTYPE_UPDATE*/
	{
		CH_COM_INFOTYPE_UPDATE,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X108000,
			0X100,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X788000,
			0X100,
		},

	},
	/*3:CH_COM_INFOTYPE_SYSTEM*/
	{
		CH_COM_INFOTYPE_SYSTEM,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X10C000,
			0X600,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X78C000,
			0X600,
		},

	},
	/*4:CH_COM_INFOTYPE_FACTBOOT*/
	{
		CH_COM_INFOTYPE_FACTBOOT,
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X104000,
			0X80,
		},
		true,
		{
			CH_COM_SAVEDEVICE_FLASH,
			0,
			NULL,
			0X784000,
			0X80,
		},
	}

};

unsigned long ulTable_MPEG32[256] =
{
   0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L,
   0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L,
   0x2608EDB8L, 0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L,
   0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL,
   0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L,
   0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L,
   0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L,
   0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL,
   0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L,
   0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
   0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L,
   0xAD2F2D84L, 0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL,
   0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L,
   0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L, 0xCA753D95L,
   0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L,
   0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL,
   0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL,
   0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
   0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L,
   0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
   0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL,
   0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L,
   0x5E9F46BFL, 0x5A5E5B08L, 0x571D7DD1L, 0x53DC6066L,
   0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL,
   0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL,
   0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L,
   0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L,
   0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL,
   0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL,
   0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
   0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L,
   0xD5B88683L, 0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL,
   0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L,
   0x7A089632L, 0x7EC98B85L, 0x738AAD5CL, 0x774BB0EBL,
   0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL,
   0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
   0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L,
   0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL,
   0x0315D626L, 0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL,
   0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
   0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L,
   0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL,
   0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L, 0xDA649D6FL,
   0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L,
   0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L,
   0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL,
   0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL,
   0x8832161AL, 0x8CF30BADL, 0x81B02D74L, 0x857130C3L,
   0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L,
   0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
   0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L,
   0x68860BFDL, 0x6C47164AL, 0x61043093L, 0x65C52D24L,
   0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L,
   0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
   0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L,
   0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L,
   0xC5A92679L, 0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L,
   0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL,
   0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL, 0xEEE2ED18L,
   0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
   0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L,
   0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL,
   0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L,
   0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L
};

U32   CHDRV_FLASH_DirectWrite(U32 Address, U8* Buffer, U32 NumberToWrite)
{
	U32 enm_FlashResult = 0;

	enm_FlashResult = TDAL_FLA_Write_OTA(Address, Buffer, NumberToWrite);

	return enm_FlashResult;
}

U32 CRC_MPEG32_(U8 * pvStartAddress, U32 ulSize_in_bytes)
{
	unsigned long ulCRC;
	U8 * pbData;
	U32 ulSize_in_bytes_;

	/**********************
	* Lookup Table method *
	**********************/
	ulSize_in_bytes_  =    ulSize_in_bytes;

	/* init the start value */
	ulCRC = 0xFFFFFFFF;

	pbData = pvStartAddress;

	/* calculate CRC */
	while (ulSize_in_bytes_--)
	{
		ulCRC = ulTable_MPEG32[((ulCRC>>24) ^ *pbData++) & 0xFFL] ^ (ulCRC << 8);
	}
	return ((U32)ulCRC);

}  /* CRC_MPEG32 */

tTDAL_OTA_ErrorCode CH_COM_SetOTAInfo(CH_UpdateInfo_t *rpstru_OTAInfo)
{
		tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 13:59:46 --!>*/
	int enm_FlashResult = 0;
	S32 i_WriteSize = 0;
	S32 i_ActualWriteSize = 0;
	S32 i_ActualBackupWriteSize = 0;
	U32 ui_Crc = 0;

	if( rpstru_OTAInfo == NULL )
	{
		return eTDAL_OTA_STATUS_ERROR;
	}
	if( strcmp(rpstru_OTAInfo->uc_name,UPDATE_INFO_MAGIC) )
	{
		return eTDAL_OTA_STATUS_ERROR;
	}


		printf("\nset CH_COM_SetOTAInfo 1111\n");

		i_WriteSize = sizeof(CH_UpdateInfo_t);
		i_ActualWriteSize = i_WriteSize;
		i_ActualBackupWriteSize = i_WriteSize;

		rpstru_OTAInfo->i_length = sizeof(CH_UpdateInfo_t) -16;
		ui_Crc = CRC_MPEG32_((U8 *)&(rpstru_OTAInfo->enm_AntType), rpstru_OTAInfo->i_length -4);
		rpstru_OTAInfo->ui_crc = ui_Crc;

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
		{
			
			enm_FlashResult = TDAL_FLA_Write_OTA( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_OTAInfo,i_ActualWriteSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->08-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}

			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress != -1 )
			{
				enm_FlashResult = TDAL_FLA_Write_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress ,(U8 *)rpstru_OTAInfo,i_ActualBackupWriteSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->09-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
			} 
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->12-->XXXX\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_UPDATE].stru_BackUpInfo.ui_OffsetAddress != -1 )
		{
			if( i_ActualBackupWriteSize != i_ActualWriteSize )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->13-->XXXX\r\n"));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}
		if( i_ActualWriteSize != i_WriteSize )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetOTAInfo-->14-->i_WriteSize=%d,i_ActualWriteSize=%d\r\n",i_WriteSize,i_ActualWriteSize));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
		}
#endif 	/*<!-- gongwenqing 2013/12/13 13:59:46 --!>*/
	FAIL0: 
	return enm_Result;
}



tTDAL_OTA_ErrorCode CH_COM_SetSystemInfo(CH_SystemInfo_t *rpstru_SystemInfo)
{
	tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1	/*<!-- gongwenqing 2013/12/13 14:01:01 --!>*/
	int enm_FlashResult = 0;
	S32 i_WriteSize = 0;
	S32 i_ActualWriteSize = 0;
	S32 i_ActualBackupWriteSize = 0;
	U32 ui_Crc = 0;

	/*判断是否已经初始化*/

	if( rpstru_SystemInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->02-->rpstru_SystemInfo[0x%x]\r\n",rpstru_SystemInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}

	if( strcmp(rpstru_SystemInfo->uc_name,SYSTEM_INFO_MAGIC) )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->03-->uc_name Error\r\n"));
		return eTDAL_OTA_STATUS_ERROR;
	}

	i_WriteSize = sizeof(CH_SystemInfo_t);
	i_ActualWriteSize = i_WriteSize;
	i_ActualBackupWriteSize = i_WriteSize;

	rpstru_SystemInfo->i_length = sizeof(CH_SystemInfo_t) -16;
	ui_Crc = CRC_MPEG32_((U8 *)&(rpstru_SystemInfo->uc_STBSN[0]), rpstru_SystemInfo->i_length -4);
	rpstru_SystemInfo->ui_CRC = ui_Crc;

	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
	{
		enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_SystemInfo,i_ActualWriteSize);
		if( enm_FlashResult == 0 )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->09-->Result[%d]\r\n",enm_FlashResult));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress != -1 )
		{
			enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress,(U8 *)rpstru_SystemInfo,i_ActualBackupWriteSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->10-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->13-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}

	if( i_ActualBackupWriteSize != i_ActualWriteSize )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->14-->i_ActualWriteSize[%d],i_ActualBackupWriteSize[%d]\r\n",
			i_ActualWriteSize,i_ActualBackupWriteSize));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}

	if( i_ActualWriteSize != i_WriteSize )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetSystemInfo-->15-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}
FAIL0:

#endif 	/*<!-- gongwenqing 2013/12/13 14:01:01 --!>*/

	return enm_Result;
}

tTDAL_OTA_ErrorCode CH_COM_GetSystemInfo(CH_SystemInfo_t *rpstru_SystemInfo)
{
	tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 14:00:51 --!>*/
	int enm_FlashResult = 0;
	CH_SystemInfo_t stru_BackupSystemInfo;
	S32 i_ReadSize = 0;
	S32 i_ActualReadSize = 0;
	S32 i_ActualBackupReadSize = 0;
	U32 ui_CrcValue = 0;
	bool b_UseBackup = false;
	bool b_MainSaveOK = false;
	bool b_BackSaveOK = false;

	/*判断是否已经初始化*/
	if( rpstru_SystemInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->02-->rpstru_SystemInfo[0x%x]\r\n",rpstru_SystemInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}

	memset(&stru_BackupSystemInfo,0,sizeof(CH_SystemInfo_t));



	memset(&stru_BackupSystemInfo,0,sizeof(CH_SystemInfo_t));

	i_ReadSize = sizeof(CH_SystemInfo_t);
	i_ActualReadSize = i_ReadSize;
	i_ActualBackupReadSize = i_ReadSize;

	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
	{
		enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_SystemInfo,i_ActualReadSize);
		if( enm_FlashResult == 0 )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->08-->Result[%d]\r\n",enm_FlashResult));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress != -1 )
		{
			enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress,(U8 *)&stru_BackupSystemInfo,i_ActualBackupReadSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->09-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
			b_UseBackup = true;
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->12-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}

	/*判断读出来的数据是否有长度信息*/
	if( rpstru_SystemInfo->i_length < 4 ||rpstru_SystemInfo->i_length > ( i_ActualReadSize -16 ) )
	{
		
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->3-->i_length[%d],i_ActualReadSize[%d]\r\n",
			rpstru_SystemInfo->i_length,i_ActualReadSize));
		b_MainSaveOK = false;/*读出来的数据有问题*/
	}
	else
	{
		/*校验主数据*/
		ui_CrcValue = CRC_MPEG32_((U8 *)&(rpstru_SystemInfo->uc_STBSN[0]), rpstru_SystemInfo->i_length -4);
		if(ui_CrcValue == rpstru_SystemInfo->ui_CRC)
		{
			if(strcmp(rpstru_SystemInfo->uc_name,SYSTEM_INFO_MAGIC) == 0)
			{
				b_MainSaveOK = true;
			}
		}
		
	}
	if( b_UseBackup == true )
	{
		if(stru_BackupSystemInfo.i_length < 4 ||stru_BackupSystemInfo.i_length > ( i_ActualBackupReadSize -16 ) )
		{
			b_BackSaveOK = false;  /*读出来的数据有问题*/
		}
		else
		{
			/*校验备份数据*/
			ui_CrcValue = CRC_MPEG32_((U8 *)&(stru_BackupSystemInfo.uc_STBSN[0]), stru_BackupSystemInfo.i_length - 4);
			if(ui_CrcValue == stru_BackupSystemInfo.ui_CRC)
			{
				if(strcmp(stru_BackupSystemInfo.uc_name,SYSTEM_INFO_MAGIC) == 0)
				{
					b_BackSaveOK = true;
				}
			}
		}
		/*如果数据都出错，则返回错误*/
		if(!(b_BackSaveOK || b_MainSaveOK))
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->13-->b_BackSaveOK[%d],b_MainSaveOK[%d]\r\n",
				b_BackSaveOK,b_MainSaveOK));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		/*如果备份出错或两个都对，但不一致，则修复备份*/
		if((b_BackSaveOK== false) ||
			(memcmp((U8*)rpstru_SystemInfo, (U8*)&stru_BackupSystemInfo, sizeof(CH_SystemInfo_t)) && b_MainSaveOK))
		{
			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
			{
				enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_BackUpInfo.ui_OffsetAddress,
														(U8*)rpstru_SystemInfo,i_ActualReadSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->14-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
			}
		}

		/*主数据出错，使用备份数据，并修复主数据*/
		if(b_MainSaveOK == false)
		{
			memcpy((U8*)rpstru_SystemInfo, (U8*)&stru_BackupSystemInfo, sizeof(CH_SystemInfo_t));
			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
			{
				enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_SYSTEM].stru_MainInfo.ui_OffsetAddress,
														(U8*)rpstru_SystemInfo,i_ActualBackupReadSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->16-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
			}
		}
	}
	else
	{
		if(b_MainSaveOK == false)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetSystemInfo-->18-->XXXX\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
	}

FAIL0:


#endif 	/*<!-- gongwenqing 2013/12/13 14:00:51 --!>*/

	return enm_Result;
}

/*******************************************************************************
*函数名　　　　:  CH_COM_SetLoaderInfo
*函数功能　　　:  存储LOADER信息
*输入参数　　　:  无
*输出参数　　　:
*		*rpstru_LoaderInfo:LOADER信息
*返回值说明　　:
*　　	CH_COM_NOT_INIT:未初始化
*	    	eTDAL_OTA_STATUS_ERROR:非法参数
*		eTDAL_OTA_STATUS_ERROR:操作失败
*　　	eTDAL_OTA_STATUS_NO_ERROR:操作成功
*全局变量和结构:
*算法说明　　　:  无
*调用注意事项　:  无
*******************************************************************************/
tTDAL_OTA_ErrorCode CH_COM_SetLoaderInfo(CH_LoaderInfo_t *rpstru_LoaderInfo)
{
	tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 13:59:11 --!>*/
	int enm_FlashResult = 0;
	S32 i_WriteSize = 0;
	S32 i_ActualWriteSize = 0;
	S32 i_ActualBackupWriteSize = 0;
	U32 ui_Crc = 0;

	if( rpstru_LoaderInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo -->02-->rpstru_LoaderInfo=0x%x\n",rpstru_LoaderInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}
	if( strcmp(rpstru_LoaderInfo->uc_name,LOADER_INFO_MAGIC) )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo -->03-->uc_name Error\n"));
		return eTDAL_OTA_STATUS_ERROR;
	}




	i_WriteSize = sizeof(CH_LoaderInfo_t);
	i_ActualWriteSize = i_WriteSize;
	i_ActualBackupWriteSize = i_WriteSize;

	rpstru_LoaderInfo->i_length = sizeof(CH_LoaderInfo_t) -16;
	ui_Crc = CRC_MPEG32_((U8 *)&(rpstru_LoaderInfo->ui_OperatorNumber), rpstru_LoaderInfo->i_length - 4);
	rpstru_LoaderInfo->ui_crc = ui_Crc;
	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
	{
		enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_LoaderInfo,i_ActualWriteSize);
		if( enm_FlashResult == 0 )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->09-->Result[%d]\r\n",enm_FlashResult));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1)
		{
			enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress,(U8 *)rpstru_LoaderInfo,i_ActualBackupWriteSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->10-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->13-->XXXX\r\n"));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}


	if( i_ActualWriteSize != i_WriteSize )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->14-->i_ActualWriteSize=%d,i_WriteSize=%d\r\n",i_ActualWriteSize,i_WriteSize));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}
	if( (gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1)&& i_ActualBackupWriteSize != i_WriteSize)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->15-->XXXX[%d]-[%d]\r\n",i_ActualBackupWriteSize,i_WriteSize));
		enm_Result = eTDAL_OTA_STATUS_ERROR;
		goto FAIL0;
	}
FAIL0:

	
#endif 	/*<!-- gongwenqing 2013/12/13 13:59:11 --!>*/

	return enm_Result;
}

tTDAL_OTA_ErrorCode CH_COM_GetLoaderInfo(CH_LoaderInfo_t *rpstru_LoaderInfo)
{
		tTDAL_OTA_ErrorCode enm_Result = eTDAL_OTA_STATUS_NO_ERROR;
		
		int enm_FlashResult = eTDAL_OTA_STATUS_NO_ERROR;
#if 1 	/*<!-- gongwenqing 2013/12/13 13:59:24 --!>*/
	S32 i_ReadSize = 0;
	S32 i_ActualReadSize = 0;
	S32 i_ActualBackupReadSize = 0;
	U32 ui_CrcValue = 0;
	CH_LoaderInfo_t stru_BackupLoaderInfo;
	bool b_UseBackup = false;
	bool b_MainSaveOK = false;
	bool b_BackSaveOK = false;


	if( rpstru_LoaderInfo == NULL )
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo -->02-->rpstru_LoaderInfo=0x%x\n",rpstru_LoaderInfo));
		return eTDAL_OTA_STATUS_ERROR;
	}

	memset(&stru_BackupLoaderInfo,0,sizeof(CH_LoaderInfo_t));

	if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].b_Used == true )
	{



		i_ReadSize = sizeof(CH_LoaderInfo_t);
		i_ActualReadSize = i_ReadSize;
		i_ActualBackupReadSize = i_ReadSize;
		
		if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH )
		{
			enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,(U8 *)rpstru_LoaderInfo,i_ActualReadSize);
			if( enm_FlashResult == 0 )
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->08-->Result[%d]\r\n",enm_FlashResult));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
			if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1 )
			{
				enm_FlashResult = TDAL_FLA_Read_OTA(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress,(U8 *)&stru_BackupLoaderInfo,i_ActualBackupReadSize);
				if( enm_FlashResult == 0 )
				{
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->09-->Result[%d]\r\n",enm_FlashResult));
					enm_Result = eTDAL_OTA_STATUS_ERROR;
					goto FAIL0;
				}
				b_UseBackup = true;
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->12-->XXXX\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		/*判断读出来的数据是否有长度信息*/
		if( rpstru_LoaderInfo->i_length < 4 ||rpstru_LoaderInfo->i_length > ( i_ActualReadSize -16 ) )
		{
			b_MainSaveOK = false;/*读出来的数据有问题*/
		}
		else
		{
			/*校验主数据*/
			ui_CrcValue = CRC_MPEG32_((U8 *)&(rpstru_LoaderInfo->ui_OperatorNumber), rpstru_LoaderInfo->i_length -4);
			if(ui_CrcValue == rpstru_LoaderInfo->ui_crc)
			{
				if(strcmp(rpstru_LoaderInfo->uc_name,LOADER_INFO_MAGIC) == 0)
				{
					b_MainSaveOK = true;
				}
			}
			
		}
		/*如果使用了备份分区，则进行备份数据校验*/
		if(b_UseBackup == true)
		{
			if(stru_BackupLoaderInfo.i_length < 4 ||stru_BackupLoaderInfo.i_length > ( i_ActualBackupReadSize -16 ) )
			{
				b_BackSaveOK = false;  /*读出来的数据有问题*/
			}
			else
			{
				/*校验备份数据*/
				ui_CrcValue = CRC_MPEG32_((U8 *)&(stru_BackupLoaderInfo.ui_OperatorNumber), stru_BackupLoaderInfo.i_length - 4);
				if(ui_CrcValue == stru_BackupLoaderInfo.ui_crc)
				{
					if(strcmp(stru_BackupLoaderInfo.uc_name,LOADER_INFO_MAGIC) == 0)
					{
						b_BackSaveOK = true;
					}
				}
				
			}
			/*如果数据都出错，则返回错误*/
			if(!(b_BackSaveOK || b_MainSaveOK))
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->13-->b_BackSaveOK=%d,b_MainSaveOK=%d\r\n",b_BackSaveOK,b_MainSaveOK));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}

			/*如果备份出错或两个都对，但不一致，则修复备份*/
			if((b_BackSaveOK== false) ||
				(memcmp((U8*)rpstru_LoaderInfo, (U8*)&stru_BackupLoaderInfo, sizeof(CH_LoaderInfo_t)) && b_MainSaveOK))
			{
				if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
				{
					enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,
															(U8*)rpstru_LoaderInfo,i_ActualReadSize);
					if( enm_FlashResult == 0 )
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->14-->Result[%d]\r\n",enm_FlashResult));
						enm_Result = eTDAL_OTA_STATUS_ERROR;
						goto FAIL0;
					}
				}
			}

			/*主数据出错，使用备份数据，并修复主数据*/
			if(b_MainSaveOK == false)
			{
				memcpy((U8*)rpstru_LoaderInfo, (U8*)&stru_BackupLoaderInfo, sizeof(CH_LoaderInfo_t));
				if( gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.enm_Device == CH_COM_SAVEDEVICE_FLASH)
				{
					enm_FlashResult = CHDRV_FLASH_DirectWrite(gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_MainInfo.ui_OffsetAddress,
															(U8*)rpstru_LoaderInfo,i_ActualBackupReadSize);
					if( enm_FlashResult == 0 )
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->16-->Result[%d]\r\n",enm_FlashResult));
						enm_Result = eTDAL_OTA_STATUS_ERROR;
						goto FAIL0;
					}
				}
			}
		}
		else
		{
			if(b_MainSaveOK == false)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->18-->XXXX\r\n"));
				enm_Result = eTDAL_OTA_STATUS_ERROR;
				goto FAIL0;
			}
		}


		if( i_ActualReadSize != i_ReadSize )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->19-->i_ActualReadSize=%d,i_ReadSize=%d\r\n",i_ActualReadSize,i_ReadSize));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}

		if( (gstru_fact_SaveParams[CH_COM_INFOTYPE_LOADER].stru_BackUpInfo.ui_OffsetAddress != -1)&& i_ActualBackupReadSize != i_ReadSize)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_SetLoaderInfo-->20-->i_ActualReadSize=%d,i_ReadSize=%d\r\n",i_ActualBackupReadSize,i_ReadSize));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
		if( strcmp(rpstru_LoaderInfo->uc_name,LOADER_INFO_MAGIC) )
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"CH_COM_GetLoaderInfo-->21-->uc_name Error\r\n"));
			enm_Result = eTDAL_OTA_STATUS_ERROR;
			goto FAIL0;
		}
		}
FAIL0:


#endif 	/*<!-- gongwenqing 2013/12/13 13:59:24 --!>*/

	return enm_Result;
}

GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_SetParameters(tTDAL_OTA_ControlParameters *otaParam)
{
	tTDAL_OTA_ErrorCode	enm_ChComRet;
	CH_UpdateInfo_t		gsstru_OTAUpdateInfo;
	CH_LoaderInfo_t rpstru_LoaderInfo={0};
	CH_SystemInfo_t     gstru_SystemInfo = {0};
    if (otaParam == NULL)
    {
        return eTDAL_OTA_STATUS_ERROR;
    }
	
	enm_ChComRet = CH_COM_GetLoaderInfo(&rpstru_LoaderInfo);
	if(eTDAL_OTA_STATUS_NO_ERROR != enm_ChComRet)
	{
		printf("\n CH_COM_GetLoaderInfo fail\n");
	}
	memset(&gsstru_OTAUpdateInfo,0,sizeof(CH_UpdateInfo_t));

	memcpy(gsstru_OTAUpdateInfo.uc_name,"Update Info",strlen("Update Info") );
	gsstru_OTAUpdateInfo.enm_AntType			= CH_DEMOD_DVB_C;
	gsstru_OTAUpdateInfo.uc_UpdaeFlag			= 0x47;
	gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.ui_FreqKHZ	= otaParam->uiFreqKHz;
	gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.ui_SymbKbps	= otaParam->uiSymbRateK;
	
	gsstru_OTAUpdateInfo.un_FreqInfo.stru_CableFreqInfo.uc_QAMMode	= otaParam->ucQAMMode -  CHMID_TUNER_QAM_QAM8;
	gsstru_OTAUpdateInfo.us_DataPid			= otaParam->uiPID;
	gsstru_OTAUpdateInfo.us_DataGid			= 0x2;
	rpstru_LoaderInfo.ui_TimeCode			= !otaParam->bInStandBy;
	enm_ChComRet = CH_COM_SetLoaderInfo(&rpstru_LoaderInfo);
	enm_ChComRet |= CH_COM_SetOTAInfo(&gsstru_OTAUpdateInfo);
#if 0		
	 {
	 
		 tTDAL_Diag_HWInfo	 hwif = {0};
		 tTDAL_Diag_SWInfo	 swif = {0};
		 U8 TempBuf[20] = {"0"};
		 
		 memset(&gstru_SystemInfo,0,sizeof(gstru_SystemInfo));
		 
		 strcpy((char*)gstru_SystemInfo.uc_name,"System Info");
		 gstru_SystemInfo.i_length = sizeof(gstru_SystemInfo) - 16;
		 
		 gstru_SystemInfo.uc_MacAddress[0] = 0x00; 
		 gstru_SystemInfo.uc_MacAddress[1] = 0x14;
		 gstru_SystemInfo.uc_MacAddress[2] = 0x49; 
		 gstru_SystemInfo.uc_MacAddress[3] = 0x99;
		 gstru_SystemInfo.uc_MacAddress[4] = 0x99;
		 gstru_SystemInfo.uc_MacAddress[5] = 0x99;
		 gstru_SystemInfo.uc_MacAddress[6] = 0x00;
		 gstru_SystemInfo.uc_MacAddress[7] = 0x00;
		 
		 
		 sprintf(gstru_SystemInfo.uc_STBSN,"%s","123456888");
		 CH_COM_SetSystemInfo(&gstru_SystemInfo);	  
	if(eTDAL_OTA_STATUS_NO_ERROR != enm_ChComRet)
	{
		printf("\nset OTA fail\n");
	}
		 printf("\nset OTA ok ok  \n");
		 TDAL_Diag_GetInfo(0, &hwif);
		 TDAL_Diag_GetInfo(1, &swif);
		TDAL_DelayTask(5000);
	
		 //CH_COM_SetSystemInfo(&gstru_SystemInfo);
	 }
#endif


	
}

#ifdef   DIAGNOSTICS_ENABLE
bool   TDAL_DIAG_LogEventRegister(tDIAG_TDAL_LogEventFct   LogEventFct,tDIAG_TDAL_TraceFct   TDAL_TraceFct)
{
   return(TRUE);
}
#endif

#endif /* USE_TDAL_OTA */

/* this table array is created with the mpeg-2 polynome :
   x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1 */
#define TDAL_CRC32_MAX_COEFFICIENTS 256
#define TDAL_CRC32_POLYNOMIAL       0x04C11DB7
static uint32_t TDAL_CRC_Table[TDAL_CRC32_MAX_COEFFICIENTS];

void TDALm_CRC32_Init()
{
    uint32_t uiCntCoef;
    uint32_t uiCntBit;
    uint32_t uiCoef32;

    for (uiCntCoef = 0; uiCntCoef < TDAL_CRC32_MAX_COEFFICIENTS; uiCntCoef++)
    {
        uiCoef32 = uiCntCoef << 24;
        for (uiCntBit=0; uiCntBit<8; uiCntBit++)
        {
            if (uiCoef32 & 0x80000000)
                uiCoef32 = ((uiCoef32 << 1) ^ TDAL_CRC32_POLYNOMIAL);
            else
                uiCoef32 <<= 1;
        }
        TDAL_CRC_Table[uiCntCoef] = uiCoef32;
    }
}

uint32_t TDALm_CRC32_Calculate(uint8_t *pcBuffer, uint32_t uiSize)
{
    uint32_t    uiCRC32 = 0xFFFFFFFF;
    uint32_t    uiCnt;
    uint8_t     uOrByte = 0;

    mTBOX_FCT_ENTER("TDALm_CRC32_Calculate");

    for (uiCnt = 0; uiCnt < uiSize; uiCnt++)
    {
        uOrByte |= *pcBuffer;
        uiCRC32 = (uiCRC32 << 8 ) ^ TDAL_CRC_Table[((uiCRC32 >> 24) ^ *pcBuffer) & 0xFF];
        pcBuffer++;
    }

    mTBOX_RETURN(uiCRC32);
}

bool TDALm_CRC32_Check(uint8_t *pcBuffer, uint32_t uiSize, uint32_t uiCRC32Saved)
{
    uint32_t uiCRC32Calculated;

    mTBOX_FCT_ENTER("TDALm_CRC32_Check");

    uiCRC32Calculated = TDALm_CRC32_Calculate(pcBuffer, uiSize);

    if (uiCRC32Calculated != uiCRC32Saved)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "extract crc [0x%08X] / calculate crc [0x%08X]\r\n", uiCRC32Saved, uiCRC32Calculated));
        mTBOX_RETURN(false);
    }

    mTBOX_RETURN(true);
}

void SEC_DIG_4200_Worker(void)
{
	return;
}