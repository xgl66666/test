////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#include <common.h>
#include <sys/common/MsTypes.h>

#include <device/Tuner.h>
#if (FRONTEND_TUNER_TYPE == NUTUNE_FT2125_TUNER)
#include "drvTuner_NuTune_FT2125.c"
#elif (FRONTEND_TUNER_TYPE == NUTUNE_FJ2207_TUNER)
#include "drvTuner_NuTune_FJ2207.c"
#include "FJ2207/tmbslNT220x/src/tmbslNT220x.c"
#include "FJ2207/tmbslNT220x/src/tmbslNT220xInstance.c"
#include "FJ2207/tmddNT220x/src/tmddNT220x.c"
#include "FJ2207/tmddNT220x/src/tmddNT220x_Advanced.c"
#include "FJ2207/tmddNT220x/src/tmddNT220xInstance.c"
#elif (FRONTEND_TUNER_TYPE == DIBCOM_DIB8096_TUNER)
#include "drvTuner_Dibcom_DIB8096.c"
#elif (FRONTEND_TUNER_TYPE == NXP_CD1616LF_TUNER)
#include "drvTuner_NXP_CD1616LF.c"
#elif (FRONTEND_TUNER_TYPE == NXP_TD1611ALF_TUNER)
#include "drvTuner_NXP_TD1611ALF.c"
#elif (FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER)
#include "drvTuner_MXL5007T.c"
#elif (FRONTEND_TUNER_TYPE == TDVG_H252F_TUNER)
#include "drvTuner_TDVG_H252F.c"
#elif (FRONTEND_TUNER_TYPE == TUNER_MXL201RF)
#include "drvTuner_MXL201RF.c"
#endif

