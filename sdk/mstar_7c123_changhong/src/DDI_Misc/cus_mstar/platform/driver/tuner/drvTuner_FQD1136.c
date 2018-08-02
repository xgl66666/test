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
/// file    drvTuner_FQD1136.c
/// @brief  NuTune FQD1136 Tuner Driver Interface
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "Board.h"
#include "drvIIC.h"
#include "drvTuner.h"
#include "drvDemod.h"

#if IF_THIS_TUNER_INUSE(TUNER_NUTUNE_FQD1136)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#if((FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC))
#if   (FRONTEND_TUNER_CTRL == PIU_IIC_0)
#define IIC_WRITE                   MDrv_IIC_Write
#define IIC_READ                    MDrv_IIC_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_1)
#define IIC_WRITE                   MDrv_IIC1_Write
#define IIC_READ                    MDrv_IIC1_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_NONE)
#define REG(addr)                   (*(volatile MS_U8*)(addr))
#else
#error "FRONTEND_TUNER_CTRL"
#endif
#else
#if   (FRONTEND_TUNER_CTRL == PIU_IIC_0)
#define IIC_WRITE                   Tuner_IIC_Write
#define IIC_READ                    Tuner_IIC_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_1)
#define IIC_WRITE                   Tuner_IIC_Write
#define IIC_READ                    Tuner_IIC_Read
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_NONE)
#define REG(addr)                   (*(volatile MS_U8*)(addr))
#else
#error "FRONTEND_TUNER_CTRL"
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define DIVIDER_RATIO_62_5KHz   0x06
#define DIVIDER_RATIO      DIVIDER_RATIO_62_5KHz
#define PAL_DBG_MSG(x)  x

#define DIGITAL_TUNER_IF    44

//#define _TUNER_I2C_ID       0xC0
#define _TUNER_I2C_ID       0xC2
//#define _TUNER_I2C_ID       0xC4
//#define _TUNER_I2C_ID       0xC6

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 dmdConfig[] =
{
#if 0
#if (MS_BOARD_TYPE_SEL == BD_MST098B_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098C_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098D_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01B_S)
      0x01, 0x22, 0x8d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
      0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
      0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
      0x52, 0x52, 0x55,
#endif
#else
#if (MS_BOARD_TYPE_SEL == BD_MST098B_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098C_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098D_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01A_S) || (MS_BOARD_TYPE_SEL == BD_MST098E_D01B_S)
      0x01, 0xE0, 0xAB, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x73, 0x00, 0xb3, 0x33, 0x03, 0x05, 0x52, 0x52, 0x73, 0x73, 0x73, 0x31, 0x52, 0x73, 0x73, 0x73,
      0x72, 0xc0, 0x24, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x7f, 0x0e, 0x2b, 0x0b, 0x47, 0x40,
      0xd3, 0x9a, 0xde, 0x11, 0xa4, 0x60, 0x08, 0x08, 0x00, 0x10, 0x02, 0x0b, 0x03, 0x05, 0x03, 0x82,
      0x52, 0x52, 0x55,
#endif
#endif
};



//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

#if 0
void devTunerSetFreqStep(FREQSTEP eFreqStep)
{
    switch(eFreqStep)
    {
    case FREQSTEP_62_5KHz:
        m_eFrequencyStep = eFreqStep;
        break;

    default:
        m_eFrequencyStep = FREQSTEP_62_5KHz;
        break;
    }
}


void devTunerSetPLLData(WORD wPLLData, RFBAND eBand)
{
    BYTE cTunerData[4];

    // Write DB1,DB2,CB1,CB2
    cTunerData[0] = HIBYTE(wPLLData);
    cTunerData[1] = LOBYTE(wPLLData);
    cTunerData[2] = 0xC8|DIVIDER_RATIO;
    if(eBand == E_RFBAND_VHF_LOW)
    {
        cTunerData[3] = 0x01;
    }
    else if(eBand == E_RFBAND_VHF_HIGH)
    {

        cTunerData[3] = 0x02;
    }
    else
    {
        cTunerData[3] = 0x04;
    }
    IIC_WRITE(_TUNER_I2C_ID, NULL, 0, cTunerData, 4);
}




void devTunerInit(void)
{
    BYTE cTunerData[4];

    //devTunerSetFreqStep(TN_FREQ_STEP); //Type mismatch,
    cTunerData[0] = 0;
    cTunerData[1] = 0;
    cTunerData[2] = 0xC8|DIVIDER_RATIO;
    cTunerData[3] = 0x01;
    IIC_WRITE(_TUNER_I2C_ID, NULL, 0, cTunerData, 4);
}

#endif

#if((FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC))
#else
MS_BOOL Tuner_IIC_Write(MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{
    MDrv_Demod_I2C_ByPass(1);
    if ( FALSE == MDrv_IIC_WriteBytes(FRONTEND_TUNER_CTRL, u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("PassThroughI2C_WriteBytes Error \n");
        MDrv_Demod_I2C_ByPass(0);
        return FALSE;
    }
    MDrv_Demod_I2C_ByPass(0);
    MsOS_DelayTask(1); // delay 1ms
    return TRUE;
}
MS_BOOL Tuner_IIC_Read(MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{
    MDrv_Demod_I2C_ByPass(1);
    //MDrv_IIC_SelectCLK(ePort, E_HWI2C_HIGH);
    MDrv_IIC_ReadBytes(FRONTEND_TUNER_CTRL, u8SlaveID, u8AddrNum, paddr, u16size, pu8data);
    //MDrv_IIC_SelectCLK(ePort, E_HWI2C_SLOW);
    MDrv_Demod_I2C_ByPass(0);
    return TRUE;
}
#endif
/*************************************************************************
  Subject:    FQD1116 tuner initialized function
  Function:   MDrv_Tuner_Init
  Parmeter:   CONFIG: pointer to tuner address
  Return :    NONE
  Remark:
**************************************************************************/
void devDigitalTuner_Init(void)
{
    unsigned char CONFIG[4];

    printf("%04d %s\n", __LINE__, __FUNCTION__);

    CONFIG[0] = 00;
    CONFIG[1] = 00;
    CONFIG[2] = 0xC8|DIVIDER_RATIO;
    CONFIG[3] = 0x01;
    IIC_WRITE(_TUNER_I2C_ID, NULL, 0, CONFIG, 4);


}

//------------------------------------------------------------------------------
//  Subject:    FQD1116 tuner paremeter write
//  Function:   Set_Digital_TUNER
//  Parmeter:   Freq: "RF+IF" in MHz
//                 eBandWidth: 6MHZ, 7MHZ, 8MHZ
//                 CONFIG: pointer to tuner address
//  Return :    NONE
//  Remark:
//------------------------------------------------------------------------------
void devDigitalTuner_SetFreq (double Freq)
{
    unsigned int value;
    unsigned char CONFIG[4];
    unsigned char status;
    printf("%04d %s %f\n", __LINE__, __FUNCTION__, Freq);
    // always init tuner here because annalog will chane the init value
    // todo: consider app flow, don't init value every channel change.
    //MDrv_Tuner_Init();
    value = ( unsigned int ) ( ( Freq + DIGITAL_TUNER_IF ) * 16);

    //DB1
    CONFIG[0] = ( unsigned char ) ( value >> 8 );
    //DB2
    CONFIG[1] = ( unsigned char ) ( value );
    //CB1
    CONFIG[2] = 0xC8|DIVIDER_RATIO;
    //CB2
    //low band
    if(Freq < 159)
    {
        CONFIG[3] = 0x01;
    }
    //mid band
    else if(Freq < 442)
    {
        CONFIG[3] = 0x02;
    }
    //high band
    else
    {
        CONFIG[3] = 0x04;
    }

    printf("\r\n%dMhz ",(MS_U16)Freq);

    printf("Tuner: 0x%x, 0x%x, 0x%x, 0x%x\n", CONFIG[0], CONFIG[1], CONFIG[2], CONFIG[3]);

    IIC_WRITE(_TUNER_I2C_ID, NULL, 0, CONFIG, 4);

    #if 1
    //MsOS_DelayTask(100);

    MS_U8 temp = 0;
    while(temp < 1)
    {
        status = 0;
        IIC_READ(_TUNER_I2C_ID, NULL, 0, &status, 1);

        printf("status = 0x%x \n",status);
        //MsOS_DelayTask(100);
        temp ++;
    }
    #endif
}


/**************************************************************************************************
  Subject:    TD1136_SetRF
  Function:   TD1136_SetRF
  Parameter:  Freq (MHz):
                Freq here mean "pure RF" rather than "RF+IF"
              Mode:
                0 ----> NTSC
                1 ----> ATSC
                2 ----> DVB-T BW=6MHz
  Return :    1 (I2C_FAIL) mean I2C fail, 0 (I2C_PASS) mean I2C success
  Remark:     None
***************************************************************************************************/
MS_BOOL TD1136_SetRF(double Freq, unsigned char Mode)
{
    double IF_Freq;
    unsigned int value;
    unsigned char DB1,DB2,CB,BB,AB;
    unsigned char CONFIG[4];
    unsigned char status;
    printf("%04d %s %f %d\n", __LINE__, __FUNCTION__, Freq, Mode);
    switch (Mode)
    {
           case 0: //NTSC
                 IF_Freq = 45.75;
                 break;
           case 1: //ATSC
           case 2: //DVB-T BW=6MHz
                 IF_Freq = 44.00;
                 break;
    }

    //Calculate from here !
    value = (unsigned int)((Freq + IF_Freq) / 0.0625 + 0.5);

    //DB1
    DB1 = (unsigned char)(value >> 8);

    //DB2
    DB2 = (unsigned char)(value);

    //CB
    switch (Mode)
    {
           case 0: //NTSC
                 CB = 0xB6;
                 break;
           case 1: //ATSC
                 if (Freq <= 440.00)
                    CB = 0xB6;
                 else if (Freq <= 863.00)
                    CB = 0xBE;
                 break;
           case 2: //DVB-T BW=6MHz
                 if (Freq <=120.00)
                    CB = 0xF6;
                 else if (Freq <= 159.00)
                    CB = 0xFE;
                 else if (Freq < 750.00)
                    CB = 0xF6;
                 else if (Freq <= 863.00)
                    CB = 0xFE;
                 break;
    }

    //BB
    switch (Mode)
    {
           case 0: //NTSC
                 if (Freq <= 159.00)
                    BB = 0x01;
                 else if (Freq <= 442.00)
                    BB = 0x02;
                 else if (Freq <= 863.00)
                    BB = 0x04;
                 break;
           case 1: //ATSC
           case 2: //DVB-T BW=6MHz
                 if (Freq <= 159.00)
                    BB = 0x01;
                 else if (Freq <= 442.00)
                    BB = 0x02;
                 else if (Freq <= 863.00)
                    BB = 0x04;
                 break;
    }

    if (Mode == 0) //NTSC
       AB = 0x50;
    else if (Mode == 1)  //ATSC
       AB = 0x20;
    else  //DVB-T BW=6MHz
       AB = 0x20;



    //First is to initialize AB Byte of TD1136 !
    //S --> DB1 --> DB2 --> CB --> AB --> P
    #if 0
    if (IIC_WRITE(_TUNER_I2C_ID, DB1, DB2, CB & 0xC7 | 0x18, AB) == FALSE)
       return FALSE;
    #else
    CONFIG[0] = DB1;
    CONFIG[1] = DB2;
    CONFIG[2] = ((CB & 0xC7)|0x18);
    CONFIG[3] = AB;

    if (IIC_WRITE(_TUNER_I2C_ID, NULL, 0, CONFIG, 4) == FALSE)
    {
        printf("IIC_WRITE1 fail \n");
         while(1);
        return FALSE;
    }
    #endif
    //Wait 1ms
    MsOS_DelayTask(10);

    //After that, Send DB1,DB2,CB,BB here !
    //S --> DB1 --> DB2 --> CB --> BB --> P
    #if 0
    if (IIC_WRITE(_TUNER_I2C_ID, DB1, DB2, CB, BB) == FALSE)
       return FALSE;
    #else
    CONFIG[0] = DB1;
    CONFIG[1] = DB2;
    CONFIG[2] = CB;
    CONFIG[3] = BB;

    if (IIC_WRITE(_TUNER_I2C_ID, NULL, 0, CONFIG, 4) == FALSE)
    {
         printf("IIC_WRITE2 fail \n");
         while(1);
        return FALSE;
    }
    #endif

    #if 1
    //MsOS_DelayTask(100);

    MS_U8 temp = 0;
    while(temp < 1)
    {
        status = 0;
        IIC_READ(_TUNER_I2C_ID, NULL, 0, &status, 1);

        printf("status = 0x%x \n",status);
        //MsOS_DelayTask(100);
        temp ++;
    }
    #endif

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Tuner_SetFreq(MS_U32 u32Freq)
{

    //devDigitalTuner_SetFreq ((((double)u32Freq)/1000.0));
    printf("%04d %s %d\n", __LINE__, __FUNCTION__, u32Freq);
    TD1136_SetRF((((double)u32Freq)/1000.0),1);
    return TRUE;
}


MS_BOOL MDrv_Tuner_PowerOnOff(MS_BOOL bPowerOn)
{
    //[TBD]
    return TRUE;
}


MS_BOOL MDrv_Tuner_Init(void)
{

    //devDigitalTuner_Init();
    return TRUE;
}



MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{
    return TRUE;
}


MS_U32 MDrv_Tuner_Stand_By(void)
{
    return TRUE;
}


MS_U32 MDrv_Tuner_Wake_Up(void)
{
    return TRUE;
}
#endif // TUNER_PHILIPS_CD1616LF_GIGH_4

