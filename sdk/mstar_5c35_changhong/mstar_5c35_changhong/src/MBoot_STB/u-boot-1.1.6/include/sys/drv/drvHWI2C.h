////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HWI2C_H_
#define _DRV_HWI2C_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_HWI2C_LIB_CODE                     {'H','I','2','C'}    //Lib code
#define MSIF_HWI2C_LIBVER                       {'0','5'}            //LIB version
#define MSIF_HWI2C_BUILDNUM                     {'0','3'}            //Build Number
#define MSIF_HWI2C_CHANGELIST                   {'0','0','4','2','0','2','8','2'} //P4 ChangeList Number

#define HWI2C_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_HWI2C_LIB_CODE,                /* IP__                                             */  \
    MSIF_HWI2C_LIBVER,                  /* 0.0 ~ Z.Z                                        */  \
    MSIF_HWI2C_BUILDNUM,                /* 00 ~ 99                                          */  \
    MSIF_HWI2C_CHANGELIST,              /* CL#                                              */  \
    MSIF_OS

/// debug level
typedef enum _HWI2C_DbgLv
{
    E_HWI2C_DBGLV_NONE,          /// no debug message
    E_HWI2C_DBGLV_ERR_ONLY,      /// show error only
    E_HWI2C_DBGLV_INFO,          /// show error & informaiton
    E_HWI2C_DBGLV_ALL            /// show error, information & funciton name
}HWI2C_DbgLv;

/// I2C select master port
typedef enum _HWI2C_PORT
{
    E_HWI2C_PORT_0 = 0, /// port 0_0 //disable port 0
    E_HWI2C_PORT0_1,    /// port 0_1
    E_HWI2C_PORT0_2,    /// port 0_2
    E_HWI2C_PORT0_3,    /// port 0_3
    E_HWI2C_PORT0_4,    /// port 0_4
    E_HWI2C_PORT0_5,    /// port 0_5
    E_HWI2C_PORT0_6,    /// port 0_6
    E_HWI2C_PORT0_7,    /// port 0_7

    E_HWI2C_PORT_1 = 8, /// port 1_0 //disable port 1
    E_HWI2C_PORT1_1,    /// port 1_1
    E_HWI2C_PORT1_2,    /// port 1_2
    E_HWI2C_PORT1_3,    /// port 1_3
    E_HWI2C_PORT1_4,    /// port 1_4
    E_HWI2C_PORT1_5,    /// port 1_5
    E_HWI2C_PORT1_6,    /// port 1_6
    E_HWI2C_PORT1_7,    /// port 1_7

    E_HWI2C_PORT_2 = 16,/// port 2_0 //disable port 2
    E_HWI2C_PORT2_1,    /// port 2_1
    E_HWI2C_PORT2_2,    /// port 2_2
    E_HWI2C_PORT2_3,    /// port 2_3
    E_HWI2C_PORT2_4,    /// port 2_4
    E_HWI2C_PORT2_5,    /// port 2_5
    E_HWI2C_PORT2_6,    /// port 2_6
    E_HWI2C_PORT2_7,    /// port 2_7

    E_HWI2C_PORT_3 = 24,/// port 3_0 //disable port 3
    E_HWI2C_PORT3_1,    /// port 3_1
    E_HWI2C_PORT3_2,    /// port 3_2
    E_HWI2C_PORT3_3,    /// port 3_3
    E_HWI2C_PORT3_4,    /// port 3_4
    E_HWI2C_PORT3_5,    /// port 3_5
    E_HWI2C_PORT3_6,    /// port 3_6
    E_HWI2C_PORT3_7,    /// port 3_7

    E_HWI2C_PORT_NOSUP  /// non-support port
}HWI2C_PORT;

/// I2C clock speed select
typedef enum _HWI2C_CLKSEL
{
    E_HWI2C_HIGH = 0,  /// high speed
    E_HWI2C_NORMAL,    /// normal speed
    E_HWI2C_SLOW,      /// slow speed
    E_HWI2C_VSLOW,     /// very slow 
    E_HWI2C_USLOW,     /// ultra slow 
    E_HWI2C_UVSLOW,    /// ultra-very slow 
    E_HWI2C_NOSUP      /// non-support speed
}HWI2C_CLKSEL;

/// I2C state
typedef enum _HWI2C_State
{
    E_HWI2C_IDLE,      /// idle state
    E_HWI2C_READ_DATA, /// read data state
    E_HWI2C_WRITE_DATA, /// write data state
    E_HWI2C_DMA_READ_DATA, /// DMA read data state
    E_HWI2C_DMA_WRITE_DATA /// DMA write data state
}HWI2C_State;

typedef enum {
    E_HWI2C_READ_MODE_DIRECT,                 ///< first transmit slave address + reg address and then start receive the data */
    E_HWI2C_READ_MODE_DIRECTION_CHANGE,       ///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device
    E_HWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device
    E_HWI2C_READ_MODE_MAX
} HWI2C_ReadMode;

typedef enum _HWI2C_DMA_ADDRMODE
{
    E_HWI2C_DMA_ADDR_NORMAL = 0,
    E_HWI2C_DMA_ADDR_10BIT,
    E_HWI2C_DMA_ADDR_MAX,
}HWI2C_DMA_ADDRMODE;

typedef enum _HWI2C_DMA_READMODE
{
    E_HWI2C_DMA_READ_NOSTOP = 0,
    E_HWI2C_DMA_READ_STOP,
    E_HWI2C_DMA_READ_MAX,
}HWI2C_DMA_READMODE;

typedef enum _HWI2C_DMA_MIUPRI
{
    E_HWI2C_DMA_PRI_LOW = 0,
    E_HWI2C_DMA_PRI_HIGH,
    E_HWI2C_DMA_PRI_MAX,
}HWI2C_DMA_MIUPRI;

typedef enum _HWI2C_DMA_MIUCH
{
    E_HWI2C_DMA_MIU_CH0 = 0,
    E_HWI2C_DMA_MIU_CH1,
    E_HWI2C_DMA_MIU_MAX,
}HWI2C_DMA_MIUCH;

/// I2C master pin config
typedef struct _HWI2C_PinCfg
{
    MS_U32  u32Reg;    /// register
    MS_U8   u8BitPos;  /// bit position
    MS_BOOL bEnable;   /// enable or disable
}HWI2C_PinCfg;

/// I2C port config
typedef struct _HWI2C_PortCfg
{
    MS_U32              u32DmaPhyAddr;  /// DMA physical address
    HWI2C_DMA_ADDRMODE  eDmaAddrMode;   /// DMA address mode
    HWI2C_DMA_MIUPRI    eDmaMiuPri;     /// DMA miu priroity
    HWI2C_DMA_MIUCH     eDmaMiuCh;      /// DMA miu channel
    MS_BOOL             bDmaEnable;     /// DMA enable

    HWI2C_PORT          ePort;          /// number
    HWI2C_CLKSEL        eSpeed;         /// clock speed
    HWI2C_ReadMode      eReadMode;      /// read mode
    MS_BOOL             bEnable;        /// enable

}HWI2C_PortCfg;

/// I2C Configuration for initialization
typedef struct _HWI2C_UnitCfg
{
    HWI2C_PortCfg   sCfgPort[4];    /// port cfg info
    HWI2C_PinCfg    sI2CPin;        /// pin info
    HWI2C_CLKSEL    eSpeed;         /// speed
    HWI2C_PORT      ePort;          /// port
    HWI2C_ReadMode  eReadMode;      /// read mode
    
}HWI2C_UnitCfg;

/// I2C information
typedef struct _HWI2C_Info
{
    MS_U32          u32IOMap; /// base address
    HWI2C_UnitCfg   sUnitCfg; /// configuration
}HWI2C_Info;

/// I2C status
typedef struct _HWI2C_Status
{
    MS_U8       u8DbgLevel;   /// debug level
    MS_BOOL     bIsInit;      /// initialized
    MS_BOOL     bIsMaster;    /// master
    HWI2C_State eState;       /// state
}HWI2C_Status;

////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_Init(HWI2C_UnitCfg *psCfg);
MS_BOOL MDrv_HWI2C_Start(void);
MS_BOOL MDrv_HWI2C_Stop(void);
MS_BOOL MDrv_HWI2C_GetPortIndex(HWI2C_PORT ePort, MS_U8* pu8Port);

MS_BOOL MDrv_HWI2C_SelectPort(HWI2C_PORT ePort);
MS_BOOL MDrv_HWI2C_SetClk(HWI2C_CLKSEL eClk);
MS_BOOL MDrv_HWI2C_SetReadMode(HWI2C_ReadMode eReadMode);
MS_BOOL MDrv_HWI2C_WriteByte(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 u8Data);
MS_BOOL MDrv_HWI2C_WriteBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
MS_BOOL MDrv_HWI2C_ReadByte(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 *pData);
MS_BOOL MDrv_HWI2C_ReadBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);

MS_BOOL MDrv_HWI2C_SelectPort1(HWI2C_PORT ePort);
MS_BOOL MDrv_HWI2C_SetClkP1(HWI2C_CLKSEL eClk);
MS_BOOL MDrv_HWI2C_SetReadModeP1(HWI2C_ReadMode eReadMode);
MS_BOOL MDrv_HWI2C_WriteByteP1(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 u8Data);
MS_BOOL MDrv_HWI2C_WriteBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
MS_BOOL MDrv_HWI2C_ReadByteP1(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 *pData);
MS_BOOL MDrv_HWI2C_ReadBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);

MS_BOOL MDrv_HWI2C_SetDbgLevel(HWI2C_DbgLv eLevel);
MS_BOOL MDrv_HWI2C_GetLibVer(const MSIF_Version **ppVersion);
void MDrv_HWI2C_GetStatus(HWI2C_Status *pStatus);
const HWI2C_Info* MDrv_HWI2C_GetInfo(void);

#ifdef __cplusplus
}
#endif


#endif
