/*****************************************************************************************
 *
 * FILE NAME          : MxL608_OEM_Drv.c
 *
 * AUTHOR             : Mahendra Kondur
 *
 * DATE CREATED       : 12/23/2011
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that
 *                      OEM should implement for MxL608 APIs
 *
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL608_OEM_Drv.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "Board.h"
#include "drvIIC.h"
#if IF_THIS_TUNER_INUSE(TUNER_MXL608)

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare608_OEM_WriteRegister
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

MXL_STATUS MxLWare608_OEM_WriteRegister(UINT8 u8TunerIndex, UINT8 devId, UINT8 RegAddr, UINT8 RegData)
{
    // OEM should implement I2C write protocol that complies with MxL608 I2C
    // format.

    // 8 bit Register Write Protocol:
    // +------+-+-----+-+-+----------+-+----------+-+-+
    // |MASTER|S|SADDR|W| |RegAddr   | |RegData(L)| |P|
    // +------+-+-----+-+-+----------+-+----------+-+-+
    // |SLAVE |         |A|          |A|          |A| |
    // +------+---------+-+----------+-+----------+-+-+
    // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK),
    // P(Stop condition)


/* If OEM data is implemented, customer needs to use OEM data structure related operation
    Following code should be used as a reference.
    For more information refer to sections 2.5 & 2.6 of MxL608_mxLWare_API_UserGuide document.

    UINT8 i2cSlaveAddr;
    UINT8 i2c_bus;
    user_data_t * user_data = (user_data_t *) MxL608_OEM_DataPtr[devId];

    if (user_data)
    {
        i2cSlaveAddr = user_data->i2c_address;           // get device i2c address
        i2c_bus = user_data->i2c_bus;                   // get device i2c bus

        sem_up(user_data->sem);                         // up semaphore if needed

        // I2C Write operation
        status = USER_I2C_WRITE_FUNCTION(i2cSlaveAddr, i2c_bus, RegAddr, RegData);

        sem_down(user_data->sem);                       // down semaphore
        user_data->i2c_cnt++;                           // user statistics
    }

*/
    MXL_STATUS status = MXL_FALSE;
    MS_IIC_PORT ePort;
    MS_U8 pArray[2];
    
    ePort = getI2CPort(u8TunerIndex);
    pArray[0] = RegAddr;
    pArray[1] = RegData;

    if(MDrv_IIC_WriteBytes(ePort, devId, 0, NULL, 2, pArray))
    {
       status = MXL_TRUE;
    }

    return status;

}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare608_OEM_ReadRegister
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

MXL_STATUS MxLWare608_OEM_ReadRegister(UINT8 u8TunerIndex,UINT8 devId, UINT8 RegAddr, UINT8 *DataPtr)
{
    // OEM should implement I2C read protocol that complies with MxL608 I2C
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


/* If OEM data is implemented, customer needs to use OEM data structure related operation
    Following code should be used as a reference.
    For more information refer to sections 2.5 & 2.6 of MxL608_mxLWare_API_UserGuide document.

    UINT8 i2cSlaveAddr;
    UINT8 i2c_bus;
    user_data_t * user_data = (user_data_t *) MxL608_OEM_DataPtr[devId];

    if (user_data)
    {
        i2cSlaveAddr = user_data->i2c_address;           // get device i2c address
        i2c_bus = user_data->i2c_bus;                   // get device i2c bus

        sem_up(user_data->sem);                         // up semaphore if needed

        // I2C Write operation
        status = USER_I2C_READ_FUNCTION(i2cSlaveAddr, i2c_bus, RegAddr, DataPtr);

        sem_down(user_data->sem);                       // down semaphore
        user_data->i2c_cnt++;                           // user statistics
    }

*/

    /* If OEM data is not required, customer should treat devId as I2C slave Address */

  MXL_STATUS status = MXL_FALSE;
  MS_U8 pArray[2];
  MS_IIC_PORT ePort;
  
  ePort = getI2CPort(u8TunerIndex);
  
  pArray[0] = 0xFB;
  pArray[1] = RegAddr;
  
  if(MDrv_IIC_ReadBytes(ePort, devId, 2, pArray, 1, DataPtr))
  {
     status = MXL_TRUE;
  }
  
  return status;  

}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare608_OEM_Sleep
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

void MxLWare608_OEM_Sleep(UINT16 DelayTimeInMs)
{
    // OEM should implement sleep operation
    MsOS_DelayTask(DelayTimeInMs);

}

#endif
