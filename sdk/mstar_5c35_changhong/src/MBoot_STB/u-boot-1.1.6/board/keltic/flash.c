/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CFG_ENV_IS_IN_SPIFLASH)
#include <sys/drv/drvSERFLASH.h>
#include <sys/drv/drvGPIO.h>
#endif

extern unsigned int cfg_env_offset;

flash_info_t flash_info[CFG_MAX_FLASH_BANKS];	/* info for FLASH chips */

#define GPIO_FLASH_WP   33

static void _SysInit_FlashHWWP_cb(MS_BOOL bWP)
{
    if (bWP)
    {
        mdrv_gpio_set_low(GPIO_FLASH_WP);
    }
    else
    {
        mdrv_gpio_set_high(GPIO_FLASH_WP);
    }
}

/*-----------------------------------------------------------------------
 * flash_init()
 *
 * sets up flash_info and returns size of FLASH (bytes)
 */
unsigned long flash_init (void)
{
#if defined(CFG_ENV_IS_IN_SPIFLASH)
    int i, j;
    const SERFLASH_Info *pSPIFlashInfo;

    MDrv_SERFLASH_Init();
    MDrv_SERFLASH_SetMode(0);
    MDrv_SERFLASH_SetFlashWPCallBack(_SysInit_FlashHWWP_cb);
    pSPIFlashInfo = MDrv_SERFLASH_GetInfo();

    // init flash_info
    for (i = 0; i < CFG_MAX_FLASH_BANKS; i++)
    {
        flash_info[i].size = pSPIFlashInfo->u32TotalSize;
        flash_info[i].sector_count = pSPIFlashInfo->u32SecNum;
        for (j = 0; j < pSPIFlashInfo->u32SecNum; j++)
        {
            flash_info[i].start[j] = j * pSPIFlashInfo->u32SecSize;
        }
    }

    // set environment offset on spi flash.
    cfg_env_offset = pSPIFlashInfo->u32TotalSize -(CFG_ENV_SIZE << 1);
#endif
	return 0;
}

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	printf ("write_buff not implemented\n");
	return (-1);
}

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
    return 0;
}

void flash_print_info (flash_info_t * info)
{
    return;
}
