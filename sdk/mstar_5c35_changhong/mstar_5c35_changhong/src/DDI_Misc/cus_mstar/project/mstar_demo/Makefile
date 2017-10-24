
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

DDI_MISC_CUS_DIR = $(DDI_MISC_DIR)/cus_mstar


# Source files
HB_DDI_MISC = $(CFG_DDI_MISC)
ifneq ($(HB_DDI_MISC),ENABLE)
ifeq ($(MI_BSP), )
DDI_MISC_SRCDIR = $(DDI_MISC_CUS_DIR)/app/demo
else
ifeq ($(MI_BSP), disable)
DDI_MISC_SRCDIR = $(DDI_MISC_CUS_DIR)/app/demo
endif
endif
endif

DDI_MISC_SRCDIR += \
            $(DDI_MISC_CUS_DIR)/api/frontend                                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/iic                                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir                                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/frontpnl                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod                                          \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod/avl_dvbsx                                \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod/avl_dvbsx/avl_dvbsx/src                  \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod/avl_dvbsx/bsp                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner                                          \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/mxl5007t_api_files_v4.1.3                \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18250/src                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18250/inc                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmddtda18250/inc                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmddtda18250/src                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA18250A/inc                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA18250A/src                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA182I5a/inc                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA182I5a/src                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/mxl201rf                                 \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dib8096                                  \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL203                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL603                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL608                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL254                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmbslTDA18260/src          \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Shared/src     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Stream/src     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner                               \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2011                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2012                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2018                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2028                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7306                     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7801                     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/tda20152                      \
            $(DDI_MISC_CUS_DIR)/platform/driver/dish

ifeq ($(OS_TYPE),ecos)
JFFS2_CHIPS = keltic kaiser keres kappa kriti
ifneq ($(CHIP), $(filter $(CHIP),$(JFFS2_CHIPS)))
DDI_MISC_SRCDIR += $(DDI_MISC_CUS_DIR)/platform/driver/jffs2
endif
endif

#Headers
ifneq ($(HB_DDI_MISC),ENABLE)
ifeq ($(MI_BSP), )
DDI_MISC_INCDIR = $(DDI_MISC_CUS_DIR)/app/demo
else
ifeq ($(MI_BSP), disable)
DDI_MISC_INCDIR = $(DDI_MISC_CUS_DIR)/app/demo
endif
endif
endif

DDI_MISC_INCDIR += \
            $(DDI_MISC_CUS_DIR)/project/mstar_demo                                                 \
            $(DDI_MISC_CUS_DIR)/api/frontend                                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/iic                                                \
            $(DDI_MISC_CUS_DIR)/platform/driver/scart                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/frontpnl                                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir                                                 \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod/avl_dvbsx                                    \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod/avl_dvbsx/avl_dvbsx/include                  \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod/avl_dvbsx/bsp                                \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/driver/tuner/inc                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/mxl5007t_api_files_v4.1.3                    \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18250/src                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18250/inc                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmddtda18250/inc                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmddtda18250/src                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA18250A/inc                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA18250A/src                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA182I5a/inc                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbslTDA182I5a/src                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/mxl201rf                                     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/nt220x/inc                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/nt220x/tmbslnt220x/inc                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dib8096                                      \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL203                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL603                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL608                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL254                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/inc                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmbslTDA18260/inc              \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmbslTDA18260/cfg              \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Shared/inc         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Shared/cfg         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Shared/src         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Stream/inc         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Stream/cfg         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Stream/src         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2011                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2012                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2018                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2028                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7306                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7801                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/tda20152                          \
            $(DDI_MISC_CUS_DIR)/platform/driver/dish

#ifeq ($(CFG_FE_AUTO_TEST), enable)
#SRCDIR      += $(DDI_MISC_DIR)/platform/driver/demod/autotest
#INCDIR      += $(DDI_MISC_DIR)/platform/driver/demod/autotest
#ndif
