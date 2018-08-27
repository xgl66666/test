#!/bin/bash
#
# Run this script to creates the final root filesystem from ROOTFS_STAGING
# The device nodes are created at that stage from device_table.txt
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

[ -z "$MAKEROOTFS_CONFIG" ] && MAKEROOTFS_CONFIG=$PWD/makerootfs.conf
[ ! -e $MAKEROOTFS_CONFIG ] && echo "ERROR: Config file ($MAKEROOTFS_CONFIG) not found!" && exit 1
. $MAKEROOTFS_CONFIG

JFFS2FILE=`basename $MYROOTFS`.jffs2

# Uncomment, if you want the rootfs padded to reach the partition size
#PADDING="--pad=0x01d80000"

# Flash erase size
ERASESIZE=0x40000

echo "=== Creating $PWD/$JFFS2FILE with erasesize $ERASESIZE"

$BUILDDIR/mkfs.jffs2 $PADDING --squash-uids --eraseblock=$ERASESIZE --little-endian --verbose \
	--root=$MYROOTFS --devtable=$PATCHES/device_table.txt --output=$JFFS2FILE --test-compression

echo "Done.\nRoot filesystem stored in: $PWD/$JFFS2FILE"
