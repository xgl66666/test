/*******************************************************************************
 *
 * FILE NAME          : MxL68x_OEM_Drv.c
 * 
 * AUTHOR             : Brenndon Lee
 *                      Dong Liu 
 *
 * DATE CREATED       : 07/30/2009
 *                      05/10/2012  
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that 
 *                      OEM should implement for MxL68x APIs
 *                             
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#include "MxL68x_OEM_Drv.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "drvIIC.h"

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_WriteRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C write operation.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_OEM_WriteRegister(UINT8 devId, UINT16 RegAddr, UINT16 RegData)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  // OEM should implement I2C write protocol that complies with MxL68x I2C
  // format.
  // 16 bit Register Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |RegAddr(H)| |RegAddr(L)| |RegData(H)| |RegData(L)| |P|
  // +------+-+-----+-+-+----------+-+----------+-+----------+-+----------+-+-+
  // |SLAVE |         |A|          |A|          |A|          |A|          |A| |
  // +------+---------+-+----------+-+----------+-+----------+-+----------+-+-+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK), 
  // P(Stop condition)
  mxl68x_oem_data_t *oemDataPtr = (mxl68x_oem_data_t *)0;
  MS_U8 u8Databuff[4];
  MS_IIC_PORT ePort;

  ePort = getI2CPort(devId);

  oemDataPtr = (mxl68x_oem_data_t *)MxL68x_OEM_DataPtr[devId];
  u8Databuff[0] = (MS_U8)((RegAddr>>8) & 0xff);
  u8Databuff[1] = (MS_U8)(RegAddr&0xff);
  u8Databuff[2] = (MS_U8)((RegData>>8)& 0xff);
  u8Databuff[3] = (MS_U8)(RegData & 0xff);

  if (oemDataPtr)
  {
    if(!MDrv_IIC_WriteBytes(ePort, (oemDataPtr->i2cAddress)<<1, 0, NULL, 4, u8Databuff))
    {
         status = MXL_FAILURE;
    }
  }
  else
  {
      status = MXL_FAILURE;
  }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_ReadRegister
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

MXL_STATUS_E MxLWare68x_OEM_ReadRegister(UINT8 devId, UINT16 RegAddr, UINT16 *DataPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  // OEM should implement I2C read protocol that complies with MxL68x I2C
  // format.

  // 16 Register Read Protocol:
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFF| |0xFB| |RegAddr(H)| |RegAddr(L)| |P|
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |SLAVE |         |A|    |A|    |A|          |A|          |A| |
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // +------+-+-----+-+-+-----+-----+--+-+
  // |MASTER|S|SADDR|R| |     |     |MN|P|
  // +------+-+-----+-+-+-----+-----+--+-+
  // |SLAVE |         |A|Data |Data |  | |
  // +------+---------+-+-----+-----+--+-+
  // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK), 
  // P(Stop condition)
  mxl68x_oem_data_t *oemDataPtr = (mxl68x_oem_data_t *)0;
  MS_BOOL bret = TRUE;
  MS_U8 u8Databuff[4];
  MS_U8 u8Readbuff[2];
  MS_IIC_PORT ePort;

  ePort = getI2CPort(devId);

  oemDataPtr = (mxl68x_oem_data_t *)MxL68x_OEM_DataPtr[devId];
  u8Databuff[0] = 0xff;
  u8Databuff[1] = 0xfb;
  u8Databuff[2] = (MS_U8)((RegAddr>>8)& 0xff);
  u8Databuff[3] = (MS_U8)(RegAddr & 0xff);

  u8Readbuff[0] = 0;
  u8Readbuff[1] = 0;

  if (oemDataPtr)
  {
      bret &= MDrv_IIC_WriteBytes(ePort, (oemDataPtr->i2cAddress)<<1, 0, NULL, 4, u8Databuff);
      bret &= MDrv_IIC_ReadBytes(ePort, (oemDataPtr->i2cAddress)<<1, 0, NULL, 2, u8Readbuff);
      if(!bret)
      {
         status = MXL_FAILURE;
      }
  }
  else
  {
      status = MXL_FAILURE;
  }
  *DataPtr = (UINT16)((u8Readbuff[0]<<8) | (u8Readbuff[1]));
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_WriteBlock
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete block write function
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_WriteBlock(UINT8 devId, UINT16 regAddr, UINT16 bufSize, UINT8* bufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  // OEM should implement sleep operation 
  mxl68x_oem_data_t *oemDataPtr = (mxl68x_oem_data_t *)0;
  UINT8 bret = 1;
  MS_IIC_PORT ePort;
  MS_U8 u8regAddr[2];


  u8regAddr[0] = (regAddr&0xFF00)>>8;
  u8regAddr[1] = regAddr&0x00FF;
  ePort = getI2CPort(devId);

  oemDataPtr = (mxl68x_oem_data_t *)MxL68x_OEM_DataPtr[devId];
  
  if (oemDataPtr)
  {
      bret &= MDrv_IIC_WriteBytes(ePort, (oemDataPtr->i2cAddress)<<1, 2, u8regAddr, bufSize, bufPtr);
  }
  else
  {
      status = MXL_FAILURE;
  }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_ReadBlock
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete block read function
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_ReadBlock(UINT8 devId, UINT16 regAddr, UINT16 readSize, UINT8* bufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  // OEM should implement sleep operation 
  mxl68x_oem_data_t *oemDataPtr = (mxl68x_oem_data_t *)0;
  UINT8 bret = 1;
  MS_IIC_PORT ePort;
  MS_U8 u8regAddr[4];

  
  u8regAddr[0] = 0xff;
  u8regAddr[1] = 0xfd;
  u8regAddr[2] = (regAddr&0xFF00)>>8;
  u8regAddr[3] = regAddr&0x00FF;

  ePort = getI2CPort(devId);

  oemDataPtr = (mxl68x_oem_data_t *)MxL68x_OEM_DataPtr[devId];
  
  if (oemDataPtr)
  {
     bret &= MDrv_IIC_WriteBytes(ePort, (oemDataPtr->i2cAddress)<<1, 0, NULL, 4, u8regAddr);
     bret &= MDrv_IIC_ReadBytes(ePort, (oemDataPtr->i2cAddress)<<1, 0, NULL, readSize, bufPtr);
  }
  else
  {
      status = MXL_FAILURE;
  }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_RegisterCallBack
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function registers call back function which used to 
--|                 read AC data 
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_RegisterCallBack(INTERRUPT_TRIGGERED_CB_FN FuncPtr, UINT8 IntSrcId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  // OEM should implement sleep operation 
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_UnRegisterCallBack
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete unregisters call back function which  
--|                 used to read AC data 
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_UnRegisterCallBack(UINT8 IntSrcId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  // OEM should implement sleep operation 
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MXL_Sleep
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete sleep operation. WaitTime is in us unit
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
void MxLWare68x_OEM_SleepInUs( UINT32 delayTimeInUs)
{
  MS_U32 u32Us;

  u32Us = (MS_U32)delayTimeInUs;
  MsOS_DelayTaskUs(u32Us);
}
