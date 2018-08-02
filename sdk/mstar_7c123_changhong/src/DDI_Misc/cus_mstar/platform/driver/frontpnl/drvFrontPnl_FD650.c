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
#if(FRNTPANL_TYPE_SEL == FRNTPANL_TYPE_FD650)
#include  "drvFrontPnl_FD650.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define FRONTPNL_START_TIME_MS      3   //((1000 / 50) / LED_NUM)
#define FRONTPNL_PERIOD_TIME_MS     150
#define FP_LED_MAX_NUM 5
#define DBG_LED(msg)                    msg

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static const  led_bitmap bcd_decode_tab[LEDMAPNUM] =
{
    {'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
    {'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
    {'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
    {'b', 0x7C}, {'B', 0x7C}, {'c', 0x58}, {'C', 0x39},
    {'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
    {'f', 0x71}, {'F', 0x71}, {'o', 0x5C}, {'t', 0x78},
    {'l', 0x30}, {'L', 0x38}, {'n', 0x37}, {'p', 0x73},
    {'P', 0x73}, {'O', 0x3F}, {'u', 0x1C}, {'U', 0x3E},
    {'S', 0x6D}, {'s', 0x6D},{'-', 0x40}
};//BCD decode

/****************************************************************
 *
 *   Function Name:Led_Get_Code
 *
 *   Description:×ª»»×Ö·ûÎªÊýÂë¹ÜµÄÏÔÊ¾Âë£¬ÈçÓöµ½ÎÞ·¨×ª»»µÄ×Ö·û·µ»Ø0
 *
 *   Parameter: cTemp  ´ý×ª»»ÎªÏÔÊ¾ÂëµÄ×Ö·û
 *
 *   return£ºÏÔÊ¾ÂëÖµ
****************************************************************/
static MS_U8 MDrv_Led_Get_Code(MS_U8 cTemp)
{
    MS_U8 i, bitmap = 0x00;

    for(i = 0; i < LEDMAPNUM; i++)
    {
    if(bcd_decode_tab[i].character == cTemp)
    {
             bitmap = bcd_decode_tab[i].bitmap;
             break;
         }
    }
    return bitmap;
}

//-------------------------------------------------------------------------------------------------
/// initialze Front Panel display control.
/// Function Name:MDrv_FrontPnl_Update
///
/// Description:¼òµ¥µÄÊýÂë¹ÜÏÔÊ¾Ó¦ÓÃÊ¾Àý
///
///  Parameter:  FPStr ÏÔÊ¾µÄ×Ö·û´®£»
///                  sec_flag¿ªÆôÐ¡Êýµã±êÖ¾Î»£»
///  return£ºÎÞ
//-------------------------------------------------------------------------------------------------
void MDrv_FrontPnl_Update(MS_U8 *u8str, MS_BOOL sec_flag)
{
    int i, iLength;
    int data[4] = {0x00, 0x00, 0x00, 0x00};

    if(!u8str)
   {
        return;
   }

    iLength = strlen(u8str);

    if(iLength > 4)
         iLength = 4;

    for(i = 0; i < iLength; i++)
   {
         data[i] = MDrv_Led_Get_Code(u8str[i]);
   }

   MDrv_FD650_Write(FD650_SYSON_8);// ¿ªÆôÏÔÊ¾ºÍ¼üÅÌ£¬8¶ÎÏÔÊ¾·½Ê½
   //·¢ÏÔÊ¾Êý¾Ý
   MDrv_FD650_Write( FD650_DIG0 | (MS_U8)data[0] ); //µãÁÁµÚÒ»¸öÊýÂë¹Ü

   if(sec_flag)
        MDrv_FD650_Write( FD650_DIG1 | (MS_U8)data[1] | FD650_DOT ); //µãÁÁµÚ¶þ¸öÊýÂë¹Ü
   else
        MDrv_FD650_Write( FD650_DIG1 | (MS_U8)data[1] );

    MDrv_FD650_Write( FD650_DIG2 | (MS_U8)data[2]); //µãÁÁµÚÈý¸öÊýÂë¹Ü
    MDrv_FD650_Write( FD650_DIG3 | (MS_U8)data[3] ); //µãÁÁµÚËÄ¸öÊýÂë¹Ü
}

//-------------------------------------------------------------------------------------------------
/// initialze Front Panel control.
/// @return TRUE - Success
///         FALSE - Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FrontPnl_EnableLED(MS_BOOL  eEnableLED)
{

    if(eEnableLED)
    {
        mdrv_gpio_set_high(GPIO_FP_CLK);
    }
    else
    {
        mdrv_gpio_set_low(GPIO_FP_CLK);
    }
    return TRUE;
}

void MDrv_FD650_Init(void)
{
      MDrv_FrontPnl_EnableLED(TRUE);
      MDrv_FD650_Write(FD650_SYSON_8);
}

void MDrv_FrontPnl_Init(void)
{
     MDrv_FD650_Init();
     MDrv_FrontPnl_Update((MS_U8*)"----",FALSE);
}

#endif
#endif //#ifdef DDI_MISC_INUSE

