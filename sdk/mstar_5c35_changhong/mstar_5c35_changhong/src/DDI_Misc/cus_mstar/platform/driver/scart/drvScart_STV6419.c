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
/// file    drvScart_STV6419.c
/// @brief  STV6419 Scart Driver Interface
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
#include "drvTVEncoder.h"



#if (MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S)
#define SCART_IIC_PORT    E_HWI2C_PORT1_1
#elif (MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A)
#define SCART_IIC_PORT    E_HWI2C_PORT1_5
#else
#define SCART_IIC_PORT    E_HWI2C_PORT1_5
#endif

#define SCART_IIC_SLAVE_ID    0x94

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

    MS_U8 config = 0x0;
    if(SCART_IIC_PORT >= E_HWI2C_PORT_1)
    {
         MDrv_IIC_InitPort1(SCART_IIC_PORT);
    }
    else
    {
         MDrv_IIC_InitPort0(SCART_IIC_PORT);
    }

    m_ScartOutEnable = TRUE;
    m_bScartOutRGB = TRUE;
    m_bBypassScart = FALSE;
    m_enAspectRatio = E_ASP_1TO1;
    m_bScartInterruptEnable = FALSE;
    m_u8AudioLevelAdjustment = 0;

     if(SCART_IIC_PORT >= E_HWI2C_PORT_1)
    {
         MDrv_IIC_ReadBytesPort1(0x95,0,0,1,&config);
    }
    else
    {
         MDrv_IIC_ReadBytesPort0(0x95,0,0,1,&config);
    }

    printf("[DAC] - SCART_RGB_OUT\n");
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

    _MDrv_SCART_OutSwitch();

    printf("%s: ZCD_FLAG %s IT_FLAG %s SLB_VCR %s\n", __FUNCTION__, ((config & 0x80) ? "TRUE" : "FALSE"), ((config & 0x10) ? "TRUE" : "FALSE"), ((config & 0xC) >> 2 == 0 ? "NA" : ((config & 0xC) >> 2 == 1 ? "<2V" : ((config & 0xC) >> 2 == 2 ? "16/9" : "4/3"))));

    bIsInit = TRUE;
    return TRUE;
}

void MDrv_SetDacOnOffSD(E_DAC_SD_ENABLE_CTRL enCtrl)
{
    MApi_DAC_OnOffSD((E_DAC_SD_ENABLE_CTRL)enCtrl);

}

void _MDrv_SCART_OutSwitch(void)
{
    MS_U8 addr = 0;
    MS_U8 config[4] = {0};

    if (m_bBypassScart)
    {
        config[0] = 0x01;
        config[1] = 0x00;
        if (m_bScartOutRGB)
        {
            // RGB + CVBS
            config[2] = 0x28;
        }
        else
        {
            // Only CVBS
            config[2] = 0x08;
        }
        // SLB TV is driven by SLB VCR & FB from VCR
        config[3] = 0x41;
    }
    else
    {
        if (m_ScartOutEnable)
        {
            config[0] = 0x0A;   // +12 dB for weak audio inputs

            if(m_u8AudioLevelAdjustment)
            {
            /*
               The first method is to send an initial command for volume change
               with the softvolume control function bit activated (set to 0) and,
               after a delay of at least 50 ms, to send a second command for
               the same volume control setting but with the softvolume control function
               disabled (set to 1). In this case the volume setting is applied
               independently of a zero-cross detection.
            */
                config[1] = (m_u8AudioLevelAdjustment << 1);
            }

            if (m_bScartOutRGB)
            {
                config[2] = 0x50;   // RGB + CVBS 
                config[3] = 0x20;   // FB high level
            }
            else
            {
                config[2] = 0x10;   // Only CVBS 
                config[3] = 0x00;   // FB low level
            }

            switch (m_enAspectRatio)
            {
                case E_ASP_1TO1:
                    config[3] |= 0x0A;
                    break;
                case E_ASP_16TO9:
                    config[3] |= 0x14;
                    break;
                case E_ASP_4TO3:
                    config[3] |= 0x1E;
                    break;
                default:
                    break;
            }
        }
        else
        {
            config[0] = 0x00;
            config[1] = 0x00;
            config[2] = 0x00;
            config[3] = 0x00;
        }
    }

    if(m_bScartInterruptEnable)
    {
        printf("%s: m_bScartInterruptEnable = %d\n", __FUNCTION__, m_bScartInterruptEnable);
        config[3] |= 0x80; // for Interrupt enable
        config[3] &= 0xE7;
    }


    addr = 0x00;


    printf("%s: config = {%02x, %02x, %02x, %02x}\n", __FUNCTION__, config[0], config[1], config[2], config[3]);

    if(SCART_IIC_PORT >= E_HWI2C_PORT_1)
    {
        MDrv_IIC_WriteBytesPort1(SCART_IIC_SLAVE_ID,1, &addr, 4, config);
    }
    else
    {
        MDrv_IIC_WriteBytesPort0(SCART_IIC_SLAVE_ID,1, &addr, 4, config);
    }

    if(m_ScartOutEnable && m_u8AudioLevelAdjustment)
    {
        MsOS_DelayTask(50); //usleep(50 * 1000);

        config[1] |= 0x1;

        if(SCART_IIC_PORT >= E_HWI2C_PORT_1)
        {
            MDrv_IIC_WriteBytesPort1(SCART_IIC_SLAVE_ID,1, &addr, 4, config);
        }
        else
        {
            MDrv_IIC_WriteBytesPort0(SCART_IIC_SLAVE_ID,1, &addr, 4, config);
        }

    }

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

    MS_U8 config = 0;

    // Read and Clear interrupt value by I2C command

    if(SCART_IIC_PORT >= E_HWI2C_PORT_1)
    {
         MDrv_IIC_ReadBytesPort1(0x95, 0, 0, 1, &config);
    }
    else
    {
         MDrv_IIC_ReadBytesPort0(0x95, 0, 0, 1, &config);
    }

//    printf("MDrv_SCART_Init config =0x%x\n", config);

    // Re-enable interrupt

    _MDrv_SCART_OutSwitch();

    if(config & 0x10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

MS_BOOL MDrv_SCART_SetInterruptEnable(MS_BOOL bEnable)
{
    if (bIsInit == FALSE)
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    if(bEnable)
    {
        m_bScartInterruptEnable = TRUE;
    }
    else
    {
        m_bScartInterruptEnable = FALSE;
    }

    _MDrv_SCART_OutSwitch();

    return TRUE;
}

MS_BOOL MDrv_SCART_SetAudioLevelAdjustment(MS_U8 u8AudioLevelAdjustment)
{
    if (bIsInit == FALSE)
    {
        printf("SCART not init yet\n");
        return FALSE;
    }

    m_u8AudioLevelAdjustment = u8AudioLevelAdjustment & 0x1F;
    _MDrv_SCART_OutSwitch();

    return TRUE;
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
