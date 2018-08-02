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

#define SCA_TOOL_VERSION            "Chakra SCA V1.0.0 "

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
#define MIU_DRAM_LEN                0x0008000000
#define MIU_DRAM_LEN0               0x0008000000

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
#define MIU0_LAYER_COUNT            (3)
#define MIU1_LAYER_COUNT            (0)

//MIU_0_START
/* KERNEL_POOL   */
//co_buffer L0
#define KERNEL_POOL_AVAILABLE                                  0x0000000000
#define KERNEL_POOL_ADR                                        0x0000000000  //Alignment 0
#define KERNEL_POOL_GAP_CHK                                    0x0000000000
#define KERNEL_POOL_LEN                                        0x0001100000
#define KERNEL_POOL_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CACHED_POOL_BUF   */
//co_buffer L0
#define CACHED_POOL_BUF_AVAILABLE                              0x0001100000
#define CACHED_POOL_BUF_ADR                                    0x0001100000  //Alignment 0
#define CACHED_POOL_BUF_GAP_CHK                                0x0000000000
#define CACHED_POOL_BUF_LEN                                    0x0000100000
#define CACHED_POOL_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* PM51_BUF   */
//co_buffer L0
#define PM51_BUF_AVAILABLE                                     0x0001200000
#define PM51_BUF_ADR                                           0x0001200000  //Alignment 0x01000
#define PM51_BUF_GAP_CHK                                       0x0000000000
#define PM51_BUF_LEN                                           0x0000019400
#define PM51_BUF_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* NON_CACHED_POOL_BUF   */
//co_buffer L0
#define NON_CACHED_POOL_BUF_AVAILABLE                          0x0001219400
#define NON_CACHED_POOL_BUF_ADR                                0x0001219400  //Alignment 0
#define NON_CACHED_POOL_BUF_GAP_CHK                            0x0000000000
#define NON_CACHED_POOL_BUF_LEN                                0x000189D000
#define NON_CACHED_POOL_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* HW_BUF0_START_TAG   */
//co_buffer L0
#define HW_BUF0_START_TAG_AVAILABLE                            0x0002AB6400
#define HW_BUF0_START_TAG_ADR                                  0x0002AB6400  //Alignment 0
#define HW_BUF0_START_TAG_GAP_CHK                              0x0000000000
#define HW_BUF0_START_TAG_LEN                                  0x0000000000
#define HW_BUF0_START_TAG_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SECTION_BUF   */
//co_buffer L0
#define SECTION_BUF_AVAILABLE                                  0x0002AB6400
#define SECTION_BUF_ADR                                        0x0002AB6400  //Alignment 0x00010
#define SECTION_BUF_GAP_CHK                                    0x0000000000
#define SECTION_BUF_LEN                                        0x0000100000
#define SECTION_BUF_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SDRAM_ADR_BMP   */
//co_buffer L0
#define SDRAM_ADR_BMP_AVAILABLE                                0x0002BB6400
#define SDRAM_ADR_BMP_ADR                                      0x0002BB6400  //Alignment 0
#define SDRAM_ADR_BMP_GAP_CHK                                  0x0000000000
#define SDRAM_ADR_BMP_LEN                                      0x0000200000
#define SDRAM_ADR_BMP_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SDRAM_ADR_FONT   */
//co_buffer L0
#define SDRAM_ADR_FONT_AVAILABLE                               0x0002DB6400
#define SDRAM_ADR_FONT_ADR                                     0x0002DB6400  //Alignment 0
#define SDRAM_ADR_FONT_GAP_CHK                                 0x0000000000
#define SDRAM_ADR_FONT_LEN                                     0x0000050000
#define SDRAM_ADR_FONT_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SC0_MENULOAD_BUF   */
//co_buffer L0
#define SC0_MENULOAD_BUF_AVAILABLE                             0x0002E06400
#define SC0_MENULOAD_BUF_ADR                                   0x0002E06400  //Alignment 0x00010
#define SC0_MENULOAD_BUF_GAP_CHK                               0x0000000000
#define SC0_MENULOAD_BUF_LEN                                   0x0000004000
#define SC0_MENULOAD_BUF_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* TSP_VQ_BUF   */
//co_buffer L0
#define TSP_VQ_BUF_AVAILABLE                                   0x0002E0A400
#define TSP_VQ_BUF_ADR                                         0x0002E0A400  //Alignment 0
#define TSP_VQ_BUF_GAP_CHK                                     0x0000000000
#define TSP_VQ_BUF_LEN                                         0x0000001000
#define TSP_VQ_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VE_FRAME_BUF   */
//co_buffer L0
#define VE_FRAME_BUF_AVAILABLE                                 0x0002E0B400
#define VE_FRAME_BUF_ADR                                       0x0002E0B400  //Alignment 0x00008
#define VE_FRAME_BUF_GAP_CHK                                   0x0000000000
#define VE_FRAME_BUF_LEN                                       0x000021C420
#define VE_FRAME_BUF_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* GOP_REG_DMA_BASE   */
//co_buffer L0
#define GOP_REG_DMA_BASE_AVAILABLE                             0x0003027820
#define GOP_REG_DMA_BASE_ADR                                   0x0003028000  //Alignment 0x04000
#define GOP_REG_DMA_BASE_GAP_CHK                               0x00000007E0
#define GOP_REG_DMA_BASE_LEN                                   0x0000001000
#define GOP_REG_DMA_BASE_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* GOP_GWIN_RB   */
//co_buffer L0
#define GOP_GWIN_RB_AVAILABLE                                  0x0003029000
#define GOP_GWIN_RB_ADR                                        0x0003029000  //Alignment 0x00010
#define GOP_GWIN_RB_GAP_CHK                                    0x0000000000
#define GOP_GWIN_RB_LEN                                        0x0000830000
#define GOP_GWIN_RB_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* GOP_VIDEO_MEM   */
//co_buffer L0
#define GOP_VIDEO_MEM_AVAILABLE                                0x0003859000
#define GOP_VIDEO_MEM_ADR                                      0x000385A000  //Alignment 0x02000
#define GOP_VIDEO_MEM_GAP_CHK                                  0x0000001000
#define GOP_VIDEO_MEM_LEN                                      0x0000400000
#define GOP_VIDEO_MEM_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* TSP_FW_BUF   */
//co_buffer L0
#define TSP_FW_BUF_AVAILABLE                                   0x0003C5A000
#define TSP_FW_BUF_ADR                                         0x0003C5A000  //Alignment 0x00100
#define TSP_FW_BUF_GAP_CHK                                     0x0000000000
#define TSP_FW_BUF_LEN                                         0x0000004000
#define TSP_FW_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF_START   */
//co_buffer L0
#define MIU0_COBUF_START_AVAILABLE                             0x0003C5E000
#define MIU0_COBUF_START_ADR                                   0x0003C5E000  //Alignment 0
#define MIU0_COBUF_START_GAP_CHK                               0x0000000000
#define MIU0_COBUF_START_LEN                                   0x0001271880
#define MIU0_COBUF_START_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDPLAYER_TAG   */
//co_buffer L1
#define VDPLAYER_TAG_AVAILABLE                                 0x0003C5E000
#define VDPLAYER_TAG_ADR                                       0x0003C5E000  //Alignment 0
#define VDPLAYER_TAG_GAP_CHK                                   0x0000000000
#define VDPLAYER_TAG_LEN                                       0x0000000000
#define VDPLAYER_TAG_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* VDPLAYER_DATA   */
//co_buffer L1
#define VDPLAYER_DATA_AVAILABLE                                0x0003C5E000
#define VDPLAYER_DATA_ADR                                      0x0003C5E000  //Alignment 0
#define VDPLAYER_DATA_GAP_CHK                                  0x0000000000
#define VDPLAYER_DATA_LEN                                      0x0000291000
#define VDPLAYER_DATA_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* VDPLAYER_SUB_BS   */
//co_buffer L1
#define VDPLAYER_SUB_BS_AVAILABLE                              0x0003EEF000
#define VDPLAYER_SUB_BS_ADR                                    0x0003EEF000  //Alignment 0
#define VDPLAYER_SUB_BS_GAP_CHK                                0x0000000000
#define VDPLAYER_SUB_BS_LEN                                    0x0000150000
#define VDPLAYER_SUB_BS_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* VDPLAYER_BS   */
//co_buffer L1
#define VDPLAYER_BS_AVAILABLE                                  0x000403F000
#define VDPLAYER_BS_ADR                                        0x000403F000  //Alignment 0x01000
#define VDPLAYER_BS_GAP_CHK                                    0x0000000000
#define VDPLAYER_BS_LEN                                        0x0000400000
#define VDPLAYER_BS_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* VDPLAYER_SUB   */
//co_buffer L1
#define VDPLAYER_SUB_AVAILABLE                                 0x000443F000
#define VDPLAYER_SUB_ADR                                       0x000443F000  //Alignment 0x01000
#define VDPLAYER_SUB_GAP_CHK                                   0x0000000000
#define VDPLAYER_SUB_LEN                                       0x0000040000
#define VDPLAYER_SUB_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* VDPLAYER_DMX_SEC   */
//co_buffer L1
#define VDPLAYER_DMX_SEC_AVAILABLE                             0x000447F000
#define VDPLAYER_DMX_SEC_ADR                                   0x000447F000  //Alignment 0
#define VDPLAYER_DMX_SEC_GAP_CHK                               0x0000000000
#define VDPLAYER_DMX_SEC_LEN                                   0x0000004000
#define VDPLAYER_DMX_SEC_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* PHY_COPY_DATA   */
//co_buffer L1
#define PHY_COPY_DATA_AVAILABLE                                0x0004483000
#define PHY_COPY_DATA_ADR                                      0x0004483000  //Alignment 0x01000
#define PHY_COPY_DATA_GAP_CHK                                  0x0000000000
#define PHY_COPY_DATA_LEN                                      0x0000020000
#define PHY_COPY_DATA_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF_L1_END_TAG   */
//co_buffer L1
#define MIU0_COBUF_L1_END_TAG_AVAILABLE                        0x00044A3000
#define MIU0_COBUF_L1_END_TAG_ADR                              0x00044A3000  //Alignment 0
#define MIU0_COBUF_L1_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L1_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L1_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* PHOTO_SHARE_MEM   */
//co_buffer L2
#define PHOTO_SHARE_MEM_AVAILABLE                              0x0003C5E000
#define PHOTO_SHARE_MEM_ADR                                    0x0003C5E000  //Alignment 0
#define PHOTO_SHARE_MEM_GAP_CHK                                0x0000000000
#define PHOTO_SHARE_MEM_LEN                                    0x0000001000
#define PHOTO_SHARE_MEM_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* PHOTO_DISPLAY   */
//co_buffer L2
#define PHOTO_DISPLAY_AVAILABLE                                0x0003C5F000
#define PHOTO_DISPLAY_ADR                                      0x0003C5F000  //Alignment 0x00010
#define PHOTO_DISPLAY_GAP_CHK                                  0x0000000000
#define PHOTO_DISPLAY_LEN                                      0x00003FC000
#define PHOTO_DISPLAY_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* JPD_INTER_BUF   */
//co_buffer L2
#define JPD_INTER_BUF_AVAILABLE                                0x000405B000
#define JPD_INTER_BUF_ADR                                      0x000405B000  //Alignment 0x00200
#define JPD_INTER_BUF_GAP_CHK                                  0x0000000000
#define JPD_INTER_BUF_LEN                                      0x0000630000
#define JPD_INTER_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* JPD_READ_BUF   */
//co_buffer L2
#define JPD_READ_BUF_AVAILABLE                                 0x000468B000
#define JPD_READ_BUF_ADR                                       0x000468B000  //Alignment 0x00010
#define JPD_READ_BUF_GAP_CHK                                   0x0000000000
#define JPD_READ_BUF_LEN                                       0x0000050000
#define JPD_READ_BUF_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* JPD_OUT   */
//co_buffer L2
#define JPD_OUT_AVAILABLE                                      0x00046DB000
#define JPD_OUT_ADR                                            0x00046DB000  //Alignment 0x00008
#define JPD_OUT_GAP_CHK                                        0x0000000000
#define JPD_OUT_LEN                                            0x00003F4800
#define JPD_OUT_MEMORY_TYPE                                    (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* TSP_FQ_BUF   */
//co_buffer L2
#define TSP_FQ_BUF_AVAILABLE                                   0x0004ACF800
#define TSP_FQ_BUF_ADR                                         0x0004ACF800  //Alignment 0x00010
#define TSP_FQ_BUF_GAP_CHK                                     0x0000000000
#define TSP_FQ_BUF_LEN                                         0x0000400080
#define TSP_FQ_BUF_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF_L2_END_TAG   */
//co_buffer L2
#define MIU0_COBUF_L2_END_TAG_AVAILABLE                        0x0004ECF880
#define MIU0_COBUF_L2_END_TAG_ADR                              0x0004ECF880  //Alignment 0
#define MIU0_COBUF_L2_END_TAG_GAP_CHK                          0x0000000000
#define MIU0_COBUF_L2_END_TAG_LEN                              0x0000000000
#define MIU0_COBUF_L2_END_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* VDPLAYER_SHARE_MEM   */
//co_buffer L0
#define VDPLAYER_SHARE_MEM_AVAILABLE                           0x0004ECF880
#define VDPLAYER_SHARE_MEM_ADR                                 0x0004ED0000  //Alignment 0x01000
#define VDPLAYER_SHARE_MEM_GAP_CHK                             0x0000000780
#define VDPLAYER_SHARE_MEM_LEN                                 0x0000001000
#define VDPLAYER_SHARE_MEM_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF_END_TAG   */
//co_buffer L0
#define MIU0_COBUF_END_TAG_AVAILABLE                           0x0004ED1000
#define MIU0_COBUF_END_TAG_ADR                                 0x0004ED1000  //Alignment 0
#define MIU0_COBUF_END_TAG_GAP_CHK                             0x0000000000
#define MIU0_COBUF_END_TAG_LEN                                 0x0000000000
#define MIU0_COBUF_END_TAG_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MAD_ADV_BUF   */
//co_buffer L0
#define MAD_ADV_BUF_AVAILABLE                                  0x0004ED1000
#define MAD_ADV_BUF_ADR                                        0x0004ED1000  //Alignment 0x01000
#define MAD_ADV_BUF_GAP_CHK                                    0x0000000000
#define MAD_ADV_BUF_LEN                                        0x0000780000
#define MAD_ADV_BUF_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_MEM_START_TAG   */
//co_buffer L0
#define VDEC_MEM_START_TAG_AVAILABLE                           0x0005651000
#define VDEC_MEM_START_TAG_ADR                                 0x0005651000  //Alignment 0
#define VDEC_MEM_START_TAG_GAP_CHK                             0x0000000000
#define VDEC_MEM_START_TAG_LEN                                 0x0000000000
#define VDEC_MEM_START_TAG_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_AEON   */
//co_buffer L0
#define VDEC_AEON_AVAILABLE                                    0x0005651000
#define VDEC_AEON_ADR                                          0x0005651000  //Alignment 0
#define VDEC_AEON_GAP_CHK                                      0x0000000000
#define VDEC_AEON_LEN                                          0x00001CCCD0
#define VDEC_AEON_MEMORY_TYPE                                  (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_FRAME_BUF   */
//co_buffer L0
#define VDEC_FRAME_BUF_AVAILABLE                               0x000581DCD0
#define VDEC_FRAME_BUF_ADR                                     0x000581DE00  //Alignment 0x00200
#define VDEC_FRAME_BUF_GAP_CHK                                 0x0000000130
#define VDEC_FRAME_BUF_LEN                                     0x00020E2200
#define VDEC_FRAME_BUF_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* VDEC_BIT_STREAM   */
//co_buffer L0
#define VDEC_BIT_STREAM_AVAILABLE                              0x0007900000
#define VDEC_BIT_STREAM_ADR                                    0x0007900000  //Alignment 0x00008
#define VDEC_BIT_STREAM_GAP_CHK                                0x0000000000
#define VDEC_BIT_STREAM_LEN                                    0x0000400000
#define VDEC_BIT_STREAM_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* SC0_MAIN_FB   */
//co_buffer L0
#define SC0_MAIN_FB_AVAILABLE                                  0x0007D00000
#define SC0_MAIN_FB_ADR                                        0x0007D00000  //Alignment 0
#define SC0_MAIN_FB_GAP_CHK                                    0x0000000000
#define SC0_MAIN_FB_LEN                                        0x00002FD000
#define SC0_MAIN_FB_MEMORY_TYPE                                (MIU0 | HW | CO_BUF_L0 | UNCACHED)

/* SC0_SUB_FB   */
//co_buffer L0
#define SC0_SUB_FB_AVAILABLE                                   0x0007FFD000
#define SC0_SUB_FB_ADR                                         0x0007FFD000  //Alignment 0
#define SC0_SUB_FB_GAP_CHK                                     0x0000000000
#define SC0_SUB_FB_LEN                                         0x0000000000
#define SC0_SUB_FB_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* HW_BUF0_END_TAG   */
//co_buffer L0
#define HW_BUF0_END_TAG_AVAILABLE                              0x0007FFD000
#define HW_BUF0_END_TAG_ADR                                    0x0007FFD000  //Alignment 0
#define HW_BUF0_END_TAG_GAP_CHK                                0x0000000000
#define HW_BUF0_END_TAG_LEN                                    0x0000000000
#define HW_BUF0_END_TAG_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

//MIU_1_START
//MIU_END
#define MIU0_END_ADR                                           0x0007FFD000
