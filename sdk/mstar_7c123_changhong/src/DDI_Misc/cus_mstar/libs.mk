EXTRAMWLIBS += releaseINIPARSER

ifeq ($(DFB_BSP),enable)
EXTRAMWLIBS += releaseFREETYPE
EXTRAMWLIBS += releaseZLIB
endif
