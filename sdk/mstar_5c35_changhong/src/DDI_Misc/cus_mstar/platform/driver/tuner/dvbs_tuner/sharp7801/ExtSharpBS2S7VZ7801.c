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

#include "Board.h"
#include "MsCommon.h"
#include "HbCommon.h"

#if IF_THIS_TUNER_INUSE(TUNER_SHARP_BS2S7VZ7801)

#include "ExtSharpBS2S7VZ7801.h"
#include "II2CRepeater.h"
#include "IBSP.h"
#include "II2C.h"
#include "IBase.h"




AVL_DVBSx_ErrorCode ExtSharpQM1D1C0042_Reset(struct AVL_Tuner * pTuner)
{
  AVL_DVBSx_ErrorCode r;
  AVL_uchar ucTemp[16];

  //three times soft reset 
  ucTemp[0] = 0x01;
  ucTemp[1] = 0x0c;
  r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

  r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

  r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );
  if(AVL_DVBSx_EC_OK != r)
  {
    return r;
  }

  AVL_DVBSx_IBSP_Delay(2);//2ms

  //LPF_Tuning ON
  ucTemp[0] = 0x0c;
  ucTemp[1] = 0x43;
  r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );
  if(AVL_DVBSx_EC_OK != r)
  {
    return r;
  }
  
  AVL_DVBSx_IBSP_Delay(20);//20ms

  //write Fixed Value register
  ucTemp[0] = 0x01;
  ucTemp[1] = 0x1C;
  ucTemp[2] = 0xA0;
  ucTemp[3] = 0x10;
  ucTemp[4] = 0xBC;
  ucTemp[5] = 0xC5;
  ucTemp[6] = 0x20;
  ucTemp[7] = 0x33;
  ucTemp[8] = 0x06;
  ucTemp[9] = 0x00;
  ucTemp[10] = 0x00;
  ucTemp[11] = 0x00;
  ucTemp[12] = 0x03;
  r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 13, pTuner->m_pAVLChip );

  ucTemp[0] = 0x11;
  ucTemp[1] = 0xFF;
  ucTemp[2] = 0xF3;
  ucTemp[3] = 0x00;
  ucTemp[4] = 0xEA;
  ucTemp[5] = 0x64;
  ucTemp[6] = 0xA6;
  ucTemp[7] = 0x86;
  ucTemp[8] = 0xCC;
  ucTemp[9] = 0xCF;
  ucTemp[10] = 0x95;
  ucTemp[11] = 0xF1;
  ucTemp[12] = 0xA8;
  ucTemp[13] = 0xF2;
  ucTemp[14] = 0x09;
  ucTemp[15] = 0x00;
  r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 16, pTuner->m_pAVLChip );
  if(AVL_DVBSx_EC_OK != r)
  {
    return r;
  }
  
  //mode setting: normal mode
  ucTemp[0] = 0x01;
  ucTemp[1] = 0x1c;
  r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

  ucTemp[0] = 0x03;
  ucTemp[1] = 0x10;
  r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

  ucTemp[0] = 0x05;
  ucTemp[1] = 0xc5;
  r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );
  
  return r; 
}


AVL_uchar ExtSharpQM1D1C0042_VCO_Setting(AVL_uint16 m_uiFrequency_MHz)
{
  AVL_uchar ucTemp;

  if(m_uiFrequency_MHz < 975)
  {
    ucTemp = 0x60;
  }
  else if(m_uiFrequency_MHz < 1200)
  {
    ucTemp = 0x70;
  }
  else if(m_uiFrequency_MHz < 1250)
  {
    ucTemp = 0xa0;
  }
  else if(m_uiFrequency_MHz < 1450)
  {
    ucTemp = 0xb0;
  }
  else if(m_uiFrequency_MHz < 1600)
  {
    ucTemp = 0xc0;
  }
  else if(m_uiFrequency_MHz < 1800)
  {
    ucTemp = 0xd0;
  }
  else if(m_uiFrequency_MHz < 1950)
  {
    ucTemp = 0xe0;
  }
  else
  {
    ucTemp = 0xf0;
  }

  return ucTemp;
}
AVL_DVBSx_ErrorCode ExtSharpQM1D1C0042_Calc_LF(AVL_uint16 m_uiFrequency_MHz, AVL_uchar *N, AVL_uchar *A, AVL_int32 *sd)
{
	AVL_uint16 M;
	AVL_uint16 alpha;
	AVL_int16 beta;
	AVL_uint16 F_ref;
	if(16 == OSCILLATION_FREQUENCY_MHZ)
	{
		F_ref = OSCILLATION_FREQUENCY_MHZ;
	}
	else
	{
		F_ref = OSCILLATION_FREQUENCY_MHZ/2;
	}

	M = m_uiFrequency_MHz;
	alpha = (M+F_ref/2)/F_ref;
	alpha = alpha*F_ref;
	beta = M - alpha;
	*N = (AVL_uchar)(alpha/(4*F_ref)-3);
	*A = (AVL_uchar)(alpha/F_ref) - 4*(*N +1)-5;

	/*make it two's complements of 22 bits.*/
	if(beta < 0)
	{
		*sd = (AVL_int32)((1048576/F_ref) * (M - alpha));
		*sd += 0x400000;
	}
	else
	{
		*sd = (AVL_int32)((1048576/F_ref) * (M - alpha));
	}

	return AVL_DVBSx_EC_OK;
}


AVL_uchar ExtSharpQM1D1C0042_LPF_FC(void)
{
  AVL_uchar ucTemp;
#if 0
  ucTemp = (AVL_uchar)(round(OSCILLATION_FREQUENCY_MHZ/4))*2;
#else
  ucTemp = (OSCILLATION_FREQUENCY_MHZ+2)/4;
  ucTemp = ucTemp*2;
#endif
  ucTemp = (ucTemp-4)/2;

  return ucTemp;
}


AVL_uchar ExtSharpQM1D1C0042_CSEL_Offset(AVL_uint16 uiLPF_100kHz)
{
  AVL_uint16 uiLPF_MHz;
  AVL_uchar ucTemp;
  
  uiLPF_MHz = uiLPF_100kHz/10;

  if(uiLPF_MHz < 10)
  {
    ucTemp = 0x60;
  }
  else if(uiLPF_MHz < 16)
  {
    ucTemp = 0x40;
  }
  else if(uiLPF_MHz < 30)
  {
    ucTemp = 0x20;
  }
  else 
  {
    ucTemp = 0x00;
  }

  return ucTemp;
}


AVL_DVBSx_ErrorCode ExtSharpBS2S7VZ7801_Initialize(struct AVL_Tuner * pTuner)
{
      AVL_DVBSx_ErrorCode r;
	r = AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_slave_addr_addr, pTuner->m_uiSlaveAddress);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_use_internal_control_addr, 0);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_LPF_margin_100kHz_addr, 0);	//clean up the LPF margin for blind scan. for external driver, this must be zero.
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_max_LPF_100kHz_addr, 360);	//set up the right LPF for blind scan to regulate the freq_step. This field should corresponding the flat response part of the LPF.
	r |= AVL_DVBSx_II2CRepeater_Initialize(pTuner->m_uiI2CBusClock_kHz, pTuner->m_pAVLChip);
	r |= AVL_DVBSx_II2C_Write32(pTuner->m_pAVLChip, 0x640160, 0xF3);
	r |= AVL_DVBSx_IBSP_Delay(5);
	r |= AVL_DVBSx_II2C_Write32(pTuner->m_pAVLChip, 0x640160, 0xD3);
	r |= AVL_DVBSx_IBSP_Delay(5);
	r |= ExtSharpQM1D1C0042_Reset(pTuner);

      HB_printf("\n\n--Init Sharp7801 %d  max_LPF_100kHz = %d--\n\n", r , 360);
      return r;
}


AVL_DVBSx_ErrorCode ExtSharpBS2S7VZ7801_GetLockStatus(struct AVL_Tuner * pTuner )
{
  AVL_DVBSx_ErrorCode r;
	AVL_uchar ucTemp;

  ucTemp = 0x0d;
  r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), &ucTemp, 1, pTuner->m_pAVLChip );

	r = AVL_DVBSx_II2CRepeater_ReadData((AVL_uchar)(pTuner->m_uiSlaveAddress), (AVL_puchar)(&ucTemp), 1, pTuner->m_pAVLChip);
	if( AVL_DVBSx_EC_OK == r ) 
	{
		if( 0 == (ucTemp & SHARPQM1D1C0042_PLL_LOCK) ) 
		{
			r = AVL_DVBSx_EC_Running;
		}
	}
	return(r);
}


AVL_DVBSx_ErrorCode ExtSharpBS2S7VZ7801_Lock(struct AVL_Tuner * pTuner)
{
  AVL_DVBSx_ErrorCode r;
  static AVL_uchar ucFlag = 0;
  AVL_uchar ucTemp[4];
  AVL_uchar N;
  AVL_uchar A;
  AVL_int32 sd;

  if(0 == ucFlag)//first time, case1
  {
    //ucFlag = 1;
    
    ucTemp[0] = 0x02;
    ucTemp[1] = ExtSharpQM1D1C0042_VCO_Setting(pTuner->m_uiFrequency_100kHz/10);
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ExtSharpQM1D1C0042_Calc_LF(pTuner->m_uiFrequency_100kHz/10, &N, &A, &sd);
    ucTemp[0] = 0x06;
    ucTemp[1] = N & 0x3f;
    if(16 != OSCILLATION_FREQUENCY_MHZ)
    {
      ucTemp[1] = ucTemp[1] + 0x80;
    }
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x07;
    ucTemp[1] = (A & 0xf) + 0x30;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x08;
    ucTemp[1] = ExtSharpQM1D1C0042_LPF_FC(); 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x09;
    ucTemp[1] = (sd>>16) & 0x3f; 
    ucTemp[2] = (sd>>8) & 0xff;
    ucTemp[3] = sd & 0xff; 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 4, pTuner->m_pAVLChip );

    ucTemp[0] = 0x0C;
    ucTemp[1] = 0x03; 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    AVL_DVBSx_IBSP_Delay(2);//2ms

    ucTemp[0] = 0x0C;
    ucTemp[1] = 0xc3; 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    AVL_DVBSx_IBSP_Delay(20);//20ms

    ucTemp[0] = 0x08;
    ucTemp[1] = (((pTuner->m_uiLPF_100kHz/10)+((pTuner->m_uiLPF_100kHz/10)%2))-4)/2;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x13;
    ucTemp[1] = ExtSharpQM1D1C0042_CSEL_Offset(pTuner->m_uiLPF_100kHz); 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );    
  }
  else//case2
  {
    ucTemp[0] = 0x02;
    ucTemp[1] = ExtSharpQM1D1C0042_VCO_Setting(pTuner->m_uiFrequency_100kHz/10);
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ExtSharpQM1D1C0042_Calc_LF(pTuner->m_uiFrequency_100kHz/10, &N, &A, &sd);
    ucTemp[0] = 0x06;
    ucTemp[1] = N & 0x3f;
    if(16 != OSCILLATION_FREQUENCY_MHZ)
    {
      ucTemp[1] = ucTemp[1] + 0x80;
    }
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x07;
    ucTemp[1] = (A & 0xf) + 0x30;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x08;
    ucTemp[1] = (((pTuner->m_uiLPF_100kHz/10)+((pTuner->m_uiLPF_100kHz/10)%2))-4)/2;
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    ucTemp[0] = 0x09;
    ucTemp[1] = (sd>>16) & 0x3f; 
    ucTemp[2] = (sd>>8) & 0xff;
    ucTemp[3] = sd & 0xff; 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 4, pTuner->m_pAVLChip );

    ucTemp[0] = 0x13;
    ucTemp[1] = ExtSharpQM1D1C0042_CSEL_Offset(pTuner->m_uiLPF_100kHz); 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );
    
    ucTemp[0] = 0x0C;
    //ucTemp[1] = 0x43; 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 1, pTuner->m_pAVLChip );
	r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp+1, 1, pTuner->m_pAVLChip );
    AVL_DVBSx_IBSP_Delay(1);//1ms
	ucTemp[1] |= 0x80;
    //ucTemp[0] = 0x0C;
    //ucTemp[1] = 0x83; 
    r |= AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip );

    AVL_DVBSx_IBSP_Delay(15);//1ms

  }

  return r;
}
#endif


