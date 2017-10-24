/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: SLIST CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/slist_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _SLIST_CFG_H_
#define _SLIST_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (defined (BAT_GENRES) && defined (DEN_SDT_GENRES_DESCRIPTOR))
	#define GENRE_NUMBERS 21
    
#elif defined (BAT_GENRES)
    #define GENRE_NUMBERS 21

#elif defined (DEN_SDT_GENRES_DESCRIPTOR)
    #define GENRE_NUMBERS 16

#else
    #define GENRE_NUMBERS 0
#endif



/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef enum
{
	eSLIST_CFG_TV_AND_RADIO,			/*two types of lists : 	TV and radio*/
	eSLIST_CFG_TV_ONLY,				/*only one type : 		TV*/
	eSLIST_CFG_TV_RADIO_ONLY			/*only one type : 		Tv & radio services in the same list*/
}tSLIST_CFG_Mode;



/*******************************************************/
/* Functions                                           */
/*******************************************************/
tSLIST_CFG_Mode SLIST_CFG_GetMode(void);
bool SLIST_CFG_IsEditingPredefinedListAllowed(void);
uint8_t SLIST_CFG_GetMaximumNumberOfFavouriteLists(void);
bool SLIST_CFG_GetSortConfig(uint8_t index);
bool SLIST_CFG_GetSelectorConfig(uint8_t index);
bool SLIST_CFG_IsDataServiceSupported(void);
bool SLIST_CFG_IsHDServiceSupported(void);
bool SLIST_CFG_IsLCNInFavoriteList(void);
bool SLIST_CFG_IsDVBServiceSupported(uint8_t dvb_service_type);

#ifdef __cplusplus
}
#endif

#endif /* _SLIST_CFG_H_ */
