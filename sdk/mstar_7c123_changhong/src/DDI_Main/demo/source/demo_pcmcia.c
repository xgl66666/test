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
/// @file   appDemo_CI.c
/// @author MStar Semiconductor Inc.
/// @brief A sample code for illustrating how to control PCMCIA functions
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_CI.c
   @brief The sample code for the CI_XXX functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include <string.h>
#include "MsCommon.h"
#include "drvPCMCIA.h"
#include "drvGPIO.h"
#include "Board.h"

#if (DEMO_PCMCIA_TEST == 1)


void _CB_Pcmcia(void* param1, void* param2)
{
    PCMCIA_ISR isr = *(PCMCIA_ISR*)param1;
    printf("%s\n", __FUNCTION__);
    if(isr.bISRCardInsert)
    {
        printf("Card Insert\n");
    }

    if(isr.bISRCardRemove)
    {
        printf("Card Remove\n");
    }

    if(isr.bISRCardData)
    {
        printf("Card data\n");
    }

}
#define MAX_LOCAL_CIS_SIZE 512
static MS_U8 pu8CIS[MAX_LOCAL_CIS_SIZE] = {0};

MS_BOOL Demo_PCMCIA(MS_U8 *devPath, MS_U32 * partitions, MS_U32 *p1_size, MS_U32 *p2_size, MS_U32 *p3_size, MS_U32 *p4_size)
{
    int i = 0;
    memset(&pu8CIS[0],0,sizeof(MS_U8)*MAX_LOCAL_CIS_SIZE);
    PCMCIA_INFO pInfo;
    MS_U16 m_u16LpduBufferSize=0;
    printf("%s\n", __FUNCTION__);
    MDrv_PCMCIA_Init(FALSE);

    MDrv_PCMCIA_InstarllIsrCallback(_CB_Pcmcia);
    MDrv_PCMCIA_Enable_Interrupt(TRUE);

    printf("waiting for card insert\n");

    MDrv_PCMCIA_Polling();

    if(FALSE == MDrv_PCMCIA_IsModuleStillPlugged())
    {
        printf("[%s] CI not detect\n", __FUNCTION__);
        return FALSE;
    }
    else
    {
        printf("[CI] CI detect ok\n");
    }

    TunerOnPCMCIA();

    printf("delay 1s\n");
    MsOS_DelayTask(1000);
    printf("delay 1s done\n");

    MDrv_PCMCIA_ResetHW();
    printf("reset HW ok\n");

    printf("delay 3s\n");
    MsOS_DelayTask(3000);
    printf("delay 3s done\n");

    for(i=0;i<0x100;i++)
    {
        if(i%16 == 0)
            printf("\n");
        MDrv_PCMCIA_ReadAttribMem(i, &pu8CIS[i]);
        printf("%02X ", pu8CIS[i]);
    }

    printf("\n");

    MDrv_PCMCIA_ParseAttribMem(pu8CIS, 512, &pInfo);

    printf( "ManufacturerName %s \n", pInfo.pszManufacturerName );
    printf( "ProductName %s \n", pInfo.pszProductName );
    printf( "ProductInfo1 %s \n", pInfo.pszProductInfo1 );
    printf( "ProductInfo2 %s \n", pInfo.pszProductInfo2 );
    printf( "fCITagsPresent %X \n", pInfo.Config[0].fCITagsPresent );
    printf("COR  Addr %X\n",(unsigned int) pInfo.ConfigOffset);
    printf("COR  Value  %X\n",pInfo.Config[0].bConfigIndex);

    if(FALSE == MDrv_PCMCIA_SwitchToIOmode(&pInfo))
    {
        printf("MDrv_PCMCIA_SwitchToIOmode FAIL\n");
    }

    if(FALSE == MDrv_PCMCIA_ResetInterface())
    {
        printf("MDrv_PCMCIA_ResetInterface FAIL\n");
    }

    MsOS_DelayTask(1000);
    printf("[CI] Nego buf size\n");
    //printf("Buffer size = 0x%x\n", (unsigned int)MDrv_PCMCIA_NegotiateBufferSize(&pInfo));
    m_u16LpduBufferSize = MDrv_PCMCIA_NegotiateBufferSizeV2(E_PCMCIA_MODULE_A, &pInfo);
    if(0 == m_u16LpduBufferSize)
    {
        printf("[%s] Negotiate buffer size FAIL\n", __FUNCTION__);
        return FALSE;
    }
    else
    {
        printf("[CI] Negotiate buffer size %u\n", m_u16LpduBufferSize);
        MDrv_PCMCIA_WriteBufferSizeV2(E_PCMCIA_MODULE_A, m_u16LpduBufferSize);
    }

    MS_U8 WriteBuffer[] = {0x01, 0x00, 0x82, 0x01, 0x01};
    MS_U8 ReadBuffer[9];
    MS_U8 GoldenBuffer[9] = {0x01, 0x00, 0x83, 0x01, 0x01, 0x80, 0x02, 0x01, 0x00};
    //printf("[CI] WriteData\n");
    MDrv_PCMCIA_WriteDataV2(E_PCMCIA_MODULE_A, WriteBuffer, sizeof(WriteBuffer) );
    printf("[CI] WriteData PASS\n");
    MDrv_PCMCIA_ReadDataV2(E_PCMCIA_MODULE_A, ReadBuffer, 9 );

    if(memcmp(ReadBuffer, GoldenBuffer, 9) != 0)
    {
        int index = 0;
        for(index = 0; index < 9; index++)
        {
            printf("0x%02x ", ReadBuffer[index]);
        }
    }
    else
    {
        printf("[CI] ReadData PASS\n");
    }
    TunerOffPCMCIA();
    return TRUE;
}

#endif
