/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: MEDIA VIEWER CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia/IDC3000/build/src/media_viewer_cfg.c $
 *            $Rev: 79 $
 *            $Date: 2011-08-02 14:26:19 +0200 (Tue, 02 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include <string.h>
#include <ctype.h>

#include "media_viewer_cfg.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define kMEDIA_VIEWER_CFGi_FILE_EXT_LENGTH_MAX 5
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

LOCAL tMEDIA_VIEWER_CFG_MediaType prefered_pictuer_order[4] = { eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_BMP, eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_JPEG, eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_GIF, eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_PNG };
LOCAL tMEDIA_VIEWER_CFG_MediaType prefered_audio_order[1] = { eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_MP3 };
LOCAL tMEDIA_VIEWER_CFG_MediaType prefered_video_order[1] = { eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_AVI };

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/*===========================================================================
 *
 * MEDIA_VIEWER_CFG_SupportedMediaTypeGet
 *
 * Parameters:
 *      none
 *
 *Description:
 *      none
 *
 * Returns:
 *      none
 *
 *===========================================================================*/
bool MEDIA_VIEWER_CFG_SupportedMediaTypeGet(uint32_t *puiMediaTypeMask,
                                                                 uint32_t *puiPictureMask,
                                                                 uint32_t *puiAudioMask,
                                                                 uint32_t *puiAudioVideoMask)
{
    if ( puiMediaTypeMask == NULL ||
        puiPictureMask == NULL ||
        puiAudioMask == NULL ||
        puiAudioVideoMask == NULL )
    {
        return false;
    }


    *puiMediaTypeMask = 0;
    *puiPictureMask = 0;
    *puiAudioMask = 0;
    *puiAudioVideoMask = 0;


#ifdef PRODUCT_USE_SDEC
    *puiMediaTypeMask |= eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT;
    *puiPictureMask |= eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_JPEG | eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_PNG | eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_BMP;
#endif

#ifdef PRODUCT_USE_RDR_MP3
    *puiMediaTypeMask |= eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO;
    *puiAudioMask |= eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_MP3;
#endif

#ifdef PRODUCT_USE_AVP
    *puiMediaTypeMask |= eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV;
    *puiAudioVideoMask = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_AVI;
#endif

    return (true);
}


/*===========================================================================
 *
 * MEDIA_VIEWER_CFG_DisplayFileExtension
 *
 * Parameters:
 *      none
 *
 * Description:
 *      none
 *
 * Returns:
 *      none
 *
 *===========================================================================*/

bool MEDIA_VIEWER_CFG_DisplayFileExtension(void)
{
    return (false);
}

/******************************************************************************
 * Function Name    : UT_ASCIIStringToLowerCase
 * Description      : Converts each characters in the string to lower case.
 * Side effects     : No verification on string validity.
 * Comment          : Copy not more than Length bytes (bytes that follow a null
 *                    byte are not copied). If Src is longer than Length bytes,
 *                    Dest is not terminated by '\0'.
 *                    If Src is shorter than Length bytes, '\0' are appended
 *                    to Dest until Length bytes are written
 *
 * Inputs           : Src    : String to convert.
 *                    Dest   : Result.
 *                    Length : Number of char to convert.
 *
 * Outputs          : None.
 *****************************************************************************/
void MEDIA_ASCIIStringToLowerCase(register char *Src,
                                  register char *Dest,
                                  uint16_t       Length)
{
    uint16_t    i = 0;

    if ((Src != NULL) && (Dest != NULL))
    {
        memset(Dest, 0, Length);

        while ((i < Length) && (*Src != '\0'))
        {
            if (isupper(*Src))
            {
                /* upper case letter */
                *Dest = (char) tolower(*Src);
            }
            else
            {
                /* lower case letter or not a letter */
                *Dest = *Src;
            }
            i++;
            Src++;
            Dest++;
        }
    }
    return;
}

/*===========================================================================
 *
 * MEDIA_VIEWER_CFG_MediaTypeFromFileGet
 *
 * Parameters:
 *      none
 *
 * Description:
 *      none
 *
 * Returns:
 *      none
 *
 *===========================================================================*/

bool MEDIA_VIEWER_CFG_MediaTypeFromFileGet(    char                    *pcFilename,
                                                                            tMEDIA_VIEWER_CFG_MediaType *peType)
{
    bool                        result=true;

    char                      *pcFileExt;
    char                       cFileExtLower[kMEDIA_VIEWER_CFGi_FILE_EXT_LENGTH_MAX+1];


    *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_UNKNOWN;

    pcFileExt = strrchr((const char*)pcFilename,'.');
    if ( pcFileExt != NULL )
    {
        MEDIA_ASCIIStringToLowerCase(pcFileExt, cFileExtLower, kMEDIA_VIEWER_CFGi_FILE_EXT_LENGTH_MAX);
        cFileExtLower[kMEDIA_VIEWER_CFGi_FILE_EXT_LENGTH_MAX] = '\0';

        /*----------------------------------------------------*/
        /*              PICTURE                                                                 */
        /*----------------------------------------------------*/
        if ( ( strcmp( cFileExtLower, ".jpg" ) == 0 ) ||
                ( strcmp( cFileExtLower, ".jpeg" ) == 0 ) ||
                ( strcmp( cFileExtLower, ".jpe" ) == 0 )           )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_JPEG;
        }
        else if ( strcmp( cFileExtLower, ".bmp" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_BMP;
        }
        else if ( strcmp( cFileExtLower, ".gif" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_GIF;
        }
        else if ( strcmp( cFileExtLower, ".png" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_PNG;
        }
        else if ( strcmp( cFileExtLower, ".tiff" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_TIFF;
        }
        else if ( strcmp( cFileExtLower, ".pri" ) == 0 ) /* PRI ? */
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_PICT_RAW;
        }
        /*----------------------------------------------------*/
        /*              AUDIO                                                                  */
        /*----------------------------------------------------*/
        else if (( strcmp( cFileExtLower, ".mp3" ) == 0 )|| (strcmp( cFileExtLower, ".mp2" ) == 0 ))
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_MP3;
        }
        else if ( strcmp( cFileExtLower, ".flac" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_FLAC;
        }
        else if ( strcmp( cFileExtLower, ".ogg" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_OGG;
        }
        else if ( strcmp( cFileExtLower, ".wma" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_WMA;
        }
        else if ( strcmp( cFileExtLower, ".aac" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_AAC;
        }
        else if ( strcmp( cFileExtLower, ".wav" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AUDIO_WAV;
        }
        /*----------------------------------------------------*/
        /*              AUDIO-VIDEO                                                        */
        /*----------------------------------------------------*/
         else if ( strcmp( cFileExtLower, ".mpg" ) == 0 || strcmp( cFileExtLower, ".mpeg" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MPEG2_ES; /* ? */
             /**peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MPEG2_TS;  ? */
        }
        else if ( ( strcmp( cFileExtLower, ".div" ) == 0 )||
                  ( strcmp( cFileExtLower, ".divx" ) == 0 ) )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_DIVX;
        }
        else if ( strcmp( cFileExtLower, ".avi" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_AVI;
        }
        else if ( strcmp( cFileExtLower, ".mkv" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MATROSKA;
        }
        else if ( strcmp( cFileExtLower, ".3gp" ) == 0 ) /* ? */
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_3GP;
        }
        else if ( strcmp( cFileExtLower, ".mp4" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_MP4;
        }
        else if ( strcmp( cFileExtLower, ".wmv" ) == 0 )
        {
             *peType = eMEDIA_VIEWER_CFG_MEDIA_TYPE_AV_WMV;
        }

    }

    return (result);
}

bool MEDIA_VIEWER_CFG_PreferredOrderMediaTypeGet(tMEDIA_VIEWER_CFG_MediaType** picture_order, int32_t * picture_order_size ,tMEDIA_VIEWER_CFG_MediaType ** audio_order, int32_t * audio_order_size, tMEDIA_VIEWER_CFG_MediaType ** video_order, int32_t * video_order_size )
{

	if (picture_order == NULL || picture_order_size == NULL ||
	        audio_order == NULL || audio_order_size == NULL ||
	        video_order ==  NULL  || video_order_size == NULL)
	{
	    return 0;
	}

    *picture_order = &prefered_pictuer_order;
	*picture_order_size = sizeof(prefered_pictuer_order);

	*audio_order = &prefered_audio_order;
	*audio_order_size = sizeof(prefered_audio_order);

	*video_order = &prefered_video_order;
	*video_order_size = sizeof(prefered_video_order);

	return 1;

}

