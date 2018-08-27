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

#if defined (MSOS_TYPE_ECOS)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <cyg/io/devtab.h>
#include <cyg/io/serial.h>
#include <cyg/io/serialio.h>

#include "HbCommon.h"
#include "drvMobileBroadband_Modem.h"

//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// API prototype
//-------------------------------------------------------------------------------------------------
static bool usb_modem_init(struct cyg_devtab_entry *tab);
static Cyg_ErrNo usb_modem_lookup(struct cyg_devtab_entry **tab, struct cyg_devtab_entry *sub_tab, const char *name);
static bool usb_modem_putc(serial_channel *chan, unsigned char c);
static unsigned char usb_modem_getc(serial_channel *chan);
static Cyg_ErrNo usb_modem_set_config(serial_channel *chan, cyg_uint32 key, const void *xbuf, cyg_uint32 *len);
static void usb_modem_start_xmit(serial_channel *chan);
static void usb_modem_stop_xmit(serial_channel *chan);

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------

// Buffer for incoming USB bulk data. The local USB driver can probably split
// packets, but just in case, making this the page size of the host might be
// helpful.
#define USBMODEM_DEVNAME           "/dev/usbs"
#define USBMODEM_DEVNAME_SIZE      9
#define USBMODEM_DEVNAME_MAXSIZE   12

#define USB_MODEM_INSTANCE(_number_,_port_,_name_)                         \
    static usb_serial_info_t usb_serial_info##_port_##_number_ = {          \
        MAX_NUM_OF_MODEM,                                                    \
        MAX_NUM_OF_MODEM                                                     \
    };                                                                      \
    SERIAL_CHANNEL_USING_INTERRUPTS(usb_serial_channel##_port_##_number_,   \
    usb_modem_funs,                                                        \
    usb_serial_info##_port_##_number_,                                      \
    CYG_SERIAL_BAUD_DEFAULT,                                                \
    CYG_SERIAL_STOP_DEFAULT,                                                \
    CYG_SERIAL_PARITY_DEFAULT,                                              \
    CYG_SERIAL_WORD_LENGTH_DEFAULT,                                         \
    CYG_SERIAL_FLAGS_DEFAULT,                                               \
    u8USBModemOutBuf,                                                      \
    USBMODEM_BUFSIZE,                                                      \
    u8USBModemInBuf,                                                       \
    USBMODEM_BUFSIZE                                                       \
    );                                                                      \
    DEVTAB_ENTRY(usb_serial_io##_port_##_number_,                           \
    _name_,                                                                 \
    NULL,                                                                   \
    &cyg_io_serial_devio,                                                   \
    &usb_modem_init,                                                       \
    &usb_modem_lookup,                                                     \
    &usb_serial_channel##_port_##_number_                                   \
    );

//-------------------------------------------------------------------------------------------------
// Static Variable
//-------------------------------------------------------------------------------------------------
static MS_S32 s32XmitMutex = -1;
static MS_U8 u8USBModemOutBuf[USBMODEM_BUFSIZE] = {0};
static MS_U8 u8USBModemInBuf[USBMODEM_BUFSIZE] = {0};
static SERIAL_FUNS(usb_modem_funs,
                   usb_modem_putc,
                   usb_modem_getc,
                   usb_modem_set_config,
                   usb_modem_start_xmit,
                   usb_modem_stop_xmit
    );

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
USB_MODEM_INSTANCE(0, 0, "/dev/usbs0");

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

// Function to initialize the device.  Called at bootstrap time.
static bool
usb_modem_init(struct cyg_devtab_entry *tab)
{
    if(s32XmitMutex < 0)
    {
        s32XmitMutex = MsOS_CreateMutex(E_MSOS_FIFO, "USBModem_Mutex",MSOS_PROCESS_SHARED);
        if(s32XmitMutex < 0)
        {
            HB_GEN_EXCEP;
        }
    }

    return true;
}

// This routine is called when the device is "looked" up (i.e. attached)
static Cyg_ErrNo
usb_modem_lookup(struct cyg_devtab_entry **tab,
                  struct cyg_devtab_entry  *sub_tab,
                  const char               *name)
{
    return ENOERR;
}

// Send a character to the device output buffer.
// Return 'true' if character is sent to device
static bool
usb_modem_putc(serial_channel *chan, unsigned char c)
{
    MS_U32 u32Port = 0;
    MS_U16 u16Sent = 0;
    int ret = ENOERR;

    if(chan && chan->dev_priv)
    {
        u32Port = ((usb_serial_info_t *)chan->dev_priv)->u32Port;
        if(u32Port == MAX_NUM_OF_MODEM)
        {
            return false;
        }
        ret = MDrv_MobileBroadband_Modem_Write(u32Port, &c, 1, &u16Sent);
    }

    if(ret < ENOERR)
    {
        return false;
    }

    return true;
}

// Fetch a character from the device input buffer, waiting if necessary
static unsigned char
usb_modem_getc(serial_channel *chan)
{
    //only implement in Non interrupt driven (i.e. polled) mode
    return (unsigned char)NULL;
}

// Set up the device characteristics; baud rate, etc.
static Cyg_ErrNo
usb_modem_set_config(serial_channel *chan, cyg_uint32 key,
                      const void *xbuf, cyg_uint32 *len)
{
    MS_U32 u32Port = 0;
    int ret = ENOERR;

    if(chan && chan->dev_priv)
    {
        u32Port = ((usb_serial_info_t *)chan->dev_priv)->u32Port;
        if(u32Port == MAX_NUM_OF_MODEM)
        {
            return -ENODEV;
        }

        if(key == CYG_IO_SET_CONFIG_SERIAL_INFO)
        {
            cyg_serial_info_t serial_info;
            if(*len >= sizeof(serial_info))
            {
                memcpy(&serial_info,  (cyg_serial_info_t *)xbuf, sizeof(serial_info));
                ret = MDrv_MobileBroadband_Modem_SetTermios(u32Port, &serial_info);
            }
            else
            {
                return -EINVAL;
            }
        }
        else if(key == CYG_IO_SET_CONFIG_SERIAL_HW_RX_FLOW_THROTTLE)
        {
            MS_U32 u32throttle = 0;
            if(*len == sizeof(MS_U32))
            {
                memcpy(&u32throttle,  xbuf, *len);
                if(u32throttle)
                {
                    MDrv_MobileBroadband_Modem_Throttle(u32Port);
                }
                else
                {
                    MDrv_MobileBroadband_Modem_Unthrottle(u32Port);
                }
            }
            else
            {
                return -EINVAL;
            }
        }
        else if(key == CYG_IO_SET_CONFIG_SERIAL_INPUT_FLUSH)
        {
            //Flush buffered input(u8USBSerialInBuf)
            //do nothing
        }
    }

    if(ret >= ENOERR)
    {
        ret = ENOERR;
    }

    return ret;
}

// Enable the transmitter on the device
static void
usb_modem_start_xmit(serial_channel *chan)
{
    MsOS_ObtainMutex(s32XmitMutex, MSOS_WAIT_FOREVER);
    int ret = ENOERR;
    MS_U32 u32Port = 0;
    MS_U16 u16BufferLength = 0, u16ActualLength = 0;
    MS_U32 u32CharsAvail = 0;
    MS_U8* u8Chars;

    if(chan && chan->dev_priv)
    {
        u32Port = ((usb_serial_info_t *)chan->dev_priv)->u32Port;
        if(u32Port == MAX_NUM_OF_MODEM)
        {
            MsOS_ReleaseMutex(s32XmitMutex);
            return;
        }

        ret = (chan->callbacks->data_xmt_req)(chan, USBMODEM_BUFSIZE, (int*)&u32CharsAvail, (unsigned char**)&u8Chars);
        if(ret != CYG_XMT_OK)
        {
            MsOS_ReleaseMutex(s32XmitMutex);
            return;
        }

        u16BufferLength = (MS_U16)u32CharsAvail;
        ret = MDrv_MobileBroadband_Modem_Write(u32Port, u8Chars, u16BufferLength, &u16ActualLength);
        if(ret < ENOERR)
        {
            u16ActualLength = 0;
        }

        (chan->callbacks->data_xmt_done)(chan, (int)u16ActualLength);
    }

    MDrv_MobileBroadband_Modem_Debug_Data(u8Chars, u16ActualLength);
    MsOS_ReleaseMutex(s32XmitMutex);
    return;
}

// Disable the transmitter on the device
static void
usb_modem_stop_xmit(serial_channel *chan)
{
    return;
}

void usb_modem_instance_disconnect(MS_U32 u32Port)
{
    serial_channel      *chan = NULL;
    usb_serial_info_t   *p_usb_serial_info = NULL;
    cyg_devtab_entry_t  *dev_h = NULL, *t = NULL;
    MS_U8               u8DevtabName[USBMODEM_DEVNAME_MAXSIZE] = {0};

    for(t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
    {
        if(strncmp(t->name, USBMODEM_DEVNAME, USBMODEM_DEVNAME_SIZE) != 0)
            continue;
        chan = (serial_channel *) t->priv;
        if(chan == NULL)
            continue;
        p_usb_serial_info = (usb_serial_info_t *) chan->dev_priv;
        if (p_usb_serial_info == NULL)
            continue;

        if((p_usb_serial_info->u32DevId == u32Port) && (p_usb_serial_info->u32Port == u32Port))
        {
            dev_h = t;
            diag_printf("Disconnect USB modem: %s, Port: %u, port device id: %u\n", dev_h->name, p_usb_serial_info->u32Port, p_usb_serial_info->u32DevId);
            break;
        }
    }

    if(t == &__DEVTAB_END__ && dev_h == NULL)
    {
        diag_printf("[%d][%s] Can't find match Port(%d)\n", __LINE__, __FUNCTION__, u32Port);
        return;
    }

    if(usb_modem_CbFun != NULL)
    {
        if(strlen(dev_h->name) < USBMODEM_DEVNAME_MAXSIZE)
        {
            strcpy((char *)u8DevtabName, dev_h->name);
        }
        usb_modem_CbFun(USB_MODEM_PLUG_OUT, u8DevtabName);//invoke ppp down first if it is up!!!
    }

    //reset serial_channel private data(usb_serial_info_t)
    p_usb_serial_info->u32DevId = p_usb_serial_info->u32Port = MAX_NUM_OF_MODEM;

    //reset serial_channel
    memset(&chan->config, 0, sizeof(cyg_serial_info_t));
    memset(&chan->flow_desc, 0, sizeof(flow_desc_t));
    chan->in_cbuf.put = chan->in_cbuf.get = chan->in_cbuf.nb = 0;
    chan->in_cbuf.waiting = chan->in_cbuf.block_mode_xfer_running = 0;
    chan->in_cbuf.blocking = 1;
    chan->out_cbuf.put = chan->out_cbuf.get = chan->out_cbuf.nb = 0;
    chan->out_cbuf.waiting = chan->out_cbuf.block_mode_xfer_running = 0;
    chan->out_cbuf.blocking = 1;
    chan->status_callback = NULL;

}

bool usb_modem_instance_connect(MS_U32 u32Port)
{
    serial_channel      *chan = NULL;
    usb_serial_info_t   *p_usb_serial_info = NULL;
    cyg_devtab_entry_t  *dev_h = NULL, *t = NULL;
    MS_U8               u8DevtabName[USBMODEM_DEVNAME_MAXSIZE] = {0};

    for(t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
    {
        if(strncmp(t->name, USBMODEM_DEVNAME, USBMODEM_DEVNAME_SIZE) != 0)
            continue;
        chan = (serial_channel *) t->priv;
        if(chan == NULL)
            continue;
        p_usb_serial_info = (usb_serial_info_t *) chan->dev_priv;
        if (p_usb_serial_info == NULL)
            continue;
        if(p_usb_serial_info->u32Port == u32Port)
        {
            diag_printf("Warning : find one previous device here !!\n");
            usb_modem_instance_disconnect(u32Port);
        }
    }

    for(t = &__DEVTAB__[0]; t != &__DEVTAB_END__; t++)
    {
        if(strncmp(t->name, USBMODEM_DEVNAME, USBMODEM_DEVNAME_SIZE) != 0)
            continue;
        chan = (serial_channel *) t->priv;
        if(chan == NULL)
            continue;
        p_usb_serial_info = (usb_serial_info_t *) chan->dev_priv;
        if (p_usb_serial_info == NULL)
            continue;

        if(p_usb_serial_info->u32Port == MAX_NUM_OF_MODEM)
        {
            //found an empty serial device node!!!!
            dev_h = t;
            p_usb_serial_info->u32Port = u32Port;
            p_usb_serial_info->u32DevId = u32Port;
            diag_printf("Connect USB modem: %s, Port: %u, port device id: %u\n", dev_h->name, p_usb_serial_info->u32Port, p_usb_serial_info->u32DevId);
            break;
        }
    }

    if(t == &__DEVTAB_END__ && dev_h == NULL)
    {
        diag_printf("Error! Can't find available usb serail name. Exceed maximum support number!\n");
        return FALSE;
    }

    (chan->callbacks->serial_init)(chan);

    //set termio, send DTR/RTS, submit read/interrupt URB
    MDrv_MobileBroadband_Modem_Open(u32Port, (void*)dev_h);

    //invoke AP CB function
    if(usb_modem_CbFun != NULL)
    {
        if (strlen(dev_h->name) < USBMODEM_DEVNAME_MAXSIZE)
        {
            strcpy((char *)u8DevtabName, dev_h->name);
            usb_modem_CbFun(USB_MODEM_PLUG_IN, u8DevtabName);
        }
    }
    return TRUE;
}

#endif
