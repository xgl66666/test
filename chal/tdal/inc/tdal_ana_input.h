/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_ANA_INPUT
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_ana_input.h $
 *
 *      $Rev: 1636 $
 *      $Date: 2009-09-25 16:29:49 +0200 (ven., 25 sept. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_ANA_INPUT abstraction layer
 *
 *****************************************************************************/

#ifndef _TDAL_ANA_INPUT_H_
#define _TDAL_ANA_INPUT_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

#define kTDAL_ANA_INPUT_MASK_CLASS   0xff0000
#define kTDAL_ANA_INPUT_MASK_TYPE    0x00ff00
#define kTDAL_ANA_INPUT_MASK_INSTANCE   0x0000ff
#define kTDAL_ANA_INPUT_NO_OBJECT    0xFFFFFF

/*******************************************************/
/*       Macros               */
/*******************************************************/

#define mTDAL_ANA_INPUT_GetObjectId( class , type , instance ) \
     (class << 16) + (type << 8) + instance

#define mTDAL_ANA_INPUT_GetObjectClass( object ) \
     (object & kTDAL_ANA_INPUT_MASK_CLASS) >> 16

#define mTDAL_ANA_INPUT_GetObjectType( object ) \
     (object & kTDAL_ANA_INPUT_MASK_TYPE) >> 8

#define mTDAL_ANA_INPUT_GetObjectInstance( object ) \
     (object & kTDAL_ANA_INPUT_MASK_INSTANCE)

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
enum {
   eTDAL_ANA_INPUT_CLASS_VIDEO   = 0 ,
   eTDAL_ANA_INPUT_CLASS_AUDIO
};

enum {
   eTDAL_ANA_INPUT_TYPE_MUX  = 0, /* Muliplexer object type */
   eTDAL_ANA_INPUT_TYPE_CVBS,   /* CVBS video input object type */
   eTDAL_ANA_INPUT_TYPE_YC,    /* YC video input object type */
   eTDAL_ANA_INPUT_TYPE_COMP,   /* Component video input object type */
   eTDAL_ANA_INPUT_TYPE_LR,    /* LR baseband audio input object type */
};

typedef enum {
   eTDAL_ANA_INPUT_NO_ERROR,      /* No Error returned */
   eTDAL_ANA_INPUT_NOT_INIT_ERROR,   /* TDAL_ANA_INPUT has not been initialised */
   eTDAL_ANA_INPUT_ALREADY_INIT_ERROR,  /* TDAL_ANA_INPUT already initialised */
   eTDAL_ANA_INPUT_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_ANA_INPUT_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_ANA_INPUT_NO_MEMORY_ERROR,   /* Not enough memory to perform action*/
   eTDAL_ANA_INPUT_NOT_AVAILABLE_ERROR, /* Feature not available*/
}tTDAL_ANA_INPUT_Error;

typedef uint32_t tTDAL_ANA_INPUT_Object;

typedef struct TDAL_ANA_INPUT_Capability_s
{
   uint8_t        nbInput;
   tTDAL_ANA_INPUT_Object *InputArray;
   uint8_t        nbMux;
   tTDAL_ANA_INPUT_Object *MuxArray;
} tTDAL_ANA_INPUT_Capability;


typedef struct TDAL_ANA_INPUT_MuxCapability_s
{
   uint8_t        nbInput;
   tTDAL_ANA_INPUT_Object *InputArray;
} tTDAL_ANA_INPUT_MuxCapability;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_Init (void);
tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_Term (void);

tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_CapabilityGet(tTDAL_ANA_INPUT_Capability *pstCapability);
tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_MuxCapabilityGet(tTDAL_ANA_INPUT_Object     MuxId,
                  tTDAL_ANA_INPUT_MuxCapability *pstMuxCapability);

const char *TDAL_ANA_INPUT_APIRevisionGet(void);
const char *TDAL_ANA_INPUT_PlatformRevisionGet(void);

tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_VideoInputSelect(tTDAL_ANA_INPUT_Object InputId,
                        tTDAL_ANA_INPUT_Object MuxId);

tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_HandleGet(tTDAL_ANA_INPUT_Object MuxId,
                     void         *pHandle);

tTDAL_ANA_INPUT_Error TDAL_ANA_INPUT_AudioInputSelect(tTDAL_ANA_INPUT_Object InputId,
                        tTDAL_ANA_INPUT_Object MuxId);

#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_ANA_INPUT_H_*/



