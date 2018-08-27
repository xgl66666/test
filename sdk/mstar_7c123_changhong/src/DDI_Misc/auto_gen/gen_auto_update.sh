#sh auto_gen/gen_auto_update.sh $UPDATE_FILE mstar  <<-- example of each parameter
#   $0                          $1           $2

source ${AUTO_GEN_OUT}/var_list

echo "# <- this is for comment / total file size must be less than 4KB" > $1
if [ "$OS_TYPE" == "ecos" ]; then
    if [ "$2" == "udstar" ]; then
        SCRIPT_PATH=/Target/script
    else
        SCRIPT_PATH=.
    fi

    echo >> $1
    echo "$2 ${SCRIPT_PATH}/parameters.txt" >> $1
    echo >> $1

    echo "#application" >> $1
    echo "$2 ${SCRIPT_PATH}/[[application" >> $1
    echo >> $1

    echo "#config & bootargs" >> $1
    echo "$2 ${SCRIPT_PATH}/set_config" >> $1
    echo >> $1

    echo "$2 ${SCRIPT_PATH}/miu_setting.txt" >> $1
    echo >> $1
    echo "printenv" >> $1
    echo "reset" >> $1
    echo "% <- this is end of file symbol" >> $1
    echo >> $1
else #[ "$OS_TYPE" == "ecos" ]; then
    if [ "$CHIP" == "k1" ]; then
        if [ "$2" == "mstar" ]; then
            MIU_CMD=net_miu
        else
            MIU_CMD=usb_miu
        fi
    else
        MIU_CMD=$2
    fi


    if [ "$FLASH_TYPE" == "SPI" ]; then
        if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
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
            if [ "${FS_TYPE}" != "ramdisk" ]; then
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

            if [ "$USERDATA_PARTITION" == "enable" ]; then
                echo "#userdata" >> $1
                echo "$2 ${SCRIPT_PATH}/[[userdata" >> $1
                echo >> $1
            fi

            echo "#config & bootargs" >> $1
            echo "$2 ${SCRIPT_PATH}/set_config" >> $1
            echo >> $1

            echo "updatemiureg ${SCRIPT_PATH}/miu_setting${MIU_TYPE}.txt" >> $1
            echo >> $1
            echo "printenv" >> $1
            if [ "$2" == "ustar" ] || [ "$2" == "udstar" ]; then
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
            if [ "${FS_TYPE}" != "ramdisk" ]; then
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
            if [ "$2" == "ustar" ]; then
                echo "setenv usb_complete 1" >> $1
                echo "saveenv" >> $1
            fi
        fi
    else
        if [ "$2" == "udstar" ]; then
            SCRIPT_PATH=/Target/script
        else
            SCRIPT_PATH=.
        fi

        if [ "$FLASH_TYPE" == "NAND" ]; then
            echo "nand erase" >> $1
            if [ "$DDI_OPTEE" == "enable" ] || [ "$MI_DRM_SUPPORT_OPTEE" == "enable" ]; then
                # OP-TEE NAND type needs to set NAND partition info from cis images
                echo "#write cis" >> $1
                echo "$2 ${SCRIPT_PATH}"/[[writecis >> $1
                echo >> $1
            fi
        else
            if [ "$BOOT_MODE" != "EMMC" ]; then
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
        if [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.86_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "4.9_64bit" ] || [ "$CHIP" == "muji" ]; then
            echo "#dtb" >> $1
            echo "$2 ${SCRIPT_PATH}/[[ld_dtb" >> $1
            echo >> $1
        fi
        if [ "${FS_TYPE}" != "ramdisk" ]; then
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
        if [ "$DDI_TEE" == "enable" ] || [ "$MI_DRM_SUPPORT_TEE" == "enable" ]; then
            echo "#tee" >> $1
            echo "$2 ${SCRIPT_PATH}"/[[tee >> $1
            echo >> $1
        fi

        if [ "$DDI_OPTEE" == "enable" ] || [ "$MI_DRM_SUPPORT_OPTEE" == "enable" ]; then
            echo "#optee" >> $1
            echo "$2 ${SCRIPT_PATH}"/[[optee >> $1
            echo >> $1

            if [ "$CHIP" != "k6lite" ]; then
                echo "#armfw" >> $1
                echo "$2 ${SCRIPT_PATH}"/[[armfw >> $1
                echo >> $1
            fi
        fi

        if [ "$MW_FLOW" == "enable" ]; then
            echo "#customer" >> $1
            echo "$2 ${SCRIPT_PATH}/[[customer" >> $1
            echo >> $1
        fi
        if [ "$DDI_ECC_SW_OBFUSCATION" == "enable" ]; then
            echo "#rtpm" >> $1
            echo "$2 ${SCRIPT_PATH}"/[[RTPM >> $1
            echo >> $1
        fi
        echo "#config & bootargs" >> $1
        echo "$2 ${SCRIPT_PATH}/set_config" >> $1
        echo >> $1
        echo "$MIU_CMD ${SCRIPT_PATH}/miu_setting${MIU_TYPE}.txt" >> $1
        echo >> $1
        echo "printenv" >> $1
        if [ "$2" == "ustar" ] || [ "$2" == "udstar" ]; then
            echo "setenv usb_complete 1" >> $1
            echo "saveenv" >> $1
        fi
    fi

    echo "reset" >> $1
    echo "% <- this is end of file symbol" >> $1
fi #[ "$OS_TYPE" == "ecos" ]; then
