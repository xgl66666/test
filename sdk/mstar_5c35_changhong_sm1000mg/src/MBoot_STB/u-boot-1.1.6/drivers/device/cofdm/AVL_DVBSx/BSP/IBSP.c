/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///
/// @file
/// @brief Implements the functions declared in IBSP.h.
///
//$Revision: 563 $
//$Date: 2008-09-24 17:24:29 -0400 (Wed, 24 Sep 2008) $
//
#include "IBSP.h"
#include "../AVL_DVBSx_Board.h"
#include "sys/drv/drvHWI2C.h"
#include <sys/drv/drvIIC.h>
#include "../../../../../include/prana2/stdio.h"
#define PIU_IIC_0                   0x0010
#define FRONTEND_TUNER_PORT         E_HWI2C_PORT0_1
#define FRONTEND_TUNER_CTRL         PIU_IIC_0
//#define FRONTEND_TUNER_PORT         E_HWI2C_PORT0_1

/// Please use the following table of PLL configurations if 55 Msps symbol rate is desired at the demod end.

///	{503,  1, 6, 4, 2,  4000, 12600, 16800, 25200} < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{447,  1, 6, 4, 2,  4500, 12600, 16800, 25200} < Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  4, 6, 4, 2, 10000, 12600, 16800, 25200} < Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  7, 6, 4, 2, 16000, 12600, 16800, 25200} < Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{111,  2, 6, 4, 2, 27000, 12600, 16800, 25200} < Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz

/// Feel free to contact Availink Support staff for further infromation on this.

/// Users can remove unused elements from the following array to reduce the SDK footprint size.
const struct AVL_DVBSx_PllConf pll_conf[] =
{
	 {503,  1, 7, 4, 2,  4000, 11200, 16800, 25200} ///< Reference clock 4 MHz,   Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{447,  1, 7, 4, 2,  4500, 11200, 16800, 25200} ///< Reference clock 4.5 MHz, Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  4, 7, 4, 2, 10000, 11200, 16800, 25200} ///< Reference clock 10 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  7, 7, 4, 2, 16000, 11200, 16800, 25200} ///< Reference clock 16 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{111,  2, 7, 4, 2, 27000, 11200, 16800, 25200} ///< Reference clock 27 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz

	// The following set of PLL configuration at different reference clock frequencies refer to demod operation
	// in high performance mode.
	,{566,  1, 7, 4, 2,  4000, 12600, 18900, 28350} /// < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{503,  1, 7, 4, 2,  4500, 12600, 18900, 28350} ///< Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{566,  4, 7, 4, 2, 10000, 12600, 18900, 28350} ///< Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{566,  7, 7, 4, 2, 16000, 12600, 18900, 28350} ///< Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{377,  8, 7, 4, 2, 27000, 12600, 18900, 28350} ///< Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz

};
const AVL_uint16 pll_array_size = sizeof(pll_conf)/sizeof(struct AVL_DVBSx_PllConf);

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS )
{
      SysDelay(uiMS);
	return(AVL_DVBSx_EC_OK);
}
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead(  AVL_uint16 uiSlaveAddr,  AVL_puchar pucBuff, AVL_puint16 puiSize )
{
       if(MDrv_IIC_ReadBytes(uiSlaveAddr, 0, NULL, (*puiSize), pucBuff))
       {
            return(AVL_DVBSx_EC_OK);
       }
       printf("\n AVL6211 I2C Read failed Slave=0x%X",uiSlaveAddr);
       return(AVL_DVBSx_EC_I2CFail);
}
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite(  AVL_uint16 uiSlaveAddr,  AVL_puchar pucBuff,  AVL_puint16  puiSize )
{
       if(MDrv_IIC_WriteBytes(uiSlaveAddr, 0, NULL, (*puiSize), pucBuff))
       {
	     return(AVL_DVBSx_EC_OK);
       }
       printf("\n AVL6211 I2C Write failed Slave=0x%X",uiSlaveAddr);
       return(AVL_DVBSx_EC_I2CFail);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore )
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore )
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore )
{
	return(AVL_DVBSx_EC_OK);
}
