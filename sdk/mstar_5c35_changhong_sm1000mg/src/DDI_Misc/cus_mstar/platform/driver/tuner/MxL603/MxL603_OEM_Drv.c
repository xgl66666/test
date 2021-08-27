/*****************************************************************************************
 *
 * FILE NAME          : MxL603_OEM_Drv.c
 *
 * AUTHOR             : Mahendra Kondur
 *
 * DATE CREATED       : 12/23/2011
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that
 *                      OEM should implement for MxL603 APIs
 *
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL603_OEM_Drv.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "Board.h"
#include "drvIIC.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL603)


//extern MS_BOOL MDrv_IIC_Write(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
//extern MS_BOOL MDrv_IIC_Read(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
//extern MS_BOOL MDrv_IIC1_Write(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
//extern MS_BOOL MDrv_IIC1_Read(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);

#define IIC0_WRITE                    MDrv_IIC_Write
#define IIC0_READ                     MDrv_IIC_Read
#define IIC1_WRITE                    MDrv_IIC1_Write
#define IIC1_READ                     MDrv_IIC1_Read


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_WriteRegister
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

MXL_STATUS MxLWare603_OEM_WriteRegister(UINT8 u8TunerIndex, UINT8 devId, UINT8 RegAddr, UINT8 RegData)
{
  // OEM should implement I2C write protocol that complies with MxL603 I2C
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


/* If OEM data is implemented, customer needs to use OEM data structure related operation
   Following code should be used as a reference.
   For more information refer to sections 2.5 & 2.6 of MxL603_mxLWare_API_UserGuide document.

  UINT8 i2cSlaveAddr;
  UINT8 i2c_bus;
  user_data_t * user_data = (user_data_t *) MxL603_OEM_DataPtr[devId];

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

  /* If OEM data is not required, customer should treat devId as I2C slave Address */

  //UINT8 i2c_address = devId; // create device i2c address
  UINT8 pArray[2];
  HWI2C_PORT hwi2c_port;
  
  hwi2c_port = getI2CPort(u8TunerIndex);
  
  if (hwi2c_port < E_HWI2C_PORT_1)
  {
      u8TunerIndex = 0; //means I2C port
  }
  else if (hwi2c_port < E_HWI2C_PORT_2)
  {
      u8TunerIndex = 1;
  }
  else
  {
      TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
      return FALSE;
  }

  pArray[0] = RegAddr;
  pArray[1] = RegData;

  //RegAddr = RegAddr;
  //RegData = RegData;
  //printf("\n1\n");
//MDrv_IIC_Write(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
  //if (IIC_WRITE( devId, 0, 0, pArray, 2))
/*
    if (u8TunerIndex == 1)
    {
        if (IIC1_WRITE( devId, NULL, 0, pArray, 2) != 0)
        {
            MsOS_DelayTask(1);
            return status =MXL_TRUE ;
        }
        else
        {

            printf("\ni2c1 write err\n");
            return status=MXL_FALSE;
        }

    }
    else
    {
        if (IIC_WRITE( devId, NULL, 0, pArray, 2) != 0)
        {
            MsOS_DelayTask(1);
            return status =MXL_TRUE ;
        }
        else
        {

            printf("\ni2c write err\n");
            return status=MXL_FALSE;
        }
    }
*/
    switch(u8TunerIndex)
    {
        case 0:
        {
            if (IIC0_WRITE( devId, NULL, 0, pArray, 2) != 0)
            {
                MsOS_DelayTask(1);
                return status =MXL_TRUE ;
            }
            else
            {
                TUNER_ERR(("i2c write err\n"));
                return status=MXL_FALSE;
            }
            break;
        }
        case 1:
        {
            if (IIC1_WRITE( devId, NULL, 0, pArray, 2) != 0)
            {
                MsOS_DelayTask(1);
                return status =MXL_TRUE ;
            }
            else
            {

                TUNER_ERR(("i2c1 write err\n"));
                return status=MXL_FALSE;
            }
            break;
        }
        case 2:
        {
            if (IIC0_WRITE( devId, NULL, 0, pArray, 2) != 0)
            {
                MsOS_DelayTask(1);
                return status =MXL_TRUE ;
            }
            else
            {
                TUNER_ERR(("i2c write err\n"));
                return status=MXL_FALSE;
            }
            break;
        }
        case 3:
        {
            if (IIC1_WRITE( devId, NULL, 0, pArray, 2) != 0)
            {
                MsOS_DelayTask(1);
                return status =MXL_TRUE ;
            }
            else
            {

                TUNER_ERR(("i2c1 write err\n"));
                return status=MXL_FALSE;
            }
            break;
        }
        default:
        {
            if (IIC0_WRITE( devId, NULL, 0, pArray, 2) != 0)
            {
                MsOS_DelayTask(1);
                return status =MXL_TRUE ;
            }
            else
            {
                TUNER_ERR(("i2c write err\n"));
                return status=MXL_FALSE;
            }
            break;
        }
    } //end switch(u8TunerIndex)
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_ReadRegister
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

MXL_STATUS MxLWare603_OEM_ReadRegister(UINT8 u8TunerIndex,UINT8 devId, UINT8 RegAddr, UINT8 *DataPtr)
{
  // OEM should implement I2C read protocol that complies with MxL603 I2C
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

  MXL_STATUS status = MXL_TRUE;

/* If OEM data is implemented, customer needs to use OEM data structure related operation
   Following code should be used as a reference.
   For more information refer to sections 2.5 & 2.6 of MxL603_mxLWare_API_UserGuide document.

  UINT8 i2cSlaveAddr;
  UINT8 i2c_bus;
  user_data_t * user_data = (user_data_t *) MxL603_OEM_DataPtr[devId];

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

  //devId = devId;
  //RegAddr = RegAddr;
  //*DataPtr = *DataPtr;

  UINT8 pArray[2];
  HWI2C_PORT hwi2c_port;
  
  hwi2c_port = getI2CPort(u8TunerIndex);
  
  if (hwi2c_port < E_HWI2C_PORT_1)
  {
      u8TunerIndex = 0; //means I2C port
  }
  else if (hwi2c_port < E_HWI2C_PORT_2)
  {
      u8TunerIndex = 1;
  }
  else
  {
      TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
      return FALSE;
  }


  pArray[0] = 0xFB;
  pArray[1] = RegAddr;
  //printf("\n2\n");
/*
    if (u8TunerIndex == 1)
    {
      if (IIC1_READ( devId, pArray, 2, DataPtr, 1)!= 0)
      {
          return status =MXL_TRUE ;
      }
      else
      {

          printf("\ni2c1 write err\n");
          return status=MXL_FALSE;
      }

    }
    else
    {
      if (IIC_READ( devId, pArray, 2, DataPtr, 1) != 0)
      {
          return status =MXL_TRUE ;
      }
      else
      {

          printf("\ni2c read err\n");
          return status=MXL_FALSE;
      }
    }
*/
    switch(u8TunerIndex)
    {
        case 0:
        {
            if (IIC0_READ( devId, pArray, 2, DataPtr, 1) != 0)
            {
              return status =MXL_TRUE ;
            }
            else
            {

              TUNER_ERR(("i2c read err\n"));
              return status=MXL_FALSE;
            }
            break;
        }
        case 1:
        {
            if (IIC1_READ( devId, pArray, 2, DataPtr, 1)!= 0)
            {
              return status =MXL_TRUE ;
            }
            else
            {
              TUNER_ERR(("i2c1 write err\n"));
              return status=MXL_FALSE;
            }
            break;
        }
        case 2:
        {
            if (IIC0_READ( devId, pArray, 2, DataPtr, 1) != 0)
            {
              return status =MXL_TRUE ;
            }
            else
            {
              TUNER_ERR(("i2c read err\n"));
              return status=MXL_FALSE;
            }
            break;
        }
        case 3:
        {
            if (IIC1_READ( devId, pArray, 2, DataPtr, 1)!= 0)
            {
              return status =MXL_TRUE ;
            }
            else
            {
              TUNER_ERR(("i2c1 write err\n"));
              return status=MXL_FALSE;
            }
            break;
        }
        default:
        {
            if (IIC0_READ( devId, pArray, 2, DataPtr, 1) != 0)
            {
              return status =MXL_TRUE ;
            }
            else
            {
              TUNER_ERR(("i2c read err\n"));
              return status=MXL_FALSE;
            }
            break;
        }
    } //end switch(u8TunerIndex)
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare603_OEM_Sleep
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

void MxLWare603_OEM_Sleep(UINT16 DelayTimeInMs)
{
  // OEM should implement sleep operation
  MsOS_DelayTask(DelayTimeInMs);
}



#endif


