#!/usr/bin/python
"""
./app_append.py BINDIR, APP_ELF_PATH, APP_BIN_PATH, APP_ZIP_PATH, POSTFIX
"""

import sys
import os
import re
from os.path import join
from subprocess import Popen, PIPE
import hashlib
from struct import pack, unpack

PREFIX = '  [APP_APPEND]'
HEADER_SIZE = 0x100

SBOOT_ELF = 'sboot.elf'
FLASH = 'flash.bin'
FLASH_OUT = 'flash_with_%s.bin'
LOADER_OUT = 'bin/bfn_loader.bin'
MIU_OUT = 'bin/bfn_miu.bin'
APP_OUT = 'bin/bfn_app.bin'

def get_nmap(NM, path):
    out = Popen([NM, path], stdout=PIPE).communicate()[0]
    return dict([(m[2], (m[0], m[1]))
        for m in re.findall('([0-9a-zA-Z]{8})\s*(.)\s*(.+)', out)])


def get_app_info(READELF, path):
    out = Popen([READELF, '-h', path], stdout=PIPE).communicate()[0]
    entry = int(re.search(r'Entry point address:\s*(0x\w+)', out).group(1), 0)

    out = Popen([READELF, '-S', path], stdout=PIPE).communicate()[0]
    for m in re.finditer(r'(\[\s*\d+\])\s+(\S+)\s+(\S+)\s+(\S+)', out):
        addr = int(m.group(4), 16)
        if addr != 0:
            break
    return entry, addr


def main(argv):
    bindir, app_elf_path, app_bin_path, app_zip_path, postfix = argv[1:]

    READELF = os.environ['READELF']
    NM = os.environ['NM']

    sboot_nmap = get_nmap(NM, join(bindir, SBOOT_ELF))
    _ld_load_start = int('0x' + sboot_nmap['_ld_load_start'][0], 16)
    _ld_APP_load_start = int('0x' + sboot_nmap['_ld_APP_load_start'][0], 16)

    _ld_LDR_load_start = int('0x' + sboot_nmap['_ld_LDR_load_start'][0], 16)
    _ld_LDR_load_end = int('0x' + sboot_nmap['_ld_LDR_load_end'][0], 16)

    _ld_LDR_run_base = int('0x' + sboot_nmap['_ld_LDR_run_base'][0], 16)
    AppDigest = int('0x' + sboot_nmap['AppDigest'][0], 16)

    _ld_HASH1_run_start = int('0x' + sboot_nmap['_ld_HASH1_run_start'][0], 16)
    _ld_HASH1_run_end = int('0x' + sboot_nmap['_ld_HASH1_run_end'][0], 16)

    _ld_HASH0_run_start = int('0x' + sboot_nmap['_ld_HASH0_run_start'][0], 16)
    FLASH_BASE_ADDR = _ld_load_start#_ld_HASH0_run_start - 0x2000

    app_entry, app_ram_start = get_app_info(READELF, app_elf_path)

    print PREFIX, 'Address:'
    print '    _ld_APP_load_start:  0x%08x' % _ld_APP_load_start
    print '    _ld_HASH1_run_start: 0x%08x' % _ld_HASH1_run_start
    print '    _ld_HASH1_run_end:   0x%08x' % _ld_HASH1_run_end
    print '    app_ram_start:       0x%08x' % app_ram_start
    print '    app_entry:           0x%08x' % app_entry
    print '    _ld_LDR_load_start:  0x%08x' % _ld_LDR_load_start
    print '    _ld_LDR_load_end:    0x%08x' % _ld_LDR_load_end
    print '    HEADER_SIZE:         0x%08x' % HEADER_SIZE
    print '    FLASH_BASE_ADDR:     0x%08x' % FLASH_BASE_ADDR

    loader_bin = open(join(bindir, FLASH), 'rb').read()[_ld_LDR_load_start - _ld_load_start:_ld_LDR_load_end - _ld_load_start]
    if os.path.exists(LOADER_OUT):
        os.remove(LOADER_OUT)
    open(LOADER_OUT, 'wb').write(loader_bin)

    miu_bin = open(join(bindir, FLASH), 'rb').read()[_ld_HASH1_run_start - _ld_load_start:_ld_HASH1_run_end - _ld_load_start]
    if os.path.exists(MIU_OUT):
        os.remove(MIU_OUT)
    open(MIU_OUT, 'wb').write(miu_bin)

    dst = app_ram_start - HEADER_SIZE
    size = os.stat(app_bin_path).st_size + HEADER_SIZE
    size = (size / 32 + 1) * 32
    zip_size = os.stat(app_zip_path).st_size + HEADER_SIZE
    entry = app_entry - dst
    app_zip = open(app_zip_path, 'rb').read()
    print PREFIX, 'dst=0x%08x entry=0x%08x' % (dst, entry)
    print PREFIX, 'size=0x%08x(%d) zip_size=0x%08x(%d)' % (
            size, size, zip_size, zip_size)

    header = pack('<IIII', dst, entry, zip_size, size)
    header += '\0' * (HEADER_SIZE - len(header))

    print PREFIX, 'header:', len(header)
    flash = open(join(bindir, FLASH), 'rb').read()
    # Replace the APP digest
    h = hashlib.sha256(header + app_zip)
    l = len(h.digest())
    print PREFIX, 'APP Digest:', h.hexdigest()
    AppDigest = _ld_LDR_load_start + (AppDigest - _ld_LDR_run_base) - FLASH_BASE_ADDR
    flash = flash[:AppDigest] + h.digest()[::-1] + flash[AppDigest+l:]
    flash = flash[:_ld_APP_load_start - FLASH_BASE_ADDR] + header + app_zip

    out_path = join(bindir, FLASH_OUT % postfix)
    print PREFIX, 'Generate', out_path
    open(out_path, 'wb').write(flash)

    #zip_size -= 0x40000
    #size -= 0x40000

    header = pack('<IIII', dst, entry, zip_size, size)
    header += '\0' * (HEADER_SIZE - len(header))

    flash_app = (header + app_zip)
    if os.path.exists(APP_OUT):
        os.remove(APP_OUT)
    open(APP_OUT, 'wb').write(flash_app)

if __name__ == '__main__':
    main(sys.argv)
