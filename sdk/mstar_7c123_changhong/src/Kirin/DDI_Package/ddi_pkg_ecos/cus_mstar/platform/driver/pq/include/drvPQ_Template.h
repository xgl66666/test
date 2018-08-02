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
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#ifndef _DRVPQ_TEMPLATE_H_
#define _DRVPQ_TEMPLATE_H_

#define PQ_INSTALL_FUNCTION

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#if(PQ_ONLY_SUPPORT_BIN == 0)
static PQTABLE_INFO _PQTableInfo;
static MS_U8 _u8PQTabIdx[PQ_IP_NUM_Main];    // store all TabIdx of all IPs
static MS_BOOL _bMLoadEn = FALSE;
static MS_U8 _bUserDeFlicker = FALSE;

#define MLOAD_MAX_CMD   30
static MS_U32 _u32MLoadCmd[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadMsk[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadVal[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadCmdCnt = 0;
#endif

extern MS_U32 gu32LastPQIPSRAM1_Main;
extern MS_U32 gu32LastPQIPSRAM2_Main;
extern MS_U32 gu32LastPQIPCSRAM1_Main;
extern MS_U32 gu32LastPQIPCSRAM2_Main;
extern MS_U32 gu32LastPQIPVEHSDSRAM1_Main;
extern MS_U32 gu32LastPQIPVEHSDSRAM2_Main;
extern MS_U32 gu32LastPQIPVEVSDSRAM1_Main;
extern MS_U32 gu32LastPQIPVEVSDSRAM2_Main;



#if (PQ_ENABLE_CHECK == 0)
#define PQ_REG_FUNC( u32Reg, u8Value, u8Mask )    MApi_XC_WriteByteMask( u32Reg, u8Value, u8Mask )
#define PQ_REG_FUNC_xc( u32Reg, u8Value, u8Mask ) \
    do{ \
            if(u32Reg %2){ \
                    MApi_XC_W2BYTEMSK(u32Reg-1, ((MS_U16)u8Value)<<8, ((MS_U16)u8Mask)<<8); \
                }else{ \
                    MApi_XC_W2BYTEMSK(u32Reg, u8Value, u8Mask); \
                }\
        }while(0)


#define PQ_REG_MLOAD_WRITE_CMD(u32Reg,u8Value,u8Mask) \
    do{ \
        if(u32Reg%2) \
        { \
            _u32MLoadCmd[_u16MLoadCmdCnt] = u32Reg-1; \
            _u16MLoadMsk[_u16MLoadCmdCnt] = ((MS_U16)u8Mask)<<8; \
            _u16MLoadVal[_u16MLoadCmdCnt] = ((MS_U16)u8Value)<<8; \
        } \
        else \
        { \
            _u32MLoadCmd[_u16MLoadCmdCnt] = u32Reg; \
            _u16MLoadMsk[_u16MLoadCmdCnt] = ((MS_U16)u8Mask); \
            _u16MLoadVal[_u16MLoadCmdCnt] = ((MS_U16)u8Value); \
        } \
        _u16MLoadCmdCnt++; \
        if(_u16MLoadCmdCnt >= MLOAD_MAX_CMD)\
        {\
            printf("[PQ ERROR] ====ML overflow !!! \n");\
            _u16MLoadCmdCnt = MLOAD_MAX_CMD - 1;        \
        }\
    }while(0)

#define PQ_REG_MLOAD_FUNC(u32Reg,u8Value,u8Mask) \
    do{ \
        if(u32Reg%2) \
        { \
            MApi_XC_MLoad_WriteCmd_And_Fire(u32Reg-1, ((MS_U16)u8Value)<<8, ((MS_U16)u8Mask)<<8); \
        } \
        else \
        { \
            MApi_XC_MLoad_WriteCmd_And_Fire(u32Reg, u8Value, u8Mask); \
        } \
    }while(0)

#define PQ_SWREG_FUNC( u16Reg, u8Value, u8Mask )  SWReg_WriteByteMask( u16Reg, u8Value, u8Mask )

#else // #if(PQ_ENABLE_CHECK == 1)

static MS_U8 _u8PQfunction = PQ_FUNC_DUMP_REG;

static void _MDrv_PQ_SetFunction(MS_U8 u8Func)
{
    _u8PQfunction = u8Func;
}

#define PQ_REG_FUNC( u16Reg, u8Value, u8Mask ) \
    do{ \
        if (_u8PQfunction == PQ_FUNC_DUMP_REG){ \
            MApi_XC_WriteByteMask( (MS_U32)u16Reg, u8Value, u8Mask ); \
        }else{ \
            if ((MApi_XC_ReadByte((MS_U32)u16Reg) & u8Mask) != ((u8Value) & (u8Mask))){ \
                printf("[PQRegErr] "); \
                if (((u16Reg) >> 8) == 0x2F){ \
                    printf("bk=%02x, ", (MS_U16)SC_BK_CURRENT); \
                } \
                else if (((u16Reg) >> 8) == 0x36){ \
                    printf("bk=%02x, ", (MS_U16)COMB_BK_CURRENT); \
                } \
                printf("addr=%04x, mask=%02x, val=%02x[%02x]\n", \
                    u16Reg, (MS_U16)u8Mask, (MS_U16)MApi_XC_ReadByte((MS_U32)u16Reg), (MS_U16)u8Value); \
            } \
        } \
    }while(0)

#define PQ_SWREG_FUNC( u16Reg, u8Value, u8Mask ) \
    do{ \
        if (_u8PQfunction == PQ_FUNC_DUMP_REG){ \
            SWReg_WriteByteMask( u16Reg, u8Value, u8Mask ); \
        }else{ \
            if (SWReg[u16Reg] & (u8Mask) != (u8Value) & (u8Mask)){ \
                printf("[PQSWRegErr] "); \
                printf("addr=%04x, mask=%02x, val=%02x[%02x]\n", \
                    u16Reg, (MS_U16)u8Mask, (MS_U16)SWReg[u16Reg], (MS_U16)u8Value); \
            } \
        } \
    }while(0)

#endif  //#if (PQ_ENABLE_CHECK)


#if(PQ_ONLY_SUPPORT_BIN == 0)

static void _MDrv_PQ_Set_MLoadEn(MS_BOOL bEn)
{
    MS_U16 i;
    if(bEn == DISABLE)
    {
        SC_BK_STORE;

        if(_u16MLoadCmdCnt)
        {
            for(i=1; i<_u16MLoadCmdCnt; i++)
            {
                if(_u32MLoadCmd[i-1] == _u32MLoadCmd[i])
                {
                    _u16MLoadMsk[i] |= _u16MLoadMsk[i-1];
                    _u16MLoadVal[i] |= _u16MLoadVal[i-1];
                }
            }
    #if(ENABLE_PQ_MLOAD)

            MApi_XC_MLoad_WriteCmds_And_Fire(
                &_u32MLoadCmd[0], &_u16MLoadVal[0], &_u16MLoadMsk[0], _u16MLoadCmdCnt);
    #endif
            _u16MLoadCmdCnt = 0;
        }

        SC_BK_RESTORE;
    }
    else
    {
        //_u16MLoadCmdCnt = 0;
    }

    _bMLoadEn = bEn;
}
static void _MDrv_PQ_DumpGeneralRegTable(EN_IP_Info* pIP_Info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;

    PQ_DUMP_DBG(printf("tab: general\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: General Reg Table\n"));
        return;
    }

    while (1)
    {
         u32Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
         u8Mask  = pIP_Info->pIPTable[2];
         u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

         if (u32Addr == _END_OF_TBL_) // check end of table
             break;

         u32Addr |= NON_PM_BASE;
         PQ_DUMP_DBG(printf("[addr=%04lx, msk=%02x, val=%02x]\n", u32Addr, u8Mask, u8Value));
         PQ_REG_FUNC(u32Addr, u8Value, u8Mask);

         pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next
    }
}

static void _MDrv_PQ_DumpCombRegTable(EN_IP_Info* pIP_Info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    COMB_BK_STORE;

    PQ_DUMP_DBG(printf("tab: comb\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: Comb Reg Table\n"));
        return;
    }

    while (1)
    {
        u32Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
        u8Mask  = pIP_Info->pIPTable[2];
        u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

        if (u32Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u32Addr >> 8;
        if (u8CurBank != COMB_BK_CURRENT)
        {
            PQ_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
            COMB_BK_SWITCH(u8CurBank);
        }

        u32Addr = COMB_REG_BASE | (u32Addr & 0x00FF);

        PQ_DUMP_DBG(printf("[addr=%04x, msk=%02x, val=%02x]\n", u32Addr, u8Mask, u8Value));
        PQ_REG_FUNC(u32Addr, u8Value, u8Mask);

         pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next
    }

    COMB_BK_RESTORE;
}

static void _MDrv_PQ_DumpScalerRegTable(EN_IP_Info* pIP_Info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;

#if(ENABLE_PQ_MLOAD)
#if defined (MSOS_TYPE_LINUX)
    pthread_mutex_lock(&_PQ_MLoad_Mutex);
#endif
#endif

    SC_BK_STORE;
    PQ_DUMP_DBG(printf("tab: sc\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums)
    {
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: Scaler Reg Table\n"));

        SC_BK_RESTORE;

#if(ENABLE_PQ_MLOAD)
#if defined (MSOS_TYPE_LINUX)
        pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif
        return;
    }

    while (1)
    {
        u32Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
        u8Mask  = pIP_Info->pIPTable[2];
        u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

        if (u32Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u32Addr >> 8;
        PQ_DUMP_DBG(printf("XC bk =%x, addr=%x, msk=%x, value=%x\n", (MS_U16)((u32Addr&0xFF00)>>8), (MS_U16)(u32Addr&0xFF), (MS_U16)u8Mask, (MS_U16)u8Value));
        if(_bMLoadEn)
        {
            //printf("MLad: %lx, %x, %x\n", u32Addr, u8Value, u8Mask);
            PQ_REG_MLOAD_WRITE_CMD(u32Addr, u8Value, u8Mask);
            //PQ_REG_MLOAD_FUNC(u32Addr, u8Value, u8Mask);
        }
        else
        {
            if (u8CurBank != SC_BK_CURRENT)
            {
                PQ_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
                SC_BK_SWITCH(u8CurBank);
            }

            u32Addr = BK_SCALER_BASE | (u32Addr & 0x00FF);
            PQ_REG_FUNC(u32Addr, u8Value, u8Mask);
            //PQ_REG_FUNC_xc(u32Addr, u8Value, u8Mask);
        }
        pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next
    }

    SC_BK_RESTORE;

#if(ENABLE_PQ_MLOAD)
#if defined (MSOS_TYPE_LINUX)
    pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif

}

static void _MDrv_PQ_DumpSWDriverRegTable(EN_IP_Info* pIP_Info)
{
    MS_U16 u16Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    SC_BK_STORE;

    PQ_DUMP_DBG(printf("tab: sc\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums)
    {
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: SWDriver Reg Table\n"));
        SC_BK_RESTORE;

        return;
    }

    while (1)
    {
        u16Addr = (pIP_Info->pIPTable[0]<<8) + pIP_Info->pIPTable[1];
        u8Mask  = pIP_Info->pIPTable[2];
        u8Value = pIP_Info->pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabIdx];

        if (u16Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u16Addr >> 8;

        if (u8CurBank == SC_BK_SWDRIVER)
        {
            u16Addr = (u16Addr & 0x00FF);

            PQ_SWREG_FUNC(u16Addr, u8Value, u8Mask);
        }
        else
        {
#if 0
            PQ_DUMP_DBG(printf("----SW Driver Reg\n"));
            PQ_REG_FUNC_xc(u16Addr, u8Value, u8Mask);
#else
            if (u8CurBank != SC_BK_CURRENT)
            {
                PQ_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
                SC_BK_SWITCH(u8CurBank);
            }

            u16Addr = BK_SCALER_BASE | (u16Addr & 0x00FF);

            PQ_REG_FUNC(u16Addr, u8Value, u8Mask);
#endif

        }
        PQ_DUMP_DBG(printf("[addr=%04x, msk=%02x, val=%02x]\n", u16Addr, u8Mask, u8Value));
        pIP_Info->pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+pIP_Info->u8TabNums); // next

    }

    SC_BK_RESTORE;
}

static void _MDrv_PQ_DumpFilterTable(EN_IP_Info* pIP_Info)
{

    PQ_DUMP_FILTER_DBG(printf("tab: sram\n"));
    if (pIP_Info->u8TabIdx >= pIP_Info->u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: SRAM Table\n"));
        return;
    }

    switch(pIP_Info->u8TabType)
    {
    case PQ_TABTYPE_SRAM1:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM1, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM1)]));
        gu32LastPQIPSRAM1_Main = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM1)];
        break;
    case PQ_TABTYPE_SRAM2:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM2, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM2)]));
        gu32LastPQIPSRAM2_Main = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM2)];        
        break;

    case PQ_TABTYPE_SRAM3:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM3, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM3)]));
        break;
    case PQ_TABTYPE_SRAM4:
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM4, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM4)]));
        break;

    case PQ_TABTYPE_C_SRAM1:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM1, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM1)]));
        gu32LastPQIPCSRAM1_Main = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM1)];        
        break;

    case PQ_TABTYPE_C_SRAM2:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM2, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM2)]));
        gu32LastPQIPCSRAM2_Main = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM2)];        
        break;

    case PQ_TABTYPE_C_SRAM3:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM3, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM3)]));
        break;

    case PQ_TABTYPE_C_SRAM4:
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM4, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM4)]));
        break;

    case PQ_TABTYPE_SRAM_COLOR_INDEX:
        Hal_PQ_set_sram_color_index_table(SC_FILTER_SRAM_COLOR_INDEX, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_COLOR_INDEX)]));
        break;

    case PQ_TABTYPE_SRAM_COLOR_GAIN_SNR:
        Hal_PQ_set_sram_color_gain_snr_table(SC_FILTER_SRAM_COLOR_GAIN_SNR, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_COLOR_GAIN_SNR)]));
        break;

    case PQ_TABTYPE_SRAM_COLOR_GAIN_DNR:
        Hal_PQ_set_sram_color_gain_snr_table(SC_FILTER_SRAM_COLOR_GAIN_DNR, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_SRAM_COLOR_GAIN_DNR)]));
        break;
    case PQ_TABTYPE_VE_HSD_SRAM:
        //load table0 & table1 (128 phase & 64 phase)
        Hal_PQ_set_ve_h_sram_table(SC_FILTER_VE_H_SRAM1, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_H_SRAM1)]));
        Hal_PQ_set_ve_h_sram_table(SC_FILTER_VE_H_SRAM2, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_H_SRAM2)]));
        gu32LastPQIPVEHSDSRAM1_Main  = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_H_SRAM1)];
        gu32LastPQIPVEHSDSRAM2_Main  = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_H_SRAM2)];        
        break;

    case PQ_TABTYPE_VE_VSD_SRAM:
        //load table0 & table1 (128 phase & 64 phase)
        Hal_PQ_set_ve_v_sram_table(SC_FILTER_VE_V_SRAM1, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_V_SRAM1)]));
        Hal_PQ_set_ve_v_sram_table(SC_FILTER_VE_V_SRAM2, (void *)((MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_V_SRAM2)]));
        gu32LastPQIPVEVSDSRAM1_Main  = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_V_SRAM1)];
        gu32LastPQIPVEVSDSRAM2_Main  = (MS_U32)&pIP_Info->pIPTable[pIP_Info->u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_VE_V_SRAM2)];
        break;
    default:
        printf("[PQ]Unknown sram type %u\n", pIP_Info->u8TabType);
        break;
    }
}

static void _MDrv_PQ_DumpTable(EN_IP_Info* pIP_Info)
{
    // to save loading SRAM table time, SRAM are only downloaded
    // when current SRAM table is different to previous SRAM table
    static MS_U8 _u8SRAM1Table=0xFF;
    static MS_U8 _u8SRAM2Table=0xFF;
    static MS_U8 _u8SRAM3Table=0xFF;
    static MS_U8 _u8SRAM4Table=0xFF;
    static MS_U8 _u8CSRAM1Table=0xFF;
    static MS_U8 _u8CSRAM2Table=0xFF;
    static MS_U8 _u8CSRAM3Table=0xFF;
	#if PQ_ENABLE_DEBUG
    static MS_U8 _u8CSRAM4Table=0xFF;
	#endif
    static MS_U8 _u8VE_V_SRAMTable=0xFF;
    static MS_U8 _u8VE_H_SRAMTable=0xFF;

    if (pIP_Info->pIPTable == NULL)
    {
        PQ_DUMP_DBG(printf("NULL Table\n"););
        return;
    }
    PQ_DUMP_DBG(printf("Table Type =%x, Index =%x\n", (MS_U16)pIP_Info->u8TabType, (MS_U16)pIP_Info->u8TabIdx));
    switch(pIP_Info->u8TabType )
    {
    case PQ_TABTYPE_SCALER:
        _MDrv_PQ_DumpScalerRegTable(pIP_Info);
        break;
    case PQ_TABTYPE_COMB:
        _MDrv_PQ_DumpCombRegTable(pIP_Info);
        break;
    case PQ_TABTYPE_SWDRIVER:
        _MDrv_PQ_DumpSWDriverRegTable(pIP_Info);
        break;
    case PQ_TABTYPE_GENERAL:
        _MDrv_PQ_DumpGeneralRegTable(pIP_Info);
        break;

    case PQ_TABTYPE_SRAM1:
        if (_u8SRAM1Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram1: %u, new sram1: %u\n",
                (MS_U16)_u8SRAM1Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM1Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram1: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;
    case PQ_TABTYPE_SRAM2:
        if (_u8SRAM2Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram2: %u, new sram2: %u\n",
                (MS_U16)_u8SRAM2Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM2Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram2: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;

    case PQ_TABTYPE_SRAM3:
        if (_u8SRAM3Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram3: %u, new sram3: %u\n",
                (MS_U16)_u8SRAM3Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM3Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram3: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;

    case PQ_TABTYPE_SRAM4:
        if (_u8SRAM4Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram4: %u, new sram4: %u\n",
                (MS_U16)_u8SRAM4Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8SRAM4Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram4: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;


    case PQ_TABTYPE_C_SRAM1:
        if (_u8CSRAM1Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old Csram1: %u, new Csram1: %u\n",
                (MS_U16)_u8CSRAM1Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8CSRAM1Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same Csram1: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM2:
        if (_u8CSRAM2Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old Csram2: %u, new Csram2: %u\n",
                (MS_U16)_u8CSRAM2Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8CSRAM2Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same Csram2: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;

    case PQ_TABTYPE_C_SRAM3:
        if (_u8CSRAM3Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old Csram3: %u, new Csram3: %u\n",
                (MS_U16)_u8CSRAM3Table, (MS_U16)pIP_Info->u8TabIdx));
            _u8CSRAM3Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same sram3: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;

    case PQ_TABTYPE_C_SRAM4:
        if (_u8SRAM4Table != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old Csram4: %u, new Csram4: %u\n",
                (MS_U16)_u8CSRAM4Table, (MS_U16)pIP_Info->u8TabIdx));
            //_u8CSRAM4Table = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same Csram4: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;

    case PQ_TABTYPE_VE_HSD_SRAM:
        if (_u8VE_H_SRAMTable != pIP_Info->u8TabIdx)
        {
            PQ_DUMP_FILTER_DBG(printf("old sram1: %u, new sram1: %u\n",
                (MS_U16)_u8VE_H_SRAMTable, (MS_U16)pIP_Info->u8TabIdx));
            _u8VE_H_SRAMTable = pIP_Info->u8TabIdx;
            _MDrv_PQ_DumpFilterTable(pIP_Info);
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same VE HSD SRAM: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;
    case PQ_TABTYPE_VE_VSD_SRAM:
        if (_u8VE_V_SRAMTable != pIP_Info->u8TabIdx)
        {
            if(!_bUserDeFlicker)
            {
                PQ_DUMP_FILTER_DBG(printf("old sram1: %u, new sram1: %u\n",
                    (MS_U16)_u8VE_V_SRAMTable, (MS_U16)pIP_Info->u8TabIdx));
                _u8VE_V_SRAMTable = pIP_Info->u8TabIdx;
                _MDrv_PQ_DumpFilterTable(pIP_Info);
            }
        }
        else
        {
            PQ_DUMP_FILTER_DBG(printf("use the same VE VSD SRAM: %u\n", (MS_U16)pIP_Info->u8TabIdx));
        }
        break;

    default:
        PQ_DUMP_DBG(printf("[PQ]DumpTable:unknown type\n"));
        break;
    }
}

// return total IP count
static MS_U16 _MDrv_PQ_GetIPNum(void)
{
    PQ_DBG(printf("[PQ]IPNum=%u\n",_PQTableInfo.u8PQ_IP_Num));
    return (MS_U16)_PQTableInfo.u8PQ_IP_Num;
}

// return total table count of given IP
static MS_U16 _MDrv_PQ_GetTableNum(MS_U8 u8PQIPIdx)
{
    PQ_DBG(printf("[PQ]TabNum=%u\n", _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabNums));
    return (MS_U16)_PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabNums;
}

// return current used table index of given IP
static MS_U16 _MDrv_PQ_GetCurrentTableIndex(MS_U8 u8PQIPIdx)
{
    PQ_DBG(printf("[PQ]CurrTableIdx=%u\n", _u8PQTabIdx[u8PQIPIdx]));
    return (MS_U16)_u8PQTabIdx[u8PQIPIdx];
}

static MS_U16 _MDrv_PQ_GetTableIndex(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx)
{
    if (u16PQSrcType >=_PQTableInfo.u8PQ_InputType_Num){
        PQ_DBG(printf("[PQ]invalid input type\n"));
        return PQ_IP_NULL;
    }
    if (u8PQIPIdx >= _PQTableInfo.u8PQ_IP_Num){
        PQ_DBG(printf("[PQ]invalid ip type\n"));
        return PQ_IP_NULL;
    }

    PQ_DBG(printf("[PQ]TableIdx=%u\n",(MS_U16)_PQTableInfo.pQuality_Map_Aray[u16PQSrcType * _PQTableInfo.u8PQ_IP_Num + u8PQIPIdx]));

    return (MS_U16)_PQTableInfo.pQuality_Map_Aray[u16PQSrcType * _PQTableInfo.u8PQ_IP_Num + u8PQIPIdx];
}

static EN_IP_Info _MDrv_PQ_GetTable(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx)
{
    EN_IP_Info ip_Info;
    _u8PQTabIdx[u8PQIPIdx] = u8TabIdx;
    if ( (u8TabIdx != PQ_IP_NULL) && (u8TabIdx != PQ_IP_COMM) ) {
        ip_Info.pIPTable  = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].pIPTable;
        ip_Info.u8TabNums = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabNums;
        ip_Info.u8TabType = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabType;
        ip_Info.u8TabIdx = u8TabIdx;
    }
    else if (u8TabIdx == PQ_IP_COMM) {
        ip_Info.pIPTable = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].pIPCommTable;
        ip_Info.u8TabNums = 1;
        ip_Info.u8TabType = _PQTableInfo.pIPTAB_Info[u8PQIPIdx].u8TabType;
        ip_Info.u8TabIdx = 0;
    }
    else {
        ip_Info.pIPTable  = 0;
        ip_Info.u8TabNums = 0;
        ip_Info.u8TabType = 0;
        ip_Info.u8TabIdx = 0;
    }
    return ip_Info;
}

static void _MDrv_PQ_LoadTableData(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize)
{
    EN_IP_Info ip_Info;
    MS_U32 u32Addr;
    MS_U16 u16Idx = 0;


    ip_Info = _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx);
    if (ip_Info.u8TabIdx >= ip_Info.u8TabNums){
        PQ_DUMP_DBG(printf("[PQ]IP_Info error: Scaler Reg Table\n"));
        return;
    }

    while (1)
    {
        u32Addr = (ip_Info.pIPTable[0]<<8) + ip_Info.pIPTable[1];
        pTable[u16Idx] = ip_Info.pIPTable[REG_ADDR_SIZE+REG_MASK_SIZE+ip_Info.u8TabIdx];
        u16Idx++;

        if( (u16Idx > u16TableSize) || (u32Addr == _END_OF_TBL_) )
            break;

        PQ_DUMP_DBG(printf("XC bk =%x, addr=%x, msk=%x, value=%x\n", (MS_U16)((u32Addr&0xFF00)>>8), (MS_U16)(u32Addr&0xFF), (MS_U16)u8Mask, (MS_U16)u8Value));
        ip_Info.pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+ip_Info.u8TabNums); // next
    }
}

static void _MDrv_PQ_LoadTable(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx)
{
    EN_IP_Info ip_Info;
    ip_Info = _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx);
    _MDrv_PQ_DumpTable(&ip_Info);
}

static void _MDrv_PQ_LoadCommTable(void)
{
    MS_U8 i;
    EN_IP_Info ip_Info;

    PQ_DBG(printf("[PQ]LoadCommTable\n"));
    for (i=0; i<_PQTableInfo.u8PQ_IP_Num; i++){

        PQ_DBG(printf("  IP:%u\n", (MS_U16)i));
        ip_Info = _MDrv_PQ_GetTable(PQ_IP_COMM, i);

    #if (PQTBL_REGTYPE == PQTBL_NORMAL)
        if( ((ip_Info.u8TabType >= PQ_TABTYPE_SRAM1) && (ip_Info.u8TabType <= PQ_TABTYPE_C_SRAM4))
            ||(ip_Info.u8TabType == PQ_TABTYPE_VE_HSD_SRAM)
            ||(ip_Info.u8TabType == PQ_TABTYPE_VE_VSD_SRAM)
           )
        {
            continue;
        }
    #endif
        _MDrv_PQ_DumpTable(&ip_Info);
    }
}

static void _MDrv_PQ_LoadDeFlickerTable(MS_U8 u8DeFlickType)
{
    MS_U8 i;
	#if (PQTBL_REGTYPE == PQTBL_NORMAL)
    EN_IP_Info ip_Info;
	#endif

    PQ_DBG(printf("[PQ]_MDrv_PQ_LoadDeFlickerTable\n"));
    for (i=0; i<_PQTableInfo.u8PQ_IP_Num; i++)
    {
    #if (PQTBL_REGTYPE == PQTBL_NORMAL)
        ip_Info = _MDrv_PQ_GetTable(PQ_IP_COMM, i);


        if((ip_Info.u8TabType == PQ_TABTYPE_VE_VSD_SRAM)
            ||(ip_Info.u8TabType == PQ_TABTYPE_VE_HSD_SRAM)
        )
        {
            MS_U8 u8TabIdx = 0;
            if(ip_Info.u8TabType == PQ_TABTYPE_VE_VSD_SRAM)
            {
                 u8TabIdx = Hal_PQ_get_ve_v_sram_table_index(u8DeFlickType);
                 ip_Info = _MDrv_PQ_GetTable(u8TabIdx, i);
                 _MDrv_PQ_DumpTable(&ip_Info);

                 _bUserDeFlicker = (u8DeFlickType != 0)?TRUE : FALSE;   //need set after dumptable
            }
            else
            {
                 u8TabIdx = Hal_PQ_get_ve_h_sram_table_index(0);    //h always using default(in index"0"), because of CVBS test
                 ip_Info = _MDrv_PQ_GetTable(u8TabIdx, i);
                 _MDrv_PQ_DumpTable(&ip_Info);
            }
        }
        else
            continue;
    #endif
    }
}

static MS_U8 _MDrv_PQ_GetXRuleTableIndex(MS_U8 u8XRuleType, MS_U8 u8XRuleIdx, MS_U8 u8XRuleIP)
{
    MS_U8 *pArray = _PQTableInfo.pXRule_Array[u8XRuleType];
    return pArray[((MS_U16)u8XRuleIdx) * _PQTableInfo.u8PQ_XRule_IP_Num[u8XRuleType] + u8XRuleIP];
}

static MS_U8 _MDrv_PQ_GetXRuleIPIndex(MS_U8 u8XRuleType, MS_U8 u8XRuleIP)
{
    MS_U8 *pArray = _PQTableInfo.pXRule_IP_Index[u8XRuleType];
    return pArray[u8XRuleIP];
}

static MS_U8 _MDrv_PQ_GetXRuleIPNum(MS_U8 u8XRuleType)
{
    return _PQTableInfo.u8PQ_XRule_IP_Num[u8XRuleType];
}

static MS_U8 _MDrv_PQ_GetGRule_LevelIndex(MS_U8 u8GRuleType, MS_U8 u8GRuleLvlIndex)
{
    MS_U8 *pArray = _PQTableInfo.pGRule_Level[u8GRuleType];
    return pArray[u8GRuleLvlIndex];
}

static MS_U8 _MDrv_PQ_GetGRule_IPIndex(MS_U8 u8GRuleType, MS_U8 u8GRuleIPIndex)
{
     MS_U8 *pArray = _PQTableInfo.pGRule_IP_Index[u8GRuleType];
    return pArray[u8GRuleIPIndex];
}

static MS_U8 _MDrv_PQ_GetGRule_TableIndex(MS_U8 u8GRuleType, MS_U8 u8PQSrcType, MS_U8 u8PQ_NRIdx, MS_U8 u8GRuleIPIndex)
{
//    return _PQTableInfo.pGRule_Array[u8PQSrcType][u8PQ_NRIdx][u8GRuleIPIndex];
    MS_U16 u16index;
    MS_U8 *pArray = _PQTableInfo.pGRule_Array[u8GRuleType];

    u16index = ((MS_U16)u8PQSrcType) * _PQTableInfo.u8PQ_GRule_Num[u8GRuleType] * _PQTableInfo.u8PQ_GRule_IPNum[u8GRuleType] +
               ((MS_U16)u8PQ_NRIdx) * _PQTableInfo.u8PQ_GRule_IPNum[u8GRuleType] +
               u8GRuleIPIndex;
    return pArray[u16index];
}

static void _MDrv_PQ_LoadTableBySrcType(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx)
{
    EN_IP_Info ip_Info;
    MS_U8 QMIPtype_size,i;
    MS_U8 u8TabIdx;

    if (u8PQIPIdx==PQ_IP_ALL)
    {
        QMIPtype_size=_PQTableInfo.u8PQ_IP_Num;
        u8PQIPIdx=0;
    }
    else
    {
        QMIPtype_size=1;
    }

    //for debug
    //msAPI_Scaler_SetBlueScreen(DISABLE, 0x00);
    //MApi_XC_GenerateBlackVideo(FALSE);

    for(i=0; i<QMIPtype_size; i++)
    {
        if (_PQTableInfo.pSkipRuleIP[u8PQIPIdx]) {
            PQ_DBG(printf("skip ip idx:%u\n", u8PQIPIdx));
            u8PQIPIdx++;
            continue;
        }

        u8TabIdx = _MDrv_PQ_GetTableIndex(u16PQSrcType, u8PQIPIdx);
        PQ_DBG(printf("[PQ]SrcType=%u, IPIdx=%u, TabIdx=%u\n",
            (MS_U16)u16PQSrcType, (MS_U16)u8PQIPIdx, (MS_U16)u8TabIdx));

        ip_Info = _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx);

        _MDrv_PQ_DumpTable(&ip_Info);

        //MsOS_DelayTask(1500);
        u8PQIPIdx++;
    }
}

static void _MDrv_PQ_AddTable(PQTABLE_INFO *pPQTableInfo)
{
    _PQTableInfo = *pPQTableInfo;
}

static void _MDrv_PQ_CheckCommTable(void)
{
#if (PQ_ENABLE_CHECK == 1)
    u8PQfunction = PQ_FUNC_CHK_REG;
    _MDrv_PQ_LoadCommTable();
    u8PQfunction = PQ_FUNC_DUMP_REG;
#endif
}

static void _MDrv_PQ_CheckTableBySrcType(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx)
{
#if (PQ_ENABLE_CHECK == 1)
    u8PQfunction = PQ_FUNC_CHK_REG;
    _MDrv_PQ_LoadTableBySrcType(u16PQSrcType, u8PQIPIdx);
    u8PQfunction = PQ_FUNC_DUMP_REG;
#else
    UNUSED(u16PQSrcType);
    UNUSED(u8PQIPIdx);
#endif
}


INSTALL_PQ_FUNCTIONS(PQTABLE_NAME)
#endif

#undef _DRVPQ_TEMPLATE_H_
#endif /* _DRVPQ_TEMPLATE_H_ */

