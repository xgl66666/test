/** \file fs_test_interactive.c
 *  \brief Interactive Tests for the FS layer of the TDAL API
 *  \author Arnaud Viala
 *  \date 2007, 20th of August
 *  \version 1.0
 */

#ifdef USE_TDAL_FS
/* The test is meaningless if you don't use TDAL_FS */

/******************************************************
 *              Includes                              *
 ******************************************************/
#include <string.h>
#include <stdio.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_fs.h"

#include "testhelper.h"
#include "TestManager.h"
#include "fstests.h"
#include "tester_prefs.h"




/******************************************************
 *              FS test tree                         *
 ******************************************************/
void    TestInteractive_FS_Menu( void );

static tTestNode gTestInteractive = {
	"TestInteractive",
	"Interactive test",
	0,
	TestInteractive_FS_Menu,
	NULL
};


tTestNode* gFSInteractiveTestNodeList[] = {
	&gTestInteractive,
	NULL
};

tTestNode gTDAL_FS_InteractiveTestSuite = {
	"TDAL_FS Interactive Tests",
	"test TDAL_FS Interactive",
	1, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gFSInteractiveTestNodeList
};




/******************************************************
 *              Defines                               *
 ******************************************************/
#define MAX_THREAD_NB                       8
#define MAX_FILE_NB                         8

#define kTEST_TDAL_FS_BUFFER_SIZE           750*1024

#define THREAD_TEMPOIZATION_MS              0

/******************************************************
 *              Enums / Typedefs / Structures         *
 ******************************************************/
struct tTestInteractive_FS_Thread_s {
    bool            used;
    uint8_t         id;
    TKEL_task_id    task_id;
    char            read_or_write;
    uint8_t         order_stop;
    uint8_t         file_id;
    uint32_t        total_data_size;
};

struct tTestInteractive_FS_File_s {
    char *          filename;
    bool            writing;
    bool            reading;
};

/******************************************************
 *              Local variables (LOCAL)               *
 ******************************************************/
LOCAL   struct tTestInteractive_FS_Thread_s     threads_array[MAX_THREAD_NB];
LOCAL   bool                                    threads_array_initialized = FALSE;

LOCAL   uint8_t *                               TestFS_Int_buffer;

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL   bool                                    gammalive_started = FALSE;
#endif

LOCAL   struct tTestInteractive_FS_File_s       files_array[MAX_FILE_NB] =
{
    { FS_HDD_LOCATION"/file0", FALSE, FALSE },
    { FS_HDD_LOCATION"/file1", FALSE, FALSE },
    { FS_HDD_LOCATION"/file2", FALSE, FALSE },
    { FS_HDD_LOCATION"/file3", FALSE, FALSE },
    { FS_HDD_LOCATION"/file4", FALSE, FALSE },
    { FS_HDD_LOCATION"/file5", FALSE, FALSE },
    { FS_HDD_LOCATION"/file6", FALSE, FALSE },
    { FS_HDD_LOCATION"/file7", FALSE, FALSE }
};

/******************************************************
 *              Functions declarations                *
 ******************************************************/
LOCAL   void    p_TestInteractive_FS_WriterTask( void * arg );
LOCAL   void    p_TestInteractive_FS_ReaderTask( void * arg );
LOCAL   void    p_TestInteractive_FS_ThreadStart( char read_or_write );
LOCAL   void    p_TestInteractive_FS_ThreadList( void );
LOCAL   void    p_TestInteractive_FS_ThreadStop( bool );
LOCAL   uint8_t p_TestInteractive_FS_ThreadRunningCountGet( void );

LOCAL   void    p_TestInteractive_FS_FileList( void );

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL   void    p_TestInteractive_FS_GammaLiveToggle( void );
#endif

IMPORT  void    TestFSi_Sce_Initialize_GammaLive( int tuner );
IMPORT  void    TestFSi_Sce_Terminate_GammaLive( void );



/*! \fn void TestInteractive_FS_Menu( void )
 *  \brief
 */
void TestInteractive_FS_Menu(void)
{
    char            command = 'h';
    bool            end = FALSE;
    TKEL_err        tk_err;

    tk_err = TKEL_Malloc( kTEST_TDAL_FS_BUFFER_SIZE ,
                (void **) &TestFS_Int_buffer );
    if ( tk_err != TKEL_NO_ERR || TestFS_Int_buffer == NULL )
    {
        mTBOX_PRINT((kTBOX_LF,"Cannot allocate buffer (%s bytes) => test aborted\n",
                     kTEST_TDAL_FS_BUFFER_SIZE));
        return;
    }

    TestHelper_Configure_TDAL_FS();

    while( !end )
    {
        switch( command )
        {
        case 'h':
        case 'H':
            mTBOX_PRINT((kTBOX_LF,"Interactive test : on \""FS_HDD_LOCATION"\"\n"));
            mTBOX_PRINT((kTBOX_LF," w - create a writer thread\n"));
            mTBOX_PRINT((kTBOX_LF," r - create a reader thread\n"));
            mTBOX_PRINT((kTBOX_LF," l - list current activity\n"));
            mTBOX_PRINT((kTBOX_LF," s - stop one thread\n"));
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
            mTBOX_PRINT((kTBOX_LF," v - toggle on/off the audiovideo gammalive\n"));
#endif /* PACKAGE_PVR */
            mTBOX_PRINT((kTBOX_LF," h - this help\n"));
            mTBOX_PRINT((kTBOX_LF," q - exit this test\n"));
            mTBOX_PRINT((kTBOX_LF,"\n" ));
            break;
        case 'w':
        case 'W':
            /*mTBOX_PRINT((kTBOX_LF,"Starting a writer thread\n"));*/
            p_TestInteractive_FS_ThreadStart( 'w' );
            break;
        case 'r':
        case 'R':
            /*mTBOX_PRINT((kTBOX_LF,"Starting a reader thread\n"));*/
            p_TestInteractive_FS_ThreadStart( 'r' );
            break;
        case 's':
        case 'S':
            /*mTBOX_PRINT((kTBOX_LF,"Stopping a thread\n"));*/
            p_TestInteractive_FS_ThreadStop( FALSE );
            break;
        case 'l':
        case 'L':
            /*mTBOX_PRINT((kTBOX_LF,"Listing threads\n"));*/
            p_TestInteractive_FS_ThreadList();
            break;
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
        case 'v':
        case 'V':
            /*mTBOX_PRINT((kTBOX_LF,"Toggling audiovideo gammalive\n"));*/
            p_TestInteractive_FS_GammaLiveToggle();
            break;
#endif /* PACKAGE_PVR */
        case 'q':
        case 'Q':
            p_TestInteractive_FS_ThreadStop( TRUE );
            end = TRUE;
            break;
        }

        if ( end == FALSE )
        {
            command = TestManager_GetChar( "Enter your choice : " );
        }
    }

    while( p_TestInteractive_FS_ThreadRunningCountGet() != 0 )
    {
        TKEL_DelayTask( 1000 );
    }

    TestHelper_Unconfigure_TDAL_FS();

    TKEL_Free( TestFS_Int_buffer );
}

LOCAL   void    p_TestInteractive_FS_ThreadStart( char read_or_write )
{
    TKEL_err    tkel_err;
    uint8_t     uc_loop;
    char        file_char;
    uint8_t     file_id = 0xFF;

    if ( threads_array_initialized == FALSE )
    {
        threads_array_initialized = TRUE;
        for( uc_loop=0 ; uc_loop<MAX_THREAD_NB ; uc_loop++ )
        {
            threads_array[uc_loop].used     = FALSE;
            threads_array[uc_loop].id       = uc_loop;
        }
    }


    /* Choose a file */
    p_TestInteractive_FS_FileList();
    while( file_id == 0xFF )
    {
        file_char = TestManager_GetChar( "Enter the number of file you want to use : " );
        if ( file_char >= '0' && file_char <= '9' )
        {
            file_id = file_char - '0';
        }
        if ( file_id < MAX_FILE_NB )
        {
            if ( ( files_array[file_id].reading == TRUE && read_or_write == 'r' ) ||
                 ( files_array[file_id].writing == TRUE && read_or_write == 'w' ) )
            {
                mTBOX_PRINT((kTBOX_LF,"File is already used for '%c'\n", read_or_write));
                file_id = 0xFF;
            }
        }
    }


    /* Look for a free thread structure */
    for( uc_loop=0 ; uc_loop<MAX_THREAD_NB ; uc_loop++ )
    {
        if ( threads_array[uc_loop].used == FALSE )
        {
            threads_array[uc_loop].used = TRUE;
            break;
        }
    }

    if ( uc_loop < MAX_THREAD_NB )
    {
        threads_array[uc_loop].order_stop = FALSE;
        threads_array[uc_loop].read_or_write = read_or_write;
        threads_array[uc_loop].file_id = file_id;

        tkel_err = TKEL_CreateTask( 64 ,
                                    NULL , 4096 ,
                                    ( read_or_write == 'w' ?
                                        p_TestInteractive_FS_WriterTask :
                                        p_TestInteractive_FS_ReaderTask ),
                                    &threads_array[uc_loop] ,
                                    &threads_array[uc_loop].task_id );
        if ( tkel_err != TKEL_NO_ERR )
        {
            mTBOX_PRINT((kTBOX_LF,"Thread creation failed\n"));
            threads_array[uc_loop].used = FALSE;
        }
    }
    else
    {
        mTBOX_PRINT((kTBOX_LF,"Max number of thread reached\n"));
    }
}

LOCAL   void    p_TestInteractive_FS_ThreadStop( bool all_thread )
{
    uint8_t     thread_id = 0xFF;
    char        thread_char;


    if ( all_thread == FALSE )
    {
        p_TestInteractive_FS_ThreadList();

        /*thread_id = TestManager_GetNumber( "Enter the thread number you want to stop : " , 4 );*/
        thread_char = TestManager_GetChar( "Enter the thread number you want to stop : " );
        if ( thread_char >= '0' && thread_char <= '9' )
        {
            thread_id = thread_char - '0';
        }
        if ( thread_id < MAX_THREAD_NB )
        {
            if ( threads_array[thread_id].used == TRUE )
            {
                threads_array[thread_id].order_stop = TRUE;
            }
            else
            {
                mTBOX_PRINT((kTBOX_LF,"Thread %d is not active\n", thread_id ));
            }
        }
    }
    else
    {
        for( thread_id=0 ; thread_id<MAX_THREAD_NB ; thread_id++ )
        {
            if ( threads_array[thread_id].used == TRUE )
            {
                threads_array[thread_id].order_stop = TRUE;
            }
        }
    }
}


LOCAL   uint8_t p_TestInteractive_FS_ThreadRunningCountGet( void )
{
    uint8_t uc_loop;
    uint8_t ret = 0;

    for( uc_loop=0 ; uc_loop<MAX_THREAD_NB ; uc_loop++ )
    {
        if ( threads_array[uc_loop].used == TRUE )
        {
            ret++;
        }
    }

    return ret;
}

LOCAL   void    p_TestInteractive_FS_ThreadList( void )
{
    uint8_t     uc_loop;
    uint8_t     nb_of_thread = 0;
    mTBOX_PRINT((kTBOX_LF,"List of threads :\n"));
    for( uc_loop=0 ; uc_loop<MAX_THREAD_NB ; uc_loop++ )
    {
        if ( threads_array[uc_loop].used == TRUE )
        {
            mTBOX_PRINT((kTBOX_LF,"   %d : Thread '%s' in the file '%s' (data_size = %d MB)\n",
                threads_array[uc_loop].id,
                (threads_array[uc_loop].read_or_write == 'w' ? "writing" : "reading"),
                files_array[ threads_array[uc_loop].file_id ].filename,
                threads_array[uc_loop].total_data_size/(1024*1024) ));
            nb_of_thread++;
        }
    }
    mTBOX_PRINT((kTBOX_LF,"   %s\n", (nb_of_thread==0?"[no threads]":"") ));
}



LOCAL   void    p_TestInteractive_FS_FileList( void )
{
    uint8_t     uc_loop;
    uint8_t     nb_files;

    mTBOX_PRINT((kTBOX_LF,"List of files :\n"));

    nb_files = sizeof( files_array ) / sizeof( struct tTestInteractive_FS_File_s);
    for( uc_loop=0 ; uc_loop<nb_files ; uc_loop++ )
    {
        mTBOX_PRINT((kTBOX_LF,"   %d : [%c%c] '%s'\n",
                    uc_loop,
                    (files_array[uc_loop].writing?'w':' '),
                    (files_array[uc_loop].reading?'r':' '),
                    files_array[uc_loop].filename ));
    }

    mTBOX_PRINT((kTBOX_LF,"   %s\n", (nb_files==0?"[no files]":"") ));
}



LOCAL   void    p_TestInteractive_FS_WriterTask( void * arg )
{
    struct tTestInteractive_FS_Thread_s *   p_thread;
    struct tTestInteractive_FS_File_s *     p_file;
    tTDAL_FS_File                           file_handle;
    tTDAL_FS_Size                           data_size;
    uint8_t                                 nb_successive_errors;
    uint8_t                                 nb_errors;

    p_thread = (struct tTestInteractive_FS_Thread_s *)arg;
    p_file = &files_array[p_thread->file_id];

    mTBOX_PRINT((kTBOX_LF,"WriteThread %d : Starting (with file %d)\n", p_thread->id, p_thread->file_id ));


    if(TDAL_FS_FileExist(p_file->filename) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( p_file->filename ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs delete" );
	}

    file_handle = TDAL_FS_Open( p_file->filename ,
                                kTDAL_FS_MODE_WRITE|
                                kTDAL_FS_MODE_CREATE |
                                kTDAL_FS_MODE_TRUNC  );

    TestManager_AssertTrue( ( file_handle != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );

    if ( file_handle != kTDAL_FS_INVALID_HANDLE )
    {
        p_file->writing = TRUE;
        p_thread->total_data_size = 0;
        while( p_thread->order_stop == FALSE )
        {
            data_size = TDAL_FS_Write( file_handle , TestFS_Int_buffer , kTEST_TDAL_FS_BUFFER_SIZE );
            if ( data_size > 0 )
            {
                p_thread->total_data_size += data_size;
                nb_successive_errors=0;
            }
            else if ( data_size == 0 )
            {
                mTBOX_PRINT((kTBOX_LF,"WriteThread %d : may we encounter EOF ?\n", p_thread->id ));
                nb_successive_errors=0;
            }
            else if ( data_size == -1 )
            {
                mTBOX_PRINT((kTBOX_LF,"WriteThread %d : error ?\n", p_thread->id ));
                nb_successive_errors++;
                nb_errors++;
            }

            #if THREAD_TEMPOIZATION_MS > 0
            TKEL_DelayTask( ms2tick(THREAD_TEMPOIZATION_MS) );
            #endif

            if ( (uint32_t)(p_thread->total_data_size + kTEST_TDAL_FS_BUFFER_SIZE) > 0x80000000 /*(2*1024*1024*1024)*/ )
            {
                mTBOX_PRINT((kTBOX_LF,"WriteThread %d : 2 GB limit going to be reach => stop writing\n", p_thread->id ));
                p_thread->order_stop = TRUE;
            }
        }

        p_file->writing = FALSE;

        TDAL_FS_Close( file_handle );
    }

    mTBOX_PRINT((kTBOX_LF,"WriteThread %d : Exiting\n", p_thread->id ));

    p_thread->used = FALSE;
}


LOCAL   void    p_TestInteractive_FS_ReaderTask( void * arg )
{
    struct tTestInteractive_FS_Thread_s *   p_thread;
    struct tTestInteractive_FS_File_s *     p_file;
    tTDAL_FS_File                           file_handle = kTDAL_FS_INVALID_HANDLE;
    tTDAL_FS_Size                           data_size;
    uint8_t                                 nb_successive_errors;
    uint8_t                                 nb_errors;

    p_thread = (struct tTestInteractive_FS_Thread_s *)arg;
    p_file = &files_array[p_thread->file_id];

    mTBOX_PRINT((kTBOX_LF,"ReadThread %d : Starting (with file %d)\n", p_thread->id, p_thread->file_id ));


    if(TDAL_FS_FileExist(p_file->filename) != eTDAL_FS_NO_ERROR)
	{
        mTBOX_PRINT((kTBOX_LF,"ReadThread %d : file %s doesn't exist\n", p_thread->id , p_file->filename));
        p_thread->order_stop = TRUE;
	}

    if ( p_thread->order_stop == FALSE )
    {
        file_handle = TDAL_FS_Open( p_file->filename ,
                                    kTDAL_FS_MODE_READ );
        TestManager_AssertTrue( ( file_handle != kTDAL_FS_INVALID_HANDLE ) ,
                                "fs file open" );
    }

    if ( file_handle != kTDAL_FS_INVALID_HANDLE )
    {
        p_file->reading = TRUE;
        p_thread->total_data_size = 0;
        while( p_thread->order_stop == FALSE )
        {
            data_size = TDAL_FS_Read( file_handle , TestFS_Int_buffer , kTEST_TDAL_FS_BUFFER_SIZE );
            if ( data_size > 0 )
            {
                p_thread->total_data_size += data_size;
                nb_successive_errors=0;
            }
            else if ( data_size == 0 )
            {
                mTBOX_PRINT((kTBOX_LF,"TDAL_FS_Read %d : EOF ? -> Seek(begin)\n", p_thread->id ));
                TDAL_FS_Seek( file_handle , 0 , eTDAL_FS_SEEK_ORIGIN_BEGIN );
                p_thread->total_data_size = 0;
                nb_successive_errors=0;
            }
            else if ( data_size == -1 )
            {
                mTBOX_PRINT((kTBOX_LF,"ReadThread %d : error\n", p_thread->id ));
                nb_successive_errors++;
                nb_errors++;
            }

            #if THREAD_TEMPOIZATION_MS > 0
            TKEL_DelayTask( ms2tick(THREAD_TEMPOIZATION_MS) );
            #endif
        }

        p_file->reading = FALSE;

        TDAL_FS_Close( file_handle );
    }

    mTBOX_PRINT((kTBOX_LF,"ReadThread %d : Exiting\n", p_thread->id ));

    p_thread->used = FALSE;
}



#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL   void    p_TestInteractive_FS_GammaLiveToggle( void )
{
    if ( gammalive_started == FALSE )
    {
        mTBOX_PRINT((kTBOX_LF,"GammaLiveToggle: Starting\n"));
        TestFSi_Sce_Initialize_GammaLive( 0 /* tuner */ );
        gammalive_started = TRUE;
    }
    else
    {
        mTBOX_PRINT((kTBOX_LF,"GammaLiveToggle: Stopping\n"));
        TestFSi_Sce_Terminate_GammaLive();
        gammalive_started = FALSE;
    }
}
#endif /* PACKAGE_PVR */

#endif /* USE_TDAL_FS */



