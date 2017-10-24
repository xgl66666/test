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

LIB	:= $(obj)libcofdm.a

COBJS	:= COFDM_Demodulator.o

ifeq ($(CONFIG_DEMOD_AVL6211), y)
COBJS	+=  AVL_DVBSx/AVL6211_patch_dat.o \
            AVL_DVBSx/AVL_DVBSx_Func.o    \
            AVL_DVBSx/drvDemodAVL6211.o   \
            AVL_DVBSx/devDemodAVL6211.o   \
            AVL_DVBSx/BSP/IBSP.o          \
            AVL_DVBSx/AVL_DVBSx/src/avl.o \
            AVL_DVBSx/AVL_DVBSx/src/avl_dvbsx.o \
            AVL_DVBSx/AVL_DVBSx/src/IBase.o \
            AVL_DVBSx/AVL_DVBSx/src/IBlindScan.o    \
            AVL_DVBSx/AVL_DVBSx/src/IDiseqc.o  \
            AVL_DVBSx/AVL_DVBSx/src/II2C.o  \
            AVL_DVBSx/AVL_DVBSx/src/II2CRepeater.o  \
            AVL_DVBSx/AVL_DVBSx/src/IRx.o   \
            AVL_DVBSx/AVL_DVBSx/src/ITuner.o
endif




ifeq ($(CONFIG_MSTAR_MSB1236C_DEMOD), y)
COBJS  +=   DEMOD1236C/device_demodulator_msb1236c.o
endif

ifeq ($(CONFIG_MSTAR_MSB131X_DEMOD), y)
COBJS  +=   DEMOD_MSB131X/device_demodulator_msb131x.o 
COBJS  +=   DEMOD_MSB131X/demod131x_dvbs.o
endif



ifeq ($(CONFIG_DEMOD_ATBM885x), y)
COBJS  +=   DEMOD_ATBM885x/atbm885x.o 
COBJS  +=   DEMOD_ATBM885x/atbmdvbc.o
COBJS  +=   DEMOD_ATBM885x/drvDemodATBM885x.o 
endif

ifeq ($(CONFIG_DEMOD_SamSung_S5H1411), y)
COBJS  +=   S5H1411/S5H1411_Ver1.o
endif

ifeq ($(CONFIG_MSTAR_MSB1400_DEMOD),y)
COBJS += MSB140X/device_demodulator_msb1400.o
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
