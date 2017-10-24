
# This file is for mstar_5c35_telesystem

###############################################################################
#                              Target OS section
###############################################################################

OS_C_FLAGS=
OS_CXX_FLAGS=
OS_AS_FLAGS=
OS_LD_FLAGS=
OS_INCLUDE_PATH=
OS_LIBS_PATH=
OS_LIBS=

SDK_C_FLAGS=
SDK_CXX_FLAGS=
SDK_LD_FLAGS=
SDK_AS_FLAGS=
SDK_INCLUDE_PATH=
SDK_LIBS_PATH=
SDK_LIBS = 

# --------------               local variables         ------------------------

SDK_SRC_ROOT= $(SDK_ROOT)/src

OS_BUILT = ecos_keltic_34kf_fileio_posix_fat_c++_mipsisa32
OS_EX_BUILD_OBJ = $(wildcard ecos/$(OS_BUILT)/libntfs/*.o)
OS_EX_BUILD_OBJ += $(wildcard ecos/$(OS_BUILT)/libjffs2/*.o)


ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
CFG_DVB_TYPE = DVBC
endif

###############################################################################
#                               OS section
###############################################################################


# --------------               OS C flags              ------------------------

OS_C_FLAGS +=

# --------------               OS Include paths        ------------------------

ifeq "$(PRODUCT_PREBUILT_SDK)" "YES"
OS_INCLUDE_PATH += $(SDK_ROOT)/inc/os
else
OS_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/ecos/$(OS_BUILT)/include 
endif

# --------------               OS Libraries            ------------------------

OS_LIBS += $(LIB_PREFFIX)ecos.$(LIB_SUFFIX)
OS_LIBS += $(LIB_PREFFIX)stdc++.$(LIB_SUFFIX)
OS_LIBS += $(LIB_PREFFIX)supc++.$(LIB_SUFFIX)

# --------------               OS Libraries paths      ------------------------

ifeq "$(PRODUCT_PREBUILT_SDK)" "YES"
OS_LIBS_PATH += $(SDK_ROOT)/lib/os
else
OS_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/ecos/$(OS_BUILT)/lib
endif


###############################################################################
#                               Driver section
###############################################################################

# --------------                 SDK C flags          -------------------------

SDK_C_FLAGS +=-D'CA_TYPE=NONE'
SDK_C_FLAGS +=-D'NDS_LIB=NONE' 
SDK_C_FLAGS +=-DXRPC_MODULES_SERVER 
SDK_C_FLAGS +=-DQC_svpldi 
SDK_C_FLAGS +=-DQC_reset 
SDK_C_FLAGS +=-DQC_qc_tests 
SDK_C_FLAGS +=-DQC_nskldi 
SDK_C_FLAGS +=-DQC_nskhal 
SDK_C_FLAGS +=-DQC_harm_tests 
SDK_C_FLAGS +=-DQC_cahdiver 
SDK_C_FLAGS +=-DSOCKET_GLUE_SERVER 
SDK_C_FLAGS +=-D'MSOS_TYPE_ECOS' 
SDK_C_FLAGS +=-D'ENABLE_MM_PACKAGE' 
SDK_C_FLAGS +=-D'MM_MPEG2SD_PROFILE' 
SDK_C_FLAGS +=-D'DEMO_SMC_IO_POLLING=0' 
SDK_C_FLAGS +=-D'DEMO_SMC_USE_51=1' 
SDK_C_FLAGS +=-D'CFG_USB_ENABLE = 1' 
SDK_C_FLAGS +=-D'TVfunc_Normal=0' 
SDK_C_FLAGS +=-D'TVfunc_MpegSD=1' 
SDK_C_FLAGS +=-D'TVfunc_H264SD=2' 
SDK_C_FLAGS +=-D'Audio_Lite=1' 
SDK_C_FLAGS +=-D'Audio_Normal=0' 
SDK_C_FLAGS +=-D'AUDIO_LIB_TYPE = Audio_Normal' 
SDK_C_FLAGS +=-D'TVfunc = TVfunc_MpegSD' 
SDK_C_FLAGS +=-D'CFG_DDR_FREQ = 800' 
SDK_C_FLAGS +=-D'CHIP_KELTIC'
SDK_C_FLAGS +=-D'MEMORY_MAP = MMAP_64MB' 
SDK_C_FLAGS +=-D'CHIP_KELTIC' 
SDK_C_FLAGS +=-D'CFG_CHIP_MSD5XX5' 
SDK_C_FLAGS +=-D'MS_OPTIMIZE' 
SDK_C_FLAGS +=-DMS_C_STDLIB 
SDK_C_FLAGS +=-DMS_BOARD_TYPE_SEL=$(PLATFORM_BOARD_MODEL)
SDK_C_FLAGS +=-DMS_DVB_TYPE_SEL=$(CFG_DVB_TYPE)
SDK_C_FLAGS +=-DENABLE_DEBUG=1 
SDK_C_FLAGS +=-DSTB_ENABLE=1 
SDK_C_FLAGS +=-DFBL_ENABLE=0 
SDK_C_FLAGS +=-DMBOOT_LOGO_ENABLE=0
SDK_C_FLAGS +=-DOS_TYPE=ecos
SDK_C_FLAGS +=-DMSOS_TYPE_ECOS
SDK_C_FLAGS +=-DMCU_TYPE=mips34k
SDK_C_FLAGS +=-DDEBUGLEVEL=5
SDK_C_FLAGS +=-DCA_TYPE=NONE
SDK_C_FLAGS +=-DXRPC_MODULES_SERVER
SDK_C_FLAGS +=-DQC_svpldi
SDK_C_FLAGS +=-DQC_reset
SDK_C_FLAGS +=-DQC_qc_tests
SDK_C_FLAGS +=-DQC_nskldi
SDK_C_FLAGS +=-DQC_nskhal
SDK_C_FLAGS +=-DQC_harm_tests
SDK_C_FLAGS +=-DQC_cahdiver
SDK_C_FLAGS +=-DSOCKET_GLUE_SERVER
SDK_C_FLAGS +=-DLAN_EN=0

ifeq "$(PRODUCT_USE_USB)" "YES"
SDK_C_FLAGS += -DCFG_USB_ENABLE=1
SDK_C_FLAGS += -DUSBHOST_INIT
endif

SDK_C_FLAGS += -DENABLE_CAPVR
SDK_C_FLAGS += -DENABLE_STATICMEM_PVR

ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"
SDK_C_FLAGS += -DENABLE_MM_PACKAGE
endif

SDK_C_FLAGS += -DMUTE_SCREEN=1

ifeq "$(PRODUCT_PREBUILT_HAL)" "NO"
include $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/project/mstar_demo/cus_config/configure_define
include $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/project/mstar_demo/cus_config/configure_mstar_64MB
SDK_C_FLAGS += $(CFG_CC_OPTS)
SDK_C_FLAGS += $(CFG_CC_DEFS)
endif

# -------------- SDK Linker Flags ------------------

SDK_LD_FLAGS += \
-G0 -EL -T$(SDK_SRC_ROOT)/ecos_mipsisa32.ld

# -------------- SDK Include Paths ---------------------

ifeq "$(PRODUCT_PREBUILT_HAL)" "YES"
SDK_INCLUDE_PATH += $(SDK_ROOT)/inc/sdk
else
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/bsp/include
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver/iic
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver/pq/include
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver/demod
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver/tuner
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver/frontpnl
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/api/frontend
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/project/mstar_demo/
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/project/mstar_demo/include
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/project/mstar_demo/
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/bsp/lib/fw/

SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/platform/driver
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/gpio
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/lan
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/mm
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/pvr
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/sc
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/vdec
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/xc
SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/xml
endif

SDK_INCLUDE_PATH += $(SDK_SRC_ROOT)/libnocs1x_csd

# -------------- SDK libraries ---------------------

ifeq "$(PRODUCT_PREBUILT_SDK)" "YES"
SDK_LIBS += $(LIB_PREFFIX)mstar_sdk$(PRODUCT_HAL_SUFFIX).$(LIB_SUFFIX)
else
SDK_LIBS += $(LIB_PREFFIX)apiACP.$(LIB_SUFFIX) 
SDK_LIBS += $(LIB_PREFFIX)apiDLC.$(LIB_SUFFIX)  
SDK_LIBS += $(LIB_PREFFIX)apiPNL.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiCEC.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiGOP.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiJPEG.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiVDEC.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiAUDIO.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiXC.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiACE.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiHDMITX.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiDAC.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiDMX.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiGFX.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)apiSWI2C.$(LIB_SUFFIX)  	 
SDK_LIBS += $(LIB_PREFFIX)drvAUDSP.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvECOS_USBHOST.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvWBLE.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvSAR.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvIPAUTH.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvVBI.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvPM.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvDEMOD.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvURDMA.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvSC.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvGPIO.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvVE.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvHWI2C.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvWDT.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvPWS.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvSERFLASH.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvIR.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvRTC.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvDDC2BI.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)drvMVOP.$(LIB_SUFFIX)  
SDK_LIBS += $(LIB_PREFFIX)drvCPU.$(LIB_SUFFIX)   
SDK_LIBS += $(LIB_PREFFIX)drvPCMCIA.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)drvMSPI.$(LIB_SUFFIX) 
SDK_LIBS += $(LIB_PREFFIX)MsFS.$(LIB_SUFFIX) 
SDK_LIBS += $(LIB_PREFFIX)drvCIPHER.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)drvDSCMB2.$(LIB_SUFFIX) 
SDK_LIBS += $(LIB_PREFFIX)drvCA2.$(LIB_SUFFIX)

SDK_LIBS += $(LIB_PREFFIX)msAPI_XC.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)vdplayer.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)msAPI_GEGOP.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)msrv_mmplayer.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)zmplayer.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)MStarSDK.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)mwPVR.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)mwtspsr.$(LIB_SUFFIX)
#SDK_LIBS += $(LIB_PREFFIX)vdplayer2.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)mlogger.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)sdk_deps.$(LIB_SUFFIX)
endif

# NAGRA On CHIP SECURITY libraries
SDK_LIBS += $(LIB_PREFFIX)nocs1x_csd.$(LIB_SUFFIX)
SDK_LIBS += $(LIB_PREFFIX)drvOTP.$(LIB_SUFFIX)

# -------------- SDK libraries paths ---------------

SDK_LIBS_PATH += $(SDK_ROOT)/lib

ifeq "$(PRODUCT_PREBUILT_SDK)" "YES"
SDK_LIBS_PATH += $(SDK_ROOT)/lib/sdk/release
else
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/bsp/lib
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/api/frontend
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/DDI_Misc/cus_mstar/platform/driver
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/gpio
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/lan
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/mm
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/pvr
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/sc
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/vdec
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/xc
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/cus_mstar/api/xml
SDK_LIBS_PATH += $(SDK_SRC_ROOT)/Keltic/DDI_Package/ddi_pkg_ecos/tools/mipsisa32-elf/EL_mips32r2_mhard-float
endif

SDK_LIBS_PATH += $(SDK_SRC_ROOT)/libnocs1x_csd
