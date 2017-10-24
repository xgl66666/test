/************************************************************************
 *     COPYRIGHT 2008 IWEDIA                  *
 ************************************************************************
 *
 * MODULE NAME: TDAL_CI
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_ci.h $
 *      $Rev: 191 $
 *      $Date: 2008-10-22 14:34:48 +0200 (mer., 22 oct. 2008) $
 *
 * DESCRIPTION
 *
 ************************************************************************/

#ifndef TDAL_CI
#define TDAL_CI

/* mandatory for CIPLUS stack */
#ifndef _EXTERN
#define _EXTERN extern
#define _DEFEXTERN
extern unsigned char TDAL_CI_slotNumber;
#else
unsigned char TDAL_CI_slotNumber;
#endif

/* Status register bits   */
#define kTDAL_CI_RE 0x01   /* Read error */
#define kTDAL_CI_WE 0x02   /* Write error */
#define kTDAL_CI_FR 0x40   /* Free */
#define kTDAL_CI_DA 0x80   /* Data available */

#ifdef TDAL_CIPLUS
#define kTDAL_CI_IIR 0x10   /* Init request */
#endif

/* Command register bits */
#define kTDAL_CI_HC 0x01   /* Host control */
#define kTDAL_CI_SW 0x02   /* Size write   */
#define kTDAL_CI_SR 0x04   /* Size read   */
#define kTDAL_CI_RS 0x08   /* Interface reset */

/* Commands for TDAL_CI_Ctl() */
typedef enum
{
   eTDAL_CI_WR_RESET,   /* To reset the module */
   eTDAL_CI_WR_BYPASS,   /* To control the MPEG input */
   eTDAL_CI_RD_DETECT,   /* To read the status of the card detect pin */
   eTDAL_CI_RD_RDYBSY   /* To read the status of ready busy pin */
}tTDAL_CI_Control;

/* Errors in the driver access  */
typedef enum
{
   eTDAL_CI_NO_ERROR,
   eTDAL_CI_ERROR,
   eTDAL_CI_RD_ERROR,   /* Reading on the PCMCIA driver */
   eTDAL_CI_WR_ERROR,   /* Writing on the PCMCIA driver */
   eTDAL_CI_BUSY
#ifdef TDAL_CIPLUS
   ,
   eTDAL_CI_CIS_COMPATIBLE_CIPLUS,   /* only for TDAL_CI_CheckCIS */
   eTDAL_CI_CIS_NOT_COMPATIBLE_CIPLUS /* only for TDAL_CI_CheckCIS */
#endif
} tTDAL_CI_Error;

#ifdef TDAL_CIPLUS
   typedef enum
   {
    eTDAL_CI_INT_FR_SET,   /* interrupt FR set */
    eTDAL_CI_INT_DA_SET   /* interrupt DA set */
   } tTDAL_CI_IntCause;

   /* Callback functions */
   typedef void (*tTDAL_CI_OnCardMovementFn)(
   unsigned short deviceId
   );
   typedef void (*tTDAL_CI_OnModuleInterruptFn)(
   unsigned short deviceId,
   tTDAL_CI_IntCause intCause
   );
   typedef void (*tTDAL_CI_OnIirSetFn)(
   unsigned short deviceId
   );
#endif

/* Prototypes */
tTDAL_CI_Error TDAL_CI_Init(void);
/* return the maximum slot numbers */
tTDAL_CI_Error TDAL_CI_CapabilitiesGet(unsigned short *pUiMaxSlotNumber);
tTDAL_CI_Error TDAL_CI_Terminate(void);

#ifdef TDAL_CIPLUS
tTDAL_CI_Error TDAL_CI_Open(char * name_ptr, unsigned short *devId);
tTDAL_CI_Error TDAL_CI_Ctl(unsigned short devId, tTDAL_CI_Control cmd, unsigned char *ioValue);
#else
tTDAL_CI_Error TDAL_CI_Open(char * name_ptr, short *devId);
tTDAL_CI_Error TDAL_CI_Ctl(unsigned short devId, unsigned short cmd, unsigned char *ioValue);
#endif

tTDAL_CI_Error TDAL_CI_Read(unsigned short devId,unsigned short *length_ptr, unsigned char * ptr);
tTDAL_CI_Error TDAL_CI_Write(unsigned short devId, unsigned short length, unsigned char * data_ptr);
tTDAL_CI_Error TDAL_CI_Close(unsigned short devId);
tTDAL_CI_Error TDAL_CI_ReadStatus(unsigned short devId, unsigned char bit, unsigned char *state);
tTDAL_CI_Error TDAL_CI_WriteCmd(unsigned short devId, unsigned char bit, unsigned short value);
tTDAL_CI_Error TDAL_CI_CheckCIS(unsigned short devId);
tTDAL_CI_Error TDAL_CI_WriteCOR(unsigned short devId);
tTDAL_CI_Error TDAL_CI_RegisterDisplay(unsigned long display_mask);

#ifdef TDAL_CIPLUS
tTDAL_CI_Error  TDAL_CI_RegisterCallbacks(  unsigned short deviceId
                   ,tTDAL_CI_OnCardMovementFn cbSTK_DRV_OnCardMovement
                   ,tTDAL_CI_OnModuleInterruptFn cbSTK_DRV_OnModuleInterrupt
                   ,tTDAL_CI_OnIirSetFn cbSTK_DRV_OnIirSet);
tTDAL_CI_Error TDAL_CI_GetCompatibilityLength(unsigned short devId, unsigned short *length_ptr);
tTDAL_CI_Error TDAL_CI_GetCompatibilityString(unsigned short devId, unsigned short *length_ptr, unsigned char *ptr);
#endif

#ifdef _DEFEXTERN
#undef _EXTERN
#undef _DEFEXTERN
#endif

#endif /* TDAL_CI */

