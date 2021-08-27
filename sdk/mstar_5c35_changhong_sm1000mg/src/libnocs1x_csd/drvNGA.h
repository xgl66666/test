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
/// @brief  Nagra NOCS 1.2 Driver Interface
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_NGA_H_
#define _DRV_NGA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "nocs_csd_impl.h"
#include "nocs_csd.h"

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

#define NARGA_CAVID 0x02

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

#define KEYLADDER_TIMEOUT 10

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

void MDrv_NGA_FlushMemory(const void *addr, size_t size);
void MDrv_NGA_Native2MSBF(TCsd4BytesVector xBytes, const MS_U32 value);
MS_U32 MDrv_NGA_MSBF2Native(const TCsd4BytesVector xBytes);

TCsdStatus MDrvNGA_Init(void);
void MDrvNGA_Finish(void);
void MDrvNGA_R2RDMA_Fire(MS_U8 *pu8Input, MS_U8 *pu8Output, MS_U32 u32Size);

TCsdStatus MDrvNGA_R2RDMA_ClearTextHostKey
(
        TCsdR2RAlgorithm            xAlgorithm,
        TCsdR2RCryptoOperationMode  xMode,
  const TCsdUnsignedInt8*           pxClearTextHostKey,
        TCsdSize                    xClearTextHostKeySize,
  const TCsdUnsignedInt8*           pxInitializationVector,
        TCsdSize                    xInitializationVectorSize,
        TCsdBoolean                 xRefreshIv,
        TCsdR2RKeyPathHandle*       pxR2RKeyPathHandle,
  const TCsdUnsignedInt8*           pxInputData,
        TCsdUnsignedInt8*           pxOutputData,
        TCsdSize                    xDataSize,
        TCsdBoolean                 xDecrypt
);

TCsdStatus MDrvNGA_R2RDMA_SecretContentKey
(
        TCsdR2RAlgorithm                xAlgorithm,
        TCsdR2RCryptoOperationMode      xMode,
  const TCsdR2RCipheredProtectingKeys   xR2RCipheredProtectingKeys,
  const TCsdUnsignedInt8*               pxCipheredContentKey,
        TCsdSize                        xCipheredContentKeySize,
  const TCsdUnsignedInt8*               pxInitializationVector,
        TCsdSize                        xInitializationVectorSize,
        TCsdBoolean                     xRefreshIv,
        TCsdR2RKeyPathHandle*           pxR2RKeyPathHandle,
  const TCsdUnsignedInt8*               pxInputData,
        TCsdUnsignedInt8*               pxOutputData,
        TCsdSize                        xDataSize,
        TCsdBoolean                     xDecrypt
);

TCsdStatus MDrvNGA_SCR_Payload_SetKey
(
        TCsdUnsignedInt16           xEmi,
  const TCsdUnsignedInt8*           pxClearTextScrHostKey,
        TCsdSize                    xClearTextScrHostKeySize,
  const TCsdUnsignedInt8*           pxInitializationVector,
        TCsdSize                    xInitializationVectorSize,
        TCsdScrKeyPathHandle*       pxScrKeyPathHandle,
        TCsdBoolean                 xDecrypt
);

TCsdStatus MDrvNGA_SCR_Raw_SetKey
(
        TCsdUnsignedInt16           xEmi,
  const TCsdUnsignedInt8*           pxClearTextScrHostKey,
        TCsdSize                    xClearTextScrHostKeySize,
  const TCsdUnsignedInt8*           pxInitializationVector,
        TCsdSize                    xInitializationVectorSize,
        TCsdScrKeyPathHandle*       pxScrKeyPathHandle,
        TCsdBoolean                 xDecrypt
);

TCsdStatus MDrvNGA_SCR_Raw_SetContentKey
(
        TCsdUnsignedInt16               xEmi,
  const TCsdScrCipheredProtectingKeys   xScrCipheredProtectingKeys,
  const TCsdUnsignedInt8*               pxCipheredScrContentKey,
        TCsdSize                        xCipheredScrContentKeySize,
  const TCsdUnsignedInt8*               pxInitializationVector,
        TCsdSize                        xInitializationVectorSize,
        TCsdScrKeyPathHandle*           pxScrKeyPathHandle,
        TCsdBoolean                     xDecrypt
);

TCsdStatus MDrvNGA_SCR_Payload_SetContentKey
(
        TCsdUnsignedInt16               xEmi,
  const TCsdScrCipheredProtectingKeys   xScrCipheredProtectingKeys,
  const TCsdUnsignedInt8*               pxCipheredScrContentKey,
        TCsdSize                        xCipheredScrContentKeySize,
  const TCsdUnsignedInt8*               pxInitializationVector,
        TCsdSize                        xInitializationVectorSize,
        TCsdScrKeyPathHandle*           pxScrKeyPathHandle,
        TCsdBoolean                     xDecrypt
);
#ifdef __cplusplus
}
#endif

#endif

