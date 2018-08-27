//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#if (DEMO_DOLBYHDR_PREIDK_TEST == 1)

#include <string.h>
#include <sys/stat.h>
#include <math.h>

#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"
#include "MsFS.h"

#include "apiVDEC_EX.h"
#include "apiHDMITx.h"
#include "apiGOP.h"
#include "apiGFX.h"

#include "drvMVOP.h"
#include "drvDTC.h"

#if (DEMO_XC_DUALXC_TEST == 1)
#include "msAPI_XC_EX.h"
#include "apiPNL_EX.h"
#else
#include "msAPI_XC.h"
#include "apiPNL.h"
#endif

#include "mmsdk_interface_def.h"
#include "demo_mm.h"
#include "demo_xc.h"
#include "demo_osd.h"
#include "demo_dolbyhdr_preidk.h"
#include "demo_utility.h"

#if defined(MSOS_TYPE_LINUX)
#include "drvMMIO.h"
#endif

#if defined(MSOS_TYPE_LINUX)
#define REG16_NPM(addr)           *((volatile MS_U16*)(virt32NonPmBase+ (addr)))
#define REG16_PM(addr )           *((volatile MS_U16*)(virt32PmBase+ (addr)))
#define REG8_NPM(addr)            *((volatile MS_U8*)(virt32NonPmBase+ (addr)))
#define REG8_PM(addr )            *((volatile MS_U8*)(virt32PmBase+ (addr)))
#endif

#define VDEC_MIU        (VDEC_FRAME_BUF_MEMORY_TYPE & BIT(0))
#define VDEC_FB_ADDR    MEM_ADR_BY_MIU(VDEC_FRAME_BUF_ADR, VDEC_FRAME_BUF_MEMORY_TYPE)
#define VDEC_FB_SIZE    VDEC_FRAME_BUF_LEN
#define GOP_FB_ADDR     MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE)
#define GOP_FB_SIZE     GOP_GWIN_RB_LEN
#define ADC_DMA_ADDR    MEM_ADR_BY_MIU(ADC_DMA_ADR, ADC_DMA_MEMORY_TYPE)
#define ADC_DMA_SIZE    ADC_DMA_LEN

#define BL_8B_Y_FILE    "_y_8b_bl.bin"
#define BL_8B_C_FILE    "_c_8b_bl.bin"
#define BL_2B_Y_FILE    "_y_2b_bl.bin"
#define BL_2B_C_FILE    "_c_2b_bl.bin"
#define EL_8B_Y_FILE    "_y_8b_el.bin"
#define EL_8B_C_FILE    "_c_8b_el.bin"
#define EL_2B_Y_FILE    "_y_2b_el.bin"
#define EL_2B_C_FILE    "_c_2b_el.bin"

#define WORD_ALIGN                8
#define FILE_NAME_LENGTH          1024
#define EXTENTION_NAME_LENGTH     10

#define MAX_FRAME_INDEX           999
#define DMA_TRIGGER_RESET_TIMEOUT 1000
#define DMA_TRIGGER_DUMP_TIMEOUT  (3*60*1000)

#define ADC_DUMP_FE_SD_SIZE       (50*1024*1024)
#define ADC_DUMP_FE_HD_SIZE       (150*1024*1024)
#define ADC_DUMP_FE_FHD_SIZE      (300*1024*1024)
#define ADC_DUMP_FE_4K_SIZE       (1200*1024*1024)
#define ADC_DUMP_BE_SD_SIZE       (4*1024*1024)
#define ADC_DUMP_BE_HD_SIZE       (12*1024*1024)
#define ADC_DUMP_BE_FHD_SIZE      (24*1024*1024)
#define ADC_DUMP_BE_4K_SIZE       (96*1024*1024)

#define WORD_HADDR(addr)          (MS_U16)((addr / 8) >> 16)
#define WORD_LADDR(addr)          (MS_U16)((addr / 8) & 0xffff)
#define ADC_HADDR(addr)           (MS_U16)((addr / 0x20) >> 16)
#define ADC_LADDR(addr)           (MS_U16)((addr / 0x20) & 0xffff)

#define RIU_BANK(val)             (MS_U16)(val >> 8)
#define RIU_OFFSET(val)           (MS_U16)(val & 0xff)
#define MEM_ALIGN(unit, val)      (1 + (((val) - 1) | (unit - 1)))


#define CHECK_END_SLASH(str)                 \
    if(str[strlen((char*)str) -1] != '/')    \
    {                                        \
        strncat((char*)str, "/", 1);         \
    }

#define LOAD_NEXT_2DRAM(filePath, addr, align, size, endAddr)                                                           \
    size = _Demo_DolbyHDR_PreIDK_LoadBin2Dram((MS_U8*)filePath, &addr);                                                 \
    if(0 != size)                                                                                                       \
    {                                                                                                                   \
        addr += MEM_ALIGN(align, size);                                                                                 \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        printf("[Err(%s : %d)] Load %s to Dram fail! \n", __FUNCTION__, __LINE__, (char*)filePath);                     \
        return FALSE;                                                                                                   \
    }                                                                                                                   \
    if(addr > endAddr)                                                                                                  \
    {                                                                                                                   \
        printf("[Err(%s : %d)] Load %s Size is over the buffer! \n", __FUNCTION__, __LINE__, (char*)filePath);          \
        return FALSE;                                                                                                   \
    }

#define LOAD_DUMP_ADDR(addr, align, size, endAddr)                                                                      \
    if(0 != size)                                                                                                       \
    {                                                                                                                   \
        addr += MEM_ALIGN(align, size);                                                                                 \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        printf("[Err(%s : %d)] dump size is zero ! \n", __FUNCTION__, __LINE__);                                        \
        return FALSE;                                                                                                   \
    }                                                                                                                   \
    if(addr > endAddr)                                                                                                  \
    {                                                                                                                   \
        printf("[Err(%s : %d)] dump size is over the total buffer size ! \n", __FUNCTION__, __LINE__);                  \
        return FALSE;                                                                                                   \
    }

#define ADC_DMA_ENABLE 1
#define GOP_ENABLE 1

typedef struct
{
    MS_U16  u16Width;
    MS_U16  u16Height;
} _ST_DOLBYHDR_PREIDK_RECTSIZE;

typedef struct
{
    MS_U8 pu8BLPath[FILE_NAME_LENGTH];
    MS_U8 pu8ELPath[FILE_NAME_LENGTH];
    _ST_DOLBYHDR_PREIDK_RECTSIZE  stBL_Size;
    _ST_DOLBYHDR_PREIDK_RECTSIZE  stEL_Size;
} _ST_DOLBYHDR_PREIDK_FRMLAYER_INFO;

typedef enum
{
    E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_8BIT = 8,
    E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_10BIT = 10,
    E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_12BIT = 12,
} EN_DOLBYHDR_PREIDK_OUTPUT_DEPTH_TYPE;

EN_DOLBYHDR_PREIDK_OUTPUT_DEPTH_TYPE enOutDepth;

static MS_U32 _gu32BL8Bit_YAddr = 0;
static MS_U32 _gu32BL8Bit_CAddr = 0;
static MS_U32 _gu32BL2Bit_YAddr = 0;
static MS_U32 _gu32BL2Bit_CAddr = 0;
static MS_U32 _gu32EL8Bit_YAddr = 0;
static MS_U32 _gu32EL8Bit_CAddr = 0;
static MS_U32 _gu32EL2Bit_YAddr = 0;
static MS_U32 _gu32EL2Bit_CAddr = 0;

static MS_BOOL _gbEL_Enable = TRUE;
static MS_BOOL _gbIs10Bit_Input = TRUE;
static MS_BOOL _gbIsRGB_Output = FALSE;
static MS_BOOL _gbADL_Enable = FALSE;
static MS_BOOL _gbADL_Vsync_Attach = FALSE;
static MS_U8* _gpu8ScrambleArray = NULL;
static MS_U32 _gu32ScrambleSize = 0;
static MS_S32 _gs32ScrambleNonCachedPoolID = 0;

static _ST_DOLBYHDR_PREIDK_FRMLAYER_INFO stFrmLayerInfo = {};
static char strLaunchPath[FILE_NAME_LENGTH] = {'\0'};
static char strFrmInputPath[FILE_NAME_LENGTH] = {'\0'};
static char strFrmOutputPath[FILE_NAME_LENGTH] = {'\0'};
static char strFrmScpPath[FILE_NAME_LENGTH] = {'\0'};
static char strFrmOSDPath[FILE_NAME_LENGTH] = {'\0'};
static MS_U32 _gu32DumpSize = 0;

#if (DEMO_XC_DUALXC_TEST == 1)
MSAPI_XC_DEVICE_ID _gstXC_DeviceId[2] = {{0, E_MSAPI_XC_DEVICE0}, {0, E_MSAPI_XC_DEVICE1}};
#define stPNL_DeviceId (PNL_DeviceId){_gstXC_DeviceId[0].u32Version, _gstXC_DeviceId[0].u32Id}
#define IPANEL(dev, func) g_IPanelEx.func(dev)
#else
MSAPI_XC_DEVICE_ID _gstXC_DeviceId = {0, E_MSAPI_XC_DEVICE0};
#define stPNL_DeviceId (PNL_DeviceId){_gstXC_DeviceId.u32Version, _gstXC_DeviceId.u32Id}
#define IPANEL(dev, func) g_IPanel.func()
#endif

////////////////////////for DMA parser////<<<It's offered by Algo. Team>>>//////////////////////////////////////////////////////////////
/* debug flag */
#define DEBUG 0


/* constants */
#define DP_SUCCESS  1
#define DP_FAIL     2

#define IMAGE_SIZE(w, h) (w*h)
#define HALF_IMAGE_SIZE(w, h) (w*h/2)
#define MAX_BUFFER_SIZE(w, h) (w*h+w)

#define FMT_444 0
#define FMT_422 1
#define FMT_420 2


#if 0
#define SRC_FMT FMT_444
BDP=12;

#ifndef PRELOAD_CONFIG
#define BDP 12
#define SCRAMBLE_IN 1
#define ODD_PASS 0
#define SRC_FMT FMT_422
#endif
#endif
/* needed by library */
int WIDTH;
int HEIGHT;
int BDP;
int SCRAMBLE_IN;
int ODD_PASS;
int SRC_FMT;

#define MASK_VSYNC  0x80
#define MASK_LAST   0x40
#define MASK_VALID  0x20


/* op defines */
#define DP_COMBINE  0
#define DP_ORIGIN   1


typedef struct _pixel{
    MS_U8 nl;
    MS_U16 y;
    MS_U16 u;
    MS_U16 v;
#if DEBUG
    MS_U32 pos;
    MS_U64 content;
#endif
}pixel;
////////////////////////for DMA parser////<<<It's offered by Algo. Team>>>//////////////////////////////////////////////////////////////

#if DEBUG
    FILE* debug_out;
    FILE* debug_garbage;
    FILE* debug_vsync;
    FILE* debug_last;
    FILE* debug_text;

    void PrintBits(MS_U8 byte, FILE* hdl)
    {
        int i;
        for (i=7; i>=0; --i)
        {
            fprintf(hdl, "%d", ( ((byte>>i)&0x1) > 0));
        }
        fprintf(hdl, " ");
    }
#endif



/*
*   Trim unaligned pixels.
*
*   @buffer_to_trim:    pointer to the buffer to be trimmed.
*   @pixel_count:       number of pixels in buffer_to_trim.
*   @pixel_per_frame:   expected number of pixels per frame
*   @frame_buffer:      pointer to the result buffer.
*   @vsync_pos:         position of vsync. this variable will be adjusted to fit the size change of trimmed buffer.
*   @return:            number of trimmed pixels.
*/
static int TrimBuffer(
    const pixel* buffer_to_trim,
    const int pixel_count,
    const int pixel_per_frame,
    int* vsync_pos  ,
    pixel* frame_buffer )
{
    int y_count = -1;
    int copy_start = -1;
    int copy_size = -1;
    int i;
    int prev_last = -1;
/* scan for first line end and last line end */
    for (i=0;i< pixel_count; ++i)
    {
        if ( buffer_to_trim[i].nl == 1)
        {
            if (prev_last >= 0)
            {
                int limit;
                if (SCRAMBLE_IN)
                    limit = WIDTH;
                else
                    limit = WIDTH/2;

                if(i - prev_last != limit)
                {
                    fprintf(stderr, "There are %d points scanned from previous NL.\n", i - prev_last);
#if DEBUG
                    fprintf(stderr, "NL pos unmatched at 0x%08x\n", buffer_to_trim[i].pos);

                    for (j = prev_last; j<= i; ++j)
                    {
                        fprintf(debug_last, "[LAST CONTENT @ offset 0x%x]", buffer_to_trim[j].pos);
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 56) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 48) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 40) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 32) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 24) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 16) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 8) & 0xFF );
                        fprintf(debug_last, " %02x", (buffer_to_trim[j].content >> 0) & 0xFF );
                        fprintf(debug_last, "\n");
                    }
#endif
                    return 0;
                }
            }
            prev_last = i;
            y_count ++;

            if (y_count == 0)
            {
                copy_start = i+1;
            }

            if (y_count == HEIGHT)
            {
                copy_size = i - copy_start + 1;
                break;
            }
        }
    }

    /* not enough for one frame */
    if (copy_size == -1)
    {
        fprintf(stderr, "Only %d<%d lines counted.\n", y_count, HEIGHT);
        return 0;
    }

    /* encounter garbage */

    if (copy_size != pixel_per_frame)
    {
        fprintf(stderr, "Unmatched frame border. Expected: %d   Actual: %d\n", pixel_per_frame, copy_size);
        return 0;
    }

    /* crop buffer */
    memcpy(frame_buffer, &buffer_to_trim[copy_start], copy_size * sizeof(pixel) );

    /* calculate new vsync position*/
    *vsync_pos = (*vsync_pos - copy_start + pixel_per_frame) % pixel_per_frame;

    return copy_size;
}


/*
*   Extracts the pixels information from DRAM dump file.
*
*   @fin:           file pointer to even pixel dump file.
*   @pixel_count:   pointer to number of pixel extracted.
*   @vsync_pos:     pointer to vsync position.
*   @buffer:        buffer to store extracted pixels.
*   @return:        {DP_SUCCESS, DP_FAIL}
*/
int ParseFile(FILE* fin, int* pixel_count, int* vsync_pos, pixel* buffer)
{

    /* struct to facilitate parsing*/
    union{
        struct{
            MS_U8 bytes[7];
            MS_U8 status;
        } splitted;
        MS_U64 flat;
    }transcript;



    MS_U8 byte_read;

    /* statistics */
    MS_U32 total_last = 0;
    MS_U32 total_valid = 0;
    MS_U32 total_vsync = 0;
    MS_U32 total_pixel = 0;
    MS_U32 bad_pixel = 0;
    MS_U8 is_prev_vsync = 0;

    /* initialize pixel count*/
    *pixel_count = 0;

    for (;;)
    {
        /* get pixel status */
        MS_U8 vsync = 0;
        MS_U8 last = 0;
        MS_U8 valid = 0;



        byte_read = MsFS_Fread(&transcript.splitted, 1, 8, fin);
        if ( 0 == byte_read)
            break;
        else if ( 8 > byte_read) /* ditch incomplete data */
        {
            fprintf(stderr, "Data is not 64 bits aligned.\n");
            //return DP_FAIL;
            break;
        }

        total_pixel ++;

        /* format check */
        if ( 0 < ((transcript.flat >> 48) & (0x1FFF)) )
        {
#if DEBUG
            fprintf(debug_garbage, "[BAD CONTENT @ offset 0x%x] %02x %02x %02x %02x %02x %02x %02x %02x\n", total_pixel*8, transcript.splitted.bytes[0], transcript.splitted.bytes[1], transcript.splitted.bytes[2], transcript.splitted.bytes[3], transcript.splitted.bytes[4], transcript.splitted.bytes[5], transcript.splitted.bytes[6], transcript.splitted.status);
#endif
            bad_pixel ++;
            continue;
        }



        vsync = ( 0 < (transcript.splitted.status & MASK_VSYNC) );
        valid = ( 0 < (transcript.splitted.status & MASK_VALID) );
        last = ( 0 < (transcript.splitted.status & MASK_LAST) );

#if DEBUG
        if (vsync)
            fprintf(debug_out, "VSYNC\n");
        if (valid)
        {
            fprintf(debug_out, "VALID #%03d", total_valid);
            if (last)
                fprintf(debug_out, " (last)\n");
            else
                fprintf(debug_out, "\n");
        }
#endif



        if (vsync)
        {
            /* prevent vsync pos overwrite */
            if (*vsync_pos == -1)
                *vsync_pos = total_valid;
#if DEBUG

            fprintf(debug_vsync, "[VSYNC @ offset 0x%x]" , total_pixel*8 );
            PrintBits(transcript.splitted.status, debug_vsync);
            fprintf(debug_vsync, " ");
            for (i=6; i>=0; --i)
            {
                PrintBits(transcript.splitted.bytes[i], debug_vsync);
                fprintf(debug_vsync, " ");
            }
            fprintf(debug_vsync, "\n[VSYNC @ offset 0x%x] %02x %02x %02x %02x %02x %02x %02x %02x\n", total_pixel*8, transcript.splitted.bytes[0], transcript.splitted.bytes[1], transcript.splitted.bytes[2], transcript.splitted.bytes[3], transcript.splitted.bytes[4], transcript.splitted.bytes[5], transcript.splitted.bytes[6], transcript.splitted.status);
#endif
        }


        /* vsync signal should not be regarded as either valid or last */
        last = (!vsync) && last && valid;
        valid = (!vsync) && valid;





        /* statistics */
        total_last += last;
        if (SCRAMBLE_IN)
            total_valid += valid*2;
        else
            total_valid += valid;

        total_vsync += (!is_prev_vsync) && vsync; /* only count positive edge */
        is_prev_vsync = vsync;


        /* handle valid pixel*/
        if (valid)
        {
//            int i;
            if (SCRAMBLE_IN)
            {
                /* get yc */
                MS_U16 y0, y1, c0, c1;
                MS_U32 mask;
                /* extract y/cb/cr components */
                mask = (1<< BDP ) -1;

                c0 = transcript.flat & (mask);
                y0 = (transcript.flat >> (BDP) )  & (mask);
                c1 = (transcript.flat >> (BDP*2) )  & (mask);
                y1 = (transcript.flat >> (BDP*3) )  & (mask);

#if DEBUG
                fprintf(debug_out, "\n[VALID @ offset 0x%x] %02x %02x %02x %02x %02x %02x %02x %02x\n", total_pixel*8, transcript.splitted.bytes[0], transcript.splitted.bytes[1], transcript.splitted.bytes[2], transcript.splitted.bytes[3], transcript.splitted.bytes[4], transcript.splitted.bytes[5], transcript.splitted.bytes[6], transcript.splitted.status);
                PrintBits(transcript.splitted.bytes[0], debug_out);
                PrintBits(transcript.splitted.bytes[1], debug_out);
                PrintBits(transcript.splitted.bytes[2], debug_out);
                PrintBits(transcript.splitted.bytes[3], debug_out);
                PrintBits(transcript.splitted.bytes[4], debug_out);
                PrintBits(transcript.splitted.bytes[5], debug_out);
                PrintBits(transcript.splitted.bytes[6], debug_out);
                PrintBits(transcript.splitted.status, debug_out);

                fprintf(debug_out, "\n[VALID @ offset 0x%x] 0x%04x 0x%04x 0x%04x 0x%04x\n", total_pixel*8, y0, c0, y1, c1);
#endif

                buffer[*pixel_count].y = y0;
                buffer[*pixel_count].u = c0;
                buffer[*pixel_count].v = c1;
                buffer[*pixel_count].nl = 0;
                buffer[*pixel_count+1].y = y1;
                buffer[*pixel_count+1].u = c0;
                buffer[*pixel_count+1].v = c1;
                buffer[*pixel_count+1].nl = last;
                *pixel_count+=2;
                if (*pixel_count >= MAX_BUFFER_SIZE(WIDTH, HEIGHT))
                    break;
            }
            else
            {
                /* get yuv */
                MS_U16 y,u,v;
                MS_U32 mask;
                y=u=v=0;

                /* extract y/cb/cr components */
                mask = (1<< BDP ) -1;
                v = transcript.flat & (mask);
                u = (transcript.flat >> (BDP) )  & (mask);
                y = (transcript.flat >> (BDP<<1) )  & (mask);

                buffer[*pixel_count].y = y;
                buffer[*pixel_count].u = u;
                buffer[*pixel_count].v = v;
                buffer[*pixel_count].nl = last;
#if DEBUG
                buffer[*pixel_count].pos = total_pixel*8;
                memcpy( &buffer[*pixel_count].content, &transcript.flat, 8 );
                fprintf(debug_out, "\n[VALID @ offset 0x%x] %x %x %x\n", total_pixel*8, y, u, v);
#endif
                *pixel_count+=1;
                if (*pixel_count >= MAX_BUFFER_SIZE(WIDTH, HEIGHT))
                    break;
            }
        }
        else if (last)
        {
#if DEBUG
            fprintf(debug_out, "\n[INVALID @ pixel %u] %02x %02x %02x %02x %02x %02x %02x %02x\n", total_valid, transcript.splitted.bytes[0], transcript.splitted.bytes[1], transcript.splitted.bytes[2], transcript.splitted.bytes[3], transcript.splitted.bytes[4], transcript.splitted.bytes[5], transcript.splitted.bytes[6], transcript.splitted.status);
#endif
        }

    }

    printf("Bad pixels: %d\n", bad_pixel);
    /* report & statistics */
    printf("Valid/Good/Total:\t%8u/%8u/%8u (%3.1f%%/%3.1f%%/%3.1f%%)\nLast:\t%u\nVsync:\t%u\n",
        total_valid,
        total_pixel - bad_pixel,
        total_pixel,
        ((double) total_valid / (double) total_pixel) * 100.0f,
        ((double) (total_pixel - bad_pixel) / (double) total_pixel) * 100.0f,
        (double) 100.0f,
        total_last,
        total_vsync);

    /* vsync not found */
    if (*vsync_pos == -1)
    {
        fprintf(stderr, "VSync not found!\n");
        return DP_FAIL;
    }

    printf("VSync Pixel Position:\t%d\n", *vsync_pos);


    return DP_SUCCESS;
}


/*
*   Display the images from both buffers as sub pictures.
*
*   @fin_even:  file pointer to even pixel buffer.
*   @odd_even:  file pointer to odd pixel buffer.
*   @fout:      file pointer to the output file.
*   @bdp:       output file bit depth
*   @operation: how to display even/odd pixel images.
*               -DP_COMBINE means combine two images into one;
*               -DP_ORIGIN means put two images in different columns.)
*   @return:    {DP_SUCCESS, DP_FAIL}
*/
int ParseBothFile(FILE* fin_even, FILE* fin_odd, FILE* fout, int bdp, int operation)
{
    MS_U32 u32LoadAddr = 0;
    pixel *image;
    pixel *even_buffer;
    pixel *odd_buffer;
    pixel* frame_buffer_even;
    pixel* frame_buffer_odd;

    int i;

    /* return status */
    int ret;

    /* for combining buffer */
    int x=0, y=0, pos;
    int frame_buffer_size;

    /* pixel count*/
    int even_pixel_count = 0;
    int odd_pixel_count = 0;

    /* vsync pos */
    int even_vsync = -1;
    int odd_vsync = -1;

    /* report status */
    printf("BDP: %d WIDTH: %d HEIGHT: %d  FORMAT:", BDP, WIDTH, HEIGHT);
    switch (SRC_FMT)
    {
        case FMT_444:
            printf(" 444\n");
            break;
        case FMT_422:
            printf(" 422\n");
            break;
        case FMT_420:
            printf(" 420\n");
            break;
        default:
            printf(" unknown\n");
            break;
    }

    /* init memory */
    u32LoadAddr = (MS_U32)ADC_DMA_ADDR;
    memset((pixel*)MS_PA2KSEG1(u32LoadAddr), 0, ADC_DMA_SIZE);//dump buffer addr

    image = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * IMAGE_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));
    even_buffer = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * MAX_BUFFER_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));
    odd_buffer = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * MAX_BUFFER_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));
    frame_buffer_even = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * MAX_BUFFER_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));
    frame_buffer_odd = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * MAX_BUFFER_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));

#if DEBUG
    debug_out = MsFS_Fopen("debug.txt","w");
    debug_garbage = MsFS_Fopen("debug_garbage.txt", "w");
    debug_vsync = MsFS_Fopen("debug_vsync.txt", "w");
    debug_last = MsFS_Fopen("debug_last.txt","w");
    debug_text = MsFS_Fopen("debug_text.txt","w");
#endif

    /* even pass */
    printf("Parsing even pixels.\n");
    ret = ParseFile(fin_even, &even_pixel_count, &even_vsync, even_buffer);

    if (ret != DP_SUCCESS)
    {
        return ret;
    }

    /* trim buffer */
    frame_buffer_size = TrimBuffer(even_buffer, even_pixel_count, HALF_IMAGE_SIZE(WIDTH, HEIGHT), &even_vsync, frame_buffer_even);
    if (frame_buffer_size == 0)
    {
        return DP_FAIL;
    }
    /* bypass trim */
    //frame_buffer_size = even_pixel_count;
    //memcpy(frame_buffer, even_buffer, sizeof(pixel) * HALF_IMAGE_SIZE(WIDTH, HEIGHT));


    x = 0;
    y = 0;

    for (i=0;i< frame_buffer_size; ++i)
    {
        pos = (i + even_vsync) % frame_buffer_size;

        image[y*WIDTH + x].y = frame_buffer_even[pos].y;
        image[y*WIDTH + x].u = frame_buffer_even[pos].u;
        image[y*WIDTH + x].v = frame_buffer_even[pos].v;

        if(operation == DP_COMBINE)
        {
            x+=2;
        }
        else if (operation == DP_ORIGIN)
        {
            x+=1;
        }
        else
        {
            return DP_FAIL;
        }

        if (frame_buffer_even[pos].nl)
        {
            y+=1;
            x=0;
        }

        if (y >= HEIGHT)
        {
            break;
        }

    }



    if (ODD_PASS)
    {
        printf("Parsing odd pixels.\n");
        /* odd pass */
        ret = ret & ParseFile(fin_odd, &odd_pixel_count, &odd_vsync, odd_buffer);

        if (ret != DP_SUCCESS)
        {
            return ret;
        }

        /* trim buffer */
        frame_buffer_size = TrimBuffer(odd_buffer, odd_pixel_count, HALF_IMAGE_SIZE(WIDTH, HEIGHT), &odd_vsync, frame_buffer_odd);
        if (frame_buffer_size == 0)
        {
            return DP_FAIL;
        }

        /* bypass trim */
        //frame_buffer_size = odd_pixel_count;
        //memcpy(frame_buffer, odd_buffer, sizeof(pixel) * HALF_IMAGE_SIZE(WIDTH, HEIGHT));

        if (operation == DP_COMBINE)
        {
            x=1;
        }
        else if (operation == DP_ORIGIN)
        {
            x=WIDTH/2;
        }
        else
        {
            return DP_FAIL;
        }

        y=0;
        for (i=0;i< HALF_IMAGE_SIZE(WIDTH, HEIGHT); ++i)
        {
            pos = (i + odd_vsync) % frame_buffer_size;
            image[y*WIDTH + x].y = frame_buffer_odd[pos].y;
            image[y*WIDTH + x].u = frame_buffer_odd[pos].u;
            image[y*WIDTH + x].v = frame_buffer_odd[pos].v;

            if (operation == DP_COMBINE)
            {
                x+=2;
                if (frame_buffer_odd[pos].nl)
                {
                    y+=1;
                    x=1;
                }
            }
            else if (operation == DP_ORIGIN)
            {
                x+=1;
                if (frame_buffer_odd[pos].nl)
                {
                    y+=1;
                    x=WIDTH/2;
                }
            }
            else
            {
                return DP_FAIL;
            }

            if (y >= HEIGHT)
            {
                break;
            }

        }
    }

/*
    if(0)
    { // uyvy 422
        FILE* uyvy = MsFS_Fopen("uyvy422.yuv", "wb");

        int i, pos_even=0, pos_odd=0;

        // y
        for (i=0; i< IMAGE_SIZE(WIDTH, HEIGHT) ; ++i)
        {
            if (i%2 == 0)
            {
                pos_even = (i/2 + even_vsync) % HALF_IMAGE_SIZE(WIDTH, HEIGHT);
                MsFS_Fwrite(&frame_buffer_even[pos_even].u, sizeof(MS_U8), 2, uyvy);
                MsFS_Fwrite(&frame_buffer_even[pos_even].y, sizeof(MS_U8), 2, uyvy);
            }
            else
            {
                pos_odd = (i/2 + odd_vsync) % HALF_IMAGE_SIZE(WIDTH, HEIGHT);
                MsFS_Fwrite(&frame_buffer_odd[pos_odd].u, sizeof(MS_U8), 2, uyvy);
                MsFS_Fwrite(&frame_buffer_odd[pos_odd].y, sizeof(MS_U8), 2, uyvy);
            }

        }
        MsFS_Fflush(uyvy);
        MsFS_Fclose(uyvy);
    }

    if(0)
    { // yuv 420
        FILE* yuv420 = MsFS_Fopen("yuv420.yuv", "wb");

        int i,  pos_even=0, pos_odd=0;

        // y
        for (i=0; i< IMAGE_SIZE(WIDTH, HEIGHT) ; ++i)
        {

            if (i%2 == 0)
            {
                pos_even = (i/2 + even_vsync) % HALF_IMAGE_SIZE(WIDTH, HEIGHT);
                MsFS_Fwrite(&frame_buffer_even[pos_even].y, sizeof(MS_U8), 2, yuv420);
            }
            else
            {
                pos_odd = (i/2 + odd_vsync) % HALF_IMAGE_SIZE(WIDTH, HEIGHT);
                MsFS_Fwrite(&frame_buffer_odd[pos_odd].y, sizeof(MS_U8), 2, yuv420);
            }

        }

        // u
        for (i=0; i< IMAGE_SIZE(WIDTH, HEIGHT) ; ++i)
        {
            if ( (i/WIDTH)%2  == 0)
            {
                if (i%2 == 0)
                {
                    pos_even = (i/2 + even_vsync) % HALF_IMAGE_SIZE(WIDTH, HEIGHT);
                    MsFS_Fwrite(&frame_buffer_even[pos_even].u, sizeof(MS_U8), 2, yuv420);
                }
            }
        }


        // v
        for (i=0; i< IMAGE_SIZE(WIDTH, HEIGHT) ; ++i)
        {
            if ( (i/WIDTH)%2  == 0)
            {
                if (i%2 == 1)
                {
                    pos_odd = (i/2 + odd_vsync) % HALF_IMAGE_SIZE(WIDTH, HEIGHT);
                    MsFS_Fwrite(&frame_buffer_odd[pos_odd].u, sizeof(MS_U8), 2, yuv420);
                }
            }
        }
        MsFS_Fflush(yuv420);
        MsFS_Fclose(yuv420);
    }
*/

    if ((SRC_FMT == FMT_420) || (SRC_FMT == FMT_422))
    {//422 or 420 pass
        int i, j;
        for (i=0; i<HEIGHT; ++i)
        {
            for (j=0; j<WIDTH; ++j)
            {
                if (operation == DP_COMBINE)
                {
                    if (j%2 == 0)
                    {
                        image[i*WIDTH + j].v = image[i*WIDTH + j + 1].u;
                    }
                    else
                    {
                        image[i*WIDTH + j].v = image[i*WIDTH + j].u;
                        image[i*WIDTH + j].u = image[i*WIDTH + j - 1].u;
                    }

                }
                else
                {
                    if (j >= WIDTH/2)
                    image[i*WIDTH + j].u = image[i*WIDTH + j].v;
                }

                if (SRC_FMT == FMT_420)
                {
                    if (i%2 == 1)
                    {
                        image[i*WIDTH + j].v = image[ (i-1)*WIDTH + j].v;
                        image[i*WIDTH + j].u = image[ (i-1)*WIDTH + j].u;
                    }
                }
            }
        }
    }


    {/* output yuv file */
        int i, j;
        int dtp = ( bdp > 8 ) ? 2 : 1;
        for (i=0; i<HEIGHT; ++i)
        {
            for (j=0; j<WIDTH; ++j)
            {
                MsFS_Fwrite(&image[i*WIDTH + j].y, sizeof(MS_U8), dtp, fout);
                MsFS_Fwrite(&image[i*WIDTH + j].u, sizeof(MS_U8), dtp, fout);
                MsFS_Fwrite(&image[i*WIDTH + j].v, sizeof(MS_U8), dtp, fout);
    #if DEBUG
                if (j%2 == 0)
                {
                    fprintf(debug_text, "(%03d, %03d) Y:%d CB:%d\n",i, j, image[i*WIDTH+j].y, image[i*WIDTH+j].u);
                }
                else
                {
                    fprintf(debug_text, "(%03d, %03d) Y:%d CR:%d\n",i, j, image[i*WIDTH+j].y, image[i*WIDTH+j].v);
                }
    #endif
            }
        }
    }

#if DEBUG
    MsFS_Fflush(debug_out);
    MsFS_Fflush(debug_garbage);
    MsFS_Fflush(debug_vsync);
    MsFS_Fflush(debug_last);
    MsFS_Fflush(debug_text);

    MsFS_Fclose(debug_out);
    MsFS_Fclose(debug_garbage);
    MsFS_Fclose(debug_vsync);
    MsFS_Fclose(debug_last);
    MsFS_Fclose(debug_text);
#endif

    MsOS_Sync();
    MsOS_FlushMemory();

    return  ret;
}


/*
*   Display the images from a single buffer.
*
*   @fin:       file pointer to write.
*   @fout:      file pointer to the output file.
*   @return:    {DP_SUCCESS, DP_FAIL}
*/
int ParseOneFile(FILE* fin, FILE* fout, int bdp)
{
    MS_U32 u32LoadAddr = 0;
    pixel *image;
    pixel *pixel_buffer;

    int i;

    /* return status */
    int ret;

    /* for combining buffer */
    int x=0, y=0, pos;
    int frame_buffer_size;
    pixel* frame_buffer;

    /* pixel count*/
    int pixel_count = 0;

    /* vsync pos */
    int num_vsync = -1;

    /* report status */
    printf("BDP: %d WIDTH: %d HEIGHT: %d  FORMAT:", BDP, WIDTH, HEIGHT);
    switch (SRC_FMT)
    {
        case FMT_444:
            printf(" 444\n");
            break;
        case FMT_422:
            printf(" 422\n");
            break;
        case FMT_420:
            printf(" 420\n");
            break;
        default:
            printf(" unknown\n");
            break;
    }


#if DEBUG
    debug_out = MsFS_Fopen("debug.txt","w");
    debug_garbage = MsFS_Fopen("debug_garbage.txt", "w");
    debug_vsync = MsFS_Fopen("debug_vsync.txt", "w");
    debug_last = MsFS_Fopen("debug_last.txt","w");
    debug_text = MsFS_Fopen("debug_text.txt","w");
#endif

    /* init memory */
    u32LoadAddr = (MS_U32)ADC_DMA_ADDR;
    memset((pixel*)MS_PA2KSEG1(u32LoadAddr), 0, ADC_DMA_SIZE);//dump buffer addr

    image = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * IMAGE_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));
    pixel_buffer = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * MAX_BUFFER_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));
    frame_buffer = (pixel*)MS_PA2KSEG1(u32LoadAddr);
    LOAD_DUMP_ADDR(u32LoadAddr, WORD_ALIGN, (sizeof(pixel) * MAX_BUFFER_SIZE(WIDTH, HEIGHT)), (ADC_DMA_ADDR + ADC_DMA_SIZE));

    // if one of the calloc failed
    if ( (image && pixel_buffer && frame_buffer) == 0)
    {
        printf("Not enough free memory.\n");
        return DP_FAIL;
    }

    /* even pass */
    printf("Parsing pixels.\n");
    ret = ParseFile(fin, &pixel_count, &num_vsync, pixel_buffer);

    if (ret != DP_SUCCESS)
        return ret;

    /* trim buffer */
    frame_buffer_size = TrimBuffer(pixel_buffer, pixel_count, IMAGE_SIZE(WIDTH, HEIGHT), &num_vsync, frame_buffer);
    if (frame_buffer_size == 0)
        return DP_FAIL;

    x = 0;
    y = 0;

    for (i=0;i< frame_buffer_size; ++i)
    {
        int shift_bits = 12 - bdp;
        pos = (i + num_vsync) % frame_buffer_size;

        /* force right shift 2 bits */

        image[y*WIDTH + x].y = frame_buffer[pos].y >> shift_bits;
        image[y*WIDTH + x].u = frame_buffer[pos].u >> shift_bits;
        image[y*WIDTH + x].v = frame_buffer[pos].v >> shift_bits;

        x+=1;

        if (frame_buffer[pos].nl)
        {
            y+=1;
            x=0;
        }

        if (y >= HEIGHT)
            break;

    }


    /* output 422 */
    {
        int i, j;
        int dtp = ( bdp > 8 ) ? 2 : 1;
        for (i=0; i<HEIGHT; ++i)
            for (j=0; j<WIDTH; ++j)
            {
                // c
                if (j%2 == 0)
                MsFS_Fwrite(&image[i*WIDTH + j].u, sizeof(MS_U8), dtp, fout);
                else
                MsFS_Fwrite(&image[i*WIDTH + j].v, sizeof(MS_U8), dtp, fout);
                // y
                MsFS_Fwrite(&image[i*WIDTH + j].y, sizeof(MS_U8), dtp, fout);
            }
    }

#if DEBUG
    MsFS_Fflush(debug_out);
    MsFS_Fflush(debug_garbage);
    MsFS_Fflush(debug_vsync);
    MsFS_Fflush(debug_last);
    MsFS_Fflush(debug_text);

    MsFS_Fclose(debug_out);
    MsFS_Fclose(debug_garbage);
    MsFS_Fclose(debug_vsync);
    MsFS_Fclose(debug_last);
    MsFS_Fclose(debug_text);
#endif

    MsOS_Sync();
    MsOS_FlushMemory();

    return  ret;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_Convert2VUV(MS_U8* pInPath, FILE* fOut)
{
    FILE* fin_even = MsFS_Fopen((char*)pInPath, "rb");

    BDP=12;
    SCRAMBLE_IN=1;
    ODD_PASS=0;
    SRC_FMT=FMT_422;

    printf("Output Depth = [%d] \n", enOutDepth);
    if (DP_SUCCESS == ParseOneFile(fin_even ,fOut, enOutDepth))
    {
            // do something...
    }
    else
    {
        fprintf(stderr, "Parse error.\n");
        return FALSE;
    }

    MsFS_Fflush(fin_even);
    MsFS_Fclose(fin_even);

#if (DEBUG == 0)
    remove((char*)pInPath);
#endif

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_Convert2VUV_Multi(MS_U8* pEvenPath, MS_U8* pOddPath,FILE* fOut)
{
    FILE* fin_even = MsFS_Fopen((char*)pEvenPath, "rb");
    FILE* fin_odd = MsFS_Fopen((char*)pOddPath, "rb");

    BDP=12;
    SCRAMBLE_IN=0;
    ODD_PASS=1;
    SRC_FMT=FMT_444;

    printf("Output Depth = [%d] \n", enOutDepth);
    if (DP_SUCCESS == ParseBothFile(fin_even ,fin_odd,  fOut, enOutDepth, DP_COMBINE))
    {
            // do something...
    }
    else
    {
        fprintf(stderr, "Parse error.\n");
        return FALSE;
    }

    MsFS_Fflush(fin_even);
    MsFS_Fflush(fin_odd);
    MsFS_Fclose(fin_even);
    MsFS_Fclose(fin_odd);

#if (DEBUG == 0)
    remove((char*)pEvenPath);
    remove((char*)pOddPath);
#endif

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////


static MS_BOOL _Demo_DolbyHDR_PreIDK_WriteReg16(MS_U16 u16Bank, MS_U16 u16Offset, MS_U16 u16RegValue)
{
#if defined(MSOS_TYPE_ECOS)
    *(volatile MS_U16*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset)<<1))= u16RegValue;
#elif defined(MSOS_TYPE_LINUX)
    MS_PHY phyBankSize;

    MS_VIRT virt32NonPmBase = 0;
    MS_VIRT virt32PmBase = 0;
    if( !MDrv_MMIO_GetBASE(&virt32NonPmBase, &phyBankSize, MS_MODULE_HW))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_HW)!\n");
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&virt32PmBase, &phyBankSize, MS_MODULE_PM))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_PM)!\n");
        return FALSE;
    }

    if(u16Bank >= 0x1000)
    {
        REG16_NPM(((u16Bank-0x1000)<<9) + ((u16Offset)<<1)) = u16RegValue;
    }
    else
    {
        REG16_PM((u16Bank<<9) + ((u16Offset)<<1)) = u16RegValue;
    }
#endif
//printf("##[%s]##%lx, %lx, %lx##\n",__FUNCTION__, u16Bank, u16Offset, u16RegValue);
    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_WriteReg8(MS_U16 u16Bank, MS_U16 u16Offset, MS_U8 u8RegValue)
{
#if defined(MSOS_TYPE_ECOS)
    if((u16Offset & 0x1)  == 0)
        *(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset)<<1))= u8RegValue;
    else
        *(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset-1)<<1) + 1)= u8RegValue;
#elif defined(MSOS_TYPE_LINUX)
    MS_PHY phyBankSize;

    MS_VIRT virt32NonPmBase = 0;
    MS_VIRT virt32PmBase = 0;
    if( !MDrv_MMIO_GetBASE(&virt32NonPmBase, &phyBankSize, MS_MODULE_HW))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_HW)!\n");
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&virt32PmBase, &phyBankSize, MS_MODULE_PM))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_PM)!\n");
        return FALSE;
    }

    if(u16Bank >= 0x1000)
    {
        if((u16Offset & 0x1)  == 0)
            REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset)<<1)) = u8RegValue;
        else
            REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset-1)<<1) + 1) = u8RegValue;
    }
    else
    {
        if((u16Offset & 0x1)  == 0)
            REG8_PM((u16Bank<<9) + ((u16Offset)<<1)) = u8RegValue;
        else
            REG8_PM((u16Bank<<9) + ((u16Offset-1)<<1) + 1) = u8RegValue;
    }
#endif
//printf("##[%s]##%lx, %lx, %lx##\n",__FUNCTION__, u16Bank, u16Offset, u8RegValue);
    return TRUE;
}

MS_BOOL _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(MS_U16 u16Bank, MS_U16 u16Offset, MS_U8 u8Mask, MS_U8 u8RegValue)
{
#if defined(MSOS_TYPE_ECOS)
    if((u16Offset & 0x1)  == 0)
        *(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset)<<1))= (((*(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset)<<1)))  & ~(u8Mask)) | (u8RegValue & u8Mask));
    else
        *(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset-1)<<1) + 1)= (((*(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset)<<1)))  & ~(u8Mask)) | (u8RegValue & u8Mask));
#elif defined(MSOS_TYPE_LINUX)
    MS_PHY phyBankSize;

    MS_VIRT virt32NonPmBase = 0;
    MS_VIRT virt32PmBase = 0;
    if( !MDrv_MMIO_GetBASE(&virt32NonPmBase, &phyBankSize, MS_MODULE_HW))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_HW)!\n");
        return FALSE;
    }

    if( !MDrv_MMIO_GetBASE(&virt32PmBase, &phyBankSize, MS_MODULE_PM))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_PM)!\n");
        return FALSE;
    }

    if(u16Bank >= 0x1000)
    {
        if((u16Offset & 0x1)  == 0)
            REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset)<<1)) = ((REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset)<<1))  & ~(u8Mask)) | (u8RegValue & u8Mask));
        else
            REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset-1)<<1) + 1) = ((REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset-1)<<1) + 1)  & ~(u8Mask)) | (u8RegValue & u8Mask));
    }
    else
    {
        if((u16Offset & 0x1)  == 0)
            REG8_PM((u16Bank<<9) + ((u16Offset)<<1)) = ((REG8_PM((u16Bank<<9) + ((u16Offset)<<1))  & ~(u8Mask)) | (u8RegValue & u8Mask));
        else
            REG8_PM((u16Bank<<9) + ((u16Offset-1)<<1) + 1) = ((REG8_PM((u16Bank<<9) + ((u16Offset-1)<<1) + 1)  & ~(u8Mask)) | (u8RegValue & u8Mask));
    }
#endif
//printf("##[%s]##%lx, %lx, %lx, %lx##\n",__FUNCTION__, u16Bank, u16Offset, u8Mask, u8RegValue);
    return TRUE;
}


MS_U8 _Demo_DolbyHDR_PreIDK_ReadReg8(MS_U16 u16Bank, MS_U16 u16Offset)
{
#if defined(MSOS_TYPE_ECOS)
    if((u16Offset & 0x1)  == 0)
        return (*(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset)<<1)));
    else
        return (*(volatile MS_U8*)(0xBF000000+((2*(u16Bank))<<8)+((u16Offset-1)<<1) + 1));
#elif defined(MSOS_TYPE_LINUX)
    MS_PHY phyBankSize;

    MS_VIRT virt32NonPmBase = 0;
    MS_VIRT virt32PmBase = 0;
    if( !MDrv_MMIO_GetBASE(&virt32NonPmBase, &phyBankSize, MS_MODULE_HW))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_HW)!\n");
        return 0;
    }

    if( !MDrv_MMIO_GetBASE(&virt32PmBase, &phyBankSize, MS_MODULE_PM))
    {
        printf("Error: failed to MDrv_MMIO_GetBASE(MS_MODULE_PM)!\n");
        return 0;
    }

    if(u16Bank >= 0x1000)
    {
        if((u16Offset & 0x1)  == 0)
            return REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset)<<1));
        else
            return REG8_NPM(((u16Bank-0x1000)<<9) + ((u16Offset-1)<<1) + 1);
    }
    else
    {
        if((u16Offset & 0x1)  == 0)
            return REG8_PM((u16Bank<<9) + ((u16Offset)<<1));
        else
            return REG8_PM((u16Bank<<9) + ((u16Offset-1)<<1) + 1);
    }
#endif
}

MS_BOOL _Demo_DolbyHDR_PreIDK_GetLUTArray(MS_U8* pPath)
{
    FILE* s32FdRead =NULL ;
    printf("=====[%s][%d]===== pPath: %s\n",__FUNCTION__,__LINE__,pPath);

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&_gs32ScrambleNonCachedPoolID);

    if (pPath == NULL)
    {
        printf("Please enter the Path! \n");
    }
    else
    {
        s32FdRead = MsFS_Fopen((char*)pPath, "rb");
    }

    if (s32FdRead == NULL)
    {
        printf("[%s]File open error!! \n",__FUNCTION__);
        return FALSE;
    }

    MsFS_Fseek(s32FdRead,0 ,SEEK_END);
    _gu32ScrambleSize = MsFS_Ftell(s32FdRead);
    printf("[%s][%d]File Size %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,_gu32ScrambleSize);
    MsFS_Fseek(s32FdRead,0 ,SEEK_SET);

    if (_gpu8ScrambleArray == NULL)
    {
        _gpu8ScrambleArray = MsOS_AllocateMemory((_gu32ScrambleSize) *sizeof(MS_U8),_gs32ScrambleNonCachedPoolID);
    }

    if (_gpu8ScrambleArray == NULL)
    {
         MsFS_Fflush(s32FdRead);
         MsFS_Fclose(s32FdRead);
         printf("Memory allocate failed, file size = %"DTC_MS_U32_x" \n", _gu32ScrambleSize);
         return FALSE;
    }

    MsFS_Fread(_gpu8ScrambleArray,1,_gu32ScrambleSize,s32FdRead);

    MsOS_Sync();
    MsOS_FlushMemory();
    MsFS_Fflush(s32FdRead);
    MsFS_Fclose(s32FdRead);

    return TRUE;
}

static void PreIDK_Cb(MS_U32 u32Vector)
{
    XC_AUTODOWNLOAD_DATA_INFO stDataInfo = {};
    XC_AUTODOWNLOAD_FORMAT_INFO stFormatInfo = {};
    MS_U16 u16CopiedLength = sizeof(stDataInfo);

    stFormatInfo.u16StartAddr = 0;
    stFormatInfo.u16EndAddr = _gu32ScrambleSize -1;
    stFormatInfo.bEnableRange = 1;

    stDataInfo.u32DataInfo_Version = AUTODOWNLOAD_DATA_INFO_VERSION;
    stDataInfo.u16DataInfo_Length = u16CopiedLength;
    stDataInfo.enClient = E_XC_AUTODOWNLOAD_CLIENT_SCRAMBLE;
    stDataInfo.pu8Data = _gpu8ScrambleArray;
    stDataInfo.u32Size = _gu32ScrambleSize;
    stDataInfo.pParam = &stFormatInfo;

    MApi_XC_AutoDownload_Write(&stDataInfo);
    MApi_XC_AutoDownload_Fire(E_XC_AUTODOWNLOAD_CLIENT_SCRAMBLE);

    MsOS_EnableInterrupt((InterruptNum)u32Vector);
}

MS_BOOL _Demo_DolbyHDR_PreIDK_ADL_Enable(void)
{
    if(_gbADL_Vsync_Attach == FALSE)
    {
        MsOS_AttachInterrupt(E_INT_IRQ_DISP, (InterruptCb)PreIDK_Cb);
        _gbADL_Vsync_Attach = TRUE;
    }
    MsOS_EnableInterrupt(E_INT_IRQ_DISP);

    _gbADL_Enable = TRUE;

    return TRUE;
}

MS_BOOL _Demo_DolbyHDR_PreIDK_ADL_Disable(void)
{
    MsOS_DisableInterrupt(E_INT_IRQ_DISP);
    MsOS_FreeMemory((void *)_gpu8ScrambleArray,_gs32ScrambleNonCachedPoolID);

    _gpu8ScrambleArray = NULL;
    _gu32ScrambleSize = 0;
    _gs32ScrambleNonCachedPoolID = 0;
    _gbADL_Enable = FALSE;

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_PathParser(MS_U8* pPath)
{
    DIR *dpMain, *dpInput;
    struct dirent *dirpMain,*dirpInput;
    char* strTok = NULL;
    char strTemp[FILE_NAME_LENGTH] = {'\0'};
//    _gbEL_Enable = FALSE;

    if((dpMain = MsFS_OpenDir((char*)pPath)))
    {
        snprintf((char*)strLaunchPath, FILE_NAME_LENGTH-1, (char*)pPath);
        CHECK_END_SLASH(strLaunchPath);

        snprintf((char*)strFrmScpPath, FILE_NAME_LENGTH-1, "%sscp/", (char*)strLaunchPath);

        while ((dirpMain = MsFS_ReadDir(dpMain)))
        {
            if((dirpMain->d_type == 4) && (strcmp(dirpMain->d_name, ".") != 0) && (strcmp(dirpMain->d_name, "..") != 0)) //run cmd for each vector here
            { //in_10b out_10b
                snprintf((char*)strTemp, FILE_NAME_LENGTH-1, dirpMain->d_name);
                strTok = strtok((char*)strTemp, "_"); //input?output

                if(strcmp(strTok, "in") == 0) //intput dir
                {
                    snprintf((char*)strFrmInputPath, FILE_NAME_LENGTH-1, "%s%s/", (char*)strLaunchPath, dirpMain->d_name);
                    strTok = strtok(NULL, "_"); //bits
                    if(strcmp(strTok, "10b") == 0) //10bit
                    {
                        _gbIs10Bit_Input = TRUE;
                    }
                    else if(strcmp(strTok, "8b") == 0) //8bit
                    {
                        _gbIs10Bit_Input = FALSE;
                    }

                    if((dpInput = MsFS_OpenDir((char*)strFrmInputPath)))  //intput frame bin file path
                    {
                        while ((dirpInput = MsFS_ReadDir(dpInput)))
                        {
                            if((dirpInput->d_type == 4) && (strcmp(dirpInput->d_name, ".") != 0) && (strcmp(dirpInput->d_name, "..") != 0))
                            {
                                snprintf((char*)strTemp, FILE_NAME_LENGTH-1, dirpInput->d_name);
                                strTok = strtok((char*)strTemp, "_x"); //BL?EL

                                if(strcmp(strTok, "bl") == 0) //Base Layer
                                {
                                    strTok = strtok(NULL, "_x"); //Width
                                    stFrmLayerInfo.stBL_Size.u16Width = (MS_U16)atoi(strTok);
                                    strTok = strtok(NULL, "_x"); //Height
                                    stFrmLayerInfo.stBL_Size.u16Height= (MS_U16)atoi(strTok);

                                    snprintf((char*)stFrmLayerInfo.pu8BLPath, FILE_NAME_LENGTH-1, (char*)strFrmInputPath);
                                    strncat((char*)stFrmLayerInfo.pu8BLPath, dirpInput->d_name, FILE_NAME_LENGTH - strlen((char*)stFrmLayerInfo.pu8BLPath) - 1);
                                }
                                else if(strcmp(strTok, "el") == 0) //Enhance Layer
                                {
//                                    _gbEL_Enable = TRUE;
                                    strTok = strtok(NULL, "_x"); //Width
                                    stFrmLayerInfo.stEL_Size.u16Width = (MS_U16)atoi(strTok);
                                    strTok = strtok(NULL, "_x"); //Height
                                    stFrmLayerInfo.stEL_Size.u16Height= (MS_U16)atoi(strTok);

                                    snprintf((char*)stFrmLayerInfo.pu8ELPath, FILE_NAME_LENGTH-1, (char*)strFrmInputPath);
                                    strncat((char*)stFrmLayerInfo.pu8ELPath, dirpInput->d_name, FILE_NAME_LENGTH - strlen((char*)stFrmLayerInfo.pu8ELPath) - 1);
                                }
                            }
                        }
                        MsFS_CloseDir(dpInput);
                    } //openInputdir end
                 } //input end
                else if(strcmp(strTok, "out") == 0) //output dir
                {
                    snprintf((char*)strFrmOutputPath, FILE_NAME_LENGTH-1, "%s%s/", (char*)strLaunchPath, dirpMain->d_name);
                    strTok = strtok(NULL, "_"); //bits
                    if(strcmp(strTok, "12b") == 0) //12bit
                    {
                        enOutDepth = E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_12BIT;
                    }
                    else if(strcmp(strTok, "10b") == 0) //10bit
                    {
                        enOutDepth = E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_10BIT;
                    }
                    else if(strcmp(strTok, "8b") == 0) //8bit
                    {
                        enOutDepth = E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_8BIT;
                    }

                    if(NULL != (strTok = strtok(NULL, "_"))) //rgb??
                    {
                        if(strcmp(strTok, "rgb") == 0) //outputRGB
                        {
                            _gbIsRGB_Output = TRUE;
                        }
                    }
                 } //output end
            } //end if search dirs
        } //end while ((dirpMain = MsFS_ReadDir(dpMain)))
        MsFS_CloseDir(dpMain);
        return TRUE;
    } //openMaindir end

    printf("[%s][ERR] It's not a folder path! \n",__FUNCTION__);
    return FALSE;
}

static MS_U32 _Demo_DolbyHDR_PreIDK_GetTimeDiff(MS_U32 u32Time)
{
    MS_U32 u32CurTime = MsOS_GetSystemTime();

    if(u32CurTime  >= u32Time)
    {
        return u32CurTime  - u32Time;
    }
    else
    {
        return 0xFFFFFFFF - u32Time + u32CurTime;
    }
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_SetMode(void)
{
    MS_U32 u32XCDevice = 0;
    MS_U32 u32OutputTiming = 0;

    switch(stFrmLayerInfo.stBL_Size.u16Height)
    {
        case 480:
            u32OutputTiming = E_OUTPUT_TIMING_720X480_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        case 720:
            u32OutputTiming = E_OUTPUT_TIMING_1280X720_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        case 1080:
            u32OutputTiming = E_OUTPUT_TIMING_1920X1080_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        case 2160:
            u32OutputTiming = E_OUTPUT_TIMING_3840X2160_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        default:
            break;
    }
    MsOS_DelayTask(1000);

    MSAPI_XC_VDEC_DispInfo stVidStatus = {0};
    memset(&stVidStatus, 0, sizeof(MSAPI_XC_VDEC_DispInfo));
     stVidStatus.u32FrameRate  = 25000;
     stVidStatus.u8Interlace   = 0;
     stVidStatus.u16HorSize      = (stFrmLayerInfo.stBL_Size.u16Width + 7) & 0xfff8;
     stVidStatus.u16VerSize      = stFrmLayerInfo.stBL_Size.u16Height;
     stVidStatus.u8AspectRate    = 2;
     stVidStatus.u16SarWidth     = 1;
     stVidStatus.u16SarHeight    = 1;
     stVidStatus.u16CropRight    = 0;
     stVidStatus.u16CropLeft     = 0;
     stVidStatus.u16CropBottom   = 0;
     stVidStatus.u16CropTop      = 0;
 //        stVidStatus.u32AspectWidth  = (720 + 7) & 0xfff8;
 //        stVidStatus.u32AspectHeight = 480;
    msAPI_XC_SetVDECInfo_EX_ByPath(E_MSAPI_XC_MAIN_DECODER_PATH, stVidStatus);

    msAPI_XC_ForceMVOPDramType_EX(E_MVOP_DEV_0, E_MSAPI_XC_MVOP_SOURCE_TYPE_422);
    msAPI_XC_SetMVOPMiuSel_EX(E_MSAPI_XC_MAIN_DECODER_PATH, TRUE, VDEC_MIU);
    msAPI_XC_SetMVOPConfig_EX(E_MSAPI_XC_MAIN_DECODER_PATH, MVOP_INPUT_DRAM);
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x133f, 0x7e, 0x01, 0x01);

return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_RunRiuScripts(MS_U8* pPath)
{

    FILE* pFile =NULL ;
    char pu8ReadStr[256] = {'\0'};
    char* strEndOfLine = "%*[^\n]";
    MS_U32 u32ReadVal[3] = {0};
    MS_U32 u32CurReadPos = 0;

    if (pPath == NULL)
    {
        printf("Please enter the RIU Script Path! \n");
        return FALSE;
    }
    else
    {
        pFile = MsFS_Fopen((char*)pPath, "r");
    }

    if (pFile == NULL)
    {
        printf("Script open error, please check if the script is in storage! \n");
        return FALSE;
    }

    while (!feof(pFile))
    {
        fscanf(pFile, "%s", pu8ReadStr);
        if(strcmp(pu8ReadStr, "wriu") == 0)
        {
//            printf("Load WRIU OK!![%p]\n",pFile);
            u32CurReadPos = MsFS_Ftell(pFile);
            fscanf(pFile, "%s", pu8ReadStr);

            if(strcmp(pu8ReadStr, "-b") == 0)
            {
                    fscanf(pFile, "%"DTC_MS_U32_x" %"DTC_MS_U32_x" %"DTC_MS_U32_x"", &u32ReadVal[0], &u32ReadVal[1], &u32ReadVal[2]);
                    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(RIU_BANK(u32ReadVal[0]), RIU_OFFSET(u32ReadVal[0]), (MS_U8)u32ReadVal[1], (MS_U8)u32ReadVal[2]);
            }
            else if(strcmp(pu8ReadStr, "-w") == 0)
            {
                    fscanf(pFile, "%"DTC_MS_U32_x" %"DTC_MS_U32_x"", &u32ReadVal[0], &u32ReadVal[1]);
                    _Demo_DolbyHDR_PreIDK_WriteReg16(RIU_BANK(u32ReadVal[0]), RIU_OFFSET(u32ReadVal[0]), (MS_U16)u32ReadVal[1]);
            }
            else
            {
                    MsFS_Fseek(pFile, u32CurReadPos, SEEK_SET);
                    fscanf(pFile, "%"DTC_MS_U32_x" %"DTC_MS_U32_x"", &u32ReadVal[0], &u32ReadVal[1]);
                    _Demo_DolbyHDR_PreIDK_WriteReg8(RIU_BANK(u32ReadVal[0]), RIU_OFFSET(u32ReadVal[0]), (MS_U8)u32ReadVal[1]);
            }
            fscanf(pFile, strEndOfLine, pu8ReadStr);  //fseek to the end of the line
        }
        else  //comment
        {
            fscanf(pFile, strEndOfLine, pu8ReadStr);  //fseek to the end of the line
        }
    }

    MsFS_Fflush(pFile);
    MsFS_Fclose(pFile);
    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_SaveDram2Bin(MS_U8* pPath, MS_U32* u32Addr, MS_U32* u32Size)
{
    FILE* s32FdWrite =NULL ;
    MS_U8* pu8Buf = NULL;

    pu8Buf = (MS_U8 *)MS_PA2KSEG1(*u32Addr);

    if (pPath == NULL)
    {
        printf("Please enter the Path!! \n");
        return FALSE;
    }
    else
    {
        s32FdWrite = MsFS_Fopen((char*)pPath, "wb");
    }

    if (s32FdWrite == NULL)
    {
        printf("File open error!! \n");
        return FALSE;
    }

    MsFS_Fwrite(pu8Buf, 1, *u32Size, s32FdWrite);
    MsFS_Fflush(s32FdWrite);
    MsFS_Fclose(s32FdWrite);

    printf("###[%s]#Dump has been done to %s !!############# \n",__FUNCTION__, (char*)pPath);
    return TRUE;
}

MS_U32 _Demo_DolbyHDR_PreIDK_LoadBin2Dram(MS_U8* pPath, MS_U32* u32Addr)
{
    MS_U8* pu8FB = NULL;
    MS_U32 u32FileRSize = 0;
    MS_U32 u32FileSize = 0;
    FILE* s32FdRead =NULL ;
    printf("=====[%s][%d]===== pPath: %s\n",__FUNCTION__,__LINE__,pPath);

    if (pPath == NULL)
    {
        printf("Please enter the Path! \n");
    }
    else
    {
        s32FdRead = MsFS_Fopen((char*)pPath, "rb");
    }

    if (s32FdRead == NULL)
    {
        printf("File open error!! \n");
        return FALSE;
    }

    MsFS_Fseek(s32FdRead,0 ,SEEK_END);
    u32FileSize = MsFS_Ftell(s32FdRead);
    printf("[%s][%d]File Size %"DTC_MS_U32_d" \n",__FUNCTION__,__LINE__,u32FileSize);
    MsFS_Fseek(s32FdRead,0 ,SEEK_SET);

    pu8FB = (MS_U8 *)MS_PA2KSEG1(*u32Addr);  //Dram addr
    memset(pu8FB, 0, u32FileSize);

    printf("[%s][%d] addr:0x%"DTC_MS_U32_x" size:%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,*u32Addr, u32FileSize);
    {
        u32FileRSize = MsFS_Fread(pu8FB,1,u32FileSize,s32FdRead);
        if (u32FileRSize == 0)
        {
            MsFS_Fflush(s32FdRead);
            MsFS_Fclose(s32FdRead);
            printf("[%s][%d] MsFS_Fread Failed \n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        else
        {
            *(pu8FB+u32FileRSize) = '\0';//for coverity issue String not null terminated (STRING_NULL)
        }
    }
    printf("[%s][%d] Load bin from 0x%p to 0x%"DTC_MS_U32_x", size 0x%"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,pu8FB, *u32Addr, u32FileSize);

    MsOS_Sync();
    MsOS_FlushMemory();
    MsFS_Fflush(s32FdRead);
    MsFS_Fclose(s32FdRead);

    printf("###[%s][%d] Done!\n",__FUNCTION__,__LINE__);
    return u32FileSize;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_OSD_FindBin(MS_U8* pPath)
{
    DIR *dpOSD;
    struct dirent *dirpOSD;


    if((dpOSD = MsFS_OpenDir((char*)pPath)))
    {
        while ((dirpOSD = MsFS_ReadDir(dpOSD)))
        {
            if(dirpOSD->d_type == 8) //run cmd for each vector here
            {
                strncat((char*)strFrmOSDPath, dirpOSD->d_name, strlen(dirpOSD->d_name));
                printf("@Find OSD bin file in [%s]\n", (char*)strFrmOSDPath);
                MsFS_CloseDir(dpOSD);
                return TRUE;
            }
        }
        MsFS_CloseDir(dpOSD);
    }

    printf("[%s][ERR]This case is without OSD bin file !! \n",__FUNCTION__);
    return FALSE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_OSD_Init(void)
{
    MS_U16 u16pfmt = E_MS_FMT_ABGR8888;
    MS_U32 u32ColorType = 0;
    MS_U32 bEnable = 1;
    MS_U32 u32Auto = 0;
    MS_U32 u32AlphaValue = 0xff;
    MS_U32 u32Width = IPANEL(&stPNL_DeviceId ,Width);
    MS_U32 u32height = IPANEL(&stPNL_DeviceId ,Height);

    Demo_MM_Photo_SetGwinFmt(&u16pfmt);
    Demo_OSD_SetConfig();
    Demo_OSD_CreateWin(&u32ColorType, (MS_U32*)&u32Width, (MS_U32*)&u32height,
        (MS_U32*)&u32Width, (MS_U32*)&u32height);
    Demo_OSD_EnablePerPixelAlpha(&u32Auto, &u32AlphaValue);
    Demo_OSD_EnableWin(&bEnable);

    return TRUE;
}

/*
static MS_BOOL _Demo_DolbyHDR_PreIDK_OSD_SetMixerBypass(void)
{

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x100a, 0x88, 0x0000); //clk_lut:       BK100b,44[0] = 0
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x100a, 0x8c, 0x0000); //othdr_clk_lut: BK100a,46[0] = 0

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0xf8, 0x0100); // Mixer Bank {7c[8], 7F[3:0]} = 5'h15
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0xfe, 0x0105);

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0x02, 0x0000);//[3]: Group 1 HDR engine sel  [2]: Group 1 HDR bypass
                        //[1]: Group 0 HDR engine sel  [0]: Group 0 HDR bypass

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0x04, 0x0000);//[0]: HDR setting Link        [1]: HDR CM setting link
                        //[2]: SDR CM setting link     [3]: H121 setting link
                        //[4]: SCD setting link

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0x22, 0xFF01);
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0xf8, 0x0000);
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x101f, 0xfe, 0x0200);
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1202, 0xfc, 0x7ffe);

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x120f, 0x21, 0x10, 0x10);

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1202, 0x01, 0x04, 0x00);
    _Demo_DolbyHDR_PreIDK_WriteReg8(0x120f, 0x20, 0x22);
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x132f, 0x6e, 0x80, 0x00);

    return TRUE;
}
*/

static MS_BOOL _Demo_DolbyHDR_PreIDK_FinalizeLastFrame(void)
{
    _gu32BL8Bit_YAddr = 0;
    _gu32BL8Bit_CAddr = 0;
    _gu32BL2Bit_YAddr = 0;
    _gu32BL2Bit_CAddr = 0;
    _gu32EL8Bit_YAddr = 0;
    _gu32EL8Bit_CAddr = 0;
    _gu32EL2Bit_YAddr = 0;
    _gu32EL2Bit_CAddr = 0;
    Demo_OSD_FreeWin();

#if (DEMO_XC_DUALXC_TEST == 1)
    msAPI_XC_SetVideoMuteByMode_EX(&_gstXC_DeviceId[0], TRUE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
#else
    msAPI_XC_SetVideoMuteByMode_EX(&_gstXC_DeviceId, TRUE, E_MSAPI_XC_MAIN_WINDOW, E_MSAPI_XC_VIDEO_MUTE_MODE_AUTO_SYNC);
#endif
    MsOS_DelayTask(1000);

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_CearGlobalVar(void)
{
    _gbEL_Enable = TRUE;
    _gbIs10Bit_Input = TRUE;
    _gbIsRGB_Output = FALSE;
    enOutDepth = E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_12BIT;

    memset(&stFrmLayerInfo, 0, sizeof(_ST_DOLBYHDR_PREIDK_FRMLAYER_INFO));
    strLaunchPath[0] = '\0';
    strFrmInputPath[0] = '\0';
    strFrmOutputPath[0] = '\0';
    strFrmScpPath[0] = '\0';
    strFrmOSDPath[0] = '\0';
    _gu32DumpSize = 0;

    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_LoadPicture(MS_U8* pPath)
{
    MS_U32 u32LoadAddr = 0;
    MS_U32 u32LoadSize = 0;
    MS_U32 u32PathLen = 0;

    u32PathLen = strlen((char *)pPath);
    if(u32PathLen > FILE_NAME_LENGTH)
    {
        printf("[Err] u32BLPathLen length is too long!!\n");
        return FALSE;
    }

    u32LoadAddr = (MS_U32)GOP_FB_ADDR;
    memset((MS_U8 *)MS_PA2KSEG1(u32LoadAddr), 0, GOP_FB_SIZE);//GOP buffer addr

    LOAD_NEXT_2DRAM(pPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (GOP_FB_ADDR + GOP_FB_SIZE));

    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_LoadFrame(MS_U8* pPathBL, MS_U8* pPathEL, unsigned int* uIdx, MS_BOOL* b10Bit)
{
    MS_U32 u32LoadAddr = 0;
    MS_U32 u32LoadSize = 0;
    MS_U32 u32BLPathLen = 0;
    MS_U32 u32ELPathLen = 0;

    char pu8BLPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8ELPath[FILE_NAME_LENGTH] = {'\0'};

    char pu8BL8YPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8BL8CPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8BL2YPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8BL2CPath[FILE_NAME_LENGTH] = {'\0'};

    char pu8EL8YPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8EL8CPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8EL2YPath[FILE_NAME_LENGTH] = {'\0'};
    char pu8EL2CPath[FILE_NAME_LENGTH] = {'\0'};

//printf("[%d]", *pPathEL);

    if((0 == *pPathEL) || (strcmp((char*)pPathEL, "0") == 0))
    {
        printf("This frame has [no] enhance layer!!\n");
        _gbEL_Enable = FALSE;
    }
    else
    {
        printf("This frame has enhance layer!!\n");
        _gbEL_Enable = TRUE;
    }

    u32BLPathLen = strlen((char *)pPathBL);
    if(u32BLPathLen > FILE_NAME_LENGTH)
    {
        printf("[Err] u32BLPathLen length is too long!!\n");
        return FALSE;
    }

    u32LoadAddr = (MS_U32)VDEC_FB_ADDR;
    memset((MS_U8 *)MS_PA2KSEG1(u32LoadAddr), 0, VDEC_FB_SIZE);//Framebuffer addr

    snprintf((char*)pu8BLPath, FILE_NAME_LENGTH-1, (char*)pPathBL);
    CHECK_END_SLASH(pu8BLPath);

    snprintf((char*)pu8BL8YPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8BLPath, *uIdx);
    strncat((char*)pu8BL8YPath, BL_8B_Y_FILE, strlen(BL_8B_Y_FILE));
    snprintf((char*)pu8BL8CPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8BLPath, *uIdx);
    strncat((char*)pu8BL8CPath, BL_8B_C_FILE, strlen(BL_8B_C_FILE));

    _gu32BL8Bit_YAddr = u32LoadAddr;
    LOAD_NEXT_2DRAM(pu8BL8YPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));
    _gu32BL8Bit_CAddr = u32LoadAddr;
    LOAD_NEXT_2DRAM(pu8BL8CPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));

    if(*b10Bit == TRUE)
    {
        snprintf((char*)pu8BL2YPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8BLPath, *uIdx);
        strncat((char*)pu8BL2YPath, BL_2B_Y_FILE, strlen(BL_2B_Y_FILE));
        snprintf((char*)pu8BL2CPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8BLPath, *uIdx);
        strncat((char*)pu8BL2CPath, BL_2B_C_FILE, strlen(BL_2B_C_FILE));

        _gu32BL2Bit_YAddr = u32LoadAddr;
        LOAD_NEXT_2DRAM(pu8BL2YPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));
        _gu32BL2Bit_CAddr = u32LoadAddr;
        LOAD_NEXT_2DRAM(pu8BL2CPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));
    }

    if(_gbEL_Enable == TRUE)
    {
        u32ELPathLen = strlen((char *)pPathEL);
        if(u32ELPathLen > FILE_NAME_LENGTH)
        {
            printf("[Err] u32ELPathLen length is too long!!\n");
            return FALSE;
        }

        snprintf((char*)pu8ELPath, FILE_NAME_LENGTH-1, (char*)pPathEL);
        CHECK_END_SLASH(pu8ELPath);

        snprintf((char*)pu8EL8YPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8ELPath, *uIdx);
        strncat((char*)pu8EL8YPath, EL_8B_Y_FILE, strlen(EL_8B_Y_FILE));
        snprintf((char*)pu8EL8CPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8ELPath, *uIdx);
        strncat((char*)pu8EL8CPath, EL_8B_C_FILE, strlen(EL_8B_C_FILE));

        _gu32EL8Bit_YAddr = u32LoadAddr;
        LOAD_NEXT_2DRAM(pu8EL8YPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));
        _gu32EL8Bit_CAddr = u32LoadAddr;
        LOAD_NEXT_2DRAM(pu8EL8CPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));

        if(*b10Bit == TRUE)
        {
            snprintf((char*)pu8EL2YPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8ELPath, *uIdx);
            strncat((char*)pu8EL2YPath, EL_2B_Y_FILE, strlen(EL_2B_Y_FILE));
            snprintf((char*)pu8EL2CPath, FILE_NAME_LENGTH-1, "%s%03u", (char*)pu8ELPath, *uIdx);
            strncat((char*)pu8EL2CPath, EL_2B_C_FILE, strlen(EL_2B_C_FILE));

            _gu32EL2Bit_YAddr = u32LoadAddr;
            LOAD_NEXT_2DRAM(pu8EL2YPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));
            _gu32EL2Bit_CAddr = u32LoadAddr;
            LOAD_NEXT_2DRAM(pu8EL2CPath, u32LoadAddr, WORD_ALIGN, u32LoadSize, (VDEC_FB_ADDR + VDEC_FB_SIZE));
        }
    }

    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_OSDBlending(MS_U8* pPath)
{
    _Demo_DolbyHDR_PreIDK_OSD_Init();
    Demo_DolbyHDR_PreIDK_LoadPicture((MS_U8*)pPath);
//    _Demo_DolbyHDR_PreIDK_OSD_SetMixerBypass();

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_LoadHDR10BitsDolbyBL(void)
{
//--------------------------------------------
// MVOP main load Image to frame buffer0
//--------------------------------------------
//swch 4
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x131f, 0x20, 0x1088);
//wriu -w 0x131f20  0x1088  10
//_Demo_DolbyHDR_PreIDK_WriteReg16(0x110a, 0x22, 0x0000);
//wriu -w 0x110a22  0x0000  11
    MsOS_DelayTask(100);
//wait 100

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x4e, 0x0010);
//wriu -w 0x10144E  0x0010  2F //when riu_debug_sel = 0

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x4a, stFrmLayerInfo.stBL_Size.u16Width); // h_size=360
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x4c, stFrmLayerInfo.stBL_Size.u16Height); // v_size=240

//load bin Y/UV 8 bit
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x42, WORD_LADDR(_gu32BL8Bit_YAddr));
//wriu -w 0x101442  0x0000  21 //Y Start address (Word:8byte)
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x44, WORD_HADDR(_gu32BL8Bit_YAddr));
//wriu -w 0x101444  0x05E0  22 //Y Start address
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x46, WORD_LADDR(_gu32BL8Bit_CAddr));
//wriu -w 0x101446  0xFA00  23 //UV Start address
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x48, WORD_HADDR(_gu32BL8Bit_CAddr));
//wriu -w 0x101448  0x05E0  24 //UV Start address

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x24, 0x80, 0x00);
//wriu -b 0x101424  0x80    0x00 // vsync_inv = off
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x8e, 0x01, 0x00);

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x72, 0x60 , 0x60);
if(_gbIs10Bit_Input == TRUE)
{
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x73, 0x06 , 0x06);  // 10b enable
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x8e, 0x02, 0x00);
}
else
{
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x73, 0x06 , 0x00);
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x8e, 0x02, 0x02);
}
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x71, 0x40, 0x00);
//10b

//load bin Y/UV 2 bit
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x94, WORD_LADDR(_gu32BL2Bit_YAddr));
//wriu -w 0x101494 0xF400     4A//lsb start address Y
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x96, WORD_HADDR(_gu32BL2Bit_YAddr));
//wriu -w 0x101496 0x05E1     4B//lsb start address Y
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x98, WORD_LADDR(_gu32BL2Bit_CAddr));
//wriu -w 0x101498 0xEE00     4C//lsb start address C
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x9a, WORD_HADDR(_gu32BL2Bit_CAddr));
//wriu -w 0x10149A 0x05E2     4D//lsb start address C

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x84, 0x0); // no-crop
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x86, 0x0); // no-crop

    MsOS_DelayTask(100);
//wait 100

//swch 4
    _Demo_DolbyHDR_PreIDK_WriteReg8(0x1014, 0x41, ceil(((double)stFrmLayerInfo.stBL_Size.u16Width) / 32.0) * 4.0);
    _Demo_DolbyHDR_PreIDK_WriteReg8(0x1014, 0x40, 0x06);
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x30, 0x07, (MS_U16)(ceil(((double)stFrmLayerInfo.stBL_Size.u16Width) / 32.0) * 4.0) >> 8); //???STRIP

    _Demo_DolbyHDR_PreIDK_WriteReg8(0x1014, 0xA4, ceil(((double)stFrmLayerInfo.stBL_Size.u16Width) / 128.0) * 4.0);

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x50, 0x0180);
//wriu -w 0x101450  0x0180   28//Switch EVD tile mode + Switch MVD interface to MVD interface

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_LoadHDR10BitsDolbyEL(void)
{
    _Demo_DolbyHDR_PreIDK_WriteReg8(0x100B, 0xAF, 0x08); // clock_gen

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x30, 0x0F, 0x00); //strip???? bl=1 will cause garbage
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x22, 0x01, 0x01); // mvop_en
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x24, 0x80, 0x00); // vsync_inv = off

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x4a, stFrmLayerInfo.stEL_Size.u16Width); // h_size=360
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x4c, stFrmLayerInfo.stEL_Size.u16Height); // v_size=240

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x7c, 0x07, 0x07); // must do
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0xa6, 0x02, 0x02); // 2p mode
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x84, 0x0); // no-crop
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x86, 0x0); // no-crop
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x7e, 0x80, 0x00); //csc_en= off
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x7f, 0x01, 0x01); //422_en= on
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x4E, 0x0010);

//load bin Y/UV 8 bit
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x42, WORD_LADDR(_gu32EL8Bit_YAddr));
//wriu -w 0x101442  0x0000  21 //Y Start address (Word:8byte)
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x44, WORD_HADDR(_gu32EL8Bit_YAddr));
//wriu -w 0x101444  0x05E0  22 //Y Start address
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x46, WORD_LADDR(_gu32EL8Bit_CAddr));
//wriu -w 0x101446  0xFA00  23 //UV Start address
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x48, WORD_HADDR(_gu32EL8Bit_CAddr));
//wriu -w 0x101448  0x05E0  24 //UV Start address

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x8e, 0x01, 0x00);
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x72, 0x60 , 0x60);
if(_gbIs10Bit_Input == TRUE)
{
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x73, 0x06, 0x06); // 10b enable
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x8e, 0x02, 0x00);
}
else
{
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x73, 0x06, 0x00); // 10b enable
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x8e, 0x02, 0x02);
}
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x71, 0x40, 0x00); // also for 10b

//load bin Y/UV 2 bit
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x94, WORD_LADDR(_gu32EL2Bit_YAddr));
//wriu -w 0x101494 0xF400     4A//lsb start address Y
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x96, WORD_HADDR(_gu32EL2Bit_YAddr));
//wriu -w 0x101496 0x05E1     4B//0x103d start address Y
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x98, WORD_LADDR(_gu32EL2Bit_CAddr));
//wriu -w 0x101498 0xEE00     4C//lsb start address C
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x9a, WORD_HADDR(_gu32EL2Bit_CAddr));
//wriu -w 0x10149A 0x05E2     4D//lsb start address C

    _Demo_DolbyHDR_PreIDK_WriteReg8(0x103d, 0x41, ceil(((double)stFrmLayerInfo.stEL_Size.u16Width) / 32.0) * 4.0);
    _Demo_DolbyHDR_PreIDK_WriteReg8(0x103d, 0x40, 0x06);
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x103d, 0x30, 0x07, (MS_U16)(ceil(((double)stFrmLayerInfo.stEL_Size.u16Width) / 32.0) * 4.0) >> 8); //???STRIP

    _Demo_DolbyHDR_PreIDK_WriteReg8(0x103d, 0xA4, ceil(((double)stFrmLayerInfo.stEL_Size.u16Width) / 128.0) * 4.0);

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x50, 0x0180);

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_12BitsBypass2VOP(void)
{
//-- 12 bits bypass to vop
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x00, 0x0050);
//wriu -w 0x102f00 0x0050
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x04, 0x0035);
//wriu -w 0x102f04 0x0035
// [0] reg_hdr_din_en_f1       enable f1
// [1] reg_hdr_din_en_f2       enable f2
// [2] reg_ip2_hdr_dat_sel_f1  0:sub   1:main
// [3] reg_ip2_hdr_dat_sel_f2  0:main  1:sub
// [4] reg_hdr2dc0_ack_en
// [5] reg_hdr2dc1_ack_en
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x00, 0x0000);
//wriu -w 0x102f00 0x0000
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x80, 0x2300);
//wriu -w 0x102f80 0x2300
// [9] reg_mvop2vip_en
// [8] reg_mvop2vop_en
// [13] reg_mvop2vop_vs_op
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x82, 0x0001);
//wriu -w 0x102f82 0x0001
// [0] reg_vop_12b_path_en

    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x00, 0x0002);
//wriu -w 0x102f00 0x0002
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x54, 0x0300);
//wriu -w 0x102f54 0x0300
// [8] reg_yhsl_src_sel    0: select HDR
// [9] reg_hdr2ip_path_en  1: select HDR
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x00, 0x0050);
//wriu -w 0x102f00 0x0050
// Bank50
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x06, 0x0001);
//wriu -w 0x102f06 0x0001
// [0]:reg_vtk_bypass

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_MVOPExtra(void)
{
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x24, 0xa210);
//wriu -w 0x101424 0xa210  //[4]UV_swap_en
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x1014, 0x7e, 0xc100);
//wriu -w 0x10147e 0xc100  //[7]csc_en, [8]422_en
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x7b, BIT(7), BIT(7)); //for BW
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1014, 0x2a, BIT(2), BIT(2)); //for BW


    if(_gbEL_Enable == TRUE)
    {
        _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x24, 0xa210);
//wriu -w 0x103d24 0xa210  //[4]UV_swap_en
        _Demo_DolbyHDR_PreIDK_WriteReg16(0x103d, 0x7e, 0xc100);
//wriu -w 0x103d7e 0xc100  //[7]csc_en, [8]422_en
    }
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0x00, 0x007a);
//wriu -w 0x102f00 0x007a // vdo_fe
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102f, 0xf6, 0x0000);
//wriu -w 0x102ff6 0x0000 //[3:2]shift_el
                        //[1:0]shift_bl
    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_PreSetting(void)
{
    _Demo_DolbyHDR_PreIDK_WriteReg8(0x137a, 0x02, 0x00);
//wriu 0x137a02 0x00
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x137a, 0xe0, 0x0000);
//wriu -w 0x137ae0 0x0000

    if(_gbIs10Bit_Input != TRUE)
    {
        _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x137a, 0xf6, 0x04, 0x04); // if 8 bit
    }
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x137b, 0xe4, 0x01, 0x00); // bypass scramble
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x100a, 0x86, 0x07, 0x00); // clk

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_ADC_DMA_Trigger(void)
{
    MS_U32 u32StartTrigTime = 0;
    MS_U32 u32TrigTimeDiff = 0;

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102e, 0x87, 0x01, 0x00); //set START=0
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102e, 0x87, 0x80, 0x80); //set RESET=1

    u32StartTrigTime = MsOS_GetSystemTime();
    while((_Demo_DolbyHDR_PreIDK_ReadReg8(0x102e, 0x8c) & BIT(0)) != 0) //wait FINISH_FLAG=0
    {
        u32TrigTimeDiff = _Demo_DolbyHDR_PreIDK_GetTimeDiff(u32StartTrigTime);
        if (u32TrigTimeDiff > DMA_TRIGGER_RESET_TIMEOUT) // set timeout
        {
            printf("[%s][%d] Set Reset timeout !! \n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        MsOS_DelayTask(1000);
    }

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102e, 0x87, 0x80, 0x00); //set RESET=0
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102e, 0x87, 0x01, 0x01); //set START=1

    u32StartTrigTime = MsOS_GetSystemTime();
    while((_Demo_DolbyHDR_PreIDK_ReadReg8(0x102e, 0x8c) & BIT(0)) != 1) //wait FINISH_FLAG=1
    {
        u32TrigTimeDiff = _Demo_DolbyHDR_PreIDK_GetTimeDiff(u32StartTrigTime);
        if (u32TrigTimeDiff > DMA_TRIGGER_DUMP_TIMEOUT) // set timeout
        {
            printf("[%s][%d] DMA DUMP timeout !! \n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        MsOS_DelayTask(1000);
    }

    return TRUE;
}

static MS_BOOL _Demo_DolbyHDR_PreIDK_ADC_DMA(EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL hdrSel, EN_DOLBYHDR_PREIDK_REG_DBG_SEL dbgSel, EN_DOLBYHDR_PREIDK_REG_DBG_EVEN evenSel)
{
    MS_U16 u16PanelHeight = 0;
    u16PanelHeight = IPANEL(&stPNL_DeviceId ,Height);

// miu priority
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x3106, 0x48, 0x80, 0x00);
// scramble power saving
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x133f, 0x7e, 0x01, 0x01);

//reg_dma_st_addr
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102e, 0x80, ADC_LADDR(ADC_DMA_ADDR));
    _Demo_DolbyHDR_PreIDK_WriteReg16(0x102e, 0x82, ADC_HADDR(ADC_DMA_ADDR));

    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102e, 0x86, 0x1f, 0x10); //reg_dma_threshold
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102e, 0x86, 0x20, 0x20); //reg_dma_wpriority

//clear ADC buffer
    memset((MS_U8 *)MS_PA2KSEG1((MS_U32)ADC_DMA_ADDR), 0, ADC_DMA_SIZE);//dump buffer addr
    MsOS_Sync();
    MsOS_FlushMemory();

    switch(hdrSel)
    {
        case E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_FE:
            switch(u16PanelHeight)
            {
                case 480:
                    _gu32DumpSize = ADC_DUMP_FE_SD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_FE_SD_SIZE @@\n");
                    break;
                case 720:
                    _gu32DumpSize = ADC_DUMP_FE_HD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_FE_HD_SIZE @@\n");
                    break;
                case 1080:
                    _gu32DumpSize = ADC_DUMP_FE_FHD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_FE_FHD_SIZE @@\n");
                    break;
                case 2160:
                    _gu32DumpSize = ADC_DUMP_FE_4K_SIZE;
                    printf("@@ Set dump size ADC_DUMP_FE_4K_SIZE @@\n");
                    break;
                default:
                    break;
            }
            _Demo_DolbyHDR_PreIDK_WriteReg16(0x102e, 0x84, (MS_U16)(_gu32DumpSize/(256*32))); // size
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x100b, 0xaa, 0xFF, 0x0C);   //clock_gen  FE: 0x0C,OSD:0x08,BE:0x20
            _Demo_DolbyHDR_PreIDK_WriteReg8(0x102f, 0x00, 0x79); // subbank 79
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102f, 0x6a, 0x70, 0x10); // 0x10: FE, 0x20: BE, 0x40: OSD
            break;

        case E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_BE:
            switch(u16PanelHeight)
            {
                case 480:
                    _gu32DumpSize = ADC_DUMP_BE_SD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_BE_SD_SIZE @@\n");
                    break;
                case 720:
                    _gu32DumpSize = ADC_DUMP_BE_HD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_BE_HD_SIZE @@\n");
                    break;
                case 1080:
                    _gu32DumpSize = ADC_DUMP_BE_FHD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_BE_FHD_SIZE @@\n");
                    break;
                case 2160:
                    _gu32DumpSize = ADC_DUMP_BE_4K_SIZE;
                    printf("@@ Set dump size ADC_DUMP_BE_4K_SIZE @@\n");
                    break;
                default:
                    break;
            }
            _Demo_DolbyHDR_PreIDK_WriteReg16(0x102e, 0x84, (MS_U16)(_gu32DumpSize/(256*32))); // size
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x100b, 0xaa, 0xFF, 0x20);   //clock_gen  FE: 0x0C,OSD:0x08,BE:0x20
            _Demo_DolbyHDR_PreIDK_WriteReg8(0x102f, 0x00, 0x79); // subbank 79
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102f, 0x6a, 0x70, 0x20); // 0x10: FE, 0x20: BE, 0x40: OSD
            break;

        case E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_OSD:
            switch(u16PanelHeight)
            {
                case 480:
                    _gu32DumpSize = ADC_DUMP_BE_SD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_OSD_SD_SIZE @@\n");
                    break;
                case 720:
                    _gu32DumpSize = ADC_DUMP_BE_HD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_OSD_HD_SIZE @@\n");
                    break;
                case 1080:
                    _gu32DumpSize = ADC_DUMP_BE_FHD_SIZE;
                    printf("@@ Set dump size ADC_DUMP_OSD_FHD_SIZE @@\n");
                    break;
                case 2160:
                    _gu32DumpSize = ADC_DUMP_BE_4K_SIZE;
                    printf("@@ Set dump size ADC_DUMP_OSD_4K_SIZE @@\n");
                    break;
                default:
                    break;
            }
            _Demo_DolbyHDR_PreIDK_WriteReg16(0x102e, 0x84, (MS_U16)(_gu32DumpSize/(256*32))); // size
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x100b, 0xaa, 0xFF, 0x08);   //clock_gen  FE: 0x0C,OSD:0x08,BE:0x20
            _Demo_DolbyHDR_PreIDK_WriteReg8(0x102f, 0x00, 0x79); // subbank 79
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102f, 0x6a, 0x70, 0x40); // 0x10: FE, 0x20: BE, 0x40: OSD
            break;

        default:
            break;
    }

    _Demo_DolbyHDR_PreIDK_WriteReg8(0x102f, 0x00, 0x7b); // subbank 7b
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102f, 0x6a, 0x03, dbgSel); // 0x00: B0101_in,0x01: B0101_out,0x02: B0106_out 0x03: B02_out
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x102f, 0x6a, 0x04, (evenSel << 2)); // 0x04: even pixel


    if(_Demo_DolbyHDR_PreIDK_ADC_DMA_Trigger() != TRUE)
    {
        return FALSE;
    }

    if(_gbADL_Enable == TRUE)
    {
        _Demo_DolbyHDR_PreIDK_ADL_Disable();
    }

    printf("###[%s]Ready to dump dram!!###\n",__FUNCTION__);
    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_IDKMode(MS_U8 *u8Mode)
{
    switch(*u8Mode)
    {
        case 0:
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x40, 0x03, 0);
            break;
        case 1:
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x40, 0x03, 0x03);
            break;
        case 2:
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x40, 0x03, 0x02);
            break;
        default:
            printf("[%s] Invalid IDK mode, only supports for mode 0,1,2\n",__FUNCTION__);
            return FALSE;
    }

    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_ADC_Dump(MS_U8* pPath, EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL *hdrSel, EN_DOLBYHDR_PREIDK_REG_DBG_SEL *dbgSel, EN_DOLBYHDR_PREIDK_REG_DBG_EVEN *evenSel)
{
    MS_BOOL bRet = FALSE;

#if ADC_DMA_ENABLE && defined(ADC_DMA_ADR)
    MS_U32 u32DumpAddr = ADC_DMA_ADDR;

    if ((E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_NUM  <= *hdrSel) || (E_DOLBYHDR_PREIDK_REG_DBG_SEL_NUM  <= *dbgSel) || (E_DOLBYHDR_PREIDK_REG_DBG_EVEN_NUM  <= *evenSel))
    {
        printf("The REG_DBG_SEL setting error!!\n");
        return bRet;
    }

    _Demo_DolbyHDR_PreIDK_ADC_DMA(*hdrSel, *dbgSel, *evenSel);
    bRet = Demo_DolbyHDR_PreIDK_SaveDram2Bin(pPath, &u32DumpAddr, &_gu32DumpSize);
    printf("######[Done]####Dump dram [Size: %d] Finish !! ##############\n", _gu32DumpSize);
#endif

    return bRet;
}

MS_BOOL Demo_DolbyHDR_PreIDK_Capture2File(MS_U8* pPath, EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL *hdrSel, EN_DOLBYHDR_PREIDK_REG_DBG_SEL *dbgSel, MS_U8* u8OutDepth)
{
    MS_BOOL bRet = FALSE;

#if ADC_DMA_ENABLE && defined(ADC_DMA_ADR)
    EN_DOLBYHDR_PREIDK_REG_DBG_EVEN evenSel = 0;

    switch(*u8OutDepth)
    {
        case E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_8BIT:
        case E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_10BIT:
        case E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_12BIT:
            enOutDepth = *u8OutDepth;
            break;
        default:
            printf("[%s] Invalid BDP, only supports for 8/10/12 bits\n",__FUNCTION__);
            return bRet;
    }

    WIDTH = IPANEL(&stPNL_DeviceId ,Width);
    HEIGHT = IPANEL(&stPNL_DeviceId ,Height);

    FILE* fout = MsFS_Fopen((char*)pPath, "wb");

    if ((E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_BE == *hdrSel) && ((E_DOLBYHDR_PREIDK_REG_DBG_SEL_Scramble_OUT == *dbgSel) || (E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0406_OUT == *dbgSel)))
    {
        char strDump[FILE_NAME_LENGTH] = {'\0'};
        evenSel = E_DOLBYHDR_PREIDK_REG_DBG_EVEN_PIXEL;

        snprintf((char*)strDump, FILE_NAME_LENGTH-1, "%s_dump.bin", (char*)pPath);
        Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strDump, hdrSel, dbgSel, &evenSel);

        bRet = _Demo_DolbyHDR_PreIDK_Convert2VUV((MS_U8*)strDump, fout);
        MsFS_Fflush(fout);
    }
    else
    {
        char strEven[FILE_NAME_LENGTH] = {'\0'};
        char strOdd[FILE_NAME_LENGTH] = {'\0'};
        evenSel = E_DOLBYHDR_PREIDK_REG_DBG_EVEN_PIXEL;

        snprintf((char*)strEven, FILE_NAME_LENGTH-1, "%s_even.bin", (char*)pPath);
        Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strEven, hdrSel, dbgSel, &evenSel); //dump even pixel

        evenSel = E_DOLBYHDR_PREIDK_REG_DBG_ODD_PIXEL;
        snprintf((char*)strOdd, FILE_NAME_LENGTH-1, "%s_odd.bin", (char*)pPath);
        Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strOdd, hdrSel, dbgSel, &evenSel); //dump odd pixel

        bRet = _Demo_DolbyHDR_PreIDK_Convert2VUV_Multi((MS_U8*)strEven, (MS_U8*)strOdd, fout);
        MsFS_Fflush(fout);
    }

    MsFS_Fclose(fout);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

    return bRet;
}

MS_BOOL Demo_DolbyHDR_PreIDK_VerifyFrame(MS_U8* pPath, unsigned int* uIdx)
{
    char fout_name[FILE_NAME_LENGTH] = {'\0'};
    char strTemp[FILE_NAME_LENGTH] = {'\0'};
    unsigned int fileIdx = 0;

    _Demo_DolbyHDR_PreIDK_CearGlobalVar();
    if(TRUE != _Demo_DolbyHDR_PreIDK_PathParser((MS_U8*)pPath))
    {
        printf("[%s][ERR] It's not a folder path! \n",__FUNCTION__);
        return FALSE;
    }

    if(enOutDepth == E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_12BIT)
    {
        snprintf((char*)fout_name, FILE_NAME_LENGTH-1, "%sframes_422.ipt", (char*)strFrmOutputPath);
    }
    else
    {
        snprintf((char*)fout_name, FILE_NAME_LENGTH-1, "%sframes_422.yuv", (char*)strFrmOutputPath);
    }
    FILE* fout = MsFS_Fopen(fout_name, "wb");

    fileIdx = (*uIdx == MAX_FRAME_INDEX) ? 0 : *uIdx;
    do
    {
        EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL eHdrSel = E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_BE;
        EN_DOLBYHDR_PREIDK_REG_DBG_SEL eDbgSel = E_DOLBYHDR_PREIDK_REG_DBG_SEL_Scramble_OUT;
        EN_DOLBYHDR_PREIDK_REG_DBG_EVEN eEvenSel = E_DOLBYHDR_PREIDK_REG_DBG_EVEN_PIXEL;

        _Demo_DolbyHDR_PreIDK_FinalizeLastFrame();
        if( FALSE == Demo_DolbyHDR_PreIDK_LoadFrame((MS_U8*)stFrmLayerInfo.pu8BLPath, (MS_U8*)stFrmLayerInfo.pu8ELPath, &fileIdx, &_gbIs10Bit_Input))
        {
            printf("###[%s Finish]### No more frame file !!\n", __FUNCTION__);
            break;
        }

        _Demo_DolbyHDR_PreIDK_SetMode();
        _Demo_DolbyHDR_PreIDK_LoadHDR10BitsDolbyBL();
        if(_gbEL_Enable == TRUE)
        {
            _Demo_DolbyHDR_PreIDK_LoadHDR10BitsDolbyEL();
        }
        _Demo_DolbyHDR_PreIDK_12BitsBypass2VOP();
#if 1
        _Demo_DolbyHDR_PreIDK_MVOPExtra();
        _Demo_DolbyHDR_PreIDK_PreSetting();

        printf("######wait 1 sec##################\n");
        MsOS_DelayTask(1000);

#if GOP_ENABLE
        snprintf((char*)strFrmOSDPath, FILE_NAME_LENGTH-1, "%sosd/", (char*)strLaunchPath);
        if(FALSE != _Demo_DolbyHDR_PreIDK_OSD_FindBin((MS_U8*)strFrmOSDPath))
        {
            Demo_DolbyHDR_PreIDK_OSDBlending((MS_U8*)strFrmOSDPath);
        }
#endif

#if defined(XC_AUTODOWNLOAD_ADR) && (XC_AUTODOWNLOAD_LEN >= 0x38000)
        snprintf((char*)strTemp, FILE_NAME_LENGTH-1, "%s%03u.bin", (char*)strFrmScpPath, fileIdx);
        if(FALSE != _Demo_DolbyHDR_PreIDK_GetLUTArray((MS_U8*)strTemp))
        {
            _Demo_DolbyHDR_PreIDK_ADL_Enable();
        }
#endif

        printf("######wait 1 sec##################\n");
        MsOS_DelayTask(1000);

        snprintf((char*)strTemp, FILE_NAME_LENGTH-1, "%s%03u.scp", (char*)strFrmScpPath, fileIdx);
        printf("#####[%s]###\n",(char*)strTemp);

        Demo_DolbyHDR_PreIDK_RunRiuScripts((MS_U8*)strTemp);
        _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x137a, 0xf5, 0xc0, 0xc0);
        printf("######Wait 1 sec####Need wait for setting available##############\n");
        MsOS_DelayTask(1000);
#endif

#if ADC_DMA_ENABLE && defined(ADC_DMA_ADR)
        WIDTH=stFrmLayerInfo.stBL_Size.u16Width;
        HEIGHT=stFrmLayerInfo.stBL_Size.u16Height;
        snprintf((char*)strTemp, FILE_NAME_LENGTH-1, "%s%03u_even.bin", (char*)strFrmOutputPath, fileIdx);

        if (TRUE == _gbIsRGB_Output)
        {
            char strOdd[FILE_NAME_LENGTH] = {'\0'};
            eDbgSel = E_DOLBYHDR_PREIDK_REG_DBG_SEL_B0406_IN;

            Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strTemp, &eHdrSel, &eDbgSel, &eEvenSel); //dump even pixel

            eEvenSel = E_DOLBYHDR_PREIDK_REG_DBG_ODD_PIXEL;
            snprintf((char*)strOdd, FILE_NAME_LENGTH-1, "%s%03u_odd.bin", (char*)strFrmOutputPath, fileIdx);
            Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strOdd, &eHdrSel, &eDbgSel, &eEvenSel); //dump odd pixel

            _Demo_DolbyHDR_PreIDK_Convert2VUV_Multi((MS_U8*)strTemp, (MS_U8*)strOdd, fout);
            MsFS_Fflush(fout);
        }
        else
        {
            Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strTemp, &eHdrSel, &eDbgSel, &eEvenSel);

            _Demo_DolbyHDR_PreIDK_Convert2VUV((MS_U8*)strTemp, fout);
            MsFS_Fflush(fout);
        }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
        fileIdx++;
    }while(*uIdx == MAX_FRAME_INDEX);

    MsFS_Fclose(fout);

#if 0 //dump dbg
//////////////DBG///////////////////////////
DIR *dpDBG;
struct dirent *dirpDBG;
snprintf((char*)strTemp, FILE_NAME_LENGTH-1, "%s%s/dbg/", (char*)strLaunchPath, dirpMain->d_name);
printf("##[%d]##%s\n, ", __LINE__, (char*)strTemp);
if((dpDBG = MsFS_OpenDir((char*)strTemp)))
{
    while ((dirpDBG = MsFS_ReadDir(dpDBG)))
    {
        if((strcmp(dirpDBG->d_name, ".") != 0) && (strcmp(dirpDBG->d_name, "..") != 0)) //run cmd for each vector here
        {
            snprintf((char*)strTemp, FILE_NAME_LENGTH-1, "%s%s/dbg/", (char*)strLaunchPath, dirpMain->d_name);
            strncat((char*)strTemp, dirpDBG->d_name, FILE_NAME_LENGTH - strlen((char*)strTemp) - 1);
            printf("##[%d]##%s\n, ", __LINE__, (char*)strTemp);
            Demo_DolbyHDR_PreIDK_RunRiuScripts((MS_U8*)strTemp);

            if(_Demo_DolbyHDR_PreIDK_ADC_DMA_Trigger() != TRUE)
            {
                return FALSE;
            }
            strncat((char*)strTemp, ".bin", FILE_NAME_LENGTH - 4 - 1);
            printf("##[%d]##%s\n, ", __LINE__, (char*)strTemp);
            Demo_DolbyHDR_PreIDK_SaveDram2Bin(0x29bfd000, (50*1024*1024), (MS_U8*)strTemp);
            printf("######wait 1 sec####Starting dump dram##############\n");
            MsOS_DelayTask(1000);
        //_Demo_DolbyHDR_PreIDK_Convert2VUV((MS_U8*)strTemp);
        }
    }
    MsFS_CloseDir(dpDBG);
}

////////////////////////////////////////////////////////////////////////////
#endif

    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_Set_OutputType(MS_U8* pOutType)
{
    if (*pOutType > 2)
    {
        printf("[Error] Error HDR output type setting !! \n");
        return FALSE;
    }
    else
    {
        _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x3D, 0x30, (*pOutType<<4));
        return TRUE;
    }
}

MS_BOOL Demo_DolbyHDR_PreIDK_Set_VSVDB(MS_U8* pVsvdb)
{
    if (*pVsvdb > 4)
    {
        printf("[Error] Error VSVDB number setting !! \n");
        return FALSE;
    }
    else
    {
        _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x3D, 0x07, (*pVsvdb));
        return TRUE;
    }
}

MS_BOOL Demo_DolbyHDR_PreIDK_Set_GraphicPriority(MS_U8* pPriority)
{
    if (*pPriority > 2)
    {
        printf("[Error] Error priority setting !! \n");
        return FALSE;
    }
    else
    {
        _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x3C, 0x30, (*pPriority<<4));
        return TRUE;
    }
}

MS_BOOL Demo_DolbyHDR_PreIDK_Set_LowLatencyMode(MS_BOOL* pbLow)
{
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x3C, 0x02, (*pbLow)<<1);
    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_Set_DolbyMappingHDR10(MS_BOOL* pbDVMapHDR10)
{
    _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x1330, 0x3C, 0x01, (*pbDVMapHDR10));
    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_DumpMM(MS_U8* pPath, MS_U32* pResolution, MS_U8* pOutType, MS_U8* pVsvdb, MS_U8* pPriority, MS_BOOL* pbGop, MS_BOOL* pbLow, MS_BOOL* pbDVMapHDR10)
{
#if ADC_DMA_ENABLE && defined(ADC_DMA_ADR)
    char strEvenBin[FILE_NAME_LENGTH] = {'\0'};
    char strResult[FILE_NAME_LENGTH] = {'\0'};
    EN_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL hdrSel = E_DOLBYHDR_PREIDK_REG_DBG_HDR_SEL_BE;
    EN_DOLBYHDR_PREIDK_REG_DBG_SEL dbgSel = E_DOLBYHDR_PREIDK_REG_DBG_SEL_Scramble_OUT;
    EN_DOLBYHDR_PREIDK_REG_DBG_EVEN evenSel = E_DOLBYHDR_PREIDK_REG_DBG_EVEN_PIXEL;
    MS_U32 u32StepFrameCnt = 1;
    MS_U32 u32MmPlayFlag = 0x10;
    MS_U32 u32FrameCnt = 0;
    MS_U32 u32XCDevice = 0;
    MS_U32 u32OutputTiming = 0;
    MS_U32 bEnableGOP = 0;

    switch(*pResolution)
    {
        case 480:
            u32OutputTiming = E_OUTPUT_TIMING_720X480_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        case 720:
            u32OutputTiming = E_OUTPUT_TIMING_1280X720_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        case 1080:
            u32OutputTiming = E_OUTPUT_TIMING_1920X1080_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        case 2160:
            u32OutputTiming = E_OUTPUT_TIMING_3840X2160_60P;
            Demo_XC_SetOutputTiming(&u32XCDevice, &u32OutputTiming);
            break;
        default:
            printf("[Error] Error resolution setting !! \n");
            return FALSE;
    }

    Demo_DolbyHDR_PreIDK_Set_OutputType(pOutType);
    Demo_DolbyHDR_PreIDK_Set_VSVDB(pVsvdb);
    Demo_DolbyHDR_PreIDK_Set_GraphicPriority(pPriority);
    Demo_DolbyHDR_PreIDK_Set_LowLatencyMode(pbLow);
    Demo_DolbyHDR_PreIDK_Set_DolbyMappingHDR10(pbDVMapHDR10);

    if (TRUE != Demo_MM_StepPlay(&u32MmPlayFlag, pPath))
    {
        return FALSE;
    }

    snprintf((char*)strEvenBin, FILE_NAME_LENGTH-1, "%s_even.bin", (char*)pPath);
    if (*pOutType == 0)
    {
        snprintf((char*)strResult, FILE_NAME_LENGTH-1, "%s.ipt", (char*)pPath);
    }
    else
    {
        snprintf((char*)strResult, FILE_NAME_LENGTH-1, "%s.yuv", (char*)pPath);
    }
    FILE* fout = MsFS_Fopen((char*)strResult, "wb");

    enOutDepth = (*pOutType == 0) ? E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_12BIT : E_DOLBYHDR_PREIDK_OUTPUT_DEPTH_10BIT;
    WIDTH = IPANEL(&stPNL_DeviceId ,Width);
    HEIGHT = IPANEL(&stPNL_DeviceId ,Height);

    do
    {
        if (*pOutType != 0)
        {
            _Demo_DolbyHDR_PreIDK_WriteReg8_MASK(0x137b, 0x90, 0xe0, (u32FrameCnt%8) << 5);  //Setting for IDK dither ....May overWrite by DV driver...
        }

        printf("\033[1;36m[%s] Frame Count = %d !! \033[0m\n",__FUNCTION__,u32FrameCnt++);
        printf("######Wait 1sec for file sync########\n");
        MsOS_DelayTask(1000);

        Demo_DolbyHDR_PreIDK_ADC_Dump((MS_U8*)strEvenBin, &hdrSel, &dbgSel, &evenSel);
        _Demo_DolbyHDR_PreIDK_Convert2VUV((MS_U8*)strEvenBin, fout);
        MsFS_Fflush(fout);

    } while((TRUE == Demo_MM_Step(&u32StepFrameCnt)) || (1 == u32FrameCnt));

    if (*pbGop)
    {
        Demo_OSD_EnableWin(&bEnableGOP);
    }
    Demo_MM_Stop();
    MsFS_Fclose(fout);
#endif

    return TRUE;
}

MS_BOOL Demo_DolbyHDR_PreIDK_AutoVerify(MS_U8* pPath)
{
    DIR *dpMain;
    struct dirent *dirpMain;
    unsigned int uFrmIdx = (unsigned int)MAX_FRAME_INDEX;

    char strCurFrmLayerPath[FILE_NAME_LENGTH] = {'\0'};
    char strTemp[FILE_NAME_LENGTH] = {'\0'};

    if((dpMain = MsFS_OpenDir((char*)pPath)))
    {
        snprintf((char*)strTemp, FILE_NAME_LENGTH-1, (char*)pPath);
        CHECK_END_SLASH(strTemp);

        while ((dirpMain = MsFS_ReadDir(dpMain)))
        {
            if((dirpMain->d_type == 4) && (strcmp(dirpMain->d_name, ".") != 0) && (strcmp(dirpMain->d_name, "..") != 0)) //run cmd for each vector here
            {
                 snprintf((char*)strCurFrmLayerPath, FILE_NAME_LENGTH-1, "%s%s/", (char*)strTemp, dirpMain->d_name);
                 Demo_DolbyHDR_PreIDK_VerifyFrame((MS_U8*)strCurFrmLayerPath, &uFrmIdx);
            }
        }
        MsFS_CloseDir(dpMain);
        return TRUE;
    }

    printf("[%s][ERR] It's not a folder path! \n",__FUNCTION__);
    return FALSE;
}

#endif
