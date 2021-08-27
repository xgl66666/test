//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///@file drvuart.h
///@brief System functions:UART Module
/// This file is vaild when "ENABLE_MSTV_UART_DEBUG" is enable
/// The brief function is get the register content by UART without halt the whole
/// system.
//
///////////////////////////////////////////////////////////////////////////////

#define _DRVUART_C_
#include "Board.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "UartDebug.h"
#include "drvIIC.h"

#include "drvUART.h"
#include "drvDemod.h"
#include "apiDigiTuner.h"

#ifndef DDI_MISC_INUSE
#include "drvDMD_INTERN_DVBT.h"
#include "appDemo_DigiTuner.h"
#else
//#include "MApp_MenuChannel.h" //depend with apiDTVDataManager.h
#include "appMain.h"

#ifdef MSOS_TYPE_LINUX
#include "madp.h"
#endif
#if UART_DEBUG_SUPPORT
#include "TaskManage.h"
#endif

#endif


#define PHOENIX_DEBUG   1

//#define UARTMSG(x)    x

#if (CHANNEL_SCAN_AUTO_TEST)
#define CHSCANTEST_MSG(x)    x
#else
#define CHSCANTEST_MSG(x)
#endif


//static const U8 CMD_TRE_ACK[] = {'P', 'T','R','E', 'A','C','K', 'P'};
static MS_U8 _u8UTaskStartStk[8192];



/******************************************************************************/
/// UART device: putchar and ISR
/******************************************************************************/
static MS_U32 uart_debug;
void putcharb(MS_U8 byte)
{
    if (uart_debug)
    {
        mdrv_uart_write(uart_debug, (MS_U8 *)&byte, 1);
    }
}

void MDrv_UART_RecvHandler(int c)
{
    g_Uart0CheckTick = 50;// time-out control ms
    if (g_UartCommand.Index >= _UART_CMD_LENGTH_)
    {
        g_UartCommand.Index = 0;
    }

    g_UartCommand.Buffer[g_UartCommand.Index] = c;


    // check command buffer index
    if (UART_EXT)
    {
        if (g_UartCommand.Index == 0) // check 1st data
        {
            g_UartCommand.Index++; // for get UART_CMD_EXT_LENGTH
        }
        else
        {
            if (g_UartCommand.Index < UART_CMD_EXT_LENGTH) // still read command
            {
                g_UartCommand.Index++; // next index of command buffer
            }
            if (g_UartCommand.Index >= UART_CMD_EXT_LENGTH) // read command ok
            {
                g_bUart0Detected = TRUE; // command  buffer recieve ok
                g_UartCommand.Index = 0; // reset index of command buffer
                g_Uart0CheckTick = 0;
            }
        }
    }
    else
    {
        if (g_UartCommand.Index < UART_CMD_LENGTH) // still read command
        {
            g_UartCommand.Index++; // next index of command buffer
        }
        if (g_UartCommand.Index >= UART_CMD_LENGTH) // read command ok
        {
            // check if Enter Key. (0x0D)
            if (g_UartCommand.Buffer[_UART_CMD_CODE_] == 0x0D) {
              #if 0 //(ENABLE_MMS)
                //set flag to enter menu mode
                DbgMenuMode = 1;
                MDrv_LogoPrint();
                if (!(DbgMenuLevel ||CatMenuLevel || CfgMenuLevel || UntMenuLevel))
                    MDrv_MainPagePrint();
               #endif
            }

            g_bUart0Detected = TRUE; // command  buffer recieve ok
            g_UartCommand.Index = 0; // reset index of command buffer
            g_Uart0CheckTick = 0;
        }
    }

    #if defined(MIPS_CHAKRA) || defined(__AEONR2__)
        MsOS_EnableInterrupt(E_INT_IRQ_UART0);
    #endif

  //  printf("[%s] , %d\n",g_UartCommand.Buffer);
}

MS_S32 g_autotest_polling_task = 0;

static void _msAPI_uart_polling_task(MS_U32 argc, void *argv)
{
    MS_U8 u8Str[2] = {0};

    while(1)
    {

        mdrv_uart_read(uart_debug, u8Str, 0x01);
        u8Str[1] = 0;
        MDrv_UART_RecvHandler(u8Str[0]);
        if(g_bUart0Detected == TRUE)
        {
           if (UART_EXT)   // DLC & DBC
           {
                // MDrv_UART_DecodeExtCommand();
                Cus_UART_DecodeExtCommand();
           }
           else    // Normal command: XDATA read/write
           {
                MDrv_UART_DecodeNormalCommand();
           }
           g_bUart0Detected = FALSE;

                //reset command buffer
                //MDrv_UART_DebugInit();
        }
    }
}

/******************************************************************************/
///Initial UART relative variable and flag.
/******************************************************************************/
void MDrv_UART_DebugInit(void)
{
    // reset Uart variables
    printf("MDrv_UART_DebugInit \n");

    g_UartCommand.Index = 0;

    g_bDebugASCIICommandFlag = 0;
    g_bDebugProgStopFlag = FALSE;

    g_Uart0CheckTick = 0;


    uart_debug = mdrv_uart_open(E_UART_PIU_UART0);

  //  mdrv_uart_write( uart_debug, CMD_TRE_ACK, 8 );

    if (uart_debug)
    {

        g_autotest_polling_task = MsOS_CreateTask(_msAPI_uart_polling_task, (MS_U32)NULL, E_TASK_PRI_MEDIUM,
                                             TRUE, _u8UTaskStartStk, 8192, "uart_poll_task");
#ifdef DDI_MISC_INUSE
#if UART_DEBUG_SUPPORT
    if(g_polling_task >= 0)
    {
        Task_Info *taskinfo = (Task_Info *)MsOS_AllocateMemory(sizeof(Task_Info),gs32CachedPoolID);
        taskinfo->ePriority = E_TASK_PRI_MEDIUM;
        taskinfo->iId = g_polling_task;
        taskinfo->pStack = _u8UTaskStartStk;
        strcpy(taskinfo->szName,"uart_poll_task");
        taskinfo->u32StackSize = 8192;
        TaskLink_Add(taskinfo);
    }
#endif
#endif
    }else
    {
        printf("mdrv_uart_open fail : UART_DEV_NULL \n");
    }

}

/******************************************************************************/
/// Decode the Command gets from UART port for main chip.
/// Command Buffer [0]:
///  -bit7~bit5 :Command length:1 ~ 6
///  -bit4~bit0 :UART command
/// Command Buffer [1]:
///  -Control address high.
/// Command Buffer [2]:
///  -Control address low.
/// if write command:
///   -Command buffer [3]: control data
/******************************************************************************/
void MDrv_UART_DecodeNormalCommand(void)
{

#if PHOENIX_DEBUG
    U16 RegAddr=0, RegData=0;
    U8    u8Standard=0;

    U8 RegData_Read=0;
#endif
    switch ( UART_CMD )
    {
#if PHOENIX_DEBUG
        case uartRead_Demod:
            {
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                u8Standard = g_UartCommand.Buffer[_UART_CMD_INDEX3_];
                //RegData = MDrv_Demod_ReadReg( RegAddr );
                MApi_DigiTuner_ReadReg(0,RegAddr,&RegData_Read);
                putcharb( 0xF3 );
                putcharb( ( U8 ) ( RegData >> 8 ) );
                putcharb( ( U8 ) ( RegData & 0xff ) );
                printf("[%x] = %x\n", RegAddr, RegData);
            }
            break;
        case uartWrite_Demod:
            {
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                RegData = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX3_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX4_];
                u8Standard = g_UartCommand.Buffer[_UART_CMD_INDEX5_];
                //RegData = MDrv_Demod_WriteReg( RegAddr, RegData );
                RegData = MApi_DigiTuner_WriteReg( 0,RegAddr, RegData );
                putcharb( 0xF1 );
                printf("[%x] = %x\n", RegAddr, RegData);
            }
            break;
#endif

        default:
            printf("Unsupport Normal Command: 0x%x\n", (U16)UART_CMD );
            break;
    }
}


#if ((CHANNEL_SCAN_AUTO_TEST) || (SFU_AUTO_TEST))

void Cus_UART_Scan(void)
{
    switch(UART_EXT_CMD)
    {
       case UART_SCAN_CMD_SET_TUNE_TO_RFCH:
       {

            MS_U32 u32Data;
            MS_FE_CARRIER_PARAM    FeParam;

#ifndef DDI_MISC_INUSE
            if (appDemo_DigiTuner_InitTuner())
            {
                printf("Tuner Success.");
            }
            else
            {
                printf("Tuner Error.");
            }
#endif
            memset(&FeParam,0,sizeof(MS_FE_CARRIER_PARAM));

            u32Data = (g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX5_]| (u32Data<<8);
            u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX6_]| (u32Data<<8);
            u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX7_]| (u32Data<<8);
         /*   printf("RRR-1: UART_SCAN_DATA1 = 0x%bx,0x%bx, 0x%bx, 0x%bx, 0x%lx,  \n"
                     ,g_UartCommand.Buffer[_UART_CMD_INDEX4_]
                     ,g_UartCommand.Buffer[_UART_CMD_INDEX5_]
                     ,g_UartCommand.Buffer[_UART_CMD_INDEX6_]
                     ,g_UartCommand.Buffer[_UART_CMD_INDEX7_],u32Data);         */
            FeParam.u32Frequency             = u32Data;
          //  printf("RRR-2: stTpSettingPC.u32Frequency = %lu \n",u32Data);
          switch (UART_EXT_CMD_MS_DAT6)
          {

#if MS_DVBS_INUSE

            DEMOD_MS_FE_CARRIER_PARAM dmdParam;

            case E_STANDARD_DVBS:
            case E_STANDARD_DVBS2:
                dmdParam.u32Frequency = FeParam.u32Frequency;

                u32Data = (g_UartCommand.Buffer[_UART_CMD_INDEX10_]);
                u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX11_]| (u32Data<<8);
                u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX12_]| (u32Data<<8);
                u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX13_]| (u32Data<<8);
                dmdParam.SatParam.u32SymbolRate = u32Data;
                dmdParam.SatParam.u32SymbolRate *= 1000;
                //MDrv_Demod_Restart(&dmdParam);

                //====> need to more think
                MApi_DigiTuner_ForceRetune(0);


                   // FeParam.SatParam.u16SymbolRate = UART_SCAN_DATA5;


                AppCommand appCmd;

                appCmd.eCmd = E_APP_CMD_MONITOR_SIGNAL_STOP;
                appCmd.eFlg = E_APP_CMDFLG_NONE;
                appCmd.eSrcAppType = E_APP_TYPE_ANY;
                appCmd.eDstAppType = E_APP_TYPE_MONITOR;
                appCmd.u8Data[0] = NULL;
                appMain_RecvNotify(&appCmd);

                 //   MApi_DigiTuner_Tune2RfCh(_stSATParam,&tuneParam,  FE_TUNE_MANUAL);
                printf("OK\n");

                U32 u32Timeout = 50;

                EN_LOCK_STATUS bLocked=E_DEMOD_NULL;
                 while(u32Timeout-- > 0 && bLocked == FALSE)
                {

                    MsOS_DelayTask(100);//mick
                    //MDrv_Demod_GetLock(&bLocked);
                    MApi_DigiTuner_GetLock(0, &bLocked);

                }
                if (bLocked == E_DEMOD_LOCK)
                {
                    printf("Locked \n");
                }else
                {
                    printf("unLock\n");
                }
              break;
#elif(MS_DVB_TYPE_SEL == DVBC)
            case E_STANDARD_DVBC:

                u32Data = (g_UartCommand.Buffer[_UART_CMD_INDEX10_]);
                u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX11_]| (u32Data<<8);
                u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX12_]| (u32Data<<8);
                u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX13_]| (u32Data<<8);
                FeParam.CabParam.eConstellation  = g_UartCommand.Buffer[_UART_CMD_INDEX8_]; //_eDVBCQAMMod, u8QAMType;
                FeParam.CabParam.u16SymbolRate   = u32Data; //u32SymbolRate;
#ifdef FRONTEND_DEMOD_IQ_TYPE
                FeParam.CabParam.eIQMode         = FRONTEND_DEMOD_IQ_TYPE;
#elif
                FeParam.CabParam.eIQMode         = 0;
#endif // FRONTEND_DEMOD_IQ_TYPE
                FeParam.CabParam.u8TapAssign     = 1;
                FeParam.CabParam.u32FreqOffset   = 0;

                MApi_DigiTuner_Tune2RfCh(0,&FeParam, FE_TUNE_MANUAL);
                printf("OK\n");
                break;
#else
            case E_STANDARD_DVBT2:
                FeParam.TerParam.u8ScanType = E_DEMOD_TYPE_T2;
            case E_STANDARD_DVBT:
                FeParam.TerParam.eBandWidth = (EN_TER_BW_MODE)(g_UartCommand.Buffer[_UART_CMD_INDEX8_]-1);;
#ifdef DDI_MISC_INUSE
                AppCommand appCmd;

                appCmd.eCmd = E_APP_CMD_MONITOR_SIGNAL_STOP;
                appCmd.eFlg = E_APP_CMDFLG_NONE;
                appCmd.eSrcAppType = E_APP_TYPE_ANY;
                appCmd.eDstAppType = E_APP_TYPE_MONITOR;
                appCmd.u8Data[0] = NULL;
                appMain_RecvNotify(&appCmd);
#endif
                MApi_DigiTuner_Tune2RfCh(0,&FeParam, FE_TUNE_MANUAL);
                printf("OK\n");
            break;
#endif
            default:
            break;
            }
        }
        break;

        case UART_SCAN_CMD_LOCK_STATE:
            {
#if PHOENIX_DEBUG

                EN_LOCK_STATUS lockState = E_DEMOD_NULL;
                switch(UART_EXT_CMD_MS_DAT1)
                {
                    case E_STANDARD_DVBS:

                    case E_STANDARD_DVBS2:

                    case E_STANDARD_DVBT:

                    case E_STANDARD_DVBT2:

                    default:
                        //MDrv_Demod_GetLock(&lockState);
                        MApi_DigiTuner_GetLock(0,&lockState);
                    break;
                }
                if (lockState == E_DEMOD_LOCK)
                {
                        printf("OK\n");
                }
                else
                {
                        printf("err\n");
                }

#endif
            }
            break;
        case UART_SCAN_CMD_CHECK_PACKET_ERR:
            {
                MS_U16 u16PktErr = 0;

                //if (MDrv_Demod_Get_Packet_Error(&u16PktErr)==TRUE)
                if (MApi_DigiTuner_Get_Packet_Error(0,&u16PktErr)==TRUE)
                {
                    if (u16PktErr > 0)
                    {
                        printf("err\n");
                    }
                    else
                    {
                        printf("OK\n");
                    }
                }
                else
                {
                    printf("MDrv_Demod_Get_Packet_Error error\n");
                }
            }
            break;
        default:
            break;
    }
}
#endif

void Cus_UART_DecodeExtCommand(void)
{

    switch(UART_EXT_DEV)
    {
#if ((CHANNEL_SCAN_AUTO_TEST) || (SFU_AUTO_TEST))
        case uartExtDev_SCAN_TUNER:
            Cus_UART_Scan();
            break;
#endif
        case uartExtDev_I2C_DEVICE_NEW:
        case uartExtDev_AUDIO_DEBUG:
        default:
            break;
    }
}


BOOLEAN UART_GetChar(MS_U8 *u8Char)
{
    BOOLEAN bOK;

    //ES1 = 0;                      // disable uart interrupt
    //_SetInt_SERIAL1(DISABLE);
    if( uartRXByteCount > 0 )                 /* is there any char in buffer   */
    {
        bOK = TRUE;
        *u8Char = uartRXBuf[uartRxQueueTail];        /* fetch the character           */

        uartRxQueueTail++;                    /* adjust it to the next         */

        if( uartRxQueueTail == UART_RXBUF_SIZE)
        {
            uartRxQueueTail = 0x00;                   /* going round                   */
        }
        uartRXByteCount--;                     /* decrement character counter   */
    }
    else
    {
        bOK = FALSE;
    }
    //ES1 = 1;                      // enable uart interrupt
    //_SetInt_SERIAL1(ENABLE);

    return (bOK);

} /* UART_GetChar() */

void UART_Clear(void)
{
    uartRXByteCount   = 0x00;                  /* reset pointers and counter    */
     uartRxQueueHeader = 0x00;
    uartRxQueueTail = 0x00;
}

#undef _DRVUART_C_
