#include "MsTypes.h"
#include "MsOS.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HbCommon.h"
#include "porting_sysinfo.h"
#include "porting_cusinfo.h"

#ifdef MS_DEBUG
#define PT_SYS_DBG(x) x
#else
#define PT_SYS_DBG(x)
#endif

#define MAX_WINDOW_NUM 4

MMSDK_U8 u8Customer_info[PT_CUS_INFO_SIZE];
MMSDK_U8 u8Customer_hash[PT_CUS_HASH_SIZE];
MMSDK_U16 u16DRM_MODEL_ID = 0;
static MS_S32 _gs32CachedPoolID = 0;
static MS_S32 _gs32NonCachedPoolID = 0;
static MMSDK_BOOL gbEnGraphicDisplay = TRUE;
static MS_U8 gu8DrawPhotoPath = 0;
PT_SYS_DataBaseOp PT_SYS_DB;
PT_SYS_MmapInfo stSysMmapInfo;
ST_MMSDK_MMAP_INFO stSysMIUInfo;
MS_U32 gu32VQBuffer = 0;
MS_U32 gu32VQSize = 0;
static AUDIO_DEC_ID geAudDecID = AU_DEC_INVALID;
static MMSDK_U8 u8AVPWindowOrder[MAX_WINDOW_NUM] = {0, 1, 2, 3};

PT_SYS_CUS_FunPtr gstSysCusFunPtr = {NULL};
PT_SYS_SubRnd_FunPtr gstSysSubRndFunPtr = {NULL};

#if 0
static void _DumpCustInfo(void)
{
    MMSDK_U8 i = 0;

    PT_SYS_DBG(printf("****** Customer_info: ******"));

    for (i = 0; i < PT_CUS_INFO_SIZE; i++)
    {
        if (i%8 == 0)
            PT_SYS_DBG(printf("\n"));
        PT_SYS_DBG(printf("0x%02x,", u8Customer_info[i]));

    }

    PT_SYS_DBG(printf("\n"));
}

static void _DumpHashKey(void)
{
    MMSDK_U8 i = 0;

    PT_SYS_DBG(printf("****** Hash_Key: ******\n"));

    for (i = 0; i < PT_CUS_HASH_SIZE; i++)
    {
        if (i%8 == 0)
            PT_SYS_DBG(printf("\n"));
        PT_SYS_DBG(printf("0x%02x,", u8Customer_hash[i]));

    }

    PT_SYS_DBG(printf("\n"));
}
#endif

MMSDK_BOOL PT_SYS_SetCusInfo(MMSDK_U8 *pu8CusInfo, MMSDK_U8 *pu8CusHash)
{

    if ((pu8CusInfo == NULL) || (pu8CusHash == NULL))
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    memcpy(u8Customer_info, pu8CusInfo, PT_CUS_INFO_SIZE);
    memcpy(u8Customer_hash, pu8CusHash, PT_CUS_HASH_SIZE);

    return TRUE;
}

MMSDK_BOOL PT_SYS_GetCusInfo(MMSDK_U8 *pu8Info, MMSDK_U8 pu8CusInfoSize)
{
    if (pu8Info == NULL)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    memcpy(pu8Info, u8Customer_info, pu8CusInfoSize);

    return TRUE;
}

MMSDK_BOOL PT_SYS_GetHashKey(MMSDK_U8 *pu8Key, MMSDK_U8 pu8HashKeySize)
{
    if (pu8Key == NULL)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    memcpy(pu8Key, u8Customer_hash, pu8HashKeySize);

    return TRUE;
}

MMSDK_BOOL PT_SYS_SetDRMID(MMSDK_U16 u16CusDRMID)
{
    u16DRM_MODEL_ID = u16CusDRMID;

    return TRUE;
}

MMSDK_BOOL PT_SYS_GetDRMID(MMSDK_U16 *pu16CusDRMID)
{
    if (pu16CusDRMID == NULL)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    *pu16CusDRMID = u16DRM_MODEL_ID;

    return TRUE;
}

MMSDK_BOOL PT_SYS_SetMemPoolID(MMSDK_S32 gs32MstarCachedPoolID, MMSDK_S32 gs32MstarNonCachedPoolID)
{
    _gs32CachedPoolID = gs32MstarCachedPoolID;
    _gs32NonCachedPoolID = gs32MstarNonCachedPoolID;

    return TRUE;
}

MMSDK_S32 PT_SYS_GetCachePoolID(void)
{
    return _gs32CachedPoolID;
}

MMSDK_S32 PT_SYS_GetNonCachePoolID(void)
{
    return _gs32NonCachedPoolID;
}

MMSDK_BOOL PT_SYS_SetDBReadWrite(PT_SYS_ReadDB pReadCallback, PT_SYS_WriteDB pWriteCallback)
{
    PT_SYS_DB.pSysReadDB = pReadCallback;
    PT_SYS_DB.pSysWriteDB = pWriteCallback;

    return TRUE;
}

MMSDK_BOOL PT_SYS_ReadFromDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    if ((pstDRMInfo == NULL) || (PT_SYS_DB.pSysReadDB == NULL))
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    PT_SYS_DB.pSysReadDB(pstDRMInfo);

    return TRUE;
}

MMSDK_BOOL PT_SYS_WriteToDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo)
{
    if ((pstDRMInfo == NULL) || (PT_SYS_DB.pSysWriteDB == NULL))
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    PT_SYS_DB.pSysWriteDB(pstDRMInfo);

    return TRUE;
}

MMSDK_BOOL PT_SYS_SetTspVQInfo(MMSDK_U32 u32VQBuffer, MMSDK_U32 u32VQSize)
{
    gu32VQBuffer = u32VQBuffer;
    gu32VQSize = u32VQSize;

    return TRUE;
}

MMSDK_BOOL PT_SYS_GetTspVQInfo(MMSDK_U32 *u32VQBuffer, MMSDK_U32 *u32VQSize)
{
    *u32VQBuffer = gu32VQBuffer;
    *u32VQSize = gu32VQSize;

    return TRUE;
}

MMSDK_BOOL PT_SYS_SetMmapInfo(PT_SYS_MmapInfo *stMmapInfo, ST_MMSDK_MMAP_INFO *stMIUInfo)
{
    if (stMmapInfo == NULL)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    memcpy(&stSysMmapInfo, stMmapInfo, sizeof(PT_SYS_MmapInfo));
    memcpy(&stSysMIUInfo, stMIUInfo, sizeof(ST_MMSDK_MMAP_INFO));
    return TRUE;
}

MMSDK_BOOL  PT_SYS_GetMIUInfo(ST_MMSDK_MMAP_INFO* pInfo)
{
    if (pInfo == NULL)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid Src Info\n", __FUNCTION__));
        return FALSE;
    }

    memcpy(pInfo, &stSysMIUInfo, sizeof(ST_MMSDK_MMAP_INFO));

    return  TRUE;
}

MMSDK_U32 PT_SYS_GetMmapInfo(ST_MMSDK_BUF_INFO* pstMemInfo, EN_MMSDK_BUF_ID eBufferID, EN_MMSDK_BUF_INFO eInfo)
{
    ST_MMSDK_BUF_INFO *stMemInfo = NULL;

    PT_SYS_DBG(printf("Enter. PT_SYS_GetMmapInfo (%d)\n", eBufferID));
    switch(eBufferID)
    {
        case E_MMSDK_BUF_PHOTO_DISPLAY:
            stMemInfo = &stSysMmapInfo.stPhotoDispBufAddr;
            break;

        case E_MMSDK_BUF_PHOTO_DISPLAY_MPO:
            stMemInfo = &stSysMmapInfo.stPhotoMPODispBufAddr;
            break;

        case E_MMSDK_BUF_PHOTO_INTERNAL:
            stMemInfo = &stSysMmapInfo.stJPDInterBufAddr;
            break;

        case E_MMSDK_BUF_PHOTO_MEMORY_POOL:
            stMemInfo = &stSysMmapInfo.stPhotoMemPoolAddr;
            break;

        case E_MMSDK_BUF_PHOTO_OUTPUT:
            stMemInfo = &stSysMmapInfo.stJPDOutBufAddr;
            break;

        case E_MMSDK_BUF_PHOTO_PROGRESSIVE:
            stMemInfo = &stSysMmapInfo.stPhotoProgAddr;
            break;

        case E_MMSDK_BUF_PHOTO_READ:
            stMemInfo = &stSysMmapInfo.stJPDReadBufAddr;
            break;

        case E_MMSDK_BUF_PHOTO_SHAREMEMORY:
            stMemInfo = &stSysMmapInfo.stPhotoShareMemAddr;
            break;

        case E_MMSDK_BUF_VDPLAYER_AEON_DATA:
            stMemInfo = &stSysMmapInfo.stVdplayerDataAddr;
            break;

        case E_MMSDK_BUF_VDPLAYER_BITSTREAM_AUDIO:
            stMemInfo = &stSysMmapInfo.stVdplayerBSExtAddr;
            break;

        case E_MMSDK_BUF_VDPLAYER_BITSTREAM_SUBTITLE:
            stMemInfo = &stSysMmapInfo.stVdplayerSubBSAddr;
            break;

        case E_MMSDK_BUF_VDPLAYER_BITSTREAM_VIDEO:
            stMemInfo = &stSysMmapInfo.stVdplayerBSAddr;
            break;

        case E_MMSDK_BUF_VDPLAYER_SHAREMEMORY:
            stMemInfo = &stSysMmapInfo.stVdplayerShmMemAddr;
            break;

        case E_MMSDK_BUF_SUBTITLE_PUSH:
            stMemInfo = &stSysMmapInfo.stVdplayerSubAddr;
            break;

        case E_MMSDK_BUF_DEMUX_SEC:
            stMemInfo = &stSysMmapInfo.stTspFWAddr;
            break;

        case E_MMSDK_BUF_MAD_DECODE:
            stMemInfo = &stSysMmapInfo.stMADDecBufAddr;
            break;

        case E_MMSDK_BUF_VDEC_CPU:
            stMemInfo = &stSysMmapInfo.stVdecAeonAddr;
            break;

        case E_MMSDK_BUF_VDEC_FRAMEBUFFER:
            stMemInfo = &stSysMmapInfo.stVdecFrameBufAddr;
            break;

        case E_MMSDK_BUF_VDEC_SUB_FRAMEBUFFER:
            stMemInfo = &stSysMmapInfo.stVdecSubFrameBufAddr;
            break;

        case E_MMSDK_BUF_VDEC_FRAMEBUFFER_MVC:
            stMemInfo = &stSysMmapInfo.stVdecMVCFrameBufAddr;
            break;

        case E_MMSDK_BUF_VDEC_FRAMEBUFFER_SD:
            stMemInfo = &stSysMmapInfo.stVdecFrameBufferSDAddr;
            break;

        case E_MMSDK_BUF_VDEC_STREAMBUFFER:
            stMemInfo = &stSysMmapInfo.stVdecBitStreamAddr;
            break;

        case E_MMSDK_BUF_VDEC_SUB_STREAMBUFFER:
            stMemInfo = &stSysMmapInfo.stVdecSubBitStreamAddr;
            break;

        case E_MMSDK_BUF_VDEC_STREAMBUFFER_MVC:
            stMemInfo = &stSysMmapInfo.stVdecMVCBitStreamAddr;
            break;

        case E_MMSDK_BUF_XC_MAIN_FB:
            stMemInfo = &stSysMmapInfo.stXCMainFrameBufAddr;
            break;

        case E_MMSDK_BUF_PHY_ADDR_FOR_COPY_DATA:
            stMemInfo = &stSysMmapInfo.stPHYAddrForCopyDataAddr;
            break;

		case E_MMSDK_BUF_XC_DS:
			stMemInfo = & stSysMmapInfo.stXCMCUDynamicScalingAddr;
			break;

        case E_MMSDK_BUF_DV_XC:
            stMemInfo = &stSysMmapInfo.stXCHdrAddr;
            break;

        case E_MMSDK_BUF_VDEC_SHARE_MEM:
        case E_MMSDK_BUF_VDEC_FRAMEBUFFER_EVD_EXT:
        case E_MMSDK_BUF_SUBTITLE_FONT_TABLE:
        case E_MMSDK_BUF_VDPLAYER_LOADCODE:
        default:
            PT_SYS_DBG(printf("[Error] wrong eBufferID id %d\n", eBufferID));
            return 0;
    }

    if (pstMemInfo != NULL)
    {
        memcpy(pstMemInfo, stMemInfo, sizeof(ST_MMSDK_BUF_INFO));
    }

    switch(eInfo)
    {
        case E_MMSDK_BUF_INFO_PHY_ADDR:
            return stMemInfo->u32Addr;

        case E_MMSDK_BUF_INFO_VIRTUAL_ADDR:
            return stMemInfo->u32VirtualAddr;

        case E_MMSDK_BUF_INFO_SIZE:
            return stMemInfo->u32Size;

        case E_MMSDK_BUF_INFO_ISMIU0:
            return stMemInfo->bIsMiu0;

        default:
            printf("[Error] wrong eInfo id %d\n", eInfo);
            return 0;
    }
}

MMSDK_BOOL PT_SYS_SetAudDecID(AUDIO_DEC_ID eAudDecID)
{
    if(eAudDecID >= AU_DEC_MAX)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid input argument\n", __FUNCTION__));
        return FALSE;
    }

    geAudDecID = eAudDecID;
    return TRUE;
}

MMSDK_BOOL PT_SYS_GetAudDecID(AUDIO_DEC_ID *peAudDecID)
{
    if(peAudDecID == NULL)
    {
        PT_SYS_DBG(printf("[%s] Fail. Invalid input argument\n", __FUNCTION__));
        return FALSE;
    }

    *peAudDecID = geAudDecID;

    return TRUE;
}


MMSDK_BOOL PT_SYS_SetCustomerizeFunPtr(PT_SYS_CUS_FunPtr *pstCusFunPtr)
{
    if (NULL != pstCusFunPtr)
    {
        gstSysCusFunPtr.pBlackScreen = pstCusFunPtr->pBlackScreen;
        gstSysCusFunPtr.pSetXCWindow = pstCusFunPtr->pSetXCWindow;
        gstSysCusFunPtr.pFreezeImage = pstCusFunPtr->pFreezeImage;
        gstSysCusFunPtr.pSetDSOnOFF  = pstCusFunPtr->pSetDSOnOFF;
        gstSysCusFunPtr.pSetMode     = pstCusFunPtr->pSetMode;
        gstSysCusFunPtr.pAudMute     = pstCusFunPtr->pAudMute;
        gstSysCusFunPtr.pDSSendXCStatus2Firmware = pstCusFunPtr->pDSSendXCStatus2Firmware;
        gstSysCusFunPtr.pCloseWindow = pstCusFunPtr->pCloseWindow;
        gstSysCusFunPtr.pSetMode2 = pstCusFunPtr->pSetMode2;
        gstSysCusFunPtr.pSetXCWindow2 = pstCusFunPtr->pSetXCWindow2;
        gstSysCusFunPtr.pInitialize = pstCusFunPtr->pInitialize;
        gstSysCusFunPtr.pAudInitSetup = pstCusFunPtr->pAudInitSetup;
        gstSysCusFunPtr.pAudFinalizeSetup = pstCusFunPtr->pAudFinalizeSetup;
        gstSysCusFunPtr.pSetDispParams  = pstCusFunPtr->pSetDispParams;
        gstSysCusFunPtr.pDispFinalize  = pstCusFunPtr->pDispFinalize;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MMSDK_BOOL PT_SYS_SetSubtitleRndFunPtr(PT_SYS_SubRnd_FunPtr *pstSubRndFunPtr)
{
    if (NULL != pstSubRndFunPtr)
    {
        gstSysSubRndFunPtr.pRndOpen = pstSubRndFunPtr->pRndOpen;
        gstSysSubRndFunPtr.pRndClose = pstSubRndFunPtr->pRndClose;
        gstSysSubRndFunPtr.pRndCreateWindow = pstSubRndFunPtr->pRndCreateWindow;
        gstSysSubRndFunPtr.pRndDestroyWindow  = pstSubRndFunPtr->pRndDestroyWindow;
        gstSysSubRndFunPtr.pRndSetPalette     = pstSubRndFunPtr->pRndSetPalette;
        gstSysSubRndFunPtr.pRndBitblt     = pstSubRndFunPtr->pRndBitblt;
        gstSysSubRndFunPtr.pRndClearWindowByIndex = pstSubRndFunPtr->pRndClearWindowByIndex;
        gstSysSubRndFunPtr.pRndClearWindowByColor = pstSubRndFunPtr->pRndClearWindowByColor;
        gstSysSubRndFunPtr.pRndGetDisplaySize = pstSubRndFunPtr->pRndGetDisplaySize;
        gstSysSubRndFunPtr.pRndShow = pstSubRndFunPtr->pRndShow;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MMSDK_BOOL PT_SYS_SetGraphicDisplayFlag(MMSDK_BOOL bEnable)
{
    gbEnGraphicDisplay = bEnable;
    return TRUE;
}

MMSDK_BOOL PT_SYS_GetGraphicDisplayFlag(void)
{
    return gbEnGraphicDisplay;
}

MMSDK_BOOL PT_SYS_InitAVPZOrder(void)
{
    int WinIdx = 0;
    for (WinIdx = 0; WinIdx <MAX_WINDOW_NUM; WinIdx++)
    {
        u8AVPWindowOrder[WinIdx] = WinIdx;
    }

    return TRUE;
}

MMSDK_BOOL PT_SYS_SetAVPZOrder(MMSDK_U8 u8WinID, MMSDK_U8 u8Order)
{
    int WinIdx = 0;
    for (WinIdx = 0; WinIdx <MAX_WINDOW_NUM; WinIdx++)
    {
        if ((WinIdx != u8WinID) && (u8AVPWindowOrder[WinIdx] == u8Order))
        {
            return FALSE;
        }
    }

    u8AVPWindowOrder[u8WinID] = u8Order;
    return TRUE;
}

MMSDK_U8 PT_SYS_GetAVPZOrder(MMSDK_U8 u8WinID)
{
    return u8AVPWindowOrder[u8WinID];
}

MMSDK_BOOL PT_SYS_SetMMPhotoPath(MMSDK_U8 u8Path)
{
    gu8DrawPhotoPath = u8Path;
    return TRUE;
}

MMSDK_U8 PT_SYS_GetMMPhotoPath(void)
{
    return gu8DrawPhotoPath;
}
