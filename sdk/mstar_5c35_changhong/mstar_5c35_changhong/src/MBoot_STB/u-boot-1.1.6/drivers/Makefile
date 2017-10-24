#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

# CFLAGS += -DET_DEBUG -DDEBUG

LIB	= $(obj)libdrivers.a

COBJS	= cfb_console.o \
	  netconsole.o \
	  serial.o serial_uranus.o serial_titania.o chip_setup.o

COBJS += titania_hw_i2c.o

ifeq ($(CONFIG_MICOM),y)
COBJS += drvMicom.o
endif

COBJS += cfi_flash.o dataflash.o

ifeq ($(CONFIG_USB),y)
COBJS += usbdcore.o usbtty.o \
         drvusbmain.o drvhostlib.o \
         drvusbmain2.o drvhostlib2.o drvusbmain3.o drvhostlib3.o \
         drvusbmain4.o drvhostlib4.o \
         ehci_usb.o usb_lan.o ax88772a.o rtl8150.o pegasus.o
endif

COBJS += drvMIU.o
COBJS += ./mstar/unfd/src/config/drvNAND_platform.o ./mstar/unfd/src/common/drvNAND_hal.o \
         ./mstar/unfd/src/common/drvNAND_utl.o ./mstar/unfd/src/api/drvNAND_prg.o \
         ./mstar/unfd/src/common/drvNAND_ReadRetry.o

# ifeq ($(CONFIG_SPI),y)
# COBJS += spiflash.o
# endif

ifeq ($(CONFIG_ALBANY),y)
CFLAGS += -DSUPPORT_ALBANY=1
else
CFLAGS += -DSUPPORT_ALBANY=0
endif
ifeq ($(CONFIG_NET),y)
COBJS += mdrv_emac.o mhal_emac.o
endif

COBJS += msKeypad.o
COBJS += msAPI_Tuner.o

SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
