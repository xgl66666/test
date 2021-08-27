/*
 *           Copyright 2007-2009 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


#ifndef AVL_DVBSx_FUNC_H_
#define AVL_DVBSx_FUNC_H_

#include "AVL_DVBSx/include/avl_dvbsx.h"
#include "AVL_DVBSx/include/ITuner.h"


struct Signal_Level
{
    unsigned short SignalLevel;
    short SignalDBM;
};

AVL_DVBSx_ErrorCode AVL_DVBSx_SetMpegMode(AVL_uchar bParallelTS,AVL_uchar bLockChannel);
AVL_DVBSx_ErrorCode AVL_DVBSx_Initialize(void);
AVL_DVBSx_ErrorCode AVL_DVBSx_SetFrequency(AVL_uint16 uiFreq_MHz,AVL_uint32 uiSymbolRate_Hz);

AVL_DVBSx_ErrorCode AVL_DVBSx_GetLockStatus(void);
AVL_DVBSx_ErrorCode AVL_DVBSx_GetBER(float *pfBER);
AVL_DVBSx_ErrorCode AVL_DVBSx_GetPER(float *pfPER);
AVL_DVBSx_ErrorCode AVL_DVBSx_GetSNR(AVL_int32 *pfSNR);
AVL_DVBSx_ErrorCode AVL_DVBSx_GetRFOffset(AVL_int16 *piRFOff);
AVL_DVBSx_ErrorCode AVL_DVBSx_GetSinalLevel(AVL_int16 *piRFSignalLevel);
AVL_DVBSx_ErrorCode AVL_DVBSx_GetSignalInfo(struct AVL_DVBSx_SignalInfo *pSignalInfo);

AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_SetToneOut(AVL_uchar ucTone);
AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_SetLNBOut(AVL_uchar ucLow);
AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_GetLNBOut(AVL_puchar pucLNBOut);
AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_Set22kOnOff(AVL_uchar uc22kOn);
AVL_DVBSx_ErrorCode  AVL_DVBSx_DiSEqC_Get22kOnOff(AVL_puchar puc22kOn);
AVL_DVBSx_ErrorCode AVL_DVBSx_DiSEqC_SetCmd(AVL_puchar pucCmd,AVL_uchar ucCmdSize);

AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_start(AVL_int16 u16StartFreq,AVL_int16 u16EndFreq);
AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_NextFreqBlindScan(AVL_uchar* u8BlindScanEnd);
AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_WaitCurFreqFinished(AVL_uchar* u8Progress,AVL_uchar* ucFindNum);
AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_Cancel(void);
AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_End(void);
AVL_DVBSx_ErrorCode AVL_DVBSx_Blind_GetChannel(AVL_uint16 u16Start,AVL_uint16 *chNum,struct AVL_DVBSx_Channel *channels);
AVL_uint32 AVL_DVBSx_Blind_GetCurrentFreq(void);
AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetContinuous (AVL_puchar puiCon,struct AVL_DVBSx_Chip * pAVLChip );
#endif

