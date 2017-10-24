/** \file fs_test_sce.c
 *  \brief Scenario Tests for the FS layer of the TDAL API
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
void    TestInfo_FS_Info( void );
void    TestInfo_FS_MountCheckFormat( void );

static tTestNode gTestInfo_Info = {
	"TestInfo_Info",
	"Info test",
	0,
	TestInfo_FS_Info,
	NULL
};

static tTestNode gTestInfo_MountCheckFormat = {
	"TestInfo_MountCheckFormat",
	"MountCheckFormat test",
	0,
	TestInfo_FS_MountCheckFormat,
	NULL
};


tTestNode* gFSInfoTestNodeList[] = {
	&gTestInfo_Info,
	&gTestInfo_MountCheckFormat,
	NULL
};

tTestNode gTDAL_FS_InfoTestSuite = {
	"TDAL_FS Info Tests",
	"test TDAL_FS Info",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gFSInfoTestNodeList
};




/******************************************************
 *              Defines                               *
 ******************************************************/


/******************************************************
 *              Enums / Typedefs / Structures         *
 ******************************************************/


/******************************************************
 *              Local variables (LOCAL)               *
 ******************************************************/


/******************************************************
 *              Functions declarations                *
 ******************************************************/


/*! \fn void TestInfo_FS_Info( void )
 *  \brief
 */
void TestInfo_FS_Info(void)
{
    tTDAL_FS_FsInfo fs_info;
    tTDAL_FS_Error  fs_err;
    uint32_t        uiPercent;
    TestHelper_Configure_TDAL_FS();


    fs_err = TDAL_FS_GetFsInfo( FS_HDD_LOCATION , &fs_info );
    TestManager_AssertEqual( fs_err ,
                             eTDAL_FS_NO_ERROR ,
                             "fs get_info" );
    if ( fs_err == eTDAL_FS_NO_ERROR )
    {
        mTBOX_PRINT((kTBOX_LF,"\n"));
        mTBOX_PRINT((kTBOX_LF,"Information about \""FS_HDD_LOCATION"\"\n"));
        mTBOX_PRINT((kTBOX_LF,"    .freeBlocks:      %d\n", fs_info.freeBlocks ));
        mTBOX_PRINT((kTBOX_LF,"    .allocationUnit:  %d\n", fs_info.allocationUnit ));
        mTBOX_PRINT((kTBOX_LF,"    .totalSize:       %d\n", fs_info.totalSize ));
        mTBOX_PRINT((kTBOX_LF,"\n"));

        uiPercent = ( ( fs_info.freeBlocks / 1024 ) * 100 )
                  / ( fs_info.totalSize / 1024 );
        mTBOX_PRINT((kTBOX_LF,"Available space is %d %%\n", uiPercent));
        mTBOX_PRINT((kTBOX_LF,"\n"));
    }

    TestHelper_Unconfigure_TDAL_FS();
}



/*! \fn void TestInfo_FS_MountCheckFormat( void )
 *  \brief
 */
void TestInfo_FS_MountCheckFormat(void)
{
    tTDAL_FS_Error          fs_err;
    tTDAL_FS_VolumeHandle   fs_volume;
    bool                    call_mount_again = FALSE;
    bool                    call_umount = FALSE;

    TestHelper_AskForVolume_TDAL_FS( &fs_volume );


    fs_err = TDAL_FS_Mount( fs_volume , FS_HDD_LOCATION , kTDAL_FS_MODE_RDWR );
    if ( fs_err == eTDAL_FS_NO_ERROR )
    {
        mTBOX_PRINT((kTBOX_LF,"Mount succeed\n"));
        call_umount = TRUE;
    }
    else if ( fs_err == eTDAL_FS_ERROR_CORRUPTED )
    {
        mTBOX_PRINT((kTBOX_LF,"Mount failed - corrupted\n"));

        fs_err = TDAL_FS_Check( fs_volume );
        if ( fs_err == eTDAL_FS_NO_ERROR )
        {
            mTBOX_PRINT((kTBOX_LF,"Check succeed\n"));
            call_mount_again = TRUE;
        }
        else if ( fs_err == eTDAL_FS_ERROR_CORRUPTED )
        {
            mTBOX_PRINT((kTBOX_LF,"Check failed - corrupted\n"));

        }
        else
        {
            mTBOX_PRINT((kTBOX_LF,"Check failed - fs_err=%d\n", fs_err));
        }
    }
    else
    {
        mTBOX_PRINT((kTBOX_LF,"Mount failed - fs_err=%d\n", fs_err));
    }



    if ( call_mount_again == TRUE )
    {
        fs_err = TDAL_FS_Mount( fs_volume , FS_HDD_LOCATION , kTDAL_FS_MODE_RDWR );
        if ( fs_err != eTDAL_FS_NO_ERROR )
        {
            mTBOX_PRINT((kTBOX_LF,"Mount(again) failed\n"));
        }
        else
        {
            call_umount = TRUE;
        }
    }


    if ( call_umount == TRUE )
    {
        fs_err = TDAL_FS_Umount( FS_HDD_LOCATION );
        if ( fs_err != eTDAL_FS_NO_ERROR )
        {
            mTBOX_PRINT((kTBOX_LF,"Umount(again) failed\n"));
        }
    }

}

#endif /* USE_TDAL_FS */

