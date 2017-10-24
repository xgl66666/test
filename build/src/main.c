#include "crules.h"
#ifdef __DEBUG_TRACE__
#include "trace_cfg.h"
#endif

#if defined(PRODUCT_USE_MSTAR) && !defined(PRODUCT_PC_SIM)
#include "tdal_common.h"
#include "tbox.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "drvUART.h"
#include "cyg/posix/types.h"
#include "tdal_dmx.h"
#if defined (MSOS_TYPE_ECOS)
#include <cyg/hal/hal_cache.h>
#endif

#define UARTBUF_SIZE        512
static MS_U32  UartDisableCounter = 0 ;
static MS_BOOL UartStop = FALSE ;
static MS_U32  u32Read = 0 , u32Write = 0 , CharNum = 0;
static MS_U8 UartBuf[64];
static MS_U8   UartBuffer[UARTBUF_SIZE] ;
extern void UartCB(int c);

#define TASK_STK_SIZE       0x4000
static MS_U8 _u8TaskStartStk[TASK_STK_SIZE];
static MS_U8 _u8CHALStartStk[TASK_STK_SIZE];
extern MS_S32 gs32CachedPoolID;
void TaskStart (MS_U32 argc, VOID *argv);
static pthread_t taskID;

#define DYNA_CHG_MIU_CLK


#define UART_BASE         (0xBF201300)
#define UART_LSR_THRE     (0x20)
#define UART_LSR_TEMT     (0x40)
#define UART_BOTH_EMPTY   (UART_LSR_TEMT | UART_LSR_THRE)
#define UART_RX           (*((volatile unsigned int *)(UART_BASE + (0 * 8))))
#define UART_TX           (*((volatile unsigned int *)(UART_BASE + (0 * 8))))
#define UART_LSR          (*((volatile unsigned int *)(UART_BASE + (5 * 8))))

void MAsm_MIU_CHCLK(void);
extern void cmLight_main(void *arg);

void uart_putc(int c)
{    
    while((UART_LSR & UART_LSR_THRE) != UART_LSR_THRE)        ;
    UART_TX = c;    
    while((UART_LSR & UART_BOTH_EMPTY) != UART_BOTH_EMPTY)        ;
}

void uart_puts(char *s)
 {
while(*s)        uart_putc(*s++);
}
#if defined (MSOS_TYPE_ECOS)
void * cmLightMain_Wrapper(void * param)
#else
int main(int argc, char *argv[])
#endif
{
#ifdef __DEBUG_TRACE__
    TRACE_CFG_Init();
#endif
	cmLight_main(param);
	return NULL;
}

MS_BOOL appDemo_Main(void)
{
    pthread_attr_t task_attr;
    struct sched_param schedparam;

    schedparam.sched_priority = 16;
    pthread_attr_init( &task_attr );
    pthread_attr_setinheritsched( &task_attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setstackaddr( &task_attr, &_u8CHALStartStk[sizeof(_u8CHALStartStk)] );
    pthread_attr_setstacksize( &task_attr, sizeof(_u8CHALStartStk) );
    pthread_attr_setschedpolicy( &task_attr, 3); //SCHED_RR
    pthread_attr_setschedparam( &task_attr, &schedparam );

    pthread_create( &taskID, &task_attr, cmLightMain_Wrapper, NULL );

    return TRUE;
}

#if defined (MSOS_TYPE_ECOS)
int main(int argc, char *argv[])
{
    //chalvalidator_main();

    return(0);
}
#endif

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
    memset( UartBuffer,0x0 , UARTBUF_SIZE );

    MDrv_UART_SetRxMode(TRUE,UartBuf,64,UartCB);

	MsOS_CreateTask((TaskEntry) TaskStart,
	                    (MS_U32)NULL,
	                    E_TASK_PRI_HIGH,
	                    TRUE,
	                    _u8TaskStartStk,
	                    TASK_STK_SIZE,
	                    "App" );    ;
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


#if defined(DYNA_CHG_MIU_CLK)
void  __attribute__ ((section (".miuclk"))) MAsm_MIU_CHCLK(void)
{


    uart_puts("MAsm_MIU_CHCLK\n");
    asm __volatile__ (
        "1:;"

        //_RV32_2(0x1246, 0xFFFE),   //mask group0
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x248C;"
        "li	$26,0xFFFE;"
        "sw	$26,0($27);"

        //_RV32_2(0x1266, 0xFFFF),   //mask group1
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x24CC;"
        "li	$26,0xFFFF;"
        "sw	$26,0($27);"

        //_RV32_2(0x1286, 0xFFFF),   //mask group2
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x250C;"
        "li	$26,0xFFFF;"
        "sw	$26,0($27);"

        //_RV32_2(0x12A6, 0xFFFF),   //mask group3
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x254C;"
        "li	$26,0xFFFF;"
        "sw	$26,0($27);"

        //WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0018), // clk_miu = 288M
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x167C;"
        "li	$26,0x0018;"
        "sw	$26,0($27);"



        //_RV32_2(0x1246, 0x0000),   //unmask group0
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x248C;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"


        //_RV32_2(0x1266, 0x0000),   //unmask group1
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x24CC;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"


        //_RV32_2(0x1286, 0x0000),   //unmask group2

        "lui	$26,0xbf20;"
        "ori	$27,$26,0x250C;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"

        //_RV32_2(0x12A6, 0x0000),   //unmask group3 //jc_db: should u4 have group3
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x254C;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"

        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"

        );
}
#endif
#endif

#ifdef PRODUCT_USE_ST

extern void cmLight_main(void *arg);

int main(int argc, char *argv[])
{
#ifdef __DEBUG_TRACE__
    TRACE_CFG_Init();
#endif
    cmLight_main(NULL);
    return 0;
}
#endif

#ifdef PRODUCT_PC_SIM

#include "tdal_common.h"
#include "tbox.h"
#include "tdal_dmx.h"
#include "tkel.h"
#if defined (MSOS_TYPE_ECOS)
#include "MsCommon.h"
#include "MsOS.h"
#include "drvUART.h"
#include "cyg/posix/types.h"
#include <cyg/hal/hal_cache.h>
#endif
#ifdef __DEBUG_TRACE__
#include "trace_cfg.h"
#else
#include "pthread.h"
#endif
#include <sys/types.h>
#include <sys/stat.h>

#define UARTBUF_SIZE        512
static uint32_t  UartDisableCounter = 0 ;
static bool UartStop = FALSE ;
static uint32_t  u32Read = 0 , u32Write = 0 , CharNum = 0;
static uint8_t UartBuf[64];
static uint8_t   UartBuffer[UARTBUF_SIZE] ;
extern void UartCB(int c);

#define TASK_STK_SIZE       0x4000
static uint8_t _u8TaskStartStk[TASK_STK_SIZE];
static uint8_t _u8CHALStartStk[TASK_STK_SIZE];
extern int32_t gs32CachedPoolID;
void TaskStart (uint32_t argc, void *argv);
static pthread_t taskID;

#define DYNA_CHG_MIU_CLK


#define UART_BASE         (0xBF201300)
#define UART_LSR_THRE     (0x20)
#define UART_LSR_TEMT     (0x40)
#define UART_BOTH_EMPTY   (UART_LSR_TEMT | UART_LSR_THRE)
#define UART_RX           (*((volatile unsigned int *)(UART_BASE + (0 * 8))))
#define UART_TX           (*((volatile unsigned int *)(UART_BASE + (0 * 8))))
#define UART_LSR          (*((volatile unsigned int *)(UART_BASE + (5 * 8))))

int GLOBAL_argc;
char** GLOBAL_argv;

IMPORT char PC_DATA_PATH[512];
IMPORT char PC_CFG_FILE[512];
IMPORT bool bUseDefaultDataPath;

void MAsm_MIU_CHCLK(void);
extern void cmLight_main(void *arg);

//int main(int argc, char *argv[])
//{
//#ifdef __DEBUG_TRACE__
//    TRACE_CFG_Init();
//#endif
//    cmLight_main(NULL);
//    return 0;
//}


void uart_putc(int c)
{    
    while((UART_LSR & UART_LSR_THRE) != UART_LSR_THRE)        ;
    UART_TX = c;    
    while((UART_LSR & UART_BOTH_EMPTY) != UART_BOTH_EMPTY)        ;
}

void uart_puts(char *s)
 {
while(*s)        uart_putc(*s++);
}


void * cmLightMain_Wrapper(void * param)
{
#ifdef __DEBUG_TRACE__
    TRACE_CFG_Init();
#endif
	cmLight_main(param);
	return NULL;
}

bool checkDirectoryExists(const char *path){
	struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

bool appDemo_Main(void)
{
	char path[512];
	char c = '\0';
	bUseDefaultDataPath = true;

	int argc = GLOBAL_argc;
	char** argv = GLOBAL_argv;

	int i = 1;
	while(i < argc-1){
		if(!strcmp(argv[i], "--data")){
			strcpy(path, argv[++i]);
			bUseDefaultDataPath = false;
		}
		++i;
	}

	if(!bUseDefaultDataPath){
		bool ex = checkDirectoryExists(path);
		if(!ex){
			printf("Directory does not exists, provide correct path!\n");
			return -1;
		}else{
#ifdef __linux__
			if(path[strlen(path)-1] != '/'){
				strcat(path, "/");
			}
#elif _WIN32
			if(path[strlen(path)-1] != '/' || (path[strlen(path)-1] != '\\')){
				strcat(path, "/");
			}
#endif
			strcpy(PC_DATA_PATH, path);
			strcpy(PC_CFG_FILE, path);
			strcat(PC_CFG_FILE, "streams/tsfiles.cfg");
		}
	}

    pthread_attr_t task_attr;
    struct sched_param schedparam;

    schedparam.sched_priority = 16;
    pthread_attr_init( &task_attr );
    pthread_attr_setinheritsched( &task_attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setstackaddr( &task_attr, &_u8CHALStartStk[sizeof(_u8CHALStartStk)] );
    pthread_attr_setstacksize( &task_attr, sizeof(_u8CHALStartStk) );
    pthread_attr_setschedpolicy( &task_attr, 3); //SCHED_RR
    pthread_attr_setschedparam( &task_attr, &schedparam );

    pthread_create( &taskID, &task_attr, cmLightMain_Wrapper, NULL );
    getchar();
    while((c = getchar())){
		if(c == 'q' || c == 'Q')
			break;
	}
    return TRUE;
}

int main(int argc, char *argv[])
{
    GLOBAL_argc = argc;
    GLOBAL_argv = argv;
    //chalvalidator_main();
    appDemo_Main();
    return(0);
}
#if defined(MSOS_TYPE_ECOS)
 void cyg_user_start(void)
{
    bool msBool;
    extern char __heap1_end[];
    void *pFirstAvailableMemory = (void *)__heap1_end;

	/* Adjust clock rate for MIU */
#if defined(DYNA_CHG_MIU_CLK)
    uint32_t u32Intr;

    u32Intr = MsOS_DisableAllInterrupts();

    //printf("MIU CLK = 0x%x\n",*(volatile MS_U16*)(0xBF200000+(0xb3e<<1)));


    //MAsm_MIU_CHCLK(); // change miu clk to 288Mhz

    //printf("MIU CLK = 0x%x\n",*(volatile MS_U16*)(0xBF200000+(0xb3e<<1)));

    MsOS_RestoreAllInterrupts(u32Intr);
#endif

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
    memset( UartBuffer,0x0 , UARTBUF_SIZE );

    MDrv_UART_SetRxMode(TRUE,UartBuf,64,UartCB);

	MsOS_CreateTask((TaskEntry) TaskStart,
	                    (uint32_t)NULL,
	                    E_TASK_PRI_HIGH,
	                    TRUE,
	                    _u8TaskStartStk,
	                    TASK_STK_SIZE,
	                    "App" );    ;
}

void TaskStart (uint32_t argc, void *argv)
{
    //MApi_SystemDeviceHost_Init();
    /* This is needed for DEMOD and DMX to work */
    MApi_DMX_CMD_Run( 0xF0010001 , 1, 0 , NULL );

    appDemo_Main();

    while(1)
    {
	uart_puts("While\n");
    MsOS_DelayTask(10000);
    }
}


#if defined(DYNA_CHG_MIU_CLK)
void  __attribute__ ((section (".miuclk"))) MAsm_MIU_CHCLK(void)
{


    uart_puts("MAsm_MIU_CHCLK\n");
    asm __volatile__ (
        "1:;"

        //_RV32_2(0x1246, 0xFFFE),   //mask group0
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x248C;"
        "li	$26,0xFFFE;"
        "sw	$26,0($27);"

        //_RV32_2(0x1266, 0xFFFF),   //mask group1
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x24CC;"
        "li	$26,0xFFFF;"
        "sw	$26,0($27);"

        //_RV32_2(0x1286, 0xFFFF),   //mask group2
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x250C;"
        "li	$26,0xFFFF;"
        "sw	$26,0($27);"

        //_RV32_2(0x12A6, 0xFFFF),   //mask group3
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x254C;"
        "li	$26,0xFFFF;"
        "sw	$26,0($27);"

        //WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0018), // clk_miu = 288M
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x167C;"
        "li	$26,0x0018;"
        "sw	$26,0($27);"



        //_RV32_2(0x1246, 0x0000),   //unmask group0
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x248C;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"


        //_RV32_2(0x1266, 0x0000),   //unmask group1
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x24CC;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"


        //_RV32_2(0x1286, 0x0000),   //unmask group2

        "lui	$26,0xbf20;"
        "ori	$27,$26,0x250C;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"

        //_RV32_2(0x12A6, 0x0000),   //unmask group3 //jc_db: should u4 have group3
        "lui	$26,0xbf20;"
        "ori	$27,$26,0x254C;"
        "li	$26,0x0000;"
        "sw	$26,0($27);"

        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"
        "nop;"

        );
}
#endif
#endif

#endif

