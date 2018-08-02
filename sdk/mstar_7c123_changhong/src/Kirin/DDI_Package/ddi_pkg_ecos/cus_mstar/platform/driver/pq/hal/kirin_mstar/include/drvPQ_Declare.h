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
#ifndef _DRVPQ_DECLARE_H
#define _DRVPQ_DECLARE_H


///////////////////////////////////////////////////////////////////////////////
// function macro
//
//### MDrv_PQ_DumpTable
#define MDrv_PQ_DumpTable_(PQTABLE_NAME, pIP_Info) \
    MDrv_PQ_DumpTable_##PQTABLE_NAME(pIP_Info)

#define DECLARE_MDrv_PQ_DumpTable_(PQTABLE_NAME) \
    extern void MDrv_PQ_DumpTable_##PQTABLE_NAME(EN_IP_Info* pIP_Info);

#define INSTALL_MDrv_PQ_DumpTable_(PQTABLE_NAME) \
    void MDrv_PQ_DumpTable_##PQTABLE_NAME(EN_IP_Info* pIP_Info){ \
        _MDrv_PQ_DumpTable(pIP_Info); \
    }


//### MDrv_PQ_GetIPNum
#define MDrv_PQ_GetIPNum_(PQTABLE_NAME) \
    MDrv_PQ_GetIPNum_##PQTABLE_NAME()

#define DECLARE_MDrv_PQ_GetIPNum_(PQTABLE_NAME) \
    extern MS_U16 MDrv_PQ_GetIPNum_##PQTABLE_NAME(void);

#define INSTALL_MDrv_PQ_GetIPNum_(PQTABLE_NAME) \
    MS_U16 MDrv_PQ_GetIPNum_##PQTABLE_NAME(void){ \
        return _MDrv_PQ_GetIPNum(); \
    }


//### MDrv_PQ_GetTableNum
#define MDrv_PQ_GetTableNum_(PQTABLE_NAME, u8PQIPIdx) \
    MDrv_PQ_GetTableNum_##PQTABLE_NAME(u8PQIPIdx)

#define DECLARE_MDrv_PQ_GetTableNum_(PQTABLE_NAME) \
    extern MS_U16 MDrv_PQ_GetTableNum_##PQTABLE_NAME(MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_GetTableNum_(PQTABLE_NAME) \
    MS_U16 MDrv_PQ_GetTableNum_##PQTABLE_NAME(MS_U8 u8PQIPIdx){ \
        return _MDrv_PQ_GetTableNum(u8PQIPIdx); \
    }


//### MDrv_PQ_GetCurrentTableIndex
#define MDrv_PQ_GetCurrentTableIndex_(PQTABLE_NAME, u8PQIPIdx) \
    MDrv_PQ_GetCurrentTableIndex_##PQTABLE_NAME(u8PQIPIdx)

#define DECLARE_MDrv_PQ_GetCurrentTableIndex_(PQTABLE_NAME) \
    extern MS_U16 MDrv_PQ_GetCurrentTableIndex_##PQTABLE_NAME(MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_GetCurrentTableIndex_(PQTABLE_NAME) \
    MS_U16 MDrv_PQ_GetCurrentTableIndex_##PQTABLE_NAME(MS_U8 u8PQIPIdx){ \
        return _MDrv_PQ_GetCurrentTableIndex(u8PQIPIdx); \
    }


//### MDrv_PQ_GetTableIndex
#define MDrv_PQ_GetTableIndex_(PQTABLE_NAME, u16PQSrcType, u8PQIPIdx) \
    MDrv_PQ_GetTableIndex_##PQTABLE_NAME(u16PQSrcType, u8PQIPIdx)

#define DECLARE_MDrv_PQ_GetTableIndex_(PQTABLE_NAME) \
    extern MS_U16 MDrv_PQ_GetTableIndex_##PQTABLE_NAME(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_GetTableIndex_(PQTABLE_NAME) \
    MS_U16 MDrv_PQ_GetTableIndex_##PQTABLE_NAME(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx){ \
        return _MDrv_PQ_GetTableIndex(u16PQSrcType, u8PQIPIdx ); \
    }


//### MDrv_PQ_GetTable
#define MDrv_PQ_GetTable_(PQTABLE_NAME, u8TabIdx, u8PQIPIdx) \
    MDrv_PQ_GetTable_##PQTABLE_NAME(u8TabIdx, u8PQIPIdx)

#define DECLARE_MDrv_PQ_GetTable_(PQTABLE_NAME) \
    extern EN_IP_Info MDrv_PQ_GetTable_##PQTABLE_NAME(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_GetTable_(PQTABLE_NAME) \
    EN_IP_Info MDrv_PQ_GetTable_##PQTABLE_NAME(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx){ \
        return _MDrv_PQ_GetTable(u8TabIdx, u8PQIPIdx); \
    }


//### MDrv_PQ_LoadTableData
#define MDrv_PQ_LoadTableData_(PQTABLE_NAME, u8TabIdx, u8PQIPIdx, pTable, u16TableSize) \
    MDrv_PQ_LoadTableData_##PQTABLE_NAME(u8TabIdx, u8PQIPIdx, pTable, u16TableSize)

#define DECLARE_MDrv_PQ_LoadTableData_(PQTABLE_NAME) \
    extern void MDrv_PQ_LoadTableData_##PQTABLE_NAME(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize);

#define INSTALL_MDrv_PQ_LoadTableData_(PQTABLE_NAME) \
    void MDrv_PQ_LoadTableData_##PQTABLE_NAME(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize){ \
        _MDrv_PQ_LoadTableData(u8TabIdx, u8PQIPIdx, pTable, u16TableSize); \
    }

//### MDrv_PQ_LoadTable
#define MDrv_PQ_LoadTable_(PQTABLE_NAME, u8TabIdx, u8PQIPIdx) \
    MDrv_PQ_LoadTable_##PQTABLE_NAME(u8TabIdx, u8PQIPIdx)

#define DECLARE_MDrv_PQ_LoadTable_(PQTABLE_NAME) \
    extern void MDrv_PQ_LoadTable_##PQTABLE_NAME(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_LoadTable_(PQTABLE_NAME) \
    void MDrv_PQ_LoadTable_##PQTABLE_NAME(MS_U8 u8TabIdx, MS_U8 u8PQIPIdx){ \
        _MDrv_PQ_LoadTable(u8TabIdx, u8PQIPIdx); \
    }


//### MDrv_PQ_LoadCommTable
#define MDrv_PQ_LoadCommTable_(PQTABLE_NAME) \
    MDrv_PQ_LoadCommTable_##PQTABLE_NAME()

#define DECLARE_MDrv_PQ_LoadCommTable_(PQTABLE_NAME) \
    extern void MDrv_PQ_LoadCommTable_##PQTABLE_NAME(void);

#define INSTALL_MDrv_PQ_LoadCommTable_(PQTABLE_NAME) \
    void MDrv_PQ_LoadCommTable_##PQTABLE_NAME(void){ \
        _MDrv_PQ_LoadCommTable(); \
    }

//### MDrv_PQ_LoadDeFlickerTable
#define MDrv_PQ_LoadDeFlickerTable_(PQTABLE_NAME,u8DeFlickType) \
    MDrv_PQ_LoadDeFlickerTable_##PQTABLE_NAME(u8DeFlickType)

#define DECLARE_MDrv_PQ_LoadDeFlickerTable_(PQTABLE_NAME) \
    extern void MDrv_PQ_LoadDeFlickerTable_##PQTABLE_NAME(MS_U8 u8DeFlickType);

#define INSTALL_MDrv_PQ_LoadDeFlickerTable_(PQTABLE_NAME) \
    void MDrv_PQ_LoadDeFlickerTable_##PQTABLE_NAME(MS_U8 u8DeFlickType){ \
        _MDrv_PQ_LoadDeFlickerTable(u8DeFlickType); \
    }

//### MDrv_PQ_AddTable
#define MDrv_PQ_AddTable_(PQTABLE_NAME, pPQTableInfo) \
    MDrv_PQ_AddTable_##PQTABLE_NAME(pPQTableInfo)

#define DECLARE_MDrv_PQ_AddTable_(PQTABLE_NAME) \
    extern void MDrv_PQ_AddTable_##PQTABLE_NAME(PQTABLE_INFO *pPQTableInfo);


#define INSTALL_MDrv_PQ_AddTable_(PQTABLE_NAME) \
    void MDrv_PQ_AddTable_##PQTABLE_NAME(PQTABLE_INFO *pPQTableInfo){ \
        _MDrv_PQ_AddTable(pPQTableInfo); \
    }


//### MDrv_PQ_LoadTableBySrcType
#define MDrv_PQ_LoadTableBySrcType_(PQTABLE_NAME, u16PQSrcType, u8PQIPIdx) \
    MDrv_PQ_LoadTableBySrcType_##PQTABLE_NAME(u16PQSrcType, u8PQIPIdx)

#define DECLARE_MDrv_PQ_LoadTableBySrcType_(PQTABLE_NAME) \
    extern void MDrv_PQ_LoadTableBySrcType_##PQTABLE_NAME(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_LoadTableBySrcType_(PQTABLE_NAME) \
    void MDrv_PQ_LoadTableBySrcType_##PQTABLE_NAME(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx){ \
        _MDrv_PQ_LoadTableBySrcType(u16PQSrcType, u8PQIPIdx); \
    }


//### MDrv_PQ_CheckCommTable
#define MDrv_PQ_CheckCommTable_(PQTABLE_NAME) \
    MDrv_PQ_CheckCommTable_##PQTABLE_NAME()

#define DECLARE_MDrv_PQ_CheckCommTable_(PQTABLE_NAME) \
    extern void MDrv_PQ_CheckCommTable_##PQTABLE_NAME(void);

#define INSTALL_MDrv_PQ_CheckCommTable_(PQTABLE_NAME) \
    void MDrv_PQ_CheckCommTable_##PQTABLE_NAME(void){ \
        _MDrv_PQ_CheckCommTable(); \
    }


//### MDrv_PQ_CheckTableBySrcType
#define MDrv_PQ_CheckTableBySrcType_(PQTABLE_NAME, u16PQSrcType, u8PQIPIdx) \
    MDrv_PQ_CheckTableBySrcType_##PQTABLE_NAME(u16PQSrcType, u8PQIPIdx)

#define DECLARE_MDrv_PQ_CheckTableBySrcType_(PQTABLE_NAME) \
    extern void MDrv_PQ_CheckTableBySrcType_##PQTABLE_NAME(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx);

#define INSTALL_MDrv_PQ_CheckTableBySrcType_(PQTABLE_NAME) \
    void MDrv_PQ_CheckTableBySrcType_##PQTABLE_NAME(MS_U16 u16PQSrcType, MS_U8 u8PQIPIdx){ \
        _MDrv_PQ_CheckTableBySrcType(u16PQSrcType, u8PQIPIdx); \
    }

//### MDrv_PQ_GetXRuleIPIndex
#define MDrv_PQ_GetXRuleIPIndex_(PQTABLE_NAME, u8XRuleType, u8XRuleIP) \
    MDrv_PQ_GetXRuleIPIndex_##PQTABLE_NAME(u8XRuleType, u8XRuleIP)

#define DECLARE_MDrv_PQ_GetXRuleIPIndex_(PQTABLE_NAME) \
    extern MS_U8 MDrv_PQ_GetXRuleIPIndex_##PQTABLE_NAME(MS_U8 u8XRuleType, MS_U8 u8XRuleIP);

#define INSTALL_MDrv_PQ_GetXRuleIPIndex_(PQTABLE_NAME) \
    MS_U8 MDrv_PQ_GetXRuleIPIndex_##PQTABLE_NAME(MS_U8 u8XRuleType, MS_U8 u8XRuleIP){ \
        return _MDrv_PQ_GetXRuleIPIndex(u8XRuleType, u8XRuleIP); \
    }

//### MDrv_PQ_GetXRuleTableIndex
#define MDrv_PQ_GetXRuleTableIndex_(PQTABLE_NAME, u8XRuleType, u8XRuleIdx, u8XRuleIP) \
    MDrv_PQ_GetXRuleTableIndex_##PQTABLE_NAME(u8XRuleType, u8XRuleIdx, u8XRuleIP)

#define DECLARE_MDrv_PQ_GetXRuleTableIndex_(PQTABLE_NAME) \
    extern MS_U8 MDrv_PQ_GetXRuleTableIndex_##PQTABLE_NAME(MS_U8 u8XRuleType, MS_U8 u8XRuleIdx, MS_U8 u8XRuleIP);

#define INSTALL_MDrv_PQ_GetXRuleTableIndex_(PQTABLE_NAME) \
    MS_U8 MDrv_PQ_GetXRuleTableIndex_##PQTABLE_NAME(MS_U8 u8XRuleType, MS_U8 u8XRuleIdx, MS_U8 u8XRuleIP){ \
        return _MDrv_PQ_GetXRuleTableIndex(u8XRuleType, u8XRuleIdx, u8XRuleIP); \
    }


//### MDrv_PQ_GetXRuleIPNum
#define MDrv_PQ_GetXRuleIPNum_(PQTABLE_NAME, u8XRuleType) \
    MDrv_PQ_GetXRuleIPNum_##PQTABLE_NAME(u8XRuleType)

#define DECLARE_MDrv_PQ_GetXRuleIPNum_(PQTABLE_NAME) \
    extern MS_U8 MDrv_PQ_GetXRuleIPNum_##PQTABLE_NAME(MS_U8 u8XRuleType);

#define INSTALL_MDrv_PQ_GetXRuleIPNum_(PQTABLE_NAME) \
    MS_U8 MDrv_PQ_GetXRuleIPNum_##PQTABLE_NAME(MS_U8 u8XRuleType){ \
        return _MDrv_PQ_GetXRuleIPNum(u8XRuleType); \
    }


//### MDrv_PQ_GetGRule_LevelIndex
#define MDrv_PQ_GetGRule_LevelIndex_(PQTABLE_NAME, u8GRuleType, u8GRuleLevelIndex) \
    MDrv_PQ_GetGRule_LevelIndex_##PQTABLE_NAME(u8GRuleType, u8GRuleLevelIndex)

#define DECLARE_MDrv_PQ_GetGRule_LevelIndex_(PQTABLE_NAME) \
    extern MS_U8 MDrv_PQ_GetGRule_LevelIndex_##PQTABLE_NAME(MS_U8 u8GRuleType, MS_U8 u8GRuleLevelIndex);

#define INSTALL_MDrv_PQ_GetGRule_LevelIndex_(PQTABLE_NAME) \
    MS_U8 MDrv_PQ_GetGRule_LevelIndex_##PQTABLE_NAME(MS_U8 u8GRuleType, MS_U8 u8GRuleLevelIndex){ \
        return _MDrv_PQ_GetGRule_LevelIndex(u8GRuleType, u8GRuleLevelIndex); \
    }


//### MDrv_PQ_GetGRule_IPIndex
#define MDrv_PQ_GetGRule_IPIndex_(PQTABLE_NAME, u8GRuleType, u8GRuleIPIndex) \
    MDrv_PQ_GetGRule_IPIndex_##PQTABLE_NAME(u8GRuleType, u8GRuleIPIndex)

#define DECLARE_MDrv_PQ_GetGRule_IPIndex_(PQTABLE_NAME) \
    extern MS_U8 MDrv_PQ_GetGRule_IPIndex_##PQTABLE_NAME(MS_U8 u8GRuleType, MS_U8 u8GRuleIPIndex);

#define INSTALL_MDrv_PQ_GetGRule_IPIndex_(PQTABLE_NAME) \
    MS_U8 MDrv_PQ_GetGRule_IPIndex_##PQTABLE_NAME(MS_U8 u8GRuleType, MS_U8 u8GRuleIPIndex){ \
        return _MDrv_PQ_GetGRule_IPIndex(u8GRuleType, u8GRuleIPIndex); \
    }


//### MDrv_PQ_GetGRule_TableIndex
#define MDrv_PQ_GetGRule_TableIndex_(PQTABLE_NAME, u8GRuleType, u8PQSrcType, u8PQ_NRIdx, u8GRuleIPIndex) \
    MDrv_PQ_GetGRule_TableIndex_##PQTABLE_NAME(u8GRuleType, u8PQSrcType, u8PQ_NRIdx, u8GRuleIPIndex)

#define DECLARE_MDrv_PQ_GetGRule_TableIndex_(PQTABLE_NAME) \
    extern MS_U8 MDrv_PQ_GetGRule_TableIndex_##PQTABLE_NAME(MS_U8 u8GRuleType, MS_U8 u8PQSrcType, MS_U8 u8PQ_NRIdx, MS_U8 u8GRuleIPIndex);

#define INSTALL_MDrv_PQ_GetGRule_TableIndex_(PQTABLE_NAME) \
    MS_U8 MDrv_PQ_GetGRule_TableIndex_##PQTABLE_NAME(MS_U8 u8GRuleType, MS_U8 u8PQSrcType, MS_U8 u8PQ_NRIdx, MS_U8 u8GRuleIPIndex){ \
        return _MDrv_PQ_GetGRule_TableIndex(u8GRuleType, u8PQSrcType, u8PQ_NRIdx, u8GRuleIPIndex); \
    }

//### MDrv_PQ_Set_MLoadEn
#define MDrv_PQ_Set_MLoadEn_(PQTABLE_NAME, bEn) \
    MDrv_PQ_Set_MLoadEn_##PQTABLE_NAME(bEn)

#define DECLARE_MDrv_PQ_Set_MLoadEn_(PQTABLE_NAME) \
    extern void MDrv_PQ_Set_MLoadEn_##PQTABLE_NAME(MS_BOOL bEn);

#define INSTALL_MDrv_PQ_Set_MLoadEn_(PQTABLE_NAME) \
    void MDrv_PQ_Set_MLoadEn_##PQTABLE_NAME(MS_BOOL bEn){ \
        _MDrv_PQ_Set_MLoadEn(bEn); \
    }

#define DECLARE_PQ_FUNCTIONS(table_name) \
    DECLARE_MDrv_PQ_LoadCommTable_(table_name) \
    DECLARE_MDrv_PQ_DumpTable_(table_name) \
    DECLARE_MDrv_PQ_GetIPNum_(table_name) \
    DECLARE_MDrv_PQ_GetTableNum_(table_name) \
    DECLARE_MDrv_PQ_GetTableIndex_(table_name) \
    DECLARE_MDrv_PQ_GetCurrentTableIndex_(table_name) \
    DECLARE_MDrv_PQ_GetTable_(table_name) \
    DECLARE_MDrv_PQ_LoadTableData_(table_name) \
    DECLARE_MDrv_PQ_LoadTable_(table_name) \
    DECLARE_MDrv_PQ_LoadCommTable_(table_name) \
    DECLARE_MDrv_PQ_LoadDeFlickerTable_(table_name) \
    DECLARE_MDrv_PQ_LoadTableBySrcType_(table_name) \
    DECLARE_MDrv_PQ_CheckCommTable_(table_name) \
    DECLARE_MDrv_PQ_CheckTableBySrcType_(table_name) \
    DECLARE_MDrv_PQ_AddTable_(table_name) \
    DECLARE_MDrv_PQ_GetXRuleIPIndex_(table_name) \
    DECLARE_MDrv_PQ_GetXRuleTableIndex_(table_name) \
    DECLARE_MDrv_PQ_GetXRuleIPNum_(table_name) \
    DECLARE_MDrv_PQ_GetGRule_LevelIndex_(table_name) \
    DECLARE_MDrv_PQ_GetGRule_IPIndex_(table_name) \
    DECLARE_MDrv_PQ_GetGRule_TableIndex_(table_name) \
    DECLARE_MDrv_PQ_Set_MLoadEn_(table_name)


#define INSTALL_PQ_FUNCTIONS(table_name) \
    INSTALL_MDrv_PQ_DumpTable_(table_name) \
    INSTALL_MDrv_PQ_GetIPNum_(table_name) \
    INSTALL_MDrv_PQ_GetTableNum_(table_name) \
    INSTALL_MDrv_PQ_GetTableIndex_(table_name) \
    INSTALL_MDrv_PQ_GetCurrentTableIndex_(table_name) \
    INSTALL_MDrv_PQ_GetTable_(table_name) \
    INSTALL_MDrv_PQ_LoadTableData_(table_name) \
    INSTALL_MDrv_PQ_LoadTable_(table_name) \
    INSTALL_MDrv_PQ_LoadCommTable_(table_name) \
    INSTALL_MDrv_PQ_LoadDeFlickerTable_(table_name) \
    INSTALL_MDrv_PQ_LoadTableBySrcType_(table_name) \
    INSTALL_MDrv_PQ_CheckCommTable_(table_name) \
    INSTALL_MDrv_PQ_CheckTableBySrcType_(table_name) \
    INSTALL_MDrv_PQ_AddTable_(table_name) \
    INSTALL_MDrv_PQ_GetXRuleIPIndex_(table_name) \
    INSTALL_MDrv_PQ_GetXRuleTableIndex_(table_name) \
    INSTALL_MDrv_PQ_GetXRuleIPNum_(table_name) \
    INSTALL_MDrv_PQ_GetGRule_LevelIndex_(table_name) \
    INSTALL_MDrv_PQ_GetGRule_IPIndex_(table_name) \
    INSTALL_MDrv_PQ_GetGRule_TableIndex_(table_name) \
    INSTALL_MDrv_PQ_Set_MLoadEn_(table_name)


// TODO: declare PQ functions for all tables in this file
DECLARE_PQ_FUNCTIONS(MAIN)    // table config parameter
DECLARE_PQ_FUNCTIONS(MAINEX)      // table config parameter


#include "Kirin_Main.h"              // table config parameter
#include "Kirin_Main_HSDRule.h"      // table config parameter
#include "Kirin_Main_VSDRule.h"      // table config parameter
#include "Kirin_Main_HSPRule.h"      // table config parameter
#include "Kirin_Main_VSPRule.h"      // table config parameter
#include "Kirin_Main_CSCRule.h"      // table config parameter
#include "Kirin_Main_GRule.h"        // table config parameter
#include "Kirin_Main_Ex.h"           // table config parameter

#undef _DRVPQ_DECLARE_H
#endif
