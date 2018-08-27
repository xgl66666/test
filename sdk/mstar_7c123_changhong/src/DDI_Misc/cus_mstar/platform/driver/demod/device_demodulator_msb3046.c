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
// 6. Nothing contained herein shall be consTRUEd as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and consTRUEd in accordance with the laws
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
/**********************************************************************
Copyright (c) 2006-2009 MStar Semiconductor, Inc.
All rights reserved.
Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
MStar Semiconductor Inc. and be kept in strict confidence
(MStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of MStar Confidential
Information is unlawful and strictly prohibited. MStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
 **********************************************************************/

#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "MsOS.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvGPIO.h"
#include "drvSYS.h"
#include <string.h>
#define USE_UTOPIA

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB3046)
#include "device_demodulator_msb3046.h"

static MS_U8      u8max_dev_num = 0;

static MS_BOOL* pDemodRest = NULL;


MSB3046_Handle* pstMSB3046_Handle = NULL;

MSB3046_Handle MSB3046_Init =
{
    FALSE, //bInited
    FALSE, //bOpen
    -1,    //s32_MSB1245_Mutex
    0,     //u8sramCode
    0,     //u32CurrFreq
    FALSE, //bFECLock
    0,     //u8ScanStatus
    {NULL},//MSB1245_InitParam
    0xF2,  //u8SlaveID
    FALSE, //bIsDVBS2
    //-1,    //s32DemodHandle
    0,     //u32DmxInputPath;
    //FALSE, //bIsMCP_DMD
    FALSE, //bDiSeqc_Tx22K_Off
};


SLAVE_ID_USAGE* pstMSB3046_slave_ID_TBL = NULL;
static SLAVE_ID_USAGE MSB3046_possible_slave_ID[3] =
{
    {0xF2, FALSE},
    {0xD2, FALSE},
    {0xFF, FALSE}
};

static MS_U8 u8MSB3046_DevCnt = 0;

static MS_BOOL get_demod_handle(MS_U8 u8DemodIndex, MSB3046_Handle** ppHandle)
{
    if((pstMSB3046_Handle == NULL) || (u8DemodIndex > u8max_dev_num))
    {
        TUNER_ERR(("[MSB3046] Get Handle FAIL\n"));
        return FALSE;
    }
	
    *ppHandle = pstMSB3046_Handle + u8DemodIndex;
    return TRUE;
}

static MS_BOOL get_i2c_port(MS_U8 u8DemodIndex, MS_IIC_PORT* ePort)
{
    MSB3046_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
    else
    {
        *ePort = pstHandle->MSB3046_InitParam.stDMDCon.eI2C_PORT;
        return TRUE;
    }
}



static MS_BOOL MSB3046_Variables_alloc(void)
{
    MS_U8 i,j;
    MSB3046_Handle *pstHandle = NULL;
    SLAVE_ID_USAGE* pSlaveIDTBL = NULL;
    MS_U8 u8MaxI2CPort;

    u8MaxI2CPort = (MS_U8)((E_MS_IIC_SW_PORT_0/8) + (E_MS_IIC_PORT_NOSUP - E_MS_IIC_SW_PORT_0));

    if(NULL == pstMSB3046_Handle)
    {
        pstMSB3046_Handle = (MSB3046_Handle *)malloc(sizeof(MSB3046_Handle) * u8max_dev_num);
        if(NULL == pstMSB3046_Handle)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
            {
                pstHandle = (pstMSB3046_Handle + i);
				memcpy(pstHandle, &MSB3046_Init, sizeof(MSB3046_Handle));
                pstHandle->MSB3046_InitParam.stDMDCon.u32TSVLDInterrupt = DMD_CON_INFO_NOT_DEFINE;
            }
        }
    }

    if(NULL == pDemodRest)
    {
        pDemodRest = (MS_BOOL*)malloc(sizeof(MS_BOOL) * u8max_dev_num);
        if(NULL == pDemodRest)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8max_dev_num; i++)
                *(pDemodRest + i) = TRUE;
        }
    }


    if(NULL == pstMSB3046_slave_ID_TBL)
    {
        pstMSB3046_slave_ID_TBL = (SLAVE_ID_USAGE *)malloc(sizeof(MSB3046_possible_slave_ID) * u8MaxI2CPort);
        if(NULL == pstMSB3046_slave_ID_TBL)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< u8MaxI2CPort; i++)
            {
                for(j=0; j< (sizeof(MSB3046_possible_slave_ID)/sizeof(SLAVE_ID_USAGE)); j++)
                {
                    pSlaveIDTBL = (pstMSB3046_slave_ID_TBL + i*sizeof(MSB3046_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + j);
                    memcpy(pSlaveIDTBL, &MSB3046_possible_slave_ID[j], sizeof(SLAVE_ID_USAGE));
                 }
            }

        }
    }
     
    return TRUE;

}

static void _variable_free(void** pp)
{
    if(NULL != *pp)
    {
        free(*pp);
        *pp = NULL;
    }
}

static MS_BOOL MSB3046_Variables_free(void)
{
    _variable_free((void*)&pstMSB3046_Handle);
    _variable_free((void*)&pDemodRest);
    _variable_free((void*)&pstMSB3046_slave_ID_TBL);
    return TRUE;
}

static void MSB3046_hw_reset(MS_U8 u8DemodIndex)
{
    int rstPin;
    MSB3046_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return;


    rstPin = (int)(pstHandle->MSB3046_InitParam.stDMDCon.u32HW_ResetPin);
    mdrv_gpio_set_high(rstPin);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(rstPin);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(rstPin);
    MsOS_DelayTask(5);

    pstHandle->bInited = FALSE;
    pstHandle->bOpen = FALSE;
    *(pDemodRest + u8DemodIndex) = TRUE;

}

/*================================================
==                       IIC write/read interface
=================================================*/
MS_BOOL MSB3046_WriteBytes(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MS_IIC_PORT ePort;
	MSB3046_Handle* pstHandle = NULL;
	MS_U8 i;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
    
    get_i2c_port(u8DemodIndex, &ePort);
    bRet = MDrv_IIC_WriteBytes(ePort, (MS_U16)pstHandle->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);
    if (FALSE == bRet)
    {
        DMD_ERR(("[MSB3046] IIC write error, ePort = %x, ID = 0x%x\n", (int)ePort, pstHandle->u8SlaveID));
        for(i=0;i<u16Size;i++)
        {
           printf("%x data = 0x%x\n", i, *(pu8Data + i));
		}
    }
    return bRet;
}

MS_BOOL MSB3046_ReadBytes(MS_U8 u8DemodIndex, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    MS_IIC_PORT ePort;
    MSB3046_Handle* pstHandle = NULL;
	
    if(!get_demod_handle(u8DemodIndex, &pstHandle))
        return FALSE;
    
    get_i2c_port(u8DemodIndex, &ePort);
    bRet = MDrv_IIC_ReadBytes(ePort, (MS_U16)pstHandle->u8SlaveID, u8AddrSize, pu8Addr, u16Size, pu8Data);

    if (FALSE == bRet)
    {
        DMD_ERR(("[MSB3046] IIC read error\n"));
    }
    return bRet;
}


/*================================================
==               demod  MSB3046  base interface
=================================================*/

MS_BOOL MSB3046_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSB3046][beg]MSB3046_I2C_CH_Reset, CH=0x%x\n", ch_num));

    if(pDemodRest == NULL)
        return FALSE;

    if (*(pDemodRest + u8DemodIndex))
    {
        printf("@@@@ CH reset set password\n");
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        if(MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 5, data))
            *(pDemodRest + u8DemodIndex) = FALSE;
        else
            return FALSE;
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, data);

    DMD_DBG(("[MSB3046][end]MSB3046_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}

MS_BOOL MSB3046_GetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 5, u8MsbData);
    bRet &= MSB3046_ReadBytes(u8DemodIndex, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    return bRet;
}

MS_BOOL MSB3046_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &= MSB3046_WriteBytes(u8DemodIndex, 0, NULL, 1, u8MsbData);
    return bRet;
}


/********************************************
Function: To get chip version
Parameter: pu8Chip   0x01->MSB1230, 0x02->MSB124X,
                    0xff ->unknown
*********************************************/

MS_BOOL MSB3046_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MS_BOOL bret = TRUE;

    //if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {

        bret &= MSB3046_I2C_CH_Reset(u8DemodIndex,3);
        bret &= MSB3046_SetReg(u8DemodIndex,0x0951, 0x00);
        bret &= MSB3046_SetReg(u8DemodIndex,0x090C, 0x10);
        bret &= MSB3046_SetReg(u8DemodIndex,0x090E, 0x10);
        if(bOn)
        {
            DMD_DBG(("set MSB3046 I2C bypass ON\n"));
            bret &= MSB3046_SetReg(u8DemodIndex,0x0910, 0x10);
        }
        else
        {
            DMD_DBG(("set MSB3046 I2C bypass OFF\n"));
            bret &= MSB3046_SetReg(u8DemodIndex,0x0910, 0x00);
        }
        return bret;
    }

    return TRUE;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB3046_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{
    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB3046_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL status = TRUE;
    return status;
}

MS_BOOL MSB3046_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MS_BOOL bret = TRUE;
    return bret;
}

MS_BOOL MSB3046_Demod_GetSSI(MS_U8 u8DemodIndex, MS_U16 *pu16SSI)
{
    MS_BOOL bret = TRUE;
    return bret;
}


MS_BOOL MSB3046_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{
    return TRUE;
}

MS_BOOL MSB3046_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{
    *peLockStatus = E_DEMOD_CHECKING;

    return TRUE;
}
MS_BOOL MSB3046_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
    MS_BOOL bRet = TRUE;

    return bRet;
}


MS_BOOL MSB3046_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    MS_BOOL     status = TRUE;
  
    return status;
}

#define MSB3046_CHIP_ID 0xC3
static MS_BOOL _msb3046_I2C_CH3_reset(MS_U8 u8DemodIndex)
{
    //Reset twice to check if reset pin is correct
    MSB3046_hw_reset(u8DemodIndex);
    if(!MSB3046_I2C_CH_Reset(u8DemodIndex,3))
    {
        DMD_ERR(("[MSB3046] I2C_CH_Reset fail \n"));
        return FALSE;
    }
    else
    {
        MSB3046_hw_reset(u8DemodIndex);
        if(!MSB3046_I2C_CH_Reset(u8DemodIndex,3))
        {
            DMD_ERR(("[MSB3046] I2C_CH_Reset fail \n"));
            return FALSE;
        }
   }
   return TRUE;
}

MS_BOOL MSB3046_Check_Exist(MS_U8 u8DemodIndex, MS_U8* pu8SlaveID)
{
    MS_U8 u8_tmp = 0;
    MS_U8 i, u8I2C_Port = 0;
    SLAVE_ID_USAGE *pMSB3046_ID_TBL;
    MS_IIC_PORT ePort = 0;
	MSB3046_Handle* pstHandle = NULL;

    if(!MSB3046_Variables_alloc())
    {
        MSB3046_Variables_free();
        return FALSE;
    }
    else
    {
       if(!get_demod_handle(u8DemodIndex, &pstHandle))
	       return FALSE;
    }

    get_i2c_port(u8DemodIndex, &ePort);
    if((int)ePort < (int)E_MS_IIC_SW_PORT_0)
    {
       u8I2C_Port = (MS_U8)ePort/8;
    }
    else if((int)ePort < (int)E_MS_IIC_PORT_NOSUP)//sw i2c
    {
       u8I2C_Port = E_MS_IIC_SW_PORT_0/8 + (ePort - E_MS_IIC_SW_PORT_0);
    }

    i=0;
    do
    {
        pMSB3046_ID_TBL = pstMSB3046_slave_ID_TBL + u8I2C_Port*sizeof(MSB3046_possible_slave_ID)/sizeof(SLAVE_ID_USAGE) + i;
        DMD_DBG(("### %x\n",i));
        if(pMSB3046_ID_TBL->u8SlaveID == 0xFF)
        {
            DMD_DBG(("[MSB3046] All Slave ID have tried but not detect\n"));
            break;
        }

        if(pMSB3046_ID_TBL->bInUse)
        {
            DMD_DBG(("[MSB3046] Slave ID 0x%x have been used\n", pMSB3046_ID_TBL->u8SlaveID));
            i++;
            continue;
        }
        else
        {
            pstHandle->u8SlaveID = pMSB3046_ID_TBL->u8SlaveID;
            DMD_DBG(("[MSB3046] Try slave ID 0x%x\n",pMSB3046_ID_TBL->u8SlaveID));
        }

        if(_msb3046_I2C_CH3_reset(u8DemodIndex))
        {
            DMD_DBG(("[MSB3046] I2C slave id :%x \n",pstHandle->u8SlaveID ));
            if(MSB3046_GetReg(u8DemodIndex,0x0900,&u8_tmp))
            {
                DMD_DBG(("[MSB3046] read id :%x \n",u8_tmp ));
                if(u8_tmp == MSB3046_CHIP_ID)
                {
                    pMSB3046_ID_TBL->bInUse = TRUE;
                    break;
                }
            }
        }

        i++;
    }while((pMSB3046_ID_TBL->u8SlaveID) != 0xFF);


    if(u8_tmp == MSB3046_CHIP_ID)
    {
        u8MSB3046_DevCnt++;
        *pu8SlaveID = pstHandle->u8SlaveID;        
        return TRUE;
    }
    else
    {        
        if(!u8MSB3046_DevCnt)
            MSB3046_Variables_free();
        return FALSE;
    }

}

MS_BOOL MSB3046_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;
	MSB3046_Handle* pstHandle = NULL;
    DEMOD_EXT_FUNCTION_PARAM* pstData;
    DEMOD_CON_CONFIG* pstCon;

    switch(fuction_type)
    {
      case DEMOD_EXT_FUNC_FINALIZE:
          //if(!get_demod_handle(u8DemodIndex, &pstHandle))
	      //     return FALSE;

        //if(pstHandle->s32_MSB3046_Mutex >= 0)
        //{
        //    bret &= MsOS_DeleteMutex(pstHandle->s32_MSB3046_Mutex);
        //    pstHandle->s32_MSB3046_Mutex = -1;
        //}
        MSB3046_hw_reset(u8DemodIndex);//for I2C re-write password issue
        break;
		
    case DEMOD_EXT_FUNC_SET_CON_INFO:
        pstData = (DEMOD_EXT_FUNCTION_PARAM*)data;
        u8max_dev_num = pstData->u32Param1;
        pstCon = (DEMOD_CON_CONFIG*)(pstData->pParam);
        if(!MSB3046_Variables_alloc())
        {
            MSB3046_Variables_free();
            return FALSE;
        }
        else
        {
            if(!get_demod_handle(u8DemodIndex, &pstHandle))
	            return FALSE;

            memcpy(&pstHandle->MSB3046_InitParam.stDMDCon, pstCon,sizeof(DEMOD_CON_CONFIG));
        }
        break;
    default:
        DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
        break;
    }
    return bret;
}
#if MS_DVBS_INUSE

MS_BOOL MSB3046_DiSEqC_Init(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex,MS_BOOL bLow)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}
MS_BOOL MSB3046_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex,MS_BOOL* bLNBOutLow)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}
MS_BOOL MSB3046_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex,MS_BOOL b22kOn)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}
MS_BOOL MSB3046_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex,MS_BOOL* b22kOn)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_DiSEqC_SendCmd(MS_U8 u8DemodIndex,MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_DiSEqC_SetTone(MS_U8 u8DemodIndex,MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_Start(MS_U8 u8DemodIndex, MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_NextFreq(MS_U8 u8DemodIndex, MS_BOOL* bBlindScanEnd)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_WaitCurFreqFinished(MS_U8 u8DemodIndex, MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_Cancel(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_End(MS_U8 u8DemodIndex)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_GetChannel(MS_U8 u8DemodIndex, MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}

MS_BOOL MSB3046_Demod_BlindScan_GetCurrentFreq(MS_U8 u8DemodIndex, MS_U32 *u32CurrentFeq)
{
    MS_BOOL bRet=TRUE;
    return bRet;
}
#endif

MS_BOOL MSB3046_Demod_GetParam(MS_U8 u8DemodIndex,DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bRet=TRUE;

    return bRet;
}

#ifdef FE_AUTO_TEST
MS_BOOL  MSB3046_Demod_AutoTestReadReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    //*pu8Data = (MS_U8)MSB1245_Demod_ReadReg(u8DemodIndex, RegAddr);
    return TRUE;
}

MS_BOOL MSB3046_Demod_AutoTestWriteReg(MS_U8 u8DemodIndex, MS_U16 RegAddr, MS_U16 RegData)
{
    return TRUE;//MSB1245_Demod_WriteReg(u8DemodIndex, RegAddr, RegData);
}
#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB3046) DDI_DRV_TABLE_ENTRY(demodtab) =
{
    .name                         = "DEMOD_MSB3046",
    .data                         = DEMOD_MSB3046,
    .SupportINT                   = FALSE,
    .init                         = MSB3046_Demod_Init,
    .GetLock                      = MSB3046_Demod_GetLock,
    .GetSNR                       = MSB3046_Demod_GetSNR,
    .GetBER                       = MSB3046_Demod_GetBER,
    .GetPWR                       = MSB3046_Demod_GetPWR,
    .GetSSI                       = MSB3046_Demod_GetSSI,
    .GetQuality                   = MSB3046_Demod_GetSignalQuality,
    .GetParam                     = MSB3046_Demod_GetParam,
    .Restart                      = MSB3046_Demod_Restart,
    .I2CByPass                    = MSB3046_Demod_I2C_ByPass,
    .I2CByPassPreSetting          = NULL,
    .CheckExist                   = MSB3046_Check_Exist,
    .Extension_Function           = MSB3046_Extension_Function,
    .Extension_FunctionPreSetting = NULL,
    .Get_Packet_Error             = MDrv_Demod_null_Get_Packet_Error,
#ifdef FE_AUTO_TEST
     .ReadReg                     = MSB3046_Demod_AutoTestReadReg,
     .WriteReg                    = MSB3046_Demod_AutoTestWriteReg,
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
    .BlindScanStart               = MSB3046_Demod_BlindScan_Start,
    .BlindScanNextFreq            = MSB3046_Demod_BlindScan_NextFreq,
    .BlindScanWaitCurFreqFinished = MSB3046_Demod_BlindScan_WaitCurFreqFinished,
    .BlindScanCancel              = MSB3046_Demod_BlindScan_Cancel,
    .BlindScanEnd                 = MSB3046_Demod_BlindScan_End,
    .BlindScanGetChannel          = MSB3046_Demod_BlindScan_GetChannel,
    .BlindScanGetCurrentFreq      = MSB3046_Demod_BlindScan_GetCurrentFreq,
    .DiSEqCSetTone                = MSB3046_DiSEqC_SetTone,
    .DiSEqCSetLNBOut              = MSB3046_DiSEqC_SetLNBOut,
    .DiSEqCGetLNBOut              = MSB3046_DiSEqC_GetLNBOut,
    .DiSEqCSet22kOnOff            = MSB3046_DiSEqC_Set22kOnOff,
    .DiSEqCGet22kOnOff            = MSB3046_DiSEqC_Get22kOnOff,
    .DiSEqC_SendCmd               = MSB3046_DiSEqC_SendCmd,
    .DiSEqC_GetReply              = MDrv_Demod_null_DiSEqC_GetReply,
    .GetISIDInfo                  = MDrv_Demod_null_GetVCM_ISID_INFO,
    .SetISID                      = MDrv_Demod_null_SetVCM_ISID
#endif
};

#endif

