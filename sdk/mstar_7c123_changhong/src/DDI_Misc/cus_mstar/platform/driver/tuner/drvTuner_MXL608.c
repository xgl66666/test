
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "error.h"
#include "MsCommon.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "drvIIC.h"


#if IF_THIS_TUNER_INUSE(TUNER_MXL608)
#include "MxL608_TunerApi.h"
#include "MxL608_OEM_Drv.h"
#include "MxL608_TunerCfg.h"


#define EXAMPLE_DEV_MAX   2
//#define MXL608_I2C_ADDR   0xC0
#define MXL608_CHIP_ID 0x02
MS_BOOL MXL608_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt);
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM]; //MAX_FRONTEND_NUM
static MS_BOOL bUnderExtDMDTest = FALSE;
SLAVE_ID_USAGE* pstMxL608_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE MxL608_possible_slave_ID[5] =
{
    {0xC0, FALSE},
    {0xC2, FALSE},
    {0xC4, FALSE},
    {0xC6, FALSE},
    {0xFF, FALSE}
};
//-------------------------------------------------------------------------------------------------
//  Macros
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX_KERNEL)
#define free kfree
#define malloc(size) kmalloc((size), GFP_KERNEL)
#define V_FREE(p)                                       if(p){ kfree((void*)p); p = NULL;}
#define V_MALLOC(size) kmalloc((size), GFP_KERNEL)
#else
#define V_MALLOC(z)                                     malloc(z) ///virtual memory allocate
#define V_FREE(p)                                       if(p){ free((void*)p); p = NULL;} ///virtual memory free
#endif


/*
 * Increase driving current for IFamp
 */
static MXL_STATUS MDrv_MXL608Tuner_IncCurrent(MS_U8 u8TunerIndex)
{
    UINT8 status = MXL_SUCCESS;

    status |= MxLWare608_OEM_WriteRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, 0x5B, 0x18);
    status |= MxLWare608_OEM_WriteRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, 0x5C, 0xF1);

    return (MXL_STATUS) status;
}

MS_BOOL MDrv_MXL608Tuner_PowerOnOff(MS_U8 u8TunerIndex,MS_BOOL bPowerOn)
{
    return TRUE;
}

int MxL608_init_main(MS_U8 u8TunerIndex)
{
    MXL_STATUS status;
    UINT8 devId;
    MXL_BOOL singleSupply_3_3V;
    MXL608_XTAL_SET_CFG_T xtalCfg;
    MXL608_IF_OUT_CFG_T ifOutCfg;
    MXL608_AGC_CFG_T agcCfg;
    MXL608_TUNER_MODE_CFG_T tunerModeCfg;


    /* If OEM data is not required, customer should treat devId as
     I2C slave Address */
    devId = InitParam[u8TunerIndex].u8SlaveID;

    //Step 1 : Soft Reset MxL603
    status = MxLWare608_API_CfgDevSoftReset(u8TunerIndex,devId);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgDevSoftReset\n"));
        return status;
    }

    //Step 2 : Overwrite Default
    singleSupply_3_3V = MXL_ENABLE;
    status = MxLWare608_API_CfgDevOverwriteDefaults(u8TunerIndex,devId, singleSupply_3_3V);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgDevOverwriteDefaults\n"));
        return status;
    }

    //Step 3 : XTAL Setting
    xtalCfg.xtalFreqSel = MXL608_XTAL_24MHz;
    xtalCfg.xtalCap = 10; //tuner suggest 10pF
    xtalCfg.clkOutEnable = MXL_DISABLE;
    xtalCfg.clkOutDiv = MXL_DISABLE;
    xtalCfg.clkOutExt = MXL_DISABLE;
    xtalCfg.singleSupply_3_3V = MXL_ENABLE;
    xtalCfg.XtalSharingMode = MXL_DISABLE;
    status = MxLWare608_API_CfgDevXtal(u8TunerIndex,devId, xtalCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgDevXtal\n"));
        return status;
    }

    //Step 4 : IF Out setting
    ifOutCfg.ifOutFreq = MXL608_IF_5MHz;
    ifOutCfg.manualIFOutFreqInKHz = 5000;
    ifOutCfg.ifInversion = MXL_DISABLE;//MXL_ENABLE;
    ifOutCfg.gainLevel = 11;
    ifOutCfg.manualFreqSet = MXL_DISABLE;
    status = MxLWare608_API_CfgTunerIFOutParam(u8TunerIndex,devId, ifOutCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgTunerIFOutParam\n"));
        return status;
    }

    //Step 5 : AGC Setting
    agcCfg.agcType = MXL608_AGC_EXTERNAL;
    agcCfg.setPoint = 66;
    agcCfg.agcPolarityInverstion = MXL_DISABLE;
    status = MxLWare608_API_CfgTunerAGC(u8TunerIndex,devId, agcCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgTunerAGC\n"));
        return status;
    }

    //Step 6 : Application Mode setting
    tunerModeCfg.ifOutFreqinKHz = 5000;
    tunerModeCfg.xtalFreqSel = MXL608_XTAL_24MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    if(*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBC)
        tunerModeCfg.signalMode = MXL608_DIG_DVB_C;
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBT2))
        tunerModeCfg.signalMode = MXL608_DIG_DVB_T_DTMB;
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type== ISDBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type== ATSC))
        tunerModeCfg.signalMode = MXL608_DIG_ISDBT_ATSC;
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type== J83B))
        tunerModeCfg.signalMode = MXL608_DIG_J83B;
    else
        return FALSE;

    status = MxLWare608_API_CfgTunerMode(u8TunerIndex,devId, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgTunerMode, %d\n", status));
        return status;
    }

    //Patch for MSB1237 demod
    if(InitParam[u8TunerIndex].pstDemodtab->data == DEMOD_MSB1237)
    {
      status = MxLWare608_OEM_WriteRegister(u8TunerIndex,devId, 0xCD, 0x64);
      if (status != MXL_SUCCESS)
      {
        TUNER_ERR(("Error! MxLWare608_OEM_WriteRegister, %d\n", status));
        return status;
      }

      status = MxLWare608_OEM_WriteRegister(u8TunerIndex,devId, 0xCE, 0x7C);
      if (status != MXL_SUCCESS)
      {
        TUNER_ERR(("Error! MxLWare608_OEM_WriteRegister, %d\n", status));
        return status;
      }
    }

    return status;
}

MS_BOOL MDrv_MXL608Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MXL608_PLL_STATE_E pllstatus;
    MXL_BOOL agcLock;

    // Double Confirm Tuner I2C is normal

    //if(MXL608_CheckExist(u8TunerIndex, NULL) != TRUE)
    //{
    //   TUNER_ERR(("[mxl608] Chip ID is incorrect\n"));
    //   return FALSE;
    //}
    // Check PLL lock status, if PLL have locked before tuner init, it might be in used

    MxLWare608_API_ReqDevPllState(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID,&pllstatus);
    MxLWare608_API_ReqTunerAGCLock(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID,&agcLock);

    if((MXL608_PLL_STATE_NORMAL == pllstatus) || (MXL_LOCKED == agcLock))
    {
        TUNER_ERR(("[mxl608] Tuner might be in use\n"));
        return FALSE;
    }


    if((pParam->pCur_Broadcast_type == NULL) || (pParam->pstDemodtab == NULL))
        return FALSE;
    else
    {
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;
        InitParam[u8TunerIndex].pstDemodtab = pParam->pstDemodtab;
    }

    if(MxL608_init_main(u8TunerIndex) == MXL_SUCCESS)
    {
        TUNER_DBG((" MXL608 INIT OK\n"));
        return TRUE;
    }
    else
    {
        TUNER_ERR((" MXL608 INIT FAILED\n"));
        return FALSE;
    }
}

MS_U32 MDrv_MXL608Tuner_Loop_Through_On(MS_BOOL bOn)
{
    return 0;
}

MS_U32 MDrv_MXL608Tuner_Stand_By(void)
{
    return 0;
}

MS_U32 MDrv_MXL608Tuner_Wake_Up(void)
{
    return 0;
}

MS_BOOL MDrv_MXL608Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    UINT8 devId = InitParam[u8TunerIndex].u8SlaveID;
    MXL_STATUS status = MXL_FALSE;
    MXL608_BW_E eBw = ucBw;//(ucBw - 6);
    MXL608_CHAN_TUNE_CFG_T chanTuneCfg;
    MXL_BOOL refLockPtr = MXL_UNLOCKED;
    MXL_BOOL rfLockPtr = MXL_UNLOCKED;
    UINT8 regData = 0;

    //Step 7 : Channel mode & bandwidth & frequency setting
    if(*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBC)
        chanTuneCfg.signalMode = MXL608_DIG_DVB_C;
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBT2))
        chanTuneCfg.signalMode = MXL608_DIG_DVB_T_DTMB;
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type== ISDBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type== ATSC))
        chanTuneCfg.signalMode = MXL608_DIG_ISDBT_ATSC;
    else if((*InitParam[u8TunerIndex].pCur_Broadcast_type== J83B))
        chanTuneCfg.signalMode = MXL608_DIG_J83B;
    else
        return FALSE;

    switch(chanTuneCfg.signalMode)
    {
        case MXL608_DIG_DVB_C:
        case MXL608_DIG_J83B:
        {
            switch(eBw)
            {
                case MXL608_CABLE_BW_6MHz:
                case MXL608_CABLE_BW_7MHz:
                case MXL608_CABLE_BW_8MHz:
                    chanTuneCfg.bandWidth = eBw;
                    break;
                default:
                    chanTuneCfg.bandWidth = MXL608_CABLE_BW_8MHz;
                    break;
            }
            break;
         }
         case MXL608_DIG_DVB_T_DTMB:
         {
             eBw = eBw + MXL608_TERR_BW_6MHz;
             switch(eBw)
             {
                 case MXL608_TERR_BW_6MHz:
                 case MXL608_TERR_BW_7MHz:
                 case MXL608_TERR_BW_8MHz:
                     chanTuneCfg.bandWidth = eBw;
                     break;
                 default:
                 chanTuneCfg.bandWidth = MXL608_TERR_BW_8MHz;
                 break;
             }
             break;
         }
         case MXL608_DIG_ISDBT_ATSC:
         {
                eBw = eBw + MXL608_TERR_BW_6MHz;
                switch(eBw)
                {
                    case MXL608_TERR_BW_6MHz:
                    case MXL608_TERR_BW_7MHz:
                    case MXL608_TERR_BW_8MHz:
                        chanTuneCfg.bandWidth = eBw;
                        break;
                    default:
                        chanTuneCfg.bandWidth = MXL608_TERR_BW_6MHz;
                        break;
                }
                break;
         }
         default:
             chanTuneCfg.bandWidth = MXL608_TERR_BW_8MHz;
             break;
     }


    /* Only T/T2 needs to increase pad cuurent*/
   if ((*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBT)|| (*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBT2) ||\
       (*InitParam[u8TunerIndex].pCur_Broadcast_type == INTERNAL_DVBT))
    {
        status = MDrv_MXL608Tuner_IncCurrent(u8TunerIndex);
       if(status != MXL_SUCCESS)
       {
            TUNER_ERR(("Error! MDrv_Tuner_IncCurrent\n"));
            return status;
       }
    }

    chanTuneCfg.freqInHz = dwFreq * 1000;
    chanTuneCfg.xtalFreqSel = MXL608_XTAL_24MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
    //chanTuneCfg.bandWidth = eBw;
    if(*InitParam[u8TunerIndex].pCur_Broadcast_type== DVBC)
        status |= MxLWare608_OEM_WriteRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, 0xC2, 0xA8);

    status = MxLWare608_API_CfgTunerChanTune(u8TunerIndex,devId, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxLWare608_API_CfgTunerChanTune\n"));
        return status;
    }

    // Wait 15 ms
    MxLWare608_OEM_Sleep(15);

    // Read back Tuner lock status
    status = MxLWare608_API_ReqTunerLockStatus(u8TunerIndex,devId, &rfLockPtr, &refLockPtr);
    if (status == MXL_TRUE)
    {
        if ((MXL_LOCKED == rfLockPtr) && (MXL_LOCKED == refLockPtr))
        {
            TUNER_DBG(("\nTuner locked\n"));

            MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, 0x5B, &regData);
            TUNER_DBG (("MxL608_0x5B : %x\n", regData));
            MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, 0x5C, &regData);
            TUNER_DBG (("MxL608_0x5C : %x\n", regData));
            MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, 0xC2, &regData);
            TUNER_DBG (("MxL608_0xC2 : %x\n", regData));
        }
        else
        {
            TUNER_DBG(("Tuner unlocked\n"));
        }
    }

    if(status == MXL_TRUE)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MXL608_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MXL608_VER_INFO_T mxlDevVerInfo;
    MS_U8 i,j, u8I2C_Port = 0, u8MaxI2CPort;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_IIC_PORT ePort;

    memset(&mxlDevVerInfo, 0x00, sizeof(MXL608_VER_INFO_T));
    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    ePort = getI2CPort(u8TunerIndex);
    if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
    {
        u8I2C_Port = (MS_U8)ePort/8;
    }
    else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
    {
       u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
    }

    if(pstMxL608_slave_ID_TBL == NULL)
    {
        pstMxL608_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MxL608_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMxL608_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MxL608_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMxL608_slave_ID_TBL + i*sizeof(MxL608_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MxL608_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                 }
            }
        }
    }

    i = 0;
    do
    {
        pSlaveIDTBL = pstMxL608_slave_ID_TBL + u8I2C_Port*sizeof(MxL608_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        if(pSlaveIDTBL->bInUse)
        {
            TUNER_DBG(("[mxl608]I2C Slave ID 0x%x Have Used on the same I2C Port\n", pSlaveIDTBL->u8SlaveID));
        }
        else if((pSlaveIDTBL->u8SlaveID) == 0xFF)
        {
            break;
        }
        else
        {
            if(MXL_SUCCESS != MxLWare608_API_ReqDevVersionInfo(u8TunerIndex, pSlaveIDTBL->u8SlaveID, &mxlDevVerInfo))
            {
                TUNER_ERR(("[mxl608] Read chip ID fail with slave ID 0x%x\n", pSlaveIDTBL->u8SlaveID));
            }
            else
            {
                break;
            }
        }
        i++;
    }while((pSlaveIDTBL->u8SlaveID) != 0xFF);

    TUNER_DBG(("[mxl608] read id =0x%x\n",mxlDevVerInfo.chipId));

    if(mxlDevVerInfo.chipId == MXL608_CHIP_ID)
    {
        InitParam[u8TunerIndex].u8SlaveID = pSlaveIDTBL->u8SlaveID;
        if(!bUnderExtDMDTest)
        {
            pSlaveIDTBL->bInUse = TRUE;
        }

        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
        return TRUE;
    }
    else
        return FALSE;
}

MS_BOOL TUNER_MXL608_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_S16 sPWRData;

    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            if( MXL_SUCCESS != MxLWare608_API_ReqTunerRxPower(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, &sPWRData))
                return FALSE;
            sPWRData/=100;
            *(int*)data = (int)sPWRData;
            break;

        case TUNER_EXT_FUNC_FINALIZE:
            break;
        case TUNER_EXT_FUNC_GET_SLAVE_ID:
             *(MS_U8*)data = InitParam[u8TunerIndex].u8SlaveID;
            break;
        case TUNER_EXT_FUNC_UNDER_EXT_DMD_TEST:
            bUnderExtDMDTest = *(MS_BOOL*)data;
            break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return TRUE;
}

MS_BOOL MXL608_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* u32IF_Freq)
{
    UINT16 ifFcw;
    UINT8 status = MXL_SUCCESS;
    UINT8 readData = 0;

  // Read back register for manual IF Out
  status |= MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, IF_FREQ_SEL_REG, &readData);
     // IF out manual setting : bit<5>
  if((readData & 0x20) == 0x20)
  {
      //Get low 8 bit
      status |= MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, IF_FCW_LOW_REG, &readData);
      ifFcw = (UINT16)readData;
      // Get high 4 bit
      status |= MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, IF_FCW_HIGH_REG, &readData);
      ifFcw |= ((UINT16)(readData) & 0x0f) << 8;
      *u32IF_Freq = (MS_U32)((ifFcw * 216000)/8192);
  }
  else
  {
      // IF Freq <4:0>
      status |= MxLWare608_OEM_ReadRegister(u8TunerIndex,InitParam[u8TunerIndex].u8SlaveID, IF_FREQ_SEL_REG, &readData);
      readData &= 0x1f;
      switch(readData)
      {
          case MXL608_IF_3_65MHz:
              *u32IF_Freq = 3650;
              break;
          case MXL608_IF_4MHz:
              *u32IF_Freq = 4000;
              break;
          case MXL608_IF_4_1MHz:
              *u32IF_Freq = 4100;
              break;
          case MXL608_IF_4_15MHz:
              *u32IF_Freq = 4150;
              break;
          case MXL608_IF_4_5MHz:
              *u32IF_Freq = 4500;
              break;
          case MXL608_IF_4_57MHz:
              *u32IF_Freq = 4570;
              break;
          case MXL608_IF_5MHz:
              *u32IF_Freq = 5000;
              break;
          case MXL608_IF_5_38MHz:
              *u32IF_Freq = 5380;
              break;
          case MXL608_IF_6MHz:
              *u32IF_Freq = 6000;
              break;
          case MXL608_IF_6_28MHz:
              *u32IF_Freq = 6280;
              break;
          case MXL608_IF_7_2MHz:
               *u32IF_Freq = 7200;
              break;
          case MXL608_IF_8_25MHz:
              *u32IF_Freq = 8250;
              break;
          case MXL608_IF_35_25MHz:
              *u32IF_Freq = 35250;
              break;
          case MXL608_IF_36MHz:
              *u32IF_Freq = 36000;
              break;
          case MXL608_IF_36_15MHz:
              *u32IF_Freq = 36150;
              break;
          case MXL608_IF_36_65MHz:
              *u32IF_Freq = 36650;
              break;
          case MXL608_IF_44MHz:
              *u32IF_Freq = 44000;
              break;
          default:
              *u32IF_Freq = 5000;
              break;
      }
  }

  if(MXL_SUCCESS == status)
    return TRUE;
  else
    return FALSE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL608) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL608",          // demod system name
    .data               = TUNER_MXL608,            // private data value
    .Init               = MDrv_MXL608Tuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_MXL608Tuner_SetTuner,
    .CheckExist         = MXL608_CheckExist,
    .GetTunerIF         = MXL608_GetTunerIF,
    .Extension_Function = TUNER_MXL608_Extension_Function
};


#endif
