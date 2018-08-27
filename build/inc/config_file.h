/**************************************************************************//**
** @file config_file.h
**
** @brief
**   This file defines the API to read the configuration file stored in flash.
**  
** @ingroup CONFIGURATION
**
** @version $Rev: 10478 $
**          $URL: http://ren-svn-01/svncomedia/idecode/product/downloader_hnt/downloader_main/trunk/build/inc/config_file.h $
**          $Date: 2012-06-20 12:34:21 +0200 (mer., 20 juin 2012) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2011 SmarDTV
**
******************************************************************************/

#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H

/******************************************************************************
* Includes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Defines
******************************************************************************/

/**
 *  @brief
 *    Serial Location pre-defined values.
 *
*/
#define kCFGF_SERIAL_PARITY_NONE      0x00
#define kCFGF_SERIAL_PARITY_ODD       0x01
#define kCFGF_SERIAL_PARITY_EVEN      0x02
#define kCFGF_SERIAL_FCTRL_NONE       0x00
#define kCFGF_SERIAL_FCTRL_HARDW      0x01
#define kCFGF_SERIAL_FCTRL_XONOFF     0x02
#define kCFGF_SERIAL_SPEED_600        0x00
#define kCFGF_SERIAL_SPEED_1200       0x01
#define kCFGF_SERIAL_SPEED_2400       0x02
#define kCFGF_SERIAL_SPEED_4800       0x03
#define kCFGF_SERIAL_SPEED_9600       0x04
#define kCFGF_SERIAL_SPEED_14400      0x05
#define kCFGF_SERIAL_SPEED_19200      0x06
#define kCFGF_SERIAL_SPEED_38400      0x07
#define kCFGF_SERIAL_SPEED_57600      0x08
#define kCFGF_SERIAL_SPEED_115200     0x09

/**
 *  @brief
 *    DVB-T (and DVB-T2) Location pre-defined values.
 *
 *    Note: These pre-defined values correspond to NVM exchange
 *          enumerates values: tOAD_Modulation, tOAD_TerMode,
 *          tOAD_TerGuard, tOAD_TerHierarchy, tOAD_TerPrioStream,
 *          tOAD_Convolution and tOAD_TerFreqOff.
 *          (see nvm_exchange_mapping_cfg.h)
 *
*/
#define kCFGF_DVBT_MODULATION_BPSK    0x00
#define kCFGF_DVBT_MODULATION_QPSK    0x01
#define kCFGF_DVBT_MODULATION_QAM4    0x02
#define kCFGF_DVBT_MODULATION_8PSK    0x03
#define kCFGF_DVBT_MODULATION_QAM16   0x04
#define kCFGF_DVBT_MODULATION_QAM32   0x05
#define kCFGF_DVBT_MODULATION_QAM64   0x06
#define kCFGF_DVBT_MODULATION_QAM128  0x07
#define kCFGF_DVBT_MODULATION_QAM256  0x08
#define kCFGF_DVBT_MODULATION_AUTO    0x09
#define kCFGF_DVBT_MODULATION_ANALOG  0x0A
#define kCFGF_DVBT_MODE_2K            0x00
#define kCFGF_DVBT_MODE_8K            0x01
#define kCFGF_DVBT_MODE_AUTO          0x02
#define kCFGF_DVBT_MODE_MAX           0x03
#define kCFGF_DVBT_GUARD_1_32         0x00
#define kCFGF_DVBT_GUARD_1_16         0x01
#define kCFGF_DVBT_GUARD_1_8          0x02
#define kCFGF_DVBT_GUARD_1_4          0x03
#define kCFGF_DVBT_GUARD_AUTO         0x04
#define kCFGF_DVBT_GUARD_MAX          0x05
#define kCFGF_DVBT_HIER_NONE          0x00
#define kCFGF_DVBT_HIER_1             0x01
#define kCFGF_DVBT_HIER_2             0x02
#define kCFGF_DVBT_HIER_4             0x03
#define kCFGF_DVBT_HIER_AUTO          0x04
#define kCFGF_DVBT_HIER_MAX           0x05
#define kCFGF_DVBT_PRIO_HIGH          0x00
#define kCFGF_DVBT_PRIO_LOW           0x01
#define kCFGF_DVBT_PRIO_MAX           0x02
#define kCFGF_DVBT_CONVOL_NONE        0x00
#define kCFGF_DVBT_CONVOL_1_2         0x01
#define kCFGF_DVBT_CONVOL_2_3         0x02
#define kCFGF_DVBT_CONVOL_3_4         0x03
#define kCFGF_DVBT_CONVOL_4_5         0x04
#define kCFGF_DVBT_CONVOL_5_6         0x05
#define kCFGF_DVBT_CONVOL_6_7         0x06
#define kCFGF_DVBT_CONVOL_7_8         0x07
#define kCFGF_DVBT_CONVOL_8_9         0x08
#define kCFGF_DVBT_CONVOL_AUTO        0x09
#define kCFGF_DVBT_CONVOL_MAX         0x0A
#define kCFGF_DVBT_FREQOFF_NONE       0x00
#define kCFGF_DVBT_FREQOFF_1_6_MORE   0x01
#define kCFGF_DVBT_FREQOFF_1_6_MINUS  0x02
#define kCFGF_DVBT_FREQOFF_2_6_MORE   0x03
#define kCFGF_DVBT_FREQOFF_2_6_MINUS  0x04
#define kCFGF_DVBT_FREQOFF_3_6_MORE   0x05
#define kCFGF_DVBT_FREQOFF_3_6_MINUS  0x06
#define kCFGF_DVBT_FREQOFF_AUTO       0x07
#define kCFGF_DVBT_FREQOFF_MAX        0x08

#define kCFGF_DVBT2_MODE_2K           0x00
#define kCFGF_DVBT2_MODE_8K           0x01
#define kCFGF_DVBT2_MODE_4K           0x02
#define kCFGF_DVBT2_MODE_1K           0x03
#define kCFGF_DVBT2_MODE_16K          0x04
#define kCFGF_DVBT2_MODE_32K          0x05
#define kCFGF_DVBT2_MODE_AUTO         0x06
#define kCFGF_DVBT2_MODE_MAX          0x07

#define kCFGF_DVBT2_GUARD_1_32        0x00
#define kCFGF_DVBT2_GUARD_1_16        0x01
#define kCFGF_DVBT2_GUARD_1_8         0x02
#define kCFGF_DVBT2_GUARD_1_4         0x03
#define kCFGF_DVBT2_GUARD_1_128       0x04
#define kCFGF_DVBT2_GUARD_19_128      0x05
#define kCFGF_DVBT2_GUARD_19_256      0x06
#define kCFGF_DVBT2_GUARD_AUTO        0x07
#define kCFGF_DVBT2_GUARD_MAX         0x08

/**
 *  @brief
 *    DVB-C Location pre-defined values.
 *
 *    Note: These pre-defined values correspond to NVM exchange
 *          enumerates values: tOAD_Modulation.
 *          (see nvm_exchange_mapping_cfg.h)
*/
#define kCFGF_DVBC_MODULATION_BPSK    0x00
#define kCFGF_DVBC_MODULATION_QPSK    0x01
#define kCFGF_DVBC_MODULATION_QAM4    0x02
#define kCFGF_DVBC_MODULATION_8PSK    0x03
#define kCFGF_DVBC_MODULATION_QAM16   0x04
#define kCFGF_DVBC_MODULATION_QAM32   0x05
#define kCFGF_DVBC_MODULATION_QAM64   0x06
#define kCFGF_DVBC_MODULATION_QAM128  0x07
#define kCFGF_DVBC_MODULATION_QAM256  0x08
#define kCFGF_DVBC_MODULATION_AUTO    0x09
#define kCFGF_DVBC_MODULATION_ANALOG  0x0A

/**
 *  @brief
 *    DVB-S (and DVB-S2) Location pre-defined values.
 *
 *    Note: These pre-defined values correspond to NVM exchange
 *          enumerates values: tOAD_LNB_Polarization, tOAD_Modulation, 
 *          tOAD_Convolution and tOAD_ODU_TRACK.
 *          (see nvm_exchange_mapping_cfg.h)
 *
*/
#define kCFGF_DVBS_POLARIZ_HORIZONTAL 0x00
#define kCFGF_DVBS_POLARIZ_VERTICAL   0x01
#define kCFGF_DVBS_POLARIZ_CIRC_LEFT  0x02
#define kCFGF_DVBS_POLARIZ_CIRC_RIGHT 0x03
#define kCFGF_DVBS_POLARIZ_ALL        0x04
#define kCFGF_DVBS_MODULATION_BPSK    0x00
#define kCFGF_DVBS_MODULATION_QPSK    0x01
#define kCFGF_DVBS_MODULATION_QAM4    0x02
#define kCFGF_DVBS_MODULATION_8PSK    0x03
#define kCFGF_DVBS_MODULATION_QAM16   0x04
#define kCFGF_DVBS_MODULATION_QAM32   0x05
#define kCFGF_DVBS_MODULATION_QAM64   0x06
#define kCFGF_DVBS_MODULATION_QAM128  0x07
#define kCFGF_DVBS_MODULATION_QAM256  0x08
#define kCFGF_DVBS_MODULATION_AUTO    0x09
#define kCFGF_DVBS_MODULATION_ANALOG  0x0A
#define kCFGF_DVBS_FECRATE_NONE       0x00
#define kCFGF_DVBS_FECRATE_1_2        0x01
#define kCFGF_DVBS_FECRATE_2_3        0x02
#define kCFGF_DVBS_FECRATE_3_4        0x03
#define kCFGF_DVBS_FECRATE_4_5        0x04
#define kCFGF_DVBS_FECRATE_5_6        0x05
#define kCFGF_DVBS_FECRATE_6_7        0x06
#define kCFGF_DVBS_FECRATE_7_8        0x07
#define kCFGF_DVBS_FECRATE_8_9        0x08
#define kCFGF_DVBS_FECRATE_AUTO       0x09
#define kCFGF_DVBS_FECRATE_MAX        0x0A
#define kCFGF_DVBS_OUTSEL_NONE        0x00
#define kCFGF_DVBS_OUTSEL_TRACK1      0x01
#define kCFGF_DVBS_OUTSEL_TRACK2      0x02
#define kCFGF_DVBS_OUTSEL_TRACK3      0x03
#define kCFGF_DVBS_OUTSEL_TRACK4      0x04


/**
 *  @brief
 *    OSD Resources pre-defined values.
 *
*/
#define kCFGF_OSD_PICT_BMP            0x00
#define kCFGF_OSD_PICT_BMPRL          0x01
#define kCFGF_OSD_PICT_GIF            0x02
#define kCFGF_OSD_PICT_RAWZIP         0x03
#define kCFGF_OSD_PICT_BMPZIP         0x04


/**
 *  @brief
 *    RF Mod Band pre-defined values.
 *
*/
#define kCFGF_RFMOD_BAND_UHF          0x01
#define kCFGF_RFMOD_BAND_VHF          0x02


/**
 *  @brief
 *    Elements of configuration identifier.
 *    One bit reserved for each element.
 *    A combination (or) of these elements on 32 bits 
 *    given to the read configuration function CFGF_Read
 *    return only the elements specified.
*/
#define kCFGF_ID_PROD_ID        0x00000001
#define kCFGF_ID_DLD_CUST       0x00000002
#define kCFGF_ID_FLASH          0x00000004
#define kCFGF_ID_OSD_RES        0x00000008
#define kCFGF_ID_RFMOD_RES      0x00000010

#define kCFGF_ID_VIDOUT_RES     0x00000020
#define kCFGF_ID_DLD_OPTIONS    0x00000040
#define kCFGF_ID_SCANNING       0x00000080

#define kCFGF_ID_LOC_SERIAL     0x00010000
#define kCFGF_ID_LOC_USB        0x00020000
#define kCFGF_ID_LOC_URL        0x00040000
#define kCFGF_ID_LOC_DVBT       0x00080000
#define kCFGF_ID_LOC_DVBT2      0x00100000
#define kCFGF_ID_LOC_DVBC       0x00200000
#define kCFGF_ID_LOC_DVBS       0x00400000
#define kCFGF_ID_LOC_DVBS2      0x00800000

#define kCFGF_ID_LOC_ALL        0x00FF0000 /* All locations     */
#define kCFGF_ID_ALL            0xFFFFFFFF /* All elements      */


/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Typedefs
******************************************************************************/

typedef enum
{
   eCONFIG_FILE = 0x04280205
} tCFGF_TracesID;


/**
 *  @brief
 *    Product IDs definition.
 *
*/
typedef struct
{
   uint32_t           ui32OUI;         /**< The DVB-OUI (manufacturer OUI) */
   uint16_t           ui16PltfModel;   /**< Platform Model                 */
   uint16_t           ui16PltfVersion; /**< Platform Version               */
   uint16_t           ui16ProdModel;   /**< Product Model                  */
   uint16_t           ui16ProdVersion; /**< Product Version                */
   char              *pcBrandName;     /**< File path name on USB port */
   uint8_t           ui8DefaultUsageId;
} tCFGF_ProductIDs;

/**
 *  @brief
 *    RF modulator resource definition.
 *
*/
typedef struct
{
   uint8_t            ui8RFModOutputId;   /** TDAL OUTPUT identifier.  eTDAL_OUTPUT_RF_MOD_ID_0=16 */
   uint8_t            ui8RFModBand;       /** UHF=1 VHF=2   */
   uint8_t            ui8RFModStandard;   /** eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG=1 */
   uint16_t           ui16RFModChannel;   /** for IMCL channel = 65   */
   uint8_t            ui8RFModI2cAddress; /** eTDAL_OUTPUT_VIDEO_SD_STD_PAL_BG=1 */
} tCFGF_RFMod;

/**
 *  @brief
 *    Serial Location definition.
 *
*/
typedef struct
{
   uint8_t            ui8PortId;       /**< Port identifier.   */
   uint8_t            ui8DataBit;      /**< Data bits (7 or 8)   */
   uint8_t            ui8Parity;       /**< Parity type (none, even, odd) 
                                            see kCFGF_SERIAL_PARITY_xxx */
   uint8_t            ui8StopBit;      /**< Stop bit (1 or 2)   */
   uint8_t            ui8FlowCtrl;     /**< Flow control (none, hardware, xon/xoff) 
                                            see kCFGF_SERIAL_FCTRL_xxx */
   uint8_t            ui8Speed;        /**< Speed (600, 1200, 2400, 4800, 9600, 
                                            14400, 19200, 38400, 57600, 115200)
                                            see kCFGF_SERIAL_SPEED_xxx */
} tCFGF_LocSerial;

/**
 *  @brief
 *    USB Location definition.
 *
*/
typedef struct
{
   char              *pcPathName;      /**< File path name on USB port */
} tCFGF_LocUSB;

/**
 *  @brief
 *    URL Location definition.
 *
*/
typedef struct
{
   char              *pcURL;           /**< URL*/
} tCFGF_LocURL;

#if defined (MEDIUM_TER)
/**
 *  @brief
 *    DVB-T Location definition.
 *
*/
typedef struct
{
   uint16_t           ui16PmtPID;      /**< PID of PMT. */
   uint16_t           ui16DsmccPID;    /**< PID of DSMCC carousel. */
   uint32_t           ui32Frequency;   /**< Frequency (in kHz). */
   uint32_t           ui32BandWidth;   /**< Band width (in kHz). */
   uint8_t            ui8Modulation;   /**< Modulation (BPSK, QPSK, QAM4, 8PSK, 
                                            QAM16, QAM32, QAM64, QAM128, QAM256, 
                                            AUTO, ANALOG) 
                                            see kCFGF_DVBT_MODULATION_xxx  */
   uint8_t            ui8Mode;         /**< Mode (2k, 8k, AUTO, MAX) 
                                            see kCFGF_DVBT_MODE_xxx  */
   uint8_t            ui8Guard;        /**< Mode (1/32, 1/16, 1/8, 1/4, AUTO, 
                                            MAX) see kCFGF_DVBT_GUARD_xxx  */
   uint8_t            ui8Hierarchy;    /**< Mode (NONE, 1, 2, 4, AUTO, MAX) 
                                            see kCFGF_DVBT_HIER_xxx  */
   uint8_t            ui8Priority;     /**< Priority stream (HIGH, LOW, MAX) 
                                            see kCFGF_DVBT_PRIO_xxx  */
   uint8_t            ui8ConvolutionHP;/**< Convolution HP (NONE, 1/2, 2/3, 3/4,
                                            4/5, 5/6, 6/7, 7/8, 8/9, AUTO, MAX) 
                                            see kCFGF_DVBT_CONVOL_xxx  */
   uint8_t            ui8ConvolutionLP;/**< Convolution HP (same HP values) */
   uint8_t            ui8FreqOffset;   /**< Frequency Offset (NONE, 1/6 more,
                                            1/6 minus, 2/6 more, 2/6 minus, 
                                            3/6 more, 3/6 minus, AUTO, MAX) 
                                            see kCFGF_DVBT_FREQOFF_xxx  */
} tCFGF_LocDVBT;

/**
 *  @brief
 *    DVB-T2 Location definition.
 *
*/
typedef struct
{
   uint16_t           ui16PmtPID;      /**< PID of PMT.  */
   uint16_t           ui16DsmccPID;    /**< PID of DSMCC carousel. */
   uint32_t           ui32Frequency;   /**< Frequency (in kHz). */
   uint32_t           ui32BandWidth;   /**< Band width (in kHz). */
   uint8_t            ui8Mode;         /**< Mode (2k, 8k, 4k, 1k, 16k, 32k, AUTO, MAX) 
                                            see kCFGF_DVBT2_MODE_xxx  */
   uint8_t            ui8Guard;        /**< Mode (1/32, 1/16, 1/8, 1/4, 1/128, 19/128, 19/256, AUTO, 
                                            MAX) see kCFGF_DVBT2_GUARD_xxx  */
   uint16_t           ui8PlpID;        /**< ID of PLP. */
} tCFGF_LocDVBT2;
#endif /* MEDIUM_TER */

#if defined (MEDIUM_CAB)
/**
 *  @brief
 *    DVB-C Location definition.
 *
*/
typedef struct
{
   uint16_t           ui16PmtPID;      /**< PID of PMT.   */
   uint16_t           ui16DsmccPID;    /**< PID of DSMCC carousel.   */
   uint16_t           ui16NetworkID;   /**< Network ID.   */
   uint32_t           ui32Frequency;   /**< Frequency (in kHz).   */
   uint32_t           ui32SymbolRate;  /**< Symbole rate (in symbols/s).   */
   uint8_t            ui8Modulation;   /**< Modulation (BPSK, QPSK, QAM4, 8PSK, 
                                            QAM16, QAM32, QAM64, QAM128, QAM256, 
                                            AUTO, ANALOG) 
                                            see kCFGF_DVBC_MODULATION_xxx  */
} tCFGF_LocDVBC;
#endif /* MEDIUM_CAB */

#if defined (MEDIUM_SAT)
/**
 *  @brief
 *    DVB-S Location definition.
 *
*/
typedef struct
{
   uint16_t           ui16PmtPID;      /**< PID of PMT.   */
   uint16_t           ui16DsmccPID;    /**< PID of DSMCC carousel.   */
   uint32_t           ui32Frequency;   /**< BIS Frequency (in kHz).   */
   uint32_t           ui32SymbolRate;  /**< Symbole rate (in symbols/s).   */
   uint8_t            ui8Polarization; /**< Polarization (horizontal, vertical,
                                            Circular Left, Circular Right, All) 
                                            see kCFGF_DVBS_POLARIZ_xxx  */
   uint8_t            ui8Modulation;   /**< Modulation (BPSK, QPSK, QAM4, 8PSK, 
                                            QAM16, QAM32, QAM64, QAM128, QAM256, 
                                            AUTO, ANALOG) 
                                            see kCFGF_DVBS_MODULATION_xxx  */
   uint8_t            ui8FecRate;      /**< FEC Rate (NONE, 1/2, 2/3, 3/4,
                                            4/5, 5/6, 6/7, 7/8, 8/9, AUTO, MAX) 
                                            see kCFGF_DVBS_FECRATE_xxx  */
   uint8_t            ui8OutSelector;  /**< Outdor Selector (NONE, None, Track 1, 
                                            Track 2, Track 3, Track 4) 
                                            see kCFGF_DVBS_OUTSEL_xxx  */
} tCFGF_LocDVBS;

/**
 *  @brief
 *    DVB-S2 Location definition.
 *
 *    Note: Not totaly specified => prepare it with same DVB-S structure
*/
typedef struct
{
   uint16_t           ui16PmtPID;      /**< PID of PMT.   */
   uint16_t           ui16DsmccPID;    /**< PID of DSMCC carousel.   */
   uint32_t           ui32Frequency;   /**< BIS Frequency (in kHz).   */
   uint32_t           ui32SymbolRate;  /**< Symbole rate (in symbols/s).   */
   uint8_t            ui8Polarization; /**< Polarization (horizontal, vertical,
                                            Circular Left, Circular Right, All) 
                                            see kCFGF_DVBS_POLARIZ_xxx  */
   uint8_t            ui8Modulation;   /**< Modulation (BPSK, QPSK, QAM4, 8PSK, 
                                            QAM16, QAM32, QAM64, QAM128, QAM256, 
                                            AUTO, ANALOG) 
                                            see kCFGF_DVBS_MODULATION_xxx  */
   uint8_t            ui8FecRate;      /**< FEC Rate (NONE, 1/2, 2/3, 3/4,
                                            4/5, 5/6, 6/7, 7/8, 8/9, AUTO, MAX) 
                                            see kCFGF_DVBS_FECRATE_xxx  */
   uint8_t            ui8OutSelector;  /**< Outdor Selector (NONE, None, Track 1, 
                                            Track 2, Track 3, Track 4) 
                                            see kCFGF_DVBS_OUTSEL_xxx  */
} tCFGF_LocDVBS2;
#endif /* MEDIUM_SAT */


/**
 *  @brief
 *    Downloader Custom data definition.
 *
*/
typedef struct
{
   uint32_t           ui32SectTimeout; /**< Timeout in millisecond of section acquisition  */
   uint8_t            ui8CarouMaxLoop; /**< Maximum number of loop on carousel.            */
} tCFGF_DownldCustom;

/**
 *  @brief
 *    ICS of flash definition.
 *
*/
typedef struct
{
   uint8_t            ui8IcsId;        /**< ICS Identifier   */
   uint8_t            ui8IcsClass;     /**< ICS Class   */
   uint32_t           ui32IcsAddress;  /**< Adress in flash   */
   uint32_t           ui32IcsSize;     /**< Size in bytes   */
} tCFGF_FlashICS;

/**
 *  @brief
 *    Flash Mapping definition.
 *
*/
typedef struct
{
   uint32_t           ui32BaseAddress; /**< Base address   */
   uint8_t            ui8IcsCount;     /**< Number of ICS in next table   */
   tCFGF_FlashICS    *pstICSs;         /**< Table of ICS   */
} tCFGF_FlashMapping;

/**
 *  @brief
 *    OSD Palette Color definition.
 *
*/
typedef struct
{
   uint8_t            ui8Alpha;        /**< Alpha                             */
   uint8_t            ui8Red;          /**< Red                               */
   uint8_t            ui8Green;        /**< Green                             */
   uint8_t            ui8Blue;         /**< Blue                              */
} tCFGF_PalColor;

/**
 *  @brief
 *    OSD Picture definition.
 *
*/
typedef struct
{
   uint16_t           ui16Width;       /**< Picture width in pixel            */
   uint16_t           ui16Height;      /**< Picture heigth in pixel           */
   uint8_t            ui8Type;         /**< Picture type (BMP, BMPRL, GIF...) 
                                            see kCFGF_OSD_PICT_xxxx  */
   uint16_t           ui16Size;        /**< Picture size in bytes             */
   uint8_t           *pui8Bitmap;      /**< Picture Bitmap                    */
} tCFGF_Picture;

/**
 *  @brief
 *    OSD definition.
 *
*/
typedef struct
{
   uint16_t           ui16ColorCount;  /**< Palette colors count (256)        */
   tCFGF_PalColor    *pstPaletteColor; /**< Palette of colors                 */
   uint8_t            ui8EntryCount;   /**< Entrys count in color scheme (8)  */
   uint8_t           *pui8PalEntry;    /**< The color scheme (table of index 
                                            in the palette of color)          */
   uint8_t            ui8PictCount;    /**< Pictures count                    */
   tCFGF_Picture     *pstPictures;     /**< Pictures table                    */
} tCFGF_OSD;

/**
 *  @brief
 *    Downloader Options data definition.
 *
*/
typedef struct
{
   uint8_t            ui8UseTuningFromDialogArea;   /**< use_tuning_from_dialog_area       */
   uint8_t            ui8EnableScanning;            /**< enable_scanning                   */
} tCFGF_DownldOption;

#if defined (MEDIUM_TER)
/**
 *  @brief
 *    Terrestrial scanning descriptor: frequency location definition. (DVB-T / DVB-T2)
 *
*/
typedef struct
{
   uint32_t           ui32Frequency;        /**< frequency                  */
   uint32_t           ui32Bandwidth;        /**< bandwidth                  */
   uint32_t           ui32ChannelNumber;    /**< channel number             */
} tCFGF_FreqLocDVBT;

/**
 *  @brief
 *    Terrestrial scanning descriptor: band location definition. (DVB-T / DVB-T2)
 *
*/
typedef struct
{
   uint32_t           ui32FirstFreq;        /**< first frequency to scan        */
   uint32_t           ui32LastFreq;         /**< last frequency to scan         */
   uint32_t           ui32CommonBandwidth;  /**< common bandwidth               */
   uint32_t           ui32FirstChNumber;    /**< first channel number           */
} tCFGF_BandLocDVBT;
#endif

/**
 *  @brief
 *    Configuration definition.
 *
*/
typedef struct
{
   uint32_t           ui32Version;     /**< Version of the configuration      */
   tCFGF_ProductIDs   stProductIDs;    /**< Product identifiers               */
   uint8_t            ui8CountSerial;  /**< Serial Locations Counter          */
   tCFGF_LocSerial   *pstTableSerial;  /**< Serial Locations Table            */
   uint8_t            ui8CountUSB;     /**< USB Locations Counter             */
   tCFGF_LocUSB      *pstTableUSB;     /**< USB Locations Table               */
   uint8_t            ui8CountURL;     /**< URL Locations Counter             */
   tCFGF_LocURL      *pstTableURL;     /**< URL Locations Table               */
#if defined (MEDIUM_TER)
   uint8_t            ui8CountDVBT;    /**< DVB-T Locations Counter           */
   tCFGF_LocDVBT     *pstTableDVBT;    /**< DVB-T Locations Table             */
   uint8_t            ui8CountDVBT2;   /**< DVB-T2 Locations Counter          */
   tCFGF_LocDVBT2    *pstTableDVBT2;   /**< DVB-T2 Locations Table            */
#endif
#if defined (MEDIUM_CAB)
   uint8_t            ui8CountDVBC;    /**< DVB-C Locations Counter           */
   tCFGF_LocDVBC     *pstTableDVBC;    /**< DVB-C Locations Table             */
#endif
#if defined (MEDIUM_SAT)
   uint8_t            ui8CountDVBS;    /**< DVB-S Locations Counter           */
   tCFGF_LocDVBS     *pstTableDVBS;    /**< DVB-S Locations Table             */
   uint8_t            ui8CountDVBS2;   /**< DVB-S2 Locations Counter          */
   tCFGF_LocDVBS2    *pstTableDVBS2;   /**< DVB-S2 Locations Table            */
#endif
   tCFGF_DownldCustom stDownldCustom;  /**< Downloader custom                 */
   tCFGF_FlashMapping stFlashMapping;  /**< Flash mapping                     */
   tCFGF_OSD          stOSD;           /**< OSD Resources                     */
   tCFGF_RFMod        stRFMod;         /**< RFMod Resources                   */
   tCFGF_DownldOption stDownldOptions; /**< Downloader options                */
#if defined (MEDIUM_TER)
   uint8_t            ui8CountFreqDVBT;/**< DVB-T Scanning frequency Counter  */
   tCFGF_FreqLocDVBT *pstTableFreqDVBT;/**< DVB-T Scanning frequency Table    */
   uint8_t            ui8CountBandDVBT;/**< DVB-T Scanning band Counter       */
   tCFGF_BandLocDVBT *pstTableBandDVBT;/**< DVB-T Scanning band Table         */
#endif
} tCFGF_Configuration;


/******************************************************************************
* Functions
******************************************************************************/

/**
 *  @brief
 *    Load the configuration file in memory and return in the allocated 
 *    pointer returned the set of element asked by the caller.
 *
 *  @param[in] xu32EltsSet
 *              Set of identifier kCFGF_ID_PROD_xxx) of elements asked.
 *              A 'or' on identifiers of each desired elements, for
 *              example: kCFGF_ID_PROD_ID|kCFGF_ID_FLASH|kCFGF_ID_LOC_DVBC
 *
 *  @retval  Not NULL
 *              Pointer on configuration loaded.
 *
 *  @retval  NULL
 *              Loading failed (read, allocation or parsing error)
 *
 *  @remarks
 *    -# The pointer returned must be released by next function 
 *       configuration 'CFGF_Free'.
 *
*/
tCFGF_Configuration *CFGF_Read
(
   uint32_t u32xEltsSet
);

/**
 *  @brief
 *    Free a configuration structure allocated and returned by CFGF_Read.
 *
 *  @param[in] pxstConfiguration
 *              Pointer on configuration to free
 *
 *
*/
void CFGF_Free
(
   tCFGF_Configuration *pstxConfiguration
);


#ifdef __cplusplus
}
#endif

#endif /* _CONFIG_FILE_H */

/* config_file.h */

