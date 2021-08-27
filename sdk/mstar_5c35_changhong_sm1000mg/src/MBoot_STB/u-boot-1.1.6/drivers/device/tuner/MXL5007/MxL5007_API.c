/*

 Driver APIs for MxL5007 Tuner

 Copyright, Maxlinear, Inc.
 All Rights Reserved

 File Name:      MxL5007_API.c

 Version:    4.1.3
*/


//#include "MsCommon.h"
//#include "drvIIC.h"
//#include "Board.h"

#if (FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER)
#include <device/COFDM_Demodulator.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvIIC.h>
#include "device/MxL5007_API.h"
#include "device/MxL5007.h"

#undef DBG_TRACE
#define MS_DEBUG

#ifdef MS_DEBUG
#define DBG_TRACE(fmt, args...) printf(fmt, ##args)
#else
#define DBG_TRACE(fmt, args...) {}
#endif


extern void MsOS_DelayTask (MS_U32 u32Ms);

typedef enum{
	I2C_OK			=   0,
	I2C_ERR_BYPASS		=   1,
	I2C_ERR_WRITE		=   2,
	I2C_ERR_READ		=   3,
	I2C_ERR_OTHERS		=   10
}I2C_ERR_MSG;



//#define IIC_WRITE(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   devCOFDM_PassThroughI2C_WriteBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)
//#define IIC_READ(u8SlaveId, pu8Addr, u8AddrSize, pu8Buf, u32BufSize)   devCOFDM_PassThroughI2C_ReadBytes(u8SlaveId, u8AddrSize, pu8Addr, (MS_U16)u32BufSize, pu8Buf)


static MS_BOOL IIC_WRITE(MS_U8 u8SlaveID, MS_U8* paddr,MS_U8 u8AddrNum, MS_U8* pu8data,  MS_U16 u16size)
{

    if ( FALSE == MDrv_IIC_WriteBytes((MS_U16)u8SlaveID, u8AddrNum, paddr, u16size, pu8data) )
    {
        printf("devCOFDM_PassThroughI2C_WriteBytes Error \n");
        return FALSE;
    }

    MsOS_DelayTask(1); // delay 1ms

    return TRUE;


}


static MS_BOOL IIC_READ(MS_U8 u8SlaveID, MS_U8* paddr, MS_U8 u8AddrNum, MS_U8* pu8data, MS_U16 u16size)
{

    MDrv_IIC_ReadBytes(u8SlaveID, u8AddrNum, paddr, u16size, pu8data);

    return TRUE;

}

/******************************************************************************
**
**  Name: Tuner_Delay
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
void Tuner_Delay(MS_U32 mSec)
{
	 MsOS_DelayTask(mSec);
}

/******************************************************************************
**
**  Name: Tuner_I2C_Write
**
**  Description:    I2C write operations
**
**  Parameters:
**					DeviceAddr	- MxL5007 Device address
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
MS_U32  Tuner_I2C_Write(MS_U8 DeviceAddr, MS_U8* pArray, MS_U32 count)
{
	MS_U32	u8Error	  		= I2C_OK;


	if (IIC_WRITE(DeviceAddr, NULL, 0, pArray, count) == FALSE)
	{
		DBG_TRACE("%s fail \n", __FUNCTION__);
		u8Error = I2C_ERR_WRITE;
	}
	else
	{
		u8Error =  I2C_OK;
	}
    Tuner_Delay(1); //time for tuner setting

	return u8Error;
}

/******************************************************************************
**
**  Name: Tuner_I2C_Read
**
**  Description:    I2C read operations
**
**  Parameters:
**					DeviceAddr	- MxL5007 Device address
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
MS_U32 Tuner_I2C_Read(MS_U8 DeviceAddr, MS_U8 Addr, MS_U8* mData)
{
	MS_U8 	u8Addr[2] 		= {0xF8, Addr};
	MS_U32	u8Error	  		= I2C_OK;


	if (IIC_READ(DeviceAddr, u8Addr, 2, mData, 1) == FALSE)
	{
		DBG_TRACE("%s fail \n", __FUNCTION__);
		u8Error =  I2C_ERR_READ;
	}
	else
	{
		u8Error =  I2C_OK;
	}


	return u8Error;
}





//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Tuner Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
MxL_ERR_MSG MxL_Set_Register(MxL5007_TunerConfigS* myTuner, MS_U8 RegAddr, MS_U8 RegData)
{
	MS_U32 Status=0;
	MS_U8 pArray[2];
	pArray[0] = RegAddr;
	pArray[1] = RegData;
	Status = Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, pArray, 2);
	if(Status) return MxL_ERR_SET_REG;

	return MxL_OK;

}

MxL_ERR_MSG MxL_Get_Register(MxL5007_TunerConfigS* myTuner, MS_U8 RegAddr, MS_U8 *RegData)
{
	if(Tuner_I2C_Read((MS_U8)myTuner->I2C_Addr, RegAddr, RegData))
		return MxL_ERR_GET_REG;
	return MxL_OK;

}

MxL_ERR_MSG MxL_Soft_Reset(MxL5007_TunerConfigS* myTuner)
{
//	MS_U32 Status=0;
	MS_U8 reg_reset;
	reg_reset = 0xFF;
	if(Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, &reg_reset, 1))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Loop_Through_On(MxL5007_TunerConfigS* myTuner, MxL5007_LoopThru isOn)
{
	MS_U8 pArray[2];	// a array pointer that store the addr and data pairs for I2C write

	pArray[0]=0x04;
	if(isOn)
	 pArray[1]= 0x01;
	else
	 pArray[1]= 0x0;

	if(Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, pArray, 2))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Stand_By(MxL5007_TunerConfigS* myTuner)
{
	MS_U8 pArray[4];	// a array pointer that store the addr and data pairs for I2C write

	pArray[0] = 0x01;
	pArray[1] = 0x0;
	pArray[2] = 0x0F;
	pArray[3] = 0x0;

	if(Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, pArray, 4))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Wake_Up(MxL5007_TunerConfigS* myTuner)
{
	MS_U8 pArray[2];	// a array pointer that store the addr and data pairs for I2C write

	pArray[0] = 0x01;
	pArray[1] = 0x01;

	if(Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, pArray, 2))
		return MxL_ERR_OTHERS;

	if(MxL_Tuner_RFTune(myTuner, myTuner->RF_Freq_Hz, myTuner->BW_MHz))
		return MxL_ERR_RFTUNE;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Tuner_Init(MxL5007_TunerConfigS* myTuner)
{
	MS_U8 pArray[MAX_ARRAY_SIZE];	// a array pointer that store the addr and data pairs for I2C write
	MS_U32 Array_Size;							// a integer pointer that store the number of element in above array

	//Soft reset tuner
	DBG_TRACE("\r\n myTuner->I2C_Addr = %d",myTuner->I2C_Addr);
	if(MxL_Soft_Reset(myTuner))
		return MxL_ERR_INIT;

	//perform initialization calculation
	MxL5007_Init(pArray, &Array_Size, (MS_U8)myTuner->Mode, myTuner->IF_Diff_Out_Level, (MS_U32)myTuner->Xtal_Freq,
				(MS_U32)myTuner->IF_Freq, (MS_U8)myTuner->IF_Spectrum, (MS_U8)myTuner->ClkOut_Setting, (MS_U8)myTuner->ClkOut_Amp);

	//perform I2C write here
	DBG_TRACE("\r\n myTuner->I2C_Addr = %d",myTuner->I2C_Addr);
	if(Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, pArray, Array_Size))
		return MxL_ERR_INIT;

	return MxL_OK;
}


MxL_ERR_MSG MxL_Tuner_RFTune(MxL5007_TunerConfigS* myTuner, MS_U32 RF_Freq_Hz, MxL5007_BW_MHz BWMHz)
{
//	MS_U32 Status=0;
	MS_U8 pArray[MAX_ARRAY_SIZE];	// a array pointer that store the addr and data pairs for I2C write
	MS_U32 Array_Size;							// a integer pointer that store the number of element in above array

	//Store information into struc
	myTuner->RF_Freq_Hz = RF_Freq_Hz;
	myTuner->BW_MHz = BWMHz;

	//perform Channel Change calculation
	MxL5007_RFTune(pArray,&Array_Size,RF_Freq_Hz,BWMHz);

	//perform I2C write here
	if(Tuner_I2C_Write((MS_U8)myTuner->I2C_Addr, pArray, Array_Size))
		return MxL_ERR_RFTUNE;

#if 0
{
       int i;
       DBG_TRACE("\n---------------------------------\n")
       for(i=0;i<Array_Size;i++)
       {
            DBG_TRACE("%02x ",pArray[i]);
       }
	DBG_TRACE("\n---------------------------------\n")
}
#endif
	//wait for 3ms
	Tuner_Delay(3);

	return MxL_OK;
}

MxL5007_ChipVersion MxL_Check_ChipVersion(MxL5007_TunerConfigS* myTuner)
{
	MS_U8 Data;
	if(Tuner_I2C_Read((MS_U8)myTuner->I2C_Addr, 0xD3, &Data))
		return MxL_GET_ID_FAIL;

	switch(Data)
	{
	case 0x41: return MxL_5007T_V4; break;
	default: return MxL_UNKNOWN_ID;
	}
}

MxL_ERR_MSG MxL_RFSynth_Lock_Status(MxL5007_TunerConfigS* myTuner, BOOL* isLock)
{
	MS_U8 Data;
	*isLock = FALSE;
	if(Tuner_I2C_Read((MS_U8)myTuner->I2C_Addr, 0xD8, &Data))
		return MxL_ERR_OTHERS;
	Data &= 0x0C;

    printf("MxL_RFSynth_Lock_Status = 0x%x \n",Data);

	if (Data == 0x0C)
		*isLock = TRUE;  //RF Synthesizer is Lock
	return MxL_OK;
}

MxL_ERR_MSG MxL_REFSynth_Lock_Status(MxL5007_TunerConfigS* myTuner, BOOL* isLock)
{
	MS_U8 Data;
	*isLock = FALSE;
	if(Tuner_I2C_Read((MS_U8)myTuner->I2C_Addr, 0xD8, &Data))
		return MxL_ERR_OTHERS;
	Data &= 0x03;
	if (Data == 0x03)
		*isLock = TRUE;   //REF Synthesizer is Lock
	return MxL_OK;
}
#endif


