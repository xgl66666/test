#sh auto_gen/gen_auto_update.sh $UPDATE_FILE mstar  <<-- example of each parameter
#   $0                          $1           $2

source ${AUTO_GEN_OUT}/var_list

echo "# <- this is for comment / total file size must be less than 4KB" > $1

if [ "$CHIP" == "k1" ]
	then
		if [ "$2" == "mstar" ]
			then
				MIU_CMD=net_miu
		else
				MIU_CMD=usb_miu
		fi
else
		MIU_CMD=$2
fi


if [ "$FLASH_TYPE" == "SPI" ]
	then
		if [ "$CHIP" == "kratos" ]; then
		    if [ "$2" == "udstar" ]; then
    			SCRIPT_PATH=/Target/script
    		else
    			SCRIPT_PATH=.
    		fi
		
    		echo "setenv mtdparts;saveenv;spi rmgpt;spi ptinit;" >> $1
    		echo >> $1
    		echo "#set partition" >> $1
    		echo "$2 ${SCRIPT_PATH}/set_partition" >> $1
    		echo >> $1
    		echo "#kernel" >> $1
    		echo "$2 ${SCRIPT_PATH}/[[kernel" >> $1
    		echo >> $1
    		if [ "${FS_TYPE}" != "ramdisk" ]
    			then
    				echo "#ROOTFS" >> $1
    				echo "$2 ${SCRIPT_PATH}/[[ROOTFS" >> $1
    				echo >> $1
    		fi
    		echo "#conf" >> $1
    		echo "$2 ${SCRIPT_PATH}/[[config" >> $1
    		echo >> $1
    		
    		echo "#application" >> $1
    		echo "$2 ${SCRIPT_PATH}/[[application" >> $1
    		echo >> $1
    		
    		if [ "$RELEASE_MW_LIB" == "enable" ]; then
        		echo "#mm.so & mwlib" >> $1
        		echo "$2 ${SCRIPT_PATH}/[[mwlib" >> $1
        		echo >> $1
        	fi
        	
    		if [ "$LINK_TYPE" == "dynamic" ]; then
        		echo "#libdynamic" >> $1
        		echo "$2 ${SCRIPT_PATH}/[[libdynamic" >> $1
        		echo >> $1
        	fi
    		
    		echo "#config & bootargs" >> $1
    		echo "$2 ${SCRIPT_PATH}/set_config" >> $1
    		echo >> $1
    		
    		echo "updatemiureg ${SCRIPT_PATH}/miu_setting${MIU_TYPE}.txt" >> $1
    		echo >> $1
    		echo "printenv" >> $1
    		if [ "$2" == "ustar" ] || [ "$2" == "udstar" ]
    			then
    				echo "setenv usb_complete 1" >> $1
    				echo "saveenv" >> $1
    		fi
		else
    		echo "setenv mtdparts;saveenv;spi_rmgpt;spi_ptinit;" >> $1
    		echo >> $1
    		echo "#set partition" >> $1
    		echo "$2 ${SCRIPT_PATH}/set_partition" >> $1
    		echo >> $1
    		echo "#kernel" >> $1
    		echo "$2 ${SCRIPT_PATH}/[[kernel" >> $1
    		echo >> $1
    		if [ "${FS_TYPE}" != "ramdisk" ]
    			then
    				echo "#ROOTFS" >> $1
    				echo "$2 ${SCRIPT_PATH}/[[ROOTFS" >> $1
    				echo >> $1
    				echo "#application" >> $1
    				echo "$2 ${SCRIPT_PATH}/[[application" >> $1
    				echo >> $1
    		fi
    		echo "#config & bootargs" >> $1
    		echo "$2 ${SCRIPT_PATH}/set_config" >> $1
    		echo >> $1
    		echo "usb_miu ${SCRIPT_PATH}/miu_setting${MIU_TYPE}.txt" >> $1
    		echo >> $1
    		echo "printenv" >> $1
    		if [ "$2" == "ustar" ]
    			then
    				echo "setenv usb_complete 1" >> $1
    				echo "saveenv" >> $1
    		fi
		fi
else
		if [ "$2" == "udstar" ]
			then
				SCRIPT_PATH=/Target/script
		else
				SCRIPT_PATH=.
		fi

		if [ "$FLASH_TYPE" == "NAND" ]
			then
				echo "nand erase" >> $1
		else
            if [ "$BOOT_MODE" != "EMMC" ]
                then
                    echo "mmc erase" >> $1
            fi
				echo "mmc slc 0 1" >> $1
				echo "mmc rmgpt" >> $1
		fi
		echo >> $1
		echo "#set partition" >> $1
		echo "$2 ${SCRIPT_PATH}/set_partition" >> $1
		echo >> $1
		echo "#kernel" >> $1
		echo "$2 ${SCRIPT_PATH}/[[kernel" >> $1
		echo >> $1
		if [ "$CHIP" == "muji" ]
			then
		echo "#dtb" >> $1
		echo "$2 ${SCRIPT_PATH}/[[ld_dtb" >> $1
		echo >> $1
		fi
		if [ "${FS_TYPE}" != "ramdisk" ]
			then
				echo "#ROOTFS" >> $1
				echo "$2 ${SCRIPT_PATH}/[[ROOTFS" >> $1
				echo >> $1
		fi
		echo "#conf" >> $1
		echo "$2 ${SCRIPT_PATH}/[[${CFG_FOLDER_NAME}" >> $1
		echo >> $1
		echo "#application" >> $1
		echo "$2 ${SCRIPT_PATH}/[[application" >> $1
		echo >> $1
		if [ "$RELEASE_MW_LIB" == "enable" ]
			then
				echo "#mm.so & mwlib" >> $1
				echo "$2 ${SCRIPT_PATH}/[[mwlib" >> $1
				echo >> $1
		fi
		if [ "$LINK_TYPE" == "dynamic" ]
			then
				echo "#libdynamic" >> $1
				echo "$2 ${SCRIPT_PATH}/[[libdynamic" >> $1
				echo >> $1
		fi
		if [ "$DDI_TEE" == "enable" ]; then
            echo "#tee" >> $1
            echo "$2 ${SCRIPT_PATH}"/[[tee >> $1
            echo >> $1
        fi
		if [ "$MW_FLOW" == "enable" ]
			then
				echo "#customer" >> $1
				echo "$2 ${SCRIPT_PATH}/[[customer" >> $1
				echo >> $1
		fi
		echo "#config & bootargs" >> $1
		echo "$2 ${SCRIPT_PATH}/set_config" >> $1
		echo >> $1
		echo "$MIU_CMD ${SCRIPT_PATH}/miu_setting${MIU_TYPE}.txt" >> $1
		echo >> $1
		echo "printenv" >> $1
		if [ "$2" == "ustar" ] || [ "$2" == "udstar" ]
			then
				echo "setenv usb_complete 1" >> $1
				echo "saveenv" >> $1
		fi
fi

echo "reset" >> $1
echo "% <- this is end of file symbol" >> $1
