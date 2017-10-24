/** \file hdmi_test_helper.c
 *  \brief Helper functions to write tests
 *  for the HDMI layer of the TDAL API.
 *  \author Pascal PIGNON
 *  \date 06/05/06
 *  \version 1.0
 */

#ifdef TDAL_HDMI_USE

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "crules.h"
#include <string.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_hdmi.h"
/*#include "tdal_common.h"*/

#include "testhelper.h"
#include "hdmitests.h"

/******************************************************
 *              Defines                     *
 ******************************************************/

mTBOX_SET_MODULE(eTDAL_TEST_HDMI);


/******************************************************
 *              Globale variables
 *
 ******************************************************/
extern bool bWaitHdmiEvent;

/******************************************************
 *              Local variables
 *
 ******************************************************/


/******************************************************
 *          test helper functions                   *
 ******************************************************/

/*! \fn void DummyCallback( tTDAL_HDMI_SourceId sourceId, tTDAL_HDMI_Event Event )
 *  \brief Test initialization and termination of HDMI driver
 */

void DummyCallback(tTDAL_HDMI_SourceId sourceId, tTDAL_HDMI_Event Event )
{
    char *EventCallbackName[]= {"eTDAL_HDMI_EVENT_NO_RECEIVER",
                                "eTDAL_HDMI_EVENT_RECEIVER_CONNECTED",
                                "eTDAL_HDMI_EVENT_RECEIVER_NOT_HDCP",
                                "eTDAL_HDMI_EVENT_AUTHENTICATION_IN_PROGRESS",
                                "eTDAL_HDMI_EVENT_AUTHENTICATION_SUCCEED",
                                "eTDAL_HDMI_EVENT_AUTHENTICATION_FAILED",
                                "eTDAL_HDMI_EVENT_NO_ENCRYPTION"};

    mTBOX_FCT_ENTER("DummyCallback(...)");

    mTBOX_PRINT( ( kTBOX_LF, "\n->HDMI callback(SourceId:%d, Event:%s)\n",sourceId, EventCallbackName[Event] ) );
    mTBOX_PRINT( ( kTBOX_LF, "\n" ) );

    if( (Event == eTDAL_HDMI_EVENT_RECEIVER_CONNECTED) ||
        (Event == eTDAL_HDMI_EVENT_NO_ENCRYPTION) ||
        (Event == eTDAL_HDMI_EVENT_NO_RECEIVER) )
    {
        if(bWaitHdmiEvent)
        {
            TKEL_SignalSemaphore(TDAL_HDMI_TST_NotifySem);
        }
    }
}

void TestHelper_HDMI_Term( void )
{
    if(TDAL_HDMI_TST_NotifySem != NULL)
    {
        TKEL_DeleteSemaphore(TDAL_HDMI_TST_NotifySem);
        TDAL_HDMI_TST_NotifySem = NULL;
    }
    /* termintate HDMI */
    TestManager_AssertEqual( TDAL_HDMI_Term(), eTDAL_HDMI_NO_ERROR, "HDMI terminate" );
}

void TestHelper_HDMI_Init( void )
{
    TestManager_AssertEqual( TDAL_HDMI_Init(), eTDAL_HDMI_NO_ERROR, "HDMI first init" );

    /* create semaphore for callback */
    if(TDAL_HDMI_TST_NotifySem == NULL)
    {
        TKEL_CreateSemaphore(0, &TDAL_HDMI_TST_NotifySem);
    }
}


#endif /* TDAL_HDMI_USE */
