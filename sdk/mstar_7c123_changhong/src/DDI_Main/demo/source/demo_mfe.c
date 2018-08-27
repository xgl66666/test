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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_MFE.h
/// @author MStar Semiconductor Inc.
/// @brief  MFE Function Sample Demo
///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_MFE_TEST == 1)
#include <linux/string.h>
#include <sys/stat.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"
#include "MsFS.h"
#include "utopia.h"

#include "drvMVOP.h"
#include "drv_mfe_st.h"
#include "apiVOIPMFE.h"
#include "apiMFE_v2.h"

#include "apiXC.h" // this is a implicit inclusion needed by "mapi_video_base.h"
#include "apiXC_DWIN.h"
#include "apiXC_DWIN_EX.h"
#include "apiVDEC_EX.h"
#include "apiHDMITx.h"

#include "demo_mfe.h"
#include "demo_utility.h"

#include "msAPI_XC_EX.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define CEILING_ALIGN(value, align)       ((((value)+(align)-1)/(align))*(align))
#define BUFFER_ALIGNMENT (1024) // bytes

#define IN_SIZE (CEILING_ALIGN(1920 * 1088 * 2, BUFFER_ALIGNMENT)) // YUV422 @ FHD, aligned to 1024 bytes
#define MFE_SIZE (0x800400)    // FHD  2 * Frame buffer size ( 1 reconstruct frame and 1 reference frame) + out buffer size + HW internal buffer size

#define RESET_BITRATE 0
#define RESET_FRAMERATE 0

#define MIU_OFFSET 0
#define MFE_DBG 0

#define INVALID_TASK_ID     -1

#if MFE_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

static ST_MFE_VENC stVenc_info[E_MFE_DEVICE_MAX];
static ST_MFE_Controller stVencController = {NULL,E_MFE_NotInital,{-1, E_TASK_PRI_HIGH, NULL, 0, "VencController"},-1};

//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------
MS_BOOL ConvertYV12ToHVDTile(unsigned char *pSrc1, unsigned char *pSrc2, int nSrcWidth, int nSrcHeight, int nSrcStride, unsigned char *pDst, int nDstSize, int YPlane);

//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------
static void _Demo_MFE_CleanBufStatus(MS_U32 u32FrameCoded,MS_U32 u32YUVPlane)
{

}

static void _Demo_MFE_CallbackFunc(void *pCtx, MS_S32 s32FrameType, void *pStreamData, MS_U32 u32DataLen, MS_U32 bFirstOrLastByteOfFrame)
{
    ST_MFE_CTX *ptmp = pCtx;
    ptmp->s32Frame_type = s32FrameType;
    ptmp->u32Frame_size = u32DataLen;

    // s32FrameType : I_VOP, P_VOP, or B_VOP
    db_print("u32Index[%d] FrameType[%d] u32DataLen[%d]\n",ptmp->u32Index,s32FrameType,u32DataLen);
}

static void _Demo_MFE_DIP_FrameDoneCallback(void)
{
    MS_U16 IntStatus = MApi_XC_DIP_GetIntStatus(DIP_WINDOW);
    MApi_XC_DIP_ClearInt(IntStatus, DIP_WINDOW);
    MsOS_SetEvent (stVencController.MFE_DIPEventid, IntStatus);
}

static MS_U32 _Demo_MFE_DMS_DIPWHeightAlign(MFE_COLOR_FORMAT_INFO eFmt,MS_U32 u32Height)
{
    MS_U32 u32Align_Height = u32Height;

    switch(eFmt)
    {
        case MSTTILE:
        case MFE_32x32TILE:
            u32Align_Height = CEILING_ALIGN(u32Height,64);
            break;
        case MFE_EVDTILE:
            u32Align_Height = CEILING_ALIGN(u32Height,32);
            break;
        case MFE_YUYV:
        case MFE_YVYU:
        case MFE_UYVY:
        case MFE_VYUY:
        case MFE_NV12:
        case MFE_NV21:
        default:
            break;
    }
    db_print("\033[35m =======[%s][%d] u32Align_Height [%d]============= \033[0m\n",__FUNCTION__,__LINE__,u32Align_Height);
    return u32Align_Height;
}

static MS_BOOL _Demo_MFE_FreeMemory(MS_U8 **pu8Buffer)
{
    if (*pu8Buffer)
    {
        MS_BOOL bRet;
        MS_S32 s32MstarNonCachedPoolID = 0;

        Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

        bRet = MsOS_FreeMemory((void *)*pu8Buffer, s32MstarNonCachedPoolID);
        if(FALSE == bRet)
        {
            printf("[%s]-Error : MsOS_FreeMemory fail\n", __func__);
            return FALSE;
        }
        *pu8Buffer = 0;
    }
    return TRUE;
}

static MS_BOOL _Demo_MFE_VENC_Init(MS_U32 u32Index)
{
    MS_U32 u32Gop = 0;
    MS_S32 s32MstarNonCachedPoolID = 0;
    void *pAttribute = NULL;

    if(stVencController.eMFEStatus == E_MFE_NotInital)
    {
        printf("\033[35m[%s][%d]\033[0m\n",__FUNCTION__,__LINE__);
        if (MFE_Open(&stVencController.pInstanceMFE, 0, pAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MFE_Open() failed.\n");
            return FALSE;
        }
        stVencController.eMFEStatus = E_MFE_Inital;
    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    stVenc_info[u32Index].stVENCInfo.encode_info = MsOS_AllocateMemory(sizeof(PVR_Info), s32MstarNonCachedPoolID);
    if(stVenc_info[u32Index].stVENCInfo.encode_info == NULL)
    {
        printf("[%s]-Error : allocate stVenc_info[*pu32Index].stVENCInfo.encode_info fail\n", __func__);
        return FALSE;
    }

    stVenc_info[u32Index].stVENCInfo.g_compress_ctx = MsOS_AllocateMemory(sizeof(ST_MFE_CTX), s32MstarNonCachedPoolID);
    if(stVenc_info[u32Index].stVENCInfo.g_compress_ctx == NULL)
    {
        printf("[%s]-Error : allocate stVenc_info[*pu32Index].stVENCInfo.g_compress_ctx fail\n", __func__);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info);
        return FALSE;
    }

    memset(stVenc_info[u32Index].stVENCInfo.encode_info,0,sizeof(PVR_Info));

    //set parameter
    stVenc_info[u32Index].stVENCInfo.encode_info->width = stVenc_info[u32Index].u32Width;
    stVenc_info[u32Index].stVENCInfo.encode_info->height = _Demo_MFE_DMS_DIPWHeightAlign(stVenc_info[u32Index].eColorFormat,stVenc_info[u32Index].u32Height);
    stVenc_info[u32Index].stVENCInfo.encode_info->nDispWidth= stVenc_info[u32Index].u32Width;
    stVenc_info[u32Index].stVENCInfo.encode_info->nDispHeight= _Demo_MFE_DMS_DIPWHeightAlign(stVenc_info[u32Index].eColorFormat,stVenc_info[u32Index].u32Height);

    stVenc_info[u32Index].stVENCInfo.encode_info->nBufWidth = CEILING_ALIGN(stVenc_info[u32Index].stVENCInfo.encode_info->width,16);
    stVenc_info[u32Index].stVENCInfo.encode_info->nBufHeight= _Demo_MFE_DMS_DIPWHeightAlign(stVenc_info[u32Index].eColorFormat,stVenc_info[u32Index].u32Height);

    stVenc_info[u32Index].stVENCInfo.encode_info->MfeCropInfo.crop_en = TRUE;
    stVenc_info[u32Index].stVENCInfo.encode_info->MfeCropInfo.crop_bottom = ((stVenc_info[u32Index].stVENCInfo.encode_info->height) - stVenc_info[u32Index].u32Height);

    stVenc_info[u32Index].stVENCInfo.encode_info->MfeApiInfo.miu_offset = stVenc_info[u32Index].u32MiuOffset;
    stVenc_info[u32Index].stVENCInfo.encode_info->FrameRatex100 = stVenc_info[u32Index].u32FrameRatex100;
    stVenc_info[u32Index].stVENCInfo.encode_info->nBitrate= stVenc_info[u32Index].u32Bitrate;

    u32Gop = (stVenc_info[u32Index].u32FrameRatex100 / 100) * 2;

    if(u32Gop == 0)
    {
        stVenc_info[u32Index].stVENCInfo.encode_info->nBbetweenP = 0;
        stVenc_info[u32Index].stVENCInfo.encode_info->nPbetweenI = u32Gop;
        stVenc_info[u32Index].stVENCInfo.encode_info->nP_is_infinite = 1;
    } else {
        stVenc_info[u32Index].stVENCInfo.encode_info->nBbetweenP = 0;
        stVenc_info[u32Index].stVENCInfo.encode_info->nPbetweenI = u32Gop - 1;
        stVenc_info[u32Index].stVENCInfo.encode_info->nP_is_infinite = 0;
    }

    //default
    stVenc_info[u32Index].stVENCInfo.encode_info->nCodecType = H264_ENC;
    stVenc_info[u32Index].stVENCInfo.encode_info->m_bFrameMode = 1;
    stVenc_info[u32Index].stVENCInfo.encode_info->quality = 70;
    stVenc_info[u32Index].stVENCInfo.encode_info->fdc_mode = 0;
    stVenc_info[u32Index].stVENCInfo.encode_info->MFEClk = MFE_CLK_FAST;
    stVenc_info[u32Index].stVENCInfo.encode_info->bInterlace = PROGRESSIVE;
    stVenc_info[u32Index].stVENCInfo.encode_info->enableISR = 1;
    stVenc_info[u32Index].stVENCInfo.encode_info->bColorFormat = stVenc_info[u32Index].eColorFormat;
    stVenc_info[u32Index].stVENCInfo.encode_info->enableSecurityMode = 0;

    //output vir buffer
    stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer = MsOS_AllocateMemory((1024*1024*2), s32MstarNonCachedPoolID);
    stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuAddress = MsOS_VA2PA((MS_VIRT)stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer);
    stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.size = 0;
    stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBufferSize = 1024*1024*2; //out buffer size

    if(stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer == NULL)
    {
        printf("[%s]-Error : allocate stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer fail\n", __func__);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.g_compress_ctx);
        return FALSE;
    }

    //mfe phy buffer
    stVenc_info[u32Index].stVENCInfo.encode_info->MfeApiInfo.mfe_phy_buf_addr = stVenc_info[u32Index].stMFEBufAddr.u32Pa;
    stVenc_info[u32Index].stVENCInfo.encode_info->MfeApiInfo.mfe_vir_buf_ptr = (MS_VIRT)((MS_U8*)((MS_U32)stVenc_info[u32Index].stMFEBufAddr.pu8Ptr));
    stVenc_info[u32Index].stVENCInfo.encode_info->MfeApiInfo.mfe_phy_buf_size = stVenc_info[u32Index].stMFEBufAddr.u32Size;

    db_print("[%s] : width = %d ; height = %d ; FrameRatex100 = %d ; nBitrate = %d ; u32Gop = %d\n",
    __func__, (int)stVenc_info[u32Index].stVENCInfo.encode_info->width, (int)stVenc_info[u32Index].stVENCInfo.encode_info->height, (int)stVenc_info[u32Index].stVENCInfo.encode_info->FrameRatex100, (int)stVenc_info[u32Index].stVENCInfo.encode_info->nBitrate, (int)u32Gop);


    stVenc_info[u32Index].stVENCInfo.g_compress_ctx->u32Index = u32Index;

    stVenc_info[u32Index].stVENCInfo.encode_info->g_pCustomCtx = (void *)stVenc_info[u32Index].stVENCInfo.g_compress_ctx;
    stVenc_info[u32Index].stVENCInfo.encode_info->g_StreamCallBackFunc = (StreamCallback)_Demo_MFE_CallbackFunc;
    stVenc_info[u32Index].stVENCInfo.encode_info->g_CleanCallBackFunc = (CleanCallback)_Demo_MFE_CleanBufStatus;

#if (DEMO_MFE_IMI_TEST == 1)
    if(stVenc_info[u32Index].u32InitMode == 1)
    {
        stVenc_info[u32Index].stVENCInfo.encode_info->MfeAdvInfo.low_bandwidth_en = 1;
        stVenc_info[u32Index].stVENCInfo.encode_info->MfeAdvInfo.input_imi_en = 1;
    }
#endif

    MFE_INIT_PARAM stInitPar;
    stInitPar.mfe_Info = stVenc_info[u32Index].stVENCInfo.encode_info;

    if(MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_Init, &stInitPar) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_CMD_MFE_Init() failed.\n");

        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.g_compress_ctx);
        return FALSE;
    }

    if(MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_Encode, &stInitPar) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_CMD_MFE_Encode() failed.\n");

        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info);
        _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.g_compress_ctx);
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_MFE_VENC_GetVol(MS_U32 u32Index,ST_MFE_MEMADDR* pstSPSPPS)
{
    SPSPPS_INFO_t stSPSPPS_Info;

    MFE_GETVOL_PARAM stGetvolPar;
    stGetvolPar.mfe_Info = stVenc_info[u32Index].stVENCInfo.encode_info;
    stGetvolPar.header_info = &stSPSPPS_Info;

    MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_GetVOL, &stGetvolPar);

    pstSPSPPS->pu8Ptr = stSPSPPS_Info.pHeaderSPS;
    pstSPSPPS->u32Size = stSPSPPS_Info.SPSLen + stSPSPPS_Info.PPSLen;
    return TRUE;
}

static MS_BOOL _Demo_MFE_VENC_ONEFRAME(MS_U32 u32Index,ST_MFE_MEMADDR_CUR stCur,MS_BOOL bIsForceI)
{
    MS_BOOL bRet = FALSE;

    MEMMAP_CUR_t stCurframe;
    stCurframe.Cur_Y0.miuAddress = stCur.stCurY.u32Pa;
    stCurframe.Cur_C0.miuAddress = stCur.stCurUV.u32Pa;

    MFE_COMPRESSONEPICTURE_PARAM stCompPar;
    stCompPar.mfe_Info = stVenc_info[u32Index].stVENCInfo.encode_info;
    stCompPar.YUVPlane = stCurframe;
    stCompPar.bForceIframe = bIsForceI;

    if(MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_CompressOnePicture, &stCompPar) == UTOPIA_STATUS_SUCCESS)
        bRet = TRUE;

    return bRet;
}

static MS_BOOL _Demo_MFE_VENC_GETBITS(MS_U32 u32Index,ST_MFE_MEMADDR* pstOutBuf)
{
    MEMMAP_t stOutBuf;

    MFE_GET_OBUF_PARAM stOutBufPar;
    stOutBufPar.mfe_Info = stVenc_info[u32Index].stVENCInfo.encode_info;
    stOutBufPar.outbuf = &stOutBuf;

    MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_GetOutBuffer, &stOutBufPar);

    pstOutBuf->pu8Ptr = stOutBuf.miuPointer;
    pstOutBuf->u32Pa = stOutBuf.miuAddress;
    pstOutBuf->u32Size = stOutBuf.size;

    return TRUE;
}

static MS_BOOL _Demo_MFE_VENC_Deinit(MS_U32 u32Index)
{
    MFE_INIT_PARAM stInitPar;
    stInitPar.mfe_Info = stVenc_info[u32Index].stVENCInfo.encode_info;

    MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_DeInit, &stInitPar);

    _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info->BitstreamOutBuffer.miuPointer);
    _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.encode_info);
    _Demo_MFE_FreeMemory((MS_U8**)&stVenc_info[u32Index].stVENCInfo.g_compress_ctx);

    stVenc_info[u32Index].bInited = FALSE;
    stVenc_info[u32Index].stVENCStatus = E_MFE_NotInital;

    int i = 0;
    for(i= 0; i<E_MFE_DEVICE_MAX; i++)
    {
        if(stVenc_info[i].bInited == TRUE)
        {
            return TRUE;
        }
    }

    printf("MFE_Close()!!!!!!!!!!!\n");
    if (MFE_Close(stVencController.pInstanceMFE) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s]MFE_Close fail\n", __func__);
        return FALSE;
    }
    stVencController.pInstanceMFE = NULL;
    stVencController.eMFEStatus = E_MFE_Stop;

    return TRUE;
}

#if (RESET_BITRATE || RESET_FRAMERATE)
static MS_BOOL _Demo_MFE_VENC_Change_BitRateAndFrameRate(MS_U32 u32Index)
{
    stVenc_info[u32Index].stVENCInfo.encode_info->FrameRatex100 = stVenc_info[u32Index].u32FrameRatex100;
    stVenc_info[u32Index].stVENCInfo.encode_info->nBitrate= stVenc_info[u32Index].u32Bitrate;

    MFE_INIT_PARAM stInitPar;
    stInitPar.mfe_Info = stVenc_info[u32Index].stVENCInfo.encode_info;

    MFE_Ioctl(stVencController.pInstanceMFE, MApi_CMD_MFE_SetBitrateFramerate, &stInitPar);

    return TRUE;
}
#endif

static MS_BOOL _Demo_MFE_STOP(MS_U32 u32Index,EN_MFE_INPUTSOURCE eInputSource)
{
    _Demo_MFE_VENC_Deinit(u32Index);

    MsFS_Sync();
    MsFS_Fflush(stVenc_info[u32Index].stFile.out_f);
    MsFS_Fclose(stVenc_info[u32Index].stFile.out_f);
    printf("[%s][%d] Close the file\n",__FUNCTION__,__LINE__);

    if(eInputSource == E_MFE_INPUTSOURCE_FILE)
    {
        MsFS_Fclose(stVenc_info[u32Index].stFile.in_f);
    }

    if((eInputSource == E_MFE_INPUTSOURCE_DIP) && (stVencController.eMFEStatus != E_MFE_Start))
    {
        MApi_XC_DIP_Ena(FALSE,DIP_WINDOW);
        MApi_XC_DIP_InterruptDetach(DIP_WINDOW);
    }

    if(eInputSource == E_MFE_INPUTSOURCE_DMS)
    {
    #if (DEMO_DMS_TEST == 1)
        ST_MSAPI_DIP_MULTIVIEW_SET_CAPTURE_INFO stMsapi_CAPInfo;

        memset(&stMsapi_CAPInfo,0,sizeof(ST_MSAPI_DIP_MULTIVIEW_SET_CAPTURE_INFO));
        stMsapi_CAPInfo.u32WindowID = stVenc_info[u32Index].stSetCaptureInfo.u32Window;
        stMsapi_CAPInfo.u32Enable   = stVenc_info[u32Index].stSetCaptureInfo.u32CaptureEnable;

        msAPI_XC_DIPMultiView_Set_CaptureInfo(&stMsapi_CAPInfo);

        stVenc_info[u32Index].stSetCaptureInfo.bCaptureInited = FALSE;

    #if (DEMO_MFE_IMI_TEST == 0)
        _Demo_MFE_FreeMemory(&stVenc_info[u32Index].stSetCaptureInfo.pu8BufferAddr);
    #endif
    #endif
    }

    _Demo_MFE_FreeMemory(&stVenc_info[u32Index].pu8VENCBufOri);

    return TRUE;
}

#if (DEMO_DMS_TEST == 1)
MS_BOOL _Demo_MFE_DMS_Release_CaptureBuffer(MS_U32 u32Index)
{
    ST_MSAPI_DIP_MULTIVIEW_CAPTURE_INFO stReleaseCapInfo;

    stReleaseCapInfo.phyBufferYAddr = stVenc_info[u32Index].stCaptureInfo.phyBufferYAddr;
    stReleaseCapInfo.phyBufferCAddr = stVenc_info[u32Index].stCaptureInfo.phyBufferCAddr;
    stReleaseCapInfo.u64Pts         = stVenc_info[u32Index].stCaptureInfo.u64Pts;

    msAPI_XC_DIPMultiView_Release_CaptureBuffer(stVenc_info[u32Index].stSetCaptureInfo.u32Window,&stReleaseCapInfo);

    //IMI case release frame : let DIP enable when mfe capture done
    return TRUE;
}

MS_BOOL _Demo_MFE_DMS_Get_CaptureBuffer(MS_U32 u32Index)
{
    MS_BOOL bNewFrame = FALSE;
    MS_U32 u32Timeout = MsOS_GetSystemTime();

    while(bNewFrame != TRUE)
    {
        ST_MSAPI_DIP_MULTIVIEW_CAPTURE_INFO stGetCapInfo;
        memset(&stGetCapInfo,0,sizeof(ST_MSAPI_DIP_MULTIVIEW_CAPTURE_INFO));
        if(msAPI_XC_DIPMultiView_Get_CaptureBuffer(stVenc_info[u32Index].stSetCaptureInfo.u32Window,&stGetCapInfo) == E_MSAPI_XC_OK)
        {
            bNewFrame= TRUE;
            stVenc_info[u32Index].stCaptureInfo.phyBufferYAddr = stGetCapInfo.phyBufferYAddr;
            stVenc_info[u32Index].stCaptureInfo.phyBufferCAddr = stGetCapInfo.phyBufferCAddr;
            stVenc_info[u32Index].stCaptureInfo.u64Pts         = stGetCapInfo.u64Pts;
            stVenc_info[u32Index].stCaptureInfo.u32frameCount  = stGetCapInfo.u32frameCount;
        }
        else
        {
            if(MsOS_GetSystemTime()- u32Timeout > 10)
            {
                db_print("\033[37m[%s][%d] timeout !!!! u32Index[%d] \033[0m\n",__FUNCTION__,__LINE__,u32Index);
                return FALSE;
            }
        }
    }
    return TRUE;
}

EN_MSAPI_DMS_CAPTURE_COLORFORMAT _Demo_MFE_DMS_FmtMappping(MFE_COLOR_FORMAT_INFO eFmt)
{
    EN_MSAPI_DMS_CAPTURE_COLORFORMAT eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_16X32TILE;
    switch(eFmt)
    {
        case MSTTILE:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_16X32TILE;
            break;
        case MFE_YUYV:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_YUYV;
            break;
        case MFE_YVYU:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_YVYU;
            break;
        case MFE_UYVY:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_UYVY;
            break;
        case MFE_VYUY:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_VYUY;
            break;
        case MFE_EVDTILE:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_32X16TILE;
            break;
        case MFE_32x32TILE:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_32X32TILE;
            break;
        case MFE_NV12:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_NV12;
            break;
        case MFE_NV21:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_NV21;
            break;
        default:
            eMsapiFmt = E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_16X32TILE;
            break;
    }
    printf("\033[35m =======[%s][%d] eMsapiFmt [%d]============= \033[0m\n",__FUNCTION__,__LINE__,eMsapiFmt);

    return eMsapiFmt;
}

static void _Demo_MFE_DMS_EncodeTask(void)
{
    MS_BOOL bForceI = FALSE;
    MS_BOOL bRet = FALSE;

    while(stVencController.eMFEStatus != E_MFE_Stop)
    {
        int i = 0;
        for(i = 0 ; i < E_MFE_DEVICE_MAX ; i++)
        {
            if(stVenc_info[i].stVENCStatus == E_MFE_Start)
            {
                {
                    bRet = _Demo_MFE_DMS_Get_CaptureBuffer((MS_U32)i);
                    if(bRet == FALSE)
                    {
                        if((stVenc_info[i].stSetCaptureInfo.bCaptureInited == TRUE) && (stVenc_info[i].stSetCaptureInfo.u32CaptureEnable == FALSE))
                        {
                            _Demo_MFE_STOP(i,E_MFE_INPUTSOURCE_DMS);
                        }
                        continue;
                    }

                    ST_MFE_MEMADDR_CUR stCur;
                    memset(&stCur,0,sizeof(ST_MFE_MEMADDR_CUR));
                    stCur.stCurY.u32Pa  = stVenc_info[i].stCaptureInfo.phyBufferYAddr;//DIP_buffer;
                    stCur.stCurUV.u32Pa = stVenc_info[i].stCaptureInfo.phyBufferCAddr;

                    {
                        if (FALSE == _Demo_MFE_VENC_ONEFRAME(i,stCur, bForceI))
                        {
                            printf("\033[35m[%s][%d] index [%d] Failed to encode frame %d.\033[0m\n",__FUNCTION__,__LINE__,i,stVenc_info[i].u32FrameNum);
                            break;
                        }
                        stVenc_info[i].u32FrameNum++;
                        db_print("\033[35m[%s][%d] [SYS: %d] index [%d] [Y:0x%x, C:0x%x PTS:%lld framecnt:%d] encode frame %d.\033[0m\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime(),i,stCur.stCurY.u32Pa,stCur.stCurUV.u32Pa,stVenc_info[i].stCaptureInfo.u64Pts,stVenc_info[i].stCaptureInfo.u32frameCount,stVenc_info[i].u32FrameNum);
                    }
                    _Demo_MFE_DMS_Release_CaptureBuffer((MS_U32)i);
                }

                {
                    ST_MFE_MEMADDR stOutBuf;
                    _Demo_MFE_VENC_GETBITS(i,&stOutBuf);
                    MsFS_Fwrite(stOutBuf.pu8Ptr,1,stOutBuf.u32Size,stVenc_info[i].stFile.out_f);
                    MsFS_Fflush(stVenc_info[i].stFile.out_f);
                }

                if((stVenc_info[i].stSetCaptureInfo.bCaptureInited == TRUE) && (stVenc_info[i].stSetCaptureInfo.u32CaptureEnable == FALSE))
                {
                    _Demo_MFE_STOP(i,E_MFE_INPUTSOURCE_DMS);
                }
            }
        }
        //MsOS_DelayTask(1);
    }

    stVencController.stMFETask.iId = INVALID_TASK_ID;
    stVencController.eMFEStatus = E_MFE_NotInital;

    printf("\033[35m[%s][%d]MFE encode finish\033[0m\n",__FUNCTION__,__LINE__);

}

#if (DEMO_MFE_IMI_TEST == 0)
static MS_U32 _Demo_MFE_DMS_CalcPitch(EN_MSAPI_DMS_CAPTURE_COLORFORMAT eFmt, MS_U32 u32Width)
{
    MS_U32 u32Pitch = 0;

    switch (eFmt)
    {
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_16X32TILE:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_32X16TILE:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_32X32TILE:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_NV12:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_NV21:
            u32Pitch = u32Width * 3 / 2; //*1.5
            break;
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_YUYV:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_YVYU:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_UYVY:
        case E_MSAPI_DMS_CAPTURE_COLOR_FORMAT_VYUY:
            u32Pitch = u32Width << 1;
            break;
        default :
            u32Pitch = 0;
            break;
    }
    return u32Pitch;
}
#endif
#endif

static void _Demo_MFE_DIP_EncodeTask(void)
{
    MS_BOOL bForceI = FALSE;
    MS_U32 u32Events = 0;
    MS_U32 u32WaitEventFlag = BIT(0);

    while(stVencController.eMFEStatus != E_MFE_Stop)
    {
        int i = 0;
        for(i = 0 ; i < E_MFE_DEVICE_MAX ; i++)
        {
            if(stVenc_info[i].stVENCStatus == E_MFE_Start)
            {
                if(stVenc_info[i].u32FrameNum == 0)
                {
                    _Demo_MFE_STOP(i,E_MFE_INPUTSOURCE_DIP);
                    continue;
                }

                MsOS_WaitEvent(stVencController.MFE_DIPEventid, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

                {
                    ST_MFE_MEMADDR_CUR stCur;
                    memset(&stCur,0,sizeof(ST_MFE_MEMADDR_CUR));

                    BUFFER_INFO stInfo;
                    stInfo = MApi_XC_DIP_GetBufInfo(DIP_WINDOW);
                    stCur.stCurY.u32Pa = stInfo.u32YBuf[0];//DIP_buffer;
                    stCur.stCurUV.u32Pa = stInfo.u32CBuf[0];//DIP_buffer;
                    {
                        if (FALSE == _Demo_MFE_VENC_ONEFRAME(i,stCur, bForceI))
                        {
                            printf("\033[35m[%s][%d]Failed to encode frame %d.\033[0m\n",__FUNCTION__,__LINE__,stVenc_info[i].u32FrameNum);
                            break;
                        }
                        db_print("\033[35m[%s][%d] [SYS: %d] index [%d] [0x%x, 0x%x,size:%d, %lld] encode frame %d.\033[0m\n",__FUNCTION__,__LINE__,MsOS_GetSystemTime(),i,stCur.stCurY.u32Pa,stCur.stCurUV.u32Pa,stVenc_info[i].u32Height*stVenc_info[i].u32Width, stVenc_info[i].stCaptureInfo.u64Pts,stVenc_info[i].u32FrameNum);
                    }
                }

                {
                    ST_MFE_MEMADDR stOutBuf;
                    _Demo_MFE_VENC_GETBITS(i,&stOutBuf);
                    MsFS_Fwrite(stOutBuf.pu8Ptr,1,stOutBuf.u32Size,stVenc_info[i].stFile.out_f);
                    MsFS_Fflush(stVenc_info[i].stFile.out_f);
                }

                stVenc_info[i].u32FrameNum--;
            }
        }

        MsOS_DelayTask(1);
    }

    stVencController.stMFETask.iId = INVALID_TASK_ID;
    stVencController.eMFEStatus = E_MFE_NotInital;

    printf("\033[35m[%s][%d]MFE encode finsih\033[0m\n",__FUNCTION__,__LINE__);

}

static void _Demo_MFE_FILE_EncodeTask(void)
{

#if (RESET_BITRATE || RESET_FRAMERATE)
    MS_U32 u32TempFrameNum[E_MFE_DEVICE_MAX] = {0};
#endif

    MS_BOOL bForceI = FALSE;
    MS_S32 s32MstarNonCachedPoolID = 0;
    MS_U8 *pu8BufStart = NULL;

    ST_MFE_MEMADDR stCurtmp;
    ST_MFE_MEMADDR stInaddr;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    stCurtmp.pu8Ptr = MsOS_AllocateMemory(IN_SIZE , s32MstarNonCachedPoolID);
    if (NULL == stCurtmp.pu8Ptr)
    {
        printf("Failed to allocate memory for cur_tmp.pu8Ptr.\n");
        return;
    }

    pu8BufStart = MsOS_AllocateMemory(IN_SIZE + BUFFER_ALIGNMENT , s32MstarNonCachedPoolID);
    if (pu8BufStart == NULL)
    {
        printf("[%s]-Error : MsOS_AllocateMemory fail\n", __func__);
        _Demo_MFE_FreeMemory(&stCurtmp.pu8Ptr);
        return;
    }
    stInaddr.pu8Ptr = (MS_U8*)(CEILING_ALIGN((MS_U32)pu8BufStart,BUFFER_ALIGNMENT));
    stInaddr.u32Pa = MsOS_VA2PA((MS_U32)stInaddr.pu8Ptr);

    while(stVencController.eMFEStatus != E_MFE_Stop)
    {
        int i = 0;
        for(i=0; i<E_MFE_DEVICE_MAX; i++)
        {
            if(stVenc_info[i].stVENCStatus == E_MFE_Start)
            {
                MS_U32 u32Width  = stVenc_info[i].u32Width;
                MS_U32 u32Height = stVenc_info[i].u32Height;
                MS_U32 u32InSize = u32Width*u32Height*3/2;

                if(stVenc_info[i].u32FrameNum == 0)
                {
                    _Demo_MFE_STOP(i,E_MFE_INPUTSOURCE_FILE);
                    continue;
                }

#if RESET_BITRATE
                if (stVenc_info[i].u32FrameNum == u32TempFrameNum[i]/2)
                {
                    stVenc_info[i].u32Bitrate = stVenc_info[i].u32Bitrate / 2;
                    db_print("bit rate change to %d\n",stVenc_info[i].u32Bitrate);
                    _Demo_MFE_VENC_Change_BitRateAndFrameRate(i);
                    db_print("bit rate change done\n");
                }
#endif

#if RESET_FRAMERATE
                if (stVenc_info[i].u32FrameNum == u32TempFrameNum[i]/2)
                {
                    stVenc_info[i].u32FrameRatex100 = stVenc_info[i].u32FrameRatex100 / 2;
                    db_print("frame rate change to %d\n",stVenc_info[i].u32FrameRatex100);
                    _Demo_MFE_VENC_Change_BitRateAndFrameRate(i);
                    db_print("frame rate change done\n");
                }
#endif
                while((u32InSize) != MsFS_Fread(stCurtmp.pu8Ptr,1,u32InSize,stVenc_info[i].stFile.in_f))
                {
                    printf("fseek\n");
                    MsFS_Fseek ( stVenc_info[i].stFile.in_f , 0 , SEEK_SET );
                }

                {
                    MS_U8* pu8BufY_src = stCurtmp.pu8Ptr;
                    MS_U8* pu8BufC_src = (MS_U8 *)((MS_U32)stCurtmp.pu8Ptr+u32Width*u32Height);
                    MS_U32 u32Stride_offset = u32Width;
                    MS_U32 u32DstSizeY = CEILING_ALIGN(CEILING_ALIGN(u32Height,32)*u32Stride_offset, BUFFER_ALIGNMENT);
                    MS_U32 u32DstSizeUV = CEILING_ALIGN(CEILING_ALIGN(u32Height,64)*u32Stride_offset/2, BUFFER_ALIGNMENT);

                    ConvertYV12ToHVDTile(pu8BufY_src, NULL, u32Stride_offset, u32Height, u32Stride_offset, stInaddr.pu8Ptr, u32DstSizeY,1);
                    ConvertYV12ToHVDTile(pu8BufC_src, pu8BufC_src+(u32Height/2)*(u32Stride_offset/2), u32Stride_offset>>1, u32Height>>1, u32Stride_offset/2, stInaddr.pu8Ptr+u32DstSizeY, u32DstSizeUV,0);
                }

                {
                    ST_MFE_MEMADDR_CUR stCur;
                    MS_U32 u32Ysize=0;

                    memset(&stCur,0,sizeof(ST_MFE_MEMADDR_CUR));

                    u32Ysize = CEILING_ALIGN(CEILING_ALIGN(u32Height,32)*u32Width, BUFFER_ALIGNMENT);

                    stCur.stCurY.u32Pa = stInaddr.u32Pa;
                    stCur.stCurUV.u32Pa = stInaddr.u32Pa + u32Ysize;
                    {
                        if (FALSE == _Demo_MFE_VENC_ONEFRAME(i,stCur, bForceI))
                        {
                            printf("\033[35m[%s][%d]Failed to encode frame %d.\033[0m\n",__FUNCTION__,__LINE__,stVenc_info[i].u32FrameNum);
                            break;
                        }
                        db_print("\033[35m[%s][%d]index [%d] encode frame %d.\033[0m\n",__FUNCTION__,__LINE__,i,stVenc_info[i].u32FrameNum);
                    }
                }

                {
                    ST_MFE_MEMADDR stOutBuf;
                    _Demo_MFE_VENC_GETBITS(i,&stOutBuf);
                    MsFS_Fwrite(stOutBuf.pu8Ptr,1,stOutBuf.u32Size,stVenc_info[i].stFile.out_f);
                    MsFS_Fflush(stVenc_info[i].stFile.out_f);
                }
            }
#if (RESET_BITRATE || RESET_FRAMERATE)
            u32TempFrameNum[i] = stVenc_info[i].u32FrameNum;
#endif
            stVenc_info[i].u32FrameNum--;
        }
    }

    _Demo_MFE_FreeMemory(&stCurtmp.pu8Ptr);
    _Demo_MFE_FreeMemory(&pu8BufStart);

    stVencController.stMFETask.iId = INVALID_TASK_ID;
    stVencController.eMFEStatus = E_MFE_NotInital;

    printf("\033[35m[%s][%d]MFE encode finsih\033[0m\n",__FUNCTION__,__LINE__);

}

static MS_BOOL _Demo_MFE_Controller(void (*pfTaskEntry)(void))
{
    if (stVencController.stMFETask.iId == INVALID_TASK_ID)
    {
        stVencController.eMFEStatus = E_MFE_Start;
        stVencController.stMFETask.iId = MsOS_CreateTask((TaskEntry)pfTaskEntry,
                                                        0,
                                                        stVencController.stMFETask.ePriority,
                                                        TRUE,
                                                        stVencController.stMFETask.pStack,
                                                        stVencController.stMFETask.u32StackSize,
                                                        stVencController.stMFETask.szName);

        if (stVencController.stMFETask.iId == INVALID_TASK_ID)
        {
            printf("[%s][%d] Create Task Failed  INVALID_TASK_ID \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            printf("%d \n",stVencController.stMFETask.iId);
        }
    }

    return TRUE;
}

MS_BOOL _Demo_MFE_Encode(MS_U32 u32Index,EN_MFE_INPUTSOURCE eInputSource)
{
    ST_MFE_MEMADDR stVol;
    _Demo_MFE_VENC_GetVol(u32Index,&stVol);

    MsFS_Fwrite(stVol.pu8Ptr,1,stVol.u32Size,stVenc_info[u32Index].stFile.out_f);
    MsFS_Fflush(stVenc_info[u32Index].stFile.out_f);

    stVenc_info[u32Index].stVENCStatus = E_MFE_Start;

    if(stVencController.eMFEStatus == E_MFE_Inital)
    {
        switch(eInputSource)
        {
            case E_MFE_INPUTSOURCE_FILE:
                    {
                        _Demo_MFE_Controller(_Demo_MFE_FILE_EncodeTask);
                        break;
                    }
            case E_MFE_INPUTSOURCE_DIP:
                    {
                        if(stVencController.MFE_DIPEventid < 0)
                        {
                            stVencController.MFE_DIPEventid = MsOS_CreateEventGroup("MFE_Event");
                        }

                        MApi_XC_DIP_EnaInt(0x2, FALSE, DIP_WINDOW);
                        MApi_XC_DIP_InterruptAttach((InterruptCb)_Demo_MFE_DIP_FrameDoneCallback, DIP_WINDOW);

                        _Demo_MFE_Controller(_Demo_MFE_DIP_EncodeTask);
                        break;
                    }

            case E_MFE_INPUTSOURCE_DMS:
                    #if (DEMO_DMS_TEST == 1)
                    {
                        _Demo_MFE_Controller(_Demo_MFE_DMS_EncodeTask);
                        break;
                    }
                    #endif
            case E_MFE_INPUTSOURCE_NONE:
            default:
            break;
        }
    }
    return TRUE;
}

MS_BOOL Demo_MFE_Init(const MS_U32 *pu32FrameWidth, const MS_U32 *pu32FrameHeight, const MS_U32 *pu32FrameRate, const MS_U32 *pu32BitRate)
{
    MS_U8  u8Codec = 1;  // 1:Avc 3:Hevc
    MS_U32 u32Index = 0;
    MS_U32 u32IndexMode = 0;
    return Demo_MFE_Init_EX(&u32Index,&u8Codec,pu32FrameWidth,pu32FrameHeight,pu32FrameRate,pu32BitRate,&u32IndexMode);
}

MS_BOOL Demo_MFE_Init_EX(const MS_U32 *pu32Index, const MS_U8 *pu8Codec,const MS_U32 *pu32FrameWidth, const MS_U32 *pu32FrameHeight, const MS_U32 *pu32FrameRate, const MS_U32 *pu32BitRate, const MS_U32 *pu32InitMode)
{
    MS_U8 *pu8BufStart = NULL;
    MS_U32 u32BufStartPa = 0;

    MS_U8 u8Codec = *pu8Codec;
    MS_U32 u32Width = *pu32FrameWidth;
    MS_U32 u32Height = *pu32FrameHeight;
    MS_U32 u32FrameRate = *pu32FrameRate;
    MS_U32 u32BitRate = *pu32BitRate;
    MS_U32 u32InitMode = *pu32InitMode;
    MS_S32 s32MstarNonCachedPoolID = 0;

    MFE_COLOR_FORMAT_INFO eTempColorFmt = stVenc_info[*pu32Index].eColorFormat;

    if(u8Codec != 1)
    {
        printf("[%s][%d] Not Support HEVC encode\n", __func__,__LINE__);
        return FALSE;
    }

    memset(&stVenc_info[*pu32Index],0,sizeof(ST_MFE_VENC));

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    stVenc_info[*pu32Index].pu8VENCBufOri = MsOS_AllocateMemory(MFE_SIZE + BUFFER_ALIGNMENT , s32MstarNonCachedPoolID);
    if (stVenc_info[*pu32Index].pu8VENCBufOri == NULL)
    {
        printf("[%s]-Error : MsOS_AllocateMemory fail\n", __func__);
        return FALSE;
    }

    pu8BufStart = (MS_U8*)(CEILING_ALIGN((MS_U32)stVenc_info[*pu32Index].pu8VENCBufOri,BUFFER_ALIGNMENT));

    u32BufStartPa = MsOS_VA2PA((MS_U32)pu8BufStart);
    if (u32BufStartPa == 0)
    {
        printf("[%s]-Error : MsOS_VA2PA fail\n", __func__);
        _Demo_MFE_FreeMemory(&stVenc_info[*pu32Index].pu8VENCBufOri);
        return FALSE;
    }

    stVenc_info[*pu32Index].u32Width    = u32Width;
    stVenc_info[*pu32Index].u32Height   = u32Height;
    stVenc_info[*pu32Index].u32FrameRatex100 = u32FrameRate * 100;
    stVenc_info[*pu32Index].u32Bitrate  = u32BitRate * 1024;
    stVenc_info[*pu32Index].u8Codec     = u8Codec;

    stVenc_info[*pu32Index].stMFEBufAddr.pu8Ptr = pu8BufStart;
    stVenc_info[*pu32Index].stMFEBufAddr.u32Pa  = u32BufStartPa;
    stVenc_info[*pu32Index].stMFEBufAddr.u32Size= MFE_SIZE;
    stVenc_info[*pu32Index].u32MiuOffset = MIU_OFFSET;
    stVenc_info[*pu32Index].eColorFormat = eTempColorFmt;
    stVenc_info[*pu32Index].u32InitMode  = u32InitMode;

    if(stVenc_info[*pu32Index].eColorFormat == 0)
        stVenc_info[*pu32Index].eColorFormat = MSTTILE;

    if (FALSE == _Demo_MFE_VENC_Init(*pu32Index))
    {
        printf("_Demo_MFE_VENC_Init() failed.\n");
        _Demo_MFE_FreeMemory(&stVenc_info[*pu32Index].pu8VENCBufOri);
        return FALSE;
    }

    stVenc_info[*pu32Index].bInited = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Encode FMT \n
/// @param[in] Encode index
/// @param[in] Encode Fmt
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
///          \b MFE_Encode_FMT_EX 0 0 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MFE_Encode_FMT_EX(MS_U32 *pu32Index,MS_U32 *pu32Fmt)
{
    printf("======================Demo_MFE_Encode_FMT [index] [Fmt]======================\n");
    printf("0 : YUVTILE         - YUV420 16x16\n");
    printf("1 : MSTTILE         - YUV420 16x32\n");
    printf("2 : MFE_YUYV        - YUV422\n");
    printf("3 : MFE_YVYU        - YUV422\n");
    printf("4 : MFE_UYVY        - YUV422\n");
    printf("5 : MFE_VYUY        - YUV422\n");
    printf("6 : MFE_EVDTILE     - YUV420 32x16\n");
    printf("7 : MFE_32x32TILE   - YUV420 32x32\n");
    printf("8 : MFE_NV12        - YUV420sp\n");
    printf("9 : MFE_NV21        - YUV420sp\n");
    printf("====================End Demo_MFE_Encode_FMT====================\n");

    stVenc_info[*pu32Index].eColorFormat = *pu32Fmt;

#if (DEMO_MFE_IMI_TEST == 1)
    if(stVenc_info[*pu32Index].eColorFormat != 1)
    {
        stVenc_info[*pu32Index].eColorFormat = 1;
        printf("[%s][%d] Only support encode MSTTILE Format\n",__FUNCTION__,__LINE__);
    }
#endif

    return TRUE;
}

MS_BOOL Demo_MFE_Encode_File(char *pInfile, char *pOutfile, const MS_U32 *pu32FrameNumber)
{
    MS_U32 u32Index = 0;
    return Demo_MFE_Encode_File_EX(&u32Index,pInfile,pOutfile,pu32FrameNumber);
}

MS_BOOL Demo_MFE_Encode_File_EX(MS_U32 *pu32Index,char *pInfile, char *pOutfile, const MS_U32 *pu32FrameNumber)
{
    char* pInFileName = pInfile;
    char* pOutFileName = pOutfile;

    if(stVenc_info[*pu32Index].bInited == FALSE)
    {
        printf("Not init index %d\n",*pu32Index);
        return FALSE;
    }

    stVenc_info[*pu32Index].u32FrameNum = *pu32FrameNumber;

    stVenc_info[*pu32Index].stFile.in_f = MsFS_Fopen(pInFileName, "rb");
    if(stVenc_info[*pu32Index].stFile.in_f == NULL)
    {
        printf("open in file fail\n");
        return FALSE;
    }

    stVenc_info[*pu32Index].stFile.out_f = MsFS_Fopen(pOutFileName, "wb");
    if(stVenc_info[*pu32Index].stFile.out_f == NULL)
    {
        MsFS_Fclose(stVenc_info[*pu32Index].stFile.in_f);
        printf("open out file fail\n");
        return FALSE;
    }

    _Demo_MFE_Encode(*pu32Index,E_MFE_INPUTSOURCE_FILE);

    return TRUE;
}

MS_BOOL Demo_MFE_Encode_From_DIP(char *pOutfile, const MS_U32 *pu32FrameNumber)
{
    MS_U32 u32Index = 0;
    return Demo_MFE_Encode_From_DIP_EX(&u32Index,pOutfile,pu32FrameNumber);
}

MS_BOOL Demo_MFE_Encode_From_DIP_EX(const MS_U32 *pu32Index,char *pOutfile,const MS_U32 *pu32FrameNumber)
{
    char* pOutFileName = pOutfile;

    if(stVenc_info[*pu32Index].bInited == FALSE)
    {
        printf("Not init index %d\n",*pu32Index);
        return FALSE;
    }

    stVenc_info[*pu32Index].u32FrameNum = *pu32FrameNumber;

    stVenc_info[*pu32Index].stFile.out_f = MsFS_Fopen(pOutFileName, "wb");
    if(stVenc_info[*pu32Index].stFile.out_f == NULL)
    {
        printf("open out file fail\n");
        return FALSE;
    }

    _Demo_MFE_Encode(*pu32Index,E_MFE_INPUTSOURCE_DIP);

    return TRUE;
}

#if (DEMO_DMS_TEST == 1)
MS_BOOL Demo_MFE_Encode_From_Capture_EX(const MS_U32 *pu32Index,char *pOutfile)
{
    char* pOutFileName = pOutfile;

    if(stVenc_info[*pu32Index].bInited == FALSE)
    {
        printf("Not init index %d\n",*pu32Index);
        return FALSE;
    }

    stVenc_info[*pu32Index].stFile.out_f = MsFS_Fopen(pOutFileName, "wb");
    if(stVenc_info[*pu32Index].stFile.out_f == NULL)
    {
        printf("open out file fail\n");
        return FALSE;
    }

    _Demo_MFE_Encode(*pu32Index,E_MFE_INPUTSOURCE_DMS);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set Capture info\n
/// @param[in] Encode index
/// @param[in] Capture window
/// @param[in] Capture Enable
/// @param[in] Capture window Visible
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:
///          \b MFE_Set_CaptureInfo 0 0 1 1 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_MFE_Set_CaptureInfo(MS_U32 *pu32Index,MS_U32 *u32Window,MS_U32 *pu32Enable,MS_U32 *pu32Visible)
{
    stVenc_info[*pu32Index].stSetCaptureInfo.u32Window        = *u32Window;
    stVenc_info[*pu32Index].stSetCaptureInfo.u32CaptureEnable = *pu32Enable;
    stVenc_info[*pu32Index].stSetCaptureInfo.u32WindowVisible = *pu32Visible;

    ST_MSAPI_DIP_MULTIVIEW_SET_CAPTURE_INFO stMsapiCaptureInfo;

    stMsapiCaptureInfo.u32Enable    = stVenc_info[*pu32Index].stSetCaptureInfo.u32CaptureEnable;
    stMsapiCaptureInfo.u32Visible   = stVenc_info[*pu32Index].stSetCaptureInfo.u32WindowVisible;
    stMsapiCaptureInfo.u32WindowID  = stVenc_info[*pu32Index].stSetCaptureInfo.u32Window;
    stMsapiCaptureInfo.u32Width     = stVenc_info[*pu32Index].u32Width;
    stMsapiCaptureInfo.u32Height    = _Demo_MFE_DMS_DIPWHeightAlign(stVenc_info[*pu32Index].eColorFormat,stVenc_info[*pu32Index].u32Height);
    stMsapiCaptureInfo.eFmt         = _Demo_MFE_DMS_FmtMappping(stVenc_info[*pu32Index].eColorFormat);
    stMsapiCaptureInfo.u32FrameRate = stVenc_info[*pu32Index].u32FrameRatex100*10;

    if(stVenc_info[*pu32Index].stSetCaptureInfo.u32CaptureEnable != FALSE)
    {

#if (DEMO_MFE_IMI_TEST == 0)
        if(stVenc_info[*pu32Index].stSetCaptureInfo.bCaptureInited == FALSE)
        {
            MS_U8 *pu8BufStart = NULL;
            MS_S32 s32MstarNonCachedPoolID = 0;

            Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);
            stVenc_info[*pu32Index].stSetCaptureInfo.phyBufferSize = (stMsapiCaptureInfo.u32Height*_Demo_MFE_DMS_CalcPitch(stMsapiCaptureInfo.eFmt,stMsapiCaptureInfo.u32Width))*3;
            stVenc_info[*pu32Index].stSetCaptureInfo.pu8BufferAddr = MsOS_AllocateMemory(stVenc_info[*pu32Index].stSetCaptureInfo.phyBufferSize + BUFFER_ALIGNMENT, s32MstarNonCachedPoolID);

            if (stVenc_info[*pu32Index].stSetCaptureInfo.pu8BufferAddr == NULL)
            {
                printf("[%s]-Error : MsOS_AllocateMemory fail\n", __func__);
                return FALSE;
            }
            pu8BufStart = (MS_U8*)(CEILING_ALIGN((MS_U32)stVenc_info[*pu32Index].stSetCaptureInfo.pu8BufferAddr,BUFFER_ALIGNMENT));
            stVenc_info[*pu32Index].stSetCaptureInfo.phyBufferAddr = MsOS_VA2PA((MS_U32)pu8BufStart);
        }
#endif

        stMsapiCaptureInfo.phyAddr = stVenc_info[*pu32Index].stSetCaptureInfo.phyBufferAddr;
        stMsapiCaptureInfo.phySize = stVenc_info[*pu32Index].stSetCaptureInfo.phyBufferSize;

        msAPI_XC_DIPMultiView_Set_CaptureInfo(&stMsapiCaptureInfo);

        stVenc_info[*pu32Index].stSetCaptureInfo.bCaptureInited = TRUE;
    }

    return TRUE;
}
#endif
#endif
