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

#ifndef _APPDEMO_INPUT_H_
#define _APPDEMO_INPUT_H_


//-------------------------------------------------------------------------------------------------
//  Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/* --- key defines ---*/
/* General Key - for general use ( Range: 0x10~0xDF )*/
#define DTV_KEY_OK					0x10
#define DTV_KEY_EXIT                0x11
#define DTV_KEY_MENU                0x12
#define DTV_KEY_TTX					0x13
#define DTV_KEY_STDBY				0x14
#define DTV_KEY_MUTE                0x15
#define DTV_KEY_SUBT                0x16
#define DTV_KEY_LEFT                0x20
#define DTV_KEY_RIGHT				0x21
#define DTV_KEY_UP					0x22
#define DTV_KEY_DOWN                0x23
#define DTV_KEY_CHDOWN				0x24
#define DTV_KEY_CHUP                0x25
#define DTV_KEY_VDOWN				0x26
#define DTV_KEY_VUP					0x27
#define DTV_KEY_PP					0x28
#define DTV_KEY_0					0x30
#define DTV_KEY_1					0x31
#define DTV_KEY_2					0x32
#define DTV_KEY_3					0x33
#define DTV_KEY_4					0x34
#define DTV_KEY_5					0x35
#define DTV_KEY_6					0x36
#define DTV_KEY_7					0x37
#define DTV_KEY_8					0x38
#define DTV_KEY_9					0x39
#define DTV_KEY_VFORMAT				0x40
#define DTV_KEY_TV					0x41
#define DTV_KEY_PIP					0x42
#define DTV_KEY_ARATIO				0x43
#define DTV_KEY_AUDIO				0x50
#define DTV_KEY_INFO                0x51
#define DTV_KEY_FAV					0x52
#define DTV_KEY_LIST                0x53
#define DTV_KEY_EPG					0x54
#define DTV_KEY_RED					0x60
#define DTV_KEY_GREEN				0x61
#define DTV_KEY_YELLOW				0x62
#define DTV_KEY_BLUE                0x63
#define DTV_KEY_SELECT				0x64
#define DTV_KEY_PICTURE				0x65
#define DTV_KEY_FILE                0x66
#define DTV_KEY_EXT					0x67
#define DTV_KEY_TEXT                0x68
#define DTV_KEY_PLAY                0x70
#define DTV_KEY_STOP                0x71
#define DTV_KEY_PAUSE				0x72
#define DTV_KEY_FF					0x73
#define DTV_KEY_FR					0x74
#define DTV_KEY_REC					0x75
#define DTV_KEY_REC_LIST            0x76
#define DTV_KEY_FAST_FORWARD		0x77
#define DTV_KEY_EJECT               0x78
#define DTV_KEY_FORWARD             0x79
#define DTV_KEY_BACKWARD			0x7A
#define DTV_KEY_STOP_RECORD         0x7B
#define DTV_KEY_PAUSE_RECORD		0x7C
	// 0x7D~0x7F: Reserved
#define DTV_KEY_POWER_ON			0x80
#define DTV_KEY_ENTER               0x81
#define DTV_KEY_RETURN			    0x82
#define DTV_KEY_BACK			    0x82	/* To Do: Remove this key, please don't use it */
#define DTV_KEY_CANCEL			    0x83
#define DTV_KEY_CLEAR			    0x84
#define DTV_KEY_MENU_TV			    0x85
#define DTV_KEY_MENU_DTT			0x86
#define DTV_KEY_HELP			    0x87
#define DTV_KEY_REPEAT			    0x88
#define DTV_KEY_SETUP			    0x89
#define DTV_KEY_GUIDE			    0x8A
#define DTV_KEY_FREEZE			    0x8B
#define DTV_KEY_PARENT			    0x8C
#define DTV_KEY_SLEEP			    0x8D
#define DTV_KEY_SOUND_SELECT		0x8E
#define DTV_KEY_PMODE			    0x8F
#define DTV_KEY_RECALL			    0x90
#define DTV_KEY_PRE_CHANNEL		    0x91
#define DTV_KEY_PIC_SIZE			0x92
#define DTV_KEY_USB			        0x93
#define DTV_KEY_TTX_CANCEL		    0x94
#define DTV_KEY_TTX_FREEZE		    0x95
#define DTV_KEY_TTX_SUBTITLE		0x96
#define DTV_KEY_TTX_REVEAL		    0x97
#define DTV_KEY_UNL			        0x98
#define DTV_KEY_AUX			        0x99
#define DTV_KEY_INPUT_TV			0x9A
#define DTV_KEY_INPUT_DTT		    0x9B
#define DTV_KEY_INPUT_DTT_TV		0x9C
#define DTV_KEY_INPUT_SELECT		0x9D
#define DTV_KEY_INPUT_AUTO		    0x9E
	// 0x9F: Reserved
#define DTV_KEY_INPUT_AV1		    0xA0
#define DTV_KEY_INPUT_AV2		    0xA1
#define DTV_KEY_INPUT_AV3		    0xA2
#define DTV_KEY_INPUT_AV4		    0xA3
#define DTV_KEY_INPUT_AV5		    0xA4
#define DTV_KEY_INPUT_AV6		    0xA5
#define DTV_KEY_INPUT_AV7		    0xA6
#define DTV_KEY_INPUT_AV8		    0xA7
#define DTV_KEY_INPUT_AV9		    0xA8
#define DTV_KEY_INPUT_AV10		    0xA9
#define DTV_KEY_INPUT_HDMI1		    0xAA
#define DTV_KEY_INPUT_HDMI2		    0xAB
#define DTV_KEY_INPUT_HDMI3		    0xAC
#define DTV_KEY_INPUT_HDMI4		    0xAD
#define DTV_KEY_INPUT_RGB		    0xAE
#define DTV_KEY_INPUT_DVI		    0xAF
#define DTV_KEY_TUNE				0xB0
#define DTV_KEY_SUB_CH				0xB1
#define DTV_KEY_SERVICE				0xB2
#define DTV_KEY_LINK                0xB3
#define DTV_KEY_OPTION				0xB4
// Add new key here

/* Multi Functional Key - for Front Panel ( Range: 0xE0~0xEF )*/
#define DTV_FP_KEY_MENU			    0xE0
#define DTV_FP_KEY_VUP			    0xE1
#define DTV_FP_KEY_VDOWN			0xE2
#define DTV_FP_KEY_CHUP			    0xE3
#define DTV_FP_KEY_CHDOWN		    0xE4
#define DTV_FP_KEY_INPUT_SELECT		0xE5
// Add new key here

/* Internal Key - for special purpose ( Range: 0xF0~0xFF )*/
#define DTV_KEY_EXITALL				0xF0
// Add new key here

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32_1stDelayTimeMs;
    MS_U32 u32_2ndDelayTimeMs;
    MS_U8 data3;
} MS_IR_DelayTime, *PMS_IR_DelayTime;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

MS_BOOL Demo_Input_Help_linux(void);
MS_BOOL Demo_Input_Init_linux(void);
MS_BOOL Demo_Input_Exit_linux(void);


#endif // _APPDEMO_INPUT_H_

