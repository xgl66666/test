/**************************************************************************//**
** @file dlk_cfg.h
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
** @version $Rev: 9641 $
**          $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_imcl_dlk_312B/build/inc/dlk_cfg.h $
**          $Date: 2012-03-15 17:32:59 +0100 (jeu., 15 mars 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2012 SmarDTV
**
******************************************************************************/

#ifndef _DLK_CFG_H
#define _DLK_CFG_H

/******************************************************************************
* Includes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/

typedef enum
{
   eDLK_CFG = 0x042DF0DD
} tDLK_CFG_TracesID;


/******************************************************************************
* Functions
******************************************************************************/

/**
 *  @brief
 *    Return the DLK MODE.
 *
 *    If the DLK mode is not defined, run the card detection and set the
 *    DLK mode once and for all.
 *
 *  @retval  TRUE
 *       Mode Carless
 *
 *  @retval  FALSE
 *       Mode Smartcard
 *
*/
bool DLK_CFG_IsModeCardless
(
   void
);

#if defined (CAK_DLK) && defined (DLK_MODE_HYBRID)
uint32_t DLK_CFG_ValidCardDetection (void);
#endif
/**
 *  @brief
 *    Catastrophic failure detected on the CAS Data Base.
 *    Process the catastrophic scenario:
 *    - Print a message the user with a popup
 *    - Process the reinstallation
 *    - Reboot
 *
 *  @remarks
 *    -# Significant only if CARDLESS mode, else this 
 *       function have no effect.
*/
void DLK_CFG_NotifyCatastrophicFailure
(
   void
);
void DLK_CFG_ReleaseAPPSema(void);
void DLK_CFG_CreateAPPSema(void);

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
);

#ifdef _DEBUG_VEROL_CASDB_
/**
 *  @brief
 *    Volontary crash the data base CAS to provoc a CATASTROPHIC FAILURE.
 *
 *    Write 'debilous' data in CAS DB files.
 *
*/
void DLK_CFG_DEBUG_CrashDataBase
(
   void
);
#endif
  
bool DLK_CFG_IsModeMatter
(
   void
);  

#ifdef __cplusplus
}
#endif

#endif /* _DLK_CFG_H */

/* dlk_cfg.h */

