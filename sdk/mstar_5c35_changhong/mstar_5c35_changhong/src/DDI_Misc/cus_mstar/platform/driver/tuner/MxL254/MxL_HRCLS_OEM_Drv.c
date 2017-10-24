/*******************************************************************************
 * Example MxL_HRCLS_OEM_Drv.c
 ******************************************************************************/
#include "Board.h"
#if(FRONTEND_TUNER_TYPE == TUNER_MXL254)

#ifndef S_SPLINT_S

//#include <sys/time.h>
//#include <unistd.h>
#include <string.h>

#include "MxL_HRCLS_Common.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "drvIIC.h"

//extern MS_BOOL MDrv_IIC_Write(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
//extern MS_BOOL MDrv_IIC_Read(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
//extern MS_BOOL MDrv_IIC1_Write(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);
//extern MS_BOOL MDrv_IIC1_Read(MS_U8 u8SlaveID, MS_U8 *pu8Addr, MS_U8 u8AddrSize, MS_U8 *pu8Buf, MS_U32 u32BufSize);


#define MXL254_SLAVE_ID 0x50
/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_Reset
--| 
--| DESCRIPTION   : This function resets MxL_HRCLS through Reset Pin
--| PARAMETERS    : devId - MxL_HRCLS device id
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_Reset(UINT8 devId)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  // !!! FIXME !!!
  // OEM should toggle reset pin of MxL_HRCLS specified by I2C Slave Addr
  
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_WriteRegister
--|
--| DESCRIPTION   : This function does I2C write operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register address of MxL_HRCLS to write.
--|                 regData - Data to write to the specified address.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 regData)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  HWI2C_PORT hwi2c_port;
  
  UINT8 RegAddr[2], RegData[2], bret = 1;
  
  RegAddr[0] = (UINT8)((regAddr & 0xff00) >> 8);
  RegAddr[1] = (UINT8)(regAddr & 0xff);
  RegData[0] = (UINT8)((regData & 0xff00) >> 8);
  RegData[1] = (UINT8)(regData & 0xff);
  // !!! FIXME !!!
  // OEM should implement I2C write protocol that complies with MxL_HRCLS I2C
  // format.

  // 16bit Register Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |regAddr(H)| |regAddr(L)| |regData(H)| |regData(L)| |P|
  // +------+-+-----+-+-+----------+-+----------+-+----------+-+----------+-+-+
  // |SLAVE |         |A|          |A|          |A|          |A|          |A| |
  // +------+---------+-+----------+-+----------+-+----------+-+----------+-+-+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK), P(Stop condition)
  hwi2c_port = getI2CPort(devId)/8;
  switch(hwi2c_port)
  {
     case 0:   
        bret &= MDrv_IIC_Write(MXL254_SLAVE_ID<<1, RegAddr, 2, RegData, 2);
        if(!bret)
           status = MXL_FAILURE; 
        break;
     case 1:  
        bret &= MDrv_IIC1_Write(MXL254_SLAVE_ID<<1, RegAddr, 2, RegData, 2);
        if(!bret)
           status = MXL_FAILURE; 
        break;
     default:
        status = MXL_FAILURE;
        break;
  }
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_ReadRegister
--|
--| DESCRIPTION   : This function does I2C read operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register address of MxL_HRCLS to read.
--|                 dataPtr - Data container to return 16 data.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_ReadRegister(UINT8 devId, UINT16 regAddr, UINT16 *dataPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  HWI2C_PORT hwi2c_port;
  UINT8 RegAddr[4], bret = 1;
  UINT8 RegData[2];

  RegAddr[0] = 0xFF;
  RegAddr[1] = 0xFB;
  RegAddr[2] = (UINT8)((regAddr & 0xff00) >> 8);
  RegAddr[3] = (UINT8)(regAddr & 0xff);

  // !!! FIXME !!!
  // OEM should implement I2C read protocol that complies with MxL_HRCLS I2C
  // format.

  // 16 Register Read Protocol:
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFF| |0xFB| |regAddr(H)| |regAddr(L)| |P|
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |SLAVE |         |A|    |A|    |A|          |A|          |A| |
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // +------+-+-----+-+-+-------+--+-------+--+-+
  // |MASTER|S|SADDR|R| |       |MA|       |MN|P|
  // +------+-+-----+-+-+-------+--+-------+--+-+
  // |SLAVE |         |A|Data(H)|  |Data(L)|  | |
  // +------+---------+-+-------+--+----------+-+
  // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK), P(Stop condition)
  //printf("I2C_Read MXL254 slave ID = 0x%x\n", MXL254_SLAVE_ID);
  hwi2c_port = getI2CPort(devId)/8;
  switch(hwi2c_port)
  {
     case 0: 
        bret &= MDrv_IIC_Read(MXL254_SLAVE_ID<<1, RegAddr, 4, RegData, 2);
        if(!bret)
           status = MXL_FAILURE;           
        break;
     case 1:  
        bret &= MDrv_IIC1_Read(MXL254_SLAVE_ID<<1, RegAddr, 4, RegData, 2);
        if(!bret)
           status = MXL_FAILURE; 
        break; 
     default:
        status = MXL_FAILURE;
        break;
  }
  *dataPtr = ((UINT16)RegData[0] << 8) | ((UINT16)RegData[1]);
  return status;
}
  
/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_WriteBlock
--|
--| DESCRIPTION   : This function does I2C block write operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register address of MxL_HRCLS to start a block write.
--|                 bufSize - The number of bytes to write
--|                 bufPtr - Data bytes to write to the specified address.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_WriteBlock(UINT8 devId, UINT16 regAddr, UINT16 bufSize, UINT8 *bufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  HWI2C_PORT hwi2c_port;
  UINT8 RegAddr[2],bret = 1;
  
  RegAddr[0] = (UINT8)((regAddr & 0xff00) >> 8);
  RegAddr[1] = (UINT8)(regAddr & 0xff);

  // !!! FIXME !!!
  // OEM should implement I2C block write protocol that complies with MxL_HRCLS I2C
  // format.

  // Block Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+---------+-+---+-----------------+-+-+
  // |MASTER|S|SADDR|W| |regAddr(H)| |regAddr(L)| |bufPtr[0]| |   |bufPtr[Bufsize-1]| |P|
  // +------+-+-----+-+-+----------+-+----------+-+---------+-+...+-----------------+-+-+
  // |SLAVE |         |A|          |A|          |A|         |A|   |                 |A| |
  // +------+---------+-+----------+-+---- -----+-+---------+-+---+-----------------+-+-+
  // Legends: SADDR(I2c slave address), S(Start condition), A(Ack), P(Stop condition)
  hwi2c_port = getI2CPort(devId)/8;
  switch(hwi2c_port)
  {
     case 0: 
        bret &= MDrv_IIC_Write(MXL254_SLAVE_ID<<1, RegAddr, 2, bufPtr, bufSize);
        if(!bret)
           status = MXL_FAILURE; 
        break;
     case 1:  
        bret &= MDrv_IIC1_Write(MXL254_SLAVE_ID<<1, RegAddr, 2, bufPtr, bufSize);
        if(!bret)
           status = MXL_FAILURE; 
        break;
     default:
        status = MXL_FAILURE;
        break;
  }
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_ReadBlock
--|
--| DESCRIPTION   : This function does I2C block read operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register Address to start a block read
--|                 readSize - The number of bytes to read
--|                 bufPtr - Container to hold readback data
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlock(UINT8 devId, UINT16 regAddr, UINT16 readSize, UINT8 *bufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  HWI2C_PORT hwi2c_port;
  UINT8 RegAddr[4], bret = 1;

  RegAddr[0] = 0xFF;
  RegAddr[1] = 0xFD;
  RegAddr[2] = (UINT8)((regAddr & 0xff00) >> 8);
  RegAddr[3] = (UINT8)(regAddr & 0xff);
  // !!! FIXME !!!
  // OEM should implement I2C block read protocol that complies with MxL_HRCLS I2C
  // format.
  
  // Block Read Protocol (n bytes of data):
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFF| |0xFD| |regAddr(H)| |regAddr(L)| |P|
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |SLAVE |         |A|    |A|    |A|          |A|          |A| |
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // +------+-+-----+-+-------+-+-----+-+-----+-+-----+-+-+
  // |MASTER|S|SADDR|R|       |A|     |A|       |     |N|P|
  // +------+-+-----+-+-+-----+-+-----+-+  ...  +-----+-+-+
  // |SLAVE |         |A|DATA1| |DATA2| |       |DATAn|   |
  // +------+---------+-+-----+-+-----+-+-----+-+-----+---+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Acknowledgement), N(NACK), P(Stop condition)
  hwi2c_port = getI2CPort(devId)/8;
  switch(hwi2c_port)
  {
     case 0:   
        bret &= MDrv_IIC_Read(MXL254_SLAVE_ID<<1, RegAddr, 4, bufPtr, readSize);
        if(!bret)
           status = MXL_FAILURE;
        //else
        //{
           //printf("\n");
           //for(i=0;i<readSize;i++)
           // printf(" 0x%02x ", *(bufPtr + i));
           //printf("\n");
        //}
        break;
     case 1:  
        bret &= MDrv_IIC1_Read(MXL254_SLAVE_ID<<1, RegAddr, 4, bufPtr, readSize);
        if(!bret)
           status = MXL_FAILURE; 
        break;
     default:
        status = MXL_FAILURE;
        break;
  }

  return status;
}

MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlockExt(UINT8 devId, UINT16 cmdId, UINT16 offset, UINT16 readSize, UINT8 *bufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  HWI2C_PORT hwi2c_port;
  UINT8 RegAddr[6], bret = 1;

  RegAddr[0] = 0xFF;
  RegAddr[1] = 0xFD;
  RegAddr[2] = (UINT8)((cmdId & 0xff00) >> 8);
  RegAddr[3] = (UINT8)(cmdId & 0xff);
  RegAddr[4] = (UINT8)((offset & 0xff00) >> 8);
  RegAddr[5] = (UINT8)(offset & 0xff);
  // !!! FIXME !!!
  // OEM should implement I2C block read protocol that complies with MxL_HRCLS I2C
  // format.
  
  // Block Read Protocol (n bytes of data):
  // +------+-+-----+-+-+----+-+----+-+--------+-+--------+-+----------+-+---------+-+-+
  // |MASTER|S|SADDR|W| |0xFF| |0xFD| |cmdId(H)| |cmdId(L)| |offset(H) | |offset(L)| |P|
  // +------+-+-----+-+-+----+-+----+-+--------+-+--------+-+----------+-+---------+-+-+
  // |SLAVE |         |A|    |A|    |A|        |A|        |A|          |A|         |A| |
  // +------+-+-----+-+-+----+-+----+-+--------+-+--------+-+----------+-+---------+-+-+
  // +------+-+-----+-+-------+-+-----+-+-----+-+-----+-+-+
  // |MASTER|S|SADDR|R|       |A|     |A|       |     |N|P|
  // +------+-+-----+-+-+-----+-+-----+-+  ...  +-----+-+-+
  // |SLAVE |         |A|DATA1| |DATA2| |       |DATAn|   |
  // +------+---------+-+-----+-+-----+-+-----+-+-----+---+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Acknowledgement), N(NACK), P(Stop condition)
  hwi2c_port = getI2CPort(devId)/8;
  switch(hwi2c_port)
  {
     case 0:  
        bret &= MDrv_IIC_Read(MXL254_SLAVE_ID<<1, RegAddr, 6, bufPtr, readSize);
        if(!bret)
           status = MXL_FAILURE;           
        break;
     case 1:  
        bret &= MDrv_IIC1_Read(MXL254_SLAVE_ID<<1, RegAddr, 6, bufPtr, readSize);
        if(!bret)
           status = MXL_FAILURE; 
        break; 
     default:
        status = MXL_FAILURE;
        break;
  }
  return status;
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_LoadNVRAMFile
--|
--| DESCRIPTION   : Load MxLNVRAMFile
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_LoadNVRAMFile(UINT8 devId, UINT8 *bufPtr, UINT32 bufLen)
{
  MXL_STATUS_E status = MXL_FAILURE;

  // !!! FIXME !!! 
  // To be implemented for customer OEM platform
  return status;
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_SaveNVRAMFile
--|
--| DESCRIPTION   : Save MxLNVRAMFile
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_SaveNVRAMFile(UINT8 devId, UINT8 *bufPtr, UINT32 bufLen)
{
  MXL_STATUS_E status = MXL_FAILURE;
  // !!! FIXME !!! 
  // To be implemented for customer OEM platform

  return status;  
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_DelayUsec
--|
--| DESCRIPTION   : Delay in micro-seconds
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

void MxLWare_HRCLS_OEM_DelayUsec(UINT32 usec)
{
  // !!! FIXME !!! 
  // To be implemented for customer OEM platform
  //usleep(usec);
  MsOS_DelayTask((usec + 500) /1000);
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_GetCurrTimeInUsec
--|
--| DESCRIPTION   : Get current time in micro-seconds
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

void MxLWare_HRCLS_OEM_GetCurrTimeInUsec(UINT64* usecPtr)
{
  unsigned long long timeInUsec;
  
  timeInUsec = MsOS_GetSystemTime() * 1000;

  *usecPtr= (UINT64) timeInUsec;

  // return MXL_SUCCESS;
}

#endif
#endif

