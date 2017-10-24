/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>

#if defined(CONFIG_I386)
DECLARE_GLOBAL_DATA_PTR;
#endif

// for ReadWrite Testing
int i_adding(void)
{
    int i=0;

    i++;
    i++;
    i++;
    i++;
    i++;
    return i;
}

// for ReadWrite Testing
int miu0_write_test( void ){

    int tsize = 64*0x100000 ;
    int i, seed, r, err ;

    volatile unsigned char* vp ;

    seed = 0x183687 ;
    err = 0 ;

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    r = 0xFF ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }

    r = 0x00 ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }

    return(!err) ;
}

// for ReadWrite Testing
int miu0_write_test_cache_first( void ){

    int tsize = 64*0x100000 ;
    int i, seed, r, err ;

    volatile unsigned char* vp ;

    seed = 0x183687 ;
    err = 0 ;

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    vp = (volatile unsigned char*)0x84000000 ;
    printf( "test %d MB from 0x%08X\n", tsize/0x100000, (unsigned int)vp ) ;

    r = seed ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
        r = 110017 + 2360483*r ;
    }
    r = seed ;
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
        r = 110017 + 2360483*r ;
    }

    r = 0xFF ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }

    r = 0x00 ;
    for( i=0; i<tsize; i++ ){
        vp[i] = r ;
    }
    for( i=0; i<tsize; i++ ){
        if( vp[i]!=(r&0xFF) ){
            printf( "#1 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#2 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#3 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            printf( "#4 error idx=%d, vp[i]=0x%02X, r=0x%02X\n", i, vp[i], (r&0xFF) ) ;
            err = 1 ;
        }
    }

    return(!err) ;
}

// for ReadWrite Testing
void do_rw_test( char arg, int loop ){
    int i, j ;

    for( j=0; j<loop; j++ ){
        switch(arg){
        case '0':
            i = i_adding() ;
            printf("i_adding = %d\n", i) ;
            break ;
        case '1':
            i = miu0_write_test() ;
            printf("MIU0 write and read testing = %s\n", i?"Pass":"Fail") ;
            break ;
        case '2':
            i = miu0_write_test_cache_first() ;
            printf("MIU0 write and read testing cache first = %s\n", i?"Pass":"Fail") ;
            break ;
        default:
            printf( "not defined test case\n" ) ;
            return ;
            break ;
        }
    }
}



static ulong auto_search_kernel_entry( unsigned int s ){
    int i ;
    int len = 0x100000 ;
    unsigned int* p = (unsigned int*)s ;

#if !defined(CONFIG_JANUS) && !defined(CONFIG_MARIA10)
    for( i=0; i<len; i++ )
    {
        if( p[i]==0x3c08bf20 && p[i+1]==0x2409029f )
            return( (ulong)&(p[i]) ) ;
    }
#else
    for( i=0; i<len; i++ )
    {
        if( p[i]==0x3c08bf80 && p[i+1]==0x2409029f )
            return( (ulong)&(p[i]) ) ;
    }
#endif

    return( 0 ) ;
}

extern void auto_boot(void);
int do_autoboot (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    auto_boot();
    return 0;
}

int do_go (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

#if !defined(CONFIG_JANUS) && !defined(CONFIG_MARIA10)
    {
        #include <sys/common/MsDevice.h>
        #include <sys/common/MsOS.h>
        #include <sys/drv/drvMIU.h>
        // Added by coverity_0210
#if defined(CONFIG_KRONUS)||defined(CONFIG_KAISERIN)
        U8 zero_array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
#else
        U8 zero_array[4] = {0, 0, 0, 0};
#endif
        U32 u32TmpStart, u32TmpEnd;
#if defined(CONFIG_KAISERIN)
        U32 u32Size,pageshift;
#endif
        extern U8  _readonly_start[];
        extern U8  _readonly_end[];
        printf("Start %x End %x \n",MsOS_VA2PA((U32)_readonly_start),MsOS_VA2PA((U32)_readonly_end) );

        // Modified by coverity_0210
        u32TmpStart = MsOS_VA2PA((U32)_readonly_start);
        u32TmpEnd = MsOS_VA2PA((U32)_readonly_end);
#if (!CONFIG_MSTAR_SILENT)
        printf("UnProtect MIU block 0 : 0x%x 0x%x\n", u32TmpStart, u32TmpEnd);

#endif

#if defined(CONFIG_KAISERIN)
        u32Size = u32TmpEnd - u32TmpStart;
        MDrv_MIU_ProtectAlign((MS_U32 *)&pageshift);
        MDrv_MIU_Protect(0, zero_array, ALIGN(u32TmpStart, pageshift), ALIGN(u32TmpStart + u32Size -1, pageshift), DISABLE);
#else
        MDrv_MIU_Protect(0, zero_array, u32TmpStart, u32TmpEnd, DISABLE);
#endif
    }
#endif

    disable_interrupts();
    printf("disable interrupts\n");

    // for read/write testing,Samuel
    if( argv[1][0]=='.' ){
        if( argc==3 )
            do_rw_test( argv[1][1], simple_strtoul(argv[2], NULL, 10) ) ;
        else
            do_rw_test( argv[1][1], 1 ) ;
        return 1 ;
    }

	if (argc < 2)
    {
		printf ("Usage:\n%s\n", cmdtp->usage);
		addr = auto_search_kernel_entry(0x80880000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80780000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80980000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80680000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80A80000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80B80000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80C80000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80580000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80480000) ;
		if( 0==addr )
		    addr = auto_search_kernel_entry(0x80D80000) ;
		if( 0==addr )
		    return 1 ;
		printf( "Auto Search Kernel_Entry: 0x%08X\n", addr ) ;
	}else
	    addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lX ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
#if defined(CONFIG_I386)
	/*
	 * x86 does not use a dedicated register to pass the pointer
	 * to the global_data
	 */
	argv[0] = (char *)gd;
#endif
#if !defined(CONFIG_NIOS)
	rc = ((ulong (*)(int, char *[]))addr) (--argc, &argv[1]);
#else
	/*
	 * Nios function pointers are address >> 1
	 */
	rc = ((ulong (*)(int, char *[]))(addr>>1)) (--argc, &argv[1]);
#endif
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
	return rcode;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	go, CFG_MAXARGS, 1,	do_go,
	"go      - start application at address 'addr'\n",
	"addr [arg ...]\n    - start application at address 'addr'\n"
	"      passing 'arg' as arguments\n"
);

extern int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

U_BOOT_CMD(
	reset, CFG_MAXARGS, 1,	do_reset,
	"reset   - Perform RESET of the CPU\n",
	NULL
);

U_BOOT_CMD(
	autoboot, CFG_MAXARGS, 1,	do_autoboot,
	"autoboot   - Continue auto-boot flow\n",
	NULL
);
