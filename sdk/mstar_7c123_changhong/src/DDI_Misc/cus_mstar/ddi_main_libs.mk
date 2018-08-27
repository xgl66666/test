#always release iniparser for mm_cfg.ini
EXTRAMWLIBS += releaseINIPARSER

ifeq ($(DDI_DEMO_MM),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
ifeq ($(DDI_DEMO_MM_AVP),enable)
EXTRAMWLIBS += releaseMMSDK_AVP
ifeq ($(DDI_DEMO_HDCP22),disable)
EXTRAMWLIBS += releaseOPENSSL
endif
endif
endif

ifeq ($(DDI_DEMO_OPENGLES2),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
endif

ifeq ($(DDI_DEMO_DFB),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
endif

ifeq ($(MI_BSP),enable)
ifeq ($(MMSDK_BSP), enable)
EXTRAMWLIBS += releaseZLIB releaseFREETYPE
ifeq ($(DDI_MMSDK_AVP),enable)
EXTRAMWLIBS += releaseMMSDK_AVP
ifeq ($(DDI_DEMO_HDCP22),disable)
EXTRAMWLIBS += releaseOPENSSL
endif
endif
endif

ifeq ($(DFB_BSP), enable)
EXTRAMWLIBS +=  releaseZLIB releaseFREETYPE
endif

endif

