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
#ifdef DDI_MISC_INUSE

#ifndef __FP__FD650__H
#define __FP__FD650__H
#include "drvGPIO.h"
#include "Board.h"
/****************************begin for FD650 define struct***************************** */
#define LEDMAPNUM 35
//key mapping
typedef struct
{
  MS_U8 keyMapData;
  MS_U8 keyMapLevel;

} MS_KEYMAP;

//led mapping
typedef struct _led_bitmap
{
  MS_U8 character;
  MS_U8 bitmap;
} led_bitmap;


/****************************end for FD650 define struct***************************** */


/* **************************************function dec*********************************************** */
extern void Delay1(void);
/* *************************************hardware related*********************************************** */
#define   DELAY1    {Delay1();}


#define HIGH 1
#define LOW  0


/* ********************************************************************************************* */
// ÉèÖÃÏµÍ³²ÎÊýÃüÁî

#define FD650_BIT_ENABLE  0x01    // ¿ªÆô/¹Ø±ÕÎ»
#define FD650_BIT_SLEEP   0x04    // Ë¯Ãß¿ØÖÆÎ»
#define FD650_BIT_7SEG    0x08    // 7¶Î¿ØÖÆÎ»
#define FD650_BIT_INTENS1 0x10    // 1¼¶ÁÁ¶È
#define FD650_BIT_INTENS2 0x20    // 2¼¶ÁÁ¶È
#define FD650_BIT_INTENS3 0x30    // 3¼¶ÁÁ¶È
#define FD650_BIT_INTENS4 0x40    // 4¼¶ÁÁ¶È
#define FD650_BIT_INTENS5 0x50    // 5¼¶ÁÁ¶È
#define FD650_BIT_INTENS6 0x60    // 6¼¶ÁÁ¶È
#define FD650_BIT_INTENS7 0x70    // 7¼¶ÁÁ¶È
#define FD650_BIT_INTENS8 0x00    // 8¼¶ÁÁ¶È

#define FD650_SYSOFF  0x0400      // ¹Ø±ÕÏÔÊ¾¡¢¹Ø±Õ¼üÅÌ
#define FD650_SYSON ( FD650_SYSOFF | FD650_BIT_ENABLE ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ
#define FD650_SLEEPOFF  FD650_SYSOFF  // ¹Ø±ÕË¯Ãß
#define FD650_SLEEPON ( FD650_SYSOFF | FD650_BIT_SLEEP )  // ¿ªÆôË¯Ãß
#define FD650_7SEG_ON ( FD650_SYSON | FD650_BIT_7SEG )  // ¿ªÆôÆß¶ÎÄ£Ê½
#define FD650_8SEG_ON ( FD650_SYSON | 0x00 )  // ¿ªÆô°Ë¶ÎÄ£Ê½
#define FD650_SYSON_1 ( FD650_SYSON | FD650_BIT_INTENS1 ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ¡¢1¼¶ÁÁ¶È
//ÒÔ´ËÀàÍÆ
#define FD650_SYSON_4 ( FD650_SYSON | FD650_BIT_INTENS4 ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ¡¢4¼¶ÁÁ¶È
//ÒÔ´ËÀàÍÆ
#define FD650_SYSON_8 ( FD650_SYSON | FD650_BIT_INTENS8 ) // ¿ªÆôÏÔÊ¾¡¢¼üÅÌ¡¢8¼¶ÁÁ¶È


// ¼ÓÔØ×ÖÊý¾ÝÃüÁî
#define FD650_DIG0    0x1400      // ÊýÂë¹ÜÎ»0ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý
#define FD650_DIG1    0x1500      // ÊýÂë¹ÜÎ»1ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý
#define FD650_DIG2    0x1600      // ÊýÂë¹ÜÎ»2ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý
#define FD650_DIG3    0x1700      // ÊýÂë¹ÜÎ»3ÏÔÊ¾,ÐèÁí¼Ó8Î»Êý¾Ý

#define FD650_DOT     0x0080      // ÊýÂë¹ÜÐ¡ÊýµãÏÔÊ¾

// ¶ÁÈ¡°´¼ü´úÂëÃüÁî
#define FD650_GET_KEY 0x0700          // »ñÈ¡°´¼ü,·µ»Ø°´¼ü´úÂë

// Íâ²¿³ÌÐò
MS_U8 MDrv_FD650_Read( void );        // ´ÓFD650¶ÁÈ¡°´¼ü´úÂë
void MDrv_FD650_Write( MS_U16 cmd );  // ÏòFD650·¢³ö²Ù×÷ÃüÁ

#endif
#endif //#ifdef DDI_MISC_INUSE
