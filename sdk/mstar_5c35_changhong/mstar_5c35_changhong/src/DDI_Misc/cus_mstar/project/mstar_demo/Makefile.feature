
################################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
# Makefile used for include DDI_MISC files
#
#
################################################################################
SHELL=/bin/bash

ifeq ($(DDI_DEMO_SUPPORT_DVBC_DMD_BW_CHANGE), enable)
CFG_CC_OPTS += -D'SUPPORT_DVBC_DMD_BW_CHANGE=1'
else ifeq ($(DDI_DEMO_SUPPORT_DVBC_DMD_BW_CHANGE), disable)
CFG_CC_OPTS += -D'SUPPORT_DVBC_DMD_BW_CHANGE=0'
else
$(error 'DDI_DEMO_SUPPORT_DVBC_DMD_BW_CHANGE not define, please setup this at feature_configuration')
endif
