////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
//
////////////////////////////////////////////////////////////////////////////////

#define SCA_TOOL_VERSION            "Chakra SCA V1.1.0 "

////////////////////////////////////////////////////////////////////////////////
// DRAM memory map
//
// Every Module Memory Mapping need 4 define,
// and check code in "msAPI_Memory_DumpMemoryMap"
// 1. XXX_AVAILABLE : For get avaialble memory start address
// 2. XXX_ADR       : Real Address with Alignment
// 3. XXX_GAP_CHK   : For Check Memory Gap, for avoid memory waste
// 4. XXX_LEN       : For the Memory size of this Module usage
////////////////////////////////////////////////////////////////////////////////
#define MIU_DRAM_LEN                0x0002000000
#define MIU_DRAM_LEN0               0x0002000000

#define MIU_DRAM_LEN1               0x0000000000

#define ENABLE_MIU_1                0
#define MIU_INTERVAL                0x0000000000

#define MIU0_MMAP_BOUNDARY                0x0000000000
#define MIU1_MMAP_BOUNDARY                0x0000000000
////////////////////////////////////////////////////////////////////////////////
//MEMORY TYPE
////////////////////////////////////////////////////////////////////////////////
#define MIU0                        (0x0000)
#define MIU1                        (0x0001)

#define SW                          (0x0000 << 1)
#define HW                          (0x0001 << 1)
#define SW_HW                       (0x0002 << 1)

#define UNCACHED                    (0x0000 << 3)
#define WRITE_THROUGH               (0x0001 << 3)
#define WRITE_COMBINING             (0x0002 << 3)
#define WRITE_PROTECT               (0x0003 << 3)
#define WRITE_BACK                  (0x0004 << 3)

#define CO_BUF_OFFSET               (6)
#define CO_BUF_L0                   (0x0000 << CO_BUF_OFFSET)
#define CO_BUF_L1                   (0x0001 << CO_BUF_OFFSET)
#define CO_BUF_L2                   (0x0002 << CO_BUF_OFFSET)
#define CO_BUF_L3                   (0x0003 << CO_BUF_OFFSET)
#define CO_BUF_L4                   (0x0004 << CO_BUF_OFFSET)
#define CO_BUF_L5                   (0x0005 << CO_BUF_OFFSET)
#define CO_BUF_L6                   (0x0006 << CO_BUF_OFFSET)
#define CO_BUF_L7                   (0x0007 << CO_BUF_OFFSET)
#define CO_BUF_L8                   (0x0008 << CO_BUF_OFFSET)
#define CO_BUF_L9                   (0x0009 << CO_BUF_OFFSET)
#define CO_BUF_L10                  (0x000A << CO_BUF_OFFSET)
#define CO_BUF_L11                  (0x000B << CO_BUF_OFFSET)
#define MIU0_LAYER_COUNT            (5)
#define MIU1_LAYER_COUNT            (0)

//MIU_0_START
/* KERNEL_POOL   */
//co_buffer L0
#define KERNEL_POOL_AVAILABLE                                  0x0000000000
#define KERNEL_POOL_ADR                                        0x0000000000  //Alignment 0
#define KERNEL_POOL_GAP_CHK                                    0x0000000000
#define KERNEL_POOL_LEN                                        0x0000A00000
#define KERNEL_POOL_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CACHED_POOL_BUF   */
//co_buffer L0
#define CACHED_POOL_BUF_AVAILABLE                              0x0000A00000
#define CACHED_POOL_BUF_ADR                                    0x0000A00000  //Alignment 0
#define CACHED_POOL_BUF_GAP_CHK                                0x0000000000
#define CACHED_POOL_BUF_LEN                                    0x0000100000
#define CACHED_POOL_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* PM51_BUF   */
//co_buffer L0
#define PM51_BUF_AVAILABLE                                     0x0000B00000
#define PM51_BUF_ADR                                           0x0000B00000  //Alignment 0x01000
#define PM51_BUF_GAP_CHK                                       0x0000000000
#define PM51_BUF_LEN                                           0x0000019400
#define PM51_BUF_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* NON_CACHED_POOL_BUF   */
//co_buffer L0
#define NON_CACHED_POOL_BUF_AVAILABLE                          0x0000B19400
#define NON_CACHED_POOL_BUF_ADR                                0x0000B1A000  //Alignment 0x01000
#define NON_CACHED_POOL_BUF_GAP_CHK                            0x0000000C00
#define NON_CACHED_POOL_BUF_LEN                                0x000097F000
#define NON_CACHED_POOL_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* HW_BUF0_START_TAG   */
//co_buffer L0
#define HW_BUF0_START_TAG_AVAILABLE                            0x0001499000
#define HW_BUF0_START_TAG_ADR                                  0x0001499000  //Alignment 0
#define HW_BUF0_START_TAG_GAP_CHK                              0x0000000000
#define HW_BUF0_START_TAG_LEN                                  0x0000000000
#define HW_BUF0_START_TAG_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SECTION_BUF   */
//co_buffer L0
#define SECTION_BUF_AVAILABLE                                  0x0001499000
#define SECTION_BUF_ADR                                        0x0001499000  //Alignment 0x00010
#define SECTION_BUF_GAP_CHK                                    0x0000000000
#define SECTION_BUF_LEN                                        0x0000100000
#define SECTION_BUF_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* TSP_VQ_BUF   */
//co_buffer L0
#define TSP_VQ_BUF_AVAILABLE                                   0x0001599000
#define TSP_VQ_BUF_ADR                                         0x0001599000  //Alignment 0
#define TSP_VQ_BUF_GAP_CHK                                     0x0000000000
#define TSP_VQ_BUF_LEN                                         0x0000001000
#define TSP_VQ_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* TSP_FW_BUF   */
//co_buffer L0
#define TSP_FW_BUF_AVAILABLE                                   0x000159A000
#define TSP_FW_BUF_ADR                                         0x000159A000  //Alignment 0x00100
#define TSP_FW_BUF_GAP_CHK                                     0x0000000000
#define TSP_FW_BUF_LEN                                         0x0000006000
#define TSP_FW_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* PM51_VAR_MEM   */
//co_buffer L0
#define PM51_VAR_MEM_AVAILABLE                                 0x00015A0000
#define PM51_VAR_MEM_ADR                                       0x00015A0000  //Alignment 0
#define PM51_VAR_MEM_GAP_CHK                                   0x0000000000
#define PM51_VAR_MEM_LEN                                       0x0000000400
#define PM51_VAR_MEM_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* Other   */
//co_buffer L4
#define Other_AVAILABLE                                        0x00015A0000
#define Other_ADR                                              0x00015A0000  //Alignment 0
#define Other_GAP_CHK                                          0x0000000000
#define Other_LEN                                              0x0000000000
#define Other_MEMORY_TYPE                                      (MIU0 | SW | CO_BUF_L4 | UNCACHED)

/* MIU0_COBUF_START   */
//co_buffer L0
#define MIU0_COBUF_START_AVAILABLE                             0x00015A0400
#define MIU0_COBUF_START_ADR                                   0x00015A0400  //Alignment 0
#define MIU0_COBUF_START_GAP_CHK                               0x0000000000
#define MIU0_COBUF_START_LEN                                   0x0000000000
#define MIU0_COBUF_START_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF_L1_END_TAG   */
//co_buffer L1
#define MIU0_COBUF_L1_END_TAG_AVAILABLE                        0x00015A0400
#define MIU0_COBUF_L1_END_TAG_ADR                              0x00015A0400  //Alignment 0
#define MIU0_COBUF_L1_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L1_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L1_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF_L2_END_TAG   */
//co_buffer L2
#define MIU0_COBUF_L2_END_TAG_AVAILABLE                        0x00015A0400
#define MIU0_COBUF_L2_END_TAG_ADR                              0x00015A0400  //Alignment 0
#define MIU0_COBUF_L2_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L2_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L2_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF_L3_END_TAG   */
//co_buffer L3
#define MIU0_COBUF_L3_END_TAG_AVAILABLE                        0x00015A0400
#define MIU0_COBUF_L3_END_TAG_ADR                              0x00015A0400  //Alignment 0
#define MIU0_COBUF_L3_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L3_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L3_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MIU0_COBUF_END_TAG   */
//co_buffer L0
#define MIU0_COBUF_END_TAG_AVAILABLE                           0x00015A0400
#define MIU0_COBUF_END_TAG_ADR                                 0x00015A0400  //Alignment 0
#define MIU0_COBUF_END_TAG_GAP_CHK                             0x0000000000
#define MIU0_COBUF_END_TAG_LEN                                 0x0000000000
#define MIU0_COBUF_END_TAG_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* HW_CA_BUF   */
//co_buffer L0
#define HW_CA_BUF_AVAILABLE                                    0x00015A0400
#define HW_CA_BUF_ADR                                          0x00015A0400  //Alignment 0
#define HW_CA_BUF_GAP_CHK                                      0x0000000000
#define HW_CA_BUF_LEN                                          0x0000433400
#define HW_CA_BUF_MEMORY_TYPE                                  (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* UNUSED   */
//co_buffer L0
#define UNUSED_AVAILABLE                                       0x00019D3800
#define UNUSED_ADR                                             0x00019D3800  //Alignment 0
#define UNUSED_GAP_CHK                                         0x0000000000
#define UNUSED_LEN                                             0x0000001E00
#define UNUSED_MEMORY_TYPE                                     (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SBOOT_LOADER   */
//co_buffer L0
#define SBOOT_LOADER_AVAILABLE                                 0x00019D5600
#define SBOOT_LOADER_ADR                                       0x00019D5600  //Alignment 0
#define SBOOT_LOADER_GAP_CHK                                   0x0000000000
#define SBOOT_LOADER_LEN                                       0x0000020000
#define SBOOT_LOADER_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SC0_SUB_FB   */
//co_buffer L0
#define SC0_SUB_FB_AVAILABLE                                   0x00019F5600
#define SC0_SUB_FB_ADR                                         0x00019F5600  //Alignment 0
#define SC0_SUB_FB_GAP_CHK                                     0x0000000000
#define SC0_SUB_FB_LEN                                         0x0000000000
#define SC0_SUB_FB_MEMORY_TYPE                                 (MIU0 | HW | CO_BUF_L0 | UNCACHED)

/* HW_BUF0_END_TAG   */
//co_buffer L0
#define HW_BUF0_END_TAG_AVAILABLE                              0x00019F5600
#define HW_BUF0_END_TAG_ADR                                    0x00019F5600  //Alignment 0
#define HW_BUF0_END_TAG_GAP_CHK                                0x0000000000
#define HW_BUF0_END_TAG_LEN                                    0x0000000000
#define HW_BUF0_END_TAG_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

//MIU_1_START
//MIU_END
#define MIU0_END_ADR                                           0x00019F5600
