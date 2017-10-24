/*****************************************************************************
 *                         COPYRIGHT 2006 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: PVR MODULE
 *
 * FILE NAME:   PVR_CFG.C
 *   $URL: http://10.50.1.219/svn/products/st5100_ref/tags/pesi_pvr/2008_04_28_v1.36/build/src/pvr_cfg.c $
 *   $Rev: 47452 $
 *   $Date: 2008-01-14 18:29:49 +0100 (lun., 14 janv. 2008) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/*****************************************************************************
 *  Includes                                                                 *
 *****************************************************************************/
/********************************************************/
#include <string.h>

#include "crules.h"
#include "pvr_cfg.h"

/*****************************************************************************
 *  Defines                                                                  *
 *****************************************************************************/
/* values are in MegaBytes */
#define kPVR_CFG_MINIMUM_SIZE_FOR_FULL_MODE             512
#define kPVR_CFG_MINIMUM_SIZE_FOR_TIMESHIFT_ONLY_MODE   256
#define kPVR_CFG_MAXIMUM_SIZE_FOR_HDD_SPEED_WARNING     4*1024


/*****************************************************************************
 *  Macros                                                                   *
 *****************************************************************************/
/*****************************************************************************
 *  Structures                                                               *
 *****************************************************************************/
 
typedef struct
{
    tPVR_CFG_FileType_Id     Ident;
    char                                 FileExtension[4];
} tPVR_CFG_FileType;
/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                                         *
 *****************************************************************************/
#if defined(PACKAGE_PVR_INTERNAL)
char *  PVR_CFG_DirectoryName = "PVRi";
#else
char *  PVR_CFG_DirectoryName = "";
#endif



#define kPVR_CFG_FILETYPE_NB  	     9
tPVR_CFG_FileType	PVR_CFG_FileTypeList[kPVR_CFG_FILETYPE_NB] =
{
    {ePVR_CFG_FILETYPE_MPEG2TS,"trp"},          /*Warning : only for first part of whole media*/
                                                                        /*   others : first letter then 01, 02 ...            */
    {ePVR_CFG_FILETYPE_PROPRIETARY_METADATA,"mda"},
    {ePVR_CFG_FILETYPE_PROPRIETARY_THUMBNAIL,"tmb"},
    {ePVR_CFG_FILETYPE_PROPRIETARY_TEMPORARY_METADATA,"tmp"},
    {ePVR_CFG_FILETYPE_PROPRIETARY_MISC,"msc"},
    {ePVR_CFG_FILETYPE_PROPRIETARY_TIMEINDEXING,"idx"},
    //{ePVR_CFG_FILETYPE_PROPRIETARY_IFRAMEINDEXING, "fid"},
    
    {ePVR_CFG_FILETYPE_PLAYLIST_WMP,"wpl"},
    {ePVR_CFG_FILETYPE_PLAYLIST_MPEG,"m3u"},
    {ePVR_CFG_FILETYPE_PLAYLIST_WINAMP,"pls"},
};



#define kPVR_CFG_PLAYLIST_FILETYPE_NB              2
tPVR_CFG_FileType_Id	PVR_CFG_PLAYLIST_FileTypeList[kPVR_CFG_PLAYLIST_FILETYPE_NB] =
{
    ePVR_CFG_FILETYPE_PLAYLIST_WMP,
    ePVR_CFG_FILETYPE_PLAYLIST_MPEG
};


/*****************************************************************************
 *  Local File Variables (LOCAL)                                             *
 *****************************************************************************/
/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                                     *
 *****************************************************************************/
/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                                     *
 *****************************************************************************/
/*****************************************************************************
 *  Functions Definitions (LOCAL/GLOBAL)                                     *
 *****************************************************************************/
bool PVR_CFG_GotoLiveAfterEndOfPlayback(void)
{
	return TRUE;
}

bool PVR_CFG_GotoRecLiveAfterEndOfChasingPlayback(void)
{
    return FALSE;
}

uint32_t		PVR_CFG_GetAutomaticBookMarkDuration(void)
{
	return 5;
}

bool    PVR_CFG_DisplayDiskSpaceInTime(void)
{
  #ifdef PRODUCT_PVR_FREEVIEW_PLAYBACK
    return TRUE;
  #else
    return FALSE;
  #endif
}

tPVR_CFG_VolumeChoicePolicy     PVR_CFG_GetVolumeChoicePolicy_ForUse( void )
{
	/* Add this user choice in order to select SD memory card always */
#if 0
	return ePVR_CFG_VOLUME_CHOICE_POLICY_USER_CHOICE;
#else
    return ePVR_CFG_VOLUME_CHOICE_POLICY_FIRST;
#endif
}

tPVR_CFG_VolumeChoicePolicy     PVR_CFG_GetVolumeChoicePolicy_ForActivation( void )
{
	/* Add this user choice in order to select SD memory card always */
#if 0
	return ePVR_CFG_VOLUME_CHOICE_POLICY_USER_CHOICE;
#else
    return ePVR_CFG_VOLUME_CHOICE_POLICY_BIGGEST;
#endif
}

tPVR_CFG_DeviceChoicePolicy     PVR_CFG_GetDeviceChoicePolicy_ForActivation( void )
{
	/* Add this user choice in order to select SD memory card always */
#if 0
	return ePVR_CFG_VOLUME_CHOICE_POLICY_USER_CHOICE;
#else
    return ePVR_CFG_DEVICE_CHOICE_POLICY_BIGGEST;
#endif
}

uint32_t        PVR_CFG_GetMinimumSizeForFullMode( void )
{
    return kPVR_CFG_MINIMUM_SIZE_FOR_FULL_MODE; /* in MegaBytes */
}


uint32_t        PVR_CFG_GetMinimumSizeForTimeshiftOnlyMode( void )
{
    return kPVR_CFG_MINIMUM_SIZE_FOR_TIMESHIFT_ONLY_MODE; /* in MegaBytes */
}


char *          PVR_CFG_GetDirectoryName( void )
{
    return PVR_CFG_DirectoryName;
}


tPVR_CFG_ActivationPolicy       PVR_CFG_GetActivationPolicyForFullMode( void )
{
    return ePVR_CFG_ACTIVATION_POLICY_MANUAL;
}


tPVR_CFG_ActivationPolicy       PVR_CFG_GetActivationPolicyForTimeshiftOnlyMode( void )
{
    return ePVR_CFG_ACTIVATION_POLICY_MANUAL;
}


uint32_t        PVR_CFG_GetMaximumSizeForHDDSpeedWarning( void )
{
    return kPVR_CFG_MAXIMUM_SIZE_FOR_HDD_SPEED_WARNING; /* in MegaBytes */
}



/*===========================================================================
 *
 * PVR_CFG_does_PlayKey_start_LastPlayedMedia
 *
 * Parameters:
 *      None
 *
 * Description:
 *      In viewing mode, when the current service is a live service (not a Timeshift, not a record, not a playback),
 *              does 'Play' key start the last played media ?
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool        PVR_CFG_does_PlayKey_start_LastPlayedMedia(void)
{
    return FALSE;
}


/*===========================================================================
 *
 * PVR_CFG_does_OKKey_same_PlayKey
 *
 * Parameters:
 *      None
 *
 * Description:
 *      In viewing mode, when playback running , 'ok' key does the same as 'Play' key :
 *              -unpause, reset speed to 1X, ...
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool        PVR_CFG_does_OKKey_same_PlayKey(void)
{
    return FALSE;
}




/*===========================================================================
 *
 * PVR_CFG_FileType_ExtensionGet
 *
 * Parameters:
 *      tPVR_CFG_FileType_Id : id of the file extension asked
 *
 * Description:
 *
 * Returns:
 *
 *
 *===========================================================================*/
char *PVR_CFG_FileType_ExtensionGet(tPVR_CFG_FileType_Id Id)
{
    uint8_t     index;
    
    for ( index=0 ; index< kPVR_CFG_FILETYPE_NB ; index++)
    {
        if( Id == PVR_CFG_FileTypeList[index].Ident )
        {
            return PVR_CFG_FileTypeList[index].FileExtension;
        }
    }
    
    return NULL;
}
  
/*===========================================================================
 *
 * PVR_CFG_FileType_IdGet
 *
 * Parameters:
 *      Extension : Extension of the file
 *
 * Description:
 *
 * Returns:
 *
 *
 *===========================================================================*/
tPVR_CFG_FileType_Id PVR_CFG_FileType_IdGet(char *Extension)
{
    uint8_t     index;
    char         cFileExtLower[5];
    
    UT_ASCIIStringToLowerCase(Extension, cFileExtLower, 4);
    cFileExtLower[4] = '\0';
    
    for ( index=0 ; index< kPVR_CFG_FILETYPE_NB ; index++)
    {
        if( strcmp(cFileExtLower, PVR_CFG_FileTypeList[index].FileExtension) == 0 )
        {
            return PVR_CFG_FileTypeList[index].Ident;
        }
    }
    
    return ePVR_CFG_FILTYPE_UNKNOWN;
}



/*===========================================================================
 *
 * PVR_CFG_PLAYLIST_ListGet
 *
 * Parameters:
 *
 * Description: returns the list of supported Playlist File type
 *
 * Returns:
 *
 *
 *===========================================================================*/
bool PVR_CFG_PLAYLIST_ListGet(uint8_t *puiFileTypeNb, tPVR_CFG_FileType_Id **pstFileTypeList)
{
    *puiFileTypeNb = kPVR_CFG_PLAYLIST_FILETYPE_NB;
    *pstFileTypeList = PVR_CFG_PLAYLIST_FileTypeList;
    
    return TRUE;
}


/*===========================================================================
 *
 * PVR_CFG_ForwardBackward_GetNextSpeed
 *
 * Parameters:
 *
 * Description: returns the next speed in given direction regarding to current speed
 *
 * Returns:
 *
 *
 *===========================================================================*/
tPVR_CFG_Speed PVR_CFG_ForwardBackward_GetNextSpeed(tPVR_CFG_Speed currentSpeed, tPVR_CFG_Direction direction)
{
	switch( direction )
	{
	case ePVR_CFG_Forward:
	{
		switch ( currentSpeed )
		{
		case ePVR_CFG_SPEED_64X_BACKWARD:
			return ePVR_CFG_SPEED_32X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_32X_BACKWARD:
			return ePVR_CFG_SPEED_16X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_16X_BACKWARD:
			return ePVR_CFG_SPEED_8X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_8X_BACKWARD:
			return ePVR_CFG_SPEED_4X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_4X_BACKWARD:
			return ePVR_CFG_SPEED_2X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_2X_BACKWARD:
			return ePVR_CFG_SPEED_1X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_1X_BACKWARD:
			return ePVR_CFG_SPEED_0X5_BACKWARD;
			break;
		case ePVR_CFG_SPEED_0X5_BACKWARD:
			return ePVR_CFG_SPEED_0X5_FORWARD;
			break;
		case ePVR_CFG_SPEED_0X25_BACKWARD:
			return ePVR_CFG_SPEED_0X25_FORWARD;
			break;
		case ePVR_CFG_SPEED_0X:
			return ePVR_CFG_SPEED_1X_FORWARD;
			break;
		case ePVR_CFG_SPEED_0X25_FORWARD:
			return ePVR_CFG_SPEED_0X5_FORWARD;
			break;
		case ePVR_CFG_SPEED_0X5_FORWARD:
			return ePVR_CFG_SPEED_1X_FORWARD;
			break;
		case ePVR_CFG_SPEED_1X_FORWARD:
			return ePVR_CFG_SPEED_2X_FORWARD;
			break;
		case ePVR_CFG_SPEED_2X_FORWARD:
			return ePVR_CFG_SPEED_4X_FORWARD;
			break;
		case ePVR_CFG_SPEED_4X_FORWARD:
			return ePVR_CFG_SPEED_8X_FORWARD;
			break;
		case ePVR_CFG_SPEED_8X_FORWARD:
			return ePVR_CFG_SPEED_16X_FORWARD;
			break;
		case ePVR_CFG_SPEED_16X_FORWARD:
			return ePVR_CFG_SPEED_32X_FORWARD;
			break;
		case ePVR_CFG_SPEED_32X_FORWARD:
			return ePVR_CFG_SPEED_64X_FORWARD;
			break;
		case ePVR_CFG_SPEED_64X_FORWARD:
			return ePVR_CFG_SPEED_64X_FORWARD;
			break;
		}
	}
	case ePVR_CFG_Backward:
	{
		switch( currentSpeed )
		{
		case ePVR_CFG_SPEED_64X_BACKWARD:
			return ePVR_CFG_SPEED_64X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_32X_BACKWARD:
			return ePVR_CFG_SPEED_64X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_16X_BACKWARD:
			return ePVR_CFG_SPEED_32X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_8X_BACKWARD:
			return ePVR_CFG_SPEED_16X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_4X_BACKWARD:
			return ePVR_CFG_SPEED_8X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_2X_BACKWARD:
			return ePVR_CFG_SPEED_4X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_1X_BACKWARD:
			return ePVR_CFG_SPEED_2X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_0X5_BACKWARD:
			return ePVR_CFG_SPEED_1X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_0X25_BACKWARD:
			return ePVR_CFG_SPEED_0X5_BACKWARD;
			break;
		case ePVR_CFG_SPEED_0X:
			return ePVR_CFG_SPEED_1X_BACKWARD;
			break;
		case ePVR_CFG_SPEED_0X25_FORWARD:
			return ePVR_CFG_SPEED_0X25_BACKWARD;
			break;
		case ePVR_CFG_SPEED_0X5_FORWARD:
			return ePVR_CFG_SPEED_0X5_BACKWARD;
			break;
		case ePVR_CFG_SPEED_1X_FORWARD:
			return ePVR_CFG_SPEED_0X5_FORWARD;
			break;
		case ePVR_CFG_SPEED_2X_FORWARD:
			return ePVR_CFG_SPEED_1X_FORWARD;
			break;
		case ePVR_CFG_SPEED_4X_FORWARD:
			return ePVR_CFG_SPEED_2X_FORWARD;
			break;
		case ePVR_CFG_SPEED_8X_FORWARD:
			return ePVR_CFG_SPEED_4X_FORWARD;
			break;
		case ePVR_CFG_SPEED_16X_FORWARD:
			return ePVR_CFG_SPEED_8X_FORWARD;
			break;
		case ePVR_CFG_SPEED_32X_FORWARD:
			return ePVR_CFG_SPEED_16X_FORWARD;
			break;
		case ePVR_CFG_SPEED_64X_FORWARD:
			return ePVR_CFG_SPEED_32X_FORWARD;
			break;
		}
	}
	}
}

