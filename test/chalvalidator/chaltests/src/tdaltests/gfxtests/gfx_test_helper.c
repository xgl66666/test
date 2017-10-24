/** \file gfx_test_helper.c
 *  \brief Helper functions to write tests
 *  for the GFX layer of the TDAL API.
 *  \author Pascal PIGNON
 *  \date 20/04/06
 *  \version 1.0
 */

/******************************************************
 *              Includes                              *
 ******************************************************/

#include "crules.h"
#include <string.h>

#include "tkel.h"
#include "tbox.h"
#include "tdal_gfx.h"
/*#include "tdal_common.h"*/

#include "testhelper.h"
#include "gfxtests.h"

/******************************************************
 *              Defines                               *
 ******************************************************/

mTBOX_SET_MODULE(eTDAL_TEST_GFX);
#define kUT_DETECT_ALIGNMENT_MASK     0x03

/******************************************************
 *              Local variables                       *
 ******************************************************/

typedef struct
{
    uint8_t     Blue;
    uint8_t     Green;
    uint8_t     Red;
    uint8_t     Reserved;
} tRGBQuad;


typedef enum
{
    eBI_RGB,                            /* No compression.    */
    eBI_RLE8,                           /* 8bit RLE encoding. */
    eBI_RLE4                            /* 4bit RLE encoding. */
} tCompressionLevel;

/*------------------------------------------*/
/*   BMP format Private types definition    */
/*------------------------------------------*/
typedef struct
{
    uint8_t Count;                      /* Absolute or compressed mode. */
    uint8_t Command;                    /* Code of the command to apply. */
} tCompressionCmd;


typedef union
{
    tCompressionCmd     *RleBytes;
    uint8_t             *Data;
} tBmpData;


typedef struct
{
    uint16_t     Type;
    uint32_t     Size;
    uint16_t     Reserved1;
    uint16_t     Reserved2;
    uint32_t     OffBits;
} tBmpFileHeader;


typedef struct
{
    uint32_t            Size;
    uint32_t            Width;
    uint32_t            Height;
    uint16_t            Planes;
    uint16_t            BitCount;
    tCompressionLevel   Compression;
    uint32_t            SizeImage;
    uint32_t            XPelsPerMeter;
    uint32_t            YPelsPerMeter;
    uint32_t            ClrUsed;
    uint32_t            ClrImportant;
} tBmpInfoHeader;

/*-------------------------------------------*/
/*     TBM format Private types definition   */
/*-------------------------------------------*/
#define         TBM_SIGNATURE               0x504D4254                                  /* Equal to 'P', 'M', 'B', 'T'. */
#define         TBM_NB_VERSION_SUPPORTED    1                                           /* Number of supported versions. */

/* Mandatory chunk types. */
#define         TBM_CHUNK_BMPR_TYPE         0x52504D42                                  /* Equal to 'R', 'P', 'M', 'B'. */
#define         TBM_CHUNK_BMPD_TYPE         0x44504D42                                  /* Equal to 'D', 'P', 'M', 'B', */


/************************************************/
/*          TFUT constants definition.          */
/************************************************/
#define         TFUT_MANDATORY_CHUNK            0x00000010

/* Mandatory chunk types. */
#define         TFUT_CHUNK_FIHD_TYPE            0x44484946                              /* Equal to 'F', 'I', 'H', 'D'. */
#define         TFUT_CHUNK_EOFI_TYPE            0x49464f45                              /* Equal to 'E', 'O', 'F', 'I'. */

const uint32_t  TFUT_TBM_SupportedVersions[TBM_NB_VERSION_SUPPORTED] = {
                                                                            0x302E3330  /* 03.0 */

                                                                       };

/*------------------------*/
/* TFUT Chunks definition. */
/*------------------------*/
typedef struct
{
    uint32_t            Length;
    uint32_t            Type;
} tTFUT_ChunkHeader;


typedef struct
{
    tTFUT_ChunkHeader   Header;
    uint32_t            Crc32;
    uint32_t            Version;
    uint32_t            FileSize;
} tTFUT_ChunkFIHD;


/* TBM Chunks definition. */
typedef struct
{
    tTFUT_ChunkHeader   Header;
    tCompressionLevel   Compression;
    uint32_t            Width;
    uint32_t            Height;
    uint32_t            BitsPerPel;
} tGFX_ChunkBMPR;


typedef struct
{
    tTFUT_ChunkHeader   Header;
    uint8_t             *Data;
} tGFX_ChunkBMPD;


typedef tTFUT_ChunkHeader   tTFUT_ChunkEOFI;

LOCAL void *MemMalloc( uint32_t SegtSize );
LOCAL int MemFree( void *Ptr );

/******************************************************
 *          test helper functions                     *
 ******************************************************/
/*! \fn void TestHelper_GFX_FillPalette( tTDAL_GFX_Color *pPalette )
 *  \brief Fill a palette with RGB colors in opaque mode.
 *
 *  The size of the palette must be 256 entries and the first color
 *  is the transparency color.
 *
 *  \param[out] *pPalette the palette to fill
 */
void TestHelper_GFX_FillPalette( tTDAL_GFX_Palette *pPalette )
{
    uint16_t CountRed, CountGreen, CountBlue, CountColor=0;

    pPalette->colorType = eTDAL_GFX_COLOR_CLUT_ARGB8888;

    /* White */
    pPalette->pColor[CountColor].ARGB8888.alpha  = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.R      = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.G      = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.B      = 0xFF;

    /* Green */
    pPalette->pColor[++CountColor].ARGB8888.alpha  = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.R      = 0x00;
    pPalette->pColor[CountColor].ARGB8888.G      = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.B      = 0x00;

    /* Blue */
    pPalette->pColor[++CountColor].ARGB8888.alpha  = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.R      = 0x00;
    pPalette->pColor[CountColor].ARGB8888.G      = 0x00;
    pPalette->pColor[CountColor].ARGB8888.B      = 0xFF;

    /* Red */
    pPalette->pColor[++CountColor].ARGB8888.alpha  = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.R      = 0xFF;
    pPalette->pColor[CountColor].ARGB8888.G      = 0x00;
    pPalette->pColor[CountColor].ARGB8888.B      = 0x00;

    for(CountRed = 0; CountRed < 7; CountRed++)
    {
        for(CountGreen = 0; CountGreen < 6; CountGreen++)
        {
            for(CountBlue = 0; CountBlue < 6; CountBlue++)
            {
                ++CountColor;/* = (CountRed*6*6) + (CountGreen*6) + (CountBlue)+4;*/

                pPalette->pColor[CountColor].ARGB8888.alpha  = kTDAL_GFX_OPAQUE;
                pPalette->pColor[CountColor].ARGB8888.R      = (uint8_t)(CountRed+1) * 42;
                pPalette->pColor[CountColor].ARGB8888.G      = (uint8_t)(CountGreen+1) * 50;
                pPalette->pColor[CountColor].ARGB8888.B      = (uint8_t)(CountBlue+1) * 50;
            }
        }
    }
}

/*! \fn tTDAL_GFX_Error TestHelper_GFX_BmpLoad( uint8_t *Bmp_ptr, tTDAL_GFX_Bitmap **bitmap, tTDAL_GFX_Color **Palette )
 *  \brief Load a bitmap from memory
 *
 *  \param[in] *Bmp_ptr Pointer on the memory that contains the bitmap file
 *  \param[out] **bitmap to retrieve a handle for the bitmap
 *  \param[out] **Palette will contain the palette associated to the bitmap
 *  \return a GFX error code
 */
tTDAL_GFX_Error TestHelper_GFX_BmpLoad( uint8_t *Bmp_ptr, tTDAL_GFX_Bitmap **bitmap, tTDAL_GFX_Palette **Palette )
{
    tTFUT_ChunkFIHD                FIHD;                        /* Bmp_ptr may be not aligned on %4 address, so    */
    tGFX_ChunkBMPR                BMPR;                        /* we must copy informations ...                */
    tTFUT_ChunkHeader            ChunkHeader;
    uint8_t                        *BmpDest = NULL, *BmpSrcEnd;
    tBmpFileHeader                BitmapFileHeader;
    tBmpInfoHeader                BitmapInfoHeader;
    tRGBQuad                    *RGBQuad = NULL;
    register tBmpData            CurrentBmp_ptr;
    uint32_t                    Signature;
    uint32_t                    RowSize;
    uint32_t                    NbBytePadded;
    uint32_t                        Index;
    tTDAL_GFX_Error                returnError    = eTDAL_GFX_NO_ERROR;
    register uint16_t            NbCharPerRow, NbCharToWrite;
    uint8_t                        NbBytePaddedToBurst = 0;    /* Nb. bytes added at each line of bmp to allow the use of move2d_non_zero(...) function. */
    bool                        NoPalette = FALSE;


    mTBOX_FCT_ENTER("GFX_BmpLoad(...)");

    CurrentBmp_ptr.Data    = (uint8_t *)Bmp_ptr;

    memset(&BitmapFileHeader, 0, sizeof(tBmpFileHeader));
    memset(&BitmapInfoHeader, 0, sizeof(tBmpInfoHeader));

    /* Destination pointers must be set to NULL to avoid unattemped free if an error occur. */
    if(bitmap != NULL)
    {
        *bitmap = NULL;
    }

    if(Palette != NULL)
    {
        *Palette = NULL;
    }
    else
    {
        NoPalette = TRUE;
    }


    /* Parameters verification. */
    if(Bmp_ptr == NULL)
    {
        returnError    = eTDAL_GFX_BAD_PARAMETER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bitmap pointer is equal to NULL !!\n"));
    }
    else if((&BitmapFileHeader == NULL) || (&BitmapInfoHeader == NULL))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not enougth memory !!\n"));
        returnError    = eTDAL_GFX_NO_MEMORY;
    }
    else
    {
        /*--------------------- */
        /*  Header parsing */
        /*--------------------- */
        Signature  = CurrentBmp_ptr.Data[3] << 24;
        Signature |= CurrentBmp_ptr.Data[2] << 16;
        Signature |= CurrentBmp_ptr.Data[1] << 8;
        Signature |= CurrentBmp_ptr.Data[0];

        /* TKUI Bitmap without palette and some headders */
        if(Signature == TBM_SIGNATURE)
        {
            CurrentBmp_ptr.Data += sizeof(uint32_t);

            /* Getting first chunk : FIle HeaDer (FIHD, mandatory first) */
            FIHD.Header.Length = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            FIHD.Header.Type = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            FIHD.Crc32 = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            FIHD.Version = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            FIHD.FileSize = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);

            mTBOX_TRACE((kTBOX_NIV_1, "File Size\t\t\t= %d octets\n", FIHD.FileSize));

            /* Analysing FIHD ... */
            if((FIHD.Header.Type == TFUT_CHUNK_FIHD_TYPE)  &&  ((FIHD.FileSize & 0x03) == 0))        /* (FIHD.FileSize & 0x03) == (FIHD.FileSize % 4). */
            {
                for(Index=0; Index < TBM_NB_VERSION_SUPPORTED; Index++)
                {
                    if(TFUT_TBM_SupportedVersions[Index] == FIHD.Version)
                    {
                        /* Getting second chunk : BMp PRoperties (BMPR, mandatory second) */
                        BMPR.Header.Length = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                        CurrentBmp_ptr.Data += sizeof(uint32_t);
                        BMPR.Header.Type = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                        CurrentBmp_ptr.Data += sizeof(uint32_t);
                        BMPR.Compression = (tCompressionLevel)(CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0]);
                        CurrentBmp_ptr.Data += sizeof(uint32_t);
                        BMPR.Width = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                        CurrentBmp_ptr.Data += sizeof(uint32_t);
                        BMPR.Height = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                        CurrentBmp_ptr.Data += sizeof(uint32_t);
                        BMPR.BitsPerPel = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                        CurrentBmp_ptr.Data += sizeof(uint32_t);

                        /* Analysing BMPR ... */
                        if(BMPR.Header.Type == TBM_CHUNK_BMPR_TYPE)
                        {
                            if(BMPR.Compression == eBI_RLE8)
                            {
                                /* Getting first chunk : FIle HeaDer (FIHD, mandatory first) */
                                ChunkHeader.Length = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                                 CurrentBmp_ptr.Data += sizeof(uint32_t);
                                ChunkHeader.Type = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
                                 CurrentBmp_ptr.Data += sizeof(uint32_t);

                                if(ChunkHeader.Type == TBM_CHUNK_BMPD_TYPE)
                                {
                                    uint8_t *pLocalData = CurrentBmp_ptr.Data;

                                    BitmapInfoHeader.Width            = BMPR.Width;
                                    BitmapInfoHeader.Height            = BMPR.Height;
                                    BitmapInfoHeader.BitCount        = (uint8_t)BMPR.BitsPerPel;
                                    BitmapInfoHeader.Compression    = BMPR.Compression;
                                    BitmapInfoHeader.SizeImage        = ChunkHeader.Length;
                                    BitmapInfoHeader.ClrUsed        = 0;

                                    NoPalette = TRUE;

                                    /* Getting fourth chunk : BMP Data (BMPR, mandatory fourth & last). */
                                    if((NbBytePadded = ChunkHeader.Length & 0x03) > 0)  /* (ChunkHeader.Length & 0x03) == (ChunkHeader.Length % 4). */
                                    {
                                        NbBytePadded = 4 - NbBytePadded;
                                    }

                                    /* Getting first chunk : FIle HeaDer (FIHD, mandatory first) */
                                    pLocalData += ChunkHeader.Length + NbBytePadded;
                                    ChunkHeader.Length = pLocalData[3] << 24 | pLocalData[2] << 16 | pLocalData[1] << 8 | pLocalData[0];
                                    pLocalData += sizeof(uint32_t);
                                    ChunkHeader.Type = pLocalData[3] << 24 | pLocalData[2] << 16 | pLocalData[1] << 8 | pLocalData[0];

                                    if(ChunkHeader.Type != TFUT_CHUNK_EOFI_TYPE)
                                    {
                                        returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR in GFX_BmpLoad(...) : TBM file corrupted (EOFI).\n"));
                                    }
                                }
                                else /* != TBM_CHUNK_BMPD_TYPE */
                                {
                                    returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR in GFX_BmpLoad(...) : TBM file corrupted (BMPD).\n"));
                                }
                            }
                            else /* != eBI_RLE8 */
                            {
                                returnError = eTDAL_GFX_FEATURE_NOT_SUPPORTED;
                                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR in GFX_BmpLoad(...) : This is not a valid bitmap (Not compressed).\n"));
                            }
                        }
                    }
                }
            }
            else
            {
                returnError = eTDAL_GFX_UNKNOWN_ERROR;
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR in GFX_BmpLoad(...) : TBMP file corrupted\n"));
            }
        }

        /* Bitmap with palette */
        else if((uint16_t)Signature  == 0x4d42)
        {
            /*--------------------- */
            /* Get BitmapFileHeader */
            /*--------------------- */
            /* the header MUST be little endian whatever the CPU is */
            BitmapFileHeader.Type = (uint16_t)(CurrentBmp_ptr.Data[0] << 8 | CurrentBmp_ptr.Data[1]);
            CurrentBmp_ptr.Data += sizeof(uint16_t);
            BitmapFileHeader.Size = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapFileHeader.Reserved1 = (uint16_t)(CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0]);
            CurrentBmp_ptr.Data += sizeof(uint16_t);
            BitmapFileHeader.Reserved2 = (uint16_t)(CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0]);
            CurrentBmp_ptr.Data += sizeof(uint16_t);
            BitmapFileHeader.OffBits = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);

            /*--------------------- */
            /* Get BitmapInfoHeader */
            /*--------------------- */
            /* the header MUST be little endian whatever the CPU is */
            BitmapInfoHeader.Size = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.Width = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.Height = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.Planes = (uint16_t)(CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0]);
            CurrentBmp_ptr.Data += sizeof(uint16_t);
            BitmapInfoHeader.BitCount = (uint16_t)(CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0]);
            CurrentBmp_ptr.Data += sizeof(uint16_t);
            BitmapInfoHeader.Compression = (tCompressionLevel)(CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0]);
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.SizeImage = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.XPelsPerMeter = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.YPelsPerMeter = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.ClrUsed = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);
            BitmapInfoHeader.ClrImportant = CurrentBmp_ptr.Data[3] << 24 | CurrentBmp_ptr.Data[2] << 16 | CurrentBmp_ptr.Data[1] << 8 | CurrentBmp_ptr.Data[0];
            CurrentBmp_ptr.Data += sizeof(uint32_t);

            /* The information ClrUsed may be setted to zero in some cases ... */
            if(BitmapInfoHeader.ClrUsed == 0)
            {
                BitmapInfoHeader.ClrUsed = (1<<BitmapInfoHeader.BitCount);
            }

            /* Palette computing. */
            RGBQuad = (tRGBQuad *)CurrentBmp_ptr.Data;
            CurrentBmp_ptr.Data += sizeof(tRGBQuad)*BitmapInfoHeader.ClrUsed;
        }
        else
        {
            returnError    = eTDAL_GFX_UNKNOWN_ERROR;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not get the format of the bitmap !!\n"));
        }


#if defined(_GFX_DEBUG_MEM_)  &&  defined(__DEBUG_MEM__)
        Print("\n\nBEFORE Load Bitmap %d*%d*%d", BitmapInfoHeader.Width, BitmapInfoHeader.Height, BitmapInfoHeader.ClrUsed);
        UT_MemMemoryStat();
#endif


        if(returnError == eTDAL_GFX_NO_ERROR )
        {
            /*--------------------- */
            /*  Palette parsing */
            /*--------------------- */
            if(NoPalette == FALSE)
            {
                if((((*Palette)         = MemMalloc(sizeof(tTDAL_GFX_Palette))) != NULL) &&
                   (((*Palette)->pColor = MemMalloc(sizeof(tTDAL_GFX_Color)*BitmapInfoHeader.ClrUsed)) != NULL))
                {
                    (*Palette)->nbColors  = BitmapInfoHeader.ClrUsed;
                    (*Palette)->colorType = eTDAL_GFX_COLOR_CLUT_ARGB8888;

                    for(Index=0; Index<(BitmapInfoHeader.ClrUsed); Index++)
                    {
                        /* Returns an RGB palette. */
                        (*Palette)->pColor[Index].ARGB8888.alpha = kTDAL_GFX_OPAQUE;
                        /*(*Palette)->pColor[Index].ARGB8888.alpha = RGBQuad[Index].Reserved;*/        /* Reserved byte will may be use by MixWeight in future format. */

                        (*Palette)->pColor[Index].ARGB8888.R = RGBQuad[Index].Red;
                        (*Palette)->pColor[Index].ARGB8888.G = RGBQuad[Index].Green;
                        (*Palette)->pColor[Index].ARGB8888.B = RGBQuad[Index].Blue;
                    }
                }
                else
                {
                    returnError = eTDAL_GFX_NO_MEMORY;
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not enougth memory !!\n"));
                }
            }
            else
            {
                   returnError = eTDAL_GFX_UNKNOWN_ERROR;
                mTBOX_TRACE((kTBOX_NIV_1, "The user don't want the palette data to be loaded.\n\n\n"));
            }

            /*--------------------- */
            /*    Data parsing */
            /*--------------------- */
            if((bitmap != NULL) && (returnError == eTDAL_GFX_NO_ERROR))
            {
                if(BitmapInfoHeader.Compression == eBI_RGB)
                {
                    /* Pixels value computing. */
                    if(BitmapInfoHeader.BitCount ==    4)
                    {
                        /* Each    pixel in the bitmap    is represented by a    4-bit index    into the palette. */
                        RowSize = (BitmapInfoHeader.Width >> 1) + (BitmapInfoHeader.Width & 0x01);                /* ((BitmapInfoHeader.Width >> 1) + (BitmapInfoHeader.Width & 0x01)) == ((BitmapInfoHeader.Width / 2) + (BitmapInfoHeader.Width % 2)). */
                    }
                    else if(BitmapInfoHeader.BitCount == 8)
                    {
                        /* Each    pixel in the bitmap    is represented by a    8-bit index    into the palette. */
                        RowSize = BitmapInfoHeader.Width;
                    }
                    else
                    {
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "This    is not a 4 or 8 bpp bitmap !!\n"));
                        mTBOX_RETURN(eTDAL_GFX_UNKNOWN_ERROR);
                    }

                       if((NbBytePadded = RowSize & 0x03) > 0)
                       {
                           NbBytePadded = 4 - NbBytePadded;
                       }


                    if(((BmpDest = MemMalloc(RowSize * BitmapInfoHeader.Height)) != NULL) && \
                       ((*bitmap = MemMalloc(sizeof(tTDAL_GFX_Bitmap))) != NULL))
                    {
                        CurrentBmp_ptr.Data += ((RowSize + NbBytePadded) * (BitmapInfoHeader.Height - 1));

                        for(Index=0; Index < BitmapInfoHeader.Height; Index++)
                        {
                            memcpy(BmpDest + (RowSize*Index), CurrentBmp_ptr.Data, RowSize);
                            CurrentBmp_ptr.Data -= (RowSize + NbBytePadded);
                        }

                        /* tTDAL_GFX_Bitmap header filling. */
                        (*bitmap)->pData = BmpDest;
                    }
                    else
                    {
                        returnError = eTDAL_GFX_NO_MEMORY;
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not enougth memory !!\n"));
                    }
                }
                else if(BitmapInfoHeader.Compression == eBI_RLE8)
                {
                    /* Pixels value computing. */
                    if((BitmapInfoHeader.BitCount != 8)  ||  (BitmapInfoHeader.SizeImage ==0))
                    {
                        returnError    = eTDAL_GFX_UNKNOWN_ERROR;
                        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "This    is not a 8 bpp RLE8 compressed bitmap !!\n"));
                    }

                    /* Each    pixel in the bitmap    is represented by a    8-bit index    into the palette. */
                    RowSize = BitmapInfoHeader.Width;

                       if((NbBytePaddedToBurst = (uint8_t)(RowSize & kUT_DETECT_ALIGNMENT_MASK)) > 0)
                       {
                        NbBytePaddedToBurst = kUT_DETECT_ALIGNMENT_MASK - NbBytePaddedToBurst + 1;
                        mTBOX_TRACE((kTBOX_NIV_1, "GFX_BmpLoad(...) : RowSize = %d,\tNbBytePaddedToBurst = %d.\n", RowSize, NbBytePaddedToBurst));
                    }

                       if((NbBytePadded = BitmapInfoHeader.SizeImage & 0x01) > 0)
                       {
                           NbBytePadded = 2 - NbBytePadded;
                       }

                    /* Traitement des donnees Bitmap. */
                    if((bitmap != NULL) && (returnError == eTDAL_GFX_NO_ERROR))
                    {
                        if(((BmpDest = MemMalloc((BitmapInfoHeader.Width + NbBytePaddedToBurst) * BitmapInfoHeader.Height)) != NULL) && \
                           ((*bitmap = MemMalloc(sizeof(tTDAL_GFX_Bitmap))) != NULL))
                        {
                            /* tTDAL_GFX_Bitmap header filling. */
                            (*bitmap)->pData     = BmpDest;
                            Index                = BitmapInfoHeader.Height - 1;                    /* Represent the current row. */
                            BmpDest             = (void *)((uint8_t *)((*bitmap)->pData) + (Index * (BitmapInfoHeader.Width + NbBytePaddedToBurst)));

                            /* Variables initialisation. */
                            NbCharPerRow        = 0;
                            /*NbCharToWrite        = 0; // not usefull */

                            BmpSrcEnd = CurrentBmp_ptr.Data + (BitmapInfoHeader.SizeImage - 1);    /* Size image don't comprise the padded bytes. */
                            CurrentBmp_ptr.Data += NbBytePadded;

                            while((CurrentBmp_ptr.Data < BmpSrcEnd) && ((uint8_t *)((*bitmap)->pData) <= BmpDest))
                            {
                                if(CurrentBmp_ptr.RleBytes->Count != 0x00)                    /* Is this Absolute mode ? */
                                {
                                    NbCharToWrite = CurrentBmp_ptr.RleBytes->Count;
                                    NbCharPerRow  += NbCharToWrite;

                                    if(NbCharPerRow > BitmapInfoHeader.Width)
                                    {
                                        NbCharToWrite = (uint16_t)BitmapInfoHeader.Width - (NbCharPerRow - NbCharToWrite);
                                        returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                        mTBOX_TRACE((kTBOX_NIV_2, "WARNING : Header row size (%d) differ from data row size (%d) !!\n", BitmapInfoHeader.Width, NbCharPerRow));
                                        NbCharPerRow = (uint16_t)BitmapInfoHeader.Width;
                                    }

                                    memset(BmpDest, CurrentBmp_ptr.RleBytes->Command, NbCharToWrite);
                                    BmpDest += NbCharToWrite;
                                    CurrentBmp_ptr.Data += 2;
                                }
                                else                                                        /* The MSB (first char) is a control char ? */
                                {
                                    if(CurrentBmp_ptr.RleBytes->Command > 0x02)                /* Copy Nb. pixels. */
                                    {
                                        NbCharToWrite = CurrentBmp_ptr.RleBytes->Command;
                                        NbCharPerRow  += NbCharToWrite;

                                        if(NbCharPerRow > BitmapInfoHeader.Width)
                                        {
                                            NbCharToWrite = (uint16_t)BitmapInfoHeader.Width - (NbCharPerRow - NbCharToWrite);
                                            returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                            mTBOX_TRACE((kTBOX_NIV_2, "WARNING : Header row size (%d) differ from data row size (%d) !!\n", BitmapInfoHeader.Width, NbCharPerRow));
                                            NbCharPerRow = (uint16_t)BitmapInfoHeader.Width;
                                        }

                                        memcpy(BmpDest, (void *)(CurrentBmp_ptr.Data+2), NbCharToWrite);
                                        BmpDest += NbCharToWrite;
                                        CurrentBmp_ptr.Data += (NbCharToWrite + 2 + (NbCharToWrite & 0x01));
                                    }
                                    else if(CurrentBmp_ptr.RleBytes->Command == 0x02)        /* Delta bytes. */
                                    {
                                        CurrentBmp_ptr.Data += 2;

                                        /* Calcul des "Delta bytes" a affecter. */
                                        NbCharToWrite = CurrentBmp_ptr.RleBytes->Count + (CurrentBmp_ptr.RleBytes->Command * (uint16_t)RowSize);
                                        NbCharPerRow  += NbCharToWrite;

                                        if(NbCharPerRow > BitmapInfoHeader.Width)
                                        {
                                            NbCharToWrite = (uint16_t)BitmapInfoHeader.Width - (NbCharPerRow - NbCharToWrite);
                                            returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                            mTBOX_TRACE((kTBOX_NIV_2, "WARNING : Header row size (%d) differ from data row size (%d) !!\n", BitmapInfoHeader.Width, NbCharPerRow));
                                            NbCharPerRow = (uint16_t)BitmapInfoHeader.Width;
                                        }

                                        /* Affectation de n characteres la couleur 0. */
                                        memset(BmpDest, 0, NbCharToWrite);
                                        BmpDest += NbCharToWrite;
                                        NbCharPerRow += NbCharToWrite;
                                        CurrentBmp_ptr.Data += 2;
                                    }
                                    else if(CurrentBmp_ptr.RleBytes->Command == 0x00)        /* End of row. */
                                    {
                                        if(NbBytePaddedToBurst != 0)
                                        {
                                            memset(BmpDest, 0, NbBytePaddedToBurst);
                                        }

                                        if(Index > 0)
                                        {
                                            Index--;
                                            BmpDest = (void *)((uint8_t *)((*bitmap)->pData) + (Index * (BitmapInfoHeader.Width + NbBytePaddedToBurst)));
                                        }

                                        /* Cas normalement impossible du fait de la gestion de la corruption des donnees BMP. */
                                        if(NbCharPerRow != BitmapInfoHeader.Width)
                                        {
                                            returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR : Header row size (%d) differ from data row size (%d) !!\n", BitmapInfoHeader.Width, NbCharPerRow));
                                            break;
                                        }

                                        CurrentBmp_ptr.Data += 2;
                                        NbCharPerRow = 0;
                                    }
                                    else if(CurrentBmp_ptr.RleBytes->Command == 0x01)        /* End of bitmap. */
                                    {
                                        if(CurrentBmp_ptr.Data != BmpSrcEnd-1)
                                        {
                                            returnError = eTDAL_GFX_UNKNOWN_ERROR;
                                            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "ERROR : End of Bitmap found before end of image !!\n"));
                                        }

                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            returnError = eTDAL_GFX_NO_MEMORY;
                            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Not enougth memory !!\n"));
                        }
                    }
                }
                else if(BitmapInfoHeader.Compression == eBI_RLE4)
                {
                    returnError    = eTDAL_GFX_UNKNOWN_ERROR;
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "RLE4 bitmap format not supported!!\n"));
                }
                else
                {
                    returnError    = eTDAL_GFX_UNKNOWN_ERROR;
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "This is an unsupported bitmap format!!\n"));
                }
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_1, "The user don't want the BMP data to be loaded.\n"));
            }
        }
        else
        {
            returnError    = eTDAL_GFX_UNKNOWN_ERROR;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "This is not a bitmap : Type = %x = %2.2s !!\n", BitmapFileHeader.Type, (char *)&(BitmapFileHeader.Type)));
        }
    }


    if(returnError == eTDAL_GFX_NO_ERROR)
    {
        /* tTDAL_GFX_Bitmap header filling. */
        (*bitmap)->colorType   = eTDAL_GFX_COLOR_CLUT_ARGB8888;
        (*bitmap)->size.width  = BitmapInfoHeader.Width + NbBytePaddedToBurst;
        (*bitmap)->size.height = BitmapInfoHeader.Height;
    }
    else
    {
        if((Palette != NULL) && (*Palette != NULL))
        {
            if((*Palette)->pColor != NULL)
            {
                MemFree((*Palette)->pColor);
            }
            MemFree(*Palette);
            *Palette = NULL;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bitmap Palette FREED !!\n"));
        }

        if(BmpDest != NULL)
        {
            MemFree(BmpDest);
            /*BmpDest = NULL; // not usefull */
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bitmap Data FREED !!\n"));
        }

        if((bitmap != NULL) && (*bitmap != NULL))
        {
            MemFree(*bitmap);
            *bitmap = NULL;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bitmap Header FREED !!\n"));
        }
    }


    mTBOX_RETURN(returnError);
}

/*! \fn tTDAL_GFX_Error TestHelper_GFX_BmpUnLoad( tTDAL_GFX_Bitmap **bitmap, tTDAL_GFX_Color **Palette )
 *  \brief Unload a bitmap from memory
 *
 *  \param[in] **bitmap is a pointer on the bitmap handle associated to the bitmap to unload
 *  \param[in] **Palette is a palette pointer associated to the bitmap to unload
 *  \return a GFX error code
 */
tTDAL_GFX_Error TestHelper_GFX_BmpUnLoad( tTDAL_GFX_Bitmap **bitmap, tTDAL_GFX_Palette **Palette )
{
    tTDAL_GFX_Error        returnError    = eTDAL_GFX_NO_ERROR;
    uint8_t            CountErrors = 0;


    mTBOX_FCT_ENTER("GFX_BmpUnLoad(...)");

#if defined(_GFX_DEBUG_MEM_)  &&  defined(__DEBUG_MEM__)
    Print("\nAFTER Unload Bitmap %d*%d", (*bitmap)->Width, (*bitmap)->Height);
#endif

    if((bitmap != NULL) && (*bitmap != NULL))
    {
        if((*bitmap)->pData != NULL)
        {
            MemFree((*bitmap)->pData);
            (*bitmap)->pData = NULL;
        }

        MemFree(*bitmap);
        *bitmap = NULL;
    }
    else
    {
        CountErrors++;
        mTBOX_TRACE((kTBOX_NIV_WARNING, "GFX_BmpUnLoad() : Bitmap data pointer is equal to NULL\n"));
    }

    if((Palette != NULL) && (*Palette != NULL))
    {
        if((*Palette)->pColor != NULL)
        {
            MemFree((*Palette)->pColor);
            (*Palette)->pColor = NULL;
        }
        MemFree(*Palette);
        *Palette = NULL;
    }
    else
    {
        CountErrors++;
        mTBOX_TRACE((kTBOX_NIV_WARNING, "GFX_BmpUnLoad() : Palette data pointer is equal to NULL\n"));
    }

    if(CountErrors >= 2)
    {
        returnError = eTDAL_GFX_BAD_PARAMETER;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "GFX_BmpUnLoad() : Bad input parameters (Bitmap and Palette are NULL)\n"));
    }

#if defined(_GFX_DEBUG_MEM_)  &&  defined(__DEBUG_MEM__)
        UT_MemMemoryStat();
#endif

    mTBOX_RETURN(returnError);
}

LOCAL void *MemMalloc(
    uint32_t SegtSize
)
{
    void *PtrTmp = NULL;
    TKEL_err tkelErr;

    mTBOX_FCT_ENTER(MemMalloc);

    /* allocate buffer */
    tkelErr = TKEL_Malloc(SegtSize, &PtrTmp);

    if(tkelErr != TKEL_NO_ERR)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TKEL_Malloc failed\n"));
        mTBOX_RETURN(NULL);
    }

    mTBOX_RETURN(PtrTmp);
}

LOCAL int MemFree(
    void *Ptr
)
{
    TKEL_err tkelErr;

    mTBOX_FCT_ENTER(MemFree);

    /*mTBOX_TRACE((kTBOX_NIV_1, "Free: 0x%X\n", (int)Ptr));*/

    /* free buffer */
    tkelErr = TKEL_Free(Ptr);
    if(tkelErr != TKEL_NO_ERR)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TKEL_Free failed, %d\n"));
        mTBOX_RETURN(-1);
    }

    mTBOX_RETURN(0);
}
