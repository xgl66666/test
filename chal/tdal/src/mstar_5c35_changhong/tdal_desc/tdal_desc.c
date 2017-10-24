/******************************************************************************
 *                   COPYRIGHT 2008 IWEDIA TECHNOLOGIES                   *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DESC
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_desc/tdal_desc.c $
 *          $Rev: 414 $
 *          $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DESC implementation
 *
 *****************************************************************************/

/********************************************************/
/*          Includes                     */
/********************************************************/

#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tdal_common.h"

#include "tdal_dmx.h"
#include "tdal_desc.h"


#include "tbox.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvCA.h"
#include "drvDSCMB.h"
#include "drvNGA.h"
//#include "nocs1x_csd_impl.h"
//#include "nocs1x_csd.h"

/********************************************************/
/*          Defines                         */
/********************************************************/
#define   kTDAL_DESC_MAX_CHANNELS   40
#define   kTDAL_DESC_MAX_FORBIDDEN   8
#define   kTDAL_DESC_KEY_SIZE 8
#define   kTDAL_DESC_FORBIDDEN_DEF_PID   0xFFFF
#define kTDAL_DESC_OFFSET_LENGTH 16
#define CSD_DEVICENAME "NOCS1X"        /**< The device name of the underlying ST secure library. */

/* Define this to use the CSD library in non-secure (clear-text) mode */
#define TDAL_DESC_USE_CSD_FOR_UNSECURE

/********************************************************/
/*          Macros                         */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_DESC);

/********************************************************/
/*          Typedefs                     */
/********************************************************/
typedef   struct
{
   MS_BOOL   used;
   tTDAL_DESC_descrambler descId;
   MS_U32   descHandle;
   TDAL_DESC_stream_type_e stream_type;
   MS_U32 Pid;
   TDAL_DESC_DescType   desc_type;
   uint8_t dataDescEven[kTDAL_DESC_KEY_SIZE];    /* Even Keys */
   uint8_t dataDescOdd[kTDAL_DESC_KEY_SIZE];     /* Odd Keys */
}tTDAL_DESC_table_t;

/* Descrambler synchronisation structure */
typedef struct
{
    tTDAL_DMX_ChannelStream ChannelStream;      /* channel type         */
    int32_t ChannelPid;         /* Pid of this Channel  */
}tTDAL_DESC_channel_t;

char const * DBG_TCsdStatus( TCsdStatus s )
{
    switch( s )
    {
        case CSD_NO_ERROR:                          return "NO_ERROR";
        case CSD_ERROR:                             return "OPERATION_FAILED";
        case CSD_ERROR_DRIVER_ALREADY_INITIALIZED:  return "DRIVER_ALREADY_INITIALIZED";
        case CSD_ERROR_INVALID_PARAMETERS:          return "INVALID_PARAMETERS";
        case CSD_ERROR_OPERATION_NOT_ALLOWED:       return "OPERATION_NOT_ALLOWED";
        case CSD_ERROR_OPERATION_NOT_SUPPORTED:     return "OPERATION_NOT_SUPPORTED";
        case CSD_ERROR_INVALID_KEY_PATH_HANDLE:     return "INVALID_KEY_PATH_HANDLE";
        case LAST_CSD_STATUS:                       return "LAST_CSD_STATUS";
        default:                                    return "??";
    }
}

/********************************************************/
/*          Global   Variables   (GLOBAL/IMPORT)      */
/********************************************************/

/********************************************************/
/*          Local   Module   Variables   (MODULE)        */
/********************************************************/

/********************************************************/
/*          Local   File   Variables   (LOCAL)        */
/********************************************************/
LOCAL   MS_S32   TDAL_DESC_table_lock;
LOCAL   MS_S32   TDAL_DESC_table_sync;
LOCAL   tTDAL_DESC_table_t   TDAL_DESC_table[kTDAL_DESC_MAX_DESCRAMBLERS];
LOCAL   tTDAL_DESC_channel_t TDAL_DESC_channels[kTDAL_DESC_MAX_CHANNELS];
LOCAL   uint16_t   TDAL_DESC_channelforbidden[kTDAL_DESC_MAX_FORBIDDEN];
LOCAL   bool   TDAL_DESC_isInit = FALSE;
LOCAL   bool   TDAL_DESC_isSecureMode = FALSE;

/********************************************************/
/*          Functions   Definitions   (LOCAL/GLOBAL)   */
/********************************************************/
/* Static callbacks that the demux driver may use to update the descrambler driver */
void TDAL_DESC_DemuxPidCallback     ( tTDAL_DMX_ChannelId const, uint16_t const );
void TDAL_DESC_DemuxChannelCallback ( tTDAL_DMX_ChannelId const, tTDAL_DMX_ChannelStream const);
/* Local CSD-related activations */

/*===================================================================
   *
   *   TDAL_DESC_Init
   *
   *   Parameters:
   *   none.
   *
   *   Description:
   *
   *
   *   Returns:   tTDAL_DESC_Error
   *   eTDAL_DESC_ERROR   if   a   "create"   fails,
   *   eTDAL_DESC_NO_ERROR   otherwise.
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Init(void)
{
    uint8_t   i;
    MS_BOOL errorCA;
    TCsdStatus e;
    if (TDAL_DESC_isInit == FALSE)
    {
       TDAL_DESC_table_lock = MsOS_CreateSemaphore(1,E_MSOS_FIFO,"TDAL_DESC_table_lock");
       TDAL_DESC_table_sync = MsOS_CreateSemaphore(1,E_MSOS_FIFO,"TDAL_DESC_table_sync");

    /* Clear internals */
        memset( &TDAL_DESC_table, 0, sizeof( TDAL_DESC_table ) );
        memset( &TDAL_DESC_channels, 0, sizeof( TDAL_DESC_channels ) );
        memset( &TDAL_DESC_channelforbidden, 0, sizeof( TDAL_DESC_channelforbidden ) );


    /* Initialize descrambler table */
        for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
        {
            TDAL_DESC_table[i].used = FALSE;
            TDAL_DESC_table[i].descId = DRV_DSCMB_FLT_NULL;
        }

    /* Initialize channel table */
        for(i=0 ; i < kTDAL_DESC_MAX_CHANNELS; i++ )
        {
            TDAL_DESC_channels[i].ChannelPid = kTDAL_DESC_ILLEGAL_PID;
            TDAL_DESC_channels[i].ChannelStream = 0xFF;
        }

    /* Initialize forbidden channel table */
        for (i=0; i < kTDAL_DESC_MAX_FORBIDDEN; i++)
        {
            TDAL_DESC_channelforbidden[i] = kTDAL_DESC_FORBIDDEN_DEF_PID;
        }

        errorCA=MDrv_CA_Init();
        MDrv_DSCMB_Init(); 
        MDrv_DSCMB2_SetDefaultCAVid(0,0x02);
        MDrv_AESDMA_SetDefaultCAVid(0x02);
        /* Initialize the CSD library */
        e = csdInitialize(NULL);
        if( CSD_NO_ERROR != e )
        {
             mTBOX_TRACE(( kTBOX_NIV_WARNING, "%s: failed disabling secure layer (%s)\n", DBG_TCsdStatus( e ) ));
        }
       // e = csdEnableScs();

        TDAL_DESC_isInit = TRUE;
        return  eTDAL_DESC_NO_ERROR;
    }
    return  eTDAL_DESC_NOT_DONE;
}

/******************************************************************************
   *   Function   Name   :   TDAL_DESC_Terminate
   *
   *   Description   :   This   function   free   a   hpi   desc   channel.
   *
   *   Side   effects   :
   *
   *              return      kTDAL_DESC_NO_ERROR   if   all   is   correct.
   *                   kTDAL_DMX_ERROR_NOT_DONE   if   the   hpi   unsubscribe   return
   *                   an   error.
   *
   *   Comment      :
   *
   *****************************************************************************/
tTDAL_DESC_Error   TDAL_DESC_Terminate(void)
{
    if (TDAL_DESC_isInit == TRUE)
    {
    MsOS_DeleteSemaphore(TDAL_DESC_table_lock);
    MsOS_DeleteSemaphore(TDAL_DESC_table_sync);
    TDAL_DESC_isInit = FALSE;
    return eTDAL_DESC_NO_ERROR;
    }
    
    return eTDAL_DESC_NOT_DONE;
}


/*===================================================================
   *
   *   TDAL_DESC_Open_Descrambler
   *
   *   Parameters:
   *   track   :   the   stream   track.
   *
   *   Description:
   *   This   function   opens   a   descrambling   channel.
   *
   *   Returns:   tTDAL_DESC_descrambler
   *   Returns   a   handle   to   a   descrambling   channel.
   *   Or   TDAL_DESC_ILLEGAL_DESCRAMBLER   if   the   channel   can't   be   allocated.
   *
   *==================================================================*/
tTDAL_DESC_descrambler   TDAL_DESC_Open_Descrambler(TDAL_DESC_stream_type_e   stream_type) 
{
    MS_BOOL errorDSCMB;
    tTDAL_DESC_descrambler     descId = kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    uint8_t   i;
    
    /*     not   used   */
    if (stream_type)     {};
    
    if (TDAL_DESC_isInit == FALSE)
    {
       return kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    }

    MsOS_ObtainSemaphore (TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    for   (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if    (TDAL_DESC_table[i].used != TRUE)
        {
            TDAL_DESC_table[i].stream_type = stream_type;
            descId = i;
            break;
        }
    }
    
    if (descId != kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
       /*    Allocate   a   descrambler     */     
       if((TDAL_DESC_table[descId].descId = MDrv_DSCMB_FltAlloc()) == DRV_DSCMB_FLT_NULL)        
       {
           MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
           mTBOX_TRACE(( kTBOX_NIV_1, "MDrv_DSCMB_FltAlloc() TDAL_DESC_table[descId].descId %x failed\n",TDAL_DESC_table[descId].descId));
           return kTDAL_DESC_ILLEGAL_DESCRAMBLER;
       }
       else
       {
           TDAL_DESC_table[descId].used = TRUE;
       }
    }
    else 
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return     kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    }
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return     (TDAL_DESC_table[descId].descId);
}

/*===================================================================
   *
   *   TDAL_DESC_Close_Descrambler
   *
   *   Parameters:
   *   descId   :   descrambling   channel   to   close.
   *
   *   Description:
   *   This   function   closes   a   descrambling   channel.
   *
   *   Returns:   tTDAL_DESC_Error
   *   The   returned   value   is   eTDAL_DESC_NO_ERROR   in   case   of   success,
   *   eTDAL_DESC_ERROR   in   case   of   failure   :   if   descrambler   was   not   a   valid   open   descrambler.
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Close_Descrambler(tTDAL_DESC_descrambler   descId)
{
    MS_BOOL errorDSCMB = TRUE;
    uint8_t   i;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    
    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_ERROR);
    }
    MsOS_ObtainSemaphore (TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);

     /* Check if descrambler used */
    for   (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if(TDAL_DESC_table[i].descId == descId)
        {
            tempDescId = descId;
            break;
        }
    }
    if (tempDescId == kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
    if (TDAL_DESC_table[i].used != TRUE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
    errorDSCMB = MDrv_DSCMB_FltFree(descId);
    if( errorDSCMB == FALSE )
    {
        mTBOX_TRACE(( kTBOX_NIV_WARNING, "TDAL_DESC_Close_Descrambler could not free descrambler resources\n"));
        TDAL_DESC_table[i].used = FALSE;
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_NOT_DONE);
    }

    /* Mark descrambler as free */
    TDAL_DESC_table[i].used = FALSE;
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);

    return(eTDAL_DESC_NO_ERROR);
}

/*===================================================================
   *
   *   TDAL_DESC_Set_Descrambler_Pid
   *
   *   Parameters:
   *   descId   :   descrambling   channel   as   returned   by   descrambler_open.
   *   pid   :   PID   of   transport   stream   packet   to   descramble.
   *
   *   Description:
   *   This   function   sets   the   packets   ID   of   a   stream   to   descramble.
   *
   *   Returns:   tTDAL_DESC_Error
   *   The   returned   value   is   eTDAL_DESC_NO_ERROR   in   case   of   success,
   *   eTDAL_DESC_ERROR   in   case   of   failure   :   if   descrambler   was   not   valid   open   descrambler
   *   or   pid   is   not   valid.
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Set_Descrambler_Pid(tTDAL_DESC_descrambler   descId, int16_t   pid)
{
    MS_BOOL errorDSCMB = TRUE;
    uint8_t   i,k;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_NOT_DONE);
    }
    
    MsOS_ObtainSemaphore (TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /*     Check     if   descrambler    used   */
    for   (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if    (TDAL_DESC_table[i].descId == descId)
        {
            tempDescId = descId;
            break;
        }
    }
    if (tempDescId == kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
    if (TDAL_DESC_table[i].used != TRUE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }

    /*     ---------------------------   */
    /*     Check     the   pid     authorization     */
    /*     ---------------------------   */
    for(k=0; k < kTDAL_DESC_MAX_FORBIDDEN; k++)
    {
        if(TDAL_DESC_channelforbidden[k] == pid)
        {
            MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
            return   eTDAL_DESC_NOT_DONE;
        }
    }

    MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
    TDAL_DESC_table[i].Pid = 0;
    for (k=0 ; (k < kTDAL_DESC_MAX_CHANNELS) ; k++)
    {
        if ((TDAL_DESC_channels[k].ChannelPid == pid) &&
           (TDAL_DESC_channels[k].ChannelStream != eTDAL_DMX_PCR_STREAM))
        {
        /* DMX channel retrieved */
            mTBOX_TRACE(( kTBOX_NIV_1, "TDAL_DESC_Set_Descrambler_Pid TDAL_DESC_channels[%d].ChannelPid : %d\n",i,pid));
            TDAL_DESC_table[i].Pid = pid;
            break;
        }
    }
    MsOS_ReleaseSemaphore(TDAL_DESC_table_sync);

    if(k == kTDAL_DESC_MAX_CHANNELS)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return   eTDAL_DESC_NO_ERROR;
    }

    /*     ------------------------------   */
    /*     Associate     Descrambler   with   PID    */
    /*     ------------------------------   */

    if(MDrv_DSCMB_FltConnectPid(tempDescId, TDAL_DESC_table[i].Pid) == FALSE)
    {
        mTBOX_TRACE(( kTBOX_NIV_1, "MDrv_DSCMB_FltConnectPid   ERROR=%x\n",errorDSCMB));
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return    eTDAL_DESC_ERROR;
    }

    if(MDrv_DSCMB_FltTypeSet(descId, E_DSCMB_TYPE_CSA) == FALSE)        
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR);
    } 

    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return eTDAL_DESC_NO_ERROR;

}

/*===================================================================
   *
   *   TDAL_DESC_Set_Descrambler_Keys
   *
   *   Parameters:
   *   descId   :   descrambling   channel   as   returned   by   TDAL_DESC_Open_Descrambler.
   *   odd_key_length   :   length   in   byte   of   the   odd   key.
   *   odd_key   :   pointer   to   the   odd   key   buffer.
   *   even_key_length   :   length   in   byte   of   even   key.
   *   even_key   :   pointer   to   the   even   key   buffer.
   *
   *   Description:
   *   This   function   sets   the   control   keys   of   a   stream   to   descramble.
   *
   *   Returns:   tTDAL_DESC_Error
   *   The   returned   value   is   eTDAL_DESC_NO_ERROR   in   case   of   success,
   *   eTDAL_DESC_ERROR   in   case   of   failure   :   if   descrambler   was   not   a   valid   open   descrambler.
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Set_Descrambler_Keys(tTDAL_DESC_descrambler   descId,
                   int16_t   odd_key_length,
                   const   int8_t   *odd_key,
                   int16_t   even_key_length,
                   const   int8_t   *even_key)
{
    MS_BOOL errorDSCMB = TRUE;
    uint8_t   i,k;
    TCsdStatus csdStatus;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    TCsdDscKeyPathHandle DscKeyPathHandle;
    TCsdUnsignedInt16         xEmi = 0;
    DSCMB_Info* Info;
    DSCMB_Status * pStatus;
    
    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_NOT_DONE);
    }
    
    MsOS_ObtainSemaphore (TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /*     Check     if   descrambler    used   */
    for   (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if    (TDAL_DESC_table[i].descId == descId)
        {
            tempDescId = descId;
            break;
        }
    }
    if (tempDescId == kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
    if (TDAL_DESC_table[i].used != TRUE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
/*
    if(MDrv_DSCMB_FltKeySet(descId, E_DSCMB_KEY_ODD, odd_key) == FALSE)        
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR); 
    }
    if(MDrv_DSCMB_FltKeySet(descId, E_DSCMB_KEY_EVEN, even_key) == FALSE)        
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR);       
    }*/
    MDrv_AESDMA_Lock();
    DscKeyPathHandle.u32DscmbId = descId;
    csdStatus = csdSetClearTextDscHostKeys( xEmi, odd_key, odd_key_length, even_key, even_key_length, &DscKeyPathHandle );
    if(csdStatus != CSD_NO_ERROR)    
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"csdSetClearTextDscHostKeys %s\n",DBG_TCsdStatus(csdStatus)));
        MDrv_AESDMA_Unlock();
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR);
    }

    MDrv_AESDMA_Unlock();
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return   eTDAL_DESC_NO_ERROR;
}

/** Checking if a descrambler resource handle is currently valid for use.
 *
 * @param[in] descrambler is a handle returned by TDAL_DESC_Open_Descrambler.
 *
 * @return eTDAL_DESC_NO_ERROR if the argument descrambler resource is currently valid for use.
 * @return eTDAL_DESC_ERROR_UNKNOW_ID if the argument descrambler resource is not currently valid.
 * @return eTDAL_DESC_NOT_DONE if layer is not initialized.
 *
 * @note At worst, the returned state of descrambler resource handle is only valid during the call.
 *
 */
tTDAL_DESC_Error TDAL_DESC_Is_Descrambler_Open( tTDAL_DESC_descrambler descrambler )
{
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    uint8_t   i;
    mTBOX_FCT_ENTER( "TDAL_DESC_Is_Descrambler_Open" );
    /* Sanitize on current state */
    if  (TDAL_DESC_isInit == FALSE)
    {
        mTBOX_RETURN( eTDAL_DESC_NOT_DONE );
    }

    /* ------------------------------------- */
    /* Lock Descr Protection Table Semaphore */
    /* ------------------------------------- */
    MsOS_ObtainSemaphore (TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /*     Check     if   descrambler    used   */
    for   (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if (TDAL_DESC_table[i].descId == descrambler)
        {
            tempDescId = descrambler;
            break;
        }
    }
    /* Check if descrambler used */     
    if (tempDescId == kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
    if (TDAL_DESC_table[i].used != TRUE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }

    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return   eTDAL_DESC_NO_ERROR;
}

/** Assigning deciphering keys to a secure descrambler resource.
 *
 * @param[in] descrambler is a handle returned by TDAL_DESC_Open_Descrambler.
 * @param[in] L1_key_length is the byte length of the L1 key.
 * @param[in] L1_key is the buffer containing the L1 key.
 * @param[in] odd_key_length is the byte length of the deciphering odd key.
 * @param[in] odd_key is the buffer containing the odd key.
 * @param[in] even_key_length is the byte length of the deciphering even key.
 * @param[in] even_key is the buffer containing to the even key.
 *
 * @return eTDAL_DESC_NO_ERROR if the assignment is successful.
 * @return eTDAL_DESC_ERROR if the operation failed.
 * @return eTDAL_DESC_ERROR_UNKNOW_ID if the descrambler resource is not valid.
 * @return eTDAL_DESC_NOT_DONE if layer is not initialized.
 *
 */
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys_L1(tTDAL_DESC_descrambler descrambler,
                            int16_t L1_key_length,
                            const int8_t *L1_key,
                            int16_t odd_key_length,
                            const int8_t *odd_key,
                            int16_t even_key_length,
                            const int8_t *even_key)
{
    MS_BOOL errorDSCMB = TRUE;
    uint8_t   i,k;
    TCsdStatus csdStatus;
    TCsdDscCipheredProtectingKeys dsc2CipheredProtectingKeysTable;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    TCsdDscKeyPathHandle DscKeyPathHandle;
    TCsdDscKeyPathHandle* pDscKeyPathHandle=NULL;
    TCsdUnsignedInt16         xEmi = 0;
    const TCsdUnsignedInt8 L2ProtectingKey[] = {0xe4, 0x21, 0x13, 0xa7, 0x99, 0xca, 0xf4, 0xc7, 0xb8, 0x73, 0x07, 0x00, 0x7b, 0xde, 0xb2, 0xbb};


    printf(">>>>>>>>>>>>>>> TDAL_DESC_Set_Descrambler_Keys_L1 ENTER <<<<<<<<<<<<<<<<<<<<<<<<\n");

    if (TDAL_DESC_isInit == FALSE)
    {
        printf(">>>>>>>>>>>>>>> TDAL_DESC_isInit == FALSE <<<<<<<<<<<<<<<<<<<<<<<<\n");
        return(eTDAL_DESC_NOT_DONE);
    }

    MsOS_ObtainSemaphore (TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /*     Check     if   descrambler    used   */
    for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if (TDAL_DESC_table[i].descId == descrambler)
        {
            tempDescId = descrambler;
            break;
        }
    }
    if (tempDescId == kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
    if (TDAL_DESC_table[i].used != TRUE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return (eTDAL_DESC_ERROR_UNKNOW_ID);
    }
/*
    if(MDrv_DSCMB_FltKeySet(descrambler, E_DSCMB_KEY_ODD, odd_key) == FALSE)        
    {
       MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
       return(eTDAL_DESC_ERROR); 
    }
    if(MDrv_DSCMB_FltKeySet(descrambler, E_DSCMB_KEY_EVEN, even_key) == FALSE)        
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR);       
    }
*/
    if(KEY_LADDER_LEVEL == 3)
    {
        memcpy (&dsc2CipheredProtectingKeysTable[0], L2ProtectingKey, sizeof(L2ProtectingKey));
    }

    if((L1_key != NULL) && (L1_key_length != 0))
    {
        memcpy (&dsc2CipheredProtectingKeysTable[KEY_LADDER_LEVEL-2], L1_key, L1_key_length);
    }
    else
    {
        memset (&dsc2CipheredProtectingKeysTable[KEY_LADDER_LEVEL-2], 0x0, kTDAL_DESC_OFFSET_LENGTH);
    }
    if(TDAL_DESC_table[i].Pid!=0)
    { /*if channel is found*/
        DscKeyPathHandle.u32DscmbId = descrambler;
        pDscKeyPathHandle = &DscKeyPathHandle;
    }
    MDrv_AESDMA_Lock();
    csdStatus = csdSetProtectedDscContentKeys(xEmi, dsc2CipheredProtectingKeysTable,odd_key, odd_key_length, even_key, even_key_length, pDscKeyPathHandle );

    if(csdStatus != CSD_NO_ERROR)     
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"csdSetProtectedDscContentKeys %s\n",DBG_TCsdStatus(csdStatus)));
        MDrv_AESDMA_Unlock();
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR);
    }

    MDrv_AESDMA_Unlock();
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);

    printf(">>>>>>>>>>>>>>> TDAL_DESC_Set_Descrambler_Keys_L1 EXIT <<<<<<<<<<<<<<<<<<<<<<<<\n");

    return   eTDAL_DESC_NO_ERROR;
}

/*===================================================================
   *
   *   TDAL_DESC_Set_Descrambler_Type
   *
   *   Parameters:
   *   descId   :   descrambling   channel   as   returned   by   TDAL_DESC_Open_Descrambler.
   *   descrambler_type   :   tye   of   descrambler
   *
   *   Description:
   *   This   function   set   the   descrambler   type
   *
   *   Returns:   tTDAL_DESC_Error
   *   The   returned   value   is   eTDAL_DESC_NO_ERROR   in   case   of   success,
   *   eTDAL_DESC_ERROR   in   case   of   failure   :   if   descrambler   was   not   previously   forbidden
   *   or   there   is   not   enough   place   for   more   PIDs.
   *
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Set_Descrambler_Type   (tTDAL_DESC_descrambler   descId,
        TDAL_DESC_DescType   descrambler_type)
{
    return   eTDAL_DESC_NO_ERROR;
}

/*===================================================================
   *
   *   TDAL_DESC_Stream_Authorisation
   *
   *   Parameters:
   *   pid   :   PID   of   transport   stream   packet   to   Forbid   or   re-Authorize.
   *   authorize   :   FALSE   to   inhib, TRUE   to   re-authorize.
   *
   *   Description:
   *   This   function   allows   the   control   of   the   descrambler   from   oustide   on   selected   PID.
   *
   *   Returns:   tTDAL_DESC_Error
   *   The   returned   value   is   eTDAL_DESC_NO_ERROR   in   case   of   success,
   *   eTDAL_DESC_ERROR   in   case   of   failure   :   if   descrambler   was   not   previously   forbidden
   *   or   there   is   not   enough   place   for   more   PIDs.
   *
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Stream_Authorisation(int16_t   pid, bool   authorize)
{
    return(eTDAL_DESC_NO_ERROR);
}

/**========================================================================**
   *   Function      :   TDAL_DESC_APIRevisionGet
   *
   *   Description   :
   *
   *   Side   effects   :
   *
   *   Comment      :
   *
   **========================================================================**/
const   char   *TDAL_DESC_APIRevisionGet(void)
{
   static   const   char   API_Revision[] = "TDAL_DESC_v1.0";

   return((const   char   *)API_Revision);

}

/**========================================================================**
   *   Function      :   TDAL_DESC_PlatformRevisionGet
   *
   *   Description   :
   *
   *   Side   effects   :
   *
   *   Comment      :
   *
   **========================================================================**/
const   char   *TDAL_DESC_PlatformRevisionGet(void)
{
   static   const   char   PLTF_Revision[] = "STFAEA16_7109";

   return((const   char   *)PLTF_Revision);
}

/**========================================================================**
   *   Function      :   TDAL_DESC_PlatformRevisionGet
   *
   *   Description   :
   *
   *   Side   effects   :
   *
   *   Comment      :
   *
   **========================================================================**/
tTDAL_DESC_Error   TDAL_DESC_GetCapability(tTDAL_DESC_Capability   *pstCapability)
{
   return(eTDAL_DESC_NO_ERROR);
}

/** @internal
 *
 * Engaging the secure mode.
 *
 * @note Controlling the secure mode changes the way the keys are sent to
 * the descrambler. See the CSD documentation for directions.
 *
 * @return eTDAL_DESC_ERROR if the secure layer cannot be initialized.
 * @return eTDAL_DESC_NO_ERROR if the secure mode is engaged.
 *
 */
tTDAL_DESC_Error TDAL_DESCi_EnableSecureMode( void )
{

    return( eTDAL_DESC_NO_ERROR );

}


/** @internal
 *
 * Disabling the secure mode.
 *
 * @note Instead of reinitializing the CSD library in non-secure mode, this
 * function simply terminates it. Keys should be sent as usual on a non-secure
 * architecture.
 *
 * @warning If the secure mode cannot be disabled, the state of the
 * TDAL_DESC layer will remain in secure mode.
 *
 * @return eTDAL_DESC_ERROR if the secure layer cannot be terminated.
 * @return eTDAL_DESC_NO_ERROR if the secure mode is disabled.
 *
 */
tTDAL_DESC_Error TDAL_DESCi_DisableSecureMode( void )
{

     return( eTDAL_DESC_NO_ERROR );

}

/** Notifying that a demultiplexer channel changed PID, in order to mirror
 * the channel information in this layer.
 *
 * @param[in] channel is the identifier of the channel.
 * @param[in] pid is the new PID value configured for the channel.
 *
 */ 
void TDAL_DESC_DemuxPidCallback( tTDAL_DMX_ChannelId const channel, uint16_t const pid )
{
    /* Check layer status, and mirror the channel information */
    if   (TDAL_DESC_isInit == TRUE)
    {
        MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
        TDAL_DESC_channels[ channel ].ChannelPid = pid;
        MsOS_ReleaseSemaphore( TDAL_DESC_table_sync );
    }
}

/** Notifying that a demultiplexer channel was opened or closed, in order to mirror
 * the channel information in this layer.
 *
 * @param[in] channel is the identifier of the channel.
 * @param[in] channel_stream is the type of the channel.
 * @param[in] slot_handle is the STPTI handle associated to the channel.
 *
 */
void TDAL_DESC_DemuxChannelCallback( tTDAL_DMX_ChannelId const channel, tTDAL_DMX_ChannelStream const channel_stream)
{
    /* Check layer status, and mirror the channel information */
    if (TDAL_DESC_isInit == TRUE)
    {
        MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
        TDAL_DESC_channels[ channel ].ChannelStream = channel_stream;
        MsOS_ReleaseSemaphore( TDAL_DESC_table_sync );
    }
}
