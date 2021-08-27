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
    return eTDAL_OTA_STATUS_NO_ERROR;

}
GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_GetParameters(tTDAL_OTA_StatusParameters *otaParam)
{
		return eTDAL_OTA_STATUS_NO_ERROR;
}

GLOBAL tTDAL_OTA_ErrorCode TDAL_OTA_SetParameters(tTDAL_OTA_ControlParameters *otaParam){
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
