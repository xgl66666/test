/********************************************************************************
 *                       COPYRIGHT 2006 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: MHEG CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/mheg_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/


/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "tkel.h"

#include "comedia_keys.h"
/********************************************************/
/*              Defines                                 */
/********************************************************/

#ifndef MHEG_MEMORY_SIZE
    /* It is advise to set the MHEG Memory size to 5MBytes for a normal usage */
    #define MHEG_MEMORY_SIZE (6*1024*1024)
#endif


/* defines MHEG receiver keys groups key lists */
#define MHG_RECEIVER_GROUP_KEY_LIST \
    eSYS_EPG_KEY, eSYS_INFO_KEY

#define MHG_ICSTREAMING_GROUP_KEY_LIST  \
    eSYS_PLAY_KEY, eSYS_STOP_KEY, eSYS_PAUSE_KEY, eSYS_BACKWARD_KEY, eSYS_FORWARD_KEY, \
    eSYS_SKIP_BACKWARD_KEY, eSYS_SKIP_FORWARD_KEY /* not all keys supported */

#define MHG_DIGIT_GROUP_KEY_LIST \
    eSYS_KEY_DIGIT0, eSYS_KEY_DIGIT1, eSYS_KEY_DIGIT2, eSYS_KEY_DIGIT3, eSYS_KEY_DIGIT4, eSYS_KEY_DIGIT5, eSYS_KEY_DIGIT6, eSYS_KEY_DIGIT7, eSYS_KEY_DIGIT8, eSYS_KEY_DIGIT9

#define MHG_ARROW_GROUP_KEY_LIST \
    eSYS_SELECT_KEY, eSYS_UP_ARROW_KEY, eSYS_DOWN_ARROW_KEY, eSYS_LEFT_ARROW_KEY, eSYS_RIGHT_ARROW_KEY

#define MHG_ALWAYS_MHEG_GROUP_KEY_LIST \
    eSYS_EXIT_KEY, eSYS_TELETEXT_KEY, eSYS_RED_KEY, eSYS_GREEN_KEY, eSYS_YELLOW_KEY, eSYS_BLUE_KEY


/********************************************************/
/*              Macros                                  */
/********************************************************/

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
uint8_t pMHEG_Memory_Partition[MHEG_MEMORY_SIZE];
#pragma ST_section      (pMHEG_Memory_Partition, "mheg_section")



/* Receiver Keys*/ /* SA extention */
LOCAL tSYS_KeyEvtCode VIRTUAL_ReceiverKeyCodeArray[]={MHG_RECEIVER_GROUP_KEY_LIST};
LOCAL uint16_t MHG_ReceiverKeyCount=(sizeof(VIRTUAL_ReceiverKeyCodeArray)/sizeof(tSYS_KeyEvtCode));

/* IC Streaming Keys*/
LOCAL tSYS_KeyEvtCode VIRTUAL_ICStreamingKeyCodeArray[]={MHG_ICSTREAMING_GROUP_KEY_LIST};
LOCAL uint16_t MHG_ICStreamingKeyCount=(sizeof(VIRTUAL_ICStreamingKeyCodeArray)/sizeof(tSYS_KeyEvtCode));

/* Digit Keys*/
LOCAL tSYS_KeyEvtCode VIRTUAL_DigitKeyCodeArray[]={MHG_DIGIT_GROUP_KEY_LIST};
LOCAL uint16_t MHG_DigitKeyCount=(sizeof(VIRTUAL_DigitKeyCodeArray)/sizeof(tSYS_KeyEvtCode));

/* Arrow Keys*/
LOCAL tSYS_KeyEvtCode VIRTUAL_ArrowKeyCodeArray[]={MHG_ARROW_GROUP_KEY_LIST};
LOCAL uint16_t MHG_ArrowKeyCount=(sizeof(VIRTUAL_ArrowKeyCodeArray)/sizeof(tSYS_KeyEvtCode));

/* Keys always sent to MHEG*/
LOCAL tSYS_KeyEvtCode VIRTUAL_AlwaysMhegKeyCodeArray[]={MHG_ALWAYS_MHEG_GROUP_KEY_LIST};
LOCAL uint16_t MHG_AlwaysMhegKeyCount=(sizeof(VIRTUAL_AlwaysMhegKeyCodeArray)/sizeof(tSYS_KeyEvtCode));


/* All keys */
LOCAL tSYS_KeyEvtCode MHG_AllKeyCodesArray[]=
{
    MHG_RECEIVER_GROUP_KEY_LIST,
    MHG_ICSTREAMING_GROUP_KEY_LIST,
    MHG_DIGIT_GROUP_KEY_LIST,
    MHG_ARROW_GROUP_KEY_LIST,
    MHG_ALWAYS_MHEG_GROUP_KEY_LIST
};
LOCAL uint16_t MHG_AllKeyCount=(sizeof(MHG_AllKeyCodesArray)/sizeof(tSYS_KeyEvtCode));


/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/



/*===========================================================================
 *
 * MHEG_CFG_Init
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
uint32_t MHEG_CFG_GetMhegMemorySize(void)
{
    return(MHEG_MEMORY_SIZE);
}

/*===========================================================================
 *
 * MHEG_CFG_MEM_Init
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
void MHEG_CFG_MEM_Init(void)
{
    /* Nothing to do here, MHEG memory is a predefined array */

#ifdef __DEBUG_TRACE__
    printf("==========================================================\n");
    printf("CM_EMBEDDED _ mheg_cfg.c or compilation define MHEG_MEMORY_SIZE\n");
    printf("CM_EMBEDDED _ Size of STATIC memory defined for MHEG stack = %ud bytes\n",
                             MHEG_CFG_GetMhegMemorySize());

    printf("CM_EMBEDDED _ If not sufficient, MHEG may not work properly (minimum is 6 MB)\n");
    printf("==========================================================\n");
#endif
}



/*===========================================================================
 *
 * MHEG_CFG_GetMhegMemoryPartition
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
uint8_t *
MHEG_CFG_GetMhegMemoryPartition(void)
{
    return(pMHEG_Memory_Partition);
}

uint16_t MHEG_CFG_GetReceiverKeyCount(void)
{
    return MHG_ReceiverKeyCount;
}

uint16_t MHEG_CFG_GetICStreamingKeyCount(void)
{
    return MHG_ICStreamingKeyCount;
}

uint16_t MHEG_CFG_GetDigitKeyCount(void)
{
	return MHG_DigitKeyCount;
}

uint16_t MHEG_CFG_GetArrowKeyCount(void)
{
	return MHG_ArrowKeyCount;
}

uint16_t MHEG_CFG_GetAlwaysMHEGKeyCount(void)
{
	return MHG_AlwaysMhegKeyCount;
}

uint16_t MHEG_CFG_GetAllKeyCount(void)
{
	return MHG_AllKeyCount;
}

bool	MHEG_CFG_GetKeyCodeArrayFromIndex(uint8_t index, tSYS_KeyEvtCode** ppKeycodeArray)
{
	if ((index >= MHG_AllKeyCount) || (ppKeycodeArray == NULL))
	{
		return FALSE;
	}
	else
	{
		*ppKeycodeArray = &MHG_AllKeyCodesArray[index];
		return TRUE;
	}
}

