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

#if(DEMO_AUDIO_MULTI_TEST == 1)
#include "demo_audio_multi.h"
#endif

#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
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
#define db_print(fmt, args...)  while(0);
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
    EN_DEMO_DMX_FLOW DmxPlayback;
    EN_DEMO_DMX_FLOW_INPUT DmxInput;
    int DmxClkInv;
    int DmxExtSync;
    int DmxParallal;
} ST_ZAPPING_CHANNEL_INFO;

typedef struct
{
    EN_DEMO_DMX_FLOW DmxPlayback;
    EN_DEMO_DMX_FLOW_INPUT DmxInput;
    int DmxClkInv;
    int DmxExtSync;
    int DmxParallal;
} ST_ZAPPING_TUNER_INFO;


static EN_ZAPPING_TYPE eZappingType = E_ZAPPING_NORMAL;

static MS_U32 u32LastChannel = E_ZAPPING_CHNUM;

#if(DEMO_VDEC_MULTI_TEST == 1)
static MS_U32 u32LastChannel1 = E_ZAPPING_CHNUM;
#endif

static ST_ZAPPING_CHANNEL_INFO ChannelInfo[E_ZAPPING_CHNUM];
static ST_ZAPPING_TUNER_INFO TunerInfo[E_ZAPPING_TUNERMAX];

#if (DEMO_ZAPPING_FQ_TEST == 1)
static MSAPI_XC_DEVICE_ID _gstXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
EN_AV_Device _geDevice = E_AV_DEVICE_MAIN;
MS_U32 _gu32Ch = 0;

static void* FQ_Buffer[E_ZAPPING_CHNUM] = {NULL};

#define QUOTE(x) (#x)
#define STRINGIZE(x) QUOTE(x)
#define IS_MACRO_DEFINED(y) strcmp(#y, STRINGIZE(y))

typedef struct
{
    MS_U32 u32PhyAddr;
    MS_U32 u32MemType;
} ST_FQ_BUF_TYPE;

static ST_FQ_BUF_TYPE FQ_AllocatedBuf[E_ZAPPING_CHNUM] =  {
#ifdef TSP_FQ_BUF_ADR
    {TSP_FQ_BUF_ADR,  TSP_FQ_BUF_MEMORY_TYPE},
#else
    {0,0},
#endif
#ifdef TSP_FQ_BUF1_ADR
    {TSP_FQ_BUF1_ADR, TSP_FQ_BUF1_MEMORY_TYPE},
#else
    {0,0},
#endif
#ifdef TSP_FQ_BUF2_ADR
    {TSP_FQ_BUF2_ADR, TSP_FQ_BUF2_MEMORY_TYPE},
#else
    {0,0},
#endif
#ifdef TSP_FQ_BUF3_ADR
    {TSP_FQ_BUF3_ADR, TSP_FQ_BUF3_MEMORY_TYPE},
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

    printf("======================ZappingType======================\n");
    printf("0x0 : E_ZAPPING_NORMAL\n");
    printf("0x1 : E_ZAPPING_XC_SEAMLESS\n");
    printf("0x2 : E_ZAPPING_GOP_SEAMLESS\n");
    printf("====================End ZappingType====================\n");

    eZappingType = *pu8ZappingType;

    return TRUE;
}

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

    MS_U32 u32XCDevice = 0;
    MS_U32 u32XCWindow = *peDevice;
    MS_U32 u32XCInpusrc = 0;

    printf("[%s][%d]Demo_Zapping_Channel : %"DTC_MS_U32_u" \n",__FUNCTION__,__LINE__,*pu32Ch);

#if(DEMO_VDEC_MULTI_TEST == 1)
    if (*peDevice >= E_AV_DEVICE_MAX)
    {
        printf("[%s][%d] Wrong Decoder \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
#else
    if (*peDevice > 0)
    {
        printf("[%s][%d] Not support vdec multi \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
#endif

    if((*pu32Ch >= E_ZAPPING_CHNUM) || (*pu32Ch < E_ZAPPING_CH1))
    {
        printf("[%s][%d]Error channel number!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    if(*peDevice == E_AV_DEVICE_MAIN && *pu32Ch == u32LastChannel)
    {
        printf("[%s][%d]No channel switch!\n",__FUNCTION__,__LINE__);
        return TRUE;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    if(*peDevice == E_AV_DEVICE_SUB && *pu32Ch == u32LastChannel1)
    {
        printf("[%s][%d]No channel switch!\n",__FUNCTION__,__LINE__);
        return TRUE;
    }
#endif


    switch(eZappingType)
    {
        case E_ZAPPING_XC_SEAMLESS:
        #if (DEMO_XC_SEAMLESS_ZAPPING_TEST == 1)
            /***** XC enable freeze image *****/
            //change NO black screen
            printf("[%s][%d]E_ZAPPING_XC_SEAMLESS!\n",__FUNCTION__,__LINE__);

            MS_U32 u32DisplayMute = ENABLE;
            u32XCDevice = 0;

            Demo_XC_EnableSeamlessZapping(&u32XCDevice, &u32XCWindow,&u32DisplayMute);
            Demo_XC_SeamlessZapping_SetFreeze(&u32XCDevice, &u32XCWindow, &u32DisplayMute); //Freeze HDMI
            if(*peDevice == E_AV_DEVICE_MAIN)
            {
                u32XCDevice = 1;
                Demo_XC_EnableSeamlessZapping(&u32XCDevice, &u32XCWindow,&u32DisplayMute);
                Demo_XC_SeamlessZapping_SetFreeze(&u32XCDevice, &u32XCWindow, &u32DisplayMute);//Freeze CVBS
            }
        #else
            printf("[%s][%d]NOT Support E_ZAPPING_XC_SEAMLESS!\n",__FUNCTION__,__LINE__);
            eZappingType = E_ZAPPING_NORMAL;
        #endif
            break;

        case E_ZAPPING_GOP_SEAMLESS:
        #if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
            /***** GOP enable freeze image *****/
            //change NO black screen
            printf("[%s][%d]E_ZAPPING_GOP_SEAMLESS!\n",__FUNCTION__,__LINE__);

            MS_BOOL bDisplayMute = ENABLE;

#if (DEMO_XC_DUALXC_TEST == 1)
            u32XCDevice = 0;
            Demo_AV_PauseAV((EN_AV_Device*)&u32XCWindow);
            Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32XCWindow, &bDisplayMute);

            if(msAPI_VE_GetSourceType() != E_MSAPI_VE_SOURCE_SCALER_OP2)
            {
                if(*peDevice == E_AV_DEVICE_MAIN)
                {
                    u32XCDevice = 1;
                    Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32XCWindow, &bDisplayMute);
                }
            }
#else
            Demo_XC_EnableStillImage(&bDisplayMute);
#endif //(DEMO_XC_DUALXC)
        #else
            printf("[%s][%d]NOT Support E_ZAPPING_GOP_SEAMLESS!\n",__FUNCTION__,__LINE__);
            eZappingType = E_ZAPPING_NORMAL;
        #endif
            break;

        case E_ZAPPING_NORMAL:
        default:
            printf("[%s][%d]NORMAL_SEAMLESS_ZAPPING!!!\n",__FUNCTION__,__LINE__);
            break;
    }


    /***** DigiTuner *****/

    if(*peDevice == E_AV_DEVICE_MAIN)
    {
        if(u32LastChannel == E_ZAPPING_CHNUM)  // First time
        {
            if(!appDemo_DigiTuner_SetFreq(&ChannelInfo[*pu32Ch].u32Freq, &ChannelInfo[*pu32Ch].u32QAM, &ChannelInfo[*pu32Ch].u32SymbolRate))
            {
                return FALSE;
            }
        }
        else
        {
            if(ChannelInfo[*pu32Ch].u32Freq != ChannelInfo[u32LastChannel].u32Freq)
            {
                if(!appDemo_DigiTuner_SetFreq(&ChannelInfo[*pu32Ch].u32Freq, &ChannelInfo[*pu32Ch].u32QAM, &ChannelInfo[*pu32Ch].u32SymbolRate))
                {
                    return FALSE;
                }
            }
        }
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if(*peDevice == E_AV_DEVICE_SUB)
    {
        if(u32LastChannel1 == E_ZAPPING_CHNUM)  // First time
        {
            if(!appDemo_DigiTuner_SetFreq(&ChannelInfo[*pu32Ch].u32Freq, &ChannelInfo[*pu32Ch].u32QAM, &ChannelInfo[*pu32Ch].u32SymbolRate))
            {
                return FALSE;
            }
        }
        else
        {
            if(ChannelInfo[*pu32Ch].u32Freq != ChannelInfo[u32LastChannel1].u32Freq)
            {
                if(!appDemo_DigiTuner_SetFreq(&ChannelInfo[*pu32Ch].u32Freq, &ChannelInfo[*pu32Ch].u32QAM, &ChannelInfo[*pu32Ch].u32SymbolRate))
                {
                    return FALSE;
                }
            }
        }
    }
#endif
    Demo_AV_StopAV(peDevice);

    MS_U8 u8TunerIndex = appDemo_DigiTuner_GetIndex();

    Demo_AV_Tuner_Config(peDevice,&TunerInfo[u8TunerIndex].DmxPlayback,&TunerInfo[u8TunerIndex].DmxInput,&TunerInfo[u8TunerIndex].DmxClkInv,&TunerInfo[u8TunerIndex].DmxExtSync,&TunerInfo[u8TunerIndex].DmxParallal);

    /***** SetPid *****/
    if(!Demo_AV_TSP_SetPid(peDevice,&ChannelInfo[*pu32Ch].u32VideoPID, &ChannelInfo[*pu32Ch].u32AudioPID, &ChannelInfo[*pu32Ch].u32PCRPID, &ChannelInfo[*pu32Ch].u32VdecType, &ChannelInfo[*pu32Ch].u32AdecType))
    {
        return FALSE;
    }


    if( TRUE != Demo_AV_PlayAV(peDevice))
    {
        printf("[%s][%d]Demo_AV_PlayAV is Fail!!\n Please Check Out the Code Flow!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

#if 0
#if ENABLE_MIU_1
#else
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = Demo_VDEC_GetStreamID(eDevice);

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

    if (*peDevice == E_AV_DEVICE_MAIN)
    {
        u32XCDevice = 0;
        u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_DTV;
        printf("[%s][%d]Demo_XC_PlayVideo u32XCDevice:%"DTC_MS_U32_d"  u32XCWindow:%"DTC_MS_U32_d" FromDTV\n",__FUNCTION__,__LINE__,u32XCDevice,u32XCWindow);
        Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInpusrc);
#if (DEMO_XC_DUALXC_TEST == 1)
        u32XCDevice = 1;

#ifndef temp_Kano
u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_DTV;
#else
u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_SCALER0_OP;
//Check SC0 GOP seamless zapping can go through SC1
//Do not change OSD2VE path when SC1 video play. SC1 video will shake.
MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
#endif
        Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInpusrc);
#endif
        Demo_VE_PlayVideo();
    }
#if (DEMO_VDEC_MULTI_TEST == 1)
    else if(*peDevice == E_AV_DEVICE_SUB)
    {
        u32XCDevice = 0;
        u32XCInpusrc = E_DDI_XC_INPUT_SOURCE_DTV2;
        printf("[%s][%d]Demo_XC_PlayVideo u32XCDevice:%"DTC_MS_U32_d"  u32XCWindow:%"DTC_MS_U32_d" FromDTV2\n",__FUNCTION__,__LINE__,u32XCDevice,u32XCWindow);
        Demo_XC_PlayVideo(&u32XCDevice, &u32XCWindow, &u32XCInpusrc);
    }
#endif

    if(eZappingType == E_ZAPPING_GOP_SEAMLESS)
    {
#if (DEMO_GOP_SEAMLESS_ZAPPING_TEST == 1)
        /***** GOP enable freeze image *****/
        //change NO black screen
        MS_BOOL bDisplayMute = DISABLE;

#if (DEMO_XC_DUALXC_TEST == 1)
        u32XCDevice = 0;
        Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32XCWindow, &bDisplayMute);

        if(msAPI_VE_GetSourceType() != E_MSAPI_VE_SOURCE_SCALER_OP2)
        {
            if(*peDevice == E_AV_DEVICE_MAIN)
            {
                u32XCDevice = 1;
                Demo_XC_EnableStillImage_ByDevice(&u32XCDevice, &u32XCWindow, &bDisplayMute);
            }
        }
#else
        Demo_XC_EnableStillImage(&bDisplayMute);
#endif //(DEMO_XC_DUALXC)

#endif
    }

    if(*peDevice == E_AV_DEVICE_MAIN)
    {
        u32LastChannel = *pu32Ch;
    }
#if(DEMO_VDEC_MULTI_TEST == 1)
    else if(*peDevice == E_AV_DEVICE_SUB)
    {
        u32LastChannel1 = *pu32Ch;
    }
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief get ZappingType
/// @param[out] ZappingType
/// @return TRUE: success.
/// @return FALSE: Process fail or Invalid input argument.
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Demo_Zapping_GetZappingType(EN_ZAPPING_TYPE* peZappingType)
{
    *peZappingType = eZappingType;
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


    if(*pu32Ch >= E_ZAPPING_CHNUM)
    {
        db_print("Channel index is wrong !!\n");
        return FALSE;
    }

    if((u32FQEng = Demo_DMX_FlowToFQEngMapping(ChannelInfo[*pu32Ch].DmxPlayback)) == E_DMX_FLOW_INVALID)
    {
        db_print("DMX flow is wrong !!\n");
        return FALSE;
    }

    if(_IsFQ_PreAllocated(u32FQEng))
    {
        db_print("use pre-allocated buffer !!\n");
        u32PhyAddr = MEM_ADR_BY_MIU(FQ_AllocatedBuf[u32FQEng].u32PhyAddr, FQ_AllocatedBuf[u32FQEng].u32MemType);
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
    }

    stFQInfo.u8AddrMode = FALSE;
    stFQInfo.u32BufStart = u32PhyAddr;
    stFQInfo.u32BufSize = ZAPPING_FQ_BUF_LEN;

    if(MApi_DMX_FQ_Init(u32FQEng, &stFQInfo) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MsOS_DelayTask(*pu32FillFQTime);//for FQ filling

    eZappingType = E_ZAPPING_FQ;
    return TRUE;
}

MS_BOOL Demo_FQ_Eng_Exit(MS_U32* pu32Ch)
{
    MS_U32  u32FQEng;

    if(*pu32Ch >= E_ZAPPING_CHNUM)
    {
        db_print("Channel index is wrong !!\n");
        return FALSE;
    }

    if((u32FQEng = Demo_DMX_FlowToFQEngMapping(ChannelInfo[*pu32Ch].DmxPlayback)) == E_DMX_FLOW_INVALID)
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
    MS_U32 u32AudioType = AUDIO_APP_DTV;
    MS_U32 u32During = 0;
    MS_U32 u32XCWindow = E_MSAPI_XC_MAIN_WINDOW;
    VDEC_StreamId* pstVDECStreamId = Demo_VDEC_GetStreamID(*peDevice);
    EN_PCR_ENG eSetSTC = E_PCR_ENG0;
    MS_U32 u32DisplayMute = TRUE;
    ST_AV_INFO stAVInfo;

    printf("appDemo_FQ_Zapping \n");
    u32During = MsOS_GetSystemTime();

    MS_BOOL bMute = TRUE;
    Demo_Audio_SetMute(&bMute);

    Demo_VE_SetVideoMute(&u32DisplayMute);
    msAPI_XC_SetVideoMuteByMode_EX(&_gstXC_DeviceId, ENABLE, u32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

    /***** SetFlow *****/
    Demo_AV_SetAVInfo(*peDevice,E_AV_INFO_DMX_FLOW,(void *)(&ChannelInfo[*pu32Ch].DmxPlayback));


    /***** SetPid *****/
    if(!Demo_AV_TSP_SetPid(peDevice,&ChannelInfo[*pu32Ch].u32VideoPID, &ChannelInfo[*pu32Ch].u32AudioPID, &ChannelInfo[*pu32Ch].u32PCRPID, &ChannelInfo[*pu32Ch].u32VdecType, &ChannelInfo[*pu32Ch].u32AdecType))
    {
        printf("[%s][%d] Demo_AV_TSP_SetPid fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    Demo_AV_GetAVInfo(*peDevice,&stAVInfo);
    eSetSTC = stAVInfo.stPCRParams.u8PCREngID;

    if (stAVInfo.stVideoParams.u8Filter != INVALID_FILTER_ID)
    {
        MApi_DMX_FQ_SetFltRushPass(stAVInfo.stVideoParams.u8Filter, TRUE);
    }

    if (TRUE != Demo_VDEC_Init(*peDevice, pstVDECStreamId,ChannelInfo[*pu32Ch].u32VdecType,E_VDEC_DDI_SRC_MODE_DTV,eSetSTC)) // default mepg2
    {
        printf("[%s][%d] Demo_VDEC_Init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
        printf("eDevice : %d ,Stream ID After Init  0x%"DTC_MS_U32_x"\n",*peDevice, pstVDECStreamId->u32Id);
    }

    //change sync offset to 120ms, this value will influence lip-sync
    //If you want to change this value, please inform video and audio team at the same time
    if(E_VDEC_EX_OK != MApi_VDEC_EX_AVSyncOn(pstVDECStreamId,TRUE, 180, 0))
    {
        printf("[%s][%d] MApi_VDEC_EX_AVSyncOn fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    //set the threshold of Freerun condition.If the difference of STC and PTS large than
    // threshold, FW will free run. The threshold value is 90KHZ base.
    //set 0xFFFFFFFF to make sure VDEC will keep doing AV-sync till rush data is comsumed.
    if(E_VDEC_EX_OK != MApi_VDEC_EX_SetAVSyncFreerunThreshold(pstVDECStreamId,0xFFFFFFFF))
    {
        printf("[%s][%d] MApi_VDEC_EX_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Set Field Delay count befor VDEC play
    MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT, 8);
    MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT,1);
    MApi_VDEC_EX_SetControl(pstVDECStreamId, E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH,0xFF);

    if (E_VDEC_EX_OK != MApi_VDEC_EX_Play(pstVDECStreamId))
    {
        printf("[%s][%d] MApi_VDEC_EX_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //AVFIFO Reset
    if (TRUE != Demo_DMX_AVFifo_Reset(E_DMX_FLT_TYPE_VID0,FALSE))
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
    }

    MApi_DMX_FQ_RushEnable(Demo_DMX_FlowToFQEngMapping(ChannelInfo[*pu32Ch].DmxPlayback));

    MS_U32 u32DelayTaskTime = 1;
    Demo_VDEC_DelayTaskTime(&u32DelayTaskTime);
    //Check Video Decoder Status
    if( TRUE != Demo_VDEC_CheckDecoderStatus((EN_VDEC_Device*)peDevice))
    {
        db_print("Video Decoder Status is Not Ready!!\n Please Check Out the Code Flow!!\n");
        return FALSE;
    }


    msAPI_XC_SetWin_EX(&_gstXC_DeviceId, NULL, NULL, NULL, u32XCWindow);

    msAPI_VE_SetMode();

    //Disable Blue Screen to avoid garbage video
    u32DisplayMute = FALSE;
    Demo_VE_SetVideoMute(&u32DisplayMute);
    msAPI_XC_SetVideoMuteByMode_EX(&_gstXC_DeviceId, DISABLE, u32XCWindow, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);

    printf("[%s][%d]Check Point 1:  %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,(MsOS_GetSystemTime()-u32During));

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

    Demo_VDEC_IsAVSyncDone(*peDevice);

    printf("[%s][%d]Check Point 2: %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,(MsOS_GetSystemTime()-u32During));
    return TRUE;
}


MS_BOOL Demo_FQ_Init(MS_U32* pu32FillFQTime)
{
    ST_AV_INFO sAVInfo;

    //Get main device dmx info and save to channel 0
    Demo_AV_GetAVInfo(_geDevice,&sAVInfo);
    Demo_Zapping_SaveTunerConfig(&_gu32Ch, &(sAVInfo.stDMXflowParams.eFlow), &(sAVInfo.stDMXflowParams.eDmxInput),&(sAVInfo.stDMXflowParams.DmxClkInv),&(sAVInfo.stDMXflowParams.DmxExtSync),&(sAVInfo.stDMXflowParams.DmxParallal));

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
