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
#include "MsCommon.h"
#include "MsMemory.h"
#include "drvSEM.h"
#include "MsOS.h"

#include "drv_mfe_st.h"
#include "apiVOIPMFE.h"

#include "drvXC_IOPort.h"
#include "apiXC_EX.h"
#include "apiXC_PCMonitor_EX.h"
#include "apiVDEC_EX.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "apiHDMITx.h"
#include "apiXC_Ace_EX.h"
#include "apiXC_DWIN_EX.h"
#include "apiXC.h" // this is a implicit inclusion needed by "mapi_video_base.h"
#include "apiXC_DWIN.h"

#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define CEILING_ALIGN(value, align) (((unsigned int)(value)+((align)-1)) & ~((align)-1))

#define IN_SIZE (CEILING_ALIGN(720,32)*1280+CEILING_ALIGN(720,64)*1280/2+0x100)
//#define IN_SIZE (CEILING_ALIGN(1080,32)*1920+CEILING_ALIGN(1080,64)*1920/2+0x100)

#define MFE_SIZE (0x400000)

#define RESET_BITRATE 0
#define RESET_FRAMERATE 0

#define MIU_OFFSET 0

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
typedef struct
{
    unsigned int pa;
    unsigned char* ptr;
}ST_MFE_MEMADDR;

typedef struct
{
    int fd;
    int framecnt;
    int framecnt_after_last_I;
    int current_buffer_index;
    int frame_type;
    int frame_size;
    void *streamData;
}ST_MFE_M4VECtx;

typedef struct
{
    MS_U32 pa;
    MS_U8* ptr;
    MS_U32 size;
}ST_MFE_MEMADDR_st;

typedef struct 
{
    MS_U32 Width; //16 align
    MS_U32 Height; //16 align
    MS_U32 FrameRatex100;
    MS_U32 nBitrate;
    MS_U32 Miu_offset;
    ST_MFE_MEMADDR_st mfe_buf;
}ST_MFE_Venc;

typedef enum
{
    VENC_I_FRM=0,
    VENC_P_FRM=1,
} EN_MFE_Venc_Frm_Type;

ST_MFE_Venc Venc_info;

ST_MFE_MEMADDR mfe_buf;
ST_MFE_MEMADDR in_addr;

PVR_Info *encode_info;
ST_MFE_M4VECtx *g_compress_ctx;

static MS_BOOL have_init = FALSE;

static MS_S32 MFE_Event_id = -1;

static MS_U32 *APVRBuffPAStart;
static MS_U32 *APVRBuffPAStart_orignal;
static MS_U32 APVRBuffPAStart_PA;

typedef MS_BOOL   MFE_BOOL;

MFE_BOOL ConvertYV12ToHVDTile(unsigned char *pSrc1, unsigned char *pSrc2, int nSrcWidth, int nSrcHeight, int nSrcStride, unsigned char *pDst, int nDstSize, int YPlane);

void M4VE_CallbackFunc(void *ctx, /*FrameType*/int type, void *streamData, unsigned long datalen, int bFirst_or_last_byte_of_Frame)
{
    ST_MFE_M4VECtx* tmp = ctx;
    tmp->frame_type = type;
    tmp->frame_size = datalen;
}

void VOIP_CleanBufStatus(MS_U32 nFrameCoded,MS_U32 YUVPlane)
{

}

MS_BOOL VENC_Init(void)
{
    MS_U32 Gop;

    encode_info = malloc(sizeof(PVR_Info));
    if(encode_info == NULL) {
        printf("[%s]-Error : malloc encode_info fail\n", __func__);
        return FALSE;
    }

    g_compress_ctx = malloc(sizeof(ST_MFE_M4VECtx));
    if(g_compress_ctx == NULL) {
        printf("[%s]-Error : malloc g_compress_ctx fail\n", __func__);
        return FALSE;
    }

    memset(encode_info,0,sizeof(PVR_Info));

    //set parameter
    encode_info->width = Venc_info.Width;
    encode_info->height = Venc_info.Height;
    encode_info->nDispWidth= Venc_info.Width;
    encode_info->nDispHeight= Venc_info.Height;

    encode_info->nBufWidth = encode_info->width/16*16  + (encode_info->width%16!=0 ? 16:0);
    encode_info->nBufHeight= encode_info->height/16*16 + (encode_info->height%16!=0 ? 16:0);

    encode_info->MfeApiInfo.miu_offset = Venc_info.Miu_offset;

    encode_info->FrameRatex100 = Venc_info.FrameRatex100;
    encode_info->nBitrate= Venc_info.nBitrate;

    Gop = (Venc_info.FrameRatex100 / 100) * 2;

    if(Gop == 0) {
        encode_info->nBbetweenP = 0;
        encode_info->nPbetweenI = Gop - 1;
        encode_info->nP_is_infinite = 1;
    } else {
        encode_info->nBbetweenP = 0;
        encode_info->nPbetweenI = Gop - 1;
        encode_info->nP_is_infinite = 0;
    }

    //default
    encode_info->nCodecType = VOIP_H264;
    encode_info->m_bFrameMode = 1;
    encode_info->quality = 70;
    encode_info->fdc_mode = 0;
    encode_info->MFEClk = MFE_CLK_FAST;
    encode_info->bInterlace = PROGRESSIVE;
    encode_info->enableISR = 1;
    encode_info->bColorFormat = MSTTILE;
    encode_info->enableSecurityMode = 0;

    //output vir buffer
    encode_info->BitstreamOutBuffer.miuPointer = (unsigned char *)malloc(1024*1024);
    encode_info->BitstreamOutBuffer.size = 0;
    encode_info->BitstreamOutBufferSize = 1024*1024;

    //mfe phy buffer
    encode_info->MfeApiInfo.mfe_phy_buf_addr = Venc_info.mfe_buf.pa;
    encode_info->MfeApiInfo.mfe_vir_buf_ptr = (unsigned int)Venc_info.mfe_buf.ptr;
    encode_info->MfeApiInfo.mfe_phy_buf_size = Venc_info.mfe_buf.size;

    printf("[%s] : width = %d ; height = %d ; FrameRatex100 = %d ; nBitrate = %d ; Gop = %d\n",
    __func__, encode_info->width, encode_info->height, encode_info->FrameRatex100, encode_info->nBitrate, (int)Gop);

    MApi_MFE_Initialize_ex(encode_info,(StreamCallback)M4VE_CallbackFunc, (CleanCallback)VOIP_CleanBufStatus, g_compress_ctx);

    MApi_MFE_Encode();

    return TRUE;
}


MS_BOOL VENC_GetVol(ST_MFE_MEMADDR_st* SPSPPS)
{
    SPSPPS_INFO_t spspps_info;
    MApi_MFE_GetVOL_ex(&spspps_info);
    SPSPPS->ptr = (MS_U8*) spspps_info.pHeaderSPS;
    SPSPPS->size = (MS_U32)(spspps_info.SPSLen + spspps_info.PPSLen);
    return TRUE;
}

MS_BOOL VENC_ENC_ONEFRAME(ST_MFE_MEMADDR_st CurY,ST_MFE_MEMADDR_st CurUV,MS_BOOL IsForceI)
{
    MEMMAP_CUR_t cur_frame;

    cur_frame.Cur_Y0.miuAddress = CurY.pa;
    cur_frame.Cur_C0.miuAddress = CurUV.pa;

    MApi_MFE_CompressOnePicture_ex(cur_frame , IsForceI);
    return TRUE;
}

MS_BOOL VENC_ENC_GETBITS(ST_MFE_MEMADDR_st* OutBuf,EN_MFE_Venc_Frm_Type* FrmType)
{
    MEMMAP_t outbuf;
    MApi_MFE_GetConfig(MFE_ENC_GETCONF_OUT_BSP_BUFFER,&outbuf);
    OutBuf->ptr = outbuf.miuPointer;
    OutBuf->size = outbuf.size;

    if(g_compress_ctx->frame_type == 0)
        *FrmType = VENC_I_FRM;
    else if(g_compress_ctx->frame_type == 1)
        *FrmType = VENC_P_FRM;
    else
        printf("[%s]-Error : Unknown Frm Type\n", __func__);
    return TRUE;
}

MS_BOOL VENC_Change_BitRateAndFrameRate(void)
{
    encode_info->FrameRatex100 = Venc_info.FrameRatex100;
    encode_info->nBitrate= Venc_info.nBitrate;
    MApi_MFE_SetConfig(MFE_ENC_SETCONF_CHANGE_BIT_RATE_AND_FRAME_RATE,encode_info);

    return TRUE;
}

MS_BOOL VENC_Deinit(void)
{
    if(encode_info->BitstreamOutBuffer.miuPointer) {
        free(encode_info->BitstreamOutBuffer.miuPointer);
    } else {
        printf("[%s]-Error : encode_info->BitstreamOutBuffer.miuPointer = NULL\n", __func__);
    }

    MApi_MFE_DeInitialize();

    if(encode_info) {
        free(encode_info);
    } else {
        printf("[%s]-Error : encode_info = NULL\n", __func__);
    }

    if(g_compress_ctx) {
        free(g_compress_ctx);
    } else {
        printf("[%s]-Error : g_compress_ctx = NULL\n", __func__);
    }

    return TRUE;
}

MS_BOOL Demo_MFE_Init(const MS_U32 *Frame_width, const MS_U32 *Frame_height, const MS_U32 *Frame_Rate, const MS_U32 *Bit_Rate)
{
    MS_U32 width = *Frame_width;
    MS_U32 height = *Frame_height;
    MS_U32 frame_rate = *Frame_Rate;
    MS_U32 bit_rate = *Bit_Rate;
    MS_S32 s32MstarNonCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    APVRBuffPAStart_orignal = MsOS_AllocateMemory(MFE_SIZE + IN_SIZE + 256, s32MstarNonCachedPoolID);
    if (APVRBuffPAStart_orignal == NULL)
    {
        printf("[%s]-Error : MsOS_AllocateMemory fail\n", __func__);
        return FALSE;
    }

    APVRBuffPAStart = (MS_U32*)(((MS_U32)APVRBuffPAStart_orignal + 256) & ~(256-1));

    APVRBuffPAStart_PA = MsOS_VA2PA((MS_U32)APVRBuffPAStart);
    if (APVRBuffPAStart_PA == NULL)
    {
        printf("[%s]-Error : MsOS_VA2PA fail\n", __func__);
        return FALSE;
    }

    in_addr.pa = APVRBuffPAStart_PA;
    mfe_buf.pa = APVRBuffPAStart_PA + IN_SIZE;

    if((width*height*3/2) > IN_SIZE)
    {
        printf("(width*height*3/2) > IN_SIZE (0x%x)>(0x%x)\n",(unsigned int)(width*height*3/2), (unsigned int)IN_SIZE);
        return FALSE;
    }

    in_addr.ptr = (unsigned char *)APVRBuffPAStart;

    //printf("in vermfe,in_addr.pa=0x%x,in_addr.ptr=0x%x\n",(unsigned int)in_addr.pa,(unsigned int)in_addr.ptr);

    mfe_buf.ptr = (unsigned char *)((MS_U32)APVRBuffPAStart + IN_SIZE);

    //printf("in vermfe,mfe_buf.pa=0x%x,mfe_buf.ptr=0x%x\n",(unsigned int)mfe_buf.pa,(unsigned int)mfe_buf.ptr);

    Venc_info.Width = width;
    Venc_info.Height = height;
    Venc_info.FrameRatex100 = frame_rate * 100;
    Venc_info.nBitrate = bit_rate * 1024;
    Venc_info.mfe_buf.ptr = mfe_buf.ptr;
    Venc_info.mfe_buf.pa = mfe_buf.pa;
    Venc_info.mfe_buf.size = MFE_SIZE;
    Venc_info.Miu_offset = MIU_OFFSET;

    VENC_Init();
    have_init = TRUE;

    return TRUE;
}

MS_BOOL Demo_MFE_Encode_File(char *in_file, char *out_file, const MS_U32 *Frame_Number)
{
    FILE* in_f, *out_f;
    int frame_num = *Frame_Number;
    int i;
    int width = Venc_info.Width;
    int height = Venc_info.Height;
    EN_MFE_Venc_Frm_Type frm_type;
    char* in_filename;
    char* out_filename;
    ST_MFE_MEMADDR cur_tmp;
    MS_S32 s32MstarNonCachedPoolID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    in_filename = in_file;
    out_filename = out_file;

    in_f = fopen(in_filename,"rb");

    if(in_f == NULL)
    {
        printf("open in file fail\n");
        return FALSE;
    }

    out_f = fopen(out_filename,"wb");

    if(out_f == NULL)
    {
        fclose(in_f);
        printf("open out file fail\n");
        return FALSE;
    }

    cur_tmp.ptr = malloc(IN_SIZE);

    {
        ST_MFE_MEMADDR_st vol;
        VENC_GetVol(&vol);
        fwrite( vol.ptr,1,vol.size,out_f);
    }

    for(i = 0;i<frame_num;i++)
    {
        int in_size = width*height*3/2;
        MS_BOOL force_i = FALSE;

#if RESET_BITRATE
        if (i == frame_num/2)
        {
            Venc_info.nBitrate = Venc_info.nBitrate / 2;
            printf("bit rate change to %d\n",Venc_info.nBitrate);
            VENC_Change_BitRateAndFrameRate();
            printf("bit rate change done\n");
        }
#endif

#if RESET_FRAMERATE
        if (i == frame_num/2)
        {
            Venc_info.FrameRatex100 = Venc_info.FrameRatex100 / 2;
            printf("frame rate change to %d\n",Venc_info.FrameRatex100);
            VENC_Change_BitRateAndFrameRate();
            printf("frame rate change done\n");
        }
#endif

        while((in_size) != fread(cur_tmp.ptr,1,in_size,in_f))
        {
            printf("fseek\n");
            fseek ( in_f , 0 , SEEK_SET );
        }

        {
            MS_U8* BufY_src = cur_tmp.ptr;
            MS_U8* BufC_src = (MS_U8 *)((MS_U32)cur_tmp.ptr+width*height);
            int stride_offset = width;
            int DstSizeY = CEILING_ALIGN(height,32)*stride_offset;
            int DstSizeUV = CEILING_ALIGN(height,64)*stride_offset/2;
            ConvertYV12ToHVDTile(BufY_src, NULL, stride_offset, height, stride_offset, in_addr.ptr, DstSizeY,1);
            ConvertYV12ToHVDTile(BufC_src, BufC_src+(height/2)*(stride_offset/2), stride_offset>>1, height>>1, stride_offset/2, in_addr.ptr
            +DstSizeY, DstSizeUV,0);
        }

        {
            ST_MFE_MEMADDR_st CurY,CurUV;
            int y_size=0;
            memset(&CurY,0,sizeof(ST_MFE_MEMADDR_st));
            memset(&CurUV,0,sizeof(ST_MFE_MEMADDR_st));
            y_size = CEILING_ALIGN(height,32)*width;
            CurY.pa = in_addr.pa;
            CurUV.pa = in_addr.pa + y_size;
            {
                //MS_U32 t_s,t_e;
                //t_s = MsOS_GetSystemTime();
                VENC_ENC_ONEFRAME(CurY,CurUV, force_i);
                //t_e = MsOS_GetSystemTime();
                //printf("\nencode time: %d\n",t_e-t_s);
            }
        }

        {
            ST_MFE_MEMADDR_st OutBuf;
            VENC_ENC_GETBITS(&OutBuf,&frm_type);
            //printf("[%d][frm:%d]size = 0x%04d\n", i, frm_type,OutBuf.size);
            //printf("0x%x,0x%x,0x%x,0x%x\n",*OutBuf.ptr,*(OutBuf.ptr+1),*(OutBuf.ptr+2),*(OutBuf.ptr+3));
            fwrite( OutBuf.ptr,1,OutBuf.size,out_f);
        }
    }

    if (have_init == TRUE)
    {
        VENC_Deinit();
        have_init = FALSE;
    }

    fclose(in_f);
    fclose(out_f);
    free(cur_tmp.ptr);

    if (APVRBuffPAStart_orignal)
    {
        int ret;
        ret = MsOS_FreeMemory((void *)APVRBuffPAStart_orignal, s32MstarNonCachedPoolID);
        if(FALSE == ret)
        {
            printf("[%s]-Error : MsOS_FreeMemory fail\n", __func__);
            return FALSE;
        }
        APVRBuffPAStart_orignal = 0;
    }

    return TRUE;

}

void MApiFrameDoneCallback(SC_INT_SRC enIntNum, void *pParam)
{
    MsOS_SetEvent (MFE_Event_id, 0x1);
    MApi_XC_DIP_EX_ClearInt(0x1, E_XC_DIP_EX_DIP_WINDOW);
}

MS_BOOL Demo_MFE_Encode_From_DIP(char *out_file, const MS_U32 *Frame_Number)
{
    FILE *out_f;
    int frame_num = *Frame_Number;
    int i;
    //int width = Venc_info.Width;
    //int height = Venc_info.Height;
    EN_MFE_Venc_Frm_Type frm_type;
    char* out_filename;
    MS_U32 Time0 = 0;
    MS_U32 Time1 = 0;
    BUFFER_INFO info;
    MS_U32 u32Events = 0;
    MS_U32 Time_Encode = 0;
    MS_U32 Time_Fwrite = 0;
    MS_S32 s32MstarNonCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    out_filename = out_file;

    out_f = fopen(out_filename,"wb");

    if(out_f == NULL)
    {
        printf("open out file fail\n");
        return FALSE;
    }

    {
        ST_MFE_MEMADDR_st vol;
        VENC_GetVol(&vol);
        fwrite( vol.ptr,1,vol.size,out_f);
    }

    MFE_Event_id = MsOS_CreateEventGroup("MFE_Event");

    MApi_XC_DIP_EX_EnaInt(0x2, FALSE, E_XC_DIP_EX_DIP_WINDOW);

    MApi_XC_DIP_EX_InterruptAttach((InterruptCb)MApiFrameDoneCallback, E_XC_DIP_EX_DIP_WINDOW);

    Time0 = MsOS_GetSystemTime();

    for(i = 0;i<frame_num;i++)
    {
        MS_BOOL force_i = FALSE;

        MsOS_WaitEvent(MFE_Event_id, 0x1, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        {
            ST_MFE_MEMADDR_st CurY,CurUV;
            memset(&CurY,0,sizeof(ST_MFE_MEMADDR_st));
            memset(&CurUV,0,sizeof(ST_MFE_MEMADDR_st));
            //int y_size=0;
            //y_size = CEILING_ALIGN(height,32)*width;

            info = MApi_XC_DIP_GetBufInfo(DIP_WINDOW);

            CurY.pa = info.u32YBuf[0];//DIP_buffer;
             CurUV.pa = info.u32CBuf[0];//DIP_buffer + y_size;
         {
                MS_U32 t_s,t_e;
                t_s = MsOS_GetSystemTime();
                VENC_ENC_ONEFRAME(CurY,CurUV, force_i);
                t_e = MsOS_GetSystemTime();
                Time_Encode += t_e - t_s;
            }
        }

        {
            MS_U32 t_s,t_e;
            ST_MFE_MEMADDR_st OutBuf;
            VENC_ENC_GETBITS(&OutBuf,&frm_type);
            t_s = MsOS_GetSystemTime();
            fwrite( OutBuf.ptr,1,OutBuf.size,out_f);
            t_e = MsOS_GetSystemTime();
            Time_Fwrite += t_e - t_s;
        }
    }

    Time1 = MsOS_GetSystemTime();
    {
         MS_U32 Temp = 0;
         Temp = (frame_num*1000000) / (Time1-Time0);
         printf("H264 Encode time = %d ms ; fps = %d.%d\n", (int)(Time1-Time0), (int)(Temp/1000), (int)(Temp - ((Temp/1000)*1000)));
         printf("Time_Encode = %d ms, Time_Fwrite = %d ms\n", (int)Time_Encode, (int)Time_Fwrite);
    }

    VENC_Deinit();
    fclose(out_f);

    if (APVRBuffPAStart_orignal)
    {
        int ret;
        ret = MsOS_FreeMemory((void *)APVRBuffPAStart_orignal, s32MstarNonCachedPoolID);
        if(FALSE == ret)
        {
            printf("[%s]-Error : MsOS_FreeMemory fail\n", __func__);
            return FALSE;
        }
        APVRBuffPAStart_orignal = 0;
    }

    return TRUE;

}
#endif

