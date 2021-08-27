/* testmini.c -- very simple test program for the miniLZO library

   This file is part of the LZO real-time data compression library.

   Copyright (C) 2008 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2007 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2006 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2005 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2004 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2003 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2002 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2001 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2000 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1999 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/opensource/lzo/
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "minilzo.h"


#define PRINT_INFO(fmt,args...)       // printf(fmt,##args)
#define PRINT_ERROR_MSG(fmt,args...)  printf(fmt,##args)


/* Work-memory needed for compression. Allocate memory in units
 * of `lzo_align_t' (instead of `char') to make sure it is properly aligned.
 */

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);


/*************************************************************************
//
**************************************************************************/

int main(int argc, char *argv[])
{
    char *in_buf = NULL;
    char *out_buf = NULL;
    int in_fd;
    int in_len;
    int out_fd;
    long out_len;

    /* check arguments */
    if (argc != 3)
    {
        PRINT_ERROR_MSG("Usage: %s in_file out_file\n", argv[0]);
        goto error;
    }

    /* open file */
    if ((in_fd = open(argv[1], O_RDONLY)) == -1)
    {
        PRINT_ERROR_MSG("Error: can't open %s\n", argv[1]);
        goto error;
    }

    if ((out_fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0777 )) == -1)
    {
        close(in_fd);
        PRINT_ERROR_MSG("Error: can't open/create %s\n", argv[2]);
        goto error;
    }

    /* calc file size */
    if ((in_len = lseek(in_fd, 0, SEEK_END)) == -1)
    {
        PRINT_ERROR_MSG("Error: calc file size fail\n");
        goto error_free_resource;
    }
    else
    {
        PRINT_INFO("file size: %d\n", in_len);
    }

    if (lseek(in_fd, 0, SEEK_SET) == -1)
    {
        PRINT_ERROR_MSG("Error: calc file size fail\n");
        goto error_free_resource;
    }

    /* allocate memory */
    if (   (in_buf  = malloc(in_len)) == NULL
        || (out_buf = malloc(in_len)) == NULL
        )
    {
        PRINT_ERROR_MSG("Error: memory allocation fail\n");
        goto error_free_resource;
    }

    /* read file */
    out_len = read(in_fd, in_buf, in_len);

    if (in_len != out_len)
    {
        PRINT_ERROR_MSG("Error: file read error\n");
        goto error_free_resource;
    }

    /* init lzo */
    if (lzo_init() != LZO_E_OK)
    {
        PRINT_ERROR_MSG("Error: lzo_init()\n");
        goto error_free_resource;
    }

    /* compress */
    if (lzo1x_1_compress((const lzo_bytep)in_buf, in_len, (lzo_bytep)out_buf, (lzo_uintp)&out_len, wrkmem) == LZO_E_OK)
    {
        PRINT_INFO("compressed %lu bytes into %lu bytes\n", (unsigned long) in_len, (unsigned long) out_len);
    }
    else
    {
        PRINT_ERROR_MSG("Error: compression failed\n");
        goto error_free_resource;
    }

    /* check for an incompressible block */
    if (out_len >= in_len)
    {
        PRINT_ERROR_MSG("this block contains incompressible data.\n");
    }

    /* write compress data to file */
    in_len = write(out_fd, out_buf, out_len);

    if (in_len != out_len)
    {
        PRINT_ERROR_MSG("Error: write file fail\n");
    }
    else
    {
        PRINT_INFO("\nminiLZO simple compression test passed.\n");
    }

    return 0;

error_free_resource:
    close(in_fd);
    close(out_fd);

    if (in_buf != NULL)
    {
        free(in_buf);
    }

    if (out_buf != NULL)
    {
        free(out_buf);
    }

error:

    return 1;
}
