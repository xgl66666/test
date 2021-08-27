/*
 * tdal_mp_status.c
 *
 *  Created on: 5 Feb 2013
 *      Author: mijovic
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "tbox.h"

#include "tdal_common.h"

#include "tdal_mp.h"
#include "tdal_mp_p.h"

mTBOX_SET_MODULE(eTDAL_MP);

tTDAL_MP_Error TDAL_MP_StatusGet(tTDAL_MP_Handle Handle, tTDAL_MP_Status *pstStatus)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;

    return result;
}


tTDAL_MP_Error TDAL_MP_LengthGet(tTDAL_MP_Handle Handle, uint32_t *puiSec)
{
    unsigned int r;

    mTBOX_FCT_ENTER("TDAL_MP_LengthGet");

    if (puiSec == NULL || Handle == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_LengthGet] Bad params\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    TDAL_MPi_LockMutex();

    r = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_TOTAL_TIME_ISVALID);

    if (!r)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_LengthGet] Length is not valid\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }

    r = MApi_MPlayer_GetOption(TDAL_MPm_eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_TOTAL_TIME);

    if (r == 0)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_LengthGet] Length is zero\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }

    *puiSec = r;

    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(eTDAL_MP_NO_ERROR);
}
