/*****************************************************************************
 *                      COPYRIGHT 2008 IWEDIA TECHNOLOGIES              *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_CI
 *
 * FILE NAME: URL:  $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_ci/tdal_ci.c $
 *          Rev:  $Rev: 414 $
 *          Date: $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : Common Interface TDAL library implementation.
 * Target : All
 * Author :
 * Creation : 17/11/2004
 *
 ****************************************************************************/

/****************************************************************************
   *   INCLUDES                                           *
   ****************************************************************************/
#include <crules.h>

#define _EXTERN
#include "tdal_ci.h"
#undef _EXTERN

/*#include "tdal_ci_cfg.h"*/
#include "tdal_common.h"
#include "tbox.h"

/*#define FPA_LOOP*/
#if defined(FPA_LOOP)
#include "tdal_tsroute_module_priv.h"
#endif

/*******************************************************************************
   NAME : TDAL_CI_drvInit
   DESCRIPTION : This   function   initialize   the   PCMCIA   driver   and   return   the   initialization   result
   INPUT   PARAMETERS : void
   OUTPUT   PARAMETERS : void
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_Init(void)
{
   return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_CapabilitiesGet
   DESCRIPTION : This   function   returns   the   maximum   number   of   slot
   INPUT   PARAMETERS : void
   OUTPUT   PARAMETERS : void
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_CapabilitiesGet(unsigned   short   *pUiMaxSlotNumber)
{
    return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_Terminate
   DESCRIPTION : This   function   allows   to   terminate   (un-initialize)   the   PCMCIA   driver
   INPUT   PARAMETERS : void
   OUTPUT   PARAMETERS : void
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_Terminate(void)
{
    return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_Open
   DESCRIPTION : This   function   allows   to   open   the   PCMCIA   Driver
   INPUT   PARAMETERS : name_ptr : driver   name
   OUTPUT   PARAMETERS : devID : the   device   Id   to   use   for   all   future   function   calls.
*******************************************************************************/
/*   Please   note   that   this   implementation   has   a   known   restriction   (see   below)
   *   but   it   has   not   been   corrected   to   be   compliant   with   the   ST5518   implementation.
   *   This   restriction   makes   this   series   of   call   impossible :
   *   TDAL_CI_Open("A", &A);
   *   TDAL_CI_Open("B", &B);
   *   TDAL_CI_Close(A);
   *   TDAL_CI_Open("A", &A); <=   Returns   eTDAL_CI_ERROR
   */
#ifdef   TDAL_CIPLUS   /*   for   compatiblity   with   CI   stack   */
tTDAL_CI_Error   TDAL_CI_Open(char   *   name_ptr, unsigned   short   *devId)
#else
tTDAL_CI_Error   TDAL_CI_Open(char   *   name_ptr, short   *devId)
#endif
{
    return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_Read
   DESCRIPTION : This   function   allows   to   read   in   IO   memory   and   attributes   memory
   INPUT   PARAMETERS : devID  : device   identifier
              length_ptr : the   length   of   the   data   to   read
              ptr  : pointer   on   the   read   data
   OUTPUT   PARAMETERS : length : if   length   is   greater   than   0 : length   of   the   data
                     else : an   error   is   occured   TDAL_CI_RD_ERROR
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_Read(unsigned   short   devId,unsigned   short   *length_ptr, unsigned   char   *   ptr)
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_Write
   DESCRIPTION : This function allows to write in IO memory and attributes memory
   INPUT   PARAMETERS : devID : device   identifier
   length : the length of the data to write
   data_ptr : pointer on the data to write
   OUTPUT   PARAMETERS : length : if length is greater than 0 : length of the data
   else : an error is occured
   TDAL_CI_WR_ERROR or TDAL_CI_RD_BUSY
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_Write(unsigned   short   devId, unsigned   short   length, unsigned   char   *   data_ptr)
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_CheckCIS
   DESCRIPTION : This   function   read   and   check   the   CIS
   INPUT   PARAMETERS : devID : device   identifier
   OUTPUT   PARAMETERS :
   SIDE   Effect : Compute   TPCC_RADR   &   TPCC_INDX
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_CheckCIS(unsigned   short   devId)
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_WriteCOR
   DESCRIPTION : This   function   read   and   check   the   CIS
   INPUT   PARAMETERS : devID : device   identifier
   OUTPUT   PARAMETERS :
   SIDE   Effect : Write   the   COR   using   TPCC_RADR   &   TPCC_INDX
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_WriteCOR(unsigned   short   devId)
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_ReadStatus
   DESCRIPTION : This function reads a bit of the Status register
   INPUT   PARAMETERS : devID : device identifier
   bit : bit to read
   OUTPUT PARAMETERS : TRUE : If the bit is set
   FALSE : if the bit is low
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_ReadStatus(unsigned   short   devId, unsigned   char   bit, unsigned   char   *state)
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_WriteCmd
   DESCRIPTION : This   function   sets   or   resets   a   bit   of   the   Command   register
   INPUT   PARAMETERS : devID : device   identifier
              bit : bit   to   read
              value : bit   value   (0, 1)
   OUTPUT   PARAMETERS : value : bit   value   (0, 1)
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_WriteCmd(unsigned   short   devId, unsigned   char   bit, unsigned   short   value)
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_Ctl
   DESCRIPTION : This function allows to set or clear IO signals and read IO signals
   INPUT PARAMETERS : devID : device identifier
   cmd : command indicates the IO bit to read or to write
   ioValue : the value for the writing commands
   OUTPUT PARAMETERS : ioValue : the value for the reading commands (0 or 1)
*******************************************************************************/
#ifdef   TDAL_CIPLUS   /*   for   compatiblity   with   CI   stack   */
tTDAL_CI_Error   TDAL_CI_Ctl(unsigned   short   devId, tTDAL_CI_Control   cmd, unsigned   char   *ioValue)
#else
tTDAL_CI_Error   TDAL_CI_Ctl(unsigned   short   devId, unsigned   short   cmd, unsigned   char   *ioValue)
#endif
{
return eTDAL_CI_NO_ERROR;
}

/*******************************************************************************
   NAME : TDAL_CI_Close
   DESCRIPTION : This   function   allows   to   close   the   PCMCIA   Driver
   INPUT   PARAMETERS : devID : device   identifier
   OUTPUT   PARAMETERS : status : if   status   is   0 : the   device   is   close
                     else : eTDAL_CI_ERROR
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_Close(unsigned   short   devId)
{
return eTDAL_CI_NO_ERROR;
}


#ifdef   TDAL_CIPLUS
/*******************************************************************************
   NAME : TDAL_CI_RegisterCallbacks
   DESCRIPTION : This function registers the callback functions for the
   implementation   to   use
   INPUT   PARAMETERS : handle : handle to the associated Device Identifier
   OUTPUT   PARAMETERS :
   SIDE Effect :
*******************************************************************************/
tTDAL_CI_Error   TDAL_CI_RegisterCallbacks(   unsigned   short   devId
                              ,tTDAL_CI_OnCardMovementFn   cbSTK_DRV_OnCardMovement
                              ,tTDAL_CI_OnModuleInterruptFn   cbSTK_DRV_OnModuleInterrupt
                              ,tTDAL_CI_OnIirSetFn   cbSTK_DRV_OnIirSet)
{
return eTDAL_CI_NO_ERROR;
}
#endif

#ifdef   DUMP_COMMAND
LOCAL   void   p_TDAL_CI_Dump(uint32_t   Length, uint8_t*   Data, bool   Write, uint32_t   BeginSize)
{
}
#endif
