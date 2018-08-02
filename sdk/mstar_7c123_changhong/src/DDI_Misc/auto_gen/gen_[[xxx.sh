#sh auto_gen/gen_[[xxx.sh $RFS_FILE $RFS_SIZE ROOTFS RFS NAND  <<-- example of each parameter
#   $0                    $1        $2        $3     $4  $5

source ${AUTO_GEN_OUT}/var_list

# assign kernel image name & set partition ext name
if [ "$FS_TYPE" == "ramdisk" ] && [ "$3" == "ROOTFS" ]
	then
		exit 0;
fi

need_to_signed=false

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
elif [ "$TOOLCHAIN" == "aarch64-linux-gnu" ]
	then
		TFTP_ADR=25000000
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
	    if [ "$DDI_TEE" == "enable" ] || [ "$DDI_SECURE_UBOOT" == "enable" ]; then
            KERNEL_IMG_NAME="$KERNEL_IMG_NAME".aes
        fi

		if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]
			then
				echo "tftp ${TFTP_ADR} $5/${KERNEL_IMG_NAME}" >> $FILE_PATH
		else
				echo "fatload usb 0 ${TFTP_ADR} $5/${KERNEL_IMG_NAME}" >> $FILE_PATH
		fi

elif [ "$3" == "tee" ]; then
    if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]; then
        echo "tftp 0x${TFTP_ADR} $5/tee.aes" >> $FILE_PATH
    else
        echo "fatload usb 0 ${TFTP_ADR} $5/tee.aes" >> $FILE_PATH
    fi


elif  [ "$3" == "dtb" ]
	then
		if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]
			then
				echo "tftp ${TFTP_ADR} $5/DTB.dtb" >> $FILE_PATH
		else
				echo "fatload usb 0 ${TFTP_ADR} $5/DTB.dtb" >> $FILE_PATH
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
		if [ "$FS_TYPE" == "sqsh" ] || [ "$FS_TYPE" == "ramdisk" ]; then
			if [ "$CHIP" == "kratos" ]; then
			    printf "spi wrc.p 0x${TFTP_ADR} $4 0 0x%08X\n" ${IMG_SIZE} >> $FILE_PATH
			else
			    printf "spi_wrc_p 0x${TFTP_ADR} $4 0 0x%08X\n" ${IMG_SIZE} >> $FILE_PATH
			fi
		fi
elif [ "$FLASH_TYPE" == "EMMC" ]
    then
        if [ "$FS_TYPE" == "ext4" ]
            then
            echo "setenv fileaddr 0x${TFTP_ADR}" >> $FILE_PATH
            echo "mmc write.p \${fileaddr} $4 \${filesize}" >> $FILE_PATH
            if [ "$3" == "kernel" ]
                then
                if [ "$CHIP" == "muji" ]
                then
                    echo "setenv bootcmd mmc read.p 0x23000000 DTB 0x1000\;mmc read.p 0x25000000 KL \${filesize}\; bootm 0x25000000" >> $FILE_PATH
                else
                    echo "setenv bootcmd mmc read.p 0x25000000 KL \${filesize}\; bootm 0x25000000" >> $FILE_PATH
                fi
                echo "saveenv" >> $FILE_PATH
            fi
        elif [ "$FS_TYPE" == "ramdisk" ]
            then
            echo "gen_[[xxx ramdisk emmc "
            echo "setenv fileaddr 0x${TFTP_ADR}" >> $FILE_PATH
            echo "mmc write.p \${fileaddr} $4 \${filesize}" >> $FILE_PATH
            if [ "$3" == "kernel" ]
                then
                if [ "$CHIP" == "kaiser" ]
                then
                echo "setenv bootcmd mmc read.p 0x25000000 KL \${filesize}\; bootm 0x25000000" >> $FILE_PATH
                fi
            fi
            echo "saveenv" >> $FILE_PATH
        fi
else
        if [ "$DDI_TEE" == "enable" -o "$DDI_SECURE_UBOOT" == "enable" ] && [ "$3" == "kernel" ]; then
            echo "nand erase.part KL" >> $FILE_PATH
            printf "nand write.e 0x${TFTP_ADR} KL \$(filesize)"  >> $FILE_PATH
            echo >> $FILE_PATH

        elif [ "$DDI_TEE" == "enable" ] && [ "$3" == "tee" ]; then
            echo "nand erase.part tee" >> $FILE_PATH
            printf "nand write.e 0x${TFTP_ADR} tee \$(filesize)"  >> $FILE_PATH
            echo >> $FILE_PATH
            echo >> $FILE_PATH

            if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]; then
                echo "tftp ${TFTP_ADR} $5/nuttx_config.bin" >> $FILE_PATH
            else
                echo "fatload usb 0 ${TFTP_ADR} $5/nuttx_config.bin" >> $FILE_PATH
            fi
            echo "store_nuttx_config NuttxConfig ${TFTP_ADR}" >> $FILE_PATH
            echo >> $FILE_PATH

		elif [ "$FS_TYPE" == "ubifs" ] || [ "$FS_TYPE" == "ramdisk" ]
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
						if [ "$CHIP" == "muji" ]
						then
							echo "setenv bootcmd mmc read.p 0x23000000 DTB 0x1000\;mmc read.p 0x25000000 KL \${filesize}\; bootm 0x25000000" >> $FILE_PATH
						else
						echo "setenv bootcmd mmc read.p 0x25000000 KL \${filesize}\; bootm 0x25000000" >> $FILE_PATH
						fi
						echo "saveenv" >> $FILE_PATH
				fi
		elif [ "$FS_TYPE" == "yaffs2" ]
			then
				echo "nand erase $4" >> $FILE_PATH
				printf "nand write.yaffs 0x$TFTP_ADR $4 0x%08X\n" ${IMG_SIZE} >> $FILE_PATH
		fi


        # sign tee and kernel raw partition if DDI_TEE is enable
        if [ "$DDI_TEE" == "enable" ] && [ "$3" == "tee" -o "$3" == "kernel" ]; then
            signed_partition="$3"
            need_to_signed=true
        # sign kernel and other parititons when DDI_SECURE_UBOOT is enable
        # customer partition no need to sign
        elif [ "$DDI_SECURE_UBOOT" == "enable" ] && [ "$3"  != "tee" ] && [ "$3" != "customer" ] ; then
            signed_partition="$3"
            need_to_signed=true
        fi


        if [ "$need_to_signed" == true ] ; then
            if [ "$5" == "NAND" ] || [ "$5" == "EMMC" ] || [ "$5" == "SPI" ]; then
                echo "tftp ${TFTP_ADR} $5/secure_info_${signed_partition}.bin" >> $FILE_PATH
            else
                echo "fatload usb 0 ${TFTP_ADR} $5/secure_info_${signed_partition}.bin" >> $FILE_PATH
            fi
            echo "store_secure_info ${signed_partition}Sign ${TFTP_ADR}" >> $FILE_PATH
            need_to_signed=false
        fi

fi
echo >> $FILE_PATH
echo "% <- this is end of file symbol" >> $FILE_PATH
