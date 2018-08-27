/********************************************************************************************************
File:ATBM2040Porting.c
Description:
    ATBM2040 Tuner porting APIs which need to be implemented by user.

*********************************************************************************************************/
#include <stdio.h>
#include "ATBM2040Porting.h"
#include "drvIIC.h"
#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_ATBM2040)

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
ATBM2040_ERROR_e ATBM2040I2CRead(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 *pData, UINT32 Length)
{
    MS_IIC_PORT hwi2c_port;
    UINT8 pArray[2];
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
        ATBM2040Print(("hwi2c_port number exceeds limitation\n"));
        return ATBM2040_ERROR_I2C_FAILED;
    }
    pArray[0] = BaseReg;
    pArray[1] = OffReg;
    switch (u8TunerIndex)
    {
        case 0:
        {
            if (MDrv_IIC_Read(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {
                ATBM2040Print(("i2c read err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        case 1:
        {
            if (MDrv_IIC1_Read(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {

                ATBM2040Print(("i2c1 read err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        case 2:
        {
            if (MDrv_IIC_Read(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {
                ATBM2040Print(("i2c read err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        case 3:
        {
            if (MDrv_IIC1_Read(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {

                ATBM2040Print(("i2c1 read err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        default:
        {
            if (MDrv_IIC_Read(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {
                ATBM2040Print(("i2c read err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
    } //end switch(u8TunerIndex)

    return ATBM2040_NO_ERROR;
}

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
ATBM2040_ERROR_e ATBM2040I2CWrite(UINT8 u8TunerIndex, UINT8 I2CAddr, UINT8 BaseReg, UINT8 OffReg, UINT8 *pData, UINT32 Length)
{
    MS_IIC_PORT hwi2c_port;
    UINT8 pArray[2];
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
        ATBM2040Print(("hwi2c_port number exceeds limitation\n"));
        return ATBM2040_ERROR_I2C_FAILED;
    }
    pArray[0] = BaseReg;
    pArray[1] = OffReg;
    switch (u8TunerIndex)
    {
        case 0:
        {
            if (MDrv_IIC_Write(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {
                ATBM2040Print(("i2c write err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        case 1:
        {
            if (MDrv_IIC1_Write(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {

                ATBM2040Print(("i2c1 write err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        case 2:
        {
            if (MDrv_IIC_Write(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {
                ATBM2040Print(("i2c write err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        case 3:
        {
            if (MDrv_IIC1_Write(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {

                ATBM2040Print(("i2c1 write err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
        default:
        {
            if (MDrv_IIC_Write(I2CAddr, pArray, 2, pData, Length) != 0)
            {
                MsOS_DelayTask(1);
            }
            else
            {
                ATBM2040Print(("i2c write err\n"));
                return ATBM2040_ERROR_I2C_FAILED;
            }
            break;
        }
    } //end switch(u8TunerIndex)

    return ATBM2040_NO_ERROR;

}

/********************************************************************
* Function: ATBM2040Delay
* Description: Delay. User need to implement this function addcording to system platform.
*
* Input:    TimeoutUs -- timeout in us to delay
*
* Output: N/A
* Retrun: N/A
********************************************************************/
void ATBM2040Delay(UINT32 TimeoutUs)
{
    MsOS_DelayTaskUs(TimeoutUs);

}

#endif

