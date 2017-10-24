
/**
*SFILE****************************************************************
* FILE NAME       : 1412x.cpp
* AUTHOR          : Mobile Solution Division, Channel Team, Channel.cs
* MODERATOR   :
* ISSUED To   :
* ISSUED Day      : February 25,2009
* VERSION         : 1.0.0
* Init.Version    : Init_Air1.0.0
* FILE DESCRIPTION: S5H1412 VSB/QAM Porting Code
* ABBREVIATION    :

* Copyright (c) 2006 Samsung Electronics, Inc.
* All rights reserved.
*
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc("Confidential Information"). You
* Shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into SAMSUNG.
 **************************************************************EFILE*/
#include <common.h>
#include "sys/common/MsOS.h"
#include <sys/drv/drvGPIO.h>
#include <device/drvDemod.h>
#include <sys/drv/drvIIC.h>
#include <frontend_config.h>
#include <device/drvTuner.h>
#if(FRONTEND_DEMOD_TYPE == DEMOD_SamSung_S5H1411)
#include "S5H1411_Ver1.h"

#if(FRONTEND_TUNER_TYPE == TUNER_NMI120)

#include "../../tuner/NMI120/nmituner_api.h"//
#endif

/* *************************************************************** */
/*                                                                 */
/*              CODE PROVIDED BY Channel Team                      */
/*                                                                 */
/* *************************************************************** */
#define S25H1411_DBG_ENABLE 1
#ifdef S25H1411_DBG_ENABLE
#define DEMOD_DBG(_print)            _print
#else
#define DEMOD_DBG(_print)
#endif
static MS_S32                       _s32TaskId  = -1;
static MS_S32                       _s32MutexId;
#define COFDMDMD_STACK_SIZE          (2048)
#define COFDMDMD_MUTEX_TIMEOUT       (2000)

static EN_CONSTEL_MODE _eDemodMode;

#define INIT_1411_TABLE_NUMS (sizeof(tDTVS5H1411Tbl)/sizeof(SamsungChipInit_t))
static SamsungChipInit_t tDTVS5H1411Tbl[] =
{
    {0x00, 0x0071},
    {0x08, 0x0047},
    {0x1c, 0x0400},
    {0x1e, 0x0370},
    {0x1f, 0x342c},
    {0x24, 0x0231},
    {0x25, 0x1011},
    {0x26, 0x0f07},
    {0x27, 0x0f04},
    {0x28, 0x070f},
    {0x29, 0x2820},
    {0x2a, 0x102e},
    {0x2b, 0x0220},
    {0x2e, 0x0d0e},
    {0x2f, 0x1013},
    {0x31, 0x171b},
    {0x32, 0x0e0f},
    {0x33, 0x0f10},
    {0x34, 0x170e},
    {0x35, 0x4b10},
    {0x36, 0x0f17},
    {0x38, 0x18E2},//IF=4.8M
    {0x39, 0x7317},//
    {0x3c, 0x1577},
    {0x3d, 0x081a},
    {0x3e, 0x77ee},
    {0x40, 0x1e09},
    {0x41, 0x0f0c},
    {0x42, 0x1f10},
    {0x4d, 0x0509},
    {0x4e, 0x0a00},
    {0x50, 0x0000},
    {0x5b, 0x0000},
    {0x5c, 0x0008},
    {0x57, 0x1101},
    {0x65, 0x007c},
    {0x68, 0x0512},
    {0x69, 0x0258},
    {0x70, 0x0004},
    {0x71, 0x0007},
    {0x76, 0x00a9},
    {0x78, 0x3141},
    {0x7a, 0x3141},
    {0xb3, 0x8003},
    {0xb5, 0xa6bb},
    {0xb6, 0x0609},
    {0xb7, 0x2f06},
    {0xb8, 0x003f},
    {0xb9, 0x2700},
    {0xba, 0xfac8},
    {0xbe, 0x1003},
    {0xbf, 0x103f},
    {0xce, 0x2000},
    {0xcf, 0x0800},
    {0xd0, 0x0800},
    {0xd1, 0x0400},
    {0xd2, 0x0800},
    {0xd3, 0x2000},
    {0xd4, 0x3000},
    {0xdb, 0x4a9b},
    {0xdc, 0x1000},
    {0xde, 0x0001},
    {0xdf, 0x0000},
    {0xe3, 0x0301},
};


#define INIT_1411_QAM_TABLE_NUMS (sizeof(tDTVS5H1411_QAM_Tbl)/sizeof(SamsungChipInit_t))
static SamsungChipInit_t tDTVS5H1411_QAM_Tbl[] =
{
    {0xf3, 0x0000},
    {0xf3, 0x0001},
    {0x08, 0x0600},
    {0x18, 0x4201},
    {0x1e, 0x6476},
    {0x21, 0x0830},
    {0x0c, 0x5679},
    {0x0d, 0x579b},
    {0x24, 0x0102},
    {0x31, 0x7488},
    {0x32, 0x0a08},
    {0x3d, 0x8689},
    {0x41, 0x0f98},
    {0x49, 0x0048},
    {0x57, 0x2012},
    {0x5d, 0x7676},
    {0x04, 0x0400},
    {0x58, 0x00c0},
    {0x5B, 0x0100},

};

static MS_BOOL _IIC_Write2Bytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addr, MS_U16 u16data)
{
    MS_U8 data[2];
    MS_BOOL ret;
    data[0] = (u16data>>8)&0xFF;//u16data&0xFF;
    data[1] = u16data&0xFF;
    ///return MDrv_IIC_Write(u16BusNumSlaveID,&u8addr,0,data,2);
    ret = MDrv_IIC_WriteBytes(u16BusNumSlaveID,1,&u8addr,2,data);
    if(!ret)
    {
        printf("S5H1411 IIC WRITE FAILED\n");
        return FALSE;
    }
    else
        return TRUE;
}

static MS_U16 _IIC_Read2Bytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addr)
{
    MS_U8 u8data[2];
    MS_U16 u16data;
    MDrv_IIC_ReadBytes(u16BusNumSlaveID,1,&u8addr,2,u8data);
    u16data = (u8data[1]|(u8data[0]<<8));
    return u16data;
}

///////////////// ///////////////// ///////////////////
///////////////// SEMCO Tuner Function ////////////////
///////////////// ///////////////// ///////////////////

/*********************************************************************************************************
* S5H1411 Initialization Driver
* Conditions:
* Version:Init_Air 4.5
* Description:
* This function is called to initialize the register values.
* This function must be called once to run the system properly.
* After this function is called, the VSB mode is set as a default.
**********************************************************************************************************/

BOOLEAN Sec_1411xInitialize(void)
{
    BOOLEAN result = FALSE;
    U8 u8Loop;

    for(u8Loop=0; u8Loop<INIT_1411_TABLE_NUMS; u8Loop++)
	    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, tDTVS5H1411Tbl[u8Loop].Reg, tDTVS5H1411Tbl[u8Loop].Value);

    for(u8Loop=0; u8Loop<INIT_1411_QAM_TABLE_NUMS; u8Loop++)
	    result |= _IIC_Write2Bytes(IIC_1411x_QAM_RegId, tDTVS5H1411_QAM_Tbl[u8Loop].Reg, tDTVS5H1411_QAM_Tbl[u8Loop].Value);

    result = _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf7, 0x0000);
    result = _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf7, 0x0001);


    return result;
}

/*********************************************************************************************************
* S5H1411 Soft reset Driver
* Conditions: When a channel is changed or LNA is on/off, this function is called.
* Description:
* This function is called in order to reset all software conditions.
* But it does not reset register values.

* Addr: 0xf7
* Value: 0x00 -> Chip Reset Start
* Value: 0x01 -> Chip Reset End
**********************************************************************************************************/
BOOLEAN Sec_1411xSoftReset(void)
{
	BOOLEAN result = FALSE;
	result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf7, 0x0000);
	//MDrv_Timer_Delayms(5); //leo_1218
	result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf7, 0x0001);
	return result;
}

/*********************************************************************************************************
* S5H1411 Repeater Control Driver
* Conditions:
* Description:
* This function is used to set the bypass mode or normal mode. (Repeater Enable or Disable)
* In the bypass mode, the host PC controls a channel chip first and the channel chip controls
* a tuner using GPIO.
* Addr: 0xf5
* Value: 0x00 -> Tuner by pass close (Repeater Disable) ? Default mode
* Value: 0x01 -> Tuner by pass open (Repeater Enable)
**********************************************************************************************************/
BOOLEAN Sec_1411xRepeaterEnable(void)
{
	return _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf5, 0x0001);
}
BOOLEAN Sec_1411xRepeaterDisable(void)
{
	return _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf5, 0x0000);
}


/**************************************************************************************************
S5H1411 VSB, QAM mode change Driver
* Conditions:
* Description: These functions set an 8-VSB & QAM (64/256) mode
* Addr: 0xf6
* Value: 0x00 -> VSB mode (Default)
* Value: 0x01 -> QAM mode
* Addr: 0x16
* Value: 0x1100 -> QAM (Detect 64/256 QAM manually)
* Value: 0x1101 -> QAM (Detect 64/256 QAM automatically)
* Addr: 0x17
* Value: 0x0101 -> 64 QAM (Manual mode)
* Value: 0x0111 -> 256 QAM (Manual mode)
*************************************************************************************************/
BOOLEAN Sec_1411xVsbMode(void)
{
	BOOLEAN result = FALSE;
	result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0x00, 0x0071);
	result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf6, 0x0000);
	result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xcd, 0x00f1);
	return result;
}

BOOLEAN Sec_1411xQamMode(void)
{
    BOOLEAN result = FALSE;
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0x00, 0x0171);
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf6, 0x0001);
    result |= _IIC_Write2Bytes(IIC_1411x_QAM_RegId, 0x16, 0x1101);
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xcd, 0x00f0);
    return result;
}

BOOLEAN Sec_1411x64QamMode(void)
{
    BOOLEAN result = FALSE;
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0x00, 0x0171);
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf6, 0x0001);
    result |= _IIC_Write2Bytes(IIC_1411x_QAM_RegId, 0x16, 0x1100);
    result |= _IIC_Write2Bytes(IIC_1411x_QAM_RegId, 0x17, 0x0101);
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xcd, 0x00f0);
    return result;
}

BOOLEAN Sec_1411x256QamMode(void)
{
    BOOLEAN result = FALSE;
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0x00, 0x0171);
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf6, 0x0001);
    result |= _IIC_Write2Bytes(IIC_1411x_QAM_RegId, 0x16, 0x1100);
    result |= _IIC_Write2Bytes(IIC_1411x_QAM_RegId, 0x17, 0x0111);
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xcd, 0x00f0);
    return result;
}

/**************************************************************************************************
S5H1411 Get Mode Driver
* Conditions:
* Description: This function get a mode(VSB/QAM) of S5H1411.
* Addr: 0xf6
* Value: 0x00 -> VSB mode (Default)
* Value: 0x01 -> QAM mode
*************************************************************************************************/
U16 Sec_1411xGetMode(void)
{
    U16 mode;
    mode = _IIC_Read2Bytes(IIC_1411x_TOP_RegId, 0xf6);
    return mode;
}

/**************************************************************************************************
S5H1411 Get QAM Mode Driver
* Conditions:
* Description: This function get a QAM mode(64/256).
* Addr: 0xf0
* Value: 0x01 -> 256QAM mode
* Value: 0x00 -> 64QAM mode
*************************************************************************************************/
U16 Sec_1411xGetQAMMode(void)
{
    U16 qam_mode;
    qam_mode = _IIC_Read2Bytes(IIC_1411x_TOP_RegId, 0xf0) & 0x0001;
    return qam_mode;
}

/*************************************************************************************************
* S5H1411 Sleep mode change Driver
* Conditions: VSB/QAM mode
* Description:
* The software-based power down function is as follows.
* This function is called in order to set a sleep on mode or sleep off mode.
* Addr: 0xf4
* Value: 0x01 -> Sleep mode
* Value: 0x00 -> Running mode - Default mode
*************************************************************************************************/
BOOLEAN Sec_1411xSleepOn(void)
{
    BOOLEAN result = FALSE;
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf4,0x0001);
    return result;
}
BOOLEAN Sec_1411xSleepOff(void)
{
    BOOLEAN result = FALSE;
    result |= _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xf4,0x0000);
    Sec_1411xSoftReset();
    return result;
}
BOOLEAN  Sec_1411xCheckIsSleep(void)
{
   return (BOOLEAN)(_IIC_Read2Bytes(IIC_1411x_TOP_RegId, 0xf4)==0x0001);
}


/****************************************************************************************************
* S5H1411 Alter Mpeg Parallel/Serial Mode
* Conditions:
* Description:  Parallel output TS.(Default)
* Addr: 0xbd
* Value: 0x1001 -> PARALLEL , 0x1101 -> SERIAL

****************************************************************************************************/
BOOLEAN Sec_1411xAlterMpegMode(SamsungMpegMode_t mode)
{
    BOOLEAN result;

    if(mode == MPEG_MODE_PARAL)
        result = _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xbd, 0x1001);
    else
        result = _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xbd, 0x1101);

	return result;
}

/**************************************************************************
* S5H1411 Alter Mpeg Normal/Invert Clock
*	Conditions:
*	Description : Inverting output clk.(Default)
*	After this function is called TS timing could be changed.
*	Addr : 0xbe
*	Value: 0x0003
**************************************************************************/
BOOLEAN Sec_1411xAlterMpegClk(SamsungMpegClk_t clock)
{
    BOOLEAN result;

    if(clock == MPEG_CLK_INVERT)
        result = _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xbe,0x0003);
    else
        result = _IIC_Write2Bytes(IIC_1411x_TOP_RegId, 0xbe,0x1003);

   	return result;
}

/*************************************************************************************************
* S5H1411 VSB Sync Lock Check Driver
* Conditions: VSB mode
* Description: This function is used to check whether the sync detector is lock or not in VSB mode.
* Addr: 0x5e
* Value: [0] bit -> 1:Lock , 0: Unlock
*************************************************************************************************/
U16 Sec_1411xSyncLock(void)
{
    U16 sync_lock;
    sync_lock = (_IIC_Read2Bytes(IIC_1411x_TOP_RegId,0x5e)) & 0x0001;
    return sync_lock;
}

/*************************************************************************************************
S5H1411 QAM Equalizer Lock Check Driver
* Conditions: QAM mode
* Description: This function is used to check whether the FEC is lock or not in QAM mode.
* Addr: 0xf0
* Value: [8] bit -> 1:Lock , 0: Unlock
*************************************************************************************************/
U16 Sec_1411xQAMEq_Lock(void)
{
    U16 eq_lock;
    eq_lock = (_IIC_Read2Bytes(IIC_1411x_TOP_RegId,0xf0) >> 8) & 0x0001;
    return eq_lock;
}

/*************************************************************************************************
* S5H1411 VSB/QAM Master Lock Check Driver
* Conditions: VSB/QAM mode
* Description: This function is used to check whether the channel chip is lock or not in VSB/QAM mode.
* Addr: 0xf2
* Value: 15bit -> 1:Lock , 0: Unlock
*************************************************************************************************/
U16 Sec_1411xVsbQAM_Master_Lock(void)
{
    U16 master_lock;
    master_lock = (_IIC_Read2Bytes(IIC_1411x_TOP_RegId,0xf2)>>15) & 0x0001;
    return master_lock;
}

/**************************************************************************
* S5H1412 VSB SNR value(Signal Strength) Check Driver
*	Conditions:
*	Description :
*	This function lets the user's application software read a value
*	that is related to the SNR through the SNR Look Up Table in VSB mode.
*	Addr : 0xf2
*	Value: [9:0]
**************************************************************************/
static  double SnrLookUpTable(U16 SnrRead)
{
	double SnrLookUP;
       if(SnrRead > 0x39f ) SnrLookUP = 30 ;//10
	else if(SnrRead > 0x39b ) SnrLookUP = 29.5 ;
	else if(SnrRead > 0x397 ) SnrLookUP = 29.0;//9
	else if(SnrRead > 0x394 ) SnrLookUP = 28.5 ;
	else if(SnrRead > 0x38f ) SnrLookUP = 28.0 ;
	else if(SnrRead > 0x38b ) SnrLookUP = 27.5 ;
	else if(SnrRead > 0x387 ) SnrLookUP = 27.0 ;//8
	else if(SnrRead > 0x382 ) SnrLookUP = 26.5 ;
	else if(SnrRead > 0x37d ) SnrLookUP = 26.0 ;
	else if(SnrRead > 0x377 ) SnrLookUP = 25.5 ;
	else if(SnrRead > 0x370 ) SnrLookUP = 25.0 ;//7
	else if(SnrRead > 0x36a ) SnrLookUP = 24.5 ;
	else if(SnrRead > 0x364 ) SnrLookUP = 24.0 ;
	else if(SnrRead > 0x35b ) SnrLookUP = 23.5 ;
	else if(SnrRead > 0x353 ) SnrLookUP = 23.0 ;//6
	else if(SnrRead > 0x349 ) SnrLookUP = 22.5 ;
	else if(SnrRead > 0x340 ) SnrLookUP = 22.0 ;
	else if(SnrRead > 0x337 ) SnrLookUP = 21.5 ;
	else if(SnrRead > 0x327 ) SnrLookUP = 21.0 ;//5
	else if(SnrRead > 0x31b ) SnrLookUP = 20.5 ;
	else if(SnrRead > 0x310 ) SnrLookUP = 20.0 ;
	else if(SnrRead > 0x302 ) SnrLookUP = 19.5 ;
	else if(SnrRead > 0x2f3 ) SnrLookUP = 19.0 ;//4
	else if(SnrRead > 0x2e4 ) SnrLookUP = 18.5 ;
	else if(SnrRead > 0x2d7 ) SnrLookUP = 18.0 ;
	else if(SnrRead > 0x2cd ) SnrLookUP = 17.5 ;
	else if(SnrRead > 0x2bb ) SnrLookUP = 17.0 ;//3
	else if(SnrRead > 0x2a9 ) SnrLookUP = 16.5 ;
	else if(SnrRead > 0x29e ) SnrLookUP = 16.0 ;
	else if(SnrRead > 0x284 ) SnrLookUP = 15.5 ;
	else if(SnrRead > 0x27a ) SnrLookUP = 15.0 ;
	else if(SnrRead > 0x260 ) SnrLookUP = 14.5 ;
	else if(SnrRead > 0x23a ) SnrLookUP = 14.0 ;
	else if(SnrRead > 0x224 ) SnrLookUP = 13.5 ;
	else if(SnrRead > 0x213 ) SnrLookUP = 13.0 ;
	else if(SnrRead > 0x204 ) SnrLookUP = 12.5 ;
	else if(SnrRead > 0x1fe ) SnrLookUP = 12.0 ;
	else SnrLookUP = 0;
	return SnrLookUP;
}

double Sec_1411xVsbSnrResultCheck(void)
{
    U16 value;
    double SnrValue;
    value = _IIC_Read2Bytes(IIC_1411x_TOP_RegId,0xf2);
    value = (value & 0x3ff);
    SnrValue = SnrLookUpTable(value);
    return SnrValue;
}


/*************************************************************************************************
* S5H1411 QAM SNR value(Signal Quality) Check Driver
* Conditions:
* Description
* This function read a mse value output from equalizer and mapped into the SNR table.
* Addr : 0xf1
* Value: [15:0]
*************************************************************************************************/
static  double Qam64SnrLookUpTable(U16 SnrRead)
{
    double SnrLookUP;
    if (SnrRead< 0xaf0 ) SnrLookUP = 30 ;
    else if(SnrRead < 0xd80 ) SnrLookUP = 29 ;
    else if(SnrRead < 0x10a0 ) SnrLookUP = 28 ;
    else if(SnrRead < 0x14b5 ) SnrLookUP = 27 ;
    else if(SnrRead < 0x1590 ) SnrLookUP = 26.8 ;
    else if(SnrRead < 0x1680 ) SnrLookUP = 26.6 ;
    else if(SnrRead < 0x17b0 ) SnrLookUP = 26.4 ;
    else if(SnrRead < 0x18c0 ) SnrLookUP = 26.2 ;
    else if(SnrRead < 0x19b0 ) SnrLookUP = 26 ;
    else if(SnrRead < 0x1ad0 ) SnrLookUP = 25.8 ;
    else if(SnrRead < 0x1d00 ) SnrLookUP = 25.6 ;
    else if(SnrRead < 0x1da0 ) SnrLookUP = 25.4 ;
    else if(SnrRead < 0x1ef0 ) SnrLookUP = 25.2 ;
    else if(SnrRead < 0x2050 ) SnrLookUP = 25 ;
    else if(SnrRead < 0x20f0 ) SnrLookUP = 24.9 ;
    else if(SnrRead < 0x21d0 ) SnrLookUP = 24.8 ;
    else if(SnrRead < 0x22b0 ) SnrLookUP = 24.7 ;
    else if(SnrRead < 0x23a0 ) SnrLookUP = 24.6 ;
    else if(SnrRead < 0x2470 ) SnrLookUP = 24.5 ;
    else if(SnrRead < 0x24f0 ) SnrLookUP = 24.4 ;
    else if(SnrRead < 0x25a0 ) SnrLookUP = 24.3 ;
    else if(SnrRead < 0x26c0 ) SnrLookUP = 24.2 ;
    else if(SnrRead < 0x27b0 ) SnrLookUP = 24.1 ;
    else if(SnrRead < 0x28d0 ) SnrLookUP = 24 ;
    else if(SnrRead < 0x29b0 ) SnrLookUP = 23.9 ;
    else if(SnrRead < 0x2ad0 ) SnrLookUP = 23.8 ;
    else if(SnrRead < 0x2ba0 ) SnrLookUP = 23.7 ;
    else if(SnrRead < 0x2c80 ) SnrLookUP = 23.6 ;
    else if(SnrRead < 0x2d20 ) SnrLookUP = 23.5 ;
    else if(SnrRead < 0x2e00 ) SnrLookUP = 23.4 ;
    else if(SnrRead < 0x2f10 ) SnrLookUP = 23.3 ;
    else if(SnrRead < 0x3050 ) SnrLookUP = 23.2 ;
    else if(SnrRead < 0x3190 ) SnrLookUP = 23.1 ;
    else if(SnrRead < 0x3300 ) SnrLookUP = 23 ;
    else if(SnrRead < 0x3340 ) SnrLookUP = 22.9 ;
    else if(SnrRead < 0x3200 ) SnrLookUP = 22.8 ;
    else if(SnrRead < 0x3550 ) SnrLookUP = 22.7 ;
    else if(SnrRead < 0x3610 ) SnrLookUP = 22.6 ;
    else if(SnrRead < 0x3600 ) SnrLookUP = 22.5 ;
    else if(SnrRead < 0x3700 ) SnrLookUP = 22.4 ;
    else if(SnrRead < 0x3800 ) SnrLookUP = 22.3 ;
    else if(SnrRead < 0x3920 ) SnrLookUP = 22.2 ;
    else if(SnrRead < 0x3a20 ) SnrLookUP = 22.1 ;
    else if(SnrRead < 0x3b30 ) SnrLookUP = 22 ;
    else if(SnrRead < 0x3d00 ) SnrLookUP = 21.9 ;
    else if(SnrRead < 0x3e00 ) SnrLookUP = 21.8 ;
    else if(SnrRead < 0x4000 ) SnrLookUP = 21.7 ;
    else if(SnrRead < 0x4100 ) SnrLookUP = 21.6 ;
    else if(SnrRead < 0x4300 ) SnrLookUP = 21.5 ;
    else if(SnrRead < 0x4400 ) SnrLookUP = 21.4 ;
    else if(SnrRead < 0x4600 ) SnrLookUP = 21.3 ;
    else if(SnrRead < 0x4700 ) SnrLookUP = 21.2 ;
    else if(SnrRead < 0x4800 ) SnrLookUP = 21.1 ;
    else if(SnrRead < 0x4a00 ) SnrLookUP = 21 ;
    else if(SnrRead < 0x4b00 ) SnrLookUP = 20.9 ;
    else if(SnrRead < 0x4d00 ) SnrLookUP = 20.8 ;
    else if(SnrRead < 0x4f00 ) SnrLookUP = 20.7 ;
    else if(SnrRead < 0x5050 ) SnrLookUP = 20.6 ;
    else if(SnrRead < 0x5200 ) SnrLookUP = 20.5 ;
    else if(SnrRead < 0x53c0 ) SnrLookUP = 20.4 ;
    else if(SnrRead < 0x5450 ) SnrLookUP = 20.3 ;
    else if(SnrRead < 0x5650 ) SnrLookUP = 20.2 ;
    else if(SnrRead < 0x5820 ) SnrLookUP = 20.1 ;
    else if(SnrRead < 0x6000 ) SnrLookUP = 20 ;
    else SnrLookUP = 0;
    return SnrLookUP;
}

static double Qam256SnrLookUpTable(U16 SnrRead)
{
    double SnrLookUP;
    if (SnrRead < 0x970 ) SnrLookUP = 40 ;
    else if(SnrRead < 0xa90 ) SnrLookUP = 39 ;
    else if(SnrRead < 0xb90 ) SnrLookUP = 38 ;
    else if(SnrRead < 0xd90 ) SnrLookUP = 37 ;
    else if(SnrRead < 0xff0 ) SnrLookUP = 36 ;
    else if(SnrRead < 0x1240) SnrLookUP = 35 ;
    else if(SnrRead < 0x1345) SnrLookUP = 34.8 ;
    else if(SnrRead < 0x13c0) SnrLookUP = 34.6 ;
    else if(SnrRead < 0x14c0) SnrLookUP = 34.4 ;
    else if(SnrRead < 0x1500) SnrLookUP = 34.2 ;
    else if(SnrRead < 0x1610) SnrLookUP = 34 ;
    else if(SnrRead < 0x1700) SnrLookUP = 33.8 ;
    else if(SnrRead < 0x1800) SnrLookUP = 33.6 ;
    else if(SnrRead < 0x18b0) SnrLookUP = 33.4 ;
    else if(SnrRead < 0x1900) SnrLookUP = 33.2 ;
    else if(SnrRead < 0x1ab0) SnrLookUP = 33 ;
    else if(SnrRead < 0x1bc0) SnrLookUP = 32.8 ;
    else if(SnrRead < 0x1cb0) SnrLookUP = 32.6 ;
    else if(SnrRead < 0x1db0) SnrLookUP = 32.4 ;
    else if(SnrRead < 0x1eb0) SnrLookUP = 32.2 ;
    else if(SnrRead < 0x2030) SnrLookUP = 32 ;
    else if(SnrRead < 0x2200) SnrLookUP = 31.8 ;
    else if(SnrRead < 0x2280) SnrLookUP = 31.6 ;
    else if(SnrRead < 0x2410) SnrLookUP = 31.4 ;
    else if(SnrRead < 0x25b0) SnrLookUP = 31.2 ;
    else if(SnrRead < 0x27a0) SnrLookUP = 31 ;
    else if(SnrRead < 0x2840) SnrLookUP = 30.8 ;
    else if(SnrRead < 0x29d0) SnrLookUP = 30.6 ;
    else if(SnrRead < 0x2b10) SnrLookUP = 30.4 ;
    else if(SnrRead < 0x2d30) SnrLookUP = 30.2 ;
    else if(SnrRead < 0x2f20) SnrLookUP = 30 ;
    else if(SnrRead < 0x30c0) SnrLookUP = 29.8 ;
    else if(SnrRead < 0x3260) SnrLookUP = 29.7 ;
    else if(SnrRead < 0x32c0) SnrLookUP = 29.6 ;
    else if(SnrRead < 0x3300) SnrLookUP = 29.5 ;
    else if(SnrRead < 0x33b0) SnrLookUP = 29.4 ;
    else if(SnrRead < 0x34b0) SnrLookUP = 29.3 ;
    else if(SnrRead < 0x35a0) SnrLookUP = 29.2 ;
    else if(SnrRead < 0x3650) SnrLookUP = 29.1 ;
    else if(SnrRead < 0x3800) SnrLookUP = 29 ;
    else if(SnrRead < 0x3900) SnrLookUP = 28.9 ;
    else if(SnrRead < 0x3a50) SnrLookUP = 28.8 ;
    else if(SnrRead < 0x3b30) SnrLookUP = 28.7 ;
    else if(SnrRead < 0x3cb0) SnrLookUP = 28.6 ;
    else if(SnrRead < 0x3e20) SnrLookUP = 28.5 ;
    else if(SnrRead < 0x3fa0) SnrLookUP = 28.4 ;
    else if(SnrRead < 0x40a0) SnrLookUP = 28.3 ;
    else if(SnrRead < 0x41c0) SnrLookUP = 28.2 ;
    else if(SnrRead < 0x42f0) SnrLookUP = 28.1 ;
    else if(SnrRead < 0x44a0) SnrLookUP = 28 ;
    else if(SnrRead < 0x4600) SnrLookUP = 27.9 ;
    else if(SnrRead < 0x47b0) SnrLookUP = 27.8 ;
    else if(SnrRead < 0x4900) SnrLookUP = 27.7 ;
    else if(SnrRead < 0x4a00) SnrLookUP = 27.6 ;
    else if(SnrRead < 0x4ba0) SnrLookUP = 27.5 ;
    else if(SnrRead < 0x4d00) SnrLookUP = 27.4 ;
    else if(SnrRead < 0x4f00) SnrLookUP = 27.3 ;
    else if(SnrRead < 0x5000) SnrLookUP = 27.2 ;
    else if(SnrRead < 0x51f0) SnrLookUP = 27.1 ;
    else if(SnrRead < 0x53a0) SnrLookUP = 27 ;
    else if(SnrRead < 0x5520) SnrLookUP = 26.9 ;
    else if(SnrRead < 0x5700) SnrLookUP = 26.8 ;
    else if(SnrRead < 0x5800) SnrLookUP = 26.7 ;
    else if(SnrRead < 0x5a00) SnrLookUP = 26.6 ;
    else if(SnrRead < 0x5c00) SnrLookUP = 26.5 ;
    else if(SnrRead < 0x5d00) SnrLookUP = 26.4 ;
    else if(SnrRead < 0x5f00) SnrLookUP = 26.3 ;
    else if(SnrRead < 0x6000) SnrLookUP = 26.2 ;
    else if(SnrRead < 0x6200) SnrLookUP = 26.1 ;
    else if(SnrRead < 0x6400) SnrLookUP = 26 ;
    else SnrLookUP = 0;

    return SnrLookUP;
}

double Sec_1411xQamSnrResultCheck(void)
{
    U16 mse;
    U16 qam_mode;
    double SnrValue=0;
    qam_mode = Sec_1411xGetQAMMode();
    mse = _IIC_Read2Bytes(IIC_1411x_TOP_RegId , 0x00f1);
    switch(qam_mode)
    {
        case 0:
        SnrValue = Qam64SnrLookUpTable(mse);
        break;
        case 1:
        SnrValue = Qam256SnrLookUpTable(mse);
        break;
    }
    return SnrValue;
}

void Sec_1411xCheckSignalCondition(SamsungSignalCondition_t* pstatus)
{
	U16 value;
	double SnrValue = 0;

	value = Sec_1411xGetMode();
	if(value==0)//VSB mode
	{
            /*************************************************************************************************
            * S5H1411 VSB SNR value(Signal Quality) Check Driver
            * Conditions:
            * Description :
            * This function read a mse value output from equalizer and mapped into the SNR table.
            * Addr : 0xf2
            * Value: [9:0]
            *************************************************************************************************/
            SnrValue=Sec_1411xVsbSnrResultCheck();
    			//printf("\n===%d[==\n",SnrValue);
            if(!Sec_1411xSyncLock())
            {
    			//printf("\n==harry==unlock!!!\n");
                SnrValue=0;
            }
		if(SnrValue==0)
			*pstatus=Samsung_SIGNAL_NO;
		else if(SnrValue<17)
			*pstatus=Samsung_SIGNAL_WEAK;
		else if(SnrValue<18.5)
			*pstatus=Samsung_SIGNAL_WEAK1;
		else if(SnrValue<20)
			*pstatus=Samsung_SIGNAL_MODERATE;
		else if(SnrValue<21.5)
			*pstatus=Samsung_SIGNAL_MODERATE1;
		else if(SnrValue<23)
			*pstatus=Samsung_SIGNAL_MODERATE2;
		else if(SnrValue<24.5)
			*pstatus=Samsung_SIGNAL_STRONG;
		else if(SnrValue<26)
			*pstatus=Samsung_SIGNAL_STRONG1;
		else if(SnrValue<27.5)
			*pstatus=Samsung_SIGNAL_STRONG2;
		else if(SnrValue<29)
			*pstatus=Samsung_SIGNAL_VERY_STRONG;
		else
			*pstatus=Samsung_SIGNAL_VERY_STRONG1;
	}
	else //QAM
	{

            SnrValue = Sec_1411xQamSnrResultCheck();
            if(Sec_1411xVsbQAM_Master_Lock()==0)
            {
                *pstatus=Samsung_SIGNAL_NO;
                return;
            }
            if(Sec_1411xGetQAMMode() == 1) //256 QAM
            {
                if(SnrValue==0)
                	*pstatus=Samsung_SIGNAL_NO;
                else if(SnrValue<31)
                	*pstatus=Samsung_SIGNAL_WEAK;
                else if(SnrValue<32)
                	*pstatus=Samsung_SIGNAL_WEAK1;
                else if(SnrValue<33)
                	*pstatus=Samsung_SIGNAL_MODERATE;
                else if(SnrValue<34)
                	*pstatus=Samsung_SIGNAL_MODERATE1;
                else if(SnrValue<35)
                	*pstatus=Samsung_SIGNAL_MODERATE2;
                else if(SnrValue<36)
                	*pstatus=Samsung_SIGNAL_STRONG;
                else if(SnrValue<37)
                	*pstatus=Samsung_SIGNAL_STRONG1;
                else if(SnrValue<38)
                	*pstatus=Samsung_SIGNAL_STRONG2;
                else if(SnrValue<39)
                	*pstatus=Samsung_SIGNAL_VERY_STRONG;
                else
                	*pstatus=Samsung_SIGNAL_VERY_STRONG1;

            }
            else    // 64 QAM
            {
                if(SnrValue==0)
                	*pstatus=Samsung_SIGNAL_NO;
                else if(SnrValue<21)
                	*pstatus=Samsung_SIGNAL_WEAK;
                else if(SnrValue<22)
                	*pstatus=Samsung_SIGNAL_WEAK1;
                else if(SnrValue<23)
                	*pstatus=Samsung_SIGNAL_MODERATE;
                else if(SnrValue<24)
                	*pstatus=Samsung_SIGNAL_MODERATE1;
                else if(SnrValue<25)
                	*pstatus=Samsung_SIGNAL_MODERATE2;
                else if(SnrValue<26)
                	*pstatus=Samsung_SIGNAL_STRONG;
                else if(SnrValue<27)
                	*pstatus=Samsung_SIGNAL_STRONG1;
                else if(SnrValue<28)
                	*pstatus=Samsung_SIGNAL_STRONG2;
                else if(SnrValue<29)
                	*pstatus=Samsung_SIGNAL_VERY_STRONG;
                else
                	*pstatus=Samsung_SIGNAL_VERY_STRONG1;

            }

	}
}

/*********************************************************************************************************
* S5H1411 VSB Packet Error Check Driver
* Conditions:
* Description:
* Error count value after RS decoder
* This function is used to count the packet error.
* Addr: 0xc9
* Value: [15:0]
**********************************************************************************************************/
MS_U16 S5H1411x_UncorrectableError(void)
{
	MS_U16 unc_err;
	unc_err = _IIC_Read2Bytes(IIC_1411x_TOP_RegId, 0xc9);
    DEMOD_DBG(printf("-----%s BER=%x-----\n", __FUNCTION__,unc_err));
	return 	unc_err;
}

void MApi_Demod_HWReset(void)
{
     mdrv_gpio_set_high(GPIO_FE_RST);
     MsOS_DelayTask(100);
     mdrv_gpio_set_low(GPIO_FE_RST);
     MsOS_DelayTask(200);
     mdrv_gpio_set_high(GPIO_FE_RST);
     MsOS_DelayTask(100);
}

static MS_BOOL Sec_1411xCheckLock(void)
{
     MS_BOOL bLock = FALSE;
     MS_U16 u16Mode;
     
     u16Mode = Sec_1411xGetMode();
     if(u16Mode==0)//VSB mode
     {
        if(Sec_1411xSyncLock())
        {
            bLock=TRUE;
        }
     }
     else //QAM
     {
         if(Sec_1411xVsbQAM_Master_Lock())
         {
             bLock=TRUE;
         }
     }
     return bLock;
}

MS_BOOL MApi_FE_Power_On_Initialization(void)
{
    MS_BOOL ret = TRUE;

    DEMOD_DBG(printf("-----%s start-----\n", __FUNCTION__));

    if (_s32TaskId >= 0)
    {
        return TRUE;
    }

    _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "OfDmd_Mutex", MSOS_PROCESS_SHARED);

    if (_s32MutexId < 0)
    {
        return FALSE;
    }

    MApi_Demod_HWReset();
    ret &= Sec_1411xInitialize();
    #if TS_PARALLEL_OUTPUT
    ret &= Sec_1411xAlterMpegMode(MPEG_MODE_PARAL);
    #else
    ret &= Sec_1411xAlterMpegMode(MPEG_MODE_SERIAL);
    #endif

    #if(!TS_CLK_INV)
    ret &= Sec_1411xAlterMpegClk(MPEG_CLK_INVERT);
    #else
    ret &= Sec_1411xAlterMpegClk(MPEG_CLK_NOT_INVERT);
    #endif

	Sec_1411xQamMode();
    //ret &= Sec_1411xSleepOff();
    ret &= Sec_1411xSoftReset();
    ret &= Sec_1411xRepeaterEnable();
    _eDemodMode = DEMOD_MODE_64QAM;
    ret &= MDrv_Tuner_Init();
    ret &= Sec_1411xRepeaterDisable();
    if(ret)
    {
        DEMOD_DBG(printf("S25H1411 init OK!\n"));
        return TRUE;
    }
    else
    {
        DEMOD_DBG(printf("S25H1411 init failed!\n"));
        return FALSE;
    }
}

MS_BOOL MDrv_Demod_Open(void)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    return TRUE;
}
MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{
    return TRUE;
}
MS_U32 MDrv_Tuner_Stand_By(void)
{

    return TRUE;
}
MS_U32 MDrv_Tuner_Wake_Up(void)
{

    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DEMOD_DBG(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        if(bPowerOn)
        {
            if(Sec_1411xSleepOff())
            {
                DEMOD_DBG(printf("S25H1411 power on OK!\n"));
                MsOS_ReleaseMutex(_s32MutexId);
                return TRUE;
            }
        }
        else
        {
            if(Sec_1411xSleepOn())
            {
                DEMOD_DBG(printf("S25H1411 power off OK!\n"));
                MsOS_ReleaseMutex(_s32MutexId);
                return TRUE;
            }
        }
    }
    DEMOD_DBG(printf("S25H1411 power ON/Off failed!\n"));
    MsOS_ReleaseMutex(_s32MutexId);
    return FALSE;
}

MS_BOOL  MApi_FE_CheckLock(void)
{
    MS_BOOL bLock;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DEMOD_DBG(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
       bLock = Sec_1411xCheckLock();
    }
    MsOS_ReleaseMutex(_s32MutexId);
    return bLock;
}
MS_U32 MApi_FE_GetSNR(void)
{
    MS_U32 u32SNR = 0;
    MS_BOOL pLOCK;
    pLOCK = Sec_1411xCheckLock();
    #if(FRONTEND_TUNER_TYPE == TUNER_NMI120)
    if(pLOCK)
    {
        u32SNR = NMI120_GetRSSI(1);
    }
    else
        u32SNR = 0;
    #else
    SamsungSignalCondition_t status;
    Sec_1411xCheckSignalCondition(&status);

    if(status ==Samsung_SIGNAL_VERY_STRONG1)
        u32SNR = 100;
    else if(status ==Samsung_SIGNAL_VERY_STRONG)
        u32SNR = 90;
    else if(status ==Samsung_SIGNAL_STRONG2)
        u32SNR = 80;
    else if(status ==Samsung_SIGNAL_STRONG1)
        u32SNR = 70;
    else if(status ==Samsung_SIGNAL_STRONG)
        u32SNR = 60;
    else if(status ==Samsung_SIGNAL_MODERATE2)
        u32SNR = 50;
    else if(status ==Samsung_SIGNAL_MODERATE1)
        u32SNR = 40;
    else if(status ==Samsung_SIGNAL_MODERATE)
        u32SNR = 30;
    else if(status ==Samsung_SIGNAL_WEAK1)
        u32SNR = 20;
    else if(status ==Samsung_SIGNAL_WEAK)
        u32SNR = 10;
    else if(status ==Samsung_SIGNAL_NO)
        u32SNR = 0;
    #endif
  
    return u32SNR;
}

#define E 2.7
#define PRECISE 20
static double pow( double d, int n)
{
    int s;
    double r = 1;

    if( n == 0 )
        return 1;

    if( n < 0 )
        s = 1 , n = -n;
    else
        s = 0;

    while( n != 1 )
    {
        if( n&1 ) r*=d;
        n /= 2;
        d *= d;
    }

    r *= d;
    return s?1/r:r;
}

float LnPrecise(float x)
{

    float e_x=0.0;
    float temp =x;
    MS_S32 y=0,i,j;

    if(x>0&&x<=2)
    {
        i=1;
        j=2;
        x=x-1;
        for(; i<PRECISE&&j<PRECISE; i+=2,j+=2)
        {
            e_x += pow(x,i)/i;
            e_x -= pow(x,j)/j;
        }
        return e_x;
    }

    while(temp>2)
    {
        temp /=E;
        y++;
    }
    return y+LnPrecise(x/pow(E,y));
}

float Log10Precise(float x)
{
    float a = LnPrecise(x);
    float b = LnPrecise(10);
    return  a/b;
}

MS_U16 MApi_FE_GetSignalQuality(void)
{
    MS_U16 u16ber;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DEMOD_DBG(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    u16ber = S5H1411x_UncorrectableError();
    printf("[S5H1411]==harry==%f==\n",u16ber);
    MsOS_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    SamsungSignalCondition_t status;
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DEMOD_DBG(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        Sec_1411xCheckSignalCondition(&status);

        if(status ==Samsung_SIGNAL_VERY_STRONG1)
            *ps32Signal = 100;
        else if(status ==Samsung_SIGNAL_VERY_STRONG)
            *ps32Signal = 90;
        else if(status ==Samsung_SIGNAL_STRONG2)
            *ps32Signal = 80;
        else if(status ==Samsung_SIGNAL_STRONG1)
            *ps32Signal = 70;
        else if(status ==Samsung_SIGNAL_STRONG)
            *ps32Signal = 60;
        else if(status ==Samsung_SIGNAL_MODERATE2)
            *ps32Signal = 50;
        else if(status ==Samsung_SIGNAL_MODERATE1)
            *ps32Signal = 40;
        else if(status ==Samsung_SIGNAL_MODERATE)
            *ps32Signal = 30;
        else if(status ==Samsung_SIGNAL_WEAK1)
            *ps32Signal = 20;
        else if(status ==Samsung_SIGNAL_WEAK)
            *ps32Signal = 10;
        else if(status ==Samsung_SIGNAL_NO)
            *ps32Signal = 0;
    }

    MsOS_ReleaseMutex(_s32MutexId);
    DEMOD_DBG(printf("S25H1411 signal STATUS=%d\n",*ps32Signal));
    return TRUE;

}

EN_CONSTEL_MODE MDrv_Demod_GetDemodMode(void)
{
    if(Sec_1411xGetMode())
    {
        if(Sec_1411xGetQAMMode())
            return DEMOD_MODE_256QAM;
        else
            return DEMOD_MODE_64QAM;
    }
    else
        return DEMOD_MODE_8VSB;
}

void MDrv_Demod_SetDemodMode(EN_CONSTEL_MODE mode)
{

    if(mode == DEMOD_MODE_8VSB)
    {
        DEMOD_DBG(printf("[S5H1411] 8VSB mode\n"));
        Sec_1411xVsbMode();
    }
    else if(mode == DEMOD_MODE_64QAM)
    {
        DEMOD_DBG(printf("[S5H1411] 64QAM mode\n"));
		Sec_1411xQamMode();
        Sec_1411x64QamMode();
    }
    else
    {
        DEMOD_DBG(printf("[S5H1411] 256QAM mode\n"));
		Sec_1411xQamMode();
        Sec_1411x256QamMode();
    }
	MsOS_DelayTask(50);
	//Sec_1411xSoftReset();
	MsOS_DelayTask(350);
}

void MApi_FE_SetFrequency(MS_U32 u32Frequency,MS_U8 eBandWidth)
{
    //Sec_1411xRepeaterEnable();//bypass open
    if (MsOS_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DEMOD_DBG(printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return ;
    }
    else
    {
        MDrv_Demod_SetDemodMode(_eDemodMode);
        Sec_1411xRepeaterEnable();//bypass open
        switch (eBandWidth)
        {
             case DEMOD_BW_MODE_6MHZ:
             {
                 DEMOD_DBG(printf("S25H1411 tune freq=%d bw =6\n",u32Frequency));
                 MDrv_Tuner_SetTuner(u32Frequency,6);
                 break;
             }
             case DEMOD_BW_MODE_7MHZ:
             {
                 DEMOD_DBG(printf("S25H1411 tune freq=%d bw =7\n",u32Frequency));
                 MDrv_Tuner_SetTuner(u32Frequency,7);
                 break;
             }
             case DEMOD_BW_MODE_8MHZ:
             {
                 DEMOD_DBG(printf("S25H1411 tune freq=%d bw =8\n",u32Frequency));
                 MDrv_Tuner_SetTuner(u32Frequency,8);
                 break;
             }
             default:
             {
                DEMOD_DBG(printf("S25H1411 tune freq=%d bw is unsupport\n",u32Frequency));
                break;
             }
        }
        Sec_1411xRepeaterDisable();//bypass close
    }
	Sec_1411xSoftReset();

    MsOS_ReleaseMutex(_s32MutexId);
    MsOS_DelayTask(300);
}

MS_BOOL MApi_FE_SetFECarrierParam(MS_FE_CARRIER_PARAM *pParam)
{
     _eDemodMode = pParam->TerParam.eDemodMode;
     return TRUE;
}
#endif

