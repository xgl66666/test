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
/// @file   appDemo_SMC.c
/// @author MStar Semiconductor Inc.
/// @brief  Uranus Sample Code Template
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_SMC.c
   @brief The demo code for the MDrv_SC functions

*/

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "apiSC.h"
#include "drvSC.h"
#include "string.h"

#if DEMO_SMC_IO_POLLING
#include "MsMemory.h"
#include "drvMMIO.h"
#include "drvPM.h"
#include "drvBDMA.h"
#include "MsOS.h"
#endif

#if (DEMO_SMC_USE_51 == 1)
#include "MsMemory.h"
#include "drvPM.h"
#include "drvMBX.h"
#endif

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define APPSMC_TASK_STACK_SIZE      4096

// Select the test cases:
#define APP_DEMO_SC_PARAM            _Default_InitParam
#define APP_DEMO_SC_CMD              _IRD_T0_CmdTest

#define APP_SMC_EVT_MASK1           0x0000000F
#define APP_SMC_EVT_MASK2           0x000000F0
#define APP_SMC_EVT_IN1             0x00000001
#define APP_SMC_EVT_OUT1            0x00000002
#define APP_SMC_EVT_IN2             0x00000010
#define APP_SMC_EVT_OUT2            0x00000020
#define APP_SMC_EVT_STOP            0x00000100



//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
#if DEMO_SMC_IO_POLLING
extern MS_U32 gs32NonCachedPoolID;
#endif
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8    u8TaskStack[APPSMC_TASK_STACK_SIZE];
static MS_S32   s32TaskId, s32EventId;
static MS_U8    u8Protocol = 0x00;
static MS_U32   u32Events;
static MS_U16   u16RetryTime = 0;
static MS_BOOL  bRunning = FALSE;
MS_U8  _u8AtrBuf[SC_ATR_LEN_MAX];
MS_U16 _u16AtrLen = 0;

#if DEMO_SMC_IO_POLLING
MS_U8 pmlitedat_polling_smc_io[]=
{
    #include "pmlite_polling_smc_io.dat"
};
#endif

#if (DEMO_SMC_USE_51 == 1)
MS_U8 u8SCFirmware[] = {
    #include "fwSC.dat"
};
#endif

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
#if (DEMO_SMC_USE_51 == 1)
MS_BOOL _appDemo_SMC_Load_Fw(MS_U32 u32FwSCAddr)
{
    MBX_Result mbxResult;
    SC_BuffAddr ScBuffAddr;

    ScBuffAddr.u32DataBuffAddr = PM51_MEM_ADR;
    ScBuffAddr.u32FwBuffAddr   = u32FwSCAddr;

    MDrv_SC_SetBuffAddr(&ScBuffAddr);

    printf("load fw to u32FwSCAddr %lx\n", u32FwSCAddr);
    memcpy((void*)MS_PA2KSEG1((MS_U32)u32FwSCAddr/*u8SCFirmware*/) , u8SCFirmware, sizeof(u8SCFirmware));
    MDrv_PM_SetDRAMOffsetForMCU(u32FwSCAddr >> 16); //64K unit

    MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_NONE);
    mbxResult = MDrv_MBX_Init(E_MBX_CPU_MIPS, E_MBX_ROLE_HK, 1500);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("MBX init error\n");
        return FALSE;
    }
    MDrv_MBX_Enable(TRUE);

    printf("E_MBX_CLASS_PM_WAIT - MBX register msg\n");
    mbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_WAIT, 6);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg error\n");
        return FALSE;
    }
    else
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg success\n");
        return TRUE;
    }
}
#endif

#if DEMO_SMC_IO_POLLING
static MS_BOOL _PM51_Polling_SMC_IO_Init(void)
{
    MS_U8 *pmlitedat_buffer = NULL;
    MS_U32 pmlitedat_buffer_pa = 0;


    if (TRUE != MDrv_BDMA_Init(0))
    {
        printf("[%s][%d] BDMA init fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    pmlitedat_buffer = MsOS_AllocateMemory(sizeof(pmlitedat_polling_smc_io), gs32NonCachedPoolID);

    if (pmlitedat_buffer == NULL)
    {
       printf("[%s][%d] Memory Allocate fail\n", __FUNCTION__, __LINE__);
       return FALSE;
    }
        
    memcpy(pmlitedat_buffer, pmlitedat_polling_smc_io, sizeof(pmlitedat_polling_smc_io));

    pmlitedat_buffer_pa = MsOS_VA2PA((MS_U32)pmlitedat_buffer);
    printf("[%s][%d] PM51_LITE_ADDR %lx \n",__FUNCTION__,__LINE__,PM51_LITE_ADR);
    if (MDrv_BDMA_CopyHnd((MS_PHYADDR)pmlitedat_buffer_pa,(MS_PHYADDR)PM51_LITE_ADR ,sizeof(pmlitedat_polling_smc_io),E_BDMA_SDRAM2SDRAM,0x08)!= E_BDMA_OK)
    {
        printf("[%s][%d] BDMA copy fail\n", __FUNCTION__, __LINE__);
        MsOS_FreeMemory(pmlitedat_buffer,gs32NonCachedPoolID);
        return FALSE;
    }

    MsOS_FreeMemory(pmlitedat_buffer,gs32NonCachedPoolID);


    printf("[%s][%d] \n",__FUNCTION__,__LINE__);
    printf("[%s][%d] PM51_LITE_ADDR Offset %lx \n",__FUNCTION__,__LINE__,PM51_LITE_ADR>>16);
    MDrv_PM_SetDRAMOffsetForMCU(PM51_LITE_ADR>>16);

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief Dump the message to the console
/// @param pu8Data \b the pointer to the message
/// @param u16DataLen \b the length of the message
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _appDemo_SMC_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen)
{
    int i;

    printf("Rx :\n         ");
    for (i = 0; i < u16DataLen; i++)
    {
        printf("%02x ", pu8Data[i]);
        if (((i+1) % 16) == 0)
        {
            printf("\n         ");
        }
    }
    printf("\n");
}


//------------------------------------------------------------------------------
/// @brief The default UART setting
/// @param pParam \b the pointer to the return param of the UART
/// @sa
/// @note
//------------------------------------------------------------------------------
#if 1
static void _Default_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_4P5M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
    pParam->u16ClkDiv = 372;
}

#elif 0
void _TEST_T1_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_4P5M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
    pParam->u16ClkDiv = 372;
}
#elif 0
void _VIA_T0_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_4P5M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
    pParam->u16ClkDiv = 372;

    printf("// ATR : 3f 77 18 00 00 c2 47 40 00 68 90 00\n");
}
#elif 0
void _IRD_T14_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_6M;
    pParam->u8UartMode = SC_UART_CHAR_8;
    pParam->u16ClkDiv = 620;
}
#endif

#if 1
//------------------------------------------------------------------------------
/// @brief The demo code to test the irdeto T=0 smart card command
/// @param u8SCID \b the smart card id
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _IRD_T0_CmdTest(MS_U32 u8SCID)
{
    SC_Result     result;
    MS_U8   _u8ReadData[255]; // is it possible more than 255 with NULL data?
    MS_U16  _u16ReadLen = 255;
    MS_U16  _u16CommandLen = 0;

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=0 command to smart card and receive the response message
        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : d2 42 00 00 01 1d\n");
            printf("// RX  : 90 19\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x90 && _u8ReadData[1] == 0x19))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0xd2, 0xfe, 0x00, 0x00, 0x19};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=0 command to smart card and receive the response message
        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : d2 fe 00 00 19\n");
            printf("// RX  : 01 02 00 00 21 00 00 10 03 03 09 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 04 90 00\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[24] == 0x04 && _u8ReadData[25] == 0x90 && _u8ReadData[26] == 0x00))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0xd2, 0x3c, 0x00, 0x00, 0x01, 0x22};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=0 command to smart card and receive the response message
        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : d2 3c 00 00 01 22\n");
            printf("// RX  : 90 15\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x90 && _u8ReadData[1] == 0x15))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }

    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0xd2, 0xfe, 0x00, 0x00, 0x15};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=0 command to smart card and receive the response message
        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : d2 fe 00 00 15\n");
            printf("// RX  : 01 02 00 00 1e 00 00 0c 50 53 01 0e 00 04 00 00\n");
            printf("//       00 00 00 00 26 90 00\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[20] == 0x26 && _u8ReadData[21] == 0x90 && _u8ReadData[22] == 0x00))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0xd2, 0x3e, 0x00, 0x00, 0x01, 0x23};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=0 command to smart card and receive the response message
        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : d2 3e 00 00 01 23\n");
            printf("// RX  : 90 0b\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x90 && _u8ReadData[1] == 0x0b))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }
}

#elif 0

static void _TF_T0_CmdTest(MS_U32 u8SCID)
{
    //
    // ATR: 3B 6C 00 00 4E 54 49 43 33 90 59 01 4A 03 00 00
    //

    SC_Result     result;
    MS_U8   _u8ReadData[255]; // is it possible more than 255 with NULL data?
    MS_U16  _u16ReadLen = 0;
    MS_U16  _u16CommandLen = 0;
    {
        _u16ReadLen = 0;
        MS_U8   _u8CommandData[]={0x80, 0x46, 0x00, 0x00, 0x04, 0x01, 0x00, 0x3a, 0x1e};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : 80 46 00 00 04 01 00 3a 1e\n");
            printf("// RX  : 61 1e\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x61 && _u8ReadData[1] == 0x1e))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 0;
        MS_U8   _u8CommandData[]={0x00, 0xc0, 0x00, 0x00, 0x1e};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : 00 c0 00 00 1e\n");
            printf("// RX  : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 00 90 00\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[30] == 0x90 && _u8ReadData[31] == 0x00))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 0;
        MS_U8   _u8CommandData[]={0x80, 0x46, 0x00, 0x03, 0x04, 0x02, 0x00, 0x00, 0x2d};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : 80 46 00 03 04 02 00 00 2d\n");
            printf("// RX  : 61 2d\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x61 && _u8ReadData[1] == 0x2d))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }

    }

    {
        _u16ReadLen = 0;
        MS_U8   _u8CommandData[]={0x00, 0xc0, 0x00, 0x00, 0x2d};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : 00 c0 00 00 2d\n");
            printf("// RX  : 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 06 bd 76 ce 90 00\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[42] == 0x76 && _u8ReadData[43] == 0xce))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 0;
        MS_U8   _u8CommandData[]={0x80, 0x46, 0x00, 0x04, 0x04, 0x02, 0x00, 0x00, 0x2d};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);
        //memset(_u8CommandData, 0, 255);

        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : 80 46 00 04 04 02 00 00 2d\n");
            printf("// RX  : 61 2d\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x61 && _u8ReadData[1] == 0x2d))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }

    }

    {
        _u16ReadLen = 0;
        MS_U8   _u8CommandData[]={0x00, 0xc0, 0x00, 0x00, 0x2d};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);
        //memset(_u8CommandData, 0, 255);

        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : 00 c0 00 00 2d\n");
            printf("// RX  : 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 90 00\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[45] == 0x90 && _u8ReadData[46] == 0x00))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }
}

#elif 0
static void _VIA_T0_CmdTest(MS_U32 u8SCID)
{
    SC_Result     result;
    MS_U8   _u8ReadData[255]; // is it possible more than 255 with NULL data?
    MS_U16  _u16ReadLen = 255;
    MS_U16  _u16CommandLen = 0;
    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0xca, 0xb8, 0x00, 0x00, 0x02};
        _u16CommandLen = sizeof(_u8CommandData);

        memset(_u8ReadData, 0, 255);

        // Check CMD
        result = MApi_SC_T0_Communication(u8SCID, _u8CommandData, &_u16CommandLen,  _u8ReadData, &_u16ReadLen);
        if (_u16ReadLen)
        {
            printf("// CMD : ca b8 00 00 02\n");
            printf("// RX  : 90 02\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x90 && _u8ReadData[1] == 0x02))
            {
                printf("1st cmd Fail!\n");
            }
        }
    }
}

#elif 0
static void _HealthInsurance_T1_CmdTest(MS_U32 u8SCID)
{   //Health Insurance card
    MS_U8 SelectAPDU[] = { 0x21, 0x00, 0x15,0x00, 0xA4, 0x04, 0x00, 0x10, 0xD1, 0x58, 0x00, 0x00, 0x01, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00 ,0x1D};
    MS_U8 ReadProfileAPDU[] = {0x21, 0x40, 0x07, 0x00, 0xca, 0x11, 0x00, 0x02, 0x00, 0x00,0xbf };
    MS_U16 _uCmdLen ;
    MS_U8 pu8ReadData[255];
    MS_U16 u16ReadDataLen = 255;

    _uCmdLen = sizeof(SelectAPDU);

    MApi_SC_T1_Parse_Atr(u8SCID, _u8AtrBuf, _u16AtrLen);
    
    if(MApi_SC_T1_Communication(u8SCID, SelectAPDU, &_uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)    
    {
        printf("MDrv_SC_T1SendRecv1  Error\n");
    }
    _appDemo_SMC_PrintData(pu8ReadData, u16ReadDataLen);

    u16ReadDataLen = 255;
    _uCmdLen = sizeof(ReadProfileAPDU);
    if(MApi_SC_T1_Communication(u8SCID, ReadProfileAPDU,& _uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
    {
        printf("MDrv_SC_T1SendRecv  Error\n");
    }
    _appDemo_SMC_PrintData(pu8ReadData, u16ReadDataLen);
}
#elif 0
//------------------------------------------------------------------------------
/// @brief The demo code to test the irdeto T=14 smart card command
/// @param u8SCID \b the smart card id
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _IRD_T14_CmdTest(MS_U32 u8SCID)
{
    SC_Result     result;
    printf("_verSC_Irdeto_T14\n");
    MS_U16  _u16ReadLen = 255;
    MS_U8   _u8ReadData[255]; // is it possible more than 255 with NULL data?
    MS_U16  _u16CommandLen;

   {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x3c};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 00 00 00 00 3c\n");
            printf("// RX  : 01 02 00 00 00 00 00 14 34 30 30 36 31 39 34 36\n");
            printf("//     : 30 36 54 34 34 39 30 34 41 00 00 00 0e\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[27] == 0x00 && _u8ReadData[28] == 0x0e))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x3d};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 01 00 00 00 3d\n");
            printf("// RX  : 01 02 00 00 01 00 00 10 ff ff ff 00 00 00 00 00\n");
            printf("//     : 00 00 04 07 00 de c7 18 d0\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[23] == 0x18 && _u8ReadData[24] == 0xd0))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x3e};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 02 00 00 00 3e\n");
            printf("// RX  : 01 02 00 00 02 00 00 10 05 05 12 06 23 06 24 06\n");
            printf("//     : 21 06 22 00 00 5a 41 46 65\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[23] == 0x46 && _u8ReadData[24] == 0x65))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x08, 0x00, 0x00, 0x00, 0x34};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 08 00 00 00 34\n");
            printf("// RX:   01 02 00 00 08 00 00 40 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
            printf("//       00 00 00 00 00 00 00 00 74\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[71] == 0x00 && _u8ReadData[72] == 0x74))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x03, 0x00, 0x00, 0x00, 0x3f};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 03 00 00 00 3\n");
            printf("// RX  : 01 02 00 00 03 00 00 18 00 03 e9 00 00 00 00 00\n");
            printf("//       00 00 00 1b 65 01 2d 2d 2d 82 62 00 00 00 00 00\n");
            printf("//       7f\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[31] == 0x00 && _u8ReadData[32] == 0x7f))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x1c, 0x00, 0x00, 0x00, 0x20};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 1c 00 00 00 20\n");
            printf("// RX  : 01 02 00 00 1c 00 00 07 00 00 00 00 00 00 00 27\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[14] == 0x00 && _u8ReadData[15] == 0x27))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }

    {
        _u16ReadLen = 255;
        MS_U8   _u8CommandData[]={0x01, 0x02, 0x03, 0x00, 0x01, 0x00, 0x3e};
        _u16CommandLen = sizeof(_u8CommandData);
        memset(_u8ReadData, 0, 255);

        // Send the T=14 command t0 smart card and receive the response message
        result = MDrv_SC_Send(u8SCID, _u8CommandData, (MS_U16)(_u16CommandLen),100);
        if (result != E_SC_OK)
        {
            printf("T14 Send command failed\n");
            return;
        }        
        result = MDrv_SC_Recv(u8SCID, _u8ReadData, &_u16ReadLen, 100);
        if (_u16ReadLen)
        {
            printf("// CMD : 01 02 03 00 01 00 3e\n");
            printf("// RX  : 01 02 00 00 03 00 01 18 01 02 00 00 00 00 00 00\n");
            printf("//       00 00 00 01 00 00 2d 2d 2d 82 62 00 00 00 00 00\n");
            printf("//       e9\n");
            _appDemo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[31] == 0x00 && _u8ReadData[32] == 0xe9))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }
}
#elif 0
static void _NAGRA_T1_CmdTest(MS_U32 u8SCID)
{
    MS_U8 u8SendData1[] = { 0x00, 0xC1, 0x01, 0xFE, 0x3E };
    MS_U8 u8SendData2[] = { 0x00, 0x00, 0x14, 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x31, 0x50,
                            0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46,
                            0x30, 0x31, 0x00, 0xDD };
    MS_U8 pu8ReadData[255];
    MS_U16 u16SendDataLen = sizeof(u8SendData1);
    MS_U16 u16ReadDataLen = 255;
    
    printf("\nSending ... 0x00, 0xC1, 0x01, 0xFE, 0x3E\n");
    MDrv_SC_RawExchange(u8SCID, u8SendData1, &u16SendDataLen, pu8ReadData, &u16ReadDataLen);
    _appDemo_SMC_PrintData(pu8ReadData, u16ReadDataLen);

    u16SendDataLen = sizeof(u8SendData2);
    u16ReadDataLen = 255;

    printf("\nSending ... 0x00, 0x00, 0x14, 0x00, 0xA4, 0x04, 0x00, 0x0E,\n");
    printf("            0x31, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53,\n");
    printf("            0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00, 0xDD \n");
    MDrv_SC_RawExchange(u8SCID, u8SendData2, &u16SendDataLen, pu8ReadData, &u16ReadDataLen);
    _appDemo_SMC_PrintData(pu8ReadData, u16ReadDataLen);
}
#endif

//------------------------------------------------------------------------------
/// @brief The demo code to reset the smart card and dump ATR message
/// @param u8SCID \b the smart card id
/// @param eEvent \b the event of the smart card
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _appDemo_SMC_Notify(MS_U8 u8SCID, SC_Event eEvent)
{
    MS_U32 u32Mask, u32Evt;

    switch (eEvent)
    {
        case E_SC_EVENT_IN:
            printf("SMC%d: Card In\n", u8SCID);
            u32Mask = (0 == u8SCID) ? APP_SMC_EVT_MASK1 : APP_SMC_EVT_MASK2;
            u32Evt = (0 == u8SCID) ? APP_SMC_EVT_IN1 : APP_SMC_EVT_IN2;
            MsOS_ClearEvent(s32EventId, u32Mask);
            u32Events &= ~u32Mask;
            u16RetryTime = 0;
            MsOS_SetEvent(s32EventId, u32Evt);
            break;
        case E_SC_EVENT_OUT:
            printf("SMC%d: Card Out\n", u8SCID);
            u32Mask = (0 == u8SCID) ? APP_SMC_EVT_MASK1 : APP_SMC_EVT_MASK2;
            u32Evt = (0 == u8SCID) ? APP_SMC_EVT_OUT1 : APP_SMC_EVT_OUT2;
            MsOS_ClearEvent(s32EventId, u32Mask);
            u32Events &= ~u32Mask;
            u16RetryTime = 0;
            MsOS_SetEvent(s32EventId, u32Evt);
            break;
        case E_SC_EVENT_DATA:
            printf("SMC%d: Incoming Data\n", u8SCID);
            break;
        default:
            printf("SMC%d: Unknown Event\n", u8SCID);
            break;
    }
}

//------------------------------------------------------------------------------
/// @brief The demo code to reset the smart card and dump ATR message
/// @param u8SCID \b the smart card id
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
//------------------------------------------------------------------------------

static MS_BOOL _appDemo_SMC_Reset2(MS_U8 u8SCID,SC_Param *pParam)
{
    //MS_U8               _u8AtrBuf[SC_ATR_LEN_MAX];
    //MS_U16              _u16AtrLen = 0;
    MS_U8               history[SC_HIST_LEN_MAX];
    MS_U16              history_len = 200;
    MS_U8               u8Protocol = 0xff;
    SC_Result  _scresult = E_SC_FAIL;
    
    APP_DEMO_SC_PARAM(pParam);

    // reset
    if (MDrv_SC_Reset(u8SCID, pParam) != E_SC_OK)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    MDrv_SC_ClearState(u8SCID);

    // Timeout value can be tuned
    _scresult = MDrv_SC_GetATR(u8SCID, 100*5, _u8AtrBuf, &_u16AtrLen, history, &history_len);
    if (_scresult != E_SC_OK) // the timeout value should be tumed by user
    {
        printf("[%s][%d] _scresult %d \n",__FUNCTION__,__LINE__,_scresult);
        return FALSE;
    }

    if (_u8AtrBuf[0] == 0x3b)
    {
        pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_EVEN;
    }
    else if (_u8AtrBuf[0] == 0x3f)
    {
        pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
    }
    else
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (MDrv_SC_Config(u8SCID, pParam) != E_SC_OK)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    // Parsing the protocol from ATR
    int i;

    if (_u8AtrBuf[1] & 0x80)
    {
        MS_U8 u8T0 = _u8AtrBuf[1] >> 4;
        i = 1;
        while (u8T0)
        {
            if (u8T0 & 1)
                i++;
            u8T0 >>= 1;
        }
        u8Protocol = _u8AtrBuf[i] & 0xF;
    }
    else
    {
        u8Protocol = 0;
    }

    // if you want to change the PPS to specific Fi and Di, use the MDrv_SC_SetPPS

    if (MDrv_SC_PPS(u8SCID) != E_SC_OK)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    printf("\n ***Smartcard%d  T=%d  ***\n", u8SCID, u8Protocol);
    {
        printf("SMC%d: ATR Message :     \n", u8SCID);
        printf("Rx :\n         ");
        for (i = 0; i < _u16AtrLen; i++)
        {
            printf("%02x ", _u8AtrBuf[i]);
            if ((i+1) % 16 == 0)
            {
                printf("\n         ");
            }
        }
        printf("\nend \n");
    }
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The demo code of the smart test task
/// @param u8SCID \b the smart card id
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _appDemo_SMC_Task(MS_U8 u8SCID)
{
    SC_Param            sInitParams;
    SC_Result           errCode;

    printf("##########    START TO TEST SMARTCARD[%d]   ##########\n", u8SCID);

    // Init the smart card driver
    if ((errCode = MDrv_SC_Init(u8SCID)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Init failed with error code=%d !!!!\n", __FUNCTION__, errCode);
        return;
    }

    // UART setting
    APP_DEMO_SC_PARAM(&sInitParams);
    sInitParams.eVccCtrl     = E_SC_VCC_CTRL_8024_ON;
    sInitParams.pfOCPControl = NULL;
    if ((errCode = MDrv_SC_Open(u8SCID, u8Protocol, &sInitParams, _appDemo_SMC_Notify)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Open failed with error code = %d!!!!\n", __FUNCTION__, errCode);
        goto EXIT;
    }

    // Test smart card
    while (bRunning)
    {
        SC_Status status;
        printf("to wait event\n");
        // Wait for the smart card in/out event or task stop event
        MsOS_WaitEvent(s32EventId, APP_SMC_EVT_IN1 | APP_SMC_EVT_OUT1 |
                APP_SMC_EVT_IN2 | APP_SMC_EVT_OUT2 | APP_SMC_EVT_STOP,
                &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        // Get the smart card status
        MDrv_SC_GetStatus(u8SCID, &status);
        //printf("status get %x\n",status);
        if (status.bCardIn)
        {
            printf(" ######## Insert Test sm[%d] OK ######## ==> [COMMAND]\n", u8SCID);
            // Test the smart card reset
            if (_appDemo_SMC_Reset2(u8SCID,&sInitParams))
            //if (_appDemo_SMC_Reset(u8SCID))
            {
                printf("[%s][%d] \n",__FUNCTION__,__LINE__);
                // Test the smart card command
                APP_DEMO_SC_CMD(u8SCID);
            }
            printf(" ########  CMD Test sm[%d] End  ######## ==> [REMOVE]\n", u8SCID);
        }
        else
        {
            printf(" ######## Remove Test sm[%d] OK ######## ==> [INSERT]\n", u8SCID);
        }
    }

    // Close the smart card
    if ((errCode = MDrv_SC_Close(u8SCID)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Close failed with error code =%d\n", __FUNCTION__, errCode);
    }

EXIT:
    // Exit the smart card driver

    return ;
}


//------------------------------------------------------------------------------
/// @brief The demo code to start the smart test task
/// @param[in] pu8SC_Id \b pointer to the smart card id
/// @return TRUE - Success
/// @return FALSE - fail
/// @sa
/// @note
/// Command: \b SMC_Start \n
///
/// Sample Command: \b SMC_Start 0  \n
///                 \b SMC_Start 1  \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SMC_Start(MS_U8 *pu8SC_Id)
{
    printf("appDemo_SMC_Start\n");
    bRunning = TRUE;

#if DEMO_SMC_IO_POLLING
    if(!_PM51_Polling_SMC_IO_Init())
         return FALSE;
#endif

#if (DEMO_SMC_USE_51 == 1)
    _appDemo_SMC_Load_Fw(PM51_LITE_ADR);
#endif

    // Create SMC event group
    s32EventId = MsOS_CreateEventGroup("appSMC_Event");
    // Create SMC demo task
    s32TaskId = MsOS_CreateTask( (TaskEntry) _appDemo_SMC_Task,
                                  (MS_U32)*pu8SC_Id,
                                  E_TASK_PRI_HIGH,
                                  TRUE,
                                  u8TaskStack,
                                  APPSMC_TASK_STACK_SIZE,
                                  "appSMC_Task" );

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The demo code to stop the smart test task
/// @param[in] pu8SC_Id \b pointer to the smart card id
/// @return TRUE - Success
/// @return FALSE - fail
/// @sa
/// @note
/// Command: \b SMC_Stop \n
///
/// Sample Command:  \b SMC_Stop 0 \n
///                  \b SMC_Stop 1 \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SMC_Stop(MS_U8 *pu8SC_Id)
{
    printf("appDemo_SMC_Stop\n");
    // To stop the while loop of the test task
    bRunning = FALSE;
    MsOS_SetEvent(s32EventId, APP_SMC_EVT_STOP);

    // Wait for test task to stop
    MsOS_DelayTask(500);

    // Delete SMC demo task
    MsOS_DeleteEventGroup(s32EventId);
    s32EventId = -1;

    // Delete SMC event group
    MsOS_DeleteTask(s32TaskId);
    s32TaskId = -1;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The demo code to show usage of the smart card command
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - fail
/// @sa
/// @note
/// Command: \b HelpSMC \n
///
/// Sample Command:  \b HelpSMC \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_SMC_Help(void)
{
    printf("---------------------------------- Smart Card Help ------------------------------------\n");
    printf("1. SMC_Start\n");
    printf("Usage: smc_start sc_id\n");
    printf("Sample UART Command: Start the task to test the smart card\n");
    printf("\n");
    printf("2. SMC_Stop\n");
    printf("Usage: smc_stop sc_id\n");
    printf("Sample UART Command: Stop the smart card test task\n");
    printf("\n");
    printf("------------------------------- End of Smart Card Help --------------------------------\n");

    return TRUE;
}
