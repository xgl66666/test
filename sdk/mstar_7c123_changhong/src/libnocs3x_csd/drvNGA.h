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
/// @file   drvNGA.h
/// @brief  Nagra NOCS 3.0 Driver Interface
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_NGA_H_
#define _DRV_NGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "nocs_csd_impl.h"
#include "nocs_csd.h"
#include "drvDSCMB.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Macro and Define
//------------------------------------------------------------------------------

/// Version string.
#define NGA_DRV_VERSION   /* Character String for DRV/API version */  \
    MSIF_TAG,             /* 'MSIF'                               */  \
    MSIF_CLASS,           /* '00'                                 */  \
    MSIF_CUS,             /* 0x0000                               */  \
    MSIF_MOD,             /* 0x0000                               */  \
    MSIF_CHIP,                                                        \
    MSIF_CPU,                                                         \
    {'N','G','A',' '},    /* IP__                                 */  \
    {'0','1'},            /* 0.0 ~ Z.Z                            */  \
    {'0','2'},            /* 00 ~ 99                              */  \
    {'0','0','0','0','0','0','6','5'},  /* CL#                    */  \
    MSIF_OS

#define _MDrvNGA_Info(format, ...) __MDrvNGA_Info(__FILE__, __FUNCTION__, __LINE__, "\033[0;36m", format, ## __VA_ARGS__)
#define _MDrvNGA_Error(format, ...) __MDrvNGA_Error(__FILE__, __FUNCTION__, __LINE__, format, ## __VA_ARGS__)

int __MDrvNGA_Info(const char *file, const char *func, int line, const char *prefix, const char *format, ...);
void __MDrvNGA_Error(const char *file, const char *func, int line, const char *format, ...);
MS_BOOL _csdIsInited(void);

#define EMI_IS_NAGRA(x) ({ TCsdUnsignedInt16 e = x; ((e >> 15) & 1) == 0; })
#define EMI_IS_RAWMODE(x) ({ TCsdUnsignedInt16 e = x; (e >> 14) & 1; })

#define EMI_ALGO_MASK 0x0ff0
#define EMI_ALGO_SHIFT 4
#define EMI_ALGO_AES 0x02
#define EMI_ALGO_TDES 0x04
#define EMI_ALGO(x) ({ TCsdUnsignedInt16 e = x; ((e & EMI_ALGO_MASK) >> EMI_ALGO_SHIFT); })

#define EMI_INDEX_MASK 0x000f
#define EMI_SIZE_MASK  0xffff

#define NARGA_CAVID         0x02

#define R2R_KEY_SLOT_MAX    0x04

//#define TEST_KEY_LADDER
#define NOCS_NAGRA_KEYLADDER E_DSCMB_KL_SEL_CW
#define NOCS_ETSI_KEYLADDER E_DSCMB_KL_SEL_PVR

enum EMI {
    EMI_PAYLOAD_DVB_CSA2 = 0x0000,
    EMI_PAYLOAD_DVB_CSA3 = 0x0001,

    EMI_PAYLOAD_AES_IDSA = 0x0020,     // AES-128 ATIS IIF Default Scrambling Algorithm (IDSA)
    EMI_PAYLOAD_AES_ECB = 0x0021,      // AES-128 ECB mode. Termination in clear.
    EMI_PAYLOAD_AES_CI_PLUS = 0x0022,  // AES-128 CBC mode. IV modifiable. Termination in clear.  (CI+)
    EMI_PAYLOAD_TDES_CBC = 0x0040,     // TDES keying option 2 in CBC mode with all bits set to zero IV. DVS-042 termination.
    EMI_PAYLOAD_TDES_ECB = 0x0041,     // TDES keying option 2 in ECB mode. Termination in clear.

    EMI_RAW_AES_CBC = 0x4020,          // AES-128 in CBC mode with all bits set to zero IV.  Termination in clear.
    EMI_RAW_AES_ECB = 0x4021,          // AES-128 in ECB mode. Termination in clear.
    EMI_RAW_AES_CBC_CTR_PIFF = 0x4022, //
    EMI_RAW_AES_CBC_APPLE = 0x4023,    //
    EMI_RAW_AES_CTR_APPLE = 0x4024,    //  AES-128 in CTR  mode
    EMI_RAW_TDES_CBC = 0x4040,         // TDES keying option 2 in CBC mode with all bits set to zero IV. Termination in clear.
    EMI_RAW_TDES_ECB = 0x4041,         // TDES keying option 2 in ECB mode. Termination in clear.
};

typedef enum
{
    DSCMB_KTE_KEYENTROPY_64,
    DSCMB_KTE_KEYENTROPY_128,
}DSCMB_KTE_KEYENTROPY;

typedef enum
{
    NGA_EMI_R2R,
    NGA_EMI_SCR,
    NGA_EMI_DSC,
}NGA_EMI_MODE;

#define KEYLADDER_TIMEOUT 10

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

void MDrv_NGA_FlushMemory(void);
void MDrv_NGA_Native2MSBF(TCsd4BytesVector xBytes, const MS_U32 value);
MS_U32 MDrv_NGA_MSBF2Native(const TCsd4BytesVector xBytes);
MS_BOOL MDrv_NGA_ReverseU8Buf(MS_U8 *buf, MS_U32 len);
MS_BOOL MDrv_NGA_R2R_CipherId2KLDst(MS_U32 u32CipherId, DSCMB_KLDst *pKLDst);
MS_BOOL MDrv_NGA_R2R_Algo2EMI(TCsdR2RAlgorithm xAlgorithm, TCsdR2RCryptoOperationMode xMode, MS_U16 *pu16EMI);

TCsdStatus MDrvNGA_Init(void);
void MDrvNGA_Finish(void);
MS_BOOL MDrv_NGA_EmiIsSupport(NGA_EMI_MODE EmiMode, MS_U16 u16Emi);
MS_BOOL MDrv_NGA_R2R_Fire(MS_U8 *pu8Input, MS_U8 *pu8Output, MS_U32 u32Size, MS_U32 u32CipherId);

#ifdef TEST_KEY_LADDER
void MDrv_NGA_TestSetRootKey(MS_U8 *pRootKey);
#endif

MS_BOOL MDrv_NGA_R2RAlloc(MS_U32 *pu32CipherId);
MS_BOOL MDrv_NGA_R2RFree(MS_U32 u32CipherId);

MS_BOOL MDrv_NGA_KeyLadder(
    MS_U16 u16Emi,
    MS_U32 u32KeyPathId,
    DSCMB_Key_Type eKeyType,
    MS_U8 u8level,
    MS_U32 u32CipheredContentKeySize,
    MS_U8 *pu8CipheredContentKey,
    MS_U32 u32CipheredProtectingKeySize,
    MS_U8 *pu8L1CipheredProtectingKey,
    MS_U8 *pu8L2CipheredProtectingKey
);

MS_BOOL MDrv_NGA_Payload_SetAlgo(
    MS_U16 u16Emi,
    MS_U32 u32DscmbId,
    DSCMB_Eng_Type eEngType,
    MS_BOOL bDecrypt
);

MS_BOOL MDrv_NGA_Payload_SetKey (
        MS_U16 u16Emi,
        MS_U8 *pu8ClearHostKey,
        MS_U32 u32ClearHostKeySize,
        MS_U8 *pu8InitializationVector,
        MS_U32 u32InitializationVectorSize,
        MS_U32 u32DscmbId,
        MS_BOOL bDecrypt,
        DSCMB_Key_Type eKeyType
);

MS_BOOL MDrv_NGA_Payload_SetContentKey(
    MS_U16 u16Emi,
    MS_U32 u32CipheredContentKeySize,
    MS_U8 *pu8CipheredContentKey,
    MS_U32 u32CipheredProtectingKeySize,
    MS_U8 *pu8L1CipheredProtectingKey,
    MS_U8 *pu8L2CipheredProtectingKey,
    MS_U32 u32InitializationVectorSize,
    MS_U8 *pu8InitializationVector,
    MS_U32 u32DscmbId,
    MS_U8  u8level,
    MS_BOOL bDecrypt,
    DSCMB_Key_Type eKeyType
);

MS_BOOL MDrv_NGA_R2R_SetAlgo(
    MS_U16 u16Emi,
    MS_U32 u32CipherId,
    MS_BOOL bDecrypt
);

MS_BOOL MDrv_NGA_R2R_SetKey (
    MS_U16 u16Emi,
    MS_U32 u32ClearTextScrHostKeySize,
    MS_U8 *pu8ClearTextScrHostKey,
    MS_U32 u32InitializationVectorSize,
    MS_U8 *pu8InitializationVector,
    MS_U32 u32CipherId,
    MS_BOOL bDecrypt
);


MS_BOOL MDrv_NGA_R2R_SetContentKey(
    MS_U16 u16Emi,
    MS_U32 u32CipheredContentKeySize,
    MS_U8 *pu8CipheredContentKey,
    MS_U32 u32CipheredProtectingKeySize,
    MS_U8 *pu8L1CipheredProtectingKey,
    MS_U8 *pu8L2CipheredProtectingKey,
    MS_U32 u32InitializationVectorSize,
    MS_U8 *pu8InitializationVector,
    MS_U32 u32CipherId,
    MS_U8  u8level,
    MS_BOOL bDecrypt
);

MS_BOOL MDrv_NGA_R2R_UpdateIV(MS_U8 *pu8IV, MS_U32 u32Size, MS_U32 u32CipherId);

//PVR Flush
MS_BOOL MDrv_NGA_TSP_PVR_Flush(void);
MS_U8 MDrv_NGA_ReadStrapping(void);

#ifdef __cplusplus
}
#endif

#endif

