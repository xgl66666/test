////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

#include <common.h>
#include <device/COFDM_Demodulator.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvIIC.h>
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include <frontend_config.h>
#endif
extern void MsOS_DelayTask (MS_U32 u32Ms);

#if ((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD))

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
#include "MSB123x_TUNER_MXL201RF.c"
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
#include "MSB123x.c"
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
#include "MSB122x.c"
#elif (FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
#include "lgdt3305.c"
#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD)
#include "DIB8096.c"
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
#include "Uranus4_Internal.c"
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
#include "Kronus_InternalDVBT.c"
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
#include "Kronus_InternalDVBC.c"
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define COFDM_DBG(x)  //x

extern unsigned char bCardAInside;
extern void msAPI_Timer_Delayms(unsigned long u32DelayTime); //unit = ms


void devCOFDM_Init(void)
{
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    INTERN_DVBT_Power_On_Initialization();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
    DTV_Power_On_Initialization();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
    DTV_Power_On_Initialization();
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD|| FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    Demod_Reset_On();
    MsOS_DelayTask(20);
    Demod_Reset_Off();
    MsOS_DelayTask(30);
#else
    // hard-wire reset pin
  	Demodulator_OFF();
    MsOS_DelayTask(20);
    Demodulator_ON();
    MsOS_DelayTask(30);
#endif

#if (FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD)
    //SWIIC_SetIICSpeed( 150 );
    LGS_SelectADMode(0);
    //SWIIC_SetIICSpeed( 10 );
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
    LGS_SoftReset();
    MsOS_DelayTask(5);
    LGS8G52_Init();

    LGS8G52_SelectADMode(0);
//    LGS_SetMpegMode(0, 1,0);

    LGS_SetAutoMode();

    MsOS_DelayTask(10); //add 20080616
//    LGS_SetManualMode();
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    {
        INT8 err = LGS_NO_ERROR;

        LGS_RegisterWait(msAPI_Timer_Delayms);
        err = LGS8G75_SoftReset();
        msAPI_Timer_Delayms(5);
        LGS8G75_Init();
        msAPI_Timer_Delayms(10);
        LGS8G75_SelectADMode(0);
        msAPI_Timer_Delayms(5);
    }
    LGS8G75_SelectTunerType(1);
    msAPI_Timer_Delayms(10);
    LGS8G75_SetAutoMode();
    msAPI_Timer_Delayms(10);
//    LGS8G75_SetManualMode();
//    msAPI_Timer_Delayms(5);
//    LGS8G52_AGC(0xF0,0x00,0xA2);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
	lgs8g80_device_register();
    	MsOS_DelayTask(5);
	LGS8G80_DownloadFirmware();
	LGS8G80_SelectADMode(0x00);
	LGS8G80_SelectTunerType(0x01);

	//LGS8G80_SoftReset();
	LGS8G80_Init(NULL);
    LGS8G80_SetAutoMode();

    MsOS_DelayTask(10);
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    ATBMInit();
#endif
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_Power_On_Initialization();

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    MSB1200_Power_On_Initialization();

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

   MSB1210_Power_On_Initialization();

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD   \
||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD    \
||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    //SWIIC_SetIICSpeed( 150 );
    devCOFDM_SwitchTransportSteamInferface(E_PARALLEL_INTERFACE);
    //SWIIC_SetIICSpeed( 10 );
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)

    MT5131_Initialize();

#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    CE6355_Init();

#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
//    printf("---> Set Clk Inv =1 \n");//??? To be modified: Set Demod to clk invert
     //XBYTE[0x1E26] |= 0x02;
/*
     {
        U8 u8Data;

        u8Data = MDrv_ReadByte(0x1E26);
        u8Data|=0x02;
        MDrv_WriteByte(0x1E26,u8Data);
      }
*/
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    TC90517_Power_On_Initialization();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    M88DC2800_Initialize();

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    M88DD2000_Initialize();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    MDrv_IIC_Init();
    if (MDrv_Demod_Init() == FALSE)
    {
        printf("%s: Init demodulator fail\n", __FUNCTION__);
    }
    else
    {
        printf("MDrv_Demod_Init ok \n");
    }


    if (MDrv_Demod_Open() == FALSE)
    {
        printf("%s: Open demodulator fail\n", __FUNCTION__);
        return;
    }
    else
    {
        printf("MDrv_Demod_Open ok \n");
    }
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
    Connect(DTV_Get_Demod_TYPE());
    MSB123x_Power_On_Initialization(1);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    Connect(DTV_Get_Demod_TYPE());
    MSB122x_Power_On_Initialization(1);
#elif (FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
    MDrv_Demod_Init();

#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    Power_On_Initialization();

#elif (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x || FRONTEND_DEMOD_TYPE == DEMOD_AVL6211 || \
       FRONTEND_DEMOD_TYPE == DEMOD_SamSung_S5H1411 || FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || \
       FRONTEND_DEMOD_TYPE == MSTAR_MSB131X_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD|| FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    MApi_FE_Power_On_Initialization();
#endif //end ENABLE_HUMMINGBIRD_CUSTOMER
#endif

#if ((FRONTEND_DEMOD_TYPE != MSTAR_MSB123x_DEMOD) || (FRONTEND_TUNER_TYPE != TUNER_MXL201RF))
#if (!ENABLE_HUMMINGBIRD_CUSTOMER)
    devDigitalTuner_Init();
#endif
#endif
    //printf("%s,%d\n",__FILE__,__LINE__);

    //devCOFDM_SetFrequency(562000, E_RF_CH_BAND_8MHz);   // <<< Ken 20090629
    //printf("%s,%d\n",__FILE__,__LINE__);

}

void devCOFDM_ControlPowerOnOff(BOOLEAN bPowerOn)
{
#if 0//!_TEMP_S8_
    return;
#endif

#if (FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF || FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF_MSB1210 || FRONTEND_IF_DEMODE_TYPE == MSTAR_INTERN_VIF)
    if (bPowerOn == ENABLE) // DTV mode
    {
        EXT_RF_AGC_OFF();
        //printf("KKK: (Temp)EXT_RF_AGC_OFF...for DTV\n");
    }
    else
    {
        EXT_RF_AGC_ON();
        //printf("KKK: (Temp)EXT_RF_AGC_On...for ATV\n");
    }
#endif




#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_Power_ON_OFF(bPowerOn);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    MSB1200_Power_ON_OFF(bPowerOn);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    MSB1210_Power_ON_OFF(bPowerOn);

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    //printf("\ndevCOFDM_ControlPowerOnOff %d\n",(int) bPowerOn);
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    bPowerOn = bPowerOn;
//    printf("\nThis is MTK solution\n");
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    CE6355_InitRegister(FALSE);

    if(FALSE == bPowerOn)
        CE6355_MCLOKEnable(bPowerOn);

#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    //TC90512XBG_Power_ON_OFF(bPowerOn);
    bPowerOn = bPowerOn;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    //TC90517_Power_ON_OFF(bPowerOn);
    bPowerOn = bPowerOn;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    bPowerOn = bPowerOn;
//    printf("\nThis is MONTAGE solution\n");
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    bPowerOn = bPowerOn;
//    printf("\nThis is MONTAGE solution\n");
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    bPowerOn = bPowerOn;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    INTERN_DVBT_Power_ON_OFF(bPowerOn);
#endif
}

FUNCTION_RESULT devCOFDM_PowerSave(void)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    if (MSB1200_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    if (MSB1210_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    if (INTERN_DVBT_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    return E_RESULT_SUCCESS;
#endif
}

void devCOFDM_TunerProgramming(U8 *cTunerData)
{
    #if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
        ZL10353_Tuner_Programming( cTunerData );
    #elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
        MSB1200_Tuner_Programming( cTunerData );
    #elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
        *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *cTunerData=*cTunerData;
    #elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    *cTunerData=*cTunerData;
    #else
    *cTunerData=*cTunerData;
    #endif
}

#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devCOFDM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetFrequencyOffset(pFreqOff, u8BW);
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetFrequencyOffset(pFreqOff, u8BW);
#else
    *pFreqOff = 0;  //Temp Solution;;
    u8BW = u8BW; //To avoid compiler warning
    return 1;
#endif
}
BOOLEAN devCOFDM_SetAfcEnable(BOOLEAN bEnable)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    bEnable = bEnable;
    return 1;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_SetAfcEnable(bEnable);
#else
    bEnable = bEnable; //To avoid compiler warning
    return 1;
#endif
}

#endif

void devCOFDM_SetFrequency(DWORD dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth, RF_CHANNEL_HP_LP eHpLp, U8 u8PlpID)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    BOOLEAN                            b_pal_bg;
#endif
    UNUSED(u8PlpID);
#if ((FRONTEND_DEMOD_TYPE != EMBEDDED_DVBT_DEMOD) && (FRONTEND_DEMOD_TYPE != MSTAR_MSB1210_DEMOD))
    UNUSED(eHpLp);
#endif
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    MSB1200_Active(DISABLE);
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    MT5131_SAMSUNG_NIM_Initialize();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    devDigitalTuner_Init();

    TC90512XBG_Channel_Selection();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)

#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE != TUNER_MXL201RF))
    MSB123x_Active(DISABLE);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    BOOLEAN ret = FALSE;
    ret = MSB122x_Active(DISABLE);
    printf("MSB122x_Active OK ret %d\n",ret);
#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    Active(DISABLE);
#endif

    //dwFrequency = 562000;                   // << Ken 20090629
    //eBandWidth = E_RF_CH_BAND_8MHz;         // << Ken 20090629

    //printf("====dwFrequency = %lu \n", dwFrequency/1000);
    //printf("====eBandWidth = %bx\n", eBandWidth);
#if ((FRONTEND_DEMOD_TYPE != MSTAR_MSB123x_DEMOD) || (FRONTEND_TUNER_TYPE != TUNER_MXL201RF))
#if (!ENABLE_HUMMINGBIRD_CUSTOMER)
    devDigitalTuner_SetFreq((double)(dwFrequency/1000.0), eBandWidth);
#endif
#endif


#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_FSM_Restart();
    ZL10353_BW_Programming(eBandWidth);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

  #if (FRONTEND_TUNER_TYPE == MSTAR_MSR1200_TUNER)
    MSB1200_SetFreq((U32)dwFrequency);    //FRA_DBG_071120_Canopus
  #endif

    MsOS_DelayTask(10); //waiting for tuner write

    MSB1200_Config(eBandWidth);

    #if TS_PARALLEL_OUTPUT
        #if ENABLE_CI
        if(!msAPI_Tuner_IsParallelMode())
        {
            MSB1200_Serial_Control(ENABLE);
        }
        else
        #endif
        {
            MSB1200_Serial_Control(DISABLE);
        }
    #else
        #if ENABLE_CI
        if ( msAPI_Tuner_IsParallelMode())
        {
            MSB1200_Serial_Control(DISABLE);
        }
        else
        #endif
        {
            MSB1200_Serial_Control(ENABLE);
        }
    #endif

    //MsOS_DelayTask( 50 );

    MSB1200_Active(ENABLE);

    //MsOS_DelayTask( 5 );

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    MsOS_DelayTask(10); //waiting for tuner write

    //printf("msb1210 config\n");

    b_pal_bg = false;

    #if ENABLE_CI
    if ( msAPI_Tuner_IsParallelMode())
        MSB1210_Config(eBandWidth, false, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE);
    else
    #endif
    MSB1210_Config(eBandWidth, !TS_PARALLEL_OUTPUT, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE);

    //printf("msb1210 active\n");

    MSB1210_Active(ENABLE);

#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)

//    MsOS_DelayTask(100);
    MT5131_SetBW(eBandWidth);
//    MsOS_DelayTask(100);
/*
    if(MT5131_GetLockStatus()==0)
        MT5131_Reset();
*/
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD)
      //SWIIC_SetIICSpeed(150);  //2
               if(LGS_SetAutoMode()== LGS_NO_ERROR)
               {
                   if(LGS_AutoDetect() == LGS_NO_ERROR)
                   {
                        if(LGS_SetManualMode()== LGS_NO_ERROR)
                {
                    //printf("\n\n--------------------------Auto detect done, carrier mode ---------------------\n\n");
                }
                        else
                {
                    //printf("\n\n------------Set ManualMode error ----------------\n\n");
                }
                   }
                   else
                    {
               //printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
               //printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
      //SWIIC_SetIICSpeed(150);  //2
               if(LGS8G52_SetAutoMode()== LGS_NO_ERROR)
               {
                   if(LGS8G52_AutoDetect() == LGS_NO_ERROR)
                   {
                        if(LGS_SetManualMode()== LGS_NO_ERROR)
                {
                    //printf("\n\n--------------------------Auto detect done, carrier mode ---------------------\n\n");
                }
                        else
                {
                    //printf("\n\n------------Set ManualMode error ----------------\n\n");
                }
                   }
                   else
                    {
               //printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
               //printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
	//SWIIC_SetIICSpeed(150);  //2
       	if(LGS8G75_SetAutoMode()== LGS_NO_ERROR)
              {
                   if(LGS8G75_AutoDetect() == LGS_NO_ERROR)
                   {
                        if(LGS8G75_SetManualMode()== LGS_NO_ERROR)
                	   {
                    		printf("\n\n--------------------------Auto detect done, carrier mode ---------------------\n\n");
                	   }
                        else
                	   {
                    		printf("\n\n------------Set ManualMode error ----------------\n\n");
                        }
                    }
                    else
                    {
                       printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
                printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
      //SWIIC_SetIICSpeed(150);  //2
               if(LGS8G80_SetAutoMode()== LGS_NO_ERROR)
               {
                   if(LGS8G80_AutoDetect() == LGS_NO_ERROR)
                   {
                   }
                   else
                    {
               printf("\n\n----------------------------Auto detect error,carrier mode ---------------------\n\n");
                    }
               }
           else
           {
               //printf("\n\n----------Set AutoMode error-------------\n\n");
           }
     //SWIIC_SetIICSpeed(10);  //2


#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    //Demod reset
     TC90512XBG_OFDM_demod_reset();
     TC90512XBG_OFDM_fft_reset();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    //Demod reset
    TC90517_Reset();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    MsOS_DelayTask(5); //waiting for tuner write
    M88DC2800_Config();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    MsOS_DelayTask(5); //waiting for tuner write
    M88DD2000_Config();

#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)

    MsOS_DelayTask(10); //waiting for tuner write

    //printf("msb1210 config\n");

    b_pal_bg = false;

    #if ENABLE_CI
    if ( msAPI_Tuner_IsParallelMode())// || (0 == TS_SERIAL_OUTPUT_IF_CI_REMOVED))
        INTERN_DVBT_Config(eBandWidth, false, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE);
    else
    #endif
     INTERN_DVBT_Config(eBandWidth, !TS_PARALLEL_OUTPUT, b_pal_bg,E_RF_CH_LP==eHpLp ? TRUE : FALSE); // << ken 2009.06.29 serial
     //INTERN_DVBT_Config(eBandWidth, false, b_pal_bg);
    //printf("msb1210 active\n");

    INTERN_DVBT_Active(ENABLE);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
    DTV_SetFrequency(dwFrequency,eBandWidth);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
    DTV_SetFrequency(dwFrequency,eBandWidth);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    DEMOD_MS_FE_CARRIER_PARAM dmdParam;
    dmdParam.u32Frequency = dwFrequency;
    dmdParam.u8BandWidth = eBandWidth + 6;//u16BandWidth / 1000;
    dmdParam.u8PlpID = 0xff;
    if (MDrv_Demod_Restart(&dmdParam) == FALSE)
    {
        printf("%s: COfdm DMD restart failed.123x\n", __FUNCTION__);
    }
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
    DTV_SetFrequency((double)(dwFrequency/1000.0), eBandWidth);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    DTV_SetFrequency((double)(dwFrequency/1000.0), eBandWidth);
#elif (FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
    MDrv_Demod_Reset();
#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    DTV_SetFrequency(dwFrequency, eBandWidth);

#elif (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x || FRONTEND_DEMOD_TYPE == DEMOD_AVL6211 || \
       FRONTEND_DEMOD_TYPE == DEMOD_SamSung_S5H1411 || FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || \
       FRONTEND_DEMOD_TYPE == MSTAR_MSB131X_DEMOD         ||  FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD|| FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    MApi_FE_SetFrequency(dwFrequency, eBandWidth);
#endif
#endif
}

BOOLEAN devCOFDM_GetLockStatus(COFDM_LOCK_STATUS eStatus)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    return ZL10353_Lock_Indicate(eStatus);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    return MSB1200_Lock(eStatus);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    return MSB1210_Lock(eStatus);

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    U8 u8Result;
    eStatus=eStatus;
    //SWIIC_SetIICSpeed(150);
    u8Result = LGS_CheckLocked();
    //SWIIC_SetIICSpeed(10);
    if(u8Result == 1)
        return TRUE;
    else
        return FALSE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    U8 u8Result;
    U8 u8Temp;
    eStatus=eStatus;
    //SWIIC_SetIICSpeed(150);
    u8Result = LGS8G75_CheckLocked(&u8Temp);
    //SWIIC_SetIICSpeed(10);
    if(u8Temp == 1)
        return TRUE;
    else
        return FALSE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    eStatus=eStatus;
    if(MT5131_GetLockStatus()==FALSE)
    {
        MT5131_Reset();
        return FALSE;
    }else
        return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_Lock_Indicate(eStatus);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return(TC90512XBG_Lock(eStatus));
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    eStatus=eStatus;
    return(TC90517_Lock());
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    eStatus=eStatus;
    return(M88DC2800_GetLock());
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    eStatus=eStatus;
    return(M88DD2000_GetLock());
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    eStatus=eStatus;
    return ATBMLockedFlag();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
    return DTV_Lock();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
    return DTV_Lock();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_Lock(eStatus);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    MS_BOOL bLock;
    MDrv_Demod_GetLock(&bLock);
    return bLock;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
    return DTV_DVB_T_GetLockStatus();
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    return DTV_DVB_T_GetLockStatus();
#elif (FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
    return MDrv_Demod_GetLockStatus();
#elif (FRONTEND_DEMOD_TYPE == DIBCOM_DIB8096_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD)
    return DTV_GetLockStatus();

#elif (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x || FRONTEND_DEMOD_TYPE == DEMOD_AVL6211 || \
       FRONTEND_DEMOD_TYPE == DEMOD_SamSung_S5H1411 || FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || \
       FRONTEND_DEMOD_TYPE == MSTAR_MSB131X_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD|| FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
#if (ENABLE_HUMMINGBIRD_CUSTOMER)

    return MApi_FE_CheckLock();

#endif
#else
    eStatus=eStatus;
    return TRUE;
#endif
}

BYTE devCOFDM_GetSignalToNoiseRatio(void)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    return (BYTE)ZL10353_GetSNR();

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    return (BYTE)MSB1200_GetSNR();

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    return (BYTE)MSB1210_GetSNR();

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return (BYTE)CE6355_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return (BYTE)TC90512XBG_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    return (BYTE)TC90517_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    return (BYTE)M88DC2800_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    return 1;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    return (BYTE)ATBMSignalNoiseRatio();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return (BYTE)INTERN_DVBT_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
    return (BYTE)DTV_GetSNR();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
    return (BYTE)DTV_GetSNR();

#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    BYTE quality;
    MS_U32 u32SNR;
    if (MDrv_Demod_GetSNR(&u32SNR) != TRUE)
    {
        u32SNR = 0;
    }
   quality = (BYTE)(u32SNR);
   return quality;

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)|| (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    return (BYTE)DTV_GetSNR();
	
#elif (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x || FRONTEND_DEMOD_TYPE == DEMOD_AVL6211 || \
       FRONTEND_DEMOD_TYPE == DEMOD_SamSung_S5H1411 || FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || \
       FRONTEND_DEMOD_TYPE == MSTAR_MSB131X_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD|| FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
  #if (ENABLE_HUMMINGBIRD_CUSTOMER)

  return (BYTE)MApi_FE_GetSNR();

  #endif
#else
    return 1;
#endif
}

#if (FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
BYTE devCOFDM_GetSignalMode(void)
{
    return LGS8G52_ReadMode();
}
BYTE devCOFDM_GetSignalPN(void)
{
        return LGS8G52_ReadPN();
}
BYTE devCOFDM_GetSignalPNFLIP(void)
{
    return LGS8G52_ReadPNFLIP();
}
BYTE devCOFDM_GetSignalCarrier(void)
{
    return LGS8G52_ReadCarrier();
}
BYTE devCOFDM_SetSignalMode(BYTE Value)
{
    return LGS8G52_SetMode(Value);
}
BYTE devCOFDM_SetSignalPN(BYTE Value)
{
    return LGS8G52_SetPN(Value);
}
BYTE devCOFDM_SetSignalPNFLIP(BYTE Value)
{
    return LGS8G52_SetPNFLIP(Value);
}
BYTE devCOFDM_SetSignalCarrier(BYTE Value)
{
    return LGS_SetCarrierMode(Value);
}
#endif

void devCOFDM_PassThroughI2C(BOOLEAN bEnable)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_RepeatControl(bEnable);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    bEnable++; //dummy code
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    bEnable=bEnable;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    if(bEnable)
        ATBM_I2CByPassOn();
    else
        ATBM_I2CByPassOff();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    bEnable=bEnable;
#else
    bEnable=bEnable;
#endif
}

#if (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface, RF_CHANNEL_BANDWIDTH BandWidth)
{
    BOOLEAN bEnable;

    if ( interface == E_SERIAL_INTERFACE )
    {
        bEnable = TRUE;
    }
    else
    {
    #if 0
        #if (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD)
        XBYTE[0x1558] = (XBYTE[0x1558]&0xfb) | 0x02;
        #else
        XBYTE[0x1558] = XBYTE[0x1558] | 0x02;
        #endif
     #endif
        bEnable = FALSE;
    }

    CE6355_SwitchTransportSteamInferface(interface, BandWidth);
}
#else
void devCOFDM_SwitchTransportSteamInferface(TS_INTERFACE interface)
{
    BOOLEAN bEnable;

    if ( interface == E_SERIAL_INTERFACE )
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }

#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    ZL10353_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    MSB1200_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    MSB1210_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD)
    LGS_SetCarrierMode(interface);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    LGS8G75_SetMpegMode(1,1,0);//bEnable);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    LGS8G80_SetMpegMode(1,1,0);
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
//    MT5131_Serial_Control(bEnable);
    if(interface==E_SERIAL_INTERFACE)
    {
//        printf("Serial\n");
        MT5131_SetTsIf(0x0A);
    }else if(interface==E_PARALLEL_INTERFACE)
    {
//        printf("Parallel\n");
        MT5131_SetTsIf(0x0E);
    }
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)

#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD ||\
       FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    interface=interface;
     //set Demod to clk invert to make sure the TSP clk invert is correct
     //XBYTE[0x1E26] |= 0x02;
     {
        U8 u8Data;

        u8Data = MDrv_ReadByte(0x1E26);
        u8Data|=0x02;
        MDrv_WriteByte(0x1E26,u8Data);
      }
    //printf("---> Do not implement devCOFDM_SwitchTransportSteamInferface???\n");
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    M88DC2800_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    M88DD2000_Serial_Control(bEnable);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    INTERN_DVBT_Serial_Control(bEnable);
    INTERN_DVBT_PAD_TS1_Enable(~bEnable);

#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    {
        struct MPEG_TS_mode_t ts_mode;

        ts_mode.output_edge = TS_OUTPUT_FALLING_EDGE;
        ts_mode.SPI_clock_constant_output = TS_CLOCK_CONST_OUTPUT;
        if(bEnable)
        {
            //printf("set to serial mode\n");
            ts_mode.TS_Transfer_Type = TS_SERIAL_MODE;
        }
        else
        {
            //printf("set to parallel mode\n");
            ts_mode.TS_Transfer_Type = TS_PARALLEL_MODE;
            MDrv_WriteByte(0x101558,0x06);  //add patch here to change TS mode to parallel
        }
        ATBMSetTSMode(ts_mode);
    }
#endif

}
#endif

FUNCTION_RESULT devCOFDM_GetSignal_Vit( DWORD *vit )
{

#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetSignal_Vit(vit);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    float ber;
    *vit = *vit;
    return MSB1200_GetPostViterbiBer(&ber);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    float ber;
    *vit = *vit;
    if (MSB1210_GetPostViterbiBer(&ber)==TRUE)
       return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_GetSignal_Vit(vit);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return TC90512XBG_GetSignal_BER(vit);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *vit = *vit;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    float ber;
    *vit = *vit;
    if (INTERN_DVBT_GetPostViterbiBer(&ber)==TRUE)
       return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    *vit = *vit;
    return E_RESULT_FAILURE;
#endif
}

FUNCTION_RESULT devCOFDM_GetSignalStrength(WORD *strength)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD)
	*strength=LGS8G52_ReadIntension();
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
     *strength=*strength;
    return E_RESULT_SUCCESS;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
     *strength=LGS8G80_ReadIntension();
    return E_RESULT_SUCCESS;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
     *strength=*strength;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return TC90512XBG_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    return TC90517_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    return M88DC2800_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
     *strength=*strength;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    *strength=*strength;
    return (FUNCTION_RESULT)ATBMSignalStrength();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_GetSignalStrength(strength);
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    *strength = MSB123X_DTV_GetSignalStrength();
    return E_RESULT_SUCCESS;

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)|| (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    *strength = DTV_GetSignalStrength();
    return E_RESULT_SUCCESS;
#else
    *strength=*strength;
    return E_RESULT_FAILURE;
#endif

}

BOOLEAN devCOFDM_Get_CELL_ID(WORD *wCell_id)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_Get_CELL_ID(wCell_id);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_Get_CELL_ID(wCell_id);
#else
    *wCell_id = *wCell_id;
    return FALSE;
#endif

}

FUNCTION_RESULT devCOFDM_GetSignalQuality(WORD *quality)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    return CE6355_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    return  TC90512XBG_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    return  TC90517XBG_GetSignalQuality(quality);

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *quality = *quality;
    return E_RESULT_FAILURE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    *quality = *quality;
    return (FUNCTION_RESULT)ATBMSignalQuality();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_GetSignalQuality(quality);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
    return DTV_GetSignalQuality();
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD)
    return DTV_GetSignalQuality();
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    MS_U32 u32SNR;
    if (MDrv_Demod_GetSNR(&u32SNR) != TRUE)
    {
        u32SNR = 0;
    }
    *quality = (WORD)(u32SNR);
    return E_RESULT_SUCCESS;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)|| (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    *quality = (WORD)DTV_GetSignalQuality();
    return E_RESULT_SUCCESS;
	
	
#elif (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x || FRONTEND_DEMOD_TYPE == DEMOD_AVL6211 || \
       FRONTEND_DEMOD_TYPE == DEMOD_SamSung_S5H1411 || FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || \
       FRONTEND_DEMOD_TYPE == MSTAR_MSB131X_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1400_DEMOD|| FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
	#if (ENABLE_HUMMINGBIRD_CUSTOMER)
	*quality = MApi_FE_GetSignalQuality();
	return E_RESULT_SUCCESS;    
    #endif
#else
    *quality = *quality;
    return E_RESULT_FAILURE;
#endif
}

BOOLEAN devCOFDM_GetBER(float *ber)
{
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    return MSB1200_GetPostViterbiBer(ber);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_GetPostViterbiBer(ber);
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_GetPostViterbiBer(ber);
#else
    *ber=*ber;
    return FALSE;
#endif
}

BOOLEAN devCOFDM_Get_Modul_Mode(WORD *Modul_Mode)
{
#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
    return ZL10353_Get_TSP_parameter_Mode(Modul_Mode);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;//MSB1200_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_Get_TPS_Parameter_Const(Modul_Mode);
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;//CE6355_GetSignalStrength(strength);
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    *Modul_Mode=*Modul_Mode;
    return 0;
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_Get_TPS_Parameter_Const(Modul_Mode);
#else
    *Modul_Mode=*Modul_Mode;
    return 0;
#endif

}


BOOLEAN devCOFDM_Is_Hierarchy_On(void)
{
#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    return INTERN_DVBT_Is_HierarchyOn();
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)
    return MSB1210_Is_HierarchyOn();
#else
    return FALSE;
#endif

}


BOOLEAN devCOFDM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
	U16 U16SlaveID=u8SlaveID;
	#ifdef USE_SEC_IIC
	U16SlaveID=(U16)((E_I2C_BUS_SYS2<<8)|u8SlaveID);
	#endif

#if(FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)

    return MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) ;

#else // (FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)

#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)

    ZL10353_RepeatControl(ENABLE);
#if (CHIP_FAMILY_TYPE==CHIP_FAMILY_S8 | CHIP_FAMILY_TYPE==CHIP_FAMILY_S7L)
    MDrv_IIC_ReadBytes(U16SlaveID|0x200, u8AddrNum, paddr, u16size, pu8data);
#else
    MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
#endif
    ZL10353_RepeatControl(DISABLE);
    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

    return MSB1200_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);

#elif  (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    MDrv_1210_IIC_Bypass_Mode(TRUE);        // IIC by-pass mode on
    MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    MDrv_1210_IIC_Bypass_Mode(FALSE);        // IIC by-pass mode off
    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
    //SWIIC_SetIICSpeed(150);  //2
//    LGS_OpenTunerI2C(u8SlaveID)  ;
    MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
//    LGS_CloseTunerI2C() ;
    //SWIIC_SetIICSpeed(10);  //2
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
    LGS8G75_OpenTunerI2C(u8SlaveID)  ;
    MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    LGS8G75_CloseTunerI2C() ;
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    MDrv_LDELCK_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD ||\
       FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
   {
        U8 u8data[10];

        u8data[0] = 0xFE;//through start address
        u8data[1] = (U16SlaveID|0x01);//Tuner Add
        u8AddrNum = u8AddrNum; //Remove compiler waring
        *paddr = *paddr;//Remove compiler waring

    MDrv_IIC_ReadBytes(COFDM_DEM_I2C_ID, 2, u8data, u16size, pu8data);
//    printf("--->Demod Read data 0x3c, 0x%bx, 0x%bx (Size=0x%bx Read=0x%bx)\n"
//        ,u8data[0],u8data[1],u16size,*pu8data);
    return TRUE;
    }
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    M88DC2800_RepeatControl(ENABLE);
    MDrv_IIC_ReadBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    M88DC2800_RepeatControl(DISABLE);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    M88DD2000_ReadBytesRepeat(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    ATBM_I2CByPassOn();
    MDrv_IIC_ReadBytes(U16SlaveID|0x0200,u8AddrNum,paddr,u16size,pu8data);
    ATBM_I2CByPassOff();
    return TRUE;
#elif  (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    UNUSED(U16SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;

#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    UNUSED(U16SlaveID);
    MSB123X_IIC_Bypass_Mode(1);

    MDrv_IIC_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data);

    MSB123X_IIC_Bypass_Mode(0);

    return TRUE;



#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
    UNUSED(U16SlaveID);
    IIC_Bypass_Mode(1);

    MDrv_IIC_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data);

    IIC_Bypass_Mode(0);

    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    UNUSED(U16SlaveID);

    IIC_Bypass_Mode(1);

    MDrv_IIC_SelectCLK(E_HWI2C_HIGH);


    MDrv_IIC_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data);


    MDrv_IIC_SelectCLK(E_HWI2C_SLOW);

    IIC_Bypass_Mode(0);

    return TRUE;

#else
    UNUSED(U16SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
#endif
#endif  //(FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)
}

BOOLEAN devCOFDM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
	U16 U16SlaveID=u8SlaveID;
	#ifdef USE_SEC_IIC
	U16SlaveID=(U16)((E_I2C_BUS_SYS2<<8)|u8SlaveID);
	#endif
#if (OTHER_TUNER_DEBUG==1)
    int i;
    printf("===> COFDM(ID=0x%bx)(Size=0x%x) ",U16SlaveID,u16size);
    printf("  Data =  ");
    for (i=0;i<u16size;i++)
        printf("0x%bx  ",*(pu8data+i));
    printf("\n");
#endif

#if(FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H||FRONTEND_TUNER_TYPE == XUGUANG_STM_6F_V236H_TUNER \
    ||FRONTEND_TUNER_TYPE==XUGUANG_HFT_8B1_V116H_TUNER || FRONTEND_TUNER_TYPE==XUGUANG_HFT_8B_115CW_TUNER \
    || FRONTEND_TUNER_TYPE == LINGHUA_TDP_1H1_E_TUNER)
    U16SlaveID |= (U16)((TUNER_IIC_BUS<<8)|u8SlaveID);
    return MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) ;

#else // (FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)


#if (FRONTEND_DEMOD_TYPE == ZARLINK_ZL10353_DEMODE)
#ifdef BD_MST072A_D01A_WITH_ONBOARD_TUNER_ATV
    printf("---> ATV Only, I2c skip DTV-Demod ...\n");
#else
    ZL10353_RepeatControl(ENABLE);
#endif
#if ((MS_BOARD_TYPE_SEL == BD_MST087A_D01A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087B_S7M_D01A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087B_S7_D01A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087C_D02A_S) || \
            (MS_BOARD_TYPE_SEL == BD_MST087D_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST105A_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST105B_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST106A_D01A_S)|| \
            (MS_BOARD_TYPE_SEL == BD_MST106B_D01A_S))

if ( FALSE == MDrv_IIC_WriteBytes((U32)(U16SlaveID|0x0200), u8AddrNum, paddr, u16size, pu8data) )
{
    printf("---> devCOFDM_PassThroughI2C_WriteBytes Error-2 \n");
    return FALSE;
}


#else
    #if (CHIP_FAMILY_TYPE==CHIP_FAMILY_S7J) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARIA10)
    U16SlaveID=(U16)((DEMOD_IIC_BUS<<8)|U16SlaveID);
    #endif
    if ( FALSE == MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }
#endif
    MsOS_DelayTask(5); //waiting for tuner write
#ifdef BD_MST072A_D01A_WITH_ONBOARD_TUNER_ATV
    printf("---> ATV Only, I2c skip DTV-Demod ...\n");
#else
    ZL10353_RepeatControl(DISABLE);
#endif

    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)
        return MSB1200_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

    MDrv_1210_IIC_Bypass_Mode(TRUE);        // IIC by-pass mode on

    if (MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) == FALSE)
    {
        printf("read write fail---%x\n",U16SlaveID);
        return FALSE;
    }

    MDrv_1210_IIC_Bypass_Mode(FALSE);        // IIC by-pass mode off
        return TRUE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G13_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G42_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G52_DEMOD||FRONTEND_DEMOD_TYPE == LEGEND_8G80_DEMOD)
  #if (MS_BOARD_TYPE_SEL == BD_MST087F_D01A_T)
	U16SlaveID=(U16)((E_I2C_BUS_DDCD0<<8)|u8SlaveID);

    if (MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) == FALSE)
        printf("devCOFDM_PassThroughI2C_WriteBytes error-- %x-\n",U16SlaveID);
  #else
    //SWIIC_SetIICSpeed( 150 );
    LGS_OpenTunerI2C(u8SlaveID)  ;
   	if(MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data)==FALSE)
		printf("devCOFDM_PassThroughI2C_WriteBytes error------error---error-- %x-\n",U16SlaveID);
    LGS_CloseTunerI2C() ;
    //SWIIC_SetIICSpeed( 10 );
  #endif
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == LEGEND_8G75_DEMOD)
	U16SlaveID=(U16)((E_I2C_BUS_DDCA0<<8)|u8SlaveID);

    if (MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) == FALSE)
    {
        printf("\r\n 20091027--write fail\n");
    }
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MTK_5131_DEMOD)
    MDrv_LDELCK_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ZARLINK_CE6355_DEMOD)
    if ( FALSE == MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }

    msAPI_Timer_Delayms(1); // delay 1ms

    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == TOSHIBA_TC90512XBG_DEMOD ||\
       FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
    {
        U8 u8data[10],i;

        u8data[0] = 0xFE;//through start address
        u8data[1] = U16SlaveID;//through start address
        if (u16size > 9)
        {
            printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
            return FALSE;
        }

        for (i=0;i<u16size;i++)
            u8data[2+i] = *(pu8data+i);

#if 0
        printf("--->Demod write data 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx, 0x%bx \n",
                  u8data[0],u8data[1],u8data[2],u8data[3],u8data[4],u8data[5],u8data[6],u8data[7]);
#endif
        MDrv_IIC_WriteBytes(COFDM_DEM_I2C_ID, 0, paddr, (u16size+2), u8data) ;
        u8AddrNum = u8AddrNum;//Remove Compiler Warning
            return TRUE;
    }

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DC2800_DEMOD)
    M88DC2800_RepeatControl(ENABLE);
    if ( FALSE == MDrv_IIC_WriteBytes(U16SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }
    M88DC2800_RepeatControl(DISABLE);

    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == MONTAGE_M88DD2000_DEMOD)
    M88DD2000_WriteBytesRepeat(U16SlaveID, u8AddrNum, paddr, u16size, pu8data);
    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == ALTOBEAM_883X)
    ATBM_I2CByPassOn();
    //printf("bypass write addr 0x%02x\n",U16SlaveID);
    MDrv_IIC_WriteBytes(U16SlaveID|0x0200,u8AddrNum,paddr,u16size,pu8data);
    ATBM_I2CByPassOff();
    return TRUE;

#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
    UNUSED(U16SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
#elif ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
    UNUSED(U16SlaveID);
    MSB123X_IIC_Bypass_Mode(1);
    if ( FALSE == MDrv_IIC_WriteBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("%04d %s devCOFDM_PassThroughI2C_WriteBytes Error \n", __LINE__, __FUNCTION__);
        MSB123X_IIC_Bypass_Mode(0);
        return FALSE;
    }

    MSB123X_IIC_Bypass_Mode(0);
    msAPI_Timer_Delayms(1); // delay 1ms

    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD || FRONTEND_DEMOD_TYPE == LGDT3305_DEMOD)
    UNUSED(U16SlaveID);
    IIC_Bypass_Mode(1);

    if ( FALSE == MDrv_IIC_WriteBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("%04d %s devCOFDM_PassThroughI2C_WriteBytes Error \n", __LINE__, __FUNCTION__);
        IIC_Bypass_Mode(0);
        return FALSE;
    }

    IIC_Bypass_Mode(0);
    msAPI_Timer_Delayms(1); // delay 1ms

    return TRUE;
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB122x_DEMOD)
    UNUSED(U16SlaveID);

    IIC_Bypass_Mode(1);
    msAPI_Timer_Delayms(5); // delay 5ms

    if ( FALSE == MDrv_IIC_WriteBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("%04d %s devCOFDM_PassThroughI2C_WriteBytes Error \n", __LINE__, __FUNCTION__);
        IIC_Bypass_Mode(0);
        msAPI_Timer_Delayms(5); // delay 5ms
        return FALSE;
    }

    msAPI_Timer_Delayms(5); // delay 5ms
    IIC_Bypass_Mode(0);
    msAPI_Timer_Delayms(5); // delay 5ms

    return TRUE;
#else
    UNUSED(U16SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
#endif

#endif  //(FRONTEND_TUNER_TYPE == XUGUANG_TDQ_6FT_W116H)

}

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB1200_DEMOD)

U16 devCOFDM_ReadReg(U16 RegAddr)
{
    U8 RegData;
    if((RegAddr >= 0xC000) && (RegAddr <= 0xCEFF))
        MSB1200_ReadRaptor(RegAddr, &RegData);
    else
        MSB1200_ReadReg(RegAddr, &RegData);
    return (U16) RegData;
}

BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData)
{
    if((RegAddr >= 0xC000) && (RegAddr <= 0xCEFF))
        return MSB1200_WriteRaptor( RegAddr, (U8)RegData);
    else
        return MSB1200_WriteReg( RegAddr, (U8)RegData);
}
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1210_DEMOD)

U16 devCOFDM_ReadReg(U16 RegAddr)
{
    U8 RegData;
    MSB1210_ReadReg(RegAddr, &RegData);
    return (U16) RegData;
}

BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData)
{
    return MSB1210_WriteReg( RegAddr, (U8)RegData);
}
#elif (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD || FRONTEND_DEMOD_TYPE == TOSHIBA_TC90517FG_DEMOD)
U16 devCOFDM_ReadReg(U16 RegAddr)
{
    U8 RegData;
    INTERN_DVBT_ReadReg(RegAddr, &RegData);
    return (U16) RegData;
}

BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData)
{
    return INTERN_DVBT_WriteReg( RegAddr,(U8) RegData);
}
#endif

#if ((FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD) && (FRONTEND_TUNER_TYPE == TUNER_MXL201RF))
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    //Disconnect();
    MDrv_CofdmDmd_SetCurrentDemodType(enDemodType);
    return TRUE;
    //return Connect(enDemodType);
}
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void)
{
    return MDrv_CofdmDmd_GetCurrentDemodType();
}
#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{printf("\n%s\n",__func__);
	return TRUE;
}
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void)
{
	extern EN_DEVICE_DEMOD_TYPE MDrv_Demod_GetCurrentDemodType(void);
    return MDrv_Demod_GetCurrentDemodType();
}

#elif (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
BOOLEAN devCOFDM_SetDemodType(EN_DEVICE_DEMOD_TYPE enDemodType)
{
    Disconnect();
    SetCurrentDemodulatorType(enDemodType);
    return Connect(enDemodType);
}
EN_DEVICE_DEMOD_TYPE devCOFDM_GetDemodType(void)
{
    return DTV_Get_Demod_TYPE();
}
#endif

#endif

