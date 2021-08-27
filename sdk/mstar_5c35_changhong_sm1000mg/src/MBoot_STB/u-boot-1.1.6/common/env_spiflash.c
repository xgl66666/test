/* LowLevel function for DataFlash environment support
 * Author : Gilles Gastaldi (Atmel)
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
 *
 */
#include <common.h>

#if defined(CFG_ENV_IS_IN_SPIFLASH) /* Environment is in SPIFlash */

#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <sys/drv/drvSERFLASH.h>
#include <malloc.h>

#define SPIFLASH_KSEG0_ADDR 0x94000000

DECLARE_GLOBAL_DATA_PTR;

env_t *env_ptr = NULL;

char * env_name_spec = "spiflash";

extern uchar default_environment[];
extern int default_environment_size;
extern unsigned int cfg_env_offset;

static int write_flash(MS_U32 u32Addr, const MS_U8* pu8Src, MS_U32 u32Size)
{
#if (!CONFIG_MSTAR_SILENT)
    printf("Write addr=0x%08x, size=0x%08x\n", u32Addr, u32Size);
#endif

    MDrv_SERFLASH_WriteProtect(FALSE);

    int ret = -1;

    if (MDrv_SERFLASH_AddressErase(u32Addr, u32Size, TRUE))
    {
        if (MDrv_SERFLASH_Write(u32Addr, u32Size, (MS_U8*)pu8Src))
        {
            ret = 0;
        }
    }

    MDrv_SERFLASH_WriteProtect(TRUE);

    return ret;
}

uchar env_get_char_spec (int index)
{
    return ( *((uchar *)(gd->env_addr + index)) );
}

void env_relocate_spec (void)
{
    void* pEnvBuf = malloc(CFG_ENV_SIZE);
	memcpy(pEnvBuf, (void*)(SPIFLASH_KSEG0_ADDR + CFG_ENV_ADDR), CFG_ENV_SIZE);

    void* pEnvBufBak = malloc(CFG_ENV_SIZE);
    memcpy(pEnvBufBak, (void*)(SPIFLASH_KSEG0_ADDR + CFG_ENV_ADDR_BAK), CFG_ENV_SIZE);

    size_t offsetCrc = offsetof(env_t, crc);
    size_t offsetData = offsetof(env_t, data);

    ulong crc = *(ulong *)(pEnvBuf + offsetCrc);
    ulong crcNew = crc32(0, pEnvBuf + offsetData, ENV_SIZE);
    ulong crcBak = *(ulong *)(pEnvBufBak + offsetCrc);
    ulong crcNewBak = crc32(0, pEnvBufBak + offsetData, ENV_SIZE);

    if (crc != crcNew && crcBak != crcNewBak)
    {
        memcpy (env_ptr->data, default_environment, default_environment_size);
        env_ptr->crc = crc32(0, env_ptr->data, ENV_SIZE);
        gd->env_valid = 1;
        goto Exit;
    }

    if (crc != crcNew)
    {
        write_flash(CFG_ENV_ADDR, (MS_U8*)pEnvBufBak, CFG_ENV_SIZE);
    }
    else if (crcBak != crcNewBak)
    {
        write_flash(CFG_ENV_ADDR_BAK, (MS_U8*)pEnvBuf, CFG_ENV_SIZE);
    }

    memcpy(env_ptr, (crc != crcNew) ? pEnvBufBak : pEnvBuf, CFG_ENV_SIZE);

Exit:
    free(pEnvBufBak);
    free(pEnvBuf);
}

int saveenv(void)
{
    MDrv_SERFLASH_Init();
    int ret = write_flash(CFG_ENV_ADDR, (MS_U8*)env_ptr, CFG_ENV_SIZE);
    int retBak = write_flash(CFG_ENV_ADDR_BAK, (MS_U8*)env_ptr, CFG_ENV_SIZE);
    return (ret != 0 && retBak != 0) ? -1 : 0;
}

int env_init(void)
{
	if (0 == gd->env_valid)
    {
        gd->env_addr  = (ulong)&default_environment[0];
        gd->env_valid = 1;
	}

 	return 0;
}

#endif /* CFG_ENV_IS_IN_SPIFLASH */
