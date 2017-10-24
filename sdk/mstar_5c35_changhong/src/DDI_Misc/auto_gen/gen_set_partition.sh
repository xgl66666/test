#sh auto_gen/gen_set_partition.sh $PART_FILE  <<-- example of each parameter
#   $0                            $1         

source ${AUTO_GEN_OUT}/var_list

echo "# <- this is for comment / total file size must be less than 4KB" > $1
echo >> $1

if [ "$FLASH_TYPE" == "SPI" ]
	then	
		if [ "$FS_TYPE" != "ramdisk" ]
			then	
				#check used NAND size
				let USED_NAND_SIZE=0
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$KERNEL_SIZE))
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$RFS_SIZE))
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$APP_SIZE))
				printf "  USED_NAND_SIZE=0x%X\n" $USED_NAND_SIZE
				let NAND_SIZE=$NAND_SIZE
				if [ "$NAND_SIZE" -lt "$USED_NAND_SIZE" ]
					then
						printf "!!! USED_NAND_SIZE=0x%x, over NAND_SIZE=0x%x !!!\n" $USED_NAND_SIZE $NAND_SIZE
						exit 1
				fi
				
				echo >>$1
			
				printf "setenv mtdparts mtdparts=edb8M-SPI:0x70000(MBOOT)," >>$1
				printf "0x%x(KL),"    $KERNEL_SIZE >> $1
				printf "0x%x(RFS),"   $RFS_SIZE >> $1
				printf "0x%x(APP),"   $APP_SIZE >> $1
				echo "-(NA)" >> $1
				echo >>$1			
				
				printf "spi_create KL 0x%x\n" $KERNEL_SIZE >> $1
				printf "spi_create RFS 0x%x\n" $RFS_SIZE >> $1
				printf "spi_create APP 0x%x\n" $APP_SIZE >> $1
		else
				#check used NAND size
				let USED_NAND_SIZE=0
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$KERNEL_SIZE))
				printf "  USED_NAND_SIZE=0x%X\n" $USED_NAND_SIZE
				let NAND_SIZE=$NAND_SIZE
				if [ "$NAND_SIZE" -lt "$USED_NAND_SIZE" ]
					then
						printf "!!! USED_NAND_SIZE=0x%x, over NAND_SIZE=0x%x !!!\n" $USED_NAND_SIZE $NAND_SIZE
						exit 1
				fi
				
				echo >>$1
			
				printf "setenv mtdparts mtdparts=edb8M-SPI:0x70000(MBOOT)," >>$1
				printf "0x%x(KL),"    $RAM_DISK_BIN_SIZE >> $1
				echo "-(NA)" >> $1
				echo >>$1			
				
				printf "spi_create KL 0x%x\n" $RAM_DISK_BIN_SIZE >> $1
		fi		
else
		#check used NAND size
		let USED_NAND_SIZE=0
		let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$KERNEL_SIZE))
		if [ "$FS_TYPE" != "ramdisk" ]
			then
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$RFS_SIZE))
		fi
		let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$CONF_SIZE))
		let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$APP_SIZE))
		if [ "$RELEASE_MW_LIB" == "enable" ]
			then
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$MWLIB_SIZE))
		fi
		if [ "$LINK_TYPE" == "dynamic" ]
			then
				let USED_NAND_SIZE=$((${USED_NAND_SIZE}+$LIB_SIZE))
		fi
		printf "  USED_NAND_SIZE=0x%X\n" $USED_NAND_SIZE
		let NAND_SIZE=$NAND_SIZE
		if [ "$NAND_SIZE" -lt "$USED_NAND_SIZE" ]
			then
				printf "!!! USED_NAND_SIZE=0x%x, over NAND_SIZE=0x%x !!!\n" $USED_NAND_SIZE $NAND_SIZE
				exit 1
		fi
		
		echo >>$1
		
		if [ "$FS_TYPE" == "ubifs" ]
			then
				printf "dynpart edb64M-nand:0x%x(UBI),-(NA)" $NAND_SIZE >>$1
				echo >> $1
				echo "saveenv" >> $1
				echo >>$1
				echo "ubi part UBI" >> $1
				echo "saveenv" >> $1
				echo >>$1
				printf "ubi create KL 0x%x\n" $KERNEL_SIZE >> $1
				printf "ubi create RFS 0x%x\n" $RFS_SIZE >>$1
				printf "ubi create CON 0x%x\n"  $CONF_SIZE >> $1
				printf "ubi create APP 0x%x\n"  $APP_SIZE >> $1
				if [ "$RELEASE_MW_LIB" == "enable" ]
					then
						printf "ubi create MWLIB 0x%x\n"  $MWLIB_SIZE >> $1
				fi
				if [ "$LINK_TYPE" == "dynamic" ]
					then
						printf "ubi create LIB 0x%x\n"  $LIB_SIZE >> $1
				fi
				if [ "$MW_FLOW" == "enable" ]
					then
						printf "ubi create CUST 0x%x\n" ${CUST_SIZE} >> $1
				fi
		elif [ "$FS_TYPE" == "ext4" ]
			then
				let BLK_NO=0
				if [ "$CHIP" == "kaiser" ]
					then
						BLK_NO=$(($BLK_NO+1))
						printf "## Mboot space\n" >> $1
						printf "mmc create MBOOT 0x00300000\n" >> $1
						echo "MBOOT_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "#mmc driver usage\n" >> $1
						printf "mmc create MPOOL 0x00200000\n" >> $1
						echo "MPOOL_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "#Reserved for mboot recovery usage\n" >> $1
						printf "mmc create misc     0x00080000\n" >> $1
						echo "misc_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create recovery 0x00800000\n" >> $1
						echo "recovery_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create boot     0x00400000\n" >> $1
						echo "boot_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create reserved 0x00C80000\n" >> $1
						echo "reserved_BLK_NO=$BLK_NO" >> $CONF_INFO

						BLK_NO=$(($BLK_NO+1))
						printf "## User space\n" >> $1
						printf "mmc create KL 0x%x\n" $KERNEL_SIZE >> $1
						echo "KL_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create RFS 0x%x\n" $RFS_SIZE >>$1
						echo "RFS_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create CON 0x%x\n"  $CONF_SIZE >> $1
						echo "CON_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create APP 0x%x\n"  $APP_SIZE >> $1
						echo "APP_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						if [ "$RELEASE_MW_LIB" == "enable" ]
							then
								printf "mmc create MWLIB 0x%x\n"  $MWLIB_SIZE >> $1
								echo "MWLIB_BLK_NO=$BLK_NO" >> $CONF_INFO
								BLK_NO=$(($BLK_NO+1))
						fi
		
						echo "# USB emmc storage (rename it)" >> $1
						echo "mmc create EMMCSTORAGE 0x2000000" >> $1
						echo "ES_BLK_NO=$BLK_NO" >> $CONF_INFO
						BLK_NO=$(($BLK_NO+1))
						if [ "$LINK_TYPE" == "dynamic" ]
							then
								printf "mmc create LIB 0x%x\n"  $LIB_SIZE >> $1
								echo "LIB_BLK_NO=$BLK_NO" >> $CONF_INFO
								BLK_NO=$(($BLK_NO+1))
						fi
						if [ "$MW_FLOW" == "enable" ]
							then
								printf "mmc create CUST 0x%x\n"  ${CUST_SIZE} >> $1
								echo "CUST_BLK_NO=$BLK_NO" >> $CONF_INFO
								BLK_NO=$(($BLK_NO+1))
						fi
				else
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create KL 0x%x\n" $KERNEL_SIZE >> $1
						echo "KL_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create RFS 0x%x\n" $RFS_SIZE >>$1
						echo "RFS_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create CON 0x%x\n"  $CONF_SIZE >> $1
						echo "CON_BLK_NO=$BLK_NO" >> $CONF_INFO
						
						BLK_NO=$(($BLK_NO+1))
						printf "mmc create APP 0x%x\n"  $APP_SIZE >> $1
						echo "APP_BLK_NO=$BLK_NO" >> $CONF_INFO
												
						if [ "$RELEASE_MW_LIB" == "enable" ]
							then
								BLK_NO=$(($BLK_NO+1))
								printf "mmc create MWLIB 0x%x\n"  $MWLIB_SIZE >> $1
								echo "MWLIB_BLK_NO=$BLK_NO" >> $CONF_INFO								
						fi
						
						BLK_NO=$(($BLK_NO+1))
						echo "# emv partition" >> $1
						echo "mmc create MPOOL 0x100000" >> $1
						echo "MPOOL_BLK_NO=$BLK_NO" >> $CONF_INFO

						BLK_NO=$(($BLK_NO+1))
						echo "# USB emmc storage (rename it)" >> $1
						echo "mmc create EMMCSTORAGE 0x2000000" >> $1
						echo "ES_BLK_NO=$BLK_NO" >> $CONF_INFO

						if [ "$LINK_TYPE" == "dynamic" ]
							then
								BLK_NO=$(($BLK_NO+1))
								printf "mmc create LIB 0x%x\n"  $LIB_SIZE >> $1
								echo "LIB_BLK_NO=$BLK_NO" >> $CONF_INFO
						fi
						if [ "$MW_FLOW" == "enable" ]
							then
								BLK_NO=$(($BLK_NO+1))
								printf "mmc create CUST 0x%x\n"  ${CUST_SIZE} >> $1
								echo "CUST_BLK_NO=$BLK_NO" >> $CONF_INFO
						fi				
				fi		
		elif [ "$FS_TYPE" == "ramdisk" ]
			then
				printf "dynpart edb64M-nand:0x%x(UBI),-(NA)" $NAND_SIZE >>$1
				echo >>$1
				echo "ubi part UBI" >> $1
				echo "set UBI" >> $1
				echo "saveenv" >> $1
				echo >>$1
				printf "ubi create KL 0x%x\n" $RAM_DISK_BIN_SIZE >> $1
				printf "ubi create CON 0x%x\n"  $CONF_SIZE >> $1
				printf "ubi create APP 0x%x\n"  $APP_SIZE >> $1
				if [ "$RELEASE_MW_LIB" == "enable" ]
					then
						printf "ubi create MWLIB 0x%x\n"  $MWLIB_SIZE >> $1
				fi
				if [ "$LINK_TYPE" == "dynamic" ]
					then
						printf "ubi create LIB 0x%x\n"  $LIB_SIZE >> $1
				fi
		elif [ "$FS_TYPE" == "jffs2" ] || [ "$FS_TYPE" == "cramfs" ] ||[ "$FS_TYPE" == "yaffs2" ]
			then
				let BLK_NO=0
				printf "dynpart edb64M-nand:" >>$1
				printf "0x%08X(KL)," $KERNEL_SIZE >> $1
				echo "KL_BLK_NO=$BLK_NO" >> $CONF_INFO
				BLK_NO=$(($BLK_NO+1))
				printf "0x%08X(RFS)," $RFS_SIZE >> $1
				echo "RFS_BLK_NO=$BLK_NO" >> $CONF_INFO
				BLK_NO=$(($BLK_NO+1))
				printf "0x%08X(CON)," $CONF_SIZE >> $1
				echo "CON_BLK_NO=$BLK_NO" >> $CONF_INFO
				BLK_NO=$(($BLK_NO+1))
				printf "0x%08X(APP)," $APP_SIZE >> $1
				echo "APP_BLK_NO=$BLK_NO" >> $CONF_INFO
				if [ "$RELEASE_MW_LIB" == "enable" ]
					then
						BLK_NO=$(($BLK_NO+1))
						printf "0x%08X(MWLIB)," $MWLIB_SIZE >> $1
						echo "MWLIB_BLK_NO=$BLK_NO" >> $CONF_INFO
				fi
				if [ "$LINK_TYPE" == "dynamic" ]
					then
						BLK_NO=$(($BLK_NO+1))
						printf "0x%08X(LIB)," $LIB_SIZE >> $1
						echo "LIB_BLK_NO=$BLK_NO" >> $CONF_INFO
				fi
				echo "-(NA)" >> $1
#				echo "saveenv" >> $1
				echo >>$1
		elif [ "$FS_TYPE" == "sqsh" ]
			then
				printf "dynpart edb64M-nand:0x40000(NPT),0x40000(KL_BP)," >>$1
				printf "0x%08X(KL)," $KERNEL_SIZE >> $1
				printf "0x%08X(RFS)," $RFS_SIZE >> $1
				printf "0x%08X(CON)," $CONF_SIZE >> $1
				printf "0x%08X(APP)," $APP_SIZE >> $1
				if [ "$RELEASE_MW_LIB" == "enable" ]
					then
						printf "0x%08X(MWLIB)," $MWLIB_SIZE >> $1
				fi
				if [ "$LINK_TYPE" == "dynamic" ]
					then
						printf "0x%08X(LIB)," $LIB_SIZE >> $1
				fi
				echo "-(NA)" >> $1
				echo >>$1
				
				let BLK_NO=2
				printf "setenv mtdparts mtdparts=edb64M-nand:256k(NPT),256k(KL_BP)," >>$1
				printf "%dm(KL),"    $(($KERNEL_SIZE/1024/1024)) >> $1
				echo "KL_BLK_NO=$BLK_NO" >> $CONF_INFO
				BLK_NO=$(($BLK_NO+1))
				printf "%dm(RFS),"   $(($RFS_SIZE/1024/1024)) >> $1
				echo "RFS_BLK_NO=$BLK_NO" >> $CONF_INFO
				BLK_NO=$(($BLK_NO+1))
				printf "%dm(CON),"   $(($CONF_SIZE/1024/1024)) >> $1
				echo "CON_BLK_NO=$BLK_NO" >> $CONF_INFO
				BLK_NO=$(($BLK_NO+1))
				printf "%dm(APP),"   $(($APP_SIZE/1024/1024)) >> $1
				echo "APP_BLK_NO=$BLK_NO" >> $CONF_INFO
				if [ "$RELEASE_MW_LIB" == "enable" ]
					then
						BLK_NO=$(($BLK_NO+1))
						printf "%dm(MWLIB)," $(($MWLIB_SIZE/1024/1024)) >> $1
						echo "MWLIB_BLK_NO=$BLK_NO" >> $CONF_INFO
				fi		
				if [ "$LINK_TYPE" == "dynamic" ]
					then
						BLK_NO=$(($BLK_NO+1))
						printf "%dm(LIB)," $(($LIB_SIZE/1024/1024)) >> $1
						echo "LIB_BLK_NO=$BLK_NO" >> $CONF_INFO
				fi
				echo "-(NA)" >> $1
				echo >>$1

		
		fi
fi	

echo "saveenv" >> $1


echo >>$1
echo "% <- this is end of file symbol" >> $1
