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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
///
/// @file   Demo_certification.c
/// @author MStar Semiconductor Inc.
/// @brief  Generate Segmentation fault
///
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_HDMI_CTS_CERTIFICATION_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include <unistd.h>
#include <stdio.h>
#include "MsCommon.h"
//#include "msAPI_uart.h"
#include "drvMMIO.h"
#include "demo_uart.h"
#include "demo_xc.h"
#include "demo_osd.h"

#if (DEMO_AUDIO_TEST == 1)
#include "apiAUDIO.h"
#include "demo_audio.h"
#endif

#if (DEMO_AUDIO_MULTI_TEST == 1)
#include "apiAUDIO.h"
#include "demo_audio_multi.h"
#endif

#include "demo_cec.h"

//Api
#include "drvXC_IOPort.h"
#include "apiXC.h"
//#include "apiXC_EX.h"
//#include "apiXC_PCMonitor_EX.h"
//#include "apiXC_Ace_EX.h"
//#include "apiPNL_EX.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "apiVDEC_EX.h"
#include "apiACP.h"

#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiXC_EX.h"
#include "apiPNL_EX.h"
#include "apiXC_PCMonitor_EX.h"
#include "apiXC_Ace_EX.h"
#else
#include "apiPNL.h"
#endif
//Drv
#include "drvTVEncoder.h"
#include "drvMVOP.h"
#include "drvPQ.h"
#include "Panel.h"
//msAPI
#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#else
#include "msAPI_XC.h"
#endif

//#include "demo_dualav.h"
#include "demo_eeprom.h"
#include "demo_certification.h"
#include "demo_utility.h"
//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
#define CERT_DBG 1

#if (CERT_DBG == 0)
#define cert_info while(0) {}
#else
#define cert_info printf
#endif
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
extern MS_S32 gs32MstarCachedPoolID;
extern MS_S32 gs32MstarNonCachedPoolID;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define CERTIFICATION_EXIT 'q'
#define CERTIFICATION_EXIT_WITHOUT_TIMING_RESET 'n'
#define CERTIFICATION_EXIT_RESOLUTION_RESET 'r'
#define UARTCOLOR_NONE "\033[m"
#define UARTCOLOR_RED "\033[0;32;31m"
#define UARTCOLOR_LIGHT_RED "\033[1;31m"
#define UARTCOLOR_GREEN "\033[0;32;32m"
#define UARTCOLOR_LIGHT_GREEN "\033[1;32m"
#define UARTCOLOR_BLUE "\033[0;32;34m"
#define UARTCOLOR_LIGHT_BLUE "\033[1;34m"
#define UARTCOLOR_DARY_GRAY "\033[1;30m"
#define UARTCOLOR_CYAN "\033[0;36m"
#define UARTCOLOR_LIGHT_CYAN "\033[1;36m"
#define UARTCOLOR_PURPLE "\033[0;35m"
#define UARTCOLOR_LIGHT_PURPLE "\033[1;35m"
#define UARTCOLOR_BROWN "\033[0;33m"
#define UARTCOLOR_YELLOW "\033[1;33m"
#define UARTCOLOR_LIGHT_GRAY "\033[0;37m"
#define UARTCOLOR_WHITE "\033[1;37m"

#define REG16_NPM(addr)         *((volatile MS_U16*)(u32NonPmBase+ (addr)))
#define REG16_PM(addr )         *((volatile MS_U16*)(u32PmBase+ (addr)))

#define MAX_CMD_SIZE   16 //file size < 100KB

typedef enum
{
    E_CERTIFICATION_LAYER0=0,
    E_CERTIFICATION_LAYER1=1,
    E_CERTIFICATION_LAYER2=2,
    E_CERTIFICATION_LAYER3=3,
    E_CERTIFICATION_LAYER4=4,
    E_CERTIFICATION_LAYER5=5,
    E_CERTIFICATION_LAYERE=0xF,
}EN_CERTIFICATION_LAYER;

typedef enum{
    E_CERTIFICATION_INVALID=0xFF,
    E_CERTIFICATION_HDMI_DVI=1,
    E_CERTIFICATION_HDCP,
    E_CERTIFICATION_CEC ,
    E_CERTIFICATION_EDID,
    E_CERTIFICATION_DBG ,
}EN_CERTIFICATION_TYPE;

typedef enum{
    E_CERTIFICATION_SOURCE_INVALID=0xFF,
    E_CERTIFICATION_SOURCE_NORMAL=1,
    E_CERTIFICATION_SOURCE_3D=2,
    E_CERTIFICATION_SOURCE_4K2K=3,
    E_CERTIFICATION_SOURCE_HDMISETTING=4,
}EN_CERTIFICATION_TIMING_SOURCE_TYPE;

typedef enum{
  E_CERTIFICATION_HDMISETTING_INVALID=0xFF,
  E_CERTIFICATION_HDMISETTING_TXMODE=1,
  E_CERTIFICATION_HDMISETTING_COLORSPACE,
}EN_CERTIFICATION_HDMISETTING_TYPE;

typedef enum{
    E_RESOLUTION_640x480p_60Hz_4_3_24bit = 1,
    E_RESOLUTION_640x480p_60Hz_4_3_30bit,
    E_RESOLUTION_640x480p_60Hz_4_3_36bit,
    E_RESOLUTION_720x480p_60Hz_4_3_24bit,
    E_RESOLUTION_720x480p_60Hz_4_3_30bit,
    E_RESOLUTION_720x480p_60Hz_4_3_36bit,
    E_RESOLUTION_720x480p_60Hz_16_9_24bit,
    E_RESOLUTION_720x480p_60Hz_16_9_30bit,
    E_RESOLUTION_720x480p_60Hz_16_9_36bit,
    E_RESOLUTION_1280x720p_60Hz_16_9_24bit,
    E_RESOLUTION_1280x720p_60Hz_16_9_30bit,
    E_RESOLUTION_1280x720p_60Hz_16_9_36bit,
    E_RESOLUTION_1920x1080i_60Hz_16_9_24bit,
    E_RESOLUTION_1920x1080i_60Hz_16_9_30bit,
    E_RESOLUTION_1920x1080i_60Hz_16_9_36bit,
    E_RESOLUTION_720x480i_60Hz_4_3_24bit,
    E_RESOLUTION_720x480i_60Hz_4_3_30bit,
    E_RESOLUTION_720x480i_60Hz_4_3_36bit,
    E_RESOLUTION_720x480i_60Hz_16_9_24bit,
    E_RESOLUTION_720x480i_60Hz_16_9_30bit,
    E_RESOLUTION_720x480i_60Hz_16_9_36bit,
    E_RESOLUTION_1920x1080p_60Hz_16_9_24bit,
    E_RESOLUTION_1920x1080p_60Hz_16_9_30bit,
    E_RESOLUTION_1920x1080p_60Hz_16_9_36bit,
    E_RESOLUTION_720x576p_50Hz_4_3_24bit,
    E_RESOLUTION_720x576p_50Hz_4_3_30bit,
    E_RESOLUTION_720x576p_50Hz_4_3_36bit,
    E_RESOLUTION_720x576p_50Hz_16_9_24bit,
    E_RESOLUTION_720x576p_50Hz_16_9_30bit,
    E_RESOLUTION_720x576p_50Hz_16_9_36bit,
    E_RESOLUTION_1280x720p_50Hz_16_9_24bit,
    E_RESOLUTION_1280x720p_50Hz_16_9_30bit,
    E_RESOLUTION_1280x720p_50Hz_16_9_36bit,
    E_RESOLUTION_1920x1080i_50Hz_16_9_24bit,
    E_RESOLUTION_1920x1080i_50Hz_16_9_30bit,
    E_RESOLUTION_1920x1080i_50Hz_16_9_36bit,
    E_RESOLUTION_720x576i_50Hz_4_3_24bit,
    E_RESOLUTION_720x576i_50Hz_4_3_30bit,
    E_RESOLUTION_720x576i_50Hz_4_3_36bit,
    E_RESOLUTION_720x576i_50Hz_16_9_24bit,
    E_RESOLUTION_720x576i_50Hz_16_9_30bit,
    E_RESOLUTION_720x576i_50Hz_16_9_36bit,
    E_RESOLUTION_1920x1080p_50Hz_16_9_24bit,
    E_RESOLUTION_1920x1080p_50Hz_16_9_30bit,
    E_RESOLUTION_1920x1080p_50Hz_16_9_36bit,
    E_RESOLUTION_3840x2160p_24Hz_16_9_24bit,
    E_RESOLUTION_3840x2160p_24Hz_16_9_30bit,
    E_RESOLUTION_3840x2160p_24Hz_16_9_36bit,
    E_RESOLUTION_3840x2160p_24Hz_16_9_48bit,
    E_RESOLUTION_3840x2160p_25Hz_16_9_24bit,
    E_RESOLUTION_3840x2160p_25Hz_16_9_30bit,
    E_RESOLUTION_3840x2160p_25Hz_16_9_36bit,
    E_RESOLUTION_3840x2160p_25Hz_16_9_48bit,
    E_RESOLUTION_3840x2160p_30Hz_16_9_24bit,
    E_RESOLUTION_3840x2160p_30Hz_16_9_30bit,
    E_RESOLUTION_3840x2160p_30Hz_16_9_36bit,
    E_RESOLUTION_3840x2160p_30Hz_16_9_48bit,
    E_RESOLUTION_3840x2160p_50Hz_16_9_24bit,
    E_RESOLUTION_3840x2160p_50Hz_16_9_30bit,
    E_RESOLUTION_3840x2160p_50Hz_16_9_36bit,
    E_RESOLUTION_3840x2160p_50Hz_16_9_48bit,
    E_RESOLUTION_3840x2160p_60Hz_16_9_24bit,
    E_RESOLUTION_3840x2160p_60Hz_16_9_30bit,
    E_RESOLUTION_3840x2160p_60Hz_16_9_36bit,
    E_RESOLUTION_3840x2160p_60Hz_16_9_48bit,
    E_RESOLUTION_4096x2160p_24Hz_16_9_24bit,
    E_RESOLUTION_4096x2160p_24Hz_16_9_30bit,
    E_RESOLUTION_4096x2160p_24Hz_16_9_36bit,
    E_RESOLUTION_4096x2160p_24Hz_16_9_48bit,
    E_RESOLUTION_4096x2160p_25Hz_16_9_24bit,
    E_RESOLUTION_4096x2160p_25Hz_16_9_30bit,
    E_RESOLUTION_4096x2160p_25Hz_16_9_36bit,
    E_RESOLUTION_4096x2160p_25Hz_16_9_48bit,
    E_RESOLUTION_4096x2160p_30Hz_16_9_24bit,
    E_RESOLUTION_4096x2160p_30Hz_16_9_30bit,
    E_RESOLUTION_4096x2160p_30Hz_16_9_36bit,
    E_RESOLUTION_4096x2160p_30Hz_16_9_48bit,
    E_RESOLUTION_4096x2160p_50Hz_16_9_24bit,
    E_RESOLUTION_4096x2160p_50Hz_16_9_30bit,
    E_RESOLUTION_4096x2160p_50Hz_16_9_36bit,
    E_RESOLUTION_4096x2160p_50Hz_16_9_48bit,
    E_RESOLUTION_4096x2160p_60Hz_16_9_24bit,
    E_RESOLUTION_4096x2160p_60Hz_16_9_30bit,
    E_RESOLUTION_4096x2160p_60Hz_16_9_36bit,
    E_RESOLUTION_4096x2160p_60Hz_16_9_48bit,
}EN_CERTIFICATION_RESOLUTION;

typedef enum{
  E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_FRAME_PACKING=1,
  E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_FRAME_PACKING,
  E_3D_RESOLUTION_1280x720p50Hz_FRAME_PACKING,
  E_3D_RESOLUTION_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE,
  E_3D_RESOLUTION_1920x1080i50Hz_SIDE_BY_SIDE,
  E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_TOP_BOTTOM,
  E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_TOP_BOTTOM,
  E_3D_RESOLUTION_1280x720p50Hz_TOP_BOTTOM, 
  E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE, //Primary
  E_3D_RESOLUTION_1280x720p50Hz_SIDE_BY_SIDE,
  E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE,
  E_3D_RESOLUTION_1920x1080p59d94Hz_60Hz_TOP_BOTTOM,
  E_3D_RESOLUTION_1920x1080p50Hz_TOP_BOTTOM,
  E_3D_RESOLUTION_1920x1080p29d97Hz_30Hz_TOP_BOTTOM,
}EN_CERTIFICATION_3DRESOLUTION;

typedef enum{
  E_CERT_CEC_ACTIVE_SOURCE            =1,
  E_CERT_CEC_INACTIVE_SOURCE        ,
  E_CERT_CEC_SEND_STANDBY            ,
  E_CERT_CEC_SWITCH_POS1                ,
  E_CERT_CEC_SWITCH_POS2            ,
  E_CERT_CEC_RESET,
}EN_CERTIFICATION_CECITEM;

typedef enum{
  E_CERT_RGB444=1,
  E_CERT_YCBCR422,
  E_CERT_YCBCR444,
  E_CERT_YCBCR420,
}EN_CERTIFICATION_OUTPUTCOLORSPACE;

typedef enum{
  E_CERT_DVI=1,
  E_CERT_HDMI,
}EN_CERTIFICATION_OUTTYPE;

typedef enum{
    E_CERTIFICATION_DBG_ON =1,
    E_CERTIFICATION_DBG_OFF=2,
}EN_CERTIFICATION_DBG_TYPE;

typedef enum{
    E_CERTIFICATION_HDCP_SOURCE =1,
    E_CERTIFICATION_HDCP_REPEATER=2,
}EN_CERTIFICATION_HDCP_ITEM;

typedef enum{
    E_CERTIFICATION_EDID_ON =1,
    E_CERTIFICATION_EDID_OFF=2,
}EN_CERTIFICATION_EDID_TYPE;

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _certFun
{
    MS_U32 u8Enum;
    MS_BOOL (*func_ptr)(MS_U32 u32Selection);
} ST_DemoCertFun,*pSTDemoCertFun;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//static MS_U32 u32NonPmBase = 0;
//static MS_U32 u32PmBase = 0;

static MS_BOOL _bXC_Init = FALSE;
//static Task_Info _Demo_HDMI_Cert_Task = {-1, E_TASK_PRI_MEDIUM, NULL, 4096, "Cert Mon Task"};

static MS_U32 u32Layer = E_CERTIFICATION_LAYER0;

static MS_U8 u8TestItem = E_CERTIFICATION_INVALID;
//static E_CERTIFICATION_TIMING_SOURCE_TYPE srctiming = E_CERTIFICATION_SOURCE_INVALID;

static MS_BOOL _bEDID = FALSE;
static MS_U8 _u8Edid[128];

static MS_U8 _u8HDMI_TEST_TYPE;
static HDMITX_VIDEO_TIMING _3DTiming = HDMITX_RES_MAX;

static MS_BOOL _bFinished3DSetting = FALSE;

#if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
static MS_U32 _eOri3DTiming = HDMITX_RES_MAX;
//static MS_BOOL _b3D = 0xFF;
#endif

#if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
#include "drvXC_HDMI_if_EX.h"
#define TX_HPD_TASK_SIZE (4096)
static Task_Info _Demo_Mon_HDMI_CertTask = {-1, E_TASK_PRI_MEDIUM, NULL, 4096, "Demo Cert Task"};
static MS_S32       _s32TxHPD_Event = -1;
static MS_U8        _u8TxHPDTaskStack[TX_HPD_TASK_SIZE];
static Task_Info    _s32TxHPD_Task = {-1, E_TASK_PRI_MEDIUM, (void *)_u8TxHPDTaskStack, TX_HPD_TASK_SIZE, "TxHPD Rec Task"};
#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void _Demo_Certification_HDMIConfiguration(void)
{
    //HDMITX_EDID_3D_STRUCTURE_ALL st3DStructure;
    //MSAPI_XC_DEVICE_ID stXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
    if(u8TestItem == E_CERTIFICATION_HDMI_DVI)
    {
        if(_3DTiming == 9)
        {
            MApi_HDMITx_SetTMDSOnOff(FALSE);
            printf("TUrn Off TMDS\n");
        }

    }
}

char *_Demo_Certification_3D2String(E_MSAPI_XC_3D_OUTPUT_MODE eMode)
{
    switch(eMode)
    {
        case E_MSAPI_XC_3D_OUTPUT_MODE_NONE:
            return ("NONE 3D");
            break;

        case E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM:
            return ("TOP BOTTOM");
            break;
        case E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING:
            return ("FRAME PACKING");
            break;
        case E_MSAPI_XC_3D_OUTPUT_LINE_ALTERNATIVE:
            return ("LINE ALTERNATIVE");
            break;
        case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF:
            return ("SIDE BY SIDE HALF");
            break;
        case E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_FULL:
            return ("SIDE BY SIDE FULL");
            break;
        default:
            break;
    }

    return ("UNKNOWN 3D");

}

char *_Demo_Certification_colorfmt2String(HDMITX_VIDEO_COLOR_FORMAT colorfmt)
{
    switch(colorfmt)
    {

        case HDMITX_VIDEO_COLOR_RGB444:
            return ("RGB444");
            break;
        case HDMITX_VIDEO_COLOR_YUV422:
            return ("YUV422");
            break;
        case HDMITX_VIDEO_COLOR_YUV444:
            return ("YUV444");
            break;
        default:
            break;
    }
    return ("unknown color format");
}

char *_Demo_Certification_xcRet2String(E_MSAPI_XC_RESULT ret)
{
    switch(ret)
    {
        case E_MSAPI_XC_NOT_SUPPORT:
            return ("Not Support");
            break;
        case E_MSAPI_XC_FAIL:
            return ("FAIL");
            break;
        case E_MSAPI_XC_OK:
            return ("Sucess");
            break;
        case E_MSAPI_XC_INVALID_PARAM:
            return ("Invalid Param");
            break;
        default:
            break;
    }
    return ("unknown return value");

}

char * _Demo_Certification_as2String(HDMITX_VIDEO_ASPECT_RATIO as)
{
    switch(as)
    {
        case HDMITX_VIDEO_AR_Reserved:
            return ("reserved");
        case HDMITX_VIDEO_AR_4_3:
            return ("4:3");
        case HDMITX_VIDEO_AR_16_9:
            return ("16:9");
        default:
            break;
    }
    return ("unknown as");
}

char * _Demo_Certification_Out2String(MS_U32 u32Out)
{
    switch(u32Out)
    {
        case E_OUTPUT_TIMING_720X480_60I:
            return ("720x480i@60Hz");
        case E_OUTPUT_TIMING_720X480_60P:
            return ("720X480p@60Hz");
        case E_OUTPUT_TIMING_720X576_50I:
            return ("720X576i@50Hz");
        case E_OUTPUT_TIMING_720X576_50P:
            return ("720X576p@50Hz");
        case E_OUTPUT_TIMING_1280X720_50P:
            return ("1280X720p@50Hz");
        case E_OUTPUT_TIMING_1280X720_60P:
            return ("1280X720p@60Hz");
        case E_OUTPUT_TIMING_1920X1080_50I:
            return ("1920X1080i@50Hz");
        case E_OUTPUT_TIMING_1920X1080_60I:
            return ("1920X1080i@60Hz");
        case E_OUTPUT_TIMING_1920X1080_24P:
            return ("1920X1080p@24Hz");
        case E_OUTPUT_TIMING_1920X1080_25P:
            return ("1920X1080p@25Hz");
        case E_OUTPUT_TIMING_1920X1080_30P:
            return ("1920X1080p@30Hz");
        case E_OUTPUT_TIMING_1920X1080_50P:
            return ("1920X1080p@50Hz");
        case E_OUTPUT_TIMING_1920X1080_60P:
            return ("1920X1080p@60Hz");
        case E_OUTPUT_TIMING_640X480_60P:
            return ("640X480p@60Hz");
        case E_OUTPUT_TIMING_3840X2160_24P:
            return ("3840X2160p@24Hz");
        case E_OUTPUT_TIMING_3840X2160_25P:
            return ("3840X2160p@25Hz");
        case E_OUTPUT_TIMING_3840X2160_30P:
            return ("3840X2160p@30Hz");
        case E_OUTPUT_TIMING_3840X2160_50P:
            return ("3840X2160p@50Hz");
        case E_OUTPUT_TIMING_3840X2160_60P:
            return ("3840X2160p@60Hz");
        case E_OUTPUT_TIMING_4096X2160_24P:
            return ("4096X2160p@24Hz");
        case E_OUTPUT_TIMING_4096X2160_25P:
            return ("4096X2160p@25Hz");
        case E_OUTPUT_TIMING_4096X2160_30P:
            return ("4096X2160p@30Hz");
        case E_OUTPUT_TIMING_4096X2160_50P:
            return ("4096X2160p@50Hz");
        case E_OUTPUT_TIMING_4096X2160_60P:
            return ("4096X2160p@60Hz");
#if 0
        case E_OUTPUT_TIMING_1920x2205P_3DFP_24Hz:
            return ("1920x2205p@24Hz 3D Frame Packet");
        case E_OUTPUT_TIMING_1280x1470P_3DFP_50Hz:
            return ("1280x1470p@50Hz 3D Frame Packet");
        case E_OUTPUT_TIMING_1280x1470P_3DFP_60Hz:
            return ("1280x1470p@60Hz 3D Frame Packet");
#endif
        default:
            break;
    }

    return ("unknown output resolution");
}

char * _Demo_Certification_Type2String( E_CERTIFICATION_TYPE etype)
{

    switch (etype)
    {
        case E_CERTIFICATION_HDMI_DVI:
            return ("HDMI/DVI");
            break;
        case E_CERTIFICATION_HDCP:
            return ("HDCP");
            break;
        case E_CERTIFICATION_CEC:
            return ("CEC");
            break;
        case E_CERTIFICATION_EDID:
            return ("EDID");
            break;
        case E_CERTIFICATION_DBG:
            return ("DBG");
            break;

        default:
            break;
    }

    return ("unknown test item");



}

char * _Demo_Certification_CD2string(MS_U32 cd)
{
    switch(cd)
    {
        case E_MSAPI_XC_HDMITX_CD_8BITS:
            return ("24 bit");
        case E_MSAPI_XC_HDMITX_CD_10BITS:
            return ("30 bit");
        case E_MSAPI_XC_HDMITX_CD_12BITS:
            return ("36 bit");
        case E_MSAPI_XC_HDMITX_CD_16BITS:
            return ("48 bit");
        case E_MSAPI_XC_HDMITX_CD_AUTO:
            return ("auto");
        default:
            break;
    }

    return ("unknown bit");

}
char * _Demo_Certification_3Dresolution2string(E_CERTIFICATION_3DRESOLUTION eTiming)
{
    switch (eTiming)
    {
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_FRAME_PACKING:
            return ("1920x1080p23.98Hz/24Hz Frame Packing");
            break;
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_FRAME_PACKING:
            return ("1280x720p59.94Hz/60Hz Frame Packing");
            break;
        case E_3D_RESOLUTION_1280x720p50Hz_FRAME_PACKING:
            return ("1280x720p50Hz Frame Packing");
            break;
        case E_3D_RESOLUTION_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE:
            return ("1920x1080i59.94/60Hz Side by Side(Half)");
            break;
        case E_3D_RESOLUTION_1920x1080i50Hz_SIDE_BY_SIDE:
            return ("1920x1080i50Hz Side by Side(Half)");
            break;
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_TOP_BOTTOM:
            return ("1920x1080p23.98Hz/24Hz Top and Bottom");
            break;
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_TOP_BOTTOM:
            return ("1280x720p59.94Hz/60Hz Top and Bottom");
            break;
        case E_3D_RESOLUTION_1280x720p50Hz_TOP_BOTTOM:
            return ("1280x720p50Hz Top and Bottom");
            break;
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE:
            return ("1280x720p59.94Hz/60Hz Side by Side(Half)");
            break;
        case E_3D_RESOLUTION_1280x720p50Hz_SIDE_BY_SIDE:
            return ("1280x720p50Hz Side by Side(Half)");
            break;
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE:
            return ("1920x1080p23.98Hz/24Hz Side by Side(Half)");
            break;
        case E_3D_RESOLUTION_1920x1080p59d94Hz_60Hz_TOP_BOTTOM:
            return ("1280x720p59.94Hz/60Hz Top and Bottom");
            break;
        case E_3D_RESOLUTION_1920x1080p50Hz_TOP_BOTTOM:
            return ("1920x1080p50Hz Top and Bottom");
            break;
        case E_3D_RESOLUTION_1920x1080p29d97Hz_30Hz_TOP_BOTTOM:
            return ("1920x1080p29.97Hz/30Hz Top and Bottom");

 
        default:
            break;
    }

    return ("unknown 3D resolution");


}

char * _Demo_Certification_resolution2string(E_CERTIFICATION_RESOLUTION eTiming)
{
    switch (eTiming)
    {
        case E_RESOLUTION_640x480p_60Hz_4_3_24bit:
            return ("640x480p_60Hz_4_3_24bit    ");
            break;
        case E_RESOLUTION_640x480p_60Hz_4_3_30bit:
            return ("640x480p_60Hz_4_3_30bit    ");
            break;
        case E_RESOLUTION_640x480p_60Hz_4_3_36bit:
            return ("640x480p_60Hz_4_3_36bit    ");
            break;
        case E_RESOLUTION_720x480p_60Hz_4_3_24bit:
            return ("720x480p_60Hz_4_3_24bit    ");
            break;
        case E_RESOLUTION_720x480p_60Hz_4_3_30bit:
            return ("720x480p_60Hz_4_3_30bit    ");
            break;
        case E_RESOLUTION_720x480p_60Hz_4_3_36bit:
            return ("720x480p_60Hz_4_3_36bit    ");
            break;
        case E_RESOLUTION_720x480p_60Hz_16_9_24bit:
            return ("720x480p_60Hz_16_9_24bit   ");
            break;
        case E_RESOLUTION_720x480p_60Hz_16_9_30bit:
            return ("720x480p_60Hz_16_9_30bit   ");
            break;
        case E_RESOLUTION_720x480p_60Hz_16_9_36bit:
            return ("720x480p_60Hz_16_9_36bit   ");
            break;
        case E_RESOLUTION_720x480i_60Hz_4_3_24bit:
            return ("720x480i_60Hz_4_3_24bit    ");
            break;
        case E_RESOLUTION_720x480i_60Hz_4_3_30bit:
            return ("720x480i_60Hz_4_3_30bit    ");
            break;
        case E_RESOLUTION_720x480i_60Hz_4_3_36bit:
            return ("720x480i_60Hz_4_3_36bit    ");
            break;
        case E_RESOLUTION_720x480i_60Hz_16_9_24bit:
            return ("720x480i_60Hz_16_9_24bit   ");
            break;
        case E_RESOLUTION_720x480i_60Hz_16_9_30bit:
            return ("720x480i_60Hz_16_9_30bit   ");
            break;
        case E_RESOLUTION_720x480i_60Hz_16_9_36bit:
            return ("720x480i_60Hz_16_9_36bit   ");
            break;
        case E_RESOLUTION_1280x720p_60Hz_16_9_24bit:
            return ("1280x720p_60Hz_16_9_24bit  ");
            break;
        case E_RESOLUTION_1280x720p_60Hz_16_9_30bit:
            return ("1280x720p_60Hz_16_9_30bit  ");
            break;
        case E_RESOLUTION_1280x720p_60Hz_16_9_36bit:
            return ("1280x720p_60Hz_16_9_36bit  ");
            break;
        case E_RESOLUTION_1920x1080i_60Hz_16_9_24bit:
            return ("1920x1080i_60Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_1920x1080i_60Hz_16_9_30bit:
            return ("1920x1080i_60Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_1920x1080i_60Hz_16_9_36bit:
            return ("1920x1080i_60Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_1920x1080p_60Hz_16_9_24bit:
            return ("1920x1080p_60Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_1920x1080p_60Hz_16_9_30bit:
            return ("1920x1080p_60Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_1920x1080p_60Hz_16_9_36bit:
            return ("1920x1080p_60Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_720x576p_50Hz_4_3_24bit:
            return ("720x576p_50Hz_4_3_24bit    ");
            break;
        case E_RESOLUTION_720x576p_50Hz_4_3_30bit:
            return ("720x576p_50Hz_4_3_30bit    ");
            break;
        case E_RESOLUTION_720x576p_50Hz_4_3_36bit:
            return ("720x576p_50Hz_4_3_36bit    ");
            break;
        case E_RESOLUTION_720x576p_50Hz_16_9_24bit:
            return ("720x576p_50Hz_16_9_24bit   ");
            break;
        case E_RESOLUTION_720x576p_50Hz_16_9_30bit:
            return ("720x576p_50Hz_16_9_30bit   ");
            break;
        case E_RESOLUTION_720x576p_50Hz_16_9_36bit:
            return ("720x576p_50Hz_16_9_36bit   ");
            break;
        case E_RESOLUTION_1280x720p_50Hz_16_9_24bit:
            return ("1280x720p_50Hz_16_9_24bit  ");
            break;
        case E_RESOLUTION_1280x720p_50Hz_16_9_30bit:
            return ("1280x720p_50Hz_16_9_30bit  ");
            break;
        case E_RESOLUTION_1280x720p_50Hz_16_9_36bit:
            return ("1280x720p_50Hz_16_9_36bit  ");
            break;
        case E_RESOLUTION_1920x1080i_50Hz_16_9_24bit:
            return ("1920x1080i_50Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_1920x1080i_50Hz_16_9_30bit:
            return ("1920x1080i_50Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_1920x1080i_50Hz_16_9_36bit:
            return ("1920x1080i_50Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_720x576i_50Hz_4_3_24bit:
            return ("720x576i_50Hz_4_3_24bit    ");
            break;
        case E_RESOLUTION_720x576i_50Hz_4_3_30bit:
            return ("720x576i_50Hz_4_3_30bit    ");
            break;
        case E_RESOLUTION_720x576i_50Hz_4_3_36bit:
            return ("720x576i_50Hz_4_3_36bit    ");
            break;
        case E_RESOLUTION_720x576i_50Hz_16_9_24bit:
            return ("720x576i_50Hz_16_9_24bit   ");
            break;
        case E_RESOLUTION_720x576i_50Hz_16_9_30bit:
            return ("720x576i_50Hz_16_9_30bit   ");
            break;
        case E_RESOLUTION_720x576i_50Hz_16_9_36bit:
            return ("720x576i_50Hz_16_9_36bit   ");
            break;
        case E_RESOLUTION_1920x1080p_50Hz_16_9_24bit:
            return ("1920x1080p_50Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_1920x1080p_50Hz_16_9_30bit:
            return ("1920x1080p_50Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_1920x1080p_50Hz_16_9_36bit:
            return ("1920x1080p_50Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_3840x2160p_24Hz_16_9_24bit:
            return ("3840x2160p_24Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_3840x2160p_24Hz_16_9_30bit:
            return ("3840x2160p_24Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_3840x2160p_24Hz_16_9_36bit:
            return ("3840x2160p_24Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_3840x2160p_24Hz_16_9_48bit:
            return ("3840x2160p_24Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_3840x2160p_25Hz_16_9_24bit:
            return ("3840x2160p_25Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_3840x2160p_25Hz_16_9_30bit:
            return ("3840x2160p_25Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_3840x2160p_25Hz_16_9_36bit:
            return ("3840x2160p_25Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_3840x2160p_25Hz_16_9_48bit:
            return ("3840x2160p_25Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_3840x2160p_30Hz_16_9_24bit:
            return ("3840x2160p_30Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_3840x2160p_30Hz_16_9_30bit:
            return ("3840x2160p_30Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_3840x2160p_30Hz_16_9_36bit:
            return ("3840x2160p_30Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_3840x2160p_30Hz_16_9_48bit:
            return ("3840x2160p_30Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_3840x2160p_50Hz_16_9_24bit:
            return ("3840x2160p_50Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_3840x2160p_50Hz_16_9_30bit:
            return ("3840x2160p_50Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_3840x2160p_50Hz_16_9_36bit:
            return ("3840x2160p_50Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_3840x2160p_50Hz_16_9_48bit:
            return ("3840x2160p_50Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_3840x2160p_60Hz_16_9_24bit:
            return ("3840x2160p_60Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_3840x2160p_60Hz_16_9_30bit:
            return ("3840x2160p_60Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_3840x2160p_60Hz_16_9_36bit:
            return ("3840x2160p_60Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_3840x2160p_60Hz_16_9_48bit:
            return ("3840x2160p_60Hz_16_9_48bit ");	
            break;
        case E_RESOLUTION_4096x2160p_24Hz_16_9_24bit:
            return ("4096x2160p_24Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_4096x2160p_24Hz_16_9_30bit:
            return ("4096x2160p_24Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_4096x2160p_24Hz_16_9_36bit:
            return ("4096x2160p_24Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_4096x2160p_24Hz_16_9_48bit:
            return ("4096x2160p_24Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_4096x2160p_25Hz_16_9_24bit:
            return ("4096x2160p_25Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_4096x2160p_25Hz_16_9_30bit:
            return ("4096x2160p_25Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_4096x2160p_25Hz_16_9_36bit:
            return ("4096x2160p_25Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_4096x2160p_25Hz_16_9_48bit:
            return ("4096x2160p_25Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_4096x2160p_30Hz_16_9_24bit:
            return ("4096x2160p_30Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_4096x2160p_30Hz_16_9_30bit:
            return ("4096x2160p_30Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_4096x2160p_30Hz_16_9_36bit:
            return ("4096x2160p_30Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_4096x2160p_30Hz_16_9_48bit:
            return ("4096x2160p_30Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_4096x2160p_50Hz_16_9_24bit:
            return ("4096x2160p_50Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_4096x2160p_50Hz_16_9_30bit:
            return ("4096x2160p_50Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_4096x2160p_50Hz_16_9_36bit:
            return ("4096x2160p_50Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_4096x2160p_50Hz_16_9_48bit:
            return ("4096x2160p_50Hz_16_9_48bit ");
            break;
        case E_RESOLUTION_4096x2160p_60Hz_16_9_24bit:
            return ("4096x2160p_60Hz_16_9_24bit ");
            break;
        case E_RESOLUTION_4096x2160p_60Hz_16_9_30bit:
            return ("4096x2160p_60Hz_16_9_30bit ");
            break;
        case E_RESOLUTION_4096x2160p_60Hz_16_9_36bit:
            return ("4096x2160p_60Hz_16_9_36bit ");
            break;
        case E_RESOLUTION_4096x2160p_60Hz_16_9_48bit:
            return ("4096x2160p_60Hz_16_9_48bit ");	
            break;
        default:
            break;
    }

    return ("unknown normal resolution");
}

char * _Demo_Certification_Txmode2string(E_CERTIFICATION_OUTTYPE eTxMode)
{
    switch (eTxMode)
    {
        case E_CERT_DVI:
            return ("DVI mode  ");
            break;
        case E_CERT_HDMI:
            return ("HDMI mode  ");
            break;
        default:
            break;
    }
    return ("unknown Tx mode");
}

char * _Demo_Certification_ColorSpace2string(E_CERTIFICATION_OUTPUTCOLORSPACE eColorSpace)
{
    switch (eColorSpace)
    {
        case E_CERT_YCBCR444:
            return ("YCbCr 444  ");
            break;
        case E_CERT_YCBCR422:
            return ("YCbCr 422  ");
            break;
        case E_CERT_RGB444:
            return ("RGB 444");
            break;
        case E_CERT_YCBCR420:
            return ("YCbCr 420");
            break;	
        default:
            break;
    }
    return ("unknown Color Space");
}

E_ASPECT_RATIO_TYPE _Demo_Certification_GetAS(HDMITX_VIDEO_ASPECT_RATIO eAS)
{
    if(eAS==HDMITX_VIDEO_AR_4_3)
        return E_ASPECT_RATIO_4_3;
    else if(eAS==HDMITX_VIDEO_AR_16_9)
        return E_ASPECT_RATIO_16_9;
    return E_ASPECT_RATIO_NORMAL;
}


HDMITX_VIDEO_ASPECT_RATIO _Demo_Certification_GetAspect(MS_U32 timing)
{
    HDMITX_VIDEO_ASPECT_RATIO ar = HDMITX_VIDEO_AR_Reserved;
    switch(timing)
    {

        default:
        case E_RESOLUTION_640x480p_60Hz_4_3_30bit:
        case E_RESOLUTION_640x480p_60Hz_4_3_36bit:
        case E_RESOLUTION_640x480p_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_36bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_36bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_24bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_30bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_36bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_24bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_30bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_36bit:

            ar = HDMITX_VIDEO_AR_4_3;
            break;

        case E_RESOLUTION_720x480p_60Hz_16_9_30bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_36bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_24bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_24bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_30bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_36bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_24bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_30bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_36bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_24bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_30bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_30bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_30bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_48bit:
            ar = HDMITX_VIDEO_AR_16_9;
            break;

    }

    return ar;
}

E_MSAPI_XC_3D_OUTPUT_MODE _Demo_Certification_Get3DMode(MS_U32 timing)
{

    E_MSAPI_XC_3D_OUTPUT_MODE eMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE ;

    switch(timing)
    {
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_FRAME_PACKING:
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_FRAME_PACKING:
        case E_3D_RESOLUTION_1280x720p50Hz_FRAME_PACKING:
            eMode = E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING;
            break;
        case E_3D_RESOLUTION_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE:
        case E_3D_RESOLUTION_1920x1080i50Hz_SIDE_BY_SIDE:
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE:
        case E_3D_RESOLUTION_1280x720p50Hz_SIDE_BY_SIDE:
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE:
            eMode = E_MSAPI_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF;
            break;
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1280x720p50Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1920x1080p59d94Hz_60Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1920x1080p50Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1920x1080p29d97Hz_30Hz_TOP_BOTTOM:
            eMode = E_MSAPI_XC_3D_OUTPUT_TOP_BOTTOM;
            break;
        default:
            eMode = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
            break;
    }

    return eMode;
}

E_MSAPI_XC_OUTPUT_TIMING_TYPE _Demo_Certification_Get3DOutputTiming(MS_U32 timing)
{
    E_MSAPI_XC_OUTPUT_TIMING_TYPE eType = E_3D_RESOLUTION_1280x720p50Hz_FRAME_PACKING;
    switch(timing)
    {
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_FRAME_PACKING:
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE:
            eType = E_OUTPUT_TIMING_1920X1080_24P;
            _3DTiming = HDMITX_RES_1920x1080p_24Hz;
            break;
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_FRAME_PACKING:
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE:
            eType = E_OUTPUT_TIMING_1280X720_60P;
            _3DTiming = HDMITX_RES_1280x720p_60Hz;
            break;
        case E_3D_RESOLUTION_1280x720p50Hz_FRAME_PACKING:
        case E_3D_RESOLUTION_1280x720p50Hz_TOP_BOTTOM:
        case E_3D_RESOLUTION_1280x720p50Hz_SIDE_BY_SIDE:
            eType = E_OUTPUT_TIMING_1280X720_50P;
            _3DTiming = HDMITX_RES_1280x720p_50Hz;
            break;
        case E_3D_RESOLUTION_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE:
            eType = E_OUTPUT_TIMING_1920X1080_60I;
            _3DTiming = HDMITX_RES_1920x1080i_60Hz;
            break;
        case E_3D_RESOLUTION_1920x1080i50Hz_SIDE_BY_SIDE:
            eType = E_OUTPUT_TIMING_1920X1080_50I;
            _3DTiming = HDMITX_RES_1920x1080i_50Hz;
            break;
        case E_3D_RESOLUTION_1920x1080p59d94Hz_60Hz_TOP_BOTTOM:
            eType = E_OUTPUT_TIMING_1920X1080_60P;
            _3DTiming = HDMITX_RES_1920x1080p_60Hz;
            break;
        case E_3D_RESOLUTION_1920x1080p50Hz_TOP_BOTTOM:
            eType = E_OUTPUT_TIMING_1920X1080_50P;
            _3DTiming = HDMITX_RES_1920x1080p_50Hz;
            break;
        case E_3D_RESOLUTION_1920x1080p29d97Hz_30Hz_TOP_BOTTOM:
            eType = E_OUTPUT_TIMING_1920X1080_30P;
            _3DTiming = HDMITX_RES_1920x1080p_30Hz;
            break;
        default:
            break;
    }

    return eType;
}

E_OUTPUT_TIMING_TYPE _Demo_Certification_GetOutputTiming(MS_U32 timing)
{
    E_OUTPUT_TIMING_TYPE eType = 17;
    switch(timing)
    {

        case E_RESOLUTION_640x480p_60Hz_4_3_30bit:
        case E_RESOLUTION_640x480p_60Hz_4_3_36bit:
        case E_RESOLUTION_640x480p_60Hz_4_3_24bit:
            eType = E_OUTPUT_TIMING_640X480_60P;
            break;
        case E_RESOLUTION_720x480p_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_30bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_36bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_36bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_24bit:
            eType = E_OUTPUT_TIMING_720X480_60P;
            break;

        case E_RESOLUTION_720x480i_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_24bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_30bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_36bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_36bit:
            eType = E_OUTPUT_TIMING_720X480_60I;
            break;

        case E_RESOLUTION_720x576p_50Hz_4_3_24bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_24bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_30bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_30bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_36bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_36bit:
            eType = E_OUTPUT_TIMING_720X576_50P;
            break;

        case E_RESOLUTION_720x576i_50Hz_4_3_24bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_24bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_30bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_30bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_36bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_36bit:
            eType = E_OUTPUT_TIMING_720X576_50I;
            break;
            
        case E_RESOLUTION_1280x720p_60Hz_16_9_30bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_24bit:
            eType = E_OUTPUT_TIMING_1280X720_60P;
            break;
        case E_RESOLUTION_1280x720p_50Hz_16_9_30bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_24bit:
            eType = E_OUTPUT_TIMING_1280X720_50P;
            break;

          case E_RESOLUTION_1920x1080i_50Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_24bit:
            eType = E_MSAPI_XC_RES_1920x1080I_50Hz;
            break;
        case E_RESOLUTION_1920x1080i_60Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_30bit:
            eType = E_OUTPUT_TIMING_1920X1080_60I;
            break;
        case E_RESOLUTION_1920x1080p_60Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_36bit:
            eType = E_OUTPUT_TIMING_1920X1080_60P;
            break;
        case E_RESOLUTION_1920x1080p_50Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_36bit:
            eType = E_OUTPUT_TIMING_1920X1080_50P;
            break;
        case E_RESOLUTION_3840x2160p_24Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_3840X2160_24P;
            break;
        case E_RESOLUTION_3840x2160p_25Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_3840X2160_25P;
            break;
		case E_RESOLUTION_3840x2160p_30Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_36bit:
		case E_RESOLUTION_3840x2160p_30Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_3840X2160_30P;
            break;
		case E_RESOLUTION_3840x2160p_50Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_36bit:
		case E_RESOLUTION_3840x2160p_50Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_3840X2160_50P;
            break;
		case E_RESOLUTION_3840x2160p_60Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_36bit:
		case E_RESOLUTION_3840x2160p_60Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_3840X2160_60P;
            break;
        case E_RESOLUTION_4096x2160p_24Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_4096X2160_24P;
            break;
        case E_RESOLUTION_4096x2160p_25Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_4096X2160_25P;
            break;
		case E_RESOLUTION_4096x2160p_30Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_36bit:
		case E_RESOLUTION_4096x2160p_30Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_4096X2160_30P;
            break;
		case E_RESOLUTION_4096x2160p_50Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_36bit:
		case E_RESOLUTION_4096x2160p_50Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_4096X2160_50P;
            break;
		case E_RESOLUTION_4096x2160p_60Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_36bit:
		case E_RESOLUTION_4096x2160p_60Hz_16_9_48bit:
            eType = E_OUTPUT_TIMING_4096X2160_60P;
            break;	
        default:
            printf("unknown timing :%lu\n",timing);
            break;
    }
    return eType;

}


E_MSAPI_XC_HDMITX_CD_TYPE _Demo_Certification_GetColorDepth(MS_U32 timing)
{

    E_MSAPI_XC_HDMITX_CD_TYPE eType = E_MSAPI_XC_HDMITX_CD_8BITS;
    switch(timing)
    {
        case E_RESOLUTION_640x480p_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_30bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_30bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_30bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_30bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_30bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_30bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_30bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_30bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_30bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_30bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_30bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_30bit:
            eType = E_MSAPI_XC_HDMITX_CD_10BITS;
            break;
        case E_RESOLUTION_640x480p_60Hz_4_3_36bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_36bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_36bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_36bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_36bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_36bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_36bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_36bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_36bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_36bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_36bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_36bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_36bit:
            eType = E_MSAPI_XC_HDMITX_CD_12BITS;
            break;

        default:
        case E_RESOLUTION_640x480p_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480p_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480p_60Hz_16_9_24bit:
        case E_RESOLUTION_720x480i_60Hz_4_3_24bit:
        case E_RESOLUTION_720x480i_60Hz_16_9_24bit:
        case E_RESOLUTION_1280x720p_60Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080i_60Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080p_60Hz_16_9_24bit:
        case E_RESOLUTION_720x576p_50Hz_4_3_24bit:
        case E_RESOLUTION_720x576p_50Hz_16_9_24bit:
        case E_RESOLUTION_720x576i_50Hz_4_3_24bit:
        case E_RESOLUTION_720x576i_50Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080i_50Hz_16_9_24bit:
        case E_RESOLUTION_1920x1080p_50Hz_16_9_24bit:
        case E_RESOLUTION_1280x720p_50Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_24Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_24bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_24bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_24bit:
            eType = E_MSAPI_XC_HDMITX_CD_8BITS;
            break;
        case E_RESOLUTION_3840x2160p_24Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_25Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_30Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_50Hz_16_9_48bit:
        case E_RESOLUTION_3840x2160p_60Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_24Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_25Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_30Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_50Hz_16_9_48bit:
        case E_RESOLUTION_4096x2160p_60Hz_16_9_48bit:
            eType = E_MSAPI_XC_HDMITX_CD_16BITS;
            break;	
    }
    return eType;
}

MS_BOOL _Demo_Certification_CheckHDMITx(void)
{
    if(MApi_HDMITx_GetRxStatus()== TRUE)
    {

        MS_BOOL bHDMISupport = FALSE;
        if(MApi_HDMITx_EDID_HDMISupport(&bHDMISupport) == TRUE)
        //if(MApi_HDMITx_IsStable()==TRUE)
        {

            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief Set HDMI Tx output mode
/// @param[in] u32OutputMode a MS_U32 value of the following enum
///  typedef enum
///  0:   E_MSAPI_XC_HDMITX_OUTPUT_HDMI
///  1:   E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP
///  2:   E_MSAPI_XC_HDMITX_OUTPUT_DVI
///  3:   E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL _Demo_Certification_SetTxOutputMode(MS_BOOL bEnable, MS_U32 u32OutputMode)
{
    MApi_HDMITx_UnHDCPRxControl(E_UNHDCPRX_BLUE_SCREEN);
    MApi_HDMITx_HDCPRxFailControl(E_HDCPRXFail_BLUE_SCREEN);

    msAPI_XC_SetHDMITxMode((E_MSAPI_XC_HDMITX_OUTPUT_MODE)u32OutputMode);

    switch(u32OutputMode)
    {
        case E_MSAPI_XC_HDMITX_OUTPUT_HDMI:
            MApi_HDMITx_ForceHDMIOutputMode(bEnable, (HDMITX_OUTPUT_MODE)HDMITX_HDMI);
            MApi_HDMITx_ForceHDMIOutputColorFormat(bEnable, (HDMITX_VIDEO_COLOR_FORMAT)HDMITX_VIDEO_COLOR_YUV444);
            break;
        case E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP:
            MApi_HDMITx_ForceHDMIOutputMode(bEnable, (HDMITX_OUTPUT_MODE)HDMITX_HDMI_HDCP);
            MApi_HDMITx_ForceHDMIOutputColorFormat(bEnable, (HDMITX_VIDEO_COLOR_FORMAT)HDMITX_VIDEO_COLOR_YUV444);
            break;
        case E_MSAPI_XC_HDMITX_OUTPUT_DVI:
            MApi_HDMITx_ForceHDMIOutputMode(bEnable, (HDMITX_OUTPUT_MODE)HDMITX_DVI);
            MApi_HDMITx_ForceHDMIOutputColorFormat(bEnable, (HDMITX_VIDEO_COLOR_FORMAT)HDMITX_VIDEO_COLOR_RGB444);
            break;
        case E_MSAPI_XC_HDMITX_OUTPUT_DVI_HDCP:
            MApi_HDMITx_ForceHDMIOutputMode(bEnable, (HDMITX_OUTPUT_MODE)HDMITX_DVI_HDCP);
            MApi_HDMITx_ForceHDMIOutputColorFormat(bEnable, (HDMITX_VIDEO_COLOR_FORMAT)HDMITX_VIDEO_COLOR_RGB444);
            break;
    }

    printf("Switch to %s%s mode!\n", (u32OutputMode & 0x2)?"DVI":"HDMI",
                                     (u32OutputMode & 0x1)?"_HDCP":"");
    return TRUE;
}

HDMITX_VIDEO_COLOR_FORMAT _Demo_Certification_getOutColorFromEdid(void)
{
    MS_U16 i = 0;
    MS_U8 block_num = 0,current, offset;// , k;
    //MS_U8 descriptor, tag, len;
    HDMITX_VIDEO_COLOR_FORMAT output_color= HDMITX_VIDEO_COLOR_RGB444;
    for(i = 0; i<2 ;i++)
    {
        cert_info("%d %d\n",i,i*128);
        if(MApi_HDMITx_GetEDIDData(&_u8Edid[0], i) == TRUE)
        {
            if(i == 0)
            {
                block_num = _u8Edid[0x7E];
                cert_info("block_num: 0x%02u \n",block_num);
            }
            else if(i == 1)
            {
                printf("_u8Edid: %02X %02X\n",_u8Edid[0],_u8Edid[1]);
                if((_u8Edid[0]==0x02) && (_u8Edid[1]<=0x04))
                {
                    offset = _u8Edid[0x02];
                    cert_info("offset: %8u\n",offset);
                    if (offset == 0xFF)
                    {
                        continue;
                    }
                    if (offset < 0x05)
                    {
                        continue;
                    }

                    current = 0x04;
                    cert_info("current:%8u ,offset: %8u   0x%X\n",current,offset,_u8Edid[0x03] & 0x30);

                    if(_u8Edid[0x03] & 0x20)
                    {
                            output_color = HDMITX_VIDEO_COLOR_YUV444;
                    }
                    else if(_u8Edid[0x03] & 0x10)
                    {
                        output_color = HDMITX_VIDEO_COLOR_YUV422;
                    }

                }
            }


        }
    }
    cert_info("color fmt: 0x%02X \n",output_color);
    return output_color;
}

#if (HDMI_CTS_CERTIFICATION_HDCP_SOURCE || HDMI_CTS_CERTIFICATION_HDCP_REPEATER)
MS_BOOL Demo_HDMI_InitHDCP(MS_BOOL *bHDCP)
{
    E_MSAPI_XC_RESULT ret ;

    if(*bHDCP)
    {
        ret = msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_HDMI_HDCP);
    }
    else
    {
        ret = msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_HDMI);
    }

    printf("ret:%d\n",ret);
    return (ret==E_MSAPI_XC_OK)?TRUE:FALSE;
}
#endif

void Demo_Certification_Init(void)
{

    printf("HDMI/HDCP Certification:\n");
    printf("\t(%d) HDMI/DVI \n",E_CERTIFICATION_HDMI_DVI);
    printf("\t(%d) HDCP \n",E_CERTIFICATION_HDCP);
    printf("\t(%d) CEC \n",E_CERTIFICATION_CEC);
    printf("\t(%d) EDID \n",E_CERTIFICATION_EDID);
    printf("\t(%d) DBG \n",E_CERTIFICATION_DBG);
    printf("\t(%c) Exit \n",CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER0;

}




void Demo_Certification_SelectTimingSourceTest(void)
{
    printf("Source Timing:\n");
    printf("\t(%d) Normal\n",E_CERTIFICATION_SOURCE_NORMAL);
    printf("\t(%d) 3D \n",E_CERTIFICATION_SOURCE_3D);
    printf("\t(%d) 4K2K \n",E_CERTIFICATION_SOURCE_4K2K);
    printf("\t(%d) HDMI Setting \n",E_CERTIFICATION_SOURCE_HDMISETTING);
    printf("\t(%c) Resolution Reset \n",CERTIFICATION_EXIT_RESOLUTION_RESET);
    printf("\t(%c) Exit\n",CERTIFICATION_EXIT);
    printf("Please select Test item[1/2/3]:");
    u32Layer= E_CERTIFICATION_LAYER1;

}

void Demo_Certification_SelectCECTest(void)
{
    printf("Test Below Item:\n");
    printf("\t(%d) Set Active Source\n"        ,E_CERT_CEC_ACTIVE_SOURCE);
    printf("\t(%d) Set Inactive Source \n"        ,E_CERT_CEC_INACTIVE_SOURCE);
    printf("\t(%d) Set StandBy Mode\n"        ,E_CERT_CEC_SEND_STANDBY);
    printf("\t(%d) Switch to Position 1\n"        ,E_CERT_CEC_SWITCH_POS1);
    printf("\t(%d) Switch to Position 2\n"        ,E_CERT_CEC_SWITCH_POS2);
    printf("\t(%d) Reset\n"                        ,E_CERT_CEC_RESET);
    printf("\t(%c) Exit\n"                ,CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER1;
}



void Demo_Certification_SelectHDCPTest(void)
{
    printf("Test Below Item:\n");
    printf("\t(%d) 1A 1B\n"            ,E_CERTIFICATION_HDCP_SOURCE);
    printf("\t(%d) 3A 3B 3C \n"        ,E_CERTIFICATION_HDCP_REPEATER);
    printf("\t(%c) Exit\n"                ,CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER1;
}


void Demo_Certification_SelectOutputColorSpace(void)
{
    printf("Output Color Space Selection:   \n");
    printf("\t(%d) RGB 444\n",             E_CERT_RGB444);
    printf("\t(%d) YCbCR444\n",             E_CERT_YCBCR444);
    printf("\t(%d) YCbCR422\n",             E_CERT_YCBCR422);
    printf("\t(%d) YCbCR420\n",             E_CERT_YCBCR420);
    printf("\t(%c) Exit\n",            CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER3;

}


void Demo_Certification_SelectTiming(void)
{

    printf("Timing Selection:   Only YCbCR444\n");
    printf("\t(%2d) 640x480p60Hz 4:3 24bit\n",             E_RESOLUTION_640x480p_60Hz_4_3_24bit);
    printf("\t(%2d) 640x480p60Hz 4:3 30bit\n",             E_RESOLUTION_640x480p_60Hz_4_3_30bit);
    printf("\t(%2d) 640x480p60Hz 4:3 36bit\n",             E_RESOLUTION_640x480p_60Hz_4_3_36bit);
    printf("\t(%2d) 720x480p60Hz 4:3 24bit\n",             E_RESOLUTION_720x480p_60Hz_4_3_24bit);
    printf("\t(%2d) 720x480p60Hz 4:3 30bit\n",             E_RESOLUTION_720x480p_60Hz_4_3_30bit);
    printf("\t(%2d) 720x480p60Hz 4:3 36bit\n",             E_RESOLUTION_720x480p_60Hz_4_3_36bit);
    printf("\t(%2d) 720x480p60Hz 16:9 24bit\n",            E_RESOLUTION_720x480p_60Hz_16_9_24bit);
    printf("\t(%2d) 720x480p60Hz 16:9 30bit\n",            E_RESOLUTION_720x480p_60Hz_16_9_30bit);
    printf("\t(%2d) 720x480p60Hz 16:9 36bit\n",            E_RESOLUTION_720x480p_60Hz_16_9_36bit);
    printf("\t(%2d) 1280x720p60Hz 16:9 24bit\n",            E_RESOLUTION_1280x720p_60Hz_16_9_24bit);
    printf("\t(%2d) 1280x720p60Hz 16:9 30bit\n",            E_RESOLUTION_1280x720p_60Hz_16_9_30bit);
    printf("\t(%2d) 1280x720p60Hz 16:9 36bit\n",            E_RESOLUTION_1280x720p_60Hz_16_9_36bit);
    printf("\t(%2d) 1920x1080i60Hz 16:9 24bit\n",        E_RESOLUTION_1920x1080i_60Hz_16_9_24bit);
    printf("\t(%2d) 1920x1080i60Hz 16:9 30bit\n",        E_RESOLUTION_1920x1080i_60Hz_16_9_30bit);
    printf("\t(%2d) 1920x1080i60Hz 16:9 36bit\n",        E_RESOLUTION_1920x1080i_60Hz_16_9_36bit);
    printf("\t(%2d) 720x480i60Hz 4:3 24bit\n",             E_RESOLUTION_720x480i_60Hz_4_3_24bit);
    printf("\t(%2d) 720x480i60Hz 4:3 30bit\n",             E_RESOLUTION_720x480i_60Hz_4_3_30bit);
    printf("\t(%2d) 720x480i60Hz 4:3 36bit\n",             E_RESOLUTION_720x480i_60Hz_4_3_36bit);
    printf("\t(%2d) 720x480i60Hz 16:9 24bit\n",            E_RESOLUTION_720x480i_60Hz_16_9_24bit);
    printf("\t(%2d) 720x480i60Hz 16:9 30bit\n",            E_RESOLUTION_720x480i_60Hz_16_9_30bit);
    printf("\t(%2d) 720x480i60Hz 16:9 36bit\n",            E_RESOLUTION_720x480i_60Hz_16_9_36bit);
    printf("\t(%2d) 1920x1080p60Hz 16:9 24bit\n",        E_RESOLUTION_1920x1080p_60Hz_16_9_24bit);
    printf("\t(%2d) 1920x1080p60Hz 16:9 30bit\n",        E_RESOLUTION_1920x1080p_60Hz_16_9_30bit);
    printf("\t(%2d) 1920x1080p60Hz 16:9 36bit\n",        E_RESOLUTION_1920x1080p_60Hz_16_9_36bit);
    printf("\t(%2d) 720x576p50Hz 4:3 24bit\n",             E_RESOLUTION_720x576p_50Hz_4_3_24bit);
    printf("\t(%2d) 720x576p50Hz 4:3 30bit\n",             E_RESOLUTION_720x576p_50Hz_4_3_30bit);
    printf("\t(%2d) 720x576p50Hz 4:3 36bit\n",             E_RESOLUTION_720x576p_50Hz_4_3_36bit);
    printf("\t(%2d) 720x576p50Hz 16:9 24bit\n",            E_RESOLUTION_720x576p_50Hz_16_9_24bit);
    printf("\t(%2d) 720x576p50Hz 16:9 30bit\n",            E_RESOLUTION_720x576p_50Hz_16_9_30bit);
    printf("\t(%2d) 720x576p50Hz 16:9 36bit\n",            E_RESOLUTION_720x576p_50Hz_16_9_36bit);
    printf("\t(%2d) 1280x720p50Hz 16:9 24bit\n",            E_RESOLUTION_1280x720p_50Hz_16_9_24bit);
    printf("\t(%2d) 1280x720p50Hz 16:9 30bit\n",            E_RESOLUTION_1280x720p_50Hz_16_9_30bit);
    printf("\t(%2d) 1280x720p50Hz 16:9 36bit\n",            E_RESOLUTION_1280x720p_50Hz_16_9_36bit);
    printf("\t(%2d) 1920x1080i50Hz 16:9 24bit\n",        E_RESOLUTION_1920x1080i_50Hz_16_9_24bit);
    printf("\t(%2d) 1920x1080i50Hz 16:9 30bit\n",        E_RESOLUTION_1920x1080i_50Hz_16_9_30bit);
    printf("\t(%2d) 1920x1080i50Hz 16:9 36bit\n",        E_RESOLUTION_1920x1080i_50Hz_16_9_36bit);
    printf("\t(%2d) 720x576i50Hz 4:3 24bit\n",             E_RESOLUTION_720x576i_50Hz_4_3_24bit);
    printf("\t(%2d) 720x576i50Hz 4:3 30bit\n",             E_RESOLUTION_720x576i_50Hz_4_3_30bit);
    printf("\t(%2d) 720x576i50Hz 4:3 36bit\n",             E_RESOLUTION_720x576i_50Hz_4_3_36bit);
    printf("\t(%2d) 720x576i50Hz 16:9 24bit\n",            E_RESOLUTION_720x576i_50Hz_16_9_24bit);
    printf("\t(%2d) 720x576i50Hz 16:9 30bit\n",            E_RESOLUTION_720x576i_50Hz_16_9_30bit);
    printf("\t(%2d) 720x576i50Hz 16:9 36bit\n",            E_RESOLUTION_720x576i_50Hz_16_9_36bit);
    printf("\t(%2d) 1920x1080p50Hz 16:9 24bit\n",        E_RESOLUTION_1920x1080p_50Hz_16_9_24bit);
    printf("\t(%2d) 1920x1080p50Hz 16:9 30bit\n",        E_RESOLUTION_1920x1080p_50Hz_16_9_30bit);
    printf("\t(%2d) 1920x1080p50Hz 16:9 36bit\n",        E_RESOLUTION_1920x1080p_50Hz_16_9_36bit);
    printf("\t(%c)  Exit with Timing Reset\n",          CERTIFICATION_EXIT);
    printf("\t(%c)  Exit without Timing Reset\n",       CERTIFICATION_EXIT_WITHOUT_TIMING_RESET);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER2;
}


void Demo_Certification_Select3DTiming(void)
{

    printf("Timing Selection:\n");
    printf("\t(%d) 1920x1080p@23.98/24Hz (Frame packing)\n",      E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_FRAME_PACKING);
    printf("\t(%d) 1280x720p@59.94/60Hz (Frame packing)\n",       E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_FRAME_PACKING);
    printf("\t(%d) 1280x720p@50Hz (Frame packing)\n",             E_3D_RESOLUTION_1280x720p50Hz_FRAME_PACKING);
    printf("\t(%d) 1920x1080i@59.94/60Hz (Side by Side)\n",       E_3D_RESOLUTION_1920x1080i59d94HZ_60Hz_SIDE_BY_SIDE);
    printf("\t(%d) 1920x1080i@50Hz (Side by Side)\n",             E_3D_RESOLUTION_1920x1080i50Hz_SIDE_BY_SIDE);
    printf("\t(%d) 1920x1080p@23.98/24Hz (Top and Bottom)\n",     E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_TOP_BOTTOM);
    printf("\t(%d) 1280x720p@59.94/60Hz (Top and Bottom)\n",      E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_TOP_BOTTOM);
    printf("\t(%d) 1280x720p@50Hz (Top and Bottom)\n",            E_3D_RESOLUTION_1280x720p50Hz_TOP_BOTTOM);
    printf("\t(%d) 1280x720p@59.94/60Hz (Side by Side)\n",        E_3D_RESOLUTION_1280x720p59d94Hz_60Hz_SIDE_BY_SIDE);
    printf("\t(%d) 1280x720p@50Hz (Side by Side)\n",              E_3D_RESOLUTION_1280x720p50Hz_SIDE_BY_SIDE);
    printf("\t(%d) 1920x1080p@23.98/24Hz (Side by Side)\n",       E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_SIDE_BY_SIDE);
    printf("\t(%d) 1920x1080p@59.94/60Hz (Top and Bottom)\n",     E_3D_RESOLUTION_1920x1080p59d94Hz_60Hz_TOP_BOTTOM);
    printf("\t(%d) 1920x1080p@50Hz (Top and Bottom)\n",           E_3D_RESOLUTION_1920x1080p50Hz_TOP_BOTTOM);
    printf("\t(%d) 1920x1080i@29.97/30Hz (Top and Bottom)\n",     E_3D_RESOLUTION_1920x1080p29d97Hz_30Hz_TOP_BOTTOM);
    printf("\t(%c) Exit\n",CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER2;
}

void Demo_Certification_Select4K2KTiming(void)
{

    printf("4K2K Timing Selection:\n");
    printf("\t(%2d) 3840x2160p24Hz 16:9 24bit\n",        E_RESOLUTION_3840x2160p_24Hz_16_9_24bit);
    printf("\t(%2d) 3840x2160p24Hz 16:9 30bit\n",        E_RESOLUTION_3840x2160p_24Hz_16_9_30bit);
    printf("\t(%2d) 3840x2160p24Hz 16:9 36bit\n",        E_RESOLUTION_3840x2160p_24Hz_16_9_36bit);
    printf("\t(%2d) 3840x2160p24Hz 16:9 48bit\n",        E_RESOLUTION_3840x2160p_24Hz_16_9_48bit); 
    printf("\t(%2d) 3840x2160p25Hz 16:9 24bit\n",        E_RESOLUTION_3840x2160p_25Hz_16_9_24bit);
    printf("\t(%2d) 3840x2160p25Hz 16:9 30bit\n",        E_RESOLUTION_3840x2160p_25Hz_16_9_30bit);
    printf("\t(%2d) 3840x2160p25Hz 16:9 36bit\n",        E_RESOLUTION_3840x2160p_25Hz_16_9_36bit);
    printf("\t(%2d) 3840x2160p25Hz 16:9 48bit\n",        E_RESOLUTION_3840x2160p_25Hz_16_9_48bit);
    printf("\t(%2d) 3840x2160p30Hz 16:9 24bit\n",        E_RESOLUTION_3840x2160p_30Hz_16_9_24bit);
    printf("\t(%2d) 3840x2160p30Hz 16:9 30bit\n",        E_RESOLUTION_3840x2160p_30Hz_16_9_30bit);
    printf("\t(%2d) 3840x2160p30Hz 16:9 36bit\n",        E_RESOLUTION_3840x2160p_30Hz_16_9_36bit);
    printf("\t(%2d) 3840x2160p30Hz 16:9 48bit\n",        E_RESOLUTION_3840x2160p_30Hz_16_9_48bit);
    printf("\t(%2d) 3840x2160p50Hz 16:9 24bit\n",        E_RESOLUTION_3840x2160p_50Hz_16_9_24bit);
    printf("\t(%2d) 3840x2160p50Hz 16:9 30bit\n",        E_RESOLUTION_3840x2160p_50Hz_16_9_30bit);
    printf("\t(%2d) 3840x2160p50Hz 16:9 36bit\n",        E_RESOLUTION_3840x2160p_50Hz_16_9_36bit);
    printf("\t(%2d) 3840x2160p50Hz 16:9 48bit\n",        E_RESOLUTION_3840x2160p_50Hz_16_9_48bit);
    printf("\t(%2d) 3840x2160p60Hz 16:9 24bit\n",        E_RESOLUTION_3840x2160p_60Hz_16_9_24bit);
    printf("\t(%2d) 3840x2160p60Hz 16:9 30bit\n",        E_RESOLUTION_3840x2160p_60Hz_16_9_30bit);
    printf("\t(%2d) 3840x2160p60Hz 16:9 36bit\n",        E_RESOLUTION_3840x2160p_60Hz_16_9_36bit);
    printf("\t(%2d) 3840x2160p60Hz 16:9 48bit\n",        E_RESOLUTION_3840x2160p_60Hz_16_9_48bit); 
    printf("\t(%2d) 4096x2160p24Hz 16:9 24bit\n",        E_RESOLUTION_4096x2160p_24Hz_16_9_24bit);
    printf("\t(%2d) 4096x2160p24Hz 16:9 30bit\n",        E_RESOLUTION_4096x2160p_24Hz_16_9_30bit);
    printf("\t(%2d) 4096x2160p24Hz 16:9 36bit\n",        E_RESOLUTION_4096x2160p_24Hz_16_9_36bit);
    printf("\t(%2d) 4096x2160p24Hz 16:9 48bit\n",        E_RESOLUTION_4096x2160p_24Hz_16_9_48bit);
    printf("\t(%2d) 4096x2160p25Hz 16:9 24bit\n",        E_RESOLUTION_4096x2160p_25Hz_16_9_24bit);
    printf("\t(%2d) 4096x2160p25Hz 16:9 30bit\n",        E_RESOLUTION_4096x2160p_25Hz_16_9_30bit);
    printf("\t(%2d) 4096x2160p25Hz 16:9 36bit\n",        E_RESOLUTION_4096x2160p_25Hz_16_9_36bit);
    printf("\t(%2d) 4096x2160p25Hz 16:9 48bit\n",        E_RESOLUTION_4096x2160p_25Hz_16_9_48bit);
    printf("\t(%2d) 4096x2160p30Hz 16:9 24bit\n",        E_RESOLUTION_4096x2160p_30Hz_16_9_24bit);
    printf("\t(%2d) 4096x2160p30Hz 16:9 30bit\n",        E_RESOLUTION_4096x2160p_30Hz_16_9_30bit);
    printf("\t(%2d) 4096x2160p30Hz 16:9 36bit\n",        E_RESOLUTION_4096x2160p_30Hz_16_9_36bit);
    printf("\t(%2d) 4096x2160p30Hz 16:9 48bit\n",        E_RESOLUTION_4096x2160p_30Hz_16_9_48bit);
    printf("\t(%2d) 4096x2160p50Hz 16:9 24bit\n",        E_RESOLUTION_4096x2160p_50Hz_16_9_24bit);
    printf("\t(%2d) 4096x2160p50Hz 16:9 30bit\n",        E_RESOLUTION_4096x2160p_50Hz_16_9_30bit);
    printf("\t(%2d) 4096x2160p50Hz 16:9 36bit\n",        E_RESOLUTION_4096x2160p_50Hz_16_9_36bit);
    printf("\t(%2d) 4096x2160p50Hz 16:9 48bit\n",        E_RESOLUTION_4096x2160p_50Hz_16_9_48bit);
    printf("\t(%2d) 4096x2160p60Hz 16:9 24bit\n",        E_RESOLUTION_4096x2160p_60Hz_16_9_24bit);
    printf("\t(%2d) 4096x2160p60Hz 16:9 30bit\n",        E_RESOLUTION_4096x2160p_60Hz_16_9_30bit);
    printf("\t(%2d) 4096x2160p60Hz 16:9 36bit\n",        E_RESOLUTION_4096x2160p_60Hz_16_9_36bit);
    printf("\t(%2d) 4096x2160p60Hz 16:9 48bit\n",        E_RESOLUTION_4096x2160p_60Hz_16_9_48bit);
    printf("\t(%c)  Exit with Timing Reset\n",          CERTIFICATION_EXIT);
    printf("\t(%c)  Exit without Timing Reset\n",       CERTIFICATION_EXIT_WITHOUT_TIMING_RESET);

    u32Layer= E_CERTIFICATION_LAYER2;
}

void Demo_Certification_SelectHDMISetting(void)
{

    printf("Timing Selection:\n");
    printf("\t(%d) HDMI Setting: Tx Mode\n",             E_CERTIFICATION_HDMISETTING_TXMODE);
    printf("\t(%d) HDMI Setting: Color Space\n",         E_CERTIFICATION_HDMISETTING_COLORSPACE);
    printf("\t(%c) Exit\n",                             CERTIFICATION_EXIT);
    printf("\t(%c) Exit without Timing Reset\n",        CERTIFICATION_EXIT_WITHOUT_TIMING_RESET);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER2;


}


void Demo_Certification_SelectHDMITxMode(void)
{

    printf("Timing Selection:\n");
    printf("\t(%d) HDMI Tx mode: DVI mode\n",         E_CERT_DVI);
    printf("\t(%d) HDMI Tx mode: HDMI mode\n",         E_CERT_HDMI);
    printf("\t(%c) Exit\n",CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER3;


}


void Demo_Certification_Dbg(void)
{
    printf("DBG :\n");
    printf("\t(%d) ON\n",E_CERTIFICATION_DBG_ON);
    printf("\t(%d) OFF\n",E_CERTIFICATION_DBG_OFF);
    printf("\t(%c) Exit\n",CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER1;

}

void Demo_Certification_EDID(void)
{
    printf("DBG :\n");
    printf("\t(%d) ON\n",E_CERTIFICATION_EDID_ON);
    printf("\t(%d) OFF\n",E_CERTIFICATION_EDID_OFF);
    printf("\t(%c) OFF and then Exit\n",CERTIFICATION_EXIT);
    printf("Please select Test item:");
    u32Layer= E_CERTIFICATION_LAYER1;
}

MS_BOOL Demo_Certification_Monitor(void)
{
#if 0
    if(_Demo_HDMI_Cert_Task.pStack == NULL)
    {
        _Demo_HDMI_Cert_Task.pStack = MsOS_AllocateMemory(_Demo_HDMI_Cert_Task.u32StackSize, gs32MstarCachedPoolID);
        if(!_Demo_HDMI_Cert_Task.pStack)
        {
            printf("Cannot allocate memory\n");
            return FALSE;

        }
    }

    if(_Demo_HDMI_Cert_Task.iId < 0)
    {
        /// - Create Task Application
        _Demo_HDMI_Cert_Task.iId = MsOS_CreateTask( _appDemo_XC_Montask,
                                            (MS_U32)NULL,
                                            _Demo_HDMI_Cert_Task.ePriority,
                                            TRUE,
                                            _Demo_HDMI_Cert_Task.pStack,
                                            _Demo_HDMI_Cert_Task.u32StackSize,
                                            _Demo_HDMI_Cert_Task.szName);
        if(_Demo_HDMI_Cert_Task.iId < 0)
        {
            printf("Cannot create task\n");
            return FALSE;
        }
    }
#endif
    return TRUE;
}

void Demo_Certification_InitHDMI(void)
{
    MS_U32 u32XCDevice = 0;

    if(_bXC_Init==TRUE)
    {
        return;
    }

    Demo_XC_Init(&u32XCDevice);

    #if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
    Demo_Certification_InitRepeater();
    #endif

    u32Layer= E_CERTIFICATION_LAYER1;
    _bXC_Init=TRUE;
}

//#if CERTIFICATION_HDMI_HDCP_REPEATER
void Demo_Certification_Repeater_Exit(void)
{

    MS_U32 u32XCDevice = 0;
    //MS_U32 u32OutputTiming = E_OUTPUT_TIMING_720X480_60P;
    MS_U32 u32XCWindow=0;
    MS_U32 u32XCInputSrc=2;
    MS_BOOL bHDCP = FALSE;
    {
        Demo_HDMI_InitHDCP(&bHDCP);
    }
    {
        u32XCDevice = 0;

        u32XCWindow=0;
        u32XCInputSrc=0xFF;//E_DDI_XC_INPUT_SOURCE_NONE;

        if(Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc)==FALSE)
        {
            printf("[%s][%d]  Dev0 Play Video fail!\n",__FUNCTION__,__LINE__);
            //return ;
        }

        MsOS_DelayTask(100);
    }

}
//#endif

void Demo_Certification_Exit(void)
{
    Demo_OSD_FreeWin();

    #if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
    Demo_Certification_Repeater_Exit();
    #endif
    /*
    MS_U32 u32XCDevice = 0;
    appDemo_XC_Shutdown(&u32XCDevice);
    */
}

MS_BOOL Demo_Certification_ResolutionReset(void)
{
    MS_U32 u32XCDevice = 0;
    MS_BOOL bHDCP = FALSE;
    #if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
    MS_U32 u32Enable = FALSE;
    MS_U32 u32OutMod = E_MSAPI_XC_3D_OUTPUT_MODE_NONE;
    MS_U32 u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_HDMI;
    #endif
    MS_U32 u32CDType = E_MSAPI_XC_HDMITX_CD_8BITS;
    MS_U32 u32XCWindow = E_MSAPI_XC_MAIN_WINDOW;
    MS_U32 u32AS = E_MSAPI_XC_ASPECT_RATIO_NORMAL;

    u32XCDevice = 0;

    #if (HDMI_CTS_CERTIFICATION_HDCP_SOURCE || HDMI_CTS_CERTIFICATION_HDCP_REPEATER)
    if(Demo_HDMI_InitHDCP(&bHDCP) == FALSE)
    {
        printf("[%s][%d]  Disable HDCP!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    #endif

    #if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
    if(Demo_XC_Enable3D(&u32XCDevice, &u32Enable, &u32OutMod) == FALSE)
    {
        printf("[%s][%d]  Dev0 Enable 3D fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    #endif
    MApi_HDMITx_SetVideoOutputAsepctRatio(HDMITX_VIDEO_AR_Reserved);
    Demo_HDMI_SetOutputColorDepth(&u32CDType);
        Demo_XC_SetAS(&u32XCDevice, &u32XCWindow, &u32AS);

    return TRUE;
}

#if HDMI_CTS_CERTIFICATION_HDMI_CEC
MS_BOOL Demo_Certification_SartCEC(void)
{
    Demo_CEC_init();
}
#endif

#if HDMI_CTS_CERTIFICATION_HDCP_SOURCE
MS_BOOL Demo_Certification_SartHDCP_Source(void)
{

    MS_BOOL bHDCP = FALSE;

    Demo_Certification_InitHDMI();

    //Demo_HDMI_InitHDCP(&bHDCP);
    if(0)
    {

        MS_U32 u32XCDevice = 0;
        //MS_U32 u32OutputTiming = E_OUTPUT_TIMING_720X480_60P;
        MS_U32 u32XCWindow=0;
        MS_U32 u32XCInputSrc=2;
        u32XCDevice = 0;

        u32XCWindow=0;
        u32XCInputSrc=2;

        if(Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc)==FALSE)
        {
            printf("[%s][%d]  Dev0 Play Video fail!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        MsOS_DelayTask(100);
    }
    else
    {


        #if 0
        MS_U32 HD_GOP=0;
        MS_U32 SD_GOP=1;
        MS_U32 HD_DST=0;
        MS_U32 SD_DST=3;
        MS_U32 ColorType=2;
        MS_U32 GWinWidth0=720;
        MS_U32 GWinHeight0=480;
        MS_U32 GWinWidth1=720;
        MS_U32 GWinHeight1=480;
        printf("%d\n",__LINE__);
        Demo_OSD_Init(&HD_GOP, &SD_GOP, &HD_DST, &SD_DST);
        MsOS_DelayTask(1000);
        printf("%d\n",__LINE__);
        Demo_OSD_SetConfig();
        MsOS_DelayTask(1000);
        printf("%d\n",__LINE__);

        Demo_OSD_CreateWin(&ColorType, &GWinWidth0, &GWinHeight0, &GWinWidth1, &GWinHeight1);
        MsOS_DelayTask(1000);
                printf("%d\n",__LINE__);
        Demo_OSD_DrawBmp(&ColorType);  //GOP_DrawBmp();
        MsOS_DelayTask(1000);
                printf("%d\n",__LINE__);
        #else
        MS_U32 HD_GOP=0;
        MS_U32 SD_GOP=1;
        MS_U32 HD_DST=0;
        MS_U32 SD_DST=4;
        MS_U32 ColorType=2;
        MS_U32 GWinWidth0=720;
        MS_U32 GWinHeight0=480;
        MS_U32 GWinWidth1=720;
        MS_U32 GWinHeight1=480;
        MS_U32 Target=0;
        MS_U32 index=1;
        MS_U32 x1=0;
        MS_U32 y1=0;
        MS_U32 w1=720;
        MS_U32 h1=480;
        MS_U32 x2=0;
        MS_U32 y2=480;
        MS_U32 w2=720;
        MS_U32 h2=0;

        //=================
        MS_U16 GWinFmt = 10;

        printf("%d\n",__LINE__);
        Demo_OSD_Init(&HD_GOP, &SD_GOP, &HD_DST, &SD_DST);
        MsOS_DelayTask(100);
        printf("%d\n",__LINE__);
        Demo_OSD_SetConfig();
        MsOS_DelayTask(100);
        printf("%d\n",__LINE__);

        //=================
        Demo_OSD_SetGWinFmt(&GWinFmt);
        MsOS_DelayTask(100);
                printf("%d\n",__LINE__);

        Demo_OSD_CreateWin(&ColorType, &GWinWidth0, &GWinHeight0, &GWinWidth1, &GWinHeight1);
        MsOS_DelayTask(100);
                printf("%d\n",__LINE__);
        Demo_OSD_DrawLine(&Target, &index, &x1, &y1, &w1, &h1);
        MsOS_DelayTask(100);
                printf("%d\n",__LINE__);
        Demo_OSD_DrawLine(&Target, &index, &x2, &y2, &w2, &h2);
        MsOS_DelayTask(100);
                printf("%d\n",__LINE__);
        #endif
    }

    bHDCP = TRUE;
    Demo_HDMI_InitHDCP(&bHDCP);

    return TRUE;
}
#endif

#if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
//static MS_BOOL bParam = TRUE;
static MS_BOOL bTxConnected = 0xFF;
#define HPD_L2H_EVENT 1
void _pfn_EventCb(MS_U32 eFlag, void *param)
{
    printf("[%s][%d] HDCP PASS call back function invoked!  0x%lX\n", __FUNCTION__, __LINE__,eFlag);

    #if (HDMI_CTS_CERTIFICATION_HDCP_REPEATER == 0)
    //appDemo_EEPROM_HWI2C_Init();
    if ((eFlag & E_HDMITX_EVENT_DISCONNECT) == E_HDMITX_EVENT_DISCONNECT)
    {
        if(bTxConnected != FALSE)
        {
            printf("[%s][%d][%8lu] Trigger disconnected! \n", __FUNCTION__, __LINE__,MsOS_GetSystemTime());
        }
        bTxConnected = FALSE;
    }
    else if ((eFlag & E_HDMITX_EVENT_CONNECT) == E_HDMITX_EVENT_CONNECT)
    {
        if(bTxConnected != TRUE)
        {
            printf("[%s][%d][%8lu] Trigger Connected! \n", __FUNCTION__, __LINE__,MsOS_GetSystemTime());
            MsOS_SetEvent (_s32TxHPD_Event, HPD_L2H_EVENT);
        }
        bTxConnected = TRUE;
    }
    #endif
}

void _Demo_Certification_Repeater_Task (MS_U32 argc, void *argv)
{
    MS_U32     u32RetrievedEventFlag;

    while(TRUE)
    {
        MsOS_WaitEvent (_s32TxHPD_Event,HPD_L2H_EVENT,&u32RetrievedEventFlag,E_OR_CLEAR,MSOS_WAIT_FOREVER);

        u32RetrievedEventFlag = 0;
        MDrv_HDMI_EX_pullhpd(FALSE, INPUT_PORT_DVI0, TRUE);
        MsOS_DelayTask(150);
        MDrv_HDMI_EX_pullhpd(TRUE, INPUT_PORT_DVI0, TRUE);


    }
}

MS_BOOL Demo_Certification_TriggerRxHPD(void)
{

    printf("[%s][%d][%8lu] Trigger Rx HPD! \n", __FUNCTION__, __LINE__,MsOS_GetSystemTime());
    MsOS_SetEvent (_s32TxHPD_Event, HPD_L2H_EVENT);
    return TRUE;
}

MS_BOOL Demo_Certification_isTxConnected(void)
{
    return bTxConnected;
}

void Demo_Certification_InitRepeater(void)
{
    if(_s32TxHPD_Task.iId == -1)
    {
        _s32TxHPD_Task.iId = MsOS_CreateTask(_Demo_Certification_Repeater_Task, 0,
                                            _s32TxHPD_Task.ePriority,
                                            TRUE,
                                            _s32TxHPD_Task.pStack,
                                            _s32TxHPD_Task.u32StackSize,
                                            (char *)_s32TxHPD_Task.szName);

        if(_s32TxHPD_Task.iId < 0)
        {
            printf("[%s][%d] Fail to create task\n",__FUNCTION__,__LINE__);
        }

    }

    if(_s32TxHPD_Event == -1)
    {
        _s32TxHPD_Event = MsOS_CreateEventGroup("Tx Hpd Event");

        if(_s32TxHPD_Event < 0)
        {
            printf("[%s][%d] Fail to create event group\n",__FUNCTION__,__LINE__);
        }
        else
        {
            printf("Create Event for detecting HPD \n");
        }
    }

    #if (HDMI_CTS_CERTIFICATION_HDCP_REPEATER == 0)
    MS_BOOL bRet;
    bRet = MApi_HDMITx_SetEvent(E_HDMITX_EVENT_HDCP_PASS, _pfn_EventCb, &bParam);
    printf("register call back done! bRet = %u \n", bRet);
    #endif
}

void Demo_Certification_Store_RxEDID(MS_U8 *u8adr)
{
    MS_U16 i = 0;
    MS_U8 current, offset;//,block_num = 0, k;
    MS_U8 descriptor, tag, len;
    //HDMITX_VIDEO_COLOR_FORMAT output_color= HDMITX_VIDEO_COLOR_RGB444;

#if 1
    for(i=E_EDID_BLOCK_0;i<=E_EDID_BLOCK_1;i++)
    {
        Demo_EEPROM_edidblockread(i, &_u8Edid[i*128], 128);
        if(i == 0)
        {
            //block_num = _u8Edid[0x7E];
            //cert_info("block_num: 0x%02u \n",u8Edid[0x7E]);
        }
        else if(i == 1)
        {
            //cert_info("_u8Edid: %02X %02X\n",_u8Edid[0+i*128],_u8Edid[1+i*128]);
            if((_u8Edid[0+i*128]==0x02) && (_u8Edid[1+i*128]<=0x04))
            {
                offset = _u8Edid[0x02+i*128];
                current = 0x04;
                //cert_info("offset: %8u\n",offset);
                if (offset == 0xFF)
                {
                    continue;
                }
                if (offset < 0x05)
                {
                    continue;
                }



                while(current<offset)
                {
                    descriptor = _u8Edid[(i*128)+current];
                    tag = ((descriptor&0xE0)>>5);
                    len = (descriptor&0x1F);
                    //cert_info("current:%8u ,offset: %8u   0x%X\n",current,offset,_u8Edid[0x03] & 0x30);
                    if(tag == 3)
                    {
                        current++;
                        // EDID physical address
                        _u8Edid[(i*128)+current+3] = u8adr[0];
                        _u8Edid[(i*128)+current+4] = u8adr[1];
                        cert_info("addr :%02X%02X\n",u8adr[0],u8adr[1]);
                        break;
                    }
                    else
                    {
                        current += (len+1);
                    }
                }
            }
        }

    }


#else
    for(i = 0; i<2 ;i++)
    {
        //cert_info("%d %d\n",i,i*128);
        if(MApi_HDMITx_GetEDIDData(&_u8Edid[0+i*128], i) == TRUE)
        {
            if(i == 0)
            {
                block_num = _u8Edid[0x7E];
                //cert_info("block_num: 0x%02u \n",block_num);
            }
            else if(i == 1)
            {
                //cert_info("_u8Edid: %02X %02X\n",_u8Edid[0+i*128],_u8Edid[1+i*128]);
                if((_u8Edid[0+i*128]==0x02) && (_u8Edid[1+i*128]<=0x04))
                {
                    offset = _u8Edid[0x02+i*128];
                    current = 0x04;
                    //cert_info("offset: %8u\n",offset);
                    if (offset == 0xFF)
                    {
                        continue;
                    }
                    if (offset < 0x05)
                    {
                        continue;
                    }



                    while(current<offset)
                    {
                        descriptor = _u8Edid[(i*128)+current];
                        tag = ((descriptor&0xE0)>>5);
                        len = (descriptor&0x1F);
                        //cert_info("current:%8u ,offset: %8u   0x%X\n",current,offset,_u8Edid[0x03] & 0x30);
                        if(tag == 3)
                        {
                            current++;
                            // EDID physical address
                            _u8Edid[(i*128)+current+3] = u8adr[0];
                            _u8Edid[(i*128)+current+4] = u8adr[1];
                            cert_info("addr :%02X%02X\n",u8adr[0],u8adr[1]);
                            break;
                        }
                        else
                        {
                            current += (len+1);
                        }
                    }
                }
            }


        }
    }
#endif
    MS_U16 checksum = 0;

    for(i=0;i<127;i++)
    {
        checksum += _u8Edid[i+128];

    }
    _u8Edid[1*128+127] = 0x100-(checksum%256);



    for(i=E_EDID_BLOCK_0;i<=E_EDID_BLOCK_1;i++)
    {
        Demo_EEPROM_edidblockwrite(i, &_u8Edid[i*128], 128);
    }


}

void Demo_Certidication_getEDID(MS_U8 *addr)
{
    MS_U8 connection_label;
    MS_U16 i = 0;
    //MS_U16 u16Value;
    //MS_U8 u8Value = 0;
    MS_U8 u8Tmp = 0;
    MS_U8 u8edidadr[2];
    MS_U8 u8rxedidadr[2];

    MApi_HDMITx_GetEDIDPhyAdr(u8edidadr);


    //Vendor Specific Data Block
    if ((u8edidadr[0]!=addr[0]) || (u8edidadr[1]!=addr[1]))
    {

        cert_info("[%s][%d] Sink EDID PA: %02X %02X  ORI PA: %02X %02X\n",__FUNCTION__,__LINE__,u8edidadr[0],u8edidadr[1],addr[0],addr[1]);

        //memcpy(addr,u8edidadr,2);
        //memcpy(u8rxedidadr,u8edidadr,2);
        for(i=0;i<2;i++)
        {
            addr[i] = u8edidadr[i];
            u8rxedidadr[i] = u8edidadr[i];
        }

        if((u8edidadr[1]&0x0F) == 0x0)
        {
            connection_label = 1; // end with 0
        }
        else
        {
            connection_label = 0;
            u8rxedidadr[0]=0xFF;
            u8rxedidadr[1]=0xFF;

        }

        cert_info("[%s][%d]connection_label: %d \n",__FUNCTION__,__LINE__,connection_label);
        //Converter Mode
        // Write Physical Address to VSDB in EDID
        if(connection_label==1)
        {
            for (i=0;i<4;i++)
            {
                if((i%2) == 0)
                {
                    u8Tmp = u8rxedidadr[i/2]>>4;

                    if(u8Tmp == 0)
                    {
                        u8rxedidadr[i/2] = u8rxedidadr[i/2]|0x10;
                        break;
                    }
                }
                else
                {
                    u8Tmp = u8rxedidadr[i/2]&0x0F;
                    if(u8Tmp == 0)
                    {
                        u8rxedidadr[i/2] = u8rxedidadr[i/2]|0x01;

                        break;
                    }

                }

            }

        }

        printf("[%s][%d] Set RX EDID Physical Address as \"%02X %02X\" \n",__FUNCTION__,__LINE__,u8rxedidadr[0],u8rxedidadr[1]);

        Demo_Certification_Store_RxEDID(u8rxedidadr);
    }
    Demo_Certification_TriggerRxHPD();

}

void Demo_Certidication_resetEDID(MS_U8 *addr)
{
    //Vendor Specific Data Block
    printf("[%s][%d] Reset RX EDID Physical Address as \"%02X %02X\" \n",__FUNCTION__,__LINE__,addr[0],addr[1]);
    Demo_Certification_Store_RxEDID(addr);
    Demo_Certification_TriggerRxHPD();
}

void Demo_Certification_threadEDID(MS_U32 argc, void *argv)
{

    MS_BOOL bConnected = FALSE;
    MS_BOOL bTrigger = FALSE;
    MS_U8 addr[2]={0xFF,0xFF};
    MS_BOOL bUnstable = 0xFF;
    HDMITX_EDID_3D_STRUCTURE_ALL st3DStructure;
    MSAPI_XC_DEVICE_ID stXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
    MS_BOOL b3D = 0xFF;

    while(1)
    {

        if(_bEDID == TRUE)
        {
            if(Demo_Certification_isTxConnected() == FALSE)
            {
                if(bConnected == TRUE)
                {
                    addr[0]=0xFF;
                    addr[1]=0xFF;
                    Demo_Certidication_resetEDID(addr);
                }
                bConnected = FALSE;
                MsOS_DelayTask(100);
                continue;
            }
            else
            {
                ///////printf("[%s][%d] bTrigger\r\n",__FUNCTION__,__LINE__);
                if(bConnected == FALSE)
                {
                    bTrigger = TRUE;
                    printf("[%s][%d] bTrigger\r\n",__FUNCTION__,__LINE__);
                }
                bConnected = TRUE;
            }


            //if(MApi_HDMITx_IsStable()==FALSE )
            if(_Demo_Certification_CheckHDMITx()==FALSE )
            {
                /////printf("[%s][%d] Is not stable\r\n",__FUNCTION__,__LINE__);
                MsOS_DelayTask(20);
                continue;
            }

            if(bTrigger == TRUE)
            {
                Demo_Certidication_getEDID(addr);
            }
            bTrigger = FALSE;
        }
        else
        {
            if(u8TestItem == E_CERTIFICATION_HDMI_DVI)
            {
                if(Demo_Certification_isTxConnected() == FALSE)
                {
                    if(bConnected == TRUE)
                    {
                        bTrigger = TRUE;
                        printf("[%s][%d] bTrigger\r\n",__FUNCTION__,__LINE__);
                    }

                    bConnected = FALSE;
                    MsOS_DelayTask(100);
                    continue;
                }
                else
                {
                    ///////printf("[%s][%d] bTrigger\r\n",__FUNCTION__,__LINE__);
                    if(bConnected == FALSE)
                    {
                        bTrigger = TRUE;
                        printf("[%s][%d] bTrigger\r\n",__FUNCTION__,__LINE__);
                    }
                    bConnected = TRUE;
                }

                if((_u8HDMI_TEST_TYPE == E_CERTIFICATION_SOURCE_3D)|| (_u8HDMI_TEST_TYPE == E_CERTIFICATION_SOURCE_NORMAL))
                {
                    //if(MApi_HDMITx_IsStable()==FALSE )
                    if(_Demo_Certification_CheckHDMITx()==FALSE )
                    {
                        /////printf("[%s][%d] Is not stable\r\n",__FUNCTION__,__LINE__);
                        bUnstable = TRUE;
                        MsOS_DelayTask(20);
                        continue;
                    }
                }

                if(_u8HDMI_TEST_TYPE == E_CERTIFICATION_SOURCE_3D)
                {

                    if(_bFinished3DSetting == FALSE)
                    {
                        MsOS_DelayTask(20);
                        continue;
                    }

                    if(bUnstable == TRUE)
                    {
                        printf("[%s][%d] _3DTiming : %d\r\n",__FUNCTION__,__LINE__,_3DTiming);
                        if(MApi_HDMITx_GetRx3DStructureFromEDID(_3DTiming, &st3DStructure)== TRUE)
                        {
                            msAPI_XC_SetVideoMuteByMode_EX(&stXC_DeviceId, TRUE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                            msAPI_XC_SetVideoMuteByMode_EX(&stXC_DeviceId, TRUE, E_MSAPI_XC_SUB_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                            //msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_HDMI);
                            //// 7-38 edid 9
                            printf("[%s][%d] TX st3DStructure :%d\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__,st3DStructure);
                            if(st3DStructure & HDMITX_EDID_3D_FramePacking)
                            {
                                if(b3D == FALSE)
                                {
                                    if(E_MSAPI_XC_OK != msAPI_XC_Set3DMode_EX(&stXC_DeviceId, E_MSAPI_XC_3D_INPUT_FRAME_PACKING, E_MSAPI_XC_3D_OUTPUT_FRAME_PACKING))
                                    {
                                        printf("msAPI_XC_Set3DMode failed\n");

                                    }

                                    printf("2D-->3D:  FB<--> FB\n");

                                }

                                printf(UARTCOLOR_LIGHT_RED"[%s][%d] msAPI_XC_HDMITx_Set3DMode\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__);
                                msAPI_XC_HDMITx_Set3DMode(ENABLE,HDMITX_VIDEO_3D_FramePacking);

                                printf("[%s][%d] eTiming:%d \r\n",__FUNCTION__,__LINE__,MApi_XC_GetHDMIType(g_PNL_TypeSel[E_MSAPI_XC_DEVICE0]));
                                if(MApi_XC_GetHDMIType(g_PNL_TypeSel[E_MSAPI_XC_DEVICE0])<10)
                                {
                                    E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming;
                                    msAPI_XC_GetOutputResolution_EX(&stXC_DeviceId, &eTiming);
                                    switch(eTiming)
                                    {
                                        case E_MSAPI_XC_RES_1920x1080P_24Hz:
                                            printf("[%s] [%d] E_MSAPI_XC_RES_1920x1080P_24Hz\n",__FUNCTION__,__LINE__);
                                            msAPI_XC_EnableMiscOutput(DISABLE);
                                            msAPI_XC_ChangeOutputResolution_EX(&stXC_DeviceId, E_MSAPI_XC_RES_1920x2205P_3DFP_24Hz);
                                            msAPI_XC_EnableMiscOutput(ENABLE);
                                            msAPI_XC_HDMITx_Set3DMode(TRUE,HDMITX_VIDEO_3D_FramePacking);
                                            printf("[%s][%d]  \n",__FUNCTION__,__LINE__);
                                            break;
                                        case E_MSAPI_XC_RES_1280x720P_50Hz:
                                            printf("[%s] [%d] E_MSAPI_XC_RES_1280x720P_50Hz\n",__FUNCTION__,__LINE__);
                                            msAPI_XC_EnableMiscOutput(DISABLE);
                                            msAPI_XC_ChangeOutputResolution_EX(&stXC_DeviceId, E_MSAPI_XC_RES_1280x1470P_3DFP_50Hz);
                                            msAPI_XC_EnableMiscOutput(ENABLE);
                                            msAPI_XC_HDMITx_Set3DMode(TRUE,HDMITX_VIDEO_3D_FramePacking);
                                            printf("[%s][%d]  \n",__FUNCTION__,__LINE__);
                                            break;
                                        case E_MSAPI_XC_RES_1280x720P_60Hz:
                                            printf("[%s] [%d]E_MSAPI_XC_RES_1280x720P_50Hz\n",__FUNCTION__,__LINE__);
                                            msAPI_XC_EnableMiscOutput(DISABLE);
                                            msAPI_XC_ChangeOutputResolution_EX(&stXC_DeviceId, E_MSAPI_XC_RES_1280x1470P_3DFP_60Hz);
                                            msAPI_XC_EnableMiscOutput(ENABLE);
                                            msAPI_XC_HDMITx_Set3DMode(TRUE,HDMITX_VIDEO_3D_FramePacking);
                                            printf("[%s][%d]  \n",__FUNCTION__,__LINE__);
                                            break;
                                        case E_MSAPI_XC_RES_1920x2205P_3DFP_24Hz:
                                        case E_MSAPI_XC_RES_1280x1470P_3DFP_50Hz:
                                        case E_MSAPI_XC_RES_1280x1470P_3DFP_60Hz:
                                            break;
                                        default:
                                            printf("Current output timing does not support frame packing!\n");

                                            return ;
                                    }


                                }
                                msAPI_XC_SetWin_EX(&stXC_DeviceId, NULL, NULL, NULL, E_MSAPI_XC_MAIN_WINDOW);


                                if(MApi_XC_EX_IsBlackVideoEnable((XC_DEVICE_ID *)&stXC_DeviceId,(XC_EX_SCALER_WIN)E_MSAPI_XC_MAIN_WINDOW)==TRUE)
                                {
                                    msAPI_XC_SetVideoMuteByMode_EX(&stXC_DeviceId, FALSE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                                }
                                if(MApi_XC_EX_IsBlackVideoEnable((XC_DEVICE_ID *)&stXC_DeviceId,(XC_EX_SCALER_WIN)E_MSAPI_XC_SUB_WINDOW)==TRUE)
                                {
                                    msAPI_XC_SetVideoMuteByMode_EX(&stXC_DeviceId, FALSE, E_MSAPI_XC_SUB_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                                }

                            }
                            else{
                                //// 7-38 edid 1
                                printf(UARTCOLOR_LIGHT_RED"[%s][%d] TX not support 3D ... start\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__);
                                if(_3DTiming == 9)
                                {
                                    MApi_HDMITx_SetTMDSOnOff(FALSE);
                                    printf("TUrn Off TMDS\n");
                                }
                                else
                                {
                                    // - If the source is HDMIRx input, the 3D input mode will be auto detect in HDMI Rx monitor
                                    // - The input parameter 'en3DInMode' is don't care if the source is HDMIRx
                                    // - Use 'msAPI_XC_HDMIRx_Get3DMode' to get input 3D mode
                                    if(E_MSAPI_XC_OK != msAPI_XC_Set3DMode_EX(&stXC_DeviceId, E_MSAPI_XC_3D_INPUT_FRAME_PACKING, E_MSAPI_XC_3D_OUTPUT_MODE_NONE))
                                    {
                                        printf("msAPI_XC_Set3DMode failed\n");

                                    }

                                    {
                                        E_MSAPI_XC_OUTPUT_TIMING_TYPE eTiming;
                                        msAPI_XC_GetOutputResolution_EX(&stXC_DeviceId, &eTiming);
                                        switch(eTiming)
                                        {
                                            case E_MSAPI_XC_RES_1920x1080P_24Hz:
                                                printf("[%s] [%d] E_MSAPI_XC_RES_1920x1080P_24Hz\n",__FUNCTION__,__LINE__);
                                                msAPI_XC_EnableMiscOutput(DISABLE);
                                                msAPI_XC_ChangeOutputResolution_EX(&stXC_DeviceId, E_MSAPI_XC_RES_1920x1080P_24Hz);
                                                msAPI_XC_EnableMiscOutput(ENABLE);

                                                printf("[%s][%d]  \n",__FUNCTION__,__LINE__);
                                                break;
                                            case E_MSAPI_XC_RES_1280x720P_50Hz:
                                                printf("[%s] [%d] E_MSAPI_XC_RES_1280x720P_50Hz\n",__FUNCTION__,__LINE__);
                                                msAPI_XC_EnableMiscOutput(DISABLE);
                                                msAPI_XC_ChangeOutputResolution_EX(&stXC_DeviceId, E_MSAPI_XC_RES_1280x720P_50Hz);
                                                msAPI_XC_EnableMiscOutput(ENABLE);

                                                printf("[%s][%d]  \n",__FUNCTION__,__LINE__);
                                                break;
                                            case E_MSAPI_XC_RES_1280x720P_60Hz:
                                                printf("[%s] [%d]E_MSAPI_XC_RES_1280x720P_50Hz\n",__FUNCTION__,__LINE__);
                                                msAPI_XC_EnableMiscOutput(DISABLE);
                                                msAPI_XC_ChangeOutputResolution_EX(&stXC_DeviceId, E_MSAPI_XC_RES_1280x720P_60Hz);
                                                msAPI_XC_EnableMiscOutput(ENABLE);

                                                printf("[%s][%d]  \n",__FUNCTION__,__LINE__);
                                                break;
                                            case E_MSAPI_XC_RES_1920x2205P_3DFP_24Hz:
                                            case E_MSAPI_XC_RES_1280x1470P_3DFP_50Hz:
                                            case E_MSAPI_XC_RES_1280x1470P_3DFP_60Hz:
                                                break;
                                            default:
                                                printf("Current output timing does not support frame packing!\n");

                                                return ;
                                        }
                                    }
                                    //msAPI_XC_HDMITx_Set3DMode(FALSE,HDMITx_VIDEO_3D_Not_in_Use);
                                    MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_STOP_PACKET, 0xFF);
                                    MApi_HDMITx_Set_VS_InfoFrame(HDMITx_VIDEO_VS_Reserved, HDMITx_VIDEO_3D_Not_in_Use, HDMITx_VIDEO_4k2k_Reserved);
                                    // Set TMDS off->on for compatibility issue for some TVs
                                    MApi_HDMITx_SetTMDSOnOff(FALSE);
                                    MsOS_DelayTask(20);
                                    MApi_HDMITx_SetTMDSOnOff(TRUE);
                                    msAPI_XC_SetWin_EX(&stXC_DeviceId, NULL, NULL, NULL, E_MSAPI_XC_MAIN_WINDOW);
                                    if(MApi_XC_EX_IsBlackVideoEnable((XC_DEVICE_ID *)&stXC_DeviceId,(XC_EX_SCALER_WIN)E_MSAPI_XC_MAIN_WINDOW)==TRUE)
                                    {
                                        msAPI_XC_SetVideoMuteByMode_EX(&stXC_DeviceId, FALSE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                                    }
                                    if(MApi_XC_EX_IsBlackVideoEnable((XC_DEVICE_ID *)&stXC_DeviceId,(XC_EX_SCALER_WIN)E_MSAPI_XC_SUB_WINDOW)==TRUE)
                                    {
                                        msAPI_XC_SetVideoMuteByMode_EX(&stXC_DeviceId, FALSE, E_MSAPI_XC_SUB_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
                                    }
                                    printf("[%s][%d] umnute : timing:%d\r\n",__FUNCTION__,__LINE__,_3DTiming);
                                    printf(UARTCOLOR_LIGHT_RED"[%s][%d] TX not support 3D ... end\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__);
                                }
                            }


                            b3D = TRUE;
                        }
                        else
                        {
                            //// 7-38 edid 1
                            printf(UARTCOLOR_LIGHT_RED"[%s][%d] TX not support 3D ... start\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__);
                            // - If the source is HDMIRx input, the 3D input mode will be auto detect in HDMI Rx monitor
                            // - The input parameter 'en3DInMode' is don't care if the source is HDMIRx
                            // - Use 'msAPI_XC_HDMIRx_Get3DMode' to get input 3D mode
                            if(E_MSAPI_XC_OK != msAPI_XC_Set3DMode_EX(&stXC_DeviceId, E_MSAPI_XC_3D_INPUT_FRAME_PACKING, E_MSAPI_XC_3D_OUTPUT_MODE_NONE))
                            {
                                printf("msAPI_XC_Set3DMode failed\n");

                            }
                            //msAPI_XC_HDMITx_Set3DMode(FALSE,HDMITx_VIDEO_3D_Not_in_Use);
                            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_STOP_PACKET, 0xFF);
                            MApi_HDMITx_Set_VS_InfoFrame(HDMITx_VIDEO_VS_Reserved, HDMITx_VIDEO_3D_Not_in_Use, HDMITx_VIDEO_4k2k_Reserved);
                            // Set TMDS off->on for compatibility issue for some TVs
                            MApi_HDMITx_SetTMDSOnOff(FALSE);
                            MsOS_DelayTask(20);
                            MApi_HDMITx_SetTMDSOnOff(TRUE);
                            msAPI_XC_SetWin_EX(&stXC_DeviceId, NULL, NULL, NULL, E_MSAPI_XC_MAIN_WINDOW);
                            printf(UARTCOLOR_LIGHT_RED"[%s][%d] TX not support 3D ... end\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__);
                            b3D = FALSE;

                        }
                    }
                }

                bUnstable = FALSE;
            }


        }
        MsOS_DelayTask(100);

    }


}

MS_BOOL Demo_Certification_SartHDCP_Repeater(void)
{

    MS_BOOL bHDCP = TRUE;
    MS_U32 u32XCDevice = 0;

    Demo_Certification_InitHDMI();

    {

        u32XCDevice = 0;
        //MS_U32 u32OutputTiming = E_OUTPUT_TIMING_720X480_60P;
        MS_U32 u32XCWindow=0;
        MS_U32 u32XCInputSrc=2;
        u32XCDevice = 0;

        u32XCWindow=0;
        u32XCInputSrc=2;

        if(Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc)==FALSE)
        {
            printf("[%s][%d]  Dev0 Play Video fail!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        MsOS_DelayTask(100);
    }

    Demo_HDMI_InitHDCP(&bHDCP);

    return TRUE;
}
#endif

#if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
MS_BOOL Demo_Certification_DisableThreeDimensionResolution(MS_BOOL bExitRepeater)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32Enable = 0;
    MS_U32 u32OutMod = 0;
    MS_U32 u32XCInputSrc = 0;
    MS_U32 u32OutputTiming = E_OUTPUT_TIMING_640X480_60P;
    MS_U32 u32XCWindow = 0;

    u32Enable = 1;
    Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32Enable);


    u32Enable = 0;
    if(Demo_XC_Enable3D(&u32XCDevice, &u32Enable, &u32OutMod)==FALSE)
    {
        printf("[%s][%d]  Dev0 Enable 3D fail!\n",__FUNCTION__,__LINE__);
        //return FALSE;

    }
    if(Demo_XC_PlayVideo_3D(&u32XCDevice, &u32XCInputSrc) == FALSE)
    {
        printf("[%s][%d]  Dev0 Play 3D fail!\n",__FUNCTION__,__LINE__);
        //return FALSE;

    }


    if(bExitRepeater)
    {
        if(Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming)==FALSE)
        {

        }

        //#if CERTIFICATION_HDMI_HDCP_REPEATER
        Demo_Certification_Repeater_Exit();
        //#endif

    }

    printf("[%s][%d]  ================================================================= Disable 3D!\n",__FUNCTION__,__LINE__);

    return TRUE;
}

MS_BOOL Demo_Certification_Change2ThreeDimensionResolution(MS_U32 u32Timing)
{
    E_MSAPI_XC_RESULT eResult;
    MS_BOOL bSupportHDMI = FALSE;
    MS_BOOL bWarn = FALSE;
    MS_U32 u32XCDevice = 0;
    MS_U32 u32Enable = 1;
    MS_U32 u32OutputTiming = _Demo_Certification_Get3DOutputTiming(u32Timing);
    MS_U32 u32OutMod = _Demo_Certification_Get3DMode(u32Timing);
    //HDMITX_VIDEO_ASPECT_RATIO eRatio = HDMITX_VIDEO_AR_16_9;
    MS_U32 u32XCInputSrc = E_DDI_XC_INPUT_SOURCE_HDMI;
    HDMITX_VIDEO_COLOR_FORMAT output ;
    //HDMITX_EDID_3D_STRUCTURE_ALL st3DStructure = 0;

    _eOri3DTiming = u32Timing;

    if((E_3D_RESOLUTION_1920x1080p23d98Hz_24Hz_FRAME_PACKING>u32Timing) || (u32Timing>E_3D_RESOLUTION_1920x1080p29d97Hz_30Hz_TOP_BOTTOM))
    {
        printf("[%s][%d] Invalid Timing %lu\n",__FUNCTION__,__LINE__,u32Timing);
        return FALSE;
    }
    //Demo_Certification_Exit();
    //Demo_Certification_DisableThreeDimensionResolution(TRUE);

    MsOS_DelayTask(500);

    while(_Demo_Certification_CheckHDMITx()==FALSE)
    {
        if(bWarn==FALSE)
        {
            printf(UARTCOLOR_RED"WARNING!!!  HDMI OUTPUT is not attached!\n"UARTCOLOR_NONE);
            bWarn = TRUE;
        }
        MsOS_DelayTask(250);
    }

    if(MApi_HDMITx_EDID_HDMISupport(&bSupportHDMI)==FALSE)
    {
        printf("[%s][%d] EDID Checking is not finished\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    output = _Demo_Certification_getOutColorFromEdid();
    printf(UARTCOLOR_LIGHT_GREEN"[%s][%d]  Try to Set %s %s %s %s\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__,
        _Demo_Certification_Out2String(u32OutputTiming),
        _Demo_Certification_3D2String(u32OutMod),
        (bSupportHDMI==TRUE)?"HDMI":"DVI",
        _Demo_Certification_colorfmt2String(output));

#if 0
    if(MApi_HDMITx_GetRx3DStructureFromEDID(_3DTiming, &st3DStructure)== TRUE)
    {
        //// 7-38 edid 9
        printf("[%s][%d] TX st3DStructure :%d\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__,st3DStructure);
        if((st3DStructure & HDMITX_EDID_3D_FramePacking) == 0)
        {
            printf(UARTCOLOR_LIGHT_RED"[%s][%d] Not 3D Output\r\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__);
            u32Enable = 0;
        }
    }
#endif

    u32XCDevice = 0;

    Demo_XC_Init(&u32XCDevice);
    MsOS_DelayTask(250);

    #if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
    Demo_Certification_InitRepeater();
    #endif

    if(bSupportHDMI==FALSE)
    {
        eResult = msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_DVI);
    }
    else
    {
        eResult = msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_HDMI);
    }

    MsOS_DelayTask(250);


    if(Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming)==FALSE)
    {
        printf("[%s][%d]  Dev0 Setting Timing fail!\n",__FUNCTION__,__LINE__);
        //return FALSE;
    }
    MsOS_DelayTask(250);
    if(Demo_XC_Enable3D(&u32XCDevice, &u32Enable, &u32OutMod)==FALSE)
    {
        printf("[%s][%d]  Dev0 Enable 3D fail!\n",__FUNCTION__,__LINE__);
        //return FALSE;

    }
    MsOS_DelayTask(250);
    if(Demo_XC_PlayVideo_3D(&u32XCDevice, &u32XCInputSrc) == FALSE)
    {
        printf("[%s][%d]  Dev0 Play 3D fail!\n",__FUNCTION__,__LINE__);
        //return FALSE;

    }
    MsOS_DelayTask(250);


    if(eResult != E_MSAPI_XC_OK)
    {
        printf("[%s][%d]  Dev0 Play 3D HDMI!\n",__FUNCTION__,__LINE__);
    }

    MsOS_DelayTask(250);
#if 0
    if(u32Enable == 0)
    {
        MSAPI_XC_DEVICE_ID stXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
        //// 7-38 edid 1
        printf(UARTCOLOR_RED"[%s][%d] TX not support 3D \r\n",__FUNCTION__,__LINE__);

        // - If the source is HDMIRx input, the 3D input mode will be auto detect in HDMI Rx monitor
        // - The input parameter 'en3DInMode' is don't care if the source is HDMIRx
        // - Use 'msAPI_XC_HDMIRx_Get3DMode' to get input 3D mode
        if(E_MSAPI_XC_OK != msAPI_XC_Set3DMode(&stXC_DeviceId, E_MSAPI_XC_3D_INPUT_FRAME_PACKING, E_MSAPI_XC_3D_OUTPUT_MODE_NONE))
        {
            printf("msAPI_XC_Set3DMode failed\n");

        }
        //msAPI_XC_HDMITx_Set3DMode(FALSE,HDMITx_VIDEO_3D_Not_in_Use);
        MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_STOP_PACKET, 0xFF);
        MApi_HDMITx_Set_VS_InfoFrame(HDMITx_VIDEO_VS_Reserved, HDMITx_VIDEO_3D_Not_in_Use, HDMITx_VIDEO_4k2k_Reserved);
        // Set TMDS off->on for compatibility issue for some TVs
        MApi_HDMITx_SetTMDSOnOff(FALSE);
        MsOS_DelayTask(20);
        MApi_HDMITx_SetTMDSOnOff(TRUE);

    }
#endif


    printf(UARTCOLOR_LIGHT_GREEN"[%s][%d]  Finish Setting %s %s %s \n"UARTCOLOR_NONE,__FUNCTION__,__LINE__,
        _Demo_Certification_Out2String(u32OutputTiming),
        _Demo_Certification_3D2String(u32OutMod),
        (bSupportHDMI==TRUE)?"HDMI":"DVI");
    _bFinished3DSetting = TRUE;

    return TRUE;
}
#endif

MS_BOOL Demo_Certification_Change2NormalResolution(MS_U32 u32Timing)
{
    HDMITX_VIDEO_COLOR_FORMAT output = HDMITX_VIDEO_COLOR_YUV444;
    MS_BOOL bWarn = FALSE;
    MS_BOOL bSupportHDMI=FALSE;
    MS_U32 u32XCDevice = 0;
    MS_U32 u32OutputTiming = _Demo_Certification_GetOutputTiming(u32Timing);
    MS_U32 u32CDType = _Demo_Certification_GetColorDepth(u32Timing);
    HDMITX_VIDEO_ASPECT_RATIO eRatio = _Demo_Certification_GetAspect(u32Timing);
    MS_U32 u32XCWindow = E_MSAPI_XC_MAIN_WINDOW;
    MS_U32 u32AS = _Demo_Certification_GetAS(eRatio);
    MS_BOOL bHDCP = FALSE;
    E_MSAPI_XC_RESULT eResult = E_MSAPI_XC_FAIL;
    if((E_RESOLUTION_640x480p_60Hz_4_3_24bit>u32Timing) || (u32Timing>E_RESOLUTION_4096x2160p_60Hz_16_9_48bit))
    {
        printf("[%s][%d] Invalid Timing %lu\n",__FUNCTION__,__LINE__,u32Timing);
        return FALSE;
    }

    while(_Demo_Certification_CheckHDMITx()==FALSE)
    {
        if(bWarn==FALSE)
        {
            printf(UARTCOLOR_RED"WARNING!!!  HDMI OUTPUT is not attached!\n"UARTCOLOR_NONE);
            bWarn = TRUE;
        }
        MsOS_DelayTask(250);
    }

    if(MApi_HDMITx_EDID_HDMISupport(&bSupportHDMI)==FALSE)
    {
        printf("[%s][%d] EDID Checking is not finished\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    output = _Demo_Certification_getOutColorFromEdid();

    printf(UARTCOLOR_LIGHT_GREEN"[%s][%d]  Try to Set %s %s %s  %s  %s\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__,
        _Demo_Certification_Out2String(u32OutputTiming),
        _Demo_Certification_CD2string(u32CDType),
        _Demo_Certification_as2String(eRatio),
        (bSupportHDMI==TRUE)?"HDMI":"DVI",
        _Demo_Certification_colorfmt2String(output));

    if(bSupportHDMI==FALSE)
    {
        eResult = msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_DVI);
    }
    else
    {
        eResult = msAPI_XC_SetHDMITxMode(E_MSAPI_XC_HDMITX_OUTPUT_HDMI);
    }

    u32XCDevice = 0;

    #if (HDMI_CTS_CERTIFICATION_HDCP_SOURCE || HDMI_CTS_CERTIFICATION_HDCP_REPEATER)
    Demo_HDMI_InitHDCP(&bHDCP);
    #endif

    if(Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming)==FALSE)
    {
        printf("[%s][%d]  Dev0 Setting Timing fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    Demo_XC_SetAS(&u32XCDevice, &u32XCWindow, &u32AS);

    MApi_HDMITx_SetVideoOutputAsepctRatio(eRatio);

    if(Demo_HDMI_SetOutputColorDepth(&u32CDType)==FALSE)
    {
        printf("[%s][%d]  Color Depth fail!\n",__FUNCTION__,__LINE__);
        return FALSE;

    }
    {
        u32XCDevice = 0;

        MS_U32 u32XCWindow=0;
        MS_U32 u32XCInputSrc=E_DDI_XC_INPUT_SOURCE_HDMI;

        if(Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc)==FALSE)
        {
            printf("[%s][%d]  Dev0 Play Video fail!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

    }


    if(output == HDMITX_VIDEO_COLOR_YUV444)
    {

    }
    else if(output == HDMITX_VIDEO_COLOR_RGB444)
    {

    }

    printf(UARTCOLOR_LIGHT_GREEN"[%s][%d]  Finish setting %s %s %s  %s  %s\n"UARTCOLOR_NONE,__FUNCTION__,__LINE__,
        _Demo_Certification_Out2String(u32OutputTiming),
        _Demo_Certification_CD2string(u32CDType),
        _Demo_Certification_as2String(eRatio),
        (bSupportHDMI==TRUE)?"HDMI":"DVI",
        _Demo_Certification_colorfmt2String(output));

    printf("[%s][%d]  %s!\n",__FUNCTION__,__LINE__,_Demo_Certification_xcRet2String(eResult));
    return TRUE;

}

MS_BOOL Demo_Certification_HDMISettingReset(void)
{
    MS_BOOL bWarn = FALSE;
    MS_BOOL bSupportHDMI=FALSE;

    while(_Demo_Certification_CheckHDMITx()==FALSE)
    {
        if(bWarn==FALSE)
        {
            printf(UARTCOLOR_RED"WARNING!!!  HDMI OUTPUT is not attached!\n"UARTCOLOR_NONE);
            bWarn = TRUE;
        }
        MsOS_DelayTask(250);
    }

    if(MApi_HDMITx_EDID_HDMISupport(&bSupportHDMI)==FALSE)
    {
        printf("[%s][%d] EDID Checking is not finished\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MS_BOOL bEnable = FALSE;
    MS_U32 u32HDMITxMode = 0;
    if(bSupportHDMI==FALSE)
    {
        u32HDMITxMode = 2;
        if(_Demo_Certification_SetTxOutputMode(bEnable, u32HDMITxMode)==E_MSAPI_XC_FAIL)
        {
            printf("[%s][%d] Set HDMITx DVI Out mode Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        u32HDMITxMode = 0;
        if(_Demo_Certification_SetTxOutputMode(bEnable, u32HDMITxMode)==E_MSAPI_XC_FAIL)
        {
            printf("[%s][%d] Set HDMITx HDMI Out mode Fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL Demo_Certification_ChangeHDMISetting_TxMode(E_CERTIFICATION_OUTTYPE eTxMode)
{
    MS_BOOL bEnable = TRUE;
    E_MSAPI_XC_RESULT eResult = E_MSAPI_XC_FAIL;
    MS_U32 u32HDMITxMode = 0;

    if((E_CERT_DVI>eTxMode) || (eTxMode>E_CERT_HDMI))
    {
        printf("[%s][%d] Invalid Tx mode %u\n",__FUNCTION__,__LINE__,eTxMode);
        return FALSE;
    }

    switch(eTxMode)
    {
        case E_CERT_DVI:
            u32HDMITxMode = 2;
            eResult = _Demo_Certification_SetTxOutputMode(bEnable, u32HDMITxMode);
            break;
        case E_CERT_HDMI:
            u32HDMITxMode = 0;
            eResult = _Demo_Certification_SetTxOutputMode(bEnable, u32HDMITxMode);
            break;
        default:
            break;
    }

    if(eResult==E_MSAPI_XC_FAIL)
    {
        printf("[%s][%d] Set HDMITx %s Out mode Fail\n", __FUNCTION__, __LINE__, (eTxMode==E_CERT_DVI)?"DVI":"HDMI");
        return FALSE;
    }

    return TRUE;
}

MS_BOOL Demo_Certification_ChangeHDMISetting_OutputColorSpace(E_CERTIFICATION_OUTPUTCOLORSPACE eHDMIOutputColor)
{
    E_MSAPI_XC_HDMITX_OUTPUT_COLOR eOutputColor = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;

    if((E_CERT_RGB444>eHDMIOutputColor) || (eHDMIOutputColor>E_CERT_YCBCR420))
    {
        printf("[%s][%d] Invalid Color Space %u\n",__FUNCTION__,__LINE__,eHDMIOutputColor);
        return FALSE;
    }

    switch(eHDMIOutputColor)
    {
        case E_CERT_RGB444:
            eOutputColor = E_MSAPI_XC_HDMITX_OUTPUT_RGB444;
            break;
        case E_CERT_YCBCR422:
            eOutputColor = E_MSAPI_XC_HDMITX_OUTPUT_YUV422;
            break;
        case E_CERT_YCBCR444:
            eOutputColor = E_MSAPI_XC_HDMITX_OUTPUT_YUV444;
            break;
#if (DEMO_4K2K_TEST == 1)
        case E_CERT_YCBCR420:
            eOutputColor = E_MSAPI_XC_HDMITX_OUTPUT_YUV420;
            break;
#endif
        default:
            break;
    }

    msAPI_XC_SetHDMITxColorFormat(E_MSAPI_XC_HDMITX_OUTPUT_YUV444, eOutputColor, FALSE);

    return TRUE;
}

void Demo_Certification_CmdProcess(void)
{
    MS_U8 u8CMDArray[MAX_CMD_SIZE];

    MS_U16 selection = 0xff;
    MS_U8 u8TimingTestItem=0xFF;
    _u8HDMI_TEST_TYPE = 0xFF;
    _3DTiming = HDMITX_RES_MAX;
    MS_U8 u8Layer3_L2TestItem=0xFF;

    while(1)
    {

        if(NULL != (fgets((char*)u8CMDArray, MAX_CMD_SIZE, stdin)))
        {

            selection = atoi((const char *)u8CMDArray);

            printf("u32Layer=%lu  selection=%d\n",u32Layer,selection);
            switch(u32Layer)
            {
                case E_CERTIFICATION_LAYER0:

                    if(selection==E_CERTIFICATION_HDMI_DVI)
                    {
                        u8TestItem = E_CERTIFICATION_HDMI_DVI;
                        Demo_Certification_SelectTimingSourceTest();
                    }
                    else if(selection==E_CERTIFICATION_HDCP)
                    {
                        u8TestItem = E_CERTIFICATION_HDCP;
                        Demo_Certification_SelectHDCPTest();

                    }
                    else if(selection==E_CERTIFICATION_CEC)
                    {

                        u8TestItem = E_CERTIFICATION_CEC;

                        #if HDMI_CTS_CERTIFICATION_HDMI_CEC
                        Demo_Certification_SartCEC();
                        printf(UARTCOLOR_LIGHT_GREEN"Finish Setting  %s\n"UARTCOLOR_NONE,_Demo_Certification_Type2String(selection));
                        Demo_Certification_SelectCECTest();
                        #endif

                    }
                    else if(selection==E_CERTIFICATION_EDID)
                    {
                        u8TestItem = E_CERTIFICATION_EDID;
                        Demo_Certification_EDID();
                    }
                    else if(selection==E_CERTIFICATION_DBG)
                    {
                        u8TestItem = E_CERTIFICATION_DBG;
                        Demo_Certification_Dbg();

                    }
                    else if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                    {
                        Demo_Certification_Exit();
                        Demo_Certification_Init();
                    }
                    else
                    {
                        printf("Invalid Selection\n");
                        Demo_Certification_Init();
                    }
                    break;
                case E_CERTIFICATION_LAYER1:
                    if(u8TestItem == E_CERTIFICATION_HDMI_DVI)
                    {
                        if(selection==E_CERTIFICATION_SOURCE_NORMAL)
                        {
                            Demo_Certification_SelectTiming();
                            u8TimingTestItem=E_CERTIFICATION_SOURCE_NORMAL;
                            _u8HDMI_TEST_TYPE = E_CERTIFICATION_SOURCE_NORMAL;
                            _bFinished3DSetting = FALSE;
                        }
                        else if(selection==E_CERTIFICATION_SOURCE_3D)
                        {
                            Demo_Certification_Select3DTiming();
                            u8TimingTestItem=E_CERTIFICATION_SOURCE_3D;
                            _u8HDMI_TEST_TYPE = E_CERTIFICATION_SOURCE_3D;
                        }
                        else if(selection==E_CERTIFICATION_SOURCE_4K2K)
                        {
                            Demo_Certification_Select4K2KTiming();
                            u8TimingTestItem=E_CERTIFICATION_SOURCE_4K2K;
                            _u8HDMI_TEST_TYPE = E_CERTIFICATION_SOURCE_4K2K;
                        }
                        else if(selection==E_CERTIFICATION_SOURCE_HDMISETTING)
                        {
                            Demo_Certification_SelectHDMISetting();
                            u8TimingTestItem=E_CERTIFICATION_SOURCE_HDMISETTING;
                            _u8HDMI_TEST_TYPE = E_CERTIFICATION_SOURCE_HDMISETTING;
                            _bFinished3DSetting = FALSE;
                        }
                        else if((u8CMDArray[0]=='R')||(u8CMDArray[0]=='r'))
                        {
                            Demo_Certification_ResolutionReset();
                            Demo_Certification_SelectTimingSourceTest();
                        }
                        else if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                        {
                            Demo_Certification_Exit();
                            Demo_Certification_Init();
                        }
                        else
                        {
                            printf("Invalid Selection\n");
                            Demo_Certification_SelectTimingSourceTest();
                        }
                    }
                    else if(u8TestItem == E_CERTIFICATION_CEC)
                    {
                        #if HDMI_CTS_CERTIFICATION_HDMI_CEC
                        #if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
                        _bFinished3DSetting = FALSE;
                        #endif
                        if(selection==E_CERT_CEC_ACTIVE_SOURCE)
                        {
                            Demo_CEC_active_source();
                            Demo_Certification_SelectCECTest();
                        }
                        else if(selection==E_CERT_CEC_INACTIVE_SOURCE)
                        {
                            Demo_CEC_inactive_source();
                            Demo_Certification_SelectCECTest();
                        }
                        else if(selection==E_CERT_CEC_SEND_STANDBY)
                        {
                            Demo_CEC_standby();
                            Demo_Certification_SelectCECTest();
                        }
                        else if(selection==E_CERT_CEC_SWITCH_POS1)
                        {
                            Demo_CEC_position_switch(E_CEC_SWITCH_POS1);
                            Demo_Certification_SelectCECTest();
                        }
                        else if(selection==E_CERT_CEC_SWITCH_POS2)
                        {
                            Demo_CEC_position_switch(E_CEC_SWITCH_POS2);
                            Demo_Certification_SelectCECTest();

                        }
                        else if(selection==E_CERT_CEC_RESET)
                        {
                            Demo_CEC_Reset();
                            Demo_Certification_SelectCECTest();
                        }
                        else if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                        {
                            Demo_Certification_Exit();
                            Demo_Certification_Init();
                        }
                        else
                        {
                            printf("Invalid Selection\n");
                            Demo_Certification_SelectCECTest();
                        }
                        #endif

                    }
                    else if(u8TestItem == E_CERTIFICATION_EDID)
                    {
                        #if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
                        _bFinished3DSetting = FALSE;
                        #endif
                        if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                        {
                            _bEDID = FALSE;
                            Demo_Certification_Init();
                        }
                        else if(selection==E_CERTIFICATION_EDID_ON)
                        {
                            _bEDID = TRUE;
                            printf(UARTCOLOR_LIGHT_GREEN"Finish EDID  ON\n"UARTCOLOR_NONE);
                        }
                        else if(selection==E_CERTIFICATION_EDID_OFF)
                        {
                            _bEDID = FALSE;
                            printf(UARTCOLOR_LIGHT_GREEN"Finish EDID  OFF\n"UARTCOLOR_NONE);
                        }
                        else
                        {
                            Demo_Certification_EDID();
                        }

                    }
                    else if(u8TestItem == E_CERTIFICATION_DBG)
                    {
                        if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                        {
                            Demo_Certification_Init();
                        }
                        else if(selection==E_CERTIFICATION_DBG_ON)
                        {
                            msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_DRV);
                            MApi_HDMITx_SetDbgLevel(0xFF);
                            printf(UARTCOLOR_LIGHT_GREEN"Finish DBG  ON\n"UARTCOLOR_NONE);
                        }
                        else if(selection==E_CERTIFICATION_DBG_OFF)
                        {
                            msAPI_XC_SetDebugLevel(E_MSAPI_XC_DBG_LEVEL_NONE);
                            MApi_HDMITx_SetDbgLevel(0x0);
                            printf(UARTCOLOR_LIGHT_GREEN"Finish DBG  OFF\n"UARTCOLOR_NONE);
                        }
                        else
                        {
                            Demo_Certification_Dbg();
                        }
                    }
                    else if(u8TestItem == E_CERTIFICATION_HDCP)
                    {
                        if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                        {
                            Demo_Certification_Exit();
                            Demo_Certification_Init();
                        }
                        else if(selection==E_CERTIFICATION_HDCP_REPEATER)
                        {
                            #if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
                            Demo_Certification_SartHDCP_Repeater();
                            printf(UARTCOLOR_LIGHT_GREEN"Finish Setting HDCP Repeater Part\n"UARTCOLOR_NONE);
                            #endif
                        }
                        else if(selection==E_CERTIFICATION_HDCP_SOURCE)
                        {
                            #if (HDMI_CTS_CERTIFICATION_HDCP_SOURCE || HDMI_CTS_CERTIFICATION_HDCP_REPEATER)
                            Demo_Certification_SartHDCP_Source();
                            printf(UARTCOLOR_LIGHT_GREEN"Finish Setting HDCP Source Part\n"UARTCOLOR_NONE);
                            #endif
                        }
                        else
                        {
                            Demo_Certification_SelectHDCPTest();
                        }
                    }

                    break;
                case E_CERTIFICATION_LAYER2:
                    printf("[%s][%d] u8TimingTestItem:%u \n",__FUNCTION__,__LINE__,u8TimingTestItem);
                    if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                    {
                        u8TimingTestItem=E_CERTIFICATION_SOURCE_INVALID;

                        #if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
                        Demo_Certification_ResolutionReset();
                        #endif

                        Demo_Certification_SelectTimingSourceTest();
                    }
                    else if((u8CMDArray[0]=='N')||(u8CMDArray[0]=='n'))
                    {
                        u8TimingTestItem=E_CERTIFICATION_SOURCE_INVALID;

                        Demo_Certification_SelectTimingSourceTest();
                    }
                    else
                    {
                        if(u8TimingTestItem==E_CERTIFICATION_SOURCE_NORMAL)
                        {
                            if(Demo_Certification_Change2NormalResolution(selection)==TRUE)
                            {
                                printf(UARTCOLOR_LIGHT_GREEN"Finish Setting  %s\n"UARTCOLOR_NONE,_Demo_Certification_resolution2string((E_CERTIFICATION_RESOLUTION) selection));
                            }
                            else
                            {
                                Demo_Certification_SelectTiming();
                            }
                        }
                        else if(u8TimingTestItem==E_CERTIFICATION_SOURCE_4K2K)
                        {
                            if(Demo_Certification_Change2NormalResolution(selection)==TRUE)
                            {
                                printf(UARTCOLOR_LIGHT_GREEN"Finish Setting 4K2K timing %s\n"UARTCOLOR_NONE,_Demo_Certification_resolution2string((E_CERTIFICATION_RESOLUTION) selection));
                            }
                            else
                            {
                                Demo_Certification_Select4K2KTiming();
                            }
                        }
                        else if(u8TimingTestItem==E_CERTIFICATION_SOURCE_3D)
                        {
                            #if HDMI_CTS_CERTIFICATION_HDMI_SOURCE_3D
                            if(Demo_Certification_Change2ThreeDimensionResolution(selection)==TRUE)
                            {
                                printf(UARTCOLOR_LIGHT_GREEN"Finish Setting  %s\n"UARTCOLOR_NONE,_Demo_Certification_3Dresolution2string(selection));
                            }
                            else{
                                Demo_Certification_Select3DTiming();
                            }
                            #endif
                        }
                        else if(u8TimingTestItem==E_CERTIFICATION_SOURCE_HDMISETTING)
                        {
                            if(selection==E_CERTIFICATION_HDMISETTING_TXMODE)
                            {
                                Demo_Certification_SelectHDMITxMode();
                                u8Layer3_L2TestItem = E_CERTIFICATION_HDMISETTING_TXMODE;
                            }
                            else if(selection==E_CERTIFICATION_HDMISETTING_COLORSPACE)
                            {
                                Demo_Certification_SelectOutputColorSpace();
                                u8Layer3_L2TestItem = E_CERTIFICATION_HDMISETTING_COLORSPACE;
                            }
                            else
                            {
                                Demo_Certification_SelectHDMISetting();
                            }
                        }
                        else
                        {
                            printf("Invalid Selection\n");
                            if(u8TimingTestItem==E_CERTIFICATION_SOURCE_NORMAL)
                            {
                                Demo_Certification_SelectTiming();
                            }
                            else if(u8TimingTestItem==E_CERTIFICATION_SOURCE_3D)
                            {
                                Demo_Certification_Select3DTiming();
                            }

                        }


                    }
                    break;
                case E_CERTIFICATION_LAYER3:
                    printf("[%s][%d] u8Layer3_L2TestItem:%u \n",__FUNCTION__,__LINE__,u8Layer3_L2TestItem);
                    if((u8CMDArray[0]=='Q')||(u8CMDArray[0]=='q'))
                    {
                        u8Layer3_L2TestItem=E_CERTIFICATION_HDMISETTING_INVALID;

                        Demo_Certification_HDMISettingReset();

                        Demo_Certification_SelectHDMISetting();
                    }
                    else
                    {
                        if(u8Layer3_L2TestItem==E_CERTIFICATION_HDMISETTING_TXMODE)
                        {
                            if(Demo_Certification_ChangeHDMISetting_TxMode(selection)==TRUE)
                            {
                                printf(UARTCOLOR_LIGHT_GREEN"Finish Setting: Tx mode is %s\n"UARTCOLOR_NONE,_Demo_Certification_Txmode2string((E_CERTIFICATION_OUTTYPE) selection));
                            }
                            else
                            {
                                Demo_Certification_SelectHDMITxMode();
                            }
                        }
                        else if(u8Layer3_L2TestItem==E_CERTIFICATION_HDMISETTING_COLORSPACE)
                        {
                            if(Demo_Certification_ChangeHDMISetting_OutputColorSpace(selection)==TRUE)
                            {
                                printf(UARTCOLOR_LIGHT_GREEN"Finish Setting: Color Space is  %s\n"UARTCOLOR_NONE,_Demo_Certification_ColorSpace2string((E_CERTIFICATION_OUTPUTCOLORSPACE) selection));
                            }
                            else
                            {
                                Demo_Certification_SelectOutputColorSpace();
                            }
                        }
                        else
                        {
                            printf("Invalid Selection\n");
                            if(u8Layer3_L2TestItem==E_CERTIFICATION_HDMISETTING_TXMODE)
                            {
                                Demo_Certification_SelectHDMITxMode();
                            }
                            else if(u8Layer3_L2TestItem==E_CERTIFICATION_HDMISETTING_COLORSPACE)
                            {
                                Demo_Certification_SelectOutputColorSpace();
                            }

                        }
                    }
                    break;
                default:
                    printf("Invalid Selection\n");
                    Demo_Certification_ResolutionReset();
                    Demo_Certification_Init();
                    break;

            }
        }

        MsOS_DelayTask(500);
    }
}



void Demo_Certification_SysInit(void)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32OutputTiming = E_OUTPUT_TIMING_720X480_60P;
    MS_U32 u32XCWindow=0;
    MS_U32 u32XCInputSrc=2;


    {
        u32XCDevice = 0;
        Demo_XC_Init(&u32XCDevice);

        MsOS_DelayTask(100);
		u32XCDevice = 1;
        Demo_XC_Init(&u32XCDevice);
        MsOS_DelayTask(100);
        #if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
        Demo_Certification_InitRepeater();
        #endif

        Demo_VE_Init();
        MsOS_DelayTask(100);
    }
    {
        u32XCDevice = 0;

        if(Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming)==FALSE)
        {
            printf("[%s][%d]  Dev0 Setting Timing fail!\n",__FUNCTION__,__LINE__);
            //return ;
        }
        MsOS_DelayTask(100);
    }
    {
        u32XCDevice = 0;

        u32XCWindow=0;
        u32XCInputSrc=0;

        if(Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInputSrc)==FALSE)
        {
            printf("[%s][%d]  Dev0 Play Video fail!\n",__FUNCTION__,__LINE__);
            //return ;
        }

        MsOS_DelayTask(100);

    }

    //#if(DEMO_AUDIO_TEST == 1)
    MS_U32 u32Audio_Type = 1;
    MS_U32 u32FileFmt = 1;

    Demo_Audio_Init(&u32Audio_Type);
    Demo_Audio_SetMonitor();

    Demo_Audio_Out_Init(&u32FileFmt);
    //#endif

    #if HDMI_CTS_CERTIFICATION_HDCP_REPEATER
    Demo_EEPROM_init();
    /// - Create Demo Monitor Task
    /// - Allocate Memory

    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    if(_Demo_Mon_HDMI_CertTask.pStack == NULL)
    {
        _Demo_Mon_HDMI_CertTask.pStack = MsOS_AllocateMemory(_Demo_Mon_HDMI_CertTask.u32StackSize, s32MstarNonCachedPoolID);
        if(!_Demo_Mon_HDMI_CertTask.pStack)
        {
            printf("Cannot allocate memory\n");
        }
    }

    if(_Demo_Mon_HDMI_CertTask.iId < 0)
    {
        /// - Create Task Application
        _Demo_Mon_HDMI_CertTask.iId = MsOS_CreateTask( Demo_Certification_threadEDID,
                                            (MS_U32)NULL,
                                            _Demo_Mon_HDMI_CertTask.ePriority,
                                            TRUE,
                                            _Demo_Mon_HDMI_CertTask.pStack,
                                            _Demo_Mon_HDMI_CertTask.u32StackSize,
                                            _Demo_Mon_HDMI_CertTask.szName);
        if(_Demo_Mon_HDMI_CertTask.iId < 0)
        {
            printf("Cannot create task\n");
        }
    }
    #endif


}

void Demo_Certification_Callback(void)
{

}

#endif /* _APPDEMO_CERTIFICATION_C_ */
