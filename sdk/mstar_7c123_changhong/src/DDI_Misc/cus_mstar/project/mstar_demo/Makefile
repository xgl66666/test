
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
-include $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SiLabs_feature.mk

# Source files

HB_DDI_MISC = $(CFG_FE_DDI_MISC_ENABLE)

ifneq ($(HB_DDI_MISC),1)
ifeq ($(MI_BSP), )
DDI_MISC_SRCDIR = $(DDI_MISC_CUS_DIR)/app/demo
else
ifeq ($(MI_BSP), disable)
DDI_MISC_SRCDIR = $(DDI_MISC_CUS_DIR)/app/demo
endif
endif
endif

ifeq ($(USE_SiLAB_SUPERSET), TRUE)
include $(DDI_MISC_CUS_DIR)/api/frontend_SiLabs/SiLabs.mk
DDI_MISC_SRCDIR += \
            $(DDI_MISC_CUS_DIR)/platform/driver/iic                                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir                                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir_tx                                          \
            $(DDI_MISC_CUS_DIR)/platform/driver/frontpnl

DDI_MISC_INCDIR += \
            $(DDI_MISC_CUS_DIR)/project/mstar_demo                                                 \
            $(DDI_MISC_CUS_DIR)/platform/driver/iic                                                \
            $(DDI_MISC_CUS_DIR)/platform/driver/scart                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/frontpnl                                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir                                                 \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir_tx                                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/dtc                                                \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/dish                                                \
            $(DDI_MISC_CUS_DIR)/api/frontend
else
DDI_MISC_SRCDIR += \
            $(DDI_MISC_CUS_DIR)/api/frontend                                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/iic                                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir                                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir_tx                                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/frontpnl                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod                                          \
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
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL661                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL254                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL542                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL683                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmbslTDA18260/src          \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Shared/src     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/tmbsltda18260/tmddTDA18260Stream/src     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner                               \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2011                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2012                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2017                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2018                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2028                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2035                        \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/RT710                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7306                     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7801                     \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/tda20152                      \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/stv6111                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/si2144                                   \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/ATBM2040                                 \
            $(DDI_MISC_CUS_DIR)/platform/driver/dish

DDI_MISC_INCDIR += \
            $(DDI_MISC_CUS_DIR)/project/mstar_demo                                                 \
            $(DDI_MISC_CUS_DIR)/api/frontend                                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/iic                                                \
            $(DDI_MISC_CUS_DIR)/platform/driver/scart                                              \
            $(DDI_MISC_CUS_DIR)/platform/driver/frontpnl                                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir                                                 \
            $(DDI_MISC_CUS_DIR)/platform/driver/ir_tx                                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/demod                                              \
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
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL661                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL254                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL542                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/MxL683                                       \
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
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2017                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2018                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2028                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/av2035                            \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/RT710                             \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7306                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/sharp7801                         \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/tda20152                          \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/dvbs_tuner/stv6111                          \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/si2144                                      \
            $(DDI_MISC_CUS_DIR)/platform/driver/tuner/ATBM2040                                     \
            $(DDI_MISC_CUS_DIR)/platform/driver/dish                                               \
            $(DDI_MISC_CUS_DIR)/platform/driver/dtc
endif



ifeq ($(OS_TYPE),ecos)
JFFS2_CHIPS = keltic kaiser keres kappa kriti clippers kratos kano kiwi k6 c2 k6lite k7u
ifneq ($(CHIP), $(filter $(CHIP),$(JFFS2_CHIPS)))
DDI_MISC_SRCDIR += $(DDI_MISC_CUS_DIR)/platform/driver/jffs2
endif
ifeq ($(ECOS_WIFI_SUPPORT),enable)
DDI_MISC_SRCDIR += \
            $(DDI_MISC_CUS_DIR)/api/wlan                                                       \
            $(DDI_MISC_CUS_DIR)/platform/driver/wlan                                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/wlan/MT7601U
endif
endif

#Headers
ifneq ($(HB_DDI_MISC),1)
ifeq ($(MI_BSP), )
DDI_MISC_INCDIR += $(DDI_MISC_CUS_DIR)/app/demo
else
ifeq ($(MI_BSP), disable)
DDI_MISC_INCDIR += $(DDI_MISC_CUS_DIR)/app/demo
endif
endif
endif

#ifeq ($(CFG_FE_AUTO_TEST), enable)
#SRCDIR      += $(DDI_MISC_DIR)/platform/driver/demod/autotest
#INCDIR      += $(DDI_MISC_DIR)/platform/driver/demod/autotest
#ndif

ifeq ($(ECOS_WIFI_SUPPORT),enable)
DDI_MISC_INCDIR += \
            $(DDI_MISC_CUS_DIR)/api/wlan                                                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/wlan                                           \
            $(DDI_MISC_CUS_DIR)/platform/driver/wlan/MT7601U
CFG_CC_OPTS += -D'ECOS_WIFI_SUPPORT=1'
else
CFG_CC_OPTS += -D'ECOS_WIFI_SUPPORT=0'
endif

