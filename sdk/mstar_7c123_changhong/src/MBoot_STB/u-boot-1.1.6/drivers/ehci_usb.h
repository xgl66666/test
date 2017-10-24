#ifndef EHCI_USB_H
#define EHCI_USB_H

#define mdelay(n) ({unsigned long msec=(n); while (msec--) udelay(1000);})

#if defined(CONFIG_KAPPA)||defined(CONFIG_KRITI)
void DelayMS(unsigned int msec);
#define USB_DELAY(_msec)           DelayMS(_msec*2)
#elif defined(CONFIG_KELTIC)
#define USB_DELAY(_msec)           mdelay(_msec)
#else
#define USB_DELAY(_msec)           wait_ms(_msec)
#endif

#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)
#define KSEG02KSEG1(addr)  ((void *)((U32)(addr)|0x80000000))
#define KSEG12KSEG0(addr)  ((void *)((U32)(addr)&~0x80000000))
#else
#define KSEG02KSEG1(addr)       ((void *)((UINT32)(addr)|0x20000000))  //cached -> unchched
#define KSEG12KSEG0(addr)       ((void *)((UINT32)(addr)&~0x20000000)) //unchched -> cached
#endif

#if (defined (CONFIG_TITANIA4)||defined(CONFIG_URANUS4)||defined(CONFIG_JANUS))||defined(CONFIG_MARIA10)||defined(CONFIG_KRONUS)
#define VA2PA(a)  (a)
#define PA2VA(a)  (a)
#elif defined(CONFIG_KAISERIN)
#define MIU0_BUS_BASE_ADDR	0xA0000000UL
#define MIU0_PHY_BASE_ADDR	0x00000000UL

#define MIU1_BUS_BASE_ADDR	0x00000000UL
#define MIU1_PHY_BASE_ADDR	0x80000000UL
/* transmit between BUS and USB PHY Addr */
extern U32 VA2PA(U32 u32_DMAAddr);
extern U32 PA2VA(U32 u32_DMAAddr);
#else
#define VA2PA(a)  ((a) & 0x1FFFFFFF)
#define PA2VA(a)  ((a) | 0xA0000000) //// mapping to uncache address
#endif

/* Requests: bRequest << 8 | bmRequestType */
#define RH_GET_STATUS           0x0080
#define RH_CLEAR_FEATURE        0x0100
#define RH_SET_FEATURE          0x0300
#define RH_SET_ADDRESS		0x0500
#define RH_GET_DESCRIPTOR	0x0680
#define RH_SET_DESCRIPTOR       0x0700
#define RH_GET_CONFIGURATION	0x0880
#define RH_SET_CONFIGURATION	0x0900
#define RH_GET_STATE            0x0280
#define RH_GET_INTERFACE        0x0A80
#define RH_SET_INTERFACE        0x0B00
#define RH_SYNC_FRAME           0x0C80

#if !defined(CONFIG_JANUS) && !defined(CONFIG_MARIA10)
#define _USB_FLUSH_BUFFER   1
#else
#define _USB_FLUSH_BUFFER   0
#endif

#endif

