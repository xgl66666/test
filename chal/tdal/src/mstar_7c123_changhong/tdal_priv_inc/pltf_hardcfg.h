/*****************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_COMMON
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/pltf_hardcfg.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : ST Common TDAL library implementation.
 *
 ****************************************************************************/

#ifndef   _BOOT_HARD_CFG_H_
#define   _BOOT_HARD_CFG_H_

//#include "idecodehnt7105_7105.h"
#include "MsTypes2.h"

#ifdef   __cplusplus
extern   "C"   {
#endif

/*--------------------------*/
/*   MODE   HD   by   DEFAULT      */
/*--------------------------*/
#define   MODE_YUV

/*   Array   offsets   for   VOUT   devices   */
enum
{
   VOUT_RGB,
   VOUT_YUV,
   VOUT_CVBS,
   VOUT_YC,
   VOUT_HDMI,
   VOUT_YUV_HD,
   VOUT_RGB_HD,
   VOUT_NB_DEVICES
};

#ifdef MODE_YUV
#define VOUT_XXX_HD VOUT_YUV_HD
#else
#define VOUT_XXX_HD VOUT_RGB_HD
#endif

/*   Array   offsets   for   MIXER   devices   */
#if   defined(AUXILIARY_VIDEO_DISPLAY_SD)
   #define   VIDEO_MIXER_NB_DEVICES   2
   #define   LAYER_OSD_NB_DEVICES   3
   #define   VTG_NB_DEVICES      2
#else
   #define   VIDEO_MIXER_NB_DEVICES   1
   #define   LAYER_OSD_NB_DEVICES   2
   #define   VTG_NB_DEVICES      1
#endif

enum
{
   VIDEO_MIXER1,
   VIDEO_MIXER2
};

/*   Array   offsets   for   VTG   devices   */
enum
{
   VTG1,
   VTG2
};



#if defined(TDAL_FRONTEND_STV4100)
    #define USE_PTI_ON_TSIN0
#elif defined(TDAL_FRONTEND_MXL603)
    #define USE_PTI_ON_TSIN1
#else
    #define USE_PTI_ON_TSIN0
#endif


/*--------------------------*/
/*   I2C   Driver   definitions   */
/*--------------------------*/

#define   kSYS_BACK_I2C   2
#define   kSYS_FRONT_I2C   1
#define   kSYS_EXT_I2C   1
#define   kSYS_AVS_I2C   1

/*--------------------------*/
/*   PIO   Driver   definitions   */
/*--------------------------*/


#define   NUM_PIO_PORTS   17

#define   BACK_PIO   3   /*   I2C   */

#define   FRONT_PIO   3/*   FORCE   */

#define   PWM_PIO   1


#define   kBAND_LIST_MAX   4
#define   kSCAN_LIST_MAX   8
#define   kSIGNAL_LIST_MAX   3

      /*   Array   offsets   for   I2C   devices   */
enum
{
   I2C_BACK_BUS,
   I2C_FRONT_BUS,
   I2C_EXT_BUS,
   I2C_DEVICES
};

/*   Array   offsets   for   AVMEM   devices   */
enum
{
   AVMEM_LMI_SYS,
   AVMEM_LMI_VID,
   AVMEM_SECURE,
   AVMEM_NB_DEVICES
};

/*   Array   offsets   for   PTI   devices   */
enum
{
   PTI_PLAY   =   0,   /*   PTI   used   to   view   audio/video   from   live   or   recorded   streams   */
   PTI_REC1,      /*   PTI   used   to   record   a   TS   stream   */
   PTI_REC2,      /*   PTI   used   to   record   a   TS   stream   */
   PTI_NB_DEVICES
};

/*   Array   offsets   for   TUNER   devices   */
enum
{
   TUNER1,
#if   defined   (NB_TUNERS)
#if   (NB_TUNERS==2)
   TUNER2,
#endif
#endif
   TUNER_NB_DEVICES
};

/*----------------------*/
/*   Port   pins   for   PIO   */
/*----------------------*/
#define   ASC_0_TXD_BIT   (1<<0)        /*   Port   0   */
#define   ASC_0_RXD_BIT   (1<<1)        /*   Port   0   */

#define   ASC_1_TXD_BIT   (1<<0)        /*   Port   1   */
#define   ASC_1_RXD_BIT   (1<<1)        /*   Port   1   */

#define   ASC_2_TXD_BIT   (1<<4)        /*   Port   4   */
#define   ASC_2_RXD_BIT   (1<<3)        /*   Port   4   */

#define   ASC_3_TXD_BIT   (1<<4)        /*   Port   5   */
#define   ASC_3_RXD_BIT   (1<<5)        /*   Port   5   */
#define   ASC_3_CTS_BIT   (1<<6)        /*   Port   5   */
#define   ASC_3_RTS_BIT   (1<<7)        /*   Port   5   */

#define   ASC_4_TXD_BIT   (1<<1)
#define   ASC_4_RXD_BIT   (1<<2)
#define   ASC_4_CTS_BIT   (1<<3)
#define   ASC_4_RTS_BIT   (1<<0)

/*   UART   Driver   definitions   */
#define   kSYS_ASC0   0
#define   kSYS_ASC1   1
#define   kSYS_ASC2   2
#define   kSYS_ASC3   3

#define   kMAX_ASC   4

/*----------------------*/
/*   BLAST   PIOs   */
/*----------------------*/

#define   BLAST_PIO_DEV   3
#define   BLAST_PIO_BIT   PIO_BIT_0


/*----------------------*/
/*   STCLKRV   offset      */
/*----------------------*/
/*   For   7100/7109,   add   -80ms   in   the   STC   due   to   latency   of   the   staudlx   decoder   to   decode   the   audio   frames   */
#define   CLKRV_STC_OFFSET      (((-80)*90000)/1000)


/*   -----------------------   */
/*   TUNER   Driver   definition   */
/*   -----------------------   */

/*   Demod   Manager   initialisation   values   */
#define   kSYS_NB_DEMOD   1
#define   kSYS_DEMOD_TIMEOUT   0

#define   kSYS_TUNER_LOW_FREQ   950000
#define   kSYS_TUNER_HIGH_FREQ   2150000
#define   kSYS_TUNER_STEP_FREQ   0
#define   kBOOT_NUM_PTI_SESSIONS   3

/*   ------------   */
/*   EEPROM   defines   */
/*   ------------   */
#define   kSYS_EEP_SIZEOF_EEPROM   ((32   *   1024)   -   4)   /*   ATMEL   24C256N   in   use   ,   Top   4   bytes   for   MFG   id   */
#define   kSYS_EEP_SIZEOF_PAGE   64
#define   kSYS_EEP_SUPPORTED_DEVICE   0xA0

/*   ------------   */
/*   DENC   defines   */
/*   ------------   */


/*   --------------   */
/*   MEMORY   Defines   */
/*   --------------   */
#ifdef   BOOTLOADER
   #define   kSYSTEM_MEMORY_SIZE   0x01400000

#elif   defined(__FULL_DEBUG__)   ||   defined(__DEBUG_TRACE__)   ||   defined(__DEBUG_STATS__)
   #define   kSYSTEM_MEMORY_SIZE   0x01200000   /*   18   Mo   */
#else
   #define   kSYSTEM_MEMORY_SIZE   0x01200000   /*   18   Mo   */
#endif


#ifdef   PACKAGE_PVR
   #define   kNCACHE_MEMORY_SIZE   0x00500000   /*   5Mo   */
#else
   #define   kNCACHE_MEMORY_SIZE   0x00300000   /*   3   Mo   _   Used   by   MPGFX   */
#endif

#ifdef   __cplusplus
}
#endif


/*   Define   TDAL   Task   Priority   */
#define   TDAL_OS20_MAX_USER_PRIORITY   15

#if   defined   (ST_OS20)
#define   TDAL_MAX_USER_PRIORITY   15
#elif   defined   (ST_OS21)
#define   TDAL_MAX_USER_PRIORITY   255
#elif   defined   (ST_OSLINUX)
#define   TDAL_MAX_USER_PRIORITY   99
#endif
#define   TDAL_PRIORITY(x)   ((x*TDAL_MAX_USER_PRIORITY)   /   TDAL_OS20_MAX_USER_PRIORITY)
#define   kTDAL_DMD_SCAN_TASK_PRIO        TDAL_PRIORITY(2)
#define   kTDAL_DMD_FRONT_END_TASK_PRIO      TDAL_PRIORITY(7)
#define   kTDAL_DMX_TASK_DELETION_PRIORITY      TDAL_PRIORITY(7)   /*   deletion   task   priority   */
#define   kTDAL_DMX_TASK_SECTION_PRIORITY      TDAL_PRIORITY(10)   /*   reception   task   priority   */
#define   kTDAL_AV_EVT_TASK_PRIO            TDAL_PRIORITY(5)
#define   kTDAL_AV_AUDIO_SAMPLE_INJ_TASK_PRIO      TDAL_PRIORITY(9)
#define   kTDAL_KBD_TASK_PRIORITY            TDAL_PRIORITY(7)
#define   kTDAL_PTSM_SCAN_TASK_PRIO        TDAL_PRIORITY(3)   /*   prio   max   =   15   with   OS20   */
#define   kTDAL_OUTPUTi_SendInfoFrameTask_PRIO   TDAL_PRIORITY(5)
#define   kTDAL_TSSINK_THREAD_PRIO        TDAL_PRIORITY(10)
#define   kTDAL_TSSOURCE_THREAD_PRIO            TDAL_PRIORITY(8)
#define   kTDAL_FS_MSGQUEUE_TASKPRIO        TDAL_PRIORITY(8)
#define   kTDAL_AVS_SLOWBLANKINK_TASK_PRIO      TDAL_PRIORITY(3)
#define   kTDAL_SCART_INPUT_SLOWBLANKING_TASK_PRIO   TDAL_PRIORITY(3)
#define   kTDAL_AV_VIDEO_STATISTICS_TASK_PRIORITY   TDAL_PRIORITY(3)
#define   kTDAL_HDMI_EVT_TASK_PRIO        TDAL_PRIORITY(3)
#if   defined(ST_OSLINUX)
   #define   kTDAL_FS_NOTIFICATION_TASK_PRIO      TDAL_PRIORITY(6)
   #define   kTDAL_FS_POLLING_TASK_PRIO      TDAL_PRIORITY(6)
#else
   #define   kTDAL_FS_TASK_PRIO            TDAL_PRIORITY(6)
#endif
#define   kTDAL_AV_VIDEO_VIDEOFREEZEDWATCHDOG_TASK_PRIORITY   TDAL_PRIORITY(8)


#if   defined(ST_OSLINUX)
/*--------#define   TASK   STACK   SIZE   FOR   STLINUX   ------*/

#define   kTDAL_DMD_SCAN_TASK_STACK_SIZE      (8192   )
#define   kTDAL_DMD_FRONT_END_TASK_STACK_SIZE   (4096   )
#define   RT_TASK_STACK_SIZE            (1024   )
#ifdef   __DEBUG_TRACE__
   #define   kTDAL_DMX_TASK_STACK_SIZE      (4096)   /*   stack   size   of   a   task   */
#else   /*   __DEBUG_TRACE__   */
   #define   kTDAL_DMX_TASK_STACK_SIZE      (2048)   /*   stack   size   of   a   task   */
#endif   /*   __DEBUG_TRACE__   */
#define   kTDAL_AV_EVT_TASK_STACK_SIZE        (4196)
#define   kTDAL_AV_AUDIO_SAMPLE_INJ_TASK_STACK_SIZE   (2048)
#define   kTDAL_AVS_TASK_STACKSIZE            (2048)
#define   kTDAL_SCART_INPUT_TASK_STACKSIZE      (2048)
#define   kTDAL_AV_VIDEO_STATISTICS_TASK_STACK_SIZE   (2048)
#define   kTDAL_PTSM_SCAN_TASK_STACK_SIZE        (4096)
#define   kTDAL_KBD_TASK_STACK_SIZE            (2048)
#define   kTDAL_HDMI_EVT_TASK_STACK_SIZE        (4096)
#define   kTDAL_FS_NOTIFICATION_TASK_STACK_SIZE      (4096)
#define   kTDAL_FS_POLLING_TASK_STACK_SIZE      (4096)
#define   kTDAL_TSSOURCE_TASK_STACK_SIZE        (4096)
#define   kTDAL_TSSINK_TASK_STACK_SIZE        (16*1024)


#else

/*--------#define   TASK   STACK   SIZE   FOR   OS21   ------*/
#ifndef   OS21_DEF_MIN_STACK_SIZE
#define   OS21_DEF_MIN_STACK_SIZE   0
#endif


#define   kTDAL_DMD_SCAN_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE   )
#define   kTDAL_DMD_FRONT_END_TASK_STACK_SIZE   (OS21_DEF_MIN_STACK_SIZE   )
#define   RT_TASK_STACK_SIZE            (OS21_DEF_MIN_STACK_SIZE   )
#ifdef   __DEBUG_TRACE__
   #define   kTDAL_DMX_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE)   /*   stack   size   of   a   task   */
#else   /*   __DEBUG_TRACE__   */
   #define   kTDAL_DMX_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE)   /*   stack   size   of   a   task   */
#endif   /*   __DEBUG_TRACE__   */
#define   kTDAL_AV_EVT_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_AV_AUDIO_SAMPLE_INJ_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE   )
#define   kTDAL_AVS_TASK_STACKSIZE      (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_SCART_INPUT_TASK_STACKSIZE      (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_AV_VIDEO_STATISTICS_TASK_STACK_SIZE   (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_PTSM_SCAN_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_KBD_TASK_STACK_SIZE        (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_HDMI_EVT_TASK_STACK_SIZE      (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_FS_TASK_STACK_SIZE        (OS21_DEF_MIN_STACK_SIZE)
#define   kTDAL_TSSOURCE_TASK_STACK_SIZE      (32*1024)
#define   kTDAL_TSSINK_TASK_STACK_SIZE      (32*1024)
#define   kTDAL_AV_VIDEO_VIDEOFREEZEDWATCHDOG_TASK_STACK_SIZE   (OS21_DEF_MIN_STACK_SIZE)

#endif   /*ST_OSLINUX*/

void   SYSm_WriteRegDev32LE(U32   PhysicalAddress,U32   Value);
U32   SYSm_ReadRegDev32LE(U32   PhysicalAddress);


#endif   /*_BOOT_HARD_CFG_H_*/

