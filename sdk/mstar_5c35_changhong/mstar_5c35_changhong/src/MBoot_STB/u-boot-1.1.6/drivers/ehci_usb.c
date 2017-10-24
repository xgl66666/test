/*
 * (C) Copyright 2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * This code is based on linux driver for sl811hs chip, source at
 * drivers/usb/host/sl811.c:
 *
 * SL811 Host Controller Interface driver for USB.
 *
 * Copyright (c) 2003/06, Courage Co., Ltd.
 *
 * Based on:
 *	1.uhci.c by Linus Torvalds, Johannes Erdfelt, Randy Dunlap,
 *	  Georg Acher, Deti Fliegl, Thomas Sailer, Roman Weissgaerber,
 *	  Adam Richter, Gregory P. Smith;
 *	2.Original SL811 driver (hc_sl811.o) by Pei Liu <pbl@cypress.com>
 *	3.Rewrited as sl811.o by Yin Aihua <yinah:couragetech.com.cn>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#ifdef  CONFIG_USB_EHCI
#include <usb.h>
#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)
#include <asm-aeon/io.h>
#else
#include <asm-mips/io.h>
#endif
#include "ehci_usb.h"

#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)
    #define HW_BASE         0xa0200000
#else
    #define HW_BASE         0xbf200000
#endif

static int root_hub_devnum = 0;
static struct usb_port_status rh_status = { 0 };/* root hub port status */

#ifdef SL811_DEBUG
	#define PDEBUG(level, fmt, args...) \
		if (debug >= (level)) printf("[%s:%d] " fmt, \
		__PRETTY_FUNCTION__, __LINE__ , ## args)
#else
	#define PDEBUG(level, fmt, args...) do {} while(0)
#endif

#if defined(CONFIG_KAPPA)||defined(CONFIG_KRITI)
#define CPU_CLK_FREQ	CPU_CLOCK
void DelayMS(unsigned int msec)
{
	unsigned int tmo = (unsigned int)msec * (CPU_CLK_FREQ / 2 / 1000);
	unsigned int count;

	asm volatile ("mtc0 $0, $11" : :); // clear cp0 compare
	asm volatile ("mtc0 $0, $9" : :);  // clear cp0

	do
	{
		asm volatile ("mfc0 %0, $9" : "=r" (count) :);
	} while(count < tmo);
}
#endif

#if 0
static int titania_rh_submit_urb(struct usb_device *usb_dev, unsigned long pipe,
			       void *data, int buf_len, struct devrequest *cmd);
#endif
extern int UsbPortSelect;

#if defined(CONFIG_KAISERIN)
extern U32 MsOS_VA2PA(U32 addr);
extern U32 MsOS_PA2KSEG1(U32 addr);

U32 VA2PA(U32 u32_DMAAddr)
{       
	//printf("VA:0x%x -->", u32_DMAAddr); 

	u32_DMAAddr = MsOS_VA2PA(u32_DMAAddr);
	if(u32_DMAAddr < MsOS_VA2PA(MIU1_BUS_BASE_ADDR))
	{
		u32_DMAAddr -= MsOS_VA2PA(MIU0_BUS_BASE_ADDR);
		u32_DMAAddr += MIU0_PHY_BASE_ADDR;
	}
	else 
	{
		u32_DMAAddr -= MsOS_VA2PA(MIU1_BUS_BASE_ADDR);
		u32_DMAAddr |= MIU1_PHY_BASE_ADDR;
	}    

	//printf("PA:0x%x \n", u32_DMAAddr); 
	return  u32_DMAAddr;     
}

U32 PA2VA(U32 u32_DMAAddr)
{
	//printf("PA:0x%x -->", u32_DMAAddr);

	if(u32_DMAAddr < MIU1_PHY_BASE_ADDR)
	{
		u32_DMAAddr -= MIU0_PHY_BASE_ADDR;
		u32_DMAAddr += MsOS_VA2PA(MIU0_BUS_BASE_ADDR);
	}
	else 
	{
		u32_DMAAddr -= MIU1_PHY_BASE_ADDR;
		u32_DMAAddr |= MsOS_VA2PA(MIU1_BUS_BASE_ADDR);
	}    

	u32_DMAAddr=MsOS_PA2KSEG1(u32_DMAAddr);
    
	//printf("VA:0x%x \n", u32_DMAAddr);
	return  u32_DMAAddr;      
}
#endif

static void TV_usb_init(void)
{
//===========================================================================
	writeb(0x0A, (void*) (HW_BASE+0x700*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (HW_BASE+0x700*2)); // Release UHC reset, enable UHC XIU function

	writeb(readb((void*)HW_BASE+(0x3A80+0x3C)*2) | 0x01, (void*) (HW_BASE+(0x3A80+0x3C)*2)); // set CA_START as 1
	mdelay(10);

	writeb(readb((void*)HW_BASE+(0x3A80+0x3C)*2) & ~0x01, (void*) (HW_BASE+(0x3A80+0x3C)*2)); // release CA_START

	while ((readb((void*)HW_BASE+(0x3A80+0x3C)*2) & 0x02) == 0);	// polling bit <1> (CA_END)

    #if defined(CONFIG_KRONUS)
    writeb(readb((void*)(HW_BASE+(0x3A80+0x0b)*2-1) ) |0x20, (void*) (HW_BASE+(0x3A80+0x0b)*2-1)); // dp dm swap
    #endif


	writeb(readb((void*)HW_BASE+(0x700+0x02)*2) & ~0x03, (void*) (HW_BASE+(0x700+0x02)*2)); //UHC select enable
	writeb(readb((void*)HW_BASE+(0x700+0x02)*2) | 0x01, (void*) (HW_BASE+(0x700+0x02)*2)); //UHC select enable

	writeb(readb((void*)HW_BASE+(0x2400+0x40)*2) & ~0x10, (void*) (HW_BASE+(0x2400+0x40)*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms
	writeb(readb((void*)HW_BASE+(0x2400+0x40)*2) | 0x08, (void*) (HW_BASE+(0x2400+0x40)*2)); // Active HIGH
	mdelay(1);	// delay 1ms

	writeb((readb((void*)HW_BASE+(0x3a80+0x06)*2) & 0x9F) | 0x40, (void*) (HW_BASE+(0x3a80+0x06)*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)HW_BASE+(0x3a80+0x03)*2-1) | 0x28, (void*) (HW_BASE+(0x3a80+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)HW_BASE+(0x3a80+0x03)*2-1) & 0xef, (void*) (HW_BASE+(0x3a80+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)HW_BASE+(0x3a80+0x07)*2-1) & 0xfd, (void*) (HW_BASE+(0x3a80+0x07)*2-1)); //Disable improved CDR
    
#if defined(CONFIG_KAPPA) || defined(CONFIG_KRITI) ||\
	defined(CONFIG_KELTIC) || defined(CONFIG_KERES)
	writeb(readb((void*)HW_BASE+(0x3a80+0x08)*2) | 0x08, (void*) (HW_BASE+(0x3a80+0x08)*2)); // bit<3> for 240's phase as 120's clock set 1, bit<4> for 240Mhz in mac 0 for faraday 1 for etron
#endif
	writeb(readb((void*)HW_BASE+(0x3a80+0x09)*2-1) |0x81, (void*) (HW_BASE+(0x3a80+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)HW_BASE+(0x3a80+0x0b)*2-1) |0x80, (void*) (HW_BASE+(0x3a80+0x0b)*2-1)); // TX timing select latch path
	writeb(readb((void*)HW_BASE+(0x3a80+0x15)*2-1) |0x20, (void*) (HW_BASE+(0x3a80+0x15)*2-1)); // Chirp signal source select
#if defined(CONFIG_KAPPA) ||defined(CONFIG_KRITI) || \
	defined(CONFIG_KELTIC) || defined(CONFIG_KERES)
	writeb(readb((void*)HW_BASE+(0x3a80+0x15)*2-1) |0x40, (void*) (HW_BASE+(0x3a80+0x15)*2-1)); // change to 55 interface
#endif
	writeb(readb((void*)HW_BASE+(0x3a80+0x2c)*2) |0x10, (void*) (HW_BASE+(0x3a80+0x2c)*2));
	writeb(readb((void*)HW_BASE+(0x3a80+0x2d)*2-1) |0x02, (void*) (HW_BASE+(0x3a80+0x2d)*2-1));
	writeb(readb((void*)HW_BASE+(0x3a80+0x2f)*2-1) |0x81, (void*) (HW_BASE+(0x3a80+0x2f)*2-1));

//Disable battery charge mode
#if defined(CONFIG_KAPPA) ||defined(CONFIG_KRITI) || \
	defined(CONFIG_KELTIC) || defined(CONFIG_KERES)

#if defined(CONFIG_KAPPA)||defined(CONFIG_KRITI)
	/* kappa 0x12_0500 (byte address) */
	#define USBBC_BANK	0x20500
#elif defined(CONFIG_KELTIC) || defined(CONFIG_KERES)
	/* keltic 0x11_1700 (byte address) */
	#define USBBC_BANK	0x11700
#else
	#error [USB] Undefined USBBC base
#endif
	writeb(readb((void*)HW_BASE+(USBBC_BANK+0xc)*2) &~0x40, (void*) (HW_BASE+(USBBC_BANK+0xc)*2));
	writeb(readb((void*)HW_BASE+(USBBC_BANK+0x3)*2-1) &~0x40, (void*) (HW_BASE+(USBBC_BANK+0x3)*2-1));
	writeb(readb((void*)HW_BASE+(0x3a80+0x1)*2-1) &~0x40, (void*) (HW_BASE+(0x3a80+0x1)*2-1));
	#ifdef USB20_BC_MANUAL_TEST
	printf("Port 0 BC test, spin forever, please using TV_tool to do bus reset and enable UHC\n");
	while(1);
	#endif
#endif

// Enable miu new bridge ECO
#if defined(CONFIG_KAPPA) ||defined(CONFIG_KRITI) || \
	defined(CONFIG_KELTIC) || defined(CONFIG_KERES)
	writeb(readb((void*)HW_BASE+(0x700+0x0a)*2) | 0x40, (void*) (HW_BASE+(0x700+0x0a)*2)); //fix pv2mi bridge mis-behavior
#endif
//===========================================================================
}
#if defined(ENABLE_SECOND_EHC)
void TV_usb_init_port1(void)
{
#if defined(CONFIG_KRONUS)
    #define UTMI1_BASE  0x2A00
    #define UHC1_BASE   0x10300
    #define USBC1_BASE  0x10200
#else
    #define UTMI1_BASE  0x3A00
    #define UHC1_BASE   0xD00
    #define USBC1_BASE  0x780
#endif

#if defined(CONFIG_TITANIA4) || defined(CONFIG_TITANIA12) || defined(CONFIG_JANUS) || defined(CONFIG_TITANIA8) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_MARIA10) || defined(CONFIG_URANUS4) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN)
	writeb(0x0A, (void*) (HW_BASE+USBC1_BASE*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (HW_BASE+USBC1_BASE*2)); // Release UHC reset, enable UHC XIU function

	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x3C)*2) | 0x01, (void*) (HW_BASE+(UTMI1_BASE+0x3C)*2)); // set CA_START as 1
	mdelay(10);

	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x3C)*2) & ~0x01, (void*) (HW_BASE+(UTMI1_BASE+0x3C)*2)); // release CA_START

	while ((readb((void*)HW_BASE+(UTMI1_BASE+0x3C)*2) & 0x02) == 0);	// polling bit <1> (CA_END)

#if defined(CONFIG_KRONUS)
    writeb(readb((void*)(HW_BASE+(UTMI1_BASE+0x0b)*2-1) ) |0x20, (void*) (HW_BASE+(UTMI1_BASE+0x0b)*2-1)); // dp dm swap
#endif

	writeb(readb((void*)HW_BASE+(USBC1_BASE+0x02)*2) & ~0x03, (void*) (HW_BASE+(USBC1_BASE+0x02)*2)); //UHC select enable
	writeb(readb((void*)HW_BASE+(USBC1_BASE+0x02)*2) | 0x01, (void*) (HW_BASE+(USBC1_BASE+0x02)*2)); //UHC select enable

	writeb(readb((void*)HW_BASE+(UHC1_BASE+0x40)*2) & ~0x10, (void*) (HW_BASE+(UHC1_BASE+0x40)*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms

	writeb(readb((void*)HW_BASE+(UHC1_BASE+0x40)*2) | 0x08, (void*) (HW_BASE+(UHC1_BASE+0x40)*2)); // Active HIGH
	mdelay(1);	// delay 1ms
	writeb((readb((void*)HW_BASE+(UTMI1_BASE+0x06)*2) & 0x9F) | 0x40, (void*) (HW_BASE+(UTMI1_BASE+0x06)*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x03)*2-1) | 0x28, (void*) (HW_BASE+(UTMI1_BASE+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x03)*2-1) & 0xef, (void*) (HW_BASE+(UTMI1_BASE+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x07)*2-1) & 0xfd, (void*) (HW_BASE+(UTMI1_BASE+0x07)*2-1)); //Disable improved CDR
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x09)*2-1) |0x81, (void*) (HW_BASE+(UTMI1_BASE+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x0b)*2-1) |0x80, (void*) (HW_BASE+(UTMI1_BASE+0x0b)*2-1)); // TX timing select latch path
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x15)*2-1) |0x20, (void*) (HW_BASE+(UTMI1_BASE+0x15)*2-1)); // Chirp signal source select

	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x2c)*2) |0x10, (void*) (HW_BASE+(UTMI1_BASE+0x2c)*2));
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x2d)*2-1) |0x02, (void*) (HW_BASE+(UTMI1_BASE+0x2d)*2-1));
	writeb(readb((void*)HW_BASE+(UTMI1_BASE+0x2f)*2-1) |0x81, (void*) (HW_BASE+(UTMI1_BASE+0x2f)*2-1));
#endif
//===========================================================================

}
#endif

#if defined(ENABLE_THIRD_EHC)
void TV_usb_init_port2(void)
{
#if defined(CONFIG_URANUS4) || defined(CONFIG_KAISERIN)
    #define UTMI2_BASE  0x2A00
    #define UHC2_BASE   0x10300
    #define USBC2_BASE  0x10200
#elif defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2)
    #define UTMI2_BASE  0x3900
    #define UHC2_BASE   0x13900
    #define USBC2_BASE  0x13800
#endif


    writeb(0x0A, (void*)(HW_BASE+USBC2_BASE*2)); // Disable MAC initial suspend, Reset UHC
    writeb(0x28, (void*)(HW_BASE+USBC2_BASE*2)); // Release UHC reset, enable UHC XIU function

    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x3C)*2) ) | 0x01, (void*) (HW_BASE+(UTMI2_BASE+0x3C)*2)); // set CA_START as 1
    mdelay(10);

    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x3C)*2) ) & ~0x01, (void*) (HW_BASE+(UTMI2_BASE+0x3C)*2)); // release CA_START

    while ((readb((void*)(HW_BASE+(UTMI2_BASE+0x3C)*2) ) & 0x02) == 0);	// polling bit <1> (CA_END)

#if defined(CONFIG_URANUS4) && ((MS_BOARD_TYPE_SEL != BD_MST098B_D01A) || (MS_BOARD_TYPE_SEL != BD_MST098B_D01B) || (MS_BOARD_TYPE_SEL != BD_MST128A_D01A))
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x0b)*2-1) ) |0x20, (void*) (HW_BASE+(UTMI2_BASE+0x0b)*2-1)); // dp dm swap
#endif

    writeb(readb((void*)(HW_BASE+(USBC2_BASE+0x02)*2) ) & ~0x03, (void*) (HW_BASE+(USBC2_BASE+0x02)*2)); //UHC select enable
    writeb(readb((void*)(HW_BASE+(USBC2_BASE+0x02)*2) ) | 0x01, (void*) (HW_BASE+(USBC2_BASE+0x02)*2)); //UHC select enable

    writeb(readb((void*)(HW_BASE+(UHC2_BASE+0x40)*2) ) & ~0x10, (void*) (HW_BASE+(UHC2_BASE+0x40)*2)); //0: VBUS On.
    mdelay(1);	// delay 1ms

    writeb(readb((void*)(HW_BASE+(UHC2_BASE+0x40)*2) ) | 0x08, (void*) (HW_BASE+(UHC2_BASE+0x40)*2)); // Active HIGH
    mdelay(1);	// delay 1ms
	writeb((readb((void*)(HW_BASE+(UTMI2_BASE+0x06)*2) ) & 0x9F) | 0x40, (void*) (HW_BASE+(UTMI2_BASE+0x06)*2)); //reg_tx_force_hs_current_enable

    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x03)*2-1) ) | 0x28, (void*) (HW_BASE+(UTMI2_BASE+0x03)*2-1)); //Disconnect window select
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x03)*2-1) ) & 0xef, (void*) (HW_BASE+(UTMI2_BASE+0x03)*2-1)); //Disconnect window select
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x07)*2-1) ) & 0xfd, (void*) (HW_BASE+(UTMI2_BASE+0x07)*2-1)); //Disable improved CDR
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x09)*2-1) ) |0x81, (void*) (HW_BASE+(UTMI2_BASE+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x0b)*2-1) ) |0x80, (void*) (HW_BASE+(UTMI2_BASE+0x0b)*2-1)); // TX timing select latch path
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x15)*2-1) ) |0x20, (void*) (HW_BASE+(UTMI2_BASE+0x15)*2-1)); // Chirp signal source select

    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x2c)*2) ) |0x10, (void*) (HW_BASE+(UTMI2_BASE+0x2c)*2));
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x2d)*2-1) ) |0x02, (void*) (HW_BASE+(UTMI2_BASE+0x2d)*2-1));
    writeb(readb((void*)(HW_BASE+(UTMI2_BASE+0x2f)*2-1) ) |0x81, (void*) (HW_BASE+(UTMI2_BASE+0x2f)*2-1));


}
#endif

#if defined(ENABLE_FOURTH_EHC)
void TV_usb_init_port3(void)
{
    #define UTMI3_BASE  0x20A00
    #define UHC3_BASE   0x20900
    #define USBC3_BASE  0x20800

    writeb(0x0A, (void*)(HW_BASE+USBC3_BASE*2)); // Disable MAC initial suspend, Reset UHC
    writeb(0x28, (void*)(HW_BASE+USBC3_BASE*2)); // Release UHC reset, enable UHC XIU function

    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x3C)*2) ) | 0x01, (void*) (HW_BASE+(UTMI3_BASE+0x3C)*2)); // set CA_START as 1
    mdelay(10);

    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x3C)*2) ) & ~0x01, (void*) (HW_BASE+(UTMI3_BASE+0x3C)*2)); // release CA_START

    while ((readb((void*)(HW_BASE+(UTMI3_BASE+0x3C)*2) ) & 0x02) == 0);	// polling bit <1> (CA_END)

#if 0 //defined(CONFIG_KARSERIN) 
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x0b)*2-1) ) |0x20, (void*) (HW_BASE+(UTMI3_BASE+0x0b)*2-1)); // dp dm swap
#endif

    writeb(readb((void*)(HW_BASE+(USBC3_BASE+0x02)*2) ) & ~0x03, (void*) (HW_BASE+(USBC3_BASE+0x02)*2)); //UHC select enable
    writeb(readb((void*)(HW_BASE+(USBC3_BASE+0x02)*2) ) | 0x01, (void*) (HW_BASE+(USBC3_BASE+0x02)*2)); //UHC select enable

    writeb(readb((void*)(HW_BASE+(UHC3_BASE+0x40)*2) ) & ~0x10, (void*) (HW_BASE+(UHC3_BASE+0x40)*2)); //0: VBUS On.
    mdelay(1);	// delay 1ms

    writeb(readb((void*)(HW_BASE+(UHC3_BASE+0x40)*2) ) | 0x08, (void*) (HW_BASE+(UHC3_BASE+0x40)*2)); // Active HIGH
    mdelay(1);	// delay 1ms
	writeb((readb((void*)(HW_BASE+(UTMI3_BASE+0x06)*2) ) & 0x9F) | 0x40, (void*) (HW_BASE+(UTMI3_BASE+0x06)*2)); //reg_tx_force_hs_current_enable

    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x03)*2-1) ) | 0x28, (void*) (HW_BASE+(UTMI3_BASE+0x03)*2-1)); //Disconnect window select
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x03)*2-1) ) & 0xef, (void*) (HW_BASE+(UTMI3_BASE+0x03)*2-1)); //Disconnect window select
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x07)*2-1) ) & 0xfd, (void*) (HW_BASE+(UTMI3_BASE+0x07)*2-1)); //Disable improved CDR
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x09)*2-1) ) |0x81, (void*) (HW_BASE+(UTMI3_BASE+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x0b)*2-1) ) |0x80, (void*) (HW_BASE+(UTMI3_BASE+0x0b)*2-1)); // TX timing select latch path
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x15)*2-1) ) |0x20, (void*) (HW_BASE+(UTMI3_BASE+0x15)*2-1)); // Chirp signal source select

    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x2c)*2) ) |0x10, (void*) (HW_BASE+(UTMI3_BASE+0x2c)*2));
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x2d)*2-1) ) |0x02, (void*) (HW_BASE+(UTMI3_BASE+0x2d)*2-1));
    writeb(readb((void*)(HW_BASE+(UTMI3_BASE+0x2f)*2-1) ) |0x81, (void*) (HW_BASE+(UTMI3_BASE+0x2f)*2-1));


}
#endif

extern int Usb_host_Init(void);
#if defined(ENABLE_SECOND_EHC)
extern int Usb_host_Init2(void);
#endif
#if defined(ENABLE_THIRD_EHC)
extern int Usb_host_Init3(void);
#endif
#if defined(ENABLE_FOURTH_EHC)
extern int Usb_host_Init4(void);
#endif
int usb_lowlevel_init(void)
{
    int speed=-1;
    int ret = 0;
	root_hub_devnum = 0;
    if (UsbPortSelect==0)
    {
        TV_usb_init();
        speed=Usb_host_Init();
    }
#if defined(ENABLE_SECOND_EHC)
    else if (UsbPortSelect==1)
    {
        TV_usb_init_port1();
        speed=Usb_host_Init2();
    }
#endif	
#if defined(ENABLE_THIRD_EHC)
    else if (UsbPortSelect==2)
    {
        TV_usb_init_port2();
        speed=Usb_host_Init3();
    }
#endif
#if defined(ENABLE_FOURTH_EHC)
    else if (UsbPortSelect==3)
    {
        TV_usb_init_port3();
        speed=Usb_host_Init4();
    }
#endif

#if 1
    if (speed==1)
    {
        rh_status.wPortStatus |= USB_PORT_STAT_CONNECTION | USB_PORT_STAT_LOW_SPEED;
    }
    else
    {
    	rh_status.wPortStatus |= USB_PORT_STAT_CONNECTION;
        rh_status.wPortStatus &= ~USB_PORT_STAT_LOW_SPEED;
        if(speed==-1)
           ret =-1;
    }
    rh_status.wPortChange |= USB_PORT_STAT_C_CONNECTION;
#endif
	return ret;
}

extern void MDrv_UsbClose(void);
#if defined(ENABLE_SECOND_EHC)
extern void MDrv_UsbClose2(void);
#endif
#if defined(ENABLE_THIRD_EHC)
extern void MDrv_UsbClose3(void);
#endif
#if defined(ENABLE_FOURTH_EHC)
extern void MDrv_UsbClose4(void);
#endif
#ifdef ENABLE_USB_LAN_MODULE
extern void usb_lan_release(void);
#endif
int usb_lowlevel_stop(int p)
{
    if (p==0xff)                 //use UsbPortSelect
    {
        p=UsbPortSelect;
    }

#ifdef ENABLE_USB_LAN_MODULE
    usb_lan_release();
#endif

    if (p==0)
    {
        MDrv_UsbClose();
    }
#if defined(ENABLE_SECOND_EHC)	
    else if (p==1)
    {
        MDrv_UsbClose2();
    }
#endif
#if defined(ENABLE_THIRD_EHC)
    else if (p==2)
    {
        MDrv_UsbClose3();
    }
#endif
#if defined(ENABLE_FOURTH_EHC)
       else if (p==3)
       {
           MDrv_UsbClose4();
       }
#endif
   return 0;
}

extern unsigned char Send_Receive_Bulk_Data(void *buffer,int len,int dir_out);
#if defined(ENABLE_SECOND_EHC)
extern unsigned char Send_Receive_Bulk_Data2(void *buffer,int len,int dir_out);
#endif
#if defined(ENABLE_THIRD_EHC)
extern unsigned char Send_Receive_Bulk_Data3(void *buffer,int len,int dir_out);
#endif
#if defined(ENABLE_FOURTH_EHC)
extern unsigned char Send_Receive_Bulk_Data4(void *buffer,int len,int dir_out);
#endif
extern unsigned long  gUsbStatus,gUsbStatusP1,gUsbStatusP2,gUsbStatusP3;
extern int gTotalBytes,gTotalBytesP1,gTotalBytesP2,gTotalBytesP3;
int usb_bulk_transfer_in(struct usb_device *dev, void *data, int len,int *transdata)
{
    *transdata=0;
    if (UsbPortSelect==0)
    {
        if (Send_Receive_Bulk_Data(data,len,0)>0)
        {
            dev->status=gUsbStatus;
            return -1;
         }
        *transdata=len-gTotalBytes;
    }
#if defined(ENABLE_SECOND_EHC)	
    else if (UsbPortSelect==1)
    {
        if (Send_Receive_Bulk_Data2(data,len,0)>0)
        {
            dev->status=gUsbStatusP1;
            return -1;
        }
        *transdata=len-gTotalBytesP1;
    }
#endif
#if defined(ENABLE_THIRD_EHC)
    else if (UsbPortSelect==2)
    {
        if (Send_Receive_Bulk_Data3(data,len,0)>0)
        {
            dev->status=gUsbStatusP2;
            return -1;
        }
        *transdata=len-gTotalBytesP2;
    }
#endif
#if defined(ENABLE_FOURTH_EHC)
        else if (UsbPortSelect==3)
        {
            if (Send_Receive_Bulk_Data4(data,len,0)>0)
            {
                dev->status=gUsbStatusP3;
                return -1;
            }
            *transdata=len-gTotalBytesP3;
        }
#endif

    if(dev->status==0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int usb_bulk_transfer_out(struct usb_device *dev, void *data, int len)
{
    if (UsbPortSelect==0)
    {
        if (Send_Receive_Bulk_Data(data,len,1)>0)
        {
            dev->status=gUsbStatus;
            return -1;
        }
    }
#if defined(ENABLE_SECOND_EHC)	
    else if (UsbPortSelect==1)
    {
         if (Send_Receive_Bulk_Data2(data,len,1)>0)
        {
            dev->status=gUsbStatusP1;
            return -1;
        }
    }
#endif
#if defined(ENABLE_THIRD_EHC)
    else if (UsbPortSelect==2)
    {
         if (Send_Receive_Bulk_Data3(data,len,1)>0)
        {
            dev->status=gUsbStatusP2;
            return -1;
        }
    }
#endif
#if defined(ENABLE_FOURTH_EHC)
        else if (UsbPortSelect==3)
        {
             if (Send_Receive_Bulk_Data4(data,len,1)>0)
            {
                dev->status=gUsbStatusP3;
                return -1;
            }
        }
#endif

    if(dev->status==0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int submit_bulk_msg(struct usb_device *dev, unsigned long pipe, void *buffer, int len)
{
	int dir_out = usb_pipeout(pipe);
	int done = 0;

	dev->status = 0;

    if (UsbPortSelect==0)
    {
        if (Send_Receive_Bulk_Data(buffer,len,dir_out)>0)
        {
            dev->status=gUsbStatus;
            return -1;
        }
    }
#if defined(ENABLE_SECOND_EHC)	
    else if (UsbPortSelect==1)
    {
         if (Send_Receive_Bulk_Data2(buffer,len,dir_out)>0)
        {
            dev->status=gUsbStatusP1;
            return -1;
        }
    }
#endif	
#if defined(ENABLE_THIRD_EHC)
    else if (UsbPortSelect==2)
    {
         if (Send_Receive_Bulk_Data3(buffer,len,dir_out)>0)
        {
            dev->status=gUsbStatusP2;
            return -1;
        }
    }
#endif
#if defined(ENABLE_FOURTH_EHC)
        else if (UsbPortSelect==3)
        {
             if (Send_Receive_Bulk_Data4(buffer,len,dir_out)>0)
            {
                dev->status=gUsbStatusP3;
                return -1;
            }
        }
#endif

	dev->act_len = done;

	return 0;
}

extern unsigned char flib_Host20_Issue_Control (unsigned char bEdNum, unsigned char* pbCmd, unsigned short hwDataSize, unsigned char* pbData);
#if defined(ENABLE_SECOND_EHC)
extern unsigned char flib_Host20_Issue_Control2 (unsigned char bEdNum, unsigned char* pbCmd, unsigned short hwDataSize, unsigned char* pbData);
#endif
#if defined(ENABLE_THIRD_EHC)
extern unsigned char flib_Host20_Issue_Control3 (unsigned char bEdNum, unsigned char* pbCmd, unsigned short hwDataSize, unsigned char* pbData);
#endif
#if defined(ENABLE_FOURTH_EHC)
extern unsigned char flib_Host20_Issue_Control4 (unsigned char bEdNum, unsigned char* pbCmd, unsigned short hwDataSize, unsigned char* pbData);
#endif
int submit_control_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
		       int len,struct devrequest *setup)
{
	int done = 0;
	dev->status = 0;

    if (UsbPortSelect==0)
    {

        if ( flib_Host20_Issue_Control (dev->devnum,(unsigned char*) setup,len,(unsigned char*) buffer) > 0)
        {
            dev->status=gUsbStatus;
            done=-1;
        }
    }
#if defined(ENABLE_SECOND_EHC)	
    else if (UsbPortSelect==1)
    {
        if ( flib_Host20_Issue_Control2(dev->devnum,(unsigned char*) setup,len,(unsigned char*) buffer) > 0)
        {
            dev->status=gUsbStatusP1;
            done=-1;
        }
    }
#endif	
#if defined(ENABLE_THIRD_EHC)
    else if (UsbPortSelect==2)
    {
        if ( flib_Host20_Issue_Control3(dev->devnum,(unsigned char*) setup,len,(unsigned char*) buffer) > 0)
        {
            dev->status=gUsbStatusP2;
            done=-1;
        }
    }
#endif
#if defined(ENABLE_FOURTH_EHC)
        else if (UsbPortSelect==3)
        {
            if ( flib_Host20_Issue_Control4(dev->devnum,(unsigned char*) setup,len,(unsigned char*) buffer) > 0)
            {
                dev->status=gUsbStatusP3;
            }
        }
#endif
    /* status phase */

	dev->act_len = len;

	return done;
}

int submit_int_msg(struct usb_device *dev, unsigned long pipe, void *buffer, int len, int interval)
{
	return -1;
}

/*
 * SL811 Virtual Root Hub
 */

/* Device descriptor */
#if 0
static __u8 sl811_rh_dev_des[] =
{
	0x12,	    /*	__u8  bLength; */
	0x01,	    /*	__u8  bDescriptorType; Device */
	0x10,	    /*	__u16 bcdUSB; v1.1 */
	0x01,
	0x09,	    /*	__u8  bDeviceClass; HUB_CLASSCODE */
	0x00,	    /*	__u8  bDeviceSubClass; */
	0x00,	    /*	__u8  bDeviceProtocol; */
	0x08,	    /*	__u8  bMaxPacketSize0; 8 Bytes */
	0x00,	    /*	__u16 idVendor; */
	0x00,
	0x00,	    /*	__u16 idProduct; */
	0x00,
	0x00,	    /*	__u16 bcdDevice; */
	0x00,
	0x00,	    /*	__u8  iManufacturer; */
	0x02,	    /*	__u8  iProduct; */
	0x01,	    /*	__u8  iSerialNumber; */
	0x01	    /*	__u8  bNumConfigurations; */
};

/* Configuration descriptor */
static __u8 sl811_rh_config_des[] =
{
	0x09,	    /*	__u8  bLength; */
	0x02,	    /*	__u8  bDescriptorType; Configuration */
	0x19,	    /*	__u16 wTotalLength; */
	0x00,
	0x01,	    /*	__u8  bNumInterfaces; */
	0x01,	    /*	__u8  bConfigurationValue; */
	0x00,	    /*	__u8  iConfiguration; */
	0x40,	    /*	__u8  bmAttributes;
		    Bit 7: Bus-powered, 6: Self-powered, 5 Remote-wakwup,
		    4..0: resvd */
	0x00,	    /*	__u8  MaxPower; */

	/* interface */
	0x09,	    /*	__u8  if_bLength; */
	0x04,	    /*	__u8  if_bDescriptorType; Interface */
	0x00,	    /*	__u8  if_bInterfaceNumber; */
	0x00,	    /*	__u8  if_bAlternateSetting; */
	0x01,	    /*	__u8  if_bNumEndpoints; */
	0x09,	    /*	__u8  if_bInterfaceClass; HUB_CLASSCODE */
	0x00,	    /*	__u8  if_bInterfaceSubClass; */
	0x00,	    /*	__u8  if_bInterfaceProtocol; */
	0x00,	    /*	__u8  if_iInterface; */

	/* endpoint */
	0x07,	    /*	__u8  ep_bLength; */
	0x05,	    /*	__u8  ep_bDescriptorType; Endpoint */
	0x81,	    /*	__u8  ep_bEndpointAddress; IN Endpoint 1 */
	0x03,	    /*	__u8  ep_bmAttributes; Interrupt */
	0x08,	    /*	__u16 ep_wMaxPacketSize; */
	0x00,
	0xff	    /*	__u8  ep_bInterval; 255 ms */
};

/* root hub class descriptor*/
static __u8 sl811_rh_hub_des[] =
{
	0x09,			/*  __u8  bLength; */
	0x29,			/*  __u8  bDescriptorType; Hub-descriptor */
	0x01,			/*  __u8  bNbrPorts; */
	0x00,			/* __u16  wHubCharacteristics; */
	0x00,
	0x50,			/*  __u8  bPwrOn2pwrGood; 2ms */
	0x00,			/*  __u8  bHubContrCurrent; 0 mA */
	0xfc,			/*  __u8  DeviceRemovable; *** 7 Ports max *** */
	0xff			/*  __u8  PortPwrCtrlMask; *** 7 ports max *** */
};

/*
 * helper routine for returning string descriptors in UTF-16LE
 * input can actually be ISO-8859-1; ASCII is its 7-bit subset
 */
static int ascii2utf (char *s, u8 *utf, int utfmax)
{
	int retval;

	for (retval = 0; *s && utfmax > 1; utfmax -= 2, retval += 2) {
		*utf++ = *s++;
		*utf++ = 0;
	}
	return retval;
}
#endif
/*
 * root_hub_string is used by each host controller's root hub code,
 * so that they're identified consistently throughout the system.
 */
#if 0
static int usb_root_hub_string (int id, int serial, char *type, __u8 *data, int len)
{
	char buf [30];

	/* assert (len > (2 * (sizeof (buf) + 1)));
	   assert (strlen (type) <= 8);*/

	/* language ids */
	if (id == 0) {
		*data++ = 4; *data++ = 3;	/* 4 bytes data */
		*data++ = 0; *data++ = 0;	/* some language id */
		return 4;

	/* serial number */
	} else if (id == 1) {
		sprintf (buf, "%#x", serial);

	/* product description */
	} else if (id == 2) {
		sprintf (buf, "USB %s Root Hub", type);

	/* id 3 == vendor description */

	/* unsupported IDs --> "stall" */
	} else
	    return 0;

	ascii2utf (buf, data + 2, len - 2);
	data [0] = 2 + strlen(buf) * 2;
	data [1] = 3;
	return data [0];
}
#endif
/* helper macro */
#define OK(x)	len = (x); break

/*
 * This function handles all USB request to the the virtual root hub
 */
#if 0
static int titania_rh_submit_urb(struct usb_device *usb_dev, unsigned long pipe,
			       void *data, int buf_len, struct devrequest *cmd)
{
	__u8 data_buf[16];
	__u8 *bufp = data_buf;
	int len = 0;
	int status = 0;

	__u16 bmRType_bReq;
	__u16 wValue;
	__u16 wIndex;
	__u16 wLength;

	if (usb_pipeint(pipe)) {
		PDEBUG(0, "interrupt transfer unimplemented!\n");
		return 0;
	}

	bmRType_bReq  = cmd->requesttype | (cmd->request << 8);
	wValue	      = le16_to_cpu (cmd->value);
	wIndex	      = le16_to_cpu (cmd->index);
	wLength	      = le16_to_cpu (cmd->length);

	PDEBUG(5, "submit rh urb, req = %d(%x) val = %#x index = %#x len=%d\n",
	       bmRType_bReq, bmRType_bReq, wValue, wIndex, wLength);

	/* Request Destination:
		   without flags: Device,
		   USB_RECIP_INTERFACE: interface,
		   USB_RECIP_ENDPOINT: endpoint,
		   USB_TYPE_CLASS means HUB here,
		   USB_RECIP_OTHER | USB_TYPE_CLASS  almost ever means HUB_PORT here
	*/
	switch (bmRType_bReq) {
	case RH_GET_STATUS:
		*(__u16 *)bufp = cpu_to_le16(1);
		OK(2);

	case RH_GET_STATUS | USB_RECIP_INTERFACE:
		*(__u16 *)bufp = cpu_to_le16(0);
		OK(2);

	case RH_GET_STATUS | USB_RECIP_ENDPOINT:
		*(__u16 *)bufp = cpu_to_le16(0);
		OK(2);

	case RH_GET_STATUS | USB_TYPE_CLASS:
		*(__u32 *)bufp = cpu_to_le32(0);
		OK(4);

	case RH_GET_STATUS | USB_RECIP_OTHER | USB_TYPE_CLASS:
		*(__u32 *)bufp = cpu_to_le32(rh_status.wPortChange<<16 | rh_status.wPortStatus);
		OK(4);

	case RH_CLEAR_FEATURE | USB_RECIP_ENDPOINT:
		switch (wValue) {
		case 1:
			OK(0);
		}
		break;

	case RH_CLEAR_FEATURE | USB_TYPE_CLASS:
		switch (wValue) {
		case C_HUB_LOCAL_POWER:
			OK(0);

		case C_HUB_OVER_CURRENT:
			OK(0);
		}
		break;

	case RH_CLEAR_FEATURE | USB_RECIP_OTHER | USB_TYPE_CLASS:
		switch (wValue) {
		case USB_PORT_FEAT_ENABLE:
			rh_status.wPortStatus &= ~USB_PORT_STAT_ENABLE;
			OK(0);

		case USB_PORT_FEAT_SUSPEND:
			rh_status.wPortStatus &= ~USB_PORT_STAT_SUSPEND;
			OK(0);

		case USB_PORT_FEAT_POWER:
			rh_status.wPortStatus &= ~USB_PORT_STAT_POWER;
			OK(0);

		case USB_PORT_FEAT_C_CONNECTION:
			rh_status.wPortChange &= ~USB_PORT_STAT_C_CONNECTION;
			OK(0);

		case USB_PORT_FEAT_C_ENABLE:
			rh_status.wPortChange &= ~USB_PORT_STAT_C_ENABLE;
			OK(0);

		case USB_PORT_FEAT_C_SUSPEND:
			rh_status.wPortChange &= ~USB_PORT_STAT_C_SUSPEND;
			OK(0);

		case USB_PORT_FEAT_C_OVER_CURRENT:
			rh_status.wPortChange &= ~USB_PORT_STAT_C_OVERCURRENT;
			OK(0);

		case USB_PORT_FEAT_C_RESET:
			rh_status.wPortChange &= ~USB_PORT_STAT_C_RESET;
			OK(0);
		}
		break;

	case RH_SET_FEATURE | USB_RECIP_OTHER | USB_TYPE_CLASS:
		switch (wValue) {
		case USB_PORT_FEAT_SUSPEND:
			rh_status.wPortStatus |= USB_PORT_STAT_SUSPEND;
			OK(0);

		case USB_PORT_FEAT_RESET:
			rh_status.wPortStatus |= USB_PORT_STAT_RESET;
			rh_status.wPortChange = 0;
			rh_status.wPortChange |= USB_PORT_STAT_C_RESET;
			rh_status.wPortStatus &= ~USB_PORT_STAT_RESET;
			rh_status.wPortStatus |= USB_PORT_STAT_ENABLE;
			OK(0);

		case USB_PORT_FEAT_POWER:
			rh_status.wPortStatus |= USB_PORT_STAT_POWER;
			OK(0);

		case USB_PORT_FEAT_ENABLE:
			rh_status.wPortStatus |= USB_PORT_STAT_ENABLE;
			OK(0);
		}
		break;

	case RH_SET_ADDRESS:
		root_hub_devnum = wValue;
		OK(0);

	case RH_GET_DESCRIPTOR:
		switch ((wValue & 0xff00) >> 8) {
		case USB_DT_DEVICE:
			len = sizeof(sl811_rh_dev_des);
			bufp = sl811_rh_dev_des;
			OK(len);

		case USB_DT_CONFIG:
			len = sizeof(sl811_rh_config_des);
			bufp = sl811_rh_config_des;
			OK(len);

		case USB_DT_STRING:
			len = usb_root_hub_string(wValue & 0xff, (int)(long)0,	"SL811HS", data, wLength);
			if (len > 0) {
				bufp = data;
				OK(len);
			}

		default:
			status = -32;
		}
		break;

	case RH_GET_DESCRIPTOR | USB_TYPE_CLASS:
		len = sizeof(sl811_rh_hub_des);
		bufp = sl811_rh_hub_des;
		OK(len);

	case RH_GET_CONFIGURATION:
		bufp[0] = 0x01;
		OK(1);

	case RH_SET_CONFIGURATION:
		OK(0);

	default:
		PDEBUG(1, "unsupported root hub command\n");
		status = -32;
	}

	len = min(len, buf_len);
	if (data != bufp)
		memcpy(data, bufp, len);

	PDEBUG(5, "len = %d, status = %d\n", len, status);

	usb_dev->status = status;
	usb_dev->act_len = len;

	return status == 0 ? len : status;
}
#endif
#endif	/* CONFIG_USB_SL811HS */
