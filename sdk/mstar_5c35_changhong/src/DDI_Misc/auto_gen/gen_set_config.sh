#sh auto_gen/gen_set_config.sh $SET_CONFIG_FILE <<-- example of each parameter
#   $0                         $1              

source ${AUTO_GEN_OUT}/var_list

echo "# <- this is for comment / total file size must be less than 4KB" > $1
echo >> $1
if [ "$CHIP" == "kaiser" ]
	then
		echo "# LX_MEM and LX_MEM2 must 4096 byte alignment." >> $1
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=${DRAM_LEN0} DRAM_SIZE2=${DRAM_LEN0}\n" >> $1
		echo >> $1
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN} BB_ADDR=${BB_ADDR}" $((0x20000000+${GMAC_MEM})) $((0x20000000+${LX_MEM2})) $(($BUS_ADDR+$LX_MEM3)) >> $1
elif [ "$CHIP" == "keres" ]
	then
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN}  BB_ADDR=0" $((0x00000000+${GMAC_MEM})) ${LX_MEM2} >> $1
elif [ "$CHIP" == "clippers" ]
	then
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=0x20000000 DRAM_SIZE2=0x20000000\n" >> $1
		echo >> $1
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN}" $((0x20000000+${LX_MEM2})) $(($BUS_ADDR+$LX_MEM3)) >> $1
else
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%x,${LX_MEM2_LEN} BB_ADDR=${BB_ADDR}" $(($BUS_ADDR+$LX_MEM2)) >> $1

fi
echo >> $1

if [ "$FLASH_TYPE" == "SPI" ]
	then
		if [ "$FS_TYPE" == "sqsh" ]
			then
				echo "setenv MS_RFS root=/dev/mtdblock2 rootfstype=squashfs r" >> $1
				echo  >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) \$(mtdparts)\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' spi_rdc_p ${KERNEL_START} KL 0 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		elif [ "$FS_TYPE" == "ramdisk" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS root=/dev/ram0 rw" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) rd_start=${RAMDISK_START} rd_size=${RAMDISK_SIZE}\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' spi_rdc_p ${KERNEL_START} KL 0 0x%x; bootm ${KERNEL_START};\n" $RAM_DISK_BIN_SIZE >> $1
		fi
else
		if [ "$FS_TYPE" == "ubifs" ]
			then
				if [ "$CHIP" == "kaiser" ]
					then
						echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=ubi:RFS rootfstype=${FS_TYPE} rw" >> $1
						printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} \${mtdparts} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz\n" >> $1
						printf "setenv bootcmd ' ubi part UBI;ubi read ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
				elif [ "$CHIP" == "keres" ]
						then
						echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=ubi:RFS rootfstype=${FS_TYPE} rw" >> $1
						printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} \${mtdparts} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz\n" >> $1
						printf "setenv bootcmd ' ubi part UBI;ubi read ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};'\n" $KERNEL_SIZE >> $1			
				else
						echo "setenv MS_GOP0_MIU 0" >> $1
						echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=ubi:RFS rootfstype=${FS_TYPE} rw" >> $1
						printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts)\n" >> $1
						printf "setenv bootcmd ' ubi part UBI;ubi read ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		
				fi
		elif [ "$FS_TYPE" == "ext4" ]
			then
                if [ "$CHIP" == "kaiser" ]
                    then
                        echo "setenv MS_RFS root=/dev/mmcblk0p8 rootfstype=ext4 rw rootwait" >> $1
                        printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz\n" >> $1
                else
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 rw rootwait" >> $1
                        printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz\n" >> $1

                fi
		elif [ "$FS_TYPE" == "ramdisk" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=/dev/ram0 rw" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) rd_start=${RAMDISK_START} rd_size=${RAMDISK_SIZE}\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' ubi part UBI;ubi read ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $RAM_DISK_BIN_SIZE >> $1
		elif [ "$FS_TYPE" == "jffs2" ] || [ "$FS_TYPE" == "cramfs" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS root=/dev/mtdblock1 rootfstype=${FS_TYPE} rw" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts)\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' nand read.e ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		elif [ "$FS_TYPE" == "sqsh" ]
			then
				echo "setenv MS_RFS root=/dev/mtdblock3 rootfstype=squashfs r" >> $1
				echo  >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) \$(mtdparts)\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' nand read.e ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		elif [ "$FS_TYPE" == "yaffs2" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS root=/dev/mtdblock1 rootflags=inband-tags rw rootfstype=${FS_TYPE}" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts)\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' nand read.e ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		fi
fi		

echo >> $1
echo "setenv bootdelay 1" >> $1
echo >> $1
echo "setenv verify y" >> $1
echo >> $1
echo "saveenv" >> $1
echo >> $1
echo "% <- this is end of file symbol" >> $1

