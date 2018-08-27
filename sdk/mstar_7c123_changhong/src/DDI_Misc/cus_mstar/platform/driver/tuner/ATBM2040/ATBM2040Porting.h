/********************************************************************************************************
*File name: ATBM2040Porting.h
*Description: Headerfile of ATBM2040Porting.c. The functions in this file are called by ATBM2040 SDK, and they should be implemented
*           according to specific system platform.
*
*********************************************************************************************************/

#ifndef __ATBM2040PORTING_H__
#define __ATBM2040PORTING_H__
#include "ATBM2040Api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************
* Function: ATBM2040I2CRead
* Description: Read I2C data. User need to implement this function addcording to system platform.
*           Detail of ATBM2040 I2C format is described in software guide document.
* Input:    I2CAddr -- I2C slave address
*       BaseReg -- Base register address
*       OffReg -- Offset register address
*       pData -- Pointer to register value
*       Length - Buffer length of 'pData' in byte
* Output: N/A
* Retrun: ATBM2040_ERROR_NO_ERROR -- no error; others refer to ATBM2040_ERROR_e
********************************************************************/
ATBM2040_ERROR_e ATBM2040I2CRead(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 *pData, UINT32 Length);

/********************************************************************
* Function: ATBM2040I2CWrite
* Description: Write I2C data. User need to implement this function addcording to system platform.
*           Detail of ATBM2040 I2C format is described in software guide document.
* Input:    I2CAddr -- I2C slave address
*       BaseReg -- Base register address
*       OffReg -- Offset register address
*       pData -- Register value
*       Length - Buffer length of 'pData' in byte
* Output: N/A
* Retrun: ATBM2040_ERROR_NO_ERROR -- no error; others refer to ATBM2040_ERROR_e
********************************************************************/
ATBM2040_ERROR_e ATBM2040I2CWrite(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 *pData, UINT32 Length);

/********************************************************************
* Function: ATBM2040Delay
* Description: Delay. User need to implement this function addcording to system platform.
*
* Input:    TimeoutUs -- timeout in us to delay
*
* Output: N/A
* Retrun: N/A
********************************************************************/
void ATBM2040Delay(UINT32 TimeoutUs);


/********************************************************************
* ATBM2040 debug output control.
* If define 'ATBM2040_DEBUG_PRINT' to '0', debug output will be closed;
*   otherwise,debug output will be opened.
* The default implement of 'ATBM2040Print' is 'printf', and it can be changed according to
*   system platform.
********************************************************************/
#define ATBM2040_DEBUG_PRINT (1)

#if ATBM2040_DEBUG_PRINT
#define ATBM2040Print(x) printf x
#else
#define ATBM2040Print(...)  /* empty */
#endif


#ifdef __cplusplus
}
#endif

#endif /*__ATBM2040PORTING_H__*/

