# ****************************************
# co'media system makefile
# ****************************************
include $(TDAL_MAKE)/environnement.mak
include $(TDAL_MAKE)/tdal.mak

# ****************************************
# Define module name
# ****************************************
MODULE  = tdal$(PRODUCT_HAL_SUFFIX)

# ****************************************
# If using RPC protocol OBM TDAL_KBD not
# based on standard architecture drivers
# but only on TDAL_RPC
# ****************************************
ifeq "$(PRODUCT_USE_RPC_PROTOCOL)" "YES"
KBD_PRODUCT_ARCH = RPC
else
KBD_PRODUCT_ARCH = $(PRODUCT_ARCH)
endif

# ****************************************
# This define the default command for gmake
# ****************************************
all: $(TDAL_LIST) \
	extract rebuild


# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE =

LIBOBJS = $(TDAL_LIST_LIBOBJS)

# ****************************************
# List of dependencies
# ****************************************
$(LIB_PREFFIX)tdal_ata.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_ata

$(LIB_PREFFIX)tdal_av.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_av

$(LIB_PREFFIX)tdal_avs.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_avs

$(LIB_PREFFIX)tdal_common.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_common

$(LIB_PREFFIX)tdal_ci.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_ci

$(LIB_PREFFIX)tdal_desc.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_desc

$(LIB_PREFFIX)tdal_crypt.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_crypt

ifeq "$(PRODUCT_USE_CA)" "YES"
$(LIB_PREFFIX)tdal_sec.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_sec
endif

$(LIB_PREFFIX)tdal_diag.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_diag

$(LIB_PREFFIX)tdal_disp.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_disp

$(LIB_PREFFIX)tdal_dmd.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_dmd

$(LIB_PREFFIX)tdal_dmx.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_dmx

$(LIB_PREFFIX)tdal_fla.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_fla

$(LIB_PREFFIX)tdal_fp.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_fp

$(LIB_PREFFIX)tdal_fs.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_fs

$(LIB_PREFFIX)tdal_gfx.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_gfx

$(LIB_PREFFIX)tdal_kbd.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_kbd

$(LIB_PREFFIX)tdal_output.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_output

ifeq "$(PRODUCT_USE_RPC)" "YES"	
$(LIB_PREFFIX)tdal_rpc.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_rpc
endif

ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"	
$(LIB_PREFFIX)tdal_mp.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_mp
endif

ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"	
$(LIB_PREFFIX)tdal_pvr.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_pvr
endif

$(LIB_PREFFIX)tdal_nvm.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_nvm

$(LIB_PREFFIX)tdal_ptsm.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_ptsm

$(LIB_PREFFIX)tdal_smc.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_smc

$(LIB_PREFFIX)tdal_ts.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_ts

$(LIB_PREFFIX)tdal_tsin.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_tsin

$(LIB_PREFFIX)tdal_hdmi.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_hdmi

$(LIB_PREFFIX)tdal_uart.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_uart

$(LIB_PREFFIX)tdal_vbi.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_vbi
	
$(LIB_PREFFIX)tdal_perso.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_perso

$(LIB_PREFFIX)tdal_ssu.$(LIB_SUFFIX):
	$(MAKE) -C $(TDAL_SRC_ROOT)/tdal_ssu
    
# ****************************************
# Always include crules.mak
# ****************************************
include $(TDAL_MAKE)/crules.mak

clean_all :
#	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_ata clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_av  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_avs  clean
ifeq "$(PRODUCT_USE_CI)" "YES"
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_ci  clean
endif
ifeq "$(PRODUCT_USE_CIPLUS)" "YES"
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_ci  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_desc  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_sec  clean
endif
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_common  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_crypt  clean
ifeq "$(PRODUCT_USE_CA)" "YES"
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_desc  clean
endif
ifeq "$(USE_TDAL_DIAG)" "ON"
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_diag  clean
endif
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_dmd  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_dmx  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_fla  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_fp  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_fs  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_gfx  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_hdmi  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_kbd  clean
ifeq "$(PRODUCT_USE_RPC)" "YES"	
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_rpc  clean
endif
ifeq "$(PRODUCT_USE_TDAL_MP)" "YES"	
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_mp  clean
endif
ifeq "$(PRODUCT_USE_TDAL_PVR)" "YES"	
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_pvr  clean
endif
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_nvm  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_ptsm  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_smc  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_perso  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_sec  clean
ifeq "$(PRODUCT_USE_TDAL_TS)" "USE_TDAL_TS_ON"
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_ts  clean
else
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_tsin  clean
endif
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_uart  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_vbi  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_disp  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_output  clean
	$(MAKE) -ikC $(TDAL_SRC_ROOT)/tdal_ssu  clean
	$(RM)	*.$(LIB_SUFFIX)
	
