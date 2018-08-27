#!/bin/sh
#
# Run this script to install kernel modules
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

echo "=== Copying kernel modules"

[ -z "$MAKEROOTFS_CONFIG" ] && MAKEROOTFS_CONFIG=$PWD/makerootfs.conf
[ ! -e $MAKEROOTFS_CONFIG ] && echo "ERROR: Config file ($MAKEROOTFS_CONFIG) not found!" && exit 1
. $MAKEROOTFS_CONFIG

! which $TARGET_STRIP && echo "ERROR: Can't find $TARGET_STRIP in PATH!" && exit 1

export INSTALL_MOD_PATH=$MYROOTFS

make -C $KERNEL_PATH modules_install

cd $MYROOTFS/lib/modules/$KVERS
rm build source

echo "* Stripping debug information from kernel modules"
find . | grep .ko | xargs -n1 $TARGET_STRIP --strip-debug

echo "* Done."
