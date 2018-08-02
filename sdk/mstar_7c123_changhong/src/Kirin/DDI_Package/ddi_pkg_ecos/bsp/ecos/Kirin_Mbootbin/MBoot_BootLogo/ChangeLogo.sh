
# help
show_help() {
        echo -e "\nCommand: $0 [mboot] [chunkheader offset] [logo picture] \n"
        echo " ********************************************************** "
        echo "Help:"
        echo "                  help:     Print this help message"
        echo "                 mboot:     change bootlogo mboot.bin"
        echo "    chunkheader offset:     setting chunkheader offset"
        echo "                               hex value need to start with 0x or 0X"
        echo "          logo picture:     Picture wants to change on mboot"
        echo " ********************************************************** "
}

get_chunk_offset() {
    reg_exp='^[0-9]*$'
    reg_exp_hex='^0[xX][0-9a-fA-F]+$'

    if [[ $1 =~ $reg_exp ]]; then
        CHUNK_HEADER_OFFSET=$1
    elif [[ $1 =~ $reg_exp_hex ]]; then
        CHUNK_HEADER_OFFSET=$(printf "%d" $1)
    else
        echo -e "\n invalid format of chunk header offset"
	exit 1
    fi
        CHUNK_HEADER_LOGO_OFFSET=$(($CHUNK_HEADER_OFFSET+32))
        CHUNK_HEADER_LOGO_SIZE=$(($CHUNK_HEADER_OFFSET+36))
}

# Command parsing function
check_error() {
        if [ "$1" = "help" -o "$1" = "-h" -o "$1" = "--help" ]; then
                show_help
                exit 0
        elif [ ! -f "$1" ]; then
                echo -e "\n no mboot.bin can change bootlogo \n"
                exit 1
        elif [ $# -lt 2 ]; then
                echo -e "\n chunk header offset not setting \n"
                exit 1
        elif [ $# -lt 3 -o ! -f "$3" ]; then
                echo -e "\n no picture can change bootlogo \n"
                exit 1
        fi
}

if [ "$#" -le 3 ]; then
    check_error $*
fi

get_chunk_offset $2

#=============================================================================================

MBOOT_BIN=$1

NEW_LOGO_SRC=$3

MBOOTOUT_BIN=Changelogo_$1

CHUNK_HEADER_LOGO_INFO_BIN=LogoInfo.bin

NEW_LOGO_SIZE=$(ls -Gl $NEW_LOGO_SRC | awk '{print $4}')

NEW_LOGO_SIZEHEX=$(printf '%08x' $NEW_LOGO_SIZE) 

#Big-Endian
NEW_LOGO_SIZEBIG=${NEW_LOGO_SIZEHEX:6:2}${NEW_LOGO_SIZEHEX:4:2}${NEW_LOGO_SIZEHEX:2:2}${NEW_LOGO_SIZEHEX:0:2}


#Get the CHUNK_HEADER_LOGO_OFFEST
dd if=$MBOOT_BIN of=$CHUNK_HEADER_LOGO_INFO_BIN bs=1 count=4 skip=$CHUNK_HEADER_LOGO_OFFSET

LOGO_OFFSET=$(printf '%08x'|xxd -ps $CHUNK_HEADER_LOGO_INFO_BIN)

#Big-Endian
LOGO_OFFSETBIG=${LOGO_OFFSET:6:2}${LOGO_OFFSET:4:2}${LOGO_OFFSET:2:2}${LOGO_OFFSET:0:2}

#change to DEC
LOGO_OFFSETDEC=$((16#$LOGO_OFFSETBIG))

#==============START=================
echo " ********************************************************** "
echo -e "\nCHANGE CHUNK HEADER and NEW LOGO PICTURE INFO\n"
echo "CHANGE LOGO PICTURE START : $MBOOTOUT_BIN"
echo "      CHUNK_HEADER_OFFSET = $CHUNK_HEADER_OFFSET"
echo " CHUNK_HEADER_LOGO_OFFSET = $CHUNK_HEADER_LOGO_OFFSET"
echo "   CHUNK_HEADER_LOGO_SIZE = $CHUNK_HEADER_LOGO_SIZE"
printf "            NEW_LOGO_SIZE = 0x%x\n" $NEW_LOGO_SIZE
echo "         NEW_LOGO_SIZEBIG = 0x$NEW_LOGO_SIZEBIG"
echo "              LOGO_OFFSET = 0x$LOGO_OFFSETBIG"
echo " ********************************************************** "
#change the LOGO 
dd if=$MBOOT_BIN of=$MBOOTOUT_BIN bs=1 count=$LOGO_OFFSETDEC;
cat $NEW_LOGO_SRC >> $MBOOTOUT_BIN;

#modify the LOGO size on chunk header 
printf $NEW_LOGO_SIZEBIG | xxd -r -ps | dd of=$MBOOTOUT_BIN bs=1 count=4 seek=$CHUNK_HEADER_LOGO_SIZE conv=notrunc

rm -r $CHUNK_HEADER_LOGO_INFO_BIN
 
echo "CHANGE LOGO PICTURE DONE"
