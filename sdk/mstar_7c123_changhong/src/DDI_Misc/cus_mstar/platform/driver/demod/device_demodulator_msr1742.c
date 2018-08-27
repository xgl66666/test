
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
#include "drvDemod.h"
#include "drvTunerNull.h"
#include "drvDemodNull.h"
#include "drvIIC.h"
#include "drvSYS.h"
#include "drvGPIO.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSR1742) && IF_THIS_TUNER_INUSE(TUNER_MSR1742)
#include "drvDMD_EXTERN_MSR1742.h"
#include "device_demodulator_msr1742.h"
#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
#include "drvDMD_common.h"
#include "drvMSPI.h"
#endif


#if TS_PARALLEL_OUTPUT
#define MSR1742_SERIAL_TS               0x00
#else
#define MSR1742_SERIAL_TS               0x01
#endif

static MS_BOOL MSR1742_detect[MAX_FRONTEND_NUM] =
#if (MAX_FRONTEND_NUM == 1)
{FALSE};
#elif (MAX_FRONTEND_NUM == 2)
{FALSE, FALSE};
#elif (MAX_FRONTEND_NUM == 3)
{FALSE, FALSE,FALSE};
#elif (MAX_FRONTEND_NUM == 4)
{FALSE, FALSE,FALSE,FALSE};
#else
{FALSE, FALSE,FALSE,FALSE};
#endif

static SLAVE_ID_USAGE* pstMSR1742_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE MSR1742_possible_slave_ID[5] =
{
    {0xC0, FALSE},
    {0xC2, FALSE},
    {0xC4, FALSE},
    {0xC6, FALSE},
    {0xFF, FALSE}
};

static DEMOD_MS_FE_CARRIER_PARAM DMDPreSetting[MAX_FRONTEND_NUM][MSR1742_CHANNEL_COUNT];
static MSR1742_TUNER_CONFIG TunerPreSetting[MAX_FRONTEND_NUM][MSR1742_CHANNEL_COUNT];
static MSR1742_CONFIG* pstMSR1742 = NULL;
static mapi_i2c DemodI2Chandler;
static MS_U8 ci_hdr[MSR1742_CHANNEL_COUNT] = {0x48, 0x49, 0x4A, 0x4B};


static MS_BOOL _msr1742_Variables_alloc(void)
{
    MS_U8 i,j;
    MSR1742_CONFIG *pMSR1742 = NULL;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_U8 u8MaxI2CPort;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    if(NULL == pstMSR1742)
    {
        pstMSR1742 = (MSR1742_CONFIG *)malloc(sizeof(MSR1742_CONFIG) * MAX_FRONTEND_NUM);
        if(NULL == pstMSR1742)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< MAX_FRONTEND_NUM; i++)
            {
                pMSR1742 = (pstMSR1742 + i);
                pMSR1742->bInited = FALSE;
                pMSR1742->s32_MSR1742_Mutex = -1;
                pMSR1742->bRest = TRUE;
                pMSR1742->u8SlaveID = MSR1742_SLAVE_ID; // default
                pMSR1742->stDMD_InitParam.u32DmxInputPath = DMX_INPUT_EXT_INPUT0;//default
                #if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
                pMSR1742->u32DmxInputPath = MSPI_PATH_NONE;
                #endif
                pMSR1742->RstPin = MSR1742_RST_PIN_NOT_SET;
            }
        }
    }

    if(NULL == pstMSR1742_slave_ID_TBL)
    {
        pstMSR1742_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MSR1742_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMSR1742_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MSR1742_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMSR1742_slave_ID_TBL + i*sizeof(MSR1742_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MSR1742_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                }
            }
         }
    }

    return TRUE;

}

static MS_BOOL _msr1742_Variables_free(void)
{
    if(NULL != pstMSR1742)
    {
        free(pstMSR1742);
        pstMSR1742 = NULL;
    }

    if(NULL != pstMSR1742_slave_ID_TBL)
    {
        free(pstMSR1742_slave_ID_TBL);
        pstMSR1742_slave_ID_TBL = NULL;
    }

    return TRUE;
}

static void _msr1742_hw_reset(MS_U8 u8DevIndex)
{
    MSR1742_CONFIG *pMSR1742 = (pstMSR1742 + u8DevIndex);

    if(pstMSR1742 == NULL)
        return;

    mdrv_gpio_set_high(pMSR1742->RstPin);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(pMSR1742->RstPin);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(pMSR1742->RstPin);
    MsOS_DelayTask(5);

    pMSR1742->bInited = FALSE;
    pMSR1742->bRest = TRUE;
}

MS_BOOL MSR1742_WriteBytes_demod(MS_U8 u8DevIndex,MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MSR1742_CONFIG *pMSR1742 = (pstMSR1742 + u8DevIndex);
    MS_IIC_PORT ePort;

    if(NULL == pstMSR1742)
    {
        DMD_ERR(("pstMSR1742 is NULL !\n"));
        return FALSE;
    }

    ePort = getI2CPort(u8DevIndex);
    bRet = MDrv_IIC_WriteBytes(ePort, (MS_U16)pMSR1742->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error, slave ID = 0x%02x\n", pMSR1742->u8SlaveID));
    }
    return bRet;
}

MS_BOOL MSR1742_ReadBytes_demod(MS_U8 u8DevIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MSR1742_CONFIG *pMSR1742 = (pstMSR1742 + u8DevIndex);
    MS_IIC_PORT ePort;

    if(NULL == pstMSR1742)
    {
        DMD_ERR(("pstMSR1742 is NULL !\n"));
        return FALSE;
    }

    ePort = getI2CPort(u8DevIndex);
    bRet = MDrv_IIC_ReadBytes(ePort, (MS_U16)pMSR1742->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}

static mapi_i2c* mapi_i2c_GetI2C_Dev(MS_U32 u32gID)
{
    mapi_i2c *handler;
    switch (u32gID)
    {
    default:
        DMD_ERR(("iic device not supported\n"));
    case MSR1742_DEMOD_IIC:
        handler = &DemodI2Chandler;
        handler->WriteBytes = MSR1742_WriteBytes_demod;
        handler->ReadBytes = MSR1742_ReadBytes_demod;
        break;
    }
    return handler;
}

static MS_BOOL MSR1742_I2C_CH_Reset(MS_U8 u8DevIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    mapi_i2c *iptr;
    MSR1742_CONFIG *pMSR1742 = NULL;

    iptr = mapi_i2c_GetI2C_Dev(MSR1742_DEMOD_IIC);
    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSR1742][beg]MSB1742_I2C_CH_Reset, CH=0x%x\n", ch_num));

    if (iptr == NULL)
    {
        return FALSE;
    }

    if(NULL == pstMSR1742)
        return FALSE;
    else
        pMSR1742 = pstMSR1742 + u8DevIndex;

    if (pMSR1742->bRest)
    {
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        if(iptr->WriteBytes(u8DevIndex, 0, NULL, 5, data))
            pMSR1742->bRest = FALSE;
        else
            return FALSE;
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= iptr->WriteBytes(u8DevIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= iptr->WriteBytes(u8DevIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= iptr->WriteBytes(u8DevIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= iptr->WriteBytes(u8DevIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= iptr->WriteBytes(u8DevIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= iptr->WriteBytes(u8DevIndex, 0, NULL, 1, data);

    /*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
    */
    DMD_DBG(("[MSR1742][end]MSB1742_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}


static MS_BOOL _MSR1742_GetReg(MS_U8 u8DevIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    mapi_i2c *iptr;


    iptr = mapi_i2c_GetI2C_Dev(MSR1742_DEMOD_IIC);
    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 5, u8MsbData);
    bRet &=iptr->ReadBytes(u8DevIndex, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);

    return bRet;
}

#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
static MS_BOOL _MSR1742_SetReg(MS_U8 u8DevIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    mapi_i2c *iptr;

    iptr = mapi_i2c_GetI2C_Dev(MSR1742_DEMOD_IIC);
    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);
    return bRet;
}


static MS_BOOL _MSR1742_SetReg2Bytes(MS_U8 u8DevIndex,MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    bRet &= _MSR1742_SetReg(u8DevIndex, u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= _MSR1742_SetReg(u8DevIndex, u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}
#endif


static MS_BOOL _MSR1742_I2C_Access(MS_U8 u8DevID, eDMD_MSR1742_I2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c_GetI2C_Dev(MSR1742_DEMOD_IIC);

    i2c_iptr = mapi_i2c_GetI2C_Dev(MSR1742_DEMOD_IIC);

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSR1742_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8DevID,u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSR1742_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8DevID, u8AddrSize, pu8Addr, u16Size, pu8Data);
        default:
            break;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
static void _msr1742_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void _msr1742_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void _msr1742_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {_msr1742_SPIPAD_TS0_En,\
                                   _msr1742_SPIPAD_TS1_En,\
                                   _msr1742_SPIPAD_TS2_En};

static void _msr1742_SPIPAD_TS0_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(MSR1742_SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_PARALLEL_IN);
    }

}

static void _msr1742_SPIPAD_TS1_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(MSR1742_SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_PARALLEL_IN);
    }

}

static void _msr1742_SPIPAD_TS2_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(MSR1742_SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_PARALLEL_IN);
    }
}
#endif

static  MS_BOOL _MSR1742_init(MS_U8 u8devID, MSR1742_CONFIG* pMSR1742)
{
    sDMD_MSR1742_InitData stMSR1742_DMD_InitData;
    sTUN_MSR1742_InitData stMSR1742_TUN_InitData;
    sDMD_MSR1742_TS_Param stMSR1742_TS_Param;
    sDMD_MSR1742_CIHeader stMSR1742_TS_CIHeader;
    MS_BOOL bRet=TRUE;
    MS_U8 i;

    memset(&stMSR1742_DMD_InitData, 0x00, sizeof(sDMD_MSR1742_InitData));
    memset(&stMSR1742_TUN_InitData, 0x00, sizeof(sTUN_MSR1742_InitData));
    memset(&stMSR1742_TS_Param, 0x00, sizeof(sDMD_MSR1742_TS_Param));
    memset(&stMSR1742_TS_CIHeader, 0x00, sizeof(sDMD_MSR1742_CIHeader));

    stMSR1742_DMD_InitData.fpMSR1742_I2C_Access = _MSR1742_I2C_Access;
    stMSR1742_TUN_InitData.fpMSR1742_I2C_Access = _MSR1742_I2C_Access;
    stMSR1742_DMD_InitData.pDVBC_DSP_REG = NULL; // utopia driver not ready
#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
    if(pMSR1742->u32DmxInputPath != MSPI_PATH_NONE)
    {
        stMSR1742_DMD_InitData.bEnableSPILoadCode = TRUE;
        stMSR1742_DMD_InitData.fpMSR1742_SPIPAD_En = SPIPAD_EN[pMSR1742->u32DmxInputPath];//_msr1742_SPIPAD_TS0_En;
        stMSR1742_TUN_InitData.bEnableSPILoadCode = TRUE;
        stMSR1742_TUN_InitData.fpMSR1742_SPIPAD_En = SPIPAD_EN[pMSR1742->u32DmxInputPath];
    }
    else
#endif
    {
        stMSR1742_DMD_InitData.bEnableSPILoadCode = FALSE;
        stMSR1742_DMD_InitData.fpMSR1742_SPIPAD_En = NULL;
        stMSR1742_TUN_InitData.bEnableSPILoadCode = FALSE;
        stMSR1742_TUN_InitData.fpMSR1742_SPIPAD_En = NULL;
    }

    bRet &= MDrv_DMD_MSR1742_Init(u8devID, ALL_DEMOD,&stMSR1742_DMD_InitData, sizeof(sDMD_MSR1742_InitData));
    bRet &= MDrv_TUN_MSR1742_Init(u8devID, &stMSR1742_TUN_InitData, sizeof(sTUN_MSR1742_InitData));
    //bRet &= MDrv_DMD_MSR1742_Power_On_Initialization( u8devID,0,0);

    // Config TS out
    if(MSR1742_SERIAL_TS)
    {
       DMD_DBG(("[MSR1742] TS Set Serail Mode\n"));
       stMSR1742_TS_Param.eTSMode = E_DMD_MSR1742_4_WIRE;
       stMSR1742_TS_Param.bRemuxLSB1st = FALSE;
       for(i=0;i<MSR1742_CHANNEL_COUNT;i++)
           stMSR1742_TS_Param.bLSB1st[i] = FALSE;
    }
    else
    {
       DMD_DBG(("[MSR1742] TS Set Parallel Mode\n"));
       stMSR1742_TS_Param.eTSMode = E_DMD_MSR1742_PARALLEL;
    }

    for(i=0;i<MSR1742_CHANNEL_COUNT;i++)
        stMSR1742_TS_Param.bEnable[i] = TRUE;

    stMSR1742_TS_Param.eTSClk = E_DMD_MSR1742_TS_CLK_AUTO; //not ready
    stMSR1742_TS_Param.eTSDrv = E_DMD_MSR1742_TS_DRVING_MAX; //not ready
    stMSR1742_TS_Param.bCLKInverse = TRUE;
    bRet &= MDrv_DMD_MSR1742_Set_TSOut(u8devID, &stMSR1742_TS_Param);

    //Config extra header
    if(!MSR1742_SERIAL_TS)
    {
       for(i=0;i<MSR1742_CHANNEL_COUNT;i++)
       {
          stMSR1742_TS_CIHeader.bEnable = TRUE;
          stMSR1742_TS_CIHeader.pHeaderPtr = &ci_hdr[i];
          bRet &= MDrv_DMD_MSR1742_CfgCIHeader(u8devID, i,  &stMSR1742_TS_CIHeader);
       }
    }

    memset(&DMDPreSetting, 0x00, sizeof(DEMOD_MS_FE_CARRIER_PARAM)*MAX_FRONTEND_NUM*MSR1742_CHANNEL_COUNT);
    memset(&TunerPreSetting, 0x00, sizeof(MSR1742_TUNER_CONFIG)*MAX_FRONTEND_NUM*MSR1742_CHANNEL_COUNT);
    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Global Variables
//---------------------------------------------------------------------------------------------

MS_BOOL MSR1742_Tuner_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID = u8TunerIndex/MSR1742_CHANNEL_COUNT;
    MS_BOOL bRet = TRUE;
    MSR1742_CONFIG* pMSR1742 = NULL;

    if(NULL == pstMSR1742)
        return FALSE;
    else
        pMSR1742 = pstMSR1742 + u8devID;

    if(pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        pMSR1742->stTUN_InitParam.pCur_Broadcast_type = pParam->pCur_Broadcast_type;


    if(pMSR1742->bInited== TRUE)
        return TRUE;

    bRet = _MSR1742_init(u8devID, pMSR1742);
    if(bRet)
         pMSR1742->bInited = TRUE;

    return bRet;
}


MS_BOOL MSR1742_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
    MS_BOOL bRet = TRUE;
    MSR1742_CONFIG* pMSR1742 = NULL;

    if(NULL == pstMSR1742)
        return FALSE;
    else
        pMSR1742 = pstMSR1742 + u8devID;

    if(pMSR1742->bInited== TRUE)
        return TRUE;

    bRet = _MSR1742_init(u8devID, pMSR1742);
    if(bRet)
         pMSR1742->bInited = TRUE;

    return bRet;
}


MS_BOOL MSR1742_Tuner_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
   MS_U8 u8devID = u8TunerIndex/MSR1742_CHANNEL_COUNT;
   MS_U8 u8chID = u8TunerIndex%MSR1742_CHANNEL_COUNT;
   MS_BOOL bret = TRUE;
   MSR1742_TUNER_CONFIG* pstPreSetting = NULL;

   pstPreSetting = &TunerPreSetting[u8devID][u8chID];

   if((dwFreq == pstPreSetting->u32Freq) && (ucBw == pstPreSetting->u8BW))
   {
       return bret;
   }
   else
   {
       pstPreSetting->u32Freq = dwFreq;
       pstPreSetting->u8BW = ucBw;
   }

   bret &= MDrv_TUN_MSR1742_Tuner_SetTuner(u8devID, u8chID, dwFreq, ucBw);

   return bret;
}

#define MSR1742_CHIP_ID1 0xc7
#define MSR1742_CHIP_ID2 0xaa

static MS_BOOL _msr1742_I2C_CH3_reset(MS_U8 u8DevIndex)
{
    //Reset twice to check if reset pin is correct
    _msr1742_hw_reset(u8DevIndex);
    if(!MSR1742_I2C_CH_Reset(u8DevIndex,3))
    {
        DMD_ERR(("[MSR1742] I2C_CH_Reset fail \n"));
        return FALSE;
    }
    else
    {
        _msr1742_hw_reset(u8DevIndex);
        if(!MSR1742_I2C_CH_Reset(u8DevIndex,3))
        {
            DMD_ERR(("[MSR1742] I2C_CH_Reset fail \n"));
            return FALSE;
        }
   }
   return TRUE;
}

MS_BOOL MSR1742_Check_CHIP_ID(MS_U8 u8devID, MS_BOOL* pbGotID)
{
     MS_U8 u8_tmp = 0, i;
     MS_BOOL bret = TRUE, bEverDetect = FALSE;
     MSR1742_CONFIG *pMSR1742 = NULL;
     SLAVE_ID_USAGE *pMSR1742_ID_TBL;
     MS_U8 u8I2C_Port;
     MS_IIC_PORT ePort;

    if(!_msr1742_Variables_alloc())
        return FALSE;
    else
    {
        ePort = getI2CPort(u8devID);
        pMSR1742 = (pstMSR1742 + u8devID);
        if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
        {
            u8I2C_Port = (MS_U8)ePort/8;
        }
        else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
        {
           u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
        }
    }

    i=0;
    do
    {
        DMD_DBG(("### %x\n",i));
        pMSR1742_ID_TBL = pstMSR1742_slave_ID_TBL + u8I2C_Port*sizeof(MSR1742_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        if(pMSR1742_ID_TBL->u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSR1742] All Slave ID have tried but not detect\n"));
            break;
        }

        if(pMSR1742_ID_TBL->bInUse)
        {
            DMD_DBG(("[MSR1742] Slave ID 0x%x have been used\n", pMSR1742_ID_TBL->u8SlaveID));
            i++;
            continue;
        }
        else
        {
            pMSR1742->u8SlaveID = pMSR1742_ID_TBL->u8SlaveID;
            DMD_DBG(("[MSR1742] Try slave ID 0x%x\n",pMSR1742_ID_TBL->u8SlaveID));
        }

        if(_msr1742_I2C_CH3_reset(u8devID))
        {
            if(_MSR1742_GetReg(u8devID,0x0900,&u8_tmp))
                DMD_DBG(("[MSR1742] read id :%x \n",u8_tmp ));
        }

        if((u8_tmp == MSR1742_CHIP_ID1)|| (u8_tmp == MSR1742_CHIP_ID2))
        {
            pMSR1742_ID_TBL->bInUse = TRUE;
            break;
        }
        i++;
    }while((pMSR1742_ID_TBL->u8SlaveID) != 0xFF);


     if((u8_tmp == MSR1742_CHIP_ID1)|| (u8_tmp == MSR1742_CHIP_ID2))
     {
        *pbGotID = TRUE;

#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
                //check TS path
                MDrv_DMD_SSPI_Init(0);
                MDrv_MasterSPI_CsPadConfig(0, 0xff);
                MDrv_MasterSPI_MaxClkConfig(0, 27);
                _MSR1742_SetReg2Bytes(u8devID,0x0900+(0x28)*2, 0x0000);
                _MSR1742_SetReg2Bytes(u8devID,0x0900+(0x2d)*2, 0x00ff);
                // ------enable to use TS_PAD as SSPI_PAD
                // [0:0] reg_en_sspi_pad
                // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
                if(u8_tmp == MSR1742_CHIP_ID1)
                    _MSR1742_SetReg2Bytes(u8devID,0x0900+(0x3b)*2, 0x0001);
                else
                    _MSR1742_SetReg2Bytes(u8devID,0x0900+(0x3b)*2, 0x0002);

                // ------- MSPI protocol setting
                // [8] cpha
                // [9] cpol
                _MSR1742_GetReg(u8devID,0x0900+(0x3a)*2+1,&u8_tmp);
                u8_tmp &= 0xFC;
                _MSR1742_SetReg(u8devID,0x0900+(0x3a)*2+1, u8_tmp);

                // ------- MSPI driving setting
                _MSR1742_SetReg2Bytes(u8devID,0x0900+(0x2c)*2, 0x07ff);

                for(i=0; i<MSPI_PATH_MAX; i++)
                {
                    SPIPAD_EN[i](TRUE);
                    // Note: Must read twice and ignore 1st data because pad switch noise
                    // might cause wrong MSPI signal
                    MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
                    MDrv_DMD_SSPI_RIU_Read8(0x900, &u8_tmp);
                    if((u8_tmp == MSR1742_CHIP_ID1)|| (u8_tmp == MSR1742_CHIP_ID2))
                    {
                        pMSR1742->u32DmxInputPath = (MS_U32)i;
                        SPIPAD_EN[i](FALSE);
                        DMD_DBG(("Get MSR1742 chip ID by MSPI on TS%d\n", (int)pMSR1742->u32DmxInputPath));
                        break;
                    }
                    else
                    {
                        DMD_DBG(("Cannot get MSR1742 chip ID by MSPI on TS%x\n", i));
                        if( i == (MSPI_PATH_MAX - 1))
                            pMSR1742->u32DmxInputPath = MSPI_PATH_NONE;
                    }
                    SPIPAD_EN[i](FALSE);
                }

                // ------disable to use TS_PAD as SSPI_PAD after load code
                // [0:0] reg_en_sspi_pad
                // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
                _MSR1742_SetReg2Bytes(u8devID, 0x0900 + (0x3b) * 2, 0x0001);
#else
           pMSR1742->u32DmxInputPath = 0xff;  //for compile warning
#endif //MSPI_PATH_DETECT
     }
     else
     {
        for(i=0; i< MAX_FRONTEND_NUM; i++)
        {
           bEverDetect |= MSR1742_detect[i];
        }

        if(!bEverDetect)
            _msr1742_Variables_free();

        *pbGotID = FALSE;
     }

     return bret;

}

MS_BOOL MSR1742_TunerCheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
  MS_U8 u8devID;
  MS_BOOL bGotID = FALSE, bret = TRUE;

  u8devID = u8TunerIndex; // here u8TunerIndex means device index

  if(!MSR1742_detect[u8devID])
  {
      bret = MSR1742_Check_CHIP_ID(u8devID, &bGotID);
      if(bret && bGotID)
      {
         MSR1742_detect[u8devID] = TRUE;
         TUNER_DBG(("== MSR1742 detected \n"));
      }
      else
      {
         bret = FALSE;
      }

  }
  else
  {
     TUNER_DBG(("== MSR1742 have been detected \n"));
     bGotID = TRUE;
  }

  if(bret && bGotID)
  {
      if(NULL != pu32channel_cnt)
          *(pu32channel_cnt) += MSR1742_CHANNEL_COUNT;
  }

  return bret;
}


MS_BOOL MSR1742_DemodCheckExist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8devID;
    MS_BOOL bGotID = FALSE, bret = TRUE;

    u8devID = u8DemodIndex; // here u8DemodIndex means device index

    if(!MSR1742_detect[u8devID])
    {
        bret = MSR1742_Check_CHIP_ID(u8devID, &bGotID);
        if(bret && bGotID)
        {
           MSR1742_detect[u8devID] = TRUE;
           TUNER_DBG(("== MSR1742 detected \n"));
        }
        else
        {
           bret = FALSE;
        }

    }
    else
    {
       TUNER_DBG(("== MSR1742 have been detected \n"));
       bGotID = TRUE;
    }

    if(bret && bGotID)
    {
       *pu8SlaveID = MSR1742_SLAVE_ID;
    }

    return bret;
}



MS_BOOL MSR1742_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
    {
        MS_BOOL bret=TRUE;
        MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
        MSR1742_CONFIG *pMSR1742 = NULL;

        switch(fuction_type)
        {
            case DEMOD_EXT_FUNC_FINALIZE:
                if(NULL == pstMSR1742)
                    return FALSE;

                pMSR1742 = (pstMSR1742 + u8devID);
                if(pMSR1742->s32_MSR1742_Mutex >= 0)
                {
                    bret &= MsOS_DeleteMutex(pMSR1742->s32_MSR1742_Mutex);
                    pMSR1742->s32_MSR1742_Mutex = -1;
                }

                pMSR1742->bInited = FALSE;
                pMSR1742->bRest = TRUE;

                break;
            case DEMOD_EXT_FUNC_SET_RESET_PIN:
                 u8devID = u8DemodIndex; // here means device ID
                 if(!_msr1742_Variables_alloc())
                 {
                     _msr1742_Variables_free();
                     return FALSE;
                 }
                 else
                 {
                     pMSR1742 = (pstMSR1742 + u8devID);
                     pMSR1742->RstPin= *(int*)data;
                }
            break;
             default:
                DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
                break;
         }
       return bret;
    }


MS_BOOL MSR1742_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
    MS_U8 u8DemodID = u8DemodIndex%MSR1742_CHANNEL_COUNT;
    eDMD_MSR1742_LOCK_STATUS eLockStatus = DMD_DVBC_CHECKING;

    MDrv_DMD_MSR1742_GetLock(u8devID, u8DemodID, DMD_DVBC_GETLOCK_FEC_LOCK, &eLockStatus);
    if(eLockStatus == DMD_DVBC_LOCK)
    {
       *peLockStatus = E_DEMOD_LOCK;
    }
    else if(eLockStatus == DMD_DVBC_UNLOCK)
    {
       *peLockStatus = E_DEMOD_UNLOCK;
    }
    else
    {
       *peLockStatus = E_DEMOD_CHECKING;
    }

    return bret;

}


MS_BOOL MSR1742_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
    MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
    MS_U8 u8chID = u8DemodIndex%MSR1742_CHANNEL_COUNT;
    MS_BOOL bret = TRUE;
    MS_U16 u16SymbolRate;
    eDMD_MSR1742_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq = 5000;
    MS_BOOL bSpecInv = FALSE;
    DEMOD_MS_FE_CARRIER_PARAM* pstDMDPreSetting = NULL;

    u16SymbolRate = pParam->CabParam.u16SymbolRate;
    pstDMDPreSetting = &DMDPreSetting[u8devID][u8chID];

    if((pstDMDPreSetting->CabParam.u16SymbolRate == u16SymbolRate) &&\
        (pstDMDPreSetting->CabParam.eConstellation == pParam->CabParam.eConstellation)&&\
        (pstDMDPreSetting->u32Frequency== pParam->u32Frequency))
    {
        return bret;
    }
    else
    {
       memcpy(pstDMDPreSetting, pParam, sizeof(DEMOD_MS_FE_CARRIER_PARAM));
    }

   switch(pParam->CabParam.eConstellation)
   {
       case DEMOD_CAB_QAM16:
           eQamMode = DMD_DVBC_QAM16;
           break;

       case DEMOD_CAB_QAM32:
           eQamMode = DMD_DVBC_QAM32;
           break;

       case DEMOD_CAB_QAM64:
           eQamMode = DMD_DVBC_QAM64;
           break;

       case DEMOD_CAB_QAM128:
           eQamMode = DMD_DVBC_QAM128;
           break;

       case DEMOD_CAB_QAM256:
           eQamMode = DMD_DVBC_QAM256;
           break;

       case DEMOD_CAB_QAMAUTO:
       default:
           eQamMode = DMD_DVBC_QAMAUTO;
           break;

   }

   bret &= MDrv_DMD_MSR1742_SetConfig(u8devID, u8chID, u16SymbolRate, eQamMode, u32IFFreq, bSpecInv);
   bret &= MDrv_DMD_MSR1742_SetActive(u8devID, u8chID, TRUE);
   return bret;
}

MS_BOOL MSR1742_Demod_GetPacketError(MS_U8 u8DemodIndex,MS_U16 *u16PktErr)
{
     MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
     MS_U8 u8chID = u8DemodIndex%MSR1742_CHANNEL_COUNT;
     MS_BOOL bret = TRUE;

    bret = MDrv_DMD_MSR1742_GetPacketErr(u8devID, u8chID,u16PktErr);
    return bret;

}

MS_BOOL MSR1742_Demod_GetSNR(MS_U8 u8DemodIndex,MS_U16 *u16PktErr)
{
     MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
     MS_U8 u8chID = u8DemodIndex%MSR1742_CHANNEL_COUNT;
     MS_BOOL bret = TRUE;

    bret = MDrv_DMD_MSR1742_GetPacketErr(u8devID, u8chID,u16PktErr);
    return bret;
}

MS_BOOL MSR1742_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
    float fPWR = 0;
    MS_BOOL bret = TRUE;

    bret = MDrv_TUN_MSR1742_GetPWR(u8devID, &fPWR);

    if(bret)
        *ps32Signal = (MS_S32)fPWR;
    else
        *ps32Signal = 0;

    return bret;
}


#ifdef FE_AUTO_TEST
MS_BOOL  MSR1742_Demod_ReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
    MS_BOOL bret = TRUE;

    bret = MDrv_DMD_MSR1742_GetReg(u8devID, RegAddr, pu8Data);
    return bret;
}

MS_BOOL MSR1742_Demod_WriteReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U16 RegData)
{
    MS_U8 u8devID = u8DemodIndex/MSR1742_CHANNEL_COUNT;
    MS_BOOL bret = TRUE;
    MS_U8 u8RegData;

    u8RegData = (MS_U8)RegData;
    bret = MDrv_DMD_MSR1742_SetReg(u8devID, RegAddr, u8RegData);
    return bret;
}
#endif

MS_BOOL MSR1742_Tuner_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    float fPWR;
    MS_BOOL bret = TRUE;
    MS_U8 u8devID = u8TunerIndex/MSR1742_CHANNEL_COUNT;

    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            bret = MDrv_TUN_MSR1742_GetPWR(u8devID, &fPWR);
            *(int*)data = (int)fPWR;
            break;


        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return bret;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MSR1742) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MSR1742",          // demod system name
    .data               = TUNER_MSR1742,            // private data value
    .Init               = MSR1742_Tuner_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MSR1742_Tuner_SetTuner,
    .GetTunerIF         = MDRV_Tuner_Null_GetTunerIF,
    .CheckExist         = MSR1742_TunerCheckExist,
    .Extension_Function = MSR1742_Tuner_Extension_Function
};

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSR1742) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSR1742",
     .data                         = DEMOD_MSR1742,
     .SupportINT                   = FALSE,
     .init                         = MSR1742_Demod_Init,
     .GetLock                      = MSR1742_Demod_GetLock,
     .GetSNR                       = MDrv_Demod_null_GetSNR,
     .GetBER                       = MDrv_Demod_null_GetBER,
     .GetPWR                       = MSR1742_Demod_GetPWR,
     .GetSSI                       = MDrv_Demod_null_GetSSI,
     .GetQuality                   = MDrv_Demod_null_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MSR1742_Demod_Restart,
     .I2CByPass                    = MDrv_Demod_null_I2C_ByPass,
     .CheckExist                   = MSR1742_DemodCheckExist,
     .Extension_Function           = MSR1742_Extension_Function,
     .Get_Packet_Error             = MSR1742_Demod_GetPacketError,
 #ifdef FE_AUTO_TEST
     .ReadReg                      = MSR1742_Demod_ReadReg,
     .WriteReg                     = MSR1742_Demod_WriteReg,
 #endif
 #if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
     .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
     .GetPLPType                   = MDrv_Demod_null_GetPLPType,
 #endif
 #if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd,
     .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
     .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
     .SetISID                      = MDrv_Demod_null_SetVCM_ISID
 #endif
};

#endif
