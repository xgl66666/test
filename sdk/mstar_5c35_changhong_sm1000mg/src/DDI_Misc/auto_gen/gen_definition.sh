#!/bin/bash

VAR_LIST=${AUTO_GEN_OUT}/var_list
source $VAR_LIST

echo >> $VAR_LIST
echo "### Definition in gen_definition ###" >> $VAR_LIST
if [ "$CFG_MEMORY_MAP" == "" ]
	then
		CFG_MEMORY_MAP=`grep -m1 "#define MEMORY_MAP" ../../../cus_mstar/project/mstar_demo/include/$CFG_BOARD.h | awk '{print $3}'`
		echo "CFG_MEMORY_MAP=$CFG_MEMORY_MAP" >> ${VAR_LIST}
fi

echo "function err_exit()" >> $VAR_LIST
echo "{" >> $VAR_LIST
echo "	echo \$1" >> $VAR_LIST
echo "	exit 1" >> $VAR_LIST
echo "}" >> $VAR_LIST

if [ "$CHIP" == "k1" ]
	then
		if [ "$FS_TYPE" != "ubifs" ]
			then
				if [ "$LINUX_KERNEL_VERSION" != "2.6.28" ]
					then
						echo "!!! File System ${FS_TYPE} only support Kernel version 2.6.28 !!!"
						exit 1
				fi
		fi
		echo "CFG_FOLDER_NAME=config" >> $VAR_LIST
elif [ "$CHIP" == "k2" ]
	then
		if [ "$FS_TYPE" != "ubifs" ]
			then
				if [ "$LINUX_KERNEL_VERSION" != "2.6.28" ]
					then
						echo "!!! File System ${FS_TYPE} only support Kernel version 2.6.28 !!!"
						exit 1
				fi
		fi
 		echo "CFG_FOLDER_NAME=conf" >> $VAR_LIST

    if [ "$MIU_INTERVAL" == "0x00000000" ]
      then
        MIU_TYPE=_single
    else
        MIU_TYPE=_dual
    fi
elif [ "$CHIP" == "kaiser" ]
	then
		echo "CFG_FOLDER_NAME=config" >> $VAR_LIST
		if [ "$CFG_MEMORY_MAP" == "MMAP_256MB_256MB" ]
			then
				MIU_TYPE=_256_256
		elif [ "$CFG_MEMORY_MAP" == "MMAP_512MB_512MB" ]
			then
				MIU_TYPE=_512_512
		fi
elif [ "$CHIP" == "keltic" ]
	then
		echo "CFG_FOLDER_NAME=application" >> $VAR_LIST
elif [ "$CHIP" == "keres" ]
	then
		echo "CFG_FOLDER_NAME=config" >> $VAR_LIST
elif [ "$CHIP" == "kirin" ]
	then
		echo "CFG_FOLDER_NAME=config" >> $VAR_LIST
elif [ "$CHIP" == "clippers" ]
	then
		if [ "$CFG_MEMORY_MAP" == "MMAP_512MB" ]
			then
				MIU_TYPE=_512
		elif [ "$CFG_MEMORY_MAP" == "MMAP_512MB_512MB" ]
			then
				MIU_TYPE=_512_512
		fi
		echo "CFG_FOLDER_NAME=config" >> $VAR_LIST
else
		echo "Unknow CHIP ${CHIP} !!"
		exit 1
fi

echo "MMAP_FILE=$MMAP_FILE" >> ${VAR_LIST}
echo "MIU_TYPE=$MIU_TYPE" >> ${VAR_LIST}

source $VAR_LIST
CC_OPTS=${AUTO_GEN_OUT}/application/auto_gen/cc_opts
echo "CC_OPTS=$CC_OPTS" >> $VAR_LIST
echo "CONF_INFO=${AUTO_GEN_OUT}/application/auto_gen/conf_info" >> $VAR_LIST
source $VAR_LIST
echo > $CONF_INFO

echo "### Get CC_OPTS info ###"
echo "$CFG_CC_OPTS" > ${CC_OPTS}
#echo > ${CC_OPTS}
#echo "$CFG_CC_OPTS" | awk '{for (i=1; i <= NF; i++) {printf $i"\n"}}' >> ${CC_OPTS}




# assign kernel image name & set partition ext name
if [ "$FS_TYPE" == "ramdisk" ]
	then
		if [ "$FLASH_TYPE" == "SPI" ]
			then
				PART_EXT_NAME=sqsh
		else
				PART_EXT_NAME=ubifs
		fi
else
		PART_EXT_NAME=${FS_TYPE}
fi

echo "KERNEL_START=0x81000000" >> $VAR_LIST
echo "PART_EXT_NAME=${PART_EXT_NAME}" >> $VAR_LIST
if [ "$MW_FLOW" == "enable" ]
	then
		echo "MW_FLOW=${MW_FLOW}" >> ${CONF_INFO}
		echo "CUST_SIZE=0x1500000" >> $VAR_LIST
else
		echo "CUST_SIZE=0" >> $VAR_LIST
fi

echo "TFTP_PATH=${BSP_PRJ}/bsp/linux/tftp_root" >> $VAR_LIST
echo "USB_PATH=${BSP_PRJ}/bsp/linux/usb_update" >> $VAR_LIST


echo >> $VAR_LIST
echo "### Get from platform_define ###" >> $VAR_LIST
source ${PRJ_DIR}/cus_config/platform_define || exit 1
echo "TARGET_PATH=${TARGET_PATH}" >> $VAR_LIST
echo "KERNEL_DEFAULT_SIZE=${KERNEL_DEFAULT_SIZE}" >> $VAR_LIST
echo "TAR_OPTION=${TAR_OPTION}" >> $VAR_LIST
echo "TAR_EXT_NAME=${TAR_EXT_NAME}" >> $VAR_LIST
echo "USB_UPDATE_CMD=${USB_UPDATE_CMD}" >> $VAR_LIST
echo "NAND_SIZE=${NAND_SIZE}" >> $VAR_LIST
echo "NAND_PAGE_SIZE=${NAND_PAGE_SIZE}" >> $VAR_LIST
echo "BUS_ADDR=${BUS_ADDR}" >> $VAR_LIST
echo "BB_ADDR=${BB_ADDR}" >> $VAR_LIST
echo "SHOW_CHK_MMAP=${SHOW_CHK_MMAP}" >> $VAR_LIST
echo "EXT_APP_SIZE=${EXT_APP_SIZE}" >> $VAR_LIST
echo "EXT_CONF_SIZE=${EXT_CONF_SIZE}" >> $VAR_LIST
echo "EXT_KERNEL_SIZE=${EXT_KERNEL_SIZE}" >> $VAR_LIST
echo "EXT_RFS_SIZE=${EXT_RFS_SIZE}" >> $VAR_LIST
echo "EXT_MWLIB_SIZE=${EXT_MWLIB_SIZE}" >> $VAR_LIST
echo "EXT_LIB_SIZE=${EXT_LIB_SIZE}" >> $VAR_LIST
echo "UTOPIA_PATH=${UTOPIA_PATH}" >> $VAR_LIST

# setup RESERVED_SIZE for different file system type
if [ "$FS_TYPE" == "ubifs" ]
	then
		if [ "$NAND_PAGE_SIZE" == "2048" ]
			then
				RESERVED_SIZE=$((126976*24))
		else
				RESERVED_SIZE=$((126976*24*2))
		fi
elif [ "$FS_TYPE" == "ext4" ]
	then
			RESERVED_SIZE=0x600000
else
			RESERVED_SIZE=0x300000
fi

if [ "$FLASH_TYPE" == "SPI" ]
  then
    RESERVED_SIZE=0
fi

echo "RESERVED_SIZE=${RESERVED_SIZE}" >> $VAR_LIST
