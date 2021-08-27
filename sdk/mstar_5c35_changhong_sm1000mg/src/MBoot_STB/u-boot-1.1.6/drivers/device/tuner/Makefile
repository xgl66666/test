#
# (C) Copyright 2000-2007
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

LIB	:= $(obj)libtuner.a

COBJS	:= Tuner.o

ifeq ($(CONFIG_MSTAR_TUNER_AV2011), y)
COBJS	+= AV2011/ExtAV2011.o
endif

ifeq ($(CONFIG_MSTAR_TUNER_AV2012), y)
COBJS	+= AV2012/drvTuner_AV2012.o
endif

ifeq ($(CONFIG_MSTAR_TUNER_THOMSON_DCT70707), y)
COBJS   += THOMSON_DCT70707/drvTuner_DCT70707.o
endif


ifeq ($(CONFIG_MSTAR_TUNER_RT828), y)
COBJS   += RT82X/drvTuner_R828.o
endif

ifeq ($(CONFIG_MSTAR_TUNER_RT820T), y)
COBJS   += RT82X/drvTuner_R820T.o
endif

ifeq ($(CONFIG_MSTAR_TUNER_MXL_MXL603), y)
COBJS   += MxL603/drvTuner_MxL603.o  \
              MxL603/MxL603_OEM_Drv.o  \
              MxL603/MxL603_TunerApi.o  \
              MxL603/MxL603_TunerCfg.o 
endif

ifeq ($(CONFIG_MSTAR_TUNER_PHILIPS_TDA18250HN), y)
COBJS   += TDA18250/drvTuner_TDA18250HN.o	\
	   TDA18250/tmbslTDA18250/src/System_Lo_Detuning.o	\
	   TDA18250/tmbslTDA18250/src/System_Manage_HPMTO.o	\
	   TDA18250/tmbslTDA18250/src/tmbslTDA18250.o	\
	   TDA18250/tmbslTDA18250/src/tmbslTDA18250AdvanceCustom.o	\
	   TDA18250/tmbslTDA18250/src/tmbslTDA18250Instance.o	\
	   TDA18250/tmddTDA18250/src/tmddTDA18250.o	\
	   TDA18250/tmddTDA18250/src/tmddTDA18250Instance.o	\
	   TDA18250/drvTuner_TDA18250HN.o	   
endif




ifeq ($(CONFIG_MSTAR_TUNER_NMI120), y)
COBJS   += NMI120/nmicmn.o\
           NMI120/nmitnr.o \
           NMI120/nmi_tuner_interface.o \
           NMI120/nmi_tuner_os.o \
           NMI120/nmituner_api.o 
endif

SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
