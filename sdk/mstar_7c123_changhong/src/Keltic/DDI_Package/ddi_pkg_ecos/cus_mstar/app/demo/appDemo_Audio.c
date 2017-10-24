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
/// @file   appDemo_Audio.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for audio functions
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include <string.h>
#include "MsCommon.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "drvGPIO.h"
#include "MsMemory.h"

#include "bindata/JAPAN_48_2_mp3.h"
#include "bindata/WAV_44K.h"

#include "appDemo_Audio.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
static Task_Info _audioDemo_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Demo Task"};
static Task_Info _audioMonitor_Task = {-1, E_TASK_PRI_HIGH, NULL, 4096, "Audio Monitor"};
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
} STWAVINFO;


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S32 gs32CachedPoolID;
unsigned char *pAudioData = NULL;
MS_S32 AudioLen = 0;
static MS_U32 _clipWriteLastAddr=0, _u32AudioFileIndex=0;


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


void appDemo_Audio_File_play(MS_U32 writeAddress, MS_U32 writeNumber);

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static void _appAudioDemo_task(MS_U32 argc, void *argv)
{
    MS_U32 WrtAddr,WrtBytes;

    //printf("_appAudioDemo_task...\n");
    while(1)
    {
        //if(gu8MADPlayFile)
        //{
        //    gu8MADPlayFile=0;
        //    ver_Pcm_PlayAudioFile();
        //    MsOS_EnableInterrupt(E_FIQ_DSP2UP);
        //}
        //printf("$\n");
        if(MApi_AUDIO_CheckInputRequest(&WrtAddr, &WrtBytes))
        {
            appDemo_Audio_File_play(WrtAddr, WrtBytes);
            //printf("Input Request ¡GWrtAddr is 0x%lx, WrtBytes is 0x%lx\n",WrtAddr, WrtBytes);
        }
        //printf("GWrtAddr is 0x%lx, WrtBytes is 0x%lx\n",WrtAddr, WrtBytes);
        //MsOS_DelayTask(200);
    }
}

static void _appAudioMonitor_task(MS_U32 argc, void *argv)
{
    while(1)
    {
        //printf("_appAudioMonitor_task\n");
        MApi_Audio_Monitor();
        MsOS_DelayTask(1);
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to encode FMTX signal
/// @param[in] u8Filter 0: PAL 1: NTSC
/// @return none
/// @sa
/// @note
/// Command: \b Audio_FMTX_start \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Audio_FMTX_Start(MS_U8* u8Filter, MS_U16* u16Attenuation, MS_U16* u16OutScaling)
{
    MS_U8 u8Attenuation_H = (*u16Attenuation) >> 8;
    MS_U8 u8Attenuation_L = (*u16Attenuation) & 0xFF;

    MS_U8 u8OutScaling_H = (*u16OutScaling) >> 8;
    MS_U8 u8OutScaling_L = (*u16OutScaling) & 0xFF;

    MApi_AUDIO_SetSystem(MSAPI_AUD_ATVENC_FMTX);

    //disable Dolby SPDIF volume control, when FMTX start
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , FALSE, 0);

    MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation, u8Attenuation_H, u8Attenuation_L);
    MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling, u8OutScaling_H, u8OutScaling_L);

    if(*u8Filter > 1)
    {
        *u8Filter = 1;
    }

    MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP, *u8Filter, 0);

    //start encode
    MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 1, 0);

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief The sample code demonstrate how to stop ATV
/// @param[in] None
/// @return none
/// @sa
/// @note
/// Command: \b Audio_ATV_stop \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Audio_ATV_Stop(void)
{
    //stop encode
    MApi_AUDIO_SIF_SendCmd(MSAPI_AUD_SIF_CMD_SET_ENC_CMD, 0, 0);

    //enable Dolby SPDIF volume control
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_enableDolbyBulletin11 , TRUE, 0);

    return TRUE;
}

MS_BOOL appDemo_Audio_SetMonitor(void)
{
    if(_audioMonitor_Task.iId == -1)
    {
        _audioMonitor_Task.pStack = MsOS_AllocateMemory(_audioMonitor_Task.u32StackSize, gs32CachedPoolID);
        if(!_audioMonitor_Task.pStack)
        {
            printf("allocate stack fail\n");
            GEN_EXCEP;
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
            GEN_EXCEP;
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
MS_BOOL appDemo_Audio_Debug(MS_U8* u8Level, MS_U16* u16Interval)
{
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Dump_Dsp_Info, *u8Level, *u16Interval);
    appDemo_Audio_SetMonitor();
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief The sample code This routine is used to set S/PDIF mode.
/// @param u8Mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x1 : SPDIF off
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// Command: \b Audio_SPDIF_Mode \n
////////////////////////////////////////////////////////////////////////////////
MS_BOOL appDemo_Audio_Set_SPDIF_Mode(MS_U8* u8Mode)
{
    MApi_AUDIO_SPDIF_SetMode(*u8Mode);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set audio output mode
/// @param mode
///     0:STEREO
///     1: LL
///     2: RR
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b Audio_SetOutputMode \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Audio_SetOutputMode(MS_U32* mode)
{
    MS_BOOL ret;

    ret = MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, *mode, 0);

    return ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code adjuest volume of audio
/// @param[in] vol 0~99(quiet~loud)
/// @return none
/// @sa
/// @note
/// Command: \b Audio_volume \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Audio_SetAbsoluteVolume(MS_U8* vol)
{
    MS_U8 value1, value2;
    MS_U16 u16VolumeValue=0, u16VolumeValue_LR=0, u16VolumeValue_SPDIF=0;
    MS_U8 u8LR_CompensateFrac = 0x0;    //no need SRC compensation
    MS_U8 u8SPDIF_CompensateFrac = 0x0; //no need SRC compensation
    MS_BOOL bMute = FALSE;

    if( *vol > 100 )
    {
        *vol = 100;
    }

    if(*vol == 0)
    {
        bMute = TRUE;
        appDemo_Audio_SetMute(&bMute);
    }
    else
    {
        bMute = FALSE;
        appDemo_Audio_SetMute(&bMute);
    }

    printf("\n[%s][%d]########## audio volume = %d ##########\n\n", __FUNCTION__, __LINE__, *vol);

    u16VolumeValue = AudioVolumeTable[*vol];

    //Line-out (LR) vol + compensate frac
    u16VolumeValue_LR = u16VolumeValue + (u8LR_CompensateFrac<<5);

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


    //SPDIF vol + compensate frac
    u16VolumeValue_SPDIF = u16VolumeValue + (u8SPDIF_CompensateFrac<<5);

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

    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief The sample code mute
/// @param[in] vol
///     1: mute
///     0: non-mute
/// @return none.
/// @sa
/// @note
/// Command: \b Audio_mute \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Audio_SetMute(MS_BOOL* bEnable)
{
    //mute line out
    MApi_AUDIO_SetMute(AUDIO_PATH_MAIN_SPEAKER, *bEnable);

    //mute SPDIF PCM
    MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, *bEnable);

    //mute SPDIF PCM + non PCM
    MApi_AUDIO_SPDIF_SetMute(*bEnable);

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
MS_BOOL appDemo_Audio_Stop_Demo(void)
{
    _clipWriteLastAddr=0;
    _u32AudioFileIndex=0;
    MApi_AUDIO_StopDecode();
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize audio function
/// @param none
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b Audio_Init \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Audio_Init(void)
{
    AUDIO_INIT_INFO SystemInfo;
    AUDIO_OUT_INFO OutputInfo;
    AUDIO_PATH_INFO PathInfo;

    printf("appDemo_Audio_Init \n");

    SystemInfo.tv_system=TV_PAL;
    SystemInfo.au_system_sel=AUDIO_SYSTEM_A2;
    SystemInfo.dtv_system=0;
    MApi_AUDIO_SetSystemInfo(&SystemInfo);

    //======================================
    // Set output info
    //======================================

    OutputInfo.SpeakerOut = AUDIO_AUOUT0_OUTPUT;
    OutputInfo.HpOut = 0;
    OutputInfo.ScartOut = AUDIO_AUOUT1_OUTPUT;
    OutputInfo.MonitorOut = AUDIO_AUOUT1_OUTPUT;
    MApi_AUDIO_SetOutputInfo(&OutputInfo);

    //======================================
    // Set path info
    //======================================

    PathInfo.SpeakerOut = AUDIO_PATH_MAIN;
    PathInfo.HpOut = 0;
    PathInfo.MonitorOut = AUDIO_PATH_1;
    PathInfo.SpdifOut = AUDIO_PATH_2;
    PathInfo.ScartOut = AUDIO_PATH_1;
    MApi_AUDIO_SetPathInfo(&PathInfo);

    MDrv_AUDIO_SetDspBaseAddr(DSP_DEC, 0, MAD_DEC_BUF_ADR);
    MApi_AUDIO_Initialize();

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);

    //MApi_AUDIO_EnableTone(TRUE);
    MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_DrcMode , 1, 0);

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to feed data to audio codec
/// @param[in] writeAddress The address that audio data should put in
/// @param[in] writeNumber The length that audio data should put in
/// @return none
/// @sa
/// @note
/// Command: \b none \n
//------------------------------------------------------------------------------
void appDemo_Audio_File_play(MS_U32 writeAddress, MS_U32 writeNumber)
{
    //printf("\nappDemo_Audio_File_play(0x%lx,0x%lx)...\n",writeAddress,writeNumber);

    if( writeAddress == _clipWriteLastAddr )
    {
        MApi_AUDIO_SetInput();
        return;
    }

    _clipWriteLastAddr = writeAddress ;
    if (_u32AudioFileIndex < AudioLen )
    {
        if ((_u32AudioFileIndex + writeNumber) <= AudioLen )
        {
            memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+_u32AudioFileIndex),writeNumber);
            _u32AudioFileIndex += writeNumber;
        }
        else
        {
            // clear the buffer first
            memset((void*)MS_PA2KSEG1(writeAddress), 0, writeNumber);

            if (writeNumber <= AudioLen )
            {
                // normal case
                int feedsize = 0;

                memcpy((void*)MS_PA2KSEG1(writeAddress),(void*)(pAudioData+_u32AudioFileIndex),AudioLen  - _u32AudioFileIndex);
                feedsize += (AudioLen  -_u32AudioFileIndex);

                _u32AudioFileIndex = 0;
                memcpy((void*)MS_PA2KSEG1(writeAddress+feedsize), (void*)(pAudioData+_u32AudioFileIndex), writeNumber - feedsize);

                _u32AudioFileIndex = writeNumber- feedsize;
            }
        }
    }

    MApi_AUDIO_SetInput();

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
MS_BOOL appDemo_Audio_Out_Init(MS_U32 *pFileFmt)
{
    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;
    appDemo_Audio_Init();

    if(_audioDemo_Task.iId == -1)
    {
        _audioDemo_Task.pStack = MsOS_AllocateMemory(_audioDemo_Task.u32StackSize, gs32CachedPoolID);
        if(!_audioDemo_Task.pStack)
        {
            printf("fail 1\n");
            GEN_EXCEP;
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
            GEN_EXCEP;
        }
    }

    _clipWriteLastAddr=0;
    _u32AudioFileIndex=0;
    if(*pFileFmt == 0)
    {
        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MP3);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
        //MsOS_DelayTask(30);
        MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
        pAudioData = mp3_bin_var;
        AudioLen = MP3_BIN_SIZE;
    }
    else if(*pFileFmt == 1)
    {
        STWAVINFO stWAV;
        memcpy(&stWAV.u32fmtChunkSize, wav_bin_var + 16, 4);
        memcpy(&stWAV.u16FormatTag, wav_bin_var+ 20, 2);
        memcpy(&stWAV.u16Channels, wav_bin_var + 22, 2);
        memcpy(&stWAV.u32SamplesPerSec, wav_bin_var+ 24, 4);
        memcpy(&stWAV.u32AvgBytesPerSec, wav_bin_var + 28, 4);
        memcpy(&stWAV.u16BlockSize, wav_bin_var + 32, 2);
        memcpy(&stWAV.u16BitsPerSample, wav_bin_var + 34, 2);

        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_XPCM);
        MApi_AUDIO_SetXPCMParam(LPCM, (stWAV.u16Channels-1), stWAV.u32SamplesPerSec, stWAV.u16BitsPerSample, stWAV.u16BlockSize, stWAV.u16BitsPerSample);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
        MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
        MApi_AUDIO_FileEndDataHandle(100);
        pAudioData = &wav_bin_var[44];
        AudioLen = WAV_BIN_SIZE;
    }

    else if(*pFileFmt == 2)
    {
        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3P);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
        //MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
         return TRUE;
    }

    else if(*pFileFmt == 3)
    {
        MApi_AUDIO_SetSystem(MSAPI_AUD_ATVENC_FMTX);
        return TRUE;
    }

    else
    {
        return FALSE;
    }

    bMute = FALSE;
    appDemo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    appDemo_Audio_SetAbsoluteVolume(&u8Volume);

    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);


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
MS_BOOL appDemo_Audio_Help(void)
{
    printf("*************************Audio function List*************************\n");
    printf("1.Audio_Init --Initial Audio\n");
    printf("2.Audio_demo --Audio demo 0:mp3 1:wav\n");
    printf("3.Audio_demoStop -- Stop audio demo\n");
    printf("4.Audio_volume -- Volume control; 0~100\n");
    printf("5.Audio_mute -- mute control\n");


    return TRUE;
}
