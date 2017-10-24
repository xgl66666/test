/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_SCART_INPUT
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_scart_input.h $ 
 * 
 *      $Rev: 497 $
 *      $Date: 2008-12-05 09:40:51 +0100 (ven., 05 déc. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_SCART_INPUT abstraction layer 
 *
 *****************************************************************************/

#ifndef _TDAL_SCART_INPUT_H_
#define _TDAL_SCART_INPUT_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

/*******************************************************/
/*       Macros               */
/*******************************************************/


/*******************************************************/
/*       Typedefs              */
/*******************************************************/

typedef enum
{
   eTDAL_SCART_INPUT_NO_ERROR,      /* No Error returned */
   eTDAL_SCART_INPUT_NOT_INIT_ERROR,   /* TDAL_SCART_INPUT has not been initialized */
   eTDAL_SCART_INPUT_ALREADY_INIT_ERROR,  /* TDAL_SCART_INPUT already initialized */
   eTDAL_SCART_INPUT_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_SCART_INPUT_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_SCART_INPUT_NO_MEMORY_ERROR,   /* Not enough memory to perform action*/
   eTDAL_SCART_INPUT_NOT_AVAILABLE_ERROR, /* Feature not available*/
} tTDAL_SCART_INPUT_Error;

typedef enum
{
   eTDAL_SCART_INPUT_ID_0,       /* SCART input identifier 0 */
   eTDAL_SCART_INPUT_ID_1,       /* SCART input identifier 1 */
   eTDAL_SCART_INPUT_ID_2,       /* SCART input identifier 2 */
   eTDAL_SCART_INPUT_ID_3,       /* SCART input identifier 3 */
   eTDAL_SCART_INPUT_ID_4,       /* SCART input identifier 4 */
} tTDAL_SCART_INPUT_Id;

typedef enum
{
   eTDAL_SCART_INPUT_EVENT_SLOW_BLANKING_CHANGE,
   eTDAL_SCART_INPUT_EVENT_FAST_BLANKING_CHANGE
} tTDAL_SCART_INPUT_Event;

typedef enum
{
   eTDAL_SCART_INPUT_SLOW_BLANKING_SIGNAL_BYPASS = 0,
   eTDAL_SCART_INPUT_SLOW_BLANKING_SIGNAL_4_3,
   eTDAL_SCART_INPUT_SLOW_BLANKING_SIGNAL_16_9
} tTDAL_SCART_INPUT_SlowBlankingSignal;

typedef enum
{
   eTDAL_SCART_INPUT_FAST_BLANKING_SIGNAL_COMPOSITE = 0,
   eTDAL_SCART_INPUT_FAST_BLANKING_SIGNAL_RGB
} tTDAL_SCART_INPUT_FastBlankingSignal;

typedef void (*tTDAL_SCART_INPUT_CallbackFct)
(
   tTDAL_SCART_INPUT_Id   eId,
   tTDAL_SCART_INPUT_Event  eEvent,
   const void       *pEventData
);

typedef struct TDAL_SCART_INPUT_Capability_s
{
   uint8_t        nbInput;
} tTDAL_SCART_INPUT_Capability;

typedef struct TDAL_SCART_INPUT_ScartCapability_s
{
   bool bRGBCapable;
   bool bYCCapable;
   bool bOutputCapable;
} tTDAL_SCART_INPUT_ScartCapability;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_Init (void);
tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_Term (void);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_CapabilityGet(tTDAL_SCART_INPUT_Capability      *pstCapability);

const char *      TDAL_SCART_INPUT_GetAPIRevision(void);
const char *      TDAL_SCART_INPUT_GetPlatformRevision(void);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_GetScartCapability(tTDAL_SCART_INPUT_Id       eId,
                          tTDAL_SCART_INPUT_ScartCapability  *pstScartCapability;);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_SubscribeEvent(tTDAL_SCART_INPUT_Id         eId,
                        tTDAL_SCART_INPUT_Event        eEvent,
                        tTDAL_SCART_INPUT_CallbackFct      notifyFunction);
tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_UnsubscribeEvent(tTDAL_SCART_INPUT_Id         eId,
                         tTDAL_SCART_INPUT_Event       eEvent);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_Start(tTDAL_SCART_INPUT_Id             eId);
tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_Stop(tTDAL_SCART_INPUT_Id              eId);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_GetSlowBlanking(tTDAL_SCART_INPUT_Id         eId,
                         tTDAL_SCART_INPUT_SlowBlankingSignal  *peSignal);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_GetFastBlanking(tTDAL_SCART_INPUT_Id         eId,
                         tTDAL_SCART_INPUT_FastBlankingSignal  *peSignal);

tTDAL_SCART_INPUT_Error TDAL_SCART_INPUT_MuteOutputs(tTDAL_SCART_INPUT_Id           eId,
                       bool                  bMute);


#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_SCART_INPUT_H_*/

