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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// file    apiDigiTuner.c
/// @brief  Digital Tuner Low-Level API
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "HbCommon.h"
#include "drvGPIO.h"
#include "drvIIC.h"
#include "apiDigiTuner.h"
#include "MsCommon.h"
#include "SiLabs_API_L3_Wrapper.h"
#include "Board.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAX_INPUT_FREQ 2150 //Mhz
#define MIN_INPUT_FREQ 950  //Mhz
#define UNUSED __attribute__((unused))
#define FRONTEND_MUTEX_TIMEOUT 500

#define ENTRY()                                                         \
    if (MsOS_ObtainMutex(_s32MutexId, FRONTEND_MUTEX_TIMEOUT) == FALSE)    \
    {   FE_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));              \
        return FALSE;                                                   \
    }

#define RETURN(_ret)                                                    \
    { MsOS_ReleaseMutex(_s32MutexId);                                     \
      return _ret; }

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
static MS_S32 gs32FECachedPoolID = INVALID_POOL_ID;
static MS_S32 gs32FENonCachedPoolID = INVALID_POOL_ID;

//--------------------------------------------------------------------------------------------------
//  Global Variables
//--------------------------------------------------------------------------------------------------
static MS_S32 _s32MutexId = -1;
SILABS_FE_Context     FrontEnd_Table[FRONT_END_COUNT];
CUSTOM_Status_Struct  FE_Status;
CUSTOM_Status_Struct *custom_status;

static MS_U32 u32CurBroadcastType = BROADCAST_TYPE_NOT_SEETING;
MS_SAT_PARAM* _pstSATParam = NULL;
MS_FE_CARRIER_PARAM* _ptunerParam = NULL;

static char* fe_tag[4]={"fe[0]", "fe[1]", "fe[2]", "fe[3]"};

#ifdef SATELLITE_FRONT_END
static MS_BOOL bIsHiLOF = FALSE;
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

#ifdef SATELLITE_FRONT_END
static MS_BOOL _get_DiSEqC_SwitchPortCmd(MS_U8 u8Port,MS_BOOL bUncommitted, MS_U8* u8CmdBuf)
{
    MS_U8 u8Cmd[]= {0xE2,0x10,0x38,0xF0};

    if(bUncommitted)
    {
        u8Cmd[2] = 0x39;
    }
    u8Cmd[3] = 0xF0+u8Port;

    memcpy(u8CmdBuf, u8Cmd, sizeof(u8Cmd));
    return TRUE;
}

static MS_BOOL _get_DiSEqC_GotoSatPositionCmd(MS_U8 u8Pos, MS_U8* u8CmdBuf)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x6B,u8Pos};

    u8Pos &= DISEQC12_USALS_POSITION_MASK;
    if(u8Pos == 0)
    {
        return FALSE;
    }
    memcpy(u8CmdBuf, u8Cmd, sizeof(u8Cmd));
    return TRUE;
}


static MS_BOOL _DigiTuner_SetLNBPower(MS_U8 drv_frontend_index, MS_EN_LNBPWR_ONOFF enLNBPower,MS_U8 u8Polarity,MS_BOOL bPorInvert)
{
    MS_BOOL bRet = TRUE;
    SILABS_FE_Context    *front_end;

    front_end = &(FrontEnd_Table[drv_frontend_index]);
    switch(enLNBPower)
    {
    default:
    case  EN_LNBPWR_13OR18V:
    {
        if( (!bPorInvert && u8Polarity) || (bPorInvert && u8Polarity == 0)) //V
        {
            if(SiLabs_API_SAT_voltage(front_end, 13) != 1)
                bRet = FALSE;
        }
        else  //H
        {
            if(SiLabs_API_SAT_voltage(front_end, 18) != 1)
                bRet = FALSE;
        }
        break;
    }

    case EN_LNBPWR_OFF:
    {
        if(SiLabs_API_SAT_voltage(front_end, 0) != 1)
            bRet = FALSE;
        break;
    }

    case EN_LNBPWR_13V:
    {
        if(SiLabs_API_SAT_voltage(front_end, 13) != 1)
            bRet = FALSE;
        break;
    }
    case EN_LNBPWR_18V:
    {
        if(SiLabs_API_SAT_voltage(front_end, 18) != 1)
            bRet = FALSE;
        break;
    }
    }
    return bRet;
}

static  MS_BOOL  _DigiTuner_CalculateIF(MS_U8 drv_frontend_index,MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam)
{
    MS_S32 s32Freq;
    MS_U32 u32MidFreq;
    MS_S32 s32Offset = 0;
    MS_BOOL bFreqIsRF = TRUE;

    s32Freq = pParam->u32Frequency;

    if((s32Freq >= MIN_IF_FREQ) && (s32Freq <= MAX_IF_FREQ))
    {
        FE_DBG(("Input frequency is IF already\n"));
        bFreqIsRF = FALSE;
    }

     if(pSATParam->u16LoLOF <MAX_C_LOF_FREQ)  // c band
     {
         if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
         {
             bIsHiLOF = FALSE;
         }
         else if(pSATParam->bPorInvert)
         {
             if(pParam->SatParam.u8Polarity== 1) //V ---> use low LOF
             {
                 bIsHiLOF = FALSE;
             }
             else   //H --->use high LOF
             {
                 bIsHiLOF = TRUE;
             }
         }
         else
         {
             if(pParam->SatParam.u8Polarity  == 0) //H ---> use low LOF
             {
                 bIsHiLOF = FALSE;
             }
             else   //V --->use high LOF
             {
                 bIsHiLOF = TRUE;
             }
         }
    }
    else // ku band
    {
        if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
         {
            bIsHiLOF= FALSE;
         }
         else // 2LOF
         {
             u32MidFreq =  abs(s32Freq - pSATParam->u16LoLOF) ;
             s32Offset = pSATParam->u16LoLOF+ MAX_INPUT_FREQ - pSATParam->u16HiLOF - MIN_INPUT_FREQ;
             if(s32Offset < 0)
                 s32Offset = 0;
             else
                 s32Offset /= 2;

             if( u32MidFreq <= (MAX_INPUT_FREQ-s32Offset))
                 bIsHiLOF = FALSE;
             else
                 bIsHiLOF = TRUE;     
         }
    }

    if(bFreqIsRF)
    {
        if(bIsHiLOF)
            pParam->u32Frequency = abs(s32Freq - pSATParam->u16HiLOF) ;
        else
            pParam->u32Frequency = abs(s32Freq - pSATParam->u16LoLOF) ;
    }
    else
    {
        pParam->u32Frequency = (MS_U32)s32Freq;
    }

    return TRUE;
}

static MS_BOOL _DigiTuner_DiSEqC_SendCmd(MS_U8 drv_frontend_index, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_SAT_PARAM* pstSATParam = NULL;
    signed   int sequence_length; 
    unsigned char* sequence_buffer = NULL;
    unsigned char cont_tone = 0;
    unsigned char tone_burst = 0; 
    unsigned char burst_sel = 0; 
    unsigned char end_seq = 0;
    SILABS_FE_Context    *front_end;

    front_end = &(FrontEnd_Table[drv_frontend_index]);
    
    if(NULL == _pstSATParam)
         return FALSE;
    
     pstSATParam = _pstSATParam + drv_frontend_index;
     sequence_length = u8CmdSize;

     sequence_buffer = (unsigned char*)MsOS_AllocateMemory(sizeof(unsigned char)*sequence_length,gs32FECachedPoolID);
     if(NULL == sequence_buffer)
         return FALSE;
     else
         memcpy(sequence_buffer, pCmd, sequence_length);
     
     if(pstSATParam->e22KOnOff == EN_22K_ON ||(pstSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
     {
        cont_tone = 1;
     }
     else
     {
        cont_tone = 0;
     }

     if(pstSATParam->eToneburstType == EN_TONEBURST_1)
     {
        tone_burst = 1;
        burst_sel = 1;
     }
     else if(pstSATParam->eToneburstType == EN_TONEBURST_0)
     {
        tone_burst = 1;
        burst_sel = 0;
     }

     if(SiLabs_API_SAT_send_diseqc_sequence(front_end, sequence_length, sequence_buffer, cont_tone, tone_burst, burst_sel, end_seq) == 1)
     {
        MsOS_FreeMemory(sequence_buffer, gs32FECachedPoolID);
        return TRUE;
     }
     else
     {
        MsOS_FreeMemory(sequence_buffer, gs32FECachedPoolID);
        return FALSE;
     }
}

static MS_BOOL  _DigiTuner_SwitchSatellite(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
     signed   int sequence_length = 4; 
     unsigned char sequence_buffer[4];
     unsigned char cont_tone = 0;
     unsigned char tone_burst = 0; 
     unsigned char burst_sel = 0; 
     unsigned char end_seq = 1;
     SILABS_FE_Context    *front_end;

    front_end = &(FrontEnd_Table[drv_frontend_index]);
         
    switch(pSATParam->eDiseqcLevel)
    {
        default:
        case EN_DISEQC_LVL_OFF:
        {
            if(pSATParam->eToneburstType == EN_TONEBURST_1)
            {
                tone_burst = 1;
                burst_sel = 1;
            }
            else if(pSATParam->eToneburstType == EN_TONEBURST_0)
            {
                tone_burst = 1;
                burst_sel = 0;
            }
            break;
        }
    
        case EN_DISEQC_LVL_10:
        {
            MS_U8 u8SubPort = 0;
            if(pSATParam->eToneburstType == EN_TONEBURST_1)
            {
                tone_burst = 1;
                burst_sel = 1;
            }
            else if(pSATParam->eToneburstType == EN_TONEBURST_0)
            {
                tone_burst = 1;
                burst_sel = 0;
            }
            if(u8Polarity == 0)
                u8SubPort |= 0x02;
            if(bIsHiLOF)
                u8SubPort |= 0x01;
            switch(pSATParam->eSwt10Port)
            {
            case EN_SWT_PORT_1:
                u8SubPort = u8SubPort;
                break;
            case EN_SWT_PORT_2:
                u8SubPort = 0x04+ u8SubPort;
                break;
            case EN_SWT_PORT_3:
                u8SubPort = 0x08+ u8SubPort;
                break;
            case EN_SWT_PORT_4:
                u8SubPort = 0x0C+ u8SubPort;
                break;
            default:
                break;
            }
            if(pSATParam->eSwt10Port >= EN_SWT_PORT_1 && pSATParam->eSwt10Port <= EN_SWT_PORT_4)
            {
                _get_DiSEqC_SwitchPortCmd(u8SubPort, FALSE, sequence_buffer);
            }
            break;
        }
        case EN_DISEQC_LVL_11:
        {
            if(pSATParam->eSwt11Port >= EN_SWT_PORT_1 && pSATParam->eSwt11Port <= EN_SWT_PORT_16)
            {
                _get_DiSEqC_SwitchPortCmd((MS_U8)(pSATParam->eSwt11Port)-1, TRUE, sequence_buffer);
            }
            break;
        }
    
        case EN_DISEQC_LVL_12:
        case EN_DISEQC_LVL_13:
        {
            if((pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK) != (pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK))
            {
                _get_DiSEqC_GotoSatPositionCmd(pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK, sequence_buffer);
            }
            break;
        }
    
        case EN_DISEQC_LVL_MULTI:
        {
            //_MultiDiSEqc_SwitchPort(drv_frontend_index, pSATParam,u8Polarity,bIsHiLOF);
        }
    }
    
    if(pSATParam->e22KOnOff == EN_22K_ON ||(pSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
    {
        cont_tone = 1;
    }

    if(SiLabs_API_SAT_send_diseqc_sequence(front_end, sequence_length, sequence_buffer, cont_tone, tone_burst, burst_sel, end_seq) == 1)
        return TRUE;
    else
        return FALSE;   
}
#endif

//--------------------------------------------------------------------------------------------------
//  Global Functions
//--------------------------------------------------------------------------------------------------
MS_U8 MApi_DigiTuner_GetMaxFEIdx(void)
{
    return 1;
}


MS_BOOL MApi_Frontend_SetBroadcastType(MS_U8 drv_frontend_index,MS_U32 u32BroadcastType)
{
    FE_MSG(("=========================== Broadcast Type List ==========================\n"));
    FE_MSG((" 0x%x -> BROADCAST_TYPE_NOT_SEETING\n",BROADCAST_TYPE_NOT_SEETING));
    FE_MSG((" 0x%x -> DVBC\n",DVBC));
    FE_MSG((" 0x%x -> DVBT\n",DVBT));
    FE_MSG((" 0x%x -> DVBT2\n",DVBT2));
    FE_MSG((" 0x%x -> ISDBT\n",ISDBT));
    FE_MSG((" 0x%x -> DVBS\n",DVBS));
    FE_MSG((" 0x%x -> ATSC\n",ATSC));
    FE_MSG((" 0x%x -> J83B\n",J83B));
    FE_MSG((" 0x%x -> DTMB\n",DTMB));
    FE_MSG((" 0x%x -> DVBS2\n",DVBS2));
    FE_MSG((" 0x%x -> DSS\n",DSS));
    FE_MSG(("========================== End Broadcast Type List =======================\n"));

    u32CurBroadcastType = u32BroadcastType;

    return TRUE;

}

MS_BOOL MApi_Frontend_GetBroadcastType(MS_U8 drv_frontend_index,MS_U32* pu32BroadcastType)
{

   *pu32BroadcastType = u32CurBroadcastType;

    return TRUE;
}


MS_U32 MApi_DigiTuner_GetFrontendType(MS_U8 drv_frontend_index)
{
    return u32CurBroadcastType;
}

MS_BOOL MApi_DigiTuner_ReInit(MS_U8 drv_frontend_index)
{
       return TRUE;
}

MS_BOOL MApi_DigiTuner_DeInit(MS_U8 drv_frontend_index)
{
    return TRUE;
}


MS_BOOL MApi_Frontend_SetTunerDemod(MS_U8 IIc_index,MS_U32 u32ScanTunerIndex, MS_U32 u32ScanDemodIndex)
{
    return TRUE;
}

MS_BOOL MApi_Frontend_SetDetectMode(MS_U8 drv_frontend_index,EN_FRONTEND_DETECT_MODE eDetectMode)
{
    return TRUE;
}

MS_U32 MApi_GetDmxInputPath(MS_U8 drv_frontend_index)
{
    return 0;
}

MS_BOOL MApi_Frontend_InExteralPath_Arrange(EN_IN_EX_TERNAL_PATH eInExTernalPath,DEV_FRONTEND_TYPE* pFEtab)
{
    return TRUE;
}

MS_BOOL MApi_Frontend_SetInExteralPath(MS_U8 IIC_index,EN_IN_EX_TERNAL_PATH eInExTernalPath, DEV_FRONTEND_TYPE* pFEtab)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Init(MS_U8 drv_frontend_index)
{
    SILABS_FE_Context    *front_end;
    signed int demod_code = 0x2183;
    UNUSED signed int lnb_code = 0xa8304;
    signed int tuner_code = 0x2017;
    signed int demod_addr = 0xc8; // demod I2C slave ID
    signed int tuner_addr = 0xc0; // non satellite tuner I2C slave ID
    signed int tuner_addr_SAT = 0xc6;// satellite tuner I2C slave ID 
    UNUSED signed int lnb_chip_add = 0x10; // LNB chip I2C slave ID


#ifdef  SAT_TUNER_AV2018
    tuner_code = 0xa2017;
    tuner_addr_SAT = 0xc6;
#elif defined(TER_TUNER_Si2141)
    tuner_code = 0x2141;
    tuner_addr = 0xc0;
#else
    FE_ERR(("Selected tuner NOT Support\n"));
    return FALSE;
#endif

#ifdef SATELLITE_FRONT_END
#ifdef A8304_COMPATIBLE
   lnb_code = 0xa8304;
   lnb_chip_add = 0x10;
#else
   FE_ERR(("Selected LNB chip NOT Support\n"));
   return FALSE;
#endif
#endif
    
    printf("Init SiLabs FE\n");
    MDrv_IIC_Init(getI2CPort(0));
    MApi_Demod_HWReset(0);
    front_end = &(FrontEnd_Table[drv_frontend_index]);
    SiLabs_API_Frontend_Chip(front_end, demod_code);
    SiLabs_API_SW_Init(front_end, demod_addr, tuner_addr, tuner_addr_SAT);
#ifdef SATELLITE_FRONT_END    
    SiLabs_API_Select_SAT_Tuner(front_end, tuner_code, drv_frontend_index);
    SiLabs_API_SAT_Select_LNB_Chip(front_end, lnb_code, lnb_chip_add);
    SiLabs_API_SAT_tuner_I2C_connection (front_end, drv_frontend_index);
    SiLabs_API_SAT_Clock(front_end, 2, 33, 27, 1);
    SiLabs_API_SAT_Spectrum(front_end, 0);
    SiLabs_API_SAT_AGC(front_end, 0xd, 1, 0x0, 1);
#endif 

#ifdef TERRESTRIAL_FRONT_END
    SiLabs_API_Select_TER_Tuner(front_end, tuner_code, drv_frontend_index);
    SiLabs_API_TER_tuner_I2C_connection (front_end, drv_frontend_index);
    SiLabs_API_TER_Tuner_ClockConfig(front_end, 1, 1);
    SiLabs_API_TER_Clock(front_end, 1, 33, 24, 1);
    SiLabs_API_TER_FEF_Config(front_end, 1, 0xb, 1);
    SiLabs_API_TER_AGC(front_end, 0x0, 0, 0xa, 0);
    SiLabs_API_TER_Tuner_AGC_Input(front_end, 1);
    SiLabs_API_TER_Tuner_FEF_Input(front_end, 1);
    SiLabs_API_TER_Tuner_IF_Output(front_end, 0);
#endif

    SiLabs_API_Set_Index_and_Tag(front_end, drv_frontend_index, fe_tag[drv_frontend_index]);
    SiLabs_API_HW_Connect(front_end, 2);

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "DigiTuner_Mutex",MSOS_PROCESS_SHARED);
        if (_s32MutexId < 0)
        {
            FE_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    }

    if(NULL == _ptunerParam)
    {
        _ptunerParam = (MS_FE_CARRIER_PARAM*)MsOS_AllocateMemory(sizeof(MS_FE_CARRIER_PARAM)*FRONT_END_COUNT,gs32FECachedPoolID);
        if(NULL == _ptunerParam)
            return FALSE;
    }

    if(NULL == _pstSATParam)
    {
        _pstSATParam = (MS_SAT_PARAM*)MsOS_AllocateMemory(sizeof(MS_SAT_PARAM)*FRONT_END_COUNT,gs32FECachedPoolID);
        if(NULL == _pstSATParam)
            return FALSE;
    }
    return TRUE;
}


MS_BOOL MApi_DigiTuner_SetDMDCallBack(MS_U8 drv_frontend_index, fpDemodCB fp)
{
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Power on,off digital tuner
/// @param eFE                      \b IN: Frontend id
/// @param bEnPower                 \b IN: TRUE - Power on
///                                        FALSE - Power off
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_PowerOnOff(MS_U8 drv_frontend_index, MS_BOOL bEnPower)
{
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Tune tuner to RF channel by auto or manual mode
/// @param eFE                      \b IN: Frontend id
/// @param pstParam                 \b IN: Carrier parameters
/// @param eMode                    \b IN: Tuning mode (Auto or Manual)
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_Tune2RfCh_DVBS(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam, EN_FE_TUNE_MODE eMode)
{
#ifdef SATELLITE_FRONT_END 
    SILABS_FE_Context    *front_end;
    MS_FE_CARRIER_PARAM* ptunerParam;
    MS_SAT_PARAM * psatParam;
    signed int LockStatus;
    signed int standard = SILABS_DVB_S;
    signed int freq = 0;                   //the frequency to lock to    (in Hz for TER, in kHz for SAT)
    signed int bandwidth_Hz = 0;       //the channel bandwidth in Hz (only for DVB-T, DVB-T2, ISDB-T)
    unsigned int symbol_rate_bps = 0;      //the symbol rate in baud/s   (for DVB-C, MCNS and SAT)
    unsigned int stream = 0;           //the HP/LP stream            (only for DVB-T)
    unsigned int constellation = 0;    //the DVB-C constellation     (only for DVB-C)
    unsigned int polarization = 0;     //
    unsigned int band = 0;             //
    signed int data_slice_id = 0;          //the DATA SLICE Id           (only for DVB-C2 when num_dslice  > 1)
    signed int plp_id = 0;                 //the PLP Id                  (only for DVB-T2 and DVB-C2 when num_plp > 1)
    unsigned int T2_lock_mode = 0;         //(0='ANY', 1='T2-Base', 2='T2-Lite')

    ENTRY();
    if((NULL == _ptunerParam) || (NULL == _pstSATParam))
        RETURN(FALSE);

    ptunerParam = _ptunerParam + drv_frontend_index;
    psatParam = _pstSATParam + drv_frontend_index;
    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(u32CurBroadcastType == DVBS)
    {
        FE_DBG(("%s: DVBS\n", __FUNCTION__));
        standard = SILABS_DVB_S;
    }
    else if(u32CurBroadcastType == DVBS2)
    {
        FE_DBG(("%s: DVBS2\n", __FUNCTION__));
        standard = SILABS_DVB_S2;
    }
    else if(u32CurBroadcastType == DSS)
    {
        FE_DBG(("%s: DSS\n", __FUNCTION__));
        standard = SILABS_DSS;
    }
    else
        RETURN(FALSE);
    
    bIsHiLOF = FALSE;
    _DigiTuner_CalculateIF(drv_frontend_index,pSATParam,pParam);
    _DigiTuner_SetLNBPower(drv_frontend_index, pSATParam->eLNBPwrOnOff,pParam->SatParam.u8Polarity,pSATParam->bPorInvert);
    _DigiTuner_SwitchSatellite(drv_frontend_index, pSATParam,pParam->SatParam.u8Polarity,bIsHiLOF);
    *ptunerParam = *pParam; //Must move here
    *psatParam = *pSATParam;
        
    freq = (int)((pParam->u32Frequency)*1000);
    symbol_rate_bps = (unsigned int)pParam->SatParam.u16SymbolRate *1000;
    LockStatus = SiLabs_API_lock_to_carrier(front_end,    standard,freq, 
                                             bandwidth_Hz, 
                                             stream, 
                                             symbol_rate_bps, 
                                             constellation, 
                                             polarization, 
                                             band, 
                                             data_slice_id, 
                                             plp_id, 
                                             T2_lock_mode);


    if(LockStatus == 1)
    {
        if(TS_PARALLEL_OUTPUT)
            SiLabs_API_TS_Mode(front_end, SILABS_TS_PARALLEL);
        else
            SiLabs_API_TS_Mode(front_end, SILABS_TS_SERIAL);

        SiLabs_API_TS_Config(front_end, 0, 0, 2, 1, 0, 0);
        FE_MSG(("DMD_LOCK...\n"));
        SiLabs_API_Reset_Uncorrs(front_end);
    }
    else
        FE_MSG(("DMD_UNLOCK... LockStatus = %d\n", LockStatus));
    
    RETURN(TRUE);
#else
    return FALSE;
#endif 
}

#ifdef SUPPORT_DUAL_LNB
MS_BOOL MApi_DigiTuner_Tune2RfCh_DVBS_Ext(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam, EN_CABLE_SELECT eCableID)
{
    return TRUE;
}
#endif

MS_BOOL MApi_DigiTuner_Tune2RfCh(MS_U8 drv_frontend_index, MS_FE_CARRIER_PARAM *pParam, EN_FE_TUNE_MODE eMode)
{
#ifdef TERRESTRIAL_FRONT_END 
        SILABS_FE_Context    *front_end;
        MS_FE_CARRIER_PARAM* ptunerParam;
        signed int LockStatus;
        signed int standard = SILABS_ISDB_T;
        signed int freq = 0;                   //the frequency to lock to    (in Hz for TER, in kHz for SAT)
        signed int bandwidth_Hz = 0;       //the channel bandwidth in Hz (only for DVB-T, DVB-T2, ISDB-T)
        unsigned int symbol_rate_bps = 0;      //the symbol rate in baud/s   (for DVB-C, MCNS and SAT)
        unsigned int stream = 0;           //the HP/LP stream            (only for DVB-T)
        unsigned int constellation = 0;    //the DVB-C constellation     (only for DVB-C)
        unsigned int polarization = 0;     //
        unsigned int band = 0;             //
        signed int data_slice_id = 0;          //the DATA SLICE Id           (only for DVB-C2 when num_dslice  > 1)
        signed int plp_id = 0;                 //the PLP Id                  (only for DVB-T2 and DVB-C2 when num_plp > 1)
        unsigned int T2_lock_mode = 0;         //(0='ANY', 1='T2-Base', 2='T2-Lite')
    
        ENTRY();
        if(NULL == _ptunerParam)
            RETURN(FALSE);
    
        ptunerParam = _ptunerParam + drv_frontend_index;
        front_end = &(FrontEnd_Table[drv_frontend_index]);


        if(u32CurBroadcastType == ISDBT)
        {
            standard = SILABS_ISDB_T;
            switch(pParam->ISDBTParam.eBandWidth)
            {
                case BW_MODE_7MHZ:
                        bandwidth_Hz = 7000000;
                        break;
                case BW_MODE_8MHZ:
                        bandwidth_Hz = 8000000;
                        break;
                case BW_MODE_1_7MHZ:
                        bandwidth_Hz = 1700000;
                        break;
                case BW_MODE_6MHZ:
                default:    
                        bandwidth_Hz = 6000000;
                        break;
            }
        }
        else
        {
           FE_DBG(("%s: Select Broadcast Type Not Ready\n", __FUNCTION__));
           RETURN(FALSE);
        }
        
        *ptunerParam = *pParam; //Must move here

            
        freq = (int)((pParam->u32Frequency)*1000);

        LockStatus = SiLabs_API_lock_to_carrier(front_end,    standard,freq, 
                                                 bandwidth_Hz, 
                                                 stream, 
                                                 symbol_rate_bps, 
                                                 constellation, 
                                                 polarization, 
                                                 band, 
                                                 data_slice_id, 
                                                 plp_id, 
                                                 T2_lock_mode);
    
    
        if(LockStatus == 1)
        {
            if(TS_PARALLEL_OUTPUT)
                SiLabs_API_TS_Mode(front_end, SILABS_TS_PARALLEL);
            else
                SiLabs_API_TS_Mode(front_end, SILABS_TS_SERIAL);
            
            SiLabs_API_TS_Config(front_end, 0, 0, 2, 1, 0, 0);
            FE_MSG(("DMD_LOCK...\n"));
            SiLabs_API_Reset_Uncorrs(front_end);
        }
        else
            FE_MSG(("DMD_UNLOCK... LockStatus = %d\n", LockStatus));

        RETURN(TRUE);
#else
    return FALSE;
#endif    
}

//-------------------------------------------------------------------------------------------------
/// Report Estimated Maximum Lock Time of Tune2RF mode
/// @param eMode                    \b IN: Tuning mode (Auto or Manual)
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetMaxLockTime(MS_U8 drv_frontend_index, EN_FE_TUNE_MODE eMode, MS_U32 *u32LockTime)
{
    *u32LockTime = 6000;
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get tuner lock status
/// @param eFE                      \b IN: Frontend id
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetLock(MS_U8 drv_frontend_index, EN_LOCK_STATUS *peLockStatus)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();
    front_end = &(FrontEnd_Table[drv_frontend_index]);
    *peLockStatus = E_DEMOD_CHECKING;
    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
         if(stStatus.fec_lock)
            *peLockStatus = E_DEMOD_LOCK;

         RETURN(TRUE);
    }
    else
        RETURN(FALSE);
}

MS_BOOL MApi_DigiTuner_GetPreLock(MS_U8 drv_frontend_index, EN_LOCK_STATUS *peLockStatus)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetFlameSyncLock(MS_U8 drv_frontend_index, EN_LOCK_STATUS *peLockStatus)
{
    return TRUE;
}



//-------------------------------------------------------------------------------------------------
/// Get tuner SNR
/// @param pu32SNR                  \b OUT: Signal noise ratio
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetSNR(MS_U8 drv_frontend_index, MS_U16* pu16SNR)
{
    CUSTOM_Status_Struct stStatus;
    float fSNR;
    SILABS_FE_Context    *front_end;

    ENTRY();
    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
         fSNR = (float)stStatus.c_n_100/100;
         *pu16SNR = (MS_U16)stStatus.c_n_100/100;
         FE_DBG(("%s: %f dB\n", __FUNCTION__, fSNR));
         RETURN(TRUE);
    }
    else
        RETURN(FALSE);

}


//-------------------------------------------------------------------------------------------------
/// Get tuner BER
/// @param pu32BER                  \b OUT: Bit error ratio (n * 2^-32)
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetBER(MS_U8 drv_frontend_index, float *pfltBER)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);
    
    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        *pfltBER = stStatus.ber;
         FE_DBG(("%s: %8.3e\n", __FUNCTION__, *pfltBER));
         RETURN(TRUE);
    }
    else
        RETURN(FALSE);
}


//-------------------------------------------------------------------------------------------------
/// Get tuner Signal Power Strength
/// @param pu8PWR                   \b OUT: Signal power strength
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetPWR(MS_U8 drv_frontend_index, MS_S16* ps16PWR)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        *ps16PWR = (MS_S16)stStatus.RSSI;
        FE_DBG(("%s: %d dBm\n", __FUNCTION__, *ps16PWR));
        RETURN(TRUE);
    }
    else
        RETURN(FALSE);

 return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get signal strength indicator (SSI)
/// @param pu16SSI                   \b OUT: Signal power strength
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetSSI(MS_U8 drv_frontend_index, MS_U16* pu16SSI)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        *pu16SSI = (MS_U16)stStatus.SSI;
         FE_DBG(("%s: %d (0 ~ 100)\n", __FUNCTION__, *pu16SSI));
         RETURN(TRUE);
    }
    else
        RETURN(FALSE);
}


//-------------------------------------------------------------------------------------------------
/// Get tuner Signal Power Strength
/// @param pu8PWR                   \b OUT: Signal power strength
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetSignalQuality(MS_U8 drv_frontend_index, MS_U16* pu16quality)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        *pu16quality = stStatus.SQI;
         FE_DBG(("%s: %d (0 ~ 100)\n", __FUNCTION__, *pu16quality));
         RETURN(TRUE);
    }
    else
        RETURN(FALSE);
}

//-------------------------------------------------------------------------------------------------
/// Get tuner Signal Power Strength from Tuner
/// @param pu8PWR                   \b OUT: Signal power level
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetPWRFromTuner(MS_U8 drv_frontend_index, float* pfPWR)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        *pfPWR = (float)stStatus.SSI;
         FE_DBG(("%s: %5.2f dBm\n", __FUNCTION__, *pfPWR));
         RETURN(TRUE);
    }
    else
        RETURN(FALSE);
}

//-------------------------------------------------------------------------------------------------
/// Get packet error
/// @param RegData                   \b OUT: packet error
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_Get_Packet_Error(MS_U8 drv_frontend_index, MS_U16 *RegData)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;

    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);

    
    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        *RegData = (MS_U16)stStatus.uncorrs;
         FE_DBG(("%s: %d\n", __FUNCTION__, (int)stStatus.uncorrs));
         SiLabs_API_Reset_Uncorrs(front_end);
         RETURN(TRUE);
    }
    else
        RETURN(FALSE);

}


//-------------------------------------------------------------------------------------------------
/// Get tuner parameter setting
/// @param eFE                      \b IN: Frontend id
/// @param pstParam                 \b OUT: TPS carrier parameters
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetTPSInfo(MS_U8 drv_frontend_index, MS_FE_CARRIER_PARAM* pParam)
{
    CUSTOM_Status_Struct stStatus;
    SILABS_FE_Context    *front_end;
    MS_BOOL bret = TRUE;
#if (FE_DEBUG_OPTIONS & FE_EN_DBG)
    char*  cConStr[] = {"QPSK", "8PSK", "QAM16", "16APSK", "32APSK"};
    char*  cCRStr[] = {"1/2", "2/3", "3/4", "5/6", "7/8", "3/5","4/5", "1/3", "1/4", "2/5", "8/9", "9/10"};
    char*  cRollOffStr[] = {"0.35", "0.25", "0.20"};
#endif
    ENTRY();

    front_end = &(FrontEnd_Table[drv_frontend_index]);
  
    if(SiLabs_API_FE_status(front_end, &stStatus))
    {
        if(u32CurBroadcastType == DVBS || u32CurBroadcastType == DVBS2)
        {
            switch(stStatus.constellation)
            {
                 case SILABS_QPSK:
                    pParam->SatParam.eConstellation = SAT_QPSK;
                    break;
                 case SILABS_8PSK: 
                    pParam->SatParam.eConstellation = SAT_8PSK;
                    break;
                 case SILABS_16APSK:
                 case SILABS_16APSK_L:
                    pParam->SatParam.eConstellation = SAT_16APSK;
                    break;
                 case SILABS_32APSK_1:
                 case SILABS_32APSK_2:
                 case SILABS_32APSK_L:
                    pParam->SatParam.eConstellation = SAT_32APSK;
                    break;
                 default:
                    bret = FALSE;
                    FE_ERR(("Abnormal Constellation\n"));
                    break;
            }

             switch(stStatus.code_rate)
            {
                 case SILABS_CODERATE_1_2:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_1_2;
                    break;
                 case SILABS_CODERATE_2_3:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_2_3;
                    break;
                 case SILABS_CODERATE_3_4:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_3_4;
                    break;
                 case SILABS_CODERATE_4_5:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_4_5;
                    break;   
                 case SILABS_CODERATE_8_9:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_8_9;
                    break;
                 case SILABS_CODERATE_9_10:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_9_10;
                    break;
                 case SILABS_CODERATE_1_3:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_1_3;
                    break;
                 case SILABS_CODERATE_1_4:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_1_4;
                    break;
                 case SILABS_CODERATE_2_5:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_2_5;
                    break;
                 case SILABS_CODERATE_3_5:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_3_5;
                    break; 
                 case SILABS_CODERATE_7_8:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_7_8;
                    break; 
                 case SILABS_CODERATE_5_6:
                    pParam->SatParam.eCodeRate = CONV_CODE_RATE_5_6;
                    break;     
                 default:
                    bret = FALSE;
                    FE_ERR(("Abnormal Code Rate\n"));
                    break;
            }

            if(u32CurBroadcastType == DVBS)
            {
                pParam->SatParam.eRollOff = SAT_RO_35;
            }
            else
            {
                switch(stStatus.roll_off)
                {
                   case 2:
                      pParam->SatParam.eRollOff = SAT_RO_35;
                      break;
                   case 1:
                      pParam->SatParam.eRollOff = SAT_RO_25;
                      break;
                   case 0:
                      pParam->SatParam.eRollOff = SAT_RO_20;
                      break;
                   default:
                       bret = FALSE;
                       FE_ERR(("Abnormal Roll-Off factor\n"));
                      break;                  
                }
            }

            pParam->SatParam.fCFO  = (float)stStatus.freq_offset;
#if (FE_DEBUG_OPTIONS & FE_EN_DBG)
            if(bret)
            {
                FE_DBG(("Constellation: %s\n", cConStr[pParam->SatParam.eConstellation]));
                FE_DBG(("Code Rate    : %s\n", cCRStr[pParam->SatParam.eCodeRate]));
                FE_DBG(("Roll-Off     : %s\n", cRollOffStr[pParam->SatParam.eRollOff]));
                FE_DBG(("CFO          : %lf Hz\n", pParam->SatParam.fCFO));
            }
#endif
        }
        else
        {
            FE_DBG(("NOT Ready\n"));
        }
    }
    else
    {
       bret = FALSE;
    }

    RETURN(bret);

}

MS_BOOL MApi_DigiTuner_GetTSBitRate(MS_U8 drv_frontend_index, MS_U16* pu16TsBitRate)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Loop_Through_On(MS_U8 drv_frontend_index, MS_BOOL bOn)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Stand_By(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Wake_Up(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_Frontend_LoadResetPin(void)
{
    return TRUE;
}

//------------------------------------------------------
// function callback by  demod layer
//
//--------------------------------------------------------
//demod  reset pin
void MApi_Demod_HWReset(MS_U8 u8Port_Idx)
{
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(GPIO_FE_RST);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(GPIO_FE_RST);
    MsOS_DelayTask(5);

    return;
}



MS_BOOL MApi_DigiTuner_Get_FE_InitStatus(MS_U8 drv_frontend_index)
{
    SILABS_FE_Context    *front_end;
        
    front_end = &(FrontEnd_Table[drv_frontend_index]);

    return (MS_BOOL)front_end->init_ok;
}

MS_BOOL MApi_DigiTuner_Set_DMD_BUFFER(MS_U8 drv_frontend_index, MS_U32 u32StartAddr)
{
    return TRUE;
}

/*
MS_BOOL MApi_DigiTuner_SetStateChange(MS_BOOL bOn)
{
    ENTRY();
    StateChg = bOn;
    RETURN(TRUE);

}
*/

MS_BOOL MApi_DigiTuner_GetCellID(MS_U8 drv_frontend_index,MS_U16* pu16CellID)
{
    return FALSE;
}

#if(MS_DVBT2_INUSE == 1)

MS_U8 MApi_DigiTuner_GetCurrentDemodType(MS_U8 drv_frontend_index)
{
    return 0;
}


MS_BOOL MApi_DigiTuner_SetCurrentDemodType(MS_U8 drv_frontend_index, MS_U8 type)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetPlpGroupID(MS_U8 drv_frontend_index, MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetPlpGroupID(MS_U8 drv_frontend_index, MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetPlpBitMap(MS_U8 drv_frontend_index, MS_U8* u8PlpBitMap)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetPlpType(MS_U8 drv_frontend_index, MS_U8 u8PlpID, DEMOD_DVBT2_PLP_TYPE* pePLPTYPE)
{
    return TRUE;
}


#endif


#if MS_DVBS_INUSE


MS_BOOL MApi_DigiTuner_Demod_ClearStatus(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_U16 MApi_DigiTuner_getCurRFSignalSNR_Persent(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetRollOff(MS_U8 drv_frontend_index, EN_SAT_ROLL_OFF_TYPE *pRollOff)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetRFOffset(MS_U8 drv_frontend_index, MS_S16 *ps16RFOffset)
{
    return TRUE;
}


MS_BOOL MApi_DigiTuner_GetCurrentLOF(MS_U8 drv_frontend_index, MS_U16* pLOF)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_IsOverCurrent(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END 
    SILABS_FE_Context    *front_end;

    ENTRY();
    front_end = &(FrontEnd_Table[drv_frontend_index]);
    #ifdef A8304_COMPATIBLE
    extern void  L1_A8304_WriteBytes (A8304_Context *context);
    L1_A8304_WriteBytes(front_end->A8304);
    RETURN((MS_BOOL)front_end->A8304->OCP);
    #else
    RETURN(FALSE);
    #endif
#else
    return FALSE;
#endif
}


//---------------------------
// Function For Satellite
//----------------------------
MS_BOOL MApi_DigiTuner_Satellite_Set22K(MS_U8 drv_frontend_index, MS_BOOL b22KOn)
{
#ifdef SATELLITE_FRONT_END 
    SILABS_FE_Context    *front_end;
    MS_SAT_PARAM* pstSATParam = NULL;

    ENTRY();
    if(NULL == _pstSATParam)
        return FALSE;
    
     pstSATParam = _pstSATParam + drv_frontend_index;

    front_end = &(FrontEnd_Table[drv_frontend_index]);

    if(SiLabs_API_SAT_tone(front_end, (unsigned char)(b22KOn)) == 1)
    {
        pstSATParam->e22KOnOff = (MS_EN_22K_ONOFF)b22KOn;
        RETURN(TRUE);
    }
    else
        RETURN(FALSE);
#else
    return TRUE;
#endif
}

MS_BOOL MApi_DigiTuner_Satellite_Get22K(MS_U8 drv_frontend_index, MS_BOOL* pb22KOn)
{
    MS_BOOL bRet = FALSE;
    ENTRY();
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_Satellite_SetLNBPower(MS_U8 drv_frontend_index, MS_EN_LNBPWR_ONOFF enLNBPowe,MS_U8 u8Polarity,MS_BOOL bPorInvert)
{
#ifdef SATELLITE_FRONT_END
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_SetLNBPower(drv_frontend_index, enLNBPowe,u8Polarity,bPorInvert);
    RETURN(bRet);
#else
    return TRUE;
#endif

}


MS_BOOL MApi_DigiTuner_Satellite_Set22KTone(MS_U8 drv_frontend_index, MS_U32 u32ToneType)
{
#ifdef SATELLITE_FRONT_END 
    unsigned char sequence_buffer[4];
    SILABS_FE_Context    *front_end;
    MS_SAT_PARAM* pstSATParam = NULL;
    unsigned char cont_tone;

    ENTRY();

    if(NULL == _pstSATParam)
    {
        RETURN(FALSE);
    }
    
     pstSATParam = _pstSATParam + drv_frontend_index;
     cont_tone = (unsigned char)pstSATParam->e22KOnOff;
     front_end = &(FrontEnd_Table[drv_frontend_index]);

     if(u32ToneType)
        pstSATParam->eToneburstType = EN_TONEBURST_1;
     else
        pstSATParam->eToneburstType = EN_TONEBURST_0;

    if(SiLabs_API_SAT_send_diseqc_sequence(front_end, 0, sequence_buffer, cont_tone, 1, (unsigned char)u32ToneType, 1) == 1)
    {
        RETURN(TRUE);
    }
    else
    {
        RETURN(FALSE);
    }
#else
    return TRUE;
#endif    
}

#ifdef SUPPORT_DUAL_LNB
MS_BOOL MApi_DigiTuner_Satellite_Set22K_Ext(MS_U8 drv_frontend_index, MS_BOOL b22KOn, EN_CABLE_SELECT eCableID)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Satellite_SetLNBPower_Ext(MS_U8 drv_frontend_index, MS_EN_LNBPWR_ONOFF enLNBPowe,MS_U8 u8Polarity,MS_BOOL bPorInvert, EN_CABLE_SELECT eCableID)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Satellite_Set22KTone_Ext(MS_U8 drv_frontend_index, MS_U32 u32ToneType, EN_CABLE_SELECT eCableID)
{
    return TRUE;
}
#endif
//---------------------------
// Function For Blind scan
//----------------------------
MS_BOOL MApi_DigiTuner_BlindScan_Start(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_BlindScan_NextFreq(MS_U8 drv_frontend_index, MS_BOOL* bBlindScanEnd)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_BlindScan_WaitCurFeqFinished(MS_U8 drv_frontend_index, MS_U8* u8Progress,MS_U8* u8FindNum)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_BlindScan_GetChannel(MS_U8 drv_frontend_index, MS_U16 u16ReadStart,MS_U16* u16TPNum,MS_FE_CARRIER_PARAM *pTPTable)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_BlindScan_Cancel(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_BlindScan_End(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_BlindScan_GetCurrentFreq(MS_U8 drv_frontend_index, MS_U32 *u32Freq)
{
    return TRUE;
}
//------------------------------------------------------
// DiSEqC 1.0/1.1
//------------------------------------------------------

MS_BOOL MApi_DigiTuner_DiSEqC_SendCommand(MS_U8 drv_frontend_index, MS_U8* pCmd,MS_U8 u8CmdSize)
{
#ifdef SATELLITE_FRONT_END 
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, pCmd, u8CmdSize);
    RETURN(bRet);
#else
    return TRUE;
#endif
}


MS_BOOL MApi_DigiTuner_DiSEqC_GetReply(MS_U8 drv_frontend_index, DISEQC_RX_MSG* pstRxMSG)
{
#ifdef SATELLITE_FRONT_END 
        MS_BOOL bRet = TRUE;
        SILABS_FE_Context    *front_end;
        signed   int reply_length;  
        unsigned char reply_buffer[32];  
        
        front_end = &(FrontEnd_Table[drv_frontend_index]);

        ENTRY();
        bRet = SiLabs_API_SAT_read_diseqc_reply(front_end, &reply_length, reply_buffer);
        if(bRet)
        {
            pstRxMSG->u8Nbyte = (MS_U8)reply_length;
            memcpy(pstRxMSG->u8Msg, reply_buffer, pstRxMSG->u8Nbyte);
        }
        RETURN(bRet);
#else
        return TRUE;
#endif

}


MS_BOOL MApi_DigiTuner_DiSEqC_SwitchPort(MS_U8 drv_frontend_index, MS_U8 u8Port,MS_BOOL bUnCommited)
{
#ifdef SATELLITE_FRONT_END 
    unsigned char sequence_buffer[] = {0xE2,0x10,0x38,0xF0};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet &=  _get_DiSEqC_SwitchPortCmd(u8Port, bUnCommited, sequence_buffer);
    bRet &= _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}


#ifdef SUPPORT_DUAL_LNB
MS_BOOL MApi_DigiTuner_DiSEqC_SwitchPort_Ext(MS_U8 drv_frontend_index, MS_U8 u8Port,MS_BOOL bUnCommited, EN_CABLE_SELECT eCableID)
{
    return TRUE;
}
#endif

MS_BOOL MApi_DigiTuner_DiSEqC_Reset(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END 
    unsigned char sequence_buffer[]= {0xE0,0x31,0x00};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}
//------------------------------------------------------
// DiSEqC 1.2
//------------------------------------------------------
MS_BOOL MApi_DigiTuner_DiSEqC_SetEastLimit(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END 
    unsigned char sequence_buffer[]= {0xE0,0x31,0x66};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

MS_BOOL MApi_DigiTuner_DiSEqC_SetWestLimit(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END 
    unsigned char sequence_buffer[]= {0xE0,0x31,0x67};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif   
}

MS_BOOL MApi_DigiTuner_DiSEqC_DisableLimit(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x63};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif     
}

MS_BOOL MApi_DigiTuner_DiSEqC_GoReferencePoint(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x6B,0x00};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif     
}

#if   DISEQC13_ONLY_GOTOX
MS_BOOL MApi_DigiTuner_DiSEqC_GoSatPos(MS_SAT_PARAM *pSATParam)
{
    return TRUE;
}
#else
MS_BOOL MApi_DigiTuner_DiSEqC_GoSatPos(MS_U8 drv_frontend_index, MS_U8 u8SatPos)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[4];
    MS_BOOL bRet = TRUE;
    
    ENTRY();
    u8SatPos &= DISEQC12_USALS_POSITION_MASK;
    bRet &= _get_DiSEqC_GotoSatPositionCmd(u8SatPos, sequence_buffer);
    bRet &= _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif   
}
#endif

MS_BOOL MApi_DigiTuner_DiSEqC_SaveSatPos(MS_U8 drv_frontend_index, MS_U8 u8SatPos)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x6A,u8SatPos};
    MS_BOOL bRet = TRUE;

    ENTRY();
    u8SatPos &= DISEQC12_USALS_POSITION_MASK;
    sequence_buffer[3] = u8SatPos;
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorConinuteEast(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x68,0x00};
    MS_BOOL bRet = TRUE;
    
    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorConinuteWest(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x69,0x00};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorStepEast(MS_U8 drv_frontend_index, MS_U8 u8Step)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x68,0xFE};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorStepWest(MS_U8 drv_frontend_index, MS_U8 u8Step)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x69,0xFE};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

MS_BOOL MApi_DigiTuner_DiSEqC_HaltMotor(MS_U8 drv_frontend_index)
{
#ifdef SATELLITE_FRONT_END
    unsigned char sequence_buffer[]= {0xE0,0x31,0x60};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet =  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, sequence_buffer, sizeof(sequence_buffer));
    RETURN(bRet);
#else
    return TRUE;
#endif    
}

//------------------------------------------------------
// DiSEqC USALS
//------------------------------------------------------
MS_BOOL MApi_DigiTuner_DiSEqC_GotoX(MS_U8 drv_frontend_index, MS_U16 u16SatLongitude,MS_S16 s16MyLongitude,MS_S16 s16MyLatitude)
{
    return TRUE;
}


MS_BOOL MApi_DigiTuner_IsNeedCheckCurrent(void)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetVCM_ISID_INFO(MS_U8 drv_frontend_index, MS_U8* pu8IS_ID, MS_U8* pu8Table)
{
    return FALSE;
}


MS_BOOL MApi_DigiTuner_SetVCM_ISID(MS_U8 drv_frontend_index, MS_U8 u8IS_ID)
{
    return TRUE;
}

#endif
MS_BOOL MApi_DigiTuner_Frontend_List_Show(void)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_Frontend_Scan(void)
{
     return TRUE;
}

MS_BOOL MApi_DigiTuner_GetFEtab(MS_U8 drv_frontend_index, DEV_FRONTEND_TYPE** pFETab)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetDemodName(MS_U8 drv_frontend_index, char* name)
{
    return FALSE;
}

MS_BOOL MApi_DigiTuner_GetTunerName(MS_U8 drv_frontend_index, char* name)
{
    return FALSE;
}

MS_BOOL MApi_DigiTuner_DemodSupportINT(MS_U8 drv_frontend_index)
{
    return FALSE;
}

MS_BOOL MApi_DigiTuner_Set_MemPool(E_DDI_FE_SYSPOOLID eSYSPOOL,MS_S32 s32PoolID)
{
    if (eSYSPOOL == E_DDI_FE_POOL_SYS_CACHE)
    {
        gs32FECachedPoolID = s32PoolID;
    }
    else if (eSYSPOOL == E_DDI_FE_POOL_SYS_NONCACHE)
    {
        gs32FENonCachedPoolID = s32PoolID;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetPowerState(EN_POWER_MODE emod)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetTunerParam(MS_U8 drv_frontend_index,MS_FE_CARRIER_PARAM* pParam)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetISDBTLayer(MS_U8 drv_frontend_index,EN_ISDBT_Layer eISDBT_Layer)
{
    return TRUE;
}


MS_BOOL MApi_DigiTuner_FEIdx_to_DevIdx(MS_U8 drv_frontend_index, MS_U8* pu8DevIdx)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetDMD_PIDFilter(MS_U8 drv_frontend_index, MS_FE_PID_FILTER* pstPIDFlt)
{
    return TRUE;
}


//#ifdef DDI_MISC_INUSE
#ifdef FE_AUTO_TEST
//-------------------------------------------------------------------------------------------------
/// Get Demod Read Reg
/// @param eFE                      \b IN: Frontend id
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_ReadReg(MS_U8 drv_frontend_index, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_WriteReg(MS_U8 drv_frontend_index, MS_U16 RegAddr, MS_U16 RegData)
{
    return TRUE;
}


MS_BOOL MApi_DigiTuner_Reset(MS_U8 drv_frontend_index, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    return TRUE;
}

#endif
//#endif

#if MS_DVBT2_INUSE
void MApi_DigiTuner_CtrlResetDJBFlag(MS_U8 drv_frontend_index, MS_U8 Ctrl)
{

}

MS_BOOL MApi_DigiTuner_T2MI_Restart(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_InitParameter(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_U8 MApi_DigiTuner_GetPlpIDList(MS_U8 drv_frontend_index)
{
    return 0;
}

MS_U8 MApi_DigiTuner_GetNextPlpID(MS_U8 drv_frontend_index,MS_U8 u8Index)
{
    return 0;
}

MS_BOOL MApi_DigiTuner_SetScanTypeStatus(MS_U8 drv_frontend_index,MS_U8 status)
{
    return TRUE;

}

MS_U8 MApi_DigiTuner_GetScanTypeStatus(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetT2Reset(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetT2Restart(MS_U8 drv_frontend_index)
{
    return TRUE;
}

MS_U8 MApi_DigiTuner_Get_Current_Plp_Id(void)
{
    return 0;
}

#endif //#if MS_DVBT2_INUSE

#ifdef DDI_MISC_INUSE
#if (DDI_MISC_INUSE==1)

#if (BOOT_TYPE == BOOT_TYPE_FAST)
void MApi_DigiTuner_Wakeup(MS_U8 drv_frontend_index)
{

}
#endif

#endif //#if DDI_MISC_INUSE
#endif //#ifdef DDI_MISC_INUSE

