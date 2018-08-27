#!/bin/sh
#
# Run this script to copy the toolchain shared libraries
# into final root filesystem
#
# Copyright (C) 2005 Matthias Ihmig <m.ihmig@mytum.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Library General Public License for more details.

set -e

echo "=== Copy toolchain shared libraries (Ignore File Format errors.)"

[ -z "$MAKEROOTFS_CONFIG" ] && MAKEROOTFS_CONFIG=$PWD/makerootfs.conf
[ ! -e $MAKEROOTFS_CONFIG ] && echo "ERROR: Config file ($MAKEROOTFS_CONFIG) not found!" && exit 1
. $MAKEROOTFS_CONFIG

! which $TARGET_STRIP && echo "ERROR: Can't find $TARGET_STRIP in PATH!" && exit 1

TMP1=`which arm-linux-gcc`
TMP2=`dirname $TMP1`
LIBS=`dirname $TMP2`/$CROSS_HOST/lib
SBIN=`dirname $TMP2`/$CROSS_HOST/sbin

mkdir -p $MYROOTFS/lib

cd $LIBS
cp -dpP *.so* $MYROOTFS/lib && echo -n

cd $MYROOTFS/lib
$TARGET_STRIP --discard-locals --discard-all --strip-all --strip-debug *.so* && echo -n

mkdir -p $MYROOTFS/sbin
install -m 0755 $SBIN/ldconfig $MYROOTFS/sbin
$TARGET_STRIP --discard-locals --discard-all --strip-all --strip-debug $MYROOTFS/sbin/ldconfig

echo "*** Done."