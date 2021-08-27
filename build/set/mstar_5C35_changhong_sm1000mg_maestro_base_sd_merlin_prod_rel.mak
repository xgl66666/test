# --------------------------------------
# ------ Configuration Makefile --------
# --------------------------------------

# ------ Application settings -----------
export PRODUCT_APPLICATION_PATH = $(APP_ROOT)/maestro-base-nx2-cab-sd
export PRODUCT_FLASH_MAP = map_base_maestro_mstar5c35_changhong_cak_dlk.xml
export PRODUCT_APPLICATION_PLAN = plan_resources_maestro_base_nx2_cab_sd.xml

# ------ Platform settings -----------
export PLATFORM_CHIP_VENDOR=mstar
export PLATFORM_CHIP_MODEL=5c35
export PLATFORM_BOARD_VENDOR=changhong
export PLATFORM_BOARD_MODEL=sm1000mg

# ------ HAL settings -----------
export PRODUCT_PREBUILT_SDK=YES
export PRODUCT_PREBUILT_HAL=YES
export PRODUCT_HAL_SUFFIX=_6Mb
export PRODUCT_OS=ecos

export PACK_TEST_USE_TDAL_FOR_LOW_LEVEL_ACCESS=1
export PRODUCT_USE_TDAL_FS=YES

# ------- Product settings ------------
export PRODUCT_MEDIUM=MEDIUM_CAB
export PRODUCT_ECOS_BUILD=ecos_keltic_34kf_fileio_posix_fat_c++_mipsisa32
export PRODUCT_NAME_DELIVERY=OpentTVOne
export PRODUCT_APP=MAESTRO
export PRODUCT_PROPRIETARY_BOOTLOADER=CHANGHONG
export PRODUCT_CUSTOMER_HEADEND=GTPL
export PRODUCT_USE_OTA=NO
export IMCL=YES
export COMEDIA_RESOURCES_IN_RAM=NO
export PRODUCT_KBD=GILHITS
export PRODUCT_PK_IN_RAM=YES
export SERVICE_LOGO=NO
export PRODUCT_NVOD=NO
export SUBSCRIPTION_RIGHTS=YES
export PRODUCT_HD=NO
export PRODUCT_BOOTLOADER=BOOTLOADER_OFF
export PRODUCT_DOWNLOADER=DOWNLOADER_BROADCAST
export PLATFORM_COMPACT=YES
export UNZIP_RAM_BASE=0x82F70044
#6 MB Maestro TKEL pool 
export TKEL_MEMORY_SIZE=_6Mb

# ------ Configure the CA stack -----------
export PRODUCT_USE_CA=YES
export PRODUCT_CCAS=NAGRA_MERLIN
export PRODUCT_CCAS_DLK=YES
export PRODUCT_CCAS_MODE_DLK=HYBRID
export PRODUCT_CAK_LIBVER=PROD_REL
export PRODUCT_USE_DMJFS=YES
export PRODUCT_DMJFS_IN_RAM=YES
export PRODUCT_DMJFS_ON_NAND=YES
export PRODUCT_DMJFS_NAND_WRITE_PAGE=YES
export CAK_LIBDIR=gtpl_mstar_5c35_changhong_sm1000mg
export USE_DLK_1_1=YES
export CAK_DAL_TRACE=OFF

# ------ Packages settings -----------
export PRODUCT_FEATURE_PVR=PVR_EXTERNAL_FREEVIEW
export PVR_CORE_IMPLEMENTATION=TDAL_PVR
export PRODUCT_FONT_ENGINE=FONT_ENGINE_CDAC
export USE_CDAC=YES
export NASC=0
export PRODUCT_USE_FP=NO
export PRODUCT_MEDIA_VIEWER=NO
export PRODUCT_USE_MEDIA_BROWSER=NO
export PRODUCT_USE_DDM=YES
export USE_FREETYPE2=YES
export USE_FREETYPE2_CACHE=YES
export PRODUCT_USE_HDCP=NO
export MHEG5_VERSION=NONE
export PRODUCT_USE_SDEC=YES
export PRODUCT_AUX_VIDEO_DISPLAY_SD=NO
export PRODUCT_PERMANENT_SI_MONITORING=NO
export SUPPRESS_FTA=YES
export PRODUCT_BOOL_TYPE=int


# ------- versions ----------
export PRODUCT_MW_VER?=0.0.0
export PRODUCT_APP_VER?=1.0.0
export PRODUCT_CUST_VER?=1.0.0
export PRODUCT_VERSION_DAY?=20
export PRODUCT_VERSION_MONTH?=12 
export PRODUCT_VERSION_YEAR?=2016  

# ------- EIM ----------
export EIT_MEM_SIZE=0x800000
export ECC_CACHE_LENGTH=12
export PRODUCT_PREBUILT_APPLICATION=YES
