////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    verJPD.h
/// @brief  for JPD verification
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VER_JPD_H_
#define _VER_JPD_H_
#include <sys/common/MsTypes.h>
//#include <display_logo/MsCommon.h>

#define TEST_02_Baseline1600x1200 1
#define TEST_03_Baseline352x512 2
#define TEST_04_BaselineYUV400  3
#define TEST_05_BaselineYUV411  4
#define TEST_06_BaselineYUV420  5
#define TEST_07_BaselineYUV422  6
#define TEST_08_BaselineYUV440  7
#define TEST_09_BaselineYUV444  8
#define TEST_RST_3Q             9
#define TEST_10_OtherTest       10

#define BASELINE    TEST_07_BaselineYUV422


void verJPD_Main(MS_U32 u32SrcAddr, MS_U32 u32SrcSize);
int JPG_GetAlignmentWidth(void);
int JPG_GetAlignmentHeight(void);
int JPG_GetAlignmentPitch(void);
MS_U32 JPG_GetOutRawDataAddr(void);

#endif /* _VER_JPD_H_ */

