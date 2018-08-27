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
#define MIU_DRAM_LEN                0x0004000000
#define MIU_DRAM_LEN0               0x0004000000

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
#define KERNEL_POOL_LEN                                        0x0001135000
#define KERNEL_POOL_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CACHED_POOL_BUF   */
//co_buffer L0
#define CACHED_POOL_BUF_AVAILABLE                              0x0001135000
#define CACHED_POOL_BUF_ADR                                    0x0001135000  //Alignment 0
#define CACHED_POOL_BUF_GAP_CHK                                0x0000000000
#define CACHED_POOL_BUF_LEN                                    0x0000100000
#define CACHED_POOL_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* PM51_BUF   */
//co_buffer L0
#define PM51_BUF_AVAILABLE                                     0x0001235000
#define PM51_BUF_ADR                                           0x0001235000  //Alignment 0x01000
#define PM51_BUF_GAP_CHK                                       0x0000000000
#define PM51_BUF_LEN                                           0x0000019400
#define PM51_BUF_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* NON_CACHED_POOL_BUF   */
//co_buffer L0
#define NON_CACHED_POOL_BUF_AVAILABLE                          0x000124E400
#define NON_CACHED_POOL_BUF_ADR                                0x000124F000  //Alignment 0x01000
#define NON_CACHED_POOL_BUF_GAP_CHK                            0x0000000C00
#define NON_CACHED_POOL_BUF_LEN                                0x000097F000
#define NON_CACHED_POOL_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* HW_BUF0_START_TAG   */
//co_buffer L0
#define HW_BUF0_START_TAG_AVAILABLE                            0x0001BCE000
#define HW_BUF0_START_TAG_ADR                                  0x0001BCE000  //Alignment 0
#define HW_BUF0_START_TAG_GAP_CHK                              0x0000000000
#define HW_BUF0_START_TAG_LEN                                  0x0000000000
#define HW_BUF0_START_TAG_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SECTION_BUF   */
//co_buffer L0
#define SECTION_BUF_AVAILABLE                                  0x0001BCE000
#define SECTION_BUF_ADR                                        0x0001BCE000  //Alignment 0x00010
#define SECTION_BUF_GAP_CHK                                    0x0000000000
#define SECTION_BUF_LEN                                        0x0000100000
#define SECTION_BUF_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SDRAM_ADR_BMP   */
//co_buffer L0
#define SDRAM_ADR_BMP_AVAILABLE                                0x0001CCE000
#define SDRAM_ADR_BMP_ADR                                      0x0001CCE000  //Alignment 0
#define SDRAM_ADR_BMP_GAP_CHK                                  0x0000000000
#define SDRAM_ADR_BMP_LEN                                      0x0000200000
#define SDRAM_ADR_BMP_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SDRAM_ADR_FONT   */
//co_buffer L0
#define SDRAM_ADR_FONT_AVAILABLE                               0x0001ECE000
#define SDRAM_ADR_FONT_ADR                                     0x0001ECE000  //Alignment 0
#define SDRAM_ADR_FONT_GAP_CHK                                 0x0000000000
#define SDRAM_ADR_FONT_LEN                                     0x0000005000
#define SDRAM_ADR_FONT_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SC0_MENULOAD_BUF   */
//co_buffer L0
#define SC0_MENULOAD_BUF_AVAILABLE                             0x0001ED3000
#define SC0_MENULOAD_BUF_ADR                                   0x0001ED3000  //Alignment 0x00010
#define SC0_MENULOAD_BUF_GAP_CHK                               0x0000000000
#define SC0_MENULOAD_BUF_LEN                                   0x0000004000
#define SC0_MENULOAD_BUF_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* TSP_VQ_BUF   */
//co_buffer L0
#define TSP_VQ_BUF_AVAILABLE                                   0x0001ED7000
#define TSP_VQ_BUF_ADR                                         0x0001ED7000  //Alignment 0
#define TSP_VQ_BUF_GAP_CHK                                     0x0000000000
#define TSP_VQ_BUF_LEN                                         0x0000001000
#define TSP_VQ_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VE_FRAME_BUF   */
//co_buffer L0
#define VE_FRAME_BUF_AVAILABLE                                 0x0001ED8000
#define VE_FRAME_BUF_ADR                                       0x0001ED8000  //Alignment 0x00008
#define VE_FRAME_BUF_GAP_CHK                                   0x0000000000
#define VE_FRAME_BUF_LEN                                       0x0000000000
#define VE_FRAME_BUF_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* GOP_REG_DMA_BASE   */
//co_buffer L0
#define GOP_REG_DMA_BASE_AVAILABLE                             0x0001ED8000
#define GOP_REG_DMA_BASE_ADR                                   0x0001ED8000  //Alignment 0x04000
#define GOP_REG_DMA_BASE_GAP_CHK                               0x0000000000
#define GOP_REG_DMA_BASE_LEN                                   0x0000001000
#define GOP_REG_DMA_BASE_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* GOP_GWIN_RB   */
//co_buffer L0
#define GOP_GWIN_RB_AVAILABLE                                  0x0001ED9000
#define GOP_GWIN_RB_ADR                                        0x0001ED9000  //Alignment 0x00010
#define GOP_GWIN_RB_GAP_CHK                                    0x0000000000
#define GOP_GWIN_RB_LEN                                        0x00001A0000
#define GOP_GWIN_RB_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* TSP_FW_BUF   */
//co_buffer L0
#define TSP_FW_BUF_AVAILABLE                                   0x0002079000
#define TSP_FW_BUF_ADR                                         0x0002079000  //Alignment 0x00100
#define TSP_FW_BUF_GAP_CHK                                     0x0000000000
#define TSP_FW_BUF_LEN                                         0x0000006000
#define TSP_FW_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* PM51_VAR_MEM   */
//co_buffer L0
#define PM51_VAR_MEM_AVAILABLE                                 0x000207F000
#define PM51_VAR_MEM_ADR                                       0x000207F000  //Alignment 0
#define PM51_VAR_MEM_GAP_CHK                                   0x0000000000
#define PM51_VAR_MEM_LEN                                       0x0000000400
#define PM51_VAR_MEM_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* Other   */
//co_buffer L4
#define Other_AVAILABLE                                        0x000207F000
#define Other_ADR                                              0x000207F000  //Alignment 0
#define Other_GAP_CHK                                          0x0000000000
#define Other_LEN                                              0x0000000000
#define Other_MEMORY_TYPE                                      (MIU0 | SW | CO_BUF_L4 | UNCACHED)

/* MIU0_COBUF_START   */
//co_buffer L0
#define MIU0_COBUF_START_AVAILABLE                             0x000207F400
#define MIU0_COBUF_START_ADR                                   0x000207F400  //Alignment 0
#define MIU0_COBUF_START_GAP_CHK                               0x0000000000
#define MIU0_COBUF_START_LEN                                   0x0000C1D000
#define MIU0_COBUF_START_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDPLAYER_TAG   */
//co_buffer L2
#define VDPLAYER_TAG_AVAILABLE                                 0x000207F400
#define VDPLAYER_TAG_ADR                                       0x000207F400  //Alignment 0
#define VDPLAYER_TAG_GAP_CHK                                   0x0000000000
#define VDPLAYER_TAG_LEN                                       0x0000000000
#define VDPLAYER_TAG_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* VDPLAYER_DATA   */
//co_buffer L2
#define VDPLAYER_DATA_AVAILABLE                                0x000207F400
#define VDPLAYER_DATA_ADR                                      0x000207F400  //Alignment 0
#define VDPLAYER_DATA_GAP_CHK                                  0x0000000000
#define VDPLAYER_DATA_LEN                                      0x000036F000
#define VDPLAYER_DATA_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* VDPLAYER_SUB_BS   */
//co_buffer L2
#define VDPLAYER_SUB_BS_AVAILABLE                              0x00023EE400
#define VDPLAYER_SUB_BS_ADR                                    0x00023EE400  //Alignment 0
#define VDPLAYER_SUB_BS_GAP_CHK                                0x0000000000
#define VDPLAYER_SUB_BS_LEN                                    0x00002E0000
#define VDPLAYER_SUB_BS_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* VDPLAYER_BS   */
//co_buffer L2
#define VDPLAYER_BS_AVAILABLE                                  0x00026CE400
#define VDPLAYER_BS_ADR                                        0x00026CF000  //Alignment 0x01000
#define VDPLAYER_BS_GAP_CHK                                    0x0000000C00
#define VDPLAYER_BS_LEN                                        0x0000200000
#define VDPLAYER_BS_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* VDPLAYER_SUB   */
//co_buffer L2
#define VDPLAYER_SUB_AVAILABLE                                 0x00028CF000
#define VDPLAYER_SUB_ADR                                       0x00028CF000  //Alignment 0x01000
#define VDPLAYER_SUB_GAP_CHK                                   0x0000000000
#define VDPLAYER_SUB_LEN                                       0x0000040000
#define VDPLAYER_SUB_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* VDPLAYER_DMX_SEC   */
//co_buffer L2
#define VDPLAYER_DMX_SEC_AVAILABLE                             0x000290F000
#define VDPLAYER_DMX_SEC_ADR                                   0x000290F000  //Alignment 0
#define VDPLAYER_DMX_SEC_GAP_CHK                               0x0000000000
#define VDPLAYER_DMX_SEC_LEN                                   0x0000004000
#define VDPLAYER_DMX_SEC_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* PHY_COPY_DATA   */
//co_buffer L2
#define PHY_COPY_DATA_AVAILABLE                                0x0002913000
#define PHY_COPY_DATA_ADR                                      0x0002913000  //Alignment 0x01000
#define PHY_COPY_DATA_GAP_CHK                                  0x0000000000
#define PHY_COPY_DATA_LEN                                      0x0000020000
#define PHY_COPY_DATA_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* PHOTO_SHARE_MEM   */
//co_buffer L1
#define PHOTO_SHARE_MEM_AVAILABLE                              0x000207F400
#define PHOTO_SHARE_MEM_ADR                                    0x000207F400  //Alignment 0
#define PHOTO_SHARE_MEM_GAP_CHK                                0x0000000000
#define PHOTO_SHARE_MEM_LEN                                    0x0000001000
#define PHOTO_SHARE_MEM_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* PHOTO_DISPLAY   */
//co_buffer L1
#define PHOTO_DISPLAY_AVAILABLE                                0x0002080400
#define PHOTO_DISPLAY_ADR                                      0x0002080400  //Alignment 0x00010
#define PHOTO_DISPLAY_GAP_CHK                                  0x0000000000
#define PHOTO_DISPLAY_LEN                                      0x00003FC000
#define PHOTO_DISPLAY_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* JPD_INTER_BUF   */
//co_buffer L1
#define JPD_INTER_BUF_AVAILABLE                                0x000247C400
#define JPD_INTER_BUF_ADR                                      0x000247C400  //Alignment 0x00200
#define JPD_INTER_BUF_GAP_CHK                                  0x0000000000
#define JPD_INTER_BUF_LEN                                      0x0000630000
#define JPD_INTER_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* JPD_READ_BUF   */
//co_buffer L1
#define JPD_READ_BUF_AVAILABLE                                 0x0002AAC400
#define JPD_READ_BUF_ADR                                       0x0002AAC400  //Alignment 0x00010
#define JPD_READ_BUF_GAP_CHK                                   0x0000000000
#define JPD_READ_BUF_LEN                                       0x0000050000
#define JPD_READ_BUF_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* JPD_OUT   */
//co_buffer L1
#define JPD_OUT_AVAILABLE                                      0x0002AFC400
#define JPD_OUT_ADR                                            0x0002AFC400  //Alignment 0x00008
#define JPD_OUT_GAP_CHK                                        0x0000000000
#define JPD_OUT_LEN                                            0x00001A0000
#define JPD_OUT_MEMORY_TYPE                                    (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF_L1_END_TAG   */
//co_buffer L1
#define MIU0_COBUF_L1_END_TAG_AVAILABLE                        0x0002C9C400
#define MIU0_COBUF_L1_END_TAG_ADR                              0x0002C9C400  //Alignment 0
#define MIU0_COBUF_L1_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L1_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L1_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* TSP_FQ_BUF   */
//co_buffer L2
#define TSP_FQ_BUF_AVAILABLE                                   0x0002933000
#define TSP_FQ_BUF_ADR                                         0x0002933000  //Alignment 0x00010
#define TSP_FQ_BUF_GAP_CHK                                     0x0000000000
#define TSP_FQ_BUF_LEN                                         0x00001FFF80
#define TSP_FQ_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF_L2_END_TAG   */
//co_buffer L2
#define MIU0_COBUF_L2_END_TAG_AVAILABLE                        0x0002B32F80
#define MIU0_COBUF_L2_END_TAG_ADR                              0x0002B32F80  //Alignment 0
#define MIU0_COBUF_L2_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L2_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L2_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF_L3_END_TAG   */
//co_buffer L3
#define MIU0_COBUF_L3_END_TAG_AVAILABLE                        0x000207F400
#define MIU0_COBUF_L3_END_TAG_ADR                              0x000207F400  //Alignment 0
#define MIU0_COBUF_L3_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L3_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L3_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* VDPLAYER_SHARE_MEM   */
//co_buffer L0
#define VDPLAYER_SHARE_MEM_AVAILABLE                           0x0002C9C400
#define VDPLAYER_SHARE_MEM_ADR                                 0x0002C9D000  //Alignment 0x01000
#define VDPLAYER_SHARE_MEM_GAP_CHK                             0x0000000C00
#define VDPLAYER_SHARE_MEM_LEN                                 0x0000001000
#define VDPLAYER_SHARE_MEM_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF_END_TAG   */
//co_buffer L0
#define MIU0_COBUF_END_TAG_AVAILABLE                           0x0002C9E000
#define MIU0_COBUF_END_TAG_ADR                                 0x0002C9E000  //Alignment 0
#define MIU0_COBUF_END_TAG_GAP_CHK                             0x0000000000
#define MIU0_COBUF_END_TAG_LEN                                 0x0000000000
#define MIU0_COBUF_END_TAG_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MAD_ADV_BUF   */
//co_buffer L0
#define MAD_ADV_BUF_AVAILABLE                                  0x0002C9E000
#define MAD_ADV_BUF_ADR                                        0x0002C9E000  //Alignment 0x01000
#define MAD_ADV_BUF_GAP_CHK                                    0x0000000000
#define MAD_ADV_BUF_LEN                                        0x0000240000
#define MAD_ADV_BUF_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_MEM_START_TAG   */
//co_buffer L0
#define VDEC_MEM_START_TAG_AVAILABLE                           0x0002EDE000
#define VDEC_MEM_START_TAG_ADR                                 0x0002EDE000  //Alignment 0
#define VDEC_MEM_START_TAG_GAP_CHK                             0x0000000000
#define VDEC_MEM_START_TAG_LEN                                 0x0000000000
#define VDEC_MEM_START_TAG_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_AEON   */
//co_buffer L0
#define VDEC_AEON_AVAILABLE                                    0x0002EDE000
#define VDEC_AEON_ADR                                          0x0002EDE000  //Alignment 0
#define VDEC_AEON_GAP_CHK                                      0x0000000000
#define VDEC_AEON_LEN                                          0x0000090000
#define VDEC_AEON_MEMORY_TYPE                                  (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_FRAME_BUF   */
//co_buffer L0
#define VDEC_FRAME_BUF_AVAILABLE                               0x0002F6E000
#define VDEC_FRAME_BUF_ADR                                     0x0002F6E000  //Alignment 0x00200
#define VDEC_FRAME_BUF_GAP_CHK                                 0x0000000000
#define VDEC_FRAME_BUF_LEN                                     0x0000570000
#define VDEC_FRAME_BUF_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_BIT_STREAM   */
//co_buffer L0
#define VDEC_BIT_STREAM_AVAILABLE                              0x00034DE000
#define VDEC_BIT_STREAM_ADR                                    0x00034DE000  //Alignment 0x00008
#define VDEC_BIT_STREAM_GAP_CHK                                0x0000000000
#define VDEC_BIT_STREAM_LEN                                    0x0000200000
#define VDEC_BIT_STREAM_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* HW_CA_BUF   */
//co_buffer L0
#define HW_CA_BUF_AVAILABLE                                    0x00036DE000
#define HW_CA_BUF_ADR                                          0x00036DE000  //Alignment 0
#define HW_CA_BUF_GAP_CHK                                      0x0000000000
#define HW_CA_BUF_LEN                                          0x0000433400
#define HW_CA_BUF_MEMORY_TYPE                                  (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SC0_MAIN_FB   */
//co_buffer L0
#define SC0_MAIN_FB_AVAILABLE                                  0x0003B11400
#define SC0_MAIN_FB_ADR                                        0x0003B11400  //Alignment 0
#define SC0_MAIN_FB_GAP_CHK                                    0x0000000000
#define SC0_MAIN_FB_LEN                                        0x0000200000
#define SC0_MAIN_FB_MEMORY_TYPE                                (MIU0 | HW | CO_BUF_L0 | UNCACHED)

/* UNUSED   */
//co_buffer L0
#define UNUSED_AVAILABLE                                       0x0003D11400
#define UNUSED_ADR                                             0x0003D11400  //Alignment 0
#define UNUSED_GAP_CHK                                         0x0000000000
#define UNUSED_LEN                                             0x00002CEC00
#define UNUSED_MEMORY_TYPE                                     (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SBOOT_LOADER   */
//co_buffer L0
#define SBOOT_LOADER_AVAILABLE                                 0x0003FE0000
#define SBOOT_LOADER_ADR                                       0x0003FE0000  //Alignment 0
#define SBOOT_LOADER_GAP_CHK                                   0x0000000000
#define SBOOT_LOADER_LEN                                       0x0000020000
#define SBOOT_LOADER_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SC0_SUB_FB   */
//co_buffer L0
#define SC0_SUB_FB_AVAILABLE                                   0x0004000000
#define SC0_SUB_FB_ADR                                         0x0004000000  //Alignment 0
#define SC0_SUB_FB_GAP_CHK                                     0x0000000000
#define SC0_SUB_FB_LEN                                         0x0000000000
#define SC0_SUB_FB_MEMORY_TYPE                                 (MIU0 | HW | CO_BUF_L0 | UNCACHED)

/* HW_BUF0_END_TAG   */
//co_buffer L0
#define HW_BUF0_END_TAG_AVAILABLE                              0x0004000000
#define HW_BUF0_END_TAG_ADR                                    0x0004000000  //Alignment 0
#define HW_BUF0_END_TAG_GAP_CHK                                0x0000000000
#define HW_BUF0_END_TAG_LEN                                    0x0000000000
#define HW_BUF0_END_TAG_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

//MIU_1_START
//MIU_END
#define MIU0_END_ADR                                           0x0004000000
