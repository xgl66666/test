/*
* Copyright (c) 2011-2013 MaxLinear, Inc. All rights reserved
*
* License type: GPLv2
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*
* This program may alternatively be licensed under a proprietary license from
* MaxLinear, Inc.
*
* See terms and conditions defined in file 'LICENSE.txt', which is part of this
* source code package.
*/

#include "MxLWare_HYDRA_OEM_Defines.h"
#include "MxLWare_HYDRA_OEM_Drv.h"

#include "MxL_HRCLS_Common.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "drvIIC.h"

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_DeviceReset
 *
 * @param[in]   devId           Device ID
 *
 * @author Mahee
 *
 * @date 06/12/2012 Initial release
 *
 * This API performs a hardware reset on Hydra SOC identified by devId.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
MXL_STATUS_E MxLWare_HYDRA_OEM_DeviceReset(UINT8 devId)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;

  devId = devId;
  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_SleepInMs
 *
 * @param[in]   delayTimeInMs
 *
 * @author Mahee
 *
 * @date 06/12/2012 Initial release
 *
 * his API will implement delay in milliseconds specified by delayTimeInMs.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
void MxLWare_HYDRA_OEM_SleepInMs(UINT16 delayTimeInMs)
{
  delayTimeInMs=delayTimeInMs;

   MsOS_DelayTask(delayTimeInMs);   
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_GetCurrTimeInMs
 *
 * @param[out]   msecsPtr
 *
 * @author Mahee
 *
 * @date 06/12/2012 Initial release
 *
 * This API should return system time milliseconds.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
void MxLWare_HYDRA_OEM_GetCurrTimeInMs(UINT64 *msecsPtr)
{
    MS_U32 timeInMsec;

    timeInMsec = MsOS_GetSystemTime();

    *msecsPtr= (UINT64) timeInMsec;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_I2cWrite
 *
 * @param[in]   devId           Device ID
 * @param[in]   dataLen
 * @param[in]   buffPtr
 *
 * @author Mahee
 *
 * @date 06/12/2012 Initial release
 *
 * This API performs an I2C block write by writing data payload to Hydra device.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/

MXL_STATUS_E MxLWare_HYDRA_OEM_I2cWrite(UINT8 devId, UINT16 dataLen, UINT8 *buffPtr)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  oem_data_t *oemDataPtr = (oem_data_t *)0;
  HWI2C_PORT hwi2c_port;
  UINT8 bret = 1;

  oemDataPtr = (oem_data_t *)MxL_HYDRA_OEM_DataPtr[devId];
  hwi2c_port = getI2CPort(devId)/8;

  if (oemDataPtr)
  {
      switch(hwi2c_port)
      {
         case 0: 
            bret &= MDrv_IIC_Write((oemDataPtr->i2cAddress)<<1, NULL, 0, buffPtr, dataLen);
            if(!bret)
               mxlStatus = MXL_FAILURE;
            break;
         case 1:  
             bret &= MDrv_IIC1_Write((oemDataPtr->i2cAddress)<<1, NULL, 0, buffPtr, dataLen);
            if(!bret)
               mxlStatus = MXL_FAILURE; 
            break;
         default:
            mxlStatus = MXL_FAILURE;
            break;
      }
  }
  else
  {
      mxlStatus = MXL_FAILURE;
  }

  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_I2cRead
 *
 * @param[in]   devId           Device ID
 * @param[in]   dataLen
 * @param[out]  buffPtr
 *
 * @author Mahee
 *
 * @date 06/12/2012 Initial release
 *
 * This API shall be used to perform I2C block read transaction.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
MXL_STATUS_E MxLWare_HYDRA_OEM_I2cRead(UINT8 devId, UINT16 dataLen, UINT8 *buffPtr)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  oem_data_t *oemDataPtr = (oem_data_t *)0;
  HWI2C_PORT hwi2c_port;
  UINT8 bret = 1;

  oemDataPtr = (oem_data_t *)MxL_HYDRA_OEM_DataPtr[devId];
  hwi2c_port = getI2CPort(devId)/8;

  if (oemDataPtr)
  {
      switch(hwi2c_port)
      {
         case 0:   
            bret &= MDrv_IIC_Read((oemDataPtr->i2cAddress)<<1, NULL, 0, buffPtr, dataLen);
            if(!bret)
            {
               printf("MXL542 IIC read error, slave OD = 0x%x\n", (oemDataPtr->i2cAddress));
               mxlStatus = MXL_FAILURE;
            }
            break;
         case 1:  
            bret &= MDrv_IIC1_Read((oemDataPtr->i2cAddress)<<1, NULL, 0, buffPtr, dataLen);
            if(!bret)
            {
               mxlStatus = MXL_FAILURE;
            }
            break;
         default:
            mxlStatus = MXL_FAILURE;
            break;
      }

  }
  else
  {
      mxlStatus = MXL_FAILURE;
  }
  
  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_InitI2cAccessLock
 *
 * @param[in]   devId           Device ID
 *
 * @author Mahee
 *
 * @date 04/11/2013 Initial release
 *
 * This API will initilize locking mechanism used to serialize access for
 * I2C operations.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
MXL_STATUS_E MxLWare_HYDRA_OEM_InitI2cAccessLock(UINT8 devId)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  oem_data_t *oemDataPtr = (oem_data_t *)0;

  oemDataPtr = (oem_data_t *)MxL_HYDRA_OEM_DataPtr[devId];

  if (oemDataPtr)
  {
      oemDataPtr->Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)"Mutex MXL542", MSOS_PROCESS_SHARED);
      if(oemDataPtr->Mutex < 0)
        mxlStatus = MXL_FAILURE;
  }
  else
  {
     mxlStatus = MXL_FAILURE;
  }

  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_DeleteI2cAccessLock
 *
 * @param[in]   devId           Device ID
 *
 * @author Mahee
 *
 * @date 04/11/2013 Initial release
 *
 * This API will release locking mechanism and associated resources.
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
MXL_STATUS_E MxLWare_HYDRA_OEM_DeleteI2cAccessLock(UINT8 devId)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  oem_data_t *oemDataPtr = (oem_data_t *)0;

  oemDataPtr = (oem_data_t *)MxL_HYDRA_OEM_DataPtr[devId];
  if (oemDataPtr)
  {
      MsOS_DeleteMutex(oemDataPtr->Mutex);
  }  
  else
  {
     mxlStatus = MXL_FAILURE;
  }

  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_Lock
 *
 * @param[in]   devId           Device ID
 *
 * @author Mahee
 *
 * @date 04/11/2013 Initial release
 *
 * This API will should be used to lock access to device i2c access
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
MXL_STATUS_E MxLWare_HYDRA_OEM_Lock(UINT8 devId)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  oem_data_t *oemDataPtr = (oem_data_t *)0;

  oemDataPtr = (oem_data_t *)MxL_HYDRA_OEM_DataPtr[devId];
  if (oemDataPtr)
  {
      if(oemDataPtr->Mutex == -1)
      {
          printf("[MxL542]Warning: Mutex None\n");
          mxlStatus = MXL_SUCCESS;
      }
      else if(MsOS_ObtainMutex(oemDataPtr->Mutex, MxL524_MUTEX_TIMEOUT) == FALSE)
           mxlStatus = MXL_FAILURE;
  }  
  else
  {
    printf("Warning: oemDataPtr is NULL\n");
     mxlStatus = MXL_SUCCESS;
  }
  

  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_Unlock
 *
 * @param[in]   devId           Device ID
 *
 * @author Mahee
 *
 * @date 04/11/2013 Initial release
 *
 * This API will should be used to unlock access to device i2c access
 *
 * @retval MXL_SUCCESS            - OK
 * @retval MXL_FAILURE            - Failure
 * @retval MXL_INVALID_PARAMETER  - Invalid parameter is passed
 *
 ************************************************************************/
MXL_STATUS_E MxLWare_HYDRA_OEM_Unlock(UINT8 devId)
{
  MXL_STATUS_E mxlStatus = MXL_SUCCESS;
  oem_data_t *oemDataPtr = (oem_data_t *)0;

  oemDataPtr = (oem_data_t *)MxL_HYDRA_OEM_DataPtr[devId];

  if (oemDataPtr)
  {
      if(oemDataPtr->Mutex == -1)
      {
          printf("[MxL542]Warning: Mutex None\n");
          mxlStatus = MXL_SUCCESS;
      }
      else if(MsOS_ReleaseMutex(oemDataPtr->Mutex) == FALSE)
               mxlStatus = MXL_FAILURE;
  }  
  else
  {
      printf("Warning: oemDataPtr is NULL\n");
      mxlStatus = MXL_SUCCESS;
  }


  return mxlStatus;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_MemAlloc
 *
 * @param[in]   sizeInBytes
 *
 * @author Sateesh
 *
 * @date 04/23/2015 Initial release
 *
 * This API shall be used to allocate memory.
 *
 * @retval memPtr                 - Memory Pointer
 *
 ************************************************************************/
void* MxLWare_HYDRA_OEM_MemAlloc(UINT32 sizeInBytes)
{
  void *memPtr = NULL;
  sizeInBytes = sizeInBytes;

  memPtr = (void*)malloc(sizeInBytes * sizeof(UINT8));
  
  return memPtr;
}

/**
 ************************************************************************
 *
 * @brief MxLWare_HYDRA_OEM_MemFree
 *
 * @param[in]   memPtr
 *
 * @author Sateesh
 *
 * @date 04/23/2015 Initial release
 *
 * This API shall be used to free memory.
 *
 *
 ************************************************************************/
void MxLWare_HYDRA_OEM_MemFree(void *memPtr)
{
  memPtr = memPtr;

  free(memPtr);

}
