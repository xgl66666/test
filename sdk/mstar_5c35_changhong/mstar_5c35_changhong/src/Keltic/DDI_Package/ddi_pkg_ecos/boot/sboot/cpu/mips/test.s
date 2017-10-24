################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################


        .include "boot.inc"
        .include "chip.inc"
        .text
        .set    noreorder

        #.fill 0xA0, 1, 0xb4

        PUTC '\r'
        PUTC '\n'
        PUTC 'A'
        PUTC 'P'
        PUTC 'P'
        PUTC '\r'
        PUTC '\n'

1001:
        b       1001b
