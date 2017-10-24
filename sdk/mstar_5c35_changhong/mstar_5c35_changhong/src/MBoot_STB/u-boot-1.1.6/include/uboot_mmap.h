#ifndef __UBOOT_MMAP_H__
#define __UBOOT_MMAP_H__

#define UPGRADE_BUFFER_ADDR     0x80000000

#define verJPD_READBUFF_ADR     0x00000000  //Alignment 0x00010
#define verJPD_READBUFF_LEN     0x00100000
#define verJPD_INTERBUFF_ADR    0x00100000  //Alignment 0x00200
#define verJPD_INTERBUFF_LEN    0x00630000
#define verJPD_OUT_ADR          0x00730000  //Alignment 0x00008
#define verJPD_OUT_LEN          0x003FC000
#define LOGO_JPG_DRAM_ADDR      0x00B2C000
#if (defined(CONFIG_KRONUS) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI))
#if (defined(CONFIG_MSTAR_MMAP_128MB) || defined(CONFIG_MSTAR_MMAP_256MB))
#define VE_BUFFER_ADDR          0x06A00000  // K1 128MB and 256MB VDEC Frame buffer
#define GOP_BUFFER_ADDR         0x06400000
#else
#define VE_BUFFER_ADDR          0x03200000  // K1 64MB VDEC Frame buffer
#define GOP_BUFFER_ADDR         0x02400000  
#endif
#elif (defined(CONFIG_KELTIC))
#if defined(CONFIG_MSTAR_MMAP_128MB)
#define VE_BUFFER_ADDR          0x06A00000  // K1 128MB and 256MB VDEC Frame buffer
#define GOP_BUFFER_ADDR         0x06400000
#elif defined(CONFIG_MSTAR_MMAP_64MB)
#define VE_BUFFER_ADDR          0x02C00000
#define GOP_BUFFER_ADDR         0x02100000
#elif defined(CONFIG_MSTAR_MMAP_32MB)
#define VE_BUFFER_ADDR          0x00C00000
#define GOP_BUFFER_ADDR         0x00900000
#else
#define VE_BUFFER_ADDR          0x02C00000
#define GOP_BUFFER_ADDR         0x02100000
#endif

#else
#define VE_BUFFER_ADDR          0x06C00000
#if (MS_BOARD_TYPE_SEL == BD_MST098F_D01A) || (MS_BOARD_TYPE_SEL == BD_MST098F_D01B)
#define GOP_BUFFER_ADDR         0x06100000
#else
#define GOP_BUFFER_ADDR         0x06E00000
#endif
#endif
#define VE_FRAMEBUFFER_OFFSET   16 //8(pixels)*2(Bytes)

#define UPGRADE_ADDR_STR        "UPGRADE_MEM_ADDR"
#endif //__UBOOT_MMAP_H__
