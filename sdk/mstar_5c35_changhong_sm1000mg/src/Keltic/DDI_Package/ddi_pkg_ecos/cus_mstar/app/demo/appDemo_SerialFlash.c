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


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_SerialFlash.c
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for Serial Flash control functions.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "drvSERFLASH.h"
#include "drvMMIO.h"

#include "appDemo_SerialFlash.h"

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
    #include "./bindata/_p400x400_RGB565bmp.dat"
};

static unsigned char _SERFLASH_COMPAREBIN[]  =
{
    #include "./bindata/_p400x400_RGB565bmp.dat"
};

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief switch serial flash control.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_switch \n
///
/// Sample Command: \b SerialFlash_switch \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SerFlash_switch(MS_U32 *u32device_select)
{
#define INTERNAL_DEVICE    0
#define EXTERNAL_DEVICE_0  1
#define EXTERNAL_DEVICE_1  2

#define DEVICE_CZ0 0
#define DEVICE_CZ1 1

#define DEVICE_CZ0_MASK   0x88
#define DEVICE_CZ0_ENABLE   0x08
#define DEVICE_CZ0_INTERNAL 0x80
#define DEVICE_CZ0_EXTERNAL 0x00

#define DEVICE_CZ_MASK    0x03

#define DEVICE_CZ0_SELECT 0xE3E
#define DEVICE_CZ_SELECT 0x8F4

    MS_U32 u32Device_CZ0_Select = 0,u32BankBase,u32BankSize = 0,u32Device_CZ_Select=0;
    MS_U16 u16CZ0Value = 0,u16CZValue = 0;

    if ((u32device_select == NULL)||(*u32device_select > 2))
    {
        printf("[%s][%d]Parameter Error \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (MDrv_MMIO_GetBASE(&u32BankBase,&u32BankSize,MS_MODULE_PM)==FALSE)
    {
        printf("MMIO Error \n");
        return FALSE;
    }
    else
    {
        u32Device_CZ0_Select = u32BankBase + (DEVICE_CZ0_SELECT<<1);
        u32Device_CZ_Select = u32BankBase + (DEVICE_CZ_SELECT<<1);
     
        u16CZ0Value = *((volatile MS_U16*)(u32Device_CZ0_Select));
        u16CZValue = *((volatile MS_U16*)(u32Device_CZ_Select));
    }
    printf("[%s][%d] %lx %x \n",__FUNCTION__,__LINE__,u32Device_CZ0_Select,u16CZ0Value);
    printf("[%s][%d] %lx %x \n",__FUNCTION__,__LINE__,u32Device_CZ_Select,u16CZValue);
    /* Clear Setting */

    u16CZ0Value = u16CZ0Value & (~DEVICE_CZ0_MASK);
    u16CZValue = u16CZValue & (~DEVICE_CZ_MASK);

    printf("[%s][%d] %lx %x \n",__FUNCTION__,__LINE__,u32Device_CZ0_Select,u16CZ0Value);
    printf("[%s][%d] %lx %x \n",__FUNCTION__,__LINE__,u32Device_CZ_Select,u16CZValue);

    *((volatile MS_U16*)(u32Device_CZ0_Select)) = u16CZ0Value;
    *((volatile MS_U16*)(u32Device_CZ_Select)) = u16CZValue;

    if (*u32device_select == INTERNAL_DEVICE)
    {
        /* MCP SPI Device of CZ 0 */
        u16CZValue = u16CZValue | DEVICE_CZ0;
        *((volatile MS_U16*)(u32Device_CZ_Select)) = u16CZValue;
        u16CZ0Value = u16CZ0Value | DEVICE_CZ0_INTERNAL | DEVICE_CZ0_ENABLE;
        *((volatile MS_U16*)(u32Device_CZ0_Select)) = u16CZ0Value;
    }
    else if (*u32device_select == EXTERNAL_DEVICE_0)
    {
        /* External SPI Device of CZ 0 */
        u16CZ0Value = u16CZ0Value | DEVICE_CZ0_EXTERNAL | DEVICE_CZ0_ENABLE;
        *((volatile MS_U16*)(u32Device_CZ0_Select)) = u16CZ0Value;
        u16CZValue = u16CZValue | DEVICE_CZ0;
        *((volatile MS_U16*)(u32Device_CZ_Select)) = u16CZValue;

    }
    else if (*u32device_select == EXTERNAL_DEVICE_1)
    {
        /* External SPI Device of CZ 1 */
        u16CZ0Value = u16CZ0Value | DEVICE_CZ0_EXTERNAL | DEVICE_CZ0_ENABLE;
        *((volatile MS_U16*)(u32Device_CZ0_Select)) = u16CZ0Value;
        u16CZValue = u16CZValue | DEVICE_CZ1;
        *((volatile MS_U16*)(u32Device_CZ_Select)) = u16CZValue;
    }


    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief initialze serial flash control.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_Init \n
///
/// Sample Command: \b SerialFlash_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SerFlash_Init(void)
{
    MDrv_SERFLASH_Init();

    //set to RIU mode
    MDrv_SERFLASH_SetMode(0);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief read the ID of Serial Flash.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_ReadID \n
///
/// Sample Command: \b SerialFlash_ReadID \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SerFlash_ReadID(void)
{

    MS_U8   u8FlashId[3];

    MDrv_SERFLASH_ReadID(u8FlashId,sizeof(u8FlashId));


    printf("appDemo_SerFlash_ReadID()\n"
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
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b SerialFlash_GetInfo \n
///
/// Sample Command: \b SerialFlash_GetInfo \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SerFlash_GetInfo(void)
{


    pSerFlashInfo = MDrv_SERFLASH_GetInfo();

    printf("appDemo_SerFlash_GetInfo()\n"
                           "\tu32AccessWidth = %d\n"
                           "\tu32TotalSize   = %d\n"
                           "\tu32SecNum      = %d\n"
                           "\tu32SecSize     = %d\n",
                           (int)(pSerFlashInfo->u32AccessWidth),
                           (int)(pSerFlashInfo->u32TotalSize),
                           (int)(pSerFlashInfo->u32SecNum),
                           (int)(pSerFlashInfo->u32SecSize)
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
MS_BOOL appDemo_SerFlash_RWTest(MS_U32 *u32StartAddr)
{
    MS_U32 u32StartBlock, u32EndBlock;
    MS_U32 u32EraseSize;
    unsigned char* pRead_Bin = NULL;
    MS_U32 i;
    MS_U32 u32Timer = 0;

    MDrv_SERFLASH_WriteProtect(FALSE);

    if(FALSE == MDrv_SERFLASH_AddressToBlock(*u32StartAddr, &u32StartBlock))
    {
        printf("appDemo_SerFlash_RWTest u32StartAddr[0x%lx] is error \n",*u32StartAddr);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest u32StartBlock[0x%lx] \n",u32StartBlock);
    }

    u32EraseSize = sizeof(_SERFLASH_TESTBIN);

    if(FALSE == MDrv_SERFLASH_AddressToBlock((*u32StartAddr) + u32EraseSize - 1, &u32EndBlock))
    {
        printf("appDemo_SerFlash_RWTest u32StartAddr[0x%lx] + u32EraseSize[0x%lx] = [0x%lx] is error \n",*u32StartAddr,u32EraseSize,(*u32StartAddr) + u32EraseSize - 1);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest u32EndBlock[0x%lx] \n",u32EndBlock);
    }

    printf("bin size %lx u32StartBlock %ld u32EndBlock %ld \n",u32EraseSize,u32StartBlock,u32EndBlock);
    u32Timer = MsOS_GetSystemTime();
    if(FALSE == MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, TRUE))
    {
        printf("appDemo_SerFlash_RWTest  BlockErase u32StartBlock[0x%lx], u32EndBlock[0x%lx] is FAIL \n",u32StartBlock,u32EndBlock);
        return FALSE;
    }
    else
    {
        printf("appDemo_SerFlash_RWTest BlockErase u32StartBlock[0x%lx], u32EndBlock[0x%lx] success!!\n",u32StartBlock,u32EndBlock);
    }

    printf("erase time %ld \n", (MsOS_GetSystemTime()-u32Timer));
    u32Timer = MsOS_GetSystemTime();
    if(FALSE == MDrv_SERFLASH_Write(*u32StartAddr,u32EraseSize,_SERFLASH_TESTBIN))
    {
        printf("appDemo_SerFlash_RWTest  Write u32StartAddr[0x%lx], Size[0x%lx] is FAIL \n",*u32StartAddr,u32EraseSize);
        return FALSE;
    }
    {
        printf("appDemo_SerFlash_RWTest Write u32StartAddr[0x%lx], Size[0x%lx] success!!\n",*u32StartAddr,u32EraseSize);
    }
    printf("write time %ld \n", (MsOS_GetSystemTime()-u32Timer));

    pRead_Bin = _SERFLASH_COMPAREBIN;
    memset(pRead_Bin,0,u32EraseSize);

    u32Timer = MsOS_GetSystemTime();
    if(FALSE == MDrv_SERFLASH_Read(*u32StartAddr,u32EraseSize,pRead_Bin))
    //if(FALSE == MDrv_SERFLASH_DMA(*u32StartAddr,(MS_U32)pRead_Bin,u32EraseSize))
    {
        printf("appDemo_SerFlash_RWTest  Read u32StartAddr[0x%lx], Size[0x%lx] is FAIL \n",*u32StartAddr,u32EraseSize);
        return FALSE;
    }
    {
        printf("appDemo_SerFlash_RWTest Read u32StartAddr[0x%lx], Size[0x%lx] success!!\n",*u32StartAddr,u32EraseSize);
    }

    printf("read time %ld \n", (MsOS_GetSystemTime()-u32Timer));
    for(i=0;i<u32EraseSize;i++)
    {
        if(_SERFLASH_TESTBIN[i] != pRead_Bin[i])
        {
            printf("appDemo_SerFlash_RWTest Compare write[0x%lx]=0x%x, read[0x%lx]=0x%x\n",i,_SERFLASH_TESTBIN[i],i,pRead_Bin[i]);
            return FALSE;
        }
    }

    MDrv_SERFLASH_WriteProtect(TRUE);
    return TRUE;
}

