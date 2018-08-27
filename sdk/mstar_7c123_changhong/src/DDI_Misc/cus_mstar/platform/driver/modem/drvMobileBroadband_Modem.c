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
#include "drvMobileBroadband_Modem.h"
#include "drvUSBHost.h"
#include "drvUSB_eCos.h"

extern bool usb_modem_instance_connect(MS_U32 u32Port);

extern bool usb_modem_instance_disconnect(MS_U32 u32Port);

extern MS_S32 gs32CachedPoolID;
//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#define USB_MODEM_DBG_ENABLE
#define USB_MODEM_ERR_ENABLE
//#define USB_MODEM_INFO_ENABLE

#ifdef USB_MODEM_DBG_ENABLE
#define USB_MODEM_DBG(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define USB_MODEM_DBG(fmt, arg...) do {} while (0)
#endif

#ifdef USB_MODEM_ERR_ENABLE
#define USB_MODEM_ERR(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define USB_MODEM_ERR(fmt, arg...) do {} while (0)
#endif

#ifdef USB_MODEM_INFO_ENABLE
#define USB_MODEM_INFO(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define USB_MODEM_INFO(fmt, arg...) do {} while (0)
#endif

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define DECLARE_VID_PID(VID, PID) (((MS_U32)VID & 0xFFFF) << 16 | ((MS_U32)PID & 0xFFFF))


/* Bulk Out event */
#define BLK_OUT_COMPLETE            0x01

/* Vendor and product IDs */
#define QUALCOMM_VENDOR_ID          0x05C6
#define SIMCOM_PRODUCT_SIM5360E     0x9000

#define HUAWEI_VENDOR_ID            0x12D1
#define HUAWEI_PRODUCT_E140C        0x140C

/* ZD Incorporated */
#define ZD_VENDOR_ID                0x0685
#define ZD_PRODUCT_7000             0x7000

/* structure size of SET_LINE_CODING*/
#define LINE_CODING_LENGTH          7

#define MODEM_MUTEX_TIMEOUT         100
//-------------------------------------------------------------------------------------------------
// API prototype
//-------------------------------------------------------------------------------------------------
static int _MobileBroadband_Modem_Probe(struct usb_interface *iface, const struct usb_device_id *id);
static void _MobileBroadband_Modem_Disconnect(struct usb_interface *iface);

//-------------------------------------------------------------------------------------------------
// Static Variable
//-------------------------------------------------------------------------------------------------
static Task_Info stModemTask = {-1, E_TASK_PRI_MEDIUM, NULL, 4096, "Modem Task"};

static struct usb_device_id modem_usb_ids [] =
{
    {(USBDEV_MATCH_ID_VENDOR | USBDEV_MATCH_ID_PRODUCT), QUALCOMM_VENDOR_ID, SIMCOM_PRODUCT_SIM5360E, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {(USBDEV_MATCH_ID_VENDOR | USBDEV_MATCH_ID_PRODUCT), HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_E140C, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{(USBDEV_MATCH_ID_VENDOR | USBDEV_MATCH_ID_PRODUCT), ZD_VENDOR_ID, ZD_PRODUCT_7000, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
};

static MS_U8 preLineStatus[MAX_NUM_OF_MODEM] = {0};

//Store Vid Pid pair of vaild modem
static MS_U32 vid_pid;

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
struct usb_driver usb_modem_driver =
{
    "usb-modem",
    _MobileBroadband_Modem_Probe,
    _MobileBroadband_Modem_Disconnect,
    modem_usb_ids
};

struct usb_serial_port *usb_modem_port[MAX_NUM_OF_MODEM] = {NULL};

USBModemCallback usb_modem_CbFun = NULL;
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static inline void _MobileBroadband_Modem_InfoSet(MS_U32 u32Port, struct usb_serial_port *pstData)
{
    usb_modem_port[u32Port] = pstData;
}

static inline void _MobileBroadband_Modem_InfoClear(MS_U32 u32Port)
{
    usb_modem_port[u32Port] = NULL;
}

static inline MS_BOOL _MobileBroadband_Modem_InfoValid(MS_U32 u32Port)
{
    return (usb_modem_port[u32Port] != NULL);
}

static inline MS_BOOL _MobileBroadband_Modem_IsInit(MS_U32 u32Port)
{
    return (usb_modem_port[u32Port] != NULL ? usb_modem_port[u32Port]->bDevInit : FALSE);
}

static inline void _MobileBroadband_Modem_Init(MS_U32 u32Port, MS_BOOL bInit)
{
    usb_modem_port[u32Port]->bDevInit = bInit;
}

static inline void _MobileBroadband_Modem_Connect(MS_U32 u32Port, void* pDevtab)
{
    usb_modem_port[u32Port]->dev_tab = (void*)pDevtab;
}

int _MobileBroadband_Modem_GetTermios(MS_U32 u32Port)
{
    struct usb_serial_port *port = NULL;
    serial_channel *chan = NULL;
    int ret = ENOERR;
    unsigned char *buf = NULL;
    MS_U32 u32DTERate = 0;
    MS_U8 u8Format = 0;
    MS_U8 u8ParityType = 0;
    MS_U8 u8DataBits = 0;
    MS_U8 u8Flags = 0;
    cyg_serial_info_t serial_info;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        return -ENXIO;
    }

    port = usb_modem_port[u32Port];
    buf = kmalloc(LINE_CODING_LENGTH, GFP_KERNEL);//Line Coding Structure

    ret = ms_usb_control_cmd(port->pusb_dev,
                            port->ctrl_recv_pipe,
                            GET_LINE_CODING,
                            GET_LINE_CODING_TYPE,
                            0, //Zero
                            port->interface_num, //Interface
                            buf, //Line Coding Structure
                            LINE_CODING_LENGTH, //Size of Structure
                            HZ*USB_CTRL_SET_TIMEOUT);

    if(ret < ENOERR)
    {
        kfree(buf);
        return ret;
    }

    //update serial channel current configuration
    u32DTERate = ((MS_U32)buf[0] | (MS_U32)buf[1]<<8 | (MS_U32)buf[2]<<16 | (MS_U32)buf[3]<<24);
    u8Format = buf[4];
    u8ParityType = buf[5];
    u8DataBits = buf[6];
    USB_MODEM_DBG("[%d][%s] u32DTERate: %d, u8Format: %u, u8ParityType: %u, u8DataBits: %u\n", __LINE__, __FUNCTION__, u32DTERate, u8Format, u8ParityType, u8DataBits);

    //mapping
    switch (u32DTERate)
    {
        case   9600 : serial_info.baud = CYGNUM_SERIAL_BAUD_9600;          break;
        case  38400 : serial_info.baud = CYGNUM_SERIAL_BAUD_38400;         break;
        case 115200 : serial_info.baud = CYGNUM_SERIAL_BAUD_115200;        break;
        default: USB_MODEM_DBG("Unsupported baud rate\n");                  break;
    }
    switch (u8Format)
    {
        case 0 : serial_info.stop = CYGNUM_SERIAL_STOP_1;                  break;
        case 1 : serial_info.stop = CYGNUM_SERIAL_STOP_1_5;                break;
        case 2 : serial_info.stop = CYGNUM_SERIAL_STOP_2;                  break;
        default: USB_MODEM_DBG("Unsupported u8Format\n");                   break;
    }
    switch (u8ParityType)
    {
        case 0 : serial_info.parity = CYGNUM_SERIAL_PARITY_NONE;           break;
        case 1 : serial_info.parity = CYGNUM_SERIAL_PARITY_EVEN;           break;
        case 2 : serial_info.parity = CYGNUM_SERIAL_PARITY_ODD;            break;
        case 3 : serial_info.parity = CYGNUM_SERIAL_PARITY_MARK;           break;
        case 4 : serial_info.parity = CYGNUM_SERIAL_PARITY_SPACE;          break;
        default: USB_MODEM_DBG("Unsupported u8ParityType\n");               break;
    }
    switch (u8DataBits)
    {
        case 5 : serial_info.word_length = CYGNUM_SERIAL_WORD_LENGTH_5;    break;
        case 6 : serial_info.word_length = CYGNUM_SERIAL_WORD_LENGTH_6;    break;
        case 7 : serial_info.word_length = CYGNUM_SERIAL_WORD_LENGTH_7;    break;
        case 8 : serial_info.word_length = CYGNUM_SERIAL_WORD_LENGTH_8;    break;
        default: USB_MODEM_DBG("Unsupported u8DataBits\n");                 break;
    }

    //This request generates RS-232/V.24 style control signals.D0: DTR, D1: RTS
    //u8Flags = drvMobileBroadband_Modem_GetControlLines(u32Port);
    serial_info.flags = 0;
    if(u8Flags & CONTROL_DTR)
    {
        serial_info.flags |= (CYGNUM_SERIAL_FLOW_DSRDTR_RX | CYGNUM_SERIAL_FLOW_DSRDTR_TX);
    }
    if(u8Flags & CONTROL_RTS)
    {
        serial_info.flags |= (CYGNUM_SERIAL_FLOW_RTSCTS_RX | CYGNUM_SERIAL_FLOW_RTSCTS_TX);
    }

    if(port->dev_tab)
    {
        chan = (serial_channel *)((cyg_devtab_entry_t *)port->dev_tab)->priv;
        memcpy((cyg_serial_info_t *)&(chan->config), &serial_info,  sizeof(serial_info));
        USB_MODEM_DBG("(baud: %d, stop: %d, parity: %d, wdlen: %d, flags: 0x%X)\n", chan->config.baud, chan->config.stop , chan->config.parity , chan->config.word_length , chan->config.flags);
    }
    kfree(buf);
    return ret;
}


static void _MobileBroadband_Modem_UpadateLineStatus(MS_U32 u32Port)
{
    struct usb_serial_port *port = NULL;
    serial_channel *chan = NULL;
    cyg_serial_line_status_t stLineStatus;

    port = usb_modem_port[u32Port];

    if((port->line_status != preLineStatus[u32Port]) && port->dev_tab)
    {
        chan = (serial_channel *)((cyg_devtab_entry_t *)port->dev_tab)->priv;
        if(chan)
        {
            if((port->line_status & UART_DCD) != (preLineStatus[u32Port] & UART_DCD))
            {
                //UART_DCD changed
                stLineStatus.which = CYGNUM_SERIAL_STATUS_CARRIERDETECT;
                stLineStatus.value = (port->line_status & UART_DCD);
                chan->callbacks->indicate_status(chan, &stLineStatus);
            }
            if((port->line_status & UART_DSR) != (preLineStatus[u32Port] & UART_DSR))
            {
                //UART_DSR changed
                stLineStatus.which = CYGNUM_SERIAL_STATUS_FLOW;
                stLineStatus.value = (port->line_status & UART_DSR);
                chan->callbacks->indicate_status(chan, &stLineStatus);
            }
            //save line status
            preLineStatus[u32Port] = port->line_status;
            USB_MODEM_DBG("%s - save line status 0x%2X\n", __FUNCTION__, preLineStatus[u32Port]);
        }
    }
}


//monitor line_status: DCD, DSR, RING??
static void _MobileBroadband_Modem_ReadInt_Cb(struct urb *urb, struct stPtRegs *regs)
{
    struct usb_serial_port *port = NULL;
    int status = 0;
    MS_U8 *pu8Data = NULL;
    MS_U16 u16Actual_length = 0;
    int retval = 0;

    if(!urb || !urb->pContext)
    {
        USB_MODEM_ERR("%s - urb(/context) is null\n", __FUNCTION__);
        return;
    }

    port = (struct usb_serial_port *)urb->pContext;
    status = (int)urb->s32Status;
    pu8Data = (MS_U8 *)urb->pTransferBuffer;
    u16Actual_length = (MS_U16)urb->u32ActualLength;

    switch (status)
    {
        case 0:
            /* success */
            USB_MODEM_DBG("%s - success read interrupt urb status received: %d\n", __FUNCTION__, status);
            break;
        case -ECONNRESET:
        case -ENOENT:
        case -ESHUTDOWN:
        default:
            /* this urb is terminated, clean up */
            USB_MODEM_DBG("%s - urb shutting down with status: %d\n", __FUNCTION__, status);
            return;
    }

    MDrv_MobileBroadband_Modem_Debug_Data(pu8Data, u16Actual_length);

    if (u16Actual_length < (UART_STATE+1))
    {
        USB_MODEM_ERR("%s - actual_length < length\n", __FUNCTION__);
        goto exit;
    }

    if(port->line_status != pu8Data[UART_STATE] && port->dev_tab)
    {
         //save line status
         port->line_status = (MS_U8)pu8Data[UART_STATE];
         USB_MODEM_DBG("%s - line_status 0x%2X\n", __FUNCTION__, port->line_status);
    }

exit:
    if(port->interrupt_in_urb)
    {
        retval = ms_usb_submit_urb(port->interrupt_in_urb, GFP_ATOMIC);
        if(retval < ENOERR)
        {
            USB_MODEM_ERR("[%s] submitting read interrupt urb, error %d\n", __FUNCTION__, retval);
        }
    }
}

static void _MobileBroadband_Modem_ReadBulk_Cb(struct urb *urb, struct stPtRegs *regs)
{
    struct usb_serial_port *port = NULL;
    serial_channel *chan = NULL;
    int status = 0;
    MS_U8 *pu8Data = NULL;
    MS_U16 u16Actual_length = 0;
    int retval = 0;

    if(!urb || !urb->pContext)
    {
        USB_MODEM_ERR("%s - urb(/context) is null\n", __FUNCTION__);
        return;
    }
    port = (struct usb_serial_port *)urb->pContext;
    status = (int)urb->s32Status;
    pu8Data = (MS_U8 *)urb->pTransferBuffer;
    u16Actual_length = (MS_U16)urb->u32ActualLength;

    if(status != 0)
    {
        USB_MODEM_ERR("%s - nonzero read bulk status received: %d\n", __FUNCTION__, status);
        return;
    }

    MDrv_MobileBroadband_Modem_Debug_Data(pu8Data, u16Actual_length);

    /* Throttle the device if requested by tty */
	port->throttled = port->throttle_req;
	if (!port->throttled)
    {
        MS_U8* u8Space = NULL;
        MS_U32 u32SpaceAvail = 0;

        if(port->dev_tab)
        {
            chan = (serial_channel *)((cyg_devtab_entry_t *)port->dev_tab)->priv;
            retval = (chan->callbacks->data_rcv_req)(chan, USBMODEM_BUFSIZE, (int*)&u32SpaceAvail, (unsigned char**)&u8Space);
            if(retval == CYG_XMT_OK)
            {
                if (u16Actual_length > u32SpaceAvail) u16Actual_length = u32SpaceAvail;
                memcpy(u8Space, pu8Data, u16Actual_length);
                (chan->callbacks->data_rcv_done)(chan, u16Actual_length);
            }
            //else
            //{
                //ERROR!!! actually read size is 0. Do nothing.
            //}
        }

        if(port->read_urb)
        {
            //submit read bulk
            retval = ms_usb_submit_urb(port->read_urb, GFP_ATOMIC);
            if(retval < ENOERR)
            {
                USB_MODEM_ERR("[%s] submitting read urb, error %d\n", __FUNCTION__, retval);
            }
        }
	}
}

static void _MobileBroadband_Modem_WriteBulk_Cb(struct urb *urb, struct stPtRegs *regs)
{
    struct usb_serial_port *port = NULL;

    if(!urb || !urb->pContext)
    {
        USB_MODEM_ERR("%s - urb(/context) is null\n", __FUNCTION__);
        return;
    }

    //send event
    port = (struct usb_serial_port *)urb->pContext;
    port->write_urb->u32ActualLength = urb->u32ActualLength;
    MsOS_SetEvent(port->bulk_out_event_id, BLK_OUT_COMPLETE);
}

static MS_BOOL _MobileBroadband_Modem_AssociateDevice(struct usb_serial_port *port, struct usb_interface *ms_intf)
{
    if(!port || !ms_intf)
    {
        return FALSE;
    }
    else
    {
        port->pusb_dev = interface_to_usbdev(ms_intf);
        port->pusb_intf = ms_intf;
        port->interface_num = ms_intf->altsetting[port->pusb_intf->act_altsetting].desc.bInterfaceNumber;
        ms_usb_set_intfdata(ms_intf, port);
        ms_usb_get_dev(port->pusb_dev);
        USB_MODEM_DBG("[%d][%s] u32DevNum 0x%X, devpath %s, intf no. #0x%X\n", __LINE__, __FUNCTION__, port->pusb_dev->u32DevNum, port->pusb_dev->devpath, port->interface_num);
        return TRUE;
    }
}

static void _MobileBroadband_Modem_SetPipe(struct usb_serial_port *port,
    struct usb_endpoint_descriptor *ep_bulk_in,
    struct usb_endpoint_descriptor *ep_bulk_out,
    struct usb_endpoint_descriptor *ep_int_in)
{
    if(!port)
    {
        return;
    }
    else
    {
        //control
        port->ctrl_send_pipe = usb_sndctrlpipe(port->pusb_dev, 0);
        port->ctrl_recv_pipe = usb_rcvctrlpipe(port->pusb_dev, 0);
        //bulk
        if(ep_bulk_out)
        {
            port->bulk_send_pipe = usb_sndbulkpipe(port->pusb_dev, ep_bulk_out->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
            port->bulk_out_size = ep_bulk_in->wMaxPacketSize;
        }
        if(ep_bulk_in)
        {
            port->bulk_recv_pipe = usb_rcvbulkpipe(port->pusb_dev, ep_bulk_in->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
            port->bulk_in_size = ep_bulk_in->wMaxPacketSize;
        }
        //interrupt
        if(ep_int_in)
        {
            port->interrupt_recv_pipe = usb_rcvintpipe(port->pusb_dev, ep_int_in->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
            port->interrupt_in_size = ep_int_in->wMaxPacketSize;
            port->interrupt_interval = ep_int_in->bInterval;
        }
    }
    return;
}

static MS_BOOL _MobileBroadband_Modem_GetPipe(struct usb_serial_port *port)
{
    int tmp = 0;
    struct usb_host_interface *altsetting = NULL;
    struct usb_endpoint_descriptor *ep_tmp = NULL;
    struct usb_endpoint_descriptor *ep_bulk_in = NULL;
    struct usb_endpoint_descriptor *ep_bulk_out = NULL;
    struct usb_endpoint_descriptor *ep_int_in = NULL;

    if(!port)
    {
        return FALSE;
    }
    else
    {
        altsetting = &port->pusb_intf->altsetting[port->pusb_intf->act_altsetting];
        for(tmp = 0; tmp < altsetting->desc.bNumEndpoints; tmp++)
        {
            ep_tmp = &altsetting->endpoint[tmp].desc;
            if((ep_tmp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)
            {
                if(ep_tmp->bEndpointAddress & USB_DIR_IN)
                {
                    USB_MODEM_DBG("[%d][%s] ep_bulk_in EndpointAddress 0x%X\n", __LINE__, __FUNCTION__, ep_tmp->bEndpointAddress);
                    ep_bulk_in = ep_tmp;
                }
                else
                {
                    USB_MODEM_DBG("[%d][%s] ep_bulk_out EndpointAddress 0x%X\n", __LINE__, __FUNCTION__, ep_tmp->bEndpointAddress);
                    ep_bulk_out = ep_tmp;
                }
            }
            else if((ep_tmp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)
            {
                if(ep_tmp->bEndpointAddress & USB_DIR_IN)
                {
                    USB_MODEM_DBG("[%d][%s] ep_int_in EndpointAddress 0x%X\n", __LINE__, __FUNCTION__, ep_tmp->bEndpointAddress);
                    ep_int_in = ep_tmp;
                }
                else
                {
                    USB_MODEM_DBG("[%d][%s] ep_int_out EndpointAddress 0x%X\n", __LINE__, __FUNCTION__, ep_tmp->bEndpointAddress);
                }
            }
        }
        if(ep_bulk_in != NULL && ep_bulk_out != NULL/* && ep_int_in != NULL*/)
        {
            _MobileBroadband_Modem_SetPipe(port, ep_bulk_in, ep_bulk_out, ep_int_in);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

static MS_BOOL _MobileBroadband_Modem_AcquireResources(struct usb_serial_port *port)
{
    if(!port)
    {
        return FALSE;
    }

    //allocate bulk URB
    port->read_urb = ms_usb_alloc_urb(GFP_KERNEL);
    port->write_urb = ms_usb_alloc_urb(GFP_KERNEL);

    if((!port->read_urb) || (!port->write_urb))
    {
        USB_MODEM_ERR("USB Request Block allocates error\n");
        return FALSE;
    }
    else
    {
        //allocate bulk in buffer
        port->bulk_in_buffer = kmalloc(port->bulk_in_size, GFP_KERNEL);
        if(!port->bulk_in_buffer)
        {
            USB_MODEM_ERR("allocate bulk in buffer error\n");
            return FALSE;
        }
        memset(port->bulk_in_buffer, 0, port->bulk_in_size);

        //create event group for bulk out
        port->bulk_out_event_id = -1;
        port->bulk_out_event_id = MsOS_CreateEventGroup("BULKOUT_EVENT");
        if(port->bulk_out_event_id < 0)
        {
            USB_MODEM_ERR("USB create event group for bulk out error\n");
            return FALSE;
        }
    }

    if(port->interrupt_recv_pipe)
    {
        //allocate interrupt URB
        port->interrupt_in_urb = ms_usb_alloc_urb(GFP_KERNEL);
        if(!port->interrupt_in_urb)
        {
            USB_MODEM_ERR("USB Request Block allocates error\n");
            return FALSE;
        }
        else
        {
            //allocate interrupt in buffer
            port->interrupt_in_buffer = kmalloc(port->interrupt_in_size, GFP_KERNEL);
            if(!port->interrupt_in_buffer)
            {
                USB_MODEM_ERR("allocate interrupt in buffer error\n");
                return FALSE;
            }
            memset(port->interrupt_in_buffer, 0, port->interrupt_in_size);
        }
    }

    return TRUE;
}

static void _MobileBroadband_Modem_Unregister_Device(struct usb_serial_port *port)
{
    if(!port)
    {
        return;
    }

    //free urb
    if (port->read_urb)
    {
        ms_usb_free_urb(port->read_urb);
        port->read_urb = NULL;
    }
    if (port->write_urb)
    {
        ms_usb_free_urb(port->write_urb);
        port->write_urb = NULL;
    }
    if (port->interrupt_in_urb)
    {
        ms_usb_free_urb(port->interrupt_in_urb);
        port->interrupt_in_urb = NULL;
    }

    //free buffer
    if (port->bulk_in_buffer)
    {
        kfree(port->bulk_in_buffer);
        port->bulk_in_buffer = NULL;
    }
    if (port->interrupt_in_buffer)
    {
        kfree(port->interrupt_in_buffer);
        port->interrupt_in_buffer = NULL;
    }

    //deleate event group
    if(port->bulk_out_event_id > 0)
    {
        MsOS_DeleteEventGroup(port->bulk_out_event_id);
        port->bulk_out_event_id = -1;
    }

    ms_usb_set_intfdata(port->pusb_intf, NULL);
    ms_usb_put_dev(port->pusb_dev);
    port->pusb_dev = NULL;
    port->pusb_intf = NULL;

    _MobileBroadband_Modem_InfoClear(port->u32Port);
    usb_modem_instance_disconnect(port->u32Port);
    kfree(port);
}

static MS_BOOL _MobileBroadband_Modem_SupportedInterface(MS_U16 u16IdVender, MS_U16 u16IdProduct, MS_U8 u8NumInterfaces, MS_U8 u8InterfaceNum)
{
    if(u16IdVender == QUALCOMM_VENDOR_ID && u16IdProduct == SIMCOM_PRODUCT_SIM5360E)
    {
        //Composite mode
        if(u8InterfaceNum == 3)
        {
            USB_MODEM_DBG( "Modem port found\n" );
            return TRUE;
        }
        else
        {
            // Not a port we want to support at this time
            return FALSE;
        }
    }
    else if(u16IdVender == HUAWEI_VENDOR_ID && u16IdProduct == HUAWEI_PRODUCT_E140C)
    {
        if(u8InterfaceNum == 0)
        {
            USB_MODEM_DBG( "Modem port found\n" );
            return TRUE;
        }
        else
        {
            // Not a port we want to support at this time
            return FALSE;
        }
    }
    else
    {
        //not support
        return FALSE;
    }
    return TRUE;
}

static int _MobileBroadband_Modem_Probe(struct usb_interface *ms_intf, const struct usb_device_id *id)
{
    struct usb_serial_port *port = NULL;
    MS_U16 u16IdVender = 0, u16IdProduct = 0;
    MS_U8 u8NumInterfaces = 0, u8InterfaceNum = 0;
    MS_U32 u32Port = 0;

    if(ms_intf == NULL || ms_intf->altsetting == NULL)
    {
        return -EINVAL;
    }

    u16IdVender = ((struct usb_device *)interface_to_usbdev(ms_intf))->descriptor.idVendor;
    u16IdProduct = ((struct usb_device *)interface_to_usbdev(ms_intf))->descriptor.idProduct;
    u8NumInterfaces = ((struct usb_device *)interface_to_usbdev(ms_intf))->config[0].desc.bNumInterfaces;
    u8InterfaceNum = ms_intf->altsetting[ms_intf->act_altsetting].desc.bInterfaceNumber;

    if(!_MobileBroadband_Modem_SupportedInterface(u16IdVender, u16IdProduct, u8NumInterfaces, u8InterfaceNum))
    {
        return ENOERR;
    }

    port = (struct usb_serial_port *) kmalloc(sizeof(struct usb_serial_port), GFP_KERNEL);
    if(!port)
    {
        USB_MODEM_ERR("Out of memory %d\n", port);
        return -ENOMEM;
    }
    else
    {
        memset(port, 0, sizeof(struct usb_serial_port));
    }

    if(!_MobileBroadband_Modem_AssociateDevice(port, ms_intf))
    {
        _MobileBroadband_Modem_Unregister_Device(port);
        return -EINVAL;
    }

    if(!_MobileBroadband_Modem_GetPipe(port))
    {
        _MobileBroadband_Modem_Unregister_Device(port);
        return -EINVAL;
    }

    if(!_MobileBroadband_Modem_AcquireResources(port))
    {
        _MobileBroadband_Modem_Unregister_Device(port);
        return -ENOMEM;
    }

    for(u32Port = 0; u32Port < MAX_NUM_OF_MODEM; u32Port++)
    {
        if(!_MobileBroadband_Modem_InfoValid(u32Port))
        {
            break;
        }
    }

    if(u32Port < MAX_NUM_OF_MODEM)
    {
        port->u32Port = u32Port;
        _MobileBroadband_Modem_InfoSet(u32Port, port);
        USB_MODEM_DBG("[%d][%s] usb_modem_port index %d\n", __LINE__, __FUNCTION__, u32Port);
    }
    else
    {
        USB_MODEM_ERR("USB Modem: exceed the max number of support devices!!!\n");
        _MobileBroadband_Modem_Unregister_Device(port);
        return -ENOBUFS;
    }
    USB_MODEM_DBG("[%d][%s] find USB Modem device at device number %d\n", __LINE__, __FUNCTION__, port->pusb_dev->u32DevNum);

    vid_pid=DECLARE_VID_PID(u16IdVender,u16IdProduct);

    return ENOERR;
}

static void _MobileBroadband_Modem_Disconnect(struct usb_interface *ms_intf)
{
    MS_U16 u16IdVender = 0, u16IdProduct = 0;
    MS_U8 u8NumInterfaces = 0, u8InterfaceNum = 0;
    struct usb_serial_port *port = NULL;

    if(ms_intf == NULL || ms_intf->altsetting == NULL)
    {
        return;
    }
    u16IdVender = ((struct usb_device *)interface_to_usbdev(ms_intf))->descriptor.idVendor;
    u16IdProduct = ((struct usb_device *)interface_to_usbdev(ms_intf))->descriptor.idProduct;
    u8NumInterfaces = ((struct usb_device *)interface_to_usbdev(ms_intf))->config[0].desc.bNumInterfaces;
    u8InterfaceNum = ms_intf->altsetting[ms_intf->act_altsetting].desc.bInterfaceNumber;

    if(!_MobileBroadband_Modem_SupportedInterface(u16IdVender, u16IdProduct, u8NumInterfaces, u8InterfaceNum))
    {
        return;
    }

    port = (struct usb_serial_port*)ms_usb_get_intfdata(ms_intf);
    if(port)
    {
        USB_MODEM_DBG("\033[032mUSB Modem disconnect usb_modem_port index: %d\n\033[0m", port->u32Port);
        //URB Active? Cancel USB request block
        ms_usb_unlink_urb(port->read_urb);
        ms_usb_unlink_urb(port->write_urb);
        ms_usb_unlink_urb(port->interrupt_in_urb);
        _MobileBroadband_Modem_Unregister_Device(port);
    }

    return;
}

static void _MobileBroadband_Modem_Task(MS_U32 argc, void *argv)
{
    MS_U32 u32Port = 0;
    MS_BOOL bTaskStop = FALSE;

    while(1)
    {
        for(u32Port = 0; u32Port < MAX_NUM_OF_MODEM; u32Port++)
        {
            if(_MobileBroadband_Modem_InfoValid(u32Port))
            {
                if(!_MobileBroadband_Modem_IsInit(u32Port))
                {
                    USB_MODEM_DBG("found a USB Modem device @ port %d, try to init it\n", usb_modem_port[u32Port]->u32Port);

                    if(usb_modem_instance_connect(u32Port))
                    {
                        _MobileBroadband_Modem_Init(u32Port, TRUE);
                        USB_MODEM_DBG("USB Modem plug in\n");
                    }
                }
                _MobileBroadband_Modem_UpadateLineStatus(u32Port);
            }
        }

        if(bTaskStop)
        {
            break;
        }

        MsOS_DelayTask(100);
    }
}

void MDrv_MobileBroadband_RegisterCallBack (USBModemCallback pCallbackFn)
{
    //Register a callback function for application to process the data received
    usb_modem_CbFun = pCallbackFn;
}

void MDrv_MobileBroadband_Modem_Init(void)
{
    USB_MODEM_DBG("Init USB Modem\n");

    if(stModemTask.iId < 0)
    {
        stModemTask.pStack = MsOS_AllocateMemory(stModemTask.u32StackSize, gs32CachedPoolID);
	    if(!stModemTask.pStack)
	    {
	        GEN_EXCEP;
	    }
	    stModemTask.iId = MsOS_CreateTask( _MobileBroadband_Modem_Task,
	                                       (MS_U32)NULL,
	                                       stModemTask.ePriority,
	                                       TRUE,
	                                       stModemTask.pStack,
	                                       stModemTask.u32StackSize,
	                                       stModemTask.szName);

        if(stModemTask.iId < 0)
        {
           GEN_EXCEP;
        }
    }

    if (ms_usb_register(&usb_modem_driver) < 0)
    {
        USB_ASSERT( 0, "Init USB Modem fail..\n");
    }
}

void MDrv_MobileBroadband_Modem_DeInit(void)
{
    ms_usb_deregister(&usb_modem_driver);
}

int MDrv_MobileBroadband_Modem_Open(MS_U32 u32Port, void *pDevtab)
{
    struct usb_serial_port *port = NULL;
    int ret = ENOERR;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        return -ENXIO;
    }

    port = usb_modem_port[u32Port];
    //save devtab
    _MobileBroadband_Modem_Connect(u32Port, pDevtab);
    USB_MODEM_DBG("Open USB modem with usb path:%s\n", port->pusb_dev->devpath);

    //submit interrupt in URB
    if(port->interrupt_in_urb)
    {
        ms_usb_stuff_intr_urb(port->interrupt_in_urb,
                              port->pusb_dev,
                              port->interrupt_recv_pipe,
                              port->interrupt_in_buffer,
                              port->interrupt_in_size,
                              _MobileBroadband_Modem_ReadInt_Cb,
                              (void *)port,
                              port->interrupt_interval);
        ret = ms_usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
        if(ret < ENOERR)
        {
            USB_MODEM_ERR("[%s] submitting interrupt read urb, error %d\n", __FUNCTION__, ret);
            return ret;
        }
    }

    //make sure endpoint data toggle is synchronized with the device
    ret = ms_usb_clear_halt(port->pusb_dev, port->bulk_send_pipe);
    if(ret < ENOERR)
    {
        USB_MODEM_ERR("Clearing endpoint halt err\n");
        return ret;
    }

    //submit bulk in read URB
    if(port->read_urb)
    {
        ms_usb_stuff_bulk_urb(port->read_urb,
                              port->pusb_dev,
                              port->bulk_recv_pipe,
                              port->bulk_in_buffer,
                              port->bulk_in_size,
                              _MobileBroadband_Modem_ReadBulk_Cb,
                              (void *)port);
        ret = ms_usb_submit_urb(port->read_urb, GFP_KERNEL);
        if(ret < ENOERR)
        {
            USB_MODEM_ERR("[%s] submitting bulk read urb, error %d\n", __FUNCTION__, ret);
            return ret;
        }
    }

    _MobileBroadband_Modem_GetTermios(u32Port);

    return ENOERR;
}

int MDrv_MobileBroadband_Modem_Write(MS_U32 u32Port, MS_U8* u8Chars, MS_U16 u16Length, MS_U16* pu16LengthSent)
{
    struct usb_serial_port *port = NULL;
    MS_U32 u32Events = 0;
    MS_BOOL bError = FALSE;
    int ret = ENOERR;
    MS_U16 u16BufferLength = 0, u16ActualLength = 0, u16CharsAvail = 0, u16CharsSent = 0;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        *pu16LengthSent = 0;
        return -ENODEV;
    }

    port = usb_modem_port[u32Port];

    u16CharsAvail = u16Length;
    //while(u16CharsAvail > 0)
    {
        u16BufferLength = u16CharsAvail;
        if (u16BufferLength > port->bulk_out_size) u16BufferLength = port->bulk_out_size;

        //submit write out URB
        if(port->write_urb)
        {
            ms_usb_stuff_bulk_urb(port->write_urb,
                                  port->pusb_dev,
                                  port->bulk_send_pipe,
                                  (void*)(u8Chars+u16CharsSent),
                                  (MS_S32)u16BufferLength,
                                  _MobileBroadband_Modem_WriteBulk_Cb,
                                  (void *)port);

            MsOS_ClearEvent (port->bulk_out_event_id, 0);
            ret = ms_usb_submit_urb(port->write_urb, GFP_KERNEL);

            //wait event
            bError = MsOS_WaitEvent(port->bulk_out_event_id, BLK_OUT_COMPLETE, &u32Events, E_OR_CLEAR, 10000);

            if(!bError || !(u32Events & BLK_OUT_COMPLETE)) // time out
            {
                u16ActualLength = 0;
                ret = -ETIMEDOUT;
            }
            else //ok
            {
                u16ActualLength = (MS_U16)port->write_urb->u32ActualLength;
            }
        }
        else
        {
            u16ActualLength = 0;
            ret = -ENODEV;
        }
        u16CharsAvail -= u16ActualLength;
        u16CharsSent += u16ActualLength;

        //if(ret < ENOERR)
        //{
        //    break;
        //}
    }

    *pu16LengthSent = u16CharsSent;
    return ret;
}

//This request generates RS-232/V.24 style control signals.D0: DTR, D1: RTS
int MDrv_MobileBroadband_Modem_SetControlLines(MS_U32 u32Port, MS_U8 u8Value)
{
    struct usb_serial_port *port = NULL;
    int ret = ENOERR;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        return -ENXIO;
    }

    port = usb_modem_port[u32Port];

    ret = ms_usb_control_cmd(port->pusb_dev,
                            port->ctrl_send_pipe,
                            SET_CONTROL_LINE_STATE,
                            SET_CONTROL_LINE_STATE_TYPE,
                            (u8Value & (CONTROL_RTS | CONTROL_DTR)),
                            port->interface_num,//Interface
                            NULL,//None
                            0,
                            HZ*USB_CTRL_SET_TIMEOUT);
    if(ret >= ENOERR)
    {
        port->line_control = (u8Value & (CONTROL_RTS | CONTROL_DTR));
        USB_MODEM_DBG("[%d][%s] line_control = 0x%X\n", __LINE__, __FUNCTION__, port->line_control);
    }
    return ret;
}

//set serial_info: baud, stop, parity, word length, DTR/RTS.
int MDrv_MobileBroadband_Modem_SetTermios(MS_U32 u32Port, cyg_serial_info_t *serial_info)
{
    struct usb_serial_port *port = NULL;
    serial_channel *chan = NULL;
    int ret = ENOERR;
    unsigned char *buf = NULL;
    MS_U32 u32DTERate = 0;
    MS_U8 u8Format = 0;
    MS_U8 u8ParityType = 0;
    MS_U8 u8DataBits = 0;
    MS_U8 u8Flags = 0;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        return -ENXIO;
    }

    port = usb_modem_port[u32Port];
    //reset line status
    preLineStatus[u32Port] = 0;
    port->line_status = 0;
    buf = kmalloc(LINE_CODING_LENGTH, GFP_KERNEL);//Line Coding Structure

    //mapping
    switch (serial_info->baud)
    {
        case   CYGNUM_SERIAL_BAUD_9600 : u32DTERate= 9600;      break;
        case  CYGNUM_SERIAL_BAUD_38400 : u32DTERate = 38400;    break;
        case CYGNUM_SERIAL_BAUD_115200 : u32DTERate = 115200;   break;
        default: USB_MODEM_ERR("Unsupported baud rate\n");      break;
    }
    switch (serial_info->stop)
    {
        case   CYGNUM_SERIAL_STOP_1 : u8Format = 0;             break;
        case CYGNUM_SERIAL_STOP_1_5 : u8Format = 1;             break;
        case   CYGNUM_SERIAL_STOP_2 : u8Format = 2;             break;
        default: USB_MODEM_ERR("Unsupported u8Format\n");       break;
    }
    switch (serial_info->parity)
    {
        case  CYGNUM_SERIAL_PARITY_NONE : u8ParityType = 0;     break;
        case  CYGNUM_SERIAL_PARITY_EVEN : u8ParityType = 1;     break;
        case   CYGNUM_SERIAL_PARITY_ODD : u8ParityType = 2;     break;
        case  CYGNUM_SERIAL_PARITY_MARK : u8ParityType = 3;     break;
        case CYGNUM_SERIAL_PARITY_SPACE : u8ParityType = 4;     break;
        default: USB_MODEM_ERR("Unsupported u8ParityType\n");   break;
    }
    switch (serial_info->word_length)
    {
        case CYGNUM_SERIAL_WORD_LENGTH_5 : u8DataBits = 5;      break;
        case CYGNUM_SERIAL_WORD_LENGTH_6 : u8DataBits = 6;      break;
        case CYGNUM_SERIAL_WORD_LENGTH_7 : u8DataBits = 7;      break;
        case CYGNUM_SERIAL_WORD_LENGTH_8 : u8DataBits = 8;      break;
        default: USB_MODEM_ERR("Unsupported u8DataBits\n");     break;
    }

    buf[0] = u32DTERate & 0xff;
    buf[1] = (u32DTERate >> 8) & 0xff;
    buf[2] = (u32DTERate >> 16) & 0xff;
    buf[3] = (u32DTERate >> 24) & 0xff;
    buf[4] = u8Format;
    buf[5] = u8ParityType;
    buf[6] = u8DataBits;

    ret = ms_usb_control_cmd(port->pusb_dev,
                            port->ctrl_send_pipe,
                            SET_LINE_CODING,
                            SET_LINE_CODING_TYPE,
                            0, //Zero
                            port->interface_num, //Interface
                            buf, //Line Coding Structure
                            LINE_CODING_LENGTH, //Size of Structure
                            HZ*USB_CTRL_SET_TIMEOUT);

    if(ret < ENOERR)
    {
        kfree(buf);
        return ret;
    }
    USB_MODEM_DBG("[%d][%s] u32DTERate: %d, u8Format: %u, u8ParityType: %u, u8DataBits: %u\n", __LINE__, __FUNCTION__, u32DTERate, u8Format, u8ParityType, u8DataBits);

    //This request generates RS-232/V.24 style control signals.D0: DTR, D1: RTS
    u8Flags = CONTROL_DTR | CONTROL_RTS;
    ret = MDrv_MobileBroadband_Modem_SetControlLines(u32Port, u8Flags);
    if(ret < ENOERR)
    {
        kfree(buf);
        return ret;
    }

    //update serial channel current configuration
    if(port->dev_tab)
    {
        chan = (serial_channel *)((cyg_devtab_entry_t *)port->dev_tab)->priv;
        chan->config.baud = serial_info->baud;
        chan->config.stop = serial_info->stop;
        chan->config.parity = serial_info->parity;
        chan->config.word_length = serial_info->word_length;
        chan->config.flags = serial_info->flags;

        USB_MODEM_DBG("(baud: %d, stop: %d, parity: %d, wdlen: %d, flags: 0x%X)\n", chan->config.baud, chan->config.stop , chan->config.parity , chan->config.word_length , chan->config.flags);
    }

    kfree(buf);
    return ret;
}

void MDrv_MobileBroadband_Modem_Throttle(MS_U32 u32Port)
{
    struct usb_serial_port *port = NULL;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        return;
    }
    port = usb_modem_port[u32Port];
    /* Set the throttle request flag. It will be picked up
    * by _MobileBroadband_Modem_ReadBulk_Cb(). */
    port->throttle_req = TRUE;

    return;
}

void MDrv_MobileBroadband_Modem_Unthrottle(MS_U32 u32Port)
{
    struct usb_serial_port *port = NULL;
    MS_BOOL was_throttled = FALSE;

    if(!_MobileBroadband_Modem_InfoValid(u32Port))
    {
        return;
    }
    port = usb_modem_port[u32Port];
    /* Clear the throttle flags */
    was_throttled = port->throttled;
    port->throttled = port->throttle_req = FALSE;

    if(was_throttled)
    {
        /* Resume reading from device */
		if(port->read_urb)
        {
            ms_usb_stuff_bulk_urb(port->read_urb,
                                  port->pusb_dev,
                                  port->bulk_recv_pipe,
                                  port->bulk_in_buffer,
                                  port->bulk_in_size,
                                  _MobileBroadband_Modem_ReadBulk_Cb,
                                  (void *)port);
            int retval = ms_usb_submit_urb(port->read_urb, GFP_KERNEL);
            if(retval < ENOERR)
            {
                USB_MODEM_ERR("[%s] submitting bulk read urb, error %d\n", __FUNCTION__, retval);
            }
        }
    }
}

void MDrv_MobileBroadband_Modem_Debug_Data(MS_U8 *pu8Data, MS_U16 u16Actual_length)
{
    //print receive characters
    USB_MODEM_INFO("\n-----------------------------------------\n");
    USB_MODEM_INFO("%s : length = %u, data(Hex) = ", __FUNCTION__, u16Actual_length);
    int i = 0;
    for(i = 0; i < u16Actual_length; ++i)
        USB_MODEM_INFO("%2X ", pu8Data[i]);
    USB_MODEM_INFO("\n-----------------------------------------\n\n");
}

MS_U32 MDrv_MobileBroadband_Modem_GetVidPid(void)
{
    if(vid_pid<0)
    diag_printf("Invaild Vid Pid Pair\n");

    return vid_pid;
}


#endif
#endif
#endif
