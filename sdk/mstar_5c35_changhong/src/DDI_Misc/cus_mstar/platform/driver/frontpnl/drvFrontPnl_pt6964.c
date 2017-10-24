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
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvFrontPnl.c
/// @brief  FrontPanel Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DDI_MISC_INUSE


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvFrontPnl.h"
#include "drvKeypad.h"

#if ((FRNTPANL_TYPE_SEL == FRNTPANL_TYPE_PT6964) ||(FRNTPANL_TYPE_SEL == FRNTPANL_TYPE_FD620))

#if(FRNTPANL_TYPE_SEL == FRNTPANL_TYPE_PT6964)
#include "FP_Mstar.h"
#elif(FRNTPANL_TYPE_SEL == FRNTPANL_TYPE_FD620)
#include "drvFrontPnl_FD620.h"
#endif

extern MS_U8 MDrv_PT6964_FP_Get_Key(void);
extern void MDrv_PT6964_Write_Adr_Data(MS_U8 addr, MS_U8 value);
extern void MDrv_PT6964_Init(void);
extern void MDrv_PT6964_Display_On(void);
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LED(msg)                    msg
//#define I2CIOEXPANDERID                 0x30
//#define FRONTPNL_TASK_STACK             4096


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define FRONTPNL_START_TIME_MS      3//((1000 / 50) / LED_NUM)
#define FRONTPNL_PERIOD_TIME_MS     150
#define FP_LED_MAX_NUM 5
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//static MS_U8 u8DisplayData[FP_LED_MAX_NUM];

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//static EN_7SEG_WORD _ShowWord[LED_NUM];
//static MS_S32       _s32FrontPnlTimerTaskID         = -1;
//static MS_U8        _u8CurLightNum                  = LED_NUM;

//----------------------------------------------------
// Function Name: Mdrv_6961_LED_DISP
// Description:
//        Update PT6961/PT6964/ET6201 LED display data RAM.
// Parameters:
//        u8_LedBit, LED number
//        u8_LowByte, LowByte data in RAM     LSB~MSB : SEG1~ SEG8
//        u8_HighByte, High Byte data in RAM    LSB~MSB : SEG9 ~ SEG12 + xxxx
//             LED bit sequence Can be address by the following Macro:
//        LED_WORD1_ADDR /  LED_WORD2_ADDR /LED_WORD3_ADDR / LED_WORD4_ADDR
//------------------------------------------------------------------------

void MDrv_6961_LED_DISP(MS_U8 u8_Ledbit,MS_U8 u8_LowByte,MS_U8 u8_HighByte)
{
    switch(u8_Ledbit)
    {
        case 0:

            MDrv_PT6964_Write_Adr_Data(LED_WORD2_ADDR, u8_LowByte);
            MDrv_PT6964_Write_Adr_Data(LED_WORD2_ADDR+1,u8_HighByte);
            break;

        case 1:

            MDrv_PT6964_Write_Adr_Data(LED_WORD1_ADDR, u8_LowByte);
            MDrv_PT6964_Write_Adr_Data(LED_WORD1_ADDR+1, u8_HighByte);
            break;

        case 2:

            MDrv_PT6964_Write_Adr_Data(LED_WORD3_ADDR, u8_LowByte);//3
            MDrv_PT6964_Write_Adr_Data(LED_WORD3_ADDR+1, u8_HighByte);
            break;

        case 3:
            MDrv_PT6964_Write_Adr_Data(LED_WORD4_ADDR, u8_LowByte);
            MDrv_PT6964_Write_Adr_Data(LED_WORD4_ADDR+1, u8_HighByte);
            break;

        default:
            break;
    }
}

//-------------------------------------------------------------
// Function: MDrv_FP_LED_CharSet
// Description :
//            Set dedicated LED data, including dot
// parameters:
//            ledBit, led bit select
//            u8LedChar: char data
//          maskenable: dot is enable
//            mask: DOT MASK data.
//-------------------------------------------------------------------
MS_BOOL MDrv_FP_LED_CharSet(MS_U8 ledBit,MS_U8 u8LEDChar, MS_BOOL maskEnable, MS_U8 mask)
{
    MS_U8 i;
    MS_U8 low,high;

     for (i = 0; i < sizeof(_char2SegmentTable) / sizeof (Char2Segment); i++)
     {
         if(u8LEDChar == _char2SegmentTable[i].u8Char)
         {
             if(maskEnable)
             {
                 #if(FP_DOT_SEG_NUM<8)
                 {
                     low = _char2SegmentTable[i].u8SegmentLowByte|mask;
                     high = _char2SegmentTable[i].u8SegmentHighByte;
                 }
                 #else
                 {
                    low = _char2SegmentTable[i].u8SegmentLowByte;
                     high = _char2SegmentTable[i].u8SegmentHighByte|mask;
                 }
                #endif
             }
             else
             {
                 low = _char2SegmentTable[i].u8SegmentLowByte;
                 high = _char2SegmentTable[i].u8SegmentHighByte;
             }
             MDrv_6961_LED_DISP(ledBit,low,high);
            return TRUE;
         }
     }
     return FALSE;
}

void MDrv_FrontPnl_Update(MS_U8 *u8str,MS_BOOL colon)
{
    MS_U8 i = 0;
    MS_U8 u8Data;

    #if FP_DOT_ENABLE
    MS_U8 j = 0;
    MS_U8 u8_DotMask;

    u8_DotMask = (FP_DOT_SEG_NUM>8)?(1<< (FP_DOT_SEG_NUM-9)):(1<<(FP_DOT_SEG_NUM-1));

    for(;i < LED_NUM; i++)
    {
    // get char
      if(u8str[j+1] == '.')
      {
        u8Data = u8str[j];
        j++;
        if(FALSE == MDrv_FP_LED_CharSet(i,u8Data,1, u8_DotMask))
        {
            DBG_LED(printf("FP LED char not defined.\n"));
        }
      }
      else
      {
        u8Data = u8str[j];
        if(FALSE == MDrv_FP_LED_CharSet(i,u8Data,0, u8_DotMask))
        {
            DBG_LED(printf("FP LED char not defined.\n"));
        }
      }
      j++;
     }
    #else

      for(;i< LED_NUM; i++)
      {
        u8Data = u8str[i];
        if(FALSE == MDrv_FP_LED_CharSet(i,u8Data,0, 0))
        {
        }
        else
        {
        }
      }
    #endif

    #if(COLON_ENABLE)
    if(colon)
    {
        MDrv_PT6964_Write_Adr_Data(LED_COLON_ADDR, LED_COLON_ENABLE_LOW);
        MDrv_PT6964_Write_Adr_Data(LED_COLON_ADDR+1, LED_COLON_ENABLE_HIGH);
    }
    else
    {
        MDrv_PT6964_Write_Adr_Data(LED_COLON_ADDR, LED_COLON_DISABLE_LOW);
        MDrv_PT6964_Write_Adr_Data(LED_COLON_ADDR+1, LED_COLON_DISABLE_HIGH);
    }
    #endif

    MDrv_PT6964_Display_On();

}

void MDrv_FrontPnl_Init(void)
{
    MDrv_PT6964_Init();

    MDrv_FrontPnl_Update((MS_U8*)"----",FALSE);
}

#endif
#endif //#ifdef DDI_MISC_INUSE
