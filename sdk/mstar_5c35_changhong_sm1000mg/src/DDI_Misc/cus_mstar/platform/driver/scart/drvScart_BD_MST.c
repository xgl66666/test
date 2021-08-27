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

//////////////////////////////////////////////////////////////////////////////
///
/// file    drvScart_BD_MST124.c
/// @brief  Scart Control for MST124 series board
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "Board.h"
#include "drvIIC.h"
#include "apiDAC.h"
#include "drvScart.h"
#include "drvGPIO.h"
#include "drvTVEncoder.h"

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL m_bBypassScart;
static MS_BOOL m_ScartOutEnable;
static MS_BOOL m_bScartOutRGB;
static MS_BOOL m_bScartInterruptEnable;
static MS_U8 m_u8AudioLevelAdjustment;
static MS_BOOL bIsInit = FALSE;
EN_ASPECT_RATIO_CODE m_enAspectRatio;
EN_DISPLAY_SCART_OUT_STATE m_eScartOutState;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

void _MDrv_SCART_OutSwitch(void);

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


MS_BOOL MDrv_SCART_Init(void)
{
    if (bIsInit)                                                                
    {                                                                           
        printf("\n********** %s was already inited **********\n", __FUNCTION__);
        return FALSE;                                                                 
    }                                                                           

    m_ScartOutEnable = TRUE;
    m_bScartOutRGB = TRUE;
    m_bBypassScart = FALSE;
    m_enAspectRatio = E_ASP_1TO1;
    m_bScartInterruptEnable = FALSE;
    m_u8AudioLevelAdjustment = 0;

    mdrv_gpio_init();

    m_ScartOutEnable = TRUE;
    m_eScartOutState=DISPLAY_SCART_ALL_ENABLE;

    // set CVBS+SCART-RGB                     
    // drvTVEncoder.h                         
    MS_Switch_VE_Dest_Info DestInfo;          
    DestInfo.OutputDstType = MS_VE_DEST_SCART;
    MDrv_VE_SwitchOuputDest(&DestInfo);       

    // SD DAC
    MApi_DAC_SetOutputSource(E_DAC_TO_VE, FALSE);
    MApi_DAC_SetOutputSwapSel(E_DAC_R_R_R, FALSE);
    MApi_DAC_SetOutputLevel(E_DAC_OUTPUT_LEVEL_HIGH, FALSE);

    // HD DAC
    MApi_DAC_SetOutputSource(E_DAC_TO_VE_YPBPR, TRUE);
    MApi_DAC_SetOutputSwapSel(E_DAC_R_G_B, TRUE);

    m_bBypassScart = FALSE;
    m_bScartOutRGB = TRUE;
    _MDrv_SCART_OutSwitch();

    bIsInit = TRUE;
    printf("SCART init ...\n");

    return TRUE;
}

void MDrv_SetDacOnOffSD(E_DAC_SD_ENABLE_CTRL enCtrl)
{
    MApi_DAC_OnOffSD((E_DAC_SD_ENABLE_CTRL)enCtrl);
}

void _MDrv_SCART_OutSwitch(void)
{
#ifdef SUPPORT_SCART_OUT_SWITCH
    if (m_ScartOutEnable)
    {
        if (m_bScartOutRGB)
        {
            mdrv_gpio_set_high(GPIO_SCART_PIN16);
        }
        else
        {
            mdrv_gpio_set_low(GPIO_SCART_PIN16);
        }

        switch (m_enAspectRatio)
        {
            case E_ASP_16TO9: // 6v
                SCART_ASPECT16TO9_GPIO;
            break;
            case E_ASP_4TO3: // 12v
                SCART_ASPECT4TO3_GPIO;
            break;
            case E_ASP_1TO1: // 0v
            default:
                SCART_ASPECT1TO1_GPIO;
            break;
        }
    }
    else
    {
        mdrv_gpio_set_low(GPIO_SCART_PIN16);
    }
#endif	
}

MS_BOOL MDrv_ScartOutEnable(MS_BOOL bEnable)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if (m_ScartOutEnable != bEnable)
    {
        m_ScartOutEnable = bEnable;
        _MDrv_SCART_OutSwitch();
    }
    return TRUE;

}

MS_BOOL MDrv_SCART_SetBypassEnable(MS_BOOL bEnable)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if (m_bBypassScart != bEnable)
    {
        m_bBypassScart = bEnable;
        _MDrv_SCART_OutSwitch();
    }
    return TRUE;
}

MS_BOOL MDrv_SCART_SetOutputMode(EN_DISPLAY_SCART_OUT_STATE eTargetState)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if(m_eScartOutState==eTargetState)
        return TRUE;
    if((m_eScartOutState==DISPLAY_SCART_ALL_ENABLE)&&(eTargetState==DISPLAY_SCART_ALL_DISABLE))
    {
        m_ScartOutEnable = FALSE;
    }
    else if((m_eScartOutState==DISPLAY_SCART_ALL_DISABLE)&&(eTargetState==DISPLAY_SCART_ALL_ENABLE))
    {
         m_ScartOutEnable = TRUE;
    }
    else if((m_eScartOutState==DISPLAY_SCART_ALL_ENABLE)&&(eTargetState==DISPLAY_SCART_COMPOSITE_ONLY))
    {
        MDrv_SetDacOnOffSD((E_DAC_SD_ENABLE_CTRL)E_DAC_SD_X);
        m_bScartOutRGB = FALSE;
    }
    else if((m_eScartOutState==DISPLAY_SCART_ALL_DISABLE)&&(eTargetState==DISPLAY_SCART_COMPOSITE_ONLY))
    {
        m_bScartOutRGB = FALSE;
        MDrv_SetDacOnOffSD((E_DAC_SD_ENABLE_CTRL)E_DAC_SD_X);
        m_ScartOutEnable = TRUE;
    }
    else if((m_eScartOutState==DISPLAY_SCART_COMPOSITE_ONLY)&&(eTargetState==DISPLAY_SCART_ALL_DISABLE))
    {
        m_bScartOutRGB = TRUE;
        m_ScartOutEnable = FALSE;
        MDrv_SetDacOnOffSD((E_DAC_SD_ENABLE_CTRL)E_DAC_SD_XYC);
    }
    else if((m_eScartOutState==DISPLAY_SCART_COMPOSITE_ONLY)&&(eTargetState==DISPLAY_SCART_ALL_ENABLE))
    {
        m_bScartOutRGB = TRUE;
        MDrv_SetDacOnOffSD((E_DAC_SD_ENABLE_CTRL)E_DAC_SD_XYC);
    }
    _MDrv_SCART_OutSwitch();

    m_eScartOutState=eTargetState;

    return TRUE;
}


MS_BOOL MDrv_SCART_SetAspectRatio(EN_ASPECT_RATIO_CODE eAspectRatio)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if ((eAspectRatio < E_ASP_MAXNUM) && (m_enAspectRatio != eAspectRatio)) 
    {
        m_enAspectRatio = eAspectRatio;
        _MDrv_SCART_OutSwitch();
    }

    return TRUE;
}

MS_BOOL MDrv_SCART_GetInterruptOutOnOff(void)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }
    //FIXME, not support now
    return FALSE;
}

MS_BOOL MDrv_SCART_SetInterruptEnable(MS_BOOL bEnable)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }
    //FIXME, not support now
    return FALSE;
}

MS_BOOL MDrv_SCART_SetAudioLevelAdjustment(MS_U8 u8AudioLevelAdjustment)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }
    
    //FIXME, not support now
    return FALSE;
}

MS_BOOL MDrv_SCART_OutRGB(MS_BOOL bEnableRGB)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if (m_bScartOutRGB != bEnableRGB)
    {
        m_bScartOutRGB = bEnableRGB;
     _MDrv_SCART_OutSwitch();
    }
   return TRUE;
}

MS_BOOL MDrv_SCART_OutEnable(MS_BOOL bEnable)
{
    if (bIsInit == FALSE)                                                                
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if (m_ScartOutEnable != bEnable)
    {
        m_ScartOutEnable = bEnable;
        _MDrv_SCART_OutSwitch();
    }
   return TRUE;
}

