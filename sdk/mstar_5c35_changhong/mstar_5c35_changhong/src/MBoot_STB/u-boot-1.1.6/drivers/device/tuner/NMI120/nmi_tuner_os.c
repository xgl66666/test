
#include <common.h>
#include <frontend_config.h>
#if(FRONTEND_TUNER_TYPE == TUNER_NMI120 )
#include "./nmi_tuner_os.h"
#include "./nmicmn.h"
#include "./nmiioctl.h"
#include "./nmi_tuner_os.h"

#include "./nmiioctl.h"
#include "sys/common/MsOS.h"
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>

#include <apiDigiTuner.h>
#include <msAPI_Tuner.h>
#include "sys/common/MsOS.h"
#include <sys/common/MsTypes.h>
#include <sys/drv/drvGPIO.h>
#include <sys/common/MsCommon.h>
#include <sys/common/MsTypes.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvIIC.h>
#include <device/drvDemod.h>

#define TUNER_ENABLE_GPIO  7

MS_BOOL MDrv_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    if (FALSE == MDrv_IIC_WriteBytes(u16BusNumSlaveID, AddrCnt, pu8addr, u16size, pBuf))
    {
        printf("MDrv_IIC_WriteBytes Error \n");
        return FALSE;
    }
    return TRUE;
}

MS_BOOL MDrv_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    if (FALSE == MDrv_IIC_ReadBytes(u16BusNumSlaveID, AddrCnt, pu8addr, u16size, pBuf))
    {
        printf("MDrv_IIC_ReadBytes Error \n");
        return FALSE;
    }

    return TRUE;

}
void nmi_tuner_os_delay(uint32_t msec)
{
    MsOS_DelayTask(msec);
}


uint32_t nmi_tuner_os_get_tick(void)
{
    uint32_t time;
    time = MsOS_GetSystemTime();
    return time;
}

void nmi_tuner_os_bus_deinit(void)
{
    //return 0;
}

int nmi_tuner_os_bus_init(void)
{
    return 0;
}

int nmi_tuner_os_bus_write(unsigned char DeviceAddr, unsigned char* pArray, unsigned long count)
{
    int status = 0;
    if (MDrv_IIC_Write(DeviceAddr, 0, 0, pArray, count))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return (status);
}

int nmi_tuner_os_bus_read(unsigned char DeviceAddr, unsigned char* pArray, unsigned long count)
{
    int status = 0;
    if (MDrv_IIC_Read(DeviceAddr, 0, 0, pArray, count))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return (status);    //success
}

int nmi_tuner_os_chip_enable(void)
{
    mdrv_gpio_set_high(TUNER_ENABLE_GPIO);//LGH ADD FOR ENABLE  2012_3_3
    MsOS_DelayTask(500);
    return TRUE;
}


uint8_t nmi_tuner_os_chip_power_on(void)
{
    return 1;
}

uint8_t nmi_tuner_os_chip_power_off(void)
{
    return 1;
}

uint32_t* nmi_tuner_os_malloc(uint32_t size)
{
    return 0;
}

void nmi_tuner_os_free(uint32_t* p)
{

}
void nmi_tuner_os_memset(void* mem, int value, uint32_t len)
{
    uint32_t i;
    uint8_t* temp = (uint8_t*)mem;
    for (i = 0; i < len; i++)
    {
        temp[i] = value;
    }
}

void nmi_tuner_os_memcpy(void *pu8Des, void *pu8Src, uint32_t len)
{
    uint32_t i = 0;
    uint8_t* tmpDes = pu8Des;
    uint8_t* tmpSrc = pu8Src;
    for (i = 0; i < len; i++)
    {
        tmpDes[i] = tmpSrc[i];
    }
}

#endif
