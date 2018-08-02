/*****************************************************************************
 *                      COPYRIGHT 2004 IWEDIA TECHNOLOGIES              *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_AVS
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_avs/tdal_avs_akm.c $
 *          $Rev: 414 $
 *          $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : AudioVideo Switching driver TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
   *   INCLUDES                                           *
   ****************************************************************************/
/*   Generic   Headers   */
#include<stdio.h>
#include"string.h"   /*   needed   for   memcpy()   */
#include"stos.h"

/*   ST   Specific   Headers   */
#include "stdevice.h"
#include "stboot.h"
#include "stcommon.h"
#include "sti2c.h"

#include "sttbx.h"
#include "crules.h"
#include "tbox.h"
#include "pltf_hardcfg.h"
#include "tdal_common.h"
#include "tdal_avs.h"

/****************************************************************************
   *   DEFINES                                              *
   ****************************************************************************/
#define   kTDAL_AVS_STI2C_TIMEOUT        1000

/****************************************************************************
   *   MACROS                                              *
   ****************************************************************************/
#define   mTDAL_AVS_I2C_RTRY_DELAYTIME   ((10   *   ST_GetClocksPerSecond())/1000)

/****************************************************************************
   *   TYPEDEFS                                           *
   ****************************************************************************/

struct   sTDAL_AVS_Device_State
{
   tTDAL_AVS_InputDevice   AudioInputDevice;
   tTDAL_AVS_SignalType   AudioSignalType;
   tTDAL_AVS_Level      AudioLevel;
   tTDAL_AVS_MuteState   AudioMute;
   tTDAL_AVS_InputDevice   VideoInputDevice;
   tTDAL_AVS_SignalType   VideoSignalType;
   tTDAL_AVS_Level      VideoLevel;
   tTDAL_AVS_MuteState   VideoMute;
};

struct   sTDAL_AVS_Devices_State
{
   struct   sTDAL_AVS_Device_State   TV;
   struct   sTDAL_AVS_Device_State   VCR;
   struct   sTDAL_AVS_Device_State   AUX;
   struct   sTDAL_AVS_Device_State   RCA;
   tTDAL_AVS_FastBlankingLevel      FB;
   tTDAL_AVS_SlowBlankingLevel      SB;
};

struct   sTDAL_AVS_I2C_Context
{
   STI2C_Handle_t        I2C_Handle;
   ST_DeviceName_t      I2C_DeviceName;
   STI2C_OpenParams_t   I2C_OpenParams;
};

struct   sTDAL_AVS_Register_State
{
   U8   Audio_TV;
   U8   Audio_ALL;
   U8   Video_TV_VCR;
   U8   Video_RGB_FB;
   U8   Video_RCA_OTHER;
   U8   Video_SB_OTHER;
   U8   Selection;
};

struct   sTDAL_AVS_Module_Context
{
   struct   sTDAL_AVS_I2C_Context   I2C;
   struct   sTDAL_AVS_Devices_State   DevicesState;
   struct   sTDAL_AVS_Register_State   RegisterState;
};
/****************************************************************************
   *   GLOBAL   VARIABLES   (GLOBAL/IMPORT)                           *
   ****************************************************************************/
IMPORT   ST_DeviceName_t   SYS_I2cName[];

/****************************************************************************
   *   LOCAL   MODULE   VARIABLES   (MODULE/IMPORT)                              *
   ****************************************************************************/

/****************************************************************************
   *   TRACE   CONFIGURATION                                      *
   ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_AVS);
/*   kTBOX_NIV_1 :   Print   debug   info   in   API's   loopthrough   functions      *
   *   kTBOX_NIV_3 :   Print   "NOT   IMPLEMENTED"   warnings                *
   *   kTBOX_NIV_4 :   Print   written   values   in   registers                *
   *   kTBOX_NIV_5 :   Print   debug   info   about   SlowBlanking   changes   detection   */

/****************************************************************************
   *   LOCAL   FILE   VARIABLES   (LOCAL)                              *
   ****************************************************************************/
LOCAL   bool        AlreadyInitialized =FALSE;
LOCAL   STOS_Semaphore_t   *I2CAccessSemaphore;
struct   sTDAL_AVS_Module_Context   TDAL_AVS_Context;

/****************************************************************************
   *   FUNCTIONS   DEFINITIONS   (LOCAL/GLOBAL)                         *
   ****************************************************************************/
tTDAL_AVS_Error   p_TDAL_AVS_WriteRegister(U8   driverRegister, U8   value);


/**========================================================================**
   *   Function   :   TDAL_AVS_Init
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_Init()
{
   ST_ErrorCode_t      ST_error;
   tTDAL_AVS_Error      AVS_error;
   U8              newValue;

   mTBOX_FCT_ENTER( "TDAL_AVS_Init" );


   if   (AlreadyInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAl_AVS   already   initialized\n"));
      mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
   }

   /**   OPENING   I2C   **/
   strcpy( TDAL_AVS_Context.I2C.I2C_DeviceName   ,
   SYS_I2cName[kSYS_AVS_I2C] );   /*   0:BACK   1:FRONT   2 :   EXT   */
   TDAL_AVS_Context.I2C.I2C_OpenParams.I2cAddress  = 0x22;
   TDAL_AVS_Context.I2C.I2C_OpenParams.AddressType = STI2C_ADDRESS_7_BITS;
   TDAL_AVS_Context.I2C.I2C_OpenParams.BusAccessTimeOut = 20;

   ST_error = STI2C_Open(TDAL_AVS_Context.I2C.I2C_DeviceName, &(TDAL_AVS_Context.I2C.I2C_OpenParams), &(TDAL_AVS_Context.I2C.I2C_Handle));
   if   ( ST_error != ST_NO_ERROR )
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"ERR:   STI2C_Open   failed :   [%s]\n", TBOX_GetDriverErrorText(ST_error)));
      mTBOX_RETURN(eTDAL_AVS_NOT_DONE);
   }

   /*   initialize   the   semaphore   controlling   access   to   I2C   bus   */
   I2CAccessSemaphore = STOS_SemaphoreCreateFifo(NULL,1);

   /*********************************/
   /*   Set   registers   state        */
   /*********************************/
   p_TDAL_AVS_WriteRegister(0, 0x70);
   p_TDAL_AVS_WriteRegister(1, 0x04);
   p_TDAL_AVS_WriteRegister(2, 0x30);
   p_TDAL_AVS_WriteRegister(3, 0x27);
   p_TDAL_AVS_WriteRegister(4, 0x09);
   p_TDAL_AVS_WriteRegister(5, 0xDF);
   p_TDAL_AVS_WriteRegister(6, 0x00);
   TDAL_AVS_Context.RegisterState.Video_SB_OTHER = 0x80;


   newValue = TDAL_AVS_Context.RegisterState.Video_SB_OTHER   &   0xc3;

   newValue |= (0 << 2);
   newValue |= (0 << 4);

   p_TDAL_AVS_WriteRegister(7, newValue);

   TDAL_AVS_Context.DevicesState.SB = eTDAL_AVS_SB_STANDBY_MODE;
   TDAL_AVS_Context.RegisterState.Video_SB_OTHER = newValue;

   AlreadyInitialized = TRUE;


   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);

}

/**========================================================================**
   *   Function   :   TDAL_AVS_Term
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_Term()
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_Term" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

/**========================================================================**
   *   Function   :   TDAL_AVS_APIRevisionGet
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
const   char   *TDAL_AVS_APIRevisionGet(void)
{
   static   const   char   API_Revision[] = "TDAL_AVS_v1.0";

   mTBOX_FCT_ENTER("TDAL_AVS_APIRevisionGet");

   mTBOX_RETURN((const   char   *)API_Revision);

}

/**========================================================================**
   *   Function   :   TDAL_AVS_PlatformRevisionGet
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
const   char   *TDAL_AVS_PlatformRevisionGet(void)
{
   static   const   char   PLTF_Revision[] = "STAPISDK";

   mTBOX_FCT_ENTER("TDAL_AVS_PlatformRevisionGet");

   mTBOX_RETURN((const   char   *)PLTF_Revision);
}


/**========================================================================**
   *   Function   :   TDAL_AVS_SelectInput
   *
   *   Description :
   *
   *   Side   effects :   TDAL_AVS_Context
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectInput(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_InputDevice   inputDevice)
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_SelectInput" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

/**========================================================================**
   *   Function   :   TDAL_AVS_SelectSignalType
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectSignalType(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_SignalType   signalType)
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_SelectSignalType" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function   :   TDAL_AVS_SelectLevel
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectLevel(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_Level   level)
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_SelectLevel" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function   :   TDAL_AVS_Mute
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_Mute(tTDAL_AVS_Object   object, tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_MuteState   muteState)
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_Mute" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

   /**========================================================================**
   *   Function   :   TDAL_AVS_SelectSlowBlanking
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectSlowBlanking(tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_SlowBlankingLevel   SBLevel)
{
   tTDAL_AVS_Error      AVS_error;
   U8              newValue;

   mTBOX_FCT_ENTER( "TDAL_AVS_SelectSlowBlanking" );

   newValue = TDAL_AVS_Context.RegisterState.Video_SB_OTHER   &   0xc3;

   if   (outputDevice == eTDAL_AVS_OUT_TV)
   {
      if   (SBLevel == eTDAL_AVS_SB_16_9_FORMAT)
      {
        TDAL_AVS_Context.DevicesState.SB = eTDAL_AVS_SB_16_9_FORMAT;
        /*   16x9   */
        newValue |= (1 << 2);
        newValue |= (1 << 4);
      }
      else   if   (SBLevel == eTDAL_AVS_SB_4_3_FORMAT)
      {
        TDAL_AVS_Context.DevicesState.SB = eTDAL_AVS_SB_4_3_FORMAT;
        /*   4x3   */
        newValue |= (3 << 2);
        newValue |= (3 << 4);
      }
      else
      {
        TDAL_AVS_Context.DevicesState.SB = eTDAL_AVS_SB_STANDBY_MODE;
        /*   No   SLB   */
        newValue |= (0 << 2);
        newValue |= (0 << 4);
      }

      if   (newValue != TDAL_AVS_Context.RegisterState.Video_SB_OTHER)
      {
        p_TDAL_AVS_WriteRegister(7, newValue);
        TDAL_AVS_Context.RegisterState.Video_SB_OTHER = newValue;
      }
   }

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);

}

   /**========================================================================**
   *   Function   :   TDAL_AVS_SelectFastBlanking
   *
   *   Description :
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   **========================================================================**/
tTDAL_AVS_Error   TDAL_AVS_SelectFastBlanking(tTDAL_AVS_OutputDevice   outputDevice, tTDAL_AVS_FastBlankingLevel   FBLevel)
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_SelectFastBlanking" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

/******************************************************************************
   *   Function   Name :   TDAL_AVS_EnableAutomaticLoopthrough
   *
   *   Description :   Enable   or   disable   automatic   loopthrough
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   *****************************************************************************/
tTDAL_AVS_Error   TDAL_AVS_EnableAutomaticLoopthrough(bool   enable)
{
   tTDAL_AVS_Error      AVS_error;

   mTBOX_FCT_ENTER( "TDAL_AVS_EnableAutomaticLoopthrough" );

   mTBOX_RETURN(eTDAL_AVS_NO_ERROR);
}

   /******************************************************************************
   *   Function   Name :   p_TDAL_AVS_WriteRegister
   *
   *   Scope       :   private   to   module   AVS
   *
   *   Description :   Send   the   new   parameters   to   the   AVS   chip
   *
   *   Side   effects :
   *
   *   Comment   :
   *
   *****************************************************************************/
tTDAL_AVS_Error   p_TDAL_AVS_WriteRegister(U8   driverRegister, U8   value)
{
   ST_ErrorCode_t   STerror;
   U8          Data[] = { 0x00, 0x00 };
   U32          WrittenData;
   tTDAL_AVS_Error      Error = eTDAL_AVS_NO_ERROR;
   U8   attempts = 0;

   Data[0] = driverRegister;
   Data[1] = value;

   STOS_SemaphoreWait(I2CAccessSemaphore);

   STerror = STI2C_Lock(TDAL_AVS_Context.I2C.I2C_Handle, TRUE);
   if(STerror != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING, "ERR:   STI2C_Lock   failed :   [%s]\n", TBOX_GetDriverErrorText(STerror)));
      Error = eTDAL_AVS_NOT_DONE;
   }

   if(Error == eTDAL_AVS_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_4, "REG:%d << 0x%x\n", driverRegister, value));
      
      STerror = STI2C_Write(TDAL_AVS_Context.I2C.I2C_Handle, Data, sizeof(Data), kTDAL_AVS_STI2C_TIMEOUT, &WrittenData);
      
      while((STerror != ST_NO_ERROR)   &&   (attempts<3))
      {
        task_delay(mTDAL_AVS_I2C_RTRY_DELAYTIME);
      
        STerror = STI2C_Write(TDAL_AVS_Context.I2C.I2C_Handle, Data, sizeof(Data), kTDAL_AVS_STI2C_TIMEOUT, &WrittenData);
        attempts++;
      }
      
      STerror = STI2C_Unlock(TDAL_AVS_Context.I2C.I2C_Handle);
      if(STerror != ST_NO_ERROR)
      {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "ERR:   STI2C_Unlock   failed :   [%s]\n", TBOX_GetDriverErrorText(STerror)));
        Error = eTDAL_AVS_NOT_DONE;
      }
      
      if(STerror != ST_NO_ERROR)
      {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "ERR:   STI2C_Write   failed :   [%s]\n", TBOX_GetDriverErrorText(STerror)));
        Error = eTDAL_AVS_NOT_DONE;
      }
   }

   STOS_SemaphoreSignal(I2CAccessSemaphore);

   return(Error);
}

