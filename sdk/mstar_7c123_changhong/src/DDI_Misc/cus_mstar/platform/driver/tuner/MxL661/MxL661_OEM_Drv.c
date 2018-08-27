/*****************************************************************************************
 *
 * FILE NAME          : MxL661_OEM_Drv.c
 *
 * AUTHOR             : Dong Liu
 *
 * DATE CREATED       : 01/23/2011
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that
 *                      OEM should implement for MxL661 APIs
 *
 *****************************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL661_OEM_Drv.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "drvIIC.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL661)

static TUNER_CON_CONFIG* pstCon = NULL; 
static MS_BOOL _getI2CPort(MS_U8 u8TunerIndex, MS_IIC_PORT* pePort)
{
    TUNER_CON_CONFIG* pCon = NULL;

    if(NULL == pstCon)
        return FALSE;
    pCon = pstCon + u8TunerIndex;
    *pePort = pCon->eI2C_PORT;
    return TRUE;
}
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare661_OEM_WriteRegister
--|
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C write operation.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS MxLWare661_OEM_WriteRegister(UINT8 u8TunerIndex, UINT8 devId, UINT8 RegAddr, UINT8 RegData)
{
    // OEM should implement I2C write protocol that complies with MxL661 I2C
    // format.

    // 8 bit Register Write Protocol:
    // +------+-+-----+-+-+----------+-+----------+-+-+
    // |MASTER|S|SADDR|W| |RegAddr   | |RegData(L)| |P|
    // +------+-+-----+-+-+----------+-+----------+-+-+
    // |SLAVE |         |A|          |A|          |A| |
    // +------+---------+-+----------+-+----------+-+-+
    // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK),
    // P(Stop condition)

    MXL_STATUS status = MXL_FALSE;
    MS_IIC_PORT ePort;
    
    if(!_getI2CPort(u8TunerIndex, &ePort))
        return status;


    UINT8 pArray[2];
    pArray[0] = RegAddr;
    pArray[1] = RegData;

    if(MDrv_IIC_WriteBytes(ePort, devId, 0, NULL, 2, pArray))
    {
       status = MXL_TRUE;
    }

    return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare661_OEM_ReadRegister
--|
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C read operation.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS MxLWare661_OEM_ReadRegister(UINT8 u8TunerIndex, UINT8 devId, UINT8 RegAddr, UINT8 *DataPtr)
{
    // OEM should implement I2C read protocol that complies with MxL661 I2C
    // format.

    // 8 bit Register Read Protocol:
    // +------+-+-----+-+-+----+-+----------+-+-+
    // |MASTER|S|SADDR|W| |0xFB| |RegAddr   | |P|
    // +------+-+-----+-+-+----+-+----------+-+-+
    // |SLAVE |         |A|    |A|          |A| |
    // +------+-+-----+-+-+----+-+----------+-+-+
    // +------+-+-----+-+-+-----+--+-+
    // |MASTER|S|SADDR|R| |     |MN|P|
    // +------+-+-----+-+-+-----+--+-+
    // |SLAVE |         |A|Data |  | |
    // +------+---------+-+-----+--+-+
    // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK),
    // P(Stop condition)

    MXL_STATUS status = MXL_FALSE;
    UINT8 pArray[2];
    MS_IIC_PORT ePort;
    
    if(!_getI2CPort(u8TunerIndex, &ePort))
        return status;

    pArray[0] = 0xFB;
    pArray[1] = RegAddr;

    if(MDrv_IIC_ReadBytes(ePort, devId, 2, pArray, 1, DataPtr))
    {
       status = MXL_TRUE;
    }

    return status;    
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare661_OEM_Sleep
--|
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete sleep operation. WaitTime is in ms unit
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

void MxLWare661_OEM_Sleep(UINT16 DelayTimeInMs)
{
    // OEM should implement sleep operation
    MsOS_DelayTask(DelayTimeInMs);
}

MXL_STATUS MxLWare661_OEM_SetConInfo(UINT8 u8TunerIndex, TUNER_CON_CONFIG* pstConInfo, MS_U8 u8ConCnt)
{
    TUNER_CON_CONFIG* pCon = NULL;

    if(NULL == pstCon)
    {
        pstCon = malloc(sizeof(TUNER_CON_CONFIG)*u8ConCnt);
        if(NULL == pstCon)
        {
            return MXL_FAILED;
        }
    }
    pCon = pstCon + u8TunerIndex;
    pCon->eI2C_PORT = pstConInfo->eI2C_PORT;
    pCon->u32HW_ResetPin = pstConInfo->u32HW_ResetPin; 
    return MXL_SUCCESS;
}

#endif
