/******************************************************
 *              Includes                              *
 ******************************************************/

#include <stdio.h>

#include "crules.h"
#include "tkel.h"
#include "tbox.h"
#include "tdal_fla.h"
#include "tdal_common.h"

#include "testhelper.h"
#include "ssutests.h"

/******************************************************
 *              SSU test tree                         *
 ******************************************************/

static tTestNode gTestUGetPlatformInfo = {
        "TestUGetPlatformInfo",
        "Test fetching platform information",
        0, /* not interactive */
        SSU_TestUnitary_GetPlatformInfo,
        NULL
};

static tTestNode gTestUGetStatusInfo = {
        "TestUGetStatusInfo",
        "Test fetching SSU status information",
        0, /* not interactive */
        SSU_TestUnitary_GetStatusInfo,
        NULL
};

tTestNode* gTDAL_SSU_UnitaryTestNodeList[] = {
    &gTestUGetPlatformInfo,
    &gTestUGetStatusInfo,
    NULL
};

tTestNode gTDAL_SSU_UnitaryTestSuite = {
    "TDAL_SSU Unitary Tests",
    "test the SSU module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_SSU_UnitaryTestNodeList
};

LOCAL void SSU_TestUnitary_GetPlatformInfo(void)
{
    tTDAL_SSU_PlatformInfo  ssuPltfInfo = {0};
    tTDAL_SSU_Error         ssuError    = eTDAL_SSU_NO_ERROR;

    ssuError = TDAL_SSU_GetPlatformInfo(&ssuPltfInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "================== TDAL_SSU_GetPlatformInfo =======================\n"));
        mTBOX_PRINT((kTBOX_LF, "uiOUI               = 0x%08X\n", ssuPltfInfo.uiOUI));
        mTBOX_PRINT((kTBOX_LF, "cHardwareModel      = %s\n", ssuPltfInfo.cHardwareModel));
        mTBOX_PRINT((kTBOX_LF, "uiHardwareModel     = %4d\n", ssuPltfInfo.uiHardwareModel));
        mTBOX_PRINT((kTBOX_LF, "uiHardwareVersion   = 0x%04X\n", ssuPltfInfo.uiHardwareVersion));
        mTBOX_PRINT((kTBOX_LF, "cProductModel       = %s\n", ssuPltfInfo.cProductModel));
        mTBOX_PRINT((kTBOX_LF, "uiProductModel      = %4d\n", ssuPltfInfo.uiProductModel));
        mTBOX_PRINT((kTBOX_LF, "uiProductVersion    = 0x%04X\n", ssuPltfInfo.uiProductVersion));
        mTBOX_PRINT((kTBOX_LF, "cDeviceBrandName    = %s\n", ssuPltfInfo.cDeviceBrandName));
        mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));
    }

    TestManager_AssertEqual(ssuError, eTDAL_SSU_NO_ERROR, "TDAL_SSU_GetPlatformInfo");
}

LOCAL void SSU_TestUnitary_GetStatusInfo(void)
{
    tTDAL_SSU_StatusInfo    ssuStatusInfo = {0};
    tTDAL_SSU_Error         ssuError      = eTDAL_SSU_NO_ERROR;

    if (TDAL_FLA_Init() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Init() failed!\n"));
        TestManager_AddFailure();
        return;
    }

    if (TDAL_FLA_Open() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Open() failed!\n"));
        TestManager_AddFailure();
        return;
    }

    ssuError = TDAL_SSU_GetStatusInfo(&ssuStatusInfo);
    if (ssuError == eTDAL_SSU_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "================== TDAL_SSU_GetStatusInfo =========================\n"));
        mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->bInStandBy            = 0x%02X\n", ssuStatusInfo.bInStandBy));
        mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->eErrorStatus          = 0x%02X\n", ssuStatusInfo.eErrorStatus));
        mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->uiLastUpdateID        = 0x%08X\n", ssuStatusInfo.uiLastUpdateID));
        mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->cErrorCode            = %s\n", ssuStatusInfo.cErrorCode));
        mTBOX_PRINT((kTBOX_LF, "pstSsuStatus->cUtcTime              = [0x%02X][0x%02X][0x%02X][0x%02X][0x%02X]\n",
                ssuStatusInfo.cUtcTime[0], ssuStatusInfo.cUtcTime[1], ssuStatusInfo.cUtcTime[2], ssuStatusInfo.cUtcTime[3], ssuStatusInfo.cUtcTime[4]));
        mTBOX_PRINT((kTBOX_LF, "===================================================================\n"));
    }

    TestManager_AssertEqual(ssuError, eTDAL_SSU_NO_ERROR, "TDAL_SSU_GetStatusInfo");

    if (TDAL_FLA_Close() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Close() failed!\n"));
        TestManager_AddFailure();
        return;
    }

    if (TDAL_FLA_Term() != eTDAL_FLA_NO_ERROR)
    {
        mTBOX_PRINT((kTBOX_LF, "TDAL_FLA_Term() failed!\n"));
        TestManager_AddFailure();
        return;
    }
}
