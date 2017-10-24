#! /bin/sh
for f in `ls *.bin`; do
    hexdump -e '16/1 "0x%02X, " "\n"' $f | sed -e 's/0x  ,//g' > $f.c
done
