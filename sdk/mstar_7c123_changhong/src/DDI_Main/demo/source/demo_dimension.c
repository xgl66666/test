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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_Dimension.c
/// @author MStar Semiconductor Inc.
/// @brief  Uranus Sample Code Template
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_Dimension.c
   @brief The Template code for the Dimension_Transform functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_DTE_TEST == 1)
#include "string.h"

#include "MsCommon.h"
#include "MsMemory.h"
#include "msAPI_DTE.h"
#include "demo_osd.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

#define DE_WIDTH 960
#define DE_HEIGHT 540
#define DE_BPP 2

extern MS_S32 gs32NonCachedPoolID;
MS_U32 u32VirYUVAddr = 0;

//------------------------------------------------------------------------------
/// @brief The sample code to set config to 2D to 3D
/// @param[in] Gain The gain value
/// @param[in] Offset The offset index
/// @param[in] Hblocksize The block size of horizontal
/// @param[in] Vblocksize The block size of vertical
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b Dim_SetConfig \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Dimension_SetConfig(MS_U32 *Gain, MS_U32 *Offset, MS_U32 *Hblocksize, MS_U32 *Vblocksize)
{
    //Gain - Determine depth of pixel, default is 32
    if(0 != *Gain)
    {
        msAPI_DTE_SetConfig(E_MSAPI_DTE_GAIN,*Gain);
    }
    //Offset - Determine red/blue color left/right shift length, default is 128
    if(0 != *Offset)
    {
        msAPI_DTE_SetConfig(E_MSAPI_DTE_OFFSET,*Offset);
    }
    //Hblocksize - Our 2d to 3d method use block base to calculate depth. This parameter decide horizontal size of block.
    if(0 != *Hblocksize)
    {
        msAPI_DTE_SetConfig(E_MSAPI_DTE_H_BLOCK_SIZE,*Hblocksize);
    }
    //Vblocksize - Our 2d to 3d method use block base to calculate depth. This parameter decide vertical size of block.
    if(0 != *Vblocksize)
    {
        msAPI_DTE_SetConfig(E_MSAPI_DTE_V_BLOCK_SIZE,*Vblocksize);
    }
    return TRUE;
}

#define DIMENSION_RESERVE_SPACE 8
//------------------------------------------------------------------------------
/// @brief The sample code to excute 2D to 3D
/// @param[in] FilePath Pattern file path
/// @param[in] Width The width of pattern //Width Must Align 2 pixel
/// @param[in] Height The height of pattern
/// @param[in] type The color format of pattern: 0 is YUV422, 1 is ARGB
/// @return TRUE:  Success
/// @return FALSE: Fail
/// @note
/// Command: \b Dim_2dto3d \n
//------------------------------------------------------------------------------
MS_BOOL Demo_Dimension_2Dto3D(char* FilePath, const MS_U32* Width, const MS_U32* Height, const MS_U32* type)
{
    //printf("Path:%s, (w,h) = (%d,%d), type = %d\n",FilePath,*Width,*Height,*type);
    FILE* pf = fopen(FilePath,"rb");
    if(NULL == pf)
    {
        printf("Can't open file %s\n",FilePath);
        return FALSE;
    }

    if((*Width % 2) != 0)
    {
        printf("Width must align 2 for 2D to 3D\n");
        fclose(pf);
        return FALSE;
    }
    //Attention:
    //This method need reserve memory size = (4 * Width * Height) + (4 * Width * Height) + ( 2 * ((Width/BLOCK_WIDTH)+1) * ((Height/BLOCK_HEIGHT)+1) )
    //where BLOCK_HEIGHT/BLOCK_WIDTH is set by appDemo_Dimension_SetConfig.
    //For example: BLOCK_HEIGHT and BLOCK_WIDTH = 32, Width = 960, Height = 540. memory size need = (4 * 960 * 540) + (4 * 960 * 540) + ( 2 * ((960/32)+1) * ((540/32)+1) ) = 4148254 byte
    MS_U32 u32ARGBAddr = 0x0;
    MS_U32 u32ARGBAddr_3D = 0x0;
    MS_U32 u32ARGBAddr_3D_Align = 0x0;
    MS_U32 u32Width = *Width;
    MS_U32 u32Height = *Height;
    MS_U32 u32patternSize = u32Width * u32Height;
    MS_U32 u32OperateAddr = 0x0;
    MS_U32 u32OperateSize = 0x0;

    if(0 == *type)
    {
        //Pattern is YUV format
        //YUV422 need memory size = width * height * 2
        MS_U32 u32YUVAddr = (MS_U32)MsOS_AllocateMemory(u32patternSize*2,gs32NonCachedPoolID);
        if(0 == u32YUVAddr)
        {
            printf("Alloc YUV Address Fail\n");
            fclose(pf);
            return FALSE;
        }
        //Read pattern from file
        fread((void *)u32YUVAddr,u32patternSize*2,1,pf);
        //ARGB8888 need memory size = width * height * 4
        u32ARGBAddr = (MS_U32)MsOS_AllocateMemory(u32patternSize*4,gs32NonCachedPoolID);
        if(0 == u32ARGBAddr)
        {
            printf("Alloc ARGB Address Fail\n");
            MsOS_FreeMemory((void *)((MS_U32*)u32YUVAddr),gs32NonCachedPoolID);
            fclose(pf);
            return FALSE;
        }
        //Because 2d to 3d source buffer must use ARGB8888 color format
        //Color space convert from YUV422 to ARGB8888
        msAPI_DTE_CSC_YUVtoARGB(u32YUVAddr,u32ARGBAddr,u32Width,u32Height,E_MSAPI_DTE_YVYU);
        MsOS_FreeMemory((void *)((MS_U32*)u32YUVAddr),gs32NonCachedPoolID);
    }
    else if(1 == *type)
    {
        //Pattern is ARGB format
        //ARGB8888 need memory size = width * height * 4
        u32ARGBAddr = (MS_U32)MsOS_AllocateMemory(u32patternSize*4,gs32NonCachedPoolID);
        if(0 == u32ARGBAddr)
        {
            printf("Alloc ARGB Address Fail\n");
            fclose(pf);
            return FALSE;
        }
        //Read pattern from file
        fread((void *)u32ARGBAddr,u32patternSize*4,1,pf);
    }
    else
    {
        printf("Not support this type now\n");
        fclose(pf);
        return FALSE;
    }
    //Output color format is ARGB8888
    //ARGB8888 need memory size = width * height * 4
    u32ARGBAddr_3D = (MS_U32)MsOS_AllocateMemory((u32patternSize*4) + DIMENSION_RESERVE_SPACE,gs32NonCachedPoolID);
    if(0 == u32ARGBAddr_3D)
    {
        printf("Alloc 3D ARGB Address Fail\n");
        MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr),gs32NonCachedPoolID);
        fclose(pf);
        return FALSE;
    }
    u32ARGBAddr_3D_Align = (u32ARGBAddr_3D + (DIMENSION_RESERVE_SPACE - 1)) & ~(DIMENSION_RESERVE_SPACE - 1);
    //Calculate how many memory size when 2Dto3D operating using.
    msAPI_DTE_QueryOperateSize(u32Width,u32Height,&u32OperateSize);
    //Allocate memory for 2Dto3D operating using
    u32OperateAddr = (MS_U32)MsOS_AllocateMemory(u32OperateSize,gs32NonCachedPoolID);
    if(0 == u32OperateAddr)
    {
        printf("Alloc operate Address Fail\n");
        MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr),gs32NonCachedPoolID);
        MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr_3D),gs32NonCachedPoolID);
        fclose(pf);
        return FALSE;
    }
    //Excute 2d to 3d
    if(E_MSAPI_DTE_OK != msAPI_DTE_StillImage_2Dto3D(u32ARGBAddr,u32ARGBAddr_3D_Align,u32Width,u32Height,u32OperateAddr,u32OperateSize))
    {
        printf("Excute 2d to 3d Fail\n");
        MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr),gs32NonCachedPoolID);
        MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr_3D),gs32NonCachedPoolID);
        MsOS_FreeMemory((void *)((MS_U32*)u32OperateAddr),gs32NonCachedPoolID);
        fclose(pf);
        return FALSE;
    }

    MS_U32 u32PhyAddr_3D = MsOS_VA2PA(u32ARGBAddr_3D_Align);
    //draw 3d picture by GE
    Demo_OSD_Draw3D(&u32PhyAddr_3D,&u32Width,&u32Height);

    MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr),gs32NonCachedPoolID);
    MsOS_FreeMemory((void *)((MS_U32*)u32ARGBAddr_3D),gs32NonCachedPoolID);
    MsOS_FreeMemory((void *)((MS_U32*)u32OperateAddr),gs32NonCachedPoolID);
    fclose(pf);

    return TRUE;
}

MS_BOOL Demo_Dimension_Help(void)
{
    printf ("------------------------------------Dimension Help--------------------------------------\n");
    printf ("**Please excute GE_Init, GE_Setconfig, GE_Createwin first.\n");
    printf ("**If need help for GE, please press HelpGE.\n");
    printf ("1.Dim_SetConfig [Gain] [Offset] [Hblocksize] [Vblocksize]\n");
    printf ("2.Dim_2dto3d [FilePath] [Width] [Height] [type]\n");
    printf ("-----type : The color format of pattern\n");
    printf ("----------0: YUV 422\n");
    printf ("----------1: ARGB 8888\n");
    printf ("---------------------------------End of Dimension Help----------------------------------\n");

    return TRUE;
}
#endif

