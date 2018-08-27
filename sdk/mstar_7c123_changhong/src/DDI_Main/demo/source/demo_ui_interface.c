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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_ZUI_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/string.h>
#include "MsCommon.h"
#include "demo_ui_interface.h"
#include "MApp_ZUI_Utility.h"
#include "MApp_ZUI_OsdId.h"
#include "demo_xc.h"
#include "mmsdk_interface_def.h"
#include "demo_mm.h"
#include "apiAUDIO.h"
#include "demo_audio.h"
#include "demo_cec.h"
#include "msAPI_CEC.h"
#include "drvDTC.h"
#include "apiCEC.h"
#include "msAPI_HDMI.h"
#include "MsMemory.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "MsFS.h"
#include "demo_os.h"
#include "drvMMIO.h"
#include "demo_utility.h"
#include "drvDTC.h"
#include "SysBsp.h"

#include "demo_os.h"
#include "demo_osd.h"
#if (DEMO_XC_HDMIRX_TEST == 1)
#include "drvXC_HDMI_if.h"
#endif

/****************************************************************************/
/* local defines                                                                                                         */
/****************************************************************************/
#define MM_PLAY_FILEOPTION_CLOSE_DS 0x200

/****************************************************************************/
/* local enum                                                                                                           */
/****************************************************************************/
typedef enum{
    E_HDMI_REPEATER_TXONLY = 1,
    E_HDMI_REPEATER_CONVERTER,
    E_HDMI_REPEATER_BYPASS,
    E_HDMI_REPEATER_SHOWRXINFO,
    E_HDMI_REPEATER_INVALID = 0xFF,
}EN_HDMI_REPEATER;

typedef enum{
    E_USB_TEST_IDLE = 0,
    E_USB_TEST_START,
    E_USB_TEST_PROCCESSING,
    E_USB_TEST_KILL,
    E_USB_TEST_STOP,
    E_USB_TEST_ERROR,
}EN_USB_TEST_STATE;
/****************************************************************************/
/* local structure                                                                                                       */
/****************************************************************************/
typedef struct _TUSB_TEST_PARM
{
    MS_U8 u8USBTestState;

    char*   pFullPath;
    MS_S32 s32NonCachedPoolID;
    USBTestArgs tSetting;
    MS_U8* pArray;


    Task_Info USB_WR_Task;
    MS_S32    s32NonCachedPoolID_USB_Test_Task;

    Task_Info USB_Delete_Task;
    MS_S32    s32NonCachedPoolID_USB_Delete_Task;
}TTUSB_TEST_PARM,*PNUSB_TEST_PARM;
static TTUSB_TEST_PARM  tUSBTestArgs;
static  MS_BOOL bUSB_Start = FALSE;


/****************************************************************************/
/* local variable                                                                                                        */
/****************************************************************************/
EN_HDMI_REPEATER eRepeaterMode = E_HDMI_REPEATER_CONVERTER;
MSAPI_XC_HDMIRX_GC_PACKET sGC_PACKET;
MSAPI_XC_HDMIRX_VS_PACKET sVS_PACKET;
MSAPI_XC_HDMIRX_AUDIO_PACKET sAUDIO_PACKET;
MSAPI_XC_HDMIRX_AVI_PACKET sAVI_PACKET;

MS_U8 u8LatestIdx = 0;
MS_U16 u16LineCount = 0;

//HDCP TX Info
E_MSAPI_XC_HDMITX_HDCP_VERSION_INFO _eHDCPTxVersionInfo = E_MSAPI_XC_HDMITX_HDCP_VERSION_22;
E_MSAPI_XC_HDMITX_HDCP_STATUS_INFO _eHDCPTxStatus = E_MSAPI_XC_HDMITX_HDCP_STATUS_DISABLE;
#if (DEMO_XC_HDMIRX_TEST == 1)
//HDCP RX Info
static stCMD_HDMI_CHECK_HDCP_ENC_STATE _stHDCPENCState = {0};
#endif
//HDMI TX Info
MSAPI_XC_HDMITX_OUTPUT_INFO _stAPI_XC_HDMITxOutputInfo;

MSAPI_HDMITX_EDID_BLOCK0 g_stEDID_Block0;  //block 0
MSAPI_HDMITX_EDID_BLOCK1 g_stEDID_Block1;  //block 1
MS_U16 g_u16EDID_Skip_line = 0;       //Because audio data line max is equal to 20, but it is not good if we create 20 line instead of use global value to record
/****************************************************************************/
/* extern global variable                                                                                            */
/****************************************************************************/
extern E_MSAPI_CEC_MESSAGE_BUFFER* g_stCECMessageBuffer;
extern MS_U32 u32_UICECDSTLA;
extern MS_U32 u32_UICECDSTOpcode;
extern MS_U32 u32_UICECLength;
extern MS_U32 u32_UICECOperand[14];

/****************************************************************************/
/* Demo Callbakc Functions                                                                                        */
/****************************************************************************/
extern MS_BOOL Demo_MM_Play_Test(void)
{
    MS_BOOL bCmdPass;
    MS_U32 u32Replay = TRUE;
    MS_U32 u32FileOption = 0;

    MS_U32 u32HD_GOP = 0;
    MS_U32 u32SD_GOP = 1;
    MS_U32 u32HD_DST = 0;
    MS_U32 u32SD_DST = 0;

    printf("\33[36m[%s][%d]MM_STOP\33[m\n",__FUNCTION__,__LINE__);
    if(Demo_MM_Stop() == FALSE)
    {
        printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    bCmdPass = Demo_MM_Play(&u32FileOption, (MS_U8 *)"/mnt/sdb1/HiHD13mHEst.trp");
    if( bCmdPass == FALSE )
    {
        printf("[%s][%d] MM play fail !\n",__FUNCTION__,__LINE__);
        return bCmdPass;
    }

    bCmdPass = Demo_MM_Repeat(&u32Replay);
    if( bCmdPass == FALSE )
    {
        printf("[%s][%d] MM Repeat fail !\n",__FUNCTION__,__LINE__);
        return bCmdPass;
    }

    return bCmdPass;
}

MS_BOOL _USB_AllocateMemroy( TTUSB_TEST_PARM* ptUSBTestArgs )
{
    ptUSBTestArgs->pFullPath = MsOS_AllocateMemory(strlen("/mnt/sda1/test.bin") + 1, ptUSBTestArgs->s32NonCachedPoolID);
    memset(ptUSBTestArgs->pFullPath, 0, strlen("/mnt/sda1/test.bin") + 1);
    if(!ptUSBTestArgs->pFullPath)
    {
        printf("[%s][%d] allocate memory failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    strncpy(ptUSBTestArgs->pFullPath,"/mnt/sda1", strlen("/mnt/sda1"));
    strncat(ptUSBTestArgs->pFullPath, "/test.bin", strlen("/test.bin"));
    if(strlen(ptUSBTestArgs->pFullPath) == 0)
    {
        if ( MsOS_FreeMemory(ptUSBTestArgs->pFullPath, ptUSBTestArgs->s32NonCachedPoolID) == FALSE)
        {
            printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
        }

        printf("[%s][%d] string concat failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    ptUSBTestArgs->pArray = MsOS_AllocateMemory(ptUSBTestArgs->tSetting.u32BlockSize, ptUSBTestArgs->s32NonCachedPoolID);
    if(!ptUSBTestArgs->pArray)
    {
        if ( MsOS_FreeMemory(ptUSBTestArgs->pFullPath, ptUSBTestArgs->s32NonCachedPoolID) == FALSE)
        {
            printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
        }

        printf("[%s][%d] allocate write data memory failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
}

MS_BOOL _USB_FreeMemroy( TTUSB_TEST_PARM* ptUSBTestArgs )
{
    MS_BOOL bRet = TRUE;

    if (MsOS_FreeMemory(ptUSBTestArgs->pArray, ptUSBTestArgs->s32NonCachedPoolID) == FALSE )
    {
        printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
        bRet= FALSE;
    }

    if ( MsOS_FreeMemory(ptUSBTestArgs->pFullPath, ptUSBTestArgs->s32NonCachedPoolID) == FALSE)
    {
            printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
            bRet= FALSE;
    }

    return bRet;
}

void _USB_Test_Kill_Test_Task (TTUSB_TEST_PARM* ptUSBTestArgs)
{
    while(1)
    {
        if( ptUSBTestArgs->u8USBTestState == E_USB_TEST_KILL )
        {
            if (MsOS_FreeMemory(ptUSBTestArgs->USB_WR_Task.pStack, ptUSBTestArgs->s32NonCachedPoolID_USB_Test_Task) == FALSE )
            {
                printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
            }
            else
            {
                if ( MsOS_DeleteTask(ptUSBTestArgs->USB_WR_Task.iId) == FALSE )
                {
                    printf("[%s][%d] kill USB test task failed\n", __FUNCTION__, __LINE__);
                }
                else
                {
                    ptUSBTestArgs->u8USBTestState = E_USB_TEST_STOP;
                }
            }
        }
        if( ptUSBTestArgs->u8USBTestState == E_USB_TEST_STOP )break;
    }
}

void _USB_Test_Task (TTUSB_TEST_PARM*  ptUSBTestArgs )
{

    MS_U8* pArray;

    MS_U32 i;
    MS_U32 iCheck;
    MS_BOOL bComparePass;
    MS_S32  s32PoolID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32PoolID);
    ptUSBTestArgs->s32NonCachedPoolID = s32PoolID;

    if( _USB_AllocateMemroy( ptUSBTestArgs ) == FALSE )
    {
        ptUSBTestArgs->u8USBTestState = E_USB_TEST_ERROR;
        return;
    }
    ptUSBTestArgs->u8USBTestState = E_USB_TEST_PROCCESSING;

    while(1)
    {
            // 1. USB Write to test.bin
            printf("Write >> ");

            FILE* writeFile = MsFS_Fopen(ptUSBTestArgs->pFullPath, "wb");
            if(!writeFile)
            {
                printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
                continue;
            }

            pArray = ptUSBTestArgs->pArray;
            MS_U8 hex = (MS_U8) rand();
            MS_U8 u8StartHex = hex;
            for( i=0 ; i<ptUSBTestArgs->tSetting.u32BlockSize ; i++)
            {
                pArray[i] = hex++;
            }

            for(i=0 ; i<ptUSBTestArgs->tSetting.u32Iteration ; i++)
            {
                MsFS_Fwrite(ptUSBTestArgs->pArray, 1, ptUSBTestArgs->tSetting.u32BlockSize, writeFile);
            }

            MsFS_Fclose(writeFile);

            _USB_Test_Clear_Cache();


             // 2. USB Read from test.bin
             printf("Read >> ");

            FILE* readFile = MsFS_Fopen(ptUSBTestArgs->pFullPath, "rb");
            if(!readFile)
            {
                printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
                continue;
            }

            pArray = ptUSBTestArgs->pArray;
            bComparePass = TRUE;
            for( i=0 ; i<ptUSBTestArgs->tSetting.u32Iteration ; i++ )
            {
                MsFS_Fread(pArray,ptUSBTestArgs->tSetting.u32BlockSize, 1, readFile);

                // 3. Compare Block
                for( iCheck=0 ; iCheck<ptUSBTestArgs->tSetting.u32BlockSize ; iCheck++)
                {
                    if( u8StartHex != pArray[iCheck] )
                    {
                        bComparePass = FALSE;
                    }
                    u8StartHex++;
                }
            }
            MsFS_Fclose(readFile);

            if( bComparePass == TRUE )
            {
                printf("\e[1m\e[21m USB Test Pass \e[0m\n");
            }
            else
            {
                printf("\e[1m\e[21m USB Test Fail \e[0m\n");
            }

            if( ptUSBTestArgs->u8USBTestState == E_USB_TEST_KILL) break;
    }

    if( _USB_FreeMemroy( ptUSBTestArgs ) == FALSE )
    {
        ptUSBTestArgs->u8USBTestState = E_USB_TEST_ERROR;
    }

}

void _USB_Initial_Args( TTUSB_TEST_PARM* ptUSBTestArgs )
{
    ptUSBTestArgs->u8USBTestState = E_USB_TEST_IDLE;

    ptUSBTestArgs->USB_WR_Task.iId = -1;
    ptUSBTestArgs->USB_WR_Task.ePriority = E_TASK_PRI_HIGH;
    ptUSBTestArgs->USB_WR_Task.pStack = NULL;
    ptUSBTestArgs->USB_WR_Task.u32StackSize = 0x8000;
    sprintf((char*)ptUSBTestArgs->USB_WR_Task.szName, "Write Read Task");
    ptUSBTestArgs->s32NonCachedPoolID_USB_Test_Task = INVALID_POOL_ID;


    ptUSBTestArgs->USB_Delete_Task.iId = -1;
    ptUSBTestArgs->USB_Delete_Task.ePriority = E_TASK_PRI_HIGH;
    ptUSBTestArgs->USB_Delete_Task.pStack = NULL;
    ptUSBTestArgs->USB_Delete_Task.u32StackSize = 0x8000;
    sprintf((char*)ptUSBTestArgs->USB_Delete_Task.szName, "Delete Write Read Task");
    ptUSBTestArgs->s32NonCachedPoolID_USB_Delete_Task = INVALID_POOL_ID;

    ptUSBTestArgs->tSetting.u32BlockSize = 1024;        // BlockSizeInKB = 1* 1024
    ptUSBTestArgs->tSetting.u32Iteration = 8;
}

extern MS_BOOL _USB_Start_Stop_Test( void )
{
    MS_S32  s32PoolID;

    if( bUSB_Start == TRUE )                    // IR press USB WR Start
    {
        _USB_Initial_Args( &tUSBTestArgs );
    }

    if( tUSBTestArgs.u8USBTestState == E_USB_TEST_IDLE || tUSBTestArgs.u8USBTestState == E_USB_TEST_STOP )
    {
        // Kill USB Delete Task
        if( tUSBTestArgs.u8USBTestState == E_USB_TEST_STOP )
        {
            printf("[Debug] E_USB_TEST_STOP \n");
            if (MsOS_FreeMemory(tUSBTestArgs.USB_Delete_Task.pStack, tUSBTestArgs.s32NonCachedPoolID_USB_Delete_Task) == FALSE )
            {
                printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            else
            {
                if ( MsOS_DeleteTask(tUSBTestArgs.USB_Delete_Task.iId) == FALSE )
                {
                    printf("[%s][%d] kill USB delete task failed\n", __FUNCTION__, __LINE__);
                    return FALSE;
                }
            }
        }
        else
        {
            // Create USB Test Task
            tUSBTestArgs.u8USBTestState = E_USB_TEST_START;

            Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32PoolID);
            tUSBTestArgs.s32NonCachedPoolID_USB_Test_Task = s32PoolID;


            tUSBTestArgs.USB_WR_Task.pStack = MsOS_AllocateMemory(tUSBTestArgs.USB_WR_Task.u32StackSize, tUSBTestArgs.s32NonCachedPoolID_USB_Test_Task);
            if(!tUSBTestArgs.USB_WR_Task.pStack)
            {
                printf("[%s][%d] allocate write memory failed\n", __FUNCTION__, __LINE__);
                return FALSE;
            }

            tUSBTestArgs.USB_WR_Task.iId = MsOS_CreateTask( (TaskEntry)_USB_Test_Task,
                                                            (MS_U32)&tUSBTestArgs,
                                                            tUSBTestArgs.USB_WR_Task.ePriority,
                                                            true,
                                                            tUSBTestArgs.USB_WR_Task.pStack,
                                                            tUSBTestArgs.USB_WR_Task.u32StackSize,
                                                            tUSBTestArgs.USB_WR_Task.szName);

        }
    }

    if( tUSBTestArgs.u8USBTestState == E_USB_TEST_PROCCESSING ||  tUSBTestArgs.u8USBTestState == E_USB_TEST_ERROR )
    {
        tUSBTestArgs.u8USBTestState = E_USB_TEST_KILL;

        // Create USB Delete Task to kill USB Test Task
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32PoolID);
        tUSBTestArgs.s32NonCachedPoolID_USB_Delete_Task = s32PoolID;

        tUSBTestArgs.USB_Delete_Task.pStack = MsOS_AllocateMemory(tUSBTestArgs.USB_Delete_Task.u32StackSize, tUSBTestArgs.s32NonCachedPoolID_USB_Delete_Task);
        if(!tUSBTestArgs.USB_Delete_Task.pStack)
        {
            printf("[%s][%d] allocate write memory failed\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        tUSBTestArgs.USB_Delete_Task.iId = MsOS_CreateTask( (TaskEntry)_USB_Test_Kill_Test_Task,
                                                            (MS_U32)&tUSBTestArgs,
                                                            tUSBTestArgs.USB_Delete_Task.ePriority,
                                                            TRUE,
                                                            tUSBTestArgs.USB_Delete_Task.pStack,
                                                            tUSBTestArgs.USB_Delete_Task.u32StackSize,
                                                            tUSBTestArgs.USB_Delete_Task.szName);
    }

    return TRUE;
}
extern MS_BOOL Demo_Analysis_USB_Start_Test( void )
{
    MS_BOOL bRet = TRUE;

    if( bUSB_Start == FALSE )
    {
        bUSB_Start = TRUE;
        bRet = _USB_Start_Stop_Test();
    }
    return bRet;
}

extern MS_BOOL Demo_Analysis_USB_Stop_Test( void )
{
    MS_BOOL bRet = TRUE;

    if( bUSB_Start == TRUE )
    {
        bUSB_Start = FALSE;
        bRet = _USB_Start_Stop_Test();
    }
    return bRet;
}
extern MS_BOOL Demo_Analysis_CPU_Start_Test(void)
{
    system("cd //mnt/sda1/; ./run_smp.sh &");
    return TRUE;
}

extern MS_BOOL Demo_Analysis_CPU_Stop_Test(void)
{
    system("cd //mnt/sda1/; ./terminate_smp.sh");
    system("cd //mnt/sda1/; ./kill_dhry.sh");
    system("mpstat -P ALL 1 5 &");
    system("ps");

    return TRUE;
}

extern MS_BOOL Demo_Analysis_GPU_Start_Test(void)
{
    system("cd //mnt/sda1/; ./pwrConsume &");
    return TRUE;
}

extern MS_BOOL Demo_Analysis_GPU_Stop_Test(void)
{
    system("killall  pwrConsume");
    system("ps");
    return TRUE;
}
extern MS_BOOL Demo_PVR_test_case_1(void)
{
    printf("\33[36m[%s][%d]Demo_PVR_test_case_1\33[m\n",__FUNCTION__,__LINE__);
    return TRUE;
}

extern MS_BOOL Demo_PVR_test_case_2(void)
{
    printf("\33[36m[%s][%d]Demo_PVR_test_case_2\33[m\n",__FUNCTION__,__LINE__);
    return FALSE;
}

extern MS_BOOL Demo_PVR_test_case_3(void)
{
    printf("\33[36m[%s][%d]Demo_PVR_test_case_3\33[m\n",__FUNCTION__,__LINE__);
    return TRUE;
}

extern MS_BOOL Demo_VDEC_test_case_1(void)
{
    printf("\33[36m[%s][%d]Demo_VDEC_test_case_1\33[m\n",__FUNCTION__,__LINE__);
    return TRUE;
}

extern MS_BOOL Demo_MP4_test_case_1(void)
{
    printf("\33[36m[%s][%d]Demo_MP4_test_case_1\33[m\n",__FUNCTION__,__LINE__);
    return TRUE;
}

extern MS_BOOL Demo_Audio_test_case_1(void)
{
    printf("\33[36m[%s][%d]Demo_Audio_test_case_1\33[m\n",__FUNCTION__,__LINE__);
    return TRUE;
}

extern MS_BOOL Demo_Audio_test_case_2(void)
{
    printf("\33[36m[%s][%d]Demo_Audio_test_case_2\33[m\n",__FUNCTION__,__LINE__);
    return FALSE;
}

extern MS_U16 Demo_HDMI_Get_DisplayInfoLineCount(void)
{
    return u16LineCount;
}

extern MS_BOOL Demo_HDMI_ColorFormat_case(MS_U32 *u32Sel)
{
    MS_U32 u32XCColorFormatOutput = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
    MS_U32 u32XCColorFormatInput = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
    MS_BOOL bForceEnable = FALSE;

    if((E_UI_TX_COLORFORMAT_RGB444>*u32Sel) || (*u32Sel>E_UI_TX_COLORFORMAT_YUV420))
    {
        printf("[%s][%d] Invalid Tx ColorFormat %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_COLORFORMAT_RGB444:
            u32XCColorFormatOutput = E_MSAPI_XC_HDMITX_OUTPUT_RGB444;
            break;
        case E_UI_TX_COLORFORMAT_YUV422:
            u32XCColorFormatOutput = E_MSAPI_XC_HDMITX_OUTPUT_YUV422;
            break;
        case E_UI_TX_COLORFORMAT_YUV444:
            u32XCColorFormatOutput = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
            break;
        case E_UI_TX_COLORFORMAT_YUV420:
            u32XCColorFormatOutput = E_MSAPI_XC_HDMITX_OUTPUT_YUV420;
            break;
        default:
            printf("[%s][%d] Set HDMITx ColorFormat Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetOutputColorFormat(&u32XCColorFormatInput,&u32XCColorFormatOutput,&bForceEnable);
}

extern MS_BOOL Demo_HDMI_ColorDepth_case(MS_U32 *u32Sel)
{
    MS_U32 u32XCColorDepth = E_MSAPI_XC_HDMITX_CD_8BITS;

    if((E_UI_TX_COLORDEPTH_8BITS>*u32Sel) || (*u32Sel>E_UI_TX_COLORDEPTH_16BITS))
    {
        printf("[%s][%d] Invalid Tx ColorDepth %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_COLORDEPTH_8BITS:
            u32XCColorDepth = E_MSAPI_XC_HDMITX_CD_8BITS;
            break;
        case E_UI_TX_COLORDEPTH_10BITS:
            u32XCColorDepth = E_MSAPI_XC_HDMITX_CD_10BITS;
            break;
        case E_UI_TX_COLORDEPTH_12BITS:
            u32XCColorDepth = E_MSAPI_XC_HDMITX_CD_12BITS;
            break;
        case E_UI_TX_COLORDEPTH_16BITS:
            u32XCColorDepth = E_MSAPI_XC_HDMITX_CD_16BITS;
            break;
        default:
            printf("[%s][%d] Set HDMITx ColorDepth Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetOutputColorDepth(&u32XCColorDepth);
}

extern MS_BOOL Demo_HDMI_OuputMode_case(MS_U32 *u32Sel)
{
    MS_U32 u32XCOutputMode = 0;

    if((E_UI_TX_OUTPUTMODE_HDMI>*u32Sel) || (*u32Sel>E_UI_TX_OUTPUTMODE_DVI))
    {
        printf("[%s][%d] Invalid Tx OutputMode %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_OUTPUTMODE_HDMI:
            u32XCOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_HDMI;
            break;
        case E_UI_TX_OUTPUTMODE_DVI:
            u32XCOutputMode = E_MSAPI_XC_HDMITX_OUTPUT_DVI;
            break;
        default:
            printf("[%s][%d] Set HDMITx %s OutputMode Fail\n", __FUNCTION__, __LINE__, (*u32Sel==E_UI_TX_OUTPUTMODE_HDMI)?"HDMI":"DVI");
            return FALSE;
            break;
    }

    return Demo_HDMI_SetTxOutputMode(&u32XCOutputMode);
}

extern MS_BOOL Demo_HDMI_OutputTiming_case(MS_U32 *u32Sel)
{
    MS_U32 u32XCDevice = E_MSAPI_XC_DEVICE0;
    MS_U32 u32XCOutputTiming = E_OUTPUT_TIMING_720X480_60P;

    if((E_UI_TX_OUTPUTTIMING_720X480_60I>*u32Sel) || (*u32Sel>=E_UI_TX_OUTPUTTIMING_MAX))
    {
        printf("[%s][%d] Invalid Tx OutputTiming %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    if(Demo_MM_Stop() == FALSE)
    {
        printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case  E_UI_TX_OUTPUTTIMING_720X480_60I:
            u32XCOutputTiming = E_OUTPUT_TIMING_720X480_60I;
            break;
        case  E_UI_TX_OUTPUTTIMING_720X480_60P:
            u32XCOutputTiming = E_OUTPUT_TIMING_720X480_60P;
            break;
        case  E_UI_TX_OUTPUTTIMING_720X576_50I:
            u32XCOutputTiming = E_OUTPUT_TIMING_720X576_50I;
            break;
        case  E_UI_TX_OUTPUTTIMING_720X576_50P:
            u32XCOutputTiming = E_OUTPUT_TIMING_720X576_50P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1280X720_50P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1280X720_50P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1280X720_60P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1280X720_60P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_50I:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_50I;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_60I:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_60I;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_24P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_24P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_25P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_25P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_30P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_30P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_50P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_50P;
            break;
        case  E_UI_TX_OUTPUTTIMING_1920X1080_60P:
            u32XCOutputTiming = E_OUTPUT_TIMING_1920X1080_60P;
            break;
        case  E_UI_TX_OUTPUTTIMING_3840X2160_24P:
            u32XCOutputTiming = E_OUTPUT_TIMING_3840X2160_24P;
            break;
        case  E_UI_TX_OUTPUTTIMING_3840X2160_25P:
            u32XCOutputTiming = E_OUTPUT_TIMING_3840X2160_25P;
            break;
        case  E_UI_TX_OUTPUTTIMING_3840X2160_30P:
            u32XCOutputTiming = E_OUTPUT_TIMING_3840X2160_30P;
            break;
        case  E_UI_TX_OUTPUTTIMING_3840X2160_50P:
            u32XCOutputTiming = E_OUTPUT_TIMING_3840X2160_50P;
            break;
        case  E_UI_TX_OUTPUTTIMING_3840X2160_60P:
            u32XCOutputTiming = E_OUTPUT_TIMING_3840X2160_60P;
            break;
        case  E_UI_TX_OUTPUTTIMING_4096X2160_24P:
            u32XCOutputTiming = E_OUTPUT_TIMING_4096X2160_24P;
            break;
        case  E_UI_TX_OUTPUTTIMING_4096X2160_25P:
            u32XCOutputTiming = E_OUTPUT_TIMING_4096X2160_25P;
            break;
        case  E_UI_TX_OUTPUTTIMING_4096X2160_30P:
            u32XCOutputTiming = E_OUTPUT_TIMING_4096X2160_30P;
            break;
        case  E_UI_TX_OUTPUTTIMING_4096X2160_50P:
            u32XCOutputTiming = E_OUTPUT_TIMING_4096X2160_50P;
            break;
        case  E_UI_TX_OUTPUTTIMING_4096X2160_60P:
            u32XCOutputTiming = E_OUTPUT_TIMING_4096X2160_60P;
            break;
        default:
            printf("[%s][%d] Set HDMITx OutputTiming Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_XC_SetOutputTiming(&u32XCDevice,&u32XCOutputTiming);
}

extern MS_BOOL Demo_HDMI_AspectRatio_case(MS_U32 *u32Sel)
{
    MS_U32 u32AspectRatio = E_ASPECT_RATIO_NORMAL;

    if((E_UI_TX_ASPECTRATIO_4_3>*u32Sel) || (*u32Sel>E_UI_TX_ASPECTRATIO_16_9))
    {
        printf("[%s][%d] Invalid Aspect Ratio %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_ASPECTRATIO_4_3:
            u32AspectRatio = E_ASPECT_RATIO_4_3;
            break;
        case E_UI_TX_ASPECTRATIO_16_9:
            u32AspectRatio = E_ASPECT_RATIO_16_9;
            break;
        default:
            printf("[%s][%d] Set HDMITx Aspect Ratio Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetAspectRatio(&u32AspectRatio);
}

extern MS_BOOL Demo_HDMI_3DStructure_case(MS_U32 *u32Sel)
{
    MS_U32 u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p23d98Hz_24Hz_FRAME_PACKING;
    MS_U32 u32XCDevice = E_MSAPI_XC_DEVICE0;
    MS_U32 bEnable = FALSE;
    MS_U32 u32enOutMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
    MS_U32 u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_HDMI;
    MS_U32 u32DevIdx = 0;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType = E_CONNECT_MAIN;
    EN_AUDIO_APP_TYPE eAppType = AUDIO_APP_ES_PLAY;

    if((E_UI_TX_3DSTRUCTURE_CLOSE>*u32Sel) || (*u32Sel>E_UI_TX_3DSTRUCTURE_1920x1080p29d97Hz_30Hz_TOP_BOTTOM))
    {
        printf("[%s][%d] Invalid 3D Structure %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    if(Demo_MM_Stop() == FALSE)
    {
        printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32DevIdx = 0;
    if(Demo_Audio_Release(&u32DevIdx) == FALSE)
    {
        printf("[%s][%d] Audio Release idx 0 fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32DevIdx = 1;
    if(Demo_Audio_Release(&u32DevIdx) == FALSE)
    {
        printf("[%s][%d] Audio Release idx 1 fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32DevIdx = 0;
    eAppType = AUDIO_APP_HDMI_RX;
    if(Demo_Audio_Open(&u32DevIdx,&eOutSrcType,&eAppType) == FALSE)
    {
        printf("[%s][%d] Audio Open fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_3DSTRUCTURE_CLOSE:
            if(Demo_XC_Enable3D(&u32XCDevice,&bEnable,&u32enOutMode) == FALSE)
            {
                printf("[%s][%d] Disable 3D fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }

            if(Demo_XC_PlayVideo_3D(&u32XCDevice,&u32XCInputSrc) == FALSE)
            {
                printf("[%s][%d] PlayVideo 3D fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            return TRUE;
        case E_UI_TX_3DSTRUCTURE_1920x1080p23d98Hz_24Hz_FRAME_PACKING:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p23d98Hz_24Hz_FRAME_PACKING;
            break;
        case E_UI_TX_3DSTRUCTURE_1280x720p59d94Hz_60Hz_FRAME_PACKING:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1280x720p59d94Hz_60Hz_FRAME_PACKING;
            break;
        case E_UI_TX_3DSTRUCTURE_1280x720p50Hz_FRAME_PACKING:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1280x720p50Hz_FRAME_PACKING;
            break;
        case E_UI_TX_3DSTRUCTURE_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE;
            break;
         case E_UI_TX_3DSTRUCTURE_1920x1080i50Hz_SIDE_BY_SIDE:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080i50Hz_SIDE_BY_SIDE;
            break;
          case E_UI_TX_3DSTRUCTURE_1920x1080p23d98Hz_24Hz_TOP_BOTTOM:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p23d98Hz_24Hz_TOP_BOTTOM;
            break;
          case E_UI_TX_3DSTRUCTURE_1280x720p59d94Hz_60Hz_TOP_BOTTOM:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1280x720p59d94Hz_60Hz_TOP_BOTTOM;
            break;
          case E_UI_TX_3DSTRUCTURE_1280x720p50Hz_TOP_BOTTOM:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1280x720p50Hz_TOP_BOTTOM;
            break;
          case E_UI_TX_3DSTRUCTURE_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE;
            break;
          case E_UI_TX_3DSTRUCTURE_1280x720p50Hz_SIDE_BY_SIDE:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1280x720p50Hz_SIDE_BY_SIDE;
            break;
          case E_UI_TX_3DSTRUCTURE_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE;
            break;
          case E_UI_TX_3DSTRUCTURE_1920x1080p59d94Hz_60Hz_TOP_BOTTOM:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p59d94Hz_60Hz_TOP_BOTTOM;
            break;
          case E_UI_TX_3DSTRUCTURE_1920x1080p50Hz_TOP_BOTTOM:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p50Hz_TOP_BOTTOM;
            break;
          case E_UI_TX_3DSTRUCTURE_1920x1080p29d97Hz_30Hz_TOP_BOTTOM:
            u32XC3DStructure = E_3D_OUTPUT_TIMING_1920x1080p29d97Hz_30Hz_TOP_BOTTOM;
            break;
        default:
            printf("[%s][%d] Set HDMITx 3D Structure Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_Set3DStructure(&u32XC3DStructure);
}

extern MS_BOOL Demo_HDMI_Audio_case(MS_U32 *u32Sel)
{
    MS_U32 u32AudioType = E_AUDIO_OUTPUT_PCM;

    if((E_UI_TX_AUDIO_PCM>*u32Sel) || (*u32Sel>E_UI_TX_AUDIO_NONPCM_DDPBypass))
    {
        printf("[%s][%d] Invalid Audio Type %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_AUDIO_PCM:
            u32AudioType = E_AUDIO_OUTPUT_PCM;
            break;
        case E_UI_TX_AUDIO_NONPCM:
            u32AudioType = E_AUDIO_OUTPUT_NONPCM;
            break;
        case E_UI_TX_AUDIO_NONPCM_DDPBypass:
            u32AudioType = E_AUDIO_OUTPUT_NONPCM_DDPBypass;
            break;
        default:
            printf("[%s][%d] Set HDMITx Audio Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetAudio(&u32AudioType);
}

extern MS_BOOL Demo_HDMI_Colorimetry_case(MS_U32 *u32Sel)
{
    MS_U32 u32Colorimetry = E_MSAPI_XC_HDMI_COLOR_NODATA;
    MS_U32 u32ExtendColorimetry = E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC601;

    if((E_UI_TX_COLORMETRY_NODATA>*u32Sel) || (*u32Sel>E_UI_TX_COLORMETRY_EXTEND_BT2020RGB))
    {
        printf("[%s][%d] Invalid Colormetry %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_COLORMETRY_NODATA:
            u32Colorimetry = E_MSAPI_XC_HDMI_COLOR_NODATA;
            break;
        case E_UI_TX_COLORMETRY_ITU601:
            u32Colorimetry = E_MSAPI_XC_HDMI_COLOR_ITU601;
            break;
        case E_UI_TX_COLORMETRY_ITU709:
            u32Colorimetry = E_MSAPI_XC_HDMI_COLOR_ITU709;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_XVYCC601:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC601;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_XVYCC709:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC709;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_SYCC601:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_SYCC601;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_ADOBEYCC601:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_ADOBEYCC601;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_ADOBERGB:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_ADOBERGB;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_BT2020CYCC:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_BT2020CYCC;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_BT2020YCC:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_BT2020YCC;
            break;
        case E_UI_TX_COLORMETRY_EXTEND_BT2020RGB:
            u32Colorimetry             = E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID;
            u32ExtendColorimetry    = E_MSAPI_XC_HDMI_EXT_COLOR_BT2020RGB;
            break;
        default:
            printf("[%s][%d] Set HDMITx Colorimetry Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetColorimetry(&u32Colorimetry,&u32ExtendColorimetry);
}

extern MS_BOOL Demo_HDMI_AVMute_case(MS_U32 *u32Sel)
{
    MS_U32 u32AVMuteEnable = FALSE;

     if((E_UI_TX_AVMUTE_ON>*u32Sel) || (*u32Sel>E_UI_TX_AVMUTE_OFF))
    {
        printf("[%s][%d] Invalid AV Mute %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_AVMUTE_ON:
            u32AVMuteEnable = TRUE;
            break;
        case E_UI_TX_AVMUTE_OFF:
            u32AVMuteEnable = FALSE;
            break;
        default:
            printf("[%s][%d] Set HDMITx AV Mute Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetAVMute(&u32AVMuteEnable);
}

extern MS_BOOL Demo_HDMI_QuantizationRang_case(MS_U32 *u32Sel)
{
    MS_U32 u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT;
    MS_U32 u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_RGB444;
    MS_U32 u32InColorFormat   = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
    MS_U32 u32InColorRange    = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT;
    MS_U8 u8Color = 0x00;//Black

   if((E_UI_TX_QUANTIZATIONRANGE_RGB444_LIMIT>*u32Sel) || (*u32Sel>E_UI_TX_QUANTIZATIONRANGE_TEST_BLACK))
    {
        printf("[%s][%d] Invalid Quantization Range %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_QUANTIZATIONRANGE_RGB444_LIMIT:
            u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_RGB444;
            u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT;
            break;
        case E_UI_TX_QUANTIZATIONRANGE_YUV422_LIMIT:
            u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_YUV422;
            u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT;
            break;
        case E_UI_TX_QUANTIZATIONRANGE_YUV444_LIMIT:
            u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
            u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT;
            break;
        case E_UI_TX_QUANTIZATIONRANGE_RGB444_FULL:
            u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_RGB444;
            u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_FULL;
            break;
        case E_UI_TX_QUANTIZATIONRANGE_YUV422_FULL:
            u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_YUV422;
            u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_FULL;
            break;
        case E_UI_TX_QUANTIZATIONRANGE_YUV444_FULL:
            u32OutColorFormat = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
            u32OutColorRange  = E_MSAPI_XC_HDMITX_OUTPUT_QUANT_FULL;
            break;
        case E_UI_TX_QUANTIZATIONRANGE_TEST_PURPLE:
            if(Demo_MM_Stop() == FALSE)
            {
                printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            MApi_XC_WriteByteMask(0x132F4B, 0x64, 0xFF);//Y
            MApi_XC_WriteByteMask(0x132F4A, 0x96, 0xFF);//Cr
            MApi_XC_WriteByteMask(0x132F4C, 0xc8, 0xFF);//Cb
            printf("[%s][%d] Set Disp Window Color to purple\n", __FUNCTION__, __LINE__);
            break;
        case E_UI_TX_QUANTIZATIONRANGE_TEST_BLACK:
            if(Demo_MM_Stop() == FALSE)
            {
                printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            MApi_XC_SetDispWindowColor(u8Color, MAIN_WINDOW);
            printf("[%s][%d] Set Disp Window Color to black\n", __FUNCTION__, __LINE__);
            break;
        default:
            printf("[%s][%d] Set HDMITx Quantization Range Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return Demo_HDMI_SetOutputColorRange(&u32InColorFormat,&u32OutColorFormat,
                                                  &u32InColorRange,&u32OutColorRange);
}

extern MS_BOOL Demo_HDMI_PlayVideo_case(MS_U32 *u32Sel)
{
    MS_U32 u32FileOption = MM_PLAY_FILEOPTION_CLOSE_DS;
    MS_U8  u8FileName[] = "/mnt/sda1/Ref_997_200_44k1_20dB_ddp.trp";
    MS_U32 u32On = TRUE;
    MS_U32 u32DevIdx = 0;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType = E_CONNECT_MAIN;
    EN_AUDIO_APP_TYPE eAppType = AUDIO_APP_ES_PLAY;

    if((E_UI_TX_SHOWMM_VIDEO>*u32Sel) || (*u32Sel>E_UI_TX_SHOWMM_AUDIO_48K_DDP))
    {
        printf("[%s][%d] Invalid Play HDMITx Video %u\n",__FUNCTION__,__LINE__,*u32Sel);
        return FALSE;
    }

    u32DevIdx = 0;
    if(Demo_Audio_Release(&u32DevIdx) == FALSE)
    {
        printf("[%s][%d] Audio Release idx 0 fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    eAppType = AUDIO_APP_ES_PLAY;
    if(Demo_Audio_Open(&u32DevIdx,&eOutSrcType,&eAppType) == FALSE)
    {
        printf("[%s][%d] Audio Open fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_SHOWMM_VIDEO:
            sprintf((char*)u8FileName, "/mnt/sda1/Ref_997_200_32k_20dB_dd.trp");
            break;
        case E_UI_TX_SHOWMM_AUDIO_32K_DD:
            sprintf((char*)u8FileName, "/mnt/sda1/Ref_997_200_32k_20dB_dd.trp");
            break;
        case E_UI_TX_SHOWMM_AUDIO_44K1_DD:
            sprintf((char*)u8FileName, "/mnt/sda1/Ref_997_200_44k1_20dB_dd.trp");
            break;
        case E_UI_TX_SHOWMM_AUDIO_48K_DD:
            sprintf((char*)u8FileName, "/mnt/sda1/Ref_997_200_48k_20dB_dd.trp");
            break;
        case E_UI_TX_SHOWMM_AUDIO_44K1_DDP:
            sprintf((char*)u8FileName, "/mnt/sda1/Ref_997_200_44k1_20dB_ddp.trp");
            break;
        case E_UI_TX_SHOWMM_AUDIO_48K_DDP:
            sprintf((char*)u8FileName, "/mnt/sda1/Ref_997_200_48k_20dB_ddp.trp");
            break;
        default:
            printf("[%s][%d] Play HDMITx Video Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    switch(*u32Sel)
    {
        case E_UI_TX_SHOWMM_VIDEO:
        case E_UI_TX_SHOWMM_AUDIO_32K_DD:
        case E_UI_TX_SHOWMM_AUDIO_44K1_DD:
        case E_UI_TX_SHOWMM_AUDIO_48K_DD:
        case E_UI_TX_SHOWMM_AUDIO_44K1_DDP:
        case E_UI_TX_SHOWMM_AUDIO_48K_DDP:
            if(Demo_MM_Stop() == FALSE)
            {
                printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }

            if(Demo_MM_Play(&u32FileOption, u8FileName) == FALSE)
            {
                printf("[%s][%d] MM play fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }

            if(Demo_MM_Repeat(&u32On) == FALSE)
            {
                printf("[%s][%d] MM Repeat fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            break;
        default:
            printf("[%s][%d] Play HDMITx Video Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
            break;
    }

    return TRUE;
}

extern MS_BOOL Demo_TX_INFO(void)
{
    //set line count
    u16LineCount = 10;

    MApp_ZUI_UTL_PageSwitch(E_OSD_DISPLAY_INFO);
    return TRUE;
}

extern MS_U8* Demo_TX_GetUIInfoString(MS_U16 u16Line, MS_U16 u16Pos)
{
    static MS_U8 u8Description[32] = {0};
    MS_BOOL hdmi_tx_en;
    AUDIO_FS_TYPE fs_type;
    HDMI_TX_OUTPUT_TYPE outType;

    if(u16Pos == 0) //position 0
    {
        switch(u16Line)
        {
            case E_UI_TX_INFO_COLOR_FORMAT_DESCRIPTION:
                return (MS_U8*)("Color Format");
            case E_UI_TX_INFO_COLOR_DEPTH_DESCRIPTION:
                return (MS_U8*)("Color Depth");
            case E_UI_TX_INFO_OUTPUT_MODE_DESCRIPTION:
                return (MS_U8*)("Ouput Mode");
            case E_UI_TX_INFO_OUTPUT_TIMING_DESCRIPTION:
                return (MS_U8*)("Output Timing");
            case E_UI_TX_INFO_ASPECT_RATIO_DESCRIPTION:
                return (MS_U8*)("Aspect Ratio");
            case E_UI_TX_INFO_3D_STRUCTURE_DESCRIPTION:
                return (MS_U8*)("3D Structure");
            case E_UI_TX_INFO_AUDIO_INFO_DESCRIPTION:
                return (MS_U8*)("Audio INFO");
            case E_UI_TX_INFO_COLORIMETRY_DESCRIPTION:
                return (MS_U8*)("Colorimetry");
            case E_UI_TX_INFO_AVMUTE_DESCRIPTION:
                return (MS_U8*)("AV Mute");
            case E_UI_TX_INFO_QUANTIZATION_RANGE_DESCRIPTION:
                return (MS_U8*)("Quantization Range");
            default:
                return (MS_U8*)("Invalid Input");
        }
    }
    else if(u16Pos == 1) //position 1
    {
        switch(u16Line)
        {
            case E_UI_TX_INFO_COLOR_FORMAT_DESCRIPTION:
                if(E_MSAPI_XC_OK !=  msAPI_XC_GetHDMITxOutputInfo(&_stAPI_XC_HDMITxOutputInfo))
                {
                    printf("Get Tx INFO Failed or Not Support!\n");
                    sprintf((char*)u8Description,"Get Tx INFO Failed");
                    return u8Description;
                }
                switch(_stAPI_XC_HDMITxOutputInfo.eColorFormat)
                {
                    case E_MSAPI_XC_HDMITX_OUTPUT_RGB444:
                        sprintf((char*)u8Description,"RGB");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_YUV422:
                        sprintf((char*)u8Description,"YUV422");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_YUV444:
                        sprintf((char*)u8Description,"YUV444");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_YUV420:
                        sprintf((char*)u8Description,"YUV420");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get Color Format Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_COLOR_DEPTH_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.eColorDepth)
                {
                    case E_MSAPI_XC_HDMITX_CD_8BITS:
                        sprintf((char*)u8Description,"8 BITS");
                        break;
                    case E_MSAPI_XC_HDMITX_CD_10BITS:
                        sprintf((char*)u8Description,"10 BITS");
                        break;
                    case E_MSAPI_XC_HDMITX_CD_12BITS:
                        sprintf((char*)u8Description,"12 BITS");
                        break;
                    case E_MSAPI_XC_HDMITX_CD_16BITS:
                        sprintf((char*)u8Description,"16 BITS");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get Color Depth Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_OUTPUT_MODE_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.eOutputMode)
                {
                    case E_MSAPI_XC_HDMITX_OUTPUT_HDMI:
                        sprintf((char*)u8Description,"HDMI");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP:
                        sprintf((char*)u8Description,"HDMI_HDCPI");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_DVI:
                        sprintf((char*)u8Description,"DVI");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP:
                        sprintf((char*)u8Description,"DVI_HDCP");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get Output Mode Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_OUTPUT_TIMING_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.eOutputTiming)
                {
                    case E_MSAPI_XC_RES_720x480I_60Hz:
                        sprintf((char*)u8Description,"720x480I_60Hz");
                        break;
                    case E_MSAPI_XC_RES_720x480P_60Hz:
                        sprintf((char*)u8Description,"720x480P_60Hz");
                        break;
                    case E_MSAPI_XC_RES_720x576I_50Hz:
                        sprintf((char*)u8Description,"720x576I_50Hz");
                        break;
                    case E_MSAPI_XC_RES_720x576P_50Hz:
                        sprintf((char*)u8Description,"720x576P_50Hz");
                        break;
                    case E_MSAPI_XC_RES_1280x720P_50Hz:
                        sprintf((char*)u8Description,"1280x720P_50Hz");
                        break;
                    case E_MSAPI_XC_RES_1280x720P_60Hz:
                        sprintf((char*)u8Description,"1280x720P_60Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080I_50Hz:
                        sprintf((char*)u8Description,"1920x1080I_50Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080I_60Hz:
                        sprintf((char*)u8Description,"1920x1080I_60Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080P_24Hz:
                        sprintf((char*)u8Description,"1920x1080P_24Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080P_25Hz:
                        sprintf((char*)u8Description,"1920x1080P_25Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080P_30Hz:
                        sprintf((char*)u8Description,"1920x1080P_30Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080P_50Hz:
                        sprintf((char*)u8Description,"1920x1080P_50Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x1080P_60Hz:
                        sprintf((char*)u8Description,"1920x1080P_60Hz");
                        break;
                    case E_MSAPI_XC_RES_640x480P_60Hz:
                        sprintf((char*)u8Description,"640x480P_60Hz");
                        break;
                    case E_MSAPI_XC_RES_1920x2205P_3DFP_24Hz:
                        sprintf((char*)u8Description,"1920x2205P_3DFP_24Hz");
                        break;
                    case E_MSAPI_XC_RES_1280x1470P_3DFP_50Hz:
                        sprintf((char*)u8Description,"1280x1470P_3DFP_50Hz");
                        break;
                    case E_MSAPI_XC_RES_1280x1470P_3DFP_60Hz:
                        sprintf((char*)u8Description,"1280x1470P_3DFP_60Hz");
                        break;
                    case E_MSAPI_XC_RES_3840x2160P_24Hz:
                        sprintf((char*)u8Description,"3840x2160P_24Hz");
                        break;
                   case E_MSAPI_XC_RES_3840x2160P_25Hz:
                        sprintf((char*)u8Description,"3840x2160P_25Hz");
                        break;
                    case E_MSAPI_XC_RES_3840x2160P_30Hz:
                        sprintf((char*)u8Description,"3840x2160P_30Hz");
                        break;
                    case E_MSAPI_XC_RES_3840x2160P_50Hz:
                        sprintf((char*)u8Description,"3840x2160P_50Hz");
                        break;
                    case E_MSAPI_XC_RES_3840x2160P_60Hz:
                        sprintf((char*)u8Description,"3840x2160P_60Hz");
                        break;
                    case E_MSAPI_XC_RES_4096x2160P_24Hz:
                        sprintf((char*)u8Description,"4096x2160P_24Hz");
                        break;
                    case E_MSAPI_XC_RES_4096x2160P_25Hz:
                        sprintf((char*)u8Description,"4096x2160P_25Hz");
                        break;
                    case E_MSAPI_XC_RES_4096x2160P_30Hz:
                        sprintf((char*)u8Description,"4096x2160P_30Hz");
                        break;
                    case E_MSAPI_XC_RES_4096x2160P_50Hz:
                        sprintf((char*)u8Description,"4096x2160P_50Hz");
                        break;
                    case E_MSAPI_XC_RES_4096x2160P_60Hz:
                        sprintf((char*)u8Description,"4096x2160P_60Hz");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get Output Timing Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_ASPECT_RATIO_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.eAspectRatio)
                {
                    case E_MSAPI_XC_ASPECT_RATIO_NORMAL:
                        sprintf((char*)u8Description,"NORMAL");
                        break;
                    case E_MSAPI_XC_ASPECT_RATIO_MSTK_LB_4_3:
                    case E_MSAPI_XC_ASPECT_RATIO_MSTK_PS_4_3:
                        sprintf((char*)u8Description,"4:3");
                        break;
                    case E_MSAPI_XC_ASPECT_RATIO_MSTK_PB_16_9:
                    case E_MSAPI_XC_ASPECT_RATIO_MSTK_PS_16_9:
                        sprintf((char*)u8Description,"16:9");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get Aspect Ratio Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_3D_STRUCTURE_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.eXC3DOutputMode)
                {
                    case E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM:
                        sprintf((char*)u8Description,"TOP BOTTOM");
                        break;
                    case E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING:
                        sprintf((char*)u8Description,"FRAME PACKING");
                        break;
                    case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF:
                        sprintf((char*)u8Description,"SIDE BY SIDE HALF");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get 3D Structure Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_AUDIO_INFO_DESCRIPTION:
                    MApi_AUDIO_HDMI_Tx_GetStatus(&hdmi_tx_en, &fs_type, &outType);
                    if (hdmi_tx_en)
                    {
                        if (outType == HDMI_OUT_NONPCM)
                        {
                            switch (fs_type)
                            {
                            case AUDIO_FS_32KHZ:
                                sprintf((char*)u8Description,"Channel:2,Type:NONPCM,Freq:32K");
                                break;
                            case AUDIO_FS_44KHZ:
                                sprintf((char*)u8Description,"Channel:2,Type:NONPCM,Freq:44.1K");
                                break;
                            case AUDIO_FS_176KHZ:
                                sprintf((char*)u8Description,"Channel:2,Type:NONPCM,Freq:176K");
                                break;
                            case AUDIO_FS_192KHZ:
                                sprintf((char*)u8Description,"Channel:2,Type:NONPCM,Freq:192K");
                                break;
                            case AUDIO_FS_48KHZ:
                            default:
                                sprintf((char*)u8Description,"Channel:2,Type:NONPCM,Freq:48K");
                                break;
                            }
                        }
                        else
                        {
                            sprintf((char*)u8Description,"Channel:2,Type:PCM,Freq:48K");
                        }
                    }
                    else
                    {
                        sprintf((char*)u8Description,"Get AUDIO INFO Fail");
                    }
                break;
            case E_UI_TX_INFO_COLORIMETRY_DESCRIPTION:
                if(_stAPI_XC_HDMITxOutputInfo.eColorimetry == E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID)
                {
                    switch(_stAPI_XC_HDMITxOutputInfo.eExtendColorimetry)
                    {
                        case E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC601:
                            sprintf((char*)u8Description,"XVYCC601");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC709:
                            sprintf((char*)u8Description,"XVYCC709");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_SYCC601:
                            sprintf((char*)u8Description,"SYCC601");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_ADOBEYCC601:
                            sprintf((char*)u8Description,"ADOBEYCC601");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_ADOBERGB:
                            sprintf((char*)u8Description,"ADOBERGB");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_BT2020CYCC:
                            sprintf((char*)u8Description,"BT2020CYCC");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_BT2020YCC:
                            sprintf((char*)u8Description,"BT2020YCC");
                            break;
                        case E_MSAPI_XC_HDMI_EXT_COLOR_BT2020RGB:
                            sprintf((char*)u8Description,"BT2020RGB");
                            break;
                        default:
                             sprintf((char*)u8Description,"Get Colorimetry Fail");
                            break;
                    }
                }
                else
                {
                    switch(_stAPI_XC_HDMITxOutputInfo.eColorimetry)
                    {
                        case E_MSAPI_XC_HDMI_COLOR_NODATA:
                            sprintf((char*)u8Description,"NODATA");
                            break;
                        case E_MSAPI_XC_HDMI_COLOR_ITU601:
                            sprintf((char*)u8Description,"ITU601");
                            break;
                        case E_MSAPI_XC_HDMI_COLOR_ITU709:
                            sprintf((char*)u8Description,"ITU709");
                            break;
                        default:
                            sprintf((char*)u8Description,"Get Colorimetry Fail");
                            break;
                    }
                }
                break;
            case E_UI_TX_INFO_AVMUTE_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.bAVMute)
                {
                    case TRUE:
                        sprintf((char*)u8Description,"Enable");
                        break;
                    case FALSE:
                        sprintf((char*)u8Description,"Disable");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get AV Mute Fail");
                        break;
                }
                break;
            case E_UI_TX_INFO_QUANTIZATION_RANGE_DESCRIPTION:
                switch(_stAPI_XC_HDMITxOutputInfo.QuantizationRange)
                {
                    case E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT:
                        sprintf((char*)u8Description,"LIMIT");
                        break;
                    case E_MSAPI_XC_HDMITX_OUTPUT_QUANT_FULL:
                        sprintf((char*)u8Description,"FULL");
                        break;
                    default:
                        sprintf((char*)u8Description,"Get Quantization Range Fail");
                        break;
                }
                break;
            default:
                sprintf((char*)u8Description,"Invalid Input\n");
                break;
        }
    }
    return u8Description;
}


#if (DEMO_XC_HDMIRX_TEST == 1)
MS_U8* _Demo_HDMI_REPEATER_AVIStatus2string(EN_INTERFACE_REPEATER_INFO_ITEM eInfoType)
{
    static MS_U8 u8Text[16] = {0};
    MS_BOOL bRXOffLine = TRUE;
    if((msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bRXOffLine) == E_MSAPI_XC_OK) && bRXOffLine)
    {
        //RX not connected
        sprintf((char*)u8Text,"RX_UNCONNECTED");
        return u8Text;
    }

    if(eInfoType == E_UI_REPEATER_INFO_SCAN_INFO)
    {
        switch (sAVI_PACKET.eScanInfo)
        {
            case E_MSAPI_XC_HDMI_SCAN_INFO_NODATA:
                sprintf((char*)u8Text,"NODATA");
                break;
            case E_MSAPI_XC_HDMI_SCAN_INFO_OVERSCAN:
                sprintf((char*)u8Text,"OVERSCAN");
                break;
            case E_MSAPI_XC_HDMI_SCAN_INFO_UNDERSCAN:
                sprintf((char*)u8Text,"UNDERSCAN");
                break;
            case E_MSAPI_XC_HDMI_SCAN_INFO_FEATURE:
                sprintf((char*)u8Text,"FEATURE");
                break;
            case E_MSAPI_XC_HDMI_SCAN_INFO_UNKNOW:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_COLOR_FORMAT)
    {
        switch (sAVI_PACKET.eColorFormat)
        {
            case E_MSAPI_XC_HDMITX_OUTPUT_RGB444:
                sprintf((char*)u8Text,"RGB444");
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_YUV422:
                sprintf((char*)u8Text,"YUV422");
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_YUV444:
                sprintf((char*)u8Text,"YUV444");
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_YUV420:
                sprintf((char*)u8Text,"YUV420");
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_UNKOWN:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_ASPECT_RATIO)
    {
        switch (sAVI_PACKET.eAspectRatio)
        {
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_Top:
                sprintf((char*)u8Text,"16x9_Top");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_14x9_Top:
                sprintf((char*)u8Text,"14x9_Top");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_GT_16x9:
                sprintf((char*)u8Text,"GT_16x9");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_SAME:
                sprintf((char*)u8Text,"SAME");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_4x3_C:
                sprintf((char*)u8Text,"4x3_C");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_C:
                sprintf((char*)u8Text,"16x9_C");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_14x9_C:
                sprintf((char*)u8Text,"14x9_C");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_4x3_with_14x9_C:
                sprintf((char*)u8Text,"4x3_with_14x9_C");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_with_14x9_C:
                sprintf((char*)u8Text,"16x9_with_14x9_C");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_16x9_with_4x3_C:
                sprintf((char*)u8Text,"16x9_with_4x3_C");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_NODATA:
                sprintf((char*)u8Text,"PIC_NODATA");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_4x3:
                sprintf((char*)u8Text,"PIC_4x3");
                break;
            case E_MSAPI_XC_HDMI_ASPECT_RATIO_PIC_16x9:
                sprintf((char*)u8Text,"PIC_16x9");
                break;
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_COLORMETRY)
    {
        switch (sAVI_PACKET.eColorMetry)
        {
            case E_MSAPI_XC_HDMI_COLOR_NODATA:
                sprintf((char*)u8Text,"NODATA");
                break;
            case E_MSAPI_XC_HDMI_COLOR_ITU601:
                sprintf((char*)u8Text,"ITU601");
                break;
            case E_MSAPI_XC_HDMI_COLOR_ITU709:
                sprintf((char*)u8Text,"ITU709");
                break;
            case E_MSAPI_XC_HDMI_COLOR_EXTEND_VALID:
                sprintf((char*)u8Text,"EXTEND_VALID");
                break;
            case E_MSAPI_XC_HDMI_COLOR_UNKNOWN:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_EXT_COLORMETRY)
    {
        switch (sAVI_PACKET.eExtColorMetry)
        {
            case E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC601:
                sprintf((char*)u8Text,"XVYCC601");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_XVYCC709:
                sprintf((char*)u8Text,"XVYCC709");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_SYCC601:
                sprintf((char*)u8Text,"SYCC601");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_ADOBEYCC601:
                sprintf((char*)u8Text,"ADOBEYCC601");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_ADOBERGB:
                sprintf((char*)u8Text,"ADOBERGB");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_BT2020CYCC:
                sprintf((char*)u8Text,"BT2020CYCC");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_BT2020YCC:
                sprintf((char*)u8Text,"BT2020YCC");
                break;
            case E_MSAPI_XC_HDMI_EXT_COLOR_UNKNOWN:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_COLOR_RANGE)
    {
        switch (sAVI_PACKET.eColorRange)
        {
            case E_MSAPI_XC_HDMITX_OUTPUT_QUANT_LIMIT:
                sprintf((char*)u8Text,"LIMIT");
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_QUANT_FULL:
                sprintf((char*)u8Text,"FULL");
                break;
            case E_MSAPI_XC_HDMITX_OUTPUT_QUANT_DEFAULT:
                sprintf((char*)u8Text,"DEFAULT");
                break;
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_VIC)
    {
        sprintf((char*)u8Text,"%d",sAVI_PACKET.u8VICCode);
    }

    return u8Text;
}

MS_U8* _Demo_HDMI_REPEATER_VSStatus2string(EN_INTERFACE_REPEATER_INFO_ITEM eInfoType)
{
    static MS_U8 u8Text[16] = {0};
    MS_BOOL bRXOffLine = TRUE;
    if((msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bRXOffLine) == E_MSAPI_XC_OK) && bRXOffLine)
    {
        //RX not connected
        sprintf((char*)u8Text,"RX_UNCONNECTED");
        return u8Text;
    }

    if(eInfoType == E_UI_REPEATER_INFO_VF_PRESENT)
    {
        switch (sVS_PACKET.eVideoFormatPresent)
        {
            case E_MSAPI_XC_HDMI_ADDITIONAL_NONE:
                sprintf((char*)u8Text,"NONE");
                break;
            case E_MSAPI_XC_HDMI_ADDITIONAL_4Kx2K_FORMAT:
                sprintf((char*)u8Text,"4Kx2K_FORMAT");
                break;
            case E_MSAPI_XC_HDMI_ADDITIONAL_3D_FORMAT:
                sprintf((char*)u8Text,"3D_FORMAT");
                break;
            case E_MSAPI_XC_HDMI_ADDITIONAL_NA:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_3D_MODE)
    {
        switch (sVS_PACKET.e3DMode)
        {
            case E_MSAPI_XC_3D_INPUT_MODE_NONE:
                sprintf((char*)u8Text,"NONE");
                break;
            case E_MSAPI_XC_3D_INPUT_FRAME_PACKING:
                sprintf((char*)u8Text,"FRAME_PACKING");
                break;
            case E_MSAPI_XC_3D_INPUT_FRAME_ALTERNATIVE:
                sprintf((char*)u8Text,"FRAME_ALTERNATIVE");
                break;
            case E_MSAPI_XC_3D_INPUT_FIELD_ALTERNATIVE:
                sprintf((char*)u8Text,"FIELD_ALTERNATIVE");
                break;
            case E_MSAPI_XC_3D_INPUT_LINE_ALTERNATIVE:
                sprintf((char*)u8Text,"LINE_ALTERNATIVE");
                break;
            case E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_HALF:
                sprintf((char*)u8Text,"SIDE_BY_SIDE_HALF");
                break;
            case E_MSAPI_XC_3D_INPUT_SIDE_BY_SIDE_FULL:
                sprintf((char*)u8Text,"SIDE_BY_SIDE_FULL");
                break;
            case E_MSAPI_XC_3D_INPUT_TOP_BOTTOM:
                sprintf((char*)u8Text,"TOP_BOTTOM");
                break;
            case E_MSAPI_XC_3D_INPUT_CHECK_BORAD:
                sprintf((char*)u8Text,"CHECK_BORAD");
                break;
            case E_MSAPI_XC_3D_INPUT_PIXEL_ALTERNATIVE:
                sprintf((char*)u8Text,"PIXEL_ALTERNATIVE");
                break;
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_4K2K_VIC)
    {
        switch (sVS_PACKET.e4K2KVIC)
        {
            case E_MSAPI_XC_HDMI_VIC_4Kx2K_30Hz:
                sprintf((char*)u8Text,"4Kx2K_30Hz");
                break;
            case E_MSAPI_XC_HDMI_VIC_4Kx2K_25Hz:
                sprintf((char*)u8Text,"4Kx2K_25Hz");
                break;
            case E_MSAPI_XC_HDMI_VIC_4Kx2K_24Hz:
                sprintf((char*)u8Text,"4Kx2K_24Hz");
                break;
            case E_MSAPI_XC_HDMI_VIC_4Kx2K_24Hz_SMPTE:
                sprintf((char*)u8Text,"4Kx2K_24Hz_SMPTE");
                break;
            case E_MSAPI_XC_HDMI_VIC_NA:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }

    return u8Text;
}

MS_U8* _Demo_HDMI_REPEATER_GCStatus2string(EN_INTERFACE_REPEATER_INFO_ITEM eInfoType)
{
    static MS_U8 u8Text[16] = {0};
    MS_BOOL bRXOffLine = TRUE;
    if((msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bRXOffLine) == E_MSAPI_XC_OK) && bRXOffLine)
    {
        //RX not connected
        sprintf((char*)u8Text,"RX_UNCONNECTED");
        return u8Text;
    }

    if(eInfoType == E_UI_REPEATER_INFO_AVMUTE)
    {
        if(sGC_PACKET.bAVMute)
        {
            sprintf((char*)u8Text,"ENABLE");
        }
        else
        {
            sprintf((char*)u8Text,"DISABLE");
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_COLOR_DEPTH)
    {
        switch (sGC_PACKET.eCDType)
        {
            case E_MSAPI_XC_HDMITX_CD_8BITS:
                sprintf((char*)u8Text,"8BITS");
                break;
            case E_MSAPI_XC_HDMITX_CD_10BITS:
                sprintf((char*)u8Text,"10BITS");
                break;
            case E_MSAPI_XC_HDMITX_CD_12BITS:
                sprintf((char*)u8Text,"12BITS");
                break;
            case E_MSAPI_XC_HDMITX_CD_16BITS:
                sprintf((char*)u8Text,"16BITS");
                break;
            case E_MSAPI_XC_HDMITX_CD_AUTO:
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }

    return u8Text;
}

MS_U8* _Demo_HDMI_REPEATER_AUDIOStatus2string(EN_INTERFACE_REPEATER_INFO_ITEM eInfoType)
{
    static MS_U8 u8Text[16] = {0};
    MS_BOOL bRXOffLine = TRUE;
    if((msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bRXOffLine) == E_MSAPI_XC_OK) && bRXOffLine)
    {
        //RX not connected
        sprintf((char*)u8Text,"RX_UNCONNECTED");
        return u8Text;
    }

    if(eInfoType == E_UI_REPEATER_INFO_AUDIO_CHANNEL_COUNT)
    {
        switch (sAUDIO_PACKET.eChannelCount)
        {
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_REFER_TO_HEADER:
                sprintf((char*)u8Text,"REFER_TO_HEADER");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_2CH:
                sprintf((char*)u8Text,"2CH");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_3CH:
                sprintf((char*)u8Text,"3CH");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_4CH:
                sprintf((char*)u8Text,"4CH");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_5CH:
                sprintf((char*)u8Text,"5CH");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_6CH:
                sprintf((char*)u8Text,"6CH");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_7CH:
                sprintf((char*)u8Text,"7CH");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CHANNEL_COUNT_8CH:
                sprintf((char*)u8Text,"8CH");
                break;
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_AUDIO_CODING_TYPE)
    {
        switch (sAUDIO_PACKET.eCodingType)
        {
            case E_MSAPI_XC_HDMI_AUDIO_CODING_TYPE_PCM:
                sprintf((char*)u8Text,"PCM");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_CODING_TYPE_NONPCM:
                sprintf((char*)u8Text,"NONPCM");
                break;
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }
    else if(eInfoType == E_UI_REPEATER_INFO_AUDIO_FREQUENCY)
    {
        switch (sAUDIO_PACKET.eFrequency)
        {
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_REFER_TO_HEADER:
                sprintf((char*)u8Text,"REFER_TO_HEADER");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_32KHZ:
                sprintf((char*)u8Text,"32KHZ");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_44KHZ:
                sprintf((char*)u8Text,"44KHZ");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_48KHZ:
                sprintf((char*)u8Text,"48KHZ");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_88KHZ:
                sprintf((char*)u8Text,"88KHZ");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_96KHZ:
                sprintf((char*)u8Text,"96KHZ");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_176KHZ:
                sprintf((char*)u8Text,"176KHZ");
                break;
            case E_MSAPI_XC_HDMI_AUDIO_SAMPLING_FREQUENCY_192KHZ:
                sprintf((char*)u8Text,"192KHZ");
                break;
            default:
                sprintf((char*)u8Text,"UKNOWN");
                break;
        }
    }

    return u8Text;
}

MS_BOOL _Demo_HDMI_REPEATER_UpdateRXInfo(EN_INTERFACE_REPEATER_INFO_PACEKT_TYPE ePacket)
{
    MS_BOOL bRXOffLine = TRUE;
    if((msAPI_HDMI_GetRxStatus(E_MSAPI_XC_HDMIRX_PORT0, &bRXOffLine) == E_MSAPI_XC_OK) && bRXOffLine)
    {
        //RX not connected
        return TRUE;
    }

    switch(ePacket)
    {
        case E_UI_REPEATER_INFO_AVI_PACKET:
        {
            memset(&sAVI_PACKET, 0, sizeof(MSAPI_XC_HDMIRX_AVI_PACKET));
            msAPI_XC_HDMIRx_GetPacketInfo(E_MSAPI_XC_HDMIRX_AVI, &sAVI_PACKET);
        }
        case E_UI_REPEATER_INFO_VS_PACKET:
        {
            memset(&sVS_PACKET, 0, sizeof(MSAPI_XC_HDMIRX_VS_PACKET));
            msAPI_XC_HDMIRx_GetPacketInfo(E_MSAPI_XC_HDMIRX_VS, &sVS_PACKET);
        }
        case E_UI_REPEATER_INFO_GC_PACKET:
        {
            memset(&sGC_PACKET, 0, sizeof(MSAPI_XC_HDMIRX_GC_PACKET));
            msAPI_XC_HDMIRx_GetPacketInfo(E_MSAPI_XC_HDMIRX_GC, &sGC_PACKET);
        }
        case E_UI_REPEATER_INFO_AUDIO_PACKET:
        {
            memset(&sAUDIO_PACKET, 0, sizeof(MSAPI_XC_HDMIRX_AUDIO_PACKET));
            msAPI_XC_HDMIRx_GetPacketInfo(E_MSAPI_XC_HDMIRX_AUDIO, &sAUDIO_PACKET);
        }
    }

    return TRUE;
}
#endif

MS_BOOL _Demo_HDMI_REPEATER_Reset(EN_HDMI_REPEATER eCurrentMode)
{
#if (DEMO_XC_HDMIRX_TEST == 1)
    MS_U32 u32RXbyPassEnable = FALSE;
#endif

    switch(eRepeaterMode)
    {
        case E_HDMI_REPEATER_TXONLY:
            if(Demo_MM_Stop() == FALSE)
            {
                printf("[%s][%d] MM stop fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            break;
#if (DEMO_XC_HDMIRX_TEST == 1)
        case E_HDMI_REPEATER_BYPASS:
            if(Demo_HDMI_SetRxBypass(&u32RXbyPassEnable) == FALSE)
            {
                printf("[%s][%d] Disable bypass mode fail !\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            break;
        case E_HDMI_REPEATER_CONVERTER:
#endif
        default:
            break;
    }

    eRepeaterMode = eCurrentMode;
    return TRUE;
}

extern MS_BOOL Demo_HDMI_REPEATER_SetTXOnlyMode(void)
{
    MS_U32 u32FileOption = 0;
    MS_U8  u8FileName[] = "/mnt/sda1/SNSD_1080i@30.tp";
    MS_U32 u32On = TRUE;
    MS_U32 u32AudioDevIdx = 0;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType = E_CONNECT_MAIN;
    EN_AUDIO_APP_TYPE eAppType = AUDIO_APP_ES_PLAY;

    _Demo_HDMI_REPEATER_Reset(E_HDMI_REPEATER_TXONLY);

    if(Demo_Audio_Release(&u32AudioDevIdx) == FALSE)
    {
        printf("[%s][%d] Release audio idx 0 fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32AudioDevIdx = 0;
    if(Demo_Audio_Open(&u32AudioDevIdx, &eOutSrcType, &eAppType) == FALSE)
    {
        printf("[%s][%d] Config audio fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_MM_Play(&u32FileOption, u8FileName) == FALSE)
    {
        printf("[%s][%d] MM play fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_MM_Repeat(&u32On) == FALSE)
    {
        printf("[%s][%d] MM play fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return TRUE;
}

extern MS_BOOL Demo_HDMI_REPEATER_SetConverterMode(void)
{
#if (DEMO_XC_HDMIRX_TEST == 1)

    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = 0;
    MS_U32 u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_HDMI;
    MS_U32 u32AudioDevIdx = 0;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType = E_CONNECT_MAIN;
    EN_AUDIO_APP_TYPE eAppType = AUDIO_APP_HDMI_RX;

    _Demo_HDMI_REPEATER_Reset(E_HDMI_REPEATER_CONVERTER);

    if(Demo_Audio_Release(&u32AudioDevIdx) == FALSE)
    {
        printf("[%s][%d] Release audio idx 0 fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32AudioDevIdx = 1;
    if(Demo_Audio_Release(&u32AudioDevIdx) == FALSE)
    {
        printf("[%s][%d] Release audio idx 1 fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u32AudioDevIdx = 0;
    if(Demo_Audio_Open(&u32AudioDevIdx, &eOutSrcType, &eAppType) == FALSE)
    {
        printf("[%s][%d] Config audio fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc) == FALSE)
    {
        printf("[%s][%d] Set Converter Mode fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

#endif
    return TRUE;
}

extern MS_BOOL Demo_HDMI_REPEATER_SetRXBypassMode(void)
{
#if (DEMO_XC_HDMIRX_TEST == 1)

    MS_U32 u32RXbyPassEnable = TRUE;

    _Demo_HDMI_REPEATER_Reset(E_HDMI_REPEATER_BYPASS);

    if(Demo_HDMI_SetRxBypass(&u32RXbyPassEnable) == FALSE)
    {
        printf("[%s][%d] Disable bypass mode fail !\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
#endif
    return TRUE;
}

extern MS_BOOL Demo_HDMI_REPEATER_ShowRXInfo(void)
{
#if (DEMO_XC_HDMIRX_TEST == 1)

    //set line count
    u16LineCount = 15;

    //page switch
    MApp_ZUI_UTL_PageSwitch(E_OSD_DISPLAY_INFO);
#endif
    return TRUE;
}

extern MS_U8* Demo_HDMI_REPEATER_GetRXInfoContext(MS_U16 u16Line, MS_U16 u16Pos)
{
#if (DEMO_XC_HDMIRX_TEST == 1)

    if(u16Pos == 0) //position 0
    {
        switch(u16Line)
        {
            case E_UI_REPEATER_INFO_SCAN_INFO://Get AVI
                _Demo_HDMI_REPEATER_UpdateRXInfo(E_UI_REPEATER_INFO_AVI_PACKET);
                return (MS_U8*)("[AVI]Scan_Info");
            case E_UI_REPEATER_INFO_COLOR_FORMAT:
                return (MS_U8*)("[AVI]ColorFormat");
            case E_UI_REPEATER_INFO_ASPECT_RATIO:
                return (MS_U8*)("[AVI]AspectRatio");
            case E_UI_REPEATER_INFO_COLORMETRY:
                return (MS_U8*)("[AVI]ColorMetry");
            case E_UI_REPEATER_INFO_EXT_COLORMETRY:
                return (MS_U8*)("[AVI]ExtColorMetry");
            case E_UI_REPEATER_INFO_COLOR_RANGE:
                return (MS_U8*)("[AVI]ColorRange");
            case E_UI_REPEATER_INFO_VIC:
                return (MS_U8*)("[AVI]VICCode");
            case E_UI_REPEATER_INFO_VF_PRESENT://Get VS
                _Demo_HDMI_REPEATER_UpdateRXInfo(E_UI_REPEATER_INFO_VS_PACKET);
                return (MS_U8*)("[VS]VideoFormatPresent");
            case E_UI_REPEATER_INFO_3D_MODE:
                return (MS_U8*)("[VS]3DMode");
            case E_UI_REPEATER_INFO_4K2K_VIC:
                return (MS_U8*)("[VS]4K2KVIC");
            case E_UI_REPEATER_INFO_COLOR_DEPTH://Get GC
                _Demo_HDMI_REPEATER_UpdateRXInfo(E_UI_REPEATER_INFO_GC_PACKET);
                return (MS_U8*)("[GC]ColorDepth");
            case E_UI_REPEATER_INFO_AVMUTE:
                return (MS_U8*)("[GC]AVmute");
            case E_UI_REPEATER_INFO_AUDIO_CHANNEL_COUNT://Get AUDIO
                _Demo_HDMI_REPEATER_UpdateRXInfo(E_UI_REPEATER_INFO_AUDIO_PACKET);
                return (MS_U8*)("[AUDIO]ChannelCount");
            case E_UI_REPEATER_INFO_AUDIO_CODING_TYPE:
                return (MS_U8*)("[AUDIO]CodingType");
            case E_UI_REPEATER_INFO_AUDIO_FREQUENCY:
                return (MS_U8*)("[AUDIO]Frequency");
            default:
                return (MS_U8*)("Invalid Input");
        }
    }
    else if(u16Pos == 1) //position 1
    {
        switch(u16Line)
        {
            case E_UI_REPEATER_INFO_SCAN_INFO://Get AVI
            case E_UI_REPEATER_INFO_COLOR_FORMAT:
            case E_UI_REPEATER_INFO_ASPECT_RATIO:
            case E_UI_REPEATER_INFO_COLORMETRY:
            case E_UI_REPEATER_INFO_EXT_COLORMETRY:
            case E_UI_REPEATER_INFO_COLOR_RANGE:
            case E_UI_REPEATER_INFO_VIC:
            {
                return _Demo_HDMI_REPEATER_AVIStatus2string((EN_INTERFACE_REPEATER_INFO_ITEM)u16Line);
            }
            case E_UI_REPEATER_INFO_VF_PRESENT://Get VS
            case E_UI_REPEATER_INFO_3D_MODE:
            case E_UI_REPEATER_INFO_4K2K_VIC:
            {
                return _Demo_HDMI_REPEATER_VSStatus2string((EN_INTERFACE_REPEATER_INFO_ITEM)u16Line);
            }
            case E_UI_REPEATER_INFO_COLOR_DEPTH://Get GC
            case E_UI_REPEATER_INFO_AVMUTE:
            {
                return _Demo_HDMI_REPEATER_GCStatus2string((EN_INTERFACE_REPEATER_INFO_ITEM)u16Line);
            }
            case E_UI_REPEATER_INFO_AUDIO_CHANNEL_COUNT://Get AUDIO
            case E_UI_REPEATER_INFO_AUDIO_CODING_TYPE:
            case E_UI_REPEATER_INFO_AUDIO_FREQUENCY:
            {
                return _Demo_HDMI_REPEATER_AUDIOStatus2string((EN_INTERFACE_REPEATER_INFO_ITEM)u16Line);
            }
            default:
                return (MS_U8*)("Invalid Input");
        }
    }
#endif
    return NULL;
}



extern MS_BOOL Demo_HDCP_Enable(MS_U32 *pu32Sel)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32HDCPStartTime = MsOS_GetSystemTime();
    MS_U16 u16HDCPTxWaitTime = 5000;
    E_MSAPI_XC_HDMITX_HDCP_STATUS_INFO eHDCPStatus = E_MSAPI_XC_HDMITX_HDCP_STATUS_DISABLE;
    E_MSAPI_XC_HDMITX_HDCP_INFO eHDCPInfo = E_MSAPI_XC_HDMITX_HDCP_GET_STATUS;
    MS_U32 u32HDMIMode = (MS_U32)E_MSAPI_XC_HDMITX_OUTPUT_HDMI; // HDMI mode
    if (*pu32Sel)
    {
        printf("[%s][%d] Enable HDCP !\n",__FUNCTION__,__LINE__);
        u32HDMIMode = (MS_U32)E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP; //HDMI_HDCP mode
        bRet = Demo_HDMI_SetTxOutputMode(&u32HDMIMode);
        // Wait HDCP Tx Done
        while(eHDCPStatus != E_MSAPI_XC_HDMITX_HDCP_STATUS_SUCCESS)
        {
            msAPI_XC_GetHDMITxHDCPStatus(eHDCPInfo, (void*)&eHDCPStatus);
            if (MsOS_Timer_DiffTimeFromNow(u32HDCPStartTime) > u16HDCPTxWaitTime)
            {
                printf("[%s][%d] HDCP Authentication Failed!\n",__FUNCTION__,__LINE__);
                u32HDMIMode = (MS_U32)E_MSAPI_XC_HDMITX_OUTPUT_HDMI; // HDMI mode
                Demo_HDMI_SetTxOutputMode(&u32HDMIMode);
                return FALSE;
            }
            MsOS_DelayTask(50);
        }
    }
    else
    {
        printf("[%s][%d] Disable HDCP !\n",__FUNCTION__,__LINE__);
        bRet = Demo_HDMI_SetTxOutputMode(&u32HDMIMode);
    }
    return bRet;
}

extern MS_BOOL Demo_HDCP_INFO(void)
{
    //set line count
#if (DEMO_XC_HDMIRX_TEST == 1)
    u16LineCount = 3;
#else
    u16LineCount = 2;
#endif

    MApp_ZUI_UTL_PageSwitch(E_OSD_DISPLAY_INFO);
    return TRUE;
}

extern MS_BOOL Demo_HDCP_GetINFO(void)
{
    E_MSAPI_XC_HDMITX_HDCP_INFO eHDCPInfo = E_MSAPI_XC_HDMITX_HDCP_GET_VERSION;
#if (DEMO_XC_HDMIRX_TEST == 1)
    _stHDCPENCState.enInputPortType = INPUT_PORT_DVI0;
#endif

    // Get HDCP Version
    if (msAPI_XC_GetHDMITxHDCPStatus(eHDCPInfo, (void*)&_eHDCPTxVersionInfo) == TRUE)
    {
        // Get HDCP Status
        eHDCPInfo = E_MSAPI_XC_HDMITX_HDCP_GET_STATUS;
        if (msAPI_XC_GetHDMITxHDCPStatus(eHDCPInfo, (void*)&_eHDCPTxStatus) == TRUE)
        {
            printf("[%s][%d] HDCP Version = %d \n", __FUNCTION__, __LINE__, _eHDCPTxVersionInfo);
            printf("[%s][%d] HDCP Status = %d \n", __FUNCTION__, __LINE__, _eHDCPTxStatus);
        }
        else
        {
            printf("[%s][%d] Get HDCP Status Failed \n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        printf("[%s][%d] Get HDCP Version Failed \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //HDCP RX
#if (DEMO_XC_HDMIRX_TEST == 1)
    MDrv_HDMI_Ctrl(E_HDMI_INTERFACE_CMD_GET_HDCP_ENC_STATE, &_stHDCPENCState, sizeof(stCMD_HDMI_CHECK_HDCP_ENC_STATE));

    if (_stHDCPENCState.ucHDCPENCState == E_HDCP_2_2_ENCRYPTION)
    {
        printf("[%s][%d] HDCP Version is 22 \n", __FUNCTION__, __LINE__);
    }
    else if (_stHDCPENCState.ucHDCPENCState == E_HDCP_1_4_ENCRYPTION)
    {
        printf("[%s][%d] HDCP Version is 14 \n", __FUNCTION__, __LINE__);
    }
    else //(eHDCPState == E_HDCP_NO_ENCRYPTION)
    {
        printf("[%s][%d] No HDCP \n", __FUNCTION__, __LINE__);
    }
#endif
    printf("[HDCP] Sys Time = %"DTC_MS_U32_d" \n",MsOS_GetSystemTime());

    return TRUE;
}

extern MS_U8* Demo_HDCP_GetUIInfoString(MS_U16 u16Line, MS_U16 u16Pos)
{
    //Update Message
    if((u16Line == E_UI_CEC_INFO_MESSAGE1_DESCRIPTION) && (u16Pos == 0))
    {
        Demo_HDCP_GetINFO();
    }

    if(u16Pos == 0) //position 0
    {
        switch(u16Line)
        {
            case E_UI_HDCP_INFO_TX_HDCPVERSION:
            {
                return (MS_U8*)("[HDCPTx] Version:");
            }
            case E_UI_HDCP_INFO_TX_HDCPSTATUS:
            {
                return (MS_U8*)("[HDCPTx] Status: ");
            }
            case E_UI_HDCP_INFO_RX_HDCPSTATUS:
            {
                return (MS_U8*)("[HDCPRx] Status: ");
            }
            default:
            {
                return (MS_U8*)("Invalid Parameter");
            }
        }
    }
    else if(u16Pos == 1) //position 1
    {
        switch(u16Line)
        {
            case E_UI_HDCP_INFO_TX_HDCPVERSION:
            {
                switch(_eHDCPTxVersionInfo)
                {
                    case E_MSAPI_XC_HDMITX_HDCP_VERSION_14:
                    {
                        return (MS_U8*)("HDCP14");
                    }
                    case E_MSAPI_XC_HDMITX_HDCP_VERSION_22:
                    {
                        return (MS_U8*)("HDCP22");
                    }
                    default:
                    {
                        return (MS_U8*)("Unknown HDCP Version");
                    }
                }
            }
            case E_UI_HDCP_INFO_TX_HDCPSTATUS:
            {
                switch(_eHDCPTxStatus)
                {
                    case E_MSAPI_XC_HDMITX_HDCP_STATUS_DISABLE:
                    {
                        return (MS_U8*)("Disable");
                    }
                    case E_MSAPI_XC_HDMITX_HDCP_STATUS_FAIL:
                    {
                        return (MS_U8*)("Fail");
                    }
                    case E_MSAPI_XC_HDMITX_HDCP_STATUS_SUCCESS:
                    {
                        return (MS_U8*)("Pass");
                    }
                    case E_MSAPI_XC_HDMITX_HDCP_STATUS_PROCESS:
                    {
                        return (MS_U8*)("Processing");
                    }
                    default:
                    {
                        return (MS_U8*)("Unknown Status");
                    }
                }
            }
#if (DEMO_XC_HDMIRX_TEST == 1)
            case E_UI_HDCP_INFO_RX_HDCPSTATUS:
            {
                switch(_stHDCPENCState.ucHDCPENCState)
                {
                    case E_HDCP_2_2_ENCRYPTION:
                    {
                        return (MS_U8*)("HDCP22");
                    }
                    case E_HDCP_1_4_ENCRYPTION:
                    {
                        return (MS_U8*)("HDCP14");
                    }
                    case E_HDCP_NO_ENCRYPTION:
                    {
                        return (MS_U8*)("No HDCP");
                    }
                    default:
                    {
                        return (MS_U8*)("Invalid Parameter");
                    }
                }
            }
#endif
            default:
            {
                return (MS_U8*)("Invalid Parameter");
            }
        }
    }

    return NULL;
}

extern MS_BOOL Demo_CEC_INFO(void)
{
    //set line count
    u16LineCount = 20;

    MApp_ZUI_UTL_PageSwitch(E_OSD_DISPLAY_INFO);
    return TRUE;
}

extern MS_BOOL Demo_CEC_GetReceivedMessage(void)
{
    if(msAPI_CEC_GetMessageBuffer(g_stCECMessageBuffer, &u8LatestIdx) == FALSE)
    {
        printf("[CEC] Get CEC message buffer fail \n");
        return FALSE;
    }
    return TRUE;
}

extern MS_U8* Demo_CEC_GetUIInfoString(MS_U16 u16Line, MS_U16 u16Pos)
{
    MS_U8 u8MessageIndex = ((MS_U8)u16Line) / 2;
    MS_U8 u8BufIdx = 0;

    // Row1 Column1 Time=1234567890
    static MS_U8 u8TimeDescription[16] = {0};
    memset(u8TimeDescription, 0, 16);

    // Row1 Column2 Header=0xXX,Opcode=0xXX,Len=OO
    static MS_U8 u8MessageDescription[31] = {0};
    memset(u8MessageDescription, 0, 31);

    //Row2 Column2 Operand=000102...14
    static MS_U8 u8Operand[29] = {0};
    memset(u8Operand, 0, 29);
    MS_U8 u8TempOperand[3];

    MS_U8 i = 0;

    if(g_stCECMessageBuffer == NULL)
    {
        printf("[%s][%d] Should init CEC first !\n",__FUNCTION__,__LINE__);
        return (MS_U8*)("Get CEC INFO Failed");
    }

    //Update Message
    if((u16Line == E_UI_CEC_INFO_MESSAGE1_DESCRIPTION) && (u16Pos == 0))
    {
        Demo_CEC_GetReceivedMessage();
    }

    u8BufIdx = (u8LatestIdx + CEC_MESSAGE_BUFFER_SIZE - u8MessageIndex) % CEC_MESSAGE_BUFFER_SIZE;

    if(u16Pos == 0) //position 0
    {
        switch(u16Line)
        {
            case E_UI_CEC_INFO_MESSAGE1_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE2_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE3_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE4_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE5_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE6_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE7_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE8_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE9_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE10_DESCRIPTION:
            {
                sprintf((char*)u8TimeDescription,"Time=%"DTC_MS_U32_d"",g_stCECMessageBuffer[u8BufIdx].u32RecvTime);
                return u8TimeDescription;
            }
            case E_UI_CEC_INFO_MESSAGE1_OPERAND:
            case E_UI_CEC_INFO_MESSAGE2_OPERAND:
            case E_UI_CEC_INFO_MESSAGE3_OPERAND:
            case E_UI_CEC_INFO_MESSAGE4_OPERAND:
            case E_UI_CEC_INFO_MESSAGE5_OPERAND:
            case E_UI_CEC_INFO_MESSAGE6_OPERAND:
            case E_UI_CEC_INFO_MESSAGE7_OPERAND:
            case E_UI_CEC_INFO_MESSAGE8_OPERAND:
            case E_UI_CEC_INFO_MESSAGE9_OPERAND:
            case E_UI_CEC_INFO_MESSAGE10_OPERAND:
            {
                return (MS_U8*)("Operand");
            }
            default:
            {
                return (MS_U8*)("Invalid Input");
            }
        }
    }
    else if(u16Pos == 1) //position 1
    {
        switch(u16Line)
        {
            case E_UI_CEC_INFO_MESSAGE1_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE2_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE3_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE4_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE5_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE6_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE7_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE8_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE9_DESCRIPTION:
            case E_UI_CEC_INFO_MESSAGE10_DESCRIPTION:
            {
                sprintf((char*)u8MessageDescription,"Header=0x%02X,Opcode=0x%02X,Len=%02d",g_stCECMessageBuffer[u8BufIdx].u8Header,g_stCECMessageBuffer[u8BufIdx].u8Opcode,g_stCECMessageBuffer[u8BufIdx].u8Length);
                return u8MessageDescription;
            }
            case E_UI_CEC_INFO_MESSAGE1_OPERAND:
            case E_UI_CEC_INFO_MESSAGE2_OPERAND:
            case E_UI_CEC_INFO_MESSAGE3_OPERAND:
            case E_UI_CEC_INFO_MESSAGE4_OPERAND:
            case E_UI_CEC_INFO_MESSAGE5_OPERAND:
            case E_UI_CEC_INFO_MESSAGE6_OPERAND:
            case E_UI_CEC_INFO_MESSAGE7_OPERAND:
            case E_UI_CEC_INFO_MESSAGE8_OPERAND:
            case E_UI_CEC_INFO_MESSAGE9_OPERAND:
            case E_UI_CEC_INFO_MESSAGE10_OPERAND:
            {
                if (g_stCECMessageBuffer[u8BufIdx].u8Length > 2)
                {
                    for (i = 0; i < g_stCECMessageBuffer[u8BufIdx].u8Length - 2; i++)
                    {
                        sprintf((char*)u8TempOperand,"%02X",g_stCECMessageBuffer[u8BufIdx].u8Operand[i]);
                        strcat((char*)u8Operand, (char*)u8TempOperand);
                    }
                }
                return u8Operand;
            }
            default:
            {
                return (MS_U8*)("Invalid Input");
            }
        }
    }

    return NULL;
}

extern MS_BOOL Demo_CEC_SendOneTouchPlay(void)
{
    return Demo_CEC_OneTouchPlay();
}

extern MS_BOOL Demo_CEC_SendStandby(void)
{
    MS_U32 u32DeviceLA = 0;//TV
    return Demo_CEC_Standby(&u32DeviceLA);
}

extern MS_BOOL Demo_CEC_SendGiveDevicePowerStatus(void)
{
    MS_U32 u32DeviceLA = 0;//TV
    return Demo_CEC_GiveDevicePowerStatus(&u32DeviceLA);
}

extern MS_BOOL Demo_CEC_SendSystemAudioModeControl(MS_U32 *pu32Sel)
{
    return Demo_CEC_SystemAudioModeRequest(pu32Sel);
}

extern MS_BOOL Demo_CEC_SendUserCommand(void)
{
    MS_U8 u8index = 0;
    MS_U8 u8Operand[14] = {0};
    printf("[%s][%d] Destination LA = 0x%2X \n", __FUNCTION__, __LINE__, u32_UICECDSTLA);
    printf("[%s][%d] Opcode = 0x%2X \n", __FUNCTION__, __LINE__, u32_UICECDSTOpcode);
    printf("[%s][%d] Operand : ", __FUNCTION__, __LINE__);
    for(u8index = 0; u8index < u32_UICECLength; ++u8index)
    {
        printf("0x%2X ",(MS_U8)u32_UICECOperand[u8index]);
        u8Operand[u8index] = (MS_U8)u32_UICECOperand[u8index];
    }
    printf(" \n");

    return MApi_CEC_TxSendMsg((MS_U8)u32_UICECDSTLA, (MS_U8)u32_UICECDSTOpcode, u8Operand, (MS_U8)u32_UICECLength);
}

extern MS_BOOL Demo_CEC_ClearTxSetting(void)
{
    u32_UICECDSTLA = 0;
    u32_UICECDSTOpcode = 0;
    u32_UICECLength = 0;
    memset(u32_UICECOperand, 0, 14);
    return TRUE;
}

MS_U8* _Demo_EDID_AudioFormat2String(MS_U8 u8NumOfAudioData)
{
    switch(g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].eAudioCodingType )
    {
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_LPCM:    //L-PCM
            return (MS_U8*)("LPCM");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_AC3:
            return (MS_U8*)("AC-3");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG1:
            return (MS_U8*)("MPEG1");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MP3:
            return (MS_U8*)("MP3");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG2:
            return (MS_U8*)("MPEG2");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_AACLC:
            return (MS_U8*)("AAC LC");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_DTS:
            return (MS_U8*)("DTS");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_ATRAC:
            return (MS_U8*)("ATRAC");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_OneBitAudio:
            return (MS_U8*)("One Bit Audio");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_EnhancedAC3:
            return (MS_U8*)("Enhanced AC3");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_DTSHD:
            return (MS_U8*)("DTSHD");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MAT:
            return (MS_U8*)("MAT");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_DST:
            return (MS_U8*)("DST");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_WMAPRO:
            return (MS_U8*)("WMAPRO");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4HE_AAC:
            return (MS_U8*)("MPEG4HE_AAC");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4HE_AACv2:
            return (MS_U8*)("MPEG4HE_AACv2");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4AAC_LC:
            return (MS_U8*)("MPEG4AAC LC");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_DRA:
            return (MS_U8*)("DRA");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4HE_AAC_MPEG_SURROUND:
            return (MS_U8*)("AAC MPEG SURROUND");
        case E_MSAPI_HDMI_AUDIO_CODING_TYPE_MPEG4_AACLC_MPEG_SURROUND:
            return (MS_U8*)("AACLC MPEG SURROUND");
        default:
            return NULL;
    }
}

MS_U8* _Demo_EDID_Blcok1_DTD(MS_U8 u8NumOfDTD, MS_U8 u8Item)
{
    static MS_U8 u8Text[34] = {0};
    memset(u8Text, 0 , 34);
    if (u8NumOfDTD >= g_stEDID_Block1.u8NumofDTD)
    {
        return (MS_U8*)("NO INFO");
    }
    if (u8Item == 0)
    {
        sprintf((char*)u8Text, "Horz Active: %d / Vert Active: %d", g_stEDID_Block1.stDTD[u8NumOfDTD].u16HorzActive, g_stEDID_Block1.stDTD[u8NumOfDTD].u16VertActive);
    }
    else if (u8Item == 1)
    {
        sprintf((char*)u8Text, "%s", (g_stEDID_Block1.stDTD[u8NumOfDTD].bInterlace != 0)? "Interlace":"Progressive");
    }
    return u8Text;
}

MS_BOOL _Demo_EDID_Check_Exteand_Code(EN_INTERFACE_EDID_BLOCK1_INFO_ITEM eExtendCode, MS_U8* pu8NumOfExtendTag)
{
    MS_U8 i;
    for (i = 0; i < g_stEDID_Block1.u8NumofExtendCode; i= i + 1)
    {
        switch (eExtendCode)
        {
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY2:
                if (g_stEDID_Block1.stExteandCode[i].u8Tag == 0x00)
                {
                    * pu8NumOfExtendTag = i;
                    return TRUE;
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY1:
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY2:
                if (g_stEDID_Block1.stExteandCode[i].u8Tag == 0x05)
                {
                    * pu8NumOfExtendTag = i;
                    return TRUE;
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF:
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR:
                if (g_stEDID_Block1.stExteandCode[i].u8Tag == 0x06)
                {
                    * pu8NumOfExtendTag = i;
                    return TRUE;
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1:
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2:
                if (g_stEDID_Block1.stExteandCode[i].u8Tag == 0x0E)
                {
                    * pu8NumOfExtendTag = i;
                    return TRUE;
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1:
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2:
                if (g_stEDID_Block1.stExteandCode[i].u8Tag == 0x0F)
                {
                    * pu8NumOfExtendTag = i;
                    return TRUE;
                }
                break;
            default:
                break;
        }
    }
    return FALSE;
}

MS_U8* _Demo_EDID_Get_VIC(MS_U16 u16VideoData)
{
    static MS_U8 u8Text[34] = {0};
    MS_U8 u8Vic[6] = {0};
    MS_U8 i;
    memset(u8Text, 0 , 34);
    memset(u8Vic, 0 , 6);
    if (u16VideoData == E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1)
    {
        for (i = 0 ; i < g_stEDID_Block1.u8VideoDataLen; i = i + 1)
        {
            if (i > 10)  //one line for 11 vic
            {
                break;
            }
            if (i == 0)
            {
                sprintf((char*)u8Text, "%u", g_stEDID_Block1.u8SVD[0]);
            }
            else
            {
                sprintf((char*)u8Vic, ",%u", g_stEDID_Block1.u8SVD[i]);
                strcat((char*)u8Text, (char*)u8Vic);
            }
        }
    }
    else if (u16VideoData == E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2)
    {
        for (i = 11 ; i < g_stEDID_Block1.u8VideoDataLen; i = i + 1)
        {
            if (i > 21) //one line for 10 vic
            {
                break;
            }
            if (i == 0)
            {
                sprintf((char*)u8Text, "%u", g_stEDID_Block1.u8SVD[0]);
            }
            else
            {
                sprintf((char*)u8Vic, ",%u", g_stEDID_Block1.u8SVD[i]);
                strcat((char*)u8Text, (char*)u8Vic);
            }
        }
    }
    else if (u16VideoData == E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3)
    {
        for (i = 22; i < g_stEDID_Block1.u8VideoDataLen; i = i + 1)
        {
            if (i > 32)  //one line for 11 vic
            {
                break;
            }
            if (i == 0)
            {
                sprintf((char*)u8Text, "%u", g_stEDID_Block1.u8SVD[0]);
            }
            else
            {
                sprintf((char*)u8Vic, ",%u", g_stEDID_Block1.u8SVD[i]);
                strcat((char*)u8Text, (char*)u8Vic);
            }
        }
    }
    else
    {
        return NULL;
    }
    return u8Text;
}

MS_U8* _Demo_EDID_Get_Audio_Format(MS_U8 u8NumOfAudioData, MS_U8 u8Line)
{
    static MS_U8 u8Text[32] = {0};
    memset(u8Text, 0 , 32);
    if(u8Line == 1)     //First line
    {
        sprintf((char*)u8Text, "%s", _Demo_EDID_AudioFormat2String(u8NumOfAudioData));
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport192kHz)
        {
            strcat((char*)u8Text, " ,192");
        }
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport176d4kHz)
        {
            strcat((char*)u8Text, " ,176.4");
        }
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport96kHz)
        {
            strcat((char*)u8Text, " ,96");
        }
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport88d2kHz)
        {
            strcat((char*)u8Text, " ,88.2");
        }
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport48kHz)
        {
            strcat((char*)u8Text, " ,48");
        }
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport44d1kHz)
        {
            strcat((char*)u8Text, " ,44");
        }
        if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupport32kHz)
        {
            strcat((char*)u8Text, " ,32");
        }
        strcat((char*)u8Text, " (kHz)");
    }
    else       //Seconed line
    {
        if ((g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].eAudioCodingType >= E_MSAPI_HDMI_AUDIO_CODING_TYPE_AC3) && (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].eAudioCodingType <= E_MSAPI_HDMI_AUDIO_CODING_TYPE_ATRAC))
        {
            sprintf((char*)u8Text, "Max bit rate: %d kHz", g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].u16MaxBitRate);
        }
        else if (g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].eAudioCodingType == E_MSAPI_HDMI_AUDIO_CODING_TYPE_LPCM)
        {
            sprintf((char*)u8Text, "Sampling Size: 24:%s, 20:%s, 16:%s", ((g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupportLPCM24bit != 0)? "Y":"N"), \
                ((g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupportLPCM24bit != 0)? "Y":"N"), \
                ((g_stEDID_Block1.u8AudioDataBlock[u8NumOfAudioData].bSupportLPCM24bit != 0)? "Y":"N"));
        }
        else
        {
            sprintf((char*)u8Text, "NO INFO");
        }
    }
    return u8Text;
}

MS_U8* _Demo_EDID_GetItemString(MS_U16 u16InfoItem)
{
    if (E_UI_EDID_INFO_BLOCK1_MAX < u16InfoItem)
    {
        return NULL;
    }
    if (g_stEDID_Block1.bCEAExtension != TRUE)
    {
        return NULL;
    }
    switch(u16InfoItem)
    {
        case E_UI_EDID_INFO_BLOCK1_CEA_EXTENSION_VERSION:
            return (MS_U8*)("CEA Extension Version");
        case E_UI_EDID_INFO_BLOCK1_SUPPORTS_UNDERSCAN:
            return (MS_U8*)("Supports underscan");
        case E_UI_EDID_INFO_BLOCK1_SUPPORTS_BASIC_AUDIO:
            return (MS_U8*)("Supports basic audio");
        case E_UI_EDID_INFO_BLOCK1_SUPPORTS_YCBCR:
            return (MS_U8*)("Supports YCbCr");
        case E_UI_EDID_INFO_BLOCK1_NATIVE_DTDS_IN_EDID:
            return (MS_U8*)("Native DTDs in EDID");
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_FIRST_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_FIRST_LINE:
            return (MS_U8*)("Audio code/Sampling Freq");
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_SECOND_LINE:
        case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_SECOND_LINE:
            return (MS_U8*)("MaxBitRate/Sampling Size");
        case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1:
            return (MS_U8*)(" Video Data (SVD)");
        case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2:
            return NULL;
        case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3:
            return NULL;
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IEEE_ID:
            return (MS_U8*)("IEEE Registration ID");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_CEC_PA:
            return (MS_U8*)("CEC Physical Address");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DEEP_COLOR:
            return (MS_U8*)("Deep Color");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_MAX_TMDS_CLOCK:
            return (MS_U8*)("Max TMDS clock");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_LATENCY:
            return  (MS_U8*)("Latency");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_BASIC_3D:
            return (MS_U8*)("Basic 3D");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IMAGE_SIZE:
            return (MS_U8*)("Image Size");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_4KX2K_SUPPORT:
            return (MS_U8*)("4K x 2K Support");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT1:
            return (MS_U8*)("General 3D Support");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT2:
            return NULL;
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DETAILED_3D:
            return (MS_U8*)("Detailed 3D Declarations");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_IEEE_ID_AND_VERSION:
            return (MS_U8*)("IEEE ID and VERSION");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_MAX_TMDS_CHARACTER_RATE:
            return (MS_U8*)("Max_TMDS_Character_Rate");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_3D_OSD_DISPARITY_AND_LTE:
            return (MS_U8*)("3D OSD Disparity / LTE");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_VIEW:
            return (MS_U8*)("View");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_RR_AND_SCDC:
            return (MS_U8*)("RR Capable/SCDC Present");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_DC420:
            return (MS_U8*)("Support DC 420");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1:
            return (MS_U8*)("Over/Under Scanned");
        case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY2:
            return (MS_U8*)("Quantization");
        case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF:
            return (MS_U8*)("Supported EOTF");
        case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR:
            return (MS_U8*)("Supported Static Metadata Descriptor");
        case E_UI_EDID_INFO_BLOCK1_COLORIMETRY1:
            return (MS_U8*)("Colorimetry(Bt.20/Adobe)");
        case E_UI_EDID_INFO_BLOCK1_COLORIMETRY2:
            return NULL;
        case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1:
            return (MS_U8*)("Y420 Video Data");
        case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2:
            return NULL;
        case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1:
            return (MS_U8*)("supporting Y420 sampling");
        case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2:
            return NULL;
        case E_UI_EDID_INFO_BLOCK1_DTD0_HORZ_AND_VERT_ACTIVE:
            return (MS_U8*)("DTD0: Horz /Vert Active");
        case E_UI_EDID_INFO_BLOCK1_DTD0_SCAN_TYPE:
            return (MS_U8*)("DTD0: Scan type");
        case E_UI_EDID_INFO_BLOCK1_DTD1_HORZ_AND_VERT_ACTIVE:
            return (MS_U8*)("DTD1: Horz /Vert Active");
        case E_UI_EDID_INFO_BLOCK1_DTD1_SCAN_TYPE:
            return (MS_U8*)("DTD1: Scan type");
        case E_UI_EDID_INFO_BLOCK1_DTD2_HORZ_AND_VERT_ACTIVE:
            return (MS_U8*)("DTD2: Horz /Vert Active");
        case E_UI_EDID_INFO_BLOCK1_DTD2_SCAN_TYPE:
            return (MS_U8*)("DTD2: Scan type");
        case E_UI_EDID_INFO_BLOCK1_DTD3_HORZ_AND_VERT_ACTIVE:
            return (MS_U8*)("DTD3: Horz /Vert Active");
        case E_UI_EDID_INFO_BLOCK1_DTD3_SCAN_TYPE:
            return (MS_U8*)("DTD3: Scan type");
        case E_UI_EDID_INFO_BLOCK1_DTD4_HORZ_AND_VERT_ACTIVE:
            return (MS_U8*)("DTD4: Horz /Vert Active");
        case E_UI_EDID_INFO_BLOCK1_DTD4_SCAN_TYPE:
            return (MS_U8*)("DTD4: Scan type");
        case E_UI_EDID_INFO_BLOCK1_DTD5_HORZ_AND_VERT_ACTIVE:
            return (MS_U8*)("DTD5: Horz /Vert Active");
        case E_UI_EDID_INFO_BLOCK1_DTD5_SCAN_TYPE:
            return (MS_U8*)("DTD5: Scan type");
        default:
            return NULL;
    }
}


MS_U8* _Demo_EDID_GetInfoString(EN_INTERFACE_EDID_BLOCK_ITEM eBlcokNum, MS_U16 u16InfoItem, MS_U8 u8Number)
{
    static MS_U8 u8Text[32] = {0};
    MS_U8 u8Vic[6];
    MS_U8 i, u8NumOfExtendTag;

    memset(u8Text, 0 , 32);
    memset(u8Vic, 0 , 6);
    if (eBlcokNum == 0)
    {
        switch (u16InfoItem)
        {
            case E_UI_EDID_INFO_BLOCK0_VERSION:
                sprintf((char*)u8Text, "%d", g_stEDID_Block0.u8VersionOfEDID);
                break;
            case E_UI_EDID_INFO_BLOCK0_REVISION:
                sprintf((char*)u8Text, "%d", g_stEDID_Block0.u8RevisionOfEDID);
                break;
            case E_UI_EDID_INFO_BLOCK0_EXTENSION_BLOCK_NUMBER:
                sprintf((char*)u8Text, "%d", g_stEDID_Block0.u8ExtensionBlockNumber);
                break;
            case E_UI_EDID_INFO_BLOCK0_PREFERRED_IMING_IS_NATIVE:
                sprintf((char*)u8Text, "%s",(g_stEDID_Block0.stFeatureSupport.bPreferredTiming != 0)?"YES":"NO");
                break;
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS1:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS2:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS3:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS4:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS5:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS6:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS7:
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS8:
                if (u8Number < g_stEDID_Block0.u8NumofStandardTiming)
                {
                    switch(g_stEDID_Block0.stStandardTiming[u8Number].enAspectRatio)
                    {
                        case E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_16_10:
                            sprintf((char*)u8Text, "%d x %d @%dHz (16:10)", g_stEDID_Block0.stStandardTiming[u8Number].u16HorizontalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u16VerticalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u8RefreshRate);
                        break;
                        case E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_4_3:
                            sprintf((char*)u8Text, "%d x %d @%dHz (4:3)", g_stEDID_Block0.stStandardTiming[u8Number].u16HorizontalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u16VerticalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u8RefreshRate);
                        break;
                        case E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_5_4:
                            sprintf((char*)u8Text, "%d x %d @%dHz (5:4)", g_stEDID_Block0.stStandardTiming[u8Number].u16HorizontalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u16VerticalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u8RefreshRate);
                        break;
                        case E_MSAPI_HDMITX_STANDARD_TIMING_ASPECT_RATIO_16_9:
                            sprintf((char*)u8Text, "%d x %d @%dHz (16:9)", g_stEDID_Block0.stStandardTiming[u8Number].u16HorizontalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u16VerticalActivePixels, g_stEDID_Block0.stStandardTiming[u8Number].u8RefreshRate);
                        break;
                        default:
                        break;
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "Not Used");
                }
                break;
            case E_UI_EDID_INFO_BLOCK0_DTD0_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK0_DTD1_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK0_DTD2_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK0_DTD3_HORZ_AND_VERT_ACTIVE:
                if (u8Number > 4)         //Max 4 DTD in block0
                {
                    sprintf((char*)u8Text, "ERROR");
                }
                else if (u8Number < g_stEDID_Block0.u8NumOfDTD)
                {
                    sprintf((char*)u8Text, "Horz Active: %d / Vert Active: %d", g_stEDID_Block0.stDTD[u8Number].u16HorzActive, g_stEDID_Block0.stDTD[u8Number].u16VertActive);
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK0_DTD0_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK0_DTD1_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK0_DTD2_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK0_DTD3_SCAN_TYPE:
                if (u8Number > 4)         //Max 4 DTD in block0
                {
                    sprintf((char*)u8Text, "ERROR");
                }
                else if (u8Number < g_stEDID_Block0.u8NumOfDTD)
                {
                    sprintf((char*)u8Text, "%s", (g_stEDID_Block0.stDTD[u8Number].bInterlace != 0)? "Interlace":"Progressive");
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            default:
                break;
        }
    }
    else if(eBlcokNum == 1)
    {
        if (E_UI_EDID_INFO_BLOCK1_MAX < u16InfoItem)
        {
            return NULL;
        }
        if (g_stEDID_Block1.bCEAExtension != TRUE)
        {
            return NULL;
        }
        switch (u16InfoItem)
        {
            case E_UI_EDID_INFO_BLOCK1_CEA_EXTENSION_VERSION:
                sprintf((char*)u8Text, "%d", g_stEDID_Block1.u8CEAExtensionVersion);
                break;
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_UNDERSCAN:
                sprintf((char*)u8Text, "%s", ((g_stEDID_Block1.bUnderscans != 0)? "YES":"NO"));
                break;
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_BASIC_AUDIO:
                sprintf((char*)u8Text, "%s", ((g_stEDID_Block1.bAudio != 0)? "YES":"NO"));
                break;
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_YCBCR:
                sprintf((char*)u8Text, "YCbCr 4:4:4 : %s / YCbCr 4:2:2 : %s", ((g_stEDID_Block1.bYCBCR444 != 0)? "Y":"N"), ((g_stEDID_Block1.bYCBCR422 != 0)? "Y":"N"));
                break;
            case E_UI_EDID_INFO_BLOCK1_NATIVE_DTDS_IN_EDID:
                sprintf((char*)u8Text, "%d", g_stEDID_Block1.u8NumofNativeDTDs);
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(0, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(0, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(1, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(1, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(2, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(2, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(3, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(3, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(4, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(4, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(5, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(5, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(6, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(6, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(7, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(7, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(8, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(8, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_FIRST_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(9, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_SECOND_LINE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Get_Audio_Format(9, 2));
                break;
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1:
                if (g_stEDID_Block1.u8VideoDataLen != 0)
                {
                    sprintf((char*)u8Text, "%s", _Demo_EDID_Get_VIC((MS_U16)E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2:
                if (g_stEDID_Block1.u8VideoDataLen != 0)
                {
                    sprintf((char*)u8Text, "%s", _Demo_EDID_Get_VIC((MS_U16)E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3:
                if (g_stEDID_Block1.u8VideoDataLen != 0)
                {
                    sprintf((char*)u8Text, "%s", _Demo_EDID_Get_VIC((MS_U16)E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IEEE_ID:
                if (g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "0x%02x%02x%02x", g_stEDID_Block1.stHDMISpecificData.u8RegistrationID[0], g_stEDID_Block1.stHDMISpecificData.u8RegistrationID[1]\
                        , g_stEDID_Block1.stHDMISpecificData.u8RegistrationID[2]);
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_CEC_PA:
                if (g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "%02x%02x",  g_stEDID_Block1.stHDMISpecificData.u8Phyaddr[0], g_stEDID_Block1.stHDMISpecificData.u8Phyaddr[1]);
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DEEP_COLOR:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 5))
                {
                     sprintf((char*)u8Text, "YCbCr 444:%s, 30:%s, 36:%s, 48:%s", ((g_stEDID_Block1.stHDMISpecificData.bSupportedYCbCr444 != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stHDMISpecificData.bSupported30bitsDeepColor != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stHDMISpecificData.bSupported36bitsDeepColor != 0)? "Y":"N"), ((g_stEDID_Block1.stHDMISpecificData.bSupported48bitsDeepColor != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_MAX_TMDS_CLOCK:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 5))
                {
                    sprintf((char*)u8Text, "%d MHz", g_stEDID_Block1.stHDMISpecificData.u16MaxTMDSclock);
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_LATENCY:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {
                    sprintf((char*)u8Text, "Latency:%s / Interlaced Latency:%s", ((g_stEDID_Block1.stHDMISpecificData.bLatency != 0)? "Y" : "N"), ((g_stEDID_Block1.stHDMISpecificData.bInterlacedLatency != 0)? "Y" : "N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_BASIC_3D:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {
                    sprintf((char*)u8Text, "%s", ((g_stEDID_Block1.stHDMISpecificData.bBasic3D == 1)? "support":"Not support"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IMAGE_SIZE:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {

                    if(g_stEDID_Block1.stHDMISpecificData.u8ImageSize == 0x00)
                    {
                        sprintf((char*)u8Text, "No additional Info");
                    }
                    else if (g_stEDID_Block1.stHDMISpecificData.u8ImageSize == 0x01)
                    {
                        sprintf((char*)u8Text, "Not guaranteed to be correct");
                    }
                    else if (g_stEDID_Block1.stHDMISpecificData.u8ImageSize == 0x02)
                    {
                        sprintf((char*)u8Text, "Accurate to 1 cm");
                    }
                    else
                    {
                        sprintf((char*)u8Text, "Accurate to 5 cm");
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_4KX2K_SUPPORT:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {
                    for (i = 0; i < g_stEDID_Block1.stHDMISpecificData.u8LenOfVIC; i = i + 1)
                    {
                        if (i == 0)
                        {
                            sprintf((char*)u8Text, "VIC = 95");
                        }
                        else if (i ==1)
                        {
                            strcat((char*)u8Text, ", 94");
                        }
                        else if (i ==2)
                        {
                            strcat((char*)u8Text, ", 93");
                        }
                        else
                        {
                            strcat((char*)u8Text, ", 98");
                        }
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT1:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {
                    switch(g_stEDID_Block1.stHDMISpecificData.u8Multi3D)
                    {
                        case 0x01:
                        case 0x02:
                            if (g_stEDID_Block1.stHDMISpecificData.st3DSupport.bSidBySide)
                            {
                                sprintf((char*)u8Text, "SBS,");
                                if (g_stEDID_Block1.stHDMISpecificData.st3DSupport.bFramePacking)
                                {
                                    strcat((char*)u8Text, "FP,");
                                }
                                if (g_stEDID_Block1.stHDMISpecificData.st3DSupport.bTopAndBottom)
                                {
                                    strcat((char*)u8Text, "TAB,");
                                }
                            }
                            else
                            {
                                if (g_stEDID_Block1.stHDMISpecificData.st3DSupport.bFramePacking)
                                {
                                    sprintf((char*)u8Text, "FP,");
                                    if (g_stEDID_Block1.stHDMISpecificData.st3DSupport.bTopAndBottom)
                                    {
                                        strcat((char*)u8Text, "TAB,");
                                    }
                                }
                                else
                                {
                                    if (g_stEDID_Block1.stHDMISpecificData.st3DSupport.bTopAndBottom)
                                    {
                                        sprintf((char*)u8Text, "TAB,");
                                    }
                                }
                            }
                            if (g_stEDID_Block1.stHDMISpecificData.u8Multi3D == 0x02)
                            {
                                for (i = 0; i < g_stEDID_Block1.stHDMISpecificData.u8LenOf3DSVD; i = i + 1)
                                {
                                    if (i > 6)  //one line for 6 vic
                                    {
                                        break;
                                    }
                                    sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.stHDMISpecificData.u8SVDof3D[i]);
                                    strcat((char*)u8Text, (char*)u8Vic);
                                }
                            }
                            else
                            {
                                for (i = 0; i <  g_stEDID_Block1.u8VideoDataLen; i = i + 1)
                                {
                                    if (i > 6)  //one line for 6 vic
                                    {
                                        break;
                                    }
                                    sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.u8SVD[i]);
                                    strcat((char*)u8Text, (char*)u8Vic);
                                }
                            }
                            break;
                        case 0x00:
                        case 0x03:
                        default:
                            sprintf((char*)u8Text, "NO INFO");
                            break;
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT2:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {
                    switch(g_stEDID_Block1.stHDMISpecificData.u8Multi3D)
                    {
                        case 0x01:
                        case 0x02:
                            if (g_stEDID_Block1.stHDMISpecificData.u8Multi3D == 0x02)
                            {
                                for (i = 7; i < g_stEDID_Block1.stHDMISpecificData.u8LenOf3DSVD; i = i + 1)
                                {
                                    if (i > 15)  //max num of 3D VIC is 15
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.stHDMISpecificData.u8SVDof3D[i]);
                                        strcat((char*)u8Text, (char*)u8Vic);
                                    }
                                }
                            }
                            else
                            {
                                for (i = 7; i <  g_stEDID_Block1.u8VideoDataLen; i = i + 1)
                                {
                                    if ( i > 15)  //max num of 3D VIC is 15
                                    {
                                        break;
                                    }
                                    sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.u8SVD[i]);
                                    strcat((char*)u8Text, (char*)u8Vic);
                                }
                            }
                            break;
                        case 0x00:
                        case 0x03:
                        default:
                            break;
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DETAILED_3D:
                if ((g_stEDID_Block1.u8HDMIVendorSpecificDataLen != 0) && (g_stEDID_Block1.u8HDMIVendorSpecificDataLen > 7))
                {
                    if (g_stEDID_Block1.stHDMISpecificData.u8LenOf3DDetail == 0)
                    {
                        sprintf((char*)u8Text, "NO INFO");
                    }
                    for (i = 0; i<g_stEDID_Block1.stHDMISpecificData.u8LenOf3DDetail; i = i + 1)
                    {
                        switch (g_stEDID_Block1.stHDMISpecificData.stDetail3D[i].u8TypeOf3D)
                        {
                            case 0x00:
                                if(i == 0)
                                {
                                    sprintf((char*)u8Text, "FP:");
                                }
                                else
                                {
                                    strcat((char*)u8Text, "FP:");
                                }
                                break;
                            case 0x06:
                                if(i == 0)
                                {
                                    sprintf((char*)u8Text, "TAB:");
                                }
                                else
                                {
                                    strcat((char*)u8Text, "TAB:");
                                }
                                break;
                            case 0x08:
                                if(i == 0)
                                {
                                    sprintf((char*)u8Text, "SBS:");
                                }
                                else
                                {
                                    strcat((char*)u8Text, "SBS:");
                                }
                                break;
                            default:
                                printf("%d\n", g_stEDID_Block1.stHDMISpecificData.stDetail3D[i].u8TypeOf3D);
                                break;
                        }
                        sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.u8SVD[g_stEDID_Block1.stHDMISpecificData.stDetail3D[i].u8VICof2D]);
                        strcat((char*)u8Text, (char*)u8Vic);
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_IEEE_ID_AND_VERSION:
                if (g_stEDID_Block1.u8AudioVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "IEEE: 0x%02x%02x%02x / Versin: %d", g_stEDID_Block1.stHDMI20SpecificData.u8RegistrationID[0],\
                        g_stEDID_Block1.stHDMI20SpecificData.u8RegistrationID[1],  g_stEDID_Block1.stHDMI20SpecificData.u8RegistrationID[2], \
                        g_stEDID_Block1.stHDMI20SpecificData.u8Version);
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_MAX_TMDS_CHARACTER_RATE:
                if (g_stEDID_Block1.u8AudioVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "%d MHz", g_stEDID_Block1.stHDMI20SpecificData.u16MaxTMDSCharacterRate);
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_3D_OSD_DISPARITY_AND_LTE:
                if (g_stEDID_Block1.u8AudioVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "3D: %s / LTE_340MHz_scramble: %s", ((g_stEDID_Block1.stHDMI20SpecificData.bOSDDisparityOf3D != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stHDMI20SpecificData.bLTE340MHzScramble != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_VIEW:
                if (g_stEDID_Block1.u8AudioVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "Dual View: %s /Independent view: %s", ((g_stEDID_Block1.stHDMI20SpecificData.bDualView != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stHDMI20SpecificData.bIndependentView != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_RR_AND_SCDC:
                if (g_stEDID_Block1.u8AudioVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "RR_Capable: %s /SCDC_Present: %s", ((g_stEDID_Block1.stHDMI20SpecificData.bRRCapable != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stHDMI20SpecificData.bSCDCPresent != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_DC420:
                if (g_stEDID_Block1.u8AudioVendorSpecificDataLen != 0)
                {
                    sprintf((char*)u8Text, "30bit: %s /36bit: %s /48bit: %s", ((g_stEDID_Block1.stHDMI20SpecificData.bDC30bit420 != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stHDMI20SpecificData.bDC36bit420 != 0)? "Y":"N"), ((g_stEDID_Block1.stHDMI20SpecificData.bDC48bit420 != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF, &u8NumOfExtendTag) == TRUE)
                {
                    sprintf((char*)u8Text, "SDR:%s/ HDR:%s/ ST 2084:%s/ Future:%s", ((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].stHDRStaticMetadataDataBlock.bSDRLuminanceRange != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].stHDRStaticMetadataDataBlock.bHDRLuminanceRange != 0)? "Y":"N")\
                        , ((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].stHDRStaticMetadataDataBlock.bEOTFInSMPTEST2084 != 0)? "Y":"N")\
                        ,((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].stHDRStaticMetadataDataBlock.bFutureEOTF != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR, &u8NumOfExtendTag) == TRUE)
                {
                    sprintf((char*)u8Text, "Type1:%s", ((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].stHDRStaticMetadataDataBlock.bStaticMetadataType1 != 0)? "Y":"N"));
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY1:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_COLORIMETRY1, &u8NumOfExtendTag) == TRUE)
                {
                    sprintf((char*)u8Text, "BT:");
                    if((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bBT2020RGB == 0) && (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bBT2020YCC == 0) && (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bBT2020cYCC == 0))
                    {
                        strcat((char*)u8Text, "NOT USED");
                    }
                    else
                    {
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bBT2020RGB)
                        {
                            strcat((char*)u8Text, "RGB,");
                        }
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bBT2020YCC)
                        {
                            strcat((char*)u8Text, "YCC,");
                        }
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bBT2020cYCC)
                        {
                            strcat((char*)u8Text, "cYCC");
                        }
                    }
                    strcat((char*)u8Text, " /Adobe:");
                    if((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bAdobeRGB == 0) && (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bAdobeRGB == 0))
                    {
                        strcat((char*)u8Text, "NOT USED");
                    }
                    else
                    {
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bAdobeRGB)
                        {
                            strcat((char*)u8Text, "RGB,");
                        }
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bAdobeYCC601)
                        {
                            strcat((char*)u8Text, "YCC601");
                        }
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY2:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_COLORIMETRY2, &u8NumOfExtendTag) == TRUE)
                {
                    if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bsYCC601 != 0)
                    {
                        sprintf((char*)u8Text, "sYCC601");
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bxvYCC709 != 0)
                        {
                            strcat((char*)u8Text, ", xvYCC709");
                        }
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bxvYCC601 != 0)
                        {
                            strcat((char*)u8Text, ", xvYCC601");
                        }
                    }
                    else
                    {
                        if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bxvYCC709 != 0)
                        {
                            sprintf((char*)u8Text, "xvYCC709");
                            if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bxvYCC601 != 0)
                            {
                                strcat((char*)u8Text, ", xvYCC601");
                            }
                        }
                        else
                        {
                            if (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bxvYCC601 != 0)
                            {
                                sprintf((char*)u8Text, "xvYCC601");
                            }
                        }
                    }
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1, &u8NumOfExtendTag) == TRUE)
                {
                    for (i = 0; i < g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8NumOfY420VideoDataBlock; i=i+1)
                    {
                        if (i > 11)  //one line for 11 vic
                        {
                            break;
                        }
                        if (i == 0)
                        {
                            sprintf((char*)u8Text, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8Y420VideoDataBlock[i]);
                        }
                        else
                        {
                            sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8Y420VideoDataBlock[i]);
                            strcat((char*)u8Text, (char*)u8Vic);
                        }
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2, &u8NumOfExtendTag) == TRUE)
                {
                    for (i = 11; i < g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8NumOfY420VideoDataBlock; i=i+1)
                    {
                        if (i > 21)  //one line for 11 vic
                        {
                            break;
                        }
                        if (i == 11)
                        {
                            sprintf((char*)u8Text, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8Y420VideoDataBlock[i]);
                        }
                        else
                        {
                            sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8Y420VideoDataBlock[i]);
                            strcat((char*)u8Text, (char*)u8Vic);
                        }
                    }
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1, &u8NumOfExtendTag) == TRUE)
                {
                    for (i = 0; i < g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8NumofSupportY420; i=i+1)
                    {
                        if (i > 11)  //one line for 11 vic
                        {
                            break;
                        }
                        if (i == 0)
                        {
                            sprintf((char*)u8Text, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8SupportY420[i]);
                        }
                        else
                        {
                            sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8SupportY420[i]);
                            strcat((char*)u8Text, (char*)u8Vic);
                        }
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2, &u8NumOfExtendTag) == TRUE)
                {
                    for (i = 11; i < g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8NumofSupportY420; i=i+1)
                    {
                        if (i > 21)  //one line for 11 vic
                        {
                            break;
                        }
                        if (i == 11)
                        {
                            sprintf((char*)u8Text, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8SupportY420[i]);
                        }
                        else
                        {
                            sprintf((char*)u8Vic, "%u,", g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].u8SupportY420[i]);
                            strcat((char*)u8Text, (char*)u8Vic);
                        }
                    }
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1, &u8NumOfExtendTag) == TRUE)
                {
                    switch (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].enCE)
                    {
                        case E_MSAPI_HDMI_CE_NOT_SUPPORT:
                            sprintf((char*)u8Text, "CE:NO,");
                            break;
                        case E_MSAPI_HDMI_CE_ALWAYS_OVERSCANNED:
                            sprintf((char*)u8Text, "CE:Over,");
                            break;
                        case E_MSAPI_HDMI_CE_ALWAYS_UNDERSCANNED:
                            sprintf((char*)u8Text, "CE:Under,");
                            break;
                        case E_MSAPI_HDMI_CE_SUPPORTS_OVER_AND_UNDERSCAN:
                            sprintf((char*)u8Text, "CE:BOTH,");
                            break;
                        default:
                            break;
                    }
                    switch (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].enIT)
                    {
                        case E_MSAPI_HDMI_IT_NOT_SUPPORT:
                            strcat((char*)u8Text, "IT:NO,");
                            break;
                        case E_MSAPI_HDMI_IT_ALWAYS_OVERSCANNED:
                            strcat((char*)u8Text, "IT:Over,");
                            break;
                        case E_MSAPI_HDMI_IT_ALWAYS_UNDERSCANNED:
                            strcat((char*)u8Text, "IT:Under,");
                            break;
                        case E_MSAPI_HDMI_IT_SUPPORTS_OVER_AND_UNDERSCAN:
                            strcat((char*)u8Text, "IT:BOTH,");
                            break;
                        default:
                            break;
                    }
                    switch (g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].enPT)
                    {
                        case E_MSAPI_HDMI_PT_NOT_SUPPORT:
                            strcat((char*)u8Text, "PT:Refer CE & IT");
                            break;
                        case E_MSAPI_HDMI_PT_ALWAYS_OVERSCANNED:
                            strcat((char*)u8Text, "PT:Over");
                            break;
                        case E_MSAPI_HDMI_PT_ALWAYS_UNDERSCANNED:
                            strcat((char*)u8Text, "PT:Under");
                            break;
                        case E_MSAPI_HDMI_PT_SUPPORTS_OVER_AND_UNDERSCAN:
                            strcat((char*)u8Text, "PT:BOTH");
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    sprintf((char*)u8Text, "NO INFO");
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY2:
                if (_Demo_EDID_Check_Exteand_Code(E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1, &u8NumOfExtendTag) == TRUE)
                {
                        sprintf((char*)u8Text, "RGB:%s/ YCC:%s", ((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bRGBQuantization != 0)? "Selectable": "No Data")\
                            , ((g_stEDID_Block1.stExteandCode[u8NumOfExtendTag].bYCCQuantization != 0)? "Selectable": "No Data"));
                }
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD0_HORZ_AND_VERT_ACTIVE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(0, 0));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD0_SCAN_TYPE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(0, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD1_HORZ_AND_VERT_ACTIVE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(1, 0));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD1_SCAN_TYPE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(1, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD2_HORZ_AND_VERT_ACTIVE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(2, 0));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD2_SCAN_TYPE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(2, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD3_HORZ_AND_VERT_ACTIVE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(3, 0));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD3_SCAN_TYPE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(3, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD4_HORZ_AND_VERT_ACTIVE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(4, 0));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD4_SCAN_TYPE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(4, 1));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD5_HORZ_AND_VERT_ACTIVE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(5, 0));
                break;
            case E_UI_EDID_INFO_BLOCK1_DTD5_SCAN_TYPE:
                sprintf((char*)u8Text, "%s", _Demo_EDID_Blcok1_DTD(5, 1));
                break;
            default:
                return NULL;
                break;
        }
    }
    else
    {
        return FALSE;
    }
    return u8Text;
}

extern MS_U8* Demo_EDID_GetUIInfoString_Block0(MS_U16 u16Line, MS_U16 u16Pos)
{
    if(Demo_EDID_GetEDIDInfo(E_UI_EDID_BLOCK0_INFO) == FALSE)
    {
        return NULL;
    }
    if(u16Pos == 0) //position 0
    {
        switch(u16Line)
        {
            case E_UI_EDID_INFO_BLOCK0_VERSION:
                Demo_EDID_GetEDIDInfo(E_UI_EDID_BLOCK0_INFO);
                return (MS_U8*)("EDID Version");
            case E_UI_EDID_INFO_BLOCK0_REVISION:
                return (MS_U8*)("EDID Revision");
            case E_UI_EDID_INFO_BLOCK0_EXTENSION_BLOCK_NUMBER:
                return (MS_U8*)("Extension Block Number");
            case E_UI_EDID_INFO_BLOCK0_PREFERRED_IMING_IS_NATIVE:
                return (MS_U8*)("Native Preferred Timing");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS1:
                return (MS_U8*)("Standard Timings 1");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS2:
                return (MS_U8*)("Standard Timings 2");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS3:
                return (MS_U8*)("Standard Timings 3");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS4:
                return (MS_U8*)("Standard Timings 4");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS5:
                return (MS_U8*)("Standard Timings 5");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS6:
                return (MS_U8*)("Standard Timings 6");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS7:
                return (MS_U8*)("Standard Timings 7");
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS8:
                return (MS_U8*)("Standard Timings 8");
            case E_UI_EDID_INFO_BLOCK0_DTD0_HORZ_AND_VERT_ACTIVE:
                return (MS_U8*)("DTD0: Horz /Vert Active");
            case E_UI_EDID_INFO_BLOCK0_DTD0_SCAN_TYPE:
                return (MS_U8*)("DTD0: Scan type");
            case E_UI_EDID_INFO_BLOCK0_DTD1_HORZ_AND_VERT_ACTIVE:
                return (MS_U8*)("DTD1: Horz /Vert Active");
            case E_UI_EDID_INFO_BLOCK0_DTD1_SCAN_TYPE:
                return (MS_U8*)("DTD1: Scan type");
            case E_UI_EDID_INFO_BLOCK0_DTD2_HORZ_AND_VERT_ACTIVE:
                return (MS_U8*)("DTD2: Horz /Vert Active");
            case E_UI_EDID_INFO_BLOCK0_DTD2_SCAN_TYPE:
                return (MS_U8*)("DTD2: Scan type");
            case E_UI_EDID_INFO_BLOCK0_DTD3_HORZ_AND_VERT_ACTIVE:
                return (MS_U8*)("DTD3: Horz /Vert Active");
            case E_UI_EDID_INFO_BLOCK0_DTD3_SCAN_TYPE:
                return (MS_U8*)("DTD3: Scan type");
            default:
                return NULL;
                break;
        }
    }
    else if(u16Pos == 1) //position 1
    {
        switch(u16Line)
        {
            case E_UI_EDID_INFO_BLOCK0_VERSION:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_VERSION, 0);
            case E_UI_EDID_INFO_BLOCK0_REVISION:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_REVISION, 0);
            case E_UI_EDID_INFO_BLOCK0_EXTENSION_BLOCK_NUMBER:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_EXTENSION_BLOCK_NUMBER, 0);
            case E_UI_EDID_INFO_BLOCK0_PREFERRED_IMING_IS_NATIVE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_PREFERRED_IMING_IS_NATIVE, 0);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS1:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS1, 0);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS2:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS2, 1);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS3:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS3, 2);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS4:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS4, 3);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS5:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS5, 4);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS6:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS6, 5);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS7:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS7, 6);
            case E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS8:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_STANDARD_TIMINGS8, 7);
            case E_UI_EDID_INFO_BLOCK0_DTD0_HORZ_AND_VERT_ACTIVE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD0_HORZ_AND_VERT_ACTIVE, 0);
            case E_UI_EDID_INFO_BLOCK0_DTD0_SCAN_TYPE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD0_SCAN_TYPE, 0);
            case E_UI_EDID_INFO_BLOCK0_DTD1_HORZ_AND_VERT_ACTIVE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD1_HORZ_AND_VERT_ACTIVE, 1);
            case E_UI_EDID_INFO_BLOCK0_DTD1_SCAN_TYPE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD1_SCAN_TYPE, 1);
            case E_UI_EDID_INFO_BLOCK0_DTD2_HORZ_AND_VERT_ACTIVE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD2_HORZ_AND_VERT_ACTIVE, 2);
            case E_UI_EDID_INFO_BLOCK0_DTD2_SCAN_TYPE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD2_SCAN_TYPE, 2);
            case E_UI_EDID_INFO_BLOCK0_DTD3_HORZ_AND_VERT_ACTIVE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD3_HORZ_AND_VERT_ACTIVE, 3);
            case E_UI_EDID_INFO_BLOCK0_DTD3_SCAN_TYPE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK0_INFO, (MS_U8)E_UI_EDID_INFO_BLOCK0_DTD3_SCAN_TYPE, 3);
            default:
                return NULL;
        }
    }
    return NULL;
}

extern MS_U8* Demo_EDID_GetUIInfoString_Block1(MS_U16 u16Line, MS_U16 u16Pos)
{
    if(Demo_EDID_GetEDIDInfo(E_UI_EDID_BLOCK1_INFO) == FALSE)
    {
        return NULL;
    }
    if(u16Pos == 0) //position 0
    {
        switch(u16Line)
        {
            case E_UI_EDID_INFO_BLOCK1_CEA_EXTENSION_VERSION:
                return _Demo_EDID_GetItemString((MS_U16)E_UI_EDID_INFO_BLOCK1_CEA_EXTENSION_VERSION);
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_UNDERSCAN:
                return _Demo_EDID_GetItemString((MS_U16)E_UI_EDID_INFO_BLOCK1_SUPPORTS_UNDERSCAN);
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_BASIC_AUDIO:
                return _Demo_EDID_GetItemString((MS_U16)E_UI_EDID_INFO_BLOCK1_SUPPORTS_BASIC_AUDIO);
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_YCBCR:
                return _Demo_EDID_GetItemString((MS_U16)E_UI_EDID_INFO_BLOCK1_SUPPORTS_YCBCR);
            case E_UI_EDID_INFO_BLOCK1_NATIVE_DTDS_IN_EDID:
                return _Demo_EDID_GetItemString((MS_U16)E_UI_EDID_INFO_BLOCK1_NATIVE_DTDS_IN_EDID);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 0)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 1)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 2)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 3)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 4)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 5)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 6)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 7)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 8)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 9)
                {
                    return _Demo_EDID_GetItemString(u16Line);
                }
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1:
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2:
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IEEE_ID:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_CEC_PA:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DEEP_COLOR:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_MAX_TMDS_CLOCK:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_LATENCY:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_BASIC_3D:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IMAGE_SIZE:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_4KX2K_SUPPORT:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT1:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT2:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DETAILED_3D:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_IEEE_ID_AND_VERSION:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_MAX_TMDS_CHARACTER_RATE:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_3D_OSD_DISPARITY_AND_LTE:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_VIEW:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_RR_AND_SCDC:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_DC420:
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF:
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY2:
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY1:
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY2:
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1:
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2:
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1:
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2:
            case E_UI_EDID_INFO_BLOCK1_DTD0_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD0_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD1_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD1_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD2_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD2_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD3_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD3_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD4_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD4_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD5_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD5_SCAN_TYPE:
                return _Demo_EDID_GetItemString((u16Line + g_u16EDID_Skip_line));
            default:
                return NULL;
        }
    }
    else if(u16Pos == 1) //position 1
    {
        switch(u16Line)
        {
            case E_UI_EDID_INFO_BLOCK1_CEA_EXTENSION_VERSION:
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_UNDERSCAN:
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_BASIC_AUDIO:
            case E_UI_EDID_INFO_BLOCK1_SUPPORTS_YCBCR:
            case E_UI_EDID_INFO_BLOCK1_NATIVE_DTDS_IN_EDID:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 0);
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA1:
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA2:
            case E_UI_EDID_INFO_BLOCK1_VIDEO_DATA3:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT1:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_GENERAL_3D_SUPPORT2:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DETAILED_3D:
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY1:
            case E_UI_EDID_INFO_BLOCK1_COLORIMETRY2:
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA1:
            case E_UI_EDID_INFO_BLOCK1_Y420_VIDEO_DATA2:
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP1:
            case E_UI_EDID_INFO_BLOCK1_Y420_CAPABILITY_MAP2:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IEEE_ID:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_CEC_PA:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_DEEP_COLOR:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_MAX_TMDS_CLOCK:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_LATENCY:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_BASIC_3D:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_IMAGE_SIZE:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC_4KX2K_SUPPORT:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_IEEE_ID_AND_VERSION:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_MAX_TMDS_CHARACTER_RATE:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_3D_OSD_DISPARITY_AND_LTE:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_VIEW:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_RR_AND_SCDC:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_SPECIFIC2_DC420:
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_EOTF:
            case E_UI_EDID_INFO_BLOCK1_HDR_SUPPORTED_STATIC_METADATA_DESCRIPTOR:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY1:
            case E_UI_EDID_INFO_BLOCK1_VENDOR_VIDEO_CAPABILITY2:
            case E_UI_EDID_INFO_BLOCK1_DTD0_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD0_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD1_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD1_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD2_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD2_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD3_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD3_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD4_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD4_SCAN_TYPE:
            case E_UI_EDID_INFO_BLOCK1_DTD5_HORZ_AND_VERT_ACTIVE:
            case E_UI_EDID_INFO_BLOCK1_DTD5_SCAN_TYPE:
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 0);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA1_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 0)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 0);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 0);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA2_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 1)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 1);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 1);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA3_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 2)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 2);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 2);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA4_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 3)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 3);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 3);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA5_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 4)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 4);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 4);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA6_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 5)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 5);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 5);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA7_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 6)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 6);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 6);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA8_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 7)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 7);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 7);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA9_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 8)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 8);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 8);
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_FIRST_LINE:
            case E_UI_EDID_INFO_BLOCK1_AUDIO_DATA10_SECOND_LINE:
                if ((g_stEDID_Block1.u8AudioDataLen / 3) > 9)
                {
                    return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, u16Line, 9);
                }
                return _Demo_EDID_GetInfoString(E_UI_EDID_BLOCK1_INFO, (u16Line + g_u16EDID_Skip_line), 9);
            default:
                return NULL;
                break;
        }
    }
    return NULL;
}


extern MS_BOOL Demo_EDID_GetEDIDInfo(EN_INTERFACE_EDID_BLOCK_ITEM eEDIDBlock)
{
    switch (eEDIDBlock)
    {
        case E_UI_EDID_BLOCK0_INFO:
                memset(&g_stEDID_Block0, 0, sizeof(g_stEDID_Block0));
                if (msAPI_HDMI_GetEDIDBlockInfo(eEDIDBlock, &g_stEDID_Block0, sizeof(MSAPI_HDMITX_EDID_BLOCK0)) != E_MSAPI_XC_OK)
                {
                    return FALSE;
                }
            break;
        case E_UI_EDID_BLOCK1_INFO:
                memset(&g_stEDID_Block1, 0, sizeof(g_stEDID_Block1));
                if (msAPI_HDMI_GetEDIDBlockInfo(eEDIDBlock, &g_stEDID_Block1, sizeof(MSAPI_HDMITX_EDID_BLOCK1)) != E_MSAPI_XC_OK)
                {
                    return FALSE;
                }
                g_u16EDID_Skip_line = (10 - (g_stEDID_Block1.u8AudioDataLen / 3)) * 2;  //MAX Audio Data Len = 10
            break;
        default:
            break;
    }
    return TRUE;
}
extern MS_BOOL Demo_EDID_INFO(void)
{
    //page switch
	u16LineCount = 64;
    MApp_ZUI_UTL_PageSwitch(E_OSD_DISPLAY_INFO);
    return TRUE;
}


#endif

