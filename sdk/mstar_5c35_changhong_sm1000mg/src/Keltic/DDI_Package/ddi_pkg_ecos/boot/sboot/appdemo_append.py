#!/usr/bin/python
import sys
import os
import re
import subprocess
import hashlib
from struct import pack, unpack

FLASH_ADDRESS = 0x94000000
HEADER_SIZE = 0x100
SBOOT_ELF = 'bin/sboot.elf'
FLASH = 'bin/flash.bin'

APP_RAM_START = 0x80000180
APP_ENTRY = 0x80000224
APP_STACK = 0x80A80000
APP_BIN = '../../cus_mstar/project/mstar_demo/bin/kappa_ecos.bin'
FLASH_OUT = 'bin/flash_with_ecos.bin'

PREFIX = '  [APP_APPEND]'

def main():
    NM = os.environ['NM']
    if not NM:
        sys.exit(1)

    mboot_path = sys.argv[1]
    try:
        compression = sys.argv[2]
    except:
        compression = None


    _ld_LDR_run_base = 0x00004000 - 0x8

    flash = open(mboot_path, 'rb').read(_ld_LDR_run_base)

    print PREFIX, 'Address:'
    print '    APP_RAM_START:	0x%08x' % APP_RAM_START
    print '    APP_ENTRY:   	0x%08x' % APP_ENTRY

    print '    HEADER_SIZE:        0x%08x' % HEADER_SIZE

    # # Try old MBoot_u4_DDI first
    # app_bin_fn = os.path.join(mboot_path, 'sboot/out', APP_BIN)
    # if not os.path.exists(app_bin_fn):
    #     # Try MBoot_STB
    #     app_bin_fn = os.path.join(mboot_path, 'sboot/bin', APP_BIN)
    app_bin_fn = os.path.abspath(APP_BIN)
    print app_bin_fn

    if compression == 'LZMA':
        args = ["make", "IMAGE_BIN=%s" % app_bin_fn, "IMAGE_ZIP=out/app.zip",
                "ZIP_APP_BIN=0x%08x" % APP_RAM_START,
                "ZIP_APP_BIN_ENTRY=0x%08x" % APP_ENTRY]
        make = subprocess.Popen(args, cwd='../unzip')
        make.wait()
        if make.returncode != 0:
            sys.exit(make.returncode)

        dst = 0x81000100 - HEADER_SIZE
        size = os.stat('../unzip/out/unzip.bin').st_size + HEADER_SIZE
        entry = 0x81000100 - dst
        app_bin = open('../unzip/out/unzip.bin', 'rb').read()

    else:
        dst = APP_RAM_START - HEADER_SIZE
        size = os.stat(app_bin_fn).st_size + HEADER_SIZE
        entry = APP_ENTRY - dst
        app_bin = open(app_bin_fn, 'rb').read()

    # Alignment
    size = (size / 32 + 1) * 32

    print PREFIX, 'dst=0x%08x size=0x%08x(%d) entry=0x%08x statck=0x%08x' % (dst, size, size, entry, APP_STACK)

    header = pack('<IIIIII', dst, size, dst, entry, APP_STACK, size)
    header += '\0' * (HEADER_SIZE - len(header) + 8)

    app_bin = header + app_bin
    flash = flash + app_bin
    if size != len(app_bin):
        app_bin += '\x00' * (size - len(app_bin))

    print PREFIX, 'header:  0x%08x' % len(header)
    print PREFIX, 'app_bin: 0x%08x' % len(app_bin)

    open(FLASH_OUT, 'wb').write(flash)

if __name__ == '__main__':
    main()
