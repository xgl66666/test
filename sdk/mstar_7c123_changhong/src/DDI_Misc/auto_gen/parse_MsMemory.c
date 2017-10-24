#include <stdio.h>
#include <string.h>
#include "MsMemory.h"

void main(int argc, char** argv)
{
  FILE* var_fp = fopen("../../../bsp/linux/partition/auto_gen_output/var_list", "ab");
  FILE* con_fp = fopen("../../../bsp/linux/partition/auto_gen_output/conf_list", "ab");
	
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
#ifdef KERNEL_POOL3_ADR
  fprintf(con_fp, "LX_MEM3=0x%08X\n", KERNEL_POOL3_ADR);
#endif
#ifdef KERNEL_POOL3_LEN
  fprintf(con_fp, "LX_MEM3_LEN=0x%08X\n", KERNEL_POOL3_LEN);
#endif    
  fclose(con_fp);  
  
  return;
}
