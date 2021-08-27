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
#include "Board.h"
#include "drvTuner.h"
#include "drvDemod.h"
#include "drvDemodNull.h"
#include "drvGPIO.h"
#include <string.h>
#define USE_UTOPIA

#if IF_THIS_DEMOD_INUSE(DEMOD_MSB1245)
#include "drvDMD_EXTERN_MSB124x.h"
#include "device_demodulator_msb124x.h"

//DVBS
#define MSB1245_DEMOD_WAIT_TIMEOUT    (6000)
#define MSB1245_TUNER_WAIT_TIMEOUT    (50)

#define MSB1245_TS_INV                  1
#define MSB1245_TS_DATA_SWAP            (0)
#define MSB1245_ADCPLL_IQ_SWAP         1
#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define TIMING_VERIFICATION              0
#define MSB1245_SELECT_IF_INPUT     1  // 0:dvbt_I, 1:dvbs_I
#define MSB1245_SELECT_AGC_PAD_T    0


#if MSB1245_TS_DATA_SWAP //for DVBS
static MS_BOOL    _bTSDataSwap=FALSE;
#endif
static  MS_BOOL   _bIsDVBS2=FALSE;//DVBS ot DVBS2

static DEMOD_MS_INIT_PARAM          MSB1245_InitParam;
static MS_U8 u8Possible_SLAVE_IDs[2] = {0xF2, 0xD2};
static MS_U8 DEMOD_MSB1245_SLAVE_ID = 0xF2;

#if(TIMING_VERIFICATION == 1)
static MS_U32 tmm_1 = 0x00;
static MS_U32 tmm_2 = 0x00;
static MS_U32 tmm_3 = 0x00;
static MS_U32 tmm_4 = 0x00;
static MS_U32 tmm_5 = 0x00;
static MS_U32 tmm_6 = 0x00;
static MS_U32 tmm_7 = 0x00;
static MS_U32 tmm_8 = 0x00;
static MS_U32 tmm_9 = 0x00;
static MS_U32 tmm_10 = 0x00;
static MS_U32 tmm_11 = 0x00;
static MS_U32 tmm_12 = 0x00;
static MS_U32 tmm_13 = 0x00;
static MS_U32 tmm_14 = 0x00;
static MS_U32 tmm_15 = 0x00;
static MS_U32 tmm_16 = 0x00;
static MS_U32 tmm_17 = 0x00;
static MS_U32 tmm_18 = 0x00;
static MS_U32 tmm_19 = 0x00;
static MS_U32 tmm_20 = 0x00;

#define GIVE_ME_TIME MsOS_GetSystemTime();
#endif

#define IQ_SWAP                  FRONTEND_DEMOD_IQ_SWAP
#if TS_PARALLEL_OUTPUT
#define SERIAL_TS               0x00
#else
#define SERIAL_TS               0x01
#endif
#define TS_CLK_SEL           0x00
#define TS_DATA_SWAP       0x00
#define TS_OUT_INV             TS_CLK_INV

static MS_BOOL DemodRest = TRUE;

typedef struct
{
    MS_BOOL    bInited;
    MS_BOOL    bOpen;
    MS_S32     s32_MSB1245_Mutex;
    MS_U8      u8sramCode;
    MS_U32     u32CurrFreq;
    MS_BOOL    bFECLock;
    MS_U8      u8ScanStatus;
} MDvr_CofdmDmd_CONFIG;
MDvr_CofdmDmd_CONFIG MSB1245;

static MS_BOOL _MSB1245_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data);
MS_BOOL MSB1245_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num);


#if(TIMING_VERIFICATION == 1)
static void show_timer(void)
{
    DMD_DBG(("***************************\n"));
    DMD_DBG(("[tmm1]t2-t1 = %ld (%ld - %ld)\n", tmm_2 - tmm_1, tmm_2, tmm_1));
    DMD_DBG(("[tmm2]t4-t3 = %ld (%ld - %ld)\n", tmm_4 - tmm_3, tmm_4, tmm_3));
    DMD_DBG(("[tmm3]t6-t5 = %ld (%ld - %ld)\n", tmm_6 - tmm_5, tmm_6, tmm_5));
    DMD_DBG(("[tmm4]t8-t7 = %ld (%ld - %ld)\n", tmm_8 - tmm_7, tmm_8, tmm_7));
    DMD_DBG(("[tmm5]t10-t9 = %ld (%ld - %ld)\n", tmm_10 - tmm_9, tmm_10, tmm_9));
    DMD_DBG(("[tmm6]t12-t11 = %ld (%ld - %ld)\n", tmm_12 - tmm_11, tmm_12, tmm_11));
    DMD_DBG(("[tmm7]t14-t13 = %ld (%ld - %ld)\n", tmm_14 - tmm_13, tmm_14, tmm_13));
    DMD_DBG(("[tmm8]t16-t15 = %ld (%ld - %ld)\n", tmm_16 - tmm_15, tmm_16, tmm_15));
    DMD_DBG(("[tmm9]t18-t17 = %ld (%ld - %ld)\n", tmm_18 - tmm_17, tmm_18, tmm_17));
    DMD_DBG(("[tmm10]t20-t19 = %ld (%ld - %ld)\n", tmm_20 - tmm_19, tmm_20, tmm_19));
    DMD_DBG(("[tmm11]lock_time# = %ld (%ld - %ld)\n", tmm_14 - u32StartTime, tmm_14, u32StartTime));
    DMD_DBG(("[tmm12]lock-setf = %ld (%ld - %ld)\n", tmm_14 - tmm_11, tmm_14, tmm_11));
    DMD_DBG(("[tmm13]lock-loaddsp = %ld (%ld - %ld)\n", tmm_14 - tmm_9, tmm_14, tmm_9));
    DMD_DBG(("***************************\n"));
}
#endif

#if MS_DVBS_INUSE
MS_BOOL MSB1245_DiSEqC_Init(void);
#endif

static float g_MSB1245_fSNR = 0.0;
static void _msb1245_hw_reset(MS_U8 u8DemodIndex)
{ 
  #ifdef GPIO_DEMOD_RST
     int rst_pin = 9999;
     rst_pin = GPIO_DEMOD_RST;
     mdrv_gpio_set_high(rst_pin);
     MsOS_DelayTask(100);
     mdrv_gpio_set_low(rst_pin);
     MsOS_DelayTask(200);
     mdrv_gpio_set_high(rst_pin);
     MsOS_DelayTask(100);    
  #endif
     MSB1245.bInited = FALSE;
     MSB1245.bOpen = FALSE;
     DemodRest = TRUE;
     MDrv_DMD_MSB124X_Exit();
 
}


/*================================================
==                       IIC write/read interface
=================================================*/
MS_U16 MSB1245_Demod_ReadReg(MS_U16 RegAddr);

MS_BOOL MSB1245_WriteBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    
    bRet = MDrv_IIC_Write(DEMOD_MSB1245_SLAVE_ID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error\n"));
    }
    return bRet;
}

MS_BOOL MSB1245_ReadBytes_demod(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;

    bRet = MDrv_IIC_Read(DEMOD_MSB1245_SLAVE_ID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}

MS_BOOL MSB1245_WriteBytes_demod1(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;
    
    bRet = MDrv_IIC1_Write(DEMOD_MSB1245_SLAVE_ID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC write error\n"));
    }
    return bRet;
}

MS_BOOL MSB1245_ReadBytes_demod1(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = 0;

    bRet = MDrv_IIC1_Read(DEMOD_MSB1245_SLAVE_ID, pu8Addr, u8AddrSize, pu8Data, u16Size);
    if (FALSE == bRet)
    {
        DMD_ERR(("Demod IIC read error\n"));
    }
    return bRet;
}

typedef struct
{
    MS_BOOL(*WriteBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_BOOL(*ReadBytes)(MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
} mapi_i2c;

mapi_i2c DemodI2Chandler, SpiI2Chandler;
static mapi_i2c* mapi_i2c_GetI2C_Dev(void)
{
    mapi_i2c *handler;

    handler = &DemodI2Chandler;
    handler->WriteBytes = MSB1245_WriteBytes_demod;
    handler->ReadBytes = MSB1245_ReadBytes_demod;
    return handler;
}

static mapi_i2c* mapi_i2c1_GetI2C_Dev(void)
{
    mapi_i2c *handler;

    handler = &DemodI2Chandler;
    handler->WriteBytes = MSB1245_WriteBytes_demod1;
    handler->ReadBytes = MSB1245_ReadBytes_demod1;
    return handler;
}


static MS_BOOL msb1245_I2C_Access(eDMD_MSB124X_DemodI2CSlaveID eSlaveID, eDMD_MSB124X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr;

    i2c_iptr = mapi_i2c_GetI2C_Dev();

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB124X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
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

static MS_BOOL msb1245_I2C1_Access(eDMD_MSB124X_DemodI2CSlaveID eSlaveID, eDMD_MSB124X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    mapi_i2c *i2c_iptr;

    i2c_iptr = mapi_i2c1_GetI2C_Dev();

    if (i2c_iptr != NULL)
    {
        switch (eMethod)
        {
        case E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES:
            bRet = i2c_iptr->WriteBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
            break;
        case E_DMD_MSB124X_DEMOD_I2C_READ_BYTES:
            bRet = i2c_iptr->ReadBytes(u8AddrSize, pu8Addr, u16Size, pu8Data);
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


/*================================================
==               demod  msb124x  write/read interface
=================================================*/
MS_BOOL MSB1245_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_MSB124X_SetReg(u16Addr, u8Data);
}

MS_BOOL MSB1245_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    return MDrv_DMD_MSB124X_SetRegs(u16Addr, u8pData, data_size);
}

MS_BOOL MSB1245_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    return MDrv_DMD_MSB124X_SetReg2Bytes(u16Addr, u16Data);
}

MS_BOOL MSB1245_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return MDrv_DMD_MSB124X_GetReg(u16Addr, pu8Data);
}

MS_BOOL  MSB1245_WriteDspReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return MDrv_DMD_MSB124X_SetDSPReg(u16Addr, u8Data);
}

MS_BOOL MSB1245_ReadDspReg(MS_U16 u16Addr, MS_U8* pData)
{
    return MDrv_DMD_MSB124X_GetDSPReg(u16Addr, pData);
}


/*================================================
==               demod  MSB124X  base interface
=================================================*/

MS_BOOL MSB1245_I2C_CH_Reset(MS_U8 u8DemodIndex, MS_U8 ch_num)
{
    MS_BOOL bRet = TRUE;
    MS_U8     data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    HWI2C_PORT hwi2c_port;
    mapi_i2c *iptr;
    
    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        iptr = mapi_i2c_GetI2C_Dev();
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        iptr = mapi_i2c1_GetI2C_Dev();
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }

    // ch_num, 0: for MIU access, 3: for RIU access
    DMD_DBG(("[MSB1245][beg]MSB1245_I2C_CH_Reset, CH=0x%x\n", ch_num));

    if (iptr == NULL)
    {
        return FALSE;
    }

    if (DemodRest)
    {
        DemodRest = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        iptr->WriteBytes(0, NULL, 5, data);
    }

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    data[0] = 0x71;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    data[0] = ((ch_num & 0x01) != 0) ? 0x81 : 0x80;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    data[0] = ((ch_num & 0x02) != 0) ? 0x83 : 0x82;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    data[0] = ((ch_num & 0x04) != 0) ? 0x85 : 0x84;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    data[0] = 0x53;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    data[0] = 0x7f;
    bRet &= iptr->WriteBytes(0, NULL, 1, data);

    /*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
    */
    DMD_DBG(("[MSB1245][end]MSB1245_I2C_CH_Reset, CH=0x%x\n", ch_num));
    return bRet;
}


static void MSB1245_Driving_Control(MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MS_U8 u8Temp;
    MSB1245_ReadReg(0x0958, &u8Temp);
    if (bEnable)
    {
        u8Temp = 0xFF;
    }
    else
    {
        u8Temp = 0x00;
    }
    MSB1245_WriteReg(0x0958, u8Temp);

    MSB1245_ReadReg(0x0959, &u8Temp);
    if (bEnable)
    {
        u8Temp = u8Temp | 0x0F;
    }
    else
    {
        u8Temp = u8Temp & (~0x0F);
    }
    MSB1245_WriteReg(0x0959, u8Temp);
}

static MS_BOOL MSB1245_DTV_Serial_Control(MS_BOOL bEnable)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return FALSE;
    }


    MSB1245_Driving_Control(1);
    return TRUE;
}


/*================================================
==               demod  MSB124X iic bypass interface
=================================================*/
MS_BOOL MSB1245_IIC_Bypass_Mode(MS_BOOL enable)
{
    MS_U8 u8Retry = 10;
    MS_BOOL bResult;

    DMD_DBG((">>>MSB124X: IIC_bypass() set %x\n", enable));

    while (u8Retry--)
    {
        if (enable)
            bResult = MSB1245_WriteReg(0x0900 + (0x08) * 2, 0x10); // IIC by-pass mode on
        else
            bResult = MSB1245_WriteReg(0x0900 + (0x08) * 2, 0x00); // IIC by-pass mode off

        if (bResult)
            break;
    }
    return bResult;
}

MS_BOOL dvbs2_MSB1245_TS_Enable(MS_BOOL bTsEnable)
{    
    MS_BOOL bRet=TRUE;    
    MS_U16  u16Address;    
    MS_U8   u8Data;    
    if (bTsEnable == TRUE)    
    {        
        bRet&= MSB1245_WriteReg(0x095A, 0xFF);    
    }    
    else    
    {        
        bRet&= MSB1245_WriteReg(0x095A, 0x00);    
    }    

    u16Address=0x0924;    
    bRet &= MSB1245_ReadReg(u16Address, &u8Data);
    #if MSB1245_TS_INV        
        u8Data |= (0x20); 
    #else    
        u8Data ~= (0x20); 
    #endif        
    bRet&=MSB1245_WriteReg(u16Address, u8Data);          
    return bRet;
}

MS_BOOL MSB1245_Lock_S(void)
{   
    MS_BOOL bRet=TRUE;  
    MS_U16 u16Address;  
    MS_U8 u8Data;   

    u16Address=0x0990;  
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x02)==0x00)    
    {  
        u16Address=0xB56;  
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        if((u8Data>=15) && (u8Data!=0xcd))  
        {   
            if (u8Data==15) 
            {   
               _bIsDVBS2=FALSE;   //S  
               bRet = TRUE; 
            }
            else if(u8Data==16)    
            {   
               _bIsDVBS2=TRUE;    //S2  
               bRet = TRUE; 
            }
            else
            {
               bRet = FALSE;
            }
         }
          else    
         {   
             bRet = FALSE;   
         }
 #if MSB1245_TS_DATA_SWAP   
          if (bRet==FALSE)  
          {   
               _bTSDataSwap=FALSE; 
            }   
            else    
            {   
               if (_bTSDataSwap==FALSE)    
               {   
                   _bTSDataSwap= TRUE;   
                   u16Address=0x2A40;  
                   bRet&=MSB1245_ReadReg(u16Address, &u8Data);   
                   u8Data^=0x20;   
                   bRet&=MSB1245_WriteReg(u16Address, u8Data);   
                }   
            }
#endif 
            }   
            else    
            {        
                 bRet = TRUE;   
            }    
            dvbs2_MSB1245_TS_Enable(1);    
            return bRet;
}


EN_LOCK_STATUS MSB1245_DTV_GetLockStatus(void)
{    

   if(MSB1245_Lock_S())
        return E_DEMOD_LOCK;
   else
        return E_DEMOD_UNLOCK;
}

void MSB1245_DTV_GetSNR(void)
{
    MS_U8   status = TRUE;
    float   f_snr = (float)0.0;
    //DVBS
    MS_U16 u16Address;
    MS_U32 u32Data;
    MS_U8  u8Data;
    float  fvariance;

    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return ;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return ;
    }


    u16Address=0x1B08;    
    status&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data|=0x10;
    status&=MSB1245_WriteReg(u16Address, u8Data);

    u16Address=0x1BA5;    
    status&=MSB1245_ReadReg(u16Address, &u8Data);
    u32Data=(u8Data&0x3F);

    u16Address=0x1BA4;    
    status&=MSB1245_ReadReg(u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;

    u16Address=0x1BA3;    
    status&=MSB1245_ReadReg(u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;

    u16Address=0x1BA2;    
    status&=MSB1245_ReadReg(u16Address, &u8Data);
    u32Data=(u32Data<<8)|u8Data;

    u16Address=0x1B08;    
    status&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data&=~(0x10);
    status&=MSB1245_WriteReg(u16Address, u8Data);
    if (status==FALSE)
    {
       f_snr = 0;
    }
    fvariance=(float)u32Data/0x20000000;
    if (fvariance==0)
         fvariance=1;
    f_snr=10*log10(1/fvariance);
    g_MSB1245_fSNR = f_snr;
}


MS_BOOL MSB1245_DTV_GetPostBER(float *p_postBer)
{
    float fber = 0;
    MS_U8 status = TRUE;
    MS_U16 u16Address;
    MS_U8  u8Data;
    MS_U16 u16BitErrPeriod;
    MS_U32 u32BitErr;

    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return FALSE;
    }

   if (_bIsDVBS2==FALSE)//S
   {
        u16Address=0x1c2C;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        if (status==FALSE)
        {
            DMD_DBG(("dvbs2_MSB124X_DTV_GetSignalQuality fail!!! \n"));
            return 0;
        }
        if ((u8Data&0x02)!=0x02)
        {
            return 0;
        }
        u16Address=0x1c32;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data|=0x80;
        status&=MSB1245_WriteReg(u16Address, u8Data);
        u16Address=0x1c31;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1c30;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
        u16Address=0x1c3D;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x1c3C;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c3B;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c3A;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1c32;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data&=~(0x80);
        status&=MSB1245_WriteReg(u16Address, u8Data);
        if (u16BitErrPeriod == 0)
            u16BitErrPeriod = 1;
        if (u32BitErr <= 0)
            fber = 0.5 / ((float)u16BitErrPeriod *128 * 188 * 8);
        else
            fber = (float)u32BitErr / ((float)u16BitErrPeriod *128 * 188 * 8);
        
        *p_postBer = fber;
    }
    else    //S2
    {
        u16Address=0x1d04;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data|=0x01;
        status&=MSB1245_WriteReg(u16Address, u8Data);
        u16Address=0x1d25;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=u8Data;
        u16Address=0x1d24;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u16BitErrPeriod=(u16BitErrPeriod<<8)|u8Data;
    
        u16Address=0x1d5b;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=u8Data;
        u16Address=0x1d5a;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1d59;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        u16Address=0x1d58;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u32BitErr=(u32BitErr<<8)|u8Data;
        
        u16Address=0x1d04;
        status&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data&=~(0x01);
        status&=MSB1245_WriteReg(u16Address, u8Data);
        if (u16BitErrPeriod == 0)
        {
            u16BitErrPeriod = 1;
        }
        fber = (float)u32BitErr/(u16BitErrPeriod*64800);
        *p_postBer = fber;
    }


    return TRUE;
}


MS_U16 MSB1245_DTV_GetSignalQuality(void)
{
    MS_U8     status = TRUE;
    float       fber = (float)0.0;
    float       SQI = (float)0.0;
    float log_ber = 0.0;


    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("pMSB1245 have not inited !!!"));
        return FALSE;
    }

    status &= MSB1245_DTV_GetPostBER(&fber);
    if(status)
    {
      log_ber = ( - 1) *log10(1 / fber);
      if (_bIsDVBS2==FALSE)//S
      {
          if (log_ber <= ( - 7.0))
          {         
             return 100;     
          } 
          else if (log_ber < ( - 3.7))
          {           
             SQI = (60+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-60)));
          }
          else if (log_ber < ( - 2.7))
          {
             SQI = (10+((( - 2.7) - log_ber) / (( - 2.7) - ( - 3.7))*(60-10)));
          }
          else
          {
            SQI = 10;        
          }
      }
      else //S2
      {
         if (log_ber <= ( - 7.0))
          {
              SQI = 100;
          }
          else if (log_ber < ( - 3.7))
          {
              SQI = (80+((( - 3.7) - log_ber) / (( - 3.7) - ( - 7.0))*(100-80)));
          }
          else if (log_ber < ( - 1.7))
          {
              SQI = (40+((( - 1.7) - log_ber) / (( - 1.7) - ( - 3.7))*(80-40)));
          }
          else if (log_ber < ( - 0.7))
          {
              SQI = (10+((( - 0.7) - log_ber) / (( - 0.7) - ( - 1.7))*(40-10)));
          }
          else
          {
              SQI = 5;
          }
      }	
      return SQI;
    }
    else
    {
       return 0;
    }

    return 0;
}


MS_BOOL  MSB1245_DVBS_SetFrequency(MS_U8 u8DemodIndex, MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{     
    MS_BOOL bRet=TRUE;     
    MS_U16 u16LockCount;

    bRet &= MSB1245_InitParam.pstTunertab->SetFreqS2(u8DemodIndex,u16CenterFreq, u32SymbolRate_Hz);

    if (bRet==FALSE)        
        return bRet;    

    u16LockCount=0;    
    do    
    {             
         bRet&= MSB1245_InitParam.pstTunertab->GetLock(u8DemodIndex);        
         MsOS_DelayTask(1);        
         u16LockCount++;    
    }
    while((bRet==FALSE) && (u16LockCount<MSB1245_TUNER_WAIT_TIMEOUT)) ;    

    if (bRet==TRUE)    
    {        
         DMD_DBG(("Tuner PLL Lock\n"));    
    }    
    else    
    {        
         DMD_DBG(("Tuner PLL Unlock\n"));    
    }     
    return bRet;
 }


MS_BOOL MSB1245_DTV_GetSNR_F(float *fSNR)
{
    MSB1245_DTV_GetSNR();
    *fSNR = g_MSB1245_fSNR;
    return TRUE;
}


void MSB1245_DTV_ControlTsOutput(MS_BOOL bEnable)
{
    if (bEnable)
    {
        MSB1245_WriteReg(0x0900 + (0x2D * 2), 0xFF);
    }
    else
    {
        MSB1245_WriteReg(0x0900 + (0x2D * 2), 0x00);
    }
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_U8 MSB1245_Get_Packet_Error(MS_U16 *u16PktErr)
{
    MS_BOOL    bRet = TRUE;
    MS_U8     u8Data = 0;
    MS_U16    u16Data=0,u16Address = 0;
    float      Packet_Err=0;
    MS_U16     PRD;
    MS_U16     PER=0;

    if (_bIsDVBS2==FALSE)//S
    {
        u16Address=0x1c32;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data|=0x80;
        bRet&=MSB1245_WriteReg(u16Address, u8Data); //freeze

        // bank 17 0x18 [7:0] reg_bit_err_sblprd_7_0  [15:8] reg_bit_err_sblprd_15_8
        u16Address=0x1c30;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1c31;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data;

        PRD=u16Data;
        if (PRD == 0)    // avoide division by 0
            PRD++;

        // bank 17 0x1F [7:0] reg_uncrt_pkt_num_7_0 [15:8] reg_uncrt_pkt_num_15_8
        u16Address=0x1c3E;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1c3F;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data; //Packet error
        PER=u16Data;


        u16Address=0x1c32;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x80);
        bRet&=MSB1245_WriteReg(u16Address, u8Data); //unfreeze
        Packet_Err = (float)PER/(128*PRD);
    }
    else  //s2
    {

        u16Address=0x1d04;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data|=0x01;
        bRet&=MSB1245_WriteReg(u16Address, u8Data); //freeze


        u16Address=0x1d56;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u16Data= u8Data;
        u16Address=0x1d57;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u16Data= (u8Data<<8)| u16Data;    //E-flag, error
        PER=u16Data;

        u16Address=0x1d04;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
        u8Data&=(~0x01);
        bRet&=MSB1245_WriteReg(u16Address, u8Data); //unfreeze
    }

    *u16PktErr = PER;

    return bRet;
}


/********************************************
Function: To get chip version
Parameter: pu8Chip   0x01->MSB1230, 0x02->MSB124X,
                    0xff ->unknown
*********************************************/
//MS_BOOL MSB124X_Demod_GetChip(MS_U8 *pu8Chip)
//{
//    MSB124X_I2C_CH_Reset(3);
//    *pu8Chip = MSB124X_DTV_GetHWVer();
//    return TRUE;
//}
MS_BOOL MSB1245_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    MS_BOOL bret = TRUE;
    
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {

         bret &= MSB1245_I2C_CH_Reset(u8DemodIndex,3);
         bret &= _MSB1245_SetReg(u8DemodIndex,0x0951, 0x00);
         bret &= _MSB1245_SetReg(u8DemodIndex,0x090C, 0x10);
         bret &= _MSB1245_SetReg(u8DemodIndex,0x090E, 0x10);
         if(bOn)
         {
             DMD_DBG(("set MSB1245 I2C bypass ON\n"));
             bret &= _MSB1245_SetReg(u8DemodIndex,0x0910, 0x10);
         }
         else
         {
             DMD_DBG(("set MSB1245 I2C bypass OFF\n"));
             bret &= _MSB1245_SetReg(u8DemodIndex,0x0910, 0x00);
         }  
         return bret;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_SetScanTypeStatus:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    MSB1245_IIC_Bypass_Mode(bOn);

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return TRUE;
}

MS_BOOL MSB1245_Demod_Open(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Open:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    pMSB1245->bOpen = TRUE;
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}

MS_BOOL MSB1245_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MSB1245_Demod_Close(void)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("Obtain mutex failed !!!\n"));
        return FALSE;
    }
    pMSB1245->bOpen = FALSE;
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}


/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1245_Demod_GetSNR(MS_U8 u8DemodIndex, float *pfSNR)
{

    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pfSNR = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus())
        {
            MSB1245_DTV_GetSNR_F(pfSNR);
        }
    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}

/****************************************************************************
**function:
**
**
****************************************************************************/
MS_BOOL MSB1245_Demod_GetBER(MS_U8 u8DemodIndex,float *pfBER)
{
    MS_BOOL status = TRUE;

    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetBER:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    //*pfBER = (float)MSB124X_DTV_GetSignalQuality();
    status = MSB1245_DTV_GetPostBER(pfBER);

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return status;
}

MS_BOOL MSB1245_Demod_GetPWR(MS_U8 u8DemodIndex, MS_S32 *ps32Signal)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetPWR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *ps32Signal = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus())
        {      
            *ps32Signal  = (MS_U32)MSB1245_DTV_GetSignalQuality();
        }
        
        if (*ps32Signal > 100)
        {
            *ps32Signal = 100;
        }
        else if (*ps32Signal < 0)
        {
            *ps32Signal = 0;
        }
    }

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return TRUE;
}

MS_BOOL MSB1245_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16 *pu16quality)
{

    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetSNR:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *pu16quality = 0;
        if (E_DEMOD_LOCK == MSB1245_DTV_GetLockStatus())
        {
            *pu16quality  = MSB1245_DTV_GetSignalQuality();
        }

    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);

    return TRUE;
}


MS_BOOL MSB1245_Demod_GetLock(MS_U8 u8DemodIndex, EN_LOCK_STATUS *peLockStatus)
{    
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    *peLockStatus = E_DEMOD_CHECKING;
    
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }

    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_GetLock:Obtain mutex failed !!!\n"));
        return FALSE;
    }
    else
    {
        *peLockStatus = MSB1245_DTV_GetLockStatus();
    }

    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return TRUE;
}


MS_BOOL MSB1245_DVBS_S2_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam)
{
     MS_BOOL bRet=TRUE;    
     MS_U16 u16Address;    
     MS_U8   u8Data;    
     MS_U16 u16SymbolRate;    
     MS_U16 u16CenterFreq;

     MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }


    u16SymbolRate=(pParam->SatParam.u32SymbolRate/1000);    
    u16CenterFreq=pParam->u32Frequency;    
    DMD_DBG(("MSB1245 MDrv_CofdmDmd_Restart+ Fc:%d SymbolRate %d\n", u16CenterFreq, u16SymbolRate));    
    MSB1245_IIC_Bypass_Mode(TRUE);    
    bRet&= MSB1245_DVBS_SetFrequency(u8DemodIndex, pParam->u32Frequency,(pParam->SatParam.u32SymbolRate/1000));    
    MSB1245_IIC_Bypass_Mode(FALSE);  
      
    u16Address=0x0990;    
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);    
    u8Data&=0xF0;    
    bRet&=MSB1245_WriteReg(u16Address,u8Data);    
    
    u16Address=0x0B32;//MCU reset
    u8Data = 0x03;
    bRet&=MSB1245_WriteReg(u16Address,u8Data);
    MsOS_DelayTask(1);
    u8Data = 0x00;
    bRet&=MSB1245_WriteReg(u16Address,u8Data);
    MsOS_DelayTask(10);

    u16Address=0x0B52;//DIG_DBG_2    
    u8Data=(u16SymbolRate&0xFF);    
    bRet&=MSB1245_WriteReg(u16Address,u8Data);    
    u16Address=0x0B53;//DIG_DBG_3    
    u8Data=((u16SymbolRate>>8)&0xFF);    
    bRet&=MSB1245_WriteReg(u16Address,u8Data);
    //ADCPLL IQ swap
    if(MSB1245_ADCPLL_IQ_SWAP==1)
    {
        u16Address= 0x0A03;
        bRet&= MSB1245_ReadReg(u16Address, &u8Data);
        u8Data |= (0x10);
        bRet&= MSB1245_WriteReg(u16Address, u8Data);
    }

    u16Address=0x0990;    
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);    
    u8Data&=0xF0;    
    u8Data|=0x01;    
    bRet&=MSB1245_WriteReg(u16Address,u8Data);
    bRet&= MSB1245_DiSEqC_Init();
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex); 
    return bRet;
}


MS_BOOL MSB1245_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM *pParam, MS_U32 u32BroadCastType)
{
    return MSB1245_DVBS_S2_Demod_Restart(u8DemodIndex, pParam);
}


MS_BOOL MSB1245_Demod_Init(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    DMD_DBG(("%s(),%d\n", __FUNCTION__, __LINE__));

    MS_U8     status = TRUE;
    sDMD_MSB124X_InitData sMSB1245_InitData;
    HWI2C_PORT hwi2c_port;

    if(NULL == pParam)
        return FALSE;
    
    MSB1245_InitParam.pstTunertab = pParam->pstTunertab;

    if (MSB1245.bInited)
        return TRUE;

    _msb1245_hw_reset(u8DemodIndex);
    MSB1245.bInited = TRUE;
    MSB1245.bOpen = TRUE;
    MSB1245.u8sramCode = 0;
    MSB1245.bFECLock = FALSE;
    MSB1245.u8ScanStatus = 0;
    MSB1245.u32CurrFreq = 0;

    MSB1245.s32_MSB1245_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex_MSB1245", MSOS_PROCESS_SHARED);
    if (MSB1245.s32_MSB1245_Mutex < 0)
    {
        MSB1245.bInited = FALSE;
        MSB1245.bOpen = FALSE;
        DMD_ERR(("creat MSB1245 Mutex error!!!\n"));
        return FALSE;
    }
    //void MApi_Demod_HWReset(void);
    //MApi_Demod_HWReset();

    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        sMSB1245_InitData.fpMSB124X_I2C_Access = msb1245_I2C_Access;
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        sMSB1245_InitData.fpMSB124X_I2C_Access = msb1245_I2C1_Access;
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    sMSB1245_InitData.u8WO_SPI_Flash = TRUE;
    sMSB1245_InitData.bPreloadDSPCodeFromMainCHIPI2C = FALSE;
    sMSB1245_InitData.bFlashWPEnable = TRUE;
    sMSB1245_InitData.fpGPIOReset = NULL;
    sMSB1245_InitData.pDVBC_DSP_REG = NULL;
    sMSB1245_InitData.pDVBT_DSP_REG = NULL;
    sMSB1245_InitData.pDVBT2_DSP_REG = NULL;
    sMSB1245_InitData.bEnableSPILoadCode = FALSE;
    sMSB1245_InitData.u8WO_Sdram = TRUE;

    status &= MDrv_DMD_MSB124X_Init(&sMSB1245_InitData, sizeof(sDMD_MSB124X_InitData));
   if (status)    
    {        
        MDrv_DMD_MSB124X_SetCurrentDemodulatorType(E_DMD_MSB124X_DEMOD_DVBS2);        
        status &=MDrv_DMD_MSB1245_LoadDSPCodeToSram(); 
#if MSB1245_SELECT_AGC_PAD_T
        status &= MSB1245_WriteReg2bytes(0x0A00+(0x18)*2, 0x0001);
#else
        status &= MSB1245_WriteReg2bytes(0x0A00+(0x18)*2, 0x0100);
#endif

#if MS_DVBS_INUSE
        status &= MSB1245_DiSEqC_Init();
#endif
   }


    if (!status)
    {
        MSB1245.bInited = FALSE;
        MSB1245.bOpen = FALSE;
        MsOS_DeleteMutex(MSB1245.s32_MSB1245_Mutex);      
        return FALSE;
    }

#if TS_PARALLEL_OUTPUT
    MSB1245_DTV_Serial_Control(FALSE);
#else
    MSB1245_DTV_Serial_Control(TRUE);
#endif

    return TRUE;
}

static MS_BOOL _MSB1245_GetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];    
    HWI2C_PORT hwi2c_port;
    mapi_i2c *iptr;
    
    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        iptr = mapi_i2c_GetI2C_Dev();
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        iptr = mapi_i2c1_GetI2C_Dev();
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    
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
    bRet &=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    
    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(0, NULL, 5, u8MsbData);
    bRet &=iptr->ReadBytes(0, NULL, 1, pu8Data);
    
    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(0, NULL, 1, u8MsbData);

    return bRet;
}

static MS_BOOL _MSB1245_SetReg(MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    HWI2C_PORT hwi2c_port;
    mapi_i2c *iptr;
    
    hwi2c_port = getI2CPort(u8DemodIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        iptr = mapi_i2c_GetI2C_Dev();
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        iptr = mapi_i2c1_GetI2C_Dev();
    }
    else
    {
        DMD_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    }
    
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
    bRet &=iptr->WriteBytes(0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    bRet &=iptr->WriteBytes(0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet &=iptr->WriteBytes(0, NULL, 1, u8MsbData);
    return bRet;
}


MS_U16 MSB1245_Demod_ReadReg(MS_U16 RegAddr)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    MS_U8 RegData = 0;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    
    MSB1245_ReadReg(RegAddr, &RegData);
    
    return (MS_U16) RegData;
}

MS_BOOL MSB1245_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{
    MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;

    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    
    return MSB1245_WriteReg(RegAddr, RegData);
}

#define MSB1245_CHIP_ID 0x91
MS_BOOL MSB1245_Check_Exist(MS_U8 u8DemodIndex)
{
    MS_U8 u8_tmp = 0;
    MS_U8 i;

    u8_tmp = DEMOD_MSB1245_SLAVE_ID;
    for(i=0 ; i<sizeof(u8Possible_SLAVE_IDs);i++)
    {
        _msb1245_hw_reset(u8DemodIndex);
        DEMOD_MSB1245_SLAVE_ID = u8Possible_SLAVE_IDs[i];
        MDrv_DMD_MSB124X_Exit();
        if(!MSB1245_I2C_CH_Reset(u8DemodIndex,3))
        {
            DEMOD_MSB1245_SLAVE_ID = u8_tmp;
            DMD_ERR(("[MSB1245] I2C_CH_Reset fail\n"));
        }
        else
        {
             DMD_DBG(("[MSB1245] I2C slave id :%x \n",DEMOD_MSB1245_SLAVE_ID ));
             break;
        }
    }
    
    if(_MSB1245_GetReg(u8DemodIndex,0x0900,&u8_tmp))
    {
        DMD_DBG(("[MSB1245] read id :%x \n",u8_tmp ));
    }
    else
    {
        return FALSE;
    }
    
    if(u8_tmp == MSB1245_CHIP_ID)
    {
       return TRUE;
    }
    else
    {
       return FALSE;
    }
}

MS_BOOL MSB1245_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;

    switch(fuction_type)
    {
         case DEMOD_EXT_FUNC_OPEN:
            bret &= MSB1245_Demod_Open();
            break;
          case DEMOD_EXT_FUNC_CLOSE:
            bret &= MSB1245_Demod_Close();
            break;
        case DEMOD_EXT_FUNC_FINALIZE:
            _msb1245_hw_reset(u8DemodIndex);//for I2C re-write password issue 
            break;
        default:
            DMD_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return bret;
}
#if MS_DVBS_INUSE

MS_BOOL MSB1245_DiSEqC_Init(void)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

    u16Address=0x0DCD; //Clear Status
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&(~0x2E))|(0x2E);
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x00;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

    //u16Address=0x0DC0;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&(~0x02))|(0x02);
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

#ifdef CHIP_KERES
    //Set MSPI_DO pin as DiSeqc_out, tone mode 
    bRet&=MSB1245_WriteReg(0x9D6, 0x80);
    bRet&=MSB1245_WriteReg(0x9D7, 0x01);
    bRet&=MSB1245_ReadReg(0x976, &u8Data);
    u8Data=(u8Data&(~0x01));
    bRet&=MSB1245_WriteReg(0x976, u8Data);
    //Disable DiSeqc tone mode, depend on LNB control IC
    bRet&=MSB1245_ReadReg(0xDD7, &u8Data);
    u8Data|= 0x80;
    bRet&=MSB1245_WriteReg(0xDD7, u8Data);
#endif    
    return bRet;
}

MS_BOOL MSB1245_DiSEqC_SetLNBOut(MS_U8 u8DemodIndex,MS_BOOL bLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

     MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB124X have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    if (bLow==TRUE)
    {
        u8Data=(u8Data|0x40);
    }
    else
    {
        u8Data=(u8Data&(~0x40));
    }
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
MS_BOOL MSB1245_DiSEqC_GetLNBOut(MS_U8 u8DemodIndex,MS_BOOL* bLNBOutLow)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;

     MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB1245 error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }


    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    if( (u8Data&0x40)==0x40)
    {
        * bLNBOutLow=TRUE;
    }
    else
    {
        * bLNBOutLow=FALSE;
    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
MS_BOOL MSB1245_DiSEqC_Set22kOnOff(MS_U8 u8DemodIndex,MS_BOOL b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

     MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    if (b22kOn==TRUE)
    {
        u8Data=(u8Data|0x08);
    }
    else
    {
        u8Data=(u8Data&(~0x08));
    }
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}
MS_BOOL MSB1245_DiSEqC_Get22kOnOff(MS_U8 u8DemodIndex,MS_BOOL* b22kOn)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;

     MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    if ((u8Data&0x08)==0x08)
    {
        *b22kOn=TRUE;
    }
    else
    {
        *b22kOn=FALSE;
    }
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_DiSEqC_SendCmd(MS_U8 u8DemodIndex,MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16 u16WaitCount;

     MDvr_CofdmDmd_CONFIG *pMSB1245 = &MSB1245;
    if (!pMSB1245)
    {
        DMD_ERR(("pMSB124X error !\n"));
        return FALSE;
    }
    if ((!pMSB1245->bInited) || (!pMSB1245->bOpen))
    {
        DMD_ERR(("[%s]pMSB1245 have not inited !!!\n", __FUNCTION__));
        return FALSE;
    }
    if (MsOS_ObtainMutex(pMSB1245->s32_MSB1245_Mutex, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DMD_ERR(("MDrv_Demod_Restart:Obtain mutex failed !!!\n"));
        return FALSE;
    }

    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data=(u8Data&~(0x10));
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

    //u16Address=0x0BC4;
    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u16Address=0x0DC4+u8Index;
        u8Data=*(pCmd+u8Index);
        bRet&=MSB1245_WriteReg(u16Address, u8Data);
    }
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    //Tone and Burst switch,Mantis 0232220
    if(((*pCmd)==0xE0)&&((*(pCmd + 1))==0x10)&&((*(pCmd + 2))==0x38)&&((((*(pCmd + 3))&0x0C)==0x0C)||(((*(pCmd + 3))&0x04)==0x04)))
    {
        u8Data|=0x80;   //u8Data|=0x20;Tone Burst1
    }
    else if(((*pCmd)==0xE0)&&((*(pCmd + 1))==0x10)&&((*(pCmd + 2))==0x38))
    {
        u8Data|=0x20;   //u8Data|=0x80;ToneBurst0
    }

    u16Address=0x0B54;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    u16Address=0x0990;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x10;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

#if 1       //For Unicable command timing,mick
    u16WaitCount=0;
    do
    {
        u16Address=0x0990;
        bRet&=MSB1245_ReadReg(u16Address, &u8Data);
         MsOS_DelayTask(1);
         u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < MSB1245_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= MSB1245_DEMOD_WAIT_TIMEOUT)
    {
        DMD_ERR(("MSB1240 DVBS DiSEqC Send Command Busy!!!\n"));
        return FALSE;
    }
#endif      //For Unicable command timing,mick

    DMD_DBG(("\r\n MDrv_DiSEqC_SendCmd----"));
    MsOS_ReleaseMutex(pMSB1245->s32_MSB1245_Mutex);
    return bRet;
}

MS_BOOL MSB1245_DiSEqC_SetTone(MS_U8 u8DemodIndex,MS_BOOL bTone1)
{
    MS_BOOL bRet=TRUE;
    MS_U16 u16Address;
    MS_U8 u8Data;
    MS_U8 u8ReSet22k;

   u16Address=0x0DC4;
    u8Data=0x01;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

    u16Address=0x0DC0;
    u8Data=0x4E;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    u16Address=0x0DCC;
    u8Data=0x80;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

    u16Address=0x0DC2;
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8ReSet22k=u8Data;

    if (bTone1==TRUE)
    {
       u8Data=0x19;
       bRet&=MSB1245_WriteReg(u16Address, u8Data);
       // _u8ToneBurstFlag=1;
    }
    else
    {
       u8Data=0x11;
       bRet&=MSB1245_WriteReg(u16Address, u8Data);
       //_u8ToneBurstFlag=2;
    }
    u16Address=0x0DCD;
    u8Data=u8Data&~(0x01);
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data=u8Data|0x3E;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(10);
    bRet&=MSB1245_ReadReg(u16Address, &u8Data);
    u8Data=u8Data&~(0x3E);
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(1);
    u8Data=u8Data|0x01;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);
    MsOS_DelayTask(100);
    u16Address=0x0DC2;
    u8Data=u8ReSet22k;
    bRet&=MSB1245_WriteReg(u16Address, u8Data);

    //MsOS_DelayTask(100);
    return bRet;
}

#endif

DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSB1245) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSB1245",
     .data                         = DEMOD_MSB1245,
     .init                         = MSB1245_Demod_Init,
     .GetLock                      = MSB1245_Demod_GetLock,
     .GetSNR                       = MSB1245_Demod_GetSNR,
     .GetBER                       = MSB1245_Demod_GetBER,
     .GetPWR                       = MSB1245_Demod_GetPWR,
     .GetQuality                   = MSB1245_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MSB1245_Demod_Restart,
     .I2CByPass                    = MSB1245_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .CheckExist                   = MSB1245_Check_Exist,
     .Extension_Function           = MSB1245_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MSB1245_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MSB1245_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MSB1245_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MSB1245_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MSB1245_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MSB1245_DiSEqC_SendCmd
#endif
};

#endif

