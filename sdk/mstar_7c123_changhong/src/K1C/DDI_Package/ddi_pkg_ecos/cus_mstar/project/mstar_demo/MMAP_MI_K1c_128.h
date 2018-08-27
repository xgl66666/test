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
#define MIU_DRAM_LEN                0x0008000000
#define MIU_DRAM_LEN0               0x0008000000

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
#define MIU0_LAYER_COUNT            (4)
#define MIU1_LAYER_COUNT            (0)

//MIU_0_START
/* MI_KERNEL_POOL1 LX1 memory */
//co_buffer L0
#define MI_KERNEL_POOL1_AVAILABLE                              0x0000000000
#define MI_KERNEL_POOL1_ADR                                    0x0000000000  //Alignment 0
#define MI_KERNEL_POOL1_GAP_CHK                                0x0000000000
#define MI_KERNEL_POOL1_LEN                                    0x0000C00000
#define MI_KERNEL_POOL1_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_MM_COPROCESSOR   */
//co_buffer L1
#define MI_MM_COPROCESSOR_AVAILABLE                            0x0000000000
#define MI_MM_COPROCESSOR_ADR                                  0x0000000000  //Alignment 0
#define MI_MM_COPROCESSOR_GAP_CHK                              0x0000000000
#define MI_MM_COPROCESSOR_LEN                                  0x0000200000
#define MI_MM_COPROCESSOR_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_CACHED_POOL Cached pool memory */
//co_buffer L0
#define MI_CACHED_POOL_AVAILABLE                               0x0000C00000
#define MI_CACHED_POOL_ADR                                     0x0000C00000  //Alignment 0
#define MI_CACHED_POOL_GAP_CHK                                 0x0000000000
#define MI_CACHED_POOL_LEN                                     0x0000500000
#define MI_CACHED_POOL_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_NON_CACHED_POOL Non-cache pool memory. */
//co_buffer L0
#define MI_NON_CACHED_POOL_AVAILABLE                           0x0001100000
#define MI_NON_CACHED_POOL_ADR                                 0x0001100000  //Alignment 0
#define MI_NON_CACHED_POOL_GAP_CHK                             0x0000000000
#define MI_NON_CACHED_POOL_LEN                                 0x0001000000
#define MI_NON_CACHED_POOL_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_TSP_FW_BUF   */
//co_buffer L0
#define MI_TSP_FW_BUF_AVAILABLE                                0x0002100000
#define MI_TSP_FW_BUF_ADR                                      0x0002100000  //Alignment 0
#define MI_TSP_FW_BUF_GAP_CHK                                  0x0000000000
#define MI_TSP_FW_BUF_LEN                                      0x0000006000
#define MI_TSP_FW_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_TSP_VQ_BUF   */
//co_buffer L0
#define MI_TSP_VQ_BUF_AVAILABLE                                0x0002106000
#define MI_TSP_VQ_BUF_ADR                                      0x0002106000  //Alignment 0
#define MI_TSP_VQ_BUF_GAP_CHK                                  0x0000000000
#define MI_TSP_VQ_BUF_LEN                                      0x0000001860
#define MI_TSP_VQ_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_TSP_SECT_BUF   */
//co_buffer L0
#define MI_TSP_SECT_BUF_AVAILABLE                              0x0002107860
#define MI_TSP_SECT_BUF_ADR                                    0x0002107860  //Alignment 0
#define MI_TSP_SECT_BUF_GAP_CHK                                0x0000000000
#define MI_TSP_SECT_BUF_LEN                                    0x0000100000
#define MI_TSP_SECT_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PM51_MEM   */
//co_buffer L0
#define MI_PM51_MEM_AVAILABLE                                  0x0002207860
#define MI_PM51_MEM_ADR                                        0x0002208000  //Alignment 0x01000
#define MI_PM51_MEM_GAP_CHK                                    0x00000007A0
#define MI_PM51_MEM_LEN                                        0x0000019400
#define MI_PM51_MEM_MEMORY_TYPE                                (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_GOP_REGDMA_BASE   */
//co_buffer L0
#define MI_GOP_REGDMA_BASE_AVAILABLE                           0x0002221400
#define MI_GOP_REGDMA_BASE_ADR                                 0x0002222000  //Alignment 0x02000
#define MI_GOP_REGDMA_BASE_GAP_CHK                             0x0000000C00
#define MI_GOP_REGDMA_BASE_LEN                                 0x0000001000
#define MI_GOP_REGDMA_BASE_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_GOP_GWIN_RB   */
//co_buffer L0
#define MI_GOP_GWIN_RB_AVAILABLE                               0x0002223000
#define MI_GOP_GWIN_RB_ADR                                     0x0002223000  //Alignment 0x00010
#define MI_GOP_GWIN_RB_GAP_CHK                                 0x0000000000
#define MI_GOP_GWIN_RB_LEN                                     0x0000830000
#define MI_GOP_GWIN_RB_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF0_START   */
//co_buffer L0
#define MIU0_COBUF0_START_AVAILABLE                            0x0002A53000
#define MIU0_COBUF0_START_ADR                                  0x0002A53000  //Alignment 0
#define MIU0_COBUF0_START_GAP_CHK                              0x0000000000
#define MIU0_COBUF0_START_LEN                                  0x0000E80000
#define MIU0_COBUF0_START_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_MM_VDPLAYER_DATA   */
//co_buffer L1
#define MI_MM_VDPLAYER_DATA_AVAILABLE                          0x0002A53000
#define MI_MM_VDPLAYER_DATA_ADR                                0x0002A53000  //Alignment 0
#define MI_MM_VDPLAYER_DATA_GAP_CHK                            0x0000000000
#define MI_MM_VDPLAYER_DATA_LEN                                0x0000291000
#define MI_MM_VDPLAYER_DATA_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_INJECT_BUF   */
//co_buffer L2
#define MI_INJECT_BUF_AVAILABLE                                0x0002A53000
#define MI_INJECT_BUF_ADR                                      0x0002A53000  //Alignment 0x01000
#define MI_INJECT_BUF_GAP_CHK                                  0x0000000000
#define MI_INJECT_BUF_LEN                                      0x0000400000
#define MI_INJECT_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MI_DUMMY_BUF   */
//co_buffer L2
#define MI_DUMMY_BUF_AVAILABLE                                 0x0002E53000
#define MI_DUMMY_BUF_ADR                                       0x0002E53000  //Alignment 0
#define MI_DUMMY_BUF_GAP_CHK                                   0x0000000000
#define MI_DUMMY_BUF_LEN                                       0x0000100000
#define MI_DUMMY_BUF_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MI_STILL_IMG_ZAPPING_BUF   */
//co_buffer L2
#define MI_STILL_IMG_ZAPPING_BUF_AVAILABLE                     0x0002F53000
#define MI_STILL_IMG_ZAPPING_BUF_ADR                           0x0002F53000  //Alignment 0
#define MI_STILL_IMG_ZAPPING_BUF_GAP_CHK                       0x0000000000
#define MI_STILL_IMG_ZAPPING_BUF_LEN                           0x00003F4800
#define MI_STILL_IMG_ZAPPING_BUF_MEMORY_TYPE                   (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MI_TSP_FQ_BUF   */
//co_buffer L2
#define MI_TSP_FQ_BUF_AVAILABLE                                0x0003347800
#define MI_TSP_FQ_BUF_ADR                                      0x0003347800  //Alignment 0
#define MI_TSP_FQ_BUF_GAP_CHK                                  0x0000000000
#define MI_TSP_FQ_BUF_LEN                                      0x0000400000
#define MI_TSP_FQ_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF0_L2_END_TAG   */
//co_buffer L2
#define MIU0_COBUF0_L2_END_TAG_AVAILABLE                       0x0003747800
#define MIU0_COBUF0_L2_END_TAG_ADR                             0x0003747800  //Alignment 0
#define MIU0_COBUF0_L2_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF0_L2_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF0_L2_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MI_MM_VDPLAYER_BS   */
//co_buffer L1
#define MI_MM_VDPLAYER_BS_AVAILABLE                            0x0002CE4000
#define MI_MM_VDPLAYER_BS_ADR                                  0x0002CE4000  //Alignment 0
#define MI_MM_VDPLAYER_BS_GAP_CHK                              0x0000000000
#define MI_MM_VDPLAYER_BS_LEN                                  0x0000400000
#define MI_MM_VDPLAYER_BS_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_MM_VDPLAYER_SUB_BS   */
//co_buffer L1
#define MI_MM_VDPLAYER_SUB_BS_AVAILABLE                        0x00030E4000
#define MI_MM_VDPLAYER_SUB_BS_ADR                              0x00030E4000  //Alignment 0
#define MI_MM_VDPLAYER_SUB_BS_GAP_CHK                          0x0000000000
#define MI_MM_VDPLAYER_SUB_BS_LEN                              0x0000150000
#define MI_MM_VDPLAYER_SUB_BS_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_MM_VDPLAYER_EXT_BS   */
//co_buffer L1
#define MI_MM_VDPLAYER_EXT_BS_AVAILABLE                        0x0003234000
#define MI_MM_VDPLAYER_EXT_BS_ADR                              0x0003234000  //Alignment 0x01000
#define MI_MM_VDPLAYER_EXT_BS_GAP_CHK                          0x0000000000
#define MI_MM_VDPLAYER_EXT_BS_LEN                              0x0000500000
#define MI_MM_VDPLAYER_EXT_BS_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_MM_VDPLAYER_SUB   */
//co_buffer L1
#define MI_MM_VDPLAYER_SUB_AVAILABLE                           0x0003734000
#define MI_MM_VDPLAYER_SUB_ADR                                 0x0003734000  //Alignment 0x01000
#define MI_MM_VDPLAYER_SUB_GAP_CHK                             0x0000000000
#define MI_MM_VDPLAYER_SUB_LEN                                 0x0000040000
#define MI_MM_VDPLAYER_SUB_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_MM_VDPLAYER_DMX_SECT   */
//co_buffer L1
#define MI_MM_VDPLAYER_DMX_SECT_AVAILABLE                      0x0003774000
#define MI_MM_VDPLAYER_DMX_SECT_ADR                            0x0003774000  //Alignment 0
#define MI_MM_VDPLAYER_DMX_SECT_GAP_CHK                        0x0000000000
#define MI_MM_VDPLAYER_DMX_SECT_LEN                            0x0000004000
#define MI_MM_VDPLAYER_DMX_SECT_MEMORY_TYPE                    (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF0_L1_END_TAG   */
//co_buffer L1
#define MIU0_COBUF0_L1_END_TAG_AVAILABLE                       0x0003778000
#define MIU0_COBUF0_L1_END_TAG_ADR                             0x0003778000  //Alignment 0
#define MIU0_COBUF0_L1_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF0_L1_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF0_L1_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_JPD_READ_BUF   */
//co_buffer L3
#define MI_JPD_READ_BUF_AVAILABLE                              0x0002A53000
#define MI_JPD_READ_BUF_ADR                                    0x0002A53000  //Alignment 0x00010
#define MI_JPD_READ_BUF_GAP_CHK                                0x0000000000
#define MI_JPD_READ_BUF_LEN                                    0x0000050000
#define MI_JPD_READ_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MI_JPD_INTER_BUF   */
//co_buffer L3
#define MI_JPD_INTER_BUF_AVAILABLE                             0x0002AA3000
#define MI_JPD_INTER_BUF_ADR                                   0x0002AA3000  //Alignment 0x00200
#define MI_JPD_INTER_BUF_GAP_CHK                               0x0000000000
#define MI_JPD_INTER_BUF_LEN                                   0x0000630000
#define MI_JPD_INTER_BUF_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MI_JPD_OUT   */
//co_buffer L3
#define MI_JPD_OUT_AVAILABLE                                   0x00030D3000
#define MI_JPD_OUT_ADR                                         0x00030D3000  //Alignment 0x00008
#define MI_JPD_OUT_GAP_CHK                                     0x0000000000
#define MI_JPD_OUT_LEN                                         0x00003FC000
#define MI_JPD_OUT_MEMORY_TYPE                                 (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MI_PHOTO_SHARE_MEM   */
//co_buffer L3
#define MI_PHOTO_SHARE_MEM_AVAILABLE                           0x00034CF000
#define MI_PHOTO_SHARE_MEM_ADR                                 0x00034CF000  //Alignment 0
#define MI_PHOTO_SHARE_MEM_GAP_CHK                             0x0000000000
#define MI_PHOTO_SHARE_MEM_LEN                                 0x0000001000
#define MI_PHOTO_SHARE_MEM_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MI_PHOTO_DISPLAY   */
//co_buffer L3
#define MI_PHOTO_DISPLAY_AVAILABLE                             0x00034D0000
#define MI_PHOTO_DISPLAY_ADR                                   0x00034D0000  //Alignment 0x00008
#define MI_PHOTO_DISPLAY_GAP_CHK                               0x0000000000
#define MI_PHOTO_DISPLAY_LEN                                   0x00003FC000
#define MI_PHOTO_DISPLAY_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MIU0_COBUF0_L3_END_TAG   */
//co_buffer L3
#define MIU0_COBUF0_L3_END_TAG_AVAILABLE                       0x00038CC000
#define MIU0_COBUF0_L3_END_TAG_ADR                             0x00038CC000  //Alignment 0
#define MIU0_COBUF0_L3_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF0_L3_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF0_L3_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L3 | UNCACHED)

/* MIU0_COBUF0_END_TAG   */
//co_buffer L0
#define MIU0_COBUF0_END_TAG_AVAILABLE                          0x00038D3000
#define MIU0_COBUF0_END_TAG_ADR                                0x00038D3000  //Alignment 0
#define MIU0_COBUF0_END_TAG_GAP_CHK                            0x0000000000
#define MIU0_COBUF0_END_TAG_LEN                                0x0000000000
#define MIU0_COBUF0_END_TAG_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_SEAMLESS_ZAPPING_BUF   */
//co_buffer L0
#define MI_SEAMLESS_ZAPPING_BUF_AVAILABLE                      0x00038D3000
#define MI_SEAMLESS_ZAPPING_BUF_ADR                            0x00038D3000  //Alignment 0x00020
#define MI_SEAMLESS_ZAPPING_BUF_GAP_CHK                        0x0000000000
#define MI_SEAMLESS_ZAPPING_BUF_LEN                            0x0000000000
#define MI_SEAMLESS_ZAPPING_BUF_MEMORY_TYPE                    (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_VDEC_AEON   */
//co_buffer L0
#define MI_VDEC_AEON_AVAILABLE                                 0x00038D3000
#define MI_VDEC_AEON_ADR                                       0x00038E0000  //Alignment 0x10000
#define MI_VDEC_AEON_GAP_CHK                                   0x000000D000
#define MI_VDEC_AEON_LEN                                       0x00001CCCD0
#define MI_VDEC_AEON_MEMORY_TYPE                               (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_VDEC_FRAME_BUF   */
//co_buffer L0
#define MI_VDEC_FRAME_BUF_AVAILABLE                            0x0003AACCD0
#define MI_VDEC_FRAME_BUF_ADR                                  0x0003AACE00  //Alignment 0x00200
#define MI_VDEC_FRAME_BUF_GAP_CHK                              0x0000000130
#define MI_VDEC_FRAME_BUF_LEN                                  0x0001AFD000
#define MI_VDEC_FRAME_BUF_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF2_START   */
//co_buffer L0
#define MIU0_COBUF2_START_AVAILABLE                            0x00055A9E00
#define MIU0_COBUF2_START_ADR                                  0x00055AA000  //Alignment 0x01000
#define MIU0_COBUF2_START_GAP_CHK                              0x0000000200
#define MIU0_COBUF2_START_LEN                                  0x0000400000
#define MIU0_COBUF2_START_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_MM_VDPLAYER_BS1   */
//co_buffer L2
#define MI_MM_VDPLAYER_BS1_AVAILABLE                           0x00055AA000
#define MI_MM_VDPLAYER_BS1_ADR                                 0x00055AA000  //Alignment 0x01000
#define MI_MM_VDPLAYER_BS1_GAP_CHK                             0x0000000000
#define MI_MM_VDPLAYER_BS1_LEN                                 0x0000000000
#define MI_MM_VDPLAYER_BS1_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MI_VDEC_BITSTREAM   */
//co_buffer L1
#define MI_VDEC_BITSTREAM_AVAILABLE                            0x00055AA000
#define MI_VDEC_BITSTREAM_ADR                                  0x00055AA000  //Alignment 0x00008
#define MI_VDEC_BITSTREAM_GAP_CHK                              0x0000000000
#define MI_VDEC_BITSTREAM_LEN                                  0x0000400000
#define MI_VDEC_BITSTREAM_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF2_L2_END_TAG   */
//co_buffer L2
#define MIU0_COBUF2_L2_END_TAG_AVAILABLE                       0x00055AA000
#define MIU0_COBUF2_L2_END_TAG_ADR                             0x00055AA000  //Alignment 0
#define MIU0_COBUF2_L2_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF2_L2_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF2_L2_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF2_L1_END_TAG   */
//co_buffer L1
#define MIU0_COBUF2_L1_END_TAG_AVAILABLE                       0x00059AA000
#define MIU0_COBUF2_L1_END_TAG_ADR                             0x00059AA000  //Alignment 0
#define MIU0_COBUF2_L1_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF2_L1_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF2_L1_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF2_END_TAG   */
//co_buffer L0
#define MIU0_COBUF2_END_TAG_AVAILABLE                          0x00059AA000
#define MIU0_COBUF2_END_TAG_ADR                                0x00059AA000  //Alignment 0
#define MIU0_COBUF2_END_TAG_GAP_CHK                            0x0000000000
#define MIU0_COBUF2_END_TAG_LEN                                0x0000000000
#define MIU0_COBUF2_END_TAG_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_MENULOAD_BUF   */
//co_buffer L0
#define MI_MENULOAD_BUF_AVAILABLE                              0x00059AA000
#define MI_MENULOAD_BUF_ADR                                    0x00059AA000  //Alignment 0x00010
#define MI_MENULOAD_BUF_GAP_CHK                                0x0000000000
#define MI_MENULOAD_BUF_LEN                                    0x0000004000
#define MI_MENULOAD_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MIU0_COBUF3_START   */
//co_buffer L0
#define MIU0_COBUF3_START_AVAILABLE                            0x00059AE000
#define MIU0_COBUF3_START_ADR                                  0x00059B0000  //Alignment 0x10000
#define MIU0_COBUF3_START_GAP_CHK                              0x0000002000
#define MIU0_COBUF3_START_LEN                                  0x00009F6000
#define MIU0_COBUF3_START_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_XC_MAIN_FRAME_BUF   */
//co_buffer L1
#define MI_XC_MAIN_FRAME_BUF_AVAILABLE                         0x00059B0000
#define MI_XC_MAIN_FRAME_BUF_ADR                               0x00059B0000  //Alignment 0x10000
#define MI_XC_MAIN_FRAME_BUF_GAP_CHK                           0x0000000000
#define MI_XC_MAIN_FRAME_BUF_LEN                               0x00009F6000
#define MI_XC_MAIN_FRAME_BUF_MEMORY_TYPE                       (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MIU0_COBUF3_L1_END_TAG   */
//co_buffer L1
#define MIU0_COBUF3_L1_END_TAG_AVAILABLE                       0x00063A6000
#define MIU0_COBUF3_L1_END_TAG_ADR                             0x00063A6000  //Alignment 0
#define MIU0_COBUF3_L1_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF3_L1_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF3_L1_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* MI_XC_SUB_FRAME_BUF   */
//co_buffer L2
#define MI_XC_SUB_FRAME_BUF_AVAILABLE                          0x00059B0000
#define MI_XC_SUB_FRAME_BUF_ADR                                0x00059B0000  //Alignment 0x10000
#define MI_XC_SUB_FRAME_BUF_GAP_CHK                            0x0000000000
#define MI_XC_SUB_FRAME_BUF_LEN                                0x0000000000
#define MI_XC_SUB_FRAME_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF3_L2_END_TAG   */
//co_buffer L2
#define MIU0_COBUF3_L2_END_TAG_AVAILABLE                       0x00059B0000
#define MIU0_COBUF3_L2_END_TAG_ADR                             0x00059B0000  //Alignment 0
#define MIU0_COBUF3_L2_END_TAG_GAP_CHK                         0x0000000000
#define MIU0_COBUF3_L2_END_TAG_LEN                             0x0000000000
#define MIU0_COBUF3_L2_END_TAG_MEMORY_TYPE                     (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* MIU0_COBUF3_END_TAG   */
//co_buffer L0
#define MIU0_COBUF3_END_TAG_AVAILABLE                          0x00063A6000
#define MIU0_COBUF3_END_TAG_ADR                                0x00063A6000  //Alignment 0
#define MIU0_COBUF3_END_TAG_GAP_CHK                            0x0000000000
#define MIU0_COBUF3_END_TAG_LEN                                0x0000000000
#define MIU0_COBUF3_END_TAG_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_VE_FRAME_BUF   */
//co_buffer L0
#define MI_VE_FRAME_BUF_AVAILABLE                              0x00063A6000
#define MI_VE_FRAME_BUF_ADR                                    0x00063A6000  //Alignment 0x00008
#define MI_VE_FRAME_BUF_GAP_CHK                                0x0000000000
#define MI_VE_FRAME_BUF_LEN                                    0x000021C420
#define MI_VE_FRAME_BUF_MEMORY_TYPE                            (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_MAD_ADV_BUF   */
//co_buffer L0
#define MI_MAD_ADV_BUF_AVAILABLE                               0x00065C2420
#define MI_MAD_ADV_BUF_ADR                                     0x00065C3000  //Alignment 0x01000
#define MI_MAD_ADV_BUF_GAP_CHK                                 0x0000000BE0
#define MI_MAD_ADV_BUF_LEN                                     0x0000780000
#define MI_MAD_ADV_BUF_MEMORY_TYPE                             (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_DOWNLOAD_BUF   */
//co_buffer L0
#define MI_PVR_DOWNLOAD_BUF_AVAILABLE                          0x0006D43000
#define MI_PVR_DOWNLOAD_BUF_ADR                                0x0006D43000  //Alignment 0
#define MI_PVR_DOWNLOAD_BUF_GAP_CHK                            0x0000000000
#define MI_PVR_DOWNLOAD_BUF_LEN                                0x00006C0000
#define MI_PVR_DOWNLOAD_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_UPLOAD_BUF   */
//co_buffer L0
#define MI_PVR_UPLOAD_BUF_AVAILABLE                            0x0007403000
#define MI_PVR_UPLOAD_BUF_ADR                                  0x0007403000  //Alignment 0
#define MI_PVR_UPLOAD_BUF_GAP_CHK                              0x0000000000
#define MI_PVR_UPLOAD_BUF_LEN                                  0x0000120000
#define MI_PVR_UPLOAD_BUF_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_AUDIO_BUF   */
//co_buffer L0
#define MI_PVR_AUDIO_BUF_AVAILABLE                             0x0007523000
#define MI_PVR_AUDIO_BUF_ADR                                   0x0007523000  //Alignment 0
#define MI_PVR_AUDIO_BUF_GAP_CHK                               0x0000000000
#define MI_PVR_AUDIO_BUF_LEN                                   0x0000000000
#define MI_PVR_AUDIO_BUF_MEMORY_TYPE                           (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_MN_VIDEO_BUF   */
//co_buffer L0
#define MI_PVR_MN_VIDEO_BUF_AVAILABLE                          0x0007523000
#define MI_PVR_MN_VIDEO_BUF_ADR                                0x0007523000  //Alignment 0
#define MI_PVR_MN_VIDEO_BUF_GAP_CHK                            0x0000000000
#define MI_PVR_MN_VIDEO_BUF_LEN                                0x0000000000
#define MI_PVR_MN_VIDEO_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_MN_AUDIO_BUF   */
//co_buffer L0
#define MI_PVR_MN_AUDIO_BUF_AVAILABLE                          0x0007523000
#define MI_PVR_MN_AUDIO_BUF_ADR                                0x0007523000  //Alignment 0
#define MI_PVR_MN_AUDIO_BUF_GAP_CHK                            0x0000000000
#define MI_PVR_MN_AUDIO_BUF_LEN                                0x0000000000
#define MI_PVR_MN_AUDIO_BUF_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_TSR_VIDEO_BUF   */
//co_buffer L0
#define MI_PVR_TSR_VIDEO_BUF_AVAILABLE                         0x0007523000
#define MI_PVR_TSR_VIDEO_BUF_ADR                               0x0007523000  //Alignment 0
#define MI_PVR_TSR_VIDEO_BUF_GAP_CHK                           0x0000000000
#define MI_PVR_TSR_VIDEO_BUF_LEN                               0x0000000000
#define MI_PVR_TSR_VIDEO_BUF_MEMORY_TYPE                       (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* MI_PVR_TSR_AUDIO_BUF   */
//co_buffer L0
#define MI_PVR_TSR_AUDIO_BUF_AVAILABLE                         0x0007523000
#define MI_PVR_TSR_AUDIO_BUF_ADR                               0x0007523000  //Alignment 0
#define MI_PVR_TSR_AUDIO_BUF_GAP_CHK                           0x0000000000
#define MI_PVR_TSR_AUDIO_BUF_LEN                               0x0000000000
#define MI_PVR_TSR_AUDIO_BUF_MEMORY_TYPE                       (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_1   */
//co_buffer L0
#define CUSTOM_1_AVAILABLE                                     0x0007523000
#define CUSTOM_1_ADR                                           0x0007523000  //Alignment 0
#define CUSTOM_1_GAP_CHK                                       0x0000000000
#define CUSTOM_1_LEN                                           0x0000093000
#define CUSTOM_1_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_2   */
//co_buffer L0
#define CUSTOM_2_AVAILABLE                                     0x00075B6000
#define CUSTOM_2_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_2_GAP_CHK                                       0x0000000000
#define CUSTOM_2_LEN                                           0x0000000000
#define CUSTOM_2_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_3   */
//co_buffer L0
#define CUSTOM_3_AVAILABLE                                     0x00075B6000
#define CUSTOM_3_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_3_GAP_CHK                                       0x0000000000
#define CUSTOM_3_LEN                                           0x0000000000
#define CUSTOM_3_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_4   */
//co_buffer L0
#define CUSTOM_4_AVAILABLE                                     0x00075B6000
#define CUSTOM_4_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_4_GAP_CHK                                       0x0000000000
#define CUSTOM_4_LEN                                           0x0000000000
#define CUSTOM_4_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_5   */
//co_buffer L0
#define CUSTOM_5_AVAILABLE                                     0x00075B6000
#define CUSTOM_5_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_5_GAP_CHK                                       0x0000000000
#define CUSTOM_5_LEN                                           0x0000000000
#define CUSTOM_5_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_6   */
//co_buffer L0
#define CUSTOM_6_AVAILABLE                                     0x00075B6000
#define CUSTOM_6_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_6_GAP_CHK                                       0x0000000000
#define CUSTOM_6_LEN                                           0x0000000000
#define CUSTOM_6_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_7   */
//co_buffer L0
#define CUSTOM_7_AVAILABLE                                     0x00075B6000
#define CUSTOM_7_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_7_GAP_CHK                                       0x0000000000
#define CUSTOM_7_LEN                                           0x0000000000
#define CUSTOM_7_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_8   */
//co_buffer L0
#define CUSTOM_8_AVAILABLE                                     0x00075B6000
#define CUSTOM_8_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_8_GAP_CHK                                       0x0000000000
#define CUSTOM_8_LEN                                           0x0000000000
#define CUSTOM_8_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_9   */
//co_buffer L0
#define CUSTOM_9_AVAILABLE                                     0x00075B6000
#define CUSTOM_9_ADR                                           0x00075B6000  //Alignment 0
#define CUSTOM_9_GAP_CHK                                       0x0000000000
#define CUSTOM_9_LEN                                           0x0000000000
#define CUSTOM_9_MEMORY_TYPE                                   (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* CUSTOM_10   */
//co_buffer L0
#define CUSTOM_10_AVAILABLE                                    0x00075B6000
#define CUSTOM_10_ADR                                          0x00075B6000  //Alignment 0
#define CUSTOM_10_GAP_CHK                                      0x0000000000
#define CUSTOM_10_LEN                                          0x0000000000
#define CUSTOM_10_MEMORY_TYPE                                  (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* DEMOD_COBUF_START_TAG Non-cache pool memory. */
//co_buffer L0
#define DEMOD_COBUF_START_TAG_AVAILABLE                        0x00075B6000
#define DEMOD_COBUF_START_TAG_ADR                              0x00075B6000  //Alignment 0
#define DEMOD_COBUF_START_TAG_GAP_CHK                          0x0000000000
#define DEMOD_COBUF_START_TAG_LEN                              0x0000500000
#define DEMOD_COBUF_START_TAG_MEMORY_TYPE                      (MIU0 | SW | CO_BUF_L0 | UNCACHED)

/* ISDBT_TDI_BUF Non-cache pool memory. */
//co_buffer L1
#define ISDBT_TDI_BUF_AVAILABLE                                0x00075B6000
#define ISDBT_TDI_BUF_ADR                                      0x00075B6000  //Alignment 0
#define ISDBT_TDI_BUF_GAP_CHK                                  0x0000000000
#define ISDBT_TDI_BUF_LEN                                      0x0000500000
#define ISDBT_TDI_BUF_MEMORY_TYPE                              (MIU0 | SW | CO_BUF_L1 | UNCACHED)

/* INTERNAL_DVBT2_TDI Non-cache pool memory. */
//co_buffer L2
#define INTERNAL_DVBT2_TDI_AVAILABLE                           0x00075B6000
#define INTERNAL_DVBT2_TDI_ADR                                 0x00075C0000  //Alignment 0x10000
#define INTERNAL_DVBT2_TDI_GAP_CHK                             0x000000A000
#define INTERNAL_DVBT2_TDI_LEN                                 0x0000320000
#define INTERNAL_DVBT2_TDI_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* INTERNAL_DVBT2_DUMMY   */
//co_buffer L2
#define INTERNAL_DVBT2_DUMMY_AVAILABLE                         0x00078E0000
#define INTERNAL_DVBT2_DUMMY_ADR                               0x00078E0000  //Alignment 0
#define INTERNAL_DVBT2_DUMMY_GAP_CHK                           0x0000000000
#define INTERNAL_DVBT2_DUMMY_LEN                               0x0000005000
#define INTERNAL_DVBT2_DUMMY_MEMORY_TYPE                       (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* INTERNAL_DVBT2_FW   */
//co_buffer L2
#define INTERNAL_DVBT2_FW_AVAILABLE                            0x00078E5000
#define INTERNAL_DVBT2_FW_ADR                                  0x00078E5000  //Alignment 0
#define INTERNAL_DVBT2_FW_GAP_CHK                              0x0000000000
#define INTERNAL_DVBT2_FW_LEN                                  0x0000010000
#define INTERNAL_DVBT2_FW_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* INTERNAL_DVBT2_EQ   */
//co_buffer L2
#define INTERNAL_DVBT2_EQ_AVAILABLE                            0x00078F5000
#define INTERNAL_DVBT2_EQ_ADR                                  0x00078F5000  //Alignment 0
#define INTERNAL_DVBT2_EQ_GAP_CHK                              0x0000000000
#define INTERNAL_DVBT2_EQ_LEN                                  0x0000096000
#define INTERNAL_DVBT2_EQ_MEMORY_TYPE                          (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* INTERNAL_DVBT2_DJB   */
//co_buffer L2
#define INTERNAL_DVBT2_DJB_AVAILABLE                           0x000798B000
#define INTERNAL_DVBT2_DJB_ADR                                 0x000798B000  //Alignment 0
#define INTERNAL_DVBT2_DJB_GAP_CHK                             0x0000000000
#define INTERNAL_DVBT2_DJB_LEN                                 0x0000096000
#define INTERNAL_DVBT2_DJB_MEMORY_TYPE                         (MIU0 | SW | CO_BUF_L2 | UNCACHED)

/* DEMOD_COBUF_END_TAG   */
//co_buffer L0
#define DEMOD_COBUF_END_TAG_AVAILABLE                          0x0007AB6000
#define DEMOD_COBUF_END_TAG_ADR                                0x0007AB6000  //Alignment 0
#define DEMOD_COBUF_END_TAG_GAP_CHK                            0x0000000000
#define DEMOD_COBUF_END_TAG_LEN                                0x0000000000
#define DEMOD_COBUF_END_TAG_MEMORY_TYPE                        (MIU0 | SW | CO_BUF_L0 | UNCACHED)

//MIU_1_START
//MIU_END
#define MIU0_END_ADR                                           0x0007AB6000
