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
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"
#include "apiGFX.h"

mTBOX_SET_MODULE(eTDAL_MP);

static uint32_t u32Pitch = 0;
static uint16_t u16Format = 0;

tTDAL_MP_Error TDAL_MP_StatusGet(tTDAL_MP_Handle Handle, tTDAL_MP_Status *pstStatus)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    tTDAL_MPm_Desc * desc = NULL;
    int index = 0;
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    MS_U32 u32Ret = 0;
    mTBOX_FCT_ENTER("TDAL_MP_StatusGet");

    if (Handle == (tTDAL_MP_Handle) NULL || pstStatus == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_StatusGet] Bad param\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR); 
    }

    TDAL_MPi_LockMutex();
    
    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();
    
    if (TDAL_MPi_CheckMediaDescriptor(desc) == FALSE)  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_StatusGet] Invalid tTDAL_MP_Handle\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR); 
    }

    _eMediaType = TDAL_MPm_Player_GetMediaType();
    
    if (result == eTDAL_MP_NO_ERROR)
    {    
        if(_eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MOVIE_INFO, E_MMSDK_INFO_PLAYER_STATE);
        }
        else if(_eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
        {
            u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MUSIC_INFO, E_MMSDK_INFO_PLAYER_STATE);
        }
        else
        {
            u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_PHOTO_INFO, E_MMSDK_INFO_PLAYER_STATE);
        }
    }
        
    printf("E_MMSDK_INFO_PLAYER_STATE=%lx\n",u32Ret);
    switch(u32Ret)
    {
        case E_MMSDK_PLAYER_STATE_PLAY_START:
        case E_MMSDK_PLAYER_STATE_PLAYING:
            *pstStatus = eTDAL_MP_STATUS_PLAY;
            break;
        case E_MMSDK_PLAYER_STATE_STOP_START:
        case E_MMSDK_PLAYER_STATE_STOPPED:
        case E_MMSDK_PLAYER_STATE_NULL:
            if (desc->bIsPlaying)
            {
                *pstStatus = eTDAL_MP_STATUS_FINISHED;
            }
            else
            {
                *pstStatus = eTDAL_MP_STATUS_STOP;
            }
            break;
        default :
            *pstStatus = eTDAL_MP_STATUS_UNKNOWN;
            break;
            
    }
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_LengthGet(tTDAL_MP_Handle Handle, uint32_t *puiSec)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    tTDAL_MPm_Desc * desc = NULL;
    int index = 0;
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    MS_U32 u32Ret = 0;
    mTBOX_FCT_ENTER("TDAL_MP_LengthGet");

    if (Handle == (tTDAL_MP_Handle) NULL || puiSec == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_LengthGet] Bad param\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR); 
    }

    TDAL_MPi_LockMutex();
    
    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_LengthGet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }

    _eMediaType = TDAL_MPm_Player_GetMediaType();
    u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MOVIE_INFO, E_MMSDK_INFO_PLAYER_STATE);
    
    if (result == eTDAL_MP_NO_ERROR)
    {    
        if(_eMediaType == E_MMSDK_MEDIA_TYPE_MOVIE)
        {
            u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MOVIE_INFO, E_MMSDK_INFO_PLAYER_STATE);
        }
        else if(_eMediaType == E_MMSDK_MEDIA_TYPE_MUSIC)
        {
            u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_MUSIC_INFO, E_MMSDK_INFO_PLAYER_STATE);
        }
        else
        {
            printf("Not support to get length at Photo mode!\n");
            result = eTDAL_MP_NOT_SUPPORTED_ERROR;
        }
    }

    if (result == eTDAL_MP_NO_ERROR)
    {
        if (u32Ret == 0)
        {
            result = eTDAL_MP_NOT_DONE_ERROR;
        }
        else
        {
            *puiSec = u32Ret/1000;
        }
    }
    
    TDAL_MPi_UnlockMutex();

    mTBOX_RETURN(result);
}

tTDAL_MP_Error TDAL_MP_PhotoSizeGet(tTDAL_MP_Handle Handle,
									uint32_t *pusWidth,
									uint32_t *pusHeight)
{
    tTDAL_MP_Error result = eTDAL_MP_NO_ERROR;
    tTDAL_MPm_Desc * desc = NULL;
    int index = 0;
    EN_MMSDK_MEDIA_TYPE _eMediaType;
    MS_U32 u32Ret = 0;
    uint32_t u32Heigh = 0;
    uint32_t u32Width = 0;
    
    mTBOX_FCT_ENTER("TDAL_MP_PhotoSizeGet");
    
    if (Handle == (tTDAL_MP_Handle) NULL || pusWidth == NULL || pusHeight == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_LengthGet] Bad param\n"));
        mTBOX_RETURN(eTDAL_MP_BAD_PARAMETER_ERROR); 
    }

    TDAL_MPi_LockMutex();
    
    desc = (tTDAL_MPm_Desc *) Handle;
    index = TDAL_MPi_GetRunningMediaDescriptor();

    if (index < 0 || desc != TDAL_MPi_DescList[index])  
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_MP_LengthGet] No running MM or given handle not playing\n"));
        TDAL_MPi_UnlockMutex();
        mTBOX_RETURN(eTDAL_MP_NOT_DONE_ERROR); 
    }
#if 0 // wait porting

    unsigned int u32Height = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, (unsigned int)MPLAYER_INFO_PHOTO_DECODED_HEIGHT);
    unsigned int u32Width = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_PHOTO, MPLAYER_OPTION_GET_PHOTO_INFO, (unsigned int)MPLAYER_INFO_PHOTO_DECODED_WIDTH);

    *pusWidth = u32Width;
    *pusHeight = u32Height;
#endif

    _eMediaType = TDAL_MPm_Player_GetMediaType();

    u32Ret = 0;
    u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_PHOTO_INFO, E_MMSDK_INFO_PHOTO_OUTPUT_WIDTH);
    u32Width = u32Ret;
    
    u32Ret = 0;
    u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_PHOTO_INFO, E_MMSDK_INFO_PHOTO_OUTPUT_HEIGHT);
    u32Heigh = u32Ret;

    u32Ret = 0;
    u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_PHOTO_INFO, E_MMSDK_INFO_PHOTO_OUTPUT_PITCH);
    u32Pitch = u32Ret;    

    u32Ret = 0;
    u32Ret = TDAL_MPm_Player_GetOption(_eMediaType, E_MMSDK_OPTION_PHOTO_INFO, E_MMSDK_INFO_PHOTO_OUTPUT_COLOR_FORMAT);
    
    switch(u32Ret)
    {
        case E_MMSDK_COLOR_FORMAT_YUV422:
            u16Format = GFX_FMT_YUV422;
            break;
        case E_MMSDK_COLOR_FORMAT_ARGB8888:
            u16Format = GFX_FMT_ARGB8888;
            break;
        case E_MMSDK_COLOR_FORMAT_ARGB1555:
            u16Format = GFX_FMT_ARGB1555;
            break;
        default:
            u16Format = 0;
            break;
    }
    
    printf("Photo Width=%lu Height=%lu Pitch=%lu Format=%d\n",u32Width,u32Heigh,u32Pitch,u16Format);
    
    *pusWidth = u32Width;
    *pusHeight = u32Heigh;
    TDAL_MPi_UnlockMutex();
    mTBOX_RETURN(result);
}
tTDAL_MP_Error TDAL_MP_PhotoGetOutAdr(tTDAL_MP_Handle Handle,
									uint32_t *pusAddr)
{
    mTBOX_FCT_ENTER("TDAL_MP_PhotoGetOutAdr");
#if (MM_PHOTO_PATH == 0x2)
    *pusAddr = MsOS_PA2KSEG0((MEM_ADR_BY_MIU(JPD_OUT_ADR, JPD_OUT_MEMORY_TYPE) + _gu32GifAddrOffset));
#endif
    mTBOX_RETURN(eTDAL_MP_NO_ERROR);
}
uint32_t TDAL_MPm_GetPhotoPitch(void)
{
    return u32Pitch;
}
uint16_t TDAL_MPm_GetPhotoFormat(void)
{
    return u16Format;
}