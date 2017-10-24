#include "MsCommon.h"
#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL203)

#include "MxL203RF_Common.h"
#include "MxL203RF.h"
#include "MxL203RF_API.h"
#include "MxL203RF_User_Define.h"

#include "drvTuner.h"
#include "drvTunerNull.h"
#include "apiDigiTuner.h"

//extern DEV_FRONTEND_TYPE frontendtab[MAX_FRONTEND_NUM]; // <= not good
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];

#define EXAMPLE_DEV_MAX   2
#define MXL203_I2C_ADDR   0xC0

#define MXL203_CHIP_ID 0x01

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Tuner_Mxl203_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MxL203RF_TunerConfigS myTuner;

    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;
    //Set Tuner's I2C Address
    myTuner.I2C_Addr = MXL203_I2C_ADDR;

    //Set Tuner Mode to Cable mode
    myTuner.Mode = MxL_MODE_CAB_STD;
    
    //Set Tuner's XTAL freq and capacitance
    myTuner.Xtal_Freq = MxL_XTAL_48_MHZ;
    myTuner.Xtal_Cap = MxL_XTAL_CAP_12_PF;
    
    //Set Tuner's IF Freq
    myTuner.IF_Freq = MxL_IF_5_MHZ;
    myTuner.IF_Spectrum = MxL_NORMAL_IF;
    
    //Set Tuner's Clock out setting
    myTuner.ClkOut_Setting = MxL_CLKOUT_ENABLE;
    myTuner.ClkOut_Amp = MxL_CLKOUT_AMP_10;
    
    //Init Tuner
    if(MxL_OK != MxL_Tuner_Init(&myTuner))
    {
        printf("[%s][%d]init tuner fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return TRUE;
}

/*
MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{
    return 0;
}

MS_U32 MDrv_Tuner_Stand_By(void)
{
    return 0;
}

MS_U32 MDrv_Tuner_Wake_Up(void)
{
    return 0;
}
*/

MS_BOOL MDrv_Tuner_MXL203_Tune(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    BOOL RFSynthLock, REFSynthLock;
    MxL203RF_BW_MHz mxl203BW = MxL_BW_8MHz;
    MxL203RF_TunerConfigS myTuner;
    
    //Set Tuner's I2C Address
    myTuner.I2C_Addr = MXL203_I2C_ADDR;

    //Set Tuner Mode to Cable mode
    myTuner.Mode = MxL_MODE_CAB_STD;
    
    //Set Tuner's XTAL freq and capacitance
    myTuner.Xtal_Freq = MxL_XTAL_48_MHZ;
    myTuner.Xtal_Cap = MxL_XTAL_CAP_12_PF;
    
    //Set Tuner's IF Freq
    myTuner.IF_Freq = MxL_IF_5_MHZ;
    myTuner.IF_Spectrum = MxL_NORMAL_IF;
    
    //Set Tuner's Clock out setting
    myTuner.ClkOut_Setting = MxL_CLKOUT_ENABLE;
    myTuner.ClkOut_Amp = MxL_CLKOUT_AMP_10;    

    //BandWidth Mapping
    switch (ucBw)
    {
        case TUNER_BW_MODE_6MHZ:
            mxl203BW = MxL_BW_8MHz;
            break;
        case TUNER_BW_MODE_8MHZ:
            mxl203BW = MxL_BW_8MHz;
            break;
        default:
           mxl203BW = MxL_BW_8MHz; 
           printf("[warning] Bandwidth enum:%x not support using defalut 8Mhz\n",mxl203BW);
           break;
    }
    
    //Tune Tuner
    if(MxL_OK != MxL_Tuner_RFTune(&myTuner, dwFreq*1000, mxl203BW))
    {
        printf("[%s][%d]tune frequncy fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    
    //Check Lock Status and RF Input Level
    if(MxL_OK != MxL_RFSynth_Lock_Status(&myTuner, &RFSynthLock))
    {
        printf("[%s][%d]tune frequncy fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    
    if(MxL_OK != MxL_REFSynth_Lock_Status(&myTuner, &REFSynthLock))
    {
        printf("[%s][%d]tune frequncy fail!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if(TRUE != RFSynthLock || TRUE != REFSynthLock)
    {
        printf("[%s][%d]Fail:%u,%u!\n",__FUNCTION__,__LINE__,RFSynthLock,REFSynthLock);
        return FALSE;
    }
    
    return TRUE;
}

MS_BOOL MDrv_Tuner_MXL203_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MxL203RF_TunerConfigS myTuner;
    MxL203RF_ChipVersion chipVersion;
    
    //Set Tuner's I2C Address
    myTuner.I2C_Addr = MXL203_I2C_ADDR;
    if(MxL_OK != MxL_Check_ChipVersion(&myTuner,&chipVersion))
    {
        printf("[mxl203] Read chip ID fail \n");
    }

    printf("[mxl203] read id =0x%x \n",chipVersion);

    if(MxL_203RF_ES5 == chipVersion)
    {
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
        
        return TRUE;
    }
    else
        return FALSE;
}

MS_BOOL MDrv_Tuner_MXL203_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MxL203RF_TunerConfigS myTuner;
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
                myTuner.I2C_Addr = MXL203_I2C_ADDR;
                if(MxL_OK != MxL_Check_RF_Input_Power(&myTuner, (float*)data))
                    return FALSE;
                break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            return TRUE;
    }
    return TRUE;
}

 
MS_BOOL MDrv_Tuner_MXL203_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* pu32IF_Freq)
{
   MS_BOOL bret = TRUE;
   MS_U8 u8data = 0;
   if(0 == MxL_I2C_Read(MXL203_I2C_ADDR,0x2, &u8data))
   {
       switch(u8data & 0xf)
       {
         case 0x3:
            *pu32IF_Freq = MxL_IF_4_57_MHZ/1000;
            break;
         case 0x4:
            *pu32IF_Freq = MxL_IF_5_MHZ/1000;
            break;
         case 0x6:
            *pu32IF_Freq = MxL_IF_6_MHZ/1000;
            break;
         case 0x8:
            *pu32IF_Freq = MxL_IF_7_2_MHZ/1000;
            break;
         default:
            bret = FALSE;
            break;
       }
   }
   else
   {
       bret = FALSE;
   }
   return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL203) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL203",          // demod system name
    .data               = TUNER_MXL203,            // private data value
    .Init               = MDrv_Tuner_Mxl203_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_MXL203_Tune,
    .CheckExist         = MDrv_Tuner_MXL203_CheckExist,
    .GetTunerIF         = MDrv_Tuner_MXL203_GetTunerIF, 
    .Extension_Function = MDrv_Tuner_MXL203_Extension_Function
};
#endif


