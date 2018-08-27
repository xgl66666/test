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
#include "drvGPIO.h"
#include "MsOS.h"
#include "drvSYS.h"

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB201X)
#include "drvDMD_EXTERN_MSB201X.h"
#include "device_demodulator_msb201x.h"
#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
#include "drvDMD_common.h"
#include "drvMSPI.h"
#endif

#if TS_PARALLEL_OUTPUT
#define MSB201x_SERIAL_TS               0x00
#else
#define MSB201x_SERIAL_TS               0x01
#endif
#define MSB201x_CI_HEADER_En           TRUE

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB201X) DDI_DRV_TABLE_ENTRY(demodtab);

static MSB201X_CONFIG* pstMSB201x = NULL;
static MS_U32 _u32DefaultIFrequency = 5000;
static MS_U8 u8MSB201X_Dev_Cnt=0;
static SLAVE_ID_USAGE* pstMSB201X_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE MSB201X_possible_slave_ID[3] =
{
    {0xF2, FALSE},
    {0xD2, FALSE},
    {0xFF, FALSE}
};

static MS_U8 ci_hdr[2][MSB201X_CHANNEL_COUNT] =
{
   {0x48, 0x49},
   {0x48, 0x49}
};

typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

static mapi_i2c DemodI2Chandler;
static MS_BOOL _Is_msb101x(void)
{
   if(strcmp(GET_DEMOD_ENTRY_NODE(DEMOD_MSB201X).name,"DEMOD_MSB101X") == 0)
       return TRUE;
   else
       return FALSE;
}

static void _msb201x_get_IDs(MS_U8 u8DemodIndex, MS_U8* pu8devID, MS_U8* pu8DemodID)
{
    if(_Is_msb101x())
    {
        *pu8devID = u8DemodIndex;
        *pu8DemodID = 0;
    }
    else
    {
        *pu8devID = u8DemodIndex/MSB201X_CHANNEL_COUNT;
        *pu8DemodID = u8DemodIndex%MSB201X_CHANNEL_COUNT;
    }

}

static MS_BOOL _msb201x_Variables_alloc(void)
{
    MS_U8 i,j;
    MSB201X_CONFIG *pMSB201x = NULL;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_U8 u8MaxI2CPort;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    if(NULL == pstMSB201x)
    {
        pstMSB201x = (MSB201X_CONFIG *)malloc(sizeof(MSB201X_CONFIG) * MAX_FRONTEND_NUM);
        if(NULL == pstMSB201x)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< MAX_FRONTEND_NUM; i++)
            {
                pMSB201x = (pstMSB201x + i);
                pMSB201x->s32_MSB201x_Mutex = -1;
                pMSB201x->u8SlaveID = MSB201X_SLAVE_ID;
                pMSB201x->bRest = TRUE;
                pMSB201x->RstPin = MSB201X_RST_PIN_NOT_SET;
                for(j=0; j< MSB201X_CHANNEL_COUNT; j++)
                {
                    pMSB201x->bInited[j] = FALSE;
                    pMSB201x->MSB201x_InitParam[j].pstTunertab = NULL;
                    pMSB201x->MSB201x_InitParam[j].u32DmxInputPath = DMX_INPUT_EXT_INPUT0;//default;
                }
                #if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
                pMSB201x->u32DmxInputPath = MSPI_PATH_NONE;
                #endif
            }
        }
    }


    if(NULL == pstMSB201X_slave_ID_TBL)
    {
        pstMSB201X_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MSB201X_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMSB201X_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MSB201X_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMSB201X_slave_ID_TBL + i*sizeof(MSB201X_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MSB201X_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                }
            }
        }
    }

    return TRUE;

}

static MS_BOOL _msb201x_Variables_free(void)
{
    if(NULL != pstMSB201x)
    {
        free(pstMSB201x);
        pstMSB201x = NULL;
    }

    if(NULL != pstMSB201X_slave_ID_TBL)
    {
        free(pstMSB201X_slave_ID_TBL);
        pstMSB201X_slave_ID_TBL = NULL;
    }

    return TRUE;
}

static void _msb201X_hw_reset(MS_U8 u8DevIndex)
{
    MSB201X_CONFIG *pMSB201X = (pstMSB201x + u8DevIndex);
    int i;

    if(pstMSB201x == NULL)
        return;

    mdrv_gpio_set_high(pMSB201X->RstPin);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(pMSB201X->RstPin);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(pMSB201X->RstPin);
    MsOS_DelayTask(5);

    for(i=0;i<MSB201X_CHANNEL_COUNT;i++)
    {
        pMSB201X->bInited[i] = FALSE;
    }

    pMSB201X->bRest = TRUE;

}

MS_BOOL MSB201X_WriteBytes_demod(MS_U8 u8DevIndex,MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MSB201X_CONFIG *pMSB201X = (pstMSB201x + u8DevIndex);
    MS_IIC_PORT ePort;

    if(NULL == pstMSB201x)
    {
        DMD_ERR(("pstMSB201x is NULL !\n"));
        return FALSE;
    }

    ePort = getI2CPort(u8DevIndex);
    bRet = MDrv_IIC_WriteBytes(ePort, (MS_U16)pMSB201X->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error\n"));
    }
    return bRet;
}

MS_BOOL MSB201X_ReadBytes_demod(MS_U8 u8DevIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MSB201X_CONFIG *pMSB201X = (pstMSB201x + u8DevIndex);
    MS_IIC_PORT ePort;

    if(NULL == pstMSB201x)
    {
        DMD_ERR(("pstMSB201x is NULL !\n"));
        return FALSE;
    }

    ePort = getI2CPort(u8DevIndex);
    bRet = MDrv_IIC_ReadBytes(ePort, (MS_U16)pMSB201X->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
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
    case MSB201X_DEMOD_IIC:
        handler = &DemodI2Chandler;
        handler->WriteBytes = MSB201X_WriteBytes_demod;
        handler->ReadBytes = MSB201X_ReadBytes_demod;
        break;
    }
    return handler;
}


static MS_BOOL _msb201x_I2C_CH_Reset(MS_U8 u8DevIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    mapi_i2c *iptr;
    MSB201X_CONFIG *pMSB201x = NULL;


    iptr = mapi_i2c_GetI2C_Dev(MSB201X_DEMOD_IIC);
    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSB201X][beg]MSB201X_I2C_CH_Reset, CH=0x%x\n", ch_num));

    if (iptr == NULL)
    {
        return FALSE;
    }


    if(NULL == pstMSB201x)
    {
        return FALSE;
    }
    else
    {
       pMSB201x = pstMSB201x + u8DevIndex;
    }

    if(pMSB201x->bRest)
    {
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        if(iptr->WriteBytes(u8DevIndex, 0, NULL, 5, data))
            pMSB201x->bRest = FALSE;
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
    DMD_DBG(("[MSB201X][end]MSB201X_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}


static MS_BOOL _msb201x_GetReg(MS_U8 u8DevIndex, MS_U32 u32Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    mapi_i2c *iptr;

    iptr = mapi_i2c_GetI2C_Dev(MSB201X_DEMOD_IIC);
    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = (u32Addr>>16)&0xff;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr & 0xff;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 5, u8MsbData);
    bRet &=iptr->ReadBytes(u8DevIndex, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);

    return bRet;
}

static MS_BOOL _msb201x_SetReg(MS_U8 u8DevIndex, MS_U32 u32Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    mapi_i2c *iptr;

    iptr = mapi_i2c_GetI2C_Dev(MSB201X_DEMOD_IIC);
    if (iptr == NULL)
    {
        return FALSE;
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = (u32Addr >> 16) &0xff;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(u8DevIndex, 0, NULL, 1, u8MsbData);
    return bRet;
}

#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
static MS_BOOL _msb201x_SetReg2Bytes(MS_U8 u8DevIndex,MS_U32 u32Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    bRet &= _msb201x_SetReg(u8DevIndex, u32Addr, (MS_U8)u16Data&0x00ff);
    bRet &= _msb201x_SetReg(u8DevIndex, u32Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}
#endif

static MS_BOOL _msb201x_I2C_Access(MS_U8 U8DevID, eDMD_MSB201X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr = mapi_i2c_GetI2C_Dev(MSB201X_DEMOD_IIC);

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(U8DevID,u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB201X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(U8DevID, u8AddrSize, pu8Addr, u16Size, pu8Data);
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
static void _msb201x_SPIPAD_TS0_En(MS_BOOL bOnOff);
static void _msb201x_SPIPAD_TS1_En(MS_BOOL bOnOff);
static void _msb201x_SPIPAD_TS2_En(MS_BOOL bOnOff);
static fpSPIPAD_En SPIPAD_EN[3]= {_msb201x_SPIPAD_TS0_En,\
                                   _msb201x_SPIPAD_TS1_En,\
                                   _msb201x_SPIPAD_TS2_En};

static void _msb201x_SPIPAD_TS0_En(MS_BOOL bOnOff)
{
    printf("_msb201x_SPIPAD_TS0_En %x\n", bOnOff);
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(MSB201x_SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS0_PAD_SET, E_PARALLEL_IN);
    }

}

static void _msb201x_SPIPAD_TS1_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(MSB201x_SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS1_PAD_SET, E_PARALLEL_IN);
    }

}

static void _msb201x_SPIPAD_TS2_En(MS_BOOL bOnOff)
{
    if(bOnOff)
        MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_MSPI_PAD_ON);
    else
    {
       if(MSB201x_SERIAL_TS)
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_SERIAL_IN);
       else
          MDrv_SYS_SetPadMux(E_TS2_PAD_SET, E_PARALLEL_IN);
    }
}
#endif

static  MS_BOOL _msb201x_init(MS_U8 u8devID, MSB201X_CONFIG* pMSB201x)
{
    MS_BOOL bRet=TRUE;
    sDMD_MSB201X_InitData stMSB201X_InitData;
    sDMD_MSB201X_TS_Param stMSB201X_TS_Param;
    sDMD_MSB201X_CIHeader stMSB201X_CIHeader;
    MS_U8 i;

    stMSB201X_InitData.fpMSB201X_I2C_Access= _msb201x_I2C_Access;
    stMSB201X_InitData.bEnableSPILoadCode = FALSE;
    stMSB201X_InitData.fpMSB201X_SPIPAD_En = NULL;

#if defined(SUPPORT_MSPI_LOAD_CODE) && (SUPPORT_MSPI_LOAD_CODE == 1)
    if(pMSB201x->u32DmxInputPath != MSPI_PATH_NONE)
    {
        stMSB201X_InitData.bEnableSPILoadCode = TRUE;
        stMSB201X_InitData.fpMSB201X_SPIPAD_En = SPIPAD_EN[pMSB201x->u32DmxInputPath];
    }
    else
#endif
    {
        stMSB201X_InitData.bEnableSPILoadCode = FALSE;
        stMSB201X_InitData.fpMSB201X_SPIPAD_En = NULL;
    }

    stMSB201X_InitData.bFlashWPEnable = FALSE;
    stMSB201X_InitData.bPreloadDSPCodeFromMainCHIPI2C = FALSE;
    stMSB201X_InitData.DVBC_DSP_REG_Length = 0;
    stMSB201X_InitData.fpGPIOReset = NULL;
    stMSB201X_InitData.pDVBC_DSP_REG = NULL;
    stMSB201X_InitData.u8WO_Sdram = FALSE;
    stMSB201X_InitData.u8WO_SPI_Flash = FALSE;

    stMSB201X_InitData.IFAGC_Data[0].pIFAGC_ERR = TUNER_IfagcErr_LoRef;
    stMSB201X_InitData.IFAGC_Data[0].IFAGC_ERR_Length = sizeof(TUNER_IfagcErr_LoRef)/sizeof(sDMD_MSB201X_IFAGC_ERR);
    stMSB201X_InitData.IFAGC_Data[0].pIFAGC_SSI = TUNER_IfagcSsi_LoRef;
    stMSB201X_InitData.IFAGC_Data[0].IFAGC_SSI_Length = sizeof(TUNER_IfagcSsi_LoRef)/sizeof(sDMD_MSB201X_IFAGC_SSI);

    stMSB201X_InitData.IFAGC_Data[1].pIFAGC_ERR = TUNER_IfagcErr_LoRef;
    stMSB201X_InitData.IFAGC_Data[1].IFAGC_ERR_Length = sizeof(TUNER_IfagcErr_LoRef)/sizeof(sDMD_MSB201X_IFAGC_ERR);
    stMSB201X_InitData.IFAGC_Data[1].pIFAGC_SSI = TUNER_IfagcSsi_LoRef;
    stMSB201X_InitData.IFAGC_Data[1].IFAGC_SSI_Length = sizeof(TUNER_IfagcSsi_LoRef)/sizeof(sDMD_MSB201X_IFAGC_SSI);

    bRet &= MDrv_DMD_MSB201X_Init(u8devID,ALL_DEMOD, &stMSB201X_InitData, sizeof(sDMD_MSB201X_InitData));

    // Config TS out
    if(MSB201x_SERIAL_TS)
    {
       DMD_DBG(("[MSB201x] TS Set Serail Mode\n"));
       stMSB201X_TS_Param.eTSMode= E_DMD_MSB201X_3_WIRE_DMD0_TS0_DMD1_TS1;
       stMSB201X_TS_Param.bRemuxLSB1st = FALSE;
       for(i=0;i<MSB201X_CHANNEL_COUNT;i++)
           stMSB201X_TS_Param.bLSB1st[i] = FALSE;
    }
    else
    {
       DMD_DBG(("[MSB201x] TS Set Parallel Mode\n"));
       stMSB201X_TS_Param.eTSMode = E_DMD_MSB201X_PARALLEL;
    }

    for(i=0;i<MSB201X_CHANNEL_COUNT;i++)
        stMSB201X_TS_Param.bEnable[i] = TRUE;

    stMSB201X_TS_Param.eTSClk = E_DMD_MSB201X_TS_CLK_54MHz;
    stMSB201X_TS_Param.eTSDrv = E_DMD_MSB201X_TS_DRVING_MAX; //not ready
    stMSB201X_TS_Param.bCLKInverse = TRUE;
    bRet &= MDrv_DMD_MSB201X_Set_TSOut(u8devID, &stMSB201X_TS_Param);

    //Config extra header

    if((!MSB201x_SERIAL_TS) && (!_Is_msb101x()))
    {
       for(i=0;i<MSB201X_CHANNEL_COUNT;i++)
       {
          stMSB201X_CIHeader.bEnable = MSB201x_CI_HEADER_En;
          stMSB201X_CIHeader.pHeaderPtr = &ci_hdr[u8devID][i];

          bRet &= MDrv_DMD_MSB201X_CfgCIHeader(u8devID, i,  &stMSB201X_CIHeader);
       }
    }

    return bRet;
}


//-------------------------------------------------------------------------------------------------
//  Global Variables
//---------------------------------------------------------------------------------------------

MS_BOOL MSB201X_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MS_U8 u8devID;
    MS_U8 u8DemodID;
    MS_BOOL bRet = TRUE;
    MSB201X_CONFIG *pMSB201x = NULL;

    if(pstMSB201x == NULL)
        return FALSE;

    _msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);

    pMSB201x = pstMSB201x + u8devID;

    pMSB201x->MSB201x_InitParam[u8DemodID].pstTunertab = pParam->pstTunertab;
    if((pMSB201x->bInited[u8DemodID] == TRUE) )
        return TRUE;

    bRet = _msb201x_init(u8devID, pMSB201x);
    if(bRet)
    {
        //pMSB201x->bInited[u8DemodID] = TRUE;
        pMSB201x->bInited[0] = TRUE; // due to choose init all
        pMSB201x->bInited[1] = TRUE;
    }

    return bRet;
}

MS_BOOL MSB201X_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8devID;
    MS_U8 u8DemodID;
    eDMD_MSB201X_LOCK_STATUS eLockStatus = DMD_DVBC_CHECKING;

    if(pstMSB201x == NULL)
        return FALSE;

    _msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);

    MDrv_DMD_MSB201X_GetLock(u8devID, u8DemodID, DMD_DVBC_GETLOCK_FEC_LOCK, &eLockStatus);
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


MS_BOOL MSB201X_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{

    MS_U8 u8devID;
    MS_U8 u8DemodID;
    MS_BOOL bret = TRUE;
    MS_U16 u16SymbolRate;
    eDMD_MSB201X_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq = _u32DefaultIFrequency;
    MS_BOOL bSpecInv = FALSE;
    MSB201X_CONFIG *pMSB201x = NULL;

    if(pstMSB201x == NULL)
        return FALSE;

    _msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);

    pMSB201x = pstMSB201x + u8devID;

   MDrv_DMD_MSB201X_I2C_BYPASS(u8devID, TRUE);
   if(!pMSB201x->MSB201x_InitParam[u8DemodID].pstTunertab->GetTunerIF(u8DemodIndex, &u32IFFreq))
    {
        u32IFFreq = _u32DefaultIFrequency;
        DMD_DBG(("MSB201X Get Tuner IF FAIL, use default Tuner IF %d kHz\n",(int)u32IFFreq));
    }
    else
    {
       DMD_DBG(("MSB201X Get Tuner IF %d kHz\n", (int)u32IFFreq));
    }
    MDrv_DMD_MSB201X_I2C_BYPASS(u8devID, FALSE);
    u16SymbolRate = pParam->CabParam.u16SymbolRate;

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

   bret &= MDrv_DMD_MSB201X_SetConfig(u8devID, u8DemodID, u16SymbolRate, eQamMode, u32IFFreq, bSpecInv);
   bret &= MDrv_DMD_MSB201X_SetActive(u8devID, u8DemodID, TRUE);

   return bret;
}

MS_BOOL MSB201X_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8devID;
    MS_U8 u8DemodID;
    MSB201X_CONFIG *pMSB201x = NULL;

    if(pstMSB201x == NULL)
        return FALSE;

    _msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);

    pMSB201x = pstMSB201x + u8devID;

    if (pMSB201x->bInited[u8DemodID] == FALSE)
    {
        if(!_Is_msb101x())
            u8devID = u8DemodID;

        bret &= _msb201x_I2C_CH_Reset(u8devID,3);
        bret &= _msb201x_SetReg(u8devID,0x100951, 0x00);
        if(bOn)
        {
            bret &= _msb201x_SetReg(u8devID,0x100910, 0x10);
            DMD_DBG(("set MSB201X I2C bypass ON\n"));
        }
        else
        {
            bret &= _msb201x_SetReg(u8devID,0x100910, 0x00);
            DMD_DBG(("set MSB201X I2C bypass OFF\n"));
        }
        return bret;
    }

    MDrv_DMD_MSB201X_I2C_BYPASS(u8devID, bOn);

    return TRUE;


}


#define MSB201X_CHIP_ID 0xb5
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
#define CFG_W_CMD 0x05
static MS_BOOL _msb201X_SSPI_CFG_W(MS_U8 u8_addr,MS_U8 data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Wdata[3];

    Wdata[0] = CFG_W_CMD;
    Wdata[1] = u8_addr;
    Wdata[2] = data;

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}

static MS_BOOL _msb201X_SSPI_RIU_READ8(MS_U16 u16Addr, MS_U8 *pdata)
{
   MS_BOOL bRet = TRUE;

   bRet&=_msb201X_SSPI_CFG_W(0x02, 0x10);
   bRet&=MDrv_DMD_SSPI_RIU_Read8(0x900, pdata);

   return bRet;
}
#endif

static MS_BOOL _msb201X_I2C_CH3_reset(MS_U8 u8DevIndex)
{
    //Reset twice to check if reset pin is correct
    _msb201X_hw_reset(u8DevIndex);
    if(!_msb201x_I2C_CH_Reset(u8DevIndex,3))
    {
        DMD_ERR(("[MSB201X] I2C_CH_Reset fail \n"));
        return FALSE;
    }
    else
    {
        _msb201X_hw_reset(u8DevIndex);
        if(!_msb201x_I2C_CH_Reset(u8DevIndex,3))
        {
            DMD_ERR(("[MSB201X] I2C_CH_Reset fail \n"));
            return FALSE;
        }
   }
   return TRUE;
}

static MS_BOOL _msb201X_Check_CHIP_ID(MS_U8 u8devID, MS_BOOL* pbGotID)
{
     MS_U8 u8_tmp = 0;
     MS_BOOL bret = TRUE;
     MSB201X_CONFIG *pMSB201x = NULL;

    if(NULL == pstMSB201x)
        return FALSE;
    else
        pMSB201x = pstMSB201x + u8devID;

    if(!_msb201X_I2C_CH3_reset(u8devID))
    {
         DMD_ERR(("[MSB201X] I2C_CH_Reset fail \n"));
         bret = FALSE;
    }
    else
    {
        if(_msb201x_GetReg(u8devID,0x100900,&u8_tmp))
        {
            DMD_DBG(("[MSB201X] read id :%x \n",u8_tmp ));
        }
        else
        {
            bret = FALSE;
        }
     }


     if(u8_tmp == MSB201X_CHIP_ID)
     {
        *pbGotID = TRUE;

        //check bonding
        if(_msb201x_GetReg(u8devID,0x100900 + (0x02)*2 + 1,&u8_tmp))
        {
            DMD_DBG(("[MSB201X] read bonding :%x \n",u8_tmp ));
            if(MSB1011_BONDING == (u8_tmp & MSB201X_BONDING_MASK))
            {
                GET_DEMOD_ENTRY_NODE(DEMOD_MSB201X).name = "DEMOD_MSB101X";
            }
        }
        else
        {
            bret = FALSE;
        }
#if defined(MSPI_PATH_DETECT) && (MSPI_PATH_DETECT == 1)
        MS_U8 i;
        //MSPI_DCConfig stDCConfig;
        //check TS path
        MDrv_DMD_SSPI_Init(0);
        MDrv_MasterSPI_CsPadConfig(0, 0xff);
        MDrv_MasterSPI_MaxClkConfig(0, 27);
        //stDCConfig.u8TB = 0;
        //stDCConfig.u8TrEnd = 0;
        //stDCConfig.u8TrStart = 0;
        //stDCConfig.u8TRW = 0x60;
        //MDrv_MSPI_DCConfig(&stDCConfig);
        _msb201x_SetReg2Bytes(u8devID,0x100900+(0x28)*2, 0x0000);
        _msb201x_SetReg2Bytes(u8devID,0x100900+(0x09)*2, 0x0101);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _msb201x_SetReg2Bytes(u8devID,0x100900+(0x3b)*2, 0x0002);

        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
         _msb201x_GetReg(u8devID,0x100900+(0x3a)*2+1,&u8_tmp);
         u8_tmp &= 0xFC;
         _msb201x_SetReg(u8devID,0x100900+(0x3a)*2+1, u8_tmp);

        // ------- MSPI driving setting
        _msb201x_SetReg2Bytes(u8devID,0x100900+(0x2c)*2, 0x07ff);


        for(i=0; i<MSPI_PATH_MAX; i++)
        {
            SPIPAD_EN[i](TRUE);
            // Note: Must read twice and ignore 1st data because pad switch noise
            // might cause wrong MSPI signal


            _msb201X_SSPI_RIU_READ8(0x900, &u8_tmp);
            _msb201X_SSPI_RIU_READ8(0x900, &u8_tmp);

            if(u8_tmp == MSB201X_CHIP_ID)
            {
                pMSB201x->u32DmxInputPath = (MS_U32)i;
                SPIPAD_EN[i](FALSE);
                DMD_DBG(("Get MSB201X chip ID by MSPI on TS%d\n", (int)pMSB201x->u32DmxInputPath));
                break;
            }
            else
            {
                DMD_DBG(("Cannot get MSB201X chip ID by MSPI on TS%x\n", i));
                if( i == (MSPI_PATH_MAX - 1))
                    pMSB201x->u32DmxInputPath = MSPI_PATH_NONE;
            }
            SPIPAD_EN[i](FALSE);
        }

            // ------disable to use TS_PAD as SSPI_PAD after load code
            // [0:0] reg_en_sspi_pad
            // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
            _msb201x_SetReg2Bytes(u8devID, 0x100900 + (0x3b) * 2, 0x0001);
#else
             pMSB201x->u32DmxInputPath = 0;//0xff;  //for compile warning
#endif //MSPI_PATH_DETECT
     }
     else
        *pbGotID = FALSE;

     return bret;

}



MS_BOOL MSB201X_DemodCheckExist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8devID,i;
    MS_BOOL bGotID = FALSE, bret = TRUE;
    MS_IIC_PORT ePort;
    MSB201X_CONFIG *pMSB201x = NULL;
    SLAVE_ID_USAGE *pMSB201x_ID_TBL;
    MS_U8 u8I2C_Port = 0;
    
    u8devID = u8DemodIndex; // here u8DemodIndex means device index
    bret &= _msb201x_Variables_alloc();
    if(!bret)
    {
        _msb201x_Variables_free();
        return FALSE;
    }
    else
    {
        ePort = getI2CPort(u8devID);
        pMSB201x = (pstMSB201x + u8devID);
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
        pMSB201x_ID_TBL = pstMSB201X_slave_ID_TBL + u8I2C_Port*sizeof(MSB201X_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        if(pMSB201x_ID_TBL->u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSB201X] All Slave ID have tried but not detect\n"));
            break;
        }

        if(pMSB201x_ID_TBL->bInUse)
        {
            DMD_DBG(("[MSB201X] Slave ID 0x%x have been used\n", pMSB201x_ID_TBL->u8SlaveID));
            i++;
            continue;
        }
        else
        {
            pMSB201x->u8SlaveID = pMSB201x_ID_TBL->u8SlaveID;
            DMD_DBG(("[MSB201X] Try slave ID 0x%x\n",pMSB201x_ID_TBL->u8SlaveID));
        }

        bret = _msb201X_Check_CHIP_ID(u8devID, &bGotID);
        if(bret && bGotID)
        {
            pMSB201x_ID_TBL->bInUse = TRUE;
            break;
        }
        i++;
    }while((pMSB201x_ID_TBL->u8SlaveID) != 0xFF);



    if(bret && bGotID)
    {
        *pu8SlaveID = pMSB201x->u8SlaveID;
         TUNER_DBG(("== MSB201X detected \n"));
         u8MSB201X_Dev_Cnt++;
    }
    else
    {
      if(u8MSB201X_Dev_Cnt == 0)
           _msb201x_Variables_free();

       bret = FALSE;
    }

    return bret;
}



MS_BOOL MSB201X_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret=TRUE;
    MS_U8 u8devID = u8DemodIndex/MSB201X_CHANNEL_COUNT, i;
    MSB201X_CONFIG *pMSB201x = NULL;

    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_FINALIZE:
            if(NULL == pstMSB201x)
                return FALSE;
            u8devID = u8DemodIndex; // here means device ID
            pMSB201x = (pstMSB201x + u8devID);
            if(pMSB201x->s32_MSB201x_Mutex >= 0)
            {
                bret &= MsOS_DeleteMutex(pMSB201x->s32_MSB201x_Mutex);
                pMSB201x->s32_MSB201x_Mutex = -1;
            }

            for(i=0; i< MSB201X_CHANNEL_COUNT; i++)
            {
               if(pMSB201x->bInited[i])
               {
                 /*After Demod scaned and susppend ,clean utopia demod structure memeber "bDemodRest" especially.
                 Utopia demod use reset flag to set demod password*/
                 MDrv_DMD_MSB201X_Exit(i);
                 pMSB201x->bInited[i] = FALSE;
              }
            }

            pMSB201x->bRest = TRUE;
            break;

        case DEMOD_EXT_FUNC_SET_RESET_PIN:
            u8devID = u8DemodIndex; // here means device ID
            if(!_msb201x_Variables_alloc())
            {
                _msb201x_Variables_free();
                return FALSE;
            }
            else
            {
                pMSB201x = (pstMSB201x + u8devID);
                pMSB201x->RstPin= *(int*)data;
            }
            break;
         default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
     }
   return bret;
}

MS_BOOL MSB201X_Demod_GetPacketError(MS_U8 u8DemodIndex,MS_U16 *u16PktErr)
{
     MS_BOOL bret = TRUE;
     MS_U8 u8devID;
     MS_U8 u8DemodID;

     _msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);

     bret &= MDrv_DMD_MSB201X_GetPacketErr(u8devID,u8DemodID, u16PktErr);

    return bret;

}

MS_BOOL MSB201X_Demod_GetSNR(MS_U8 u8DemodIndex,float *pfSNR)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8devID;
	MS_U8 u8DemodID;
	eDMD_MSB201X_LOCK_STATUS eLockStatus = DMD_DVBC_NULL;
	 
	_msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);
	MDrv_DMD_MSB201X_GetLock(u8devID, u8DemodID, DMD_DVBC_GETLOCK_FEC_LOCK, &eLockStatus);
	if(eLockStatus != DMD_DVBC_LOCK)
	{
		bret = FALSE;
		return bret;
	 }

    bret &= MDrv_DMD_MSB201X_GetSNR(u8devID,u8DemodID, pfSNR);;

    return bret;
}

MS_BOOL MSB201X_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8devID;
    MS_U8 u8DemodID;
	eDMD_MSB201X_LOCK_STATUS eLockStatus = DMD_DVBC_NULL;

	_msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);
	MDrv_DMD_MSB201X_GetLock(u8devID, u8DemodID, DMD_DVBC_GETLOCK_FEC_LOCK, &eLockStatus);
    if(eLockStatus != DMD_DVBC_LOCK)
    {
       bret = FALSE;
	   return bret;
    }
  
    bret &= MDrv_DMD_MSB201X_GetPostViterbiBer(u8devID,u8DemodID, pfBER);;

    return bret;
}

MS_BOOL MSB201X_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL bret = TRUE;
    MS_U8 u8devID;
    MS_U8 u8DemodID;
	eDMD_MSB201X_LOCK_STATUS eLockStatus = DMD_DVBC_NULL;
	float fRFPowerDbmResult;
	int RFPowerDbm;
	MSB201X_CONFIG *pMSB201x = NULL;
	 
	if(pstMSB201x == NULL)
		return FALSE;
	 
	_msb201x_get_IDs(u8DemodIndex, &u8devID, &u8DemodID);
	 
	pMSB201x = pstMSB201x + u8devID;
	 
	MDrv_DMD_MSB201X_GetLock(u8devID, u8DemodID, DMD_DVBC_GETLOCK_FEC_LOCK, &eLockStatus);
	if(eLockStatus != DMD_DVBC_LOCK)
	{
	    bret = FALSE;
		return bret;
	}

	MSB201X_Demod_I2C_ByPass(u8DemodIndex, TRUE);
    bret &= pMSB201x->MSB201x_InitParam[u8DemodID].pstTunertab->Extension_Function(u8DemodIndex, TUNER_EXT_FUNC_GET_POWER_LEVEL, &RFPowerDbm);
    MSB201X_Demod_I2C_ByPass(u8DemodIndex, FALSE);
	bret &= MDrv_DMD_MSB201X_GetRFLevel(u8devID, u8DemodID, &fRFPowerDbmResult, (float)RFPowerDbm);
    if(bret)
    {
      *ps32Signal = (MS_S32)fRFPowerDbmResult; 
	}
	else
	{
      *ps32Signal = 0;
	}
	
    return bret;
}



#ifdef FE_AUTO_TEST
MS_BOOL  MSB201X_Demod_ReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    MS_U8 u8devID = u8DemodIndex/MSB201X_CHANNEL_COUNT;
    MS_BOOL bret = TRUE;

    bret = MDrv_DMD_MSB201X_GetReg(u8devID, RegAddr, pu8Data);
    return bret;
}

MS_BOOL MSB201X_Demod_WriteReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U16 RegData)
{
    MS_U8 u8devID = u8DemodIndex/MSB201X_CHANNEL_COUNT;
    MS_BOOL bret = TRUE;
    MS_U8 u8RegData;

    u8RegData = (MS_U8)RegData;
    bret = MDrv_DMD_MSB201X_SetReg(u8devID, RegAddr, u8RegData);
    return bret;
}
#endif




DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB201X) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSB201X",
     .data                         = DEMOD_MSB201X,
     .SupportINT                   = FALSE,
     .init                         = MSB201X_Demod_Init,
     .GetLock                      = MSB201X_Demod_GetLock,
     .GetSNR                       = MSB201X_Demod_GetSNR,
     .GetBER                       = MSB201X_Demod_GetBER,
     .GetPWR                       = MSB201X_Demod_GetPWR,
     .GetSSI                       = MDrv_Demod_null_GetSSI,
     .GetQuality                   = MDrv_Demod_null_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MSB201X_Demod_Restart,
     .I2CByPass                    = MSB201X_Demod_I2C_ByPass,
     .CheckExist                   = MSB201X_DemodCheckExist,
     .Extension_Function           = MSB201X_Extension_Function,
     .Get_Packet_Error             = MSB201X_Demod_GetPacketError,
 #ifdef FE_AUTO_TEST
     .ReadReg                      = MSB201X_Demod_ReadReg,
     .WriteReg                     = MSB201X_Demod_WriteReg,
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
