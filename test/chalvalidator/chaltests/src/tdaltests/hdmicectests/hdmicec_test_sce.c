/** \file hdmicec_test_sce.c
 *  \brief Scenario Tests for the HDMI_CEC layer of the TDAL API
 *  \author 
 *  \date 
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "tkel.h"
#include "tbox.h"
#include "tdal_hdmi_cec.h"

#include "TestManager.h"
#include "hdmicectests.h"


/******************************************************
 *              Defines                               *
 ******************************************************/
void TestSce_DetectDevicesOnHdmi_FromDeviceTv(void);
void TestSce_DisplayHdmiDeviceDatabase(void);
LOCAL tTDAL_HDMI_CEC_LogicalAddress TestSce_LinearLogicalDeviceAddress(uint8_t ucIndex);
LOCAL uint8_t TestSce_ExpLogicalDeviceAddress(tTDAL_HDMI_CEC_LogicalAddress  eLogAddress);
/******************************************************
 *              HDMI_CEC test tree                         *
 ******************************************************/


static tTestNode gTestScenarioDetectDevicesOnHdmi_FromTv = {
	"TestScenarioDetectDevicesOnHdmi",
	"Detect devices on HDMI",
	0,
	TestSce_DetectDevicesOnHdmi_FromDeviceTv,
	NULL
};


tTestNode* gHDMI_CECScenarioTestNodeList[] = {
	&gTestScenarioDetectDevicesOnHdmi_FromTv,    
	NULL
};

tTestNode gTDAL_HDMI_CEC_ScenarioTestSuite = {
	"TDAL_HDMI_CEC Scenario Tests",
	"test TDAL_HDMI_CEC scenario",
	0, /* not interactive */
	NULL, /* no function, it's a test suite */
	(void**)gHDMI_CECScenarioTestNodeList
};

/******************************************************
 *              Scenario tests                        *
 ******************************************************/
LOCAL void TestSce_cbNotifyFunctionTdalHdmiCec(tTDAL_HDMI_CEC_Event eEvent, void                *pData);
    
typedef enum
{
    eStatusDeviceUnknown,
    eStatusDeviceNotPresent,
    eStatusDevicePresent,
}TestSce_StatusDevice_t;

typedef struct
{
    TestSce_StatusDevice_t  eStatusDevice;
}TestSce_InfoByDevice_t;

LOCAL TestSce_InfoByDevice_t   TestSce_pstHdmiDeviceDatabase[kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS];    

void TestSce_DetectDevicesOnHdmi_FromDeviceTv(void)
{    
    tTDAL_HDMI_CEC_Message          stMessage;
    uint8_t ucIndex = 0;

    memset(TestSce_pstHdmiDeviceDatabase, 0x0, sizeof(TestSce_InfoByDevice_t));
    TestSce_pstHdmiDeviceDatabase[TestSce_ExpLogicalDeviceAddress(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV)].eStatusDevice = eStatusDevicePresent;    

	TestManager_AssertEqual( TDAL_HDMI_CEC_Init(eTDAL_HDMI_CEC_DEVICE_TYPE_TV), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_Init()" );

    TestManager_AssertEqual( TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_MESSAGE_RECEIVED,TestSce_cbNotifyFunctionTdalHdmiCec ), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_MESSAGE_RECEIVED)" );
    TestManager_AssertEqual( TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_NEW_LOGICAL_ADDRESS,TestSce_cbNotifyFunctionTdalHdmiCec ), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_NEW_LOGICAL_ADDRESS)" );
    TestManager_AssertEqual( TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_NEW_PHYSICAL_ADDRESS,TestSce_cbNotifyFunctionTdalHdmiCec ), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_NEW_PHYSICAL_ADDRESS)" );
    TestManager_AssertEqual( TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_ACKNOWLEDGE_POLLING_MESSAGE,TestSce_cbNotifyFunctionTdalHdmiCec ), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_EventSubscribe(eTDAL_HDMI_CEC_EVENT_ACKNOWLEDGE_RECEIVED)" );

    TestManager_AssertEqual( TDAL_HDMI_CEC_Start(), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_Start()" );
    
    TestManager_AskQuestion( "Ready to continue with the device polling )" );

    TestSce_DisplayHdmiDeviceDatabase();

    /* Send a polling message to all devices (from TV to RESERVCED_2 */
    for(ucIndex=1;  // 0 is a TV !!!
         ucIndex<(kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS);  
         ucIndex++)
    {     
        stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
        stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
        stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_POLLING_MESSAGE;             
        TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        
    }

    sleep(5);

    TestSce_DisplayHdmiDeviceDatabase();
    
    TestManager_AskQuestion( "Ready to get physical @ of each devices connected");

    /* Get physical address of each known device */
    for(ucIndex=1;  // 0 is a TV !!!
         ucIndex<(kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS);  
         ucIndex++)
    {    
        if(TestSce_pstHdmiDeviceDatabase[ucIndex].eStatusDevice == eStatusDevicePresent)
        {
            stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
            stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
            stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS;             
            TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        

            //TestManager_AskQuestion( "\n\nReady to go to next HDMI Device ?\n\n" ); 
        }        
    }           

     TestManager_AskQuestion( "Ready to get OSD Name of each devices connected");

    /* Get OSD Name  of each known device */
    for(ucIndex=1;  // 0 is a TV !!!
         ucIndex<(kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS);  
         ucIndex++)
    {    
        if(TestSce_pstHdmiDeviceDatabase[ucIndex].eStatusDevice == eStatusDevicePresent)
        {
            stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
            stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
            stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_GIVE_OSD_NAME;             
            TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        

            //TestManager_AskQuestion( "\n\nReady to go to next HDMI Device ?\n\n" ); 
        }        
    }       

    TestManager_AskQuestion( "Ready to get other informations " ); 

    /* Get OSD Name and version of each known device */
    for(ucIndex=1;  // 0 is a TV !!!
         ucIndex<(kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS);  
         ucIndex++)
    {    
        if(TestSce_pstHdmiDeviceDatabase[ucIndex].eStatusDevice == eStatusDevicePresent)
        {            
            stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
            stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
            stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_GIVE_DEVICE_VENDOR_ID;             
            TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        
           
            stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
            stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
            stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_GIVE_DEVICE_POWER_STATUS;             
            TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        

            if(TestSce_LinearLogicalDeviceAddress(ucIndex) == eTDAL_HDMI_CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM)
            {
                stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
                stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
                stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_GIVE_AUDIO_STATUS;             
                TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        

                stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
                stMessage.Destination = TestSce_LinearLogicalDeviceAddress(ucIndex);
                stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS;             
                TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        
            }

            TestManager_AskQuestion( "\n\nReady to go to next HDMI Device ?\n\n" ); 
        }        
    }           

    TestManager_AskQuestion( "\n\nReady to broadcast a standby message\n\n" ); 
    stMessage.Source = eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV;
    stMessage.Destination =eTDAL_HDMI_CEC_LOGICAL_ADDRESS_UNREGISTERED_BROADCAST;
    stMessage.Opcode = eTDAL_HDMI_CEC_OPCODE_STANDBY;             
    TestManager_AssertEqual(TDAL_HDMI_CEC_SendMessage (&stMessage), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_SendMessage()" );                        


    TestManager_AskQuestion( "\n\nReady to leave test\n\n" ); 
    TestManager_AssertEqual( TDAL_HDMI_CEC_Stop(), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_Stop()" );
	TestManager_AssertEqual( TDAL_HDMI_CEC_Term(), eTDAL_HDMI_CEC_NO_ERROR, "TDAL_HDMI_CEC_Term()" );    
}




/******************************************************
 *              Local functions tests                 *
 ******************************************************/
void 
TestSce_DisplayHdmiDeviceDatabase(void)
{
    int i;
    for(i=0; i<kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS;i++)
    {
        printf("HDMI DEVICE %.02d/%.02d : Device %20.20s %s\n",
               i, kTDAL_HDMI_CEC_NB_REAL_LOGICAL_ADDRESS,
               TDAL_HDMI_CEC_TranslateLogicalAddressInString(TestSce_LinearLogicalDeviceAddress(i)),
               (TestSce_pstHdmiDeviceDatabase[i].eStatusDevice == eStatusDeviceUnknown) ? "Presence Unknown" : \
			   (TestSce_pstHdmiDeviceDatabase[i].eStatusDevice == eStatusDevicePresent) ? "Present" : "Not Present");
    }
}

tTDAL_HDMI_CEC_LogicalAddress 
TestSce_LinearLogicalDeviceAddress(uint8_t ucIndex)
{
    switch(ucIndex)
    {
        case 0:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV);
        case 1:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1);
        case 2:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2);
        case 3:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_1);
        case 4:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_1);
        case 5:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM);
        case 6:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_2);
        case 7:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_3);
        case 8:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_2);
        case 9:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3);
        case 10:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_4);
        case 11:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_3);
        case 12:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_1);
        case 13:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_2);            
        case 14:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_FREE_USE);            
        case 15:
        default:
            return(eTDAL_HDMI_CEC_LOGICAL_ADDRESS_UNREGISTERED_BROADCAST);                    
    }            
}

uint8_t
TestSce_ExpLogicalDeviceAddress(tTDAL_HDMI_CEC_LogicalAddress  eLogAddress)
{
    switch(eLogAddress)
    {
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV:
            return(0);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1:
            return(1);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2:
            return(2);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_1:
            return(3);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_1:
            return(4);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM:
            return(5);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_2:
            return(6);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_3:
            return(7);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_2:
            return(8);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3:
            return(9);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TUNER_4:
            return(10);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_3:
            return(11);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_1:
            return(12);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_2:
            return(13);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_FREE_USE:
            return(14);
        case eTDAL_HDMI_CEC_LOGICAL_ADDRESS_UNREGISTERED_BROADCAST:
        default:
            return(15);
    }
}


LOCAL void 
TestSce_cbNotifyFunctionTdalHdmiCec(tTDAL_HDMI_CEC_Event eEvent, 
                                    void                *pData)
{
    tTDAL_HDMI_CEC_Message          stMessage;

    printf("[TestSce_cbNotifyFunctionTdalHdmiCec] eEvent=0x%x pData=0x%x\n", eEvent, pData);

    switch(eEvent)
    {
        case eTDAL_HDMI_CEC_EVENT_MESSAGE_RECEIVED:
            printf("[TestSce_cbNotifyFunctionTdalHdmiCec] eTDAL_HDMI_CEC_EVENT_MESSAGE_RECEIVED pData=0x%x\n", pData);

            if(pData != NULL)
            {
                stMessage = *((tTDAL_HDMI_CEC_Message*) pData);
                printf("[TestSce_cbNotifyFunctionTdalHdmiCec] From %s to %s Operand=%s\n",
                       TDAL_HDMI_CEC_TranslateLogicalAddressInString(stMessage.Source),
                       TDAL_HDMI_CEC_TranslateLogicalAddressInString(stMessage.Destination),
                       TDAL_HDMI_CEC_TranslateOpcodeInString(stMessage.Opcode));

                switch(stMessage.Opcode)                       
                {
                    case eTDAL_HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:
                        printf("eTDAL_HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS  Device Type=0x%x PhysicalAddress=%d.%d.%d.%d\n", 
                               stMessage.OperandContainer.ReportPhysicalAddress.DeviceType,
                               stMessage.OperandContainer.ReportPhysicalAddress.PhysicalAddress[0],
                               stMessage.OperandContainer.ReportPhysicalAddress.PhysicalAddress[1],
                               stMessage.OperandContainer.ReportPhysicalAddress.PhysicalAddress[2],
                               stMessage.OperandContainer.ReportPhysicalAddress.PhysicalAddress[3]);
                        break;

                    case eTDAL_HDMI_CEC_OPCODE_SET_OSD_NAME:
                        printf("eTDAL_HDMI_CEC_OPCODE_SET_OSD_NAME  Device Name= [%*.*s]\n", 
                               stMessage.OperandContainer.SetOSDName.OSDName.ASCIILength,
                               stMessage.OperandContainer.SetOSDName.OSDName.ASCIILength,
                               stMessage.OperandContainer.SetOSDName.OSDName.ASCII);
                        break;
                    
                    case eTDAL_HDMI_CEC_OPCODE_DEVICE_VENDOR_ID:
                        printf("eTDAL_HDMI_CEC_OPCODE_DEVICE_VENDOR_ID  Device Vendor Id = %ld\n", 
                               stMessage.OperandContainer.DeviceVendorID.VendorID);
                        break;

                    case eTDAL_HDMI_CEC_OPCODE_FEATURE_ABORT:
                    case eTDAL_HDMI_CEC_OPCODE_IMAGE_VIEW_ON:
                    case eTDAL_HDMI_CEC_OPCODE_TUNER_STEP_INCREMENT:
                    case eTDAL_HDMI_CEC_OPCODE_TUNER_STEP_DECREMENT:
                    case eTDAL_HDMI_CEC_OPCODE_TUNER_DEVICE_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_RECORD_ON:
                    case eTDAL_HDMI_CEC_OPCODE_RECORD_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_RECORD_OFF:
                    case eTDAL_HDMI_CEC_OPCODE_TEXT_VIEW_ON:
                    case eTDAL_HDMI_CEC_OPCODE_RECORD_TV_SCREEN:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_DECK_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_DECK_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_SET_MENU_LANGUAGE:
                    case eTDAL_HDMI_CEC_OPCODE_CLEAR_ANALOGUE_TIMER:
                    case eTDAL_HDMI_CEC_OPCODE_SET_ANALOGUE_TIMER:
                    case eTDAL_HDMI_CEC_OPCODE_TIMER_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_STANDBY:
                    case eTDAL_HDMI_CEC_OPCODE_PLAY:
                    case eTDAL_HDMI_CEC_OPCODE_DECK_CONTROL:
                    case eTDAL_HDMI_CEC_OPCODE_TIMER_CLEARED_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_USER_CONTROL_PRESSED:
                    case eTDAL_HDMI_CEC_OPCODE_USER_CONTROL_RELEASED:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_OSD_NAME:                    
                    case eTDAL_HDMI_CEC_OPCODE_SET_OSD_STRING:
                    case eTDAL_HDMI_CEC_OPCODE_SET_TIMER_PROGRAM_TITLE:
                    case eTDAL_HDMI_CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_AUDIO_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_SET_SYSTEM_AUDIO_MODE:
                    case eTDAL_HDMI_CEC_OPCODE_REPORT_AUDIO_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_ROUTING_CHANGE:
                    case eTDAL_HDMI_CEC_OPCODE_ROUTING_INFORMATION:
                    case eTDAL_HDMI_CEC_OPCODE_ACTIVE_SOURCE:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:
                    case eTDAL_HDMI_CEC_OPCODE_REQUEST_ACTIVE_SOURCE:
                    case eTDAL_HDMI_CEC_OPCODE_SET_STREAM_PATH:                    
                    case eTDAL_HDMI_CEC_OPCODE_VENDOR_COMMAND:
                    case eTDAL_HDMI_CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
                    case eTDAL_HDMI_CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_DEVICE_VENDOR_ID:
                    case eTDAL_HDMI_CEC_OPCODE_MENU_REQUEST:
                    case eTDAL_HDMI_CEC_OPCODE_MENU_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_GIVE_DEVICE_POWER_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_REPORT_POWER_STATUS:
                    case eTDAL_HDMI_CEC_OPCODE_GET_MENU_LANGUAGE:
                    case eTDAL_HDMI_CEC_OPCODE_SELECT_ANALOGUE_SERVICE:
                    case eTDAL_HDMI_CEC_OPCODE_SELECT_DIGITAL_SERVICE:
                    case eTDAL_HDMI_CEC_OPCODE_SET_AUDIO_RATE:
                    case eTDAL_HDMI_CEC_OPCODE_SET_DIGITAL_TIMER:
                    case eTDAL_HDMI_CEC_OPCODE_CLEAR_DIGITAL_TIMER:
                    case eTDAL_HDMI_CEC_OPCODE_INACTIVE_SOURCE:
                    case eTDAL_HDMI_CEC_OPCODE_CEC_VERSION:
                    case eTDAL_HDMI_CEC_OPCODE_GET_CEC_VERSION:
                    case eTDAL_HDMI_CEC_OPCODE_VENDOR_COMMAND_WITH_ID:
                    case eTDAL_HDMI_CEC_OPCODE_CLEAR_EXTERNAL_TIMER:
                    case eTDAL_HDMI_CEC_OPCODE_SET_EXTERNAL_TIMER:
                    case eTDAL_HDMI_CEC_OPCODE_ABORT:
                    default:
                        break;
                }
            }
            break;

        case eTDAL_HDMI_CEC_EVENT_NEW_LOGICAL_ADDRESS:
            printf("[TestSce_cbNotifyFunctionTdalHdmiCec] eTDAL_HDMI_CEC_EVENT_NEW_LOGICAL_ADDRESS pData=0x%x\n", pData);
            if(pData != NULL)
            {
                tTDAL_HDMI_CEC_LogicalAddress   eLogicalAddress;

                eLogicalAddress = *((tTDAL_HDMI_CEC_LogicalAddress*) pData);
                printf("[TestSce_cbNotifyFunctionTdalHdmiCec] Logical Address=%s\n",
                       TDAL_HDMI_CEC_TranslateLogicalAddressInString(eLogicalAddress));
            }
            break;

        case eTDAL_HDMI_CEC_EVENT_NEW_PHYSICAL_ADDRESS:
            printf("[TestSce_cbNotifyFunctionTdalHdmiCec] eTDAL_HDMI_CEC_EVENT_NEW_PHYSICAL_ADDRESS pData=0x%x\n", pData);

            break;

        case eTDAL_HDMI_CEC_EVENT_ACKNOWLEDGE_POLLING_MESSAGE:
            printf("[TestSce_cbNotifyFunctionTdalHdmiCec] eTDAL_HDMI_CEC_EVENT_ACKNOWLEDGE_POLLING_MESSAGE pData=0x%x\n", pData);
            if(pData != NULL)
            {
                tTDAL_HDMI_CEC_PollingMessage stPollingMessage = *((tTDAL_HDMI_CEC_PollingMessage*) pData);

                if((stPollingMessage.eLogicalAddress >= eTDAL_HDMI_CEC_LOGICAL_ADDRESS_TV) && 
                   (stPollingMessage.eLogicalAddress <= eTDAL_HDMI_CEC_LOGICAL_ADDRESS_RESERVED_2))
                {
                    if(stPollingMessage.bAckNack == true)
                    {
                        TestSce_pstHdmiDeviceDatabase[TestSce_ExpLogicalDeviceAddress(stPollingMessage.eLogicalAddress)].eStatusDevice = eStatusDevicePresent;    
                    }
                    else
                    {
                        TestSce_pstHdmiDeviceDatabase[TestSce_ExpLogicalDeviceAddress(stPollingMessage.eLogicalAddress)].eStatusDevice = eStatusDeviceNotPresent; 
                    }                    
                }
                else
                {
                     printf("[TestSce_cbNotifyFunctionTdalHdmiCec] eTDAL_HDMI_CEC_EVENT_ACKNOWLEDGE_POLLING_MESSAGE %s out of scope(%s)\n", 
                            TDAL_HDMI_CEC_TranslateLogicalAddressInString(stPollingMessage.eLogicalAddress),
                            (stPollingMessage.bAckNack==true) ? "ACK":"NACK");
                }   
            }
            break;

        default:
            printf("[TestSce_cbNotifyFunctionTdalHdmiCec] Unknown message 0x%x pData=0x%x\n", eEvent, pData);
            break;
    }
}


