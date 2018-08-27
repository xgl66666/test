#include "crules.h"

#include "tdal_common.h"
#include "tbox.h"

#if defined (MSOS_TYPE_ECOS)
#include "MsCommon.h"
#include "MsOS.h"
#include "drvUART.h"
#include "cyg/posix/types.h"
#include "apiDMX.h"

#include <cyg/hal/hal_cache.h>

extern UART_Result MDrv_UART_SetRxMode
       (MS_BOOL bEnable, void *rx_buf, MS_U16 buf_len, ms_uart_rx_callback rx_cb);

#endif
#include "SysInit.h"
#include "tdal_dmx.h"
#include <pthread.h>

#define UARTBUF_SIZE        512
#ifdef PRODUCT_PC_SIM
static bool UartStop = FALSE ;
static uint32_t  u32Read = 0 , u32Write = 0 , CharNum = 0;
static uint8_t UartBuf[64];
static uint8_t   UartBuffer[UARTBUF_SIZE] ;
extern void UartCB(int c);

#define TASK_STK_SIZE       0x10000
static uint8_t _u8TaskStartStk[TASK_STK_SIZE];
static uint8_t _u8CHALStartStk[TASK_STK_SIZE];
extern int32_t gs32CachedPoolID;
#else
static MS_BOOL UartStop = FALSE ;
static MS_U32  u32Read = 0 , u32Write = 0 , CharNum = 0;
static MS_U8 UartBuf[64];
static MS_U8   UartBuffer[UARTBUF_SIZE] ;
extern void UartCB(int c);

#define TASK_STK_SIZE       0x10000
static MS_U8 _u8TaskStartStk[TASK_STK_SIZE];
static MS_U8 _u8CHALStartStk[TASK_STK_SIZE];
extern MS_S32 gs32CachedPoolID;
void TaskStart (MS_U32 argc, VOID *argv);
#endif
extern void *chalvalidator_main(void *argv);
static pthread_t taskID;

#ifndef PRODUCT_PC_SIM
MS_BOOL appDemo_Main(void)

{

    pthread_attr_t task_attr;
    struct sched_param schedparam;

    schedparam.sched_priority = 24;
    pthread_attr_init( &task_attr );
    pthread_attr_setinheritsched( &task_attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setstackaddr( &task_attr, &_u8CHALStartStk[sizeof(_u8CHALStartStk)] );
    pthread_attr_setstacksize( &task_attr, sizeof(_u8CHALStartStk) );
    pthread_attr_setschedpolicy( &task_attr, 3); //SCHED_RR
    pthread_attr_setschedparam( &task_attr, &schedparam );

    pthread_create( &taskID, &task_attr, chalvalidator_main, NULL );
    return TRUE;
}
#endif

int main(int argc, char *argv[])
{
#ifdef PRODUCT_PC_SIM
    chalvalidator_main(NULL);
#endif
    return(0);
}

#if defined(MSOS_TYPE_ECOS)
void cyg_user_start(void)
{
    MS_BOOL msBool;
    extern char __heap1_end[];
    void *pFirstAvailableMemory = (void *)__heap1_end; 

   /* This should be first function called form MW! */
#if defined (MSOS_TYPE_ECOS)
    // Set the TLB exception handler
    extern unsigned int utlb_vector[];
    memcpy((void *)0x80000000, utlb_vector, 0x80);
    HAL_DCACHE_FLUSH(0x80000000, 0x80);
    HAL_ICACHE_SYNC();
#endif

    msBool = MApi_SystemInit_Init(pFirstAvailableMemory);	
    
    u32Write = u32Read = CharNum = 0 ;
    UartStop = FALSE ;
    memset( UartBuffer,0x0,UARTBUF_SIZE);

    MDrv_UART_SetRxMode(TRUE,UartBuf,64,UartCB); 
    
	MsOS_CreateTask((TaskEntry) TaskStart,
	                    (MS_U32)NULL,
	                    E_TASK_PRI_HIGH,
	                    TRUE,
	                    _u8TaskStartStk,
	                    TASK_STK_SIZE,
	                    "App" );
}

void TaskStart (MS_U32 argc, VOID *argv)
{
    //MApi_SystemDeviceHost_Init();
    /* This is needed for DEMOD and DMX to work */
    MApi_DMX_CMD_Run( 0xF0010001 , 1, 0 , NULL );

    appDemo_Main();

    while(1)
    {
        MsOS_DelayTask(10000);
    }
}
#endif
