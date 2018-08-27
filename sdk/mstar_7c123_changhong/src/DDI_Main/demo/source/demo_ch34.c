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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appTemplate.h
/// @author MStar Semiconductor Inc.
/// @brief  Uranus Sample Code Template
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appTemplate.c
   @brief The Template code for the MDrv_XXX functions

*/

#if (DEMO_CH34_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvCH34.h"
#include "demo_ch34.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
MS_U32 g_u32CH34_Spec = E_CH34_SPEC_DEFAULT;

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief The sample code to initialize ch34
/// @return None
/// @note
/// Command: \b CH34_init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CH34_Init(void)
{
    MDrv_Ch34_Init();
    return TRUE;
}

static MS_BOOL _MappingCH34Mode(MS_U32 channel, MS_U32 videoSys, MS_U32 audioSys,
                                E_CH34_CHANNEL *eChannel, E_CH34_VIDEO_SYS *eVideoSys, E_CH34_AUDIO_SYS *eAudioSys)
{
    if(channel == 0)
    {
        *eChannel = E_CH34_CH3;
    }
    else if(channel == 1)
    {
        *eChannel = E_CH34_CH4;
    }
    else
    {
        return FALSE;
    }

    if(videoSys == 0)
    {
        *eVideoSys = E_CH34_VIDEO_SYS_NTSC;
    }
    else if(videoSys == 1)
    {
        *eVideoSys = E_CH34_VIDEO_SYS_PAL;
    }
    else
    {
        return FALSE;
    }

    if(audioSys == 0)
    {
        *eAudioSys = E_CH34_AUDIO_SYS_MONO;
    }
    else if(audioSys == 1)
    {
        *eAudioSys = E_CH34_AUDIO_SYS_BTSC;
    }
    else if(audioSys == 2)
    {
        *eAudioSys = E_CH34_AUDIO_SYS_BTSC_SAP;
    }
    else if(audioSys == 3)
    {
        *eAudioSys = E_CH34_AUDIO_SYS_A2;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to select CH34
/// @param[in] spec The spec of ch34.
/// @return None
/// @note
/// Command: \b CH34_SelectSpec 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CH34_SelectSpec(MS_U32 *pu32Spec)
{
    if(*pu32Spec == E_CH34_SPEC_ABNT_NBR_15604)
    {
        g_u32CH34_Spec = E_CH34_SPEC_ABNT_NBR_15604;
        printf("Select ABNT NBR 15604 SPEC.\n");
    }
    else
    {
        g_u32CH34_Spec = E_CH34_SPEC_DEFAULT;
        printf("Select default SPEC.\n");
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to Set CH34 DAC Swing Level
/// @param[in] level The DAC Swing Level of ch34.
/// @return None
/// @note
/// Command: \b CH34_SetDACSwingLevel 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CH34_SetDACSwingLevel(MS_U32 *pu32DACSwingLevel)
{
    CH34_SET_DAC_OUTPUT_SWING stCH34_DACSwingLevel = {0};

    stCH34_DACSwingLevel.u32SetDACOutputSwing_Version = SET_DAC_OUTPUT_SWING_VERSION;
    stCH34_DACSwingLevel.u16SetDACOutputSwing_Length = sizeof(CH34_SET_DAC_OUTPUT_SWING);

    switch(*pu32DACSwingLevel)
    {
        case E_DAC_SWING_LEVEL0:
            stCH34_DACSwingLevel.eDACSwingLevel = E_CH34_DAC_SWING_LEVEL0;
            break;
        case E_DAC_SWING_LEVEL1:
            stCH34_DACSwingLevel.eDACSwingLevel = E_CH34_DAC_SWING_LEVEL1;
            break;
        case E_DAC_SWING_LEVEL2:
            stCH34_DACSwingLevel.eDACSwingLevel = E_CH34_DAC_SWING_LEVEL2;
            break;
        case E_DAC_SWING_LEVEL3:
            stCH34_DACSwingLevel.eDACSwingLevel = E_CH34_DAC_SWING_LEVEL3;
            break;
        default:
            stCH34_DACSwingLevel.eDACSwingLevel = E_CH34_DAC_SWING_LEVEL0;
            break;
    }

    if(MDrv_Ch34_GeneralCtrl(CH34_CMD_SET_DAC_OUTPUT_SWING, (void*) &stCH34_DACSwingLevel,sizeof(CH34_SET_DAC_OUTPUT_SWING)))
    {
        printf("Set DAC Swing Level successfully\n");
        return TRUE;
    }
    else
    {
        printf("Set DAC Swing Level failed.\n");
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to Set CH34 Sigma Delta Gain Level
/// @param[in] level The Sigma Delta Gain Level of ch34.
/// @return None
/// @note
/// Command: \b CH34_SetSigmaDeltaGainLevel 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CH34_SetSigmaDeltaGainLevel(MS_U32 *pu32SigmaDeltaGainLevel)
{
    CH34_SET_SIGMA_DELTA_GAIN stCH34_SigmaDeltaGain = {0};

    stCH34_SigmaDeltaGain.u32SetSigmaDeltaGain_Version = SET_SIGMA_DELTA_GAIN_VERSION;
    stCH34_SigmaDeltaGain.u16SetSigmaDeltaGain_Length = sizeof(CH34_SET_SIGMA_DELTA_GAIN);

    switch(*pu32SigmaDeltaGainLevel)
    {
        case E_SIGMA_DELTA_GAIN_LEVEL0:
            stCH34_SigmaDeltaGain.eSigmaDeltaGainLevel = E_CH34_SIGMA_DELTA_GAIN_LEVEL0;
            break;
        case E_SIGMA_DELTA_GAIN_LEVEL1:
            stCH34_SigmaDeltaGain.eSigmaDeltaGainLevel = E_CH34_SIGMA_DELTA_GAIN_LEVEL1;
            break;
        case E_SIGMA_DELTA_GAIN_LEVEL2:
            stCH34_SigmaDeltaGain.eSigmaDeltaGainLevel = E_CH34_SIGMA_DELTA_GAIN_LEVEL2;
            break;
        case E_SIGMA_DELTA_GAIN_LEVEL3:
            stCH34_SigmaDeltaGain.eSigmaDeltaGainLevel = E_CH34_SIGMA_DELTA_GAIN_LEVEL3;
            break;
        default:
            stCH34_SigmaDeltaGain.eSigmaDeltaGainLevel = E_CH34_SIGMA_DELTA_GAIN_LEVEL0;
            break;
    }

    if(MDrv_Ch34_GeneralCtrl(CH34_CMD_SET_SIGMA_DELTA_GAIN, (void*) &stCH34_SigmaDeltaGain,sizeof(CH34_SET_SIGMA_DELTA_GAIN)))
    {
        printf("Set Sigma Delta Gain successfully\n");
        return TRUE;
    }
    else
    {
        printf("Set Sigma Delta Gain failed.\n");
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to set ch34 mode
/// @param[in] channel The channel of ch34.
/// @param[in] videoSys The video system of ch34
/// @param[in] audioSys The audio system of ch34
/// @return None
/// @note
/// Command: \b CH34_SetMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_CH34_SetMode(MS_U32 *channel, MS_U32 *videoSys, MS_U32 *audioSys)
{
    CH34_Drv_Result eRet = En_Drv_CH34_FAIL;

    E_CH34_CHANNEL eChannel = E_CH34_CH3;
    E_CH34_VIDEO_SYS eVideoSys = E_CH34_VIDEO_SYS_NTSC;
    E_CH34_AUDIO_SYS eAudioSys = E_CH34_AUDIO_SYS_MONO;
    CH34_ATTRIBUTE_INFO stCH34Info = {0};

    if(FALSE == _MappingCH34Mode(*channel, *videoSys, *audioSys, &eChannel, &eVideoSys, &eAudioSys))
    {
        printf("Parameter error. please check it.\n");
        return FALSE;
    }
    printf("ch:%d v:%d a:%d\n",eChannel,eVideoSys,eAudioSys);

    if(g_u32CH34_Spec == E_CH34_SPEC_ABNT_NBR_15604)
    {
        stCH34Info.u32AttributeInfo_Version = ATTRIBUTE_INFO_VERSION;
        stCH34Info.u16AttributeInfo_Length = sizeof(CH34_ATTRIBUTE_INFO);
        stCH34Info.eChannel = eChannel;
        stCH34Info.eVideoSys = eVideoSys;
        stCH34Info.eAudioSys = eAudioSys;
        stCH34Info.eSpecSelect = E_CH34_SPEC_ABNT_NBR_15604;
        eRet = MDrv_Ch34_SetAttribute(&stCH34Info);
    }
    else
    {
        eRet = MDrv_Ch34_SetMode(eChannel, eVideoSys, eAudioSys);
    }

    if( eRet == En_Drv_CH34_INVALID_PARAM || eRet == En_Drv_CH34_NotSupport)
    {
        printf("CH34 set Channel Index %ld and VideoSys Index %ld and AudioSys Index %ld Return Type not support.\n", *channel, *videoSys, *audioSys);
        return FALSE;
    }
    else if( eRet == En_Drv_CH34_FAIL)
    {
        printf("CH34 set Channel Index %ld and VideoSys Index %ld and AudioSys Index %ld Failed.\n", *channel, *videoSys, *audioSys);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL Demo_CH34_Help(void)
{
    printf ("------------------------------------Ch34 Demo Help--------------------------------------\n");
    printf("1.CH34_init \n");
    printf("-----Must be called first\n");
    printf("2.CH34_SetMode [channel] [videoSys] [audioSys]\n");
    printf("-----channel : 0 for channel 3, 1 for channel 4.\n");
    printf("-----videoSys : 0 for NTSC, 1 for PAL.\n");
    printf("-----audioSys : 0 for Mono, 1 for BTSC, 2 for A2.\n");
    printf ("---------------------------------End of Ch34 Demo Help----------------------------------\n");
    return TRUE;
}

#endif
