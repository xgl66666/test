#ifndef __DRV_AKL_V2_H__
#define __DRV_AKL_V2_H__

#include "drvAKL.h"
#include "UFO.h"

#ifdef UFO_AKL_CERT

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    E_MDRV_CMD_AKL_Init,
    E_MDRV_CMD_AKL_Reset,
    E_MDRV_CMD_AKL_SetDbgLevel,
    E_MDRV_CMD_AKL_SetManualACK,
    E_MDRV_CMD_AKL_SetDMAKey,
    E_MDRV_CMD_AKL_SetDMAParserKey,
    E_MDRV_CMD_AKL_SetDSCMBKey,
    E_MDRV_CMD_AKL_SetTSIOKey,
    E_MDRV_CMD_AKL_SetManualACKMode,
    E_MDRV_CMD_AKL_CERTCmd,
} eAKLIoctlOpt;

typedef struct DLL_PACKED _AKL_DMA_KEY
{
    MS_U32 u32KeyIndex;
} AKL_DMA_KEY;

typedef struct DLL_PACKED _AKL_DMAPARSER_KEY
{
    MS_U32 u32OddKeyIndex;
    MS_U32 u32EvenKeyIndex;
} AKL_DMAPARSER_KEY;

typedef struct DLL_PACKED _AKL_DSCMB_KEY
{
    MS_U32 u32EngId;
    MS_U32 u32Index;
    AKL_Eng_Type eEngType;
    AKL_Key_Type eKeyType;
} AKL_DSCMB_KEY;

typedef struct DLL_PACKED _AKL_TSIO_KEY
{
    MS_U32 u32ServiceId;
    AKL_Key_Type eKeyType;
} AKL_TSIO_KEY;

typedef struct DLL_PACKED _AKL_DBGLEVEL
{
    AKL_DBGMSG_LEVEL eDBGMsgLevel;
} AKL_DBGLEVEL;

typedef struct DLL_PACKED _AKL_ACKMODE
{
    MS_BOOL bEnable;
} AKL_ACKMODE;

typedef struct DLL_PACKED _AKL_EXCHANGE
{
    AKL_CERT_Cmd eCmd;
    MS_U8 *pu8Data;
    MS_U32 u32DataSize;
} AKL_EXCHANGE;


#ifdef __cplusplus
}
#endif

#endif //UFO_AKL_CERT

#endif // __DRV_AKL_V2_H__
