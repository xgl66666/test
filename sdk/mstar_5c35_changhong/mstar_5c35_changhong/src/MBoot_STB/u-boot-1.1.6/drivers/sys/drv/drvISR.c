#include <common.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvISR.h>

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

U32 volatile gSystemTimeCount;
U32 volatile gTimerCount0;
U32 volatile gTimerDownCount0;
U32 volatile gTimerDownCount1;

U32 volatile g100msTimeCount;
U8 gu8100msTH;

static void ProcessCPU0toCPU2Isr(MHAL_SavedRegisters *pHalReg, unsigned long vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);

    MsOS_EnableInterrupt(E_INT_FIQ_8051_TO_BEON);
}

static void ProcessCPU1toCPU2Isr(MHAL_SavedRegisters *pHalReg, unsigned long vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);

	MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_BEON);
}

static void TimerISR(void)
{
    gTimerCount0++;
    gu8100msTH++;

    // Use a threshold to check the 100 ms. If the threshold
    // is greater or equal than the 100 ms. Increase the 100 ms
    // counter.
    if (gu8100msTH >= 100)
    {
        g100msTimeCount++;
        gu8100msTH = 0;
    }
#if (0)//IR_MODE_SEL == IR_TYPE_SWDECODE_KON_MODE)
    if (g_u8IrKonRepeatTimeout)
        g_u8IrKonRepeatTimeout--;
#endif

    if (gTimerCount0%1000==0)
    {
        gSystemTimeCount++;
    }

    //--------------------------------
    //down time counter:
    if ( gTimerDownCount0 > 0 )
        gTimerDownCount0--;

    if ( gTimerDownCount1 > 0 )
        gTimerDownCount1--;

#if (0)//IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
    if(g_ucIrRepeatTimer > 0)
        g_ucIrRepeatTimer--;
#endif
    //-------------------------------

    #if (0)//!BLOADER)
    #if (ENABLE_MSTV_UART_DEBUG )
    if (g_Uart0CheckTick)
    {
        --g_Uart0CheckTick;
        if ( g_Uart0CheckTick == 0 )
        {
            g_bUart0Detected = FALSE;
            g_UartCommand.Index = 0;
        }
    }
    #elif XMODEM_DWNLD_ENABLE //for xmodem
     if ( g_Uart0CheckTick )
    {
        --g_Uart0CheckTick;
        if ( g_Uart0CheckTick == 0 )
        {
            XModem_Rx_Timeout = TRUE;
        }
    }
    #endif
    #endif
#if (ENABLE_UART1_INTERRUPT == ENABLE)
    #if(0)//ENABLE_UART1_DEBUG)
    if (g_Uart1CheckTick)
    {
        --g_Uart1CheckTick;
        if ( g_Uart1CheckTick == 0 )
        {
            g_bUart1Detected = FALSE;
            g_ucUart1BufIndex = 0;
        }
    }
    #endif
#endif

    #if (0)//!BLOADER)
    #if ENABLE_TTX
    {
        static U8 u8TTXTimer = 0;

        bTTXInISR=TRUE;

        if ((u8TTXTimer++ % 20) == 0)
          msAPI_TTX_CollectPackets();

        bTTXInISR=FALSE;
    }
    #endif
    #endif

    #if (0)//!BLOADER)
    if(MApi_XC_HDMI_Func_Caps() & CAPS_DVI_ADJ_70M)
    {
#ifdef DVI_NEW_Patch
    // 20081226 - DVI+HDCP snow noise patch - start ...
        if(g_DVI70MhzPollingCounter)
        {
            g_DVI70MhzPollingCounter--;

            if(g_DVI70MhzPollingCounter == 0)
                mdrv_dvi_clock_70mhz_swpatch2(SYS_INPUT_SOURCE_TYPE(MAIN_WINDOW));
        }
    // 20081226 - DVI+HDCP snow noise patch - end ...
#endif
    }
    #endif
}

void MDrv_Timer_ISR_Register(void)
{
    MS_U32 check_timer_status;
    check_timer_status = MsOS_CreateTimer( (TimerCb)TimerISR,
                                                  0,
                                                  1,
                                                  TRUE,
                                                  "Check timer");
}

void MDrv_ISR_Init(void)
{
    MsOS_AttachInterrupt(E_INT_FIQ_8051_TO_BEON, (InterruptCb)ProcessCPU0toCPU2Isr);
    MsOS_AttachInterrupt(E_INT_FIQ_AEON_TO_BEON, (InterruptCb)ProcessCPU1toCPU2Isr);
}

int interrupt_init(void)
{
    MsOS_Init();
    //MDrv_ISR_Init();
    MsOS_CPU_EnableInterrupt();

    return 0;
}
