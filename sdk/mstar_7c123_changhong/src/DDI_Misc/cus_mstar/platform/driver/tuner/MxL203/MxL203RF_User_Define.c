/*
 
 Driver APIs for MxL203RF Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL_User_Define.c

 */
     
#include "MsCommon.h"
#include "MsOS.h"
#include "Board.h"
#include "drvIIC.h"
#include "Frontend_Enum_Device.h"

#include "MxL203RF_User_Define.h"
#include "MxL203RF_Common.h"


#if IF_THIS_TUNER_INUSE(TUNER_MXL203)

typedef enum{
    I2C_OK            =   0,
    I2C_ERR_BYPASS        =   1,
    I2C_ERR_WRITE        =   2,
    I2C_ERR_READ        =   3,
    I2C_ERR_OTHERS        =   10
}I2C_ERR_MSG;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//					I2C Functions (implement by customer)				   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/******************************************************************************
**
**  Name: MxL_I2C_Write
**
**  Description:    I2C write operations
**
**  Parameters:    	
**					DeviceAddr	- MxL203RF Device address
**					pArray		- Write data array pointer
**					count		- total number of array
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
static MS_BOOL IIC_READ(MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{

    if ( FALSE == MDrv_IIC_ReadBytes(FRONTEND_TUNER_PORT,u8SlaveID, u8AddrNum, paddr, u16size, pu8data))
    {
        printf("devCOFDM_PassThrough I2C_Read Error \n");
        return FALSE;
    }

    return TRUE;

}

static MS_BOOL IIC_WRITE(MS_U8 u8SlaveID, MS_U8* paddr,MS_U8 u8AddrNum, MS_U8* pu8data,  MS_U16 u16size)
{
    if ( FALSE == MDrv_IIC_WriteBytes(FRONTEND_TUNER_PORT,u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThrough I2C_WriteBytes Error \n");
        return FALSE;
    }
    return TRUE;
}

UINT32 MxL_I2C_Write(UINT8 DeviceAddr, UINT8* pArray, UINT32 count)
{    
    MS_U32    u8Error              = I2C_OK;
    

    if (IIC_WRITE(DeviceAddr, NULL, 0, pArray, count) == FALSE)
    {
        printf("[%s][%d] fail \n", __FUNCTION__,__LINE__);
        u8Error = I2C_ERR_WRITE;
    }
    else
    {
        u8Error =  I2C_OK;
    }
	return MxL_OK;	
}

/******************************************************************************
**
**  Name: MxL_I2C_Read
**
**  Description:    I2C read operations
**
**  Parameters:    	
**					DeviceAddr	- MxL203RF Device address
**					Addr		- register address for read
**					*Data		- data return
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
UINT32 MxL_I2C_Read(UINT8 DeviceAddr, UINT8 Addr, UINT8* mData)
{
    MS_U8     u8Addr[2]         = {0xFB, Addr};
    MS_U32    u8Error              = I2C_OK;

    if (IIC_READ(DeviceAddr, u8Addr, 2, mData, 1) == FALSE)
    {
        printf("[%s][%d] fail \n", __FUNCTION__,__LINE__);
        u8Error =  I2C_ERR_READ;
    }
    else
    {
        u8Error =  I2C_OK;
    }

    return u8Error;
}

/******************************************************************************
**
**  Name: MxL_Delay
**
**  Description:    Delay function in milli-second
**
**  Parameters:    	
**					mSec		- milli-second to delay
**
**  Returns:        0
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
void MxL_Delay(UINT32 mSec)
{
    MsOS_DelayTask((MS_U32)mSec);
}
#endif
