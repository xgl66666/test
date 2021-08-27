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
/// file    drvTuner_CD1616.c
/// @brief  PHILIPS CD1616 DVBC Tuner Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvIIC.h"
#include "drvTuner.h"

#if IF_THIS_DEMOD_INUSE(TUNER_PHILIPS_CD1616LF_GIGH_4)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#if   (FRONTEND_TUNER_CTRL == PIU_IIC_0)
#define IIC_WRITE                   MDrv_IIC_Write
#define IIC_READ                    MDrv_IIC_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_1)
#define IIC_WRITE                   MDrv_IIC1_Write
#define IIC_READ                    MDrv_IIC1_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_NONE)
#define REG(addr)                   (*(volatile MS_U8*)(addr))
#else
#error "FRONTEND_TUNER_CTRL"
#endif


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define CD1616_ADDR                 0xC2
#define CD1616_FREF                 0x06                                // [2:1] 0=50.0kHz, 1=32.5kHz, 3=62.5kHz
#define CD1616_AGC                  0x10                                // [4:3] 0x00: for external RFAGC, 0x10: 108dBuV internal RFAGC
#define CD1616_IF                   36130                               // kHz
#define CD1616_FN(rf)               (MS_U16)( (((MS_U32)(rf + CD1616_IF)) * (MS_U32)(1000/62.5)) / 1000)

#define CD1616_STATUS_POR           0xB8                                // POR=1(0x80) on power-on
#define CD1616_STATUS_POR_MASK      0xB8
#define CD1616_STATUS_FL            0x78                                // FL=1(0x40) PLL is phase locked
#define CD1616_STATUS_FL_MASK       0x78

#define CD11316_LOCK_TIME           1000

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 dmdConfig[] =
{
#if (MS_BOARD_TYPE_SEL == BD_MST098B_D01A) || (MS_BOARD_TYPE_SEL == BD_MST098C_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098D_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01B_S)
      0x01, 0x22, 0x8d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
      0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
      0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
      0x52, 0x52, 0x55,
#endif
};


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

/*
static MS_U8 CD1616_Init(MS_U8 *u8Data)
{

    // Power on reset
    u8Data[0] = 0x00;
    u8Data[1] = 0x00;
    u8Data[2] = 0xF0;
    u8Data[3] = 0x00;
    return 4;
}
*/


static MS_U8 CD1616_SetFreq(MS_U32 u32Frf, MS_U8 *u8Data)
{
    MS_U16              u16Temp;

    // DB1 and DB2 (N14 to N0)
    u16Temp = CD1616_FN(u32Frf);
    u8Data[0] = (MS_U8)(u16Temp >> 8);
    u8Data[1] = (MS_U8)(u16Temp & 0xFF);

    // CB (RSA and RSB)
    u8Data[2] = 0xC0 | CD1616_FREF;

    // BB (AGC, SP)
    u8Data[3] = CD1616_AGC;
    if ((u32Frf + CD1616_FREF) < 167000) // 49M - 167M
    {
        u8Data[3] |= 0x01;
    }
    else if ((u32Frf + CD1616_FREF) < 454000) // 167M - 454M
    {
        u8Data[3] |= 0x02;
    }
    else // 454M - 860M
    {
        u8Data[3] |= 0x04;
    }

    return 4;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_SetFreq(MS_U8 u8TunerIndex, MS_U32 u32Freq)
{
    MS_U8                  u8Len, u8Data[4];

    u8Len = CD1616_SetFreq(u32Freq, u8Data);
    if (IIC_WRITE(CD1616_ADDR, NULL, 0, u8Data, u8Len) == FALSE)
    {
        return FALSE;
    }
#if defined(MS_OPTIMIZE)
    // demod manual tune: 195ms avg.
    // demod auto tune: 360ms avg.
    MsOS_DelayTask(15);
    return TRUE;
#else
    // for CD1616 to lock phase
    MsOS_DelayTask(100);
    if (IIC_READ(CD1616_ADDR, NULL, 0, u8Data, 1) == FALSE)
    {
        return FALSE;
    }
    if ((u8Data[0] & CD1616_STATUS_FL_MASK) == CD1616_STATUS_FL)
    {
        return TRUE;
    }
    return FALSE;
#endif

}


MS_BOOL MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_PowerOnOff(MS_U8 u8TunerIndex, MS_BOOL bPowerOn)
{
    //[TBD]
    return TRUE;
}


MS_BOOL MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Init(MS_U8 u8TunerIndex)
{
/*
    MS_U8                  u8Len, u8Data[4];
    u8Len = CD1616_Init(u8Data);

    if (IIC_WRITE(CD1616_ADDR, NULL, 0, u8Data, u8Len) == FALSE)
    {
        return FALSE;
    }
*/

    return TRUE;
}

MS_U32 MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Loop_Through_On(MS_U8 u8TunerIndex, MS_BOOL bOn)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Stand_By(MS_U8 u8TunerIndex)
{

    return TRUE;
}


MS_U32 MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Wake_Up(MS_U8 u8TunerIndex)
{

    return TRUE;
}

TUNERTAB_ENTRY(tuner_entry_TUNER_PHILIPS_CD1616LF_GIGH_4,"TUNER_PHILIPS_CD1616LF_GIGH_4", TUNER_PHILIPS_CD1616LF_GIGH_4,
            MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Init,
            MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_PowerOnOff,//MDrv_Tuner_PowerOnOff,
            MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_SetFreq,//MDrv_Tuner_SetFreq,
            MDrv_Tuner_Null_SetFreq_S2,
            MDrv_Tuner_Null_GetLock,
            MDrv_Tuner_Null_SetTuner,//MDrv_Tuner_SetFreq,
            MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Loop_Through_On,
            MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Stand_By,
            MDrv_Tuner_PHILIPS_CD1616LF_GIGH_4_Wake_Up
);


#endif // TUNER_PHILIPS_CD1616LF_GIGH_4

