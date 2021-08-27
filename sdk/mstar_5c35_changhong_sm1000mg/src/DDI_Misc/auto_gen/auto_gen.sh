#!/bin/bash

USER=`whoami`

VAR_LIST=${AUTO_GEN_OUT}/var_list
source ${VAR_LIST}


### chmod project to rw ###
cd ../../../../
chmod 777 -R .
cd -


echo "### Generate global & chip specific definitions ###"
sh ${AUTO_GEN_PATH}/gen_definition.sh || exit 1

	

sh ${AUTO_GEN_PATH}/sysInfo.sh
REF_INFO=${AUTO_GEN_OUT}/ref_info
source ${REF_INFO}


if [ "$1" == "DDI_MAIN" ]
	then
		echo "DDI_MAIN" $1
else
		echo "Not DDI_MAIN" $1
fi

source ${VAR_LIST}

echo "CHIP=$CHIP" >> ${CONF_INFO}
TOOLCHAIN=`echo $READ_ELF | sed 's/-readelf//g'`
echo "TOOLCHAIN=$TOOLCHAIN" >> ${CONF_INFO}

#check kernel version and FS type first
echo FS_TYPE=${FS_TYPE}
echo LINUX_KERNEL_VERSION=${LINUX_KERNEL_VERSION}


let MIN_IMAGE_SIZE=$((0x300000 * (${NAND_PAGE_SIZE} / 2048)))
if [ "$FS_TYPE" == "cramfs" ]
	then
		echo "RW_FS=r" >> ${VAR_LIST}
else
		echo "RW_FS=rw" >> ${VAR_LIST}
fi

echo "### Definition by Auto_Gen ###" >> $VAR_LIST

echo "********************************************"
echo "************  Auto Gen Scripts  ************"
echo "********************************************"
echo "### Info from config ###"
if [ "$FLASH_TYPE" == "SPI" ]
	then
		KERNEL_IMG_NAME=uImage.gz
elif [ "$LINUX_KERNEL_VERSION" == "3.1.10" ] ||  [ "$LINUX_KERNEL_VERSION" == "3.10.23" ]
	then
		KERNEL_IMG_NAME=uImage
else		
		KERNEL_IMG_NAME=uImage.lzo
fi		


if [ "$LINUX_KERNEL_VERSION" == "2.6.28" ]
	then
		LINUX_KERNEL_PATH=../linux_kernel/2.6.28
		LINUX_KERNEL_NAME=2_6_28
elif [ "$LINUX_KERNEL_VERSION" == "3.1.10" ]
	then
		LINUX_KERNEL_PATH=../linux_kernel/3.1.10
		LINUX_KERNEL_NAME=3_1_10
elif [ "$LINUX_KERNEL_VERSION" == "3.8.13" ]
	then
		LINUX_KERNEL_PATH=../linux_kernel/3.8.13
		LINUX_KERNEL_NAME=3_8_13
elif [ "$LINUX_KERNEL_VERSION" == "3.10.23" ]
	then
		LINUX_KERNEL_PATH=../linux_kernel/3.10.23
		LINUX_KERNEL_NAME=3_10_23
else
		echo "!!! Unknown LINUX_KERNEL_VERSION : $(LINUX_KERNEL_VERSION) !!!"
		exit 1
fi		


echo "KERNEL_IMG_NAME=${KERNEL_IMG_NAME}" >> ${VAR_LIST}
echo "ZRAM=${ZRAM}" >> ${CONF_INFO}



echo "###!! clean and make usb_update !!###"
if [ -d ${USB_PATH}/ ]
	then
		rm -rf ${USB_PATH}/
fi		
mkdir ${USB_PATH}/
mkdir ${USB_PATH}/Target/
mkdir ${USB_PATH}/Target/image/
mkdir ${USB_PATH}/Target/script/


if [ "$TFTP_UPDATE" == "enable" ]
	then
		echo "###!! clean and make tftp image folder !!###"
		rm -rf ${TFTP_PATH}/${FLASH_TYPE}
		mkdir -p ${TFTP_PATH}/${FLASH_TYPE}
fi


echo "### Set file info ###"
RFS_FILE=[[ROOTFS
APP_FILE=[[application
CONF_FILE=[[${CFG_FOLDER_NAME}
KERNEL_FILE=[[kernel
MWLIB_FILE=[[mwlib
LIB_FILE=[[libdynamic
CUST_FILE=[[customer
PART_FILE=${USB_PATH}/Target/script/set_partition
UPDATE_FILE=${TFTP_PATH}/auto_update.txt
USB_UPDATE_FILE=${USB_PATH}/Target/script/usb_auto_update.txt
SET_CONFIG_FILE=${USB_PATH}/Target/script/set_config
REF_INFO=${AUTO_GEN_OUT}/ref_info


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
if [ -f ${PRJ_DIR}/gdbc.sh ]
  then
    rm -f ${PRJ_DIR}/gdbc.sh   
fi
ln -s ${MISC_DIR}/gdb/gdbc.sh ${PRJ_DIR}/gdbc.sh

echo "### Prepare AutoGen Command ###"
if [ -f ${PRJ_DIR}/agc.sh ]
  then
    rm -f ${PRJ_DIR}/agc.sh
fi
ln -s ${MISC_DIR}/auto_gen/agc.sh ${PRJ_DIR}/agc.sh

echo "### Generate change list info ###"
sh ${AUTO_GEN_PATH}/parse_change_list.sh || err_exit "parse_change_list.sh execute fail!";


#echo "### Get MMAP info ###"
#sh ${AUTO_GEN_PATH}/parse_mmap.sh $MMAP || (echo "parse_mmap.sh executed fail!! $$?"; exit 1)

printf "  NAND_SIZE=0x%x(defined in platform_define)\n" $NAND_SIZE

# ROOTFS handle function #
function rebuild_rfs()
{
	if [ -d ROOTFS/ ]
		then
			sudo chown -R $USER ROOTFS/
			rm -rf ROOTFS/
	fi
	echo $PWD
	sudo tar -${TAR_OPTION} $TAR_FILE || err_exit "Un-tar ROOTFS fail!!"
	sudo chown -R $USER ROOTFS/
	let RE_GEN_RFS=1
}


# partition size calcaulte function #
function cal_size()
{
  let tmp1_size=0
  let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
  until [ $tmp1_size -eq $tmp2_size ]
  do
    let tmp1_size=$tmp2_size
    let tmp2_size=`du -cbs $1 | awk '!a[$1]++' | awk '{print $1}'`
  done

  let CAL_SIZE=$((tmp2_size + $RESERVED_SIZE +$2))
  let CAL_SIZE=$((($CAL_SIZE + $ALIGN_SIZE) / $ALIGN_SIZE * $ALIGN_SIZE))

	if [ "$FS_TYPE" == "ubifs" ] || [ "$FS_TYPE" == "ramdisk" ]
		then
			if [ $CAL_SIZE -lt $MIN_IMAGE_SIZE ]
				then
					let CAL_SIZE=$MIN_IMAGE_SIZE
			fi
	fi
}


# Generate ramdisk target.bin #
function gen_ramdisk_target()
{
		function hex()
		{
			printf '0x%08x' $1
		}
	
		if [ "$FLASH_TYPE" == "SPI" ]
			then
				KERNEL=${LINUX_KERNEL_PATH}/${FS_TYPE}/SPI/${KERNEL_IMG_NAME}
		else
				KERNEL=${LINUX_KERNEL_PATH}/${KERNEL_IMG_NAME}
		fi
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


if [ "$FLASH_TYPE" == "SPI" ]
	then
	
		# Assign kernel image path #
		KERNEL_IMG_PATH=${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}
		echo "KERNEL_IMG_PATH=$KERNEL_IMG_PATH"

		echo "### Deal with ROOTFS ###"
		let RE_GEN_RFS=0
		TAR_FILE=${LINUX_KERNEL_PATH}/${FS_TYPE}/SPI/ROOTFS_${LINUX_KERNEL_NAME}_${FS_TYPE}.${TAR_EXT_NAME}
		echo "tar file path name : $TAR_FILE"
		
		# extract ROOTFS/ if not exist #
		if [ ! -d ROOTFS/ ]
			then
				echo "ROOTFS folder not exist, build ROOTFS !!"
				rebuild_rfs
		elif [ ! -f $LINUX_KERNEL_PATH/${FS_TYPE}/SPI/ROOTFS.$PART_EXT_NAME ]
			then
				echo "ROOTFS.$PART_EXT_NAME not exist, build ROOTFS !!"
				rebuild_rfs
		else
        # get PREV_FS_TYPE #
        if [ "$PrevFsType" != "$FsType" ]
	        then		
		        echo "File System Type changed, re-build ROOTFS !!"
		        rebuild_rfs
        fi

        # get PREV_RFS_TS #
	      if [ "$RfsTS" != "$PrevRfsTS" ]
	        then		
		        echo "ROOTFS updated, re-build ROOTFS !!"
		        rebuild_rfs
        fi
		fi
		
		
		echo "###!! Generate build_XXX related !!###"
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_SPI_app_${FS_TYPE}.sh auto_gen_output/application 0 application || err_exit "Execute gen_build_partition.sh fail!!"
				sh build_SPI_app_${FS_TYPE}.sh
				
				if [ "$RE_GEN_RFS" -ne "0" ]
					then
						sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_SPI_rfs_${FS_TYPE}.sh ROOTFS 0 ROOTFS || err_exit "Execute gen_build_partition.sh fail!!"
						sh build_SPI_rfs_${FS_TYPE}.sh
						cp ../usb_update/Target/image/ROOTFS.sqsh ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/
				else
						[ -f build_SPI_rfs_${FS_TYPE}.sh ] && mv build_SPI_rfs_${FS_TYPE}.sh build_rfs_${FS_TYPE}_backup.sh
				fi			
		fi
		
	
		echo "### Generate target partition ###"
		if [ "$FS_TYPE" == "ramdisk" ]
			then
				if [ ! -d ROOTFS ]; then
					err_exit "Please unpack ROOTFS folder first"
				fi	
			
				gen_ramdisk_target
		fi

		echo "### Get size info ###"
		let ALIGN_SIZE=0x10000
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				# get APP_SIZE
				cal_size ../usb_update/Target/image/application.sqsh 0
				printf "APP_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
		else
				APP_SIZE=0
		fi
		
		# get KERNEL_SIZE
		cal_size ${KERNEL_IMG_PATH}/uImage.gz 0
		printf "KERNEL_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
		
		# get RFS_SIZE
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				cal_size ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/ROOTFS.sqsh 0
				printf "RFS_SIZE=0x%08X\n" $(($CAL_SIZE+0x10000)) >> ${VAR_LIST}
				RFS_SIZE=$CAL_SIZE
		else
				RFS_SIZE=0;	
		fi		
		
		# get RAM_DISK_BIN_SIZE
		if [ "$FS_TYPE" == "ramdisk" ]
			then
				# store variable to $VAR_LIST
				echo "RAMDISK_START=${RAMDISK_START}" >> ${VAR_LIST}
				echo "RAMDISK_SIZE=${RAMDISK_SIZE}" >> ${VAR_LIST}
				echo "RAM_DISK_BIN_SIZE=${RAM_DISK_BIN_SIZE}" >> ${VAR_LIST}				
		fi		
		source $VAR_LIST
		
		
		echo "###!! Generate [[xxx script files !!###"
		# generate [[ROOTFS #
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		
		# generate [[application #
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				if [ "$TFTP_UPDATE" == "enable" ]
					then
						sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
				fi
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
				
		# generate [[kernel #
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		
		
		
		echo "###!! Generate set_partition !!###"
		sh ${AUTO_GEN_PATH}/gen_set_partition.sh $PART_FILE || err_exit "Execute gen_set_partition.sh fail!!"
		
		
		
		echo "###!! Generate auto_update.txt and usb_auto_update.txt !!###"
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_auto_update.sh $UPDATE_FILE mstar || err_exit "Execute gen_auto_update.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_auto_update.sh $USB_UPDATE_FILE ${USB_UPDATE_CMD} || err_exit "Execute gen_auto_update.sh fail!!"
		
		
		
		echo "###!! Generate set_config !!###"
		sh ${AUTO_GEN_PATH}/gen_set_config.sh $SET_CONFIG_FILE || err_exit "Execute gen_set_config.sh fail!!"



		echo "###!! Prepare SPI update material !!###"
		cp -f ${BSP_PRJ}/bsp/linux/partition/miu_setting${MIU_TYPE}.txt ${USB_PATH}/Target/script || err_exit "Copy miu_setting.txt fail!!"
		cp -f ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} ${USB_PATH}/Target/image || err_exit "Copy kernel image fail!!"
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				cp -f ${LINUX_KERNEL_PATH}/${FS_TYPE}/${FLASH_TYPE}/ROOTFS.sqsh ${USB_PATH}/Target/image || err_exit "Copy ROOTFS partition fail!!"	
		fi


		echo "### Generate quick links ###"
		if [ "$FS_TYPE" != "ramdisk" ]
			then
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
else
		# Assign kernel image path #
		if [ "$CHIP" == "kaiser" ] || [ "$CHIP" == "keltic" ] || [ "$CHIP" == "clippers" ]
			then
				KERNEL_IMG_PATH=${LINUX_KERNEL_PATH}/${FS_TYPE}
		else
				KERNEL_IMG_PATH=${LINUX_KERNEL_PATH}
		fi

		echo "### Deal with ROOTFS ###"

		let RE_GEN_RFS=0
		TAR_FILE=${LINUX_KERNEL_PATH}/${FS_TYPE}/ROOTFS_${LINUX_KERNEL_NAME}_${FS_TYPE}.${TAR_EXT_NAME}
		echo "tar file path name : $TAR_FILE"
		
		# extract ROOTFS/ if not exist #
		if [ ! -d ROOTFS/ ]
			then
				echo "ROOTFS folder not exist, build ROOTFS !!"
				rebuild_rfs
		elif [ ! -f $LINUX_KERNEL_PATH/${FS_TYPE}/ROOTFS.$PART_EXT_NAME ]
			then
				echo "ROOTFS.$PART_EXT_NAME not exist, build ROOTFS !!"
				rebuild_rfs
		else
        # get PREV_FS_TYPE #
        if [ "$PrevFsType" != "$FsType" ]
	        then		
		        echo "File System Type changed, re-build ROOTFS !!"
		        rebuild_rfs
        fi
        
        # get PREV_LINUX_KERNEL_VERSION #
        if [ "$PrevLinuxKernelVersion" != "$LinuxKernelVersion" ]
	        then		
		        echo "Kernel Version changed, re-build ROOTFS !!"
		        rebuild_rfs
        fi
        
        # get PREV_NAND_PAGE_SIZE #
        if [ "$PrevNandPageSize" -ne "$NandPageSize" ]
	        then
		        echo "NAND PAGE SIZE changed, re-build ROOTFS !!"
		        rebuild_rfs
        fi
        
        # get PREV_RFS_TS #
	      if [ "$RfsTS" != "$PrevRfsTS" ]
	        then		
		        echo "ROOTFS updated, re-build ROOTFS !!"
		        rebuild_rfs
        fi        
		fi
		
		

		echo "### Get size info ###"
		let ALIGN_SIZE=$((1024*1024))
		# get APP_SIZE
		cal_size ${AUTO_GEN_OUT}/application/ $EXT_APP_SIZE
		printf "APP_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
		
		# get CONF_SIZE
		cal_size ${AUTO_GEN_OUT}/${CFG_FOLDER_NAME}/ $EXT_CONF_SIZE
		printf "CONF_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
		
		# get KERNEL_SIZE
		cal_size ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} $EXT_KERNEL_SIZE
		printf "KERNEL_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
		
		# get RFS_SIZE
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				cal_size ROOTFS/ $EXT_RFS_SIZE
				let RFS_SIZE=$CAL_SIZE
				if [ "$FS_TYPE" == "yaffs2" ]
					then
						RFS_SIZE=$(($RFS_SIZE+0x400000))
				fi
				printf "RFS_SIZE=0x%08X\n" $RFS_SIZE >> ${VAR_LIST}
		else
				RFS_SIZE=0;	
		fi
		
		#get MWLIB_SIZE
		if [ "$RELEASE_MW_LIB" == "enable" ]
			then
				cal_size ${AUTO_GEN_OUT}/mwlib $EXT_MWLIB_SIZE
				printf "MWLIB_SIZE=0x%08X\n" $CAL_SIZE >> ${VAR_LIST}
		fi
		
		# get LIB_SIZE
		if [ "$LINK_TYPE" == "dynamic" ]
			then
				if [ -d auto_gen_output/lib_dynamic ]
					then
						cal_size auto_gen_output/lib_dynamic $EXT_LIB_SIZE
				else
						cal_size ../../lib_dynamic $EXT_LIB_SIZE
				fi
				let LIB_SIZE=$CAL_SIZE
				echo "LINK_TYPE=${LINK_TYPE}" >> ${CONF_INFO}
		else
				rm -f ../tftp_root/${FLASH_TYPE}/lib_dynamic.${FS_TYPE}
				rm -f ../tftp_root/[[libdynamic
				let LIB_SIZE=0
		fi
		printf "LIB_SIZE=0x%08X\n" $LIB_SIZE >> ${VAR_LIST}
		
		# get RAM_DISK_BIN_SIZE
		if [ "$FS_TYPE" == "ramdisk" ]
			then
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
		
	
	
		echo "###!! Generate [[xxx script files !!###"
		# generate [[ROOTFS #
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		
		# generate [[application #
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $APP_FILE $APP_SIZE application APP /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		
		# generate [[config #
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $CONF_FILE $CONF_SIZE ${CFG_FOLDER_NAME} CON ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $CONF_FILE $CONF_SIZE ${CFG_FOLDER_NAME} CON /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		
		# generate [[kernel #
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $KERNEL_FILE $KERNEL_SIZE kernel KL /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		
		# generate [[mwlib #
		if [ "$RELEASE_MW_LIB" == "enable" ]
			then
				if [ "$TFTP_UPDATE" == "enable" ]
					then
						sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $MWLIB_FILE $MWLIB_SIZE mwlib MWLIB ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
				fi
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $MWLIB_FILE $MWLIB_SIZE mwlib MWLIB /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		
		# generate [[libdynamic #
		if [ "$LINK_TYPE" == "dynamic" ]
			then
				if [ "$TFTP_UPDATE" == "enable" ]
					then
						sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $LIB_FILE $LIB_SIZE lib_dynamic LIB ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
				fi
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $LIB_FILE $LIB_SIZE lib_dynamic LIB /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		fi
		
		# generate [[customer #
		if [ "$MW_FLOW" == "enable" ]
			then
				if [ "$TFTP_UPDATE" == "enable" ]
					then
						sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $CUST_FILE $CUST_SIZE customer CUST ${FLASH_TYPE} || err_exit "Execute gen_[[xxx.sh fail!!"
				fi
				sh ${AUTO_GEN_PATH}/gen_[[xxx.sh $CUST_FILE $CUST_SIZE customer CUST /Target/image || err_exit "Execute gen_[[xxx.sh fail!!"
		fi



		echo "###!! Generate set_partition !!###"
		sh ${AUTO_GEN_PATH}/gen_set_partition.sh $PART_FILE || err_exit "Execute gen_set_partition.sh fail!!"
		
		
		
		echo "###!! Generate auto_update.txt and usb_auto_update.txt !!###"
		if [ "$TFTP_UPDATE" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_auto_update.sh $UPDATE_FILE mstar || err_exit "Execute gen_auto_update.sh fail!!"
		fi
		sh ${AUTO_GEN_PATH}/gen_auto_update.sh $USB_UPDATE_FILE ${USB_UPDATE_CMD} || err_exit "Execute gen_auto_update.sh fail!!"
		
		
		
		echo "###!! Generate set_config !!###"
		sh ${AUTO_GEN_PATH}/gen_set_config.sh $SET_CONFIG_FILE || err_exit "Execute gen_set_config.sh fail!!"



		echo "###!! Generate build_XXX related !!###"
		if [ "$RE_GEN_RFS" -ne "0" ]
			then
				sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_rfs_${FS_TYPE}.sh ROOTFS $RFS_SIZE ROOTFS || err_exit "Execute gen_build_partition.sh fail!!"
		else
				[ -f build_rfs_${FS_TYPE}.sh ] && mv build_rfs_${FS_TYPE}.sh build_rfs_${FS_TYPE}_backup.sh
		fi
		sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_app_${FS_TYPE}.sh auto_gen_output/application $APP_SIZE application || err_exit "Execute gen_build_partition.sh fail!!"
		sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_${CFG_FOLDER_NAME}_${FS_TYPE}.sh auto_gen_output/${CFG_FOLDER_NAME} $CONF_SIZE ${CFG_FOLDER_NAME} || err_exit "Execute gen_build_partition.sh fail!!"
		if [ "$RELEASE_MW_LIB" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_mwlib_${FS_TYPE}.sh auto_gen_output/mwlib $MWLIB_SIZE mwlib || err_exit "Execute gen_build_partition.sh fail!!"
		fi
		if [ "$LINK_TYPE" == "dynamic" ]
			then
				if [ -d auto_gen_output/lib_dynamic ]
					then
						sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_lib_${FS_TYPE}.sh auto_gen_output/lib_dynamic $LIB_SIZE  lib_dynamic || err_exit "Execute gen_build_partition.sh fail!!"
				else
						sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_lib_${FS_TYPE}.sh ../../lib_dynamic $LIB_SIZE  lib_dynamic || err_exit "Execute gen_build_partition.sh fail!!"
				fi
		fi
		if [ "$MW_FLOW" == "enable" ]
			then
				sh ${AUTO_GEN_PATH}/gen_build_partition.sh build_cust_${FS_TYPE}.sh auto_gen_output/customer $CUST_SIZE  customer || err_exit "Execute gen_build_partition.sh fail!!"
		fi


		echo "###!! Generate partitions !!###"
		sh build_app_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
		sh build_${CFG_FOLDER_NAME}_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
		if [ "$RELEASE_MW_LIB" == "enable" ]
			then
				sh build_mwlib_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
		fi
		#echo ${LINUX_KERNEL_PATH}/${KERNEL_IMG_NAME}
		cp -f ${KERNEL_IMG_PATH}/${KERNEL_IMG_NAME} ${USB_PATH}/Target/image || err_exit "Copy kernel image fail!!"
		if [ "$FS_TYPE" != "ramdisk" ]
			then		
				if [ "$RE_GEN_RFS" -ne "0" ]
					then 
						sh build_rfs_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
						cp -f ${USB_PATH}/Target/image/ROOTFS.${PART_EXT_NAME} ${LINUX_KERNEL_PATH}/${FS_TYPE}/ROOTFS.${PART_EXT_NAME} || err_exit "Backup ROOTFS partition fail!!"
				else
						cp -f ${LINUX_KERNEL_PATH}/${FS_TYPE}/ROOTFS.${PART_EXT_NAME} ${USB_PATH}/Target/image/ROOTFS.${PART_EXT_NAME} || err_exit "Copy ROOTFS partition from backup fail!!"
				fi
		fi	
		if [ "$LINK_TYPE" == "dynamic" ]
			then
				sh build_lib_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
		fi
		if [ "$MW_FLOW" == "enable" ]
			then
				sh build_cust_${FS_TYPE}.sh || err_exit "Build partition.sh fail!!"
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
		if [ "$TFTP_UPDATE" == "enable" ]
	then
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
		if [ "$FS_TYPE" == "ramdisk" ]
			then
				cp -f ${USB_PATH}/Target/image/target.bin  ${TFTP_PATH}/${FLASH_TYPE}/ || err_exit "Copy target.bin fail!!"
		fi
		echo " ***************               Update Done.                     ****************"
		echo "********************************************************************************"
fi



echo "### Check MMAP ###"
cd ${PRJ_DIR}
if [ "$1" == "DDI_MAIN" ]
	then
		sh ${AUTO_GEN_PATH}/check_mmap.sh ${TARGET_PATH} set_config configurations/platform_configuration || err_exit "check_mmap.sh executed fail!! $$?"
else
		sh ${AUTO_GEN_PATH}/check_mmap.sh ${TARGET_PATH} set_config config || err_exit "check_mmap.sh executed fail!! $$?"
fi


echo "### Check Utopia Lib Path ###"
if [ "$UTOPIA_PATH" != "" ]
	then		
		echo "Utopia path : //${UTOPIA_PATH}"
		echo "The source path list below may not the same as Utopia lib path"
		echo -e "\e[1;33;40m******************************"
		if [ "$LINK_TYPE" == "dynamic" ]
			then
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
