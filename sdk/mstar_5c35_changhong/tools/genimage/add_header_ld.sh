#!/bin/bash

#replace 64bytes data as partner header in Loader
LOADER_CHECKSUM=out/checksum.temp
LOADER_IMAGE=out/Downloader.bin
LOADER_FINAL=out/unsign_downloader.bin
LOADER_FINAL_SIGN=out/mstarsign_downloader.bin
LOADER_HEAD=out/loader_head.temp
SIZE_HEAD=out/size_head.temp
TOOL_ENTRY=../tools
touch $LOADER_HEAD

$TOOL_ENTRY/crc $LOADER_IMAGE $LOADER_CHECKSUM

#cat $LOADER_IMAGE > $LOADER_FINAL

SIZE_Loader=`stat -c '%s' $LOADER_IMAGE`
printf 'SIZE Loader = 0x%x \n' $SIZE_Loader

perl ./addheader_loader.pl $LOADER_HEAD $LOADER_HEAD $SIZE_Loader $LOADER_CHECKSUM
perl ./addheader_size.pl $LOADER_HEAD $SIZE_HEAD $SIZE_Loader

# Generate unsigned Image
cat $LOADER_HEAD > $LOADER_FINAL
cat $LOADER_IMAGE >> $LOADER_FINAL

# Generate Mstar signed image
cat $SIZE_HEAD > $LOADER_FINAL_SIGN
cat $LOADER_IMAGE >> $LOADER_FINAL_SIGN

rm -rf $LOADER_CHECKSUM
rm -rf $LOADER_HEAD
rm -rf $SIZE_HEAD
