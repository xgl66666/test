#!/bin/bash

#replace 64bytes data as partner header in App
APP_CHECKSUM=out/checksum.temp
APP_IMAGE=$1
APP_FINAL="$1".ns.bin
APP_FINAL_SIGN="$1".signed.bin
APP_HEAD=out/app_header.temp
SIZE_HEAD=out/sign_header.temp
TOOL_ENTRY=./tools
touch $APP_HEAD

$TOOL_ENTRY/crc $APP_IMAGE $APP_CHECKSUM

#cat $APP_IMAGE > $APP_FINAL

SIZE_App=`stat -c '%s' $APP_IMAGE`

#perl ./addheader_app.pl $APP_FINAL $APP_FINAL $SIZE_App $APP_CHECKSUM
perl ./addheader_app.pl $APP_HEAD $APP_HEAD $SIZE_App $APP_CHECKSUM
perl ./addheader_size.pl $APP_HEAD $SIZE_HEAD $SIZE_App 

cat $APP_HEAD > $APP_FINAL
cat $APP_IMAGE >> $APP_FINAL

#Mstar sign binfile
cat $SIZE_HEAD > $APP_FINAL_SIGN
cat $APP_IMAGE >> $APP_FINAL_SIGN

rm -rf $APP_CHECKSUM
rm -rf $APP_HEAD
rm -rf $SIZE_HEAD
