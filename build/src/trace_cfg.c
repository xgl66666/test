/********************************************************************************
 *                       COPYRIGHT 2006 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: TRACE CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/trace_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "tbox.h"
#include "tkel.h"
#include "tdal_common.h"

/*===========================================================================
 *
 * TRACE_CFG_Init
 *
 * Parameters:
 *      none
 *
 * Description:
 *
 *
 * Returns:
 *
 *
 *===========================================================================*/
void TRACE_CFG_Init(void)
{
    mTBOX_INIT_MOD_TRACE((eTDAL_COMMON,   kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTKEL_COM,      kTBOX_TRC_NONE));
    mTBOX_INIT_MOD_TRACE((eTKEL_MEMORY,   kTBOX_TRC_NONE));
    mTBOX_INIT_MOD_TRACE((eTKEL_TASK,     kTBOX_TRC_NONE));
    mTBOX_INIT_MOD_TRACE((eTKEL_TIMER,    kTBOX_TRC_NONE));
    mTBOX_INIT_MOD_TRACE((eTKEL_SYNC,     kTBOX_TRC_NONE));
    mTBOX_INIT_MOD_TRACE((eTKEL_MISC,     kTBOX_TRC_NONE));

    mTBOX_INIT_MOD_TRACE((eTDAL_ATA,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_AV,       kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_AVS,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_CRYPT,    kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_DESC,     kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_DMD,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_DMX,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_FLA,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_FP,       kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_FS,       kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_GFX,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_HDMI,     kTBOX_TRC_CRITICAL));
#ifdef PRODUCT_USE_CI
    mTBOX_INIT_MOD_TRACE((eTDAL_CI,       kTBOX_TRC_CRITICAL));
#endif
#ifdef PRODUCT_USE_TDAL_RPC
    mTBOX_INIT_MOD_TRACE((eTDAL_RPC,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_RPC_COMM, kTBOX_TRC_CRITICAL));
#endif
    mTBOX_INIT_MOD_TRACE((eTDAL_KBD,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_NVM,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_PTSM,     kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_SMC,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_SEC,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TS_ROUTE, kTBOX_TRC_CRITICAL|kTBOX_TRC_WARNING));
    mTBOX_INIT_MOD_TRACE((eTDAL_TS_SINK,  kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_TS_SOURCE,kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_UART,     kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_VBI,      kTBOX_TRC_CRITICAL));
    mTBOX_INIT_MOD_TRACE((eTDAL_DISP,     (kTBOX_TRC_CRITICAL|kTBOX_TRC_WARNING)));
    mTBOX_INIT_MOD_TRACE((eTDAL_OUTPUT,   (kTBOX_TRC_CRITICAL|kTBOX_TRC_WARNING)));
    mTBOX_INIT_MOD_TRACE((eTDAL_PERSO,    (kTBOX_TRC_CRITICAL|kTBOX_TRC_WARNING)));
    mTBOX_INIT_MOD_TRACE((eTDAL_MP,       (kTBOX_TRC_CRITICAL|kTBOX_TRC_WARNING)));
    mTBOX_INIT_MOD_TRACE((eTDAL_PVR_EM,   (kTBOX_TRC_CRITICAL|kTBOX_TRC_WARNING)));
#define TRIGGER 0x04280001
    mTBOX_INIT_MOD_TRACE((TRIGGER,      kTBOX_TRC_ALL));
#if defined(USE_TDAL_SSU)
    mTBOX_INIT_MOD_TRACE((eTDAL_SSU,   kTBOX_TRC_CRITICAL));
#endif
    mTBOX_TRACE_ENABLED((TRUE));
    mTBOX_TRACE_LOC_ENABLED((FALSE));
    mTBOX_TRACE_TIME_ENABLED((FALSE));
    mTBOX_TRACE_SYNCHRONE((TRUE));
}

