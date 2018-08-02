
################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#
# Makefile used for building DDI
#
#

################################################################################

# **********************************************
# DRV Name
# **********************************************
DRV_NAME = ECOS_USBWIFI
RT28xx_DIR = ./MODULE
MODULE = 7601U
RT28xx_MODE = STA
CHIPSET = 7601U
# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \


# **********************************************
# Rules
# **********************************************
include ./MODULE/os/ecos/rules.mak
DRV_OBJ := ${foreach obj,$($(MOD_NAME)-objs),$(RT28xx_DIR)/os/ecos/$(obj)}
DRV_SRC := $(DRV_OBJ:.o=.c)
#DRV_OBJ := $(ROOTLIB)/drv/usb_ecos/newhost/MT7601U/MODULE/os/ecos/$($(MOD_NAME)-objs)
include drv_rule.mk

#DRV_INCDIR  = $(ROOTLIB)/drv/usb_ecos/newhost/
