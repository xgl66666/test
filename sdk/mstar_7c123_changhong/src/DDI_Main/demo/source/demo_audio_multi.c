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
/// @file   Demo_Audio.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for audio functions
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_AUDIO_MULTI_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <math.h>
#include "MsCommon.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "drvGPIO.h"
#include "MsMemory.h"
#include "apiHDMITx.h"

#include "drvXC_IOPort.h"
#include "apiXC_EX.h"
#include "apiXC_PCMonitor_EX.h"

#include "../data/JAPAN_48_2_mp3.h"
#include "../data/WAV_44K.h"

#include "demo_xc.h"
#include "demo_utility.h"
#include "demo_audio_multi.h"

#include "SysBsp.h"

#include "drvDTC.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
static Task_Info _audioDemo_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Demo Task"};
static Task_Info _audioMonitor_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Monitor"};

#define E_DECODER_NUMBER (2)

typedef struct
{
    MS_U32 u32HeaderSize;
    MS_U32 u32RIFFChunkSize;
    MS_U32 u32fmtChunkSize;
    MS_U32 u32DataChunkSize;
    MS_U16 u16FormatTag;
    MS_U16 u16Channels;
    MS_U32 u32SamplesPerSec;
    MS_U32 u32AvgBytesPerSec;
    MS_U16 u16BlockSize;
    MS_U16 u16BitsPerSample;
    MS_U16 u16CExtensionSize;
} ST_WAVINFO;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 *gapu8AudioData[E_DECODER_NUMBER];
static MS_S32 gau32AudioLen[E_DECODER_NUMBER];
static MS_U32 gau32clipWriteLastAddr[E_DECODER_NUMBER];
static MS_U32 gau32AudioFileIndex[E_DECODER_NUMBER];
static MS_BOOL _bNew_MM_Mode[E_DECODER_NUMBER];
static MS_U32 _u32StartTime[E_DECODER_NUMBER];
static ST_AUDIO_DEC_INFO stDualDemoInfo[E_DECODER_NUMBER];

MS_BOOL bdsp2enable=FALSE;
OUTPUT_SOURCE_INFO Output_SourceInfo = {  .SpeakerOut = 0,
                                                    .HpOut = 0,
                                                    .MonitorOut = 0,
                                                    .ScartOut = 0,
                                                    .SpdifOut = 0,
                                                    .ArcOut = 0,
                                                    .HDMIOut = 0};      //0 is main, 1 is sub

MS_BOOL _bf_HdmiIsRaw = FALSE;
AUDIO_HDMI_RX_TYPE _bfHDMI_RX_Mode = HDMI_RX_PCM;
MS_U32 _bfHdmiAC3inSE = 0;
static AUDIO_FS_TYPE _stCurSampleType = AUDIO_FS_NONE;
static HDMI_TX_OUTPUT_TYPE _stCurHDMIType = HDMI_OUT_PCM;

static MS_BOOL _bExit_Monitor_Task = FALSE;
static MS_BOOL _bExit_Demo_Task = FALSE;

#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
static MS_BOOL _bFMTX_enable = FALSE, _bBTSC_enable = FALSE, _bPCM_CAPTURE_enable = FALSE;
static MS_BOOL _bDDP_BYPASS_enable = FALSE, _bDSP2_Decode = FALSE;
#endif

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
MS_U16 AudioVolumeTable[] =
{ //    1     2       3        4      5       6        7      8        9       10
                                                                                                                    0x7F00, //  00
    0x4700, 0x4400, 0x4100, 0x3E00, 0x3C00, 0x3A00, 0x3800, 0x3600, 0x3400, 0x3200, //  10
    0x3000, 0x2E00, 0x2D00, 0x2C00, 0x2B00, 0x2A00, 0x2900, 0x2800, 0x2700, 0x2600, //  20
    0x2500, 0x2400, 0x2300, 0x2200, 0x2100, 0x2000, 0x1F00, 0x1E80, 0x1E00, 0x1D80, //  30
    0x1D00, 0x1C80, 0x1C00, 0x1B80, 0x1B00, 0x1A80, 0x1A00, 0x1980, 0x1900, 0x1880, //  40
    0x1800, 0x1780, 0x1700, 0x1680, 0x1600, 0x1580, 0x1540, 0x1500, 0x14C0, 0x1480, //  50
    0x1440, 0x1400, 0x13C0, 0x1380, 0x1340, 0x1300, 0x12C0, 0x1280, 0x1240, 0x1200, //  60
    0x11C0, 0x1180, 0x1140, 0x1100, 0x10C0, 0x1080, 0x1040, 0x1000, 0x0FE0, 0x0FC0, //  70
    0x0FA0, 0x0F80, 0x0F60, 0x0F40, 0x0F20, 0x0F00, 0x0EE0, 0x0EC0, 0x0EA0, 0x0E80, //  80
    0x0E60, 0x0E40, 0x0E20, 0x0E00, 0x0DE0, 0x0DC0, 0x0DA0, 0x0D80, 0x0D60, 0x0D40, //  90
    0x0D20, 0x0D00, 0x0CE0, 0x0CC0, 0x0CA0, 0x0C80, 0x0C60, 0x0C40, 0x0C20, 0x0C00  //  100
};

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief The sample code to feed data to audio codec
/// @param[in] writeAddress The address that audio data should put in
/// @param[in] writeNumber The length that audio data should put in
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
static void _Demo_Audio_File_play(MS_U32 u32DevIdx, MS_U32 writeAddress, MS_U32 writeNumber, MS_BOOL bNewMMmode)
{
    //printf("\nDemo_Audio_File_play(0x%"DTC_MS_U32_x",0x%"DTC_MS_U32_x")...\n",writeAddress,writeNumber);
    if(bNewMMmode)
    {
        //new MM mode
        MS_U32 u32RemainedSize = gau32AudioLen[u32DevIdx] - gau32AudioFileIndex[u32DevIdx];
        AUDIO_DEC_ID DecID = stDualDemoInfo[u32DevIdx].eDecID;
        MS_U32 u32es_size = 0;
        MS_U64 u64PTS = 0;

        //TRUE: Set PTS; FALSE: Not set PTS (New MM mode support this feature)
        MS_BOOL bSetPTS = TRUE;

        if (gau32AudioFileIndex[u32DevIdx] ==  0 )
        {
            if (_u32StartTime[u32DevIdx] == 0)
            {
                _u32StartTime[u32DevIdx] = MsOS_GetSystemTime();
            }
        }

        if (u32RemainedSize > 0)
        {
            if(writeNumber>0)
            {
                u32es_size = MIN(writeNumber, u32RemainedSize);

                memcpy((void*)MS_PA2KSEG1(MApi_AUDIO_GetDDRInfo(DecID, E_AUD_MEMORY_BASE)+writeAddress), (gapu8AudioData[u32DevIdx]+gau32AudioFileIndex[u32DevIdx]), u32es_size);

                gau32AudioFileIndex[u32DevIdx] += u32es_size;

                //example of setting system time differemce (ticks) as PTS
                u64PTS = (MsOS_GetSystemTime() - _u32StartTime[u32DevIdx] ) * 90;

                //info DSP data is ready
                MApi_AUDIO_MM2_inputAesFinished(DecID, u32es_size, bSetPTS, u64PTS);
            }
        }
        else
        {
            printf("[%s][%d] play finished!!!\n", __FUNCTION__, __LINE__);

            //reset file index
            gau32AudioFileIndex[u32DevIdx] = 0;
        }
    }
    else
    {
        //old MM mode
        MS_U8* pAudioData = NULL;
        MS_U32 u32FileIndex = 0;
        MS_U32 u32LastAddr = 0;
        AUDIO_DEC_ID DecID;
        MS_U32 u32AudioLen = 0;

        pAudioData = gapu8AudioData[u32DevIdx];
        u32FileIndex = gau32AudioFileIndex[u32DevIdx];
        u32LastAddr = gau32clipWriteLastAddr[u32DevIdx];
        DecID = stDualDemoInfo[u32DevIdx].eDecID;
        u32AudioLen = gau32AudioLen[u32DevIdx];

        if( writeAddress == u32LastAddr )
        {
            //MApi_AUDIO_MM_SetInput(AUDDecoderNo,_u8IntTag);
            //_u8IntTag++;
            MApi_AUDIO_SetAudioParam2(DecID, Audio_ParamType_esBuf_informDataRdy, NULL);
            return;
        }

        u32LastAddr = writeAddress ;

        if ((u32FileIndex + writeNumber) <= u32AudioLen )
        {
            memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+u32FileIndex),writeNumber);
            u32FileIndex += writeNumber;
        }
        else
        {
            // clear the buffer first
            memset((void*)MS_PA2KSEG1(writeAddress), 0, writeNumber);

            if (writeNumber <= u32AudioLen )
            {
                // normal case
                int feedsize = 0;

                memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+u32FileIndex),u32AudioLen  - u32FileIndex);
                feedsize += (u32AudioLen - u32FileIndex);

                u32FileIndex = 0;
                memcpy((void*)MS_PA2KSEG1(writeAddress+feedsize), (void*)(pAudioData+u32FileIndex), writeNumber - feedsize);

                u32FileIndex = writeNumber - feedsize;
            }
        }

        gau32AudioFileIndex[u32DevIdx] = u32FileIndex;

        //MApi_AUDIO_MM_SetInput(AUDDecoderNo,_u8IntTag);
        //_u8IntTag++;
        MApi_AUDIO_SetAudioParam2(DecID, Audio_ParamType_esBuf_informDataRdy, NULL);
    }

}

static MS_BOOL _Demo_Audio_GetHDMIRxStatus(void)
{
    XC_DEVICE_ID stXC_DeivceId = {0, E_XC_EX_DEVICE0};
    MSAPI_XC_DEVICE_ID _stXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
    XC_EX_MUX_PATH_INFO stXC_PathInfo[XC_EX_MAX_SYNC_DATA_PATH_SUPPORTED];
    MS_U8 u8PathNum = 0;

    memset(&stXC_PathInfo, 0, sizeof(stXC_PathInfo));
    
    if (msAPI_XC_CheckInitStatus_EX(&_stXC_DeviceId[0]))
    {
        u8PathNum = MApi_XC_EX_Mux_GetPathInfo(&stXC_DeivceId, stXC_PathInfo);
    
        while(u8PathNum > 0)
        {
            if(XC_EX_IsSrcTypeHDMI(stXC_PathInfo[u8PathNum-1].src))
            {
                XC_EX_SCALER_WIN eWindow = (E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW == stXC_PathInfo[u8PathNum-1].dest) ? E_XC_EX_MAIN_WINDOW : E_XC_EX_SUB_WINDOW;
    
                if(E_XC_PCMONITOR_EX_STABLE_SYNC == MApi_XC_PCMonitor_EX_GetCurrentState(&stXC_DeivceId, eWindow))
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
    
            u8PathNum--;
        }
    }

    stXC_DeivceId.u32Id = E_XC_EX_DEVICE1;
    memset(&stXC_PathInfo, 0, sizeof(stXC_PathInfo));
    
    if (msAPI_XC_CheckInitStatus_EX(&_stXC_DeviceId[1]))
    {
        u8PathNum = MApi_XC_EX_Mux_GetPathInfo(&stXC_DeivceId, stXC_PathInfo);
    
        while(u8PathNum > 0)
        {
            if(XC_EX_IsSrcTypeHDMI(stXC_PathInfo[u8PathNum-1].src))
            {
                XC_EX_SCALER_WIN eWindow = (E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW == stXC_PathInfo[u8PathNum-1].dest) ? E_XC_EX_MAIN_WINDOW : E_XC_EX_SUB_WINDOW;
    
                if(E_XC_PCMONITOR_EX_STABLE_SYNC == MApi_XC_PCMonitor_EX_GetCurrentState(&stXC_DeivceId, eWindow))
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
    
            u8PathNum--;
        }
    }

    return FALSE;
}

static void _Demo_HDMI_RX_Monitor(void)
{
    MS_BOOL bHdmiInSub = FALSE;
    MS_BOOL bcur_hdmiIsRaw = MApi_AUDIO_HDMI_GetNonpcmFlag();
    AUDIO_HDMI_RX_TYPE st_crHDMI_RX_Mode = MApi_AUDIO_HDMI_RX_GetNonPCM();
    MS_U32 u32crHdmiAC3inSE = MApi_AUDIO_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE);

    if((bcur_hdmiIsRaw == _bf_HdmiIsRaw) &&
      (st_crHDMI_RX_Mode == _bfHDMI_RX_Mode) &&
      (u32crHdmiAC3inSE == _bfHdmiAC3inSE))
    {
        return;
    }

    //check if HDMI Rx connected
    if (_Demo_Audio_GetHDMIRxStatus() == FALSE)
    {
        return;
    }

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_HDMI_IN);

    if(bcur_hdmiIsRaw)
    {
        if(bHdmiInSub)
        {
            MApi_AUDIO_HDMI_RX_SetNonpcm(0x11); //HDMI non-PCM setting in PIP Sub
        }
        else
        {
            MApi_AUDIO_HDMI_RX_SetNonpcm(0x01); //HDMI non-PCM setting in PIP Main
        }

        if(MApi_AUDIO_HDMI_RX_GetNonPCM() == HDMI_RX_DD)
        {
            MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DrcMode, LINE_MODE, 0);                                            //Line Mod
            MApi_AUDIO_SetAC3Info(Audio_AC3_infoType_DownmixMode, DOLBY_DOWNMIX_MODE_LTRT, 0);                          //LtRt
        }

    }
    else
    {
        if(bHdmiInSub)
        {
            MApi_AUDIO_HDMI_RX_SetNonpcm(0x10); //HDMI non-PCM setting in PIP Sub
        }
        else
        {
            MApi_AUDIO_HDMI_RX_SetNonpcm(0x00); //HDMI non-PCM setting in PIP Main
        }
    }

    _bfHdmiAC3inSE = u32crHdmiAC3inSE;
    _bf_HdmiIsRaw = bcur_hdmiIsRaw;
    _bfHDMI_RX_Mode = st_crHDMI_RX_Mode;

}

static void _Demo_HDMI_TX_Status_Monitor(void)
{
    MS_BOOL hdmi_tx_en;
    AUDIO_FS_TYPE fs_type;
    HDMI_TX_OUTPUT_TYPE outType;
    HDMITX_AUDIO_FREQUENCY hreq = HDMITX_AUDIO_FREQ_NO_SIG;
    HDMITX_AUDIO_CODING_TYPE htype = HDMITX_AUDIO_PCM;

    MApi_AUDIO_HDMI_Tx_GetStatus(&hdmi_tx_en, &fs_type, &outType);

    if (hdmi_tx_en)
    {
        if((_stCurSampleType == fs_type) && (_stCurHDMIType == outType))
        {
            return;
        }
        else
        {
            _stCurSampleType = fs_type;
            _stCurHDMIType = outType;
        }

        if (outType == HDMI_OUT_NONPCM)
        {
            htype = HDMITX_AUDIO_NONPCM;
            switch (fs_type)
            {
            case AUDIO_FS_32KHZ:
                hreq = HDMITX_AUDIO_32K;
                break;
            case AUDIO_FS_44KHZ:
                hreq = HDMITX_AUDIO_44K;
                break;
            case AUDIO_FS_176KHZ:
                hreq = HDMITX_AUDIO_176K;
                break;
            case AUDIO_FS_192KHZ:
                hreq = HDMITX_AUDIO_192K;
                break;
            case AUDIO_FS_48KHZ:
            default:
                hreq = HDMITX_AUDIO_48K;
                break;
            }
        }
        else
        {
            htype = HDMITX_AUDIO_PCM;
            hreq = HDMITX_AUDIO_48K;                            //for PCM mode always SRC to 48K
        }

#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
        if ((hreq == HDMITX_AUDIO_192K) || (hreq == HDMITX_AUDIO_176K))
        {
            //disable features(depend on chip's capability) when DDP bypass enable
            //kaiser disable feature: whole sound effect, 2nd decode(disable by upper layer), PCM capture, ATV encode
            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DDP_Enable_FineTune_Feature, ENABLE, 0);
            _bDDP_BYPASS_enable = TRUE;
        }
        else
        {
            MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DDP_Enable_FineTune_Feature, DISABLE, 0);
            _bDDP_BYPASS_enable = FALSE;
        }
#endif

        printf("Hdmx Tx:%x , %x\n", hreq, htype);
        MApi_HDMITx_SetAudioOnOff(TRUE);
        MApi_HDMITx_SetAudioFrequency(hreq);
        MApi_HDMITx_SetAudioConfiguration(hreq, HDMITX_AUDIO_CH_2, htype);
    }

}

static MS_BOOL _appAudioDemo_MM_CheckInputRequest(AUDIO_DEC_ID DecID, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes, MS_BOOL bNewMMmode)
{
    if((pU32WrtAddr == NULL) || (pU32WrtBytes == NULL))
    {
        return FALSE;
    }

    if (bNewMMmode)
    {
        AES_INFO es_info;

        if (MApi_AUDIO_MM2_checkAesInfo(DecID, &es_info))
        {
            *pU32WrtAddr = es_info.aes_write_addr;
            *pU32WrtBytes = es_info.aes_freeSpace;
            return TRUE;
        }
    }
    else
    {
        Audio_MM_Data_Request st_Data_Request;

        if (MApi_AUDIO_GetAudioInfo2(DecID, Audio_infoType_esBuf_req, &st_Data_Request))
        {
            *pU32WrtAddr = st_Data_Request.U32WrtAddr;
            *pU32WrtBytes = st_Data_Request.U32WrtBytes;
            return TRUE;
        }
    }
    return FALSE;
}

static void _appAudioDemo_task(MS_U32 argc, void *argv)
{
    MS_U32 u32WrtAddr;
    MS_U32 u32WrtBytes;
    MS_U8 u8Idx;

    while(_bExit_Demo_Task == FALSE)
    {
        for(u8Idx=0; u8Idx<E_DECODER_NUMBER; u8Idx++)
        {
            if(stDualDemoInfo[u8Idx].stAudioDecStatus_t.bConnect)
            {
                if(_appAudioDemo_MM_CheckInputRequest(
                    stDualDemoInfo[u8Idx].stAudioDecStatus_t.eDSPId,
                    &u32WrtAddr,
                    &u32WrtBytes, _bNew_MM_Mode[u8Idx]))
                {
                    _Demo_Audio_File_play(u8Idx, u32WrtAddr, u32WrtBytes, _bNew_MM_Mode[u8Idx]);
                }
            }
        }

        MsOS_DelayTask(1);
    }
    _bExit_Demo_Task = FALSE;
    _audioDemo_Task.iId = -1;

}

#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
static void _Demo_DDP_BYPASS_Set_Feature_Status(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    switch (eAppType)
    {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            if(stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
            {
                 _bDSP2_Decode = TRUE;
            }
            break;
        case AUDIO_APP_ATV_FMTX:
            _bFMTX_enable = TRUE;
            break;
        case AUDIO_APP_ATV_BTSC:
            _bBTSC_enable = TRUE;
            break;
        default:
            break;
    }
}

static void _Demo_DDP_BYPASS_Clear_Feature_Status(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    switch (eAppType)
    {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            if(stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
            {
                 _bDSP2_Decode = FALSE;
            }
            break;
        case AUDIO_APP_ATV_FMTX:
            _bFMTX_enable = FALSE;
            break;
        case AUDIO_APP_ATV_BTSC:
            _bBTSC_enable = FALSE;
            break;
        default:
            break;
    }
}

static MS_BOOL _Demo_DDP_BYPASS_Check_Allow_Feature(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{

    if (_bDDP_BYPASS_enable)
    {
        if ((eAppType == AUDIO_APP_ES_PLAY) || (eAppType == AUDIO_APP_DTV) || (eAppType == AUDIO_APP_DTV_AM))
        {
            if (stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_ID3)
            {
                return FALSE;
            }
        }
        else if((eAppType == AUDIO_APP_ATV_FMTX) || (eAppType == AUDIO_APP_ATV_BTSC))
        {
            return FALSE;
        }
    }

    return TRUE;

}
#endif

static void _appAudioMonitor_task(MS_U32 argc, void *argv)
{
    while(_bExit_Monitor_Task == FALSE)
    {
        //dump debug message and monitor if DSP alive
        MApi_Audio_Monitor();

#if (DEMO_XC_HDMIRX_TEST == 1)
        //monitor if send data from HDMI Rx
        _Demo_HDMI_RX_Monitor();
#endif

        //monitor if HDMI Tx status change
        _Demo_HDMI_TX_Status_Monitor();
				
        //monitor smple rate change
        MApi_Audio_SPDIF_Monitor();
				
        MsOS_DelayTask(1);
    }
    _bExit_Monitor_Task = FALSE;
    _audioMonitor_Task.iId = -1;
}

static En_DVB_decSystemType _Demo_Audio_CodecTypeMap(EN_AUDIO_CODEC_TYPE eCodecType)
{
    switch(eCodecType)
    {
        case AUDIO_CODEC_NONE: return MSAPI_AUD_DVB_NONE;
        case AUDIO_CODEC_MPEG: return MSAPI_AUD_DVB_MPEG;
        case AUDIO_CODEC_AC3: return MSAPI_AUD_DVB_AC3;
        case AUDIO_CODEC_AC3P: return MSAPI_AUD_DVB_AC3P;
        case AUDIO_CODEC_AAC: return MSAPI_AUD_DVB_AAC;
        case AUDIO_CODEC_MP3: return MSAPI_AUD_DVB_MP3;
        case AUDIO_CODEC_WMA: return MSAPI_AUD_DVB_WMA;
        case AUDIO_CODEC_CDLPCM: return MSAPI_AUD_DVB_CDLPCM;
        case AUDIO_CODEC_RA8LBR: return MSAPI_AUD_DVB_RA8LBR;
        case AUDIO_CODEC_XPCM: return MSAPI_AUD_DVB_XPCM;
        case AUDIO_CODEC_TONE: return MSAPI_AUD_DVB_TONE;
        case AUDIO_CODEC_DTS: return MSAPI_AUD_DVB_DTS;
        case AUDIO_CODEC_MS10_DDT: return MSAPI_AUD_DVB_MS10_DDT;
        case AUDIO_CODEC_MS10_DDC: return MSAPI_AUD_DVB_MS10_DDC;
        case AUDIO_CODEC_WMA_PRO: return MSAPI_AUD_DVB_WMA_PRO;
        case AUDIO_CODEC_FLAC: return MSAPI_AUD_DVB_FLAC;
        case AUDIO_CODEC_VORBIS: return MSAPI_AUD_DVB_VORBIS;
        case AUDIO_CODEC_DTSLBR: return MSAPI_AUD_DVB_DTSLBR;
        case AUDIO_CODEC_AMR_NB: return MSAPI_AUD_DVB_AMR_NB;
        case AUDIO_CODEC_AMR_WB: return MSAPI_AUD_DVB_AMR_WB;
        case AUDIO_CODEC_DRA: return MSAPI_AUD_DVB_DRA;
        case AUDIO_CODEC_DolbyTrueHDBypass: return MSAPI_AUD_DVB_DolbyTrueHDBypass;

        //AD
        case AUDIO_CODEC_AD_NONE: return MSAPI_AUD_DVB2_NONE;
        case AUDIO_CODEC_AD_MPEG: return MSAPI_AUD_DVB2_MPEG;
        case AUDIO_CODEC_AD_AC3: return MSAPI_AUD_DVB2_AC3;
        case AUDIO_CODEC_AD_MP3: return MSAPI_AUD_DVB2_MP3;
        case AUDIO_CODEC_AD_AAC: return MSAPI_AUD_DVB2_AAC;
        case AUDIO_CODEC_AD_GAAC: return MSAPI_AUD_DVB2_GAAC;
        case AUDIO_CODEC_AD_AC3P: return MSAPI_AUD_DVB2_AC3P;
        case AUDIO_CODEC_AD_DDE: return MSAPI_AUD_DVB2_DDE;
        case AUDIO_CODEC_AD_DTSE: return MSAPI_AUD_DVB2_DTSE;
        case AUDIO_CODEC_AD_XPCM: return MSAPI_AUD_DVB2_XPCM;
        case AUDIO_CODEC_AD_KTV: return MSAPI_AUD_DVB2_KTV;
        case AUDIO_CODEC_AD_KTV2: return MSAPI_AUD_DVB2_KTV2;
        case AUDIO_CODEC_AD_DTS: return MSAPI_AUD_DVB2_DTS;
        case AUDIO_CODEC_AD_TONEGEN: return MSAPI_AUD_DVB2_TONEGEN;

        //ATV SIF
        case AUDIO_CODEC_ATV_NONE: return MSAPI_AUD_ATV_NONE;
        case AUDIO_CODEC_ATV_BTSC: return MSAPI_AUD_ATV_BTSC;
        case AUDIO_CODEC_ATV_EIAJ: return MSAPI_AUD_ATV_EIAJ;
        case AUDIO_CODEC_ATV_PAL: return MSAPI_AUD_ATV_PAL;
        case AUDIO_CODEC_ATV_FM_RADIO: return MSAPI_AUD_ATV_FM_RADIO;
        case AUDIO_CODEC_ATV_MAX: return MSAPI_AUD_ATV_MAX;

        //ATV ENC
        case AUDIO_CODEC_ATVENC_NONE: return MSAPI_AUD_ATVENC_NONE;
        case AUDIO_CODEC_ATVENC_FMTX: return MSAPI_AUD_ATVENC_FMTX;
        case AUDIO_CODEC_ATVENC_BTSC: return MSAPI_AUD_ATVENC_BTSC;
        default: return MSAPI_AUD_DVB_INVALID;
    }
}

#if 1
static MS_BOOL _Demo_Audio_OpenDecSys(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, En_DVB_decSystemType eCodecType)
{
    AUDIO_SOURCE_INFO_TYPE eSrcType;
    AUDIO_OUTPORT_SOURCE_TYPE eOutSrcType;

    if(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.bConnect == TRUE)
    {
        printf("[%s][%d] decode system (%"DTC_MS_U32_u") already opened!!!\n", __FUNCTION__, __LINE__, u32DevIdx);
        return TRUE;
    }

    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
        eSrcType = E_AUDIO_INFO_GAME_IN;
        break;
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
    case AUDIO_APP_DTV_AD:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
        eSrcType = E_AUDIO_INFO_ATV_ENC_IN;
        break;
#endif
    default:
        eSrcType = E_AUDIO_INFO_GAME_IN;
    }

    if(u32DevIdx == 0)
    {
        eOutSrcType = E_CONNECT_MAIN;
    }
    else if(u32DevIdx == 1)
    {
        eOutSrcType = E_CONNECT_SUB;
    }
    else
    {
        return FALSE;
    }

    memset(&stDualDemoInfo[u32DevIdx].stAudioDecStatus_t, 0x00, sizeof(AudioDecStatus_t));

    //set status for main
    stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.bIsAD = FALSE;
    stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eAudFormat  = eCodecType;
    stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eSourceType = eSrcType;
    stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eGroup = eOutSrcType;

    //open main decoder system
    stDualDemoInfo[u32DevIdx].eDecID = MApi_AUDIO_OpenDecodeSystem(&stDualDemoInfo[u32DevIdx].stAudioDecStatus_t);
    if(stDualDemoInfo[u32DevIdx].eDecID == AU_DEC_INVALID)
    {
        return FALSE;
    }

    MApi_AUDIO_GetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDualDemoInfo[u32DevIdx].stAudioDecStatus_t);

    return TRUE;
}

static MS_BOOL _Demo_Audio_CloseDecSys(MS_U32 u32DevIdx)
{
    if((stDualDemoInfo[u32DevIdx].eDecID != AU_DEC_INVALID) && (stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.bConnect == TRUE))
    {
        MApi_AUDIO_ReleaseDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID);

        memset(&stDualDemoInfo[u32DevIdx].stAudioDecStatus_t, 0x00, sizeof(AudioDecStatus_t));
        stDualDemoInfo[u32DevIdx].eDecID = -1;
    }

    return TRUE;
}
#endif

static MS_BOOL _Demo_Audio_SetSystem(MS_U32 u32DevIdx, En_DVB_decSystemType eCodecType)
{
    AudioDecStatus_t stDecStatus;
    MS_BOOL bret = FALSE;

    bret = MApi_AUDIO_GetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    stDecStatus.eAudFormat = eCodecType;
    bret = MApi_AUDIO_SetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_SetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_Audio_STC_Switch(MS_U32 u32DevIdx, AUDIO_STC_SOURCE eSTC_Source)
{
    AudioDecStatus_t stDecStatus;
    MS_BOOL bret = FALSE;

    bret = MApi_AUDIO_GetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_GetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    stDecStatus.eStcSource = eSTC_Source;
    bret = MApi_AUDIO_SetDecodeSystem(stDualDemoInfo[u32DevIdx].eDecID, &stDecStatus);
    if (bret == FALSE)
    {
        printf("[%s][%d] MApi_AUDIO_SetDecodeSystem fail!!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Demo_Audio_CheckAppSupport(EN_AUDIO_APP_TYPE eAppType)
{
    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
#if (DEMO_AUDIO_AD_TEST == 1)
    case AUDIO_APP_DTV_AD:
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
#endif
        return TRUE;
    default:
        return FALSE;
    }
}

/*
static MS_BOOL _Demo_Audio_Set_STC(MS_U32 u32DevIdx)
{
    return FALSE;
}
*/

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#if(DEMO_AUDIO_SE_DV258_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to enable/disable Dolby Volume 258
/// @param[in] u8Enable
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DV258 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DV258(MS_U8* u8Enable)
{
    MS_BOOL bret = FALSE;
    bret = MApi_AUDIO_ADVSOUND_SetParam(DV258_ENABLE, *u8Enable, 0);
    return bret;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set DRC scale value
/// @param[in] u32CutScale
/// @param[in] u32BoostScale
/// @return none
/// @sa
/// @note DDP only
/// Command: \b Audio_DRC_Scale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Scale(MS_U32* u32CutScale, MS_U32* u32BoostScale)
{
    MS_BOOL bret = FALSE, bret1 = FALSE;
    bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_Drc_HighCutScale, *u32CutScale, 0);
    bret1 = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_Drc_LowBootScale, *u32BoostScale, 0);
    if (bret && bret1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set down mix mode
/// @param[in] eCodecType 2: AC3 3: AC3P 4: AAC
/// @param[in] mode 0: LtRt mode 1: LoRo mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_Downmix \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DownMix_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode)
{
    MS_BOOL bret = FALSE;
    switch (*eCodecType)
    {
        case AUDIO_CODEC_AC3:
        case AUDIO_CODEC_AC3P:
        case AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DownmixMode, *mode, 0);
            break;
        default:
            bret = TRUE;
            printf("Down Mix mode not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to set dyanmic range control mode (DRC mode)
/// @param[in] eCodecType 2: AC3 3: AC3P 4: AAC
/// @param[in] mode 0: LINE mode 1: RF mode
/// @return none
/// @sa
/// @note
/// Command: \b Audio_DRC \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_DRC_Mode(EN_AUDIO_CODEC_TYPE* eCodecType, MS_U8* mode)
{
    MS_BOOL bret = FALSE;
    switch (*eCodecType)
    {
        case AUDIO_CODEC_AC3:
        case AUDIO_CODEC_AC3P:
        case AUDIO_CODEC_AAC:
            bret = MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode, *mode, 0);
            break;
        default:
            bret = TRUE;
            printf("Down Mix mode not implement yet, codec: %d\n", *eCodecType);
            break;
    }
    return bret;
}

#if (DEMO_AUDIO_FMTX_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode FMTX signal
/// @param[in] u8Filter 0: PAL 1: NTSC
/// @return none
/// @sa
/// @note
/// Command: \b Audio_FMTX_start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_FMTX_Start(MS_U8* u8Filter, MS_U16* u16Attenuation, MS_U16* u16OutScaling)
{
    ST_AUDIO_FMTX_PARAM stParam;

    stParam.u8Filter = *u8Filter;
    stParam.u16Attenuation = *u16Attenuation;
    stParam.u16OutScaling = *u16OutScaling;

    /* please be notice that FMTX can only be used for sub */
    _Demo_Audio_CloseDecSys(1);

    if(_Demo_Audio_OpenDecSys(1, AUDIO_APP_ATV_FMTX, MSAPI_AUD_DVB_MPEG) == FALSE)
    {
        printf("[%s][%d] open decode system (1) fail.\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (Demo_Audio_Play(1, AUDIO_APP_ATV_FMTX, AUDIO_CODEC_ATVENC_FMTX, (void *)&stParam) == FALSE)
    {
        _Demo_Audio_CloseDecSys(1);
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop FMTX
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_FMTX_stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_FMTX_Stop(void)
{
    return Demo_Audio_Stop(1, AUDIO_APP_ATV_FMTX);
}
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode BTSC signal
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_BTSC_start \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_BTSC_Start(MS_U16* u16Attenuation, MS_U16* u16OutScaling, MS_U16* u16_M_Gain, MS_U16* u16_D_Gain, MS_U16* u16_SAP_Gain)
{
    ST_AUDIO_BTSC_PARAM stParam;

    stParam.u16Attenuation = *u16Attenuation;
    stParam.u16OutScaling = *u16OutScaling;
    stParam.u16_M_Gain = *u16_M_Gain;
    stParam.u16_D_Gain = *u16_D_Gain;
    stParam.u16_SAP_Gain = *u16_SAP_Gain;

    return Demo_Audio_Play(0, AUDIO_APP_ATV_BTSC, AUDIO_CODEC_ATVENC_BTSC, (void *)&stParam);
}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop BTSC
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_ATV_stop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_BTSC_Stop(void)
{
    return Demo_Audio_Stop(0, AUDIO_APP_ATV_BTSC);
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio
/// @param[in] vol 0~99(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAbsoluteVolume(MS_U8* vol)
{
    MS_U8 value1, value2;
    MS_U16 u16VolumeValue=0;
    MS_BOOL bMute = FALSE;

    MS_U16 u16VolumeValue_LR=0;
    MS_S16 s16LR_Compensation = AUDIO_VOL_LR_COMPENSATION;

#if (DEMO_AUDIO_HDMI_TEST == 1)
    MS_U16 u16VolumeValue_HDMI=0;
    MS_S16 s16HDMI_Compensation = AUDIO_VOL_HDMI_COMPENSATION;
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    MS_U16 u16VolumeValue_SPDIF=0;
    MS_S16 s16SPDIF_Compensation = AUDIO_VOL_SPDIF_COMPENSATION;
#endif

    if( *vol > 100 )
    {
        *vol = 100;
    }

    if(*vol == 0)
    {
        bMute = TRUE;
        Demo_Audio_SetMute(&bMute);
    }
    else
    {
        bMute = FALSE;
        Demo_Audio_SetMute(&bMute);
    }

    printf("\n[%s][%d]########## audio volume = %d ##########\n\n", __FUNCTION__, __LINE__, *vol);
    u16VolumeValue = AudioVolumeTable[*vol];

    //Line-out (LR) vol + compensation
    u16VolumeValue_LR = u16VolumeValue+s16LR_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_LR > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_LR>>8);
        value2 = (MS_U8)((u16VolumeValue_LR&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_MAIN_SPEAKER, value1, value2);

#if (DEMO_AUDIO_HDMI_TEST == 1)
    //HDMI vol + compensation
    u16VolumeValue_HDMI = u16VolumeValue+s16HDMI_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_HDMI > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {

        value1 = (MS_U8)(u16VolumeValue_HDMI>>8);
        value2 = (MS_U8)((u16VolumeValue_HDMI&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_HDMI, value1, value2);
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    //SPDIF vol + compensation
    u16VolumeValue_SPDIF = u16VolumeValue+s16SPDIF_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_SPDIF > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_SPDIF>>8);
        value2 = (MS_U8)((u16VolumeValue_SPDIF&0x00FF)>>5);
    }
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_T3_PATH_SPDIF, value1, value2);
#endif

#if (DEMO_AUDIO_BTSC_TEST == 1)
    //BTSC output to ch34 via dac circuit, the compensation is equal to Line-out
    //vol + compensation
    u16VolumeValue_LR = u16VolumeValue + s16LR_Compensation;

    //check if the volume value overflow (larger than 0x7f00)
    if( u16VolumeValue_LR > 0x7f00 )
    {
        value1 = 0x7f;
        value2 = 0x00;
    }
    else
    {
        value1 = (MS_U8)(u16VolumeValue_LR>>8);
        value2 = (MS_U8)((u16VolumeValue_LR&0x00FF)>>5);
    }

    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_BTSC1, value1, value2);
    MApi_AUDIO_SetAbsoluteVolume(AUDIO_PATH_BTSC2, value1, value2);

#endif
    return TRUE;
}

#if (DEMO_AUDIO_DMA_READER_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to do DMA Reader test
/// @param[in] pFilename input file name
/// @param[in] u8SampleRate file sampling rate
///            0: 48K
///            1: 44.1K
///            2: 32K
///            3: 24K
///            4: 22.05K
///            5: 16K
///            6: 12K
///            7: 11.025K
///            8: 8K
///            9: 4K
/// @param[in] u8RepeatTime repeat time
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_DMA_Test \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_DMA_Reader_Test(MS_U8* pFilename, MS_U8* u8SampleRate, MS_U8* u8RepeatTime)
{
    FILE *file = NULL;
    MS_U32 read_size = 0;
    MS_U32 file_size = 0;
    MS_S32 residual_size = 0;
    MS_U8* pbuf = NULL;
    MS_U32 buf_size = 4096;
    MS_U64 u64RingInbuf_size = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferSize);
    MS_U64 u64pcm_level = 0;
    MS_S8 u8loop_left = *u8RepeatTime;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;
    MS_BOOL bResend = FALSE;

    if (u64RingInbuf_size == 0)
    {
        printf("[%s][%d] Ring buffer size is 0 !!!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarNonCachedPoolID);

    pbuf = MsOS_AllocateMemory(buf_size, s32MstarNonCachedPoolID);
    if (pbuf == NULL)
    {
        printf("allocate memory fail!!!\n");
        return FALSE;
    }

    file = fopen((char*)pFilename, "rb");

    if (file == NULL)
    {
        printf("open file fail!!!\n");
        return FALSE;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);           // Get total file size
    fseek(file, 0, SEEK_SET);

    // init DMA reader
#if 1 /* darrenC */
    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
#endif
    MApi_AUDIO_DmaReader_Init(*u8SampleRate);
    printf("==========> dma reader init, smp rate is 48k\n");

    // set DMA reader command start
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DMAReader_Command, 1, 0);

    while ((u8loop_left!=0) || (residual_size!=0))
    {
        if (residual_size == 0)
        {
            fseek(file, 0, SEEK_SET);     // Seek to file begin if no data can read
            residual_size = file_size;
            //u32index=0;
            u8loop_left--;
            printf("count = %d\n", u8loop_left);
        }

        read_size = (residual_size >= buf_size) ? buf_size : residual_size;

        // check dma_reader buffer level
        u64pcm_level = (MS_U32)MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferLevel);
        printf("pcm lvl = %"DTC_MS_U64_x"\n", u64pcm_level);

        // residual buffer size > read size, keep sending PCM data to DMA reader buffer
        if ((u64RingInbuf_size - u64pcm_level) >= read_size)
        {
            if (bResend == FALSE)
            {
                // read data from usb
                read_size = fread((void *)pbuf, 1, read_size, file);
            }

            // write data to buffer
            if(MApi_AUDIO_DmaReader_WritePCM((void*)(MS_PHYADDR)(MS_VA2PA((MS_U32)pbuf)), read_size) == FALSE)
            {
                printf("write PCM ng!\n");
                bResend = TRUE;
            }
            else
            {
                residual_size -= read_size;
                bResend = FALSE;
            }
        }

        MsOS_DelayTask(20);    // for 48K ring tone
    }

    fclose(file);

    MsOS_FreeMemory(pbuf, s32MstarNonCachedPoolID);
    return TRUE;
}
#endif  //#if (DEMO_AUDIO_DMA_READER_TEST == 1)

//------------------------------------------------------------------------------
/// @brief The sample code to enable audio monitor
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Monitor \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMonitor(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    if(_audioMonitor_Task.iId == -1)
    {
        _audioMonitor_Task.pStack = MsOS_AllocateMemory(_audioMonitor_Task.u32StackSize, s32MstarCachedPoolID);
        if(!_audioMonitor_Task.pStack)
        {
            printf("allocate stack fail\n");
            return FALSE;
        }
        _audioMonitor_Task.iId = MsOS_CreateTask( _appAudioMonitor_task,
                                            NULL,
                                            _audioMonitor_Task.ePriority,
                                            TRUE,
                                            _audioMonitor_Task.pStack,
                                            _audioMonitor_Task.u32StackSize,
                                            _audioMonitor_Task.szName);

        if(_audioMonitor_Task.iId < 0)
        {
            printf("create task fail \n");
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set audio monitor debug level and enable monitor task
/// @param u8Level      \b : debug level
///                            0: Audio_DumpInfo_Off          = 0x0,
///                            1: Audio_DumpInfo_Avsync       = 0x1,
///                            2: Audio_DumpInfo_Dec1         = 0x2,
///                            4: Audio_DumpInfo_IoInfo       = 0x4,
///                            8: Audio_DumpInfo_mmFileAVsync = 0x8,
///                           21: Audio_DumpInfo_All          = 0x15,
///                           25: Audio_DumpInfo_testSTC      = 0x19,
///                          128: Audio_DumpInfo_AutoTest     = 0x80,
/// @param u16Interval  \b : print message interval
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Debug \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval)
{
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Dump_Dsp_Info, *u8Level, *u16Interval);
    if(*u8Level != 0)
    {
        Demo_Audio_SetMonitor();
    }
    else
    {
        _bExit_Monitor_Task = TRUE;
    }
    return TRUE;
}




////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set output source for main/sub path.
/// @param u8HDMI     \b : 0: main
///                        1: sub
/// @param u8LineOut  \b : 0: main
///                        1: sub
/// @param u8SPDIF    \b : 0: main
///                        1: sub
/// @param u8SCART    \b : 0: main
///                        1: sub
/// @param u8I2S      \b : 0: main
///                        1: sub
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// Command: \b Audio_OutSource_Config \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_Output_Source_Config(MS_U8* u8HDMI, MS_U8* u8LineOut, MS_U8* u8SPDIF, MS_U8* u8SCART, MS_U8* u8I2S)
{
    Output_SourceInfo.HDMIOut= *u8HDMI;
    Output_SourceInfo.SpeakerOut = *u8LineOut;
    Output_SourceInfo.SpdifOut = *u8SPDIF;
    Output_SourceInfo.ScartOut = *u8SCART;
    Output_SourceInfo.HpOut = *u8I2S;

    //Output source config (output to device)
    MApi_AUDIO_SetOutputSourceInfo(&Output_SourceInfo);

    //Execute Output Path connection
    MApi_AUDIO_SetOutConnectivity();

    return TRUE;
}

#if (DEMO_AUDIO_HDMI_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set HDMI Tx mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - 0x0 : HDMIT Tx, PCM mode
///                - 0x1 : HDMIT Tx, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: constraints on HDMI non PCM mode.
///        If set HDMI to non pcm mode, you need do the following actions. (For more
///        detail steps, please refer to the test plan.)
/// ================================================================================
///           |         48K          |        44.1K         |         32K          |
/// ================================================================================
///  Kronus   | script:              | script:              | script:              |
///  Kaiserin | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///  Kaiser   |                      | Audio_SPDIF_Mode 2   | Audio_SPDIF_Mode 2   |
///           |                      |                      |                      |
///           | (HDMI, SPDIF can be  | (If set HDMI to non  | (If set HDMI to non  |
///           | different mode)      | PCM mode, need set   | PCM mode, need set   |
///           |                      | SPDIF to non PCM     | SPDIF to non PCM     |
///           |                      | mode)                | mode)                |
/// ================================================================================
///  Kappa    | script:              | script:              | script:              |
///  Keltic   | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///  Kenya    |                      |                      |                      |
///  Kriti    | (If set HDMI to non  | (If set HDMI to non  | (If set HDMI to non  |
///           | PCM mode, SPDIF will | PCM mode, SPDIF will | PCM mode, SPDIF will |
///           | change to non PCM)   | change to non PCM)   | change to non PCM)   |
/// ================================================================================
///  Keres    | script:              | script:              | script:              |
///  Clippers | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  | Audio_HDMITx_Mode 1  |
///           |                      |                      |                      |
///           | (HDMI, SPDIF can be  | (HDMI, SPDIF can be  | (HDMI, SPDIF can be  |
///           | different mode)      | different mode)      | different mode)      |
/// ================================================================================
///
/// Command: \b Audio_HDMITx_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_HDMITx_Mode(MS_U8* u8Mode)
{
    /////////////////////////////////////////////////////////////////
    /// Steps for implement a switch HDMI_Tx pcm/nonpcm function
    /////////////////////////////////////////////////////////////////

    //step1: set pcm/nonpcm mode
    MApi_AUDIO_HDMI_TX_SetMode(*u8Mode);

    //step2: add a HDMI_Tx status monitor in the monitor loop,
    //       please refer to the _appDemo_HDMI_TX_Status_Monitor() in _appAudioMonitor_task()

    return TRUE;
}

MS_BOOL Demo_Audio_DDP(MS_U8* u8Mode)
{
#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
    if(_bFMTX_enable || _bBTSC_enable || _bPCM_CAPTURE_enable || _bDSP2_Decode)
    {
        printf("\nWarning!!! DDP bypass NOT allowed due to ATV, PCM capture or SUB AV is used!!!\n\n");
        return FALSE;
    }
    else
#endif
    {
        MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, *u8Mode, 0);      //enable
        return TRUE;
    }
}
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set S/PDIF mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x1 : SPDIF off
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note: Compare with HDMI, no additional actions needs for SPDIF non PCM mode.
///        (For more detail steps, please refer to the test plan.)
///
/// Command: \b Audio_SPDIF_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_SPDIF_Mode(MS_U8* u8Mode)
{
    MApi_AUDIO_SPDIF_SetMode(*u8Mode);
    return TRUE;
}
#endif

#if (DEMO_AUDIO_SOUND_EFFECT_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to set the prescale value
/// @param u8PreScale  \b : range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                       - 0x00: disable pre-scale
///                       - 0x6F: gain = 0db
///                       - 0xFF: gain = +18db
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_PreScale \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetPreScale(MS_U8* u8PreScale)
{

    MS_U32 p_param = 0;
    p_param = ((*u8PreScale << 8) | (AUDIO_PATH_MAIN));
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetPreScale, &p_param);

/*
    MApi_AUDIO_SetPreScale(AUDIO_PATH_MAIN, *u8PreScale);
*/

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the bass value
/// @param u8Bass      \b :Bass Level (0~100) mapping to -16 ~ +15 dB Gain
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Bass_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetBass_Param(MS_U8* u8Bass)
{
/*
    MApi_AUDIO_SetBass(*u8Bass);
    MApi_AUDIO_EnableTone(TRUE);
*/
    MS_U32 p_param = *u8Bass;
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetBass, &p_param);
    //MApi_SND_ProcessEnable(Sound_ENABL_Type_Tone, TRUE);


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set the treble value
/// @param u8Treble      \b :Bass Level (0~100) mapping to -16 ~ +15 dB Gain
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_Treble_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetTreble_Param(MS_U8* u8Treble)
{
/*
    MApi_AUDIO_SetTreble(*u8Treble);
    MApi_AUDIO_EnableTone(TRUE);
*/
    MS_U32 p_param = *u8Treble;
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetTreble, &p_param);

    //MApi_SND_ProcessEnable(Sound_ENABL_Type_Tone, TRUE);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the bass/treble function
/// @param bEnable      \b : enable/disable the bass/treble
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_Bass_Treble_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetBassTreble_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_Tone, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable auto volume control
/// @param <IN>        \b u8AvcMode    :  AVC u8Mode
///                                    - 0: L mode
///                                    - 1: S mode
///                                    - 2: M mode
/// @param Level       \b :  AVC threshold level
///                       - 0x00--  0    dBFS
///                       - 0x01--  -0.5    dBFS
///                       - 0x20--  -16    dBFS
///                       - 0x50--  -40    dBFS
/// @param AvcAT       \b :  AVC attack time
///                       - AvcAT = 0 --> 20  ms
///                       - AvcAT = 1 --> 100 ms
///                       - AvcAT = 2 --> 200 ms
///                       - AvcAT = 3 --> 1 sec
/// @param AvcRT       \b :  AVC release time
///                       - AvcRT = 0 --> 1 sec
///                       - AvcRT = 1 --> 2 sec
///                       - AvcRT = 2 --> 100 ms
///                       - AvcRT = 3 --> 200 ms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_AVC_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAVC_Param(MS_U8* AvcMode, MS_U8* Level, MS_U8* AvcAT, MS_U8* AvcRT)
{

/*
    MApi_AUDIO_SetAvcMode( *AvcMode );
    MApi_AUDIO_SetAvcThreshold(*Level);
    MApi_AUDIO_SetAvcAT( *AvcAT );
    MApi_AUDIO_SetAvcRT( *AvcRT );
    MApi_AUDIO_EnableAutoVolume(*bEnAVC);
*/

    MS_U32 p_param = 0;
    p_param = ((*AvcRT << 24) | (*AvcAT << 16) | (*AvcMode << 8) | (*Level));
    MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetAvc, &p_param);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the AVC function
/// @param bEnable      \b : enable/disable the AVC function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_AVC_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetAVC_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_AutoVolume, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable parameter equalizer
/// @param <IN>         \b Band: 0~4
///                     \b Gain: 0~240, -12db ~ +12 db
///                     \b Foh: 1~160
///                     \b Fol: 0~99
///                     \b QValue: 5~160
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_PEQ_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetPEQ_Param(MS_U8* Band, MS_U8* Gain, MS_U16* Fc, MS_U8* QValue)
{
    float coef;
    float v0, dem;
    float G, fc = *Fc, Q;

    float k;
    float kpow2;
    AUDIO_PEQ_COEF PEQCoef;
    MS_BOOL error_config = FALSE;

    if(*Gain > 240)
    {
        printf("%s err! PEQ Gain should be 0~240\n", __FUNCTION__);
        error_config = TRUE;
    }

    if(*Fc<50)
    {
        printf("%s err! PEQ fc should be > 50\n", __FUNCTION__);
        error_config = TRUE;
    }

/*
    if(*Fol > 99)
    {
        printf("%s err! PEQ Fol should be 0~99\n", __FUNCTION__);
        error_config = TRUE;
    }

    if((*Foh== 0) && (*Fol < 50))
    {
         printf("%s err! PEQ Fo should be > 50\n", __FUNCTION__);
        error_config = TRUE;
    }
*/

    if((*QValue < 5) || (*QValue > 160))
    {
        printf("%s err! PEQ QValue should be 5~160\n", __FUNCTION__);
        error_config = TRUE;
    }

    if(error_config == TRUE)
    {
        return FALSE;
    }

    PEQCoef.band = *Band;
    //fc = (float)(((int)*Foh) * 100 + (int)*Fol);

    for(PEQCoef.sfs = 0; PEQCoef.sfs <= 1; PEQCoef.sfs++)
    {
        if(PEQCoef.sfs)
        {
            //AUD_DEBUG(printf("48k: %x\r\n",1));
            k = tan((3.1415926 * fc) / 48000);
        }
        else
        {
            //AUD_DEBUG(printf("32k: %x\r\n",0));
            k = tan((3.1415926 * fc) / 32000);
        }

        Q = ((float)*QValue) / 10;

        if(*Gain >= 120)
        {
            G = ((float)(*Gain - 120)) / 10;
        }
        else
        {
            G = ((float)(120 - *Gain)) / 10;
        }

        //fc = (float)(((int)*Foh) * 100 + (int)*Fol);

        kpow2 = k * k;

        v0 = powf(10.0, (G / 20.0));

        if(*Gain >= 120)
        {
            //Peak,boost
            dem = 1 + k / Q + kpow2;
            coef = (1 + v0 * k / Q + kpow2) / dem;
            PEQCoef.a0 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.a1 = (long)(coef * 4194304);
            coef = (1 - v0 * k / Q + kpow2) / dem;
            PEQCoef.a2 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.b1 = (long)(-coef * 4194304);
            coef = (1 - k / Q + kpow2) / dem;
            PEQCoef.b2 = (long)(-coef * 4194304);
        }
        else
        {
            //peak,cut
            dem = 1 + v0 * k / Q + kpow2;
            coef = (1 + k / Q + kpow2) / dem;
            PEQCoef.a0 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.a1 = (long)(coef * 4194304);
            coef = (1 - k / Q + kpow2) / dem;
            PEQCoef.a2 = (long)(coef * 4194304);
            coef = 2 * (kpow2 - 1) / dem;
            PEQCoef.b1 = (long)(-coef * 4194304);
            coef = (1 - v0 * k / Q + kpow2) / dem;
            PEQCoef.b2 = (long)(-coef * 4194304);
        }

        MApi_SND_ProcessEnable(Sound_ENABL_Type_PEQ, FALSE);

        if(PEQCoef.sfs)
        {
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_A0, (PEQCoef.band << 8) + ((PEQCoef.a0 >> 16) & 0xFF), (PEQCoef.a0 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_A1, (PEQCoef.band << 8) + ((PEQCoef.a1 >> 16) & 0xFF), (PEQCoef.a1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_A2, (PEQCoef.band << 8) + ((PEQCoef.a2 >> 16) & 0xFF), (PEQCoef.a2 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_B1, (PEQCoef.band << 8) + ((PEQCoef.b1 >> 16) & 0xFF), (PEQCoef.b1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_48K_B2, (PEQCoef.band << 8) + ((PEQCoef.b2 >> 16) & 0xFF), (PEQCoef.b2 & 0xFFFF));
        }
        else
        {
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_A0, (PEQCoef.band << 8) + ((PEQCoef.a0 >> 16) & 0xFF), (PEQCoef.a0 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_A1, (PEQCoef.band << 8) + ((PEQCoef.a1 >> 16) & 0xFF), (PEQCoef.a1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_A2, (PEQCoef.band << 8) + ((PEQCoef.a2 >> 16) & 0xFF), (PEQCoef.a2 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_B1, (PEQCoef.band << 8) + ((PEQCoef.b1 >> 16) & 0xFF), (PEQCoef.b1 & 0xFFFF));
            MApi_SND_SetParam1(Sound_SET_PARAM_PEQ_32K_B2, (PEQCoef.band << 8) + ((PEQCoef.b2 >> 16) & 0xFF), (PEQCoef.b2 & 0xFFFF));
        }

    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the PEQ function
/// @param bEnable      \b : enable/disable the PEQ function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_PEQ_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetPEQ_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_PEQ, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable graphic Equalizer
/// @param u8band      \b :  EQ band 0~4
/// @param u8level     \b :  Absolute EQ register value  ;
///                       - 0011-0000: +12.00db. (Max)
///                       - 0000-0001: +0.25db.
///                       - 0000-0000: +0.00db.
///                       - 1111-1111: -0.25db.
///                       - 1101-0000: -12.00db. (Min)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_GEQ_Param \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetGEQ_Param(MS_U8* u8band, MS_U8* u8level)
{
/*
    MApi_AUDIO_SetEq(*u8band, *u8level);
*/

    MS_U32 p_param = 0;
    p_param = ((*u8level << 8) | (*u8band));
    return MApi_AUDIO_SOUND_SetParam(Sound_SET_Type_SetEq, &p_param);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the GEQ function
/// @param bEnable      \b : enable/disable the GEQ function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_GEQ_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetGEQ_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_EQ, *bEnable);
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable High Pass Filter
/// @param bEnable       \b : TRUE --Enable HPF;
///                           FALSE--Disable HPF
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_HPF_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetHPF_Enable(MS_BOOL* bEnable)
{
    return MApi_SND_ProcessEnable(Sound_ENABL_Type_Hpf, *bEnable);
}

#endif

#if (DEMO_AUDIO_SOUND_EFFECT_ADV_TEST == 1)
////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to enable TSHD.
///                        TSHD contains 4 moudle:
///                        (1) TruSurround
///                        (2) TruBass
///                        (3) Definition
///                        (4) Dialog Clarity
///
/// @param u8SpeakerSize \b : 0:40Hz
///                           1:60Hz
///                           2:100Hz
///                           3:150Hz
/// @param u8Bass        \b : level 0 ~ 20
/// @param u8Definition  \b : level 0 ~ 20
/// @param u8Surround    \b : level 0 ~ 20
/// @param u8InputGain   \b : level 0 ~ 20
/// @param u8OutputGain  \b : level 0 ~ 16
///
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
///
/// Command: \b Audio_SetTSHD \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Demo_Audio_Set_TSHD_Param(MS_U8* u8SpeakerSize, MS_U8* u8Bass, MS_U8* u8Definition, MS_U8* u8Surround, MS_U8* u8InputGain, MS_U8* u8OutputGain)
{

    if(*u8SpeakerSize>3)
    {
        printf("Invalid param1, Speaker Size must be 0~3, 0:40Hz 1:60Hz 2:100Hz 3:150Hz\n");
        return FALSE;
    }
    else if(*u8Bass>20)
    {
        printf("Invalid param2, TruBass level must be 0~20\n");
        return FALSE;
    }
    else if(*u8Definition>20)
    {
        printf("Invalid param3, definition level must be 0~20\n");
        return FALSE;
    }
    else if(*u8Surround>20)
    {
        printf("Invalid param4, Surround level must be 0~20\n");
        return FALSE;
    }
    else if(*u8InputGain>20)
    {
        printf("Invalid param5, Input Gain must be 0~20\n");
        return FALSE;
    }
    else if(*u8OutputGain>16)
    {
        printf("Invalid param6, Output Gain must be 0~16\n");
        return FALSE;
    }
    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SPEAKERSIZE, MS_U16 u16value1, 0);        //u16value1:0~3 0:40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SPEAKERSIZE, *u8SpeakerSize, 0);
    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_TRUBASS_CONTROL, MS_U16 u16value1, 0);    //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_TRUBASS_CONTROL, *u8Bass, 0);

    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_DEFINITION_CONTROL, MS_U16 u16value1, 0); //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_DEFINITION_CONTROL, *u8Definition, 0);

    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SURROUND_LEVEL, MS_U16 u16value1, 0);     //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_SURROUND_LEVEL, *u8Surround, 0);

    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_INPUT_GAIN, MS_U16 u16value1, 0);         //u16value1:0~20
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_INPUT_GAIN, *u8InputGain, 0);
    //MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_OUTPUT_GAIN, MS_U16 u16value1, 0);        //u16value1:0~16
    MApi_AUDIO_ADVSOUND_SetParam(SRS_TSHD_SET_OUTPUT_GAIN, *u8OutputGain, 0);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to enable/disable the TSHD function
/// @param bEnable      \b : enable/disable the TSHD function
///                        - 0 : disable
///                        - 1 : enable
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @note
/// Command: \b Audio_TSHD_Enable \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Set_TSHD_Enable(MS_BOOL* bEnable)
{
    ADVSND_TYPE bSRS_Type = ADV_NONE;
    if(*bEnable)
    {
        bSRS_Type = SRS_TSHD;
    }
    else
    {
        bSRS_Type = ADV_NONE;
    }

    if (MApi_AUDIO_ADVSOUND_ProcessEnable(bSRS_Type) == FALSE)
    {
        printf("SRS_TSHD Enable/Disable process fail\n");
        return FALSE;
    }
    else if(MApi_AUDIO_ADVSOUND_SubProcessEnable(SRS_TSHD_TRUBASS, *bEnable) == FALSE)
    {
        printf("SRS_TSHD_TRUBASS Enable/Disable process fail\n");
        return FALSE;
    }
    else if(MApi_AUDIO_ADVSOUND_SubProcessEnable(SRS_TSHD_DC, *bEnable) == FALSE)
    {
        printf("SRS_TSHD_DC Enable/Disable process fail\n");
        return FALSE;
    }
    else if(MApi_AUDIO_ADVSOUND_SubProcessEnable(SRS_TSHD_DEFINITION, *bEnable) == FALSE)
    {
        printf("SRS_TSHD_DEFINITION Enable/Disable process fail\n");
        return FALSE;
    }

    return TRUE;

}

#endif

#if (DEMO_AUDIO_AD_TEST == 1)
//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio description
/// @param[in] vol 0~100(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume_ad \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetADAbsoluteVolume(MS_U8* vol)
{
    MS_U16 u16VolumeValue = 0;
    MS_U8 value1 = 0;
    u16VolumeValue = AudioVolumeTable[*vol];
    value1 = (MS_U8)(u16VolumeValue >> 8);
    MApi_AUDIO_SetADAbsoluteVolume(value1);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute_ad \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_ADSetMute(MS_BOOL* bEnable)
{
    //mute AD
    MApi_AUDIO_ADSetMute(*bEnable);

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief The sample code to set audio output mode
/// @param mode
///     0:STEREO     (MSAPI_AUD_MODE_STEREO)
///     1: LL        (MSAPI_AUD_MODE_LL)
///     2: RR        (MSAPI_AUD_MODE_RR)
///     3: L+R mix   (MSAPI_AUD_MODE_MUTE)
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_SetOutputMode \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetOutputMode(MS_U32* mode)
{
    MS_BOOL ret;

    ret = MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, *mode, 0);

    return ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMute(MS_BOOL* bEnable)
{
    //mute line out
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, *bEnable);
#if (DEMO_AUDIO_HDMI_TEST == 1)
    //mute HDMI PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, *bEnable);

    //mute HDMI PCM + non PCM
    MApi_AUDIO_HDMI_Tx_SetMute(*bEnable);
#endif

#if (DEMO_AUDIO_SPDIF_TEST == 1)
    //mute SPDIF PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, *bEnable);

    //mute SPDIF PCM + non PCM
    MApi_AUDIO_SPDIF_SetMute(*bEnable);
#endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code mute by path
/// @param[in] u8Path
///     0: main path
///     1: sub path
/// @param[in] u8Mute
///     0: unmute
///     1: mute
/// @return none.
/// Command: \b Audio_mute_path \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_SetMutePath(MS_U8* u8Path, MS_U8* u8Mute)
{
    MS_BOOL bConnect = FALSE;
    if(*u8Mute)
    {
        bConnect=0;
    }
    else
    {
        bConnect=1;
    }

    MApi_AUDIO_SourceConnect(*u8Path, bConnect);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to stop music
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_demoStop \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Stop_Demo(void)
{
    MS_BOOL bRet = FALSE;
    memset((void *)gau32clipWriteLastAddr, 0, sizeof(MS_U32)*E_DECODER_NUMBER);
    memset((void *)gau32AudioFileIndex, 0, sizeof(MS_U32)*E_DECODER_NUMBER);
    bRet = Demo_Audio_Stop(0, AUDIO_APP_ES_PLAY);
    _bExit_Demo_Task = TRUE;
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize audio function
/// @param[in] Init type
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_Init \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Init(MS_U32 *Type)
{
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;
    AUDIO_SOURCE_INFO_TYPE eSrcType;
    EN_AUDIO_APP_TYPE eType;

    printf("%s\n", __FUNCTION__);

    eType = *(EN_AUDIO_APP_TYPE *)Type;
    if(_Demo_Audio_CheckAppSupport(eType) == FALSE)
    {
        printf("unsupport application type: %u\n", (unsigned int)eType);
        return FALSE;
    }

    MApi_AUDIO_WritePreInitTable();

    //======================================
    // Set system info
    //======================================
    SystemInfo.tv_system=TV_PAL;
    SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
    SystemInfo.dtv_system=0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    //======================================
    // Set output info(Output data from)
    //======================================
    OutputInfo.SpeakerOut = AUDIO_OUTPUT_MAIN_SPEAKER;
    OutputInfo.HpOut = AUDIO_OUTPUT_I2S;    //K3 has no HP, so used for I2S
    OutputInfo.ScartOut = AUDIO_NULL_OUTPUT;
    OutputInfo.MonitorOut = AUDIO_NULL_OUTPUT;
#if (DEMO_AUDIO_SPDIF_TEST == 1)
    OutputInfo.SpdifOut = AUDIO_OUTPUT_SPDIF;
#else
    OutputInfo.SpdifOut = AUDIO_NULL_OUTPUT;
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
    OutputInfo.ArcOut = AUDIO_OUTPUT_ARC;
    OutputInfo.HDMIOut = AUDIO_OUTPUT_HDMI;
#else
    OutputInfo.ArcOut = AUDIO_NULL_OUTPUT;
    OutputInfo.HDMIOut = AUDIO_NULL_OUTPUT;
#endif

    MApi_AUDIO_SetOutputInfo(&OutputInfo);

    //======================================
    // Set path info (volume path control)
    //======================================
    PathInfo.SpeakerOut = AUDIO_PATH_MAIN_SPEAKER;  //path4(main)
    PathInfo.HpOut = AUDIO_PATH_I2S;  //K3 has no HP, so used for I2S
    PathInfo.MonitorOut = AUDIO_PATH_NULL;
    PathInfo.ScartOut = AUDIO_PATH_NULL;
#if (DEMO_AUDIO_SPDIF_TEST == 1)
    PathInfo.SpdifOut = AUDIO_PATH_SPDIF;
#else
    PathInfo.SpdifOut = AUDIO_PATH_NULL;
#endif

#if (DEMO_AUDIO_HDMI_TEST == 1)
    PathInfo.ArcOut = AUDIO_PATH_SPDIF;
    PathInfo.HDMIOut = AUDIO_PATH_HDMI;
#else
    PathInfo.ArcOut = AUDIO_PATH_NULL;
    PathInfo.HDMIOut = AUDIO_PATH_NULL;
#endif
    MApi_AUDIO_SetPathInfo(&PathInfo);

    //======================================
    MDrv_AUDIO_SetDspBaseAddr(DSP_ADV,  0, MEM_ADR_BY_MIU(MAD_ADV_BUF_ADR, MAD_ADV_BUF_MEMORY_TYPE));

    MApi_AUDIO_Initialize();

    //======================================
    // Set source type
    //======================================
    switch(eType)
    {
    case AUDIO_APP_ES_PLAY:
        eSrcType = E_AUDIO_INFO_GAME_IN;
        break;
    case AUDIO_APP_DTV:
    case AUDIO_APP_DTV_AM:
    case AUDIO_APP_DTV_AD:
        eSrcType = E_AUDIO_INFO_DTV_IN;
        break;
    default:
        eSrcType = E_AUDIO_INFO_GAME_IN;
    }

    MApi_AUDIO_SetSourceInfo(eSrcType);

    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);

#ifdef AUDIO_MUTE_GPIO
    AUDIO_MUTE_SET_GPIO_UP(AUDIO_MUTE_GPIO);
#endif

    /* release decode system */
    _Demo_Audio_CloseDecSys(0);
    _Demo_Audio_CloseDecSys(1);

    /* init decoder system for main */
    if(_Demo_Audio_OpenDecSys(0, eType, MSAPI_AUD_DVB_MPEG) == FALSE)
    {
        printf("[%s][%d] open decode system (0) fail.\n", __FUNCTION__, __LINE__);
    }

    if(_Demo_Audio_OpenDecSys(1, eType, MSAPI_AUD_DVB_MPEG) == FALSE)
    {
        printf("[%s][%d] open decode system (1) fail.\n", __FUNCTION__, __LINE__);
    }

    //set default channel path, or set output source may be fail
    //DSP1 -> main channel (ch5)
    //DSP2 -> sub channel (ch7)
    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
    MApi_AUDIO_InputSwitch(AUDIO_DSP3_DVB_INPUT, E_AUDIO_GROUP_SUB);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Start audio dec play
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @param[in] eCodecType
///     codec type
/// @param[in] pvParam
///     input parameter (depend on codec type)
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Play(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType, EN_AUDIO_CODEC_TYPE eCodecType, void *pvParam)
{
    En_DVB_decSystemType eDecSysType;

    if(u32DevIdx >= E_DECODER_NUMBER)
    {
        //printf("unsupport device index: %u\n", (unsigned int)u32DevIdx);
        return FALSE;
    }

    if(_Demo_Audio_CheckAppSupport(eAppType) == FALSE)
    {
        //printf("unsupport application type: %u\n", (unsigned int)eAppType);
        return FALSE;
    }

    eDecSysType = _Demo_Audio_CodecTypeMap(eCodecType);

    if(eDecSysType == MSAPI_AUD_DVB_INVALID)
    {
        return FALSE;
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
    if(_Demo_DDP_BYPASS_Check_Allow_Feature(u32DevIdx, eAppType) == FALSE)
    {
        printf("\nWarning!!! ATV, PCM capture or SUB AV NOT allowed due to DDP bypass is used!!!\n\n");
        return FALSE;
    }
#endif

    _Demo_Audio_SetSystem(u32DevIdx, eDecSysType);

    switch(eAppType)
    {
    case AUDIO_APP_ES_PLAY:
        if(eDecSysType == MSAPI_AUD_DVB_XPCM)
        {
            MApi_AUDIO_SetAudioParam2(stDualDemoInfo[u32DevIdx].eDecID, Audio_ParamType_setXPCMDecParam, (MS_U32)pvParam);
        }

        if(_bNew_MM_Mode[u32DevIdx] == TRUE)
        {
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        else
        {
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYFILE);
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        }
        break;
    case AUDIO_APP_DTV:
        if(_Demo_Audio_STC_Switch(u32DevIdx, *(AUDIO_STC_SOURCE *)pvParam) == FALSE)
        {
            printf("[%s][%d] dev_idx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
        }

        MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_AVSYNC);
        MApi_AUDIO_SourceConnect(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eGroup, FALSE);
        break;
    case AUDIO_APP_DTV_AM:
        if(_Demo_Audio_STC_Switch(u32DevIdx, *(AUDIO_STC_SOURCE *)pvParam) == FALSE)
        {
            printf("[%s][%d] dev_idx=%u, set STC fail!!!\n",__FUNCTION__,__LINE__, (unsigned int)u32DevIdx);
        }
        MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAYFILETSP);
        MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        MApi_AUDIO_SourceConnect(stDualDemoInfo[u32DevIdx].stAudioDecStatus_t.eGroup, FALSE);
        break;
#if (DEMO_AUDIO_AD_TEST == 1)
    case AUDIO_APP_DTV_AD:
        MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_PLAY_AD);
        break;
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
    case AUDIO_APP_ATV_FMTX:
        {
            ST_AUDIO_FMTX_PARAM *stFMTX = (ST_AUDIO_FMTX_PARAM *)pvParam;
            MS_U8 u8Filter = stFMTX->u8Filter;
            MS_U8 u8Attenuation_H = (MS_U8)((stFMTX->u16Attenuation) >> 8);
            MS_U8 u8Attenuation_L = (MS_U8)((stFMTX->u16Attenuation) & 0xFF);
            MS_U8 u8OutScaling_H = (MS_U8)((stFMTX->u16OutScaling) >> 8);
            MS_U8 u8OutScaling_L = (MS_U8)((stFMTX->u16OutScaling) & 0xFF);

            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);

            if(u8Filter > 1)
            {
                u8Filter = 1;
            }

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP, u8Filter, 0);

            //start encode
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);

        }
        break;
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
    case AUDIO_APP_ATV_BTSC:
        {
            ST_AUDIO_BTSC_PARAM *stBTSC = (ST_AUDIO_BTSC_PARAM *)pvParam;
            MS_U8 u8Attenuation_H = (stBTSC->u16Attenuation) >> 8;
            MS_U8 u8Attenuation_L = (stBTSC->u16Attenuation) & 0xFF;
            MS_U8 u8OutScaling_H = (stBTSC->u16OutScaling) >> 8;
            MS_U8 u8OutScaling_L = (stBTSC->u16OutScaling) & 0xFF;
            MS_U8 u8_M_Gain_H = (stBTSC->u16_M_Gain) >> 8;
            MS_U8 u8_M_Gain_L = (stBTSC->u16_M_Gain) & 0xFF;
            MS_U8 u8_D_Gain_H = (stBTSC->u16_D_Gain) >> 8;
            MS_U8 u8_D_Gain_L = (stBTSC->u16_D_Gain) & 0xFF;
            MS_U8 u8_SAP_Gain_H = (stBTSC->u16_SAP_Gain) >> 8;
            MS_U8 u8_SAP_Gain_L = (stBTSC->u16_SAP_Gain) & 0xFF;

            MApi_AUDIO_SetSystem(eDecSysType);

            //disable Dolby SPDIF volume control, when BTSC start
            MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain, u8_M_Gain_H, u8_M_Gain_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain, u8_D_Gain_H, u8_D_Gain_L);
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain, u8_SAP_Gain_H, u8_SAP_Gain_L);

            //used for keres and later BTSC supported CHIPs. Kaiser will ignore the setting, it hard code in the DSP.
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_BTSC1);
            MDrv_AUDIO_SetInternalPath(INTERNAL_ATVENC, AUDIO_OUTPUT_BTSC2);

            //start encode
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);

        }
        break;
#endif
    default:
            return FALSE;
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
    _Demo_DDP_BYPASS_Set_Feature_Status(u32DevIdx, eAppType);
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Stop audio dec
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[in] eAppType
///     0: ES play (build-in audio test)
///     1: DTV
///     2: DTV AD
///     3: FMTX
///     4: BTSC
/// @return TRUE: success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Stop(MS_U32 u32DevIdx, EN_AUDIO_APP_TYPE eAppType)
{
    MS_BOOL bMute = TRUE;

    if(u32DevIdx >= E_DECODER_NUMBER)
    {
        //printf("unsupport device index: %u\n", (unsigned int)u32DevIdx);
        return FALSE;
    }

    if(_Demo_Audio_CheckAppSupport(eAppType) == FALSE)
    {
        //printf("unsupport application type: %u\n", (unsigned int)eAppType);
        return FALSE;
    }

    //if(MDrv_AUDIO_GET_INIT_FLAG())
    {
        switch(eAppType)
        {
        case AUDIO_APP_ES_PLAY:
        case AUDIO_APP_DTV:
        case AUDIO_APP_DTV_AM:
            Demo_Audio_SetMutePath((MS_U8*)&u32DevIdx,&bMute);
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_STOP);
            break;
#if (DEMO_AUDIO_AD_TEST == 1)
        case AUDIO_APP_DTV_AD:
            MApi_AUDIO_SetDecodeCmd(stDualDemoInfo[u32DevIdx].eDecID, MSAPI_AUD_DVB_DECCMD_STOP_AD);
            MApi_AUDIO_SetADOutputMode(AD_OUT_NONE);
            break;
#endif
#if (DEMO_AUDIO_FMTX_TEST == 1)
        case AUDIO_APP_ATV_FMTX:
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);
            break;
#endif
#if (DEMO_AUDIO_BTSC_TEST == 1)
        case AUDIO_APP_ATV_BTSC:
            MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);
            break;
#endif
        default:
            return FALSE;
        }
    }

#if (DEMO_AUDIO_DDP_BYPASS_V1 == 1)
    _Demo_DDP_BYPASS_Clear_Feature_Status(u32DevIdx, eAppType);
#endif

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief Return a copy of audio dec info
/// @param[in] u32DevIdx
///     0: main
///     1: sub
/// @param[out] pstInfo
///     pointer to output buffer
/// @return TRUE: success.
/// @return FALSE: Invalid input argument.
/// @sa
/// @note
/// Command: none
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_GetDecInfo(MS_U32 u32DevIdx, ST_AUDIO_DEC_INFO *pstInfo)
{
    if((u32DevIdx >= E_DECODER_NUMBER) || (pstInfo == NULL))
    {
        return FALSE;
    }

    memcpy((void *)pstInfo, (void *)&stDualDemoInfo[u32DevIdx], sizeof(ST_AUDIO_DEC_INFO));

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to playback music
/// @param[in] pFileFmt
///     0: mp3
///     1: wav
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_demo \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Out_Init(MS_U32 *pFileFmt)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;
    MS_U32 u32DevIdx=0;
    EN_AUDIO_APP_TYPE eAppType;
    EN_AUDIO_CODEC_TYPE eCodecType;
    void *pvParam = NULL;
    Audio_XPCM_Param stXPCM={};

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    //audio init need be called at the beginning
    eAppType = AUDIO_APP_ES_PLAY;
    Demo_Audio_Init((MS_U32 *)&eAppType);

    if(_audioDemo_Task.iId == -1)
    {
        _audioDemo_Task.pStack = MsOS_AllocateMemory(_audioDemo_Task.u32StackSize, s32MstarCachedPoolID);
        if(!_audioDemo_Task.pStack)
        {
            printf("fail 1\n");
            return FALSE;
        }

        /// - Create Task Application
        _audioDemo_Task.iId = MsOS_CreateTask( _appAudioDemo_task,
                                            NULL,
                                            _audioDemo_Task.ePriority,
                                            TRUE,
                                            _audioDemo_Task.pStack,
                                            _audioDemo_Task.u32StackSize,
                                            _audioDemo_Task.szName);
        if(_audioDemo_Task.iId < 0)
        {
            printf("fail 2\n");
            return FALSE;
        }
    }

    if(*pFileFmt == 0)
    {
        //demo code for playing mp3 with using old MM mode + main path
        u32DevIdx = 0;
        eCodecType = AUDIO_CODEC_MP3;

        gau32clipWriteLastAddr[u32DevIdx]=0;
        gau32AudioFileIndex[u32DevIdx]=0;
        gapu8AudioData[u32DevIdx] = mp3_bin_var;
        gau32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = FALSE;
    }
    else if(*pFileFmt == 1)
    {
        //demo code for playing wav with using old MM mode + main path
        u32DevIdx = 0;
        eCodecType = AUDIO_CODEC_XPCM;

        stXPCM.audioType = LPCM;
        memcpy(&stXPCM.channels, wav_bin_var + 22, 2);//set channel num
        memcpy(&stXPCM.sampleRate, wav_bin_var+ 24, 4);//set sampling rate
        memcpy(&stXPCM.bitsPerSample, wav_bin_var + 34, 2);//set bit per sample
        memcpy(&stXPCM.blockSize, wav_bin_var + 32, 2);//set block size

        gau32clipWriteLastAddr[u32DevIdx]=0;
        gau32AudioFileIndex[u32DevIdx]=0;
        gapu8AudioData[u32DevIdx] = wav_bin_var;
        gau32AudioLen[u32DevIdx] = WAV_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = FALSE;
        pvParam = (void *)&stXPCM;
    }
    else if(*pFileFmt == 2)
    {
        //demo code for playing mp3 with using old MM mode + sub path
        u32DevIdx = 1;
        eCodecType = AUDIO_CODEC_MP3;

        gau32clipWriteLastAddr[u32DevIdx]=0;
        gau32AudioFileIndex[u32DevIdx]=0;
        gapu8AudioData[u32DevIdx] = mp3_bin_var;
        gau32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = FALSE;
    }
    else if(*pFileFmt == 3)
    {
        //demo code for playing mp3 with using new MM mode + main path
        u32DevIdx = 0;
        eCodecType = AUDIO_CODEC_MP3;
        MApi_AUDIO_MM2_initAesInfo(stDualDemoInfo[u32DevIdx].eDecID);
        gau32clipWriteLastAddr[u32DevIdx]=0;
        gau32AudioFileIndex[u32DevIdx]=0;
        gapu8AudioData[u32DevIdx] = mp3_bin_var;
        gau32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = TRUE;
    }
    else if(*pFileFmt == 4)
    {
        //demo code for playing mp3 with using new MM mode + sub path
        u32DevIdx = 1;
        eCodecType = AUDIO_CODEC_MP3;
        MApi_AUDIO_MM2_initAesInfo(stDualDemoInfo[u32DevIdx].eDecID);
        gau32clipWriteLastAddr[u32DevIdx]=0;
        gau32AudioFileIndex[u32DevIdx]=0;
        gapu8AudioData[u32DevIdx] = mp3_bin_var;
        gau32AudioLen[u32DevIdx] = MP3_BIN_SIZE;
        _bNew_MM_Mode[u32DevIdx] = TRUE;
    }
    else
    {
        return FALSE;
    }

#if 0
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif

	Demo_Audio_Play(u32DevIdx, eAppType, eCodecType, (void *)pvParam);

    bMute = FALSE;
    Demo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    Demo_Audio_SetAbsoluteVolume(&u8Volume);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Show all demo functions of audio
/// @param none
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b HelpAudio \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Audio_Help(void)
{
    printf("*************************Audio function List*************************\n");
    printf("1.Audio_Init --Initial Audio\n");
    printf("2.Audio_demo --Audio demo MP3\n");
    printf("3.Audio_demoStop -- Stop audio demo\n");
    printf("4.Audio_volume -- Volume control; 0~100\n");
    printf("5.Audio_mute -- mute control\n");


    return TRUE;
}
#endif


