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
// Copyright (c) 2010-2011 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MSBOARD_H_
#define _MSBOARD_H_

#define BOARD_NAME                  "BD_MST290A_D01A_S"

#ifdef __cplusplus
extern "C"
{
#endif

//------Chip Type---------------------------------------------------------------
#include "chip/MSD7C137D.h"

//------Peripheral Device Setting-----------------------------------------------
#define FAKE_SPI_TOTAL_SIZE (0x1000000)
#define FAKE_SPI_BLOCK_SIZE (64 * 1024)

//============================================
// Use ATSC code base audio path setting
//============================================
#define AUDIO_SOURCE_DTV            AUDIO_DSP1_DVB_INPUT
#define AUDIO_SOURCE_DTV2           AUDIO_DSP2_DVB_INPUT
#define AUDIO_SOURCE_ATV            AUDIO_DSP3_SIF_INPUT
#define AUDIO_SOURCE_PC             AUDIO_AUIN1_INPUT
#define AUDIO_SOURCE_YPBPR          AUDIO_AUIN2_INPUT
#define AUDIO_SOURCE_YPBPR2         AUDIO_AUIN2_INPUT
#define AUDIO_SOURCE_AV             AUDIO_AUIN3_INPUT//AUDIO_AUIN3_INPUT
#define AUDIO_SOURCE_AV2            AUDIO_AUIN3_INPUT
#define AUDIO_SOURCE_AV3            AUDIO_AUIN3_INPUT
#define AUDIO_SOURCE_SV             AUDIO_AUIN3_INPUT//AUDIO_AUIN3_INPUT
#define AUDIO_SOURCE_SV2            AUDIO_AUIN3_INPUT
#define AUDIO_SOURCE_SCART          AUDIO_AUIN0_INPUT
#define AUDIO_SOURCE_SCART2         AUDIO_AUIN0_INPUT//AUDIO_AUIN0_INPUT
#define AUDIO_SOURCE_HDMI           AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI2          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI3          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_DVI            AUDIO_AUIN1_INPUT
#define AUDIO_SOURCE_DVI2           AUDIO_AUIN1_INPUT
#define AUDIO_SOURCE_DVI3           AUDIO_AUIN1_INPUT
#define AUDIO_SOURCE_KTV            AUDIO_NULL_INPUT

//volume path, used for adjust volume or set mute
#define AUDIO_PATH_MAIN_SPEAKER     AUDIO_T3_PATH_AUOUT0
#define AUDIO_PATH_HP               AUDIO_PATH_NULL
#define AUDIO_PATH_LINEOUT          AUDIO_PATH_NULL
#define AUDIO_PATH_SIFOUT           AUDIO_PATH_NULL
#define AUDIO_PATH_SCART1           AUDIO_PATH_NULL
#define AUDIO_PATH_SCART2           AUDIO_PATH_NULL
#define AUDIO_PATH_I2S              AUDIO_T3_PATH_I2S
#define AUDIO_PATH_SPDIF            AUDIO_T3_PATH_SPDIF
#define AUDIO_PATH_HDMI             AUDIO_T3_PATH_HDMI
#define AUDIO_PATH_BTSC1            AUDIO_T3_PATH_AUOUT1
#define AUDIO_PATH_BTSC2            AUDIO_T3_PATH_AUOUT2

//output port path, need check the board layout. ex: main speaker output via dac0
#define AUDIO_OUTPUT_MAIN_SPEAKER   AUDIO_AUOUT0_OUTPUT
#define AUDIO_OUTPUT_HP             AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_I2S            AUDIO_I2S_OUTPUT
#define AUDIO_OUTPUT_SPDIF          AUDIO_SPDIF_OUTPUT
#define AUDIO_OUTPUT_ARC            AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_HDMI           AUDIO_HDMI_OUTPUT
#define AUDIO_OUTPUT_LINEOUT        AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SIFOUT         AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SCART1         AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SCART2         AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_BTSC1          AUDIO_AUOUT1_OUTPUT
#define AUDIO_OUTPUT_BTSC2          AUDIO_AUOUT2_OUTPUT

//===============================================================
//------Tuner Setting-----------------------------------------------------------
#define TS_CLK_INV                  1
#define TS_PARALLEL_OUTPUT          1

#define BD_MST072A_D01A_WITH_ONBOARD_TUNER_ATV

#define MSB1210_TS_SERIAL_INVERSION          0
#define MSB1210_TS_PARALLEL_INVERSION      1
#define MSB1210_DTV_DRIVING_LEVEL             1 //0 or 1
#define MSB1210_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE  1

#define INTERN_DVBT_TS_SERIAL_INVERSION       0
#define INTERN_DVBT_TS_PARALLEL_INVERSION     1
#define INTERN_DVBT_DTV_DRIVING_LEVEL          1
#define INTERN_DVBT_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE  1

//------IR & Key Setting--------------------------------------------------------
#define XTAL_RTC_CLOCK_FREQ         CRYSTAL_12000000
#define XTAL_CLOCK_FREQ             CRYSTAL_12000000

//------IR & Key Setting--------------------------------------------------------
#define IR_TYPE_SEL                     IR_TYPE_MSTAR_DTV   // IR_TYPE_MSTAR_DTV // IR_TYPE_CUS03_DTV // IR_TYPE_NEW
#define POWER_KEY_SEL                   POWER_KEY_PAD_INT

//------Memory Setting----------------------------------------------------------
#define BOOTUP_MIU_BIST                 1
#ifndef MEMORY_MAP
#define MEMORY_MAP                      MMAP_128MB
#endif
#define  MIU_INTERFACE                  DDR3_INTERFACE_BGA

//------DEMOD Setting----------------------------------------------------------
#define FRONTEND_DEMOD_IQ_SWAP 0
#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_Q_IN // C:Q T:I

//------DEMOD 1 Setting----------------------------------------------------------
#define FRONTEND_DEMOD_IQ_TYPE1      DEMOD_IQ_I_IN // C:Q T:I

//------TUNER Setting----------------------------------------------------------
#define FRONTEND_TUNER_PORT         E_HWI2C_PORT1_1

#define FRONTEND_TUNER_PORT0        E_HWI2C_PORT1_1
#define FRONTEND_TUNER_CTRL         PIU_IIC_0

//------TUNER 1 Setting----------------------------------------------------------
#define FRONTEND_TUNER_PORT1         E_HWI2C_PORT0_1
#define FRONTEND_TUNER_CTRL1         PIU_IIC_0

//------TUNER 2 Setting----------------------------------------------------------
#define FRONTEND_TUNER_PORT2         E_HWI2C_PORT0_1
#define FRONTEND_TUNER_CTRL2         PIU_IIC_1

//-------DISH Setting---------------------------------------------------------
#define DISH_TYPE                    DISH_A8304
#define DISH_TYPE1                   DISH_A8304
#define DISH_IIC_PORT               E_HWI2C_PORT0_1
#define DISH_IIC_PORT1              E_HWI2C_PORT0_1

//------TS Input source setting-------------------------------------------------
#undef DMX_INPUT
#if((MS_DVB_TYPE_SEL == DVBC))
#define DMX_INPUT                       DMX_INPUT_DEMOD
#else
#define DMX_INPUT                       DMX_INPUT_EXT_INPUT0
#endif

//------Front Panel Setting----------------------------------------------------
#define FRONTPNL_TYPE                   FRONTPANEL_TYPE_PT6958//FRONTPANEL_TYPE_74HC164//FRONTPANEL_TYPE_PT6958

#if (FRONTPNL_TYPE == FRONTPANEL_TYPE_PT6958)
#define FRONTPNL_DIO       PAD_NC
#define FRONTPNL_CLK       PAD_NC
#define FRONTPNL_STB       PAD_NC
#define FRONTPNL_LED_RED   PAD_NC
#define FRONTPNL_LED_GREEN PAD_NC

#elif (FRONTPNL_TYPE == FRONTPANEL_TYPE_74HC164)
#define FRONTPNL_DATA   PAD_NC
#define FRONTPNL_CLK    PAD_NC
#define FRONTPNL_DIGIT0 PAD_NC
#define FRONTPNL_DIGIT1 PAD_NC
#define FRONTPNL_DIGIT2 PAD_NC
#define FRONTPNL_DIGIT3 PAD_NC
#endif

//-------------------------------------------------
// Power Saving
#define Power_On()                      _FUNC_NOT_USED()
#define Power_Off()                     _FUNC_NOT_USED()
#define MDrv_Sys_GetSvideoSw()          _FUNC_NOT_USED()

#define Peripheral_Device_Reset_ON()    _FUNC_NOT_USED()
#define Peripheral_Device_Reset_OFF()   _FUNC_NOT_USED()
#define Tuner_ON()                      _FUNC_NOT_USED()
#define Tuner_OFF()                     _FUNC_NOT_USED()
#define Demodulator_ON()                _FUNC_NOT_USED()
#define Demodulator_OFF()               _FUNC_NOT_USED()
#define LAN_ON()                        _FUNC_NOT_USED()
#define LAN_OFF()                       _FUNC_NOT_USED()

#define TunerOffPCMCIA()                _FUNC_NOT_USED()
#define TunerOnPCMCIA()                 _FUNC_NOT_USED()

// ----- HDMITX HPD pin --------------------------------------------------------
#define HDMITX_HPD_PM_GPIO		        PAD_PM_HDMI_HPD

// ----- USB Port0 Power Control -----------------------------------------------
#define GPIO_USB0_POWER PAD_PM_GPIO5
#define GPIO_USB1_POWER PAD_TM_GPIO1

// ----- HDMITX ANALOG CONFIGURATION --------------------------------------------------------
// ----- QFP216 -----------------------------------------------------------------------------
#define HDMITX_ANALOG_TUNING_SD		    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define HDMITX_ANALOG_TUNING_HD         {0x01, 0x00, 0x00, 0x00, 0x00, 0x07}
#define HDMITX_ANALOG_TUNING_DEEP_HD    {0x01, 0x00, 0x00, 0x00, 0x00, 0x07}

// ----- Dram Self-Refresh Type ----------------------------------------------------------------
#define DDR_SELF_REFRESH_TYPE           DDR2_SELFREFRESH//DDR3_SELFREFRESH

#define GPIO_FE_RST                      PAD_PM_GPIO3   //Tuner RST
#define GPIO_FE_RST1                     PAD_PM_GPIO7  //Tuner RST
#define GPIO_SMC0_EXT_8024_VOL_CTRL      PAD_SM0_GPIO   //SMC EXT8024 Voltage Ctrl

//------GPIO/SCART pin mapping------------------------------------
#define GPIO_SCART_PIN8                  PAD_NC     //SCART_PIN8_CTL0(Pin8_6V)
#define GPIO_SCART_PIN16                 PAD_NC    //RGB/CVBS
#define GPIO_SCART_ONSTBY                PAD_NC     //SCART_PIN8_CTL1(Pin8_0V)


#ifdef __cplusplus
}
#endif

#endif // _MSBOARD_H_
