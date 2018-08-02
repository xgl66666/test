/******************************************************************************
 *                   COPYRIGHT 2004 IWEDIA TECHNOLOGIES                   *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_DISP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_disp/tdal_disp_priv.h $
 *          $Rev: 414 $
 *          $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL DISP private header
 *
 *****************************************************************************/

#ifndef _TDAL_DISP_PRIV_H_
#define _TDAL_DISP_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************
 *  Includes                                       *
 *******************************************************/

/*******************************************************
 *  Defines                                       *
 *******************************************************/
#define kTDAL_DISPi_NB_LAYER_VIDEO          1
#define kTDAL_DISPi_NB_BLENDER              1
#define kTDAL_DISPi_BLENDER_ID_0_NB_LAYER   2
#define kTDAL_DISPi_BLENDER_ID_1_NB_LAYER   3
#define kTDAL_DISPi_TOTAL_BLENDER_NB_LAYER   kTDAL_DISPi_BLENDER_ID_0_NB_LAYER + kTDAL_DISPi_BLENDER_ID_1_NB_LAYER


/*******************************************************
 *  Macros                                        *
 *******************************************************/

/*******************************************************
 *  Typedefs                                       *
 *******************************************************/

/* Array offsets for LAYER devices */
enum
{
    LAYER_VIDEO_MAIN,
    LAYER_VIDEO_AUX,
    LAYER_OSD_MAIN_1,
    LAYER_OSD_MAIN_2,
    LAYER_OSD_AUX,
    LAYER_OSD_CURSOR,
    LAYER_STILL,
    LAYER_NB_DEVICES
};

/* There is one tTDAL_DISPi_Display for MAIN and one for AUX */
/* Parameters for OSD Display are duplicated and the duplicated parameters are only used if a second layer is created on MAIN */
typedef struct
{
   uint32_t                 memBitmapHandle;
   //STGXOBJ_Bitmap_t         bitmapDst;

   uint32_t                 memBitmapHandle2;
   //STGXOBJ_Bitmap_t         bitmapDst2;

   //STVTG_TimingMode_t       vtgTiming;
   //STVTG_ModeParams_t       vtgParams;

   tTDAL_DISP_LayerWindow   inputWindow;
   tTDAL_DISP_LayerWindow   outputWindow;

   tTDAL_DISP_LayerWindow   inputWindow2;
   tTDAL_DISP_LayerWindow   outputWindow2;
} tTDAL_DISPi_Display;

typedef struct
{
   tTDAL_DISP_BlenderId                 Id;
   //STOS_Semaphore_t                     *Lock;
   tTDAL_DISP_BlenderCapability         stCapability;
   uint32_t                             Width;
   uint32_t                             Height;
   bool                                 IsEnabled;
   tTDAL_DISP_BlenderZOrderPositions    *pstZOrderPositionList;
   tTDAL_DISP_LayerId                   *pstZOrderCurrentList;
   tTDAL_DISP_BlenderWindow             stBlenderOutputWindow;
}tTDAL_DISPi_BlenderDescriptorElements;

typedef struct
{
   uint32_t                             STUnitNumber;
}tTDAL_DISPi_VideoLayerDescriptor;

typedef struct
{
   tTDAL_DISP_LayerId               Id;
   tTDAL_DISP_LayerCapability       stCapability;
   bool                             IsEnabled;
   bool                             bReadyToEnable;
   uint8_t                          Transparency;
   tTDAL_DISP_LayerWindow           stInputWindow;
   tTDAL_DISP_LayerWindow           stRequestedOutputWindow;
   tTDAL_DISPi_VideoLayerDescriptor *pstVideoLayerDesc;
   uint8_t                           GFXLayerDesc;
   uint8_t                           gwinID;
}tTDAL_DISPi_LayerDescriptorElements;

/*******************************************************
 *  Variables Declarations (IMPORT)                  *
 *******************************************************/
//IMPORT   STAVMEM_PartitionHandle_t              SYS_AVMEMPartitionHandle[];

IMPORT tTDAL_DISPi_BlenderDescriptorElements    TDAL_DISP_BlenderDescriptor[];
IMPORT tTDAL_DISPi_LayerDescriptorElements      TDAL_DISP_LayerDescriptor[];
IMPORT tTDAL_DISPi_VideoLayerDescriptor         TDAL_DISP_VideoLayerDescriptor[];

IMPORT tTDAL_DISP_BlenderZOrderPositions        TDAL_DISP_InitZOrderListBlender0[];
IMPORT tTDAL_DISP_BlenderZOrderPositions        TDAL_DISP_InitZOrderListBlender1[];

IMPORT bool                                     TDAL_DISP_AlreadyInitialized;


IMPORT  void        TDAL_DISP_LayerInit(void);
IMPORT  void        TDAL_DISP_LayerTerminate(void);
IMPORT  void        TDAL_DISP_BlenderInit(void);
IMPORT  void        TDAL_DISP_BlenderTerminate(void);

/*******************************************************
 *  Internal Functions Declarations                  *
 *******************************************************/

#ifdef __cplusplus
}
#endif

#endif /*_TDAL_DISP_PRIV_H_*/
