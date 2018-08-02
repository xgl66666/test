/******************************************************************************
 *                   COPYRIGHT 2008 IWEDIA TECHNOLOGIES                       *
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
/*          Includes                                    */
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
#include "MsCommon.h"
#include "drvCA.h"
#include "drvDSCMB.h"
#include "drvCIPHER.h"
#include "drvAKL.h"
#include "drvNGA.h"
#include "nocs_csd_impl.h"
#include "nocs_csd.h"

/********************************************************/
/*          Defines                                     */
/********************************************************/
#define kTDAL_DESC_MAX_CHANNELS         40
#define kTDAL_DESC_MAX_FORBIDDEN        8
#define kTDAL_DESC_KEY_SIZE             8
#define kTDAL_DESC_FORBIDDEN_DEF_PID    0xFFFF
#define kTDAL_DESC_OFFSET_LENGTH        16

#define DSCMB_PIDSLOT_COUNT             128

/********************************************************/
/*          Macros                                      */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_DESC);

/********************************************************/
/*          Typedefs                                    */
/********************************************************/
typedef struct
{
    MS_BOOL used;
    tTDAL_DESC_descrambler descId;
    MS_U32 descHandle;
    TDAL_DESC_stream_type_e stream_type;
    MS_U32 Pid;
    TDAL_DESC_DescType desc_type;
    uint8_t dataDescEven[kTDAL_DESC_KEY_SIZE];  /* Even Keys */
    uint8_t dataDescOdd[kTDAL_DESC_KEY_SIZE];   /* Odd Keys */
    uint16_t emi;                               /* EMI */
}tTDAL_DESC_table_t;

/* Descrambler synchronisation structure */
typedef struct
{
    tTDAL_DMX_ChannelStream ChannelStream;      /* channel type */
    int32_t ChannelPid;                         /* Pid of this Channel  */
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
/*          Global   Variables   (GLOBAL/IMPORT)        */
/********************************************************/

/********************************************************/
/*          Local   Module   Variables   (MODULE)       */
/********************************************************/

/********************************************************/
/*          Local   File   Variables   (LOCAL)          */
/********************************************************/
LOCAL MS_S32 TDAL_DESC_table_lock;
LOCAL MS_S32 TDAL_DESC_table_sync;
LOCAL tTDAL_DESC_table_t   TDAL_DESC_table[kTDAL_DESC_MAX_DESCRAMBLERS];
LOCAL tTDAL_DESC_channel_t TDAL_DESC_channels[kTDAL_DESC_MAX_CHANNELS];
LOCAL uint16_t TDAL_DESC_channelforbidden[kTDAL_DESC_MAX_FORBIDDEN];
LOCAL bool TDAL_DESC_isInit = FALSE;
#if 0
LOCAL bool TDAL_DESC_isSecureMode = FALSE;
#endif

/********************************************************/
/*          Functions   Definitions   (LOCAL/GLOBAL)    */
/********************************************************/
extern tTDAL_DMX_Error TDAL_DMXi_GetFilterByPid(uint16_t pid, uint16_t *filt);
extern MS_S32 gs32NonCachedPoolID;

/* Static callbacks that the demux driver may use to update the descrambler driver */
void TDAL_DESC_DemuxPidCallback     ( tTDAL_DMX_ChannelId const, uint16_t const );
void TDAL_DESC_DemuxChannelCallback ( tTDAL_DMX_ChannelId const, tTDAL_DMX_ChannelStream const);
/* Local CSD-related activations */

/*===================================================================
   *
   *   TDAL_DESC_Init
   *
   *   Parameters:
   *     none.
   *
   *   Description:
   *
   *   Returns:   tTDAL_DESC_Error
   *     eTDAL_DESC_ERROR   if a "create" fails,
   *     eTDAL_DESC_NO_ERROR   otherwise.
   *
   *==================================================================*/
tTDAL_DESC_Error TDAL_DESC_Init(void)
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

        errorCA = MDrv_CA_Init();
        MDrv_DSCMB_Init(); 
        MDrv_CIPHER_Init(); 
        MDrv_DSCMB2_SetDefaultCAVid(0,0x02); // 0x02 Nagra Chip
        MDrv_CIPHER_KeyCtrl(TRUE);

#if( defined(NAGRA_CHIP_5C35) && (NAGRA_CHIP_5C35 == 1))
        MDrv_DSCMB2_SetDefaultCAVid(0,0x02); // 0x02 Nagra Chip
        MDrv_AESDMA_SetDefaultCAVid(0x02);   // 0x02 Nagra Chip
#endif
        /* Initialize the CSD library */
        e = csdInitialize(NULL);
        if( CSD_NO_ERROR != e )
        {
             mTBOX_TRACE(( kTBOX_NIV_WARNING, "%s: failed disabling secure layer (%s)\n", DBG_TCsdStatus( e ) ));
        }

        TDAL_DESC_isInit = TRUE;
        return  eTDAL_DESC_NO_ERROR;
    }
    return  eTDAL_DESC_NOT_DONE;
}

/******************************************************************************
   *   Function Name :   TDAL_DESC_Terminate
   *
   *   Description   :   This function free a hpi desc channel.
   *
   *   Side effects  :
   *
   *   Returns       :
   *     return kTDAL_DESC_NO_ERROR if all is correct.
   *     kTDAL_DMX_ERROR_NOT_DONE if the hpi unsubscribe return an error.
   *
   *   Comment       :
   *
   *****************************************************************************/
tTDAL_DESC_Error TDAL_DESC_Terminate(void)
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
   *     track   : the stream track.
   *
   *   Description:
   *     This function opens a descrambling channel.
   *
   *   Returns:   tTDAL_DESC_descrambler
   *     Returns a handle to a descrambling channel.
   *     Or TDAL_DESC_ILLEGAL_DESCRAMBLER if the channel can't be allocated.
   *
   *==================================================================*/
tTDAL_DESC_descrambler TDAL_DESC_Open_Descrambler(TDAL_DESC_stream_type_e stream_type)
{
    tTDAL_DESC_descrambler descId = kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    uint8_t i;

    /* not used */
    if (stream_type)     {};

    if (TDAL_DESC_isInit == FALSE)
    {
        return kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    }

    MsOS_ObtainSemaphore(TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if (TDAL_DESC_table[i].used != TRUE)
        {
            TDAL_DESC_table[i].stream_type = stream_type;
            descId = i;
            break;
        }
    }

    if (descId != kTDAL_DESC_ILLEGAL_DESCRAMBLER)
    {
        /* Allocate a descrambler */
        if ((TDAL_DESC_table[descId].descId = MDrv_DSCMB_FltAlloc()) == DRV_DSCMB_FLT_NULL)
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
        return kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    }
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return (TDAL_DESC_table[descId].descId);
}

/*===================================================================
   *
   *   TDAL_DESC_Close_Descrambler
   *
   *   Parameters:
   *   descId   :   descrambling channel to close.
   *
   *   Description:
   *     This function closes a descrambling channel.
   *
   *   Returns:   tTDAL_DESC_Error
   *     The returned value is eTDAL_DESC_NO_ERROR in case of success,
   *     eTDAL_DESC_ERROR in case of failure : if descrambler was not a valid open descrambler.
   *
   *==================================================================*/
tTDAL_DESC_Error TDAL_DESC_Close_Descrambler(tTDAL_DESC_descrambler descId)
{
    MS_BOOL errorDSCMB = TRUE;
    uint8_t   i;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    
    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_ERROR);
    }
    MsOS_ObtainSemaphore(TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);

     /* Check if descrambler used */
    for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
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
   *     descId : descrambling channel as returned by descrambler_open.
   *     pid    : PID of transport stream packet to descramble.
   *
   *   Description:
   *     This function sets the packets ID of a stream to descramble.
   *
   *   Returns:   tTDAL_DESC_Error
   *     The returned value is eTDAL_DESC_NO_ERROR in case of success,
   *     eTDAL_DESC_ERROR in case of failure : if descrambler was not valid open descrambler
   *     or pid is not valid.
   *
   *==================================================================*/
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Pid(tTDAL_DESC_descrambler descId, int16_t pid)
{
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER;
    uint8_t i,k;

    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_NOT_DONE);
    }
    
    MsOS_ObtainSemaphore(TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /* Check if descrambler used */
    for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if (TDAL_DESC_table[i].descId == descId)
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

    /* --------------------------- */
    /* Check the pid authorization */
    /* --------------------------- */
    for(k=0; k < kTDAL_DESC_MAX_FORBIDDEN; k++)
    {
        if(TDAL_DESC_channelforbidden[k] == pid)
        {
            MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
            return eTDAL_DESC_NOT_DONE;
        }
    }

    MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
    TDAL_DESC_table[i].Pid = 0;
    for (k=0; k < kTDAL_DESC_MAX_CHANNELS; k++)
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

    if (k == kTDAL_DESC_MAX_CHANNELS)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_NO_ERROR;
    }

    /* --------------------------------- */
    /* Associate Descrambler with filter */
    /* --------------------------------- */
#if 0 // use filter ID
    uint16_t filtId;
    if (TDAL_DMXi_GetFilterByPid(pid, &filtId) != kTDAL_DMX_NO_ERROR)
    {
        mTBOX_TRACE(( kTBOX_NIV_1, "TDAL_DMXi_GetFilterByPid\n"));
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }

    if (MDrv_DSCMB_FltConnectFltId(descId, filtId) == FALSE)
    {
        mTBOX_TRACE(( kTBOX_NIV_1, "MDrv_DSCMB_FltConnectFltId\n"));
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }
#else // use PID
    MDrv_DSCMB_FltConnectPid(descId, pid);
#endif

    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return eTDAL_DESC_NO_ERROR;
}

/*===================================================================
   *
   *   TDAL_DESC_Set_Descrambler_Keys
   *
   *   Parameters:
   *     descId          : descrambling channel as returned by TDAL_DESC_Open_Descrambler.
   *     odd_key_length  : length in byte of the odd key.
   *     odd_key         : pointer to the odd key buffer.
   *     even_key_length : length in byte of even key.
   *     even_key        : pointer to the even key buffer.
   *
   *   Description:
   *     This function sets the control keys of a stream to descramble.
   *
   *   Returns:   tTDAL_DESC_Error
   *     The returned value is eTDAL_DESC_NO_ERROR in case of success,
   *     eTDAL_DESC_ERROR in case of failure : if descrambler was not a valid open descrambler.
   *
   *==================================================================*/
tTDAL_DESC_Error   TDAL_DESC_Set_Descrambler_Keys(tTDAL_DESC_descrambler descId,
                   int16_t odd_key_length,
                   const int8_t *odd_key,
                   int16_t even_key_length,
                   const int8_t *even_key)
{
    uint8_t i;
    TCsdStatus csdStatus;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    TCsdDscKeyPathHandle DscKeyPathHandle;
    DSCMB_Type dscmbType;
    TCsdUnsignedInt16 xEmi;
    unsigned char defaultIV[16];

    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_NOT_DONE);
    }
    
    MsOS_ObtainSemaphore(TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /* Check if descrambler used */
    for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
    {
        if (TDAL_DESC_table[i].descId == descId)
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

    /* Set descrambler type */
    if (TDAL_DESC_table[i].emi == 0x0)          // EMI_NGA_MPEG_DVB_CSA2
    {
        dscmbType = E_DSCMB_TYPE_CSA;
    }
    else if (TDAL_DESC_table[i].emi == 0x1)     // EMI_NGA_MPEG_DVB_CSA3
    {
        dscmbType = E_DSCMB_TYPE_CSA3;
    }
    else if (TDAL_DESC_table[i].emi == 0x20)    // EMI_NGA_MPEG_AES128_IDSA
    {
        memset(defaultIV, 0x00, sizeof(defaultIV));
        MDrv_DSCMB_FltIVSet(descId, E_DSCMB_KEY_EVEN, defaultIV);
        MDrv_DSCMB_FltIVSet(descId, E_DSCMB_KEY_ODD, defaultIV);
        dscmbType = E_DSCMB_TYPE_AES;
    }
    else
    {
        mTBOX_TRACE(( kTBOX_NIV_1, "Unknown EMI\n"));
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }
    if(MDrv_DSCMB_FltTypeSet(descId, dscmbType) == FALSE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }

    /* Set Keys */
    MDrv_AESDMA_Lock();
    xEmi = TDAL_DESC_table[i].emi;
    DscKeyPathHandle.u32DscmbId = descId;
    csdStatus = csdSetClearTextDscHostKeys(xEmi, odd_key, odd_key_length, even_key, even_key_length, &DscKeyPathHandle);
    if(csdStatus != CSD_NO_ERROR)    
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"csdSetClearTextDscHostKeys %s\n", DBG_TCsdStatus(csdStatus)));
        MDrv_AESDMA_Unlock();
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }

    MDrv_AESDMA_Unlock();
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return eTDAL_DESC_NO_ERROR;
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
tTDAL_DESC_Error TDAL_DESC_Is_Descrambler_Open(tTDAL_DESC_descrambler descrambler)
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
    MsOS_ObtainSemaphore(TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /* Check if descrambler used */
    for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
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
    return eTDAL_DESC_NO_ERROR;
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
    uint8_t i;
    TCsdStatus csdStatus;
    TCsdDscCipheredProtectingKeys dsc2CipheredProtectingKeysTable;
    tTDAL_DESC_descrambler tempDescId = kTDAL_DESC_ILLEGAL_DESCRAMBLER; 
    TCsdDscKeyPathHandle DscKeyPathHandle;
    TCsdDscKeyPathHandle* pDscKeyPathHandle=NULL;
    DSCMB_Type dscmbType;
    TCsdUnsignedInt16 xEmi = 0;
    unsigned char defaultIV[16];
    const TCsdUnsignedInt8 L2ProtectingKey[] = {0xA9, 0x32, 0x30, 0x31, 0x31, 0x4E, 0x61, 0x67, 0x72, 0x61, 0x76, 0x69, 0x73, 0x69, 0x6F, 0x6E};
    
    if (TDAL_DESC_isInit == FALSE)
    {
        return(eTDAL_DESC_NOT_DONE);
    }
    
    MsOS_ObtainSemaphore(TDAL_DESC_table_lock, MSOS_WAIT_FOREVER);
    /* Check if descrambler used */
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

    /* Set descrambler type */
    if (TDAL_DESC_table[i].emi == 0x0)          // EMI_NGA_MPEG_DVB_CSA2
    {
        dscmbType = E_DSCMB_TYPE_CSA;
    }
    else if (TDAL_DESC_table[i].emi == 0x1)     // EMI_NGA_MPEG_DVB_CSA3
    {
        dscmbType = E_DSCMB_TYPE_CSA3;
    }
    else if (TDAL_DESC_table[i].emi == 0x20)    // EMI_NGA_MPEG_AES128_IDSA
    {
        memset(defaultIV, 0x00, sizeof(defaultIV));
        MDrv_DSCMB_FltIVSet(descrambler, E_DSCMB_KEY_EVEN, defaultIV);
        MDrv_DSCMB_FltIVSet(descrambler, E_DSCMB_KEY_ODD, defaultIV);
        dscmbType = E_DSCMB_TYPE_AES;
    }
    else
    {
        mTBOX_TRACE(( kTBOX_NIV_1, "Unknown EMI\n"));
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }
    if(MDrv_DSCMB_FltTypeSet(descrambler, dscmbType) == FALSE)
    {
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return eTDAL_DESC_ERROR;
    }

    memcpy(&dsc2CipheredProtectingKeysTable[0], L2ProtectingKey, sizeof(L2ProtectingKey));
    if((L1_key != NULL) && (L1_key_length != 0))
    {
        memcpy(&dsc2CipheredProtectingKeysTable[1], L1_key, L1_key_length);
    }
    else
    {
        memset(&dsc2CipheredProtectingKeysTable[1], 0x0, kTDAL_DESC_OFFSET_LENGTH);
    }
    if(TDAL_DESC_table[i].Pid!=0)
    {
        /* if channel is found */
        DscKeyPathHandle.u32DscmbId = descrambler;
        pDscKeyPathHandle = &DscKeyPathHandle;
    }

    /* Set Keys */
    MDrv_AESDMA_Lock();
    xEmi = TDAL_DESC_table[i].emi;
    csdStatus = csdSetProtectedDscContentKeys(xEmi, dsc2CipheredProtectingKeysTable, odd_key, odd_key_length, even_key, even_key_length, pDscKeyPathHandle);
    if(csdStatus != CSD_NO_ERROR)     
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"csdSetProtectedDscContentKeys %s\n", DBG_TCsdStatus(csdStatus)));
        MDrv_AESDMA_Unlock();
        MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
        return(eTDAL_DESC_ERROR);
    }

    MDrv_AESDMA_Unlock();
    MsOS_ReleaseSemaphore(TDAL_DESC_table_lock);
    return eTDAL_DESC_NO_ERROR;
}

/*===================================================================
   *
   *   TDAL_DESC_Set_Descrambler_Type
   *
   *   Parameters:
   *     descId : descrambling channel as returned by TDAL_DESC_Open_Descrambler.
   *     uint16_t : type of descrambler (EMI)
   *
   *   Description:
   *     This function set the descrambler type
   *
   *   Returns:   tTDAL_DESC_Error
   *     The returned value is eTDAL_DESC_NO_ERROR in case of success,
   *     eTDAL_DESC_ERROR in case of failure : if descrambler was not previously forbidden
   *     or there is not enough place for more PIDs.
   *
   *
   *==================================================================*/
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Type(tTDAL_DESC_descrambler descId, TDAL_DESC_DescType descrambler_type)
{
    TDAL_DESC_table[descId].emi = descrambler_type;
    return eTDAL_DESC_NO_ERROR;
}

/*===================================================================
   *
   *   TDAL_DESC_Stream_Authorisation
   *
   *   Parameters:
   *     pid : PID of transport stream packet to Forbid or re-Authorize.
   *     authorize : FALSE to inhib, TRUE to re-authorize.
   *
   *   Description:
   *     This function allows the control of the descrambler from oustide on selected PID.
   *
   *   Returns:   tTDAL_DESC_Error
   *     The returned value is eTDAL_DESC_NO_ERROR in case of success,
   *     eTDAL_DESC_ERROR in case of failure : if descrambler was not previously forbidden
   *     or there is not enough place for more PIDs.
   *
   *
   *==================================================================*/
tTDAL_DESC_Error TDAL_DESC_Stream_Authorisation(int16_t pid, bool authorize)
{
    return eTDAL_DESC_NO_ERROR;
}

/**========================================================================**
   *   Function     :   TDAL_DESC_APIRevisionGet
   *
   *   Description  :
   *
   *   Side effects :
   *
   *   Comment      :
   *
   **========================================================================**/
const char *TDAL_DESC_APIRevisionGet(void)
{
   static const char API_Revision[] = "TDAL_DESC_v1.0";

   return ((const char *)API_Revision);
}

/**========================================================================**
   *   Function     :   TDAL_DESC_PlatformRevisionGet
   *
   *   Description  :
   *
   *   Side effects :
   *
   *   Comment      :
   *
   **========================================================================**/
const char *TDAL_DESC_PlatformRevisionGet(void)
{
   static const char PLTF_Revision[] = "STFAEA16_7109";

   return ((const char *)PLTF_Revision);
}

/**========================================================================**
   *   Function     :   TDAL_DESC_PlatformRevisionGet
   *
   *   Description  :
   *
   *   Side effects :
   *
   *   Comment      :
   *
   **========================================================================**/
tTDAL_DESC_Error TDAL_DESC_GetCapability(tTDAL_DESC_Capability *pstCapability)
{
   return eTDAL_DESC_NO_ERROR;
}

/**========================================================================**
   *   Function     :   TDAL_DESC_EncryptFlashProtKey
   *
   *   Description  :   This function encrypts the global flash protection key with the chipset specific flash protection root key.
   *
   *   Side effects :
   *
   *   Comment      :
   *
   **========================================================================**/
tTDAL_DESC_Error TDAL_DESC_EncryptFlashProtKey(const uint8_t* pxInput, uint8_t* pxOutput, size_t xSize)
{
    static unsigned char FlashProtectedKey[48] =
    {
        0xEC, 0xD4, 0x37, 0x79, 0x82, 0x67, 0x4C, 0xE5, 0x09, 0xB7, 0xED, 0x00, 0xB3, 0x24, 0x5C, 0xDC,
        0x8D, 0xB1, 0x70, 0x04, 0x1B, 0x1F, 0x43, 0x2A, 0xE1, 0x96, 0x08, 0x24, 0xDA, 0xC6, 0x73, 0xB4,
        0xDE, 0xDA, 0x18, 0xF4, 0x02, 0x91, 0x57, 0x3C, 0x4E, 0xC4, 0x5F, 0x0C, 0x2B, 0x19, 0xE5, 0xE9,
    };

    if (pxInput == NULL || pxOutput == NULL || xSize != 16)
    {
        return eTDAL_DESC_ERROR;
    }

    MS_U8 *pu8Buf = MsOS_AllocateMemory(xSize*2, gs32NonCachedPoolID);
    MS_U8 *pu8DataIn = pu8Buf;
    MS_U8 *pu8DataOut = pu8Buf + xSize;
    MS_U32 u32CipherId = 0;
    DSCMB_KLDst eKLDst = 0;
    MS_U8 ACPU_Out[16] = {0x00};
    DSCMB_KL_Status KL_Status = 0;
    DRV_CIPHER_DMACFG stCfg;
    MS_U32 u32CmdId = 0;
    MS_U32 u32Exception = 0;
    tTDAL_DESC_Error e = eTDAL_DESC_NO_ERROR;

    if (NULL == pu8Buf)
    {
        return eTDAL_DESC_ERROR;
    }
    if (TRUE != MDrv_NGA_R2RAlloc(&u32CipherId))
    {
        MsOS_FreeMemory(pu8Buf, gs32NonCachedPoolID);
        return eTDAL_DESC_ERROR;
    }

    // Keyladder Config
    MDrv_NGA_R2R_CipherId2KLDst(u32CipherId, &eKLDst);

    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_TDES,
        .eSrc = E_DSCMB_KL_SRC_SECRET_6,
        .eDst = eKLDst,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = 0,
        .u32Level = 3/*SEC_KEY_LADDER_LEVEL*/,
        .u32EngID = 0,
        .u32DscID = 0,
        .u8KeyACPU = 0,
        .pu8KeyKLIn = FlashProtectedKey,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .eKLSel = NOCS_NAGRA_KEYLADDER,
        .u32CAVid = NARGA_CAVID,
    };

    if (FALSE == MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll, ACPU_Out, &KL_Status))
    {
        e = eTDAL_DESC_ERROR;
        goto SEC_EncryptFlashProtKey_DONE;
    }

    // Data Copy
    memset(pu8Buf, 0x00, xSize*2);
    memcpy(pu8DataIn, pxInput, xSize);

    // DMA config
    memset(&stCfg, 0, sizeof(DRV_CIPHER_DMACFG));
    stCfg.u32CAVid = NARGA_CAVID;

    // AES ECB
    stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
    stCfg.stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
    stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
    stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;

    stCfg.stKey.eKeySrc = E_CIPHER_KSRC_KL;
    stCfg.stKey.u8KeyIdx = u32CipherId;
    stCfg.stKey.u8KeyLen = 0;
    stCfg.stKey.pu8KeyData = NULL;

    stCfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8DataIn);
    stCfg.stInput.u32Size = xSize;

    stCfg.stOutput.u32Addr = MsOS_VA2PA((MS_U32)pu8DataOut);
    stCfg.stOutput.u32Size = xSize;

    if (DRV_CIPHER_OK != MDrv_CIPHER_DMAConfigure(u32CipherId, stCfg))
    {
        e = eTDAL_DESC_ERROR;
        goto SEC_EncryptFlashProtKey_DONE;
    }

    MDrv_NGA_FlushMemory();

    if (DRV_CIPHER_OK != MDrv_CIPHER_DMAStart(u32CipherId, &u32CmdId))
    {
        e = eTDAL_DESC_ERROR;
        goto SEC_EncryptFlashProtKey_DONE;
    }

    while (FALSE == MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception))
    {
        MsOS_DelayTask(1);
    }

    MDrv_NGA_FlushMemory();

    // Result Copy
    memcpy(pxOutput, pu8DataOut, xSize);

SEC_EncryptFlashProtKey_DONE:
    MsOS_FreeMemory(pu8Buf, gs32NonCachedPoolID);
    if (FALSE == MDrv_NGA_R2RFree(u32CipherId))
    {
        return eTDAL_DESC_ERROR;
    }

    if (eTDAL_DESC_NO_ERROR != e)
    {
        return e;
    }

    return eTDAL_DESC_NO_ERROR;
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
tTDAL_DESC_Error TDAL_DESCi_EnableSecureMode(void)
{
    return eTDAL_DESC_NO_ERROR;
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
tTDAL_DESC_Error TDAL_DESCi_DisableSecureMode(void)
{
     return eTDAL_DESC_NO_ERROR;
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
    if (TDAL_DESC_isInit == TRUE)
    {
        MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
        TDAL_DESC_channels[ channel ].ChannelPid = pid;
        MsOS_ReleaseSemaphore(TDAL_DESC_table_sync);
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
void TDAL_DESC_DemuxChannelCallback(tTDAL_DMX_ChannelId const channel, tTDAL_DMX_ChannelStream const channel_stream)
{
    /* Check layer status, and mirror the channel information */
    if (TDAL_DESC_isInit == TRUE)
    {
        MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
        TDAL_DESC_channels[ channel ].ChannelStream = channel_stream;
        MsOS_ReleaseSemaphore(TDAL_DESC_table_sync);
    }
}

/*===================================================================
   *
   *   TDAL_DESC_Set_Descrambler_Keys_Cert
   *
   *   Parameters:
   *   descId    : descrambling channel as returned by TDAL_DESC_Open_Descrambler.
   *   keyIdSize : length in byte of the odd key.
   *   KeyId     : pointer to the keyId buffer.
   *
   *   Description:
   *   This function sets the control keys of a stream to descramble.
   *
   *   Returns:   tTDAL_DESC_Error
   *   The returned value is eTDAL_DESC_NO_ERROR in case of success,
   *   eTDAL_DESC_ERROR in case of failure : if descrambler was not a valid open descrambler.
   *
   *==================================================================*/
tTDAL_DESC_Error TDAL_DESC_Set_Descrambler_Keys_Cert(tTDAL_DESC_descrambler descId, int16_t keyIdSize, const int8_t *KeyId)
{
    AKL_Key_Type eAKLKeyType = E_AKL_KEY_EVEN;
    DSCMB_Key_Type eKeyType = 0;
    DSCMB_Eng_Type eEngType = 0;
    MS_BOOL bDecrypt = FALSE;
    MS_U8 eng;
    MS_U32 filtId = 0;
    MS_U8 defaultIV[16];
    MS_BOOL bRet = TRUE;
    int i;

    for (i = 0; i < DSCMB_PIDSLOT_COUNT; i++)
    {
        if (TRUE == MDrv_DSCMB2_GetConnectStatus(0, descId, i))
        {
            filtId = i;
            break;
        }
    }
    if (i >= DSCMB_PIDSLOT_COUNT)
    {
        return eTDAL_DESC_ERROR_UNKNOW_ID;
    }

    if (KeyId == NULL) /* single key */
    {
        eAKLKeyType = E_AKL_KEY_EVEN;
        eKeyType = E_DSCMB_KEY_EVEN;
    }
    else /* pair key */
    {
        if (0 == *KeyId)
        {
            eAKLKeyType = E_AKL_KEY_EVEN;
            eKeyType = E_DSCMB_KEY_EVEN;
        }
        else if (1 == *KeyId)
        {
            eAKLKeyType = E_AKL_KEY_ODD;
            eKeyType = E_DSCMB_KEY_ODD;
        }
        else
        {
            return eTDAL_DESC_ERROR;
        }
    }

    eng = E_AKL_ENG_ESA;
    eEngType = E_DSCMB_ENG_ESA;
    bDecrypt = TRUE;

    //Init CERT AKL driver
    MDrv_AKL_Init();

    // set cert key as manual mode
    MDrv_AKL_SetManualACKMode(TRUE);

    //set CERT key to KT
    bRet = MDrv_AKL_SetKey2KT(eng, filtId, eAKLKeyType);
    if (TRUE != bRet)
    {
        return eTDAL_DESC_ERROR;
    }

    //enable the key
    bRet = MDrv_DSCMB2_EngEnableKey(0, descId, eEngType, eKeyType, TRUE);
    if (TRUE != bRet)
    {
        return eTDAL_DESC_ERROR;
    }

    bRet = MDrv_NGA_Payload_SetAlgo(TDAL_DESC_table[descId].emi, descId, eEngType, bDecrypt);
    if (TRUE != bRet)
    {
        return eTDAL_DESC_ERROR;
    }

    //Only ESA engine can encrypt/decrypt CERT algorithm ASA
    MDrv_DSCMB2_EngSetSwitch(0, descId, 0, eEngType);

    MDrv_DSCMB2_EngSetFSCB(0, descId, DSCMB_FSCB_CLEAR);

    if (TDAL_DESC_table[descId].emi == 0x20)    // EMI_NGA_MPEG_AES128_IDSA
    {
        memset(defaultIV, 0x00, sizeof(defaultIV));
        MDrv_DSCMB2_EngSetIV(0, descId, E_DSCMB_KEY_CLEAR, (MS_U8 *)defaultIV);
        MDrv_DSCMB2_EngSetIV(0, descId, E_DSCMB_KEY_EVEN, (MS_U8 *)defaultIV);
        MDrv_DSCMB2_EngSetIV(0, descId, E_DSCMB_KEY_ODD, (MS_U8 *)defaultIV);
    }

    return eTDAL_DESC_NO_ERROR;
}

#if defined(USE_TDAL_PVR)
uint32_t TDAL_DESC_GetDescramblerID( uint16_t const pid)
{
	uint32_t retDescId = 0xFFFFFFFF;
	uint8_t i = 0;
	if (TDAL_DESC_isInit == TRUE)
    {
        MsOS_ObtainSemaphore(TDAL_DESC_table_sync, MSOS_WAIT_FOREVER);
        for (i=0; i < kTDAL_DESC_MAX_DESCRAMBLERS; i++)
        {
            if(pid == TDAL_DESC_table[i].Pid)
            {
                retDescId = TDAL_DESC_table[i].descId;
                break;
            }
        }
        MsOS_ReleaseSemaphore( TDAL_DESC_table_sync );
    }
    return retDescId;
}
#endif
