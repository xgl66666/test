# ****************************************
# build main makefile
# ****************************************

include $(SDK_ROOT)/drivers.mak
include $(BUILD_MAKE)/environnement.mak
include $(BUILD_MAKE)/tdal.mak

# ****************************************
# Define module name
# ****************************************

MODULE=configuration

ifeq "$(MAIN_CHALVALIDATOR)" "1"
    MODULE=$(PRODUCT_MAIN_CHALVALIDATOR)
endif

# ****************************************
# This define the default command for gmake
# ****************************************

# ****************************************
# Include paths needed to compile those files
# ****************************************
DIR_INCLUDE_MAIN = \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(addprefix $(INCLUDE_PREFIX), $(SDK_INCLUDE_PATH)) \
	$(INCLUDE_PREFIX)$(MW_ROOT_DIR)/core/include \
	$(INCLUDE_PREFIX)$(DATETIME_INCLUDE) \
	$(INCLUDE_PREFIX)$(PIDB_INCLUDE) \
    $(INCLUDE_PREFIX)$(SLIST_INCLUDE) \
	$(INCLUDE_PREFIX)$(TABLE_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_GFX_INCLUDE) \
    $(INCLUDE_PREFIX)$(AV_CLUSTER_INCLUDE) \
    $(INCLUDE_PREFIX)$(ZLIB_INCLUDE) \
	$(TDAL_TEST_INCLUDE)
ifeq "$(PRODUCT_APP)" "MAESTRO"
 DIR_INCLUDE_MAIN  += $(INCLUDE_PREFIX)$(MAESTRO_PLAYER_GLUE_INCLUDE) 
endif    
DIR_INCLUDE_MAIN_BOOTLOADER = \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
	$(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TDAL_FLA_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_NVM_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_UART_INCLUDE) \
	$(INCLUDE_PREFIX)$(TDAL_FP_INCLUDE) \
	$(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
	$(INCLUDE_PREFIX)$(TKEL_INCLUDE) \
    $(INCLUDE_PREFIX)$(ZLIB_INCLUDE) \
	$(INCLUDE_PREFIX)$(BOOTLOADER_CORE_INCLUDE)

DIR_INCLUDE_MAIN_DALTS = \
    $(addprefix $(INCLUDE_PREFIX), $(OS_INCLUDE_PATH)) \
    $(INCLUDE_PREFIX)$(TDAL_COMMON_INCLUDE) \
    $(INCLUDE_PREFIX)$(TBOX_INCLUDE) \
    $(INCLUDE_PREFIX)$(TKEL_INCLUDE) \
    $(INCLUDE_PREFIX)$(ZLIB_INCLUDE) \
    $(INCLUDE_PREFIX)$(CAK_INCLUDE) \
    $(INCLUDE_PREFIX)$(CAK_DAL_INCLUDE)

ifeq "$(PRODUCT_USE_DMJFS)" "YES"
    DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(DMJFS_INCLUDE)
    DIR_INCLUDE_MAIN_DALTS += $(INCLUDE_PREFIX)$(DMJFS_INCLUDE)
endif

ifeq "$(PRODUCT_USE_XPEL)" "YES"
    DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(XPEL_INCLUDE)
endif

DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(MW_ROOT_DIR)/interface/include

ifneq "$(PRODUCT_TRIGGER)" "TRIGGER_OFF"
    DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(TRIGGER_INCLUDE)
    DIR_INCLUDE_MAIN_DALTS += $(INCLUDE_PREFIX)$(TRIGGER_INCLUDE)
endif
ifeq "$(PRODUCT_USE_RPC)" "YES"
  DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(TDAL_RPC_INCLUDE) \
                        $(INCLUDE_PREFIX)$(TDAL_DMD_INCLUDE) \
                        $(INCLUDE_PREFIX)$(TDAL_AV_INCLUDE) \
                        $(INCLUDE_PREFIX)$(TDAL_VBI_INCLUDE) \
                        $(INCLUDE_PREFIX)$(GFX_INCLUDE) \
                        $(INCLUDE_PREFIX)$(COMEDIA_ROOT)/system/src/sys_mngr
ifeq "$(PRODUCT_APP)" "MAESTRO"
ifneq "$(PRODUCT_EMBEDDED)" "YES"
  DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(MAESTRO_PLAYER_GLUE_INCLUDE) \
                    $(INCLUDE_PREFIX)$(MAESTRO_PLAYER_PLUGIN_INCLUDE)
endif
endif
endif
ifneq "$(PRODUCT_EIT)" "NO_EIT"
  DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(PACKAGE_EIM_INCLUDE)
endif
ifeq "$(PRODUCT_TELETEXT_OSD)" "TTXT_OSD_ON"
  DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(TTXT_INCLUDE)
endif
ifeq "$(PRODUCT_ENGINE)" "ENGINE_TER_MHEG"
  DIR_INCLUDE_MAIN += $(INCLUDE_PREFIX)$(MHEG5_GLUE_INCLUDE)
endif

# DIR_INCLUDE rule
DIR_INCLUDE = $(DIR_INCLUDE_MAIN_BOOTLOADER)
ifneq "$(MAIN_BOOTLOADER)" "1"
    DIR_INCLUDE += $(DIR_INCLUDE_MAIN)
endif

ifeq "$(PRODUCT_USE_CA)" "YES"
    DIR_INCLUDE += $(INCLUDE_PREFIX)$(CAK_INCLUDE)
    DIR_INCLUDE += $(INCLUDE_PREFIX)$(CCAS_INCLUDE)
endif

ifeq "$(PACKAGE_FILE_SYSTEM)" "YES"
    DIR_INCLUDE += $(INCLUDE_PREFIX)$(FILESYS_INCLUDE)
endif

ifeq "$(MAIN_DALTS)" "1"
    DIR_INCLUDE = $(DIR_INCLUDE_MAIN_DALTS)
endif

# ***************************************************
# List of object files needed to create library
# ***************************************************
LIBOBJS = \
        version_cfg.$(OBJ_SUFFIX) \
        flash_cfg.$(OBJ_SUFFIX) \
        flipflop_cfg.$(OBJ_SUFFIX) \
        ruxi.$(OBJ_SUFFIX)


ifeq "$(PRODUCT_ENGINE)" "ENGINE_TER_MHEG"
    LIBOBJS += mheg_cfg.$(OBJ_SUFFIX)
endif

ifeq "$(MAIN_BOOTLOADER)" "1"
    LIBOBJS +=  \
                integrity_cfg.$(OBJ_SUFFIX) \
                nvm_exchange.$(OBJ_SUFFIX) \
                bootloader_cfg.$(OBJ_SUFFIX)\
                rsa_crypt.$(OBJ_SUFFIX)
else
ifeq "$(PRODUCT_ENGINE)" "ENGINE_WOTV"
    LIBOBJS += wotv_cfg.$(OBJ_SUFFIX)
endif

ifeq "$(MAIN)" "1"
    LIBOBJS += sys_ui_mngr_kbd_mapping_cfg.$(OBJ_SUFFIX) \
                sys_ui_mngr_fp_mapping_cfg.$(OBJ_SUFFIX) \
                play_glue_evt_cfg.$(OBJ_SUFFIX) \
                country_cfg.$(OBJ_SUFFIX) \
                file_system_cfg.$(OBJ_SUFFIX) \
                config_file.$(OBJ_SUFFIX) \
                accessctrl_cfg.$(OBJ_SUFFIX)\
                appli_cfg.$(OBJ_SUFFIX)\
                sys_mngr_cfg.$(OBJ_SUFFIX)\
                slist_cfg.$(OBJ_SUFFIX)\
                display_cfg.$(OBJ_SUFFIX)\
                backup_mngr_cfg.$(OBJ_SUFFIX)\
                medium_cfg.$(OBJ_SUFFIX)
    ifeq "$(PRODUCT_USE_CA)" "YES"    
        LIBOBJS += dlk_cfg.$(OBJ_SUFFIX)
    endif   
endif

ifeq "$(MAIN_PACK_TEST)" "1"
    LIBOBJS += sys_ui_mngr_kbd_mapping_cfg.$(OBJ_SUFFIX) \
	sys_ui_mngr_fp_mapping_cfg.$(OBJ_SUFFIX) \
    country_cfg.$(OBJ_SUFFIX) \
	accessctrl_cfg.$(OBJ_SUFFIX)\
	display_cfg.$(OBJ_SUFFIX)\
    backup_mngr_cfg.$(OBJ_SUFFIX) \
	appli_cfg.$(OBJ_SUFFIX) \
	sys_mngr_cfg.$(OBJ_SUFFIX)\
	play_glue_evt_cfg.$(OBJ_SUFFIX) \
	slist_cfg.$(OBJ_SUFFIX)\
	medium_cfg.$(OBJ_SUFFIX)
  ifeq "$(PRODUCT_USE_CA)" "YES"	
    LIBOBJS += dlk_cfg.$(OBJ_SUFFIX)
  endif  
  ifeq "$(PRODUCT_USE_DMJFS)" "YES"
    LIBOBJS += file_system_cfg.$(OBJ_SUFFIX)
  endif
endif

ifeq "$(MAIN_FLASH_TOOL)" "1"
  ifeq "$(PRODUCT_USE_DMJFS)" "YES"
    LIBOBJS += file_system_cfg.$(OBJ_SUFFIX)
  endif
endif

LIBOBJS += av_cluster_cfg.$(OBJ_SUFFIX)

LIBOBJS += datetime_cfg.$(OBJ_SUFFIX)
ifneq "$(origin TRACES)" "undefined"
LIBOBJS += trace_cfg.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_USE_RPC)" "YES"
LIBOBJS += rpc_cfg.$(OBJ_SUFFIX)
LIBOBJS += sys_mngr_rpc_cmd.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_CAB"
LIBOBJS += cabinstall_cfg.$(OBJ_SUFFIX)
endif
ifneq "$(PRODUCT_EPG_ENGINE)" "EPG_ENGINE"
ifneq "$(PRODUCT_EIT)" "NO_EIT"
LIBOBJS += sys_mngr_eim_cfg.$(OBJ_SUFFIX)
endif
endif
ifneq "$(PRODUCT_TRIGGER)" "TRIGGER_OFF"
    LIBOBJS += nvm_interface_bootloader.$(OBJ_SUFFIX) \
               trigger_cfg.$(OBJ_SUFFIX) \
               sys_mngr_trigger_cfg.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_TER"
    LIBOBJS += terinstall_cfg.$(OBJ_SUFFIX)
  ifdef _ANALOG_
    LIBOBJS += terinstall_analog_cfg.$(OBJ_SUFFIX)
  endif
endif
ifeq "$(PRODUCT_TELETEXT_OSD)" "TTXT_OSD_ON"
    LIBOBJS += teletext_cfg.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_SUBTITLE)" "SUBT_ON"
    LIBOBJS += dvb_subtitle_cfg.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_PVR)" "PVR_ON"
    LIBOBJS += pvr_cfg.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_USE_XPEL)" "YES"
    LIBOBJS += xpel_cfg.$(OBJ_SUFFIX)
endif
endif

ifeq "$(PRODUCT_MEDIUM)" "MEDIUM_SAT"
    LIBOBJS += satinstall_cfg.$(OBJ_SUFFIX)
endif

ifeq "$(PRODUCT_USE_CA)" "YES"
  ifeq "$(CA_PACK_CRYPTO)" "1"
    LIBOBJS += ca_cfg\crypto_cfg\cryptommi_lexic_cfg.$(OBJ_SUFFIX)
  endif

  ifeq "$(CA_PACK_CONAX)" "1"
    LIBOBJS += ca_cfg\conax_cfg\cnxmmi_lexic_cfg.$(OBJ_SUFFIX)
    LIBOBJS += ca_cfg\conax_cfg\ccas_cnx_cfg.$(OBJ_SUFFIX)
  endif

  ifeq "$(CA_PACK_IRDETO)" "1"
    LIBOBJS += ca_cfg\irdeto_cfg\rdtmmi_lexic_cfg.$(OBJ_SUFFIX)
  endif

  ifeq "$(CA_PACK_NAGRA)" "1"
    LIBOBJS += ca_cfg/nagra_cfg/nagra_cfg.$(OBJ_SUFFIX)
  endif

  ifeq "$(CA_PACK_VIACCESS)" "1"
    LIBOBJS += ca_cfg\viaccess_cfg\viaccess_mmi_lexic_cfg.$(OBJ_SUFFIX)
  endif
endif

ifeq "$(PRODUCT_USE_DMJFS)" "YES"
    LIBOBJS += dmjfs_cfg.$(OBJ_SUFFIX)
endif

ifeq "$(MAIN_CHALVALIDATOR)" "1"
LIBOBJS = $(PRODUCT_MAIN_CHALVALIDATOR).$(OBJ_SUFFIX) \
          version_cfg.$(OBJ_SUFFIX) \
		  flipflop_cfg.$(OBJ_SUFFIX)\
          flash_cfg.$(OBJ_SUFFIX)
ifeq "$(PRODUCT_USE_RPC)" "YES"
  LIBOBJS += rpc_cfg.$(OBJ_SUFFIX)
endif
endif

ifeq "$(PRODUCT_MEDIA_VIEWER)" "YES"
    LIBOBJS += media_renderer_cfg.$(OBJ_SUFFIX)
    LIBOBJS += media_viewer_cfg.$(OBJ_SUFFIX)
    LIBOBJS += media_browser_cfg.$(OBJ_SUFFIX)
endif
ifeq "$(PRODUCT_APP)" "MAESTRO"
ifeq "$(PRODUCT_MAESTRO_PORTAL)" "PORTAL_ON"
	LIBOBJS += mstr_interactive\mstr_interactive_cfg.$(OBJ_SUFFIX)
endif
endif
ifeq "$(MAIN_DALTS)" "1"
LIBOBJS = \
          version_cfg.$(OBJ_SUFFIX) \
          flash_cfg.$(OBJ_SUFFIX) \
		  flipflop_cfg.$(OBJ_SUFFIX)
  ifeq "$(PRODUCT_USE_DMJFS)" "YES"
    LIBOBJS += file_system_cfg.$(OBJ_SUFFIX)
  endif
  ifneq "$(origin TRACES)" "undefined"
    LIBOBJS += trace_cfg.$(OBJ_SUFFIX)
  endif
endif

# ****************************************
# List of dependencies
# ****************************************
$(PRODUCT_MAIN).$(OBJ_SUFFIX): $(PRODUCT_MAIN).c makefile
$(PRODUCT_MAIN_TDAL_APP_TEST).$(OBJ_SUFFIX): $(PRODUCT_MAIN_TDAL_APP_TEST).c makefile
$(PRODUCT_MAIN_CHALVALIDATOR).$(OBJ_SUFFIX): $(PRODUCT_MAIN_CHALVALIDATOR).c makefile
$(LIB_PREFFIX)$(PRODUCT_MAIN_CHALVALIDATOR).$(LIB_SUFFIX): $(PRODUCT_MAIN_CHALVALIDATOR).$(OBJ_SUFFIX) makefile
	$(AR) -rcs  $(LIB_PREFFIX)$(PRODUCT_MAIN_CHALVALIDATOR).$(LIB_SUFFIX) $(PRODUCT_MAIN_CHALVALIDATOR).$(OBJ_SUFFIX)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_ROOT)/src/$(LIB_PREFFIX)$(PRODUCT_MAIN_CHALVALIDATOR).$(LIB_SUFFIX)) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BUILD_EXPORTS))
$(PRODUCT_MAIN_PACK_TEST).$(OBJ_SUFFIX): $(PRODUCT_MAIN_PACK_TEST).c makefile
$(PRODUCT_MAIN_DALTS).$(OBJ_SUFFIX): $(PRODUCT_MAIN_DALTS).c makefile
$(PRODUCT_MAIN_APPDEMO).$(OBJ_SUFFIX): $(PRODUCT_MAIN_APPDEMO).c makefile
$(PRODUCT_MAIN_FLASH_TOOL).$(OBJ_SUFFIX): $(PRODUCT_MAIN_FLASH_TOOL).c makefile
$(PRODUCT_MAIN_FSTP).$(OBJ_SUFFIX): $(PRODUCT_MAIN_FSTP).c makefile
$(PRODUCT_MAIN_BOOTLOADER).$(OBJ_SUFFIX): $(PRODUCT_MAIN_BOOTLOADER).c makefile
sys_ui_mngr_kbd_mapping_cfg.$(OBJ_SUFFIX): sys_ui_mngr_kbd_mapping_cfg.c sys_ui_mngr_kbd_mapping_cfg_p.h makefile
sys_ui_mngr_fp_mapping_cfg.$(OBJ_SUFFIX): sys_ui_mngr_fp_mapping_cfg.c makefile
play_glue_evt_cfg.$(OBJ_SUFFIX): play_glue_evt_cfg.c makefile
version_cfg.$(OBJ_SUFFIX): version_cfg.c makefile
config_file.$(OBJ_SUFFIX): config_file.c makefile
trigger_cfg.$(OBJ_SUFFIX): trigger_cfg.c makefile
nvm_exchange.$(OBJ_SUFFIX): nvm_exchange.c makefile
mheg_cfg.$(OBJ_SUFFIX): mheg_cfg.c makefile
sys_mngr_trigger_cfg.$(OBJ_SUFFIX): sys_mngr_trigger_cfg.c makefile
rsa_crypt.$(OBJ_SUFFIX): rsa_crypt.c makefile
cabinstall_cfg.$(OBJ_SUFFIX): cabinstall_cfg.c makefile
sys_mngr_eim_cfg.$(OBJ_SUFFIX): sys_mngr_eim_cfg.c makefile
country_cfg.$(OBJ_SUFFIX) : country_cfg.c makefile
terinstall_cfg.$(OBJ_SUFFIX) : terinstall_cfg.c makefile
terinstall_analog_cfg.$(OBJ_SUFFIX) : terinstall_analog_cfg.c makefile
rpc_cfg.$(OBJ_SUFFIX): rpc_cfg.c makefile
accessctrl_cfg.$(OBJ_SUFFIX) : accessctrl_cfg.c makefile
appli_cfg.$(OBJ_SUFFIX) : appli_cfg.c makefile
sys_mngr_cfg.$(OBJ_SUFFIX) : sys_mngr_cfg.c makefile
slist_cfg.$(OBJ_SUFFIX) : slist_cfg.c makefile
xpel_cfg.$(OBJ_SUFFIX) : xpel_cfg.c makefile
backup_mngr_cfg.$(OBJ_SUFFIX) : backup_mngr_cfg.c makefile
teletext_cfg.$(OBJ_SUFFIX) : teletext_cfg.c makefile
dvb_subtitle_cfg.$(OBJ_SUFFIX) : dvb_subtitle_cfg.c makefile
flash_cfg.$(OBJ_SUFFIX) : flash_cfg.c makefile
flipflop_cfg.$(OBJ_SUFFIX): flipflop_cfg.c makefile
datetime_cfg.$(OBJ_SUFFIX) : datetime_cfg.c makefile
satinstall_cfg.$(OBJ_SUFFIX): satinstall_cfg.c makefile
trace_cfg.$(OBJ_SUFFIX) : trace_cfg.c makefile
pvr_cfg.$(OBJ_SUFFIX) : pvr_cfg.c makefile
dmjfs_cfg.$(OBJ_SUFFIX) : dmjfs_cfg.c makefile
display_cfg.$(OBJ_SUFFIX) : display_cfg.c makefile
media_renderer_cfg.$(OBJ_SUFFIX) : media_renderer_cfg.c makefile
media_viewer_cfg.$(OBJ_SUFFIX) : media_viewer_cfg.c makefile
media_browser_cfg.$(OBJ_SUFFIX) : media_browser_cfg.c makefile
av_cluster_cfg.$(OBJ_SUFFIX) : av_cluster_cfg.c makefile
wotv_cfg.$(OBJ_SUFFIX) : wotv_cfg.c makefile
ifeq "$(COMEDIA_RESOURCES_IN_RAM)" "YES"
file_system_cfg.$(OBJ_SUFFIX) : file_system_cfg.c fs_cfg_myRAMPartitionA.h makefile
else
file_system_cfg.$(OBJ_SUFFIX) : file_system_cfg.c makefile
endif
ifeq "$(PRODUCT_USE_CA)" "YES"
dlk_cfg.$(OBJ_SUFFIX) : dlk_cfg.c makefile
endif
ca_cfg\irdeto_cfg\rdtmmi_lexic_cfg.$(OBJ_SUFFIX) : ca_cfg\irdeto_cfg\rdtmmi_lexic_cfg.c makefile
ca_cfg\conax_cfg\cnxmmi_lexic_cfg.$(OBJ_SUFFIX) : ca_cfg\conax_cfg\cnxmmi_lexic_cfg.c makefile
ca_cfg\conax_cfg\ccas_cnx_cfg.$(OBJ_SUFFIX) : ca_cfg\conax_cfg\ccas_cnx_cfg.c makefile
ca_cfg\crypto_cfg\cryptommi_lexic_cfg.$(OBJ_SUFFIX) : ca_cfg\crypto_cfg\cryptommi_lexic_cfg.c makefile
ca_cfg\viaccess_cfg\viaccess_mmi_lexic_cfg.$(OBJ_SUFFIX) : ca_cfg\viaccess_cfg\viaccess_mmi_lexic_cfg.c makefile

mstr_interactive\mstr_interactive_cfg.$(OBJ_SUFFIX) : mstr_interactive\mstr_interactive_cfg.c makefile

# ****************************************
clean_all: clean
ifeq "$(PRODUCT_USE_CA)" "YES"
	$(RM)	$(subst $(BAD_SLASH),$(GOOD_SLASH),ca_cfg\crypto_cfg\*.$(OBJ_SUFFIX))
	$(RM)	$(subst $(BAD_SLASH),$(GOOD_SLASH),ca_cfg\conax_cfg\*.$(OBJ_SUFFIX))
	$(RM)	$(subst $(BAD_SLASH),$(GOOD_SLASH),ca_cfg\irdeto_cfg\*.$(OBJ_SUFFIX))
	$(RM)	$(subst $(BAD_SLASH),$(GOOD_SLASH),ca_cfg\nagra_cfg\*.$(OBJ_SUFFIX))
endif
ifeq "$(PRODUCT_APP)" "MAESTRO"
ifeq "$(PRODUCT_MAESTRO_PORTAL)" "PORTAL_ON"
	$(RM)	$(subst $(BAD_SLASH),$(GOOD_SLASH),mstr_interactive\*.$(OBJ_SUFFIX))
endif
endif
clean_trace:
	$(RM)	$(subst $(BAD_SLASH),$(GOOD_SLASH),trace_cfg.$(OBJ_SUFFIX))

# ****************************************
# Always include crules.mak
# ****************************************
include $(BUILD_MAKE)/crules.mak

