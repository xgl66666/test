////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvOTP.h
/// @brief
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_OTP_H
#define _DRV_OTP_H

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Macro and Define
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum _OTP_Config {
	U_OTP_CA_reserved2,
	U_OTP_CA_reserved3,
	U_OTP_CA_reserved1,
	U_OTP_LockWord_W0000_0511_Addr,
	U_OTP_LockWord_W0512_1023_Addr,
	U_OTP_LockWord_W1024_1535_Addr,
	U_OTP_LockWord_W1536_2047_Addr,
	U_OTP_LockWord_W2048_2559_Addr,
	U_OTP_LockWord_W2560_3071_Addr,
	U_OTP_LockWord_W3072_3583_Addr,
	U_OTP_LockWord_W3584_3759_Addr,
	U_OTP_LockWord_Reserved_Key,
	U_OTP_RD55AA,
	U_OTP_LockWord_CAV_Key1_W,
	U_OTP_LockWord_CAV_Key2_W,
	U_OTP_LockWord_CAV_Key3_W,
	U_OTP_LockWord_CAV_Key4_W,
	U_OTP_LockWord_CAV_Key5_W,
	U_OTP_LockWord_CAV_Key6_W,
	U_OTP_LockWord_CAV_Key7_W,
	U_OTP_LockWord_CAV_Key8_W,
	U_OTP_LockWord_CAV_Key1_R,
	U_OTP_LockWord_CAV_Key2_R,
	U_OTP_LockWord_CAV_Key3_R,
	U_OTP_LockWord_CAV_Key4_R,
	U_OTP_LockWord_CAV_Key5_R,
	U_OTP_LockWord_CAV_Key6_R,
	U_OTP_LockWord_CAV_Key7_R,
	U_OTP_LockWord_CAV_Key8_R,
	U_OTP_BlkLock_Start_Addr,
	U_OTP_BlkLock_End_Addr,
	U_OTP_BlkLock_W,
	U_OTP_BlkLock_R,
	U_OTP_LOCK_SPECIAL0,
	U_OTP_LockWord_PubOtp,
	U_OTP_LockWord_VendorConstSel,
	U_OTP_LockWord_v_PubOtpBID,
	U_OTP_LockWord_v_PubOtpGP,
	U_OTP_LockWord_v_PubOtpMinConfVer,
	U_OTP_LockWord_v_PubOtpOID,
	U_OTP_LockWord_v_PubOtpVID,
	U_OTP_LockWord_v_pubOtpUniqueID1,
	U_OTP_LockWord_v_pubOtpUniqueID2,
	U_OTP_LockWord_v_PubOtpRsaIndex,
	U_OTP_LOCK_SPECIAL1,
	U_OTP_LockWord_RSA_Bank2,
    U_OTP_LockWord_RSA_Bank3,
   	U_OTP_LockWord_RSA_Bank,
	U_OTP_LockWord_OTP_CTRL,
	U_OTP_LockWord_ACPU_bank,
	U_OTP_LOCK_trimming,
	U_OTP_LockWord_DMA_bank,
	U_OTP_LockWord_KL_bank,
	U_OTP_LockWord_KT_bank,
	U_OTP_LockWord_TSCE0,
	U_OTP_LockWord_TSCE1,
	U_OTP_LockWord_VtrackDataW,
	U_OTP_LockWord_VtrackDataR,
	U_OTP_LockWord_CA_PWDW,
	U_OTP_LockWord_CA_PWDR,
	U_OTP_LockWord_Multi2SysKey,
	U_OTP_ena_SPSSPDKeyShuffle,
	U_OTP_dis_MCUWriteOTP,
	U_OTP_dis_MCUReadOTP,
	U_OTP_dis_DbusWriteOTP,
	U_OTP_dis_DbusReadOTP,
	U_OTP_RANDOM,
	U_OTP_NOISE,
	U_OTP_BGC_EN,
	U_OTP_parity_check_en,
	U_OTP_PGMask0,
	U_OTP_PGMask1,
	U_OTP_PGMask2,
	U_OTP_PGMask3,
	U_OTP_PGMask4,
	U_OTP_PGMask5,
	U_OTP_PGMask6,
	U_OTP_PGMask7,
	U_OTP_EN_XOR_ROMCODE,
	U_OTP_DeactiveDRAMBoot,
	U_OTP_ACPUBootMode,
	U_OTP_PostMskAreaRangeU,
	U_OTP_PostMskAreaRangeL,
	U_OTP_ScanMode,
	U_OTP_SecretAreaEnable,
	U_OTP_ena_KeyLadder,
	U_OTP_Sboot,
	U_OTP_I2CMode,
	U_OTP_DIG_JTAGMode,
	U_OTP_EJTAG_Mode,
	U_OTP_Disable_SPI_FastBurn_Mode,
	U_OTP_Bootcode_project,
	U_OTP_Bootcode_subversion,
	U_OTP_Bootcode_version,
	U_OTP_BOND0,
	U_OTP_BOND0_HIGH,
	U_OTP_BOND1,
	U_OTP_BOND1_HIGH,
	U_OTP_ACPUasHost,
	U_OTP_RMA_Mode_Lock,
	U_OTP_RMA_Mode_Enable,
	U_OTP_BONDING,
	U_OTP_LicRework,
	U_OTP_BOND_Reserved,
	U_OTP_ena_ACPU2DMA,
	U_OTP_ena_DMA_DESBasedCipher,
	U_OTP_ena_DMA_GetKeyFromKL,
	U_OTP_ena_DMA_KeyEntropyCheck,
	U_OTP_ena_DMA_KeyUsageCheck,
	U_OTP_KDF_secret_mask,
	U_OTP_allow_checkKLinternalkey,
	U_OTP_allow_checkKLrootkey,
	U_OTP_disable_CW_KeyLadder_SW_Key,
	U_OTP_disable_PVR_KeyLadder_SW_Key,
	U_OTP_allow_ACPU2KT,
	U_OTP_allow_3DESBasedCipher,
	U_OTP_allow_AESBasedCipher,
	U_OTP_allow_DESBasedCipher,
	U_OTP_allow_DVBCSA2Cipher,
	U_OTP_allow_DVBCSA2ComfCipher,
	U_OTP_allow_DVBCSA3BasedCipher,
	U_OTP_allow_DVBCSA3Var,
	U_OTP_allow_KL2KT,
	U_OTP_enable_CA_reserved,
	U_OTP_OBFUSCATEVideoStream,
	U_OTP_CAV1_VendorID,
	U_OTP_VendorConstSel,
	U_OTP_v_PubOtpBID,
	U_OTP_v_PubOtpGP,
	U_OTP_v_PubOtpMinConfVer,
	U_OTP_v_PubOtpOID,
	U_OTP_v_PubOtpVID,
	U_OTP_v_PubOtpRsaIndex,
	U_OTP_v_pubOtpUniqueID1,
	U_OTP_v_pubOtpUniqueID1_HIGH,
	U_OTP_v_pubOtpUniqueID2,
	U_OTP_v_pubOtpUniqueID2_HIGH,
	U_OTP_CAVKeyValid,
	U_OTP_CAV_Key1_SCK_Property0,
	U_OTP_CAV_Key1_SCK_Property1,
	U_OTP_CAV_Key2_SCK_Property0,
	U_OTP_CAV_Key2_SCK_Property1,
	U_OTP_CAV_Key3_SCK_Property0,
	U_OTP_CAV_Key3_SCK_Property1,
	U_OTP_CAV_Key4_SCK_Property0,
	U_OTP_CAV_Key4_SCK_Property1,
	U_OTP_CAV_Key5_SCK_Property0,
	U_OTP_CAV_Key5_SCK_Property1,
	U_OTP_CAV_Key6_SCK_Property0,
	U_OTP_CAV_Key6_SCK_Property1,
	U_OTP_CAV_Key7_SCK_Property0,
	U_OTP_CAV_Key7_SCK_Property1,
	U_OTP_CAV_Key8_SCK_Property0,
	U_OTP_CAV_Key8_SCK_Property1,
	U_OTP_CAV_Key1_Integrity_chk,
	U_OTP_CAV_Key2_Integrity_chk,
	U_OTP_CAV_Key3_Integrity_chk,
	U_OTP_CAV_Key4_Integrity_chk,
	U_OTP_CAV_Key5_Integrity_chk,
	U_OTP_CAV_Key6_Integrity_chk,
	U_OTP_CAV_Key7_Integrity_chk,
	U_OTP_CAV_Key8_Integrity_chk,
	U_OTP_CA_PWD_integrity_chk,
	U_OTP_I2C_PWD_use_aes,
	U_OTP_SCAN_PWD_use_aes,
	U_OTP_CA_PWD_CRC16,
	U_OTP_allow_checkKLLUT0eqKey,
    // OTP configuration for the csdGetChipExtension. All in U_OTP_BOND0
    U_OTP_BOND_DCS,
	U_OTP_BOND_MacroVision,
	U_OTP_BOND_WaterMark,

} OTP_Config;


//-------------------------------------------------------------------------------------------------
//  Function prototypes
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_NGA_OTP_GetConfig(OTP_Config eConf);
MS_BOOL MDrv_NGA_OTP_SetConfig(OTP_Config eConf, MS_U32 value);
MS_BOOL My_CA_Init(void);


#ifdef __cplusplus
}
#endif

#endif // _DRV_OTP_H

