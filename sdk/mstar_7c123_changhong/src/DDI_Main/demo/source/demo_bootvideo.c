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

#if (DEMO_BOOTVIDEO_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsMemory.h"
#include <string.h>
#include "MsOS.h"

#include "apiGOP.h"
#include "apiGFX.h"

#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiPNL_EX.h"
#else
#include "apiPNL.h"
#endif

#include "demo_bootvideo.h"
#include "demo_audio_common.h"
#include "demo_dmx.h"

#if(DEMO_AUDIO_TEST == 1)
#include "demo_audio.h"
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define GOP_ID 3  // 3 for seamless zapping

//-------------------------------------------------------------------------------------------------
// Global Definition
//-------------------------------------------------------------------------------------------------
static MS_U8 _gu8GwinId = 0xFF;
static EN_BOOTVIDEO_STATE _geBootvideoState = E_BOOTVIDEO_STATE_RUNNING;

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define BOOTVIDEO_DBG                0

#if BOOTVIDEO_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
#define IPANEL(dev, func) g_IPanelEx.func(dev)
#else
#define IPANEL(dev, func) g_IPanel.func()
#endif
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
#if (DEMO_XC_DUALXC_TEST == 1)
static PNL_DeviceId gDevId = {0,E_PNL_EX_DEVICE0};
#endif


//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------

MS_BOOL _Demo_BOOTVIDEO_CreateFb(MS_U8 *u8FBId, MS_U16 u16FBWidth, MS_U16 u16FBHeight, MS_U16 u16FBFmt)
{
    GOP_GwinFBAttr stGwinFbAttr;
    *u8FBId = MApi_GOP_GWIN_GetFreeFBID();
    if(MAX_GWIN_FB_SUPPORT <= *u8FBId)
        return FALSE;

    db_print("Create FB %d with size (%d,%d) \n", *u8FBId, u16FBWidth, u16FBHeight);
    if (MApi_GOP_GWIN_CreateFB( *u8FBId, 0, 0, u16FBWidth, u16FBHeight, u16FBFmt) != GWIN_OK)
    {
        db_print("MApi_GOP_GWIN_CreateFB: failed to create FB \n");
        return FALSE;
    }

    MApi_GOP_GWIN_GetFBInfo( *u8FBId, &stGwinFbAttr);

    if(u16FBFmt==(MS_U16)E_MS_FMT_YUV422)
    {
        MApi_GFX_ClearFrameBufferByWord(stGwinFbAttr.addr, stGwinFbAttr.size, 0x80108010);
    }
    else
    {
        MApi_GFX_ClearFrameBuffer(stGwinFbAttr.addr, stGwinFbAttr.size, 0x00);
    }

    MApi_GFX_FlushQueue();
    return TRUE;

}

MS_BOOL _Demo_BOOTVIDEO_CreateGwin(MS_U8 *u8FBId, MS_U16 u16FBFmt)
{
    db_print("Create FB %d \n", *u8FBId);

    _gu8GwinId = MApi_GOP_GWIN_CreateWin_Assign_FB(GOP_ID, *u8FBId, 0, 0);

    MApi_GOP_GWIN_SwitchGOP(GOP_ID);
    //MApi_GOP_GWIN_Switch2Gwin(_gu8GwinId);

    //set transparent color
    if( u16FBFmt == E_MS_FMT_ARGB8888 || u16FBFmt == E_MS_FMT_ARGB4444)
    {
        MApi_GOP_GWIN_SetBlending( _gu8GwinId, true, 0xff);
    }else
    {
        MApi_GOP_GWIN_SetBlending( _gu8GwinId, false, 0xff);
    }

    MApi_GOP_GWIN_EnableTransClr_EX(GOP_ID, GOPTRANSCLR_FMT0, FALSE);
    MApi_GOP_GWIN_OutputLayerSwitch(GOP_ID); //Set GOP0 show in the top layer (mux2)
    MApi_GOP_GWIN_SetGOPDst(GOP_ID, E_GOP_DST_OP0);
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);

    return TRUE;
}



MS_BOOL _Demo_BOOTVIDEO_SetDwinConfig(MS_U8 *u8FBId, MS_U16 u16FBWidth, MS_U16 u16FBHeight, MS_U16 u16FBFmt)
{
    GOP_GwinFBAttr stGwinFbAttr;
    GOP_DwinProperty stDwinProperty;
    E_GOP_API_Result eDwinRet;

    // GOP DWIN init
    eDwinRet = MApi_GOP_DWIN_Init();
    if(eDwinRet != GOP_API_SUCCESS)
    {
        printf("Error 0x%x: failed to MApi_GOP_DWIN_Init()\n", eDwinRet);
        return FALSE;
    }

    //Source can only from Scaler OP
    MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);

    //OP scan type is progressive
    MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);


    //Data format only YUV; Set data format to YUV422
    MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);

    MApi_GOP_GWIN_GetFBInfo(*u8FBId, &stGwinFbAttr);
    memset(&stDwinProperty, 0, sizeof(GOP_DwinProperty));
    // Set DWin property
    stDwinProperty.u16x = 0;
    stDwinProperty.u16y = 0;
    stDwinProperty.u16fbw = u16FBWidth;
    stDwinProperty.u16w = u16FBWidth;
    stDwinProperty.u16h = u16FBHeight;
    stDwinProperty.u32fbaddr0 = stGwinFbAttr.addr;
    stDwinProperty.u32fbaddr1 = stGwinFbAttr.addr + stGwinFbAttr.size;

    eDwinRet = MApi_GOP_DWIN_SetWinProperty(&stDwinProperty);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        printf("Error 0x%x: failed to MApi_GOP_DWIN_SetWinProperty()!\n", eDwinRet);
        return FALSE;
    }

    return TRUE;

}

MS_BOOL Demo_BOOTVIDEO_Stop(void)
{
    MS_U32 u32AudioAppType = AUDIO_APP_DTV;

    if(_geBootvideoState != E_BOOTVIDEO_STATE_RUNNING)
    {
        printf("No Bootvideo is Running\n");
        return FALSE;
    }

    // Freeze gwin image
    MApi_GOP_DWIN_Enable(FALSE);
    Demo_Audio_Init(&u32AudioAppType);
    Demo_DMX_Init(); // Init DMX after setting GOP OK

    _geBootvideoState = E_BOOTVIDEO_STATE_STOP;

    return TRUE;
}

MS_BOOL Demo_BOOTVIDEO_DwinCapture(void)
{
    MS_U8 u8DwinFbId = 0xFF;
    MS_U16 u16PanelWidth = IPANEL(&gDevId, Width);
    MS_U16 u16PanelHeight = IPANEL(&gDevId, Height);
    MS_ColorFormat eColorFormat = E_MS_FMT_YUV422;

    if(_geBootvideoState != E_BOOTVIDEO_STATE_RUNNING)
    {
        printf("No Bootvideo is Running\n");
        return FALSE;
    }

    // for Fb
    if ( !_Demo_BOOTVIDEO_CreateFb(&u8DwinFbId, u16PanelWidth, u16PanelHeight, eColorFormat))
    {
        printf("Create FB fail\n");
        return FALSE;
    }

    // for Gwin
    if ( !_Demo_BOOTVIDEO_CreateGwin(&u8DwinFbId, eColorFormat))
    {
        printf("Create Gwin fail\n");
        return FALSE;
    }

    // for Dwin
    if( !_Demo_BOOTVIDEO_SetDwinConfig(&u8DwinFbId, u16PanelWidth, u16PanelHeight, eColorFormat))
    {
        printf("Set Dwin info fail\n");
        return FALSE;
    }

    // start DWIN capture
    MApi_GOP_DWIN_Enable(TRUE);
    MsOS_DelayTask(200);    // wait Dwin capture

    MApi_GOP_GWIN_Enable(_gu8GwinId, TRUE);

    return TRUE;
}

MS_BOOL Demo_BOOTVIDEO_DesrtoyGwin(void)
{
    MS_U8 u8DwinFbId = MApi_GOP_GWIN_GetCurrentFBID();

    if(_geBootvideoState != E_BOOTVIDEO_STATE_STOP)
    {
        printf("Stop Bootvideo First\n");
        return FALSE;
    }

    MApi_GOP_GWIN_Enable(_gu8GwinId, FALSE);
    MApi_GOP_GWIN_DestroyWin(_gu8GwinId);
    _gu8GwinId = 0xFF;
    MApi_GOP_GWIN_DestroyFB(u8DwinFbId);

    _geBootvideoState = E_BOOTVIDEO_STATE_DISABLEGWIN;

    return TRUE;
}

MS_BOOL Demo_BOOTVIDEO_GetBootvideoStatus(EN_BOOTVIDEO_STATE* peState)
{
    *peState = _geBootvideoState;
    return TRUE;
}

#endif
