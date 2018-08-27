//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_SNMP.c
/// @author MStar Semiconductor Inc.
/// @brief  Uranus Sample Code Template
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#if (DEMO_SNMP_TEST == 1)

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>
#include <stdlib.h>
#include "MsCommon.h"


//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define OTHER_METHOD 1
#define OID_TO_GET1 ".1.3.6.1.2.1.1.3.0"
#define OID_TO_GET2 ".1.3.6.1.2.1.10.127.1.1.1.1.2.3"
#define OID_TO_GET3 ".1.3.6.1.2.1.1.1.0"
#define OID_TO_GET4 ".1.3.6.1.2.1.1.3.0"

#define AGENT_IP "192.168.100.1"
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define HAVE_DMALLOC_H 0

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief The sample code for how to get a snmp oid data
/// @param none
/// @return TRUE: SNMP get successfully
/// @return FALSE: SNMP get failure
/// @sa
/// @note
/// Command: \b SNMP_GET \n
//------------------------------------------------------------------------------
MS_BOOL Demo_SNMP_Get(void)
{

    netsnmp_session session, *ss;
    netsnmp_pdu *pdu;
    netsnmp_pdu *response;

    oid anOID[MAX_OID_LEN];
    size_t anOID_len;

    netsnmp_variable_list *vars;
    int status;
    int count=1;
    int current_name = 4;
    int failures = 0;
    const char *names[]={OID_TO_GET1, OID_TO_GET2, OID_TO_GET3, OID_TO_GET4};

    /*
    *      * Initialize the SNMP library
    *           */

    /*
    *      * Initialize a "session" that defines who we're going to talk to
    *           */
    snmp_sess_init( &session );                   /* set up defaults */

    session.peername = strdup(AGENT_IP);

    /* set the SNMP version number */
    session.version = SNMP_VERSION_2c;

    /* set the SNMPv1 community name used for authentication */
    session.community = "public";
    session.community_len = strlen(session.community);

    /* Open the session */
    SOCK_STARTUP;
    ss = snmp_open(&session);                     /* establish the session */

    if (!ss) {
        snmp_sess_perror("ack", &session);
        SOCK_CLEANUP;
        return FALSE;
    }

    /* Create the PDU for the data for our request.
       1) We're going to GET the system.sysDescr.0 node.
    */

    pdu = snmp_pdu_create(SNMP_MSG_GET);
/*
    anOID_len = MAX_OID_LEN;
    if (!snmp_parse_oid(OID_TO_GET1, anOID, &anOID_len)) {
        snmp_perror(OID_TO_GET1);
        SOCK_CLEANUP;
        return FALSE;
    }

    snmp_add_null_var(pdu, anOID, anOID_len);
*/
    for (count = 0; count < current_name; count++) {
        anOID_len = MAX_OID_LEN;
        if (!snmp_parse_oid(names[count], anOID, &anOID_len)) {
            snmp_perror(names[count]);
            failures++;
        } else
            snmp_add_null_var(pdu, anOID, anOID_len);
    }

    if(failures)
    {
        SOCK_CLEANUP;
        return FALSE;
    }

    /*
 *      * Send the Request out.
 *           */
    status = snmp_synch_response(ss, pdu, &response);

    /*
 *      * Process the response.
 *           */
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
        /*
        *        * SUCCESS: Print the result variables
        *               */

        for(vars = response->variables; vars; vars = vars->next_variable)
            print_variable(vars->name, vars->name_length, vars);

        /* manipuate the information ourselves */
        for(vars = response->variables; vars; vars = vars->next_variable) {

            if (vars->type == ASN_OCTET_STR) {
                char *sp = (char *)malloc(1 + vars->val_len);
                memcpy(sp, vars->val.string, vars->val_len);
                sp[vars->val_len] = '\0';
                printf("value #%d is a string: %s\n", count++, sp);
                free(sp);
            }
            else
                printf("value #%d is NOT a string! TYPE = %x Ack!\n", count++, vars->type);
        }
    } else {
      /*
 *        * FAILURE: print what went wrong!
 *               */

        if (status == STAT_SUCCESS)
            fprintf(stderr, "Error in packet\nReason: %s\n",
                snmp_errstring(response->errstat));
        else if (status == STAT_TIMEOUT)
            fprintf(stderr, "Timeout: No response from %s.\n",
            session.peername);
        else
            snmp_sess_perror("snmpdemoapp", ss);

    }

    /*
 *      * Clean up:
 *           *  1) free the response.
 *                *  2) close the session.
 *                     */
    if (response)
        snmp_free_pdu(response);
    snmp_close(ss);

    SOCK_CLEANUP;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to initialize snmp module
/// @param none
/// @return TRUE: Always return TRUE
/// @sa
/// @note
/// Command: \b SNMP_INIT \n
//------------------------------------------------------------------------------
MS_BOOL Demo_SNMP_Init(void)
{
    init_snmp("snmpdemoapp");
    return TRUE;
}
#endif
