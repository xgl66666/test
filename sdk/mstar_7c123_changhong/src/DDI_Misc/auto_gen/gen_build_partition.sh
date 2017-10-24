#sh auto_gen/gen_build_partition.sh build_app_ubifs.sh auto_gen_output/application $APP_SIZE $application <<-- example of each parameter
#   $0                              $1                 $2                          $3        $4

source ${AUTO_GEN_OUT}/var_list

#remove original build script
F_NAME=$1
F_NAME=${F_NAME:0:10}
rm -f ${F_NAME}*

echo "  Generate $1 ..."
echo "#!/bin/bash" > $1
printf "\n" >> $1

if [ "$FLASH_TYPE" == "SPI" ]
	then
		if [ "$FS_TYPE" == "sqsh" ] || [ "$FS_TYPE" == "ramdisk" ]
			then
				printf "rm -f ../usb_update/Target/image/$4.sqsh\n" >> $1
				if [ "$CHIP" == "keltic" ]
					then
						printf "./mksquashfs_4 $2 ../usb_update/Target/image/$4.sqsh -comp xz -b 512k" >> $1
				else
						printf "./mksquashfs $2 ../usb_update/Target/image/$4.sqsh -lzmadic 1048576 -b 1048576" >> $1
				fi
		fi
else		
		if [ "$FS_TYPE" == "ubifs" ] || [ "$FS_TYPE" == "ramdisk" ]
			then
				let UBI_PEB_SIZE=$((131072 * ($NAND_PAGE_SIZE / 2048)))
		
				printf "#NAND Page size\n" >> $1
				printf "NAND_PAGE_SIZE=$NAND_PAGE_SIZE\n" >> $1
				printf "\n" >> $1
				printf "#NAND Block size\n" >> $1
				printf "UBI_PEB_SIZE=$UBI_PEB_SIZE\n" >> $1
				printf "\n" >> $1
				printf "#Partition size\n" >> $1
				printf "PART_SIZE=0x%x\n" $3 >> $1
				printf "\n" >> $1
				printf "###################################################################################\n" >> $1
				printf "UBI_LEB_SIZE=\$((\${UBI_PEB_SIZE} - 2 * $NAND_PAGE_SIZE))\n" >> $1
				printf "\n" >> $1
				printf "LEB_CNT=\$((\${PART_SIZE} / \${UBI_LEB_SIZE}))\n" >> $1
				printf "\n" >> $1
				printf "TARGET=../usb_update/Target/image\n" >> $1
				printf "\n" >> $1
				printf "function build_ubifs () {\n" >> $1
				printf "    let ROOT\n" >> $1
				printf "    let MAX_LEB_CNT\n" >> $1
				printf "    ROOT=\$1\n" >> $1
				printf "    MAX_LEB_CNT=\$2\n" >> $1
				printf "    OUTPUT=\${TARGET}/\$(basename $4.ubifs)\n" >> $1
				printf "\n" >> $1
				printf "    echo \" \${ROOT} -> \${OUTPUT}\"\n" >> $1
				printf "    ./mkfs.ubifs -r \${ROOT} -o \${OUTPUT} -m $NAND_PAGE_SIZE \\" >> $1
				printf "\n" >> $1
				printf "    	-e \${UBI_LEB_SIZE} -c \${MAX_LEB_CNT} -v\n" >> $1
				printf "}\n" >> $1
				printf "\n" >> $1
				printf "build_ubifs $2 \${LEB_CNT}\n" >> $1
		elif [ "$FS_TYPE" == "jffs2" ]
			then
				printf "echo \"Make $4.jffs2\"\n" >> $1
				printf "./mkfs.jffs2 --pad=0x%08X --eraseblock=0x20000 -l -n --root=$2/ -o \"../usb_update/Target/image/$4.jffs2\"\n" $3 >> $1
		elif [ "$FS_TYPE" == "sqsh" ]
			then
				printf "rm -f ../usb_update/Target/image/$4.sqsh\n" >> $1
				printf "./mksquashfs $2 ../usb_update/Target/image/$4.sqsh" >> $1
		elif [ "$FS_TYPE" == "cramfs" ]
			then
				printf "./mkcramfs $2 ../usb_update/Target/image/$4.cramfs" >> $1
		elif [ "$FS_TYPE" == "yaffs2" ]
			then
				printf "rm -f ../usb_update/Target/image/$4.yaffs2\n" >> $1
				printf "./mkyaffs2image $2 ../usb_update/Target/image/$4.yaffs2" >> $1
		elif [ "$FS_TYPE" == "ext4" ]
			then
				printf "printf \"\nGenerate $4 partition ...\"\n" >> $1 
				echo "./make_ext4fs -l $3 ../usb_update/Target/image/$4.ext4 $2" >> $1
		fi
fi

chmod 777 $1

