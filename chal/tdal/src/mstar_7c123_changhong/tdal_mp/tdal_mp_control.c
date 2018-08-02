/*
 * tdal_mp_control.c
 *
 *  Created on: 5 Feb 2013
 *      Author: mijovic
 */

/********************************************************/
/*              Includes                                */
/********************************************************/
#include "tdal_mp.h"
#include "tdal_mp_p.h"
#include "tdal_common.h"
#include "crules.h"
#include "MsTypes.h"
#include <sys/stat.h>
#include <pthread.h>
#include <assert.h>
#include "stdio.h"

#include "tbox.h"
/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Macros                                  */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_MP);

/********************************************************/
/*              Typedefs                                */
/********************************************************/

/********************************************************/
/*              Local Module Variables (MODULE)         */
/********************************************************/

/********************************************************/
/*              Global Variables (GLOBAL/IMPORT)        */
/********************************************************/
IMPORT tTDAL_MPm_Desc *TDAL_MPi_DescList[kTDAL_MPm_OPEN_SESSION_MAX];
/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/


/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/********************************************************/


/******************************************************************************
 * Function Name : TDAL_MP_Start
 *
 * Description   : This function starts the rendering of a stream media.
 *
 * Return        : It returns eTDAL_MP_NO_ERROR if succeed.
 *                 It returns eTDAL_MP_NOT_INIT_ERROR if a previous initialization
 *                 has not been done.
 *                 It returns eTDAL_MP_BAD_PARAMETER_ERROR if input parameters
 *                 are bad.
 *                 eTDAL_MP_NOT_DONE_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_MP_Error TDAL_MP_Start(tTDAL_MP_Handle Handle, tTDAL_MP_StartParams *pstParams)
{
    mTBOX_FCT_ENTER("TDAL_MP_Start");
    
    tTDAL_MPm_Desc *pstDesc = NULL;
    if(!is_TDAL_MP_Initialized())
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Start: not initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_INIT_ERROR);
    }

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Start: handle is NULL\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    pstDesc = (tTDAL_MPm_Desc *)Handle;

    if (!TDAL_MPm_Player_PlayMediaFile())
    {
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }

    if (pstDesc->pEvtNotify)
    {
        pstDesc->pEvtNotify(Handle, eTDAL_MP_EVENT_PLAYED, pstDesc->pCtx);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "pstDesc->pEvtNotify is NULL\n"));
    }
    mTBOX_RETURN(eTDAL_MP_NO_ERROR);
}

/******************************************************************************
 * Function Name : TDAL_MP_Stop
 *
 * Description   : This function stops the rendering of a media.
 *
 * Return        : It returns eTDAL_MP_NO_ERROR if succeed.
 *                 It returns eTDAL_MP_NOT_INIT_ERROR if a previous initialization
 *                 has not been done.
 *                 It returns eTDAL_MP_BAD_PARAMETER_ERROR if input parameters
 *                 are bad.
 *                 eTDAL_MP_NOT_DONE_ERROR otherwise.
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_MP_Error TDAL_MP_Stop(tTDAL_MP_Handle Handle)
{
    tTDAL_MPm_Desc *pstDesc;

    mTBOX_FCT_ENTER("TDAL_MP_Stop");

    if (!is_TDAL_MP_Initialized())
	{
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Stop: not initialized\n"));
        mTBOX_RETURN(eTDAL_MP_NOT_INIT_ERROR);
	}

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_MP_Stop: handle is NULL\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    pstDesc = (tTDAL_MPm_Desc *)Handle;

    TDAL_MPi_LockMutex();

    if(TDAL_MPm_Stop() != false)
    {
        TDAL_MPi_UnlockMutex();
        MsOS_DelayTask(100);
        TDAL_MPi_LockMutex();

        //SetMIU_G1MaxServiceNum(0x5);

        if (pstDesc->pEvtNotify)
        {
            pstDesc->pEvtNotify(Handle, eTDAL_MP_EVENT_STOPPED, pstDesc->pCtx);
        }
        else
        {    
            mTBOX_TRACE((kTBOX_NIV_WARNING, "pstDesc->pEvtNotify is NULL\n")); 
        }
        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_MP_Stop: Stream stopped\n"));
        TDAL_MPi_UnlockMutex();

        mTBOX_RETURN(eTDAL_MP_NO_ERROR);
    }

    //SetMIU_G1MaxServiceNum(0x5);

    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not stop MM\n"));
    TDAL_MPi_UnlockMutex();
    
    mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
}

tTDAL_MP_Error TDAL_MP_PlayCtrlModeSet(tTDAL_MP_Handle Handle, uint32_t uiMode)
{
	tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
	return result;
}

tTDAL_MP_Error TDAL_MP_PosSet(tTDAL_MP_Handle Handle, uint32_t uiSec)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    MS_U32 u32Value = 0;
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    tTDAL_MPm_Desc * desc = NULL;
    int index = 0;
    mTBOX_FCT_ENTER("TDAL_MP_PosSet");

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PosGet] Bad param\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR); 
    }
    TDAL_MPi_LockMutex();
    
    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PosGet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }

    _eMediaType = TDAL_MPm_Player_GetMediaType();    
    u32Value = 0;
    
    if (result == eTDAL_MP_NO_ERROR)
    {    
        if(_eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            u32Value = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MOVIE_INFO, E_MMSDK_INFO_TOTAL_TIME);
            if((u32Value > 0) && (uiSec*1000 > u32Value))
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "uiSec time exceeds file length (=%d ms)\n",(int)u32Value));
                result = eTDAL_MP_BAD_PARAMETER_ERROR;
            }
        }
        else if(_eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
        {
            u32Value = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MUSIC_INFO, E_MMSDK_INFO_TOTAL_TIME);
            if((u32Value > 0) && (uiSec > u32Value))
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "uiSec time exceeds file length (=%d s)\n", (int)u32Value));
                result = eTDAL_MP_BAD_PARAMETER_ERROR;
            }
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "Not support PosSet at Photo mode!\n"));
            result = eTDAL_MP_BAD_PARAMETER_ERROR;
        }
    }
    
    if (result == eTDAL_MP_NO_ERROR)
    {
        if (!TDAL_MPm_Player_SetPosition(_eMediaType, uiSec*1000))
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, " fail to set position.\n"));
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
    }
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN (result);
}

tTDAL_MP_Error TDAL_MP_PosGet(tTDAL_MP_Handle Handle, uint32_t *puiSec)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    tTDAL_MPm_Desc * desc;
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    MS_U32 u32Value = 0;
    int index = 0;
    mTBOX_FCT_ENTER("TDAL_MP_PosGet");

    if (Handle == (tTDAL_MP_Handle) NULL || puiSec == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PosGet] Bad param\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR); 
    }

    TDAL_MPi_LockMutex();

    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_PosGet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }
    
    _eMediaType = TDAL_MPm_Player_GetMediaType();
    u32Value = 0;
    if (_eMediaType != E_MMSDK_MEDIA_TYPE_MOVIE && _eMediaType != E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Not support PosGet at Photo mode!\n"));
        result = eTDAL_MP_BAD_PARAMETER_ERROR;        
    }
    if (result == eTDAL_MP_NO_ERROR)
    {
        if (!TDAL_MPm_Player_GetPosition(_eMediaType, &u32Value))
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, " fail to get position.\n"));
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
        else
        {            
            *puiSec = u32Value / 1000;
        }
    }

    TDAL_MPi_UnlockMutex();

    mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_SpeedSet(tTDAL_MP_Handle Handle, int32_t uiSpeed)
{
    int32_t index;
    tTDAL_MPm_Desc * desc;
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    EN_MMSDK_TRICK_MODE eTrickMode;

    mTBOX_FCT_ENTER("TDAL_MP_SpeedSet");

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedSet] Handle not valid or speed not valid\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    TDAL_MPi_LockMutex();

    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();
    
    if (index < 0 || desc != TDAL_MPi_DescList[index])
    {   
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedSet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
     }
    
    _eMediaType = TDAL_MPm_Player_GetMediaType();
    if (_eMediaType != E_MMSDK_MEDIA_TYPE_MOVIE && _eMediaType != E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING,"Not support PosGet at Photo mode!\n"));
        result = eTDAL_MP_BAD_PARAMETER_ERROR;
    }
    
    switch(uiSpeed)
    {
    case 0:
        eTrickMode = E_MMSDK_TRICK_PAUSE;
        break;
    case 25:
        eTrickMode = E_MMSDK_TRICK_SF_4X;
        break;
    case 50:
        eTrickMode = E_MMSDK_TRICK_SF_2X;
        break;
    case 100:
        eTrickMode = E_MMSDK_TRICK_NORMAL;
        break;
    case 200:
        eTrickMode = E_MMSDK_TRICK_FF_2X;
        break;
    case 400:
        eTrickMode = E_MMSDK_TRICK_FF_4X;
        break;
    case 800:
        eTrickMode = E_MMSDK_TRICK_FF_8X;
        break;
    case 1600:
        eTrickMode = E_MMSDK_TRICK_FF_16X;
        break;	
    case 3200:
        eTrickMode = E_MMSDK_TRICK_FF_32X;
        break;
    case -100:
    case -200:
        eTrickMode = E_MMSDK_TRICK_FB_2X;
        break;
    case -400:
        eTrickMode = E_MMSDK_TRICK_FB_4X;
        break;
    case -800:
        eTrickMode = E_MMSDK_TRICK_FB_8X;
        break;
    case -1600:
        eTrickMode = E_MMSDK_TRICK_FB_16X;
        break;
    case -3200:
        eTrickMode = E_MMSDK_TRICK_FB_32X;
        break;
    default:
        result = eTDAL_MP_NOT_SUPPORTED_ERROR;
        mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_MP_SpeedSet] Unsupported trick mode %d. Skipping\n", uiSpeed));
        break;
    }

    if (result == eTDAL_MP_NO_ERROR)
    {
        if(!TDAL_MPm_Player_SetSpeed(_eMediaType, eTrickMode))
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, " fail to set trick mode.\n"));
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_1, "set trick mode success speed=%d.\n",uiSpeed));
        }
    }
    TDAL_MPi_UnlockMutex();

    mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_SpeedGet(tTDAL_MP_Handle Handle, int32_t *puiSpeed)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;    
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    tTDAL_MPm_Desc * desc;
    int index;
    mTBOX_FCT_ENTER(TDAL_MP_SpeedGet);

    if (Handle == (tTDAL_MP_Handle) NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedGet] Handle not valid or speed not valid\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    if (puiSpeed == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedGet] puiSpeed NULL\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR);
    }

    TDAL_MPi_LockMutex();

    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_SpeedGet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR);
    }
    
    _eMediaType = TDAL_MPm_Player_GetMediaType();
    if (_eMediaType != E_MMSDK_MEDIA_TYPE_MOVIE && _eMediaType != E_MMSDK_MEDIA_TYPE_MUSIC)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Not support PosGet at Photo mode!\n"));
        result = eTDAL_MP_BAD_PARAMETER_ERROR;        
    }
    if (result == eTDAL_MP_NO_ERROR)
    {
        if (!TDAL_MPm_Player_GetSpeed(_eMediaType, puiSpeed))
        {
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
    }
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}

