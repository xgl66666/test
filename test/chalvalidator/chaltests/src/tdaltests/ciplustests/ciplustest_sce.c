/*****************************************************************************
 * COPYRIGHT 2008 IWEDIA                                                     *
 *****************************************************************************
 *
 * MODULE NAME: CI+ Stack TDAL Testu
 *
 * FILE NAME: $URL$
 *            $Rev$
 *            $Date$
 *
 * DESCRIPTION:
 *    TDAL CI+ testu
 *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "tkel.h"

#include "TestManager.h"

#include "tdal_ci.h"

void cbDrvOnCardMovement(unsigned short devId);
void cbDrvOnModuleInterrupt(unsigned short devId ,tTDAL_CI_IntCause cause);
void cbDrvOnIirSet(unsigned short devId);
int  CI_testU_poll(unsigned short devId, unsigned long nb_Iteration);
unsigned char CI_testU_wait_StatusBit(unsigned short devId, unsigned char statusBit);

void CI_Mod_Open(void);
void CI_Callback_Register(void);
void CI_Mod_Init(void);
void CI_Mod_Close(void);
void CI_Hard_Reset(void);
void CI_Check_CIS(void);
void CI_Write_COR(void);
void CI_Data_Channel_Soft_Reset(void);
void CI_Data_Channel_Read_Size(void);
void CI_Data_Channel_Write_Size(void);
void CI_Enable_Bypass(void);
void CI_Write_Create_TC(void);
void CI_Read_CTC_Reply(void);
void CI_Disable_Bypass(void);

unsigned short TDAL_CI_devId;
/*------------------------------*/
/* Menus managment definitions	*/
/*------------------------------*/
static tTestNode gTestCI_Mod_Open = {
    "Open slot one (must be done first)",
    "",
    0, /* not interactive */
    CI_Mod_Open,
    NULL
};

static tTestNode gTestCI_Callback_Register = {
    "Register callback functions",
    "",
    0, /* not interactive */
    CI_Callback_Register,
    NULL
};

static tTestNode gTestMod_Init = {
    "Full module initialization sequence",
    "",
    0, /* not interactive */
    CI_Mod_Init,
    NULL
};

static tTestNode gTestCI_Mod_Close = {
    "Close slot one",
    "",
    0, /* not interactive */
    CI_Mod_Close,
    NULL
};

tTestNode* gTDAL_CIPLUSautoMenu[] = {
    &gTestCI_Mod_Open,
    &gTestCI_Callback_Register,
    &gTestMod_Init,
    &gTestCI_Mod_Close,
    NULL
};

tTestNode gTDAL_CIPLUSauto = {
    "TDAL_CI automatic",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_CIPLUSautoMenu
};

static tTestNode gTestCI_Hard_Reset = {
    "Module hard reset",
    "",
    0, /* not interactive */
    CI_Hard_Reset,
    NULL
};

static tTestNode gTestCI_Check_CIS = {
    "Check CIS",
    "",
    0, /* not interactive */
    CI_Check_CIS,
    NULL
};

static tTestNode gTestCI_Write_COR = {
    "Write COR",
    "",
    0, /* not interactive */
    CI_Write_COR,
    NULL
};

static tTestNode gTestCI_Data_Channel_Soft_Reset = {
    "Data channel initialization",
    "",
    0, /* not interactive */
    CI_Data_Channel_Soft_Reset,
    NULL
};

static tTestNode gTestCI_Data_Channel_Read_Size = {
    "Data channel buffer size reading",
    "",
    0, /* not interactive */
    CI_Data_Channel_Read_Size,
    NULL
};

static tTestNode gTestCI_Data_Channel_Write_Size = {
    "Data channel buffer size writing",
    "",
    0, /* not interactive */
    CI_Data_Channel_Write_Size,
    NULL
};

static tTestNode gTestCI_Enable_Bypass = {
    "Enable Bypass",
    "",
    0, /* not interactive */
    CI_Enable_Bypass,
    NULL
};

static tTestNode gTestCI_Write_Create_TC = {
    "Write Create TC",
    "",
    0, /* not interactive */
    CI_Write_Create_TC,
    NULL
};

static tTestNode gTestCI_Read_CTC_Reply = {
    "Read CTC Reply",
    "",
    0, /* not interactive */
    CI_Read_CTC_Reply,
    NULL
};

static tTestNode gTestCI_Disable_Bypass = {
    "Disable Bypass",
    "",
    0, /* not interactive */
    CI_Disable_Bypass,
    NULL
};

tTestNode* gTDAL_CIPLUSstepbystepMenu[] = {
    &gTestCI_Mod_Open,
    &gTestCI_Callback_Register,
    &gTestCI_Hard_Reset,
    &gTestCI_Check_CIS,
	&gTestCI_Write_COR,
	&gTestCI_Data_Channel_Soft_Reset,
	&gTestCI_Data_Channel_Read_Size,
	&gTestCI_Data_Channel_Write_Size,
	&gTestCI_Enable_Bypass,
	&gTestCI_Write_Create_TC,
	&gTestCI_Read_CTC_Reply,
	&gTestCI_Disable_Bypass,
    &gTestCI_Mod_Close,
    NULL
};

tTestNode gTDAL_CIPLUSstepbystep = {
    "TDAL_CI step by step",
    "test the av module",
    0, /* not interactive */
    NULL, /* no function, it's a test suite */
    (void**)gTDAL_CIPLUSstepbystepMenu
};

static unsigned char CI_testU_buffer[1024]; /* for all TDAL CI read call */
static unsigned short CI_testU_bufferSize[2]; /* negociated physical buffer size for each slot */
static unsigned char Create_T_C[] = 	{0x01, 0x00, 0x82, 0x01, 0x01 };
static unsigned char T_DataLastPoll[] = {0x01, 0x00, 0xA0, 0x01, 0x01 };
static unsigned char T_RCV[] = {0x01, 0x00, 0x81, 0x01, 0x01 };
static unsigned char OpenSessionResp[] = {0x01,0x00,0xa0,0x0a,0x01,0x92,0x07,0x00,0x00,0x01,0x00,0x41,0x00,0x01};
static unsigned short CI_testU_bufferLength = 0;


/* ============================================================================
	Function :	cbDrvOnCardMovement
				driver callback OnCardMovement (one callback for all devices)
	input	:
				devId:			device ID
	output	:
	Returns	:
============================================================================ */
void cbDrvOnCardMovement(
	unsigned short devId)
{
	printf( "callback cbDrvOnCardMovement\n");
    return;
}

/* ============================================================================
	Function :	cbDrvOnModuleInterrupt
				driver callback OnModuleInterrupt (one callback for all devices)
	input	:
				devId:			device ID
				cause:			interrupt (cause fr/da)
	output	:
	Returns	:
============================================================================ */
void cbDrvOnModuleInterrupt(
	unsigned short devId,
	tTDAL_CI_IntCause cause)
{
	printf( "callback cbDrvOnModuleInterrupt cause=%s\n",cause == eTDAL_CI_INT_FR_SET ? "FR" : "DA");
    return;
}

/* ============================================================================
	Function :	cbDrvOnIirSet
				driver callback OnIirSet (one callback for all devices)
	input	:
				devId:			device ID
	output	:
	Returns	:
============================================================================ */
void cbDrvOnIirSet(
	unsigned short devId)
{
	printf( "callback cbDrvOnIirSet\n");
    return;
}

/*=============================================================================
 * CI_testU_Seq_Init
 *
 * Parameters:
 *
 * Description: Step by step module initialization sequence
 *
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_Seq_Init(unsigned short devId)
{
	unsigned short i,NumberReadOK;
	unsigned char value;
	int loop_count = 0;
	unsigned char ioValue=0;
	unsigned short bufferLength;
	tTDAL_CI_Error retDrv;

	if ((devId!=0) && (devId!=1))
	{
		printf( "\n No slot has been opened\n");
		return;
	}

	printf( "-----------------------------------\n"
			"Module detection...\n");

	for ( i=100 ; i>0 ;i-- ) /* 100 time max */
	{
		retDrv = TDAL_CI_Ctl(devId, eTDAL_CI_RD_DETECT, &ioValue);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl");

		if (ioValue == 1)
		{
		    printf( "\nModule detected in slot %u\n",devId);
			break; /* looping break  */
		}
		printf( "Insert your module in PCMCIA slot\n");
        TKEL_DelayTask(ms2tick(500));
	}
	if ( ! ioValue )
	{
		printf( "Module not detected in slot %u\n",devId);
		return;
	}

	/*Module Hard reset*/
	printf( "-----------------------------------\n"
			"Hard Reset : ");
	ioValue=1;
	retDrv=TDAL_CI_Ctl( devId, eTDAL_CI_WR_RESET , &ioValue);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "Module reset pin set");
    TKEL_DelayTask(ms2tick(50));

	/* stop Hard reset*/
	ioValue=0;
	retDrv=TDAL_CI_Ctl( devId, eTDAL_CI_WR_RESET, &ioValue);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "Module reset pin clean");

	/* wait that hard reset is ended by readybusy signal */
	retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_RDYBSY, &value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl (eTDAL_CI_RD_RDYBSY)");

	while ((value!=1)&&(loop_count<100))
	{
        loop_count++;
        TKEL_DelayTask(ms2tick(50));
		retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_DETECT, &ioValue);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl");
		if (ioValue == 0)
		{
            printf( "Module removed in slot %u\n",devId);
            loop_count=100;
			break; /* looping break  */
		}

    	retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_RDYBSY, &value);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl (eTDAL_CI_RD_RDYBSY)");
	}
	if(loop_count>=100)
	{
		printf( "FAILED\nReadyBusy bit not properly set\n");
		return;
    }

    printf( "OK\n");
    printf( "Number of loops waiting ReadyBusy = [%d]  time elapsed = %d ms\n",loop_count, (loop_count*50));

    TKEL_DelayTask(ms2tick(50));

	/*Check the CIS*/
	printf( "-----------------------------------\n"
	        "Checking CIS : ");
	retDrv=TDAL_CI_CheckCIS (devId);
	if (retDrv == eTDAL_CI_NO_ERROR)
	{
	    TestManager_LogMessage( "OK Type Unknown\n");
	}
	else if (retDrv == eTDAL_CI_CIS_COMPATIBLE_CIPLUS)
	{
	    TestManager_LogMessage( "OK Type COMPATIBLE_CIPLUS\n");
	}
	else if (retDrv == eTDAL_CI_CIS_NOT_COMPATIBLE_CIPLUS)
	{
	    TestManager_LogMessage( "OK Type NOT_COMPATIBLE_CIPLUS\n");
	}
	else
	{
	    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_CheckCIS");
	}

	/* Write COR */
	printf( "-----------------------------------\n"
			"Writing COR : ");
	retDrv=TDAL_CI_WriteCOR (devId);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCOR");

	/*Soft reset for data channel*/
	printf( "-----------------------------------\n"
			"Interface Host<==>Module Reset (data channel initialization) : ");
	value=1;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_RS, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "RS bit of the command register set");

	/* stop soft reset for data channel*/
	value=0;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_RS, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "RS bit of the command register clean");

	/* Waiting the FR bit of Status register */
    CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

	/* set SR bit to read max size for data channel */
	printf( "-----------------------------------\n"
			"Reading size proposed by the module (data channel buffer size negotiation)\n");
	value=1;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SR, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SR bit of the command register set");

	/* Waiting the DA bit of Status register */
    CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

	bufferLength = 2;
	/* read max size for data channel */
	retDrv=TDAL_CI_Read(devId, &bufferLength, CI_testU_buffer);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

	/* reading max size of data channel is ended */
	value = 0;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SR, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SR bit of the command register clean");

    CI_testU_bufferSize[devId] = (((unsigned short)CI_testU_buffer[0]) << 8) | (unsigned short)CI_testU_buffer[1];
	if (!CI_testU_bufferSize[devId] )
	{
	    printf( "FAILED LengthRead=%d BufferSize=0x%02X%02X=%d\n",bufferLength, CI_testU_buffer[0],CI_testU_buffer[1], CI_testU_bufferSize[devId]);
	    return;
	}

	printf( "OK LengthRead=%d BufferSize=0x%02X%02X=%d\n", bufferLength, CI_testU_buffer[0],CI_testU_buffer[1], CI_testU_bufferSize[devId]);

	printf( "-----------------------------------\n"
			"Writing size [=0x%02X%02X=%d] imposed to the module : ",CI_testU_buffer[0], CI_testU_buffer[1], CI_testU_bufferSize[devId]);

	/* set SW bit to 1 to write negotiated size for data channel */
	value = 1;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SW, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SW bit of the command register set");

	/* now the host tell the module what buffer to use */
	retDrv=TDAL_CI_Write(devId, bufferLength, CI_testU_buffer);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

	/* size writing is ended */
	value = 0;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SW, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SW bit of the command register clean");

	/*disable the bypass*/
	value=1;
	retDrv=TDAL_CI_Ctl( devId, eTDAL_CI_WR_BYPASS, &value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "Transport Stream pass through the module");

	/* Waiting the FR bit of Status register */
    CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

	/* send transport connexion creation */
	printf( "-----------------------------------\n"
			"HOST sends a Create_T_C to MODULE : ");
	retDrv=TDAL_CI_Write(devId, sizeof(Create_T_C), Create_T_C);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

	/* Waiting the DA bit of Status register */
    CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

 	printf( "Reading what happens : ");
	NumberReadOK=CI_testU_bufferSize[devId];
    memset(CI_testU_buffer,0x20,20);
	retDrv=TDAL_CI_Read(devId, &NumberReadOK, CI_testU_buffer);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

	/* transport connexion creation is received */
	if (NumberReadOK)
	{
		printf( "HOST receives a C_T_C_Reply + T_SB from MODULE\n");
		printf( "DATA sent by MODULE : ");
		for (i=0;i<NumberReadOK;i++)
		{
			printf( "%02X ",CI_testU_buffer[i]);
		}
		printf( "\n");
	}
	else
	{
 		printf( "Host receives nothing from MODULE \n");
 	}

	/* let make some pollings */
	printf( "\n");
	if(CI_testU_poll(devId,100) != 0)
	{
		printf( "Polling operation : FAILED\n");
		return;
	}
	printf( "End of test\n"
			"-----------------------------------\n");
    return;
}/* end of CI_testU_Seq_Init */


/*=============================================================================
 * CI_testU_poll
 *
 * Parameters:
 *
 * Description: polling transport
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
int CI_testU_poll(unsigned short devId, unsigned long nb_Iteration)
{
	unsigned short					i,NumberReadOK;
	int	NbPoll;
	tTDAL_CI_Error retDrv;

	for(NbPoll=0; NbPoll < (int)nb_Iteration; NbPoll++)
	{
		printf( "\n");
		TKEL_DelayTask(ms2tick(150));

		/* Waiting the FR bit of Status register */
		CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

		/* send T_Data_Last */
		printf("HOST sends a T_Data_Last to MODULE [%d time] : ", (NbPoll+1));
     	retDrv=TDAL_CI_Write(devId, sizeof(T_DataLastPoll), T_DataLastPoll);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

        /* Waiting the DA bit of Status register */
        CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

		printf( "Reading what happens : ");
    	NumberReadOK=CI_testU_bufferSize[devId];
    	memset(CI_testU_buffer,0x20,20);
    	retDrv=TDAL_CI_Read(devId, &NumberReadOK, CI_testU_buffer);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

		/* T_SB is received normally */
		if (NumberReadOK)
		{
			printf( "DATA sent by MODULE (T_SB normally) : ");
			for (i=0;i<NumberReadOK;i++)
			{
				printf( "%02X ",CI_testU_buffer[i]);
			}
			printf( "\n");
		}
		else
 		{
 		    printf( "Host receives nothing from MODULE \n");
 			continue;
 		}

 		/* test if the module has data available to send */
 		/* --------------------------------------------- */
 		if (CI_testU_buffer[NumberReadOK-1] < 0x80)
 		{
	   		printf( "MODULE has no data to send to HOST\n");
   			continue;
   		}

   		printf( "MODULE has data available to send to HOST\n");

		/* Waiting the FR bit of Status register */
		CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

		/* send T_RCV */
   		printf( "HOST sends a T_RCV to MODULE : ");
      	retDrv=TDAL_CI_Write(devId, sizeof(T_RCV), T_RCV);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

        /* Waiting the DA bit of Status register */
        CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

 		printf( "Reading what happens : ");
    	NumberReadOK=CI_testU_bufferSize[devId];
    	memset(CI_testU_buffer,0x20,20);
    	retDrv=TDAL_CI_Read(devId, &NumberReadOK, CI_testU_buffer);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

		if (NumberReadOK)
		{
			printf( "DATA sent by MODULE (T_Data_Last + T_SB normally): ");
			for (i=0;i<NumberReadOK;i++)
			{
				printf( "%02X ",CI_testU_buffer[i]);
			}
			printf( "\n");

			/* test if the module has sent a open session request */
			/* -------------------------------------------------- */
			if (CI_testU_buffer[5] != 0x91)
			{
				/* it is not a open session */
				continue;
			}
			printf( "MODULE sends a OpenSessionReq (RM) to HOST\n");

			/* Waiting the FR bit of Status register */
			CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

			/* send OpenSessionResp */
			printf( "HOST sends a OpenSessionResp (RM) to MODULE : ");
			retDrv=TDAL_CI_Write(devId, sizeof(OpenSessionResp), OpenSessionResp);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

			/* Waiting the DA bit of Status register */
			CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

			printf( "Reading what happens : ");
			NumberReadOK=CI_testU_bufferSize[devId];
			memset(CI_testU_buffer,0x20,20);
			retDrv=TDAL_CI_Read(devId, &NumberReadOK, CI_testU_buffer);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

			if (NumberReadOK)
			{
				printf( "DATA sent by MODULE (T_SB normally) : ");
				for (i=0;i<NumberReadOK;i++)
				{
					printf( "%02X ",CI_testU_buffer[i]);
				}
				printf( "\n");
			}
			else
			{
				printf( "Host receives nothing from MODULE \n");
 			}
		}
		else
		{
 			printf( "Host receives nothing from MODULE \n");
 		}
	}
   	return 0;
}/* end of CI_testU_poll */


/*=============================================================================
 * CI_testU_HardReset
 *
 * Parameters:
 *
 * Description: Module hard reset
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_HardReset(unsigned short devId)
{
	unsigned short i;
	unsigned char value;
	unsigned char SlotName;
	int  loop_count = 0;
    unsigned char ioValue;
	tTDAL_CI_Error retDrv;

	if ((devId!=0) && (devId!=1))
	{
		printf( "\n No slot has been opened\n");
		return;
	}

	/* wait module insertion */
	for ( i=100 ; i>0 ;i-- ) /* 100 time max */
	{
		SlotName = ' ';

		printf( "Insert your module in PCMCIA Slot %u \n",devId);
		retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_DETECT, &ioValue);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl");
		if (ioValue == 1)
		{
		    printf( "\nModule detected in slot %u\n",devId);
			break; /* looping break  */
		}
        TKEL_DelayTask(ms2tick(500));
	}
	if ( ! ioValue )
	{
		printf( "Module not detected in slot %u\n",devId);
		return;
	}

	/*Module Hard reset*/
	printf( "Hard Reset : ");
	ioValue=1;
	retDrv=TDAL_CI_Ctl( devId, eTDAL_CI_WR_RESET , &ioValue);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "module reset pin set");
    TKEL_DelayTask(ms2tick(50));

	/* stop Module Hard reset*/
	ioValue=0;
    retDrv=TDAL_CI_Ctl( devId, eTDAL_CI_WR_RESET, &ioValue);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "Module reset pin clean");
	TKEL_DelayTask(ms2tick(50));

	/*waiting readybusy*/
	retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_RDYBSY, &value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl (eTDAL_CI_RD_RDYBSY)");
	while ((value!=1)&&(loop_count<100))
	{
        loop_count++;
        TKEL_DelayTask(ms2tick(50));
		retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_DETECT, &ioValue);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl (eTDAL_CI_RD_DETECT)");
		if (ioValue == 0)
		{
            printf( "Module removed in slot %u\n",devId);
            loop_count=100;
			break; /* looping break  */
		}
    	retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_RDYBSY, &value);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl (eTDAL_CI_RD_RDYBSY)");
	}
	if(loop_count>=100)
	{
		printf( "FAILED\nReadyBusy bit not properly set\n");
		return;
    }

    printf( "OK\n");
    printf( "Number of loops waiting ReadyBusy = [%d]  time elapsed = %d ms\n",loop_count, (loop_count*50));

    return;
}/* end of CI_testU_HardReset */




/*=============================================================================
 * CI_testU_CheckCIS
 *
 * Parameters:
 *
 * Description: check CIS
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_CheckCIS(unsigned short devId)
{
	tTDAL_CI_Error retDrv;

	retDrv=TDAL_CI_CheckCIS (devId);
	if (retDrv == eTDAL_CI_NO_ERROR)
	{
	    TestManager_LogMessage( "TDAL_CI_CheckCIS OK Type Unknown\n");
	}
	else if (retDrv == eTDAL_CI_CIS_COMPATIBLE_CIPLUS)
	{
	    TestManager_LogMessage( "TDAL_CI_CheckCIS OK Type COMPATIBLE_CIPLUS\n");
	}
	else if (retDrv == eTDAL_CI_CIS_NOT_COMPATIBLE_CIPLUS)
	{
	    TestManager_LogMessage( "TDAL_CI_CheckCIS OK Type NOT_COMPATIBLE_CIPLUS\n");
	}
	else
	{
	    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_CheckCIS");
	}

    return;
}/* end of CI_testU_CheckCIS */


/*=============================================================================
 * CI_testU_WriteCOR
 *
 * Parameters:
 *
 * Description: write COR
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_WriteCOR(unsigned short devId)
{
	tTDAL_CI_Error retDrv;

	retDrv=TDAL_CI_WriteCOR(devId);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_WriteCOR");

    return;
}/* end of CI_testU_WriteCOR */


/*=============================================================================
 * CI_testU_SoftReset
 *
 * Parameters:
 *
 * Description: software reset
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_SoftReset(unsigned short devId)
{
	unsigned char value;
	tTDAL_CI_Error retDrv;

	/*Soft reset*/
	printf( "Interface Host<==>Module Reset : ");
	value=1;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_RS, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "RS bit of the command register set");

	/* stop soft reset */
	value=0;
	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_RS, value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "The RS bit of the command registerclean");

	/* Waiting the FR bit of Status register */
    CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

    return;
}/* end of CI_testU_SoftReset */



/*=============================================================================
 * CI_testU_ReadWriteSize
 *
 * Parameters:
 *
 * Description: data channel buffer size reading and writing
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_ReadWriteSize(unsigned short devId, unsigned char bitName)
{
	unsigned char  value;
	tTDAL_CI_Error retDrv;

    switch(bitName)
    {
        case kTDAL_CI_SR :
        {
        	printf( "Reading size proposed by the module\n");
           	/* set SR to 1 to read size */
      		value=1;
        	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SR, value);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SR bit of the command register set");

			/* Waiting the DA bit of Status register */
			CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

        	CI_testU_bufferLength = 2;

        	/* read max size*/
        	retDrv=TDAL_CI_Read(devId, &CI_testU_bufferLength, CI_testU_buffer);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

         	/* now size reading is ended */
        	value = 0;
        	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SR, value);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "The SR bit of the command register clean");

            CI_testU_bufferSize[devId] = (((unsigned short)CI_testU_buffer[0]) << 8) | (unsigned short)CI_testU_buffer[1];

        	if (!CI_testU_bufferSize[devId] )
        	{
        	    printf( "FAILED LengthRead=%d BufferSize=0x%02X%02X=%d\n",CI_testU_bufferLength, CI_testU_buffer[0],CI_testU_buffer[1], CI_testU_bufferSize[devId]);
        	    return;
        	}

        	printf( "OK LengthRead=%d BufferSize=0x%02X%02X=%d\n", CI_testU_bufferLength, CI_testU_buffer[0],CI_testU_buffer[1], CI_testU_bufferSize[devId]);
	        break;
	    }

	    case kTDAL_CI_SW :
	    {
        	/*Writting the proposed size*/
        	printf( "Writing size [=0x%02X%02X=%d] imposed to the module : ",CI_testU_buffer[0], CI_testU_buffer[1], CI_testU_bufferSize[devId]);

        	/* set SW to 1 to write negotiated size */
        	value = 1;
        	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SW, value);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SW bit of the command register set");

        	/* the host tell the module what buffer to use */
        	retDrv=TDAL_CI_Write(devId, CI_testU_bufferLength, CI_testU_buffer);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

        	/* now size writing is ended */
        	value = 0;
        	retDrv=TDAL_CI_WriteCmd (devId, kTDAL_CI_SW, value);
            TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "SW bit of the command register set");

	        break;
	    }
	}
    return;
}/* end of CI_testU_ReadWriteSize */


/*=============================================================================
 * CI_testU_WriteBypass
 *
 * Parameters:
 *
 * Description: enable or disable bypass
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_WriteBypass(unsigned short devId, unsigned char value)
{
	tTDAL_CI_Error retDrv;

	printf( "\n\nBYPASS ");

    retDrv=TDAL_CI_Ctl( devId, eTDAL_CI_WR_BYPASS, &value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl");

	if (value == 1)
	{
		printf( "disable : OK\nTransport Stream pass through the module\n");
    }
	if (value == 0)
	{
 		printf( "enable : OK\nTransport Stream doesn't pass through the module\n");
    }

    return;
}/* end of CI_testU_WriteBypass */

/*=============================================================================
 * CI_testU_Write_Create_TC
 *
 * Parameters:
				devId:			device ID
 *
 * Description:
 *
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_Write_Create_TC(unsigned short devId)
{
	tTDAL_CI_Error retDrv;

	if ((devId!=0) && (devId!=1))
	{
		printf( "\n No slot has been opened\n");
		return;
	}

	/* Waiting the FR bit of Status register */
	CI_testU_wait_StatusBit(devId, kTDAL_CI_FR);

	/* send transport connexion creation */
	printf( "-----------------------------------\n"
			"HOST sends a Create_T_C to MODULE : ");
	retDrv=TDAL_CI_Write(devId, sizeof(Create_T_C), Create_T_C);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Write");

    return;
}/* end of CI_testU_Write_Create_TC */


/*=============================================================================
 * CI_testU_Read_CTC_Reply
 *
 * Parameters:
				devId:			device ID
 *
 * Description:
 *
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_Read_CTC_Reply(unsigned short devId)
{
	unsigned short i,NumberReadOK;
	tTDAL_CI_Error retDrv;

	if ((devId!=0) && (devId!=1))
	{
		printf( "\n No slot has been opened\n");
		return;
	}

	/* Waiting the DA bit of Status register */
	CI_testU_wait_StatusBit(devId, kTDAL_CI_DA);

 	printf( "Reading what happens : ");
	NumberReadOK=CI_testU_bufferSize[devId];
	memset(CI_testU_buffer,0x20,20);
    retDrv=TDAL_CI_Read(devId, &NumberReadOK, CI_testU_buffer);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Read");

	/* transport connexion creation is received */
	if (NumberReadOK)
	{
		printf( "HOST receives a C_T_C_Reply + T_SB from MODULE\n");
		printf( "DATA sent by MODULE : ");
		for (i=0;i<NumberReadOK;i++)
		{
			printf( "%02X ",CI_testU_buffer[i]);
		}
		printf( "\n");
	}
	else
	{
 		printf( "Host receives nothing from MODULE \n");
 	}

    return;
}/* end of CI_testU_Read_CTC_Reply */


/*=============================================================================
 * CI_testU_Open
 *
 * Parameters:
				slotName:		"SLOT_0" or "SLOT_1" or ...
				devId:			device ID
 *
 * Description:
 *
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_Open(char* slotName, unsigned short* devId)
{
	tTDAL_CI_Error retDrv;

	retDrv=TDAL_CI_Open(slotName, devId);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Open");

    return;
}/* end of CI_testU_Open */


/*=============================================================================
 * CI_testU_Close
 *
 * Parameters:
				devId:			device ID
 *
 * Description:
 *
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_Close(unsigned short* devId)
{
	tTDAL_CI_Error retDrv;

	if ((*devId!=0) && (*devId!=1))
	{
		printf( "\n No slot has been opened\n");
		return;
	}

	retDrv=TDAL_CI_Close(*devId);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Close");

	*devId = 5;

    return;
}/* end of CI_testU_Close */


/*=============================================================================
 * CI_testU_Register_Callback
 *
 * Parameters:
				devId:			device ID
 *
 * Description:
 *
 *
 * Returns:
 *      		nothing
 *===========================================================================*/
void CI_testU_Register_Callback(unsigned short devId)
{
	tTDAL_CI_Error retDrv;

	if ((devId!=0) && (devId!=1))
	{
		printf( "\n No slot has been opened\n");
		return;
	}

	retDrv=TDAL_CI_RegisterCallbacks(devId
					,cbDrvOnCardMovement
					,cbDrvOnModuleInterrupt
    				,cbDrvOnIirSet);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_RegisterCallbacks");

    return;
}/* end of CI_testU_Register_Callback */


unsigned char CI_testU_wait_StatusBit(unsigned short devId, unsigned char statusBit)
{
	unsigned char value = 0;
	int loop_count = 0;
	unsigned char ioValue=0;
	tTDAL_CI_Error retDrv;

	/* Waiting one bit of Status register */
	retDrv=TDAL_CI_ReadStatus (devId, statusBit, &value);
    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_ReadStatus");

	while ((value!=1) && (loop_count<200))
	{
        loop_count++;
        TKEL_DelayTask(ms2tick(50));
 		retDrv=TDAL_CI_Ctl(devId, eTDAL_CI_RD_DETECT, &ioValue);
	    TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_Ctl");

		if (ioValue == 0)
		{
            printf( "Module removed in slot %u\n",devId);
            loop_count=10000;
			break; /* looping break  */
		}

    	retDrv = TDAL_CI_ReadStatus (devId, statusBit, &value);
        TestManager_AssertEqual(retDrv, eTDAL_CI_NO_ERROR, "TDAL_CI_ReadStatus");
	}
	printf("Data");
	switch(statusBit)
	{
		case kTDAL_CI_DA : printf(" DA"); break;
		case kTDAL_CI_FR : printf(" FR"); break;
		default : printf(" bit=%02x",statusBit); break;
	}
	if(value!=1)
	{
	    printf( " bit FAILED not properly set, number of loops waiting=%d  time elapsed=%d ms\n",loop_count, (loop_count*50));
	    return(FALSE);
	}
	else
	{
		printf( " bit set OK, number of loops waiting=%d  time elapsed=%d ms\n",loop_count, (loop_count*50));
	}

	return(TRUE);
}

/*=============================================================================
 * Glue functions
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *===========================================================================*/
void CI_Mod_Open(void)
{
	if (TDAL_CI_Init() != eTDAL_CI_NO_ERROR)
	{
		printf( "TDAL_CI_Init ERROR\n");
	}

    CI_testU_Open("SLOT_0",&TDAL_CI_devId);
}

void CI_Callback_Register(void)
{
    CI_testU_Register_Callback(TDAL_CI_devId);
}

void CI_Mod_Init(void)
{
    CI_testU_Seq_Init(TDAL_CI_devId);
}

void CI_Hard_Reset(void)
{
    CI_testU_HardReset(TDAL_CI_devId);
}

void CI_Check_CIS(void)
{
    CI_testU_CheckCIS(TDAL_CI_devId);
}

void CI_Write_COR(void)
{
    CI_testU_WriteCOR(TDAL_CI_devId);
}

void CI_Data_Channel_Soft_Reset(void)
{
    CI_testU_SoftReset(TDAL_CI_devId);
}

void CI_Data_Channel_Read_Size(void)
{
    CI_testU_ReadWriteSize(TDAL_CI_devId, kTDAL_CI_SR);
}

void CI_Data_Channel_Write_Size(void)
{
    CI_testU_ReadWriteSize(TDAL_CI_devId, kTDAL_CI_SW);
}

void CI_Enable_Bypass(void)
{
    CI_testU_WriteBypass(TDAL_CI_devId, 1);
}

void CI_Write_Create_TC(void)
{
    CI_testU_Write_Create_TC(TDAL_CI_devId);
}

void CI_Read_CTC_Reply(void)
{
    CI_testU_Read_CTC_Reply(TDAL_CI_devId);
}

void CI_Disable_Bypass(void)
{
    CI_testU_WriteBypass(TDAL_CI_devId, 0);
}

void CI_Mod_Close(void)
{
    CI_testU_Close(&TDAL_CI_devId);
}
