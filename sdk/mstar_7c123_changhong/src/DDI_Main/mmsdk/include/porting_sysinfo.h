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

#ifndef __porting_sysinfo__
#define __porting_sysinfo__
#include "mmsdk_interface_def.h"
#include "porting_cusinfo.h"
#include "porting_mmap.h"

#include "drvAUDIO_if.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


typedef struct
{
    MMSDK_U32 u32SizeofStructure;
    MMSDK_BOOL bIsDSEnabled;
    void *pstCropRect;
    void *pstDispRect;
    MMSDK_U8 u8Flow;
    MMSDK_U8 u8Win;
} PT_SYS_CusWindowInfo;

typedef struct
{
    MMSDK_U32 u32SizeofStructure;
    void *pDispItem;
    MMSDK_U32 u32SizeofDispItem;
    MMSDK_BOOL bAvpEnable;
} PT_SYS_CusInitInfo;

typedef struct
{
    MMSDK_U32 u32Reserve;
} PT_SYS_DispFinalizeInfo;

typedef struct
{
    MMSDK_U32 u32SizeofStructure;
    MMSDK_BOOL bIsDSEnabled;
    MMSDK_U8 u8Flow;
} PT_SYS_CusSetModeInfo;

typedef void(*PT_SYS_ReadDB)(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo);
typedef void(*PT_SYS_WriteDB)(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo);

typedef MMSDK_BOOL (*PT_SYS_CUS_SetDSOnOFF)(MMSDK_BOOL bEnableDS);
typedef MMSDK_BOOL (*PT_SYS_CUS_SetDispParams)(PT_SYS_CusInitInfo *stCusInitInfo);
typedef MMSDK_BOOL (*PT_SYS_CUS_FreezeImage)(MMSDK_BOOL bOnOff);
typedef MMSDK_BOOL (*PT_SYS_CUS_SetXCWindow)(MMSDK_U16 u16MediaType, MMSDK_BOOL bIsDSEnabled, void * pstCropRect, void * pstDispRect);
typedef MMSDK_BOOL (*PT_SYS_CUS_SetXCWindow2)(MMSDK_U16 u16MediaType, PT_SYS_CusWindowInfo *stCusWindoInfo, void* pParam);
typedef MMSDK_BOOL (*PT_SYS_CUS_Initilize)(PT_SYS_CusInitInfo *stCusInitInfo);
typedef MMSDK_BOOL (*PT_SYS_CUS_BlackScreen)(MMSDK_U16 u16MediaType, MMSDK_BOOL bOnOff);
typedef MMSDK_BOOL (*PT_SYS_CUS_SetMode)(MMSDK_U16 u16MediaType, MMSDK_BOOL bIsDSEnabled);
typedef MMSDK_BOOL (*PT_SYS_CUS_SetMode2)(MMSDK_U16 u16MediaType, PT_SYS_CusSetModeInfo *stCusSetModInfo, void* pParam);
typedef MMSDK_BOOL (*PT_SYS_CUS_AudMute)(MMSDK_BOOL bMuteOnOff);
typedef MMSDK_BOOL (*PT_SYS_CUS_DSSendXCStatus2Firmware)(void * pstCropWin, void * pstDispWin, MMSDK_U8 eWin);
typedef MMSDK_BOOL (*PT_SYS_CUS_CloseWindow)(MMSDK_U16 u16MediaType);
typedef MMSDK_BOOL (*PT_SYS_CUS_AudInitSetup)(void *);
typedef MMSDK_BOOL (*PT_SYS_CUS_AudFinalizeSetup)(void *);
typedef MMSDK_BOOL (*PT_SYS_CUS_DispFinalize)(PT_SYS_DispFinalizeInfo *pstDispFinalizeInfo);

/// the callback for open
typedef MMSDK_BOOL (*PT_SYS_SubRnd_Open)(void *pvArg);
/// the callback for close
typedef MMSDK_BOOL (*PT_SYS_SubRnd_Close)(void *pvArg);
/// the callback for create window
typedef void* (*PT_SYS_SubRnd_CreateWindow)(void *ptWinAttr, void *pvArg);
/// the callback for destroy window
typedef MMSDK_BOOL (*PT_SYS_SubRnd_DestroyWindow)(void *hWinHnd, void *pvArg);
/// the callback for set palette
typedef MMSDK_BOOL (*PT_SYS_SubRnd_SetPalette)(void *hWinHnd, MMSDK_U8* pPalArray, MMSDK_U16 u16PaletteSize, MMSDK_U8 u8Index, void *pvArg);
/// the callback for bitblt
typedef MMSDK_BOOL (*PT_SYS_SubRnd_Bitblt)(void *pstRect, void *pstParam, void *pvArg);
/// the callback for clear window by color index
typedef MMSDK_BOOL (*PT_SYS_SubRnd_ClearWindowByIndex)(void *hWinHnd, void *pRect, MMSDK_U8 u8ColorIndex, void *pvArg);
/// the callback for clear window by color
typedef MMSDK_BOOL (*PT_SYS_SubRnd_ClearWindowByColor)(void *hWinHnd, void *pRect, void *pColor, void *pvArg);
//  the callback for getting display size
typedef MMSDK_BOOL (*PT_SYS_SubRnd_GetDispalySize)(void *pRect, void *pvArg);
/// the callback for show
typedef MMSDK_BOOL (*PT_SYS_SubRnd_Show)(void *phWinHnd, MMSDK_BOOL bShow, void *pvArg);

typedef struct
{
    PT_SYS_ReadDB    pSysReadDB;
    PT_SYS_WriteDB   pSysWriteDB;
} PT_SYS_DataBaseOp;

typedef struct
{
    ST_MMSDK_BUF_INFO   stPhotoDispBufAddr;         //JPD_INTERBUFF_ADR
    ST_MMSDK_BUF_INFO   stPhotoMPODispBufAddr;      //JPD_INTERBUFF_ADR
    ST_MMSDK_BUF_INFO   stJPDInterBufAddr;          //JPD_INTERBUFF_ADR
    ST_MMSDK_BUF_INFO   stPhotoMemPoolAddr;         //PHOTO_MEM_POOL_ADR
    ST_MMSDK_BUF_INFO   stJPDOutBufAddr;            //JPD_OUT_ADR
    ST_MMSDK_BUF_INFO   stPhotoProgAddr;            //PHOTO_PROG_ADR
    ST_MMSDK_BUF_INFO   stJPDReadBufAddr;           //JPD_READBUFF_ADR
    ST_MMSDK_BUF_INFO   stPhotoShareMemAddr;        //PHOTO_SHAREMEM_ADR
    ST_MMSDK_BUF_INFO   stVdplayerDataAddr;         //VDPLAYER_DATA_ADR
    ST_MMSDK_BUF_INFO   stVdplayerBSExtAddr;        //VDPLAYER_BS_EXT_ADR
    ST_MMSDK_BUF_INFO   stVdplayerSubBSAddr;        //VDPLAYER_SUB_BS_ADR
    ST_MMSDK_BUF_INFO   stVdplayerBSAddr;           //VDPLAYER_BS_ADR
    ST_MMSDK_BUF_INFO   stVdplayerShmMemAddr;       //VDPLAYER_SHMMEM_ADR
    ST_MMSDK_BUF_INFO   stVdplayerSubAddr;          //VDPLAYER_SUB_ADR
    ST_MMSDK_BUF_INFO   stTspFWAddr;                //TSP_FW_BUF
    ST_MMSDK_BUF_INFO   stMADDecBufAddr;            //MAD_DEC_BUFFER_ADR
    ST_MMSDK_BUF_INFO   stVdecAeonAddr;             //VDEC_AEON_ADR
    ST_MMSDK_BUF_INFO   stVdecFrameBufAddr;         //VDEC_FRAMEBUFFER_ADR
    ST_MMSDK_BUF_INFO   stVdecSubFrameBufAddr;      //VDEC_FRAMEBUFFER_ADR1
    ST_MMSDK_BUF_INFO   stVdecMVCFrameBufAddr;      //VDEC_MVC_FRAMEBUFFER_ADR
    ST_MMSDK_BUF_INFO   stVdecFrameBufferSDAddr;    //VDEC_FRAMEBUFFER_SD_ADR
    ST_MMSDK_BUF_INFO   stVdecBitStreamAddr;        //VDEC_BITSTREAM_ADR
    ST_MMSDK_BUF_INFO   stVdecSubBitStreamAddr;     //VDEC_BITSTREAM_ADR1
    ST_MMSDK_BUF_INFO   stVdecMVCBitStreamAddr;     //VDEC_MVC_BITSTREAM_ADR
    ST_MMSDK_BUF_INFO   stXCMainFrameBufAddr;       //SC0_MAIN_FB_ADR
    ST_MMSDK_BUF_INFO   stPHYAddrForCopyDataAddr;   //E_MMSDK_BUF_PHY_ADDR_FOR_COPY_DATA
    ST_MMSDK_BUF_INFO   stXCMCUDynamicScalingAddr;  //SC0_MCU_DS
    ST_MMSDK_BUF_INFO   stVdecShareMemoryAddr;      //VDEC_SHARE_MEM
    ST_MMSDK_BUF_INFO   stXCHdrAddr;                //SC_HDR_ADR
} PT_SYS_MmapInfo;

typedef struct
{
    PT_SYS_CUS_BlackScreen pBlackScreen;
    PT_SYS_CUS_SetXCWindow pSetXCWindow;
    PT_SYS_CUS_FreezeImage pFreezeImage;
    PT_SYS_CUS_SetDSOnOFF pSetDSOnOFF;
    PT_SYS_CUS_SetMode pSetMode;
    PT_SYS_CUS_AudMute pAudMute;
    PT_SYS_CUS_DSSendXCStatus2Firmware pDSSendXCStatus2Firmware;
    PT_SYS_CUS_CloseWindow pCloseWindow;
    PT_SYS_CUS_SetMode2 pSetMode2;
    PT_SYS_CUS_SetXCWindow2 pSetXCWindow2;
    PT_SYS_CUS_Initilize pInitialize;
    PT_SYS_CUS_AudInitSetup pAudInitSetup;
    PT_SYS_CUS_AudFinalizeSetup pAudFinalizeSetup;
    PT_SYS_CUS_SetDispParams pSetDispParams;
    PT_SYS_CUS_DispFinalize pDispFinalize;
} PT_SYS_CUS_FunPtr;

typedef struct
{
    PT_SYS_SubRnd_Open               pRndOpen;
    PT_SYS_SubRnd_Close              pRndClose;
    PT_SYS_SubRnd_CreateWindow       pRndCreateWindow;
    PT_SYS_SubRnd_DestroyWindow      pRndDestroyWindow;
    PT_SYS_SubRnd_SetPalette         pRndSetPalette;
    PT_SYS_SubRnd_Bitblt             pRndBitblt;
    PT_SYS_SubRnd_ClearWindowByIndex pRndClearWindowByIndex;
    PT_SYS_SubRnd_ClearWindowByColor pRndClearWindowByColor;
    PT_SYS_SubRnd_GetDispalySize     pRndGetDisplaySize;
    PT_SYS_SubRnd_Show               pRndShow;
} PT_SYS_SubRnd_FunPtr;

//-------------------------------------------------------------------------------------------------
/// Set Customer Info
/// @param pu8Cusinfo               \b OUT: customer info
/// @param pu8Cushash             \b OUT: hash key
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetCusInfo(MMSDK_U8 *pu8CusInfo, MMSDK_U8 *pu8CusHash);

//-------------------------------------------------------------------------------------------------
/// Get Customer Info
/// @param pu8Info                     \b OUT: customer info
/// @param pu8CusInfoSize        \b IN: bytes about customer info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_GetCusInfo(MMSDK_U8 *pu8Info, MMSDK_U8 pu8CusInfoSize);

//-------------------------------------------------------------------------------------------------
/// Get Hash Key
/// @param pu8Key                      \b OUT: hash key
/// @param pu8HashKeySize       \b IN: bytes about customer info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_GetHashKey(MMSDK_U8 *pu8Key, MMSDK_U8 pu8HashKeySize);

//-------------------------------------------------------------------------------------------------
/// Set DRM Model ID
/// @param u16CusDRMID                \b IN: drm id
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetDRMID(MMSDK_U16 u16CusDRMID);

//-------------------------------------------------------------------------------------------------
/// Get DRM Model ID
/// @param pu16CusDRMID                \b OUT: drm id
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_GetDRMID(MMSDK_U16 *pu16CusDRMID);

//-------------------------------------------------------------------------------------------------
/// Set System Memory Pool ID
/// @param gs32MstarCachedPoolID            \b IN: cache pool id
/// @param gs32MstarNonCachedPoolID      \b IN: non cache pool id
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetMemPoolID(MMSDK_S32 gs32MstarCachedPoolID, MMSDK_S32 gs32MstarNonCachedPoolID);

//-------------------------------------------------------------------------------------------------
/// Get Cache Pool ID
/// @param None
///
/// @return                     \b OUT: cache pool id
//-------------------------------------------------------------------------------------------------
MMSDK_S32 PT_SYS_GetCachePoolID(void);

//-------------------------------------------------------------------------------------------------
/// Get Non Cache Pool ID
/// @param None
///
/// @return                     \b OUT: non cache pool id
//-------------------------------------------------------------------------------------------------
MMSDK_S32 PT_SYS_GetNonCachePoolID(void);

//-------------------------------------------------------------------------------------------------
/// Get Non Cache Pool ID
/// @param None
///
/// @return                     \b OUT: non cache pool id
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetDBReadWrite(PT_SYS_ReadDB pReadCallback, PT_SYS_WriteDB pWriteCallback);

//-------------------------------------------------------------------------------------------------
/// Get Non Cache Pool ID
/// @param None
///
/// @return                     \b OUT: non cache pool id
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_ReadFromDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo);

//-------------------------------------------------------------------------------------------------
/// Get Non Cache Pool ID
/// @param None
///
/// @return                     \b OUT: non cache pool id
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_WriteToDB(ST_MMSDK_DIVXDRM_INFO *pstDRMInfo);

//-------------------------------------------------------------------------------------------------
/// Set MMAP Info
/// @param stMmapInfo            \b IN: mm mmap info
/// @param stMIUInfo               \b IN: mm miu info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetMmapInfo(PT_SYS_MmapInfo *stMmapInfo, ST_MMSDK_MMAP_INFO *stMIUInfo);

//-------------------------------------------------------------------------------------------------
/// Get MMAP Info
/// @param pstMemInfo       \b OUT: buffer structure info
/// @param eBufferID          \b IN: mmap buffer type
/// @param eInfo                \b IN: buf info menber type(PA/ VA/ SIZE/ IsMIU0)
///
/// @return                          \b OUT: buffer info
//-------------------------------------------------------------------------------------------------
MMSDK_U32 PT_SYS_GetMmapInfo(ST_MMSDK_BUF_INFO* pstMemInfo, EN_MMSDK_BUF_ID eBufferID, EN_MMSDK_BUF_INFO eInfo);


//-------------------------------------------------------------------------------------------------
/// Get MIU Info
/// @param pInfo            \b OUT: miu info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL  PT_SYS_GetMIUInfo(ST_MMSDK_MMAP_INFO* pInfo);

//-------------------------------------------------------------------------------------------------
/// Set TSP VQ Info
/// @param u32VQBuffer            \b IN: TSP VQ buffer
/// @param u32VQSize            \b IN: TSP VQ length
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetTspVQInfo(MMSDK_U32 u32VQBuffer, MMSDK_U32 u32VQSize);

//-------------------------------------------------------------------------------------------------
/// Get TSP VQ Info
/// @param u32VQBuffer            \b OUT: TSP VQ buffer
/// @param u32VQSize            \b OUT: TSP VQ length
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_GetTspVQInfo(MMSDK_U32 *u32VQBuffer, MMSDK_U32 *u32VQSize);

//-------------------------------------------------------------------------------------------------
/// Set Audio Decoder ID
/// @param u32VQBuffer            \b IN: Audio Decoder ID
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetAudDecID(AUDIO_DEC_ID eAudDecID);

//-------------------------------------------------------------------------------------------------
/// Get Audio Decoder ID
/// @param u32VQBuffer            \b IN: Audio Decoder ID
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_GetAudDecID(AUDIO_DEC_ID *peAudDecID);

//-------------------------------------------------------------------------------------------------
/// Enable display photo flow in MMSDK
/// @param bEnable      \b IN: TRUE: Enable MMSDK photo display flow, FALSE:Disable MMSDK photo display flow

/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetGraphicDisplayFlag(MMSDK_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get Flag of  MMSDK photo display flow

/// @return TRUE: MMSDK photo display flow is enable
/// @return FALSE: MMSDK photo display flow is disable
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_GetGraphicDisplayFlag(void);

//-------------------------------------------------------------------------------------------------
/// Initialize AVP window's Z-Order

/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_InitAVPZOrder(void);

//-------------------------------------------------------------------------------------------------
/// Set AVP window's Z-Order
/// @param u8WinID      \b IN: The AVP window index.
/// @param u8Order      \b IN: The layer of the window.

/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetAVPZOrder(MMSDK_U8 u8WinID, MMSDK_U8 u8Order);

//-------------------------------------------------------------------------------------------------
/// Get AVP window's Z-Order

/// @return                          \b OUT: The layer of the window.
//-------------------------------------------------------------------------------------------------
MMSDK_U8 PT_SYS_GetAVPZOrder(MMSDK_U8 u8WinID);

//-------------------------------------------------------------------------------------------------
/// Set draw photo output path
/// @param u8Path      \b IN: 0 is draw photo by GOP path; 1 is draw photo by MVOP path

/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetMMPhotoPath(MMSDK_U8 u8Path);

//-------------------------------------------------------------------------------------------------
/// Get draw photo output path
///
/// @return                          \b OUT: draw photo output path. 0 is draw photo by GOP path; 1 is draw photo by MVOP path
//-------------------------------------------------------------------------------------------------
MMSDK_U8 PT_SYS_GetMMPhotoPath(void);

//-------------------------------------------------------------------------------------------------
/// Set Function Pointer for customerize MM function.
/// @param pstCusFunPtr            \b IN: struct of function pointer from upper layer

/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetCustomerizeFunPtr(PT_SYS_CUS_FunPtr *pstCusFunPtr);

//-------------------------------------------------------------------------------------------------
/// Set Function Pointer for customerize subtitle render function.
/// @param pstSubRndFunPtr        \b IN: struct of function pointer from upper layer

/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_SYS_SetSubtitleRndFunPtr(PT_SYS_SubRnd_FunPtr *pstSubRndFunPtr);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__porting_appinfo__
///*** please do not remove change list tag ***///
///***$Change: 694245 $***///
