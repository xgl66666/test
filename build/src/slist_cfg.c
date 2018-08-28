/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: SLIST CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/slist_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <string.h>

#include "crules.h"
#include "slist_cfg.h"





/********************************************************/
/*              Defines                                 */
/********************************************************/

/*TODO:
 * This enumeration is fetched from GHITScustomization_on_st-nais-vsm branch of nais project to provide
 * proper way to check is DVB service supported.
 */
typedef enum
{
    /* Following values are taken from doc EN_300468 v1.7.1 */
    SLIST_CFG_SERV_TYPE_DIG_TV                        = 0x01,
    SLIST_CFG_SERV_TYPE_DIG_RAD                       = 0x02,
    SLIST_CFG_SERV_TYPE_TTXT                          = 0x03,
    SLIST_CFG_SERV_TYPE_NVOD_REF                      = 0x04,
    SLIST_CFG_SERV_TYPE_NVOD_TIME_SHIFT               = 0x05,
    SLIST_CFG_SERV_TYPE_MOSAIC                        = 0x06,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_DIG_RAD             = 0x0A,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_MOSAIC              = 0x0B,
    SLIST_CFG_SERV_TYPE_DATA_BROADCAST                = 0x0C,
    SLIST_CFG_SERV_TYPE_COMMON_INTERF_USAGE           = 0x0D,
    SLIST_CFG_SERV_TYPE_RCS_MAP                       = 0x0E,
    SLIST_CFG_SERV_TYPE_RCS_FLS                       = 0x0F,
    SLIST_CFG_SERV_TYPE_DVB_MHP                       = 0x10,
    SLIST_CFG_SERV_TYPE_MPEG2_HD_DIG_TV               = 0x11,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_DIG_TV              = 0x16,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_SD_NVOD_TIME_SHIFT  = 0x17,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_SD_NVOD_REF         = 0x18,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_DIG_TV           = 0x19,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_NVOD_TIME_SHIFT  = 0x1A,
    SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_NVOD_REF         = 0x1B,

    /* specific value to identify analog services */
    SLIST_CFG_SERV_TYPE_ANALOG                        = 0xFE,

    SLIST_CFG_SERV_TYPE_UNKNOWN                       = 0xFF
}SLIST_CFG_ServiceType;
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


/*----------------------------------------------------------------------------------*/
/* The order is : "Alphabetical sort","Scrambled/FTA sort","Lock/Unlock sort","Manual sort"			*/
/* Currently : only {TRUE,X,X,TRUE} is possible												*/
/*----------------------------------------------------------------------------------*/
LOCAL bool	SLIST_CFG_SortConfig[4] ={TRUE,TRUE,TRUE,TRUE};


/*----------------------------------------------------------------------------------*/
/*	Enable/disable icons in services selector screen : Lock Icon & Scrambled Icon				*/
/*----------------------------------------------------------------------------------*/
LOCAL bool	SLIST_CFG_SelectorConfig[2] ={TRUE, TRUE};


/*===========================================================================
 *
 * SLIST_CFG_GetMode
 *
 * Parameters:
 *      none
 *
 * Description: returns the mode : 
 *		-two types of lists : TV and radio
 *		-only one type : TV
 *		-only one type : Tv & radio in the same list
 *      
 *
 * Returns:
 *      enum
 *
 *===========================================================================*/
tSLIST_CFG_Mode SLIST_CFG_GetMode(void)
{
	return eSLIST_CFG_TV_AND_RADIO;
}


/*===========================================================================
 *
 * SLIST_CFG_GetMaximumNumberOfFavouriteLists
 *
 * Parameters:
 *      none
 *
 * Description: returns the maximum numbers of favourite lists an end user can create
 *  		->	If equals to 0 : end-user will not be able to create a favourite list.
 *		->    if mode equals to eSLIST_CFG_TV_AND_RADIO :  this the maximum number (& the same) for each type.
 *      
 *
 * Returns:
 *      enum
 *
 *===========================================================================*/
uint8_t SLIST_CFG_GetMaximumNumberOfFavouriteLists(void)
{
	return 4;
}


/*===========================================================================
 *
 * SLIST_CFG_IsEditingPredefinedListAllowed
 *
 * Parameters:
 *      none
 *
 * Description: Allow (or not) editing the predefined list ( skip/add  only - not sort/move )
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool SLIST_CFG_IsEditingPredefinedListAllowed(void)
{
	return FALSE;
}


/*===========================================================================
 *
 * SLIST_CFG_GetSortConfig
 *
 * Parameters:
 *      none
 *
 * Description: Enable/Disable the sort criteria
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool SLIST_CFG_GetSortConfig(uint8_t index)
{
 	if ( index > 3)
	{
		return FALSE;
	}
	return SLIST_CFG_SortConfig[index];
}

/*===========================================================================
 *
 * SLIST_CFG_GetSelectorConfig
 *
 * Parameters:
 *      none
 *
 * Description: Enable/disable icons in services selector
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool SLIST_CFG_GetSelectorConfig(uint8_t index)
{
 	if ( index > 1)
	{
		return FALSE;
	}
	return SLIST_CFG_SelectorConfig[index];
}

/*===========================================================================
 *
 * SLIST_CFG_IsDataServiceSupported
 *
 * Parameters:
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool SLIST_CFG_IsDataServiceSupported(void)
{
#ifdef INTERACTIVE_ENGINE_ON
	return TRUE;
#else
	return FALSE;
#endif
}

/*===========================================================================
 *
 * SLIST_CFG_IsHDServiceSupported
 *
 * Parameters:
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool SLIST_CFG_IsHDServiceSupported(void)
{
#ifdef HD_ENABLE
	return TRUE;
#else
	return FALSE;
#endif	
}

/*===========================================================================
 *
 * SLIST_CFG_IsLCNInFavoriteList
 *
 * Parameters:
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
bool SLIST_CFG_IsLCNInFavoriteList(void)
{
#ifdef LCN_IN_FAV
	return TRUE;
#else
	return FALSE;
#endif
}


/*TODO:
 * This function is fetched from GHITScustomization_on_st-nais-vsm branch of nais project to provide
 * proper way to check is DVB service supported.
 */

/* Good place to start with selecting the set of services
*  which is supported on the current project
*/
bool SLIST_CFG_IsDVBServiceSupported(uint8_t dvb_service_type)
{
    bool result = false;
    switch(dvb_service_type)
    {
    case SLIST_CFG_SERV_TYPE_DIG_TV:
#ifdef HD_ENABLE
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_DIG_TV:
#endif
    case SLIST_CFG_SERV_TYPE_DIG_RAD:
        result = true;
        break;
    case SLIST_CFG_SERV_TYPE_TTXT:
    case SLIST_CFG_SERV_TYPE_NVOD_REF:
    case SLIST_CFG_SERV_TYPE_NVOD_TIME_SHIFT:
    case SLIST_CFG_SERV_TYPE_MOSAIC:
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_DIG_RAD:
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_MOSAIC:
    case SLIST_CFG_SERV_TYPE_DATA_BROADCAST:
    case SLIST_CFG_SERV_TYPE_COMMON_INTERF_USAGE:
    case SLIST_CFG_SERV_TYPE_RCS_MAP:
    case SLIST_CFG_SERV_TYPE_RCS_FLS:
    case SLIST_CFG_SERV_TYPE_DVB_MHP:
    case SLIST_CFG_SERV_TYPE_MPEG2_HD_DIG_TV:
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_DIG_TV:
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_SD_NVOD_TIME_SHIFT:
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_SD_NVOD_REF:
#ifndef HD_ENABLE
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_DIG_TV:
#endif
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_NVOD_TIME_SHIFT:
    case SLIST_CFG_SERV_TYPE_ADV_CODEC_HD_NVOD_REF:
        /* specific value to identify analog services */
    case SLIST_CFG_SERV_TYPE_ANALOG:
    case SLIST_CFG_SERV_TYPE_UNKNOWN:
        result = false;
    }

    return result;
}
