#sh auto_gen/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS NAND  <<-- example of each parameter
#   $0                    $1        $2        $3     $4  $5 

source ${AUTO_GEN_OUT}/var_list

# assign kernel image name & set partition ext name
if [ "$FS_TYPE" == "ramdisk" ] && [ "$3" == "ROOTFS" ]
	then
		exit 0;
fi


# set image size
if [ "$FS_TYPE" == "ramdisk" ] && [ "$3" == "kernel" ]
	then
		IMG_SIZE=${RAM_DISK_BIN_SIZE}
		KERNEL_IMG_NAME=target.bin
else
		IMG_SIZE=$2
fi		

# setup file path (tftp or usb)
if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]
	then
		FILE_PATH=${TFTP_PATH}/$1
		echo "  Generate $1"
else
		FILE_PATH=${USB_PATH}/Target/script/$1
		echo "  Generate $1 for usb update"
fi

# set TFTP address
if [ "$TOOLCHAIN" == "arm-none-linux-gnueabi" ]
	then
		TFTP_ADR=20400000
else
		TFTP_ADR=80400000
fi

echo "# <- this is for comment / total file size must be less than 4KB" > $FILE_PATH
echo >> $FILE_PATH
echo "#$3" >> $FILE_PATH

if [ "$FS_TYPE" == "yaffs2" ] && [ "$3" != "kernel" ]
	then
		printf "mw.b 0x${TFTP_ADR} 0xff 0x%x\n" ${IMG_SIZE} >> $FILE_PATH 
fi

if [ "$3" == "kernel" ]
	then 
		if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]
			then
				echo "tftp ${TFTP_ADR} $5/${KERNEL_IMG_NAME}" >> $FILE_PATH
		else
				echo "fatload usb 0 ${TFTP_ADR} $5/${KERNEL_IMG_NAME}" >> $FILE_PATH
		fi		
else
		if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]
			then
				echo "tftp 0x${TFTP_ADR} $5/$3.${PART_EXT_NAME}" >> $FILE_PATH
		else
				echo "fatload usb 0 ${TFTP_ADR} $5/$3.${PART_EXT_NAME}" >> $FILE_PATH
		fi
fi


if [ "$FLASH_TYPE" == "SPI" ]
	then
		if [ "$FS_TYPE" == "sqsh" ] || [ "$FS_TYPE" == "ramdisk" ]
			then
				printf "spi_wrc_p 0x${TFTP_ADR} $4 0 0x%08X\n" ${IMG_SIZE} >> $FILE_PATH
		fi
else	
		if [ "$FS_TYPE" == "ubifs" ] || [ "$FS_TYPE" == "ramdisk" ]
			then
				echo "ubi part UBI" >> $FILE_PATH
				printf "ubi write 0x${TFTP_ADR} $4 0x%x\n" ${IMG_SIZE} >> $FILE_PATH
		elif [ "$FS_TYPE" == "jffs2" ] || [ "$FS_TYPE" == "sqsh" ] || [ "$FS_TYPE" == "cramfs" ]
			then
				echo "nand erase $4" >> $FILE_PATH
				printf "nand write.e 0x${TFTP_ADR} $4 0x%08X\n" ${IMG_SIZE} >> $FILE_PATH
		elif [ "$FS_TYPE" == "ext4" ]
			then
				echo "setenv fileaddr 0x${TFTP_ADR}" >> $FILE_PATH
				echo "mmc write.p \${fileaddr} $4 \${filesize}" >> $FILE_PATH
				if [ "$3" == "kernel" ]
					then
						echo "setenv bootcmd mmc read.p 0x25000000 KL \${filesize}\; bootm 0x25000000" >> $FILE_PATH
						echo "saveenv" >> $FILE_PATH
				fi
		elif [ "$FS_TYPE" == "yaffs2" ]
			then
				echo "nand erase $4" >> $FILE_PATH
				printf "nand write.yaffs 0x$TFTP_ADR $4 0x%08X\n" ${IMG_SIZE} >> $FILE_PATH		
		fi
fi		

echo >> $FILE_PATH
echo "% <- this is end of file symbol" >> $FILE_PATH
