/**
** @file ca_psi.h
**
** @brief
**   This file defines the API that can be used by the set-top software to
**   notify the Nagra CA software of channel changes and to descramble MPEG
**   streams.
**
** Switches:
**   NAGRA_CA_V5, NAGRA_CA_V6, PSI_6_CAK_5, PSI_5_CAK_6
**
**   NAGRA_CA_V[56] defines the version of the CAK. Default to V6 CAK.
**
**   When NAGRA_CA_V5 is defined, PSI_6_CAK_5 defines that the underlying PSI
**   driver complies to the V6 definitions. In this case, the PSI D6C5 glue
**   must be compiled.
**
**   When NAGRA_CA_V6 is defined, PSI_5_CAK_6 defines that the underlying PSI
**   driver complies to the V5 definitions. In this case, the PSI D5C6 glue
**   must be compiled.
**
** Summary:
**
**   NAGRA_CA_V5 NAGRA_CA_V6 PSI_x_CAK_y result
**   ----------- ----------- ----------- ------
**   not set     not set     not set     CAK V6 with PSI V6 (default)
**   not set     not set     set         conflict           (for any x or y)
**   not set     set         not set     CAK V6 with PSI V6 (standard new)
**   not set     set         set         CAK V6 with PSI V5 (x=5, y=6, glue D5C6)
**   set         not set     not set     CAK V5 with PSI V5 (standard old)
**   set         not set     set         CAK V5 with PSI V6 (x=6, y=5, glue D6C5)
**   set         set         not set     conflict
**   set         set         set         conflict           (for any x or y)
**
** @ingroup PSI
**
** @author B. Krummenacher
**
** @version $Revision: 1.36 $
**
** COPYRIGHT:
**   © 1998-2002 Nagravision S.A.
**
** HISTORY:
**   $Log: ca_psi.h,v $
**   Revision 1.36  2008/07/04 17:11:36  bieber
**   PRMS ID: NONE
**   - Updated to match version 3.1 of the API document
**
**   Revision 1.35  2007/06/18 09:00:43  bussy
**   PRMS ID: NONE
**   - Added a note on the EMM callback about the multi-tuners case.
**
**   Revision 1.34  2007/03/27 09:35:42  aceriali
**   PRMS ID: NONE
**   - Removed definition of TRANSPORT_SESSION_ID_INVALID, now defined in to
**     ca_defsx.h
**
**   Revision 1.33  2006/11/08 13:36:44  hacheman
**   PRMS ID: NONE
**   - Removed useless switch PSI_NO_TYPES.
**
**   Revision 1.32  2006/11/07 15:44:12  hacheman
**   PRMS ID: NONE
**   - Managed switches PSI_WRAP and PSI_NO_TYPES.
**
**   Revision 1.31  2004/05/28 14:33:26  robyr
**   PRMS ID: NONE
**   - Removed definition of TTransportStreamId and consorts
**   - Includes now ca_defsx.h instead.
**
**   Revision 1.30  2004/05/04 07:39:45  robyr
**   PRMS ID: NONE
**   - TServiceId has been renamed as TDbvServiceId and moved from ca_mpeg.h to
**     ca_dvb.h
**
**   Revision 1.29  2002/12/17 16:19:27  hacheman
**   PRMS ID: NONE
**   - Adapted to glue modifications (new header file psihd6c5.h).
**
**   Revision 1.28  2002/12/12 15:17:05  hacheman
**   PRMS ID: NONE
**   - Added optional inclusions for glue D5C6.
**
**   Revision 1.27  2002/11/19 11:24:31  hacheman
**   PRMS ID: NONE
**   - Added switch for glue D5C6.
**
**   Revision 1.26  2002/09/13 09:29:26  schyrr
**   PRMS ID: NONE
**   - Update comments to comply to PSI API 3.0.11.
**
**   Revision 1.25  2002/06/03 14:56:10  erard
**   PRMS ID: NONE
**   - Imporved comment about switches.
**
**   Revision 1.24  2002/06/03 13:24:31  erard
**   PRMS ID: NONE
**   - Don't use NAGRA_CA_V6 but NAGRA_CA_V5 to distinguish between a V5 and a
**     V6 implementation.
**   - Added comment about the swicthes in the header.
**
**   Revision 1.23  2002/05/16 14:02:00  hacheman
**   PRMS ID: NONE
**   - Modified the ifdef structure (using in particular NAGRA_CA_V6).
**     Replaced DRV_6_CAK_5 by PSI_6_CAK_5.
**
**   Revision 1.22  2002/05/08 10:08:56  hacheman
**   PRMS ID: NONE
**   - Removed the definition of CA_MPEG_H. It was not the right solution. Sorry
**     for this double commit. I will not do it again.
**
**   Revision 1.21  2002/05/07 15:38:06  hacheman
**   PRMS ID: NONE
**   - Added definition of CA_MPEG_H to prevent the inclusion of this V6 file
**     when the glue DRV_6_CAK_5 is defined.
**
**   Revision 1.20  2002/03/15 16:43:00  schyrr
**   PRMS ID: NONE
**   - Prevent definitions of ca_cak.h-intended definitions in case CA_CAK_h is
**     defined.
**
**   Revision 1.19  2002/02/28 13:54:23  erard
**   PRMS ID: NONE
**   - Corrected parameters name in order to be more consistent.
**
**   Revision 1.18  2002/02/27 17:34:15  erard
**   PRMS ID: NONE
**   - Moved TTransportSessionId in TElemStream.
**   - Use TElemStream in psiReportElemStreamStatus.
**
**   Revision 1.17  2002/01/28 14:33:50  schyrr
**   PRMS ID: NONE
**   - Replace TPsiControlWordCallback by the API function
**     psiReportElemStreamStatus.
**   - Control words are no longer given to the PSI.
**   - Rename TPsiEcmStreamCallback into TPsiElemStreamCallback.
**   - Rename PSI_ECM_STREAM_ADD into PSI_ELEM_STREAM_ADD, PSI_ECM_STREAM_REMOVE
**     into PSI_ELEM_STREAM_REMOVE and PSI_ECM_STREAM_UPDATE into
**     PSI_ELEM_STREAM_UPDATE.
**   - Remove TPsiCwFlags and TPsiCwData and add TPsiElemStreamStatus.
**   - The callback TPsiEmmPidCallback now has no effect on EMM filtering if
**     an invalid PID value is given.
**
**   Revision 1.16  2002/01/24 14:32:55  schyrr
**   PRMS ID: NONE
**   - Replace TpsiControlWordCallback by the API function psiSetControlWord.
**
**   Revision 1.15  2002/01/22 16:50:51  hacheman
**   PRMS ID: NONE
**   - Added pre-processor constant DRV_6_CAK_5. If set, the file psigd6c5.h is
**     included instead of this file content.
**
**   Revision 1.14  2002/01/07 13:54:15  robyr
**   PRMS ID: NONE
**   Added the compilation switch DRV_6_CAK_5 used by the glue in case this
**   driver is used with a CAK V5.
**
**   Revision 1.13  2001/12/11 07:55:19  schyrr
**   PRMS ID: NONE
**   - Add PSI_REGISTRATION_ID_INVALID and TRANSPORT_SESSION_ID_INVALID.
**   - Add ca_cak exportation comments.
**
**   Revision 1.12  2001/11/27 13:38:06  schyrr
**   PRMS ID: NONE
**   - Correct syntax errors.
**
**   Revision 1.11  2001/11/26 14:04:18  schyrr
**   PRMS ID: NONE
**   - Add const qualifier in TPsiEcmStreamCallback, psiEmmRegister and
**     psiEcmRegister.
**
**   Revision 1.10  2001/11/26 11:28:26  schyrr
**   PRMS ID: NONE
**   - Adapt file to latest PSI API document.
**
**   Revision 1.9  2001/11/22 15:14:38  robyr
**   PRMS ID: NONE
**   Removed type definition of TServiceId and added it into ca_mpeg.h file.
**
**   Revision 1.8  2001/11/20 14:14:34  robyr
**   PRMS ID: NONE
**   Removed 'TMpegStreamType' and 'TPid' definitions from file, and added them
**   to new file 'ca_mpeg.h'.
**
**   Revision 1.7  2001/11/01 13:27:20  schyrr
**   PRMS ID: NONE
**   - Rename TPsiDescrambleCallback into TPsiControlWordCallback to match the
**     documentation.
**   - Adapt comment for Doxygen documentation generation.
**
**   Revision 1.6  2001/10/12 15:45:35  krummen
**   PRMS ID: NONE
**   Fixed a few errors.
**
**   Revision 1.5  2000/09/19 10:06:20  krummen
**   PRMS ID: NONE
**   Complete rework to be complient with PSI API 3.0.0 specification.
**
**   Revision 1.4  1999/06/01 10:24:57  krummen
**   Updated comment about control word flags in function type TPsiDescrambleCallback.
**
**   Revision 1.3  1999/06/01 09:35:37  krummen
**   Added definitions of control word flags.
**
**   Revision 1.2  1999/01/05 11:44:37  krummen
**   Added definition of TPsiCwData and TPsiCwFlags types.
**
**   Revision 1.1  1998/12/18 16:18:50  krummen
**   New archive for the CA software.
**
*/
/**
 *  @defgroup PSI Program Specific Information
 *  @brief
 *    Program Specific Information component
 *
 *    This component can be used by the set-top box software to notify the
 *    NagraVision CA software of channel addition and removal and to
 *    descramble MPEG streams.
*/

#ifndef CA_PSI_H
#define CA_PSI_H

/******************************************************************************/
/*                                                                            */
/*                               INCLUDE FILES                                */
/*                                                                            */
/******************************************************************************/
#ifdef PSI_WRAP
#include "psiwrap.h"
#endif

#include "ca_defs.h"
#include "ca_defsx.h"

#ifdef NAGRA_CA_V5
/* CAK V5 */

#ifdef PSI_6_CAK_5
#include "psihd6c5.h"
#endif /* PSI_6_CAK_5 */
#include "ca_psi_o.h"

#else /* NAGRA_CA_V5 */
/* CAK V6 */
#ifdef PSI_5_CAK_6
#include "psihd5c6.h"
#endif /* PSI_5_CAK_6 */

#include "ca_mpeg.h"
#include "ca_dvb.h"

/******************************************************************************/
/*                                                                            */
/*                              TYPES DEFINITIONS                             */
/*                                                                            */
/******************************************************************************/
/**
 *  @brief
 *    Gives the access status of an elementary stream.
 *
*/
typedef enum
{
  PSI_ACCESS_GRANTED,
    /**<  The access to the elementary stream is granted.
    */
  PSI_ACCESS_DENIED
    /**<  The access to the elementary stream is denied.
    */
} TPsiElemStreamStatus;


/**
 *  @brief
 *    Command related to the stream event.
 *
*/
typedef enum
{
  PSI_ELEM_STREAM_ADD,
    /**<  Notify the NagraVision CA that an elementary stream has been added.
     *    Each added stream must later be removed using
     *    PSI_ELEM_STREAM_REMOVE
    */
  PSI_ELEM_STREAM_REMOVE,
    /**<  Notify the NagraVision CA that an elementary stream has been removed.
    */
  PSI_ELEM_STREAM_UPDATE
    /**<  Notify the NagraVision CA that the data related to an elementary
          stream has changed (ECM PID, stream type).
    */
} TPsiElemStreamCommand;


/**
 *  @brief
 *    PSI specific error codes.
 *
*/
typedef enum
{
  PSI_NO_ERROR,
    /**<  Success.
    */
  PSI_ERROR_BAD_PID,
    /**<  A PID with an illegal value was given.
    */
  PSI_ERROR_ELEMENTARY_STREAM,
    /**<  The elementary stream is already descrambled
          with another NagraVision ECM PID.
    */
  PSI_ERROR_NO_MORE_RESOURCES,
    /**<  Filter or descrambler resources are fully used or the number
          of ECM streams has reached its limit. No more filtering or
          descrambling is possible.
    */
  PSI_ERROR
    /**<  Unspecified error.
    */
} TPsiStatus;


/**
 *  @brief
 *    This section describes the structure of an item of the stream table.
*/
typedef struct
{
  TTransportSessionId transportSessionId;
    /**<  Transport session identificator selecting which transport session the
          streams comes from. It will be given down as is to the demultiplexer
          and descrambler drivers.
    */
  TDvbServiceId       serviceId;
    /**<  DVB Service ID.
    */
  TPid                elementaryStreamPid;
    /**<  Elementary stream's PID.
    */
  TPid                ecmStreamPid;
    /**<  NagraVision ECM stream PID linked to the above stream.
    */
  TMpegStreamType     mpegStreamType;
    /**<  MPEG stream type.
    */
} TPsiElemStream;


/**
 *  @brief
 *    This type is used to identify a registered item.
 *
 *  @see  PSI_REGISTRATION_ID_INVALID
*/
typedef TUnsignedInt32 TPsiRegistrationId;

/**
 *  @brief
 *    Defines an invalid PSI registration identifier.
 *
 *  @see  TPsiRegistrationId
*/
#define PSI_REGISTRATION_ID_INVALID ((TPsiRegistrationId) -1)


/******************************************************************************/
/*                                                                            */
/*                          CALLBACK FUNCTION TYPEDEFS                        */
/*                                                                            */
/******************************************************************************/


/**
 *  @brief
 *    This callback is used to notify the NagraVision CA software of changes
 *    occurred at the elementary stream level.
 *
 *    Clear (unscrambled) streams must not be notified to the Nagravision CA.
 *    The PSI must not call this callback any more after the NagraVision CA
 *    has cancelled the registration. Any active streams are implicitly removed.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param  pxRegistrationId
 *            Identifier of the related registration.
 *  @param  xStreamCommand
 *            Stream related event. Can be one of: <br>
 *            o ::PSI_ELEM_STREAM_ADD     Notify the NagraVision CA that an
 *                                         elementary stream has been added.
 *                                         <br>
 *            o ::PSI_ELEM_STREAM_REMOVE   Notify the NagraVision CA that an
 *                                         elementary stream has been
 *                                         removed.<br>
 *            o ::PSI_ELEM_STREAM_UPDATE   Notify the NagraVision CA that the
 *                                         data related to an elementary stream
 *                                         has changed (ECM PID, stream type,
 *                                         clear flag). <br>
 *  @param  xNumStreams
 *            Number of items contained in the table.
 *  @param  pxElemStreams
 *            Pointer to the elementary stream table.
 *  @param  pxPsiPrivateData
 *           Set-top box private data linked to the elementary stream given
 *            in input. This data will be given back to the set-top software
 *            when the NagraVision CA software calls
 *            ::psiReportElemStreamStatus for
 *            that particular elementary stream. The NagraVision CA software
 *            doesn't make any processing on this private data.
 *
 *  @retval PSI_NO_ERROR
 *            The callback terminated successfully; as data is processed
 *            asynchronously, it does not mean that the processing itself was
 *            successful.
 *  @retval PSI_ERROR_BAD_PID
 *            A PID with an illegal value was given.
 *  @retval PSI_ERROR
 *            Other error
 *
 *  @see psiReportElemStreamStatus
 *
 *  @remarks
 *    -#  The addition of a stream already present (i.e. using the command
 *        PSI_ELEM_STREAM_ADD with transport session ID and elementary stream
 *        PID identical to the ones of an already added stream) is handled as a
 *        stream update and thus does not generate an error. There must be a
 *        PSI_ELEM_STREAM_ADD command for each PSI_ELEM_STREAM_REMOVE command.
 *    -#  The update of a stream not present (i.e. using the command
 *        PSI_ELEM_STREAM_UPDATE with transport session ID and elementary
 *        stream PID not corresponding to a present stream) is handled as a
 *        stream addition. The stream must ultimately be removed using the
 *        PSI_ELEM_STREAM_REMOVE command and thus does not generate an error. 

*/
typedef TPsiStatus (*TPsiElemStreamCallback)
(
        TPsiRegistrationId      xRegistrationId,
        TPsiElemStreamCommand   xStreamCommand,
        TUnsignedInt8           xNumStreams,
  const TPsiElemStream*         pxElemStreams,
  const void*                   pxPsiPrivateData
);


/**
 *  @brief
 *    This callback function notifies the NagraVision CA software of the
 *    NagraVision EMMs PID.
 *
 *    It must be called upon a registration with psiEmmRegister() and on every
 *    EMM PID changes.
 *    The special value PID_NULL is used to stop EMM filtering. The PSI must
 *    make this particular notification before tuning to another transport
 *    stream to avoid filtering non-EMM sections.
 *    The PSI must not call this callback any more after the NagraVision CA has
 *    cancelled the registration.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param   pxRegistrationId
 *             Identifier of the related registration.
 *  @param   xTransportSessionId
 *             Transport session identificator selecting which transport stream
 *             the streams comes from. It is given down as is to the
 *             demultiplexer driver
 *  @param   xEmmPid
 *             Emm PID as defined in ISO/IEC 13818-1.
 *
 *  @retval  PSI_NO_ERROR
 *             Success
 *  @retval  PSI_ERROR_BAD_PID
 *             A PID with an illegal value was given.
 *  @retval  PSI_ERROR
 *             Other error
 *
 *  @remarks
 *    -#  If PID_NULL is provided by the callback, the NagraVision CA will stop
 *        EMM filtering.
 *    -#  If an invalid PID is provided by the callback, the EMM filtering will
 *        continue with the current PID.
 *    -#  The CAK handles one single EMM transport session for each call to 
 *        psiEmmRegister. As a result, if several transport sessions are 
 *        matching the registered CA system ID, the PSI shall only notify one
 *        EMM PID per EMM registration, no matter on which matching transport 
 *        session. If the transport session previously notified is closed, the
 *        PSI shall notify the EMM PID of another active and matching tansport
 *        session, if any.
*/
typedef TPsiStatus (*TPsiEmmPidCallback)
(
  TPsiRegistrationId    xRegistrationId,
  TTransportSessionId	  xTransportSessionId,
  TPid                  xEmmPid
);

/******************************************************************************/
/*                                                                            */
/*                             FUNCTION PROTOTYPES                            */
/*                                                                            */
/******************************************************************************/

/**
 *  @brief
 *    Registers the callbacks to transmit PSI-ECM information to a
 *    NagraVision CA software and gives multiple CA system IDs on which the PMT
 *    shall be parsed.
 *
 *    If an elementary stream has more than one CA descriptors matching the
 *    registered CA system IDs, the notifications are made with the PID
 *    corresponding to the first adequate CA system ID in the registration
 *    table.
 *    This registration may be cancelled by a call to the
 *    psiCancelRegistration() function with the appropriated registration ID.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param   xNumCaSystemIds
 *             Number of CA system IDs in the table.
 *  @param   pxCaSystemId
 *             Table of CA system IDs as in ISO/IEC 13818-1.
 *  @param   xEcmStreamCallback
 *             Callback to notify the DRT of an elementary stream related.
 *  @param   pxRegistrationId
 *             Identifier for this specific registration. It is present in the
 *             notification calls and also required to cancel the registration.
 *
 *  @retval PSI_NO_ERROR
 *            Success.
 *  @retval PSI_ERROR
 *            The registration failed.
 *
 *  @see
 *    psiCancelRegistration
 *
 *  @remarks
 *    In the psiEcmRegister implementation, the stream callback function
 *    must be called to add all the scrambled elementary streams of the
 *    currently active services
*/
TPsiStatus psiEcmRegister
(
        TUnsignedInt8           xNumCaSystemIds,
  const TCaSystemId*            pxCaSystemId,
        TPsiElemStreamCallback  xEcmStreamCallback,
        TPsiRegistrationId*     pxRegistrationId
);


/**
 *  @brief
 *    This function is called to notify the PSI of the access status linked to
 *    an elementary stream, every time the access status linked to an ECM
 *    received on the given ECM PID can be computed.
 *
 *    The actual parameters of the function reflect the data given by the PSI
 *    in the corresponding elemStreamCallback that lead to capturing the ECM.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param  pxElemStream
 *            Elementary stream description, as provided in the elementary
 *            stream callback.
 *  @param  xElemStreamStatus
 *            Status of the elementary stream; can be one of
 *            o ::PSI_ACCESS_GRANTED
 *            o ::PSI_ACCESS_DENIED
 *  @param  pxPsiPrivateData
 *            Pointer to the PSI data received when registering the elementary
 *            stream.
 *
 *  @retval PSI_NO_ERROR
 *            Success.
 *  @retval PSI_ERROR
 *            Function called with invalid parameters..
 *
 *  @see TPsiElemStreamCallback
*/
TPsiStatus psiReportElemStreamStatus
(
  const TPsiElemStream*      pxElemStream,
        TPsiElemStreamStatus xElemStreamStatus,
  const void*                pxPsiPrivateData
);


/**
 *  @brief
 *    Registers the NagraVision CA software callback to the PSI-EMM and gives
 *    multiple CA system ID on which the CAT table [6] shall be filtered.
 *
 *    If the CAT has CA descriptors for more than one of the registered
 *    CA system IDs, the notification is made with the PID corresponding to the
 *    first adequate CA system ID in the registration table.
 *    This registration may be cancelled by a call to the
 *    psiCancelRegistration() function with the appropriated registration ID.
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param   xNumCaSystemIds
 *             Number of CA system IDs in the table.
 *  @param   pxCaSystemId
 *             Table of CA system IDs as in ISO/IEC 13818-1.
 *  @param   xEmmPidCallback
 *             Callback to notify the NagraVision CA software of an EMM
 *             related event.
 *  @param   pxRegistrationId
 *             Identifier for this specific registration. It is present in the
 *             notification calls and also required to cancel the registration.
 *
 *  @retval  PSI_NO_ERROR
 *             Success.
 *  @retval  PSI_ERROR
 *             The registration failed.
 *
 *  @see
 *    psiCancelRegistration
 *
*/
TPsiStatus psiEmmRegister
(
        TUnsignedInt8       xNumCaSystemIds,
  const TCaSystemId*        pxCaSystemId,
        TPsiEmmPidCallback  xEmmPidCallback,
        TPsiRegistrationId* pxRegistrationId
);


/**
 *  @brief
 *    This function is responsible of the removing of a specific registration
 *    data from the PSI notification lists.
 *
 *
 *  @pre
 *    None.
 *
 *  @post
 *    None.
 *
 *  @param   xRegistrationId
 *             Identifier of the registration to cancel.
 *             It is provided by the registration function.
 *
 *  @retval  PSI_NO_ERROR
 *             Success.
 *  @retval  PSI_ERROR
 *             The registration cancelation failed.
 *
*/
TPsiStatus psiCancelRegistration
(
  TPsiRegistrationId xRegistrationId
);

#endif /* NAGRA_CA_V5 */

#endif /* CA_PSI_H */

/* ca_psi.h */
