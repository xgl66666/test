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
#if (DEMO_SMC_TEST == 1)

#include "MsCommon.h"
#include "demo_smc.h"
#include "apiSC.h"
#include "drvSC.h"
#include "string.h"
#include "drvGPIO.h"

#if ((DEMO_SMC1_ONCHIP_8024_TEST == 1) && (DEMO_SMC_PWR_CUT_DET_TEST == 1))
#include "drvSAR.h"
#endif

#if (DEMO_SMC_USE_51_TEST == 1)
#include "MsMemory.h"
#include "drvPM.h"
#include "drvMBX.h"
#endif

//For GPIO of exteranl 8024 voltage ctrl
#include "Board.h"
#define GPIO_EXT_8024_VOL_CTRL_UDEF 9999
#ifndef GPIO_SMC0_EXT_8024_VOL_CTRL
#define GPIO_SMC0_EXT_8024_VOL_CTRL GPIO_EXT_8024_VOL_CTRL_UDEF
#endif
#ifndef GPIO_SMC1_EXT_8024_VOL_CTRL
#define GPIO_SMC1_EXT_8024_VOL_CTRL GPIO_EXT_8024_VOL_CTRL_UDEF
#endif

#if (DEMO_SMC_DUAL_TEST == 1)
#define SMC_DEV_NUM 2
#else
#define SMC_DEV_NUM 1
#endif

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define APPSMC_TASK_STACK_SIZE      4096

#define APP_SMC_EVT_MASK1           0x0000000F
#define APP_SMC_EVT_MASK2           0x000000F0
#define APP_SMC_EVT_IN1             0x00000001
#define APP_SMC_EVT_OUT1            0x00000002
#define APP_SMC_EVT_IN2             0x00000010
#define APP_SMC_EVT_OUT2            0x00000020
#define APP_SMC_EVT_STOP            0x00000100


typedef enum
{
    E_SMC_BOARD_WITHOUT_8024,
    E_SMC_BOARD_WITH_8024,
    E_SMC_BOARD_MAX
}EN_SMC_BOARD_TYPE;

typedef enum
{
    E_SMC_CLK_DEF_BY_CARD,
    E_SMC_CLK_3M,
    E_SMC_CLK_4P5M,
    E_SMC_CLK_6M,
    E_SMC_CLK_13M,
    E_SMC_CLK_MAX
}EN_SMC_CLK_TYPE;

typedef enum
{
    E_SMC_VCC_3P3V,
    E_SMC_VCC_5V,
    E_SMC_VCC_MAX
}EN_SMC_VCC_VOLTAGE;

typedef enum
{
    E_SMC_HEALTH_INSURANCE_CARD,
    E_SMC_CONAX_T0_CARD,
    E_SMC_IRDETO_T0_CARD,
    E_SMC_IRDETO_T14_CARD,
    E_SMC_MAX_CAT_CARD
}EN_SMC_CARD_CAT;

static const char *gaSmcCardCatStr[E_SMC_MAX_CAT_CARD] = {
    "HEALTH INSURANCE T=1 CARD",
    "CONAX T=0 CARD",
    "IRDETO T=0 CARD",
    "IRDETO T=14 CARD"
};

static const char *gaSmcVoltageStr[E_SMC_VCC_MAX] = {
    "3.3V",
    "5V"
};

static const char *gaSmcClkTypeStr[E_SMC_CLK_MAX] = {
    "Defined by card_cat",
    "3MHz",
    "4.5MHz",
    "6MHz",
    "13MHz"
};


typedef struct
{
    MS_U32 u32GpioExt8024VolCtrl;
    MS_U8 u8Protocol;
    EN_SMC_CARD_CAT eCardCat;
    EN_SMC_VCC_VOLTAGE eVccVoltage;
    EN_SMC_CLK_TYPE eClkType;
    void (*pfInitParam)(SC_Param *pParam);
    void (*pfCmdTest)(MS_U32 u8SCID);
}ST_SMC_PROTOCOL_SETTING;

typedef struct
{
    MS_U8 u8SCID;
    MS_U8 u8CardCat;
    MS_U8 u8VccClass;
    MS_U8 u8ClkType;
}ST_SMC_INIT_PRO_SETTING;

typedef struct
{
    ST_SMC_PROTOCOL_SETTING gstSmcProSetting;
    MS_U8    *pu8TaskStack;
    MS_S32   s32TaskId, s32EventId;
    MS_U32   u32Events;
    MS_BOOL  bRunning;
    MS_U8  _u8AtrBuf[SC_ATR_LEN_MAX];
    MS_U16 _u16AtrLen;
    MS_BOOL  bUseInternal8024;
}ST_SMC_CTX;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static __attribute__((aligned(0x1000))) MS_U8 gau8TaskStack1[APPSMC_TASK_STACK_SIZE];
#if (DEMO_SMC_DUAL_TEST == 1)
static __attribute__((aligned(0x1000))) MS_U8 gau8TaskStack2[APPSMC_TASK_STACK_SIZE];
#endif


static ST_SMC_CTX agstSmcCtx[SMC_DEV_NUM] = {{.bRunning = FALSE,
#if (DEMO_SMC1_ONCHIP_8024_TEST == 1)
                                    .bUseInternal8024 = TRUE,
#else
                                    .bUseInternal8024 = FALSE,
#endif
                                    ._u16AtrLen = 0,
                                    .pu8TaskStack = gau8TaskStack1},

#if (DEMO_SMC_DUAL_TEST == 1)
                                   {.bRunning = FALSE,

#if (DEMO_SMC2_ONCHIP_8024_TEST == 1)
                                    .bUseInternal8024 = TRUE,
#else
                                    .bUseInternal8024 = FALSE,
#endif
                                    ._u16AtrLen = 0,
                                    .pu8TaskStack = gau8TaskStack2}
#endif
                                  };

#if (DEMO_SMC_USE_51_TEST == 1)
MS_U8 u8SCFirmware[] = {
    #include "fwSC.dat"
};
#endif
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Dump the message to the console
/// @param pu8Data \b the pointer to the message
/// @param u16DataLen \b the length of the message
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _Demo_SMC_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen)
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
    pParam->u8Convention = 0;
}

void _Irdeto_T14_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_6M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_1 | SC_UART_PARITY_NO;
    pParam->u16ClkDiv = 620;
    pParam->u8Convention = 0;
}

#else
void _TEST_T1_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_4P5M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
    pParam->u16ClkDiv = 372;
}

void _VIA_T0_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_4P5M;
    pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
    pParam->u16ClkDiv = 372;

    printf("// ATR : 3f 77 18 00 00 c2 47 40 00 68 90 00\n");
}

void _IRD_T14_InitParam(SC_Param *pParam)
{
    pParam->eCardClk = E_SC_CLK_6M;
    pParam->u8UartMode = SC_UART_CHAR_8;
    pParam->u16ClkDiv = 620;
}
#endif

#if 1
static void _HealthInsurance_T1_CmdTest(MS_U32 u8SCID)
{   //Health Insurance card
    MS_U8 SelectAPDU[] = { 0x21, 0x00, 0x15,0x00, 0xA4, 0x04, 0x00, 0x10, 0xD1, 0x58, 0x00, 0x00, 0x01, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00 ,0x1D};
    MS_U8 ReadProfileAPDU[] = {0x21, 0x40, 0x07, 0x00, 0xca, 0x11, 0x00, 0x02, 0x00, 0x00,0xbf };
    MS_U16 _uCmdLen ;
    MS_U8 pu8ReadData[255];
    MS_U16 u16ReadDataLen = 255;

    _uCmdLen = sizeof(SelectAPDU);
    memset(pu8ReadData, 0, sizeof (pu8ReadData));
    MApi_SC_T1_Parse_Atr(u8SCID, agstSmcCtx[u8SCID]._u8AtrBuf, agstSmcCtx[u8SCID]._u16AtrLen);

    if(MApi_SC_T1_Communication(u8SCID, SelectAPDU, &_uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
    {
        printf("MDrv_SC_T1SendRecv1  Error\n");
    }
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);

    u16ReadDataLen = 255;
    _uCmdLen = sizeof(ReadProfileAPDU);
    if(MApi_SC_T1_Communication(u8SCID, ReadProfileAPDU,& _uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
    {
        printf("MDrv_SC_T1SendRecv  Error\n");
    }
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);
}

static void _Conax_T0_CmdTest(MS_U32 u8SCID)
{   //conax T=0 card
    MS_U8 au8SendData[] = {0xDD, 0x26, 0x00, 0x00, 0x03, 0x10, 0x01, 0x03}; //Instruction 26
    MS_U8 au8ReadIns[] = {0xDD, 0xCA, 0x00, 0x00, 0x11};
    MS_U16 _uCmdLen ;
    MS_U8 pu8ReadData[255];
    MS_U16 u16ReadDataLen = 255;

    _uCmdLen = sizeof(au8SendData);
    memset(pu8ReadData, 0, sizeof (pu8ReadData));

    if(MApi_SC_T0_Communication(u8SCID, au8SendData, &_uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
    {
        printf("MApi_SC_T0_Communication  Error\n");
    }
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);

    if (pu8ReadData[0] == 0x98)
    {
        u16ReadDataLen = 255;
        au8ReadIns[4] = pu8ReadData[1];
        _uCmdLen = sizeof(au8ReadIns);
        if(MApi_SC_T0_Communication(u8SCID, au8ReadIns,& _uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
        {
            printf("MApi_SC_T0_Communication  Error\n");
        }

    }
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);
}

static void _Irdeto_T0_CmdTest(MS_U32 u8SCID)
{   //Irdeto T=0 card
    MS_U8 au8SendData[] = {0xD2, 0x04, 0x03, 0x00, 0x01, 0x3D};
    MS_U8 au8ReadIns[] = {0xD2, 0xFE, 0x03, 0x00, 0x00};
    MS_U16 _uCmdLen ;
    MS_U8 pu8ReadData[255];
    MS_U16 u16ReadDataLen = 255;

    _uCmdLen = sizeof(au8SendData);
    memset(pu8ReadData, 0, sizeof (pu8ReadData));

    if(MApi_SC_T0_Communication(u8SCID, au8SendData, &_uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
    {
        printf("MApi_SC_T0_Communication  Error\n");
    }
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);

    if (pu8ReadData[0] == 0x90)
    {
        u16ReadDataLen = 255;
        au8ReadIns[4] = pu8ReadData[1];
        _uCmdLen = sizeof(au8ReadIns);
        if(MApi_SC_T0_Communication(u8SCID, au8ReadIns,& _uCmdLen, pu8ReadData, &u16ReadDataLen) != E_API_SC_OK)
        {
            printf("MApi_SC_T0_Communication  Error\n");
        }

    }
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);
}

static void _Irdeto_T14_CmdTest(MS_U32 u8SCID)
{   //Irdeto T=14 card
    MS_U8 au8CommandData[] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x3c};
    MS_U16 _uCmdLen;
    MS_U8 au8ReadData[255];
    MS_U16 u16ReadDataLen = 255;
    SC_Result eResult;

    _uCmdLen = sizeof(au8CommandData);
    memset(au8ReadData, 0, sizeof (au8ReadData));

    eResult = MDrv_SC_Send(u8SCID, au8CommandData, _uCmdLen,100);
    if (eResult != E_SC_OK)
    {
        printf("T14 Send command failed\n");
        return;
    }
    u16ReadDataLen = 8;
    eResult = MDrv_SC_Recv(u8SCID, au8ReadData, &u16ReadDataLen, 400);
    if (eResult != E_SC_OK)
    {
        printf("T14 rcv command failed\n");
        return;
    }

    u16ReadDataLen = au8ReadData[7]+1;
    eResult = MDrv_SC_Recv(u8SCID, &au8ReadData[8], &u16ReadDataLen, 400);
    if (eResult != E_SC_OK)
    {
        printf("T14 rcv command failed\n");
        return;
    }
    u16ReadDataLen += 8;
    if (u16ReadDataLen)
    {
        _Demo_SMC_PrintData(au8ReadData, u16ReadDataLen);
    }
}

#else
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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x90 && _u8ReadData[1] == 0x0b))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }
}



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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[45] == 0x90 && _u8ReadData[46] == 0x00))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }
}


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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[0] == 0x90 && _u8ReadData[1] == 0x02))
            {
                printf("1st cmd Fail!\n");
            }
        }
    }
}


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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

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
            _Demo_SMC_PrintData(_u8ReadData, _u16ReadLen);

            if (!(_u8ReadData[31] == 0x00 && _u8ReadData[32] == 0xe9))
            {
                printf("Test Fail %d\n", __LINE__);
            }
        }
    }
}

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
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);

    u16SendDataLen = sizeof(u8SendData2);
    u16ReadDataLen = 255;

    printf("\nSending ... 0x00, 0x00, 0x14, 0x00, 0xA4, 0x04, 0x00, 0x0E,\n");
    printf("            0x31, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53,\n");
    printf("            0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00, 0xDD \n");
    MDrv_SC_RawExchange(u8SCID, u8SendData2, &u16SendDataLen, pu8ReadData, &u16ReadDataLen);
    _Demo_SMC_PrintData(pu8ReadData, u16ReadDataLen);
}
#endif

static void _Demo_SMC_SetCardClk(SC_Param *pParam, EN_SMC_CLK_TYPE eClkType)
{
    if (pParam != NULL)
    {
        switch(eClkType)
        {
            default: //Defined by card
            case 0:
               //Do nothing
               break;

            case 1: //3M
               pParam->eCardClk = E_SC_CLK_3M;
               break;

            case 2: //4.5M
               pParam->eCardClk = E_SC_CLK_4P5M;
               break;

            case 3: //6M
               pParam->eCardClk = E_SC_CLK_6M;
               break;

            case 4: //13M
               pParam->eCardClk = E_SC_CLK_13M;
               break;
        }
    }
}

static MS_BOOL _Demo_SMC_InitProSetting(ST_SMC_PROTOCOL_SETTING *pstSetting, ST_SMC_INIT_PRO_SETTING *pstSmcInitProSetting)
{
    MS_BOOL bRetVal = TRUE;
    
    // Card category
    switch(pstSmcInitProSetting->u8CardCat)
    {
        case 0: //Health Insurance card
            pstSetting->u8Protocol = 1;
            pstSetting->eCardCat = E_SMC_HEALTH_INSURANCE_CARD;
            pstSetting->pfInitParam = _Default_InitParam;
            pstSetting->pfCmdTest = _HealthInsurance_T1_CmdTest;
            break;

        case 1: //CONAX T=0 card
            pstSetting->u8Protocol = 0;
            pstSetting->eCardCat = E_SMC_CONAX_T0_CARD;
            pstSetting->pfInitParam = _Default_InitParam;
            pstSetting->pfCmdTest = _Conax_T0_CmdTest;
            break;

        case 2: //IRDETO T=0 card
            pstSetting->u8Protocol = 0;
            pstSetting->eCardCat = E_SMC_IRDETO_T0_CARD;
            pstSetting->pfInitParam = _Default_InitParam;
            pstSetting->pfCmdTest = _Irdeto_T0_CmdTest;
            break;

        case 3: //IRDETO T=14 card
            pstSetting->u8Protocol = 14;
            pstSetting->eCardCat = E_SMC_IRDETO_T14_CARD;
            pstSetting->pfInitParam = _Irdeto_T14_InitParam;
            pstSetting->pfCmdTest = _Irdeto_T14_CmdTest;
            break;
        
        default:
            printf("ERROR!! Wrong Card Category value %d\n", pstSmcInitProSetting->u8CardCat);
            bRetVal = FALSE;
            break;
    }

    // Vcc class
    switch(pstSmcInitProSetting->u8VccClass)
    {
        case 0: //3.3V
           pstSetting->eVccVoltage = E_SMC_VCC_3P3V;
           break;

        case 1: //5V
           pstSetting->eVccVoltage = E_SMC_VCC_5V;
           break;

        default:
            printf("ERROR!! Wrong VCC Class value %d\n", pstSmcInitProSetting->u8VccClass);
            bRetVal = FALSE;
            break;
    }

    // Clk type
    switch(pstSmcInitProSetting->u8ClkType)
    {
        case 0: //Defined by card
           pstSetting->eClkType = E_SMC_CLK_DEF_BY_CARD;
           break;

        case 1: //3M
           pstSetting->eClkType = E_SMC_CLK_3M;
           break;

        case 2: //4.5M
           pstSetting->eClkType = E_SMC_CLK_4P5M;
           break;

        case 3: //6M
           pstSetting->eClkType = E_SMC_CLK_6M;
           break;

        case 4: //13M
           pstSetting->eClkType = E_SMC_CLK_13M;
           break;

        default:
            printf("ERROR!! Wrong Clk Type value %d\n", pstSmcInitProSetting->u8ClkType);
            bRetVal = FALSE;
            break;
    }

    // Setup GPIO of external 8024 voltage ctrl
    if (agstSmcCtx[pstSmcInitProSetting->u8SCID].bUseInternal8024 == FALSE)
    {
        switch(pstSmcInitProSetting->u8SCID)
        {
            case 0: //SMC0
                if (GPIO_SMC0_EXT_8024_VOL_CTRL == GPIO_EXT_8024_VOL_CTRL_UDEF)
                {
                    printf("ERROR!! GPIO_SMC0_EXT_8024_VOL_CTRL is not defined at board header\n");
                    bRetVal = FALSE;
                }
                pstSetting->u32GpioExt8024VolCtrl = GPIO_SMC0_EXT_8024_VOL_CTRL;
                break;

            case 1: //SMC1
                if (GPIO_SMC1_EXT_8024_VOL_CTRL == GPIO_EXT_8024_VOL_CTRL_UDEF)
                {
                    printf("ERROR!! GPIO_SMC1_EXT_8024_VOL_CTRL is not defined at board header\n");
                    bRetVal = FALSE;
                }
                pstSetting->u32GpioExt8024VolCtrl = GPIO_SMC1_EXT_8024_VOL_CTRL;
                break;

            default:
                printf("ERROR!! Wrong SC ID value %d\n", pstSmcInitProSetting->u8SCID);
                bRetVal = FALSE;
                break;
        }
    }


    if (bRetVal)
    {
        printf("--------------------\n");
        printf("Card_Cat   : %s\n", (char*)gaSmcCardCatStr[pstSetting->eCardCat]);
        printf("Vcc_Class  : %s\n", (char*)gaSmcVoltageStr[pstSetting->eVccVoltage]);
        printf("Clk_Type   : %s\n", (char*)gaSmcClkTypeStr[pstSetting->eClkType]);
        printf("--------------------\n");
    }

    return bRetVal;
}


//------------------------------------------------------------------------------
/// @brief The demo code to reset the smart card and dump ATR message
/// @param u8SCID \b the smart card id
/// @param eEvent \b the event of the smart card
/// @sa
/// @note
//------------------------------------------------------------------------------
static void _Demo_SMC_Notify(MS_U8 u8SCID, SC_Event eEvent)
{
    MS_U32 u32Mask, u32Evt;

    if (u8SCID > (SMC_DEV_NUM-1))
    {
        printf("[%s] ERROR!! Incorrect SMC id\n", __FUNCTION__);
        return;
    }

    switch (eEvent)
    {
        case E_SC_EVENT_IN:
            printf("SMC%d: Card In\n", u8SCID);
            u32Mask = (0 == u8SCID) ? APP_SMC_EVT_MASK1 : APP_SMC_EVT_MASK2;
            u32Evt = (0 == u8SCID) ? APP_SMC_EVT_IN1 : APP_SMC_EVT_IN2;
            MsOS_ClearEvent(agstSmcCtx[u8SCID].s32EventId, u32Mask);
            agstSmcCtx[u8SCID].u32Events &= ~u32Mask;
            MsOS_SetEvent(agstSmcCtx[u8SCID].s32EventId, u32Evt);
            break;
        case E_SC_EVENT_OUT:
            printf("SMC%d: Card Out\n", u8SCID);
            u32Mask = (0 == u8SCID) ? APP_SMC_EVT_MASK1 : APP_SMC_EVT_MASK2;
            u32Evt = (0 == u8SCID) ? APP_SMC_EVT_OUT1 : APP_SMC_EVT_OUT2;
            MsOS_ClearEvent(agstSmcCtx[u8SCID].s32EventId, u32Mask);
            agstSmcCtx[u8SCID].u32Events &= ~u32Mask;
            MsOS_SetEvent(agstSmcCtx[u8SCID].s32EventId, u32Evt);
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

static MS_BOOL _Demo_SMC_Reset2(MS_U8 u8SCID,SC_Param *pParam)
{
    MS_U8               history[SC_HIST_LEN_MAX];
    MS_U16              history_len = 200;
    MS_U8               u8Protocol = 0xff;
    SC_Result  _scresult = E_SC_FAIL;
    ST_SMC_PROTOCOL_SETTING *pstSetting = &agstSmcCtx[u8SCID].gstSmcProSetting;

    // reset
    if (MDrv_SC_Reset(u8SCID, pParam) != E_SC_OK)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    MDrv_SC_ClearState(u8SCID);

    // Timeout value can be tuned
    _scresult = MDrv_SC_GetATR(u8SCID, 100*5, agstSmcCtx[u8SCID]._u8AtrBuf, &agstSmcCtx[u8SCID]._u16AtrLen, history, &history_len);
    if (_scresult != E_SC_OK) // the timeout value should be tumed by user
    {
        printf("[%s][%d] _scresult %d \n",__FUNCTION__,__LINE__,_scresult);
        if (u8SCID > 0)
        {
            printf("[%s][%d] Please check SMC pad mux is set or not!!\n",__FUNCTION__,__LINE__);
        }
        return FALSE;
    }

    // Setup UART mode
    if(agstSmcCtx[u8SCID]._u8AtrBuf[0] == 0x3b)
    {
        if(pstSetting->eCardCat == E_SMC_IRDETO_T14_CARD)
        {
            pParam->u8UartMode = (SC_UART_CHAR_8 | SC_UART_STOP_1 | SC_UART_PARITY_NO); //Irdeto T=14
        }
        else
        {
            pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_EVEN;
        }
    }
    else if(agstSmcCtx[u8SCID]._u8AtrBuf[0] == 0x3f)
    {
        if(pstSetting->eCardCat == E_SMC_IRDETO_T14_CARD)
        {
            pParam->u8UartMode = (SC_UART_CHAR_8 | SC_UART_STOP_1 | SC_UART_PARITY_NO);
        }
        else
        {
            pParam->u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
        }
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

    if (agstSmcCtx[u8SCID]._u8AtrBuf[1] & 0x80)
    {
        MS_U8 u8T0 = agstSmcCtx[u8SCID]._u8AtrBuf[1] >> 4;
        i = 1;
        while (u8T0)
        {
            if (u8T0 & 1)
                i++;
            u8T0 >>= 1;
        }
        u8Protocol = agstSmcCtx[u8SCID]._u8AtrBuf[i] & 0xF;
    }
    else
    {
        u8Protocol = 0;
    }

    // if you want to change the PPS to specific Fi and Di, use the MDrv_SC_SetPPS
    if (pstSetting->eCardCat != E_SMC_IRDETO_T14_CARD)
    {
        if (MDrv_SC_PPS(u8SCID) != E_SC_OK)
        {
            printf("[%s][%d] \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
    }


    printf("\n ***Smartcard%d  T=%d  ***\n", u8SCID, u8Protocol);
    {
        printf("SMC%d: ATR Message :     \n", u8SCID);
        printf("Rx :\n         ");
        for (i = 0; i < agstSmcCtx[u8SCID]._u16AtrLen; i++)
        {
            printf("%02x ", agstSmcCtx[u8SCID]._u8AtrBuf[i]);
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
static void _Demo_SMC_Task(MS_U8 u8SCID)
{
    SC_Param            sInitParams;
    SC_Result           errCode;

    printf("##########    START TO TEST SMARTCARD[%d]   ##########\n", u8SCID);

#if ((DEMO_SMC1_ONCHIP_8024_TEST == 1) && (DEMO_SMC_PWR_CUT_DET_TEST == 1))
    MS_U8 u8SarSmcIntChID = 3; //SAR3 (CH4)
    SAR_SmcIntLevel eSmcIntLevel = E_SAR_SMC_INT_HIGH_ACT; //High active
    MS_U32 u32BndVal = 0xF800; //0.8V
    printf("##########    Enable SMC power cut detection   ##########\n");
    MDrv_SAR_Smc_PwrDetCfg(u8SarSmcIntChID, eSmcIntLevel, u32BndVal);
#endif

    // Init the smart card driver
    if ((errCode = MDrv_SC_Init(u8SCID)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Init failed with error code=%d !!!!\n", __FUNCTION__, errCode);
        return;
    }

    // UART setting
    agstSmcCtx[u8SCID].gstSmcProSetting.pfInitParam(&sInitParams);
    _Demo_SMC_SetCardClk(&sInitParams, agstSmcCtx[u8SCID].gstSmcProSetting.eClkType);
    if (agstSmcCtx[u8SCID].bUseInternal8024)
    {
        if (agstSmcCtx[u8SCID].gstSmcProSetting.eVccVoltage == E_SMC_VCC_3P3V)
            sInitParams.eVoltage = E_SC_VOLTAGE_3_POINT_3V;
        else
            sInitParams.eVoltage = E_SC_VOLTAGE_5V;
        sInitParams.eVccCtrl     = E_SC_VCC_VCC_ONCHIP_8024;
    }
    else
    {
        if (agstSmcCtx[u8SCID].gstSmcProSetting.eVccVoltage == E_SMC_VCC_5V)
            mdrv_gpio_set_high(agstSmcCtx[u8SCID].gstSmcProSetting.u32GpioExt8024VolCtrl);
        else
            mdrv_gpio_set_low(agstSmcCtx[u8SCID].gstSmcProSetting.u32GpioExt8024VolCtrl);
        sInitParams.eVccCtrl     = E_SC_VCC_CTRL_8024_ON;
    }
    sInitParams.pfOCPControl = NULL;
    if ((errCode = MDrv_SC_Open(u8SCID, agstSmcCtx[u8SCID].gstSmcProSetting.u8Protocol, &sInitParams, _Demo_SMC_Notify)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Open failed with error code = %d!!!!\n", __FUNCTION__, errCode);
        goto EXIT;
    }

    // Test smart card
    while (agstSmcCtx[u8SCID].bRunning)
    {
        SC_Status status;
        printf("to wait event\n");
        // Wait for the smart card in/out event or task stop event
        MsOS_WaitEvent(agstSmcCtx[u8SCID].s32EventId, APP_SMC_EVT_IN1 | APP_SMC_EVT_OUT1 |
                APP_SMC_EVT_IN2 | APP_SMC_EVT_OUT2 | APP_SMC_EVT_STOP,
                &agstSmcCtx[u8SCID].u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        // Check smc stop event
        if (agstSmcCtx[u8SCID].u32Events & APP_SMC_EVT_STOP)
        {
            printf(" ######## Stop _Demo_SMC_Task sm[%d] ########\n", u8SCID);
            break;
        }

        // Get the smart card status
        MDrv_SC_GetStatus(u8SCID, &status);
        //printf("status get %x\n",status);
        if (status.bCardIn)
        {
            printf(" ######## Insert Test sm[%d] OK ######## ==> [COMMAND]\n", u8SCID);

            // Test the smart card reset
            if (_Demo_SMC_Reset2(u8SCID,&sInitParams))
            {
                // Test the smart card command
                agstSmcCtx[u8SCID].gstSmcProSetting.pfCmdTest(u8SCID);
            }
            printf(" ########  CMD Test sm[%d] End  ######## ==> [REMOVE]\n", u8SCID);
        }
        else
        {
            printf(" ######## Remove Test sm[%d] OK ######## ==> [INSERT]\n", u8SCID);
        }
    }

    // Deactive the smart card
    if ((errCode = MDrv_SC_Deactivate(u8SCID)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Deactivate failed with error code =%d\n", __FUNCTION__, errCode);
    }

    // Close the smart card
    if ((errCode = MDrv_SC_Close(u8SCID)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Close failed with error code =%d\n", __FUNCTION__, errCode);
    }

EXIT:
    // Exit the smart card driver
    if ((errCode = MDrv_SC_Exit(u8SCID)) != E_SC_OK)
    {
        printf("[%s] MDrv_SC_Exit failed with error code=%d !!!!\n", __FUNCTION__, errCode);
        return;
    }

    return ;
}

#if (DEMO_SMC_USE_51_TEST == 1)
MS_BOOL _Demo_SMC_Load_Fw(MS_U32 u32FwSCAddr)
{
    MBX_Result mbxResult;
    SC_BuffAddr ScBuffAddr;

    ScBuffAddr.u32DataBuffAddr = PM51_MEM_ADR;
    ScBuffAddr.u32FwBuffAddr   = PM51_LITE_ADR;

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

MS_BOOL _Demo_SMC_Disable_Fw(void)
{
    MBX_Result eMbxResult;

    // disable PM51
    MDrv_PM_Disable51();

    // mbx
    eMbxResult = MDrv_MBX_UnRegisterMSG(E_MBX_CLASS_PM_WAIT,0);
    if (eMbxResult != E_MBX_SUCCESS)
    {
        printf("MDrv_MBX_UnRegisterMSG(E_MBX_CLASS_PM_NOWAIT,0) FAIL %x\n", eMbxResult);
        return FALSE;
    }

    eMbxResult = MDrv_MBX_DeInit(TRUE);
    if (eMbxResult != E_MBX_SUCCESS)
    {
        printf("MDrv_MBX_DeInit FAIL %x\n", eMbxResult);
        return FALSE;
    }

    return TRUE;
}
#endif
//------------------------------------------------------------------------------
/// @brief The demo code to start the smart test task
/// @param[in] pu8SC_Id \b pointer to the smart card id
/// @return TRUE - Success
/// @return FALSE - fail
/// @sa
/// @note
/// Command: \b SMC_Start [sc_id] [card_cat] [vcc_class] \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SMC_Start(MS_U8 *pu8SC_Id, MS_U8 *pu8CardCat, MS_U8 *pu8VccClass, MS_U8 *pu8ClkType)
{
    ST_SMC_INIT_PRO_SETTING stSmcInitProSetting;

    printf("appDemo_SMC_Start\n");

    // Check dev num
    if (*pu8SC_Id > (SMC_DEV_NUM-1))
    {
        printf("ERROR!! Incorrect SMC id, SMC dev is only %d\n", SMC_DEV_NUM);
        return FALSE;
    }

    // Check APP status
    if(agstSmcCtx[*pu8SC_Id].bRunning)
    {
        printf("appDemo_SMC has been started!!\n");
        return FALSE;
    }

    // Init protocol setting
    stSmcInitProSetting.u8SCID = *pu8SC_Id;
    stSmcInitProSetting.u8CardCat = *pu8CardCat;
    stSmcInitProSetting.u8VccClass = *pu8VccClass;
    stSmcInitProSetting.u8ClkType = *pu8ClkType;
    if (_Demo_SMC_InitProSetting(&agstSmcCtx[*pu8SC_Id].gstSmcProSetting, &stSmcInitProSetting) == FALSE)
    {
        return FALSE;
    }

    // Change status flag    
    agstSmcCtx[*pu8SC_Id].bRunning = TRUE;

#if (DEMO_SMC_USE_51_TEST == 1)
    _Demo_SMC_Load_Fw(PM51_LITE_ADR);
#endif

    // Create SMC event group
    agstSmcCtx[*pu8SC_Id].s32EventId = MsOS_CreateEventGroup("appSMC_Event");
    // Create SMC demo task
    agstSmcCtx[*pu8SC_Id].s32TaskId = MsOS_CreateTask( (TaskEntry) _Demo_SMC_Task,
                                  (MS_U32)*pu8SC_Id,
                                  E_TASK_PRI_HIGH,
                                  TRUE,
                                  agstSmcCtx[*pu8SC_Id].pu8TaskStack,
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
/// Command: \b SMC_Stop [sc_id] \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SMC_Stop(MS_U8 *pu8SC_Id)
{
    printf("appDemo_SMC_Stop\n");

    // Check dev num
    if (*pu8SC_Id > (SMC_DEV_NUM-1))
    {
        printf("ERROR!! Incorrect SMC id, SMC dev is only %d\n", SMC_DEV_NUM);
        return FALSE;
    }

    // Check APP status
    if(!agstSmcCtx[*pu8SC_Id].bRunning)
    {
        printf("appDemo_SMC has been stopped!!\n");
        return FALSE;
    }
    
    // To stop the while loop of the test task
    agstSmcCtx[*pu8SC_Id].bRunning = FALSE;
    MsOS_SetEvent(agstSmcCtx[*pu8SC_Id].s32EventId, APP_SMC_EVT_STOP);

    // Wait for test task to stop
    MsOS_DelayTask(500);

    // Delete SMC demo task
    MsOS_DeleteEventGroup(agstSmcCtx[*pu8SC_Id].s32EventId);
    agstSmcCtx[*pu8SC_Id].s32EventId = -1;

    // Delete SMC event group
    MsOS_DeleteTask(agstSmcCtx[*pu8SC_Id].s32TaskId);
    agstSmcCtx[*pu8SC_Id].s32TaskId = -1;

#if (DEMO_SMC_USE_51_TEST == 1)
    _Demo_SMC_Disable_Fw();
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The demo code to set debug level
/// @param[in] pu8DbgLevel \b debug level
/// @return TRUE - Success
/// @return FALSE - fail
/// @sa
/// @note
/// Command: \b SMC_Help \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SMC_SetDbgLevel(MS_U8 *pu8DbgLevel)
{
    SC_DbgLv eLevel;

    switch(*pu8DbgLevel)
    {
        default:
        case 0:
            eLevel = E_SC_DBGLV_ERR_ONLY;
            break;

        case 1:
            eLevel = E_SC_DBGLV_INFO;
            break;

        case 2:
            eLevel = E_SC_DBGLV_ALL;
            break;

        case 3:
            eLevel = E_SC_DBGLV_NONE;
            break;
    }

    MDrv_SC_SetDbgLevel(eLevel);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The demo code to show usage of the smart card command
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - fail
/// @sa
/// @note
/// Command: \b SMC_Help \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_SMC_Help(void)
{
    printf("---------------------------------- Smart Card Help ------------------------------------\n");
    printf("1. Start the task to test the smart card\n");
    printf("   Command: SMC_Start [sc_id] [card_cat] [vcc_class] [clk_type]\n");
    printf("            sc_id     : 0[smc_id 0]\n");
    printf("            card_cat  : 0[Health Insurance card], 1[CONAX T=0], 2[IRDETO T=0], 3[IRDETO T=14]\n");
    printf("            vcc_class : 0[3.3V], 1[5V]\n");
    printf("            clk_type  : 0[Defined by card_cat], 1[3MHz], 2[4.5MHz], 3[6MHz], 4[13MHz]\n");
    printf(" \n");
    printf("2. Stop the smart card test task\n");
    printf("   Command: SMC_Stop [sc_id]\n");
    printf("            sc_id     : 0[smc_id 0]\n");
    printf(" \n");
    printf("3. Set debug level of message\n");
    printf("   Command: SMC_SetDBGLevel [dbg_level]\n");
    printf("            dbg_level : 0[ERR_ONLY], 1[INFO], 2[ALL], 3[NONE]\n");
    printf(" \n");
    printf("4. List all command usage\n");
    printf("   Command  : SMC_Help\n");
    printf("\n");
    printf("------------------------------- End of Smart Card Help --------------------------------\n");

    return TRUE;
}

#endif
