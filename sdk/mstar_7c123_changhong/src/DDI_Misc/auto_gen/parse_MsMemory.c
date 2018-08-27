#include <stdio.h>
#include <string.h>
#include "MsMemory.h"

typedef struct _BusAddrInfo
{
    unsigned long phy_start;
    unsigned long bus_start;
    unsigned long len;
}BusAddrInfo;

#ifdef CHIP_KRONUS
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000}
    };
#elif CHIP_KAISERIN
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000},
        {0x10000000, 0x50000000, 0x10000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0x60000000, 0x20000000}
    };
#elif CHIP_KAISER
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0xA0000000, 0x40000000}
    };
#elif CHIP_KELTIC
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000}
    };
#elif CHIP_KERES
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000},
        {0x10000000, 0x50000000, 0x30000000}
    };
#elif CHIP_CLIPPERS
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0xA0000000, 0x40000000}
    };
#elif CHIP_KIRIN
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000},
        {0x10000000, 0x50000000, 0x30000000}
    };
#elif CHIP_KRIS
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000},
        {0x10000000, 0x50000000, 0x30000000}
    };
#elif CHIP_KAYLA
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000},
        {0x10000000, 0x50000000, 0x30000000}
    };
#elif CHIP_MUJI
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0xA0000000, 0x40000000}
    };
#elif CHIP_KRATOS
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000}
    };
#elif CHIP_KANO
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0xC0000000, 0x40000000}
    };
#elif CHIP_KIWI
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x00000000, 0x10000000},
        {0x10000000, 0x50000000, 0x10000000}
    };
#elif CHIP_K6
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0x60000000, 0x80000000}
    };
#elif CHIP_CURRY
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0xC0000000, 0x40000000}
    };
#elif CHIP_C2P
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };
#elif CHIP_K6LITE
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x40000000}
    };

    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0x60000000, 0x40000000}
    };
#elif CHIP_K5TN
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x40000000, 0x40000000},
    };
#elif CHIP_K1C
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x40000000, 0x40000000},
    };
#elif CHIP_K5AP
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x40000000, 0x40000000},
    };
#elif CHIP_K7U
    BusAddrInfo BusAddrMIU0[] =
    {
        {0x00000000, 0x20000000, 0x80000000}
    };
    #define MIU1_IN_USE
    BusAddrInfo BusAddrMIU1[] =
    {
        {0x00000000, 0xa0000000, 0x40000000}
    };
#endif

int GetBusOffset(unsigned int miu_type, unsigned long phy_start, unsigned long phy_len, unsigned long *retBuAddr)
{
    int idx = 0, idx_max = 0;
    BusAddrInfo *pBusAddr = NULL;
#ifdef MIU1_IN_USE
    if ((miu_type & MIU1) == MIU1)
    {
        pBusAddr = BusAddrMIU1;
        idx_max = sizeof(BusAddrMIU1) / sizeof(BusAddrInfo);
    }
    else
#endif
    {
        pBusAddr = BusAddrMIU0;
        idx_max = sizeof(BusAddrMIU0) / sizeof(BusAddrInfo);
    }
    printf("miu_type=%d, phy_start=0x%lX, phy_len=0x%lX, idx_max=%d\r\n", miu_type, phy_start, phy_len, idx_max);

    for (idx=0; idx<idx_max; idx++)
    {
        if (phy_start >= (pBusAddr[idx].phy_start+pBusAddr[idx].len))
            continue;

        if ((phy_start+phy_len) > (pBusAddr[idx].phy_start+pBusAddr[idx].len))
        {
            return 0;
        }
        else
        {
            *retBuAddr = pBusAddr[idx].bus_start + phy_start - (idx*0x10000000);
            break;
        }
    }

    return 1;
}

void GetPhyAddrOffset(unsigned int miu_type, unsigned long phy_start, unsigned long *retBuAddr)
{
    int idx = 0, idx_max = 0;
    BusAddrInfo *pBusAddr = NULL;
#ifdef MIU1_IN_USE
    if ((miu_type & MIU1) == MIU1)
    {
        // Increase MIU interval for physical Address
        *retBuAddr = phy_start + 0x80000000;
    }
    else
#endif
    {
        *retBuAddr = phy_start;
    }
}

void main(int argc, char** argv)
{
  unsigned long BusAddr = 0;
  unsigned long PhyAddr = 0;
  FILE* var_fp;
  FILE* con_fp;
  //printf("argc=%d\n",argc);
  if(argc > 1){
    if(!strcmp(argv[1], "ecos")){
      var_fp = fopen("../../../bsp/ecos/partition/auto_gen_output/var_list", "ab");
      con_fp = fopen("../../../bsp/ecos/partition/auto_gen_output/conf_list", "wb");
    }
  }else{
    var_fp = fopen("../../../bsp/linux/partition/auto_gen_output/var_list", "ab");
    con_fp = fopen("../../../bsp/linux/partition/auto_gen_output/conf_list", "wb");
  }

/* output memory map info to var_list */
  fprintf(var_fp, "\n### Parsed from MMAP ###\n");
  fprintf(con_fp, "SCA_TOOL_VERSION=%s\n", SCA_TOOL_VERSION);
  fprintf(var_fp, "DRAM_LEN0=0x%08X\n", MIU_DRAM_LEN0);
#ifdef MIU_DRAM_LEN1
  fprintf(var_fp, "MIU_DRAM_LEN1=0x%08X\n", MIU_DRAM_LEN1);
#else
  fprintf(var_fp, "MIU_DRAM_LEN1=0x%08X\n", 0);
#endif
  fprintf(var_fp, "MIU_INTERVAL=0x%08X\n", MIU_INTERVAL);
  fprintf(var_fp, "LX_MEM_ADR=0x%08X\n", KERNEL_POOL_ADR);
  fprintf(var_fp, "LX_MEM_LEN=0x%08X\n", KERNEL_POOL_LEN);
#ifdef EMAC_MEM_LEN
  fprintf(var_fp, "EMAC_MEM=0x%08X\n", EMAC_MEM_LEN);
#else
  fprintf(var_fp, "EMAC_MEM=0x%08X\n", 0);
#endif
#ifdef GMAC_MEM_ADR
  fprintf(var_fp, "GMAC_MEM=0x%08X\n", GMAC_MEM_ADR);
#else
  fprintf(var_fp, "GMAC_MEM=0x%08X\n", 0);
#endif
#ifdef GMAC_MEM_LEN
  fprintf(var_fp, "GMAC_MEM_LEN=0x%08X\n", GMAC_MEM_LEN);
#else
  fprintf(var_fp, "GMAC_MEM_LEN=0x%08X\n", 0);
#endif
#ifdef E_MMAP_ID_PM51_USAGE_MEM_ADR
  fprintf(var_fp, "E_MMAP_ID_PM51_USAGE_MEM_ADR=0x%08X\n", E_MMAP_ID_PM51_USAGE_MEM_ADR);
#else
  fprintf(var_fp, "E_MMAP_ID_PM51_USAGE_MEM_ADR=0x%08X\n", 0);
#endif

#ifdef E_MMAP_ID_HW_AES_BUF_ADR
  fprintf(var_fp, "E_MMAP_ID_HW_AES_BUF_ADR=0x%08X\n", E_MMAP_ID_HW_AES_BUF_ADR);
#else
  fprintf(var_fp, "E_MMAP_ID_HW_AES_BUF_ADR=0x%08X\n", 0);
#endif

#ifdef E_MMAP_ID_HW_AES_BUF_LEN
  fprintf(var_fp, "E_MMAP_ID_HW_AES_BUF_LEN=0x%08X\n", E_MMAP_ID_HW_AES_BUF_LEN);
#else
  fprintf(var_fp, "E_MMAP_ID_HW_AES_BUF_LEN=0x%08X\n", 0);
#endif

#ifdef KERNEL_POOL2_ADR
  fprintf(var_fp, "LX_MEM2=0x%08X\n", KERNEL_POOL2_ADR);
#else
  fprintf(var_fp, "LX_MEM2=0x%08X\n", 0);
#endif
#ifdef KERNEL_POOL2_LEN
  fprintf(var_fp, "LX_MEM2_LEN=0x%08X\n", KERNEL_POOL2_LEN);
#else
  fprintf(var_fp, "LX_MEM2_LEN=0x%08X\n", 0);
#endif
#ifdef KERNEL_POOL2_MEMORY_TYPE
  fprintf(var_fp, "LX_MEM2_TYPE=%s\n", (KERNEL_POOL2_MEMORY_TYPE&0x1)?("MIU1"):("MIU0"));
  if (GetBusOffset((unsigned int)KERNEL_POOL2_MEMORY_TYPE, (unsigned long)KERNEL_POOL2_ADR, (unsigned long)KERNEL_POOL2_LEN, &BusAddr) == 1)
    fprintf(var_fp, "LX_MEM2_BUS_ADDR=0x%08lX\n", BusAddr);
  else
    fprintf(var_fp, "LX_MEM2_BUS_ADDR=%s\n", "INVALID");

  GetPhyAddrOffset((unsigned int)KERNEL_POOL2_MEMORY_TYPE, (unsigned long)KERNEL_POOL2_ADR, &PhyAddr);
  fprintf(var_fp, "LX_MEM2_PHY_ADDR=0x%08lX\n", PhyAddr);
#else
  fprintf(var_fp, "LX_MEM2_TYPE=0x%08X\n", 0);
#endif
#ifdef KERNEL_POOL3_ADR
  fprintf(var_fp, "LX_MEM3=0x%08X\n", KERNEL_POOL3_ADR);
#else
  fprintf(var_fp, "LX_MEM3=0x%08X\n", 0);
#endif
#ifdef KERNEL_POOL3_LEN
  fprintf(var_fp, "LX_MEM3_LEN=0x%08X\n", KERNEL_POOL3_LEN);
#else
  fprintf(var_fp, "LX_MEM3_LEN=0x%08X\n", 0);
#endif
#ifdef KERNEL_POOL3_MEMORY_TYPE
  fprintf(var_fp, "LX_MEM3_TYPE=%s\n", (KERNEL_POOL3_MEMORY_TYPE&0x1)?("MIU1"):("MIU0"));
  if (GetBusOffset((unsigned int)KERNEL_POOL3_MEMORY_TYPE, (unsigned long)KERNEL_POOL3_ADR, (unsigned long)KERNEL_POOL3_LEN, &BusAddr) == 1)
    fprintf(var_fp, "LX_MEM3_BUS_ADDR=0x%08lX\n", BusAddr);
  else
    fprintf(var_fp, "LX_MEM3_BUS_ADDR=%s\n", "INVALID");
  GetPhyAddrOffset((unsigned int)KERNEL_POOL3_MEMORY_TYPE, (unsigned long)KERNEL_POOL3_ADR, &PhyAddr);
  fprintf(var_fp, "LX_MEM3_PHY_ADDR=0x%08lX\n", PhyAddr);
#else
  fprintf(var_fp, "LX_MEM3_TYPE=0x%08X\n", 0);
#endif
#ifdef VDEC_FRAME_BUF_ADR
  fprintf(var_fp, "VDEC_FRAME_BUF_ADR=0x%08X\n", VDEC_FRAME_BUF_ADR);
#else
  fprintf(var_fp, "VDEC_FRAME_BUF_ADR=0x%08X\n", 0);
#endif

// The MMAP for bootvideo
#ifdef MAD_ADV_BUF_ADR
    fprintf(var_fp, "MAD_ADV_BUF_ADR=0x%08X\n", MAD_ADV_BUF_ADR);
#else
    fprintf(var_fp, "MAD_ADV_BUF_ADR=0x%08X\n", 0);
#endif
#ifdef MAD_ADV_BUF_LEN
    fprintf(var_fp, "MAD_ADV_BUF_LEN=0x%08X\n", MAD_ADV_BUF_LEN);
#else
    fprintf(var_fp, "MAD_ADV_BUF_LEN=0x%08X\n", 0);
#endif
#ifdef VDEC_AEON_ADR
    fprintf(var_fp, "VDEC_AEON_ADR=0x%08X\n", VDEC_AEON_ADR);
#else
    fprintf(var_fp, "VDEC_AEON_ADR=0x%08X\n", 0);
#endif
#ifdef VDEC_AEON_LEN
    fprintf(var_fp, "VDEC_AEON_LEN=0x%08X\n", VDEC_AEON_LEN);
#else
    fprintf(var_fp, "VDEC_AEON_LEN=0x%08X\n", 0);
#endif
#ifdef VDEC_FRAME_BUF_LEN
    fprintf(var_fp, "VDEC_FRAME_BUF_LEN=0x%08X\n", VDEC_FRAME_BUF_LEN);
#else
    fprintf(var_fp, "VDEC_FRAME_BUF_LEN=0x%08X\n", 0);
#endif
#ifdef VDEC_BIT_STREAM_ADR
    fprintf(var_fp, "VDEC_BIT_STREAM_ADR=0x%08X\n", VDEC_BIT_STREAM_ADR);
#else
    fprintf(var_fp, "VDEC_BIT_STREAM_ADR=0x%08X\n", 0);
#endif
#ifdef VDEC_BIT_STREAM_LEN
    fprintf(var_fp, "VDEC_BIT_STREAM_LEN=0x%08X\n", VDEC_BIT_STREAM_LEN);
#else
    fprintf(var_fp, "VDEC_BIT_STREAM_LEN=0x%08X\n", 0);
#endif
#ifdef SC0_MAIN_FB_ADR
    fprintf(var_fp, "SC0_MAIN_FB_ADR=0x%08X\n", SC0_MAIN_FB_ADR);
#else
    fprintf(var_fp, "SC0_MAIN_FB_ADR=0x%08X\n", 0);
#endif
#ifdef SC0_MAIN_FB_LEN
    fprintf(var_fp, "SC0_MAIN_FB_LEN=0x%08X\n", SC0_MAIN_FB_LEN);
#else
    fprintf(var_fp, "SC0_MAIN_FB_LEN=0x%08X\n", 0);
#endif
#ifdef SC1_MAIN_FB_ADR
    fprintf(var_fp, "SC1_MAIN_FB_ADR=0x%08X\n", SC1_MAIN_FB_ADR);
#else
    fprintf(var_fp, "SC1_MAIN_FB_ADR=0x%08X\n", 0);
#endif
#ifdef SC1_MAIN_FB_LEN
    fprintf(var_fp, "SC1_MAIN_FB_LEN=0x%08X\n", SC1_MAIN_FB_LEN);
#else
    fprintf(var_fp, "SC1_MAIN_FB_LEN=0x%08X\n", 0);
#endif

#ifdef SC0_MENULOAD_BUF_ADR
    fprintf(var_fp, "SC0_MENULOAD_BUF_ADR=0x%08X\n", SC0_MENULOAD_BUF_ADR);
#else
    fprintf(var_fp, "SC0_MENULOAD_BUF_ADR=0x%08X\n", 0);
#endif
#ifdef SC0_MENULOAD_BUF_LEN
    fprintf(var_fp, "SC0_MENULOAD_BUF_LEN=0x%08X\n", SC0_MENULOAD_BUF_LEN);
#else
    fprintf(var_fp, "SC0_MENULOAD_BUF_LEN=0x%08X\n", 0);
#endif
#ifdef BOOTVIDEO_FILE_IN_ADR
    fprintf(var_fp, "BOOTVIDEO_FILE_IN_ADR=0x%08X\n", BOOTVIDEO_FILE_IN_ADR);
#else
    fprintf(var_fp, "BOOTVIDEO_FILE_IN_ADR=0x%08X\n", 0);
#endif
#ifdef BOOTVIDEO_FILE_IN_LEN
    fprintf(var_fp, "BOOTVIDEO_FILE_IN_LEN=0x%08X\n", BOOTVIDEO_FILE_IN_LEN);
#else
    fprintf(var_fp, "BOOTVIDEO_FILE_IN_LEN=0x%08X\n", 0);
#endif


#ifdef JPD_OUT_ADR
  fprintf(var_fp, "JPD_OUT_ADR=0x%08X\n", JPD_OUT_ADR);
#else
  fprintf(var_fp, "JPD_OUT_ADR=0x%08X\n", 0);
#endif
#ifdef JPD_OUT_LEN
  fprintf(var_fp, "JPD_OUT_LEN=0x%08X\n", JPD_OUT_LEN);
#else
  fprintf(var_fp, "JPD_OUT_LEN=0x%08X\n", 0);
#endif
#ifdef JPD_READ_BUF_ADR
  fprintf(var_fp, "JPD_READ_BUF_ADR=0x%08X\n", JPD_READ_BUF_ADR);
#else
  fprintf(var_fp, "JPD_READ_BUF_ADR=0x%08X\n", 0);
#endif
#ifdef JPD_READ_BUF_LEN
  fprintf(var_fp, "JPD_READ_BUF_LEN=0x%08X\n", JPD_READ_BUF_LEN);
#else
  fprintf(var_fp, "JPD_READ_BUF_LEN=0x%08X\n", 0);
#endif
#ifdef JPD_INTER_BUF_ADR
  fprintf(var_fp, "JPD_INTER_BUF_ADR=0x%08X\n", JPD_INTER_BUF_ADR);
#else
  fprintf(var_fp, "JPD_INTER_BUF_ADR=0x%08X\n", 0);
#endif
#ifdef JPD_INTER_BUF_LEN
  fprintf(var_fp, "JPD_INTER_BUF_LEN=0x%08X\n", JPD_INTER_BUF_LEN);
#else
  fprintf(var_fp, "JPD_INTER_BUF_LEN=0x%08X\n", 0);
#endif
#ifdef VE_FRAME_BUF_ADR
  fprintf(var_fp, "VE_FRAME_BUF_ADR=0x%08X\n", VE_FRAME_BUF_ADR);
#else
  fprintf(var_fp, "VE_FRAME_BUF_ADR=0x%08X\n", 0);
#endif
#ifdef VE_FRAME_BUF_LEN
  fprintf(var_fp, "VE_FRAME_BUF_LEN=0x%08X\n", VE_FRAME_BUF_LEN);
#else
  fprintf(var_fp, "VE_FRAME_BUF_LEN=0x%08X\n", 0);
#endif
#if defined(MI_PHOTO_SHARE_MEM_ADR) && defined(MI_PHOTO_SHARE_MEM_LEN)
  fprintf(var_fp, "DFB_FRAMEBUFFER_ADR=0x%08X\n",(MI_PHOTO_SHARE_MEM_ADR+MI_PHOTO_SHARE_MEM_LEN));
#endif
  fclose(var_fp);


	/* output memory map info to conf_list */
  fprintf(con_fp, "SCA_TOOL_VERSION=%s\n", SCA_TOOL_VERSION);
  fprintf(con_fp, "DRAM_LEN0=0x%08X\n", MIU_DRAM_LEN0);
#ifdef MIU_DRAM_LEN1
  fprintf(con_fp, "MIU_DRAM_LEN1=0x%08X\n", MIU_DRAM_LEN1);
#endif
  fprintf(con_fp, "MIU_INTERVAL=0x%08X\n", MIU_INTERVAL);
  fprintf(con_fp, "LX_MEM_LEN=0x%08X\n", KERNEL_POOL_LEN);
#ifdef EMAC_MEM_LEN
  fprintf(con_fp, "EMAC_MEM=0x%08X\n", EMAC_MEM_LEN);
#endif
  fprintf(con_fp, "LX_MEM_LEN=0x%08X\n", KERNEL_POOL_LEN);
#ifdef GMAC_MEM_ADR
  fprintf(con_fp, "GMAC_MEM=0x%08X\n", GMAC_MEM_ADR);
#endif
#ifdef GMAC_MEM_LEN
  fprintf(con_fp, "GMAC_MEM_LEN=0x%08X\n", GMAC_MEM_LEN);
#endif
#ifdef KERNEL_POOL2_ADR
  fprintf(con_fp, "LX_MEM2=0x%08X\n", KERNEL_POOL2_ADR);
#endif
#ifdef KERNEL_POOL2_LEN
  fprintf(con_fp, "LX_MEM2_LEN=0x%08X\n", KERNEL_POOL2_LEN);
#endif
#ifdef KERNEL_POOL2_MEMORY_TYPE
  fprintf(con_fp, "LX_MEM2_TYPE=%s\n", (KERNEL_POOL2_MEMORY_TYPE&0x1)?("MIU1"):("MIU0"));
#endif
#ifdef KERNEL_POOL3_ADR
  fprintf(con_fp, "LX_MEM3=0x%08X\n", KERNEL_POOL3_ADR);
#endif
#ifdef KERNEL_POOL3_LEN
  fprintf(con_fp, "LX_MEM3_LEN=0x%08X\n", KERNEL_POOL3_LEN);
#endif
#ifdef KERNEL_POOL3_MEMORY_TYPE
  fprintf(con_fp, "LX_MEM3_TYPE=%s\n", (KERNEL_POOL3_MEMORY_TYPE&0x1)?("MIU1"):("MIU0"));
#endif
  fclose(con_fp);

  return;
}
