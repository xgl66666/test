// flash control driver
#include <cyg/hal/hal_io.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/io/flash.h>
#include <cyg/io/flash_priv.h>
#include <string.h>
#include "drvSERFLASH.h"
#include "MsTypes.h"
#include "Board.h"
#define  _FLASH_PRIVATE_

#if 0
#include <stdio.h>
#include <stdarg.h>
#define _TRACE(format, ...) __trace(__FILE__, __FUNCTION__, __LINE__, "", format, ## __VA_ARGS__)

extern int  diag_printf( const char *fmt, ... );
extern int  diag_vprintf(const char *fmt, va_list ap);

static int __trace(const char *file, const char *func, int line, const char *prefix, const char *format, ...)
{
    va_list ap;
    int r;
    const char *p = strrchr(file, '/');
    if(p)
        file = p + 1;

    diag_printf("%s[%s:%s:%d]", prefix, file, func, line);

    va_start(ap, format);
    r = diag_vprintf(format, ap);
    va_end(ap);

    return r;
}
#else
#define _TRACE(format, ...) do { } while(0)
#endif

// Structure of data private to each flash device
struct cyg_flash_fake_priv
{
    // Space for the block layout
    struct cyg_flash_block_info block_info[1];
    // Configuration parameters,
    size_t                      block_size;
    cyg_uint32                  blocks;
};

extern const struct cyg_flash_dev_funs cyg_flash_fake_funs;
// those parameters are defined in specific board.
#ifdef DDI_MISC_INUSE
#define FAKE_SPI_TOTAL_SIZE (0x1000000)
#define FAKE_SPI_BLOCK_SIZE (64 * 1024)
#endif
#define FAKE_SPI_BLOCKS (FAKE_SPI_TOTAL_SIZE/FAKE_SPI_BLOCK_SIZE)

static struct cyg_flash_fake_priv fake_flash_priv = {
    .block_size         = FAKE_SPI_BLOCK_SIZE,
    .blocks             = FAKE_SPI_BLOCKS,
};

static int fake_flash_init(struct cyg_flash_dev *dev)
{

    struct cyg_flash_fake_priv *priv = (struct cyg_flash_fake_priv*)dev->priv;
    dev->start = 0;
    dev->end = FAKE_SPI_TOTAL_SIZE - 1;
    dev->num_block_infos = 1;
    priv->block_info[0].blocks = FAKE_SPI_BLOCKS;
    priv->block_info[0].block_size = FAKE_SPI_BLOCK_SIZE;
    dev->block_info = &priv->block_info[0];

    _TRACE("[%s][%d][FAKE_SPI_TOTAL_SIZE=0x%08X][FAKE_SPI_BLOCK_SIZE=0x%08X][FAKE_SPI_BLOCKS=0x%08X]\n",
            __FUNCTION__, __LINE__, FAKE_SPI_TOTAL_SIZE, FAKE_SPI_BLOCK_SIZE, FAKE_SPI_BLOCKS);

    return CYG_FLASH_ERR_OK;
}

static int fake_flash_erase_block(struct cyg_flash_dev *dev, cyg_flashaddr_t block_base)
{
    _TRACE("dev=0x%08x block_base=0x%08x\n", (unsigned)dev, (unsigned)block_base);

    size_t block_size = dev->block_info[0].block_size;

    CYG_ASSERT((block_base >= dev->start) && (((block_base + block_size) - 1) <= dev->end), "Not inside device");

    MDrv_SERFLASH_WriteProtect(false);
    MDrv_SERFLASH_AddressErase(block_base, block_size - 1, true);

    return CYG_FLASH_ERR_OK;
}

static int fake_flash_read (struct cyg_flash_dev *dev, const cyg_flashaddr_t base, void* data, size_t len)
{
    static int i = 0;
    i++;
    if (i % 100 == 1)
    {
        _TRACE("i:%d dev=0x%08x base=0x%08x len=0x%08x\n", i, (unsigned)dev, (unsigned)base, len);
    }

    MDrv_SERFLASH_Read((MS_U32)base, (MS_U32)len, (MS_U8 *)data);

    return CYG_FLASH_ERR_OK;
}

static int fake_flash_program (struct cyg_flash_dev *dev, cyg_flashaddr_t base, void* data, size_t len)
{
    static int j = 0;
    j++;
    if (j % 25 == 1)
    {
        _TRACE("dev=0x%08x base=0x%08x len=0x%08x\n", (unsigned)dev, (unsigned)base, len);
    }

    CYG_ASSERT((base >= dev->start) && (((base + len) -1) <= dev->end), "Not inside device");

    MDrv_SERFLASH_WriteProtect(false);
    MDrv_SERFLASH_Write((MS_U32)base, (MS_U32)len, (MS_U8 *)data);

    return CYG_FLASH_ERR_OK;
}

// Map a hardware status to a package error
static int fake_flash_hwr_map_error(struct cyg_flash_dev *dev, int err)
{
    return err;
}

// Return the size of the block which is at the given address.
// __inline__ so that we know it will be in RAM, not ROM.
static __inline__ size_t flash_block_size(struct cyg_flash_dev *dev, const cyg_flashaddr_t addr)
{
    CYG_ASSERT((addr >= dev->start) && (addr <= dev->end), "Not inside device");

    return dev->block_info[0].block_size;
}

#define QUERY "Test Fake Flash"

static size_t fake_flash_query(struct cyg_flash_dev *dev, void * data, size_t len)
{
    memcpy(data,QUERY,sizeof(QUERY));
    return sizeof(QUERY);
}

// Just in case there is another flash driver which does implement locking
#ifdef CYGHWR_IO_FLASH_BLOCK_LOCKING
static int fake_flash_lock(struct cyg_flash_dev* dev, const cyg_flashaddr_t addr)
{
    return CYG_FLASH_ERR_INVALID;
}

static int fake_flash_unlock(struct cyg_flash_dev* dev, const yg_flashaddr_t addr)
{
    return CYG_FLASH_ERR_INVALID;
}
#endif

const CYG_FLASH_FUNS(cyg_flash_fake_funs,
        fake_flash_init,
        fake_flash_query,
        fake_flash_erase_block,
        fake_flash_program,
        fake_flash_read,                 // read
        fake_flash_hwr_map_error,
        fake_flash_lock,
        fake_flash_unlock);

CYG_FLASH_DRIVER(cyg_flash_fake_flashdev,
        &cyg_flash_fake_funs,
        0,                             // flags
        0,    // Start, if 0 will be updated by init
        0,                             // end, filled in by init
        0,                             // number of block_info's, filled in by init
        fake_flash_priv.block_info,
        &fake_flash_priv);


