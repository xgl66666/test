#always release iniparser for mm_cfg.ini
EXTRAMWLIBS += releaseINIPARSER

ifeq ($(DDI_DEMO_MM),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
endif

ifeq ($(DDI_DEMO_OPENGLES2),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
endif

ifeq ($(DDI_DEMO_DFB),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
endif
