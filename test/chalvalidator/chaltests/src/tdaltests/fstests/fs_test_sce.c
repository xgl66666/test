/** \file fs_test_sce.c
 *  \brief Scenario Tests for the FS layer of the TDAL API
 *  \author Arnaud Viala
 *  \date 2007, 20th of August
 *  \version 1.0
 */

#if 1//def USE_TDAL_FS
/* The test is meaningless if you don't use TDAL_FS */

/******************************************************
 *              Includes                              *
 ******************************************************/
#include <string.h>
#include <stdio.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_fs.h"

#include "tester_prefs.h"
#include "testhelper.h"
#include "TestManager.h"
#include "fstests.h"

/* Needed TDAL modules */
#include "tdal_av.h"
#include "tdal_dmx.h"
#include "tdal_disp.h"
#include "tdal_output.h"
#include "tdal_avs.h"
#include "tdal_dmd.h"
#ifdef PRODUCT_USE_CA
#include "tdal_desc.h"
#endif
#include "tdal_ts.h"



/******************************************************
 *              FS test tree                         *
 ******************************************************/
void    TestFS_Sce_WritePerf( void );
void    TestFS_Sce_ReadPerf( void );
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
void    TestFS_Sce_Mount_WithGammaLive( void );
void    TestFS_Sce_WritePerf_WithGammaLive( void );
#endif
void    TestFS_Sce_CircularFile( void );
void    TestFS_Sce_CircularFile_MT( void );

static tTestNode gTestScenario_WritePerf = {
	"TestScenario_WritePerf",
	"WritePerf scenario",
	0,
	TestFS_Sce_WritePerf,
	NULL
};
static tTestNode gTestScenario_ReadPerf = {
	"TestScenario_ReadPerf",
	"ReadPerf scenario",
	0,
	TestFS_Sce_ReadPerf,
	NULL
};
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
static tTestNode gTestScenario_MountGammaLive = {
	"TestScenario_MountGammaLive",
	"Mount during a GammaLive",
	0,
	TestFS_Sce_Mount_WithGammaLive,
	NULL
};
static tTestNode gTestScenario_WritePerfGammaLive = {
	"TestScenario_WritePerf_WithGammaLive",
	"WritePerf during a GammaLive",
	0,
	TestFS_Sce_WritePerf_WithGammaLive,
	NULL
};
#endif
static tTestNode gTestScenario_CircularFile = {
	"TestScenario_CircularFile",
	"CircularFile",
	0,
	TestFS_Sce_CircularFile,
	NULL
};
static tTestNode gTestScenario_CircularFile_MT = {
	"TestScenario_CircularFile_MT",
	"CircularFile (multithreaded version)",
	0,
	TestFS_Sce_CircularFile_MT,
	NULL
};



tTestNode* gFSScenarioTestNodeList[] = {
	&gTestScenario_WritePerf,
	&gTestScenario_ReadPerf,
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
    &gTestScenario_MountGammaLive,
    &gTestScenario_WritePerfGammaLive,
#endif
    &gTestScenario_CircularFile,
    &gTestScenario_CircularFile_MT,
	NULL
};

tTestNode gTDAL_FS_ScenarioTestSuite = {
	"TDAL_FS Scenario Tests",
	"test TDAL_FS scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gFSScenarioTestNodeList
};




/******************************************************
 *              Defines                               *
 ******************************************************/
#define kTEST_TDAL_FS_WRITE_PERF_NB_ITER    50

#define kTEST_TDAL_FS_READ_PERF_NB_ITER    kTEST_TDAL_FS_WRITE_PERF_NB_ITER

#define kTEST_TDAL_FS_TDAL_TSSOURCE_INTERNAL_BUFFER_SIZE    8192

#define kTEST_TDAL_FS_TDAL_TSSOURCE_INTERNAL_PACKET_SIZE    188

#define kTEST_TDAL_FS_BUFFER_SIZE                           1024  // 750 * 1024


/** CircularFile tests #defines **/
        /* Size of I/O block */
#define kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE      7542
        /* Block number of circular file */
#define kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_NB        9
        /* (Monothread) Step 1 : "Write some blocks" */
#define kTEST_TDAL_FS_CIRCULAR_FILE_STEP_1_BLOCK_NB 6
        /* (Monothread) Step 2 : "Alternate Read/Write one block" */
#define kTEST_TDAL_FS_CIRCULAR_FILE_STEP_2_BLOCK_NB 14
        /* (Monothread) Step 3 : "Write some blocks so that Writer exceed Reader" */
#define kTEST_TDAL_FS_CIRCULAR_FILE_STEP_3_BLOCK_NB 4
        /* (Monothread) Step 4 : "Read some blocks (to check data after W exceed R)" */
#define kTEST_TDAL_FS_CIRCULAR_FILE_STEP_4_BLOCK_NB 2

        /* (Multithread) Nb written block before start reading */
#define kTEST_TDAL_TS_CIRCULAR_FILE_MT_NB_WRITTEN_BEFORE_READ   5
        /* (Multithread) Sleep time for Writer thread */
#define kTEST_TDAL_TS_CIRCULAR_FILE_MT_WRITER_SLEEP_TIME_MS   1000
        /* (Multithread) Sleep time for Reader thread */
#define kTEST_TDAL_TS_CIRCULAR_FILE_MT_READER_SLEEP_TIME_MS   1100


#define ASSERT_TRACE_LEN                            96


/******************************************************
 *              Enums / Typedefs / Structures         *
 ******************************************************/
enum { /* MUST be mask values */
    eDMD  = 1,
    eDMX  = 2,
    eDESC = 4,
    eAV   = 8,
    eAVS  = 16,
    eDISP   = 32,
    eOUTPUT = 64
};

struct TestFSi_Sce_GammaLive_var_s
{
    tTDAL_TS_Object                 route_A_source;
    tTDAL_TS_Object                 route_B_source;
    tTDAL_TS_Object                 route_A_dest;
    tTDAL_TS_Object                 route_B_dest;

    tTDAL_TSSOURCE_Handle           tssource_handle;

    tTDAL_TSSINK_Handle             tssink_handle;

    struct TestManager_Channel *    pChannel;
};

struct TestFSi_Sce_DMX_PLAY_var_s
{
    bool                    DMX_PLAY_is_configured;
    tTDAL_DMX_ChannelId     DMX_PLAY_VideoChannelId;
    tTDAL_DMX_ChannelId     DMX_PLAY_AudioChannelId;
    tTDAL_DMX_ChannelId     DMX_PLAY_PcrChannelId;
    tTDAL_DMX_FilterId      DMX_PLAY_VideoFilterId;
    tTDAL_DMX_FilterId      DMX_PLAY_AudioFilterId;
    tTDAL_DMX_FilterId      DMX_PLAY_PcrFilterId;
};

struct TestFSi_Sce_CircularFile_var_s
{
    uint8_t *                       block_buffer;
    char                            block_values[kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_NB];
};

struct TestFSi_Sce_CircularFile_MT_var_s
{
    bool                            stop_writer;
    bool                            stop_reader;
    bool                            reader_running;
    bool                            writer_running;

    uint8_t                         nb_written;

    uint8_t *                       block_buffer;
};




/******************************************************
 *              Local variables (LOCAL)               *
 ******************************************************/
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL   struct TestFSi_Sce_GammaLive_var_s          TestFSi_Sce_GammaLive_vars;
LOCAL   struct TestFSi_Sce_DMX_PLAY_var_s           TestFSi_Sce_DMX_PLAY_vars = { FALSE , 0,0,0,0,0,0 };
#endif

LOCAL   struct TestFSi_Sce_CircularFile_var_s       TestFSi_Sce_CircularFile_vars;

LOCAL   struct TestFSi_Sce_CircularFile_MT_var_s    TestFSi_Sce_CircularFile_MT_vars;

LOCAL   char *      TestFS_Sce_CircularFile_filename = FS_HDD_LOCATION"/circular.bin";

        /* used to make specific string to use with TestManager_AssertXXX() functions
         * note: this variable must be used in main thread (not in others threads) */
LOCAL   char        assert_trace[ASSERT_TRACE_LEN];

LOCAL   char *      TestFS_Sce_Perf_filename = FS_HDD_LOCATION"/Perf.bin";

LOCAL   uint32_t    TestFS_Sce_Perf_maxiter = 0;

/******************************************************
 *              Functions declarations                *
 ******************************************************/
#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
LOCAL   void    TestFSi_Sce_InitializeTDALModules( uint32_t mask );
LOCAL   void    TestFSi_Sce_TerminateTDALModules( uint32_t mask );
void    TestFSi_Sce_Initialize_GammaLive( int tuner );
void    TestFSi_Sce_Terminate_GammaLive( void );
LOCAL   void    TestFSi_Sce_DMX_PLAY_Configure( int iDmxIdx, int iDmxIdxPcr, uint16_t pid_video, uint16_t pid_audio, uint16_t pid_pcr );
LOCAL   void    TestFSi_Sce_DMX_PLAY_Unconfigure( void );
LOCAL   void    TestFSi_Sce_DMX_PLAY_Start( void );
LOCAL   void    TestFSi_Sce_DMX_PLAY_Stop( void );
LOCAL   void    TestFSi_Sce_Configure_AV( void );
LOCAL   void    TestFSi_Sce_Unconfigure_AV( void );
#endif /* PACKAGE_PVR */

/* Allocate and fill the buffer */
void TestFSi_Sce_InitBuffer( uint8_t ** buffer )
{
    uint32_t    buffer_len;
    uint32_t    index_in_buffer;
    TKEL_err    tk_err;
	uint8_t     *p_buffer;

    buffer_len = kTEST_TDAL_FS_BUFFER_SIZE;
    tk_err = TKEL_Malloc( buffer_len , (void **)&p_buffer );
    if ( tk_err != TKEL_NO_ERR )
    {
        mTBOX_PRINT((kTBOX_LF,"Cannot allocate buffer (%d bytes)\n", buffer_len));
        p_buffer = NULL;
    }

    if ( p_buffer != NULL )
    {
        for( index_in_buffer=0 ; index_in_buffer<buffer_len ; index_in_buffer++ )
        {
            p_buffer[index_in_buffer] = ( index_in_buffer & 0xFF );
        }

        p_buffer[0] = '[';
        p_buffer[buffer_len-1] = ']';
    }

	*buffer = p_buffer;
}

void TestFSi_Sce_TermBuffer( uint8_t ** buffer )
{
    TKEL_Free( *buffer );
}




/*! \fn void TestFS_Sce_WritePerf( void )
 *  \brief WritePerf scenario
 */

void TestFSi_Sce_WritePerf( bool with_gammalive )
{
    uint32_t        cnt_iter;
    tTDAL_FS_File   file_handle;
    tTDAL_FS_Size   written;
    uint8_t         nb_error = 0;
    char            write_trace[64]; /* "fs file write #........ (written=......)" */
    uint8_t *       buffer;

    TestFSi_Sce_InitBuffer( &buffer );

    if ( TestFS_Sce_Perf_maxiter == 0 )
    {
        TestFS_Sce_Perf_maxiter = TestManager_GetNumber("Enter the number of iteration :", 50);
    }

	 TestHelper_Configure_TDAL_FS();

    if(TDAL_FS_FileExist(TestFS_Sce_Perf_filename) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TestFS_Sce_Perf_filename ) ,
								 eTDAL_FS_NO_ERROR ,
								 "fs delete" );
	}

    file_handle = TDAL_FS_Open( TestFS_Sce_Perf_filename ,
                                kTDAL_FS_MODE_WRITE |
                                kTDAL_FS_MODE_CREATE |
                                kTDAL_FS_MODE_TRUNC );

    TestManager_AssertTrue( ( file_handle != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
    if ( with_gammalive )
    {
        TestFSi_Sce_Initialize_GammaLive( 0 /* Tuner0 */ );
        TestManager_AskForChecking("do you see audio and video playing");
    }
#endif
    for( cnt_iter=0 ; (cnt_iter<TestFS_Sce_Perf_maxiter) && (nb_error < 7) ; cnt_iter++ )
    {
        sprintf( (char *)(&buffer[1]) , "Perf #%d" , cnt_iter );

        /* TKEL_DelayTask( ms2tick(1000) ); */

        written = TDAL_FS_Write( file_handle , buffer , kTEST_TDAL_FS_BUFFER_SIZE );

        snprintf( write_trace , 64 ,
                  "fs file write #%08d (written=%d)", cnt_iter, written );

        TestManager_AssertTrue( ( written == kTEST_TDAL_FS_BUFFER_SIZE ) ,
                                write_trace );
        if ( written != kTEST_TDAL_FS_BUFFER_SIZE )
        {
            nb_error++;
        }
    }

#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
    if ( with_gammalive )
    {
        TestFSi_Sce_Terminate_GammaLive();
    }
#endif

    TestManager_AssertEqual( TDAL_FS_Close( file_handle ) ,
                             eTDAL_FS_NO_ERROR ,
                             "fs delete" );
	


	TestHelper_Unconfigure_TDAL_FS();

    TestFSi_Sce_TermBuffer( &buffer );

}



void TestFS_Sce_WritePerf(void)
{
    TestFSi_Sce_WritePerf( FALSE /* without gammalive */ );
}

/*! \fn void TestFS_Sce_WritePerf( void )
 *  \brief WritePerf scenario
 */

void TestFS_Sce_ReadPerf(void)
{
    uint32_t        cnt_iter;
    tTDAL_FS_File   file_handle;
    tTDAL_FS_Size   read;
    uint8_t         nb_error = 0;
    char            read_trace[32]; /* "fs file read #........" */
    uint8_t *       buffer;

    TestFSi_Sce_InitBuffer( &buffer );
    TestManager_AssertTrue( ( buffer != NULL ) , "Buffer not NULL" );


    if ( TestFS_Sce_Perf_maxiter == 0 )
    {
        TestFS_Sce_Perf_maxiter = TestManager_GetNumber("Enter the number of iteration :", 50);
    }

	TestHelper_Configure_TDAL_FS();

    TestManager_AssertTrue( ( TDAL_FS_FileExist(TestFS_Sce_Perf_filename) == eTDAL_FS_NO_ERROR ) ,
                            "fs file exist" );

    file_handle = TDAL_FS_Open( TestFS_Sce_Perf_filename ,
                                kTDAL_FS_MODE_READ/* |
                                kTDAL_FS_MODE_CREATE |
                                kTDAL_FS_MODE_TRUNC*/ );

    TestManager_AssertTrue( ( file_handle != kTDAL_FS_INVALID_HANDLE ) ,
                            "fs file open" );

    for( cnt_iter=0 ; (cnt_iter<TestFS_Sce_Perf_maxiter) && (nb_error < 3) ; cnt_iter++ )
    {
        sprintf( (char *)(&buffer[1]) , "Perf #%d" , cnt_iter );

        read = TDAL_FS_Read( file_handle , buffer , kTEST_TDAL_FS_BUFFER_SIZE );

        snprintf( read_trace , 32 ,
                  "fs file read #%08d", cnt_iter );

        TestManager_AssertTrue( ( read == kTEST_TDAL_FS_BUFFER_SIZE ) ,
                                read_trace );
        if ( read != kTEST_TDAL_FS_BUFFER_SIZE )
        {
            nb_error++;
        }
    }

    TestManager_AssertEqual( TDAL_FS_Close( file_handle ) ,
                             eTDAL_FS_NO_ERROR ,
                             "fs delete" );
	


	TestHelper_Unconfigure_TDAL_FS();

    TestFSi_Sce_TermBuffer( &buffer );
}


#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)
void TestFS_Sce_Mount_WithGammaLive(void)
{
    TestFSi_Sce_Initialize_GammaLive( 0 );

    TestManager_AskForChecking("do you see audio and video playing");

	TestHelper_Configure_TDAL_FS();

    TestManager_AskForChecking("Is there no audio/video problem ?");

	TestHelper_Unconfigure_TDAL_FS();

    TestFSi_Sce_Terminate_GammaLive();
}



void TestFS_Sce_WritePerf_WithGammaLive(void)
{
    TestFSi_Sce_WritePerf( TRUE /* with gammalive */ );
}
#endif













LOCAL   void    TestFSi_Sce_CircularWrite( tTDAL_FS_File   file_handle_w ,
                                           uint8_t *       file_index_w ,
                                           char            block_value)
{
    tTDAL_FS_Size   written;
    tTDAL_FS_Error  fs_err;

    memset( TestFSi_Sce_CircularFile_vars.block_buffer ,
            block_value ,
            kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE );

    if ( *file_index_w >= kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_NB )
    {
        fs_err = TDAL_FS_Seek( file_handle_w , 0 , eTDAL_FS_SEEK_ORIGIN_BEGIN );
        TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                "TDAL_FS_Seek( circ[W] )" );
        *file_index_w = 0;
    }

    written = TDAL_FS_Write( file_handle_w ,
                             TestFSi_Sce_CircularFile_vars.block_buffer ,
                             kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE );
    snprintf( assert_trace , ASSERT_TRACE_LEN ,
              "TDAL_FS_Write( circ , w=%d , data='%c%c%c%c...' ) => %d bytes written",
              *file_index_w,
              TestFSi_Sce_CircularFile_vars.block_buffer[0],
              TestFSi_Sce_CircularFile_vars.block_buffer[1],
              TestFSi_Sce_CircularFile_vars.block_buffer[2],
              TestFSi_Sce_CircularFile_vars.block_buffer[3],
              written);
    TestManager_AssertTrue( ( written == kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE ) ,
                            assert_trace );
    TestFSi_Sce_CircularFile_vars.block_values[*file_index_w] = block_value;

}

LOCAL   void    TestFSi_Sce_CircularRead( tTDAL_FS_File   file_handle_r ,
                                          uint8_t *       file_index_r )
{
    tTDAL_FS_Size   read;
    tTDAL_FS_Error  fs_err;

    if ( *file_index_r >= kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_NB )
    {
        fs_err = TDAL_FS_Seek( file_handle_r , 0 , eTDAL_FS_SEEK_ORIGIN_BEGIN );
        TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                "TDAL_FS_Seek( circ[R] )" );
        *file_index_r = 0;
    }

    read = TDAL_FS_Read( file_handle_r ,
                         TestFSi_Sce_CircularFile_vars.block_buffer ,
                         kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE );
    snprintf( assert_trace , ASSERT_TRACE_LEN ,
              "TDAL_FS_Read( circ , r=%d ) => %d bytes read -> data='%c%c%c%c...'",
              *file_index_r , read ,
              TestFSi_Sce_CircularFile_vars.block_buffer[0],
              TestFSi_Sce_CircularFile_vars.block_buffer[1],
              TestFSi_Sce_CircularFile_vars.block_buffer[2],
              TestFSi_Sce_CircularFile_vars.block_buffer[3] );
    TestManager_AssertTrue( ( read == kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE ) ,
                            assert_trace );
    TestManager_AssertEqual( TestFSi_Sce_CircularFile_vars.block_values[*file_index_r],
                             TestFSi_Sce_CircularFile_vars.block_buffer[0] ,
                             "Checking file's data" );
}





void TestFS_Sce_CircularFile( void )
{
    uint8_t         uc_loop;
    uint8_t         file_index_r;
    uint8_t         file_index_w;
    tTDAL_FS_File   file_handle_r;
    tTDAL_FS_File   file_handle_w;
    /*tTDAL_FS_Size   read;
    tTDAL_FS_Size   written;*/
    tTDAL_FS_Error  fs_err;
    char            block_value = (char)('A' - 1);
    TKEL_err        tk_err;

    tk_err = TKEL_Malloc( kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE ,
                (void **) &TestFSi_Sce_CircularFile_vars.block_buffer );
    if ( tk_err != TKEL_NO_ERR ||
         TestFSi_Sce_CircularFile_vars.block_buffer == NULL )
    {
        mTBOX_PRINT((kTBOX_LF,"Cannot allocate buffer (%s bytes) => test aborted\n",
                     kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE));
        return;
    }

	TestHelper_Configure_TDAL_FS();

    if(TDAL_FS_FileExist(TestFS_Sce_CircularFile_filename) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TestFS_Sce_CircularFile_filename ) ,
								 eTDAL_FS_NO_ERROR ,
								 "TDAL_FS_Remove( circular )" );
	}

    file_handle_w = TDAL_FS_Open( TestFS_Sce_CircularFile_filename ,
                                  kTDAL_FS_MODE_WRITE |
                                  kTDAL_FS_MODE_CREATE |
                                  kTDAL_FS_MODE_TRUNC );
    TestManager_AssertTrue( ( file_handle_w != kTDAL_FS_INVALID_HANDLE ) ,
                            "TDAL_FS_Open( circ , W )" );

    file_handle_r = TDAL_FS_Open( TestFS_Sce_CircularFile_filename ,
                                  kTDAL_FS_MODE_READ );
    TestManager_AssertTrue( ( file_handle_r != kTDAL_FS_INVALID_HANDLE ) ,
                            "TDAL_FS_Open( circ , R )" );



    if ( file_handle_w != kTDAL_FS_INVALID_HANDLE &&
         file_handle_r != kTDAL_FS_INVALID_HANDLE )
    {
        file_index_r = 0;
        file_index_w = 0;

        /* STEP 1 */
        mTBOX_PRINT((kTBOX_LF,"STEP 1\n"));
        for( uc_loop=0 ; uc_loop<kTEST_TDAL_FS_CIRCULAR_FILE_STEP_1_BLOCK_NB ; uc_loop++ )
        {
            block_value++;

            TestFSi_Sce_CircularWrite( file_handle_w , &file_index_w , block_value );

            file_index_w++;
        }

        /* STEP 2 */
        mTBOX_PRINT((kTBOX_LF,"STEP 2\n"));
        for( uc_loop=0 ; uc_loop<kTEST_TDAL_FS_CIRCULAR_FILE_STEP_2_BLOCK_NB ; uc_loop++ )
        {
            /* STEP 2 - part A : Read */
            TestFSi_Sce_CircularRead( file_handle_r , &file_index_r );

            file_index_r++;

            /* STEP 2 - part B : Write */
            block_value++;

            TestFSi_Sce_CircularWrite( file_handle_w , &file_index_w , block_value );

            file_index_w++;
        }

        /* STEP 3 */
        mTBOX_PRINT((kTBOX_LF,"STEP 3\n"));
        for( uc_loop=0 ; uc_loop<kTEST_TDAL_FS_CIRCULAR_FILE_STEP_3_BLOCK_NB ; uc_loop++ )
        {
            block_value++;

            TestFSi_Sce_CircularWrite( file_handle_w , &file_index_w , block_value );

            file_index_w++;
        }

        /* STEP 4 */
        mTBOX_PRINT((kTBOX_LF,"STEP 4\n"));
        for( uc_loop=0 ; uc_loop<kTEST_TDAL_FS_CIRCULAR_FILE_STEP_4_BLOCK_NB ; uc_loop++ )
        {
            TestFSi_Sce_CircularRead( file_handle_r , &file_index_r );

            file_index_r++;
        }
    }
    else
    {
        mTBOX_PRINT((kTBOX_LF,"Skipping CircularTest\n"));
    }

    if ( file_handle_r != kTDAL_FS_INVALID_HANDLE )
    {
        fs_err = TDAL_FS_Close( file_handle_r );
        TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                "TDAL_FS_Close( circ[R] )" );
    }

    if ( file_handle_w != kTDAL_FS_INVALID_HANDLE )
    {
        fs_err = TDAL_FS_Close( file_handle_w );
        TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                "TDAL_FS_Close( circ[W] )" );
	}


	TestHelper_Unconfigure_TDAL_FS();


    TKEL_Free( TestFSi_Sce_CircularFile_vars.block_buffer );
}




void    TestFSi_Sce_CircularFile_MT_WriterThread( void * arg )
{
    tTDAL_FS_File   file_handle;
    tTDAL_FS_Size   written;
    tTDAL_FS_Error  fs_err;
    uint8_t         writer_index;
    char            assert_trace[ASSERT_TRACE_LEN];

    file_handle = TDAL_FS_Open( TestFS_Sce_CircularFile_filename ,
                                kTDAL_FS_MODE_WRITE |
                                kTDAL_FS_MODE_CREATE |
                                kTDAL_FS_MODE_TRUNC );
    TestManager_AssertTrue( ( file_handle != kTDAL_FS_INVALID_HANDLE ) ,
                            "TDAL_FS_Open( circ , W )" );

    writer_index = 0;

    while( TestFSi_Sce_CircularFile_MT_vars.stop_writer == FALSE )
    {
        if ( writer_index >= kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_NB )
        {
            fs_err = TDAL_FS_Seek( file_handle , 0 , eTDAL_FS_SEEK_ORIGIN_BEGIN );
            TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                    "TDAL_FS_Seek( circ[W] )" );
            writer_index = 0;
        }
        written = TDAL_FS_Write( file_handle ,
                                 TestFSi_Sce_CircularFile_MT_vars.block_buffer ,
                                 kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE );
        snprintf( assert_trace , ASSERT_TRACE_LEN ,
                  "TDAL_FS_Write( circ , w=%d ) => %d bytes written",
                  writer_index , written );
        TestManager_AssertTrue( ( written == kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE ) ,
                                assert_trace );

        writer_index++;
        TestFSi_Sce_CircularFile_MT_vars.nb_written++;

        TKEL_DelayTask( ms2tick(kTEST_TDAL_TS_CIRCULAR_FILE_MT_WRITER_SLEEP_TIME_MS) );
    }

    if ( file_handle != kTDAL_FS_INVALID_HANDLE )
    {
        fs_err = TDAL_FS_Close( file_handle );
        TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                "TDAL_FS_Close( circ[W] )" );
	}

    TestFSi_Sce_CircularFile_MT_vars.writer_running = FALSE;
}


void    TestFSi_Sce_CircularFile_MT_ReaderThread( void * arg )
{
    tTDAL_FS_File   file_handle;
    tTDAL_FS_Size   read;
    tTDAL_FS_Error  fs_err;
    uint8_t         reader_index;
    char            assert_trace[ASSERT_TRACE_LEN];

    file_handle = TDAL_FS_Open( TestFS_Sce_CircularFile_filename ,
                                kTDAL_FS_MODE_READ/* |
                                kTDAL_FS_MODE_CREATE |
                                kTDAL_FS_MODE_TRUNC*/ );
    TestManager_AssertTrue( ( file_handle != kTDAL_FS_INVALID_HANDLE ) ,
                            "TDAL_FS_Open( circ , R )" );

    reader_index = 0;

    while( TestFSi_Sce_CircularFile_MT_vars.stop_reader == FALSE )
    {
        if ( TestFSi_Sce_CircularFile_MT_vars.nb_written
             >= kTEST_TDAL_TS_CIRCULAR_FILE_MT_NB_WRITTEN_BEFORE_READ )
        {
            if ( reader_index >= kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_NB )
            {
                fs_err = TDAL_FS_Seek( file_handle , 0 , eTDAL_FS_SEEK_ORIGIN_BEGIN );
                TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                        "TDAL_FS_Seek( circ[R] )" );
                reader_index = 0;
            }
            read = TDAL_FS_Read( file_handle ,
                                 TestFSi_Sce_CircularFile_MT_vars.block_buffer ,
                                 kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE );
            snprintf( assert_trace , ASSERT_TRACE_LEN ,
                      "TDAL_FS_Read( circ , r=%d ) => %d bytes read",
                      reader_index , read );
            TestManager_AssertTrue( ( read == kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE ) ,
                                    assert_trace );

            reader_index++;
        }

        TKEL_DelayTask( ms2tick(kTEST_TDAL_TS_CIRCULAR_FILE_MT_READER_SLEEP_TIME_MS) );
    }

    if ( file_handle != kTDAL_FS_INVALID_HANDLE )
    {
        fs_err = TDAL_FS_Close( file_handle );
        TestManager_AssertTrue( ( fs_err == eTDAL_FS_NO_ERROR ) ,
                                "TDAL_FS_Close( circ[R] )" );
	}

    TestFSi_Sce_CircularFile_MT_vars.reader_running = FALSE;
}

void    TestFS_Sce_CircularFile_MT( void )
{
    TKEL_err        tk_err;
    TKEL_task_id    tk_writer;
    TKEL_task_id    tk_reader;

    tk_err = TKEL_Malloc( kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE ,
                (void **) &TestFSi_Sce_CircularFile_MT_vars.block_buffer );
    if ( tk_err != TKEL_NO_ERR ||
         TestFSi_Sce_CircularFile_MT_vars.block_buffer == NULL )
    {
        mTBOX_PRINT((kTBOX_LF,"Cannot allocate buffer (%s bytes) => test aborted\n",
                     kTEST_TDAL_FS_CIRCULAR_FILE_BLOCK_SIZE));
        return;
    }

	TestHelper_Configure_TDAL_FS();

    TestManager_AskForCheckingTimeout( "Press 1 to start threads (and press 1 again to stop them)", 1000 );

    TestFSi_Sce_CircularFile_MT_vars.stop_writer = FALSE;
    TestFSi_Sce_CircularFile_MT_vars.stop_reader = FALSE;
    TestFSi_Sce_CircularFile_MT_vars.reader_running = TRUE;
    TestFSi_Sce_CircularFile_MT_vars.writer_running = TRUE;
    TestFSi_Sce_CircularFile_MT_vars.nb_written = 0;

    if(TDAL_FS_FileExist(TestFS_Sce_CircularFile_filename) == eTDAL_FS_NO_ERROR)
	{
		TestManager_AssertEqual( TDAL_FS_Remove( TestFS_Sce_CircularFile_filename ) ,
								 eTDAL_FS_NO_ERROR ,
								 "TDAL_FS_Remove( circular )" );
	}

    tk_err = TKEL_CreateTask( 32 ,
                              NULL , 4096 ,
                              TestFSi_Sce_CircularFile_MT_WriterThread ,
                              NULL ,
                              &tk_writer );
    TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) ,
                            "TKEL_CreateTask( writer )" );

    tk_err = TKEL_CreateTask( 32 ,
                              NULL , 4096 ,
                              TestFSi_Sce_CircularFile_MT_ReaderThread ,
                              NULL ,
                              &tk_reader );
    TestManager_AssertTrue( ( tk_err == TKEL_NO_ERR ) ,
                            "TKEL_CreateTask( reader )" );

    TestManager_AskForCheckingTimeout( "Press 1 to stop threads", 10000 );

    TestFSi_Sce_CircularFile_MT_vars.stop_reader = TRUE;
    TestFSi_Sce_CircularFile_MT_vars.stop_writer = TRUE;

    while( TestFSi_Sce_CircularFile_MT_vars.reader_running ||
           TestFSi_Sce_CircularFile_MT_vars.writer_running )
    {
        TKEL_DelayTask( ms2tick(1000) );
    }


    TKEL_DeleteTask( tk_reader );

    TKEL_DeleteTask( tk_writer );

	TestHelper_Unconfigure_TDAL_FS();

    TKEL_Free( TestFSi_Sce_CircularFile_MT_vars.block_buffer );

}


#if defined(PACKAGE_PVR) && !defined(PVR_CORE_IMPLEMENTATION_TDAL_PVR)

/*! \fn void TestFSi_Sce_Initialize_GammaLive( int tuner )
 *  \brief initialize Gamma-LIVE stream using "tuner" parameter
 */
void    TestFSi_Sce_Initialize_GammaLive( int tuner )
{
    struct TestFSi_Sce_GammaLive_var_s *    p_vars;
    tTDAL_TSSOURCE_OpenParam                tssource_oprm;
    tTDAL_TSSOURCE_ConfigParam              tssource_cfgprm;
    tTDAL_TSSINK_OpenParam                  tssink_oprm;
    tTDAL_TSSINK_StartParam                 tssink_sprm;

    p_vars = &TestFSi_Sce_GammaLive_vars;

    p_vars->pChannel = TestHelper_GetChannel();
	
	/* Initialize tdal modules */
	TestFSi_Sce_InitializeTDALModules( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT );

	/* Initialize TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSROUTE_Init(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute init" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Init(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource init" );
	TestManager_AssertEqual(TDAL_TSSINK_Init(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink init" );
	
	TestHelper_Configure_TDAL_AVS();
	
	/* Configure TDAL_DMD and TDAL_DMX */
	TestHelper_DMDTune( tuner );
	
    TestFSi_Sce_DMX_PLAY_Configure( 0, /* AV demux */
                                       1, /* PCR demux */
                                       p_vars->pChannel->mVideoPID,
                                       p_vars->pChannel->mAudioPID,
                                       p_vars->pChannel->mPCRPID);
	
	/* Connecting Routes */
	p_vars->route_A_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
                                                   eTDAL_TS_TYPE_TUNER ,
                                                   tuner );
	p_vars->route_B_source = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE ,
                                                   eTDAL_TS_TYPE_MEMORY ,
                                                   0 );
	p_vars->route_A_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
                                                   eTDAL_TS_TYPE_DEMUX ,
                                                   1 );
	p_vars->route_B_dest   = mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION ,
                                                   eTDAL_TS_TYPE_DEMUX ,
                                                   0 );
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( p_vars->route_A_source , p_vars->route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route A" );
	
	TestManager_AssertEqual( TDAL_TSROUTE_Connect( p_vars->route_B_source , p_vars->route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute connect route B" );
	
	/* Open/Configure/Start TSSource and TSSink */
	tssource_oprm.type = eTDAL_TSSOURCE_TYPE_DEMUX;
	tssource_oprm.buffer_length = kTEST_TDAL_FS_TDAL_TSSOURCE_INTERNAL_BUFFER_SIZE;
	tssource_oprm.packet_length = kTEST_TDAL_FS_TDAL_TSSOURCE_INTERNAL_PACKET_SIZE;

	TestManager_AssertEqual( TDAL_TSSOURCE_Open( &p_vars->tssource_handle , &tssource_oprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource open source" );

    tssink_oprm.type = eTDAL_TSSINK_TYPE_DEMUX;
	/*tssink_oprm.source = */
	TestManager_AssertEqual( TDAL_TSSINK_Open( &p_vars->tssink_handle , &tssink_oprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink open sink" );
	
	tssource_cfgprm.u.dmx.demux = p_vars->route_A_dest;
	TestManager_AssertEqual( TDAL_TSSOURCE_Config( p_vars->tssource_handle , &tssource_cfgprm ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource config source" );
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( p_vars->tssource_handle , p_vars->pChannel->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add AUD pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( p_vars->tssource_handle , p_vars->pChannel->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource add VID pid" );
	if ( ( p_vars->pChannel->mPCRPID != p_vars->pChannel->mVideoPID ) &&
		 ( p_vars->pChannel->mPCRPID != p_vars->pChannel->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_AddPid( p_vars->tssource_handle , p_vars->pChannel->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource add PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSINK_SetTSSource( p_vars->tssink_handle , p_vars->tssource_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink set source" );

	TestManager_AssertEqual( TDAL_TSSOURCE_Start( p_vars->tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource start" );
	
	/* These flags are used only in ST5100 architecture */
	tssink_sprm.tv_not_radio = TRUE;
	tssink_sprm.initial_packet_number = 0;
	tssink_sprm.realtime     = TRUE;
	
	TestManager_AssertEqual( TDAL_TSSINK_Start( p_vars->tssink_handle , &tssink_sprm ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink start" );
	
	/* Demux play start filtering */
	TestFSi_Sce_DMX_PLAY_Start();

	/* Audio/Video Start/Enable SetVolume */
	TestFSi_Sce_Configure_AV();

}


/*! \fn void TestFSi_Sce_Terminate_GammaLive( void )
 *  \brief terminate Gamma-LIVE stream
 */
void    TestFSi_Sce_Terminate_GammaLive( void )
{
    struct TestFSi_Sce_GammaLive_var_s *    p_vars;

    p_vars = &TestFSi_Sce_GammaLive_vars;

	/* Audio/Video Stop */
	TestFSi_Sce_Unconfigure_AV();

	/* Demux play stop filtering */
	TestFSi_Sce_DMX_PLAY_Stop();
	
	/* Stop/Close TSSource and TSSink */
	TestManager_AssertEqual( TDAL_TSSINK_Stop( p_vars->tssink_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_Stop( p_vars->tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource stop" );
	
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( p_vars->tssource_handle , p_vars->pChannel->mVideoPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource remove VID pid" );
	TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( p_vars->tssource_handle , p_vars->pChannel->mAudioPID ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource remove AUD pid" );
	if ( ( p_vars->pChannel->mPCRPID != p_vars->pChannel->mVideoPID ) &&
		 ( p_vars->pChannel->mPCRPID != p_vars->pChannel->mAudioPID ) )
	{
		TestManager_AssertEqual( TDAL_TSSOURCE_RemovePid( p_vars->tssource_handle , p_vars->pChannel->mPCRPID ),
								 eTDAL_TSSOURCE_NO_ERROR,
								 "tssource remove PCR pid" );
	}
	
	TestManager_AssertEqual( TDAL_TSSINK_Close( p_vars->tssink_handle ),
							 eTDAL_TSSINK_NO_ERROR,
							 "tssink close" );
	TestManager_AssertEqual( TDAL_TSSOURCE_Close( p_vars->tssource_handle ),
							 eTDAL_TSSOURCE_NO_ERROR,
							 "tssource close source" );
	/* Unconfigure DMX */
	TestFSi_Sce_DMX_PLAY_Unconfigure();

	/* Disconnecting Routes */
	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( p_vars->route_A_source , p_vars->route_A_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route A" );

	TestManager_AssertEqual( TDAL_TSROUTE_Disconnect( p_vars->route_B_source , p_vars->route_B_dest ) ,
							 eTDAL_TSROUTE_NO_ERROR ,
							 "tsroute disconnect route B" );

	/* Terminate TDAL_TS modules */
	TestManager_AssertEqual(TDAL_TSSINK_Term(),
							eTDAL_TSSINK_NO_ERROR,
							"tssink terminate" );
	TestManager_AssertEqual(TDAL_TSSOURCE_Term(),
							eTDAL_TSSOURCE_NO_ERROR,
							"tssource terminate" );
	TestManager_AssertEqual(TDAL_TSROUTE_Term(),
							eTDAL_TSROUTE_NO_ERROR,
							"tsroute terminate" );
	
	/* Terminate TDAL modules */
	TestFSi_Sce_TerminateTDALModules( eDMD | eDMX | eDESC | eAV | eAVS | eDISP | eOUTPUT );
}

LOCAL   void    TestFSi_Sce_InitializeTDALModules( uint32_t mask )
{
    if ( (mask & eDMD ) != 0 )
    {
        TestHelper_Initialize_TDAL_DMD();
    }

    if ( (mask & eDMX ) != 0 )
    {
        TestManager_AssertEqual(TDAL_DMX_Init(),
                                kTDAL_DMX_NO_ERROR,
                                "dmx init");
    }

    if ( (mask & eDESC ) != 0 )
    {
      #ifdef PRODUCT_USE_CA
        TestManager_AssertEqual(TDAL_DESC_Init(),
                                kTDAL_DMX_NO_ERROR,
                                "desc init");
      #endif
    }

#ifdef AVS_STB
    if ( (mask & eAVS ) != 0 )
    {
        TestManager_AssertEqual(TDAL_AVS_Init(),
                                eTDAL_AVS_NO_ERROR,
                                "avs init");
    }
#endif

    if ( (mask & eDISP ) != 0 )
    {
        TestManager_AssertEqual(TDAL_DISP_Init(),
                                eTDAL_DISP_NO_ERROR,
                                "disp init");
    }

    if ( (mask & eOUTPUT ) != 0 )
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Init(),
                                eTDAL_OUTPUT_NO_ERROR,
                                "output init");
    }

    if ( (mask & eAV ) != 0 )
    {
        TestManager_AssertEqual(TDAL_AV_Init(),
                                eTDAL_AV_NO_ERROR,
                                "av init");
    }
}

LOCAL   void    TestFSi_Sce_TerminateTDALModules( uint32_t mask )
{
    if ( (mask & eAV ) != 0 )
    {
        TestManager_AssertEqual(TDAL_AV_Terminate(),
                                eTDAL_AV_NO_ERROR,
                                "av terminate" );
    }

    if ( (mask & eDISP ) != 0 )
    {
        TestManager_AssertEqual(TDAL_DISP_Term(),
                                eTDAL_DISP_NO_ERROR,
                                "disp terminate");
    }
    if ( (mask & eOUTPUT ) != 0 )
    {
        TestManager_AssertEqual(TDAL_OUTPUT_Term(),
                                eTDAL_OUTPUT_NO_ERROR,
                                "output terminate");
    }

#ifdef AVS_STB
    if ( (mask & eAVS ) != 0 )
    {
        TestManager_AssertEqual(TDAL_AVS_Term(),
                                eTDAL_AVS_NO_ERROR,
                                "avs terminate");
    }
#endif

    if ( (mask & eDESC ) != 0 )
    {
      #ifdef PRODUCT_USE_CA
        TestManager_AssertEqual(TDAL_DESC_Terminate(),
                                kTDAL_DMX_NO_ERROR,
                                "desc terminate");
      #endif
    }

    if ( (mask & eDMX ) != 0 )
    {
        TestManager_AssertEqual(TDAL_DMX_Terminate(),
                                kTDAL_DMX_NO_ERROR,
                                "dmx terminate");
    }

    if ( (mask & eDMD ) != 0 )
    {
        TestHelper_Terminate_TDAL_DMD();
    }
}




LOCAL   void    TestFSi_Sce_DMX_PLAY_Configure(
                    int      iDmxIdx,
                    int      iDmxIdxPcr,
                    uint16_t pid_video,
                    uint16_t pid_audio,
                    uint16_t pid_pcr
                )
{
    tTDAL_DMX_Error         dmx_err;
    tTDAL_DMX_ChannelType_t VideoType = {   eTDAL_DMX_VIDEO_STREAM,
                                            eTDAL_DMX_DECODER,
                                            eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_ChannelType_t AudioType = {   eTDAL_DMX_AUDIO_STREAM,
                                            eTDAL_DMX_DECODER,
                                            eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_ChannelType_t PcrType = {     eTDAL_DMX_PCR_STREAM,
                                            eTDAL_DMX_DECODER,
                                            eTDAL_DMX_NO_MEMORY};
    tTDAL_DMX_DemuxId       eDmxIds[eTDAL_DMX_NB_DEMUX] = { eTDAL_DMX_DEMUX0,
                                                            eTDAL_DMX_DEMUX1,
                                                            eTDAL_DMX_DEMUX2 };
    struct TestFSi_Sce_DMX_PLAY_var_s *     p_vars;

    p_vars = &TestFSi_Sce_DMX_PLAY_vars;

    TestManager_AssertGreater((double)(eTDAL_DMX_NB_DEMUX-1),
                              (double)iDmxIdx,
                              "Demux index");
    if(iDmxIdx > (eTDAL_DMX_NB_DEMUX-1))
    {
        return;
    }

    TestManager_AssertGreater((double)(eTDAL_DMX_NB_DEMUX-1),
                              (double)iDmxIdxPcr,
                              "Demux PCR index");
    if(iDmxIdxPcr > (eTDAL_DMX_NB_DEMUX-1))
    {
        return;
    }

	if (p_vars->DMX_PLAY_is_configured)
    {
		TestHelper_Unconfigure_TDAL_DMX();
    }

    /* setup video filtering */
    if (pid_video != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx],
											&VideoType,
											&p_vars->DMX_PLAY_VideoChannelId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate video channel" );

        /* set pid on the video channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(p_vars->DMX_PLAY_VideoChannelId,
										   pid_video);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx set pid on video channel");
        mTBOX_PRINT((kTBOX_LF, "pid on video channel = %d\n", pid_video));

        /* allocate a filter on the video channel */
        dmx_err = TDAL_DMX_Allocate_Filter(p_vars->DMX_PLAY_VideoChannelId,
										   &p_vars->DMX_PLAY_VideoFilterId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate video filter");

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(p_vars->DMX_PLAY_VideoFilterId,
										  eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx control video filter" );
    }

    /* setup audio filtering */
	if (pid_audio != 0)
	{
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdx],
											&AudioType,
											&p_vars->DMX_PLAY_AudioChannelId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate audio channel" );

        /* set pid on the Audio channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(p_vars->DMX_PLAY_AudioChannelId,
										   pid_audio);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx set pid audio channel");
        mTBOX_PRINT((kTBOX_LF, "pid on audio channel = %d\n", pid_audio));

        /* allocate a filter on the Audio channel */
        dmx_err = TDAL_DMX_Allocate_Filter(p_vars->DMX_PLAY_AudioChannelId,
										   &p_vars->DMX_PLAY_AudioFilterId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate audio filter" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(p_vars->DMX_PLAY_AudioFilterId,
										  eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx control enable audio filter");
  }

    /* setup pcr filtering */
    if (pid_pcr != 0)
    {
        dmx_err = TDAL_DMX_Allocate_Channel(eDmxIds[iDmxIdxPcr],
											&PcrType,
                                            &p_vars->DMX_PLAY_PcrChannelId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate channel");

        /* set pid on the Pcr channel */
        dmx_err = TDAL_DMX_Set_Channel_PID(p_vars->DMX_PLAY_PcrChannelId,
										   pid_pcr);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx set channel pcr pid");
        mTBOX_PRINT((kTBOX_LF, "pid on pcr channel = %d\n", pid_pcr));

        /* allocate a filter on the Pcr channel */
        dmx_err = TDAL_DMX_Allocate_Filter(p_vars->DMX_PLAY_PcrChannelId,
										   &p_vars->DMX_PLAY_PcrFilterId);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx allocate pcr filter" );

        /* start the filter */
        dmx_err = TDAL_DMX_Control_Filter(p_vars->DMX_PLAY_PcrFilterId,
										  eTDAL_DMX_CTRL_ENABLE);
        TestManager_AssertEqual(dmx_err,
								kTDAL_DMX_NO_ERROR,
								"dmx enable dmx pcr filter" );
    }

    p_vars->DMX_PLAY_is_configured = true;
}

LOCAL   void    TestFSi_Sce_DMX_PLAY_Unconfigure( void )
{
    tTDAL_DMX_Error                         dmx_err;
    struct TestFSi_Sce_DMX_PLAY_var_s *     p_vars;

    p_vars = &TestFSi_Sce_DMX_PLAY_vars;

    if (!p_vars->DMX_PLAY_is_configured)
    {
            return;
    }

    /* free audio channel */
    dmx_err = TDAL_DMX_Free_Channel(p_vars->DMX_PLAY_AudioChannelId);
    TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx free audio channel");

    /* free video channel */
    dmx_err = TDAL_DMX_Free_Channel(p_vars->DMX_PLAY_VideoChannelId);
    TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx free video channel");

    /* free pcr channel */
    dmx_err = TDAL_DMX_Free_Channel(p_vars->DMX_PLAY_PcrChannelId);
    TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx pcr video channel");

    p_vars->DMX_PLAY_is_configured = false;
}

LOCAL   void    TestFSi_Sce_DMX_PLAY_Start( void )
{
    tTDAL_DMX_Error                         dmx_err;
    struct TestFSi_Sce_DMX_PLAY_var_s *     p_vars;

    p_vars = &TestFSi_Sce_DMX_PLAY_vars;

	if (!p_vars->DMX_PLAY_is_configured)
    {
		return;
    }

	/* start the video channel */
	dmx_err = TDAL_DMX_Control_Channel(p_vars->DMX_PLAY_VideoChannelId,
									   eTDAL_DMX_CTRL_ENABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control video channel" );

	/* start the PCR channel */
	dmx_err = TDAL_DMX_Control_Channel(p_vars->DMX_PLAY_PcrChannelId,
									   eTDAL_DMX_CTRL_ENABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx enable dmx pcr channel");

	/* start the audio channel */
	dmx_err = TDAL_DMX_Control_Channel(p_vars->DMX_PLAY_AudioChannelId,
									   eTDAL_DMX_CTRL_ENABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control enable audio channel");

}

void TestFSi_Sce_DMX_PLAY_Stop( void )
{
    tTDAL_DMX_Error                         dmx_err;
    struct TestFSi_Sce_DMX_PLAY_var_s *     p_vars;

    p_vars = &TestFSi_Sce_DMX_PLAY_vars;

    if (!p_vars->DMX_PLAY_is_configured)
    {
		return;
    }

	/* start the video channel */
	dmx_err = TDAL_DMX_Control_Channel(p_vars->DMX_PLAY_VideoChannelId,
									   eTDAL_DMX_CTRL_DISABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control video channel");

	/* start the audio channel */
	dmx_err = TDAL_DMX_Control_Channel(p_vars->DMX_PLAY_AudioChannelId,
									   eTDAL_DMX_CTRL_DISABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx control enable audio channel");

	/* start the PCR channel */
	dmx_err = TDAL_DMX_Control_Channel(p_vars->DMX_PLAY_PcrChannelId,
									   eTDAL_DMX_CTRL_DISABLE);
	TestManager_AssertEqual(dmx_err,
							kTDAL_DMX_NO_ERROR,
							"dmx enable dmx pcr channel");

}





LOCAL   void    TestFSi_Sce_Configure_AV( void )
{
	tTDAL_AV_Decoder                        tdalAvDecoderVideo;
	tTDAL_AV_StreamType                     tdalAvStreamTypeVideo;
	tTDAL_AV_Decoder                        tdalAvDecoderAudio;
	tTDAL_AV_StreamType                     tdalAvStreamTypeAudio;
	tTDAL_OUTPUT_AudioAnaCapability         stAudioAnaCapability;
	tTDAL_OUTPUT_AudioAnaSpeakerAttenuation stAudioAnaSpeakerAttenuation;

	tdalAvDecoderVideo              = eTDAL_AV_DECODER_VIDEO_1;
	tdalAvStreamTypeVideo.videoType = eTDAL_AV_VIDEO_TYPE_MPEG2;

    TestManager_AssertEqual(TDAL_AV_Start(tdalAvDecoderVideo,
										  tdalAvStreamTypeVideo),
							eTDAL_AV_NO_ERROR,
							"av start video");

    TestManager_AssertEqual( TDAL_DISP_LayerEnable(eTDAL_DISP_LAYER_VIDEO_ID_0),
							 eTDAL_DISP_NO_ERROR,
							 "disp layer enable" );

	tdalAvDecoderAudio              = eTDAL_AV_DECODER_AUDIO_1;
	tdalAvStreamTypeAudio.audioType = eTDAL_AV_AUDIO_TYPE_MPEG;
	
	TestManager_AssertEqual(TDAL_AV_Start(tdalAvDecoderAudio,
										  tdalAvStreamTypeAudio),
							eTDAL_AV_NO_ERROR,
							"av start audio");
    TestManager_AssertEqual( TDAL_OUTPUT_Enable(eTDAL_OUTPUT_AUDIO_ANA_ID_0),
							 eTDAL_OUTPUT_NO_ERROR,
							 "output audio enable" );
    TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaCapabilityGet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
															  &stAudioAnaCapability),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio capability get");

	stAudioAnaSpeakerAttenuation.FrontRLAttenuation     = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.FrontCenterAttenuation = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.SurroundRLAttenuation  = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.LFEAttenuation         = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.RearRLAttenuation      = stAudioAnaCapability.AttenuationMax;
	stAudioAnaSpeakerAttenuation.RearCenterAttenuation  = stAudioAnaCapability.AttenuationMax;

	TestManager_AssertEqual(TDAL_OUTPUT_AudioAnaSpeakerAttenuationSet(eTDAL_OUTPUT_AUDIO_ANA_ID_0,
																	  &stAudioAnaSpeakerAttenuation),
							eTDAL_OUTPUT_NO_ERROR,
							"output audio volume set");
}

LOCAL   void    TestFSi_Sce_Unconfigure_AV( void )
{
	tTDAL_AV_Decoder tdalAvDecoderVideo;
	tTDAL_AV_Decoder tdalAvDecoderAudio;

	tdalAvDecoderVideo = eTDAL_AV_DECODER_VIDEO_1;
	tdalAvDecoderAudio = eTDAL_AV_DECODER_AUDIO_1;

    TestManager_AssertEqual(TDAL_AV_Stop(tdalAvDecoderVideo),
							eTDAL_AV_NO_ERROR,
							"av stop video");
    TestManager_AssertEqual(TDAL_AV_Stop(tdalAvDecoderAudio),
							eTDAL_AV_NO_ERROR,
							"av stop audio");
}

#endif /* PACKAGE_PVR */

#endif /* USE_TDAL_FS */

