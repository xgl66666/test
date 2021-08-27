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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define MHAL_PQ_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvPQ_Define.h"
#include "Keltic_Main.h"              // table config parameter
#include "mhal_pq.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 PQ_RIU_BASE;

void Hal_PQ_set_ve_v_sram_table(MS_U8 u8sramtype, void *pData){}
void Hal_PQ_set_ve_h_sram_table(MS_U8 u8sramtype, void *pData){}
MS_U8 Hal_PQ_get_ve_v_sram_table_index(MS_U8 u8type){ return 0;}
MS_U8 Hal_PQ_get_ve_h_sram_table_index(MS_U8 u8type){ return 0;}

// Put this function here because hwreg_utility2 only for hal.
void Hal_PQ_init_riu_base(MS_U32 u32riu_base)
{
    PQ_RIU_BASE = u32riu_base;
}


MS_U8 Hal_PQ_get_sync_flag(MS_BOOL bMainWin)
{
    MS_U16 u16val;
    MS_U8 u8SyncFlag;
    if(bMainWin)
        u16val = MApi_XC_R2BYTE(REG_SC_BK01_1E_L) & 0x00FF;
    else
        u16val = MApi_XC_R2BYTE(REG_SC_BK03_1E_L) & 0x00FF;

    u8SyncFlag = u16val;
    return u8SyncFlag;
}


MS_U8 Hal_PQ_get_input_vsync_value(MS_BOOL bMainWin)
{
    return (Hal_PQ_get_sync_flag(bMainWin) & 0x04) ? 1:0;
}

MS_U8 Hal_PQ_get_output_vsync_value(MS_BOOL bMainWin)
{
    return (Hal_PQ_get_sync_flag(bMainWin) & 0x01) ? 1 : 0;
}

MS_U8 Hal_PQ_get_input_vsync_polarity(MS_BOOL bMainWin)
{
    if(bMainWin)
       return (MApi_XC_R2BYTE(REG_SC_BK01_1E_L) & 0x100) ? 1:0;
    else
       return (MApi_XC_R2BYTE(REG_SC_BK03_1E_L) & 0x100) ? 1:0;
}

void Hal_PQ_set_memfmt_doublebuffer(MS_BOOL bEn)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK12_40_L, bEn ? 0x03 : 0x00, 0x0003);
}

void Hal_PQ_set_sourceidx(MS_BOOL bMain, MS_U16 u16Idx)
{
    if (bMain)
        MApi_XC_W2BYTE(REG_SC_BK00_36_L, u16Idx);
    else
        MApi_XC_W2BYTE(REG_SC_BK00_37_L, u16Idx);
}


void Hal_PQ_set_mem_fmt(MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK12_02_L, u16val, u16Mask);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK12_42_L, u16val, u16Mask);
}

void Hal_PQ_set_mem_fmt_en(MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK12_02_L, u16val, u16Mask);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK12_42_L, u16val, u16Mask);
}

void Hal_PQ_set_420upSample(MS_U16 u16value)
{
    MApi_XC_W2BYTE(REG_SC_BK21_76_L, u16value);
}

void Hal_PQ_set_force_y_motion(MS_BOOL bMainWin, MS_U16 u16value)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_78_L, u16value, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_78_L, u16value, 0xFF00);
}

MS_U8 Hal_PQ_get_force_y_motion(MS_BOOL bMainWin)
{
    MS_U8 u8val;

    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_78_L) & 0x00FF);
    else
        u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK22_78_L) & 0xFF00)>>8);

    return u8val;
}

void Hal_PQ_set_force_c_motion(MS_BOOL bMainWin, MS_U16 u16value)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_79_L, u16value, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_79_L, u16value, 0xFF00);
}

MS_U8 Hal_PQ_get_force_c_motion(MS_BOOL bMainWin)
{
    MS_U8 u8val;

    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_79_L) & 0x00FF);
    else
        u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK22_79_L) & 0xFF00)>>8);
    return u8val;
}

void Hal_PQ_set_dipf_temporal(MS_BOOL bMainWin, MS_U16 u16val)
{
    if(bMainWin)
        MApi_XC_W2BYTE(REG_SC_BK22_14_L, u16val);
    else
        MApi_XC_W2BYTE(REG_SC_BK22_44_L, u16val);
}

MS_U16 Hal_PQ_get_dipf_temporal(MS_BOOL bMainWin)
{
    if(bMainWin)
        return MApi_XC_R2BYTE(REG_SC_BK22_14_L);
    else
        return MApi_XC_R2BYTE(REG_SC_BK22_44_L);
}

void Hal_PQ_set_dipf_spatial(MS_BOOL bMainWin, MS_U16 u16val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK22_15_L, u16val, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK22_45_L, u16val, 0x00FF);
}

MS_U8 Hal_PQ_get_dipf_spatial(MS_BOOL bMainWin)
{
    if(bMainWin)
        return (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_15_L) & 0x00FF);
    else
        return (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK22_45_L) & 0x00FF);
}

void Hal_PQ_set_vsp_sram_filter(MS_U8 u8vale)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, ((MS_U16)u8vale)<<8, 0xFF00);
}

MS_U8 Hal_PQ_get_vsp_sram_filter(void)
{
    MS_U8 u8val;
    u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK23_0B_L) & 0xFF00) >> 8);
    return u8val;
}

void Hal_PQ_set_dnr(MS_BOOL bMainWin, MS_U8 u8val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK06_21_L, u8val, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK06_01_L, u8val, 0x00FF);
}

MS_U8 Hal_PQ_get_dnr(MS_BOOL bMainWin)
{
    MS_U8 u8val;
    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_21_L) & 0x00FF);
    else
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_01_L) & 0x00FF);
    return u8val;
}

void Hal_PQ_set_presnr(MS_BOOL bMainWin, MS_U8 u8val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK06_22_L, u8val, 0x00FF);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK06_02_L, u8val, 0x00FF);
}

MS_U8 Hal_PQ_get_presnr(MS_BOOL bMainWin)
{
    MS_U8 u8val;

    if(bMainWin)
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_22_L) & 0x00FF);
    else
        u8val = (MS_U8)(MApi_XC_R2BYTE(REG_SC_BK06_02_L) & 0x00FF);

    return u8val;
}

void Hal_PQ_set_film(MS_BOOL bMainWin, MS_U16 u16val)
{
    if(bMainWin)
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_10_L, u16val, 0xC800);
    else
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_10_L, u16val, 0x0700);
}

MS_U8 Hal_PQ_get_film(MS_BOOL bMainWin)
{
    MS_U8 u8val;
    u8val = (MS_U8)((MApi_XC_R2BYTE(REG_SC_BK0A_10_L) & 0xFF00) >> 8);
    if(bMainWin)
        u8val &= 0xC8;
    else
        u8val &= 0x07;
    return u8val;
}

void Hal_PQ_set_c_sram_table(MS_U8 u8sramtype, void *pData)
{
    MS_U8 u8Bank;
    MS_U16 i, j, x;
    MS_U8 u8Ramcode[8];
    MS_U32 u32Addr;

    //In U4 only support SC_FILTER_C_SRAM1 & SC_FILTER_C_SRAM2
    if( (u8sramtype != SC_FILTER_C_SRAM1) &&
        (u8sramtype != SC_FILTER_C_SRAM2))
    {
        printf("Unknown c sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U32)pData;
    u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE);
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0002, 0x0002); // enable c_sram_rw

    for(i=0; i<64; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK23_41_L) & 0x0100)
        {
            //wait here
        }

        j=i*5;

        if(u8sramtype == SC_FILTER_C_SRAM1)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, i, 0x01FF);
        else if(u8sramtype == SC_FILTER_C_SRAM2)
            MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (i|0x40), 0x01FF);
        else
            printf("Unknown c sram type %u\n", u8sramtype);

        for ( x=0;x<5;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_45_L, u8Ramcode[4], 0x00FF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0100, 0x0100);
    }

    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank);

}

void Hal_PQ_set_y_sram_table(MS_U8 u8sramtype, void *pData)
{
    MS_U8 u8Bank;
    MS_U16 i, j, x, step;
    MS_U8 u8Ramcode[8];
    MS_U32 u32Addr;

    //In U4 only support SRAM1 & SRAM2
    if( (u8sramtype != SC_FILTER_Y_SRAM1) &&
        (u8sramtype != SC_FILTER_Y_SRAM2) )
    {
        printf("Unknown y sram type %u\n", u8sramtype);
        return;
    }

    u32Addr = (MS_U32)pData;
    u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE);
    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0001, 0x0001); // enable y_sram_rw
    step = 5;

    for ( i=0; i<64; i++)
    {
        while(MApi_XC_R2BYTE(REG_SC_BK23_41_L) & 0x0100)
        {
            //wait
        }

        j=i*step;
        // address

        if(u8sramtype == SC_FILTER_Y_SRAM1)
          MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0000|i), 0x01FF);
        else
          MApi_XC_W2BYTEMSK(REG_SC_BK23_42_L, (0x0040|i), 0x01FF);

        for ( x=0;x<step;x++ )
        {
            u8Ramcode[x] = *((MS_U8 *)(u32Addr + (j+x)));
            PQ_DUMP_FILTER_DBG(printf(" %02x ", u8Ramcode[x] ));
        }
        PQ_DUMP_FILTER_DBG(printf("\n"));

        MApi_XC_W2BYTEMSK(REG_SC_BK23_43_L, (((MS_U16)u8Ramcode[1])<<8|(MS_U16)u8Ramcode[0]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_44_L, (((MS_U16)u8Ramcode[3])<<8|(MS_U16)u8Ramcode[2]), 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_45_L, ((MS_U16)u8Ramcode[4]), 0x00FF);

        // enable write
        MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x0100, 0x0100);
    }

    MApi_XC_W2BYTEMSK(REG_SC_BK23_41_L, 0x00, 0x00FF);
    MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank);
}

void Hal_PQ_set_sram_color_index_table(MS_U8 u8sramtype, void *pData)
{
    // u3 do not have this function
    u8sramtype = u8sramtype;
    pData = pData;
    return;
}

void Hal_PQ_set_sram_color_gain_snr_table(MS_U8 u8sramtype, void *pData)
{
    // u3 do not have this function
    u8sramtype = u8sramtype;
    pData = pData;
    return;
}

void Hal_PQ_set_sram_color_gain_dnr_table(MS_U8 u8sramtype, void *pData)
{
    // u3 do not have this function
    u8sramtype = u8sramtype;
    pData = pData;
    return;
}

MS_U16 Hal_PQ_get_420_cup_idx(MS_420_CUP_TYPE etype)
{
    MS_U16 u16ret;

    switch(etype)
    {
    case MS_420_CUP_OFF:
        u16ret = PQ_IP_420CUP_OFF_Main;
        break;

    case MS_420_CUP_ON:
        u16ret = PQ_IP_420CUP_ON_Main;
        break;

    default:
        u16ret = 0xFFFF;
        //MS_ASSERT(0);
        printf("get_420_cup_idx etype = %d is not included in enum MS_420_CUP_NUM\n", etype);
        break;
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_TYPE etype)
{
    MS_U16 u16ret;

    switch(etype)
    {
    case MS_HNONLINEAR_1920_0:
    case MS_HNONLINEAR_1920_1:
    case MS_HNONLINEAR_1920_2:
        u16ret = PQ_IP_HnonLinear_H_1920_Main;
        break;

    case MS_HNONLINEAR_1366_0:
    case MS_HNONLINEAR_1366_1:
    case MS_HNONLINEAR_1366_2:
        u16ret = PQ_IP_HnonLinear_H_1366_Main;
        break;

    default:
        u16ret = PQ_IP_HnonLinear_OFF_Main;
        MS_ASSERT(0);
        break;
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_madi_idx(MS_MADI_TYPE etype)
{
    MS_U16 u16ret;
    switch(etype)
    {
        case MS_MADI_24_4R:
            u16ret = PQ_IP_MADi_24_4R_Main;
            break;
        case MS_MADI_24_2R:
            u16ret = PQ_IP_MADi_24_2R_Main;
            break;
        case MS_MADI_25_4R_MC:
            u16ret = PQ_IP_MADi_25_4R_MC_Main;
            break;
        case MS_MADI_25_4R:
            u16ret = PQ_IP_MADi_25_4R_Main;
            break;
        case MS_MADI_25_2R:
            u16ret = PQ_IP_MADi_25_2R_Main;
            break;
        case MS_MADI_26_4R:
            u16ret = 0xFFFF;
            break;
        case MS_MADI_26_2R:
            u16ret = 0xFFFF;
            break;
        case MS_MADI_27_4R:
            u16ret = PQ_IP_MADi_27_4R_Main;
            break;
        case MS_MADI_27_2R:
            u16ret = PQ_IP_MADi_27_2R_Main;
            break;
        case MS_MADI_P_MODE8:
            u16ret = PQ_IP_MADi_P_MODE8_Main;
            break;
        case MS_MADI_P_MODE10:
            u16ret = PQ_IP_MADi_P_MODE10_Main;
            break;
        case MS_MADI_P_MODE_MOT8:
            u16ret = 0xFFFF;
            break;
        case MS_MADI_P_MODE_MOT10:
            u16ret = 0xFFFF;
            break;
        case MS_MADI_24_RFBL_NFILM:
            u16ret = PQ_IP_MADi_24_RFBL_NFilm_Main;
            break;
        case MS_MADI_24_RFBL_FILM:
            u16ret = PQ_IP_MADi_24_RFBL_Film_Main;
            break;
        case MS_MADI_FBL_DNR:
            u16ret = PQ_IP_MADi_FBL_DNR_Main;
            break;
       case MS_MADI_FBL_MIU:
            u16ret = PQ_IP_MADi_FBL_MIU_Main;
            break;
        case MS_MADI_P_MODE8_NO_MIU:
            u16ret = PQ_IP_MADi_P_MODE8_NO_MIU_Main;
            break;
        case MS_MADI_24_4R_880:
            u16ret = PQ_IP_MADi_24_4R_880_Main;
            break;
        case MS_MADI_24_2R_880:
            u16ret = PQ_IP_MADi_24_2R_880_Main;
            break;
        case MS_MADI_25_4R_MC_NW:
            u16ret = PQ_IP_MADi_25_4R_MC_NW_Main;
            break;
        case MS_MADI_25_6R_MC:
            u16ret = PQ_IP_MADi_25_6R_MC_Main;
            break;
        case MS_MADI_25_4R_884:
            u16ret = PQ_IP_MADi_25_4R_884_Main;
            break;
        case MS_MADI_25_4R_880:
            u16ret = PQ_IP_MADi_25_4R_880_Main;
            break;
        case MS_MADI_25_2R_884:
            u16ret = PQ_IP_MADi_25_2R_884_Main;
            break;
        case MS_MADI_25_2R_880:
            u16ret = PQ_IP_MADi_25_2R_880_Main;
            break;
        case MS_MADI_25_6R_MC_NW:
            u16ret = PQ_IP_MADi_25_6R_MC_NW_Main;
            break;
        case MS_MADI_25_12F_8R_MC:
            u16ret = PQ_IP_MADi_25_12F_8R_MC_Main;
            break;
        case MS_MADI_25_14F_8R_MC:
            u16ret = PQ_IP_MADi_25_14F_8R_MC_Main;
            break;
        case MS_MADI_25_16F_8R_MC:
            u16ret = PQ_IP_MADi_25_16F_8R_MC_Main;
            break;
        case MS_MADI_P_MODE8_3Frame:
            u16ret = PQ_IP_MADi_P_MODE8_3Frame_Main;
            break;
        case MS_MADI_27_2R_Y8Only:
            u16ret = PQ_IP_MADi_27_2R_Y8Only_Main;
            break;
        case MS_MADI_FBL_EODi:
            u16ret = PQ_IP_MADi_FBL_EODi_Main;
            break;
        case MS_MADI_24_4R_P:
            u16ret = PQ_IP_MADi_24_4R_P_Main;
            break;

        default:
            u16ret = 0xFFFF;
            printf("!!Invalid!!, Please check the index in [%s][%d]\n", __FUNCTION__, __LINE__);
            break;
    }
    return u16ret;
}

MS_U8 Hal_PQ_get_madi_fbl_mode(MS_BOOL bMemFmt422,MS_BOOL bInterlace)
{
    if(bMemFmt422)
    {
        if(bInterlace)   return MS_MADI_FBL_DNR;
        else                return MS_MADI_P_MODE8_NO_MIU;
    }
    else                    return MS_MADI_P_MODE8_NO_MIU;
}

MS_U16 Hal_PQ_get_sram_size(MS_U16 u16sramtype)
{
    MS_U16 u16ret;

    switch(u16sramtype)
    {
    case SC_FILTER_Y_SRAM1:
        u16ret = PQ_IP_SRAM1_SIZE_Main;
        break;

    case SC_FILTER_Y_SRAM2:
        u16ret = PQ_IP_SRAM2_SIZE_Main;
        break;

    case SC_FILTER_C_SRAM1:
        u16ret = PQ_IP_C_SRAM1_SIZE_Main;
        break;

    case SC_FILTER_C_SRAM2:
        u16ret = PQ_IP_C_SRAM2_SIZE_Main;
        break;

    default:
    case SC_FILTER_C_SRAM3:
    case SC_FILTER_C_SRAM4:
    case SC_FILTER_Y_SRAM3:
    case SC_FILTER_Y_SRAM4:
    case SC_FILTER_SRAM_COLOR_INDEX:
    case SC_FILTER_SRAM_COLOR_GAIN_SNR:
    case SC_FILTER_SRAM_COLOR_GAIN_DNR:
        u16ret = 0;
        break;
    }

    return u16ret;
}

MS_U16 Hal_PQ_get_csc_ip_idx(MS_CSC_IP_TYPE enCSC)
{
    MS_U16 u16Ret;

    switch(enCSC)
    {
    case MS_CSC_IP_CSC:
        u16Ret = (MS_U16)PQ_IP_CSC_Main;
        break;

    default:
    case MS_CSC_IP_VIP_CSC:
        u16Ret = 0xFFFF;
        break;
    }
    return u16Ret;
}

MS_U16 Hal_PQ_get_ip_idx(MS_PQ_IP_TYPE eType)
{
    MS_U16 u16ret;

    switch(eType)
    {
#if 0
    case MS_PQ_IP_VD_SAMPLING:
        u16ret = PQ_IP_VD_Sampling_no_comm_Main;
        break;
    case MS_PQ_IP_HSD_SAMPLING:
        u16ret = PQ_IP_HSD_Sampling_Main;
        break;
    case MS_PQ_IP_ADC_SAMPLING:
        u16ret = PQ_IP_ADC_Sampling_Main;
        break;
#endif
    default:
        u16ret = 0xFFFF;
        break;
    }

    return u16ret;
}


MS_U16 Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_TYPE eType)
{
    MS_U16 u16ret;

    switch(eType)
    {
#if 0
    case MS_HSD_SAMPLING_X_1_000:
        u16ret = PQ_IP_HSD_Sampling_x1o00_Main;
        break;

    case MS_HSD_SAMPLING_X_0_875:
        u16ret = PQ_IP_HSD_Sampling_x0o875_Main;
        break;

    case MS_HSD_SAMPLING_X_0_750:
        u16ret = PQ_IP_HSD_Sampling_x0o750_Main;
        break;

    case MS_HSD_SAMPLING_X_0_625:
        u16ret = PQ_IP_HSD_Sampling_x0o625_Main;
        break;

    case MS_HSD_SAMPLING_X_0_500:
        u16ret = PQ_IP_HSD_Sampling_x0o500_Main;
        break;

    case MS_HSD_SAMPLING_X_0_375:
        u16ret = PQ_IP_HSD_Sampling_x0o375_Main;
        break;

    case MS_HSD_SAMPLING_X_0_250:
        u16ret = PQ_IP_HSD_Sampling_x0o250_Main;
        break;

    case MS_HSD_SAMPLING_X_0_125:
        u16ret = PQ_IP_HSD_Sampling_x0o125_Main;
        break;
#endif
    default:
        u16ret = 0xFFFF;
        break;
    }

    return u16ret;
}

void Hal_PQ_OPWriteOff_Enable(MS_BOOL bEna)
{
    MApi_XC_W2BYTEMSK(REG_SC_BK12_27_L, bEna, BIT(0));
}

MS_U16 Hal_PQ_get_adc_sampling_idx(MS_ADC_SAMPLING_TYPE eType)
{
    MS_U16 u16ret;

    switch(eType)
    {
#if 0
    case MS_ADC_SAMPLING_X_1:
        u16ret = PQ_IP_ADC_Sampling_x1_Main;
        break;

    case MS_ADC_SAMPLING_X_2:
        u16ret = PQ_IP_ADC_Sampling_x2_Main;
        break;
#endif
    default:
        u16ret = 0xFFFF;
        break;
    }

    return u16ret;
}

