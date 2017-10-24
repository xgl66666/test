/*****************************************************************************
 *                      COPYRIGHT 2004 IWEDIA TECHNOLOGIES              *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_AVS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_avs/tdal_avs.c $
 *          $Rev: 888 $
 *          $Date: 2011-12-12 15:00:02 +0100 (Mon, 12 Dec 2011) $
 *
 * Description : AudioVideo Switching driver TDAL library implementation.
 *
 ****************************************************************************/
/****************************************************************************
   *   INCLUDES                                           *
   ****************************************************************************/
/*   Generic   Headers   */
#include <stdio.h>
#include "string.h"   /*   needed   for   memcpy()   */

#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_avs.h"


/**========================================================================**
   *   Function  :   TDAL_AVS_Init
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_Init()
{
   return (eTDAL_AVS_NO_ERROR);
}

/**========================================================================**
   *   Function  :   TDAL_AVS_Term
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_Term()
{
return (eTDAL_AVS_NO_ERROR);
}

/**========================================================================**
   *   Function  :   TDAL_AVS_APIRevisionGet
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
const   char   *TDAL_AVS_APIRevisionGet(void)
{
   static   const   char   API_Revision[] = "TDAL_AVS_v1.0";

   return((const   char   *)API_Revision);

}

/**========================================================================**
   *   Function  :   TDAL_AVS_PlatformRevisionGet
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
const   char   *TDAL_AVS_PlatformRevisionGet(void)
{
   static   const   char   PLTF_Revision[] = "STFAEA16_7109";

   return((const   char   *)PLTF_Revision);
}


/**========================================================================**
   *   Function  :   TDAL_AVS_SelectInput
   *
   *   Description :
   *
   *   Side   effects :   TDAL_AVS_Context
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectInput(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_InputDevice   inputDevice)
{
return (eTDAL_AVS_NO_ERROR);
}

/**========================================================================**
   *   Function  :   TDAL_AVS_SelectSignalType
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectSignalType(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_SignalType   signalType)
{
return (eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function  :   TDAL_AVS_SelectLevel
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectLevel(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_Level   level)
{
return (eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function  :   TDAL_AVS_Mute
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_Mute(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_MuteState   muteState)
{
return (eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function  :   TDAL_AVS_SelectSlowBlanking
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectSlowBlanking(tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_SlowBlankingLevel   SBLevel)
{
return (eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function  :   TDAL_AVS_SelectFastBlanking
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectFastBlanking(tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_FastBlankingLevel   FBLevel)
{
return (eTDAL_AVS_NO_ERROR);
}

/******************************************************************************
   *   Function   Name :   TDAL_AVS_EnableAutomaticLoopthrough
   *
   *   Description :   Enable   or   disable   automatic   loopthrough
   *
   *   Side   effects :
   *
   *   Comment  :
   *
   *****************************************************************************/
tTDAL_AVS_Error   TDAL_AVS_EnableAutomaticLoopthrough(bool   enable)
{
    return (eTDAL_AVS_NO_ERROR);
}

void   TDAL_AVS_ChangeRegisterValue(void)
{
}

