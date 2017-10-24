/**************************************************************************//**
** @file dlk_cfg.c
**
** @brief
**   DLK Mode determination:
**   - SMARTCARD = Run with smartcard
**   - CARDLESS  = Run without smartcard (smartcard emulate in Data base VFS)
**
**   With old configuration (DLK not enable), return SMARTCARD 
**
**   In DLK configuration:
**   - If DLK mode SMARTCARD, return SMARTCARD
**   - If DLK mode CARDLESS, return CARDLESS
**   - If DLK mode HYBRID: Mecanical card detection and return CARDLESS 
**                         or SMARTCARD if a card is detected 
**
** @ingroup Product CONFIGURATION
**
** @version $Rev: 9672 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_imcl_dlk_312B/build/src/dlk_cfg.c $
**          $Date: 2012-03-16 16:50:14 +0100 (ven., 16 mars 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2012 SmarDTV
**
**
******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crules.h"
#include "tbox.h"
#include "tkel.h"
#ifdef   PRODUCT_USE_MAESTRO
#include "rtm_message.h"
#endif
#include "dlk_cfg.h"

#if defined (CAK_DLK) && defined (DLK_MODE_HYBRID)
#include "tdal_smc.h"
#endif

#include "cm_setup.h"

#ifdef _DEBUG_VEROL_CASDB_
#include "file_system_cfg.h"
#endif
#define kEVT_CA_CATASTROPHIC_FAILURE                 288

mTBOX_SET_MODULE(eDLK_CFG);


/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/

/******************************************************************************
* Locales Variables
******************************************************************************/

/**
 *  @brief
 *    Flag indicating whether the determining of DLK mode was performed (TRUE) 
 *    or not (FALSE).
 *    Note: This procedure is performed only once
*/
LOCAL bool vDLK_CFGi_bIsModeDetermined = FALSE;

/**
 *  @brief
 *    DLK mode terminated:
 *    - TRUE:  Mode CARDLESS (no card detected and mode cardeless enable)
 *    - FALSE: Mode with SMATCARD (card detected or mode cardeless disable)
*/
LOCAL bool vDLK_CFGi_bIsCardless = FALSE;

/**
 *  @brief
 *    Flag set TRUE when catastrophic failure is detected.
 *    Significant only if CARDLESS mode.
*/
LOCAL bool vDLK_CFGi_bIsCatastrophicFailure = FALSE;
TKEL_sema_id  vDLK_CFG_semaWaitAPPStart = NULL;
bool vDLK_CFG_SemaReleased = FALSE;

/******************************************************************************
* Locales Functions Prototypes
******************************************************************************/

LOCAL void DLK_CFGi_GetMode       (void);
#if defined (CAK_DLK) && defined (DLK_MODE_HYBRID)
LOCAL bool DLK_CFGi_CardDetection (tTDAL_SMC_Slot eSlot);
#endif

/******************************************************************************
 * Public Function
 ******************************************************************************/
#ifdef PROCUT_USE_CELINE
// TODO: Temporary solution - will be fixed when mechanism for sending events to Celine decided
extern void post_message_from_plugin(uint16_t clas, uint16_t type, uint16_t status, uint16_t size, void *message);
#endif
/**
 *  @brief
 *    Return the current DLK mode.
 *
 *    If DLK mode determination is no performed, perform it.
 *    Else return the current DLK mode previoulsy determined.
 *
 *  @retval  TRUE
 *       Mode Cardless
 *
 *  @retval  FALSE
 *       Mode Smartcard
 *
*/
bool DLK_CFG_IsModeCardless
(
   void
)
{
   mTBOX_FCT_ENTER(DLK_CFG_IsModeCardless);
   if (!vDLK_CFGi_bIsModeDetermined)
   {
      /* First calling => Initialize trace and run the mecanical detection */
      mTBOX_INIT_MOD_TRACE((eDLK_CFG,kTBOX_TRC_CRITICAL+kTBOX_TRC_WARNING
                            +kTBOX_TRC_1 /* Trace current mode             */
                            +kTBOX_TRC_2 /* Trace determination            */
                            +kTBOX_TRC_3 /* Trace card detection           */
                            +kTBOX_TRC_4 /* Trace 'crasher'                */
                            /*+kTBOX_TRC_5                                 */
                           ));

      /* Determine DLK mode */
      DLK_CFGi_GetMode();

      /* Ok mode determined */
#ifndef USE_DLK_1_1      
      vDLK_CFGi_bIsModeDetermined = TRUE;
#endif
   }

   /* Return current mode */
   if (vDLK_CFGi_bIsCardless)
   {
      mTBOX_TRACE((kTBOX_NIV_1,"%s(): CARDLESS mode\n",
                   TBOX_FunctionName));
      mTBOX_RETURN(TRUE);
   }
   mTBOX_TRACE((kTBOX_NIV_1,"%s(): SMARTCARD mode\n",
                TBOX_FunctionName));
   mTBOX_RETURN(FALSE);
}

bool DLK_CFG_IsModeMatter(void)
{
#if defined(PRODUCT_NL_CAB_5017)
   return TRUE;
#elif defined (PRODUCT_NL_CAB_5197D) || \
    defined(PRODUCT_NL_CAB_5197G) || \
    defined(PRODUCT_CH_CAB_8080CL) || \
    defined(PRODUCT_NL_CAB_DEN5197) || \
    defined(PRODUCT_CH_CAB_DEN8080CL) || \
    defined(PRODUCT_DEN_CAB_5197)
   return FALSE;
#else
return FALSE;
//#error "One of the boards must be defined: PRODUCT_NL_CAB_5017, PRODUCT_NL_CAB_5197G, PRODUCT_NL_CAB_5197D or PRODUCT_CH_CAB_8080CL"   
#endif
}

/**
 *  @brief
 *    Catastrophic failure detected on the CAS Data Base.
 *    Process the catastrophic scenario:
 *    - Print a message the user with a popup
 *    - Process the reinstallation
 *    - Reboot
 *
 *    This notification set a flag used by the reinstallation process via
 *    the next function DLK_CFG_IsCatastrophicFailureDetected() to reset 
 *    the Data Base partition (partition N).
 *    See comedia system function SYS_ResetDataMngr().
 *
 *  INTEGRATION: While scenario (popup, message) is not completly 
 *               specified. Print a trace on UART and call directly
 *               cmSetup_Reinstall().
 *
 *  @remarks
 *    -# Significant only if CARDLESS mode, else this 
 *       function have no effect.
*/
void DLK_CFG_NotifyCatastrophicFailure
(
   void
)
{
   mTBOX_FCT_ENTER(DLK_CFG_NotifyCatastrophicFailure);

#ifndef USE_DLK_1_1
   if (!vDLK_CFGi_bIsCardless)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s() ERROR: Notify CATASTROPHIC "
                   "FAILURE in SMARTDCARD mode ABSOLULTY ABNORMAL\n",
                   TBOX_FunctionName));
      vDLK_CFGi_bIsCatastrophicFailure = FALSE;
      mTBOX_RETURN;
   }
#endif
   
//   if (!vDLK_CFGi_bIsCatastrophicFailure)
   {
      if (!vDLK_CFG_SemaReleased)
      {
	  	   TKEL_WaitSemaphoreTimeout( vDLK_CFG_semaWaitAPPStart ,kTKEL_TIMEOUT_INFINITE );
      }
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(): CATASTROPHIC FAILURE DETECTED "
                   "=> Run CATASTROPHIC FAILURE scenario\n",
                   TBOX_FunctionName));
      vDLK_CFGi_bIsCatastrophicFailure = TRUE;
#ifdef   PRODUCT_USE_MAESTRO
      RTM_Message_PostMessageFromPlugin(CLASS_PLUGIN_FOR_PLUGIN, kEVT_CA_CATASTROPHIC_FAILURE, STATUS_OK, 0, NULL);

#endif
#ifdef PROCUT_USE_CELINE
      // TODO: Temporary solution - will be fixed when mechanism for sending events to Celine decided
      post_message_from_plugin(5, kEVT_CA_CATASTROPHIC_FAILURE, 1, 0, NULL);
#endif
   }

   mTBOX_RETURN;
}

/**
 *  @brief
 *    Return TRUE when a Catastrophic Failure was detected.
 *
 *  @retval  TRUE
 *       Catastrophic Failure DETECTED
 *
 *  @retval  FALSE
 *       No catastrophic failure detected or not in CARDLESS mode.
 *
 *  @remarks
 *    -# Significant only if CARDLESS mode, else this function 
 *       return always FALSE.
*/
bool DLK_CFG_IsCatastrophicFailureDetected
(
   void
)
{
#ifndef USE_DLK_1_1
   if (!vDLK_CFGi_bIsCardless)
   {
      return FALSE;
   }
#endif

   return vDLK_CFGi_bIsCatastrophicFailure;
}

#ifdef _DEBUG_VEROL_CASDB_
/**
 *  @brief
 *    Volontary crash the data base CAS to provoc a CATASTROPHIC FAILURE.
 *
 *    Write 'debilous' data in CAS DB files.
 *
 *    Principe: 
 *    - Read the first byte of current file
 *    - Add 1 to this byte
 *    - Rewrite this bytes
 *
*/
#define kDLK_CFG_DEBUGi_NUMFILE     9
static const char *vDLK_CFG_DEBUGi_tcFileNamestoCrash[kDLK_CFG_DEBUGi_NUMFILE] = 
{
   "/iCasDb/DTA_N",
   "/iCasDb/KEY_N",
   "/iCasDb/DID_N",
   "/iCasDb/ENT_N",
   "/iCasDb/FLT_N",
   "/iCasDb/OPE_N",
   "/iCasDb/SCD_N",
   "/iCasDb/SCP_N",
   "/iCasDb/SDT_N"
};
static uint8_t vDLK_CFG_DEBUGi_ui8bCurrFile = 0;

void DLK_CFG_DEBUG_CrashDataBase
(
   void
)
{
   bool         core = TRUE;
   uint8_t      i    = vDLK_CFG_DEBUGi_ui8bCurrFile;
   tDMJFS_Error rc;
   tDMJFS_Stat  stats;
   oDMJFS_File  fd;
   uint8_t      buff[10];
   uint32_t     n;

   mTBOX_FCT_ENTER(DLK_CFG_DEBUG_CrashDataBase);

   if (!vDLK_CFGi_bIsCardless)
   {
      /* Only in CARDLESS mode */
      mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(): Not active in SMARTCARD mode\n",
                   TBOX_FunctionName));
      mTBOX_RETURN;
   }

   while (core)
   {
      rc = DMJFS_FileInfos(kFS_CFG_PART_N,
                           vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],
                           &stats);
      if (rc != eDMJFS_NO_ERROR)
      {
         mTBOX_TRACE((kTBOX_NIV_4,"%s(%s): File to crash not exist\n",
                      TBOX_FunctionName,
                      vDLK_CFG_DEBUGi_tcFileNamestoCrash[i]));
         core = TRUE;
      }
      else
      {
         mTBOX_TRACE((kTBOX_NIV_4,"%s(%s): Verol 1 byte/%lu in file\n",
                      TBOX_FunctionName,
                      vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],
                      stats.uiFileSize));

         fd = DMJFS_FileOpen(kFS_CFG_PART_N,
                             vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],"rw");
         if (fd == NULL)
         {
            mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s): Open file failure\n",
                         TBOX_FunctionName,
                         vDLK_CFG_DEBUGi_tcFileNamestoCrash[i]));
            core = TRUE;
         }
         else
         {
            n = DMJFS_FileRead((void*)buff,1,1,fd);

            if (n != 1)
            {
               mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s): Read %lu bytes/1\n",
                            TBOX_FunctionName,
                            vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],n));
               core = TRUE;
            }
            else
            {
               rc = DMJFS_FileSeek(fd,0,eDMJFS_SEEK_SET);

               if (rc != eDMJFS_NO_ERROR)
               {
                  mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s): "
                               "Seek at begining file failure (error %d)\n",
                               TBOX_FunctionName,
                               vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],rc));
                  core = TRUE;
               }
               else
               {
                  buff[0] += 1;

                  n = DMJFS_FileWrite((const void*)buff,1,1,fd);

                  if (n != 1)
                  {
                     mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s): "
                                  "Write %lu byte/1\n",
                                  TBOX_FunctionName,
                                  vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],n));
                     core = TRUE;
                  }
                  else
                  {
                     /* Okay */
                     mTBOX_TRACE((kTBOX_NIV_4,"%s(%s): "
                                  "First byte %02X VEROLED with value %02X\n",
                                  TBOX_FunctionName,
                                  vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],
                                  buff[0]-1,buff[0]));
                     core = FALSE;
                  }
               }
            }

            rc = DMJFS_FileClose(fd); 
            if (rc != eDMJFS_NO_ERROR)
            {
               mTBOX_TRACE((kTBOX_NIV_WARNING,"%s(%s): "
                            "Close file failure (error %d)\n",
                            TBOX_FunctionName,
                            vDLK_CFG_DEBUGi_tcFileNamestoCrash[i],rc));
               core = TRUE;
            }
         }
      }

      i++;
      if (i == kDLK_CFG_DEBUGi_NUMFILE) i = 0;
      if (i == vDLK_CFG_DEBUGi_ui8bCurrFile) core = FALSE;
   }

   vDLK_CFG_DEBUGi_ui8bCurrFile = i;

   mTBOX_TRACE((kTBOX_NIV_4,"%s(): Crash data base DONE\n",
                TBOX_FunctionName));

   mTBOX_RETURN;
}
#endif

/******************************************************************************
* Locales Functions Implementations
******************************************************************************/

/**
 *  @brief
 *    Determine the DLK mode (with smartcard or cardless) .
 *
 *  @remarks
 *    -# Set result in the local module variable vDLK_CFGi_bIsCardless.
 *
*/
void DLK_CFGi_GetMode
(
   void
)
{
   mTBOX_FCT_ENTER(DLK_CFGi_GetMode);

#if !defined (CAK_DLK)
   /**
    *  DLK not configured (old version)
    *  => Force SMARTCARD mode
   */
   mTBOX_TRACE((kTBOX_NIV_2,"%s(NOT DLK): Force SMARTCARD mode\n",
                TBOX_FunctionName));
   vDLK_CFGi_bIsCardless = FALSE;


#elif defined (DLK_MODE_HYBRID)
   /**
    *  DLK configured in HYBRID mode
    *  => Card detection
   */
   mTBOX_TRACE((kTBOX_NIV_2,"%s(DLK HYBRID): Card detection...\n",
                TBOX_FunctionName));

   if (DLK_CFGi_CardDetection(eTDAL_SMC_SLOT1))
   {
      mTBOX_TRACE((kTBOX_NIV_2,"%s(DLK HYBRID): Card DETECTED "
                   "=> SMARTCARD mode\n",
                   TBOX_FunctionName));
      vDLK_CFGi_bIsCardless = FALSE;
   }
   else
   {
      mTBOX_TRACE((kTBOX_NIV_2,"%s(DLK HYBRID): No card detected "
                   "=> CARDLESS mode\n",
                   TBOX_FunctionName));
      vDLK_CFGi_bIsCardless = TRUE;
   }

#elif defined (DLK_MODE_CARDLESS)
   /**
    *  DLK configured in CARDLESS mode
    *  => Force CARDLESS mode
   */
   mTBOX_TRACE((kTBOX_NIV_2,"%s(DLK CARDLESS): Force CARDLESS mode\n",
                TBOX_FunctionName));
   vDLK_CFGi_bIsCardless = TRUE;

#elif defined (DLK_MODE_SMARTCARD)
   /**
    *  DLK configured in SMARTCARD mode
    *  => Force SMARTCARD mode
   */
   mTBOX_TRACE((kTBOX_NIV_2,"%s(DLK SMARTCARD): Force SMARTCARD mode\n",
                TBOX_FunctionName));
   vDLK_CFGi_bIsCardless = FALSE;

#else
#error "BAD DLK configuration"
#endif

   mTBOX_RETURN;
}

void DLK_CFG_ReleaseAPPSema(void)
{   
   if(!vDLK_CFG_SemaReleased)  
   	{
	    TKEL_SignalSemaphore( vDLK_CFG_semaWaitAPPStart );
		vDLK_CFG_SemaReleased = TRUE;
   	}
}
void DLK_CFG_CreateAPPSema(void)
{
    TKEL_err                tk_err;
	vDLK_CFG_SemaReleased = FALSE;
    tk_err = TKEL_CreateSemaphore( 0 , &vDLK_CFG_semaWaitAPPStart );	
    if ( tk_err != TKEL_NO_ERR )
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "NAGRA_Init: TKEL_CreateSemaphore( vDLK_CFG_semaWaitAPPStart ) failed, tk_err=%d\n", tk_err));
    }
}

#if defined (CAK_DLK) && defined (DLK_MODE_HYBRID)
/**
 *  @brief
 *    Card detection with TDAL SMC.
 *
 *  @retval  TRUE
 *       Card detected
 *
 *  @retval  FALSE
 *       No card detected
 *
 *  @remarks
 *    -# The detection is mechanical, a piece of plastic pressing the detector 
 *       of the card reader, the presence of a card upside down, a mute card... 
 *       is detected as a card present 
 *
*/
LOCAL bool DLK_CFGi_CardDetection
(
   tTDAL_SMC_Slot eSlot
)
{
	tTDAL_SMC_ErrorCode smc_rc;
	tTDAL_SMC_State     smc_state;
	bool                ya_card = FALSE;

   mTBOX_FCT_ENTER(DLK_CFGi_CardDetection);

   /* Get card state */
   smc_rc = TDAL_SMC_GetState(eTDAL_SMC_SLOT1,&smc_state);
   if (smc_rc != eTDAL_SMC_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s(slot %d) ERROR: "
                   "SMC Get state failure (smc error %d)\n",
                   TBOX_FunctionName,eSlot,smc_rc));
      ya_card = FALSE;
   }
   else
   {
	   switch (smc_state)
	   {
	   case eTDAL_SMC_STATE_ERROR:
         mTBOX_TRACE((kTBOX_NIV_3,"%s(slot %d): SMC state=%d "
                      "=> STATE_ERROR\n",
                      TBOX_FunctionName,eSlot,smc_state));
         ya_card = FALSE;
		   break;

	   case eTDAL_SMC_STATE_UNKNOWN:
		   mTBOX_TRACE((kTBOX_NIV_3,"%s(slot %d): SMC state=%d "
                      "=> STATE_UNKNOWN\n",
                      TBOX_FunctionName,eSlot,smc_state));
         ya_card = FALSE;
		   break;

	   case eTDAL_SMC_OPERATIONAL:
		   mTBOX_TRACE((kTBOX_NIV_3,"%s(slot %d): SMC state=%d "
                      "=> OPERATIONAL\n",
                      TBOX_FunctionName,eSlot,smc_state));
		   ya_card = TRUE;
		   break;

	   case eTDAL_SMC_EXTRACTED:
		   mTBOX_TRACE((kTBOX_NIV_3,"%s(slot %d): SMC state=%d "
                      "=> EXTRACTED\n",
                      TBOX_FunctionName,eSlot,smc_state));
         ya_card = FALSE;
		   break;

	   case eTDAL_SMC_INSERTED:
		   mTBOX_TRACE((kTBOX_NIV_3,"%s(slot %d): SMC state=%d "
                      "=> INSERTED\n",
                      TBOX_FunctionName,eSlot,smc_state));
         ya_card = TRUE;
		   break;

	   default:
		   mTBOX_TRACE((kTBOX_NIV_3,"%s(slot %d): SMC state=%d "
                      "=> Unsupported state\n",
                      TBOX_FunctionName,eSlot,smc_state));
         ya_card = FALSE;
      }
   }

   mTBOX_RETURN(ya_card);
}
uint32_t DLK_CFG_ValidCardDetection(void)
{
   mTBOX_FCT_ENTER(DLK_CFG_ValidCardDetection);

   mTBOX_RETURN(TDAL_SMC_State(0,eTDAL_SMC_CA_NAGRA));

}
#endif
/* dlk_cfg.c */

