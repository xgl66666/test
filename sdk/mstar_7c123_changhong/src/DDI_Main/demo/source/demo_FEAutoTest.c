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

#if (FE_AUTO_TEST == 1)
#define _DRVUART_C_

#include "Board.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "demo_FEAutoTest.h"
#define PHOENIX_DEBUG   1

//#define UARTMSG(x)    x

#if (CHANNEL_SCAN_AUTO_TEST)
#define CHSCANTEST_MSG(x)    x
#else
#define CHSCANTEST_MSG(x)
#endif


//static const U8 CMD_TRE_ACK[] = {'P', 'T','R','E', 'A','C','K', 'P'};
//static MS_U8 _u8UTaskStartStk[8192];



/******************************************************************************/
/// UART device: putchar and ISR
/******************************************************************************/
#define TEST_TUNER_INDEX 0
//static MS_U32 uart_debug;
#include "drvDemod.h"
#include "apiDigiTuner.h"
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

//static int g_SpecChar = 0xF4;
#if (defined(DEMO_UART_TEST)&&(DEMO_UART_TEST == 1))
extern void cyg_uart_putchar(unsigned char c);
extern char cyg_uart_getchar(void);

#elif (defined(DEMO_UART_TTY_TEST)&&(DEMO_UART_TTY_TEST == 1))
static int g_iFD = -1;
struct termios t_tios;
struct termios old_tios;
struct termios tios;
#endif  //#endif (defined(DEMO_UART_TTY_TEST)&&(DEMO_UART_TTY_TEST == 1))

#define MAX_PG_IN_PAT           8
typedef enum
{
    SiNOT_VALID                    = 0x00,
    SiMPEG                         = 0x01,
    SiAC3                          = 0x02,
    SiMPEG_AD                      = 0x03,
    SiMPEG4                        = 0x04,
    SiAAC                          = 0x05,
    SiAC3P                         = 0x06,
    SiAC3_AD                       = 0x07,
} SI_AudioFormat;

typedef struct
{
    MS_U32 u32PidVideo;
    MS_U32 u32PidAudio;
    MS_U32 u32PidPcr;
    MS_U32 u32StreamTypeVideo;
    MS_U32 u32StreamTypeAudio;
    MS_U32 u32ProgNum;
    MS_U32 u32PidPmt;
    MS_BOOL b264;
    SI_AudioFormat eAudioFmt;
} stPG;

/*
typedef struct
{
    stPG stPgList[MAX_PG_IN_PAT];
    MS_U32 u32PgNum;
} stFreqPG;
*/

extern MS_BOOL appDemo_Dmx_GetFreqPG(stFreqPG **ppEntry);

#if (defined(DEMO_UART_TEST)&&(DEMO_UART_TEST == 1))
static MS_BOOL getcharb(MS_U8 *byte)
{
    *byte = cyg_uart_getchar();
    return TRUE;
}

static void putcharb(MS_U8 byte)
{
    cyg_uart_putchar(byte);
}

static MS_BOOL _UartInit(void)
{
    return TRUE;
}

static MS_BOOL _UartDeInit(void)
{
    return TRUE;
}

#elif (defined(DEMO_UART_TTY_TEST)&&(DEMO_UART_TTY_TEST == 1))
static MS_BOOL _UartInit(void)
{
    // redirect stdin
    freopen("/dev/null", "r", stdin);
    system("echo \"0 0 0 0\" > /proc/sys/kernel/printk");
    // open tty for uart transmission
    g_iFD = open("/dev/ttyS0", O_RDWR);

    memset(&old_tios, 0, sizeof(struct termios));
    tcgetattr(g_iFD, &old_tios);
    tcgetattr(g_iFD, &tios);

    //tios.c_lflag &= ~(ECHO | ECHONL | ICANON);
    tios.c_lflag &= ~( ECHO | ECHONL | ICANON | ISIG | IEXTEN );
    //    tios.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    tios.c_iflag &= ~(INLCR | ICRNL | IXON);
    tios.c_iflag |= IXANY;
    tcsetattr(g_iFD, TCSAFLUSH, &tios);
    return TRUE;
}

static MS_BOOL _UartDeInit(void)
{
    tcsetattr(g_iFD, TCSAFLUSH, &old_tios);
    if(g_iFD>0)
    {
        close(g_iFD);
    }
    g_iFD = -1;

    // restore stdout
    //freopen ("/dev/tty", "a", stdout);
    //restore std in.
    freopen ("/dev/tty", "r", stdin);
    system("echo \"7 4 1 7\" > /proc/sys/kernel/printk");
    return TRUE;
}

static void putcharb(MS_U8 byte)
{
#if 1
    if (g_iFD!=-1)
    {
        MS_U8 tmp[2] = {0};
        tmp[0]=byte;
        write(g_iFD,tmp, 1);
    }
#else
   fputc(byte, stdout);
#endif
}
static MS_BOOL getcharb(MS_U8 *byte)
{
#if 1
    char u8Str[2]={0};

    fd_set read_fds;
    struct timeval timeout;
    if (g_iFD<0) return FALSE;
    FD_ZERO(&read_fds);
    FD_SET(g_iFD, &read_fds);
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    int iRet = select(g_iFD + 1, &read_fds, NULL, NULL, &timeout);
    if( iRet <= 0 ) // Timeout or something error
    {
        return FALSE;
    }
    int readsize=read(g_iFD, u8Str, 1);
    *byte = u8Str[0];
    return readsize>0 ? TRUE: FALSE;
#else
    int c;

    c = fgetc(stdin);

      *byte = c;
      printf("getcharb = %x\n",*byte );
      return TRUE;
#endif
}

#endif //#endif  (defined(DEMO_UART_TTY_TEST)&&(DEMO_UART_TTY_TEST == 1))

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
}

static Task_Info _gpolling_task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "uart_poll_task"};
static MS_BOOL _gbQuitTask = FALSE;
#if 0
static void _Debug_Cmd(char charCode)
{
    static MS_U8 u8CmdString[100]={0};
    static MS_U8 u8CmdIndex = 0;
    u8CmdString[u8CmdIndex++]=charCode;
    if (g_bUart0Detected)
    {
        int i=0;
        putcharb('c');
        putcharb('m');
        putcharb('d');
        putcharb(' ');

        for (i=0; i<u8CmdIndex; i++)
        {
            MS_U8 tmp= u8CmdString[i];
            putcharb('0');
            putcharb('x');
            if ((u8CmdString[i]>>4)>9)
                putcharb('a'+(tmp>>4)-10);
            else
                putcharb('0'+(tmp>>4));
            if ((u8CmdString[i]&0xF)>9)
                putcharb('a'+(tmp&0xf)-10);
            else
                putcharb('0'+(tmp&0xf));
            putcharb(' ');

        }
        putcharb('\r');
        putcharb('\n');
        u8CmdIndex = 0;
    }
}
#endif

static MS_BOOL _gCmdReady = FALSE;
static MS_U8 _gCmdArray[1024]={0};
static MS_U8 _gCmdSize=0;
static MS_U32 _u32frontend_index=0;
#ifdef SUPPORT_DUAL_LNB
static MS_U32 _u32RFIndex=0;
#endif
static void _msAPI_uart_polling_task(MS_U32 argc, void *argv)
{
    MS_U8 u8Char=0;
    MS_U32 u32Value1=0, u32Value2=0;
    char u8QuitString[8]={0};
    g_UartCommand.Index = 0;
    g_bDebugASCIICommandFlag = 0;
    g_bDebugProgStopFlag = FALSE;
    g_Uart0CheckTick = 0;
    MS_BOOL bCmdInput=FALSE;
 /*
    EN_AV_Device eDevice;
    EN_DEMO_DMX_FLOW eDmxFlow;
    EN_DEMO_DMX_FLOW_INPUT eDmxInput;
    int ClkInv;
    int ExtSync;
    int Paral;
    */

    //MS_U32 argc=0, argv=0;

    //Demo_Main_Exit(argc, argv);

    //MS_U32 u32OutputTiming = 6;
/*
    MS_U16 x = 0;
    MS_U16 y = 0;
    MS_U16 width = 720;
    MS_U16 height = 576;
*/
    Demo_Main_Exit(argc, argv);

    _UartInit();

    //appDemo_XC_Init();
    u32Value1=0;
    Demo_XC_Init(&u32Value1);

    //appDemo_VE_Init();
    Demo_VE_Init();

    //appDemo_XC_SetOutputTiming(&u32OutputTiming, &x, &y, &width, &height);
    u32Value1=0, u32Value2=2;
    Demo_XC_SetOutputTiming(&u32Value1, &u32Value2);

    //u32OutputTiming = 1;
    //appDemo_VE_SetOutputTiming(&u32OutputTiming, &x, &y, &width, &height);
    u32Value1=1;
    Demo_VE_SetOutputTiming(&u32Value1);

    //u32Value1=0;
    //appDemo_DigiTuner_SetIndex(&_u32frontend_index);

    u32Value1=1;
    Demo_Audio_Init(&u32Value1);
/*
    eDevice = E_AV_DEVICE_MAIN;
    eDmxFlow = E_DMX_FLOW_LIVE0;
    eDmxInput = E_DMX_FLOW_INPUT_EXT_INPUT0_3WIRE + _u32frontend_index; // Knight
    //eDmxInput = E_DMX_FLOW_INPUT_EXT_INPUT3_3WIRE - _u32frontend_index; // Koln
    //eDmxInput = E_DMX_FLOW_INPUT_EXT_INPUT0 + _u32frontend_index; //MxL254 4wire
    ClkInv = 0;
    ExtSync = 1;
    Paral = 0;

    Demo_AV_Tuner_Config(&eDevice, &eDmxFlow, &eDmxInput, &ClkInv, &ExtSync, &Paral);
*/
    while(_gbQuitTask==FALSE)
    {
        if (getcharb(&u8Char)==FALSE)
        {
            continue;
        }

        u8QuitString[6]=u8QuitString[5];
        u8QuitString[5]=u8QuitString[4];
        u8QuitString[4]=u8QuitString[3];
        u8QuitString[3]=u8QuitString[2];
        u8QuitString[2]=u8QuitString[1];
        u8QuitString[1]=u8QuitString[0];
        u8QuitString[0]=u8Char;
        if (strcmp(u8QuitString, "7896655")==0 && bCmdInput == FALSE)
        {
           bCmdInput = TRUE;
           g_UartCommand.Index = 0;
           g_bUart0Detected = FALSE;
        }
        else if (strcmp(u8QuitString, "7896655")==0 && bCmdInput == TRUE)
        {
           bCmdInput = FALSE;
           g_UartCommand.Index = 0;
           g_bUart0Detected = FALSE;
        }

        #if 1
        if (bCmdInput)
        {
            if ( u8Char >=32 && u8Char<=126)
            {
                putcharb(u8Char);
                _gCmdArray[_gCmdSize++]=u8Char;
            }
            if (u8Char == 0xd)
            {
                _gCmdArray[_gCmdSize++] = 0;
                _gCmdReady = TRUE;
                int i=0;
                //wait appDemo_FEAutoTest_GetCmd
                while(_gCmdReady == TRUE && i++<200)
                {
                    MsOS_DelayTask(50);
                }
                _gCmdReady = FALSE;
                _gCmdSize = 0;

            }
            continue;
        }
        MDrv_UART_RecvHandler(u8Char);
       // _Debug_Cmd(u8Char);

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
        }
        #endif
    }
    _UartDeInit();
    _gbQuitTask = FALSE;
    _gpolling_task.iId= -1;
}

extern MS_U16  MDrv_Demod_ReadReg(MS_U16 RegAddr);
extern MS_BOOL MDrv_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData);

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
    U16 RegAddr, RegData;
    MS_U8 u8RegData;
#endif
    //printf("UART_CMD=0x%x\n", UART_CMD);
    switch ( UART_CMD )
    {
#if PHOENIX_DEBUG
        case uartRead_Demod:
            {
                printf("uartRead_Demod\n");
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];

                //RegData = MDrv_Demod_ReadReg( RegAddr );
                MApi_DigiTuner_ReadReg(_u32frontend_index, RegAddr, &u8RegData);
                RegData = (U16 )u8RegData;
                putcharb( 0xF3 );
                putcharb( ( U8 ) ( RegData >> 8 ) );
                putcharb( ( U8 ) ( RegData & 0xff ) );
                printf("[%x] = %x\n", RegAddr, RegData);
            }
            break;
        case uartWrite_Demod:
            {
                printf("uartWrite_Demod\n");
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                RegData = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX3_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX4_];
                //RegData = MDrv_Demod_WriteReg( RegAddr, RegData );
                MApi_DigiTuner_WriteReg(_u32frontend_index, RegAddr, RegData);
                putcharb( 0xF1 );
                printf("[%x] = %x\n", RegAddr, RegData);
            }
            break;
#endif
#if 0 //CANOPUS_DEBUG

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
       case UART_SCAN_CMD_RETURN_TYPE:
        printf("%02x %02x\n", (0xC0 | 11),18);

        break;
       case UART_SCAN_CMD_SET_TUNE_TO_RFCH:
       {
            MS_U32 u32Data = 0;
            MS_FE_CARRIER_PARAM stFeParam;
            MS_BOOL bRet = FALSE;
            MS_U32 u32Value1=0, u32Value2=0;//, u32Value3=0;
            EN_AV_Device enDeviceID=E_AV_DEVICE_MAIN;
            memset(&stFeParam,0,sizeof(MS_FE_CARRIER_PARAM));

            u32Data = (g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX5_]| (u32Data<<8);
            u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX6_]| (u32Data<<8);
            u32Data = g_UartCommand.Buffer[_UART_CMD_INDEX7_]| (u32Data<<8);
            stFeParam.u32Frequency = u32Data;

            printf("[%s %d]UART_SCAN_CMD_SET_TUNE_TO_RFCH \n",__FUNCTION__,__LINE__);
            switch (UART_EXT_CMD_MS_DAT6)
            {
                case E_STANDARD_DVBC:              
                    u32Value1=1;
                    appDemo_Frontend_SetBroadcastType(&u32Value1);
                    MApi_DigiTuner_Init(_u32frontend_index);

                    u32Value2 = (g_UartCommand.Buffer[_UART_CMD_INDEX10_]);
                    u32Value2 = g_UartCommand.Buffer[_UART_CMD_INDEX11_]| (u32Value2<<8);
                    u32Value2 = g_UartCommand.Buffer[_UART_CMD_INDEX12_]| (u32Value2<<8);
                    u32Value2 = g_UartCommand.Buffer[_UART_CMD_INDEX13_]| (u32Value2<<8);
                    u32Value1 = g_UartCommand.Buffer[_UART_CMD_INDEX8_]; //_eDVBCQAMMod, u8QAMType;
                    printf("[DVBC param] %d, %d, %d\n", (int)stFeParam.u32Frequency, (int)u32Value1, (int)u32Value2);
                    bRet = appDemo_DigiTuner_SetFreq(&stFeParam.u32Frequency,&u32Value1, &u32Value2);
                    putcharb( 0xF3 );
                    printf("OK\n");
                    break;
                case E_STANDARD_DVBS2:
                case E_STANDARD_DVBS:
                    if(UART_EXT_CMD_MS_DAT6 == E_STANDARD_DVBS2)
                        u32Value1=11;
                    else                          
                        u32Value1=5;
                    
                    appDemo_Frontend_SetBroadcastType(&u32Value1);
                    MApi_DigiTuner_Init(_u32frontend_index);
                    u32Value1 = g_UartCommand.Buffer[_UART_CMD_INDEX10_];
                    u32Value1 = g_UartCommand.Buffer[_UART_CMD_INDEX11_]| (u32Value1<<8);
                    u32Value1 = g_UartCommand.Buffer[_UART_CMD_INDEX12_]| (u32Value1<<8);
                    u32Value1 = g_UartCommand.Buffer[_UART_CMD_INDEX13_]| (u32Value1<<8);
                    printf("[DVBS/S2 param] %d, %d\n", (int)stFeParam.u32Frequency, (int)u32Value1);
                    u32Value2 = 0;
                    #ifdef SUPPORT_DUAL_LNB
                    u32Value2 = _u32RFIndex;
                    #endif
                    bRet = appDemo_DigiTuner_SetFreq(&stFeParam.u32Frequency,&u32Value1, &u32Value2);
                    putcharb( 0xF3 );
                    printf("OK\n");
                    break;
                case E_STANDARD_DVBT2:
                    u32Value1=3;
                    appDemo_Frontend_SetBroadcastType(&u32Value1);
                    MApi_DigiTuner_Init(_u32frontend_index);
                    stFeParam.TerParam.eBandWidth = (EN_TER_BW_MODE)(g_UartCommand.Buffer[_UART_CMD_INDEX8_]-1);
                    u32Value1=(MS_U32)stFeParam.TerParam.eBandWidth+1;
                    printf("[DVBT2 param] %d, %d\n", (int)stFeParam.u32Frequency, (int)u32Value1);
                    bRet = appDemo_DigiTuner_SetFreq(&stFeParam.u32Frequency, &u32Value1, NULL);
                    putcharb( 0xF3 );
                    printf("OK\n");
                    break;
                case E_STANDARD_DVBT:
                    u32Value1=2;
                    appDemo_Frontend_SetBroadcastType(&u32Value1);
                    MApi_DigiTuner_Init(_u32frontend_index);
                    stFeParam.TerParam.eBandWidth = (EN_TER_BW_MODE)(g_UartCommand.Buffer[_UART_CMD_INDEX8_]-1);
                    u32Value1=(MS_U32)stFeParam.TerParam.eBandWidth+1;
                    printf("[DVBT param] %d, %d\n", (int)stFeParam.u32Frequency, (int)u32Value1);
                    bRet = appDemo_DigiTuner_SetFreq(&stFeParam.u32Frequency, &u32Value1, NULL);
                    putcharb( 0xF3 );
                    printf("OK\n");
                    break;
                case E_STANDARD_ISDBT:
                    u32Value1=4;
                    appDemo_Frontend_SetBroadcastType(&u32Value1);
                    MApi_DigiTuner_Init(_u32frontend_index);
                    stFeParam.TerParam.eBandWidth = (EN_TER_BW_MODE)(g_UartCommand.Buffer[_UART_CMD_INDEX8_]-1);
                    u32Value1=(MS_U32)stFeParam.TerParam.eBandWidth+1;
                    bRet = appDemo_DigiTuner_SetFreq(&stFeParam.u32Frequency, &u32Value1, NULL);
                    putcharb( 0xF3 );
                    printf("ISDBT OK\n");
                    break;
                default:
                    printf("DVB???\n");
                    break;
            }
            
            appDemo_FEAutoTest_PGPlay(&enDeviceID);

            if (bRet == TRUE)
            {
                printf("OK\n");
            }
            else
            {
                printf("Set Demod frequency failed\n");
            }

        }
        break;

        case UART_SCAN_CMD_LOCK_STATE:
            {
#if PHOENIX_DEBUG
                EN_LOCK_STATUS lockState = E_DEMOD_CHECKING;
                //EN_LOCK_STATUS lockState_all[4]={1,1,1,1};
                //MS_U8 i;

                //printf("UART_EXT_CMD_MS_DAT1=%d\n", UART_EXT_CMD_MS_DAT1);
                switch(UART_EXT_CMD_MS_DAT1)
                {
                    case E_STANDARD_DVBS:
                    case E_STANDARD_DVBS2:
                    case E_STANDARD_DVBT:
                    case E_STANDARD_DVBT2:
                    case E_STANDARD_ISDBT:
                    default:
                    {

/*
                        for(i=0;i<4;i++)
                        {
                            MApi_DigiTuner_GetLock(i,&lockState_all[i]);
                        }

                        if((lockState_all[0] == E_DEMOD_LOCK) &&\
                            (lockState_all[1] == E_DEMOD_LOCK) &&\
                            (lockState_all[2] == E_DEMOD_LOCK) &&\
                            (lockState_all[3] == E_DEMOD_LOCK))
                        {
                             lockState = E_DEMOD_LOCK;
                        }
                        */
                        MApi_DigiTuner_GetLock(_u32frontend_index,&lockState);
                    }
                    break;
                }
                if (lockState == E_DEMOD_LOCK)
                {
                    putcharb( 0xF3 );
                    printf("OK\n");
                }
                else
                {
                    putcharb( 0xF3 );
                    printf("err2\n");
                }
#endif
            }
            break;
        case UART_SCAN_CMD_CHECK_PACKET_ERR:
            {
                MS_U16 u16PktErr = 0;
                //if (MDrv_Demod_Get_Packet_Error(&u16PktErr)==TRUE)

                if (MApi_DigiTuner_Get_Packet_Error(_u32frontend_index,&u16PktErr)==TRUE)
                {
                    if (u16PktErr != 0)
                    {
                        putcharb( 0xF3 );
                        printf("err1\n");
                    }
                    else
                    {
                        putcharb( 0xF3 );
                        printf("OK\n");
                    }
                }
                else
                {
                    printf("error\n");
                }
            }
            break;
        case UART_SCAN_CMD_GET_BER:
            {
                MS_BOOL bRet = FALSE;
                float fBER = 0;
                bRet = MApi_DigiTuner_GetBER(0, &fBER);
                //bRet = MDrv_Demod_GetBer(&fBER);
                if (bRet == FALSE)
                {
                    //printf("err\n");
                }
                else
                {
                    //printf("OK\n");
                    printf("%e\n",fBER);
                }
            }
            break;
        default:
            printf("Not handle Ext UART_EXT_CMD = %02x\n", UART_EXT_CMD);
            break;
    }
}
#endif


void Cus_UART_DecodeExtCommand(void)
{
    //printf("+++UART_EXT_DEV=0x%x\n", UART_EXT_DEV);

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
            printf("Not handle Ext UART_EXT_DEV = %02x\n", UART_EXT_DEV);
            break;
    }
}
/******************************************************************************/
///Initial UART relative variable and flag.
/******************************************************************************/
MS_BOOL FE_Auto_Test(MS_BOOL bOn)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    // reset Uart variables
    if (bOn)
    {
        if (_gpolling_task.iId>0) return TRUE;
        if(_gpolling_task.iId == -1)
        {

            _gpolling_task.pStack = MsOS_AllocateMemory(_gpolling_task.u32StackSize, s32MstarCachedPoolID);
            if(!_gpolling_task.pStack)
            {
                printf("[%s][%d] fail 1\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            _gpolling_task.iId= MsOS_CreateTask(_msAPI_uart_polling_task, (MS_U32)NULL, _gpolling_task.ePriority,
                                         TRUE, _gpolling_task.pStack, _gpolling_task.u32StackSize, _gpolling_task.szName);

            if(_gpolling_task.iId < 0)
            {
                printf("[%s][%d] fail 2\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
        }
        printf("FE_Auto_Test TRUE\n");
   }
   else
   {
        if (_gpolling_task.iId<=0) return TRUE;
        _gbQuitTask = TRUE;
        while(_gbQuitTask==TRUE)
        {
            MsOS_DelayTaskUs(10000);
        };
        if (MsOS_FreeMemory(_gpolling_task.pStack, s32MstarCachedPoolID))
        {
            _gpolling_task.iId= -1;
        }
        else
        {
            printf("[%s][%d] fail 3\n", __FUNCTION__, __LINE__);
            return FALSE;
        }


   }
   return TRUE;
}

MS_BOOL appDemo_FEAutoTest_Start(MS_U32 *FrontendIndex, MS_U32 *RFIndex)
{
    _u32frontend_index = *FrontendIndex;
#ifdef SUPPORT_DUAL_LNB
    _u32RFIndex = *RFIndex;
#endif
    appDemo_DigiTuner_SetIndex(&_u32frontend_index);
    return FE_Auto_Test(TRUE);
}

MS_BOOL appDemo_FEAutoTest_GetCmd(MS_U8 *pCmd)
{
    while(1)
    {
        if (_gCmdReady)
        {
            strcpy((char*)pCmd, (char*)_gCmdArray);
            _gCmdReady = FALSE;
            return TRUE;
        }
        MsOS_DelayTask(50);
    }

    return FALSE;
}

MS_BOOL appDemo_FEAutoTest_PGPlay(EN_AV_Device* penDeviceID)
{
    stFreqPG *pEntry= NULL;
    MS_U32 u32Pid_Video = 0;
    MS_U32 u32Pid_Audio = 0;
    MS_U32 u32Pid_PCR = 0;
    MS_U32 u32V_Type = 0;
    MS_U32 u32A_Type = 0;
    MS_U32 u32Value1=0, u32Value2=0, u32Value3=0;


    if(TRUE == Demo_DMX_Scan_CMD((MS_U8*)"LIVE0"))
    {
         appDemo_Dmx_GetFreqPG(&pEntry);
         u32Pid_Video = pEntry->stPgList[0].u32PidVideo;
         u32Pid_Audio = pEntry->stPgList[0].Audio_PG[0].u32PidAudio;
         u32Pid_PCR = pEntry->stPgList[0].u32PidVideo;
         u32A_Type = pEntry->stPgList[0].Audio_PG[0].eAudioFmt;
         u32V_Type = (MS_U32)pEntry->stPgList[0].eVideoFmt-1;
         printf(" (%lu, %lu, %lu) (%lu, %lu)", u32Pid_Video,u32Pid_Video,u32Pid_PCR, u32V_Type, u32A_Type);
                
         Demo_AV_TSP_SetPid(penDeviceID, &u32Pid_Video, &u32Pid_Audio, &u32Pid_PCR, &u32V_Type, &u32A_Type);
         Demo_AV_PlayAV(penDeviceID);
         Demo_XC_PlayVideo(&u32Value1, &u32Value2, &u32Value3);
         Demo_VE_PlayVideo();
    }
    else
    {
        return FALSE;
    }

    return TRUE;    
}

#endif  //#ifdef DDI_FE_AUTO_TEST
