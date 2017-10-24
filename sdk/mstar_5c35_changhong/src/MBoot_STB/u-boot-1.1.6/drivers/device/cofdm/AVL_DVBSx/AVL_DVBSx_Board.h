#ifndef _AVL_DVBSx_BOARD_H_
#define _AVL_DVBSx_BOARD_H_
#include <sys/common/MsOS.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>

//#include "MsCommon.h"
//#include "drvIIC.h"
//#include "drvGPIO.h"
//#include "Board.h"
//#include "MsTypes.h"
#define PIU_IIC_0                   0x0010
#define FRONTEND_TUNER_CTRL         PIU_IIC_0
#define FRONTEND_TUNER_PORT         E_HWI2C_PORT0_1

#if(FRONTEND_TUNER_CTRL == PIU_IIC_0)
#define IIC_DEVICEID               0
#define IIC_WRITE             MDrv_IIC_Write
#elif (FRONTEND_TUNER_CTRL == PIU_IIC_1)
#define IIC_DEVICEID               1
#define IIC_WRITE             MDrv_IIC1_Write
#else
#error "FRONTEND_TUNER_CTRL"
#endif
#define DEVIIC_IOC_SLAVEID 0
#define DEVIIC_OK                  0

//extern int IIC_Ioctl(MS_U8 u8DevId, MS_U32 u32Cmd, ...);
extern int IIC_Read(MS_U8 u8DevId, MS_U8 *buf, MS_U32 count);

MS_U32 SysDelay(MS_U32 dwMs);

#endif
