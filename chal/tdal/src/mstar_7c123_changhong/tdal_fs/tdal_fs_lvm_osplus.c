/*****************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME:   TDAL_FS_LVM_OSPLUS.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs_lvm_osplus.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/*****************************************************************************
 *  Includes                                            *
 *****************************************************************************/
#include <crules.h>

#ifdef PRODUCT_USE_ATAPI

/*   os_20to21_map.h   should   not   be   included   when   including   osplus   headers   */
#define   DVD_EXCLUDE_OS20TO21_MACROS

#include <stdio.h>
#include <string.h>      /*   for   strcpy()   */
#include <stdlib.h>
#include "stdevice.h"
#include "stcommon.h"

#include "stevt.h"
#include "statapi.h"

#include <tdal_common.h>
#include <tdal_fs.h>
#include "tdal_fs_lvm_p.h"

#include <tbox.h>

/*****   osplus   includes   *****/
#include <osplus/device.h>
#include <osplus/dev.h>
#include <osplus/lvm.h>
#include <osplus/ioctls/common.h>

#include <errno.h>





/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/
#define   OSPLUS_SUCCESS      (0)
#define   OSPLUS_FAILURE      (-1)

#define   SECTOR_SIZE      (512)
/*   The   maximum   value   for   max   sector   is   256,   of   course   it   gets   very   good   performances   when   */
/*   max   sector   is   256   but   will   lock   up   too   much   other   task   for   video   decode        */
/*   We   have   found   a   good   compromise   here,   which   is   to   use   64   for   hdd   transfers        */
#define   MAX_SECTORS_FOR_MUL_RW      (64)
#define   MAX_LBA_FOR_SECTOR_ACCESS   (0x0FFFFFFF)

#define   DISABLE_REVERTING_TO_POWER_ON_DEFAULT   (0x66)
#define   ENABLE_READ_CACHE            (0xAA)
#define   ENABLE_WRITE_CACHE            (0x02)

#define   kTDAL_FS_ATAPIDISCOVER_STACKSIZE      8192
#define   kTDAL_FS_ATAPIDISCOVER_TASKPRIO      8

#define   kTDAL_FS_ATAPIDISCOVER_RETRY_NB      6
#define   kTDAL_FS_ATAPIDISCOVER_RETRY_DELAY_MS   5000



/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/


/*****************************************************************************
 *  Typedefs / Structures                                    *
 *****************************************************************************/
typedef   struct   TDAL_FS_AtapiHandle_s   tTDAL_FS_AtapiHandle;

typedef   struct   TDAL_FS_AtapiInstance_s   tTDAL_FS_AtapiInstance;

struct   TDAL_FS_AtapiInstance_s
{
   ST_DeviceName_t      DeviceName;      /*   name   associated   to   the   Instance_p   */
   tTDAL_FS_AtapiHandle   *Handle_p;      /*   handle   linked   to   the   Instance_p   */
};

struct   TDAL_FS_AtapiHandle_s
{
   STATAPI_Handle_t      ATAPI_Handle;      /*   ATAPI   opened   handle   */
   U32        ATAPI_HandlesOpened;/*   numbers   of   ATAPI   open   Requests   */
   U32        NumBlocks;      /*   number   of   the   blocks   addressable   */
   U32        BlocksSize;      /*   size   of   the   blocks   */
   tTDAL_FS_AtapiInstance   *Instance_p;      /*   created   Instance_p   */
};


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FS);


/*****************************************************************************
 *  Global Variables (GLOBAL)                                 *
 *****************************************************************************/
GLOBAL   ST_DeviceName_t      TDAL_FS_AtapiName   = "STATAPI1";


/*****************************************************************************
 *  Extern Variables (IMPORT)                                 *
 *****************************************************************************/
IMPORT   ST_Partition_t      *system_partition;
IMPORT   ST_Partition_t      *ncache_partition;
IMPORT   ST_DeviceName_t      SYS_EvtName;


/*****************************************************************************
 *  Local File Variables (LOCAL)                              *
 *****************************************************************************/
LOCAL   semaphore_t               TDAL_FS_AtapiEventSem;
LOCAL   STEVT_Handle_t      TDAL_FS_AtapiEventHandle;
LOCAL   semaphore_t               TDAL_FS_AtapiAccessSem;

LOCAL   task_t   *               TDAL_FS_AtapiDiscoverTaskId;




/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                         *
 *****************************************************************************/


/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                         *
 *****************************************************************************/
LOCAL   void        TDAL_FSi_AtapiDiscoverTaskFct(   void   *   args   );
LOCAL   ST_ErrorCode_t      TDAL_FSi_STATAPI_Init(void);
LOCAL   tTDAL_FS_Error      TDAL_FSi_CreateOsplusAtapiDevice(   void   );

LOCAL   dev_instance_t   *pTDAL_FS_AtapiWrap_Create(void   *DeviceName);
LOCAL   void      pTDAL_FS_AtapiWrap_Destroy(dev_instance_t   *DevInstance_p);
LOCAL   dev_handle_t   *pTDAL_FS_AtapiWrap_Open(dev_instance_t   *DevInstance_p,   S32   Mode);
LOCAL   S32      pTDAL_FS_AtapiWrap_Close(dev_handle_t   *DevHandle_p);
LOCAL   S32      pTDAL_FS_AtapiWrap_Ioctl(dev_handle_t   *DevHandle_p,   S32   Request,   void   *Size_p);
LOCAL   S32      pTDAL_FS_AtapiWrap_BlockRead(dev_handle_t   *DevHandle_p,   void   *Buffer_p,   lba_t   LBA,   size_t   NumBlocks);
LOCAL   S32      pTDAL_FS_AtapiWrap_BlockWrite(dev_handle_t   *DevHandle_p,   const   void   *Buffer_p,   lba_t   LBA,   size_t   NumBlocks);


/*****************************************************************************
 *  Functions Definitions (LOCAL/GLOBAL)                         *
 *****************************************************************************/


/******************************************************************************
 * Function Name : TDAL_FSi_SetupAtapiDevice
 *
 * Description   : Setup the ATAPI device by creating the AtapiDiscover task
 *             which will try several times to find an ATAPI device.
 *
 * Side effects  : 
 *
 * Comment       : 
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FSi_SetupAtapiDevice(   void   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;
   
   /**   Init   the   task   **/
#error
   TDAL_FS_AtapiDiscoverTaskId = task_create(   TDAL_FSi_AtapiDiscoverTaskFct   ,
                        NULL   ,
                        kTDAL_FS_ATAPIDISCOVER_STACKSIZE   ,
                        kTDAL_FS_ATAPIDISCOVER_TASKPRIO   ,
                        "TDAL_FS_AtapiDiscoverTask"   ,
                        (task_flags_t)0   );
   if   (   TDAL_FS_AtapiDiscoverTaskId == NULL   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSi_SetupAtapiDevice:   [ERROR]   task_create   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
   }
   
   return   result;
}






/******************************************************************************
 * Function Name : TDAL_FSi_AtapiDiscoverTaskFct
 *
 * Description   : Task that will try to find an ATAPI device.
 *             If found, it will create an OSPLUS device
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
LOCAL   void        TDAL_FSi_AtapiDiscoverTaskFct(   void   *   args   )
{
   uint8_t      nb_retries = 0;
   bool      stop_statapi_init = FALSE;
   ST_ErrorCode_t   st_err = STATAPI_ERROR_DEVICE_NOT_PRESENT;
   uint32_t      tick_to_sleep;
   tTDAL_FS_Error   tdal_fs_err;
   
   
   mTBOX_FCT_ENTER("TDAL_FSi_AtapiDiscoverTaskFct");
   
   tick_to_sleep = (ST_GetClocksPerSecond()/1000)
            *   kTDAL_FS_ATAPIDISCOVER_RETRY_DELAY_MS;
   
   for(   nb_retries=0   ;   (nb_retries   <   kTDAL_FS_ATAPIDISCOVER_RETRY_NB)   &&
            (!stop_statapi_init)   ;   nb_retries++   )
   {
      st_err = TDAL_FSi_STATAPI_Init();
      if   (   st_err == STATAPI_ERROR_DEVICE_NOT_PRESENT   )
      {
      mTBOX_TRACE((kTBOX_NIV_1,"Device   not   present   :   Sleeping   for   %d   ticks\n",   tick_to_sleep));
      task_delay(   tick_to_sleep   );
      }
      else
      {
      if   (   st_err != ST_NO_ERROR   )
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_FSi_STATAPI_Init()   failed   :   st_err=%d\n",
               TBOX_FunctionName,   st_err));
      }
      stop_statapi_init = TRUE;
      }
   }
   
   if   (   st_err == ST_NO_ERROR   )
   {
      mTBOX_TRACE((kTBOX_NIV_1,"Device   found   :   create   OSPLUS   ATAPI   device\n"));
      /*   There   is   an   ATAPI   device,   let's   create   the   OSPLUS   device   */
      tdal_fs_err = TDAL_FSi_CreateOsplusAtapiDevice();
      if   (   tdal_fs_err != eTDAL_FS_NO_ERROR   )
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   TDAL_FSi_CreateOsplusAtapiDevice()   failed   :   tdal_fs_err=%d\n",
            TBOX_FunctionName,   tdal_fs_err));
      }
   }
   
   /*   Let's   sleep   all   time   */
   while(   TRUE   )
   {
      task_delay(30000);
   }
}


LOCAL   tTDAL_FS_Error      TDAL_FSi_CreateOsplusAtapiDevice(   void   )
{
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;
   dev_class_t   *      DeviceClass;
   dev_params_t      ClassParams;
   dev_ops_t      MethodTable = {
      pTDAL_FS_AtapiWrap_Create,      /*   Device   create   method   */
      pTDAL_FS_AtapiWrap_Destroy,   /*   Device   deletion   method   */
      pTDAL_FS_AtapiWrap_Open,      /*   Device   open   method   */
      pTDAL_FS_AtapiWrap_Close,      /*   Device   close   method   */
      NULL,            /*   Character   read   method   */
      NULL,            /*   Character   write   method   */
      pTDAL_FS_AtapiWrap_Ioctl,      /*   I/O   control   method   */
      pTDAL_FS_AtapiWrap_BlockRead,   /*   Block   read   method   */
      pTDAL_FS_AtapiWrap_BlockWrite,   /*   Block   write   method   */
      NULL,            /*   Streaming   block   read   method   */
      NULL            /*   Streaming   block   write   method   */
   };
   
   mTBOX_FCT_ENTER("TDAL_FSi_CreateOsplusAtapiDevice");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FSi_CreateOsplusAtapiDevice()\n"));
   
   
   ClassParams.ops       = &MethodTable;
   ClassParams.elevator_params = NULL;

   /*   create   a   generic   device   driver   */
   DeviceClass = dev_class_create(   OSPLUS_DEVICE_TYPE_FIXED_STORAGE,
                   &ClassParams   );
   if(DeviceClass == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   dev_class_create   failed\n",TBOX_FunctionName));
      result = eTDAL_FS_ERROR_NOT_DONE;
   }

   if   (result == eTDAL_FS_NO_ERROR)
   {
      /*   Instance_p   one   object   of   the   previous   created   class   */
      if   (dev_instance_create(DeviceClass,   TDAL_FS_AtapiName,   TDAL_FS_AtapiName) == OSPLUS_FAILURE)
      {
      dev_class_delete(DeviceClass);
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"%s:   dev_instance_create   failed\n",TBOX_FunctionName));
      result = eTDAL_FS_ERROR_NOT_DONE;
      }
   }
   
   
   mTBOX_RETURN(result);
}


   
   


LOCAL   ST_ErrorCode_t   TDAL_FSi_STATAPI_Init(void)
{
   STATAPI_InitParams_t   STATAPI_InitParams;
   ST_ErrorCode_t      ST_ErrorCode;
   ST_ClockInfo_t      ClockInfo;
   ST_Revision_t      revision;

   mTBOX_FCT_ENTER("TDAL_FSi_STATAPI_Init");

   /**   copied   from   stapi5100/testapp15/atapi.c   **
      **   specific   value   are   changed   accordingly   to   STFAE's   ATAPI   init   **/
   /*   Ugly   Writes   to   FMI   banks!   Where   should   this   be   provided   STCFG???   */
   /***********************************************************************************/
   #if   1
   {
      #define   FMICONFIG_BASE_ADDRESS   0x20200000
      #define   FMI_GENCFG        (FMICONFIG_BASE_ADDRESS + 0x28)
      #define   FMICONFIGDATA0_BANK0      (FMICONFIG_BASE_ADDRESS + 0x100)
      #define   FMICONFIGDATA1_BANK0      (FMICONFIG_BASE_ADDRESS + 0x108)
      #define   FMICONFIGDATA2_BANK0      (FMICONFIG_BASE_ADDRESS + 0x110)
      #define   FMICONFIGDATA3_BANK0      (FMICONFIG_BASE_ADDRESS + 0x118)

      #define   FMI_GENCFG_V      0xC0000000
      #define   FMICONFIGDATA0_BANK0_V   0x00540791      /*   0x010f8791   */
#if   defined(mb390) || defined(topaz)
      #define   FMICONFIGDATA1_BANK0_V   0x8E003321      /*   0x9f028821   */
      #define   FMICONFIGDATA2_BANK0_V   0x8E003321      /*   0x9f028821   */
#elif   defined(mb395)
      #define   FMICONFIGDATA1_BANK0_V   0x8E004221      /*   0x9f028821   */
      #define   FMICONFIGDATA2_BANK0_V   0x8E004221      /*   0x9f028821   */
#endif
      #define   FMICONFIGDATA3_BANK0_V   0x0000000A      /*   */
      
      
      
      volatile   U32   *regs;
      volatile   U8   *regs_8;

#if   defined(mb390) || defined(topaz)
      /*   Enable   HDDI   on   FMI   is   done   by   an   EPLD   register   (EPLD   in   Bank   1)   */
      regs_8 = (volatile   U8   *)0x41a00000;
      *regs_8   |=   0x01;
#endif

      /*   Configuring   ATAPI   is   done   using   FMI_GENCFG   bits   30   (bank0)   and   31
      *   (bank1).   Due   to   a   bug   on   Cut1.0   of   5100,   bit   31   actually
      *   configures   both.   We'll   write   both   30   and   31   anyway,   so   we   don't
      *   have   to   care   which   cut   we're   on.
      */
      regs = (volatile   U32   *)FMI_GENCFG;
      *regs = FMI_GENCFG_V;

      /*   3-   You   need   to   update   the   ATAPI   registers   and   Interrupt   number
      *   (as   in   stbgr-prj-vali5100\boot\interrupt.c   and
      *   stbgr-prj-vali5100\include\sti5100\sti5100.h)   as   i   did   it   on   5100
      *   validation   VOB.   */

      /*   Configure   bank0   for   ATAPI   */
      regs = (volatile   U32   *)FMICONFIGDATA0_BANK0;
      *regs = FMICONFIGDATA0_BANK0_V;
      regs = (volatile   U32   *)FMICONFIGDATA1_BANK0;
      *regs = FMICONFIGDATA1_BANK0_V;
      regs = (volatile   U32   *)FMICONFIGDATA2_BANK0;
      *regs = FMICONFIGDATA2_BANK0_V;
      regs = (volatile   U32   *)FMICONFIGDATA3_BANK0;
      *regs = FMICONFIGDATA3_BANK0_V;
   }
   /***********************************************************************************/
#endif

   memset(   &STATAPI_InitParams   ,   0   ,   sizeof(STATAPI_InitParams_t)   );            
   
   STATAPI_InitParams.DeviceType   = STATAPI_EMI_PIO4;
   STATAPI_InitParams.DriverPartition = system_partition;
#ifdef   ATAPI_FDMA
   STATAPI_InitParams.NCachePartition = ncache_partition;   /*   only   with   ATAPI_FDMA   */
#endif
   
   STATAPI_InitParams.BaseAddress   = (volatile   U32   *)   ATAPI_BASE_ADDRESS;
   STATAPI_InitParams.HW_ResetAddress = (volatile   U16   *)   ATAPI_BASE_ADDRESS;
#if   defined(mb390)   /*   PESI   board   */
   STATAPI_InitParams.InterruptNumber = EXTERNAL_1_INTERRUPT;
#else
   STATAPI_InitParams.InterruptNumber = EXTERNAL_0_INTERRUPT   /*EXTERNAL_1_INTERRUPT*/;
#endif
   STATAPI_InitParams.InterruptLevel = ATA_INTERRUPT_LEVEL;
   /*ST_ErrorCode = ST_GetClockInfo(   &ClockInfo   );
   STATAPI_InitParams.ClockFrequency = ClockInfo.HDDI;*/

   strcpy(STATAPI_InitParams.EVTDeviceName,   SYS_EvtName);

   ST_ErrorCode = STATAPI_Init(TDAL_FS_AtapiName,   &STATAPI_InitParams);
   if   (ST_ErrorCode == STATAPI_ERROR_DEVICE_NOT_PRESENT   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "STATAPI_Init   doesn't   find   any   hard   disk\r\n"   ));
   }
   else   if   (ST_ErrorCode != ST_NO_ERROR   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "STATAPI_Init   error   -   %s\r\n",
            TBOX_GetDriverErrorText(ST_ErrorCode)));
   }
   else
   {
   revision = STATAPI_GetRevision();
   mTBOX_TRACE((kTBOX_NIV_1,"STATAPI_Init   success   -   %s\r\n",   revision));
}


   mTBOX_RETURN(   ST_ErrorCode   );
}



#if   0
/*   AtapiWrap   functions   does   not   call   pTDAL_FS_Atapi_Setup()   anymore   */
ST_ErrorCode_t   pTDAL_FS_Atapi_Setup(const   char   *DeviceName)
{
   bool   result;

   mTBOX_TRACE((kTBOX_NIV_1,"pTDAL_FS_Atapi_Setup:   Calling   TDAL_FSi_STATAPI_Init()\n"));
   result = TDAL_FSi_STATAPI_Init();

   return   (   result   ?   ST_NO_ERROR   :   ST_ERROR_UNKNOWN_DEVICE   );
   }
#endif


#if   0
/*   Since   AtapiWrap   functions   does   not   call   pTDAL_FS_Atapi_Setup(),   it   should   not
   *   call   the   pTDAL_FS_Atapi_Term()   function.
   *   The   ATAPI   termination   could   be   done   by   AtapiDiscoverTask   */
/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_Atapi_Term
 * Input   : const char *DeviceName
 * Output   :
 * Return   : Error Code
 * ----------------------------------------------------------------------*/
ST_ErrorCode_t   pTDAL_FS_Atapi_Term(const   char   *DeviceName)
{
   ST_ErrorCode_t      st_err = ST_NO_ERROR;
   STATAPI_TermParams_t   ATAPI_TermParams;

   mTBOX_FCT_ENTER("pTDAL_FS_Atapi_Term");
   
   ATAPI_TermParams.ForceTerminate = TRUE;
   st_err = STATAPI_Term(DeviceName,&ATAPI_TermParams);
   if   (st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_Atapi_Setup:   STATAPI_Init   failed   (st_err=%x)\n",   st_err));
   }

   mTBOX_RETURN(st_err);
}
#endif



/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_Atapi_EventCallback
           Callback function for ATAPI events
 * Input   : Reason, Event, EventData
 * Output   :
 * Return   : none
 * ----------------------------------------------------------------------*/
LOCAL   void   pTDAL_FS_Atapi_EventCallback(   STEVT_CallReason_t      Reason,
                        STEVT_EventConstant_t   Event,
                        const   void   *      EventData   )
{
   if(Event == STATAPI_CMD_COMPLETE_EVT)
   {
      STOS_SemaphoreSignal(TDAL_FS_AtapiEventSem);
   }
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_Atapi_InitEvents
           Subscribes ATAPI events
 * Input   : none
 * Output   :
 * Return   : Error Code
 * ----------------------------------------------------------------------*/
LOCAL   ST_ErrorCode_t   pTDAL_FS_Atapi_InitEvents(void)
{
   ST_ErrorCode_t      st_err;
   STEVT_OpenParams_t      OpenParams;
   STEVT_SubscribeParams_t   SubsParams;

   /*   open   STEVT   session   */
   OpenParams.dummy = 0;
   st_err = STEVT_Open(   SYS_EvtName,
               &OpenParams,
               &TDAL_FS_AtapiEventHandle   );
   if   (st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "STEVT_Open   failed   :   st_err=0x%x\n",
            st_err));
      return   st_err;
   }   
   SubsParams.NotifyCallback = pTDAL_FS_Atapi_EventCallback;
   st_err = STEVT_Subscribe(   TDAL_FS_AtapiEventHandle,
                 (STEVT_EventConstant_t)STATAPI_CMD_COMPLETE_EVT,
                 &SubsParams   );
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"EVT_Subscribe   failed   (st_err=%x)\n",   st_err   ));
      return   st_err;
   }

   TDAL_FS_AtapiEventSem = STOS_SemaphoreCreateFifoTimeOut(NULL,   0);

   return   st_err;
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_Atapi_TermEvents
           Unsubscribes ATAPI events
 * Input   : none
 * Output   :
 * Return   : Error Code
 * ----------------------------------------------------------------------*/
LOCAL   ST_ErrorCode_t   pTDAL_FS_Atapi_TermEvents(void)
{
   ST_ErrorCode_t   st_err;

   st_err = STEVT_Unsubscribe(   TDAL_FS_AtapiEventHandle,
                 (STEVT_EventConstant_t)STATAPI_CMD_COMPLETE_EVT   );
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"EVT_Unsubscribe   failed   (st_err=%x)\n",   st_err   ));
      return   st_err;
   }

   /*   Close   EVT   session   */
   st_err = STEVT_Close(   TDAL_FS_AtapiEventHandle   );
   if   (st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "STEVT_Close   failed   :   st_err=0x%x\n",
            st_err));
      return   st_err;
   }
   STOS_SemaphoreDelete(TDAL_FS_AtapiEventSem);

   return   st_err;
}


/*   For   AtapiWrap   function,   traces   may   cause   memory   corruption
   *   =>   so   TBOX   calls   are   removed   from   these   functions   */
#undef   mTBOX_FCT_ENTER
#undef   mTBOX_RETURN
#undef   mTBOX_TRACE
#undef   mTBOX_PRINT
#define   mTBOX_FCT_ENTER(x)
#define   mTBOX_RETURN   return
#define   mTBOX_TRACE(x)
#define   mTBOX_PRINT(x)






/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_Create
            This method is called when a device of this class is created by
            a call to dev_instance_create(). It is passed in a
            parameter param which is private data that the method uses
            to instantiate the device Instance_p. It should return an
            Instance_p pointer for the newly created device Instance_p.
 * Input   : DeviceName
 * Output   :
 * Return   : Instance_p
 * ----------------------------------------------------------------------*/
LOCAL   dev_instance_t   *pTDAL_FS_AtapiWrap_Create(void   *DeviceName)
{
   ST_ErrorCode_t      st_err = ST_NO_ERROR;
   tTDAL_FS_AtapiInstance   *   Instance_p;

   mTBOX_FCT_ENTER("pTDAL_FS_AtapiWrap_Create");

   /*   check   the   input   parameter   */
   if   (DeviceName == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Create:   bad   input   parameter\n"));
      mTBOX_RETURN(   NULL   );
   }

   #if   0
   /*   initialize   the   ATAPI   device   */
   st_err = pTDAL_FS_Atapi_Setup(DeviceName);
   if   (   st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Create:   pTDAL_FS_Atapi_Setup   failed   (st_err=%d)\n",   st_err));
      mTBOX_RETURN(   NULL   );
   }
   #endif

   /*   initialize   the   ATAPI   events   */
   st_err = pTDAL_FS_Atapi_InitEvents();
   if   (   st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Create:   pTDAL_FS_Atapi_InitEvents   failed   (st_err=%d)\n",   st_err));
      mTBOX_RETURN(   NULL   );
   }

   /*   allocate   the   internal   Instance_p   */
   Instance_p = (tTDAL_FS_AtapiInstance   *)STOS_MemoryAllocate(   system_partition,
                              sizeof(tTDAL_FS_AtapiInstance)   );
   if   (Instance_p == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Create:   Can't   allocate   Instance   structure\n"));
      mTBOX_RETURN(   NULL   );
   }

   memset(Instance_p,   '\0',   sizeof(tTDAL_FS_AtapiInstance));

   memcpy(Instance_p->DeviceName,   (char   *)DeviceName,   ST_MAX_DEVICE_NAME);


   TDAL_FS_AtapiAccessSem = STOS_SemaphoreCreateFifo(NULL,   1);

   mTBOX_RETURN(   (dev_instance_t   *)Instance_p   );
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_Destroy
            This method is called when a device Instance_p of this class is
            deleted with a call to dev_instance_delete(). It is passed
            the Instance_p pointer returned when the device was created.
            This call should destroy the memory associated with the
            device Instance_p.
 * Input   : Instance_p - Instance to be destroy
 * Output   :
 * Return   : none
 * ----------------------------------------------------------------------*/
LOCAL   void   pTDAL_FS_AtapiWrap_Destroy(dev_instance_t   *DevInstance_p)
{
   ST_ErrorCode_t      st_err = ST_NO_ERROR;
   tTDAL_FS_AtapiInstance   *   Instance_p = (tTDAL_FS_AtapiInstance   *)DevInstance_p;

   mTBOX_FCT_ENTER(("pTDAL_FS_AtapiWrap_Destroy"));

   if   (Instance_p == NULL || Instance_p->Handle_p->ATAPI_HandlesOpened != 0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Destroy:   Instance   NULL   or   Handles   not   closed\n",   Instance_p));
      mTBOX_RETURN;
   }

   /*   unsubscribe   the   ATAPI   initialized   events   */
   st_err = pTDAL_FS_Atapi_TermEvents();

   #if   0
   /*   term   the   ATAPI   Instance_p   */
   st_err = pTDAL_FS_Atapi_Term(((tTDAL_FS_AtapiInstance   *)Instance_p)->DeviceName);
   #endif

   STOS_SemaphoreDelete(&TDAL_FS_AtapiAccessSem);

   /*   deallocate   the   internal   Instance_p   */
   STOS_MemoryDeallocate(system_partition,   Instance_p);

   mTBOX_RETURN;
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_Open
            This method is passed the Instance_p pointer created when the
            device was created, and a Mode specifying the sort of access
            required (read/write). It is responsible for creating an open
            device handle, and returning this to the device library. If the
            open cannot be satisfied the open method should return -1.
 * Input   : Instance_p - Instance to be open, Mode
 * Output   :
 * Return   : handle
 * ----------------------------------------------------------------------*/
extern   ST_Partition_t   *ncache_partition;

LOCAL   dev_handle_t   *pTDAL_FS_AtapiWrap_Open(dev_instance_t   *DevInstance_p,   S32   Mode)
{
   ST_ErrorCode_t      st_err = ST_NO_ERROR;
   tTDAL_FS_AtapiInstance   *   Instance_p = (tTDAL_FS_AtapiInstance   *)DevInstance_p;
   STATAPI_OpenParams_t   OpenParams;
   STATAPI_DriveType_t      Type;
   STATAPI_Cmd_t      Cmd;
   STATAPI_CmdStatus_t      CmdStatus;
   U8   *            Data_p;
   U32            BytesRead;
   tTDAL_FS_AtapiHandle   *      Handle_p;
   U8            CmdArray[] = {   DISABLE_REVERTING_TO_POWER_ON_DEFAULT,   ENABLE_READ_CACHE,   ENABLE_WRITE_CACHE   };
   U8            CmdCount;


   mTBOX_FCT_ENTER(("pTDAL_FS_AtapiWrap_Open"));

   /*   check   the   input   parameters   */
   if   (Instance_p == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   Instance   NULL\n",   Instance_p));
      mTBOX_RETURN(   NULL   );
   }

   STOS_SemaphoreWait(&TDAL_FS_AtapiAccessSem);

   if   (Instance_p->Handle_p != NULL)
   {
      Instance_p->Handle_p->ATAPI_HandlesOpened++;

      mTBOX_TRACE((kTBOX_NIV_1,"pTDAL_FS_AtapiWrap_Open:   [%08X]   Already   opened\n",   Instance_p->Handle_p));
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   (dev_handle_t   *)Instance_p->Handle_p   );
   }

   /*   allocate   an   internal   handle   */
   if   ((Handle_p = (tTDAL_FS_AtapiHandle   *)STOS_MemoryAllocate(system_partition,
                              sizeof(tTDAL_FS_AtapiHandle))) == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   can't   allocate   Handle   structure\n"));
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   memset(Handle_p,   '\0',   sizeof(tTDAL_FS_AtapiHandle));

   /*   link   the   Instance_p   with   the   handle   */
   Handle_p->Instance_p = Instance_p;

   /*   probe   to   open   the   ATAPI   device   */
   OpenParams.DeviceAddress = STATAPI_DEVICE_0;

   st_err = STATAPI_Open(Instance_p->DeviceName,   &OpenParams,   &Handle_p->ATAPI_Handle);
   if   (st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   STATAPI_Open   failed   (st_err=%d)\n",   st_err));
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   /*   store   the   ATAPI   handle   in   a   global   variale   to   be   used   by   application   further   */
   /*ATAPI_Handle = Handle_p->ATAPI_Handle;*/

   st_err = STATAPI_GetDriveType(Handle_p->ATAPI_Handle,   &Type);
   if   ((st_err != ST_NO_ERROR) || (Type != STATAPI_ATA_DRIVE))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   ATAPI_GetDriveType   failed   (st_err=%x)\n",   st_err));
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   /*   set   up   the   transfer   protocol   */
   st_err = STATAPI_SetPioMode(Handle_p->ATAPI_Handle,   STATAPI_PIO_MODE_4);
   if   (st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   ATAPI_SetPioMode   failed   (st_err=%x)\n",   st_err));
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   /*   set   up   the   standard   feature   for   one   ATAPI   device
      to   read   the   device   features   */
   Cmd.CmdCode   = STATAPI_CMD_SET_FEATURES;
   Cmd.SectorCount = 0;

   for   (   CmdCount = 0;   CmdCount   <   sizeof(CmdArray);   CmdCount++   )
   {
      Cmd.Features = CmdArray[CmdCount];

      st_err = STATAPI_CmdNoData(Handle_p->ATAPI_Handle,
                   &Cmd,
                   &CmdStatus);
      if   (st_err != ST_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   ATAPI_CmdNoData(   SET_FEATURES   )   failed   (st_err=%x)\n",   st_err));
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
      }

      /*   Wait   for   command   complete   */
      STOS_SemaphoreWait(TDAL_FS_AtapiEventSem);
   }


   /*   retrieve   the   max   LBA   addessable   */
   Data_p = STOS_MemoryAllocate(ncache_partition,   SECTOR_SIZE);
   if(Data_p == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"memory   allocate(%d)   failed\n",   SECTOR_SIZE));
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   memset(&Cmd,   0,   sizeof(Cmd));
   Cmd.CmdCode   = STATAPI_CMD_IDENTIFY_DEVICE;
   Cmd.SectorCount = 1;
   Cmd.UseLBA   = TRUE;
   Cmd.LBA       = 0;

   /*   Get   the   disk   size   */
   st_err = STATAPI_CmdIn(Handle_p->ATAPI_Handle,
               &Cmd,
               Data_p,
               SECTOR_SIZE,
               &BytesRead,
               &CmdStatus);
   if(st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   ATAPI_CmdIn(   IDENTIFY_DEVICE   )   failed   (st_err=%x)\n",   st_err));
      STOS_MemoryDeallocate(ncache_partition,   Data_p);
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   /*   Wait   for   command   complete   */
   STOS_SemaphoreWait(TDAL_FS_AtapiEventSem);

   if(BytesRead != SECTOR_SIZE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   ATAPI_CmdIn(   IDENTIFY_DEVICE   )   failed   (BytesRead=%d)\n",   BytesRead));
      STOS_MemoryDeallocate(ncache_partition,   Data_p);
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   /*   device   information   */
   /*   83   F   10   1 = 48-bit   Address   feature   set   supported   */
   if(((*(((U16   *)Data_p)+83))   &   0x0400) != 0)
   {
      /*   HDD   supports   LBA48   addressing,   get   size   from   LBA   data   */
      /*   100-103   V   Maximum   user   LBA   for   48-bit   Address   feature   set   */
      Handle_p->NumBlocks = *((U32   *)(((U16   *)Data_p)+100));
   }
   else
   {
      /*   HDD   does   not   support   LBA48   addressing,   get   size   from   Number   of   sectors   field   */
      /*   60-61   F   Total   number   of   user   addressable   sectors   (LBA   mode   only)   */
      Handle_p->NumBlocks = *((U32   *)(((U16   *)Data_p)+60));
   }
   Handle_p->BlocksSize = SECTOR_SIZE;

   /*   set   device   in   Multiple   mode   if   supported   */
   Cmd.CmdCode   = STATAPI_CMD_SET_MULTIPLE_MODE;
   /*   47   X   15-8   80h
      *   R   7-0   00h = Reserved
      *   F   01h-FFh = Maximum   number   of   sectors   that   shall   be   transferred   per   interrupt   on   READ/WRITE   MULTIPLE   commands
      */
   Cmd.SectorCount = (*(((U16   *)Data_p)+47))   &   0xFF;

   st_err = STATAPI_CmdNoData(Handle_p->ATAPI_Handle,
                 &Cmd,
                 &CmdStatus);
   if   (st_err != ST_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Open:   ATAPI_CmdNoData(   SET_MULTIPLE_MODE   )   failed   (st_err=%x)\n",   st_err));
      STOS_MemoryDeallocate(ncache_partition,   Data_p);
      STATAPI_Close(Handle_p->ATAPI_Handle);
      STOS_MemoryDeallocate(system_partition,   Handle_p);
      STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);
      mTBOX_RETURN(   NULL   );
   }

   /*   Wait   for   command   complete   */
   STOS_SemaphoreWait(TDAL_FS_AtapiEventSem);


   STOS_MemoryDeallocate(ncache_partition,   Data_p);

   /*   link   the   handle   with   the   Instance_p   */
   Instance_p->Handle_p = Handle_p;

   Handle_p->ATAPI_HandlesOpened++;

   STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);

   mTBOX_RETURN(   (dev_handle_t   *)Handle_p   );
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_Close
            This method is passed the handle to an open device Instance_p,
            previously returned by a call to the open method. It is
            responsible for tearing down any data structures it had
            associated with the open device handle.
 * Input   : handle - handle on opened Instance_p
 * Output   :
 * Return   : 0 - success, -1 - failure
 * ----------------------------------------------------------------------*/
LOCAL   int   pTDAL_FS_AtapiWrap_Close(dev_handle_t   *DevHandle_p)
{
   ST_ErrorCode_t   st_err = ST_NO_ERROR;
   tTDAL_FS_AtapiHandle   *Handle_p = (tTDAL_FS_AtapiHandle   *)DevHandle_p;
   S32        RetCode = OSPLUS_FAILURE;


   mTBOX_FCT_ENTER(("pTDAL_FS_AtapiWrap_Close"));

   /*   check   the   input   parameter   */
   if   (Handle_p == NULL)
   {
      mTBOX_RETURN(   RetCode   );
   }

   STOS_SemaphoreWait(&TDAL_FS_AtapiAccessSem);

   if   (Handle_p->ATAPI_HandlesOpened   >   1)
   {
      /*   there   are   more   than   one   opened   ATAPI   handles.   */
      Handle_p->ATAPI_HandlesOpened--;
      RetCode = OSPLUS_SUCCESS;
   }
   else   if   (Handle_p->ATAPI_HandlesOpened == 1)
   {
      /*   there   is   only   one   ATAPI   Instance_p   opened   */
      st_err = STATAPI_Close(Handle_p->ATAPI_Handle);
      if   (st_err == ST_NO_ERROR)
      {
      /*   unlink   Instance_p   to   handle   */
      Handle_p->Instance_p->Handle_p = NULL;

      STOS_MemoryDeallocate(system_partition,   Handle_p);

      Handle_p->ATAPI_HandlesOpened--;
      RetCode = OSPLUS_SUCCESS;
      }
   }

   STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);

   mTBOX_RETURN(   RetCode   );
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_Ioctl
            This method performs device control operations. If a device
            does not support this type of access, its ioctl method should be
            NULL in the method table. It is passed the handle to an open
            device Instance_p, a function to perform and an associated
            parameter.
 * Input   : handle - handle on opened Instance_p, Request, arg
 * Output   :
 * Return   : 0 - success, -1 - failure
 * ----------------------------------------------------------------------*/
LOCAL   int   pTDAL_FS_AtapiWrap_Ioctl(   dev_handle_t   *DevHandle_p,
                     S32   Request,   void   *Size_p)
{
   tTDAL_FS_AtapiHandle   *Handle_p = (tTDAL_FS_AtapiHandle   *)DevHandle_p;

   mTBOX_FCT_ENTER(("pTDAL_FS_AtapiWrap_Ioctl"));

   /*   check   the   input   parameter   */
   if   (Handle_p == NULL || Size_p == NULL)
   {
      mTBOX_RETURN(   OSPLUS_FAILURE   );
   }

   switch(Request)
   {
      case   OSPLUS_IOCTL_NUM_BLOCKS:
      /*mTBOX_PRINT((kTBOX_LF,"Ioctl(   OSPLUS_IOCTL_NUM_BLOCKS   )\n"));*/
      *((U32   *)Size_p) = Handle_p->NumBlocks;
      break;
      
      case   OSPLUS_IOCTL_BLOCK_SIZE:
      /*mTBOX_PRINT((kTBOX_LF,"Ioctl(   OSPLUS_IOCTL_BLOCK_SIZE   )\n"));*/
      *((U32   *)Size_p) = Handle_p->BlocksSize;
      break;
      
      default:
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_Ioctl:   Request   not   supported\n"));
      mTBOX_RETURN(   OSPLUS_FAILURE   );
   }

   mTBOX_RETURN(   OSPLUS_SUCCESS   );
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_BlockRead
            This method performs a block oriented read from the device. If
            a device does not support this type of access, its block read
            method should be NULL in the method table. It is passed the
            handle to an open device Instance_p, a destination Buffer_p, a
            logical block address, and a length expressed as a number of
            blocks to read.
 * Input   : handle    - handle on opened Instance_p
           Buffer_p    - data read
           LBA       - logical address to be read
           NumBlocks - number of block to be read
 * Output   :
 * Return   : blocks read
 * ----------------------------------------------------------------------*/
static   int   pTDAL_FS_AtapiWrap_BlockRead(dev_handle_t   *DevHandle_p,   void   *Buffer_p,   lba_t   LBA,   size_t   NumBlocks)
{
   ST_ErrorCode_t      st_err = ST_NO_ERROR;
   tTDAL_FS_AtapiHandle      *Handle_p = (tTDAL_FS_AtapiHandle   *)DevHandle_p;

   STATAPI_Cmd_t      ReadCmd;
   STATAPI_CmdStatus_t   CmdStatus;
   U8            *TempBuffer_p;
   U32            BytesRead,   BlocksRead,   SectorCount;
   BOOL        EnableCrypt = FALSE;


   mTBOX_FCT_ENTER(("pTDAL_FS_AtapiWrap_BlockRead"));

   /*   check   the   input   parameters   */
   if   ((Handle_p == NULL) || (Buffer_p == NULL))
   {
      mTBOX_RETURN(   OSPLUS_FAILURE   );
   }

   /*   flush   cache   if   the   Buffer   is   cached   */
   if(osplus_uncached((void   *)Buffer_p,   NumBlocks*SECTOR_SIZE)==0)
      cache_flush_data(NULL,   NULL);

   /*   Wait   on   access   sem   */
   STOS_SemaphoreWait(&TDAL_FS_AtapiAccessSem);

   memset(&ReadCmd,   0,   sizeof(STATAPI_Cmd_t));

   BlocksRead = 0;
   TempBuffer_p = Buffer_p;

   while(BlocksRead   <   NumBlocks)
   {
      SectorCount = ((NumBlocks   -   BlocksRead)   >=   MAX_SECTORS_FOR_MUL_RW)   ?   MAX_SECTORS_FOR_MUL_RW   :   (NumBlocks   -   BlocksRead);

      /*   Setup   command   to   send.   */
      ReadCmd.CmdCode   = (LBA   >   MAX_LBA_FOR_SECTOR_ACCESS)   ?   STATAPI_CMD_READ_MULTIPLE_EXT   :   STATAPI_CMD_READ_MULTIPLE;
      ReadCmd.Features = (EnableCrypt == TRUE)   ?   0xFF   :   0x00;
      ReadCmd.UseLBA   = TRUE;
      ReadCmd.LBA       = LBA;
      ReadCmd.LBAExtended = 0;
      ReadCmd.SectorCount = SectorCount;

      /*   Running   command   to   Read   data.   */
      st_err = STATAPI_CmdIn(   Handle_p->ATAPI_Handle,
                 &ReadCmd,
                 TempBuffer_p,
                 SectorCount*SECTOR_SIZE,
                 &BytesRead,
                 &CmdStatus);
      if   (st_err != ST_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_BlockRead:   ATAPI_CmdIn()   failed   (st_err=%d)\n",   st_err));
      break;
      }

      /*   Wait   for   command   complete   */
      STOS_SemaphoreWait(TDAL_FS_AtapiEventSem);

      if   (BytesRead == (SectorCount*SECTOR_SIZE))
      {
      LBA      +=   SectorCount;
      BlocksRead   +=   SectorCount;
      TempBuffer_p   +=   BytesRead;
      }
      else
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_BlockRead:   ATAPI_CmdIn()   failed   (   %d != %d   )\n",   SectorCount*SECTOR_SIZE,   BytesRead));
      break;
      }
   }

   /*   Signal   access   sem   */
   STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);

   mTBOX_TRACE((kTBOX_NIV_5,"pTDAL_FS_AtapiWrap_BlockRead():   [%d]\n",   BlocksRead));

   mTBOX_RETURN(   BlocksRead   );
}


/*-------------------------------------------------------------------------
 * Function : pTDAL_FS_AtapiWrap_BlockWrite
            This method performs a block oriented write to the device. If a
            device does not support this type of access, its block write
            method should be NULL in the method table. It is passed the
            handle to an open device Instance_p, a source Buffer_p, a logical
            block address, and a length expressed as a number of blocks to
            write.
 * Input   : handle    - handle on opened Instance_p
           Buffer_p    - data write
           LBA       - logical address to write
           NumBlocks - number of block to write
 * Output   :
 * Return   : blocks write
 * ----------------------------------------------------------------------*/
static   int   pTDAL_FS_AtapiWrap_BlockWrite(dev_handle_t   *DevHandle_p,   const   void   *Buffer_p,   lba_t   LBA,   size_t   NumBlocks)
{
   ST_ErrorCode_t      st_err = ST_NO_ERROR;
   tTDAL_FS_AtapiHandle      *Handle_p = (tTDAL_FS_AtapiHandle   *)DevHandle_p;

   STATAPI_Cmd_t      WriteCmd;
   STATAPI_CmdStatus_t   CmdStatus;
   U8            *TempBuffer_p;
   U32            BytesWritten,   BlocksWritten,   SectorCount;
   BOOL        EnableCrypt = FALSE;


   mTBOX_FCT_ENTER(("pTDAL_FS_AtapiWrap_BlockWrite"));

   /*   check   the   input   parameters   */
   if   ((Handle_p == NULL) || (Buffer_p == NULL))
   {
      mTBOX_RETURN(   OSPLUS_FAILURE   );
   }

   /*   flush   cache   if   the   Buffer   is   cached   */
   if(osplus_uncached((void   *)Buffer_p,   NumBlocks*SECTOR_SIZE)==0)
      cache_flush_data(NULL,   NULL);

   /*   Wait   on   access   sem   */
   STOS_SemaphoreWait(&TDAL_FS_AtapiAccessSem);

   memset(&WriteCmd,   0,   sizeof(STATAPI_Cmd_t));

   BlocksWritten = 0;
   TempBuffer_p = (U8   *)Buffer_p;

   while(BlocksWritten   <   NumBlocks)
   {
      SectorCount = ((NumBlocks   -   BlocksWritten)   >=   MAX_SECTORS_FOR_MUL_RW)   ?   MAX_SECTORS_FOR_MUL_RW   :   (NumBlocks   -   BlocksWritten);

      /*   Setup   command   to   send.   */
      WriteCmd.CmdCode   = (LBA   >   MAX_LBA_FOR_SECTOR_ACCESS)   ?   STATAPI_CMD_WRITE_MULTIPLE_EXT   :   STATAPI_CMD_WRITE_MULTIPLE;
      WriteCmd.Features = (EnableCrypt == TRUE)   ?   0xFF   :   0x00;
      WriteCmd.UseLBA   = TRUE;
      WriteCmd.LBA       = LBA;
      WriteCmd.LBAExtended = 0;
      WriteCmd.SectorCount = SectorCount;

      /*   Running   command   to   write   data.   */
      st_err = STATAPI_CmdOut(Handle_p->ATAPI_Handle,
                   &WriteCmd,
                   TempBuffer_p,
                   SectorCount*SECTOR_SIZE,
                   &BytesWritten,
                   &CmdStatus);
      if   (st_err != ST_NO_ERROR)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_BlockWrite:   ATAPI_CmdOut()   failed   (st_err=%d)\n",   st_err));
      break;
      }

      /*   Wait   for   command   complete   */
      STOS_SemaphoreWait(TDAL_FS_AtapiEventSem);

      if   (BytesWritten == (SectorCount*SECTOR_SIZE))
      {
      LBA      +=   SectorCount;
      BlocksWritten   +=   SectorCount;
      TempBuffer_p   +=   BytesWritten;
      }
      else
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_AtapiWrap_BlockWrite:   ATAPI_CmdOut()   failed   (%d != %d)\n",   SectorCount*SECTOR_SIZE,   BlocksWritten));
      break;
      }
   }

   /*   Signal   access   sem   */
   STOS_SemaphoreSignal(&TDAL_FS_AtapiAccessSem);

   mTBOX_TRACE((kTBOX_NIV_5,"pTDAL_FS_AtapiWrap_BlockWrite():   [%d]\n",   BlocksWritten));

   mTBOX_RETURN(   BlocksWritten   );
}

#endif


/*   EOF   ---------------------------------------------------------------------   */
