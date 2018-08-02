#sh auto_gen/gen_set_config.sh $SET_CONFIG_FILE <<-- example of each parameter
#   $0                         $1

source ${AUTO_GEN_OUT}/var_list

### check CHIP's Bus Address ###
[ "$LX_MEM2_BUS_ADDR" != "INVALID" ] || err_exit "Invalid configuration for Kernel Pool2!!!";
[ "$LX_MEM3_BUS_ADDR" != "INVALID" ] || err_exit "Invalid configuration for Kernel Pool3!!!";

RW_OPT=rw

if [ "$DDI_SECURE_UBOOT" == "enable" ]; then
    RW_OPT=ro
fi

echo "# <- this is for comment / total file size must be less than 4KB" > $1
echo >> $1
if [ "$CHIP" == "kaiser" ]
	then
		echo "# LX_MEM and LX_MEM2 must 4096 byte alignment." >> $1
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=${DRAM_LEN0} DRAM_SIZE2=${DRAM_LEN0}\n" >> $1
		echo >> $1
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN} BB_ADDR=${BB_ADDR}" $((0x20000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
elif [ "$CHIP" == "keres" ]
	then
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN}  BB_ADDR=0" $((0x00000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} >> $1
elif [ "$CHIP" == "kirin" ]
	then
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN}  BB_ADDR=0" $((0x00000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} >> $1
elif [ "$CHIP" == "kris" ]
    then
        printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN}  BB_ADDR=0" $((0x00000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} >> $1
elif [ "$CHIP" == "kratos" ]
    then
        printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} BB_ADDR=0" $((0x00000000+${EMAC_MEM})) >> $1
elif [ "$CHIP" == "clippers" ]
	then
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=0x20000000 DRAM_SIZE2=0x20000000\n" >> $1
		echo >> $1
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN}" ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
elif [ "$CHIP" == "muji" ]
	then
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=0x20000000 DRAM_SIZE2=0x20000000\n" >> $1
		echo >> $1
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN}" ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
elif [ "$CHIP" == "kano" ]
	then
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=0x40000000 DRAM_SIZE2=0x20000000\n" >> $1
		echo >> $1
		if [ "$MI_WLAN" == "enable" ]
			then
				printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN} coherent_pool=1536K" $((0x20000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
		else
				printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN}" $((0x20000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
		fi
elif [ "$CHIP" == "kiwi" ]
    then
        printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} BB_ADDR=0" $((0x00000000+${EMAC_MEM})) >> $1
elif [ "$CHIP" == "kastor" ]
	then
		printf "setenv KERNEL_PROTECT DRAM_SIZE1=0x40000000 DRAM_SIZE2=0x20000000\n" >> $1
		echo >> $1
		if [ "$MI_WLAN" == "enable" ]
			then
				printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN} coherent_pool=1536K" $((0x20000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
		else
				printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} GMAC_MEM=0x%X,${GMAC_MEM_LEN} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%X,${LX_MEM2_LEN} LX_MEM3=0x%X,${LX_MEM3_LEN}" $((0x20000000+${GMAC_MEM})) ${LX_MEM2_BUS_ADDR} ${LX_MEM3_BUS_ADDR} >> $1
		fi
else
		printf "setenv MS_MEM LX_MEM=${LX_MEM_LEN} EMAC_MEM=${EMAC_MEM} DRAM_LEN=${DRAM_LEN0} LX_MEM2=0x%x,${LX_MEM2_LEN} BB_ADDR=${BB_ADDR}" ${LX_MEM2_BUS_ADDR} >> $1

fi
echo >> $1

printf "\n# additional LX_MEM vars for uboot\n" >> $1
printf "setenv E_LX_MEM_ADR ${LX_MEM_ADR}\n" >> $1
printf "setenv E_LX_MEM_LEN ${LX_MEM_LEN}\n" >> $1
printf "setenv E_LX_MEM2_ADR ${LX_MEM2_BUS_ADDR}\n" >> $1
printf "setenv E_LX_MEM2_LEN ${LX_MEM2_LEN}\n" >> $1
printf "setenv E_LX_MEM3_ADR ${LX_MEM3_BUS_ADDR}\n" >> $1
printf "setenv E_LX_MEM3_LEN ${LX_MEM3_LEN}\n\n" >> $1

if [ "$FLASH_TYPE" == "SPI" ]
	then
		if [ "$FS_TYPE" == "sqsh" ]
			then
				echo "setenv MS_RFS root=/dev/mtdblock2 rootfstype=squashfs r" >> $1
				echo  >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) \$(mtdparts) ${CMA_ARG}\n" >> $1
				echo >> $1
				if [ "$CHIP" == "kratos" ]; then
				    printf "setenv bootcmd ' spi rdc.p ${KERNEL_START} KL 0 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
				else
				    printf "setenv bootcmd ' spi_rdc_p ${KERNEL_START} KL 0 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
				fi
		elif [ "$FS_TYPE" == "ramdisk" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS root=/dev/ram0 rw" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) rd_start=${RAMDISK_START} rd_size=${RAMDISK_SIZE} ${CMA_ARG}\n" >> $1
				echo >> $1
				if [ "$CHIP" == "kratos" ]; then
				    printf "setenv bootcmd ' spi rdc.p ${KERNEL_START} KL 0 0x%x; bootm ${KERNEL_START};\n" $RAM_DISK_BIN_SIZE >> $1
				else
				    printf "setenv bootcmd ' spi_rdc_p ${KERNEL_START} KL 0 0x%x; bootm ${KERNEL_START};\n" $RAM_DISK_BIN_SIZE >> $1
				fi
		fi
elif [ "$FLASH_TYPE" == "EMMC" ]
	then
		if [ "$FS_TYPE" == "ext4" ]
			then
          if [ "$CHIP" == "kaiser" ]
              then
                  echo "setenv MS_RFS root=/dev/mmcblk0p8 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
          elif [ "$CHIP" == "muji" ]
              then
                  echo "arm64 depend on DTB.dtb , please rebuild if changed!"
                  echo "setenv MS_RFS root=/dev/mmcblk0p3 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
          elif [ "$CHIP" == "clippers" ]
              then
                if [ "$BOOT_MODE" == "EMMC" ]
                    then
                        echo "setenv MS_RFS root=/dev/mmcblk0p4 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
                else
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
                fi
          elif [ "$CHIP" == "kano" ]
                then
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
		  elif [ "$CHIP" == "kastor" ]
                then
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
		  else
                    echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
            fi

            printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz ${CMA_ARG}\n" >> $1
    fi
		if [ "$FS_TYPE" == "ramdisk" ]
					then
					  echo "hello emmc ramdisk setconfig! "
						echo "setenv MS_GOP0_MIU 0" >> $1
						echo >> $1
						echo "setenv MS_RFS root=/dev/ram0 ${RW_OPT}" >> $1
						echo >> $1
						printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) rd_start=${RAMDISK_START} rd_size=${RAMDISK_SIZE} ${CMA_ARG}\n" >> $1
						echo >> $1
    fi
else
		if [ "$FS_TYPE" == "ubifs" ]
			then
				if [ "$CHIP" == "kaiser" ] || [ "$CHIP" == "keres" ] || [ "$CHIP" == "kirin" ] || [ "$CHIP" == "kris" ]
					then
						echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=ubi:RFS rootfstype=${FS_TYPE} ${RW_OPT}" >> $1
						printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} \${mtdparts} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz ${CMA_ARG}\n" >> $1
                elif [ "$CHIP" == "kratos" ] || [ "$CHIP" == "kiwi" ]
                    then
                        echo "setenv MS_RFS ubi.mtd=UBI root=ubi:RFS rootfstype=${FS_TYPE} ${RW_OPT}" >> $1
                        printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} \${mtdparts} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz ${CMA_ARG}\n" >> $1
                else
						echo "setenv MS_GOP0_MIU 0" >> $1
						echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=ubi:RFS rootfstype=${FS_TYPE} ${RW_OPT}" >> $1
						printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) ${CMA_ARG}\n" >> $1
				fi

				printf "setenv bootcmd ' ubi part UBI;ubi read ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};'\n" $KERNEL_SIZE >> $1
		elif [ "$FS_TYPE" == "ext4" ]
			then
                if [ "$CHIP" == "kaiser" ]
                    then
                        echo "setenv MS_RFS root=/dev/mmcblk0p8 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
                elif [ "$CHIP" == "muji" ]
                    then
                        echo "arm64 depend on DTB.dtb , please rebuild if changed!"
                        echo "setenv MS_RFS root=/dev/mmcblk0p3 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
                elif [ "$CHIP" == "clippers" ]
                    then
	                    if [ "$BOOT_MODE" == "EMMC" ]
	                        then
	                            echo "setenv MS_RFS root=/dev/mmcblk0p4 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
	                    else
	                            echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
	                    fi
	            elif [ "$CHIP" == "kano" ]
                    then
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
                elif [ "$CHIP" == "kastor" ]
                    then
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
				else
                        echo "setenv MS_RFS root=/dev/mmcblk0p2 rootfstype=ext4 ${RW_OPT} rootwait" >> $1
                fi

                printf "setenv bootargs console=ttyS0,115200 \${MS_RFS} init=/init \${MS_MEM} CORE_DUMP_PATH=/application/core_dump.%%%%p.gz ${CMA_ARG}\n" >> $1
		elif [ "$FS_TYPE" == "ramdisk" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				if [ "$CHIP" == "kratos" ]
				    then
				        echo "setenv MS_RFS ubi.mtd=UBI root=/dev/ram0 ${RW_OPT}" >> $1
				else
				        echo "setenv MS_RFS ubi.mtd=0,$NAND_PAGE_SIZE root=/dev/ram0 ${RW_OPT}" >> $1
				fi
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) rd_start=${RAMDISK_START} rd_size=${RAMDISK_SIZE} ${CMA_ARG}\n" >> $1
				echo >> $1
				if [ "$CHIP" != "kaiser" ]
                                then
				printf "setenv bootcmd ' ubi part UBI;ubi read ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $RAM_DISK_BIN_SIZE >> $1
				fi
		elif [ "$FS_TYPE" == "jffs2" ] || [ "$FS_TYPE" == "cramfs" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS root=/dev/mtdblock1 rootfstype=${FS_TYPE} ${RW_OPT}" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) ${CMA_ARG}\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' nand read.e ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		elif [ "$FS_TYPE" == "sqsh" ]
			then
				echo "setenv MS_RFS root=/dev/mtdblock3 rootfstype=squashfs r" >> $1
				echo  >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) \$(mtdparts) ${CMA_ARG}\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' nand read.e ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		elif [ "$FS_TYPE" == "yaffs2" ]
			then
				echo "setenv MS_GOP0_MIU 0" >> $1
				echo >> $1
				echo "setenv MS_RFS root=/dev/mtdblock1 rootflags=inband-tags ${RW_OPT} rootfstype=${FS_TYPE}" >> $1
				echo >> $1
				printf "setenv bootargs console=ttyS0,115200 \$(MS_RFS) \$(MS_MEM) MS_GOP0_MIU=\$(MS_GOP0_MIU) \$(mtdparts) ${CMA_ARG}\n" >> $1
				echo >> $1
				printf "setenv bootcmd ' nand read.e ${KERNEL_START} KL 0x%x; bootm ${KERNEL_START};\n" $KERNEL_SIZE >> $1
		fi
fi

echo >> $1
echo "# this is for BOOTLOGO MMAP " >> $1
echo "setenv bl_dfb_framebuffer_addr ${VDEC_FRAME_BUF_ADR}" >> $1
echo "setenv bl_jpd_write_addr ${JPD_OUT_ADR}" >> $1
echo "setenv bl_jpd_write_size ${JPD_OUT_LEN}" >> $1
echo "setenv bl_jpd_read_addr ${JPD_READ_BUF_ADR}" >> $1
echo "setenv bl_jpd_read_size ${JPD_READ_BUF_LEN}" >> $1
echo "setenv bl_jpd_inter_addr ${JPD_INTER_BUF_ADR}" >> $1
echo "setenv bl_jpd_inter_size ${JPD_INTER_BUF_LEN}" >> $1
echo "setenv bl_ve_buffer_addr ${VE_FRAME_BUF_ADR}" >> $1

if [ "$MI_BSP" == "enable" ];then
echo >> $1
echo "# this is for MI DFB MUX " >> $1
echo "setenv GOP_SET_MUX ${GOP0_MUXINDEX}:${GOP1_MUXINDEX}:${GOP2_MUXINDEX}:${GOP3_MUXINDEX}" >> $1
fi

echo >> $1
echo "setenv bootdelay 1" >> $1
echo >> $1
echo "setenv verify y" >> $1
echo >> $1
echo "saveenv" >> $1
echo >> $1
echo "% <- this is end of file symbol" >> $1

