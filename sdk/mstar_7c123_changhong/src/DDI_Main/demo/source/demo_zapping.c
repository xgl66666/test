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

#if (DEMO_ZAPPING_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "MsMemory.h"

#include "drvMVOP.h"
#include "drvTVEncoder.h"
#include "drvDTC.h"

#include "apiAUDIO.h"
#include "apiDMX.h"
#include "apiVDEC_EX.h"
#include "apiDAC.h"
#include "apiDigiTuner.h"
#include "msAPI_VE.h"

#include "appDemo_DigiTuner.h"

#include "demo_dmx.h"
#include "demo_audio_common.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_zapping.h"
#include "demo_xc.h"

#if(DEMO_AUDIO_TEST == 1)
#include "demo_audio.h"
#endif

#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/// Zapping AV check device ID
#if (DEMO_VDEC_MULTI_TEST == 1 || DEMO_VDEC_NDECODE_TEST == 1)
#define DEMO_ZAPPING_CHECK_DEVICE_ID(u8Id)                            \
    if (((EN_AV_Device)u8Id >= E_AV_DEVICE_MAX) || ((EN_AV_Device)u8Id == E_AV_DEVICE_INVALID))         \
    {                                                             \
        printf("[%s][%d] Wrong Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#else
#define DEMO_ZAPPING_CHECK_DEVICE_ID(u8Id)                            \
    if (((EN_AV_Device)u8Id > 0) || ((EN_AV_Device)u8Id == E_AV_DEVICE_INVALID))        \
    {                                                             \
        printf("[%s][%d] Not support vdec multi Device ID %d\n",__FUNCTION__,__LINE__, u8Id);      \
        return FALSE;                                             \
    }
#endif
//-------------------------------------------------------------------------------------------------
// Global Definition
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define ZAPPING_DBG              0
#define SIZE_OF_XC_FULL_FUNCTION 0xA00000
#define ZAPPING_FQ_BUF_LEN       0x0000400080

#if ZAPPING_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)
#endif
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_ZAPPING_CH1 = 0,
    E_ZAPPING_CH2,
    E_ZAPPING_CH3,
    E_ZAPPING_CH4,
    E_ZAPPING_CHNUM,
} EN_ZAPPING_CHANNEL;

typedef enum
{
    E_ZAPPING_TUNER1 = 0,
    E_ZAPPING_TUNER2,
    E_ZAPPING_TUNER3,
    E_ZAPPING_TUNER4,
    E_ZAPPING_TUNERMAX,
} EN_ZAPPING_TUNER_Index;


typedef struct
{
    MS_U32 u32Freq;
    MS_U32 u32QAM;
    MS_U32 u32SymbolRate;
    MS_U32 u32VideoPID;
    MS_U32 u32AudioPID;
    MS_U32 u32PCRPID;
    MS_U32 u32VdecType;
    MS_U32 u32AdecType;
} ST_ZAPPING_CHANNEL_INFO;

typedef struct
{
    EN_DEMO_DMX_FLOW DmxPlayback;
    EN_DEMO_DMX_FLOW_INPUT DmxInput;
    int DmxClkInv;
    int DmxExtSync;
    int DmxParallal;
} ST_ZAPPING_TUNER_INFO;

typedef struct
{
    EN_ZAPPING_TYPE eZappingType;
    ST_AV_ShowFrameInfo stShowFrameInfo;
    MS_U32 u32LastChannel;
} ST_ZAPPING_DEVICE_INFO;

static ST_ZAPPING_CHANNEL_INFO ChannelInfo[E_ZAPPING_CHNUM];
static ST_ZAPPING_TUNER_INFO TunerInfo[E_ZAPPING_TUNERMAX];
static ST_ZAPPING_DEVICE_INFO DeviceInfo[E_AV_DEVICE_MAX];


#if (DEMO_ZAPPING_FQ_TEST == 1)
EN_AV_Device _geDevice = E_AV_DEVICE_MAIN;
MS_U32 _gu32Ch = 0;

static void* FQ_Buffer[E_ZAPPING_CHNUM] = {NULL};

#define QUOTE(x) (#x)
#define STRINGIZE(x) QUOTE(x)
#define IS_MACRO_DEFINED(y) strcmp(#y, STRINGIZE(y))

typedef struct
{
    MS_U32 u32PhyAddr;
    MS_U32 u32Size;
    MS_U32 u32MemType;
} ST_FQ_BUF_TYPE;

static ST_FQ_BUF_TYPE FQ_AllocatedBuf[E_ZAPPING_CHNUM] =  {
#ifdef TSP_FQ_BUF_ADR
    {TSP_FQ_BUF_ADR, TSP_FQ_BUF_LEN, TSP_FQ_BUF_MEMORY_TYPE},
#else
    {0,0},
#endif
#ifdef TSP_FQ_BUF1_ADR
    {TSP_FQ_BUF1_ADR, TSP_FQ_BUF1_LEN, TSP_FQ_BUF1_MEMORY_TYPE},
#else
    {0,0},
#endif
#ifdef TSP_FQ_BUF2_ADR
    {TSP_FQ_BUF2_ADR, TSP_FQ_BUF2_LEN, TSP_FQ_BUF2_MEMORY_TYPE},
#else
    {0,0},
#endif
#ifdef TSP_FQ_BUF3_ADR
    {TSP_FQ_BUF3_ADR, TSP_FQ_BUF3_LEN, TSP_FQ_BUF3_MEMORY_TYPE},
#else
    {0,0},
#endif
};

static MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
#endif
//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief zapping use auto PlayVideo
/// @param[in] Device
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
static MS_BOOL _Demo_Zapping_PlayVideo(EN_AV_Device eDevice)
{

    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = eDevice;
    MS_U32 u32XCInpusrc = 0;
    static MS_BOOL bFirstDevice = TRUE;
    EN_AV_ShowFrame_Mode eAVShowFrameMode = E_AV_ShowFrame_MODE_MAX;

    if (Demo_AV_GetAVInfo(&eDevice, E_AV_GetCmd_WindowShowFrameMode, &eAVShowFrameMode) != TRUE)
    {
         printf("[%s][%d]Get Frame mode fail!!!\n",__FUNCTION__,__LINE__);
    }

    switch(eDevice)
    {
        case E_AV_DEVICE_MAIN:
#if (DEMO_VDEC_NDECODE_TEST == 1)
        case E_AV_DEVICE_SECOND:
        case E_AV_DEVICE_THIRD:
        case E_AV_DEVICE_FOURTH:
#endif
            {
                u32XCDevice = 0;
                u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_DTV;
                printf("[%s][%d]Demo_XC_PlayVideo u32XCDevice:%"DTC_MS_U32_d"  u32XCWindow:%"DTC_MS_U32_d" FromDTV\n",__FUNCTION__,__LINE__,u32XCDevice,u32XCWindow);
                Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInpusrc);
#if (DEMO_XC_DUALXC_TEST == 1)
                u32XCDevice = 1;
#if (DEMO_XC_DUALXC_IDENTICAL_DISPLAY_TEST == 1)
                u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_SCALER0_OP;
                //Check SC0 GOP seamless zapping can go through SC1
                //Do not change OSD2VE path when SC1 video play. SC1 video will shake.
                MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
#else
                u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_DTV;
#endif
                if(bFirstDevice &&((eAVShowFrameMode == E_AV_MVOP_MODE)||(eAVShowFrameMode == E_AV_PureMCU_MODE)))
                {
                    Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInpusrc);
                }
#endif
                if(bFirstDevice &&((eAVShowFrameMode == E_AV_MVOP_MODE)||(eAVShowFrameMode == E_AV_PureMCU_MODE)))
                {
                    Demo_VE_PlayVideo();
                }

                bFirstDevice = FALSE;
            }
            break;
#if (DEMO_VDEC_MULTI_TEST == 1)
        case E_AV_DEVICE_SUB:
            {
                u32XCDevice = 0;
                u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_DTV2;
                printf("[%s][%d]Demo_XC_PlayVideo u32XCDevice:%"DTC_MS_U32_d"  u32XCWindow:%"DTC_MS_U32_d" FromDTV2\n",__FUNCTION__,__LINE__,u32XCDevice,u32XCWindow);
                Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInpusrc);
            }
            break;
#endif
        default:
            break;

    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief save tuner frequency and bandwidth info for zapping
/// @param[in] channel (kHz)
/// @param[in] Freq Frequency (kHz)
/// @param[in] u32Arg1 for DVB-T and DVB-T2 means {1, 2, 3} => {6MHz, 7MHz 8MHz}; \n
///                    for DVB-C means {0, 1, 2, 3, 4} => {QAM16, QAM32, QAM64, QAM128, QAM256}; \n
///                    for DVB-S2 means symbol rate;
/// @param[in] u32Arg2 for DVB-C means symbol rate in Ksyms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Zapping_SaveFreq 0 258000 3 (DVB-T, DVB-T2) \n
///                \b Zapping_SaveFreq 1 3940000 2 6875 (DVB-C)    \n
///                \b Zapping_SaveFreq 0 11428 27500 (DVB-S2)       \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Zapping_SaveFreq(MS_U32* pu32Ch, MS_U32* pu32Freq, MS_U32* pu32QAM, MS_U32* pu32SR)
{
    ChannelInfo[*pu32Ch].u32Freq = *pu32Freq;
    ChannelInfo[*pu32Ch].u32QAM = *pu32QAM;
    ChannelInfo[*pu32Ch].u32SymbolRate = *pu32SR;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief save Video / Audio / PCR pid info for zapping
/// @param[in] Main or Sub Device
/// @param[in] VideoPid Video PID
/// @param[in] AudioPid Audio PID
/// @param[in] PCRPid PCR PID
/// @param[in] pu32VCodec Video Codec (0: MPEG 1: H.264 2:AVS )
/// @param[in] pu32ACodec Audio Codec Type
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b Zapping_SavePid 0 101 102 101 0 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Zapping_SavePid(MS_U32* pu32Ch, MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    ChannelInfo[*pu32Ch].u32VideoPID = *pu32VideoPid;
    ChannelInfo[*pu32Ch].u32AudioPID = *pu32AudioPid;
    ChannelInfo[*pu32Ch].u32PCRPID = *pu32PCRPid;
    ChannelInfo[*pu32Ch].u32VdecType = *pu32VCodec;
    ChannelInfo[*pu32Ch].u32AdecType = *pu32ACodec;

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief save Tuner info for zapping
/// @param[in] Tuner index
/// @param[in] Demux Playback Flow Path
/// @param[in] Demux Input Flow Path
/// @param[in] Demux Clock Invert
/// @param[in] Demux use externl sync
/// @param[in] Demux use parallal TS signal
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b Zapping_SaveTunerConfig 0 101 102 101 0 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Zapping_SaveTunerConfig(MS_U32* pu32TunerIndex, EN_DEMO_DMX_FLOW* DmxPlayback, EN_DEMO_DMX_FLOW_INPUT* DmxInput, int* DmxClkInv, int* DmxExtSync, int* DmxParallal)
{
    TunerInfo[*pu32TunerIndex].DmxPlayback = *DmxPlayback;
    TunerInfo[*pu32TunerIndex].DmxInput = *DmxInput;
    TunerInfo[*pu32TunerIndex].DmxClkInv = *DmxClkInv;
    TunerInfo[*pu32TunerIndex].DmxExtSync = *DmxExtSync;
    TunerInfo[*pu32TunerIndex].DmxParallal = *DmxParallal;

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief save Device show frame mode for zapping
/// @param[in] Device
/// @param[in] ShowFrame_Type
/// @param[in] Window
/// @param[in] Window X
/// @param[in] Window Y
/// @param[in] Window Width
/// @param[in] Window Height
/// @param[in] Window Layer
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b Zapping_SaveDeviceInfo 0 0x2 0 0 0 1920 1080 2 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Zapping_SaveDeviceInfo(EN_AV_Device* peDevice, MS_U32* pu32Mode, MS_U32* pu32Window, MS_U16* pu16X, MS_U16* pu16Y, MS_U16* pu16Width, MS_U16* pu16Height, MS_U16* pu16Layer)
{
    printf("================Demo_Zapping_SaveDeviceInfo [Device][ShowFrame_Type][Window][X][Y][Width][Height][Layer]================\n");
    printf("ShowFrame_Type 0x0 : MVOP1_MODE\n");
    printf("ShowFrame_Type 0x1 : MVOP2_MODE\n");
    printf("ShowFrame_Type 0x2 : DIP_MODE\n");
    printf("ShowFrame_Type 0x3 : SWDetile_MODE\n");
    printf("Layer number choose your window layer \n");
    printf("==============End Demo_Zapping_SaveDeviceInfo==============\n");

    DeviceInfo[*peDevice].stShowFrameInfo.bValid = TRUE;
    DeviceInfo[*peDevice].stShowFrameInfo.eDevice = *peDevice;
    DeviceInfo[*peDevice].stShowFrameInfo.eShowFrameMode = *pu32Mode;
    DeviceInfo[*peDevice].stShowFrameInfo.u32Window = *pu32Window;
    DeviceInfo[*peDevice].stShowFrameInfo.u16X = *pu16X;
    DeviceInfo[*peDevice].stShowFrameInfo.u16Y = *pu16Y;
    DeviceInfo[*peDevice].stShowFrameInfo.u16Width = *pu16Width;
    DeviceInfo[*peDevice].stShowFrameInfo.u16Height = *pu16Height;
    DeviceInfo[*peDevice].stShowFrameInfo.u16Layer = *pu16Layer;

    Demo_AV_Initialize(peDevice);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief save ZappingType for zapping
/// @param[in] Device
/// @param[in] ZappingType
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b Zapping_ZappingType_EX 0 0 \n
///                \b Zapping_ZappingType_EX 0 1 \n
///                \b Zapping_ZappingType_EX 0 2 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_Zapping_ZappingType_EX(EN_AV_Device* peDevice,MS_U8* pu8ZappingType)
{

    printf("======================ZappingType======================\n");
    printf("0x0 : E_ZAPPING_NORMAL\n");
    printf("0x1 : E_ZAPPING_XC_SEAMLESS\n");
    printf("0x2 : E_ZAPPING_GOP_SEAMLESS\n");
    printf("0x4 : E_ZAPPING_DIP_SEAMLESS\n");
    printf("====================End ZappingType====================\n");

    if(*pu8ZappingType == DeviceInfo[*peDevice].eZappingType)
    {
        // Initialize - no good place to init
        DeviceInfo[*peDevice].u32LastChannel = E_ZAPPING_CHNUM;
    }

    DeviceInfo[*peDevice].eZappingType = *pu8ZappingType;

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief save ZappingType for zapping
/// @param[in] ZappingType
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
/// Command: \b Zapping_ZappingType 0 \n
///                \b Zapping_ZappingType 1 \n
///                \b Zapping_ZappingType 2 \n
//------------------------------------------------------------------------------

MS_BOOL Demo_Zapping_ZappingType(MS_U8* pu8ZappingType)
{
    EN_AV_Device eDevice = 0;
    Demo_Zapping_ZappingType_EX(&eDevice,pu8ZappingType);

    return TRUE;
}

#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
MS_BOOL Demo_Zapping_GOP_SEAMLESS_Control(EN_AV_Device* peDevice,MS_BOOL *pbDisplayMute)
{
    /***** GOP enable freeze image *****/
    //change NO black screen

#if (DEMO_XC_DUALXC_TEST == 1)

    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = *peDevice;

    if(*pbDisplayMute == ENABLE)
        Demo_AV_PauseAV(peDevice);

    Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32XCWindow, pbDisplayMute);

    if(msAPI_VE_GetSourceType() != E_MSAPI_VE_SOURCE_SCALER_OP2)
    {
        if(*peDevice == E_AV_DEVICE_MAIN)
        {
            u32XCDevice = 1;
            Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32XCWindow, pbDisplayMute);
        }
    }
#else
    Demo_XC_EnableStillImage(pbDisplayMute);
#endif //(DEMO_XC_DUALXC)

    return TRUE;
}
#endif
//------------------------------------------------------------------------------
/// @brief The sample code to zapping channel
/// @param[in] Main or Sub Device or Other Device
/// @param[in] u32Ch channel number
///        0: CH1
///        1: CH2
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Zapping_Channel 0 0\n
//------------------------------------------------------------------------------
MS_BOOL Demo_Zapping_Channel(EN_AV_Device* peDevice,MS_U32* pu32Ch)
{
    printf("[%s][%d]Demo_Zapping_Channel : %"DTC_MS_U32_u" \n",__FUNCTION__,__LINE__,*pu32Ch);

#if(DEMO_VDEC_NDECODE_TEST == 1)
    EN_AV_Device eTempDevice = *peDevice;
    *peDevice = Demo_AV_VDEC_Device_Mapping(*peDevice);
#endif

    DEMO_ZAPPING_CHECK_DEVICE_ID(*peDevice)

    if((*pu32Ch >= E_ZAPPING_CHNUM))
    {
        printf("[%s][%d]Error channel number!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(*pu32Ch == DeviceInfo[*peDevice].u32LastChannel)
    {
        printf("[%s][%d]No channel switch!\n",__FUNCTION__,__LINE__);
        return TRUE;
    }

    switch(DeviceInfo[*peDevice].eZappingType)
    {
        case E_ZAPPING_XC_SEAMLESS:
        #if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
        {
            /***** XC enable freeze image *****/
            //change NO black screen
            printf("[%s][%d]E_ZAPPING_XC_SEAMLESS!\n",__FUNCTION__,__LINE__);

            MS_U32 u32DisplayMute = ENABLE;

            MS_U32 u32XCDevice = 0;
            MS_U32 u32XCWindow = *peDevice;
            MS_U32 u32XCInpusrc = 0;

            Demo_XC_EnableSeamlessZapping(&u32XCDevice, &u32XCWindow,&u32DisplayMute);
            Demo_XC_SeamlessZapping_SetFreeze(&u32XCDevice, &u32XCWindow, &u32DisplayMute); //Freeze HDMI
            if(*peDevice == E_AV_DEVICE_MAIN)
            {
                u32XCDevice = 1;
                Demo_XC_EnableSeamlessZapping(&u32XCDevice, &u32XCWindow,&u32DisplayMute);
                Demo_XC_SeamlessZapping_SetFreeze(&u32XCDevice, &u32XCWindow, &u32DisplayMute);//Freeze CVBS
            }
        }
        #else
            printf("[%s][%d]NOT Support E_ZAPPING_XC_SEAMLESS!\n",__FUNCTION__,__LINE__);
            DeviceInfo[*peDevice].eZappingType = E_ZAPPING_NORMAL;
        #endif
            break;

        case E_ZAPPING_GOP_SEAMLESS:
        #if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
        {
            MS_BOOL bDisplayMute = ENABLE;
            Demo_Zapping_GOP_SEAMLESS_Control(peDevice,&bDisplayMute);
        }
        #else
            printf("[%s][%d]NOT Support E_ZAPPING_GOP_SEAMLESS!\n",__FUNCTION__,__LINE__);
            DeviceInfo[*peDevice].eZappingType = E_ZAPPING_NORMAL;
        #endif
            break;

        case E_ZAPPING_DIP_SEAMLESS:
        #if (DEMO_DMS_TEST == 1)
        {
            MS_BOOL bTRUE = TRUE;
            Demo_AV_SetAVInfo(peDevice,E_AV_SetCmd_NotInitialize,(void *)&bTRUE);

            MS_U32 u32XCDevice = 0;
            MS_U32 u32window = DeviceInfo[*peDevice].stShowFrameInfo.u32Window;
            MS_BOOL bEnable = TRUE;

            Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32window, &bEnable);
        }
        #endif
            printf("[%s][%d]E_ZAPPING_DIP_SEAMLESS!\n",__FUNCTION__,__LINE__);
            break;
        case E_ZAPPING_NORMAL:
        default:
            printf("[%s][%d]NORMAL_SEAMLESS_ZAPPING!!!\n",__FUNCTION__,__LINE__);
            break;
    }

    /***** DigiTuner *****/

    if(DeviceInfo[*peDevice].u32LastChannel == E_ZAPPING_CHNUM)  // First time
    {
        if(!appDemo_DigiTuner_SetFreq(&ChannelInfo[*pu32Ch].u32Freq, &ChannelInfo[*pu32Ch].u32QAM, &ChannelInfo[*pu32Ch].u32SymbolRate))
        {
            return FALSE;
        }
    }
    else
    {
        if(ChannelInfo[*pu32Ch].u32Freq != ChannelInfo[DeviceInfo[*peDevice].u32LastChannel].u32Freq)
        {
            if(!appDemo_DigiTuner_SetFreq(&ChannelInfo[*pu32Ch].u32Freq, &ChannelInfo[*pu32Ch].u32QAM, &ChannelInfo[*pu32Ch].u32SymbolRate))
            {
                return FALSE;
            }
        }
    }

    Demo_AV_StopAV(peDevice);

    MS_U8 u8TunerIndex = appDemo_DigiTuner_GetIndex();
    Demo_AV_Tuner_Config(peDevice,&TunerInfo[u8TunerIndex].DmxPlayback,&TunerInfo[u8TunerIndex].DmxInput,&TunerInfo[u8TunerIndex].DmxClkInv,&TunerInfo[u8TunerIndex].DmxExtSync,&TunerInfo[u8TunerIndex].DmxParallal);

    MS_U32 u32ShowFrameMode = DeviceInfo[*peDevice].stShowFrameInfo.eShowFrameMode;
    Demo_AV_SetShowFrameMode(peDevice, &u32ShowFrameMode, &DeviceInfo[*peDevice].stShowFrameInfo.u32Window, &DeviceInfo[*peDevice].stShowFrameInfo.u16X, &DeviceInfo[*peDevice].stShowFrameInfo.u16Y, &DeviceInfo[*peDevice].stShowFrameInfo.u16Width, &DeviceInfo[*peDevice].stShowFrameInfo.u16Height, &DeviceInfo[*peDevice].stShowFrameInfo.u16Layer);

    /***** SetPid *****/
    if(!Demo_AV_TSP_SetPid(peDevice,&ChannelInfo[*pu32Ch].u32VideoPID, &ChannelInfo[*pu32Ch].u32AudioPID, &ChannelInfo[*pu32Ch].u32PCRPID, &ChannelInfo[*pu32Ch].u32VdecType, &ChannelInfo[*pu32Ch].u32AdecType))
    {
        return FALSE;
    }
#if(DEMO_VDEC_NDECODE_TEST == 1)
    if( TRUE != Demo_AV_PlayAV(&eTempDevice))   //use ori device
#else
    if( TRUE != Demo_AV_PlayAV(peDevice))
#endif
    {
        printf("[%s][%d]Demo_AV_PlayAV is Fail!!\n Please Check Out the Code Flow!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

#if 0
#if ENABLE_MIU_1
#else
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = Demo_VDEC_GetStreamID((EN_VDEC_Device*)peDevice);

    // FOR Single MIU0 we have to reduce bandwidth usage
    if(E_VDEC_EX_OK != MApi_VDEC_EX_DisableDeblocking(&stVDECStreamId,TRUE))
    {
        printf("MApi_VDEC_EX_DisableDeblocking != E_VDEC_EX_OK");
    }

    if(E_VDEC_EX_OK != MApi_VDEC_EX_DisableQuarterPixel(&stVDECStreamId,TRUE))
    {
        printf("MApi_VDEC_EX_DisableQuarterPixel != E_VDEC_EX_OK");
    }
#endif
#endif


#if (DEMO_DMS_TEST == 1)
    if(DeviceInfo[*peDevice].eZappingType != E_ZAPPING_DIP_SEAMLESS)
#endif
    {
        _Demo_Zapping_PlayVideo(*peDevice);
    }

    if(DeviceInfo[*peDevice].eZappingType == E_ZAPPING_GOP_SEAMLESS)
    {
#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
        MS_BOOL bDisplayMute = DISABLE;
        Demo_Zapping_GOP_SEAMLESS_Control(peDevice,&bDisplayMute);
#endif
    }
#if (DEMO_DMS_TEST == 1)
    else if(DeviceInfo[*peDevice].eZappingType == E_ZAPPING_DIP_SEAMLESS)
    {
        MS_U32 u32XCDevice = 0;
        MS_U32 u32window = DeviceInfo[*peDevice].stShowFrameInfo.u32Window;
        MS_BOOL bEnable = FALSE;

        Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32window, &bEnable);
        printf("[%s][%d]E_ZAPPING_DIP_SEAMLESS! disable\n",__FUNCTION__,__LINE__);
    }
#endif

    DeviceInfo[*peDevice].u32LastChannel = *pu32Ch;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief get ZappingType
/// @param[in] Device
/// @param[out] ZappingType
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Demo_Zapping_GetZappingType(EN_AV_Device* peDevice,EN_ZAPPING_TYPE* peZappingType)
{
    *peZappingType = DeviceInfo[*peDevice].eZappingType;
    return TRUE;
}


#if (DEMO_ZAPPING_FQ_TEST == 1)
static MS_BOOL _IsFQ_PreAllocated(MS_U32 u32FQEng)
{
    switch(u32FQEng)
    {
        case 0:
            return IS_MACRO_DEFINED(TSP_FQ_BUF_ADR);
        case 1:
            return IS_MACRO_DEFINED(TSP_FQ_BUF1_ADR);
        case 2:
            return IS_MACRO_DEFINED(TSP_FQ_BUF2_ADR);
        case 3:
            return IS_MACRO_DEFINED(TSP_FQ_BUF3_ADR);
        default:
            db_print("FQ engine number out of range !!\n");
            return FALSE;
    }
}

MS_BOOL Demo_FQ_Eng_Init(MS_U32* pu32Ch,MS_U32* pu32FillFQTime)
{
    DMX_FQ_Info stFQInfo;
    MS_U32  u32FQEng;
    MS_U32  u32PhyAddr;
    MS_U32  u32Size;
    MS_U8 u8TunerIndex = appDemo_DigiTuner_GetIndex();


    if(*pu32Ch >= E_ZAPPING_CHNUM)
    {
        db_print("Channel index is wrong !!\n");
        return FALSE;
    }

    if((u32FQEng = Demo_DMX_FlowToFQEngMapping(TunerInfo[u8TunerIndex].DmxPlayback)) == E_DMX_FLOW_INVALID)
    {
        db_print("DMX flow is wrong !!\n");
        return FALSE;
    }

    if(_IsFQ_PreAllocated(u32FQEng))
    {
        db_print("use pre-allocated buffer !!\n");
        u32PhyAddr = MEM_ADR_BY_MIU(FQ_AllocatedBuf[u32FQEng].u32PhyAddr, FQ_AllocatedBuf[u32FQEng].u32MemType);
        u32Size = FQ_AllocatedBuf[u32FQEng].u32Size;
    }
    else
    {
        db_print("use dynamic-allocated buffer !!\n");
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        if(!FQ_Buffer[u32FQEng] && !(FQ_Buffer[u32FQEng] = MsOS_AllocateMemory(ZAPPING_FQ_BUF_LEN, s32MstarNonCachedPoolID)))
        {
            db_print("FQ buffer allocation fail !!\n");
            return FALSE;
        }
        u32PhyAddr = MsOS_VA2PA((MS_VIRT)FQ_Buffer[u32FQEng]);
        u32Size = ZAPPING_FQ_BUF_LEN;
    }

    stFQInfo.u8AddrMode = FALSE;
    stFQInfo.u32BufStart = u32PhyAddr;
    stFQInfo.u32BufSize = u32Size;

    if(MApi_DMX_FQ_Init(u32FQEng, &stFQInfo) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MsOS_DelayTask(*pu32FillFQTime);//for FQ filling

    DeviceInfo[_geDevice].eZappingType = E_ZAPPING_FQ;
    return TRUE;
}

MS_BOOL Demo_FQ_Eng_Exit(MS_U32* pu32Ch)
{
    MS_U32  u32FQEng;
    MS_U8 u8TunerIndex = appDemo_DigiTuner_GetIndex();

    if(*pu32Ch >= E_ZAPPING_CHNUM)
    {
        db_print("Channel index is wrong !!\n");
        return FALSE;
    }

    if((u32FQEng = Demo_DMX_FlowToFQEngMapping(TunerInfo[u8TunerIndex].DmxPlayback)) == E_DMX_FLOW_INVALID)
    {
        db_print("DMX flow is wrong !!\n");
        return FALSE;
    }

    MApi_DMX_FQ_Exit(u32FQEng);

    if(FQ_Buffer[u32FQEng])
    {
        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
        MsOS_FreeMemory(FQ_Buffer[u32FQEng], s32MstarNonCachedPoolID);
    }

    return TRUE;
}

MS_BOOL Demo_FQ_Exit(void)
{
    MApi_DMX_FQ_Exit(0);
    return TRUE;
}

MS_BOOL Demo_FQ_Eng_Zapping(EN_AV_Device* peDevice,MS_U32* pu32Ch)
{
#if(DEMO_VDEC_NDECODE_TEST == 1)
    *peDevice = Demo_AV_VDEC_Device_Mapping(*peDevice);
#endif

    MS_U32 u32AudioType = AUDIO_APP_DTV;
    MS_U32 u32During = 0;
    MS_U32 u32XCWindow = E_MSAPI_XC_MAIN_WINDOW;
    VDEC_StreamId* pstVDECStreamId = Demo_VDEC_GetStreamID((EN_VDEC_Device*)peDevice);
    EN_PCR_ENG eSetSTC = E_PCR_ENG0;
    MS_U32 u32DisplayMute = TRUE;
    ST_AV_INFO stAVInfo;
    ST_AV_ShowFrameInfo stAvShowFrameInfo;
    MS_U8 u8TunerIndex = appDemo_DigiTuner_GetIndex();

    printf("appDemo_FQ_Zapping \n");
    u32During = MsOS_GetSystemTime();

    MS_BOOL bMute = TRUE;
    Demo_Audio_SetMute(&bMute);

    MS_U32 u32XCDevice = 0;

    Demo_VE_SetVideoMute(&u32DisplayMute);
    Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute);

    /***** SetFlow *****/
    Demo_AV_SetAVInfo(peDevice,E_AV_SetCmd_DMXFlow,(void *)(&TunerInfo[u8TunerIndex].DmxPlayback));

    /***** SetInput *****/
    Demo_AV_SetAVInfo(peDevice,E_AV_SetCmd_DMXInput,(void *)(&TunerInfo[u8TunerIndex].DmxInput));

    /***** SetPid *****/
    if(!Demo_AV_TSP_SetPid(peDevice,&ChannelInfo[*pu32Ch].u32VideoPID, &ChannelInfo[*pu32Ch].u32AudioPID, &ChannelInfo[*pu32Ch].u32PCRPID, &ChannelInfo[*pu32Ch].u32VdecType, &ChannelInfo[*pu32Ch].u32AdecType))
    {
        printf("[%s][%d] Demo_AV_TSP_SetPid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(Demo_AV_GetAVInfo(peDevice,E_AV_GetCmd_LiveInfo,&stAVInfo) == FALSE)
    {
        printf("[%s][%d] Demo_AV_GetAVInfo E_AV_GetCmd_LiveInfo fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    eSetSTC = stAVInfo.stPCRParams.ePCREngID;

    if (stAVInfo.stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_FQ_SetFltRushPass(stAVInfo.stVideoParams.u8Filter, TRUE);
    }

    Demo_AV_GetAVInfo(peDevice, E_AV_GetCmd_WindowInfo, &stAvShowFrameInfo);

    db_print("[%s] eShowFrameMode: %u\n", __FUNCTION__, stAvShowFrameInfo.eShowFrameMode);
    db_print("[%s] u32Window: %u\n", __FUNCTION__, stAvShowFrameInfo.u32Window);
    db_print("[%s] u16X: %u\n", __FUNCTION__, stAvShowFrameInfo.u16X);
    db_print("[%s] u16Y: %u\n", __FUNCTION__, stAvShowFrameInfo.u16Y);
    db_print("[%s] u16Width: %u\n", __FUNCTION__, stAvShowFrameInfo.u16Width);
    db_print("[%s] u16Height: %u\n", __FUNCTION__, stAvShowFrameInfo.u16Height);
    db_print("[%s] u16Layer: %u\n", __FUNCTION__, stAvShowFrameInfo.u16Layer);

    if(Demo_VDEC_Stop((EN_VDEC_Device*)peDevice, pstVDECStreamId) != TRUE)
    {
        printf("[%s][%d] Demo_VDEC_Stop fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MS_U32 u32ShowFrameMode = stAvShowFrameInfo.eShowFrameMode;
    Demo_AV_SetShowFrameMode(peDevice, &u32ShowFrameMode, &stAvShowFrameInfo.u32Window, &stAvShowFrameInfo.u16X, &stAvShowFrameInfo.u16Y, &stAvShowFrameInfo.u16Width, &stAvShowFrameInfo.u16Height, &stAvShowFrameInfo.u16Layer);

    if (TRUE != Demo_VDEC_Init((EN_VDEC_Device*)peDevice, pstVDECStreamId,ChannelInfo[*pu32Ch].u32VdecType,E_VDEC_DDI_SRC_MODE_DTV,eSetSTC)) // default mepg2
    {
        printf("[%s][%d] Demo_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
        printf("eDevice : %d ,Stream ID After Init  0x%"DTC_MS_U32_x"\n",*peDevice, pstVDECStreamId->u32Id);
    }

    if (TRUE != Demo_VDEC_Play((EN_VDEC_Device*)peDevice, pstVDECStreamId, E_AV_SYNC_PCR_MODE))
    {
        printf("[%s][%d] Demo_VDEC_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(E_VDEC_EX_OK != MApi_VDEC_EX_SetAVSyncFreerunThreshold(pstVDECStreamId,0xFFFFFFFF))
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(E_DMX_FLT_TYPE_VID0,FALSE))
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    }

    MApi_DMX_FQ_RushEnable(Demo_DMX_FlowToFQEngMapping(TunerInfo[u8TunerIndex].DmxPlayback));

    MS_U32 u32DelayTaskTime = 1;
    Demo_VDEC_DelayTaskTime(&u32DelayTaskTime);
    //Check Video Decoder Status
    if( TRUE != Demo_VDEC_CheckDecoderStatus((EN_VDEC_Device*)peDevice))
    {
        db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }

    MSAPI_XC_WINDOW_TYPE stDestWin = {0};

    stDestWin.x = stAvShowFrameInfo.u16X;
    stDestWin.y = stAvShowFrameInfo.u16Y;
    stDestWin.width = stAvShowFrameInfo.u16Width;
    stDestWin.height = stAvShowFrameInfo.u16Height;

    if(stAvShowFrameInfo.eShowFrameMode == E_AV_DIP_MODE || stAvShowFrameInfo.eShowFrameMode == E_AV_PureMCU_MODE)
    {
        printf("[%s] DIP or PureMCU_MODE!!!!!!!!!!!\n", __FUNCTION__);

        VDEC_EX_DispInfo stDispinfo;
        memset(&stDispinfo, 0, sizeof(VDEC_EX_DispInfo));
        Demo_VDEC_GetDispInfo((EN_VDEC_Device *)peDevice, &stDispinfo);

        MSAPI_XC_WINDOW_TYPE stCropWin = {0};

        stCropWin.x = 0;
        stCropWin.y = 0;
        stCropWin.width  = stDispinfo.u16HorSize - (stDispinfo.u16CropRight + stDispinfo.u16CropLeft);
        stCropWin.height = stDispinfo.u16VerSize - (stDispinfo.u16CropTop + stDispinfo.u16CropBottom);

        db_print("[%s] stDispinfo.u16HorSize = %u\n", __FUNCTION__, stDispinfo.u16HorSize);
        db_print("[%s] stDispinfo.u16VerSize = %u\n", __FUNCTION__, stDispinfo.u16VerSize);
        db_print("[%s] stDispinfo.u16CropLeft = %u\n", __FUNCTION__, stDispinfo.u16CropLeft);
        db_print("[%s] stDispinfo.u16CropRight = %u\n", __FUNCTION__, stDispinfo.u16CropRight);
        db_print("[%s] stDispinfo.u16CropTop = %u\n", __FUNCTION__, stDispinfo.u16CropTop);
        db_print("[%s] stDispinfo.u16CropBottom = %u\n", __FUNCTION__, stDispinfo.u16CropBottom);
        db_print("------------------------------\n");
        db_print("[%s] stCropWin.width = %u\n", __FUNCTION__, stCropWin.width);
        db_print("[%s] stCropWin.height = %u\n", __FUNCTION__, stCropWin.height);

        MSAPI_XC_DEVICE_ID g_stDIP_DeviceId = {0, E_MSAPI_XC_DEVICE_DIP_0};

        msAPI_XC_SetWin_EX(&g_stDIP_DeviceId, NULL, &stCropWin, &stDestWin, stAvShowFrameInfo.u32Window);
    }
    else if(stAvShowFrameInfo.eShowFrameMode == E_AV_MVOP_MODE)
    {
        printf("[%s] SC !!!!!!!!!!!\n", __FUNCTION__);

        MSAPI_XC_DEVICE_ID _gstXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};

        msAPI_XC_SetWin_EX(&_gstXC_DeviceId, NULL, NULL, &stDestWin, stAvShowFrameInfo.u32Window);
    }

    msAPI_VE_SetMode();

    //Disable Blue Screen to avoid garbage video
    u32DisplayMute = FALSE;
    Demo_VE_SetVideoMute(&u32DisplayMute);
    Demo_XC_SetVideoMute(&u32XCDevice, &u32XCWindow, &u32DisplayMute);

    printf("\033[35m[%s][%d]Check Point 1:  %"DTC_MS_U32_d"  \033[0m\n",__FUNCTION__,__LINE__,(MsOS_GetSystemTime()-u32During));

    //Handle audio later to manifestly demo the improvement of FQ
    Demo_Audio_Stop(*peDevice, u32AudioType);
    if (Demo_Audio_Play(*peDevice, u32AudioType,ChannelInfo[*pu32Ch].u32AdecType, &eSetSTC) == FALSE) // default mepg2
    {
        printf("[%s][%d] Demo_Audio_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
        printf("[%s][%d] Demo_Audio_Play ok\n", __FUNCTION__, __LINE__);
    }

    bMute = FALSE;
    Demo_Audio_SetMute(&bMute);

    if (TRUE != Demo_DMX_AVFifo_Reset(E_DMX_FLT_TYPE_AUD0,FALSE))
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    }

    //recover the AV sync freerun threshold anyway

    MApi_VDEC_EX_SetAVSyncFreerunThreshold(pstVDECStreamId, 450000);

    u32DelayTaskTime = 10;
    Demo_VDEC_DelayTaskTime(&u32DelayTaskTime);

    Demo_VDEC_IsAVSyncDone((EN_VDEC_Device*)peDevice);

    printf("\033[35m[%s][%d]Check Point 2: %"DTC_MS_U32_d" \033[0m\n",__FUNCTION__,__LINE__,(MsOS_GetSystemTime()-u32During));
    return TRUE;
}


MS_BOOL Demo_FQ_Init(MS_U32* pu32FillFQTime)
{
    ST_AV_DMX_INFO sAVDMXInfo;
    MS_U32 u32TunerIndex = (MS_U32)appDemo_DigiTuner_GetIndex();

    //Get main device dmx info and save to channel 0
    Demo_AV_GetAVInfo(&_geDevice,E_AV_GetCmd_DMXFlowInfo,&sAVDMXInfo);

    Demo_Zapping_SaveTunerConfig(&u32TunerIndex, &(sAVDMXInfo.eFlow), &(sAVDMXInfo.eDmxInput),&(sAVDMXInfo.DmxClkInv),&(sAVDMXInfo.DmxExtSync),&(sAVDMXInfo.DmxParallal));

    Demo_FQ_Eng_Init(&_gu32Ch,pu32FillFQTime);

    return TRUE;
}


MS_BOOL Demo_FQ_Zapping(MS_U32* pu32VideoPid, MS_U32* pu32AudioPid, MS_U32* pu32PCRPid, MS_U32* pu32VCodec, MS_U32* pu32ACodec)
{
    //save PIDs info to channel 0
    Demo_Zapping_SavePid(&_gu32Ch,pu32VideoPid,pu32AudioPid,pu32PCRPid,pu32VCodec,pu32ACodec);

    Demo_FQ_Eng_Zapping(&_geDevice,&_gu32Ch);

    return TRUE;
}


#endif
#endif
