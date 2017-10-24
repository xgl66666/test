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
// 设置系统参数命令

#define FD650_BIT_ENABLE  0x01    // 开启/关闭位
#define FD650_BIT_SLEEP   0x04    // 睡眠控制位
#define FD650_BIT_7SEG    0x08    // 7段控制位
#define FD650_BIT_INTENS1 0x10    // 1级亮度
#define FD650_BIT_INTENS2 0x20    // 2级亮度
#define FD650_BIT_INTENS3 0x30    // 3级亮度
#define FD650_BIT_INTENS4 0x40    // 4级亮度
#define FD650_BIT_INTENS5 0x50    // 5级亮度
#define FD650_BIT_INTENS6 0x60    // 6级亮度
#define FD650_BIT_INTENS7 0x70    // 7级亮度
#define FD650_BIT_INTENS8 0x00    // 8级亮度

#define FD650_SYSOFF  0x0400      // 关闭显示、关闭键盘
#define FD650_SYSON ( FD650_SYSOFF | FD650_BIT_ENABLE ) // 开启显示、键盘
#define FD650_SLEEPOFF  FD650_SYSOFF  // 关闭睡眠
#define FD650_SLEEPON ( FD650_SYSOFF | FD650_BIT_SLEEP )  // 开启睡眠
#define FD650_7SEG_ON ( FD650_SYSON | FD650_BIT_7SEG )  // 开启七段模式
#define FD650_8SEG_ON ( FD650_SYSON | 0x00 )  // 开启八段模式
#define FD650_SYSON_1 ( FD650_SYSON | FD650_BIT_INTENS1 ) // 开启显示、键盘、1级亮度
//以此类推
#define FD650_SYSON_4 ( FD650_SYSON | FD650_BIT_INTENS4 ) // 开启显示、键盘、4级亮度
//以此类推
#define FD650_SYSON_8 ( FD650_SYSON | FD650_BIT_INTENS8 ) // 开启显示、键盘、8级亮度


// 加载字数据命令
#define FD650_DIG0    0x1400      // 数码管位0显示,需另加8位数据
#define FD650_DIG1    0x1500      // 数码管位1显示,需另加8位数据
#define FD650_DIG2    0x1600      // 数码管位2显示,需另加8位数据
#define FD650_DIG3    0x1700      // 数码管位3显示,需另加8位数据

#define FD650_DOT     0x0080      // 数码管小数点显示

// 读取按键代码命令
#define FD650_GET_KEY 0x0700          // 获取按键,返回按键代码

// 外部程序
MS_U8 MDrv_FD650_Read( void );        // 从FD650读取按键代码
void MDrv_FD650_Write( MS_U16 cmd );  // 向FD650发出操作命�

#endif
#endif //#ifdef DDI_MISC_INUSE
