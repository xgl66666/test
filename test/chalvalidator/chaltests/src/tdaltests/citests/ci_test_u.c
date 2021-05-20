/*****************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_CI_TESTS
 *
 * FILE NAME: $URL  : $
 *            $Rev  : $
 *            $Date : $
 *
 * Description :
 * Target : All
 * Author : Erwan DOCEUX (edo).
 * Creation : 29/11/2004.
 *
 ****************************************************************************/

#include <string.h>
#include <stdio.h>


#include "tkel.h"

#include "crules.h"
#include "tdal_ci_test.h"
#include "tdal_ci_test_p.h"
#include "tdal_ci.h"
#include "tbox.h"

/*! \include testhelper.h */
#include "testhelper.h"
#include "TestManager.h"
#include "citests.h"




/********************************************************/
/*              Macros                                  */
/********************************************************/

#define MILLI_DELAY(_ms_) {TKEL_DelayTask(ms2tick (_ms_));}

/*------------------------------*/
/* Test functions prototypes    */
/*------------------------------*/
static uint32_t ci_u1(void);
static uint32_t ci_u2(void);
static uint32_t ci_u3(void);
static uint32_t ci_sce1(void);
static uint32_t ci_sce2(void);

/*------------------------------*/
/* Static variables definitions    */
/*------------------------------*/

static t_testEntry ci_test_unitaire[NB_TEST_UNITAIRE] =
{
    { ci_u1, "ci_u1","\n\rTest OK"},
    { ci_u2, "ci_u2","\n\rTest OK"},
    { ci_u3, "ci_u3","\n\rTest OK"}
};

static t_testEntry ci_test_typical_scenario[NB_TEST_TYPICAL_SCENARIO] =
{
    { ci_sce1, "ci_control_filter_sce1","\n\rTest OK"},
    { ci_sce2, "ci_control_filter_sce2","\n\rTest OK"}
};

static t_testFamily ciTestFamily[kTDAL_NB_FAMILY] =
{
    { ci_test_unitaire, NB_TEST_UNITAIRE, "01", "test unitaire de l'API TDAL CI", ""},
    { ci_test_typical_scenario, NB_TEST_TYPICAL_SCENARIO, "02", "test l'ordre d'appel des fonctions", ""}
};

static t_testModule ciTestModule =
{
    ciTestFamily,kTDAL_NB_FAMILY,"MODULE TDAL CI","Launch ci test application"
};


/*----------------------------------*/
/* internal function prototypes        */
/*----------------------------------*/

static void testPerformModuleTest(t_testModule *ciTestModule);
static void testPerformFamilyTest(t_testEntry *ciTest,uint32_t nbtest);


void TDAL_CI_test(void)
{
    testPerformModuleTest(&ciTestModule);
}

void TDAL_CI_unitary_test(void)
{
    testPerformFamilyTest((t_testEntry *)&ci_test_unitaire,(uint32_t)NB_TEST_UNITAIRE);
}


void TDAL_CI_scenario_test(void)
{
    testPerformFamilyTest((t_testEntry *)&ci_test_typical_scenario,(uint32_t)NB_TEST_TYPICAL_SCENARIO);
}


/*------------------------------------*/
/*-------Test Node---------------------*/
/*------------------------------------*/

static tTestNode gCI_TEST_unitary= {
    "MODULE TDAL unitary",
    "Launch ci unitary test application",
    0, /* not interactive */
    TDAL_CI_unitary_test,
        NULL
};

static tTestNode gCI_TEST_scenario= {
    "MODULE TDAL scenario  CI",
    "Launch ci scenario  test application",
    0, /* not interactive */
    TDAL_CI_scenario_test,
        NULL
};

/*---------------------------------*/
/*--------Node list------------------*/
/*---------------------------------*/
tTestNode* gTDAL_CI_ScenarioTestNodeList[] = {

    &gCI_TEST_scenario,
    NULL
};




tTestNode* gTDAL_CI_UnitaryTestNodeList[] = {

    &gCI_TEST_unitary,
    NULL
};




tTestNode gTDAL_CI_UnitaryTestSuite = {
    "TDAL_CI Unitary Test Suite",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_CI_UnitaryTestNodeList
};


tTestNode gTDAL_CI_ScenarioTestSuite = {
    "TDAL_CI Scenarios Test Suite",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_CI_ScenarioTestNodeList
};






/* ------------------------------------------------------------------- */
/*                    testPerformModuleTest                            */
/* ------------------------------------------------------------------- */
static void testPerformModuleTest(t_testModule *TestModule)
{
    int i, j;

    int enableFamily;

    for (i = 0; i<kTDAL_NB_FAMILY ; i++)
    {

        enableFamily = 1;

        for (j = 0; j<ciTestFamily[i].nbTest && enableFamily; j++)
        {

          ciTestFamily[i].testList[j].pFunc() ;
            TKEL_DelayTask(200);
        }


    }

}


/* --------------------------------------------------------------------- */
/*                        testPerformFamilyTest                          */
/* --------------------------------------------------------------------- */
static void testPerformFamilyTest(t_testEntry *ciTest,uint32_t nbtest)
{
    unsigned int i;

    for (i = 0; i<nbtest ; i++)
    {
       ciTest[i].pFunc() ;
       TKEL_DelayTask(200);
    }


}


/*****************************************************************************
*                                                                            *
*                    PART ONE : UNITARY TESTS                                *
*                                                                            *
*****************************************************************************/

static uint32_t ci_u1(void)
{

    TestManager_AssertEqual( TDAL_CI_Init(), eTDAL_CI_NO_ERROR, "TDAL_CI_Init()" );
   	TestManager_AssertEqual( TDAL_CI_Terminate(), eTDAL_CI_NO_ERROR, "TDAL_CI_Terminate()" );
	TestManager_AssertEqual( TDAL_CI_Init(), eTDAL_CI_NO_ERROR, "TDAL_CI_Init()" );
	TestManager_AssertEqual( TDAL_CI_Terminate(), eTDAL_CI_NO_ERROR, "TDAL_CI_Terminate()" );

    return (kTESTi_PASSED);
}

static uint32_t ci_u2(void)
{
    short devId, devId2;
    unsigned short     nbMaxSlot;

   TestManager_AssertEqual( TDAL_CI_Init(), eTDAL_CI_NO_ERROR, "TDAL_CI_Init()" );

   TDAL_CI_CapabilitiesGet(&nbMaxSlot);

   TestManager_AssertEqual( TDAL_CI_Open("PCMCIA_Slot1", &devId), eTDAL_CI_NO_ERROR, "TDAL_CI_Open()" );


   if(nbMaxSlot == 2)
   {
    TestManager_AssertEqual( TDAL_CI_Open("PCMCIA_Slot1", &devId2), eTDAL_CI_NO_ERROR, "TDAL_CI_Open()" );
   }
   else
   {
    TestManager_AssertEqual( TDAL_CI_Open("PCMCIA_Slot1", &devId2), eTDAL_CI_ERROR, "TDAL_CI_Open()" );
   }
   TestManager_AssertEqual( TDAL_CI_Close(devId), eTDAL_CI_NO_ERROR, "TDAL_CI_Close()" );
   TestManager_AssertEqual( TDAL_CI_Terminate(), eTDAL_CI_NO_ERROR, "TDAL_CI_Terminate()" );

   return (kTESTi_PASSED);
}

static uint32_t ci_u3(void)
{
    short               devId1, devId2;
    unsigned char       i, ioValue1, ioValue2;
    unsigned short      nbMaxSlot;

    TestManager_AssertEqual( TDAL_CI_Init(), eTDAL_CI_NO_ERROR, "TDAL_CI_Init()" );

    TDAL_CI_CapabilitiesGet(&nbMaxSlot);
    TestManager_AssertEqual( TDAL_CI_Open("PCMCIA_Slot1", &devId1), eTDAL_CI_NO_ERROR, "TDAL_CI_Open()" );

    if(nbMaxSlot == 2)
    {
        TestManager_AssertEqual( TDAL_CI_Open("PCMCIA_Slot1", &devId2), eTDAL_CI_NO_ERROR, "TDAL_CI_Open()" );
    }

    for(i=0; i<101 ; i++)
    {
        TestManager_AssertEqual( TDAL_CI_Ctl(devId1, eTDAL_CI_RD_DETECT, &ioValue1), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(DETECT)" );
        mTBOX_PRINT((kTBOX_LF,"CI_Ctl(devId1)-> CardDetect(%d) %d/100\n",ioValue1, i ));
        if(nbMaxSlot == 2)
        {
            mTBOX_PRINT((kTBOX_LF,"CI_Ctl(devId2)-> %d/100\n",i ));
            TestManager_AssertEqual( TDAL_CI_Ctl(devId2, eTDAL_CI_RD_DETECT, &ioValue2), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(DETECT)" );
        }
        MILLI_DELAY(500);
    }

    TestManager_AssertEqual( TDAL_CI_Close(devId1), eTDAL_CI_NO_ERROR, "TDAL_CI_Close()" );
    if(nbMaxSlot == 2)
    {
        TestManager_AssertEqual( TDAL_CI_Close(devId2), eTDAL_CI_NO_ERROR, "TDAL_CI_Close()" );
    }

    TestManager_AssertEqual( TDAL_CI_Terminate(), eTDAL_CI_NO_ERROR, "TDAL_CI_Terminate()" );

    return (kTESTi_PASSED);
}


/************************************************************************************
*                                                                                   *
*                        PART TWO : TYPICAL SCENARIOS                               *
*                                                                                   *
************************************************************************************/

static uint32_t ci_sce1(void)
{
    short devId;


    unsigned char ioValue;
    unsigned short i, negoBufferSizeLength;

    unsigned char negoBufferSize[2];
    unsigned char Create_T_C[5] =     {0x01, 0x00, 0x82, 0x01, 0x01};
    unsigned char Create_NTD[5] =     {0x01, 0x00, 0xA0, 0x01, 0x01};
    unsigned char Create_T_C_Reply[50];
    unsigned short Create_T_C_ReplyLength;
    unsigned char bp = 0;
    unsigned int  counter = 0;

    TestManager_AssertEqual( TDAL_CI_Init(), eTDAL_CI_NO_ERROR, "TDAL_CI_Init()" );
    TestManager_AssertEqual( TDAL_CI_Open("PCMCIA_Slot1", &devId), eTDAL_CI_NO_ERROR, "TDAL_CI_Open()" );

    for(i=0; i<1001 ; i++)
    {
        TestManager_AssertEqual( TDAL_CI_Ctl(devId, eTDAL_CI_RD_DETECT, &ioValue), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(DETECT)" );
        mTBOX_PRINT((kTBOX_LF,"CI_Ctl(devId1)-> CardDetect(%d) %d/1000\n",ioValue, i ));
        if(ioValue != 0)
        {
            break;
        }
        MILLI_DELAY(10);
    }

     MILLI_DELAY(300);
/*
     ioValue = 0;
     TestManager_AssertEqual( TDAL_CI_Ctl(devId, eTDAL_CI_WR_RESET, &ioValue), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl()" );
*/

	MILLI_DELAY(10);
    ioValue = 1;
    TestManager_AssertEqual( TDAL_CI_Ctl(devId, eTDAL_CI_WR_RESET, &ioValue), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(RESET_ON)" );


	MILLI_DELAY(10); /* real time needed is 11 us */
    ioValue = 0;
    TestManager_AssertEqual( TDAL_CI_Ctl(devId, eTDAL_CI_WR_RESET, &ioValue), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(RESET_OFF)" );



    /* delay of waiting readybusy is limited to 5 seconds */
    /* the ready busy stays to 1 from insertion to end of RESET (at 1)
        and stays at 0 after, when RESET is cleared */
    for(i=0; i<50 ; i++)
    {
       TestManager_AssertEqual(  TDAL_CI_Ctl(devId, eTDAL_CI_RD_RDYBSY, &ioValue), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(RDYBSY)" );
       mTBOX_PRINT((kTBOX_LF,"CI_Ctl(devId1)-> busy(%d) %d/49\n",ioValue, i ));
       if(ioValue !=  0)
        {
            break;
        }
        MILLI_DELAY(100);
    }

    MILLI_DELAY(1000);
    TestManager_AssertEqual(  TDAL_CI_CheckCIS(devId), eTDAL_CI_NO_ERROR, "TDAL_CI_CheckCIS()" );

    TestManager_AssertEqual(  TDAL_CI_WriteCOR(devId), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCOR()" );

    TestManager_AssertEqual(  TDAL_CI_WriteCmd(devId, kTDAL_CI_RS, 1), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCmd(bit RS to 1)" );
    MILLI_DELAY(10);
    TestManager_AssertEqual(  TDAL_CI_WriteCmd(devId, kTDAL_CI_RS, 0), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCmd(bit RS to 0)" );

    for(i=0; i<100 ; i++)
    {
        TestManager_AssertEqual(   TDAL_CI_ReadStatus(devId, kTDAL_CI_FR, &ioValue), eTDAL_CI_NO_ERROR, "TDAL_CI_ReadStatus(bit FR)" );
        mTBOX_PRINT((kTBOX_LF,"ReadStatus-> busy(%d) %d/99\n",ioValue, i ));
        if(ioValue != 0)
        {
            break;
        }
        MILLI_DELAY(10);
    }

    TestManager_AssertEqual(  TDAL_CI_WriteCmd(devId, kTDAL_CI_SR, 1), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCmd(bit SR to 1)" );
    MILLI_DELAY(1000);
    TestManager_AssertEqual(  TDAL_CI_Read(devId, &negoBufferSizeLength, negoBufferSize), eTDAL_CI_NO_ERROR, "TDAL_CI_Read()" );

    ioValue = negoBufferSize[1] | (negoBufferSize[0] << 8);
    mTBOX_PRINT((kTBOX_LF,"negoBufferSize=%d\n", ioValue ));

    MILLI_DELAY(100);
    TestManager_AssertEqual(  TDAL_CI_WriteCmd(devId, kTDAL_CI_SR, 0), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCmd(bit SR to 0)" );

    MILLI_DELAY(100);
    TestManager_AssertEqual(  TDAL_CI_WriteCmd(devId, kTDAL_CI_SW, 1), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCmd(bit SW to 1)" );


    MILLI_DELAY(100);
    negoBufferSize[0] = ioValue >> 8;
    negoBufferSize[1] = ioValue & 0x00FF;

    TestManager_AssertEqual(  TDAL_CI_Write(devId, negoBufferSizeLength, negoBufferSize), eTDAL_CI_NO_ERROR, "TDAL_CI_Write()" );

    MILLI_DELAY(100);
    TestManager_AssertEqual(  TDAL_CI_WriteCmd(devId, kTDAL_CI_SW, 0), eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCmd(bit SW to 0)" );

    MILLI_DELAY(100);
    TestManager_AssertEqual( TDAL_CI_Ctl(devId, eTDAL_CI_WR_BYPASS, &bp), eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl(BYPASS)" );
    MILLI_DELAY(100);


#if 1 /* add a read as CI stack do */
    Create_T_C_ReplyLength = sizeof(Create_T_C_Reply);
    TestManager_AssertEqual( TDAL_CI_Read(devId, &Create_T_C_ReplyLength, Create_T_C_Reply), eTDAL_CI_NO_ERROR, "TDAL_CI_Read()" );
    MILLI_DELAY(200);
#endif
    TestManager_AssertEqual( TDAL_CI_Write(devId, sizeof(Create_T_C), Create_T_C), eTDAL_CI_NO_ERROR, "TDAL_CI_Write()" );

    MILLI_DELAY(200);
    Create_T_C_ReplyLength = sizeof(Create_T_C_Reply);
    TestManager_AssertEqual( TDAL_CI_Read(devId, &Create_T_C_ReplyLength, Create_T_C_Reply), eTDAL_CI_NO_ERROR, "TDAL_CI_Read()" );
    for(i=0 ; i<Create_T_C_ReplyLength; i++)
    {
        mTBOX_PRINT((kTBOX_LF,"0x%02x ", Create_T_C_Reply[i]));
    }
    mTBOX_PRINT((kTBOX_LF,"\n"));

    while(counter < 500)
    {
        mTBOX_PRINT((kTBOX_LF,"Write = "));
        for(i=0 ; i<5; i++)
        {
            mTBOX_PRINT((kTBOX_LF,"0x%02x ", Create_NTD[i]));
        }
        mTBOX_PRINT((kTBOX_LF,"\n"));
        TestManager_AssertEqual( TDAL_CI_Write(devId, sizeof(Create_NTD), Create_NTD), eTDAL_CI_NO_ERROR, "TDAL_CI_Write()" );
        MILLI_DELAY(300);

        TestManager_AssertEqual( TDAL_CI_Read(devId, &Create_T_C_ReplyLength, Create_T_C_Reply), eTDAL_CI_NO_ERROR, "TDAL_CI_Read()" );

        mTBOX_PRINT((kTBOX_LF,"Read = "));
        for(i=0 ; i<Create_T_C_ReplyLength; i++)
        {
            mTBOX_PRINT((kTBOX_LF,"0x%02x ", Create_T_C_Reply[i]));
        }
        mTBOX_PRINT((kTBOX_LF,"\n"));
        counter++;
        mTBOX_PRINT((kTBOX_LF,"counter=%d/500\n",counter));
        mTBOX_PRINT((kTBOX_LF,"\n"));
    }


    TestManager_AssertEqual(  TDAL_CI_Close(devId), eTDAL_CI_NO_ERROR, "TDAL_CI_Close()" );
    TestManager_AssertEqual(  TDAL_CI_Terminate(), eTDAL_CI_NO_ERROR, "TDAL_CI_Terminate()" );

    return (kTESTi_PASSED);
}

static uint32_t ci_sce2(void)
{
    return (kTESTi_PASSED);
}

