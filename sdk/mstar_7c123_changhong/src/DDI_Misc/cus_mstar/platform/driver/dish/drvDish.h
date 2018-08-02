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
#ifndef __DRV_DISH_H__
#define __DRV_DISH_H__
#include "drvDemod.h"

typedef enum
{
    EN_LNBPWR_TYPE_OFF,
    EN_LNBPWR_TYPE_13V,
    EN_LNBPWR_TYPE_18V,
    EN_LNBPWR_TYPE_14V,
    EN_LNBPWR_TYPE_19V,
    EN_LNBPWR_TYPE_NUM
}DISH_LNBPWR_TYPE;

typedef enum
{
    EN_LNB22K_TYPE_OFF,
    EN_LNB22K_TYPE_ON,
    EN_LNB22K_TYPE_NUM
}DISH_LNB22K_TYPE;

typedef enum
{
    EN_TONEBURST_TYPE_0,
    EN_TONEBURST_TYPE_1,
    EN_TONEBURST_NUM
}EN_TONEBURST_TYPE;

typedef enum
{
    EN_CABLE_LNB_NOT_SET,
    EN_CABLE_LNB_1,
    EN_CABLE_LNB_2,
    EN_CABLE_LNB_NUM
}EN_CABLE_SELECT;

#define DDIBLD_ATTRIB_DISH_SECTION(__sect__) __attribute__((section (__sect__)))

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

#ifndef DDI_DRV_DISH_TABLE_ENTRY
#define DDI_DRV_DISH_TABLE_ENTRY( _name ) \
        DDIBLD_ATTRIB_DISH_SECTION(".ecos.dish." __xstring(_name) ".data")
#endif

typedef struct
{
    struct drv_demodtab_entry*                    pstDemodtab;
    MS_U8 u8CableIndex;                          //LNB IC to Cable connector, depend on Layout
    MS_U8 u8CurControlReg;
} DISH_MS_INIT_PARAM;

typedef MS_BOOL     drv_dishop_Init(MS_U8 u8DishIndex, DISH_MS_INIT_PARAM* pParam);
typedef MS_BOOL     drv_dishop_SetTone(MS_U8 u8DishIndex, EN_TONEBURST_TYPE enToneType);
typedef MS_BOOL     drv_dishop_SetLNBPower(MS_U8 u8DishIndex, DISH_LNBPWR_TYPE enLNBPwr);
typedef MS_BOOL     drv_dishop_Set22k(MS_U8 u8DishIndex, DISH_LNB22K_TYPE enLNB22k);
typedef MS_BOOL     drv_dishop_SendCmd(MS_U8 u8DishIndex, MS_U8* pCmd,MS_U8 u8CmdSize);
typedef MS_BOOL     drv_dishop_IsOverCurrent(MS_U8 u8DishIndex);
typedef MS_BOOL     drv_dishop_SetCable(MS_U8 u8DishIndex, EN_CABLE_SELECT eCableIndex);






typedef struct drv_dishtab_entry
{
    const char                  *name;          // demod system name
    MS_U32                      data;            // private data value
    drv_dishop_Init             *Init;
    drv_dishop_SetTone          *SetTone;
    drv_dishop_SetLNBPower      *SetLNBPower;
    drv_dishop_Set22k           *Set22k;
    drv_dishop_SendCmd          *SendCmd;
    drv_dishop_IsOverCurrent    *IsOverCurrent;
    drv_dishop_SetCable       *SetCable;
} DRV_DISH_TABLE_TYPE;

#define DISHTAB_ENTRY(                         \
        _l,                                     \
        _name,                                 \
        _data,                                 \
        _Init,                                 \
        _SetTone,                                 \
        _SetLNBPower,                                 \
        _Set22k,                                 \
        _SendCmd,                                 \
        _IsOverCurrent,                                 \
        _SetCable                                 \
)                                                                   \
struct drv_dishtab_entry _l DDI_DRV_DISH_TABLE_ENTRY(dishtab) =            \
{                                                                       \
    _name,                                                              \
    _data,                                                              \
    _Init,                                                              \
    _SetTone,                                                               \
    _SetLNBPower,                                                               \
    _Set22k,                                                               \
    _SendCmd,                                                               \
    _IsOverCurrent,                                                               \
    _SetCable                                 \
};


//--------------------------------------------------------
// initialize
// param none
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_Init(MS_U8 u8DishIndex,DISH_MS_INIT_PARAM* pParam);
//--------------------------------------------------------
// set toneburst
// param enToneType \b IN:toneburst type
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_SetTone(MS_U8 u8DishIndex, EN_TONEBURST_TYPE enToneType);
//--------------------------------------------------------
// set lnb power type
// param enLNBPwr \b IN:power type
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_SetLNBPower(MS_U8 u8DishIndex, DISH_LNBPWR_TYPE enLNBPwr);
//--------------------------------------------------------
// set lnb 22k type
// param enLNB22k \b IN:22k type
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_Set22k(MS_U8 u8DishIndex, DISH_LNB22K_TYPE enLNB22k);
//--------------------------------------------------------
// dish send command
// param pCmd \b IN:command
//          u8CmdSize \b IN:command size
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_SendCmd(MS_U8 u8DishIndex, MS_U8* pCmd,MS_U8 u8CmdSize);

//--------------------------------------------------------
// dish check over current
// param None
// return bool
//--------------------------------------------------------
extern MS_BOOL MDrv_Dish_IsOverCurrent(MS_U8 u8DishIndex);

#endif // #define __DRV_DISH_H__
