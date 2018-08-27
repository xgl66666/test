#!/bin/bash

USER=`whoami`

VAR_LIST=${AUTO_GEN_OUT}/var_list
source ${VAR_LIST}

echo "### Generate global & chip specific definitions ###"
bash ${AUTO_GEN_PATH}/gen_definition.sh || exit 1
if [ "$OS_TYPE" == "ecos" ]; then
    source ${VAR_LIST}
else #[ "$OS_TYPE" == "ecos" ]; then
    bash ${AUTO_GEN_PATH}/sysInfo.sh
    REF_INFO=${AUTO_GEN_OUT}/ref_info
    source ${REF_INFO}


    if [ "$1" == "DDI_MAIN" ]; then
        echo "DDI_MAIN" $1
    else
        echo "Not DDI_MAIN" $1
    fi

    source ${VAR_LIST}

    echo "CHIP=$CHIP" >> ${CONF_INFO}
    TOOLCHAIN=`echo $READ_ELF | sed 's/-readelf//g'`
    echo "TOOLCHAIN=$TOOLCHAIN" >> ${CONF_INFO}
    echo "LINK_TYPE=${LINK_TYPE}" >> ${CONF_INFO}

    #check kernel version and FS type first
    echo FS_TYPE=${FS_TYPE}
    echo LINUX_KERNEL_VERSION=${LINUX_KERNEL_VERSION}

    if [ "$DDI_OPTEE" == "enable" ] || [ "$MI_DRM_SUPPORT_OPTEE" == "enable" ]; then
        OPTEE_SIZE=0x600000
        ARMFW_SIZE=0x10000
    else
        OPTEE_SIZE=0
        ARMFW_SIZE=0
    fi

    let MIN_IMAGE_SIZE=$((0x300000 * (${NAND_PAGE_SIZE} / 2048)))
    if [ "$FS_TYPE" == "cramfs" ]; then
        echo "RW_FS=r" >> ${VAR_LIST}
    else
        echo "RW_FS=rw" >> ${VAR_LIST}
    fi
fi #[ "$OS_TYPE" == "ecos" ]; then
echo "### Definition by Auto_Gen ###" >> $VAR_LIST

echo "********************************************"
echo "************  Auto Gen Scripts  ************"
echo "********************************************"
if [ "$OS_TYPE" != "ecos" ]; then
    echo "### Info from config ###"
    if [ "$FLASH_TYPE" == "SPI" ]; then
        KERNEL_IMG_NAME=uImage.gz
    elif [ "$LINUX_KERNEL_VERSION" == "3.1.10" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.23" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.23.CMA" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.40" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.86_64bit" ] ||  [ "$LINUX_KERNEL_VERSION" == "3.10.86" ] || [ "$LINUX_KERNEL_VERSION" == "3.18" ] || [ "$LINUX_KERNEL_VERSION" == "4.9_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "4.9" ]; then
        KERNEL_IMG_NAME=uImage
    else
        KERNEL_IMG_NAME=uImage.lzo
    fi

    if [ "$CHIP" == "clippers" ] && ( [ "$BOOT_MODE" == "EMMC" ] || [ "$BOOT_MODE" == "NAND" ] ); then
        if [ "$LINUX_KERNEL_VERSION" == "2.6.28" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/2.6.28/ROM
            LINUX_KERNEL_NAME=2_6_28
        elif [ "$LINUX_KERNEL_VERSION" == "3.1.10" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.1.10/ROM
            LINUX_KERNEL_NAME=3_1_10
        elif [ "$LINUX_KERNEL_VERSION" == "3.8.13" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.8.13/ROM
            LINUX_KERNEL_NAME=3_8_13
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.23" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.23/ROM
            LINUX_KERNEL_NAME=3_10_23
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.23.CMA" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.23.CMA/ROM
            LINUX_KERNEL_NAME=3_10_23_CMA
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.40" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.40/ROM
            LINUX_KERNEL_NAME=3_10_40
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.40_64bit/ROM
            LINUX_KERNEL_NAME=3_10_40_64bit
        else
            echo "!!! Unknown LINUX_KERNEL_VERSION : $(LINUX_KERNEL_VERSION) !!!"
            exit 1
        fi
    else
        if [ "$LINUX_KERNEL_VERSION" == "2.6.28" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/2.6.28
            LINUX_KERNEL_NAME=2_6_28
        elif [ "$LINUX_KERNEL_VERSION" == "3.1.10" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.1.10
            LINUX_KERNEL_NAME=3_1_10
        elif [ "$LINUX_KERNEL_VERSION" == "3.8.13" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.8.13
            LINUX_KERNEL_NAME=3_8_13
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.23" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.23
            LINUX_KERNEL_NAME=3_10_23
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.23.CMA" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.23.CMA
            LINUX_KERNEL_NAME=3_10_23_CMA
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.40" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.40
            LINUX_KERNEL_NAME=3_10_40
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.40_64bit
            LINUX_KERNEL_NAME=3_10_40_64bit
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.86_64bit" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.86_64bit
            LINUX_KERNEL_NAME=3_10_86_64bit
        elif [ "$LINUX_KERNEL_VERSION" == "3.10.86" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.10.86
            LINUX_KERNEL_NAME=3_10_86
        elif [ "$LINUX_KERNEL_VERSION" == "3.18" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/3.18
            LINUX_KERNEL_NAME=3_18
        elif [ "$LINUX_KERNEL_VERSION" == "4.9_64bit" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/4.9_64bit
            LINUX_KERNEL_NAME=4_9_64bit
        elif [ "$LINUX_KERNEL_VERSION" == "4.9" ]; then
            LINUX_KERNEL_PATH=../linux_kernel/4.9
            LINUX_KERNEL_NAME=4_9
        else
            echo "!!! Unknown LINUX_KERNEL_VERSION : $(LINUX_KERNEL_VERSION) !!!"
            exit 1
        fi
    fi

    # Assign kernel image path #
    if [ "$FLASH_TYPE" == "SPI" ]; then
        KERNEL_IMG_PATH=${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}
    else
        if [ "$CHIP" == "kaiser" ] || [ "$CHIP" == "keltic" ] || [ "$CHIP" == "clippers" ] || [ "$CHIP" == "muji" ] || [ "$CHIP" == "keres" ] || [ "$CHIP" == "kirin" ] || [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ]; then
            KERNEL_IMG_PATH=${LINUX_KERNEL_PATH}/${FS_TYPE}
        else
            KERNEL_IMG_PATH=${LINUX_KERNEL_PATH}
        fi
    fi

    echo "KERNEL_IMG_NAME=${KERNEL_IMG_NAME}" >> ${VAR_LIST}
    echo "ZRAM=${ZRAM}" >> ${CONF_INFO}
fi #[ "$OS_TYPE" != "ecos" ]; then


echo "###!! clean and make usb_update !!###"
if [ -d ${USB_PATH}/ ]; then
    rm -rf ${USB_PATH}/
fi
mkdir ${USB_PATH}/
mkdir ${USB_PATH}/Target/
mkdir ${USB_PATH}/Target/image/
mkdir ${USB_PATH}/Target/script/

if [ "$OS_TYPE" != "ecos" ]; then
    if [ "$TFTP_UPDATE" == "enable" ]; then
        echo "###!! clean and make tftp image folder !!###"
        rm -rf ${TFTP_PATH}/${FLASH_TYPE}
        mkdir -p ${TFTP_PATH}/${FLASH_TYPE}
    fi
fi #[ "$OS_TYPE" != "ecos" ]; then

echo "### Set file info ###"
RFS_FILE=[[ROOTFS
APP_FILE=[[application
CONF_FILE=[[${CFG_FOLDER_NAME}
KERNEL_FILE=[[kernel
DTB_FILE=[[ld_dtb
MWLIB_FILE=[[mwlib
LIB_FILE=[[libdynamic
CUST_FILE=[[customer
TEE_FILE=[[tee
OPTEE_FILE=[[optee
ARMFW_FILE=[[armfw
OPTEE_CIS_FILE=[[writecis
RTPM_FILE=[[RTPM
USERDATA_FILE=[[userdata
PART_FILE=${USB_PATH}/Target/script/set_partition
UPDATE_FILE=${TFTP_PATH}/auto_update.txt
USB_UPDATE_FILE=${USB_PATH}/Target/script/usb_auto_update.txt
SET_CONFIG_FILE=${USB_PATH}/Target/script/set_config
REF_INFO=${AUTO_GEN_OUT}/ref_info

if [ "$OS_TYPE" != "ecos" ]; then
    echo "### Prepare GDB ###"
    until [ -d "./DDI_Misc" ]
    do
        cd ..
    done
    MISC_DIR=`pwd`/DDI_Misc
    echo "MISC_DIR=$MISC_DIR" >> $VAR_LIST
    cd ${PRJ_DIR}
    cd ../../../bsp/linux/partition
    mkdir -p ${PRJ_DIR}/../../../bsp/linux/partition/auto_gen_output/application/gdb/
    cp ${MISC_DIR}/gdb/gdbs.sh ${PRJ_DIR}/../../../bsp/linux/partition/auto_gen_output/application/gdb/
    if [ -f ${PRJ_DIR}/gdbc.sh ]; then
        rm -f ${PRJ_DIR}/gdbc.sh
    fi
    ln -s ${MISC_DIR}/gdb/gdbc.sh ${PRJ_DIR}/gdbc.sh

    echo "### Prepare AutoGen Command ###"
    if [ -f ${PRJ_DIR}/agc.sh ]; then
        rm -f ${PRJ_DIR}/agc.sh
    fi
    ln -s ${MISC_DIR}/auto_gen/agc.sh ${PRJ_DIR}/agc.sh

    echo "### Generate change list info ###"
    bash ${AUTO_GEN_PATH}/parse_change_list.sh || err_exit "parse_change_list.sh execute fail!";


    #echo "### Get MMAP info ###"
    #sh ${AUTO_GEN_PATH}/parse_mmap.sh $MMAP || (echo "parse_mmap.sh executed fail!! $$?"; exit 1)

    printf "  NAND_SIZE=0x%x(defined in platform_define)\n" $NAND_SIZE

    # ROOTFS handle function #
    function rebuild_rfs()
    {
        if [ -d ROOTFS/ ]; then
            chown -R $USER ROOTFS/
            rm -rf ROOTFS/
        fi
        echo $PWD
        if [ "$USER" == "root" ]; then
            tar -${TAR_OPTION} $TAR_FILE || err_exit "Un-tar ROOTFS fail!!"
            chown -R $USER ROOTFS/
        else
            sudo tar -${TAR_OPTION} $TAR_FILE || err_exit "Un-tar ROOTFS fail!!"
            sudo chown -R $USER ROOTFS/
        fi
        if [ -d $LINUX_KERNEL_VERSION/ ]; then
            cp -af $LINUX_KERNEL_VERSION/* ROOTFS/
        else
            echo "!!! Orignal ROOTFS !!!"
        fi

        if [ "$MSTAR_CUST_STAGING" != "" ]; then
            echo "MSTAR_CUST_STAGING = $MSTAR_CUST_STAGING"
            bash ${MISC_DIR}/makerootfs/3create-rootfs-fromstaging.sh
        else
            echo "MSTAR_CUST_STAGING environment variable not set. Skip customize rootfs."
        fi
        let RE_GEN_RFS=1
    }

    function cal_size()
    {
        if [ "$3" == "bpartition" ]; then
           cal_ubifs_partition_size $1 $2
        elif [ "$3" == "dtb_image" ]; then
           cal_dtb_image_size $1 $2
        else
           cal_ubifs_image_size $1 $2
        fi
    }

    # dtb image size calcaulte function #
    function cal_dtb_image_size()
    {
        let tmp1_size=0
        let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
		let DTB_ALIGN_SIZE=0x1000
        until [ $tmp1_size -eq $tmp2_size ]
        do
            let tmp1_size=$tmp2_size
            let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
        done

        let CAL_SIZE=$((($tmp2_size + $DTB_ALIGN_SIZE) / $DTB_ALIGN_SIZE * $DTB_ALIGN_SIZE))
    }

    # partition size calcaulte function #
    function cal_ubifs_image_size()
    {
        let tmp1_size=0
        let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
        until [ $tmp1_size -eq $tmp2_size ]
        do
            let tmp1_size=$tmp2_size
            let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
        done

        let CAL_SIZE=$((tmp2_size + $RESERVED_SIZE +$2 ))
        let CAL_SIZE=$((($CAL_SIZE + $ALIGN_SIZE) / $ALIGN_SIZE * $ALIGN_SIZE))

        if [ "$FS_TYPE" == "ubifs" ] || [ "$FS_TYPE" == "ramdisk" ]; then
            if [ $CAL_SIZE -lt $MIN_IMAGE_SIZE ]; then
                let CAL_SIZE=$MIN_IMAGE_SIZE
            fi
        fi
    }

    # ubifs partition size calcaulte function #
    function cal_ubifs_partition_size()
    {
        let tmp1_size=0
        let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
        until [ $tmp1_size -eq $tmp2_size ]
        do
            let tmp1_size=$tmp2_size
            let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
        done

        let CAL_SIZE=$((tmp2_size + $RESERVED_SIZE +$2 ))

        if [ "$FS_TYPE" == "ubifs" ] || [ "$FS_TYPE" == "ramdisk" ]
            then
                if [ $CAL_SIZE -lt $MIN_IMAGE_SIZE ]
                    then
                        let CAL_SIZE=$MIN_IMAGE_SIZE
                fi
        fi

        #cal PEB size
        if [ "$NAND_BLOCK_PAGE_CNT" != "" ]; then
            let ubi_peb_size=$(($NAND_BLOCK_PAGE_CNT * $NAND_PAGE_SIZE))
        else
            let ubi_peb_size=$((64 * $NAND_PAGE_SIZE))
        fi

        #cal LEB size
        let ubi_leb_size=$(($ubi_peb_size-2*$NAND_PAGE_SIZE))

        ##cal worst case of leb cnt
        let max_leb_count=$(( (($CAL_SIZE*(4096+48+192))/($ubi_leb_size*4096))+(($CAL_SIZE*(4096+48+192))%($ubi_leb_size*4096) > 0) ))

        ##add 1 garbage collection(GC) leb
        let max_leb_count=$(($max_leb_count+1))

        ##add overhead (10 leb)
        let max_leb_count=$(($max_leb_count+10))

        #cal maximum LEB count (22 is minimum required LEBs)
        if [ $max_leb_count -lt 22 ]; then
            let max_leb_count=22
        fi

        let CAL_SIZE=$(($max_leb_count*$ubi_leb_size))
        let CAL_SIZE=$((($CAL_SIZE + $ALIGN_SIZE) / $ALIGN_SIZE * $ALIGN_SIZE))

    }
    # Generate ramdisk target.bin #
    function gen_ramdisk_target()
    {
        function hex()
        {
            printf '0x%08x' $1
        }

        # $KERNEL_IMG_PATH should already be defined before calling this function
        KERNEL=${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME}

        TARGET=${BSP_PRJ}/bsp/linux/usb_update/Target/image/target.bin
        ROOTFS_CPIO=${BSP_PRJ}/bsp/linux/usb_update/Target/image/rootfs.cpio.gz
        KERNEL_IMG_SIZE=`stat -c %s $KERNEL`

        PAD_SIZE=$((0x4000 - ($KERNEL_IMG_SIZE % 0x4000)))
        echo KERNEL_IMG_SIZE=$KERNEL_IMG_SIZE
        echo PAD_SIZE=$PAD_SIZE

        # Pack root filesystem to a cpio archive
        echo "Pack ROOTFS to $ROOTFS_CPIO"
        cd ${BSP_PRJ}/bsp/linux/partition/ROOTFS
        find . | cpio -o -H newc | gzip -9 > ${ROOTFS_CPIO}
        cd -
        RAMDISK_SIZE=$(hex `stat -c %s $ROOTFS_CPIO`)

        echo "Copy $KERNEL to $TARGET"
        /bin/cp -f $KERNEL $TARGET || err_exit "Get size info fail!!"

        echo "Pad $PAD_SIZE bytes to $TARGET (16 KiB)"
        head -c $PAD_SIZE /dev/zero >> $TARGET
        RAMDISK_START=$(hex $((`stat -c %s $TARGET` + $KERNEL_START)))

        echo "Concatenate $ROOTFS_CPIO to $TARGET"
        cat $ROOTFS_CPIO >> $TARGET
        RAM_DISK_BIN_SIZE=$(hex `stat -c %s $TARGET`)
    }
fi # [ "$OS_TYPE" != "ecos" ]; then
if [ "$AUTO_GEN_MIU_SETTING" == "enable" ]; then
    echo "********************************************************************************"
    echo "************************ Auto generate miu_setting.txt **************************"
    echo "********************************************************************************"
    python ${AUTO_GEN_PATH}/gen_miu_setting.py ${BSP_PRJ}/bsp/linux/partition/${CHIP}_miu_client.txt ${BSP_PRJ}/cus_mstar/project/mstar_demo/${MMAP_FILE} ${BSP_PRJ}/bsp/linux/partition/auto_gen_output/var_list > miu_setting.dbg

    cp -f ${BSP_PRJ}/bsp/linux/partition/miu_setting${MIU_TYPE}.txt ${USB_PATH}/Target/script || err_exit "Copy miu_setting fail!!"
fi
if [ "$OS_TYPE" == "ecos" ]; then
    # generate [[application #
    echo "###!! Generate [[application !!###"
    if [ "$TFTP_UPDATE" == "enable" ]; then
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE APP_SIZE application APP TFTP || err_exit "Execute gen_[[xxx.sh fail!!"
    fi
    bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE APP_SIZE application APP /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
    cp -f ${USB_PATH}/Target/script/$APP_FILE ${TFTP_PATH}/usb_update/Target/script/  || err_exit "Copy [[application fail!!"

    echo "###!! Generate auto_update.txt and usb_auto_update.txt !!###"
    if [ "$TFTP_UPDATE" == "enable" ]; then
        bash ${AUTO_GEN_PATH}/gen_auto_update.sh $UPDATE_FILE mstar || err_exit "Execute gen_auto_update.sh fail!!"
    fi
    bash ${AUTO_GEN_PATH}/gen_auto_update.sh $USB_UPDATE_FILE ${USB_UPDATE_CMD} || err_exit "Execute gen_auto_update.sh fail!!"
    cp -f $USB_UPDATE_FILE ${TFTP_PATH}/usb_update/Target/script/  || err_exit "Copy usb_auto_update fail!!"

    echo "###!! Generate set_config !!###"
#    echo "SET_CONFIG_FILE" $SET_CONFIG_FILE
    bash ${AUTO_GEN_PATH}/gen_set_config.sh $SET_CONFIG_FILE || err_exit "Execute gen_set_config.sh fail!!"
    cp -f $SET_CONFIG_FILE ${TFTP_PATH}/  || err_exit "Copy set_config fail!!"

    echo "###!! Generate parameters !!###"
    bash ${AUTO_GEN_PATH}/gen_parameters.sh || err_exit "Execute gen_parameters.sh fail!!"
else #[ "$OS_TYPE" == "ecos" ]; then
    if [ "$FLASH_TYPE" == "SPI" ]; then
        echo "### Deal with ROOTFS ###"
        let RE_GEN_RFS=0
        TAR_FILE=${LINUX_KERNEL_PATH}/${FS_TYPE}/SPI/ROOTFS_${LINUX_KERNEL_NAME}_${FS_TYPE}.${TAR_EXT_NAME}
        echo "tar file path name : $TAR_FILE"

        # extract ROOTFS/ if not exist #
        if [ ! -d ROOTFS/ ]; then
            echo "ROOTFS folder not exist, build ROOTFS !!"
            rebuild_rfs
        elif [ ! -f $LINUX_KERNEL_PATH/${FS_TYPE}/SPI/ROOTFS.$PART_EXT_NAME ]; then
            echo "ROOTFS.$PART_EXT_NAME not exist, build ROOTFS !!"
            rebuild_rfs
        else
            # get PREV_FS_TYPE #
            if [ "$PrevFsType" != "$FsType" ]; then
                echo "File System Type changed, re-build ROOTFS !!"
                rebuild_rfs
            fi

            # get PREV_RFS_TS #
            if [ "$RfsTS" != "$PrevRfsTS" ]; then
                echo "ROOTFS updated, re-build ROOTFS !!"
                rebuild_rfs
            fi
        fi

        echo "###!! Generate build_XXX related !!###"
        if [ "$FS_TYPE" != "ramdisk" ]; then
            bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_SPI_app_${FS_TYPE}.sh auto_gen_output/application 0 application || err_exit "Execute gen_build_partition.sh fail!!"
            bash build_SPI_app_${FS_TYPE}.sh

            if [ "$RE_GEN_RFS" -ne "0" ]; then
                bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_SPI_rfs_${FS_TYPE}.sh ROOTFS 0 ROOTFS || err_exit "Execute gen_build_partition.sh fail!!"
                bash build_SPI_rfs_${FS_TYPE}.sh
                cp ../usb_update/Target/image/ROOTFS.sqsh ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/
            else
                [ -f build_SPI_rfs_${FS_TYPE}.sh ] && mv build_SPI_rfs_${FS_TYPE}.sh build_rfs_${FS_TYPE}_backup.sh
            fi
        fi

        if [ "$FS_TYPE" == "sqsh" ]; then
            if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                # gen build_config_sqsh.sh #
                bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_${CFG_FOLDER_NAME}_${FS_TYPE}.sh auto_gen_output/${CFG_FOLDER_NAME} 0 ${CFG_FOLDER_NAME} || err_exit "Execute gen_build_partition.sh fail!!"

                # gen build_mwlib_sqsh.sh #
                if [ "$RELEASE_MW_LIB" == "enable" ]; then
                    bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_mwlib_${FS_TYPE}.sh auto_gen_output/mwlib 0 mwlib || err_exit "Execute gen_build_partition.sh fail!!"
                fi

                # gen build_lib_sqsh.sh #
                if [ "$LINK_TYPE" == "dynamic" ]; then
                    if [ -d auto_gen_output/lib_dynamic ]; then
                        bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_lib_${FS_TYPE}.sh auto_gen_output/lib_dynamic 0 lib_dynamic || err_exit "Execute gen_build_partition.sh fail!!"
                    else
                        bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_lib_${FS_TYPE}.sh ../../lib_dynamic 0 lib_dynamic || err_exit "Execute gen_build_partition.sh fail!!"
                    fi
                fi
            fi
        fi

        echo "### Generate target partition ###"
        if [ "$FS_TYPE" == "ramdisk" ]; then
            if [ ! -d ROOTFS ]; then
                err_exit "Please unpack ROOTFS folder first"
            fi

            gen_ramdisk_target
        fi

        if [ "$FS_TYPE" == "sqsh" ]; then
            if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                # gen config.sqsh #
                bash build_${CFG_FOLDER_NAME}_${FS_TYPE}.sh || err_exit "Build config partition.sh fail!!"
                cp ../usb_update/Target/image/${CFG_FOLDER_NAME}.sqsh ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/

                # gen mwlib.sqsh #
                if [ "$RELEASE_MW_LIB" == "enable" ]; then
                    bash build_mwlib_${FS_TYPE}.sh || err_exit "Build mwlib partition.sh fail!!"
                    cp ../usb_update/Target/image/mwlib.sqsh ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/
                fi

                # gen lib_dynamic.sqsh #
                if [ "$LINK_TYPE" == "dynamic" ]; then
                    bash build_lib_${FS_TYPE}.sh || err_exit "Build lib_dynamic partition.sh fail!!"
                    cp ../usb_update/Target/image/lib_dynamic.sqsh ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/
                fi

                # gen userdata partition #
                if [ "$USERDATA_PARTITION" == "enable" ]; then
                    cp ./userdata.jffs2 ../usb_update/Target/image/userdata.jffs2.sqsh
                    cp ./userdata.jffs2 ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/userdata.jffs2.sqsh
                fi
            fi
        fi

        echo "### Get size info ###"
        let ALIGN_SIZE=0x10000
        if [ "$FS_TYPE" != "ramdisk" ]; then
            # get APP_SIZE
            cal_size ../usb_update/Target/image/application.sqsh "image"
            printf "APP_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
        else
            APP_SIZE=0
        fi

        if [ "$FS_TYPE" == "sqsh" ]; then
            if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                # get CONF_SIZE
                cal_size ../usb_update/Target/image/${CFG_FOLDER_NAME}.sqsh $EXT_CONF_SIZE "image"
                printf "CONF_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}

                #get MWLIB_SIZE
                if [ "$RELEASE_MW_LIB" == "enable" ]; then
                    cal_size ../usb_update/Target/image/mwlib.sqsh $EXT_MWLIB_SIZE "image"
                    printf "MWLIB_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
                fi

                # get LIB_SIZE
                if [ "$LINK_TYPE" == "dynamic" ]; then
                    cal_size ../usb_update/Target/image/lib_dynamic.sqsh $EXT_LIB_SIZE "image"
                    printf "LIB_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
                fi

                # get USERDATA_SIZE
                if [ "$USERDATA_PARTITION" == "enable" ]; then
                    cal_size ../usb_update/Target/image/userdata.jffs2.sqsh 0 "image"
                    printf "USERDATA_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
                fi
            fi
        fi

        # get KERNEL_SIZE
        cal_size ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} 0 "image"
        printf "KERNEL_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}

        # get RFS_SIZE
        if [ "$FS_TYPE" != "ramdisk" ]; then
            cal_size ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/ROOTFS.sqsh 0 "image"
            printf "RFS_SIZE=0x%08X\n" $(($CAL_SIZE+0x10000)) >> ${VAR_LIST}
            RFS_SIZE=$CAL_SIZE
        else
            RFS_SIZE=0;
        fi

        # get RAM_DISK_BIN_SIZE
        if [ "$FS_TYPE" == "ramdisk" ]; then
            # store variable to $VAR_LIST
            echo "RAMDISK_START=${RAMDISK_START}" >> ${VAR_LIST}
            echo "RAMDISK_SIZE=${RAMDISK_SIZE}" >> ${VAR_LIST}
            echo "RAM_DISK_BIN_SIZE=${RAM_DISK_BIN_SIZE}" >> ${VAR_LIST}
        fi
        source $VAR_LIST

        echo "###!! Generate [[xxx script files !!###"
        # generate [[ROOTFS #
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"

        # generate [[application #
        if [ "$FS_TYPE" != "ramdisk" ]; then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        # generate [[config #
        if [ "$FS_TYPE" == "sqsh" ]; then
            if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                if [ "$TFTP_UPDATE" == "enable" ]; then
                    bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $CONF_FILE $CONF_SIZE ${CFG_FOLDER_NAME} CON ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                fi
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $CONF_FILE $CONF_SIZE ${CFG_FOLDER_NAME} CON /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
        fi

        # generate [[kernel #
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"

        # generate [[mwlib #
        if [ "$FS_TYPE" == "sqsh" ]; then
            if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                if [ "$RELEASE_MW_LIB" == "enable" ]; then
                    if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                        if [ "$TFTP_UPDATE" == "enable" ]; then
                            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $MWLIB_FILE $MWLIB_SIZE mwlib MWLIB ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                        fi
                        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $MWLIB_FILE $MWLIB_SIZE mwlib MWLIB /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
                    fi
                fi
            fi
        fi

        # generate [[libdynamic #
        if [ "$FS_TYPE" == "sqsh" ]; then
            if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                if [ "$LINK_TYPE" == "dynamic" ]; then
                    if [ "$TFTP_UPDATE" == "enable" ]; then
                        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $LIB_FILE $LIB_SIZE lib_dynamic LIB_DYNAMIC ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                    fi
                    bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $LIB_FILE $LIB_SIZE lib_dynamic LIB_DYNAMIC /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
                fi
            fi
        fi

        # generate [[userdata #
        if [ "$USERDATA_PARTITION" == "enable" ]; then
            if [ "$FS_TYPE" == "sqsh" ]; then
                if [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k1c" ]; then
                    if [ "$TFTP_UPDATE" == "enable" ]
                    then
                        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $USERDATA_FILE $USERDATA_SIZE userdata.jffs2 JFFS2 ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                    fi
                    bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $USERDATA_FILE $USERDATA_SIZE userdata.jffs2 JFFS2 /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
                fi
            fi
        fi

        echo "###!! Generate set_partition !!###"
        bash ${AUTO_GEN_PATH}/gen_set_partition.sh $PART_FILE || err_exit "Execute gen_set_partition.sh fail!!"

        echo "###!! Generate auto_update.txt and usb_auto_update.txt !!###"
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_auto_update.sh $UPDATE_FILE mstar || err_exit "Execute gen_auto_update.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_auto_update.sh $USB_UPDATE_FILE ${USB_UPDATE_CMD} || err_exit "Execute gen_auto_update.sh fail!!"



        echo "###!! Generate set_config !!###"
        bash ${AUTO_GEN_PATH}/gen_set_config.sh $SET_CONFIG_FILE || err_exit "Execute gen_set_config.sh fail!!"



        echo "###!! Prepare SPI update material !!###"
        cp -f ${BSP_PRJ}/bsp/linux/partition/miu_setting${MIU_TYPE}.txt ${USB_PATH}/Target/script || err_exit "Copy miu_setting.txt fail!!"
        cp -f ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} ${USB_PATH}/Target/image || err_exit "Copy kernel image fail!!"
        if [ "$FS_TYPE" != "ramdisk" ]; then
            cp -f ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/ROOTFS.sqsh ${USB_PATH}/Target/image || err_exit "Copy ROOTFS partition fail!!"
        fi


        echo "### Generate quick links ###"
        if [ "$FS_TYPE" != "ramdisk" ]; then
            echo -e " ******************************************************************************"
            echo -e " ***************   'mstar kernel '        for kernel           ****************"
            echo -e " ***************   'mstar app    '        for application      ****************"
            echo -e " ***************   'mstar rfs    '        for rootfs           ****************"
            echo -e " ******************************************************************************"
            cp -f ${USB_PATH}/Target/script/[[application ${USB_PATH}/Target/script/app
            cp -f ${USB_PATH}/Target/script/[[kernel ${USB_PATH}/Target/script/kernel
            cp -f ${USB_PATH}/Target/script/[[ROOTFS ${USB_PATH}/Target/script/rfs
        else
            echo -e " ******************************************************************************"
            echo -e " ***************   'mstar kernel '        for kernel           ****************"
            echo -e " ******************************************************************************"
            cp -f ${USB_PATH}/Target/script/[[kernel ${USB_PATH}/Target/script/kernel
        fi
    else #if [ "$FLASH_TYPE" == "SPI" ]
        echo "### Deal with ROOTFS ###"

        let RE_GEN_RFS=0

        if [ "$CHIP" == "kiwi" ] || [ "$CHIP" == "k5tn" ] || [ "$CHIP" == "k1c" ] || [ "$CHIP" == "k5ap" ]; then
            CAL_METHOD="bpartition"
        else
            CAL_METHOD="image"
        fi

        if [ "$CHIP" == "kano" ] || [ "$CHIP" == "kayla" ] || [ "$CHIP" == "kris" ] || [ "$CHIP" == "curry" ] || [ "$CHIP" == "k6" ] || [ "$CHIP" == "k6lite" ] || [ "$CHIP" == "k5tn" ] || [ "$CHIP" == "c2p" ] || [ "$CHIP" == "k7u" ] || [ "$CHIP" == "k1c" ] || [ "$CHIP" == "k5ap" ]; then
            TAR_FILE=${LINUX_KERNEL_PATH}/ROOTFS_${LINUX_KERNEL_NAME}.${TAR_EXT_NAME}
            ROOTFS_FILE=$LINUX_KERNEL_PATH/ROOTFS.$PART_EXT_NAME
        else
            TAR_FILE=${LINUX_KERNEL_PATH}/${FS_TYPE}/ROOTFS_${LINUX_KERNEL_NAME}_${FS_TYPE}.${TAR_EXT_NAME}
            ROOTFS_FILE=$LINUX_KERNEL_PATH/${FS_TYPE}/ROOTFS.$PART_EXT_NAME
        fi
        echo "tar file path name : $TAR_FILE"

        # extract ROOTFS/ if not exist #
        if [ ! -d ROOTFS/ ]; then
            echo "ROOTFS folder not exist, build ROOTFS !!"
            rebuild_rfs
        elif [ ! -f $ROOTFS_FILE ]; then
            echo "ROOTFS.$PART_EXT_NAME not exist, build ROOTFS !!"
            rebuild_rfs
        else
            # get PREV_FS_TYPE #
            if [ "$PrevFsType" != "$FsType" ]; then
                echo "File System Type changed, re-build ROOTFS !!"
                rebuild_rfs
            fi

            # get PREV_LINUX_KERNEL_VERSION #
            if [ "$PrevLinuxKernelVersion" != "$LinuxKernelVersion" ]; then
                echo "Kernel Version changed, re-build ROOTFS !!"
                rebuild_rfs
            fi

            # get PREV_NAND_PAGE_SIZE #
            if [ "$PrevNandPageSize" -ne "$NandPageSize" ]; then
                echo "NAND PAGE SIZE changed, re-build ROOTFS !!"
                rebuild_rfs
            fi

            # get PREV_RFS_TS #
            if [ "$RfsTS" != "$PrevRfsTS" ]; then
                echo "ROOTFS updated, re-build ROOTFS !!"
                rebuild_rfs
            fi
        fi



        echo "### Get size info ###"
        let ALIGN_SIZE=$((1024*1024))
        # get APP_SIZE
        cal_size ${AUTO_GEN_OUT}/application/ $EXT_APP_SIZE $CAL_METHOD
        printf "APP_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}

        # get CONF_SIZE
        cal_size ${AUTO_GEN_OUT}/${CFG_FOLDER_NAME}/ $EXT_CONF_SIZE $CAL_METHOD
        printf "CONF_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}

        # get KERNEL_SIZE
        cal_size ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} $EXT_KERNEL_SIZE  "image"
        printf "KERNEL_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}

        if [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.86_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "4.9_64bit" ] || [ "$CHIP" == "muji" ]; then
            # get DBT_SIZE
            cal_size ${KERNEL_IMG_PATH}/DTB.dtb 0 "dtb_image"
            printf "DTB_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
        fi


        # get RFS_SIZE
        if [ "$FS_TYPE" != "ramdisk" ]; then
            cal_size ROOTFS/ $EXT_RFS_SIZE $CAL_METHOD
            let RFS_SIZE=$CAL_SIZE
            if [ "$FS_TYPE" == "yaffs2" ]; then
                RFS_SIZE=$(($RFS_SIZE+0x400000))
            fi
            printf "RFS_SIZE=0x%08X\n" $RFS_SIZE >> ${VAR_LIST}
        else
            RFS_SIZE=0;
        fi

        #get MWLIB_SIZE
        if [ "$RELEASE_MW_LIB" == "enable" ]; then
            cal_size ${AUTO_GEN_OUT}/mwlib $EXT_MWLIB_SIZE $CAL_METHOD
            printf "MWLIB_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
        fi

        # get LIB_SIZE
        if [ "$LINK_TYPE" == "dynamic" ]; then
            if [ -d auto_gen_output/lib_dynamic ]; then
                cal_size auto_gen_output/lib_dynamic $EXT_LIB_SIZE $CAL_METHOD
            else
                cal_size ../../lib_dynamic $EXT_LIB_SIZE $CAL_METHOD
            fi
            let LIB_SIZE=$CAL_SIZE
        else
            rm -f ../tftp_root/${FLASH_TYPE}/lib_dynamic.${FS_TYPE}
            rm -f ../tftp_root/[[libdynamic
            let LIB_SIZE=0
        fi
        printf "LIB_SIZE=0x%08X\n" $LIB_SIZE >> ${VAR_LIST}

        # get RAM_DISK_BIN_SIZE
        if [ "$FS_TYPE" == "ramdisk" ]; then
            if [ ! -d ROOTFS ]; then
                err_exit "Please unpack ROOTFS folder first"
            fi

            gen_ramdisk_target

            # store variable to $VAR_LIST
            echo "RAMDISK_START=${RAMDISK_START}" >> ${VAR_LIST}
            echo "RAMDISK_SIZE=${RAMDISK_SIZE}" >> ${VAR_LIST}
            echo "RAM_DISK_BIN_SIZE=${RAM_DISK_BIN_SIZE}" >> ${VAR_LIST}
        fi
        source $VAR_LIST

		# Prelink Start
		if [ "$Prelink" == "enable" ]
			then
				echo "Prelink Start"
				if [ "$USER" == "root" ]; then
					export LD_PRELOAD=${LD_PRELOAD/libfakeroot-sysv.so/}
				fi
				rm -rf ${AUTO_GEN_OUT}/../ROOTFS/etc/prelink.cache
				cp -rf ${AUTO_GEN_OUT}/application/* ${AUTO_GEN_OUT}/../ROOTFS/application/
				cp -rf ${AUTO_GEN_OUT}/config/* ${AUTO_GEN_OUT}/../ROOTFS/config/
				cp -rf ${AUTO_GEN_OUT}/mwlib/*.* ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/
				cp -rf ${AUTO_GEN_OUT}/mwlib/mmsdk_avp ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/
				cp -rf ${AUTO_GEN_OUT}/lib_dynamic/* ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/utopia/
				cp -rf ${AUTO_GEN_OUT}/../prelink/conf/prelink.conf ${AUTO_GEN_OUT}/../ROOTFS/etc/
				
				TOOL_PATH=/application/mstar_wireless_tools
				MSLIB_PATH=/vendor/lib
				export LD_LIBRARY_PATH=$MSLIB_PATH:$MSLIB_PATH/mmsdk_avp:$MSLIB_PATH/dtcp:$MSLIB_PATH/ipc_svc:$MSLIB_PATH/qtwebkit:$MSLIB_PATH/directfb-1.4-0:$MSLIB_PATH/utopia:$MSLIB_PATH/directfb-1.4-0/systems:$MSLIB_PATH/directfb-1.4-0/wm:$MSLIB_PATH/directfb-1.4-0/inputdrivers:$MSLIB_PATH/directfb-1.4-0/gfxdrivers:$MSLIB_PATH/directfb-1.4-0/interfaces/IDirectFBFont:$MSLIB_PATH/directfb-1.4-0/interfaces/IDirectFBImageProvider:$MSLIB_PATH/directfb-1.4-0/interfaces/IDirectFBVideoProvider:$MSLIB_PATH/qt/plugins/gfxdrivers/:$MSLIB_PATH/gstreamer-0.10/:$MSLIB_PATH/boost/:$MSLIB_PATH/ipc_mm:$MSLIB_PATH/drmagent:$MSLIB_PATH/WVCENC:/application/dial_v1.7.x/lib:$MSLIB_PATH/qtwebkit/
				export DFB176_PATH=$MSLIB_PATH/directfb-1.7-6:$MSLIB_PATH/directfb-1.7-6/systems:$MSLIB_PATH/directfb-1.7-6/wm:$MSLIB_PATH/directfb-1.7-6/inputdrivers:$MSLIB_PATH/directfb-1.7-6/gfxdrivers:$MSLIB_PATH/directfb-1.7-6/interfaces/IDirectFBFont:$MSLIB_PATH/directfb-1.7-6/interfaces/IDirectFBImageProvider:$MSLIB_PATH/directfb-1.7-6/interfaces/IDirectFBVideoProvider
				export PRE_PATH=$LD_LIBRARY_PATH:$TOOL_PATH:$DFB176_PATH

				#fakeroot ${AUTO_GEN_OUT}/../prelink/bin/mips-linux-gnu-prelink  --root=${AUTO_GEN_OUT}/../ROOTFS/ --ld-library-path=$PRE_PATH -vf /application/kirin_linux1 1> prelinklog.txt 2> prelinkerr.txt
				${PRELINK} --root=${AUTO_GEN_OUT}/../ROOTFS/ --ld-library-path=$PRE_PATH -avf 1> prelinklog.txt 2> prelinkerr.txt #/application/kirin_linux1 1> prelinklog.txt 2> prelinkerr.txt
				cp -rf ${AUTO_GEN_OUT}/../ROOTFS/application/* ${AUTO_GEN_OUT}/application/
				cp -rf ${AUTO_GEN_OUT}/../ROOTFS/config/* ${AUTO_GEN_OUT}/config/
				cp -rf ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/*.* ${AUTO_GEN_OUT}/mwlib/
				cp -rf ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/utopia/* ${AUTO_GEN_OUT}/lib_dynamic/
				cp -rf ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/mmsdk_avp ${AUTO_GEN_OUT}/mwlib/
				
				rm -rf ${AUTO_GEN_OUT}/../ROOTFS/application/*
				rm -rf ${AUTO_GEN_OUT}/../ROOTFS/config/*
				rm -rf ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/*.*
				rm -rf ${AUTO_GEN_OUT}/../ROOTFS/vendor/lib/utopia/*
				echo "Prelink End"
		fi		
		# Prelink End

        echo "###!! Generate [[xxx script files !!###"
        # generate [[ROOTFS #
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"

        # generate [[application #
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"

        # generate [[config #
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $CONF_FILE $CONF_SIZE ${CFG_FOLDER_NAME} CON ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $CONF_FILE $CONF_SIZE ${CFG_FOLDER_NAME} CON /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"

        # generate [[kernel #
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"


        if [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.86_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "4.9_64bit" ] || [ "$CHIP" == "muji" ]; then
            # generate [[ld_dtb #
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $DTB_FILE $DTB_SIZE dtb DTB ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $DTB_FILE $DTB_SIZE dtb DTB /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        # generate [[mwlib #
        if [ "$RELEASE_MW_LIB" == "enable" ]; then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $MWLIB_FILE $MWLIB_SIZE mwlib MWLIB ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $MWLIB_FILE $MWLIB_SIZE mwlib MWLIB /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        # generate [[libdynamic #
        if [ "$LINK_TYPE" == "dynamic" ]; then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $LIB_FILE $LIB_SIZE lib_dynamic LIB ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $LIB_FILE $LIB_SIZE lib_dynamic LIB /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        # generate [[customer #
        if [ "$MW_FLOW" == "enable" ]; then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $CUST_FILE $CUST_SIZE customer CUST ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $CUST_FILE $CUST_SIZE customer CUST /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        # generate [[tee #
        if [ "$DDI_TEE" == "enable" ] || [ "$MI_DRM_SUPPORT_TEE" == "enable" ]; 	then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $TEE_FILE 0 tee tee ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $TEE_FILE 0 tee tee /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        # generate [[optee #
        if [ "$DDI_OPTEE" == "enable" ] || [ "$MI_DRM_SUPPORT_OPTEE" == "enable" ]; then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $OPTEE_FILE $OPTEE_SIZE optee optee ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                if [ "$CHIP" != "k6lite" ]; then
                    bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $ARMFW_FILE $ARMFW_SIZE armfw armfw ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                fi
                if [ "$FLASH_TYPE" == "NAND" ]; then
                    bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $OPTEE_CIS_FILE 0 writecis writecis ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
                fi
            fi

            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $OPTEE_FILE $OPTEE_SIZE optee optee /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
            if [ "$CHIP" != "k6lite" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $ARMFW_FILE $ARMFW_SIZE armfw armfw /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            if [ "$FLASH_TYPE" == "NAND" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $OPTEE_CIS_FILE 0 writecis writecis /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
        fi

        echo "###!! Generate set_partition !!###"
        bash ${AUTO_GEN_PATH}/gen_set_partition.sh $PART_FILE || err_exit "Execute gen_set_partition.sh fail!!"

        # generate [[RTPM #
        if [ "$DDI_ECC_SW_OBFUSCATION" == "enable" ]; then
            if [ "$TFTP_UPDATE" == "enable" ]; then
                bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $RTPM_FILE 0 RTPM RTPM ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
            fi
            bash ${AUTO_GEN_PATH}/gen_[[xxx.sh $RTPM_FILE 0 RTPM RTPM /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
        fi

        echo "###!! Generate set_partition !!###"
        bash ${AUTO_GEN_PATH}/gen_set_partition.sh $PART_FILE || err_exit "Execute gen_set_partition.sh fail!!"

        echo "###!! Generate auto_update.txt and usb_auto_update.txt !!###"
        if [ "$TFTP_UPDATE" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_auto_update.sh $UPDATE_FILE mstar || err_exit "Execute gen_auto_update.sh fail!!"
        fi
        bash ${AUTO_GEN_PATH}/gen_auto_update.sh $USB_UPDATE_FILE ${USB_UPDATE_CMD} || err_exit "Execute gen_auto_update.sh fail!!"



        echo "###!! Generate set_config !!###"
        bash ${AUTO_GEN_PATH}/gen_set_config.sh $SET_CONFIG_FILE || err_exit "Execute gen_set_config.sh fail!!"



        echo "###!! Generate build_XXX related !!###"
        if [ "$RE_GEN_RFS" -ne "0" ]; then
            bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_rfs_${FS_TYPE}.sh ROOTFS $RFS_SIZE ROOTFS || err_exit "Execute gen_build_partition.sh fail!!"
        else
            [ -f build_rfs_${FS_TYPE}.sh ] && mv build_rfs_${FS_TYPE}.sh build_rfs_${FS_TYPE}_backup.sh
        fi
        bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_app_${FS_TYPE}.sh auto_gen_output/application $APP_SIZE application || err_exit "Execute gen_build_partition.sh fail!!"
        bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_${CFG_FOLDER_NAME}_${FS_TYPE}.sh auto_gen_output/${CFG_FOLDER_NAME} $CONF_SIZE ${CFG_FOLDER_NAME} || err_exit "Execute gen_build_partition.sh fail!!"
        if [ "$RELEASE_MW_LIB" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_mwlib_${FS_TYPE}.sh auto_gen_output/mwlib $MWLIB_SIZE mwlib || err_exit "Execute gen_build_partition.sh fail!!"
        fi
        if [ "$LINK_TYPE" == "dynamic" ]; then
            if [ -d auto_gen_output/lib_dynamic ]; then
                bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_lib_${FS_TYPE}.sh auto_gen_output/lib_dynamic $LIB_SIZE  lib_dynamic || err_exit "Execute gen_build_partition.sh fail!!"
            else
                bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_lib_${FS_TYPE}.sh ../../lib_dynamic $LIB_SIZE  lib_dynamic || err_exit "Execute gen_build_partition.sh fail!!"
            fi
        fi
        if [ "$MW_FLOW" == "enable" ]; then
            bash ${AUTO_GEN_PATH}/gen_build_partition.sh build_cust_${FS_TYPE}.sh auto_gen_output/Customer $CUST_SIZE  customer || err_exit "Execute gen_build_partition.sh fail!!"
        fi


        echo "###!! Generate partitions !!###"
        bash build_app_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
        bash build_${CFG_FOLDER_NAME}_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
        if [ "$RELEASE_MW_LIB" == "enable" ]; then
            bash build_mwlib_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
        fi
        #echo ${LINUX_KERNEL_PATH}/${KERNEL_IMG_NAME}
        cp -f ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} ${USB_PATH}/Target/image || err_exit "Copy kernel image fail!!"

        if [ "$LINUX_KERNEL_VERSION" == "3.10.40_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "3.10.86_64bit" ] || [ "$LINUX_KERNEL_VERSION" == "4.9_64bit" ] || [ "$CHIP" == "muji" ]; then
            cp -f ${KERNEL_IMG_PATH}/DTB.dtb ${USB_PATH}/Target/image || err_exit "Copy dtb image fail!!"
        fi

        if [ "$FS_TYPE" != "ramdisk" ]; then
            if [ "$RE_GEN_RFS" -ne "0" ]; then
                bash build_rfs_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
                if [ "$CHIP" == "kano" ] || [ "$CHIP" == "kayla" ] || [ "$CHIP" == "kris" ] || [ "$CHIP" == "curry" ] || [ "$CHIP" == "k6" ] || [ "$CHIP" == "k6lite" ] || [ "$CHIP" == "k5tn" ] || [ "$CHIP" == "c2p" ] || [ "$CHIP" == "k7u" ] || [ "$CHIP" == "k1c" ] || [ "$CHIP" == "k5ap" ]; then
                    cp -f ${USB_PATH}/Target/image/ROOTFS.${PART_EXT_NAME} ${LINUX_KERNEL_PATH}/ROOTFS.${PART_EXT_NAME} || err_exit "Backup ROOTFS partition fail!!"
                else
                    cp -f ${USB_PATH}/Target/image/ROOTFS.${PART_EXT_NAME} ${LINUX_KERNEL_PATH}/${FS_TYPE}/ROOTFS.${PART_EXT_NAME} || err_exit "Backup ROOTFS partition fail!!"
                fi
            else
                if [ "$CHIP" == "kano" ] || [ "$CHIP" == "kayla" ] || [ "$CHIP" == "kris" ] || [ "$CHIP" == "curry" ] || [ "$CHIP" == "k6" ] || [ "$CHIP" == "k6lite" ] || [ "$CHIP" == "k5tn" ] || [ "$CHIP" == "c2p" ] || [ "$CHIP" == "k7u" ] || [ "$CHIP" == "k1c" ] || [ "$CHIP" == "k5ap" ]; then
                    cp -f ${LINUX_KERNEL_PATH}/ROOTFS.${PART_EXT_NAME} ${USB_PATH}/Target/image/ROOTFS.${PART_EXT_NAME} || err_exit "Copy ROOTFS partition from backup fail!!"
                else
                    cp -f ${LINUX_KERNEL_PATH}/${FS_TYPE}/ROOTFS.${PART_EXT_NAME} ${USB_PATH}/Target/image/ROOTFS.${PART_EXT_NAME} || err_exit "Copy ROOTFS partition from backup fail!!"
                fi
            fi
        fi
        if [ "$LINK_TYPE" == "dynamic" ]; then
            bash build_lib_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
        fi
        if [ "$MW_FLOW" == "enable" ]; then
            bash build_cust_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
        fi



        echo "### Generate quick links ###"
        echo -e " ******************************************************************************"
        echo -e " ***************   'mstar app    '        for application      ****************"
        echo -e " ***************   'mstar ${CFG_FOLDER_NAME} '        for ${CFG_FOLDER_NAME}           ****************"
        echo -e " ***************   'mstar kernel '        for kernel           ****************"
        echo -e " ***************   'mstar rfs    '        for rootfs           ****************"
        [ "$MW_FLOW" == "enable" ] && 				echo -e " ***************   'mstar cust   '        for customer         ****************"
        [ "$LINK_TYPE" == "dynamic" ] && 			echo -e " ***************   'mstar lib    '        for libdynamic       ****************"
        [ "$RELEASE_MW_LIB" == "enable" ] && 	echo -e " ***************   'mstar mwlib  '        for mwlib            ****************"
        echo -e " ******************************************************************************"
        cp -f ${USB_PATH}/Target/script/usb_auto_update.txt ${USB_PATH}/Target/script/all
        cp -f ${TFTP_PATH}/auto_update.txt ${TFTP_PATH}/all
        cp -f ${USB_PATH}/Target/script/[[application ${USB_PATH}/Target/script/app
        cp -f ${TFTP_PATH}/[[application ${TFTP_PATH}/app
        cp -f ${USB_PATH}/Target/script/[[${CFG_FOLDER_NAME} ${USB_PATH}/Target/script/${CFG_FOLDER_NAME}
        cp -f ${TFTP_PATH}/[[${CFG_FOLDER_NAME} ${TFTP_PATH}/${CFG_FOLDER_NAME}
        cp -f ${USB_PATH}/Target/script/[[kernel ${USB_PATH}/Target/script/kernel
        cp -f ${TFTP_PATH}/[[kernel ${TFTP_PATH}/kernel
        [ "$FS_TYPE" != "ramdisk" ] && 				cp -f ${USB_PATH}/Target/script/[[ROOTFS ${USB_PATH}/Target/script/rfs
        [ "$FS_TYPE" != "ramdisk" ] && 				cp -f ${TFTP_PATH}/[[ROOTFS ${TFTP_PATH}/rfs
        [ "$MW_FLOW" == "enable" ] && 				cp -f ${USB_PATH}/Target/script/[[customer ${USB_PATH}/Target/script/cust
        [ "$MW_FLOW" == "enable" ] && 				cp -f ${TFTP_PATH}/[[customer ${TFTP_PATH}/cust
        [ "$LINK_TYPE" == "dynamic" ] && 			cp -f ${USB_PATH}/Target/script/[[libdynamic ${USB_PATH}/Target/script/lib
        [ "$LINK_TYPE" == "dynamic" ] && 			cp -f ${TFTP_PATH}/[[libdynamic ${TFTP_PATH}/lib
        [ "$RELEASE_MW_LIB" == "enable" ] && 	cp -f ${USB_PATH}/Target/script/[[mwlib ${USB_PATH}/Target/script/mwlib
        [ "$RELEASE_MW_LIB" == "enable" ] && 	cp -f ${TFTP_PATH}/[[mwlib ${TFTP_PATH}/mwlib
    fi



    cp -f ${BSP_PRJ}/bsp/linux/partition/miu_setting${MIU_TYPE}.txt ${USB_PATH}/Target/script || err_exit "Copy miu_setting fail!!"
    if [ "$TFTP_UPDATE" == "enable" ]; then
        source ${VAR_LIST}
        echo "********************************************************************************"
        echo "***************         Update files from usb_update.           ****************"
        cp -f ${USB_PATH}/Target/script/set_config ${TFTP_PATH}/  || err_exit "Copy set_config fail!!"
        cp -f ${USB_PATH}/Target/script/set_partition ${TFTP_PATH}/  || err_exit "Copy set_partition fail!!"
        cp -f ${BSP_PRJ}/bsp/linux/partition/miu_setting${MIU_TYPE}.txt ${TFTP_PATH}/ || err_exit "Copy miu_setting fail!!"
        #		if [ -f ${USB_PATH}/Target/image/*.${PART_EXT_NAME} ]
        #			then
        #				cp -f ${USB_PATH}/Target/image/*.${PART_EXT_NAME} ${TFTP_PATH}/${FLASH_TYPE}/ || err_exit "Copy partition fail!!"
        #		fi
        cp -f ${USB_PATH}/Target/image/* ${TFTP_PATH}/${FLASH_TYPE}/ || err_exit "Copy kernel image fail!!"
        if [ "$FS_TYPE" == "ramdisk" ]; then
            cp -f ${USB_PATH}/Target/image/target.bin  ${TFTP_PATH}/${FLASH_TYPE}/ || err_exit "Copy target.bin fail!!"
        fi
        echo " ***************               Update Done.                     ****************"
        echo "********************************************************************************"
    fi



    echo "### Check MMAP ###"
    cd ${PRJ_DIR}
    if [ "$1" == "DDI_MAIN" ]; then
        bash ${AUTO_GEN_PATH}/check_mmap.sh ${TARGET_PATH} set_config configurations/platform_configuration || err_exit "check_mmap.sh executed fail!! $$?"
    else
        bash ${AUTO_GEN_PATH}/check_mmap.sh ${TARGET_PATH} set_config config || err_exit "check_mmap.sh executed fail!! $$?"
    fi


    echo "### Check Utopia Lib Path ###"
    if [ "$UTOPIA_PATH" != "" ]; then
        echo "Utopia path : //${UTOPIA_PATH}"
        echo "The source path list below may not the same as Utopia lib path"
        echo -e "\e[1;33;40m******************************"
        if [ "$LINK_TYPE" == "dynamic" ]; then
            cd ../../../bsp/lib_dynamic
            grep -irn "${UTOPIA_PATH}" . -L | grep lib
        else
            cd ../../../bsp/lib
            grep -irn "${UTOPIA_PATH}" . -L | grep lib
        fi
        echo -e "******************************\e[0m"
        cd -
    else
        echo "UTOPIA_PATH is not set in \"cus_config/platform_define\""
    fi
    echo
fi #[ "$OS_TYPE" == "ecos" ]; then
