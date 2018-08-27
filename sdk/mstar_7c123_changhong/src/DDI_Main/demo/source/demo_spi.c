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
/// @file   appDemo_SerialFlash.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for the Serial Flash functions
///
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_SPI_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "drvSERFLASH.h"
#include "demo_spi.h"
#include "demo_utility.h"
#include "drvDTC.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
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
//  Local Functions
//-------------------------------------------------------------------------------------------------
const SERFLASH_Info* pSerFlashInfo;

static unsigned char _SERFLASH_TESTBIN[]  =
{
    #include "./../../../../../../DDI_Main/demo/data/_p400x400_RGB565bmp.dat"
};

static unsigned char _SERFLASH_COMPAREBIN[]  =
{
    #include "./../../../../../../DDI_Main/demo/data/_p400x400_RGB565bmp.dat"
};

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief initialze serial flash control.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_Init \n
/// Sample Command: \b SerialFlash_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SerFlash_Init(void)
{
    MDrv_SERFLASH_Init();

    //set to RIU mode
    MDrv_SERFLASH_SetMode(0);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief read the ID of Serial Flash.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_ReadID \n
///
/// Sample Command: \b SerialFlash_ReadID \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SerFlash_ReadID(void)
{
    MS_U8   u8FlashId[3];

    MDrv_SERFLASH_ReadID(u8FlashId,sizeof(u8FlashId));

    printf("Demo_SerFlash_ReadID()\n"
                           "\tMID = 0x%x\n"
                           "\tDID0   = 0x%x\n"
                           "\tDID1     = 0x%x\n",
                           (int)u8FlashId[0],
                           (int)u8FlashId[1],
                           (int)u8FlashId[2]
                           );

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief get the information of Serial Flash.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_GetInfo \n
///
/// Sample Command: \b SerialFlash_GetInfo \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SerFlash_GetInfo(void)
{
    pSerFlashInfo = MDrv_SERFLASH_GetInfo();

    printf("Demo_SerFlash_GetInfo()\n"
                           "\tu32AccessWidth = %"DTC_MS_U32_d"\n"
                           "\tu32TotalSize   = %"DTC_MS_U32_d"\n"
                           "\tu32SecNum      = %"DTC_MS_U32_d"\n"
                           "\tu32SecSize     = %"DTC_MS_U32_d"\n",
                           pSerFlashInfo->u32AccessWidth,
                           pSerFlashInfo->u32TotalSize,
                           pSerFlashInfo->u32SecNum,
                           pSerFlashInfo->u32SecSize
                           );

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The Serial Flash Read/Write test. \n
///        The testing flow: \n
///          (1) erase the Serial Flash. \n
///          (2) write data to Serial Flash. \n
///          (3) read data from Serial Flash. \n
///          (4) compare read/write data. \n
/// @param[in] u32StartAddr The address of Read/Write
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_RWTest \n
///
/// Sample Command: \b SerialFlash_RWTest 2097152 \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SerFlash_RWTest(MS_U32 *u32StartAddr)
{
    MS_U32 u32StartBlock, u32EndBlock;
    MS_U32 u32EraseSize;
    unsigned char* pRead_Bin = NULL;
    MS_U32 i;
    MS_S32 gs32NonCachedPoolID = INVALID_POOL_ID;
    MS_U8 *TempBuffer = NULL;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&gs32NonCachedPoolID);

    if(FALSE == MDrv_SERFLASH_WriteProtect_Disable_All_Range())
    {
        printf("Demo_SerFlash_RWTest MDrv_SERFLASH_WriteProtect_Disable_All_Range is error \n");
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest MDrv_SERFLASH_WriteProtect_Disable_All_Range is success \n");
    }

    if(FALSE == MDrv_SERFLASH_AddressToBlock(*u32StartAddr, &u32StartBlock))
    {
        printf("appDemo_SerFlash_RWTest u32StartAddr[0x%"DTC_MS_U32_x"] is error \n", *u32StartAddr);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest u32StartBlock[0x%"DTC_MS_U32_x"] \n", u32StartBlock);
    }

    //u32EraseSize = sizeof(_SERFLASH_TESTBIN);
    u32EraseSize = sizeof(_SERFLASH_TESTBIN);

    if(FALSE == MDrv_SERFLASH_AddressToBlock((*u32StartAddr) + u32EraseSize - 1, &u32EndBlock))
    {
        printf("appDemo_SerFlash_RWTest u32StartAddr[0x%"DTC_MS_U32_x"] + u32EraseSize[0x%"DTC_MS_U32_x"] = [0x%"DTC_MS_U32_x"] is error \n",
            *u32StartAddr, u32EraseSize, (*u32StartAddr) + u32EraseSize - 1);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest u32EndBlock[0x%"DTC_MS_U32_x"] \n", u32EndBlock);
    }

    MS_S32 u32BackupSize = (u32EndBlock-u32StartBlock +1)* pSerFlashInfo->u32SecSize;
    printf("appDemo_SerFlash_RWTest u32BackupSize[0x%"DTC_MS_U32_x"] \n", u32BackupSize);
    TempBuffer = MsOS_AllocateMemory( u32BackupSize , gs32NonCachedPoolID);

    // Store the original SPI data to DRAM
    if(FALSE == MDrv_SERFLASH_Read(*u32StartAddr,u32BackupSize,TempBuffer))
    {
        printf("Temp Buffer Read u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] is FAIL \n",
            *u32StartAddr, u32BackupSize);
        return FALSE;
    }
    {
        printf("Temp Buffer Read u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] success!!\n",
            *u32StartAddr, u32BackupSize);
    }

    if(FALSE == MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, TRUE))
    {
        printf("appDemo_SerFlash_RWTest  BlockErase u32StartBlock[0x%"DTC_MS_U32_x"], u32EndBlock[0x%"DTC_MS_U32_x"] is FAIL \n",
            u32StartBlock, u32EndBlock);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest BlockErase u32StartBlock[0x%"DTC_MS_U32_x"], u32EndBlock[0x%"DTC_MS_U32_x"] success!!\n",
            u32StartBlock, u32EndBlock);
    }

    if(FALSE == MDrv_SERFLASH_Write(*u32StartAddr,u32EraseSize,_SERFLASH_TESTBIN))
    {
        printf("appDemo_SerFlash_RWTest  Write u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] is FAIL \n",
            *u32StartAddr, u32EraseSize);
        return FALSE;
    }
    {
        printf("appDemo_SerFlash_RWTest Write u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] success!!\n",
            *u32StartAddr, u32EraseSize);
    }

    pRead_Bin = _SERFLASH_COMPAREBIN;
    memset(pRead_Bin,0,u32EraseSize);

    if(FALSE == MDrv_SERFLASH_Read(*u32StartAddr,u32EraseSize,pRead_Bin))
    //if(FALSE == MDrv_SERFLASH_DMA(*u32StartAddr,(MS_U32)pRead_Bin,u32EraseSize))
    {
        printf("appDemo_SerFlash_RWTest  Read u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] is FAIL \n",
            *u32StartAddr, u32EraseSize);
        return FALSE;
    }
    {
        printf("appDemo_SerFlash_RWTest Read u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] success!!\n",
            *u32StartAddr, u32EraseSize);
    }

    for(i=0;i<u32EraseSize;i++)
    {
        if(_SERFLASH_TESTBIN[i] != pRead_Bin[i])
        {
            printf("appDemo_SerFlash_RWTest Compare write[0x%"DTC_MS_U32_x"]=0x%x, read[0x%"DTC_MS_U32_x"]=0x%x\n",
                i, _SERFLASH_TESTBIN[i], i, pRead_Bin[i]);
            return FALSE;
        }
    }

    // Erase data for spi data restore
    if(FALSE == MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, TRUE))
    {
        printf("appDemo_SerFlash_RWTest  BlockErase u32StartBlock[0x%"DTC_MS_U32_x"], u32EndBlock[0x%"DTC_MS_U32_x"] is FAIL \n",
            u32StartBlock, u32EndBlock);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest BlockErase u32StartBlock[0x%"DTC_MS_U32_x"], u32EndBlock[0x%"DTC_MS_U32_x"] success!!\n",
            u32StartBlock, u32EndBlock);
    }

    // Restore the original SPI data from DRAM
    if(FALSE == MDrv_SERFLASH_Write(*u32StartAddr,u32BackupSize,TempBuffer))
    {
        printf("Temp Buffer Write u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] is FAIL \n",
            *u32StartAddr, u32BackupSize);
        return FALSE;
    }
    {
        printf("Temp Buffer Write u32StartAddr[0x%"DTC_MS_U32_x"], Size[0x%"DTC_MS_U32_x"] success!!\n",
            *u32StartAddr, u32BackupSize);
    }

	MsOS_FreeMemory(TempBuffer,gs32NonCachedPoolID);

    return TRUE;
}
#endif
