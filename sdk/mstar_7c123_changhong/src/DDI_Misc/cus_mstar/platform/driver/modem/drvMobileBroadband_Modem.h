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
#if defined(NMGR_ENABLE) && (NMGR_ENABLE==1)
#if defined(CELLULAR_ENABLE) && (CELLULAR_ENABLE==1)

#if defined (MSOS_TYPE_ECOS)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"

#include <cyg/io/devtab.h>
#include <cyg/io/serial.h>
#include <cyg/io/serialio.h>

//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------
struct usb_serial_port
{
    void *dev_tab;
    //MS_S32 dev_mutex;
    MS_BOOL throttled;
    MS_BOOL throttle_req;
    struct usb_device *pusb_dev;
    struct usb_interface *pusb_intf;
    MS_U8 line_control;
    MS_U8 line_status;

    //control endpoint
    MS_U32 ctrl_send_pipe;
    MS_U32 ctrl_recv_pipe;

    //bulk in endpoint
    struct urb *read_urb;
    MS_U32 bulk_recv_pipe;
    MS_U8 *bulk_in_buffer;
    MS_U16  bulk_in_size;

    //bulk out endpoint
    struct urb *write_urb;
    MS_U32 bulk_send_pipe;
    MS_U16 bulk_out_size;
    MS_S32 bulk_out_event_id;

    //interrupt in endpoint
    struct urb *interrupt_in_urb;
    MS_U32 interrupt_recv_pipe;
    MS_U8 *interrupt_in_buffer;
    MS_U16 interrupt_in_size;
    MS_U8 interrupt_interval;

    MS_U8 interface_num;
    MS_BOOL bDevInit;
    MS_U32 u32Port;
};

typedef struct {
    MS_U32 u32Port;
    MS_U32 u32DevId;
} usb_serial_info_t;

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define MAX_NUM_OF_MODEM                1

#define USB_MODEM_PLUG_IN               1
#define USB_MODEM_PLUG_OUT              2

//Class-Specific Request Codes
//This request is used to issue a command in the format of the supported control protocol of the Communications Class interface.
#define SEND_ENCAPSULATED_COMMAND       0x00
#define SEND_ENCAPSULATED_COMMAND_TYPE  0x21
//This request is used to request a response in the format of the supported control protocol of the Communications Class interface.
#define GET_ENCAPSULATED_RESPONSE       0x01
#define GET_ENCAPSULATED_RESPONSE_TYPE  0xa1
//This request controls the settings for a particular communications feature of a particular target
#define SET_COMM_FEATURE                0x02
#define SET_COMM_FEATURE_TYPE           0x21
//This request returns the current settings for the communications feature as selected.
#define GET_COMM_FEATURE                0x03
#define GET_COMM_FEATURE_TYPE           0xa1
//This request controls the settings for a particular communications feature of a particular target, setting the selected feature to its default state. The validity of the feature selectors depends upon the target type of the request.
#define CLEAR_COMM_FEATURE              0x04
#define CLEAR_COMM_FEATURE_TYPE         0x21
//This request allows the host to specify typical asynchronous line-character formatting properties, which may be required by some applications.
#define SET_LINE_CODING                 0x20
#define SET_LINE_CODING_TYPE            0x21
//This request allows the host to find out the currently configured line coding.
#define GET_LINE_CODING                 0x21
#define GET_LINE_CODING_TYPE            0xa1
//This request generates RS-232/V.24 style control signals.
//Control Signal Bitmap -> D0: DTR. D1: RTS. D15..D2: RESERVED (Reset to zero)
#define SET_CONTROL_LINE_STATE          0x22
#define SET_CONTROL_LINE_STATE_TYPE     0x21
//This request sends special carrier modulation that generates an RS-232 style break.
//wValue field contains the Duration of Break, in milliseconds.
//If wValue contains a value of FFFFh, then the device will send a break until another SendBreak request is received with the wValue of 0000h.
#define SEND_BREAK                      0x23
#define SEND_BREAK_TYPE                 0x21

//Control Signal Bitmap Values for SetControlLineState
#define CONTROL_DTR                     0x01
#define CONTROL_RTS                     0x02

//Class-Specific Notification Codes for PSTN subclasses
//This notification sends asynchronous notification of UART status.
#define SERIAL_STATE                    0x20
#define SERIAL_STATE_TYPE               0xa1
//UART State Bitmap Values
#define UART_STATE                      0x08
#define UART_STATE_TRANSIENT_MASK       0x74
#define UART_DCD                        0x01
#define UART_DSR			            0x02
#define UART_BREAK_ERROR		        0x04
#define UART_RING			            0x08
#define UART_FRAME_ERROR		        0x10
#define UART_PARITY_ERROR		        0x20
#define UART_OVERRUN_ERROR		        0x40
#define UART_CTS			            0x80

#define USBMODEM_BUFSIZE                (1024*24)

typedef void ( *USBModemCallback ) (MS_U8 u8Event, MS_U8* u8Dev);
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
extern struct usb_driver usb_modem_driver;
extern USBModemCallback usb_modem_CbFun;

//-------------------------------------------------------------------------------------------------
// API prototype
//-------------------------------------------------------------------------------------------------

void MDrv_MobileBroadband_RegisterCallBack (USBModemCallback pCallbackFn);

void MDrv_MobileBroadband_Modem_Init(void);

void MDrv_MobileBroadband_Modem_DeInit(void);

int MDrv_MobileBroadband_Modem_Open(MS_U32 u32Port, void *pDevtab);

int MDrv_MobileBroadband_Modem_Write(MS_U32 u32Port, MS_U8* u8Chars, MS_U16 u16Length, MS_U16* pu16LengthSent);

int MDrv_MobileBroadband_Modem_SetControlLines(MS_U32 u32Port, MS_U8 u8Value);

int MDrv_MobileBroadband_Modem_SetTermios(MS_U32 u32Port, cyg_serial_info_t *serial_info);

void MDrv_MobileBroadband_Modem_Throttle(MS_U32 u32Port);

void MDrv_MobileBroadband_Modem_Unthrottle(MS_U32 u32Port);

void MDrv_MobileBroadband_Modem_Debug_Data(MS_U8 *pu8Data, MS_U16 u16Actual_length);

MS_U32 MDrv_MobileBroadband_Modem_GetVidPid(void);

#endif
#endif
#endif
