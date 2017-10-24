/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: AVCLUSTER
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/av_cluster_cfg.c $
 *            $Rev: 673 $
 *            $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/

#include "crules.h"
#include "string.h"

#include "tkel.h"

#include "av_cluster_cfg.h"
#include "country_cfg.h"
#include "cm_setup.h"

/*#define PREF_AUDIO_LANG_UND*/
/*#define PREF_AUDIO_CLEAN_EFFECT*/
/*#define PREF_AUDIO_LOWEST_PID*/
#define PREF_AUDIO_FIRST_PID
#define PREFER_USER_PREFERENCES_OVER_AFD

#ifdef AUXILIARY_VIDEO_DISPLAY_SD
#define AVCLUSTER_NB_MAIN_OUTPUTS 4
#define AVCLUSTER_NB_AUX1_OUTPUTS 2

tAVCLUSTER_CFG_OutputId AVCLUSTER_Main_Ouptuts[AVCLUSTER_NB_MAIN_OUTPUTS]=
{
	eAVCLUSTER_CFG_OUTPUT_HDMI_ID_0,
	eAVCLUSTER_CFG_OUTPUT_VIDEO_HD_DAC_ID_0,
	eAVCLUSTER_CFG_OUTPUT_AUDIO_SPDIF_ID_0,
	eAVCLUSTER_CFG_OUTPUT_RF_MOD_ID_0
};


tAVCLUSTER_CFG_OutputId AVCLUSTER_Aux1_Ouptuts[AVCLUSTER_NB_AUX1_OUTPUTS]=
{
	eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_0,
	eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_0
};
#else /* AUXILIARY_VIDEO_DISPLAY_SD */
#define AVCLUSTER_NB_MAIN_OUTPUTS 5
#define AVCLUSTER_NB_AUX1_OUTPUTS 0

tAVCLUSTER_CFG_OutputId AVCLUSTER_Main_Ouptuts[AVCLUSTER_NB_MAIN_OUTPUTS]=
{
#ifdef HD_ENABLE
//Config for 5029/5C59
    eAVCLUSTER_CFG_OUTPUT_HDMI_ID_0,
    eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_0,
	eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_0,
#else
    eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_0,
    eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_0
#endif
};

/*
tAVCLUSTER_CFG_OutputId AVCLUSTER_Aux1_Ouptuts[AVCLUSTER_NB_AUX1_OUTPUTS]=
{
	eAVCLUSTER_CFG_OUTPUT_VIDEO_DENC_ID_0,
	eAVCLUSTER_CFG_OUTPUT_AUDIO_ANA_ID_0
};
*/
#endif /* AUXILIARY_VIDEO_DISPLAY_SD */

/*===========================================================================
 *
 * AVCLUSTER_CFG_GetOutputs
 *
 * Parameters:
 *      IN:  Address of outputId pointer
 *		OUT: outputId pointer on filled outputId array
 *
 * Description:
 *
 *
 * Returns:
 *      nb outputId on main
 *
 *===========================================================================*/
int32_t AVCLUSTER_CFG_GetOutputs(tAVCLUSTER_CFG_Output output, tAVCLUSTER_CFG_OutputId **ppOutputId)
{
	int32_t	nbOutputs = 0;

	if (ppOutputId != NULL)
	{
		if (output == eAVCLUSTER_CFG_OUTPUT_MAIN)
		{
			if (AVCLUSTER_NB_MAIN_OUTPUTS != 0)
			{
				*ppOutputId = AVCLUSTER_Main_Ouptuts;
			}
			else
			{
				*ppOutputId = NULL;
			}
			nbOutputs = AVCLUSTER_NB_MAIN_OUTPUTS;
		}
		else if (output == eAVCLUSTER_CFG_OUTPUT_AUX1)
		{
#ifdef AUXILIARY_VIDEO_DISPLAY_SD
			if (AVCLUSTER_NB_AUX1_OUTPUTS != 0)
			{
				*ppOutputId = AVCLUSTER_Aux1_Ouptuts;
			}
			else
			{
				*ppOutputId = NULL;
			}
#else /* AUXILIARY_VIDEO_DISPLAY_SD */
			*ppOutputId = NULL;
#endif /* AUXILIARY_VIDEO_DISPLAY_SD */
			nbOutputs = AVCLUSTER_NB_AUX1_OUTPUTS;
		}
		else
		{
			*ppOutputId = NULL;
			return 0;
		}
	}
	else
	{
		return -1;
	}
	
	return nbOutputs;
}

/*===========================================================================
 *
 * AVCLUSTER_CFG_GetDefaultPCMOrBitstream
 *
 * Parameters:
 *		None
 *
 * Description:
 *
 *
 * Returns:
 *      1 in case of bitstream else 0
 *
 *===========================================================================*/
uint8_t AVCLUSTER_CFG_GetDefaultPCMOrBitstream(void)
{
	return 1;
}

typedef enum
{
	eAVCLUSTER_FIRST_LANGUAGE,
	eAVCLUSTER_SECOND_LANGUAGE,
	eAVCLUSTER_UNDEFINED_LANGUAGE,
	eAVCLUSTER_LOWESTPID,
	eAVCLUSTER_PRE_MIXED,
	eAVCLUSTER_AD_FROM_MAIN_TRACK
}tAVCLUSTER_SearchType;

bool AVCLUSTER_CFGi_FillISO639(tAVCLUSTER_Language *pstISO639a, tAVCLUSTER_Language *pstISO639b, tAVCLUSTER_Language *pstISO639Source)
{
    bool                   bResult = TRUE;
    tCOUNTRY_CFG_Language *pstLanguageList;
    uint16_t               usLanguageNb;
    uint16_t               usLanguageIdx;
    bool                   bFound = FALSE;

    pstLanguageList = COUNTRY_CFG_GetLanguageList();
    usLanguageNb    = COUNTRY_CFG_GetNumberOfLanguage();

    for(usLanguageIdx = 0; ((usLanguageIdx < (usLanguageNb + 1)) && (bFound == FALSE)); usLanguageIdx++)
    {
        if((strcmp((const char*)(pstISO639Source->ucISO639trigram), (const char*)(pstLanguageList[usLanguageIdx].DVBtrigramFirst))  == 0) ||
           (strcmp((const char*)(pstISO639Source->ucISO639trigram), (const char*)(pstLanguageList[usLanguageIdx].DVBtrigramSecond)) == 0))
        {
            /* ok, save the language */
            strncpy((char*)(pstISO639a->ucISO639trigram), (const char*)(pstLanguageList[usLanguageIdx].DVBtrigramFirst), 4);
            strncpy((char*)(pstISO639b->ucISO639trigram), (const char*)(pstLanguageList[usLanguageIdx].DVBtrigramSecond), 4);
            bFound = TRUE;
        }
    }

    if(bFound == FALSE)
    {
        bResult = FALSE;
    }
    return bResult;
}

bool AVCLUSTER_CFGi_CopyTrackToTrack(tAVCLUSTER_AUDIO_Track *pstSrcTrack, tAVCLUSTER_AUDIO_Track *pstDstTrack)
{
	
	pstDstTrack->eAudioStreamType 	= pstSrcTrack->eAudioStreamType;
	pstDstTrack->ucAudioType		= pstSrcTrack->ucAudioType;
	pstDstTrack->ucDualMode			= pstSrcTrack->ucDualMode;
	strncpy((char*)(&(pstDstTrack->stLanguage.ucISO639trigram[0])),
			(const char*)(&(pstSrcTrack->stLanguage.ucISO639trigram[0])),
			4);
	pstDstTrack->usPid				= pstSrcTrack->usPid;
	pstDstTrack->sComponentTag		= pstSrcTrack->sComponentTag;

    return true;
}

bool AVCLUSTER_CFGi_GetLowestPidAudioFromArray(
			uint8_t 						ucNbTrackInList,
			tAVCLUSTER_AUDIO_Track			*pstAudioTrackList,
			tAVCLUSTER_AUDIO_Track 			*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 			*pstADTrack)
{
	uint8_t					ucAudioTrackIndex = 0;
	tAVCLUSTER_AUDIO_Track 	*pstTempTrack = NULL;

	pstTempTrack = &pstAudioTrackList[0];

	for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucNbTrackInList;ucAudioTrackIndex++)
	{
		if (pstAudioTrackList[ucAudioTrackIndex].usPid < pstTempTrack->usPid)
		{
			pstTempTrack = &(pstAudioTrackList[ucAudioTrackIndex]);
		}
	}

	AVCLUSTER_CFGi_CopyTrackToTrack(pstTempTrack, pstMainTrack);
	/* No AD for The moment*/
	pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
	return TRUE;
}

bool AVCLUSTER_CFGi_GetLowestPidAudioFromPointers(
			uint8_t 						ucNbTrackInList,
			tAVCLUSTER_AUDIO_Track			**ppstAudioTrackList,
			tAVCLUSTER_AUDIO_Track 			*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 			*pstADTrack)
{
	uint8_t					ucAudioTrackIndex = 0;
	tAVCLUSTER_AUDIO_Track 	*pstTempTrack = NULL;

	pstTempTrack = ppstAudioTrackList[0];

	for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucNbTrackInList;ucAudioTrackIndex++)
	{
		if (ppstAudioTrackList[ucAudioTrackIndex]->usPid < pstTempTrack->usPid)
		{
			pstTempTrack = ppstAudioTrackList[ucAudioTrackIndex];
		}
	}

	AVCLUSTER_CFGi_CopyTrackToTrack(pstTempTrack, pstMainTrack);
	/* No AD for The moment*/
	pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
	return TRUE;
}

bool AVCLUSTER_CFGi_GetFirstPidAudioFromArray(
			uint8_t 						ucNbTrackInList,
			tAVCLUSTER_AUDIO_Track			*pstAudioTrackList,
			tAVCLUSTER_AUDIO_Track 			*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 			*pstADTrack)
{
	AVCLUSTER_CFGi_CopyTrackToTrack(&pstAudioTrackList[0], pstMainTrack);
	/* No AD for The moment*/
	pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
	return TRUE;
}

bool AVCLUSTER_CFGi_GetFirstPidAudioFromPointers(
			uint8_t 						ucNbTrackInList,
			tAVCLUSTER_AUDIO_Track			**ppstAudioTrackList,
			tAVCLUSTER_AUDIO_Track 			*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 			*pstADTrack)
{
	AVCLUSTER_CFGi_CopyTrackToTrack(ppstAudioTrackList[0], pstMainTrack);
	/* No AD for The moment*/
	pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
	return TRUE;
}

uint8_t AVCLUSTER_CFGi_GetAudioFormat(tAVCLUSTER_AUDIO_Format eStreamType, uint8_t ucNbTrackInList, tAVCLUSTER_AUDIO_Track	**ppstAudioTrackList)
{
	uint8_t		ucAudioTrackIndex = 0;
	uint8_t		ucNbAudioFound = 0;

	for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucNbTrackInList;ucAudioTrackIndex++)
	{
		if (ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eStreamType)
		{
			ppstAudioTrackList[ucNbAudioFound] = ppstAudioTrackList[ucAudioTrackIndex];
			ucNbAudioFound++;
		}
	}
	return ucNbAudioFound;
}

bool AVCLUSTER_CFGi_IsValidVisualImpaired(tAVCLUSTER_AUDIO_Format eStreamType, uint8_t ucNbTrackInList, tAVCLUSTER_AUDIO_Track	**ppstAudioTrackList)
{
	bool 					bTrackFound = FALSE;
	uint8_t					ucTempAudioTrackVisualImpairedIndex = 0;
	uint8_t					ucAudioTrackIndex = 0;
	tAVCLUSTER_AUDIO_Track	*pstTempADTrack = NULL;

	/* Look for visual impaired track */
	for(ucTempAudioTrackVisualImpairedIndex = 0;ucTempAudioTrackVisualImpairedIndex < ucNbTrackInList;ucTempAudioTrackVisualImpairedIndex++)
	{
		if((ppstAudioTrackList[ucTempAudioTrackVisualImpairedIndex]->ucAudioType == AVCLUSTER_AUDIO_TYPE_VISUAL_IMPAIRED_COMMENTARY) &&
			(ppstAudioTrackList[ucTempAudioTrackVisualImpairedIndex]->eAudioStreamType == eStreamType))
		{
			pstTempADTrack = ppstAudioTrackList[ucTempAudioTrackVisualImpairedIndex];
			for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucNbTrackInList;ucAudioTrackIndex++)
			{
				if((ppstAudioTrackList[ucAudioTrackIndex]->ucAudioType == AVCLUSTER_AUDIO_TYPE_UNDEFINED) &&
					(
						/* Mixing same track allowed*/
						(ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eStreamType) ||
						(
							/* Or Mixing DD and DD+ allowed*/
							(ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AC3) &&
							(eStreamType == eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED)
						) ||
						(
							(ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED) &&
							(eStreamType == eAVCLUSTER_AUDIO_STREAM_AC3)
						)						
					)
				)
				{
					ppstAudioTrackList[0] = ppstAudioTrackList[ucAudioTrackIndex];
					ppstAudioTrackList[1] = pstTempADTrack;
					bTrackFound = TRUE;
				}
			}
		}
	}
	return bTrackFound;
}

bool AVCLUSTER_CFGi_IsSupplementaryAudioAvailable(uint8_t ucNbTrackInList, tAVCLUSTER_AUDIO_Track	**ppstAudioTrackList)
{
	bool 					bTrackFound = FALSE;
	uint8_t					ucAudioTrackIndex = 0;

	/* Look for visual impaired track */
	for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucNbTrackInList;ucAudioTrackIndex++)
	{
		if(ppstAudioTrackList[ucAudioTrackIndex]->supplementary.ucDefined == 1)
		{
			bTrackFound = TRUE;
			break;
		}
	}
	return bTrackFound;
}


int8_t AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(tAVCLUSTER_AUDIO_Format eStreamType, uint8_t ucNbTrackInList, tAVCLUSTER_AUDIO_Track	**ppstAudioTrackList)
{
	int8_t 					returnValue = -1;
	uint8_t					ucTempAudioIndex = 0;
	uint8_t					ucAudioTrackIndex = 0;
	tAVCLUSTER_AUDIO_Track	*pstTempTrack = NULL;

	/* Look for visual impaired track */
	for(ucTempAudioIndex = 0;((ucTempAudioIndex < ucNbTrackInList)&&(returnValue == -1));ucTempAudioIndex++)
	{
		if((ppstAudioTrackList[ucTempAudioIndex]->supplementary.ucDefined == 1) &&
			(ppstAudioTrackList[ucTempAudioIndex]->eAudioStreamType == eStreamType))
		{
			if(ppstAudioTrackList[ucTempAudioIndex]->supplementary.ucMixType == AVCLUSTER_AUDIO_MIX_TYPE_INDEPENDENT_STREAM)
			{
				/* BroadcastMixed found;*/
				returnValue = AVCLUSTER_AUDIO_MIX_TYPE_INDEPENDENT_STREAM;
				ppstAudioTrackList[0] = ppstAudioTrackList[ucTempAudioIndex];
				ppstAudioTrackList[1] = NULL;
				break;
			}
			else if((ppstAudioTrackList[ucTempAudioIndex]->supplementary.ucMixType == AVCLUSTER_AUDIO_MIX_TYPE_SUPPLEMENTARY_STREAM) &&
					(ppstAudioTrackList[ucTempAudioIndex]->supplementary.ucEditorialClassification == AVCLUSTER_AUDIO_EDIT_CLASS_MAIN_AUDIO)
					)
			{
				pstTempTrack = ppstAudioTrackList[ucTempAudioIndex];

				for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucNbTrackInList;ucAudioTrackIndex++)
				{
					/* Mixing same track allowed*/
					if(
						((ppstAudioTrackList[ucAudioTrackIndex]->supplementary.ucMixType == AVCLUSTER_AUDIO_MIX_TYPE_SUPPLEMENTARY_STREAM) &&
						((ppstAudioTrackList[ucAudioTrackIndex]->supplementary.ucEditorialClassification == AVCLUSTER_AUDIO_EDIT_CLASS_AUDIO_DESCRIPTION) ||
                            (ppstAudioTrackList[ucAudioTrackIndex]->supplementary.ucEditorialClassification == AVCLUSTER_AUDIO_EDIT_CLASS_CLEAN_AUDIO) ||
                            (ppstAudioTrackList[ucAudioTrackIndex]->supplementary.ucEditorialClassification == AVCLUSTER_AUDIO_EDIT_CLASS_SUBTITLE_SPOKEN))
						) &&
						(
							(ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eStreamType) ||
								(
									/* Or Mixing DD and DD+ allowed*/
									(ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AC3) &&
									(eStreamType == eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED)
								) ||
								(
									(ppstAudioTrackList[ucAudioTrackIndex]->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED) &&
									(eStreamType == eAVCLUSTER_AUDIO_STREAM_AC3)
								)						
							)
						)
					{
						returnValue = AVCLUSTER_AUDIO_MIX_TYPE_SUPPLEMENTARY_STREAM;
						ppstAudioTrackList[0] = pstTempTrack;
						ppstAudioTrackList[1] = ppstAudioTrackList[ucAudioTrackIndex];
						break;						
					}
				}
			}
		}
	}
	return returnValue;
}

bool AVCLUSTER_CFGi_CheckLanguage(tAVCLUSTER_AUDIO_Track 	*pstAudioTrack, tAVCLUSTER_Language *pstFirstLang, tAVCLUSTER_Language *pstSecondLang)
{
	bool returnValue = FALSE;

	if((strcmp((const char*)(pstAudioTrack->stLanguage.ucISO639trigram), (const char*)(pstFirstLang->ucISO639trigram)) == 0) ||
		(strcmp((const char*)(pstAudioTrack->stLanguage.ucISO639trigram), (const char*)(pstSecondLang->ucISO639trigram)) == 0))
	{
		returnValue = TRUE;
	}
	else if ((pstAudioTrack->supplementary.ucDefined == 1) &&
			(pstAudioTrack->supplementary.ucLanguageCodePresent == 1))
	{
		if((strcmp((const char*)(pstAudioTrack->supplementary.stLanguage.ucISO639trigram), (const char*)(pstFirstLang->ucISO639trigram)) == 0) ||
			(strcmp((const char*)(pstAudioTrack->supplementary.stLanguage.ucISO639trigram), (const char*)(pstSecondLang->ucISO639trigram)) == 0))
		{
			returnValue = TRUE;
		}
	}
	return returnValue;
}

bool AVCLUSTER_CFGi_GetPreferredAudio(
			tAVCLUSTER_SearchType 						eSearchType,
			bool 										bCleanEffect,
            bool                                        bIsNordig,
			tAVCLUSTER_AUDIO_UserSelectionPreferences 	*pstUserPref,
			tAVCLUSTER_AUDIO_TrackList					*pstTrackList,
			tAVCLUSTER_AUDIO_Track 						*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 						*pstADTrack)
{
	tAVCLUSTER_Language		ISO639aSearched;
	tAVCLUSTER_Language		ISO639bSearched;
	bool 					bReturnValue = TRUE;
	bool 					bADFoundAndUsed = FALSE;
	uint8_t					ucAudioTrackIndex = 0;
	uint8_t					ucTempAudioTrackNbAfterLanguage = 0;
	uint8_t					ucTempAudioTrackNbAfterGoodHearingImpaired = 0;
/*	uint8_t					ucTempAudioTrackNbAfterGoodVisualImpaired = 0;*/
	uint8_t					ucTempAudioTrackNbAfterGoodFormat = 0;
	tAVCLUSTER_AUDIO_Track	**ppstAudioTrackList = NULL;
	tAVCLUSTER_AUDIO_Track 	*pstTempAudioTrack = NULL;
	TKEL_err				tkel_err = TKEL_NO_ERR;
	int8_t					cADSupplementaryAudio = -1;

	ISO639aSearched.ucISO639trigram[0] = '\0';
	ISO639bSearched.ucISO639trigram[0] = '\0';

	switch(eSearchType)
	{
		case eAVCLUSTER_FIRST_LANGUAGE:
			bReturnValue = AVCLUSTER_CFGi_FillISO639(&ISO639aSearched,
									&ISO639bSearched, 									
									&(pstUserPref->stDefaultLanguage));
			break;
		case eAVCLUSTER_SECOND_LANGUAGE:
			bReturnValue = AVCLUSTER_CFGi_FillISO639(&ISO639aSearched,
									&ISO639bSearched,
									&(pstUserPref->stDefaultSecondLanguage));
			break;
		case eAVCLUSTER_UNDEFINED_LANGUAGE:
			strncpy((char*)(&(ISO639aSearched.ucISO639trigram[0])), (const char*)("und"), 4);
			strncpy((char*)(&(ISO639bSearched.ucISO639trigram[0])), (const char*)("und"), 4);
			break;
		case eAVCLUSTER_PRE_MIXED:
			strncpy((char*)(&(ISO639aSearched.ucISO639trigram[0])), (const char*)("qad"), 4);
			strncpy((char*)(&(ISO639bSearched.ucISO639trigram[0])), (const char*)("QAD"), 4);
			break;
		case eAVCLUSTER_AD_FROM_MAIN_TRACK:
			bReturnValue = AVCLUSTER_CFGi_FillISO639(&ISO639aSearched,
									&ISO639bSearched,
									&(pstMainTrack->stLanguage));
		default:
			bReturnValue = FALSE;
	}
	
	if (bReturnValue == TRUE)
	{
		tkel_err = TKEL_Malloc((uint32_t)(pstTrackList->usNbAudioTrack*sizeof(tAVCLUSTER_AUDIO_Track*)), (void**)&ppstAudioTrackList);
		if (tkel_err != TKEL_NO_ERR)
		{
			bReturnValue = FALSE;
		}
	}

	if (bReturnValue == TRUE)
	{
		/* Find track list of corresponding language by taken into account clean effect value */
		for(ucAudioTrackIndex = 0;ucAudioTrackIndex < pstTrackList->usNbAudioTrack;ucAudioTrackIndex++)
		{
			pstTempAudioTrack = &(pstTrackList->pstAudioTrackList[ucAudioTrackIndex]);
			if (AVCLUSTER_CFGi_CheckLanguage(pstTempAudioTrack, &ISO639aSearched, &ISO639bSearched) == TRUE)
			{
				if ((bCleanEffect == TRUE) ||
						((pstTempAudioTrack->ucAudioType != AVCLUSTER_AUDIO_TYPE_CLEAN_EFFECT) &&
						(bCleanEffect == FALSE))
					)
				{
					ppstAudioTrackList[ucTempAudioTrackNbAfterLanguage] = pstTempAudioTrack;
					ucTempAudioTrackNbAfterLanguage++;
				}
			}
		}

		if(eSearchType == eAVCLUSTER_AD_FROM_MAIN_TRACK)
		{
			/* Language filtered, now search AD track */
			if ((pstMainTrack->supplementary.ucDefined == 1) &&
				(pstMainTrack->supplementary.ucMixType == AVCLUSTER_AUDIO_MIX_TYPE_SUPPLEMENTARY_STREAM))
			{
				cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(pstMainTrack->eAudioStreamType,
												ucTempAudioTrackNbAfterLanguage,
												ppstAudioTrackList);
			}
			else if(pstMainTrack->supplementary.ucDefined == 0)
			{
				bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(pstMainTrack->eAudioStreamType,
												ucTempAudioTrackNbAfterLanguage,
												ppstAudioTrackList);
			}

			/* Check if main track found is really the same than the one requested by user */
			if(((bADFoundAndUsed == TRUE)|| (cADSupplementaryAudio == AVCLUSTER_AUDIO_MIX_TYPE_SUPPLEMENTARY_STREAM)) && (pstMainTrack->usPid == ppstAudioTrackList[0]->usPid))
			{
				AVCLUSTER_CFGi_CopyTrackToTrack(ppstAudioTrackList[1], pstADTrack);
			}
			else
			{
				pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
			}
		}
		else
		{
			if (ucTempAudioTrackNbAfterLanguage == 0)
			{
				bReturnValue = FALSE;
			}
			else
			{
				/* Test Hearing Impaired */
				for(ucAudioTrackIndex = 0;ucAudioTrackIndex < ucTempAudioTrackNbAfterLanguage;ucAudioTrackIndex++)
				{
					if(
						((pstUserPref->ucHearingImpairedPreferred == AVCLUSTER_AUDIO_PREFERENCES_HEARING_IMPAIRED) &&
						(ppstAudioTrackList[ucAudioTrackIndex]->ucAudioType == AVCLUSTER_AUDIO_TYPE_HEARING_IMPAIRED))
						||
						((pstUserPref->ucHearingImpairedPreferred == AVCLUSTER_AUDIO_PREFERENCES_NOT_HEARING_IMPAIRED) &&
						(ppstAudioTrackList[ucAudioTrackIndex]->ucAudioType != AVCLUSTER_AUDIO_TYPE_HEARING_IMPAIRED))
						)
					{
						ppstAudioTrackList[ucTempAudioTrackNbAfterGoodHearingImpaired] = ppstAudioTrackList[ucAudioTrackIndex];
						ucTempAudioTrackNbAfterGoodHearingImpaired++;
					}
				}
			}
	
			if(ucTempAudioTrackNbAfterGoodHearingImpaired == 0)
			{
				bReturnValue = FALSE;
			}
			else
			{
				/* Test Visual Impaired if requested  */
				if (pstUserPref->ucVisualImpairedPreferred == AVCLUSTER_AUDIO_PREFERENCES_VISUAL_IMPAIRED)
				{
					/* First use separate tracks system with audio type VISUAL IMPAIRED*/
					/* Check Audio Format for priorities search */
					if(pstUserPref->ucPcmOrBitstream == AVCLUSTER_AUDIO_PREFERENCES_PCM)
					{
						if (bADFoundAndUsed == TRUE)
						{
							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_MPEG,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
						}
                        if (bIsNordig == TRUE)
                        {
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
                        }
						if (bADFoundAndUsed == FALSE)
						{
							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AC3,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
						}
						if (bADFoundAndUsed == FALSE)
						{
							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
						}
                        if (bIsNordig == FALSE)
                        {
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
                        }
					}
					else
					{
                        if (bIsNordig == TRUE)
                        {
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_MPEG,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
                        }
						if (bADFoundAndUsed == FALSE)
						{
							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AC3,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
						}
						if (bADFoundAndUsed == FALSE)
						{
							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
						}
                        if (bIsNordig == FALSE)
                        {
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
    						if (bADFoundAndUsed == FALSE)
    						{
    							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_MPEG,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    						}
                        }
						if (bADFoundAndUsed == FALSE)
						{
							bADFoundAndUsed = AVCLUSTER_CFGi_IsValidVisualImpaired(eAVCLUSTER_AUDIO_STREAM_MPEG,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
						}
					}

					/* Secondly use search for Supplementary audio */
					if (bADFoundAndUsed == FALSE)
					{
						if (AVCLUSTER_CFGi_IsSupplementaryAudioAvailable(ucTempAudioTrackNbAfterGoodHearingImpaired, ppstAudioTrackList) == TRUE)
						{
							cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_MPEG,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);

                            if (bIsNordig == TRUE)
                            {
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
                            }
							if (cADSupplementaryAudio == -1)
							{
								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AC3,
																	ucTempAudioTrackNbAfterGoodHearingImpaired,
																	ppstAudioTrackList);

							}
							if (cADSupplementaryAudio == -1)
							{
								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED,
																	ucTempAudioTrackNbAfterGoodHearingImpaired,
																	ppstAudioTrackList);

							}

                            if (bIsNordig == FALSE)
                            {
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
                            }
						}
						else
						{
                            if (bIsNordig == TRUE)
                            {
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
                            }
							cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AC3,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);

							if (cADSupplementaryAudio == -1)
							{
								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AC3,
																	ucTempAudioTrackNbAfterGoodHearingImpaired,
																	ppstAudioTrackList);

							}
							if (cADSupplementaryAudio == -1)
							{
								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED,
																	ucTempAudioTrackNbAfterGoodHearingImpaired,
																	ppstAudioTrackList);

							}
                            if (bIsNordig == FALSE)
                            {
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
    							if (cADSupplementaryAudio == -1)
    							{
    								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																	ucTempAudioTrackNbAfterGoodHearingImpaired,
    																	ppstAudioTrackList);
    
    							}
                            }
							if (cADSupplementaryAudio == -1)
							{
								cADSupplementaryAudio = AVCLUSTER_CFGi_IsValidVisualImpairedWithSupplementaryAudio(eAVCLUSTER_AUDIO_STREAM_MPEG,
																	ucTempAudioTrackNbAfterGoodHearingImpaired,
																	ppstAudioTrackList);

							}
						}
					}
				}
			}
	
			if((bADFoundAndUsed == TRUE) || (cADSupplementaryAudio == AVCLUSTER_AUDIO_MIX_TYPE_SUPPLEMENTARY_STREAM))
			{
				/* Corresponding track MAIN and AD found */
				/* Copy them */
				AVCLUSTER_CFGi_CopyTrackToTrack(ppstAudioTrackList[0], pstMainTrack);
				AVCLUSTER_CFGi_CopyTrackToTrack(ppstAudioTrackList[1], pstADTrack);
			}
			else if (cADSupplementaryAudio == AVCLUSTER_AUDIO_MIX_TYPE_INDEPENDENT_STREAM)
			{
				/* track MAIN is Audio description broadcast mixed */
				AVCLUSTER_CFGi_CopyTrackToTrack(ppstAudioTrackList[0], pstMainTrack);
				pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
			}
			else if ((bADFoundAndUsed == FALSE) && (ucTempAudioTrackNbAfterGoodHearingImpaired != 0))
			{
				/* No AudioDescription and one or several tracks still exists in list */
				/* Check now Audio Format */
				if(pstUserPref->ucPcmOrBitstream == AVCLUSTER_AUDIO_PREFERENCES_PCM)
				{
					/* If PCM search : MPEG then DD then DD+ then HEACC */
					if (ucTempAudioTrackNbAfterGoodFormat == 0)
					{
						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_MPEG,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
					}

                    if (bIsNordig == TRUE)
                    {
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
                    }
					if (ucTempAudioTrackNbAfterGoodFormat == 0)
					{
						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AC3,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
					}
					if (ucTempAudioTrackNbAfterGoodFormat == 0)
					{
						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
					}
                    if (bIsNordig == FALSE)
                    {
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
                    }
				}
				else if(pstUserPref->ucPcmOrBitstream == AVCLUSTER_AUDIO_PREFERENCES_BITSTREAM)
				{
					/* If bitstream Search : DD DD+ HEACC then MPEG*/
                    if (bIsNordig == TRUE)
                    {
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
                    }

					if (ucTempAudioTrackNbAfterGoodFormat == 0)
					{
						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AC3,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
					}
					if (ucTempAudioTrackNbAfterGoodFormat == 0)
					{
						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
					}
                    if (bIsNordig == FALSE)
                    {
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
    					if (ucTempAudioTrackNbAfterGoodFormat == 0)
    					{
    						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2,
    																ucTempAudioTrackNbAfterGoodHearingImpaired,
    																ppstAudioTrackList);
    					}
                    }
					if (ucTempAudioTrackNbAfterGoodFormat == 0)
					{
						ucTempAudioTrackNbAfterGoodFormat = AVCLUSTER_CFGi_GetAudioFormat(eAVCLUSTER_AUDIO_STREAM_MPEG,
																ucTempAudioTrackNbAfterGoodHearingImpaired,
																ppstAudioTrackList);
					}
				}
				if(ucTempAudioTrackNbAfterGoodFormat == 0)
				{
					bReturnValue = FALSE;
				}
#if defined(PREF_AUDIO_LOWEST_PID)
				else if (ucTempAudioTrackNbAfterGoodFormat > 1)
				{
					/* Check lowest pid */
					bReturnValue = AVCLUSTER_CFGi_GetLowestPidAudioFromPointers(
												ucTempAudioTrackNbAfterGoodFormat,
												ppstAudioTrackList,
												pstMainTrack,
												pstADTrack);
				}
#elif defined(PREF_AUDIO_FIRST_PID)
				else if (ucTempAudioTrackNbAfterGoodFormat > 1)
				{
					/* Check first pid */
					bReturnValue = AVCLUSTER_CFGi_GetFirstPidAudioFromPointers(
												ucTempAudioTrackNbAfterGoodFormat,
												ppstAudioTrackList,
												pstMainTrack,
												pstADTrack);
				}
#else
#error define either PREF_AUDIO_LOWEST_PID or PREF_AUDIO_FIRST_PID
#endif
				else
				{
					/* Corresponding track found */
					/* Copy it into mainTrack */
					AVCLUSTER_CFGi_CopyTrackToTrack(ppstAudioTrackList[0], pstMainTrack);
					pstADTrack->eAudioStreamType = eAVCLUSTER_AUDIO_STREAM_NONE;
				}
			}
		 }

		TKEL_Free(ppstAudioTrackList);
	}

	return bReturnValue;
}

bool AVCLUSTER_CFG_GetAudioPreferredTrack(
			tAVCLUSTER_AUDIO_Format						eDecoderFormatSupported,
			tAVCLUSTER_AUDIO_UserSelectionPreferences 	*pstUserPref,
			tAVCLUSTER_AUDIO_TrackList					*pstTrackList,
			tAVCLUSTER_AUDIO_Track 						*pstMainTrack,
			tAVCLUSTER_AUDIO_Track 						*pstADTrack)
{
	bool bFound = FALSE;
    char DVBtrigram[4];
    bool bIsNordig = FALSE;

    strcpy(&DVBtrigram[0], COUNTRY_CFG_GetCurrentCountryDVBtrigram());

    if ((strcmp(DVBtrigram, "dnk") == 0) ||
        (strcmp(DVBtrigram, "fin") == 0) ||
        (strcmp(DVBtrigram, "isl") == 0) ||
        (strcmp(DVBtrigram, "nor") == 0) ||
        (strcmp(DVBtrigram, "est") == 0) ||
        (strcmp(DVBtrigram, "irl") == 0) ||
        (strcmp(DVBtrigram, "swe") == 0))
    {
        bIsNordig = TRUE;
    }

	if (pstTrackList->usNbAudioTrack != 0)
	{
		/* If Main track already selected by user, only serch for AD corresponding track*/
		if(pstMainTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_NONE)
		{
			if(pstUserPref->ucVisualImpairedPreferred == AVCLUSTER_AUDIO_PREFERENCES_VISUAL_IMPAIRED)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_AD_FROM_MAIN_TRACK,
													FALSE,
                                                    bIsNordig,
													pstUserPref,
													pstTrackList,
													pstMainTrack,
													pstADTrack);
			}
			else
			{
				/* Use only Main Track */
				bFound = TRUE;
			}
		}
		else
		{
			/* If audio description activated, look for "ad " first, no language specified */
			if (pstUserPref->ucVisualImpairedPreferred == AVCLUSTER_AUDIO_PREFERENCES_VISUAL_IMPAIRED)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_PRE_MIXED,
														FALSE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);

			}
			
			/* Then in any cases, look for language first */
			/* First Ignore Clean effect */
			if (bFound == FALSE)
			{

				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_FIRST_LANGUAGE,
														FALSE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);
			}
		
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_SECOND_LANGUAGE,
														FALSE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);
			}

#ifdef PREF_AUDIO_LANG_UND
			/* then look for undefined */
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_UNDEFINED_LANGUAGE,
														FALSE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);
			}
#endif
#ifdef PREF_AUDIO_CLEAN_EFFECT
			/* Then take clean effect into account */
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_FIRST_LANGUAGE,
														TRUE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);
			}
		
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_SECOND_LANGUAGE,
														TRUE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);
			}
#endif
#ifdef PREF_AUDIO_LANG_UND
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetPreferredAudio(eAVCLUSTER_UNDEFINED_LANGUAGE,
														TRUE,
                                                        bIsNordig,
														pstUserPref,
														pstTrackList,
														pstMainTrack,
														pstADTrack);
			}
#endif
#if defined(PREF_AUDIO_LOWEST_PID)
			/* At then end take the Lowest pid */
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetLowestPidAudioFromArray(
														pstTrackList->usNbAudioTrack,
														&(pstTrackList->pstAudioTrackList[0]),
														pstMainTrack,
														pstADTrack);
			}
#elif defined(PREF_AUDIO_FIRST_PID)
			/* At then end take the First pid */
			if (bFound == FALSE)
			{
				bFound = AVCLUSTER_CFGi_GetFirstPidAudioFromArray(
														pstTrackList->usNbAudioTrack,
														&(pstTrackList->pstAudioTrackList[0]),
														pstMainTrack,
														pstADTrack);
			}
#else
#error define either PREF_AUDIO_LOWEST_PID or PREF_AUDIO_FIRST_PID
#endif
		}
	}
	return bFound;
}

bool AVCLUSTER_CFG_CheckAudioTrack(tAVCLUSTER_AUDIO_Track *pstTrack)
{
	if((pstTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_MPEG) &&
		//(pstTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_AC3) &&
		//(pstTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED) &&
		(pstTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_AAC_MP2) &&
		(pstTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1) &&
		(pstTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/*===========================================================================
 *
 * AVCLUSTER_CFG_GetAudioProcessing
 *
 * Parameters:
 *		pstMainTrack IN: 		Main track
 *		pstADTrack 	IN: 		AD Track
 * 		pstOutput	IN:			Output
 * 		pstAudioProcessing IN/OUT:	Audio Processing
 *
 * Description:
 *
 *
 * Returns:
 *      TRUE 	if Audio processing Found
 * 		else FALSE
 *
 *===========================================================================*/
bool AVCLUSTER_CFG_GetAudioProcessing(
								tAVCLUSTER_AUDIO_Format						eDecoderFormatSupported,
								tAVCLUSTER_AUDIO_UserSelectionPreferences 	*pstUserPref,
								tAVCLUSTER_AUDIO_Track 						*pstMainTrack,
								tAVCLUSTER_AUDIO_Track 						*pstADTrack,
								tAVCLUSTER_AUDIO_Output						*pstOutput,
								tAVCLUSTER_AUDIO_Processing 				*pstAudioProcessing)
{
	bool bReturnValue = TRUE;

	pstAudioProcessing->cOutputId = pstOutput->cOutputId;

   switch(pstOutput->ucAudioOutputType)
   {
      case AVCLUSTER_OUTPUT_TYPE_HDMI:
      case AVCLUSTER_OUTPUT_TYPE_SPDIF:
         if ((pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_MPEG) ||
             (pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AAC_MP2) ||
             (pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AAC_MP4_1))
         {
            pstAudioProcessing->eOutputFormat = eAVCLUSTER_AUDIO_STREAM_MPEG;
         }
         else if ((pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AC3) ||
                  (pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AC3_ENHANCED) ||
                  (pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_AAC_MP4_2))
         {
            if (pstUserPref->ucPcmOrBitstream == AVCLUSTER_AUDIO_PREFERENCES_BITSTREAM)
            {
               /* check if the main track audio format is supported */
               if (pstOutput->eAudioSupportedFormatMask & pstMainTrack->eAudioStreamType)
               {
                  pstAudioProcessing->eOutputFormat = eAVCLUSTER_AUDIO_STREAM_AC3;
                  /*pstAudioProcessing->eOutputFormat = pstMainTrack->eAudioStreamType;*/
               }
               else
               {
                  pstAudioProcessing->eOutputFormat = eAVCLUSTER_AUDIO_STREAM_MPEG;
               }
            }
            else
            {
               pstAudioProcessing->eOutputFormat = eAVCLUSTER_AUDIO_STREAM_MPEG;
            }
         }
         else
         {
            bReturnValue = FALSE;
         }
         break;
      case AVCLUSTER_OUTPUT_TYPE_ANALOG:
         pstAudioProcessing->eOutputFormat = eAVCLUSTER_AUDIO_STREAM_MPEG;
         break;
      default:
         bReturnValue = FALSE;
         break;
   }
   return bReturnValue;
}


/*===========================================================================
 *
 * AVCLUSTER_CFG_GetAudioOutputConfiguration
 *
 * Parameters:
 *		pstUserCommonPref IN:				Common output preferences
 *		pstUserPref IN:						output preferences depending on an output type
 *		pstMainTrack IN: 					Main track
 *		pstADTrack 	IN: 					AD Track
 * 		pstAudioOutputConfiguration IN/OUT:	Audio output configuration
 *
 * Description:
 *
 *
 * Returns:
 *      TRUE 	if Audio configuration Found
 * 		else FALSE
 *
 *===========================================================================*/
bool AVCLUSTER_CFG_GetAudioOutputConfiguration(
								int8_t											cOutputId,
								tAVCLUSTER_AUDIO_UserCommonOutputPreferences 	*pstUserCommonPref,
								tAVCLUSTER_AUDIO_UserOutputPreferences 			*pstUserPref,
								tAVCLUSTER_AUDIO_Track 							*pstMainTrack,
								tAVCLUSTER_AUDIO_Track 							*pstADTrack,
								tAVCLUSTER_AUDIO_OutputConfiguration			*pstAudioOutputConfiguration)
{
	bool bReturnValue = TRUE;

	if (
		(pstUserCommonPref == NULL) ||
		(pstUserPref == NULL) ||
		(pstMainTrack == NULL) ||
		(pstADTrack == NULL) ||
		(pstAudioOutputConfiguration == NULL)
		)
	{
		/* Bad parameter */
		bReturnValue = FALSE;
	}

	if (bReturnValue == TRUE)
	{
		if(pstMainTrack->eAudioStreamType == eAVCLUSTER_AUDIO_STREAM_NONE)
		{
			/* No Stream */
			bReturnValue = FALSE;
		}
		else if(pstADTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_NONE)
		{
			/*Audio description Activated*/
			if(pstMainTrack->eAudioStreamType != pstADTrack->eAudioStreamType)
			{
				/* Error not the same stream type for Main and AD */
				bReturnValue = FALSE;
			}
			else
			{
				/* Audio Description OK but not managed yet */
				pstAudioOutputConfiguration->cOutputId 				= cOutputId;
				pstAudioOutputConfiguration->ucOutputDualMode 		= pstMainTrack->ucDualMode;
				pstAudioOutputConfiguration->ucOutputVolume 		= pstUserCommonPref->ucGlobalVolume;
				pstAudioOutputConfiguration->usOutputAudioLatency 	= pstUserPref->usAudioLatency;
				/* Force MixLevel to Requested MixLevel */
				pstAudioOutputConfiguration->ucMixLevel = pstUserCommonPref->ucMixLevel;
				if (pstMainTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_MPEG)
				{
					pstAudioOutputConfiguration->ucAudioVolumeAdjustment = pstUserCommonPref->ucAudioVolumeAdjustment;
				}
				else
				{
					/* No Adjustment */
					pstAudioOutputConfiguration->ucAudioVolumeAdjustment = 100;
				}
			}
		}
		else
		{
			/* Main Track Only */
			pstAudioOutputConfiguration->cOutputId 				= cOutputId;
			pstAudioOutputConfiguration->ucOutputDualMode 		= pstMainTrack->ucDualMode;
			pstAudioOutputConfiguration->ucOutputVolume 		= pstUserCommonPref->ucGlobalVolume;
			pstAudioOutputConfiguration->usOutputAudioLatency 	= pstUserPref->usAudioLatency;
			/* Force MixLevel to 100 -> No AD */
			pstAudioOutputConfiguration->ucMixLevel = 100;
			if (pstMainTrack->eAudioStreamType != eAVCLUSTER_AUDIO_STREAM_MPEG)
			{
				pstAudioOutputConfiguration->ucAudioVolumeAdjustment = pstUserCommonPref->ucAudioVolumeAdjustment;
			}
			else
			{
				/* No Adjustment */
				pstAudioOutputConfiguration->ucAudioVolumeAdjustment = 100;
			}

			/* Ignored for the moment */
/*			pstUserPref->ucAudioOutputType
			pstUserPref->ucGlobalVolumeFixedOrVariable
			pstUserCommonPref->ucCleanAudio
*/
		}
	}

	return bReturnValue;
}

/******************************************************************************
 * Function Name : AV_CLUSTER_CFGi_WindowUseAFD_HD
 *
 * Description   : Use AFD
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
LOCAL bool AVCLUSTER_CFGi_WindowUseAFD_HD(
			tAVCLUSTER_VIDEO_StreamContent			*pstStreamContent,
			tAVCLUSTER_VIDEO_UserOutputPreferences	*pstUserPref,
			tAVCLUSTER_VIDEO_Processing 			*pstVideoProcessing
			)
{
#ifdef PREFER_USER_PREFERENCES_OVER_AFD
    pstVideoProcessing->eWSS = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;

    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;

    switch (cmSetup_GetVideoTvFormat())
    {
        case eSETUP_ASPECT_RATIO_4_3:
        case eSETUP_ASPECT_RATIO_4_3_LETTERBOX:
        case eSETUP_ASPECT_RATIO_4_3_PANSCAN:
            if(pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
            {
                pstVideoProcessing->eDecFC = pstUserPref->eARConversion;
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;
        case eSETUP_ASPECT_RATIO_16_9:
        case eSETUP_ASPECT_RATIO_16_9_PILLAR_BOX:
        case eSETUP_ASPECT_RATIO_16_9_PANSCAN:
            if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
            {
                pstVideoProcessing->eDecFC = pstUserPref->eARConversion;
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;
    }
#else /* PREFER_USER_PREFERENCES_OVER_AFD */
	pstVideoProcessing->eWSS = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
	pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
	pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;

    switch (pstStreamContent->eAFD)
    {
        case 2: /* Box 16:9 (usTop) 0x0010 */
	        if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                }
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;

        case 3: /* Box 14:9 (usTop) 0x0011 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                }
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;

        case 4: /* Box > 16:9 (centre) 0x0100 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_CENTER_CUT_OUT;
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;

        case 9: /* 4:3 (centre) 0x1001 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_PILLAR_BOX;
                }
            }
            else
            {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WZ43IN169;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ST43IN169;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                }
            }
            break;

        case 10: /* 16:9 (centre) 0x1010*/
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_CENTER_CUT_OUT;
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;

        case 11: /* 14:9 (centre) 0x1011 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_PILLAR_BOX;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9;
                }
            }
            else
            {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_14_9;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ST149IN169;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                }
            }
            break;

        case 13: /* 4:3 (with shoot & protect 14:9 centre) 0x1101 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_ZOOM;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9;
                }
            }
            else
            {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WZ43SP149IN169;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ST43SP149IN169;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_ZOOM;
                }
            }
            break;

        case 14: /* 16:9 (with shoot & protect 14:9 centre) 0x1110*/
			if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_CENTER_CUT_OUT;
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;

        case 15: /* 16:9 (with shoot & protect 4:3 centre) 0x1111 */
			if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_CENTER_CUT_OUT;
            }
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;

        case 8: /* Active format is the same as the coded frame 0x1000 */
        default:
			if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9;
                }
                else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9)
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                }
                else
                {
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_PILLAR_BOX;
                }
            }
		    else if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_UNKNOWN)
		    {
				pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_SQUARE;

			}
            else
            {
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
            }
            break;
	}
#endif /* PREFER_USER_PREFERENCES_OVER_AFD */
	return TRUE;
}

/******************************************************************************
 * Function Name : AV_OUTPUTi_WindowUseAFD_SD
 *
 * Description   : Use AFD
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
LOCAL bool AVCLUSTER_CFGi_WindowUseAFD_SD(
			tAVCLUSTER_VIDEO_StreamContent			*pstStreamContent,
			tAVCLUSTER_VIDEO_UserOutputPreferences	*pstUserPref,
			tAVCLUSTER_VIDEO_Processing 			*pstVideoProcessing)
{
#ifdef PREFER_USER_PREFERENCES_OVER_AFD
    pstVideoProcessing->eWSS = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT;
    pstVideoProcessing->eDecFC = pstUserPref->eARConversion;
    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;

    switch (cmSetup_GetVideoTvFormat())
    {
        case eSETUP_ASPECT_RATIO_4_3:
        case eSETUP_ASPECT_RATIO_4_3_LETTERBOX:
        case eSETUP_ASPECT_RATIO_4_3_PANSCAN:
            pstStreamContent->eStreamAspectRatio = eAVCLUSTER_AR_16_9;
            break;
        case eSETUP_ASPECT_RATIO_16_9:
        case eSETUP_ASPECT_RATIO_16_9_PILLAR_BOX:
        case eSETUP_ASPECT_RATIO_16_9_PANSCAN:
            pstStreamContent->eStreamAspectRatio =  eAVCLUSTER_AR_4_3;
            break;
    }
#else /* PREFER_USER_PREFERENCES_OVER_AFD */
	switch (pstStreamContent->eAFD)
    {
        case 2: /* Box 16:9 (usTop) 0x0010 */
	        if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_TOP; /* 0010 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX)
				{
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
				else if(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER; /* 1000 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 3: /* Box 14:9 (usTop) 0x0011 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_LETTERBOX_TOP; /* 0100 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT))
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 4: /* Box > 16:9 (centre) 0x0100 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 		= eAVCLUSTER_VIDEO_HIGHER_THAN_16_9_LETTERBOX_CENTER; /* 1011 */
                pstVideoProcessing->eDecFC 			= eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_HIGHER_THAN_16_9_LETTERBOX_CENTER; /* 1011 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
				else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER;
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC 		= eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 9: /* 4:3 (centre) 0x1001 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            break;

        case 10: /* 16:9 (centre) 0x1010*/
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
                (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX) ||
				(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT))
				{
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
                else
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 11: /* 14:9 (centre) 0x1011 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER; /* 1000 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
				else if((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
						(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX))
				{
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER; /* 1000 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
                else
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 13: /* 4:3 (with shoot & protect 14:9 centre) 0x1101 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_FULL_FORMAT; /* 0111 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX) ||
                    (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT))
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_FULL_FORMAT; /* 0111 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
            }
            break;

        case 14: /* 16:9 (with shoot & protect 14:9 centre) 0x1110*/
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
#if 0 /* Possible option (Preferred from DTG) but not compatible with MHEG (?)*/
                if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
                    (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT))
                {
	                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER; /* 1000 */
	                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_14_9;
	                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
				else
				{
	                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
	                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
	                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
#endif
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX))
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER; /*1000*/
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 15: /* 16:9 (with shoot & protect 4:3 centre) 0x1111 */
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
#if 0 /* Possible option (Preferred from DTG) but not compatible with MHEG (?) */
                if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
                    (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT))
                {
	                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
	                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_4_3;
	                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
				else
				{
	                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
	                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
	                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
#endif
	                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
	                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
	                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
            else
            {
                if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_14_9_LETTERBOX_CENTER; /* 1000 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;

        case 8: /* Active format is the same as the coded frame 0x1000 */
        default:
		    if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3)
		    {
                pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
            }
		    else if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_UNKNOWN)
		    {
				if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT))
				{
					pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_SQUARE;
					pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
				}
				else
				{
					pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 0001 */
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_SQUARE;
					pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
				}

			}
            else
            {	/* Stream 16_9 */
                if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_LETTERBOX_CENTER; /* 1101 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
				else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 1000 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;
                }
                else
                {
                    pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT; /* 1110 */
                    pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
                    pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
                }
            }
            break;
	}
#endif /* PREFER_USER_PREFERENCES_OVER_AFD */
	return TRUE;
}

/******************************************************************************
 * Function Name : AVCLUSTER_CFGi_ComputeDecFC_Default_SD
 *
 * Description   : Compute decoder Format conversion
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
LOCAL bool AVCLUSTER_CFGi_ComputeDecFC_Default_SD(
								tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
								tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
								tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
								tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing)
{

	if (pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_NO_SOURCE)
	{
		/* No Video */
		pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
	}
	else if (pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_SOURCE_LIVE)
	{
/*		printf("Video Live\r\n");*/
		if (pstUserPref->ucFullScreen == 0)
		{
/*			printf("Video Scaling requested\r\n");*/
			if ((pstVideoProcessing->eWSS == eAVCLUSTER_VIDEO_4_3_FULL_FORMAT) &&
				(pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9))
			{
/*				printf("WSS 4/3 and Stream 16/9 -> CCO\r\n");*/
				pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
			}
			else if ((pstVideoProcessing->eWSS == eAVCLUSTER_VIDEO_16_9_FULL_FORMAT) &&
				(pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9))
			{
				if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT)
				{
/*					printf("WSS 16/9 and Stream 16/9 -> CCO\r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
				}
				else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX)
				{
/*					printf("WSS 16/9 and Stream 16/9 -> 16/9 LB\r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX;
				}
				else if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX)
				{
/*					printf("WSS 16/9 and Stream 16/9 -> 14/9 LB\r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX;
				}
				else
				{
/*					printf("WSS 4/3 and Stream 16/9 -> Full Screen\r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
				}
			}
			else if ((pstVideoProcessing->eWSS == eAVCLUSTER_VIDEO_16_9_FULL_FORMAT) &&
				(pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_4_3))
			{
				if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX) ||
					(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX))
				{
/*					printf("WSS 16/9 and Stream 4/3 -> Full Screen\r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
				}
				else
				{
/*					printf("WSS 16/9 and Stream 4/3 -> 4_3 PB\r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_PILLAR_BOX;
				}
			}
			else
			{
/*				printf(" -> Full Screen\r\n");*/
				pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
			}
		}
		else
		{
/*			printf("Video Full Screen\r\n");*/
			if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_NORMAL)
		    {
/*				printf("No Scene defined use AFD\r\n");*/
				return FALSE;
			}
			else
			{
				if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_4_3_SCENE)
        		{
/*					printf("Scene 4/3\r\n");*/
					if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
					{
/*						printf("Stream 16/9 use Forced DecFC\r\n");*/
						pstVideoProcessing->eDecFC = pstUserPref->eForcedDecFC;
					}
					else
					{
/*						printf("Stream 4/3 -> Full Screen \r\n");*/
						pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
					}
				}
				else
				{
/*					printf("Scene 16/9 -> Full Screen \r\n");*/
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
				}
			}	
		}		
	}
	else if (pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_SOURCE_SAMPLE)
	{
/*		printf("IFrame -> DecFC = Fullscreen\r\n");*/
		pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;	
	}

	return TRUE;
}

/******************************************************************************
 * Function Name : AVCLUSTER_CFGi_ComputeDecFC_MHEG_SD
 *
 * Description   : Compute decoder Format conversion
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
LOCAL bool AVCLUSTER_CFGi_ComputeDecFC_MHEG_SD(
								tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
								tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
								tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
								tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing)
{
	uint32_t					OutputWinWidth = 0;
	uint32_t					OutputWinHeight = 0;

	switch(pstUserPref->eOutputVideoStandard)
	{
		case    eAVCLUSTER_VIDEO_STD_640_480_P_60     :
			OutputWinWidth = 640;
			OutputWinHeight = 480;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_480_P_5994   :
		case    eAVCLUSTER_VIDEO_STD_720_480_P_60     :
		case    eAVCLUSTER_VIDEO_STD_720_480_I_5994   :
		case    eAVCLUSTER_VIDEO_STD_720_480_I_60     :
			OutputWinWidth = 720;
			OutputWinHeight = 480;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_483_P_5994   :
			OutputWinWidth = 720;
			OutputWinHeight = 483;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_576_P_50     :
		case    eAVCLUSTER_VIDEO_STD_720_576_I_50     :
			OutputWinWidth = 720;
			OutputWinHeight = 576;
			break;
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_2398  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_24    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_25    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_2997  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_30    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_50    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_5994  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_60    :
			OutputWinWidth = 1280;
			OutputWinHeight = 720;
			break;
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_2398 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_24   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_25   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_2997 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_30   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_50   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_5994 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_60   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_50   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_5994 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_60   :
			OutputWinWidth = 1920;
			OutputWinHeight = 1080;
			break;
		case    eAVCLUSTER_VIDEO_STD_NONE             :
		default:
			OutputWinWidth = 720;
			OutputWinHeight = 576;
			break;
	}

	if (pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_NO_SOURCE)
	{
		/* No Video */
		pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
	}
	else if (pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_SOURCE_LIVE)
	{
		/* Video Live */
		if ((pstUserPref->ucFullScreen == 0) &&
			(
			((pstUserPref->stOutputWindow.sRight - pstUserPref->stOutputWindow.sLeft + 1) == OutputWinWidth/2) &&
			((pstUserPref->stOutputWindow.sBottom - pstUserPref->stOutputWindow.sTop + 1) == OutputWinHeight/2)
			)
			)
		{
			/* Quarter screen */
			if ((pstVideoProcessing->eWSS == eAVCLUSTER_VIDEO_4_3_FULL_FORMAT) &&
				(pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9))
			{
				/* WSS 4/3 and Stream 16/9 -> CCO */
				pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT;
			}
			else
			{
				/*  -> Full Screen */
				pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;	
			}
		}
		else
		{
			/* greater than quarter screen */
			if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_NORMAL)
		    {
				/* No Scene defined use AFD */
				return FALSE;
			}
			else
			{
				if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_4_3_SCENE)
        		{
					/* Scene 4/3 */
					if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
					{
						/* Stream 16/9 use wide screen set by mheg */
						pstVideoProcessing->eDecFC = pstUserPref->eForcedDecFC;
					}
					else
					{
						/* Stream 4/3 -> Full Screen */
						pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;	
					}
				}
				else
				{
					/* Scene 16/9 -> Full Screen */
					pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;	
				}
			}	
		}		
	}
	else if (pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_SOURCE_SAMPLE)
	{
		/* IFrame -> DecFC = Fullscreen */
		pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;	
	}

	return TRUE;
}




LOCAL bool AVCLUSTER_CFGi_ComputeWSS_Default_SD(
						tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
						tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
						tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
						tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing)
{

	

	if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_4_3_SCENE)
    {
		/* Scene 4/3  WSS = 4/3 */
		pstVideoProcessing->eWSS	 			= eAVCLUSTER_VIDEO_4_3_FULL_FORMAT;
        pstVideoProcessing->eOutputAspectRatio 	= eAVCLUSTER_OUTPUT_AR_4_3;
	}
	else if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_16_9_SCENE)
    {
		/* Scene 16/9   WSS = 16/9 */
		pstVideoProcessing->eWSS	 			= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
        pstVideoProcessing->eOutputAspectRatio 	= eAVCLUSTER_OUTPUT_AR_16_9;
	}
	else
	{
		/* no Scene defined */
		if ((pstUserPref->ucFullScreen == 0) && (pstStreamContent->eVideoSourceType != eAVCLUSTER_VIDEO_SOURCE_SAMPLE))
		{
			 /* quarter screen	 wss = Display aspect ratio */
			if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN)
			{
				/* WSS = 16/9 */
				pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
			}
			else
			{
				/* WSS = 4/3 */
				pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;			
			}
		}
		else if (pstStreamContent->eVideoSourceType != eAVCLUSTER_VIDEO_SOURCE_LIVE)
		{
			/* No live video   wss = Display aspect ratio */
			if (pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN)
			{
				/* WSS = 16/9 */
				pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
			}
			else
			{
				/* WSS = 4/3 */
				pstVideoProcessing->eWSS	 	= eAVCLUSTER_VIDEO_4_3_FULL_FORMAT;
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;			
			}
		}
		else
		{
			/* Video Live greater than quarter screen -> use AFD */
			return FALSE;
		}
	}

	return TRUE;
}


LOCAL bool AVCLUSTER_CFGi_ComputeWSS_MHEG_SD(
						tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
						tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
						tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
						tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing)
{
	uint32_t					OutputWinWidth = 0;
	uint32_t					OutputWinHeight = 0;

	switch(pstUserPref->eOutputVideoStandard)
	{
		case    eAVCLUSTER_VIDEO_STD_640_480_P_60     :
			OutputWinWidth = 640;
			OutputWinHeight = 480;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_480_P_5994   :
		case    eAVCLUSTER_VIDEO_STD_720_480_P_60     :
		case    eAVCLUSTER_VIDEO_STD_720_480_I_5994   :
		case    eAVCLUSTER_VIDEO_STD_720_480_I_60     :
			OutputWinWidth = 720;
			OutputWinHeight = 480;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_483_P_5994   :
			OutputWinWidth = 720;
			OutputWinHeight = 483;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_576_P_50     :
		case    eAVCLUSTER_VIDEO_STD_720_576_I_50     :
			OutputWinWidth = 720;
			OutputWinHeight = 576;
			break;
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_2398  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_24    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_25    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_2997  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_30    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_50    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_5994  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_60    :
			OutputWinWidth = 1280;
			OutputWinHeight = 720;
			break;
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_2398 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_24   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_25   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_2997 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_30   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_50   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_5994 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_60   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_50   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_5994 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_60   :
			OutputWinWidth = 1920;
			OutputWinHeight = 1080;
			break;
		case    eAVCLUSTER_VIDEO_STD_NONE             :
		default:
			OutputWinWidth = 720;
			OutputWinHeight = 576;
			break;
	}

	if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_4_3_SCENE)
    {
		/* Scene 4/3  WSS = 4/3 */
		pstVideoProcessing->eWSS	 			= eAVCLUSTER_VIDEO_4_3_FULL_FORMAT;
        pstVideoProcessing->eOutputAspectRatio 	= eAVCLUSTER_OUTPUT_AR_4_3;
	}
	else if (pstUserCommonPref->eSceneMode == eAVCLUSTER_OSD_MODE_16_9_SCENE)
    {
		/* Scene 16/9   WSS = 16/9 */
		pstVideoProcessing->eWSS	 		= eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
	}
	else
	{
		/* no Scene defined */
		if (((pstUserPref->ucFullScreen == 0) && (pstStreamContent->eVideoSourceType != eAVCLUSTER_VIDEO_SOURCE_SAMPLE)) &&
			(
			((pstUserPref->stOutputWindow.sRight - pstUserPref->stOutputWindow.sLeft + 1) == OutputWinWidth/2) &&
			((pstUserPref->stOutputWindow.sBottom - pstUserPref->stOutputWindow.sTop + 1) == OutputWinHeight/2)
			)
		)
		{
			 /* quarter screen	 wss = Display aspect ratio */
			if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN) ||
				(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9) ||
				(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9))
			{
				/* WSS = 16/9 */
				pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
			}
			else
			{
				/* WSS = 4/3 */
				pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT; /* 0001 */
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;			
			}
		}
		else if (pstStreamContent->eVideoSourceType != eAVCLUSTER_VIDEO_SOURCE_LIVE)
		{
			/* No live video   wss = Display aspect ratio */
			if ((pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN) ||
				(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9) ||
				(pstUserPref->eARConversion == eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9))
			{
				/* WSS = 16/9 */
				pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
			}
			else
			{
				/* WSS = 4/3 */
				pstVideoProcessing->eWSS	 = eAVCLUSTER_VIDEO_4_3_FULL_FORMAT;
		        pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_4_3;			
			}
		}
		else
		{
			/* Video Live greater than quarter screen -> use AFD */
			return FALSE;
		}
	}

	return TRUE;
}

bool AVCLUSTER_CFG_GetVideoProcessing(
			tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
			tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
			tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
			tAVCLUSTER_VIDEO_Output							*pstOutput,
			tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing)
{
	bool returnValue = TRUE;

	pstVideoProcessing->cOutputId = pstOutput->cOutputId;

	/* Is SD or HD output processing requested */
	if ((pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDDAC) ||
		(pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDMI))
	{
		/* Is Embedded or MHEG rules processing requested */	
		if (pstUserCommonPref->eRules == eAVCLUSTER_VIDEO_MHEG_RULES)
		{
			if (AVCLUSTER_CFGi_ComputeWSS_MHEG_SD(pstStreamContent, pstUserCommonPref, pstUserPref, pstVideoProcessing) == FALSE)
			{
				AVCLUSTER_CFGi_WindowUseAFD_SD(pstStreamContent, pstUserPref, pstVideoProcessing);
			}
			if (AVCLUSTER_CFGi_ComputeDecFC_MHEG_SD(pstStreamContent, pstUserCommonPref, pstUserPref, pstVideoProcessing) == FALSE)
			{
				AVCLUSTER_CFGi_WindowUseAFD_SD(pstStreamContent, pstUserPref, pstVideoProcessing);
			}
		}
		else
		{
    		if((pstUserPref->ucFullScreen == 1) &&
    			(pstStreamContent->eVideoSourceType == eAVCLUSTER_VIDEO_SOURCE_SAMPLE))
    		{
    			/* Full screen Picture sample played on HD output -> Display Full Screen */
    			pstVideoProcessing->eWSS = eAVCLUSTER_VIDEO_16_9_FULL_FORMAT;
    			pstVideoProcessing->eOutputAspectRatio = eAVCLUSTER_OUTPUT_AR_16_9;
    			pstVideoProcessing->eDecFC = eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN;
    		}
    		else
    		{
    			/* use AFD */
    			returnValue = AVCLUSTER_CFGi_WindowUseAFD_HD(pstStreamContent, pstUserPref, pstVideoProcessing);
    		}
        }
	}
	else
	{
		/* Is Embedded or MHEG rules processing requested */	
		if (pstUserCommonPref->eRules == eAVCLUSTER_VIDEO_MHEG_RULES)
		{
			if (AVCLUSTER_CFGi_ComputeWSS_MHEG_SD(pstStreamContent, pstUserCommonPref, pstUserPref, pstVideoProcessing) == FALSE)
			{
				AVCLUSTER_CFGi_WindowUseAFD_SD(pstStreamContent, pstUserPref, pstVideoProcessing);
			}
			if (AVCLUSTER_CFGi_ComputeDecFC_MHEG_SD(pstStreamContent, pstUserCommonPref, pstUserPref, pstVideoProcessing) == FALSE)
			{
				AVCLUSTER_CFGi_WindowUseAFD_SD(pstStreamContent, pstUserPref, pstVideoProcessing);
			}
		}
		else
		{
			if (AVCLUSTER_CFGi_ComputeWSS_Default_SD(pstStreamContent, pstUserCommonPref, pstUserPref, pstVideoProcessing) == FALSE)
			{
				AVCLUSTER_CFGi_WindowUseAFD_SD(pstStreamContent, pstUserPref, pstVideoProcessing);
			}
			if (AVCLUSTER_CFGi_ComputeDecFC_Default_SD(pstStreamContent, pstUserCommonPref, pstUserPref, pstVideoProcessing) == FALSE)
			{
				AVCLUSTER_CFGi_WindowUseAFD_SD(pstStreamContent, pstUserPref, pstVideoProcessing);
			}
		}
	}
	
	return returnValue;
}

LOCAL void AVCLUSTER_CFGi_VideoWindowSquareCompute
(
	int16_t											outputWidth,
	int16_t											outputHeight,
	tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent,
	tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,
	tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing,
	tAVCLUSTER_VIDEO_Configuration					*pstVideoOuputConfiguration
)
{
    double                   dWRatio;
    double                   dHRatio;
    double                   dWidth;
    double                   dHeight;
    uint16_t                 dispWidth;
    uint16_t                 dispHeight;
    uint16_t                 pictWidth;
    uint16_t                 pictHeight;
    double                   dPictRatio = 0;
    uint16_t                 outWidth;
    uint16_t                 outHeight;
    uint16_t                 outLeft;
    uint16_t                 outTop;
    uint16_t                 outHeightScale;

	dispWidth  = outputWidth;
	dispHeight = outputHeight;
		
	pictWidth  = pstStreamContent->stResolution.uiWidth;
	pictHeight = pstStreamContent->stResolution.uiHeight;

	
	/* picture resolution available ? */
    if((pictWidth > 0) && (pictHeight > 0))
    {
		/* is size of display bigger than the picture  ? */
        if((dispWidth  > pictWidth) && (dispHeight > pictHeight))
        {
			/* ratio between display and picture */
            dWRatio = (double)dispWidth  / (double)pictWidth;
            dHRatio = (double)dispHeight / (double)pictHeight;

			/* picture ratio */
			dPictRatio = (double)pictWidth / (double)pictHeight;

            /* we adapt to the lower ratio */
            if(dWRatio > dHRatio)
            {
                dWidth  = (double)pictWidth * dHRatio;
                dHeight = (double)dispHeight;
                outLeft = ((dispWidth - (uint16_t)dWidth) / 2);
                outTop  = 0;
            }
            else
            {
                dWidth  = (double)dispWidth;
                dHeight = (double)pictHeight * dWRatio;
                outLeft = 0;
                outTop  = ((dispHeight - (uint16_t)dHeight) / 2);
            }
        }
        else
        {
            dWRatio = (double)pictWidth  / (double)dispWidth;
            dHRatio = (double)pictHeight / (double)dispHeight;

            /* we adapt to the bigger ratio */
            if(dHRatio > dWRatio)
            {
                dWidth  = (double)pictWidth / dHRatio;
                dHeight = (double)dispHeight;
                outLeft = ((dispWidth - (uint16_t)dWidth) / 2);
                outTop  = 0;
            }
            else
            {
                dWidth  = (double)dispWidth;
                dHeight = (double)pictHeight / dWRatio;
                outLeft = 0;
                outTop  = ((dispHeight - (uint16_t)dHeight) / 2);
            }
        }

        outWidth  = (uint16_t)dWidth;
        outHeight = (uint16_t)dHeight;

		/* check AR display versus picture */
		switch(pstVideoProcessing->eOutputAspectRatio)
		{
			case eAVCLUSTER_OUTPUT_AR_16_9:
				if(dPictRatio > (double)(1.777))
				{
					outHeightScale = (outHeight * 4) / 3;
					if(outHeightScale > dispHeight)
					{
						outHeightScale = dispHeight;
					}
					outTop    = outTop - ((outHeightScale - outHeight) / 2);
					outHeight = outHeightScale;
				}
				break;
	
			default:
			case eAVCLUSTER_OUTPUT_AR_4_3:
				break;
		}

		pstVideoOuputConfiguration->stInputWindow.sLeft 	= 0;
		pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
		pstVideoOuputConfiguration->stInputWindow.sRight 	= pictWidth - 1;
		pstVideoOuputConfiguration->stInputWindow.sBottom 	= pictHeight - 1;

		pstVideoOuputConfiguration->stOutputWindow.sLeft 	= outLeft;
		pstVideoOuputConfiguration->stOutputWindow.sTop 	= outTop;
		pstVideoOuputConfiguration->stOutputWindow.sRight 	= outWidth  + outLeft - 1;
		pstVideoOuputConfiguration->stOutputWindow.sBottom 	= outHeight + outTop - 1;
    }
}

bool AVCLUSTER_CFG_GetVideoOutputConfiguration(
			tAVCLUSTER_VIDEO_StreamContent					*pstStreamContent, 		/* Video Resolution and Video Aspect Ratio */
			tAVCLUSTER_VIDEO_UserCommonOutputPreferences	*pstUserCommonPref,
			tAVCLUSTER_VIDEO_UserOutputPreferences			*pstUserPref,			/* output resolution */
			tAVCLUSTER_VIDEO_Output							*pstOutput,				/* To check resolution capabilities */
			tAVCLUSTER_VIDEO_Processing						*pstVideoProcessing,	/* Decoder conversion */
			tAVCLUSTER_VIDEO_Configuration					*pstVideoOuputConfiguration)
{
	tAVCLUSTER_VIDEO_Window 	tempWindow;
	bool						bUseCenteredInputWidth = FALSE;
	uint16_t					inputWidthToUse = 0;
	int16_t						leftPixelToRemove = 0;
	int16_t						leftPixelToAdd = 0;
	bool						bUse702CenteredOutputWidth = FALSE;
	bool						bUseInputWidth720InsteadOf704 = FALSE;

	uint32_t	InputWinWidth = pstStreamContent->stResolution.uiWidth;
	uint32_t	InputWinHeight = pstStreamContent->stResolution.uiHeight;

	uint32_t					OutputWinWidth = 0;
	uint32_t					OutputWinHeight = 0;


	switch(pstUserPref->eOutputVideoStandard)
	{
		case    eAVCLUSTER_VIDEO_STD_640_480_P_60     :
			OutputWinWidth = 640;
			OutputWinHeight = 480;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_480_P_5994   :
		case    eAVCLUSTER_VIDEO_STD_720_480_P_60     :
		case    eAVCLUSTER_VIDEO_STD_720_480_I_5994   :
		case    eAVCLUSTER_VIDEO_STD_720_480_I_60     :
			OutputWinWidth = 720;
			OutputWinHeight = 480;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_483_P_5994   :
			OutputWinWidth = 720;
			OutputWinHeight = 483;
			break;
		case    eAVCLUSTER_VIDEO_STD_720_576_P_50     :
		case    eAVCLUSTER_VIDEO_STD_720_576_I_50     :
			OutputWinWidth = 720;
			OutputWinHeight = 576;
			break;
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_2398  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_24    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_25    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_2997  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_30    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_50    :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_5994  :
		case    eAVCLUSTER_VIDEO_STD_1280_720_P_60    :
			OutputWinWidth = 1280;
			OutputWinHeight = 720;
			break;
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_2398 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_24   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_25   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_2997 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_30   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_50   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_5994 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_P_60   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_50   :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_5994 :
		case    eAVCLUSTER_VIDEO_STD_1920_1080_I_60   :
			OutputWinWidth = 1920;
			OutputWinHeight = 1080;
			break;
		case    eAVCLUSTER_VIDEO_STD_NONE             :
		default:
			OutputWinWidth = 720;
			OutputWinHeight = 576;
			break;
	}


	if (
            ((pstStreamContent->bCopyProtection == TRUE) && (pstOutput->bHDCPCapable == FALSE)) &&
                ((pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDDAC) ||
		        (pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDMI))
        )
    {
		tempWindow.sRight = 719;
		tempWindow.sBottom = 575;
		pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_P_50;
    }
    else if ((pstUserPref->eOutputResolutionMode == eAVCLUSTER_VIDEO_OUTPUT_RESOLUTION_MODE_AUTO) &&
		((pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDDAC) ||
		(pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDMI)))
	{
        /* Forced Resolution only -> Auto resolution */
#ifdef AUTO_RESOLUTION_DEPENDING_ON_INPUT_RESOLUTION
		if ((pstOutput->eSinkResolutionSupported != eAVCLUSTER_VIDEO_STD_NONE) &&
			(pstOutput->eSinkResolutionSupported != 0xFFFFFFFF))
		{
			/* Choose the resolution corresponding to the input resolution */
			if (InputWinWidth <=720)
			{
				/*SD input video*/
				if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_P_50) == eAVCLUSTER_VIDEO_STD_1920_1080_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_P_50) == eAVCLUSTER_VIDEO_STD_1920_1080_P_50))
				{
					tempWindow.sRight = 1919;
					tempWindow.sBottom = 1079;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50))
				{
					tempWindow.sRight = 719;
					tempWindow.sBottom = 575;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_I_50) == eAVCLUSTER_VIDEO_STD_720_576_I_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_I_50) == eAVCLUSTER_VIDEO_STD_720_576_I_50))
				{
					tempWindow.sRight = 719;
					tempWindow.sBottom = 575;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_I_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50))
				{
					tempWindow.sRight = 1279;
					tempWindow.sBottom = 719;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1280_720_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50))
				{
					tempWindow.sRight = 1919;
					tempWindow.sBottom = 1079;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_I_50;
				}
				else
				{
					tempWindow.sRight = OutputWinWidth - 1;
					tempWindow.sBottom = OutputWinHeight - 1;
					pstVideoOuputConfiguration->eOutputVideoStandard = pstUserPref->eOutputVideoStandard;
				}
			}
			else if (InputWinWidth <=1280)
			{
				if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_P_50) == eAVCLUSTER_VIDEO_STD_1920_1080_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_P_50) == eAVCLUSTER_VIDEO_STD_1920_1080_P_50))
				{
					tempWindow.sRight = 1919;
					tempWindow.sBottom = 1079;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50))
				{
					tempWindow.sRight = 1279;
					tempWindow.sBottom = 719;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1280_720_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50))
				{
					tempWindow.sRight = 1919;
					tempWindow.sBottom = 1079;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_I_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50))
				{
					tempWindow.sRight = 719;
					tempWindow.sBottom = 575;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_P_50;
				}
				else
				{
					tempWindow.sRight = OutputWinWidth - 1;
					tempWindow.sBottom = OutputWinHeight - 1;
					pstVideoOuputConfiguration->eOutputVideoStandard = pstUserPref->eOutputVideoStandard;
				}

			}
			else
			{
				if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_P_50) == eAVCLUSTER_VIDEO_STD_1920_1080_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_P_50) == eAVCLUSTER_VIDEO_STD_1920_1080_P_50))
				{
					tempWindow.sRight = 1919;
					tempWindow.sBottom = 1079;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50))
				{
					tempWindow.sRight = 1919;
					tempWindow.sBottom = 1079;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_I_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50))
				{
					tempWindow.sRight = 1279;
					tempWindow.sBottom = 719;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1280_720_P_50;
				}
				else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50) &&
					((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50))
				{
					tempWindow.sRight = 719;
					tempWindow.sBottom = 575;
					pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_P_50;
				}
				else
				{
					tempWindow.sRight = OutputWinWidth - 1;
					tempWindow.sBottom = OutputWinHeight - 1;
					pstVideoOuputConfiguration->eOutputVideoStandard = pstUserPref->eOutputVideoStandard;
				}				
			}
		}
#else
		if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50) &&
			((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1920_1080_I_50) == eAVCLUSTER_VIDEO_STD_1920_1080_I_50))
		{
			tempWindow.sRight = 1919;
			tempWindow.sBottom = 1079;
			pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1920_1080_I_50;
		}
		else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50) &&
			((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_1280_720_P_50) == eAVCLUSTER_VIDEO_STD_1280_720_P_50))
		{
			tempWindow.sRight = 1279;
			tempWindow.sBottom = 719;
			pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_1280_720_P_50;
		}
		else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50) &&
			((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_P_50) == eAVCLUSTER_VIDEO_STD_720_576_P_50))
		{
			tempWindow.sRight = 719;
			tempWindow.sBottom = 575;
			pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_P_50;
		}
		else if (((pstOutput->eSinkResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_I_50) == eAVCLUSTER_VIDEO_STD_720_576_I_50) &&
			((pstOutput->eResolutionSupported & eAVCLUSTER_VIDEO_STD_720_576_I_50) == eAVCLUSTER_VIDEO_STD_720_576_I_50))
		{
			tempWindow.sRight = 719;
			tempWindow.sBottom = 575;
			pstVideoOuputConfiguration->eOutputVideoStandard = eAVCLUSTER_VIDEO_STD_720_576_I_50;
		}

#endif
	
		
		else
		{
			tempWindow.sRight = OutputWinWidth - 1;
			tempWindow.sBottom = OutputWinHeight - 1;
			pstVideoOuputConfiguration->eOutputVideoStandard = pstUserPref->eOutputVideoStandard;
		}
	}
	else
	{
		tempWindow.sRight = OutputWinWidth - 1;
		tempWindow.sBottom = OutputWinHeight - 1;
		pstVideoOuputConfiguration->eOutputVideoStandard = pstUserPref->eOutputVideoStandard;
	}

	inputWidthToUse = InputWinWidth;

	if ((pstUserPref->ucFullScreen == 1) && 
		((pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDDAC) ||
		(pstOutput->ucVideoOutputType == AVCLUSTER_OUTPUT_TYPE_HDMI))
		)
	{
		/* SD Video on HD Output */
        if ((InputWinWidth <= 720) && (InputWinWidth != 704))
		{
			inputWidthToUse = InputWinWidth * 975 /1000; /* Take 97.5 % of Video centered Width due to "SD to HD" */
			leftPixelToRemove = (InputWinWidth - inputWidthToUse)/2;
			bUseCenteredInputWidth = TRUE;

/*
			approximation makes 343 instead of 344 
*/
			if(InputWinWidth == 352)
			{
				inputWidthToUse = 344;
				leftPixelToRemove = 4;
			}
        }
		else if (InputWinWidth == 704)
		{
			/* SD input Video is 704 width */
			bUseInputWidth720InsteadOf704  = TRUE;
			inputWidthToUse = 720;
			leftPixelToAdd = 8;
			leftPixelToRemove = 8;
		}
	}
	else
	{
		if((pstOutput->ucVideoOutputType != AVCLUSTER_OUTPUT_TYPE_HDDAC) &&
			(pstOutput->ucVideoOutputType != AVCLUSTER_OUTPUT_TYPE_HDMI))
		{
			if (InputWinWidth > 720)
			{
				/* HD Video on SD output */
				bUse702CenteredOutputWidth = TRUE;
			}
		}
	}

   /* the TV is 4/3 and the stream is 16/9*/
    switch (pstVideoProcessing->eDecFC)
    {
        case eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_CENTER_CUT_OUT:
            if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
            {
                /* apply pan and scan conversion from 16/9 to 4/3 */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= inputWidthToUse/8 + leftPixelToRemove - leftPixelToAdd;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + (3*inputWidthToUse)/4 - 1;

				pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= InputWinHeight - 1;


				if (bUse702CenteredOutputWidth == TRUE)
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
				}
				else
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
				}

				pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            else
            {
                /* apply pan and scan conversion from 4/3 to 16/9 */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

				pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/8;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + ((3*InputWinHeight)/4) - 1;

				if (bUse702CenteredOutputWidth == TRUE)
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
				}
				else
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
				}

				pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_16_9_LETTER_BOX:

			pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= InputWinHeight - 1;

            if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
            {
				if (bUse702CenteredOutputWidth == TRUE)
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
				}
				else
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
				}

                /* apply letter box conversion from 16/9 to 4/3 */
				pstVideoOuputConfiguration->stOutputWindow.sTop 	= (tempWindow.sBottom + 1)/8;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= pstVideoOuputConfiguration->stOutputWindow.sTop + (3*(tempWindow.sBottom  + 1))/4 - 1;
            }
            else
            {
                /* apply letter box conversion from 4/3 to 16/9 */
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= (tempWindow.sRight + 1)/8;
				pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= (3*(tempWindow.sRight + 1))/4 - 1;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_LETTER_BOX:
            if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
            {
                /* apply combined or compromise conversion 16/9 to 4/3 */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= inputWidthToUse/16 + leftPixelToRemove - leftPixelToAdd;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + (7*inputWidthToUse)/8 - 1;

				pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= InputWinHeight - 1;

				if (bUse702CenteredOutputWidth == TRUE)
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
				}
				else
				{
					pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
					pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
				}

				pstVideoOuputConfiguration->stOutputWindow.sTop 	= (tempWindow.sBottom + 1)/16;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= pstVideoOuputConfiguration->stOutputWindow.sTop + (7*(tempWindow.sBottom + 1))/8 -1;
            }
            else
            {
                /* apply combined or compromise conversion 4/3 to 16/9 */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

				pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/16;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (7*InputWinHeight)/8 - 1;

				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= (tempWindow.sRight + 1)/16;
				pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= pstVideoOuputConfiguration->stOutputWindow.sLeft + (7*(tempWindow.sRight + 1))/8 -1;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_4_3:
            /* apply zoom conversion of a 16:9 letterbox image           */
            /* shot & protect 4:3 in a 4:3 frame to 4/3 full frame image */
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= inputWidthToUse/8 + leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + (3*inputWidthToUse)/4 - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/8;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (3*InputWinHeight)/4 - 1;

			if (bUse702CenteredOutputWidth == TRUE)
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
			}
			else
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
			}

			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_14_9:
            	/* apply zoom conversion of a 16:9 letterbox image  */
            	/* shot & protect 14:9 in a 4:3 frame to 14/9 image */
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= inputWidthToUse/16 + leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + (7*inputWidthToUse)/8 - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/16;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (7*InputWinHeight)/8 - 1;

			if (bUse702CenteredOutputWidth == TRUE)
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
			}
			else
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
			}

			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            break;
		
		case eAVCLUSTER_VIDEO_AR_CONVERSION_SQUARE:
			/*eAspectRatio == eAV_STREAM_COMPONENT_VIDEO_AR_UNKNOWN */
			AVCLUSTER_CFGi_VideoWindowSquareCompute(
												tempWindow.sRight + 1,
												tempWindow.sBottom + 1,
												pstStreamContent,
												pstUserPref,
												pstVideoProcessing,
												pstVideoOuputConfiguration);
			break;

		case eAVCLUSTER_VIDEO_AR_CONVERSION_4_3_PILLAR_BOX:
			/* Stream is 4/3 Full Frame -> Add Black bars on Left and Right */
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= InputWinHeight - 1;

			pstVideoOuputConfiguration->stOutputWindow.sLeft 	= (tempWindow.sRight + 1)/8;
			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sRight 	= pstVideoOuputConfiguration->stOutputWindow.sLeft + (3*(tempWindow.sRight + 1))/4 - 1;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
			break;

		case eAVCLUSTER_VIDEO_AR_CONVERSION_14_9_PILLAR_BOX:
			/* Stream is 4/3 Frame with 14/9 image inside
				-> Extract 14/9 Image */
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/14;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (InputWinHeight/14)*12 - 1;

			/*-> And Display 14/9 on 16/9 Image by pillar boxing */
			pstVideoOuputConfiguration->stOutputWindow.sLeft 	= (tempWindow.sRight + 1)/16;
			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sRight 	= pstVideoOuputConfiguration->stOutputWindow.sLeft + ((tempWindow.sRight + 1)/16)*14 - 1;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
			break;

		case eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_CENTER_CUT_OUT:
			/* Stream is 4/3 Frame with 16/9 image inside
				-> Extract 16/9 Image */
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/8;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (InputWinHeight/4)*3 - 1;

			/* And display Full Screen */
			if (bUse702CenteredOutputWidth == TRUE)
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
			}
			else
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
			}

			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
			break;

		case eAVCLUSTER_VIDEO_AR_CONVERSION_WIDE_ZOOM:
            if (pstStreamContent->eStreamAspectRatio == eAVCLUSTER_AR_16_9)
            {
				tAVCLUSTER_VIDEO_Window 	interWindow;
				/* Stream is 16/9 Frame with 4/3 Image inside
					with Shoot & Protect 14/9 */
					/* -> Extract  4/3 Image of 16/9 frame */
				interWindow.sLeft	= inputWidthToUse/8 + leftPixelToRemove - leftPixelToAdd;
				interWindow.sRight	= (3*inputWidthToUse)/4 + interWindow.sLeft - 1;

	            interWindow.sTop	= 0;
				interWindow.sBottom = InputWinHeight + interWindow.sTop - 1;

					/* -> Extract  14/9 Image of 4/3 Image */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= interWindow.sLeft;
				pstVideoOuputConfiguration->stInputWindow.sTop 		= (interWindow.sBottom - interWindow.sTop + 1)/14;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= interWindow.sRight;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + ((interWindow.sBottom - interWindow.sTop + 1)/14)*12 - 1;
					
					/* And display zoomed */
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= (tempWindow.sRight + 1)/16;
				pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= pstVideoOuputConfiguration->stOutputWindow.sLeft + (7*(tempWindow.sRight + 1))/8 - 1;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
			}
			else
			{
				/* Stream is 4/3 Frame with 4/3 Image inside
					with Shoot & Protect 14/9 */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove- leftPixelToAdd;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

				pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/14;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (InputWinHeight/14)*12 - 1;

					/* And display zoomed */
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= (tempWindow.sRight + 1)/16;
				pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= pstVideoOuputConfiguration->stOutputWindow.sLeft + (7*(tempWindow.sRight + 1))/8 - 1;
				pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
			}
			break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_ZOOM_16_9:
			/* Stream is 4/3 TV is 16/9 => Cut the Up and down of the Picture => Zoom */
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove- leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;
			pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/8;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (InputWinHeight/4)*3 - 1;

			if (bUse702CenteredOutputWidth == TRUE)
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
			}
			else
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
			}

			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
			break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_WZ43IN169 :
            {
                tAVCLUSTER_VIDEO_Window 	interWindow;
				/* Stream is 16/9 Frame with 4/3 Image inside */
				/* -> Extract  4/3 Image of 16/9 frame */
				interWindow.sLeft	= inputWidthToUse/8 + leftPixelToRemove- leftPixelToAdd;
				interWindow.sRight	= (3*inputWidthToUse)/4 + interWindow.sLeft - 1;
	            interWindow.sTop	= 0;
				interWindow.sBottom = InputWinHeight + interWindow.sTop - 1;

                /* _-> Cut the up and down to zoom */
                pstVideoOuputConfiguration->stInputWindow.sLeft 	= interWindow.sLeft;
				pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/8;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= interWindow.sRight;
                pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (InputWinHeight/4)*3 - 1;

                if (bUse702CenteredOutputWidth == TRUE)
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
    			}
    			else
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
    			}

    			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
    			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_ST43IN169 :
            {
                tAVCLUSTER_VIDEO_Window 	interWindow;
                /* Stream is 16/9 Frame with 4/3 Image inside */
                /* -> Extract  4/3 Image of 16/9 frame */
                interWindow.sLeft	= inputWidthToUse/8 + leftPixelToRemove- leftPixelToAdd;
                interWindow.sRight	= (3*inputWidthToUse)/4 + interWindow.sLeft - 1;
                interWindow.sTop	= 0;
                interWindow.sBottom = InputWinHeight + interWindow.sTop - 1;

				pstVideoOuputConfiguration->stInputWindow.sLeft 	= interWindow.sLeft;
				pstVideoOuputConfiguration->stInputWindow.sTop 		= interWindow.sTop;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= interWindow.sRight;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= interWindow.sBottom;

    			if (bUse702CenteredOutputWidth == TRUE)
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
    			}
    			else
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
    			}

    			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
    			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_ST149IN169 :
            {
				/* apply zoom conversion of a 16:9 letterbox image  */
            	/* strech the 14:9 frame to 16/9 image */
    			pstVideoOuputConfiguration->stInputWindow.sLeft 	= inputWidthToUse/16 + leftPixelToRemove - leftPixelToAdd;
    			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + (7*inputWidthToUse)/8 - 1;
    			pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
    			pstVideoOuputConfiguration->stInputWindow.sBottom 	= InputWinHeight - 1;

    			if (bUse702CenteredOutputWidth == TRUE)
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
    			}
    			else
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
    			}

    			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
    			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
                break;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_WZ43SP149IN169 :
            {
                tAVCLUSTER_VIDEO_Window 	interWindow;
				/* Stream is 16/9 Frame with 4/3 Image inside
					with Shoot & Protect 14/9 */
					/* -> Extract  4/3 Image of 16/9 frame */
				interWindow.sLeft	= inputWidthToUse/8 + leftPixelToRemove - leftPixelToAdd;
				interWindow.sRight	= (3*inputWidthToUse)/4 + interWindow.sLeft - 1;
	            interWindow.sTop	= 0;
				interWindow.sBottom = InputWinHeight + interWindow.sTop - 1;

				/* -> Extract  14/9 Image of 4/3 Image */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= interWindow.sLeft;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= interWindow.sRight;
                /* _-> Cut the up and down to zoom */
                pstVideoOuputConfiguration->stInputWindow.sTop 		= InputWinHeight/8;
                pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + (InputWinHeight/4)*3 - 1;

                if (bUse702CenteredOutputWidth == TRUE)
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
    			}
    			else
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
    			}

    			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
    			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_ST43SP149IN169 :
            {
                tAVCLUSTER_VIDEO_Window 	interWindow;
				/* Stream is 16/9 Frame with 4/3 Image inside
					with Shoot & Protect 14/9 */
					/* -> Extract  4/3 Image of 16/9 frame */
				interWindow.sLeft	= inputWidthToUse/8 + leftPixelToRemove - leftPixelToAdd;
				interWindow.sRight	= (3*inputWidthToUse)/4 + interWindow.sLeft - 1;

	            interWindow.sTop	= 0;
				interWindow.sBottom = InputWinHeight + interWindow.sTop - 1;

				/* -> Extract  14/9 Image of 4/3 Image */
				pstVideoOuputConfiguration->stInputWindow.sLeft 	= interWindow.sLeft;
				pstVideoOuputConfiguration->stInputWindow.sTop 		= (interWindow.sBottom - interWindow.sTop + 1)/14;
				pstVideoOuputConfiguration->stInputWindow.sRight 	= interWindow.sRight;
				pstVideoOuputConfiguration->stInputWindow.sBottom 	= pstVideoOuputConfiguration->stInputWindow.sTop + ((interWindow.sBottom - interWindow.sTop + 1)/14)*12 - 1;

                if (bUse702CenteredOutputWidth == TRUE)
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
    			}
    			else
    			{
    				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
    				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
    			}

    			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
    			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            }
            break;

        case eAVCLUSTER_VIDEO_AR_CONVERSION_STRETCH_16_9:
			/* Stream is 4/3 TV is 16/9 => Enlarge the width picture => Stretch */
        case eAVCLUSTER_VIDEO_AR_CONVERSION_FULL_SCREEN:
		default:
			pstVideoOuputConfiguration->stInputWindow.sLeft 	= leftPixelToRemove - leftPixelToAdd;
			pstVideoOuputConfiguration->stInputWindow.sRight 	= pstVideoOuputConfiguration->stInputWindow.sLeft + inputWidthToUse - 1;

			pstVideoOuputConfiguration->stInputWindow.sTop 		= 0;
			pstVideoOuputConfiguration->stInputWindow.sBottom 	= InputWinHeight - 1;

			if (bUse702CenteredOutputWidth == TRUE)
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 9;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= 710;
			}
			else
			{
				pstVideoOuputConfiguration->stOutputWindow.sLeft 	= 0;
				pstVideoOuputConfiguration->stOutputWindow.sRight 	= tempWindow.sRight;
			}

			pstVideoOuputConfiguration->stOutputWindow.sTop 	= 0;
			pstVideoOuputConfiguration->stOutputWindow.sBottom 	= tempWindow.sBottom;
            /* nothing to do */
			break;
    }

    /* This code is not useful now and disabling it resolves some issues related
     * to Video to Graphics(VTG) behaviour */
//	/* If SD input and first line has to be used, remove the two first line
//		in order to not see the troubling blanking */
//	if ((pstVideoOuputConfiguration->stInputWindow.sTop == 0) &&
//		(pstVideoOuputConfiguration->stInputWindow.sBottom == 575))
//	{
//		pstVideoOuputConfiguration->stInputWindow.sTop = 2;
//	}

	return true;
}

