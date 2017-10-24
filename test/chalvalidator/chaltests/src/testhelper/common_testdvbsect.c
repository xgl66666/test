/*****************************************************************************

File name  : dvbsect.c
Description: Functions for displaying DVBSI data
COPYRIGHT (C) STMicroelectronics 2004.

*****************************************************************************/

/* Includes --------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>

#include "crules.h"
#include "testhelper.h"
#include "testdvb_si.h"
#include "tbox.h"
#include "tdal_dmd.h"

/* Global definitions ----------------------------------------------------- */

#define OUTPUT_HEADER       0
#define OUTPUT_BYTES        0

/* macro to limit the value y to a maximum of x in which case defaults to z */
#define LIMITED_TO_(x,y,z)  (( y < x ) ? y : z )

/* Local Variables -------------------------------------------------------- */

/*
**  Service Status types
*/
static char *StatusTypeNames[] = {
                                    "Undefined",
                                    "Stopped",
                                    "Starts soon",
                                    "Paused",
                                    "Running" };

static char *ServiceTypeNames[] = {
                                    "resvd",
                                    "Digital TV",
                                    "Digital Radio",
                                    "Teletext",
                                    "NVOD Ref",
                                    "NVOD Time-shifted",
                                    "Mosaic",
                                    "PAL coded",
                                    "SECAM coded",
                                    "D/D2-MAC",
                                    "FM Radio",
                                    "NTSC coded" };

tTestHelper_DescriptorInfo Desc;

tTestHelper_ServiceList ServiceList;

/*-------------------------------------------------------------------------
 * Function : FecToStr
 * Input    : FECRate
 * Output   :
 * Return   : String
 * ----------------------------------------------------------------------*/
#if defined (MEDIUM_SAT) || defined (MEDIUM_TER)
static char *FecToStr(tTDAL_DMD_Convolution FECRate)
{
    switch(FECRate)
    {
	case eTDAL_DMD_FEC_NONE:return("NONE");
	case eTDAL_DMD_FEC_AUTO:return("AUTO");
	case eTDAL_DMD_FEC_1_2:return("1_2");
	case eTDAL_DMD_FEC_2_3:return("2_3");
    case eTDAL_DMD_FEC_3_4:return("3_4");
	case eTDAL_DMD_FEC_4_5:return("4_5");
	case eTDAL_DMD_FEC_5_6:return("5_6");
	case eTDAL_DMD_FEC_6_7:return("6_7");
	case eTDAL_DMD_FEC_7_8:return("7_8");
	case eTDAL_DMD_FEC_8_9:return("8_9");
	default:break;
    }
    return("???");
}
#endif

#ifdef MEDIUM_SAT
/*-------------------------------------------------------------------------
 * Function : PolarityToStr
 * Input    : STTUNER_Polarization_t type variable
 * Output   :
 * Return   : Return Polarization type in string form
 * ----------------------------------------------------------------------*/
static char *PolarityToStr(tTDAL_DMD_LNB_Polarization Polarization)
{
    switch (Polarization)
    {
	case eTDAL_DMD_POLAR_ALL:return("ALL");
	case eTDAL_DMD_POLAR_HORIZONTAL:return("HORIZONTAL");
	case eTDAL_DMD_POLAR_VERTICAL:return("VERTICAL");
	case eTDAL_DMD_POLAR_CIRCULAR_LEFT:return("CIRCULAR_LEFT");
	case eTDAL_DMD_POLAR_CIRCULAR_RIGHT:return("CIRCULAR_RIGHT");
	default:break;
    }

    return("???");
}
#endif

/*-------------------------------------------------------------------------
 * Function : StreamTypeNameLookup
 *
 * Input    : Stream type index
 * Output   :
 * Return   : pointer to Stream Name
 * ----------------------------------------------------------------------*/
static char *StreamTypeNameLookup( uint8_t Index )
{
    static char* StreamTypes[]=
    {
        "Reserved",
        "VidMpeg1",
        "VidMpeg2",
        "AudMpeg1",
        "AudMpeg2",
        "PrivSect",
        "PrivData",
        "MHEG",
        "DSM-CC",
        "Other",
        "type A",
        "type B",
        "type C",
        "type D",
        "type E"
    };

    if ( Index < 0xf )
        return StreamTypes[Index];
    else if ( Index < 0x7f )
        return StreamTypes[0];
    else
        return "UserPriv";
}

/*-------------------------------------------------------------------------
 * Function : GetDescriptorData
 *            Return pointer to data start of requested descriptor
 * Input    : uint8_t *Buff_p, uint16_t BuffSize, uint8_t DescriptorTag
 * Output   :
 * Return   : pointer to start of Descriptor Data or NULL if not found
 * ----------------------------------------------------------------------*/
static uint8_t *GetDescriptorData( uint8_t *Buff_p, uint16_t BuffSize, uint8_t DescriptorTag )
{
    uint16_t     Length;
    uint8_t     *Desc_p;
    uint8_t      i,j;

    memset( &Desc, 0, sizeof(Desc));
    for( Length = 0, Desc_p = Buff_p; Length < BuffSize; )
    {
        if ( Length > BuffSize )
            break;

        Desc.Tag = Desc_p[0];
        Desc.Length = Desc_p[1];

        switch ( Desc_p[0] ) /* descriptor tag */
        {
        case ECM_DESCRIPTOR:
            Desc.Info.ECMPid = extract(&Desc_p[4], PID_MASK);
            break;

        case ISO_639_LANGUAGE_DESCRIPTOR:
            break;

        case SERVICE_DESCRIPTOR:
            Desc.Info.Service.Type = Desc_p[2];
            Desc.Info.Service.ProviderLen = Desc_p[3];
            Desc.Info.Service.ProviderName_p = (char*) &Desc_p[4];
            Desc.Info.Service.ServiceLen = Desc_p[4 + Desc.Info.Service.ProviderLen];
            Desc.Info.Service.ServiceName_p = (char*) &Desc_p[5 + Desc.Info.Service.ProviderLen];
            break;

        case SHORT_EVENT_DESCRIPTOR:
            memset( Desc.Info.ShortEvent.Language, 0, 4 );
            memcpy( Desc.Info.ShortEvent.Language, &Desc_p[2], 3);
            Desc.Info.ShortEvent.NameLen = Desc_p[5];
            Desc.Info.ShortEvent.Name_p = (char*) &Desc_p[6];
            Desc.Info.ShortEvent.TextLen = Desc_p[6+Desc.Info.ShortEvent.NameLen];
            Desc.Info.ShortEvent.Text_p = (char*) &Desc_p[7+Desc.Info.ShortEvent.NameLen];
            break;

        case SUBTITLING_DESCRIPTOR:
            memset( Desc.Info.Subtitle.Language, 0, 4 );
            memcpy( Desc.Info.Subtitle.Language, &Desc_p[2], 3);
            Desc.Info.Subtitle.Type = Desc_p[5];
            Desc.Info.Subtitle.CompositionId = (Desc_p[6] << 8) + Desc_p[7];
            Desc.Info.Subtitle.AnciliaryId = Desc_p[8] * 256 + Desc_p[9];
            break;

        case TELETEXT_DESCRIPTOR:
            for ( i = 2, j = 0; (i < Desc_p[1]) && (j < 2); i+=5, j++ )
            {
                memset( Desc.Info.Teletext[j].Language, 0, 4 );
                memcpy( Desc.Info.Teletext[j].Language, &Desc_p[i], 3);
                Desc.Info.Teletext[j].Type = ( Desc_p[i+3] >> 3 ) & 0x1f;
                Desc.Info.Teletext[j].Magazine = Desc_p[i+3] & 0x7;
                Desc.Info.Teletext[j].Page = Desc_p[i+4];
            }
            break;
        default:
            break;
        }

        if ( Desc_p[0] == DescriptorTag )
            return Desc_p;                  /* return pointer to data */

        Length += 2 + Desc_p[1];
        Desc_p += 2 + Desc_p[1];
    }

    return NULL;
} /* end GetDescriptorData */

/*-------------------------------------------------------------------------
 * Function : MJD2YMD
 *          : Modified Julian Date to Year/Month/Day
 * Input    : MJD
 * Output   : Year, Month, Day
 * Return   : None
 * ----------------------------------------------------------------------*/
void MJD2YMD( uint16_t MJD, uint16_t *Year, uint16_t *Month, uint16_t *Day )
{
    uint8_t K;

    *Year = (int)((MJD - 15078.2)/365.25);
    *Month = (int)((MJD - 14956.1 - (*Year *365.25))/30.6001);
    *Day = MJD - 14956 - (int)(*Year * 365.25) - (int)(*Month * 30.6001);
    if (( *Month == 14 ) || ( *Month == 15 ))
        K = 1;
    else
        K = 0;
    *Year += K;
    *Month = *Month - 1 - K * 12;
}

/*-------------------------------------------------------------------------
 * Function : DVB_HeaderExtract
 * Input    : Data_p, Header_p
 * Output   :
 * Return   : None
 * ----------------------------------------------------------------------*/
void TestHelper_DVBHeaderExtract( uint8_t *Data_p, tTestHelper_SectionHeader *Header_p )
{
    /* extract the general data */
    Header_p->TableId           = Data_p[0];
    Header_p->Indicators        = Data_p[1] & 0x0f;
    Header_p->SectionLength     = 3 + extract(&Data_p[1], LENGTH_MASK);
    Header_p->StreamId          = extract(&Data_p[3], PROG_MASK);
    Header_p->VersionNumber     = GET_VERSION_NUMBER(Data_p[5]);
    Header_p->CurrentNext       = Data_p[5] & CURRENT_NEXT_INDICATOR_MASK;
    Header_p->SectionNumber     = Data_p[6];
    Header_p->LastSectionNumber = Data_p[7];

#if 0 /*OUTPUT_HEADER*/
	mTBOX_PRINT((kTBOX_LF,"Table id     = 0x%x\t", Header_p->TableId ));
	mTBOX_PRINT((kTBOX_LF,"Indicators   = %d\t", Header_p->Indicators ));
	mTBOX_PRINT((kTBOX_LF,"Section Len  = %d\t", Header_p->SectionLength ));
	mTBOX_PRINT((kTBOX_LF,"Stream Id    = %d\n", Header_p->StreamId ));
	mTBOX_PRINT((kTBOX_LF,"Version num  = %d\t", Header_p->VersionNumber ));
	mTBOX_PRINT((kTBOX_LF,"CurrNext Ind = %s\t", Header_p->CurrentNext ? "CURR":"NEXT" ));
	mTBOX_PRINT((kTBOX_LF,"Section Num  = %d\t", Header_p->SectionNumber ));
	mTBOX_PRINT((kTBOX_LF,"LastSect Num = %d\n", Header_p->LastSectionNumber ));
#endif
#if 0 /* OUTPUT_BYTES */
	mTBOX_PRINT((kTBOX_LF,"Data  %02x %02x %02x %02x %02x %02x %02x %02x (%02x %5d)\n",
				 Data_p[0], Data_p[3], Data_p[4], Data_p[5],
				 Data_p[6], Data_p[7], Data_p[8], Data_p[9],
				 Header_p->SectionLength, Header_p->SectionLength ));
#endif
}

/*-------------------------------------------------------------------------
 * Function : DVB_BufferDecode
 * Input    : Buffer_p, Display
 * Output   :
 * Return   : Error code
 * ----------------------------------------------------------------------*/
void TestHelper_DVBBufferDecode( uint8_t *Buffer_p, bool Display )
{
    tTestHelper_SectionHeader Header;
    uint16_t i,j;
    uint16_t DescLoopLength;
	uint8_t  Day;    /* Day value   [1-31] */
	uint8_t  Month;  /* Month value [1-12] */
	uint16_t Year;   /* Year (ie : 2008)   */
	uint8_t  Hour;   /* Hour [0-23]        */
	uint8_t  Minute; /* Minute [0-59]      */
	uint8_t  Second; /* Minute [0-59]      */

    TestHelper_DVBHeaderExtract(Buffer_p, &Header);

    switch ( Header.TableId )
    {
    case PAT_TABLE_ID: /* Program Association Table */
    {
        memset(&ServiceList, 0, sizeof(ServiceList)); /* clear old data */

        mTBOX_PRINT((kTBOX_LF,"---- PAT - Transport Id %5d ----\n", Header.StreamId));
        mTBOX_PRINT((kTBOX_LF,"Program PMT Pid\n"));
        mTBOX_PRINT((kTBOX_LF,"------- -------\n"));

        for ( i = PAT_HEAD_SIZE, j = 0;
              i < (Header.SectionLength - CRC_SIZE);
              i += PAT_DATA_SIZE )
        {
            uint16_t ProgNo, PmtPid;

            ProgNo = extract(&Buffer_p[i], PROG_MASK);
            PmtPid = extract(&Buffer_p[i+2], PID_MASK);
            if ( ProgNo != 0 && ProgNo != 0xFFFE )
            {
                if ( j < MAX_SERVICES)
                {
                    ServiceList.ServiceInfo[j].ProgNo = ProgNo;
                    ServiceList.ServiceInfo[j].PmtPid = PmtPid;
                    sprintf ( ServiceList.ServiceInfo[j].ServiceName, "Channel %05d", ProgNo);
                    j++;
                }
            }

            mTBOX_PRINT((kTBOX_LF,"%7d %7d %s\n",
						 ProgNo, PmtPid,
						 ProgNo==0?"NwID":""));
        }

        ServiceList.NoOfServices = j;
    }
    break;

    case PMT_TABLE_ID: /* Program Map Table */
    {
        uint16_t P_Info_Length;
        uint16_t ES_Info_Length;

        P_Info_Length  = extract(&Buffer_p[10], LENGTH_MASK);
        GetDescriptorData( &Buffer_p[PMT_HEAD_SIZE], P_Info_Length, ECM_DESCRIPTOR );

        mTBOX_PRINT((kTBOX_LF,"---- PMT - Transport Id %5d ----\n", Header.StreamId));
        mTBOX_PRINT((kTBOX_LF,"  %-20s  Pid %5d  ECM %04X\n",
					 "pcr",
					 extract(&Buffer_p[ 8], PID_MASK),
					 Desc.Info.ECMPid ));

        for ( i = PMT_HEAD_SIZE + P_Info_Length;
              i < (Header.SectionLength - CRC_SIZE);
              i += (PMT_DATA_SIZE + ES_Info_Length))
        {
            ES_Info_Length = extract(&Buffer_p[i+3], LENGTH_MASK);
            GetDescriptorData( &Buffer_p[i+PMT_DATA_SIZE], ES_Info_Length, 0 );
            mTBOX_PRINT((kTBOX_LF,"  %-20s  Pid %5d  ECM %04X\n",
						 StreamTypeNameLookup(Buffer_p[i]),
						 extract(&Buffer_p[i+1], PID_MASK),
						 Desc.Info.ECMPid ));
        }
    }
    break;

    case NIT_TABLE_ID: /* Network Information Table */
    {
        uint16_t n_desc_length;
        uint16_t t_loop_length;
        uint16_t t_desc_length;

        n_desc_length = extract(&Buffer_p[8], LENGTH_MASK);
        t_desc_length = 0;

        /* Get Network name from descriptor */
        if ( Buffer_p[NIT_HEAD_SIZE] == NETWORK_NAME_DESCRIPTOR )
        {
            mTBOX_PRINT((kTBOX_LF,"Name \'%-*.*s\'",
						 Buffer_p[NIT_HEAD_SIZE+1], Buffer_p[NIT_HEAD_SIZE+1],
						 &Buffer_p[NIT_HEAD_SIZE+2] ));
        }

        if ( Header.SectionNumber == 0 )
        {
            mTBOX_PRINT((kTBOX_LF,"---- NIT - Id %5d ----\n", Header.StreamId ));
            mTBOX_PRINT((kTBOX_LF,"TS_Id freq (kHz) Poln   FEC SRate\n"));
            mTBOX_PRINT((kTBOX_LF,"----- ---------- -----  --- -----\n"));
        }

        t_loop_length = extract(&Buffer_p[NIT_HEAD_SIZE+n_desc_length], LENGTH_MASK);
        for ( i = NIT_HEAD_SIZE + n_desc_length + 2;
            i < t_loop_length;
            i += NIT_DATA_SIZE + t_desc_length )
        {
            uint32_t                   Frequency;
            tTDAL_DMD_LNB_Polarization Polarization;
            tTDAL_DMD_Convolution      FECRates;
            uint16_t                   SymbolRate;

            mTBOX_PRINT((kTBOX_LF,"%5d ",  extract(&Buffer_p[i], PROG_MASK)));

            Frequency  = extractBCD( &Buffer_p[i+ 8] );
            Frequency *= 100;
            Frequency += extractBCD( &Buffer_p[i+ 9] );
            Frequency *= 100;
            Frequency += extractBCD( &Buffer_p[i+10] );
            Frequency *= 100;
            Frequency += extractBCD( &Buffer_p[i+11] );
            Frequency *= 10;
            mTBOX_PRINT((kTBOX_LF,"%10d ", Frequency ));

            switch ( ( Buffer_p[i+14] & 0x60 ) >> 5 )
            {
                case 0x0:
                    Polarization = eTDAL_DMD_POLAR_HORIZONTAL;
                    break;
                case 0x1:
                    Polarization = eTDAL_DMD_POLAR_VERTICAL;
                    break;
                default:
                    Polarization = eTDAL_DMD_POLAR_ALL;
                    break;
            }
#if defined (MEDIUM_SAT)
            mTBOX_PRINT((kTBOX_LF,"%-6.6s ", PolarityToStr(Polarization) ));
#endif
            /* convert DVB format to STTUNER format */
            switch ( Buffer_p[i+18] & 0x0f )
            {
                case 0x1:
                    FECRates = eTDAL_DMD_FEC_1_2;
                    break;
                case 0x2:
                    FECRates = eTDAL_DMD_FEC_2_3;
                    break;
                case 0x3:
                    FECRates = eTDAL_DMD_FEC_3_4;
                    break;
                case 0x4:
                    FECRates = eTDAL_DMD_FEC_5_6;
                    break;
                case 0x5:
                    FECRates = eTDAL_DMD_FEC_6_7;
                    break;
                default:
                    FECRates = eTDAL_DMD_FEC_AUTO;
                    break;
            }
#if defined (MEDIUM_SAT) || defined (MEDIUM_TER)
            mTBOX_PRINT((kTBOX_LF,"%s ", FecToStr(FECRates) ));
#endif
            /* symbol rate in M/Symbols / 1000 */
            SymbolRate  = extractBCD( &Buffer_p[i+15] );
            SymbolRate *= 100;
            SymbolRate += extractBCD( &Buffer_p[i+16] );
            SymbolRate *= 100;
            mTBOX_PRINT((kTBOX_LF,"%5d\n", SymbolRate ));

            t_desc_length = extract(&Buffer_p[i+4], LENGTH_MASK);
        }
    }
    break;

    case SDT_TABLE_ID: /* Service Descriptor Table */
        if ( Header.SectionNumber == 0 )
        {
            mTBOX_PRINT((kTBOX_LF,"---- SDT - Transport Id %d ----\n", Header.StreamId ));
            mTBOX_PRINT((kTBOX_LF,"prog#  status  CA   type            Provider name       service name\n"));
            mTBOX_PRINT((kTBOX_LF,"-----  ------  ---- --------------  ------------------  --------------------\n"));
        }

        for ( i = SDT_HEAD_SIZE;
            i < (Header.SectionLength - CRC_SIZE);
            i += SDT_DATA_SIZE + DescLoopLength )
        {
            uint16_t ProgNo;

            ProgNo = extract(&Buffer_p[i], PROG_MASK);
            mTBOX_PRINT((kTBOX_LF,"%5d  ", ProgNo));
            /* schedule_flag */
            /* present_following_flag */
            mTBOX_PRINT((kTBOX_LF,"%-7.7s ", StatusTypeNames[LIMITED_TO_(5,((Buffer_p[i+3] & 0xE0 ) >> 5),0)] ));
            mTBOX_PRINT((kTBOX_LF,"%-4.4s ", ((Buffer_p[i+3] & 0x10 ) >> 4==0) ? "free":"lock"));

            /*
            **  descriptor data
            */
            DescLoopLength = extract(&Buffer_p[i+3], LENGTH_MASK);

            GetDescriptorData( &Buffer_p[i+SDT_DATA_SIZE],
                               DescLoopLength,
                               SERVICE_DESCRIPTOR );

            if ( Desc.Tag == SERVICE_DESCRIPTOR )
            {
                mTBOX_PRINT((kTBOX_LF,"%-15s ", ServiceTypeNames[LIMITED_TO_(11,Desc.Info.Service.Type,0)] ));
                if ( Desc.Info.Service.ProviderLen > 0 )
                {
                    mTBOX_PRINT((kTBOX_LF,"%-20.*s", Desc.Info.Service.ProviderLen,
								 Desc.Info.Service.ProviderName_p ));
                }
                else
                {
                    mTBOX_PRINT((kTBOX_LF,"%-20.20s", "Unknown"));
                }

                for(j = 0; j < ServiceList.NoOfServices; j++)
                {
                    if ( ProgNo == ServiceList.ServiceInfo[j].ProgNo )
                        break;
                }

                if ( Desc.Info.Service.ServiceLen > 0 )
                {
                    mTBOX_PRINT((kTBOX_LF,"%-20.*s", Desc.Info.Service.ServiceLen,
                                  Desc.Info.Service.ServiceName_p ));

                    if ( j < ServiceList.NoOfServices )
                    {
                        if ( Desc.Info.Service.ServiceLen >= MAX_SERVICE_NAME_LENGTH )
                            Desc.Info.Service.ServiceLen = MAX_SERVICE_NAME_LENGTH-1;

                        strncpy(ServiceList.ServiceInfo[j].ServiceName,
                                Desc.Info.Service.ServiceName_p,
                                Desc.Info.Service.ServiceLen);
                        ServiceList.ServiceInfo[j].ServiceName[Desc.Info.Service.ServiceLen] = '\0';
                    }
                }
                else
                {
                    mTBOX_PRINT((kTBOX_LF,"%-20.20s", "Unknown"));
                    if ( j < ServiceList.NoOfServices )
                    {
                        sprintf(ServiceList.ServiceInfo[j].ServiceName, "Channel %05d", ProgNo);
                    }
                }
            }
            mTBOX_PRINT((kTBOX_LF,"\n"));
        }
        break;

    case EIT_TABLE_ID: /* Event Information Table */
        {
            uint16_t Year, Month, Day;

            if ( Header.SectionNumber == 0 )
            {
                mTBOX_PRINT((kTBOX_LF,"---- EIT - Id %d ----\n", Header.StreamId ));
                mTBOX_PRINT((kTBOX_LF,"EvId Start Date/Time     Duration Status  CA\n"));
                mTBOX_PRINT((kTBOX_LF,"---- ------------------- -------- ------- ----\n"));
            }

            for ( i = EIT_HEAD_SIZE;
                  i < (Header.SectionLength - CRC_SIZE);
                  i += EIT_DATA_SIZE + DescLoopLength )
            {
                /*
                **  descriptor data
                */
                MJD2YMD( extract(&Buffer_p[i+2],PROG_MASK), &Year, &Month, &Day );
                mTBOX_PRINT((kTBOX_LF,"%4x %04d/%02d/%02d %02x:%02x:%02x %02x:%02x:%02x %-7.7s %-4.4s\t",
							 extract(&Buffer_p[i], PROG_MASK),
							 Year+1900, Month, Day,
							 Buffer_p[i+4],Buffer_p[i+5],Buffer_p[i+6],
							 Buffer_p[i+7],Buffer_p[i+8],Buffer_p[i+9],
							 StatusTypeNames[LIMITED_TO_(5,((Buffer_p[i+10] & 0xE0 ) >> 5),0)],
							 (((Buffer_p[i+10] & 0x10 ) >> 4)==0) ? "free":"lock" ));

                DescLoopLength = extract(&Buffer_p[i+10], LENGTH_MASK);
                GetDescriptorData( &Buffer_p[i+EIT_DATA_SIZE],
                                   DescLoopLength,
                                   SHORT_EVENT_DESCRIPTOR );

                mTBOX_PRINT((kTBOX_LF,"%-*.*s\n",
							 Desc.Info.ShortEvent.NameLen,
							 Desc.Info.ShortEvent.NameLen,
							 Desc.Info.ShortEvent.Name_p ));
                mTBOX_PRINT((kTBOX_LF,"%-*.*s\n",
							 Desc.Info.ShortEvent.TextLen,
							 Desc.Info.ShortEvent.TextLen,
							 Desc.Info.ShortEvent.Text_p));

                mTBOX_PRINT((kTBOX_LF,"%d,%d,%d\n",
							 Desc.Info.ShortEvent.NameLen,
							 Desc.Info.ShortEvent.TextLen,
							 DescLoopLength));
            }
        }
        break;

    case TDT_TABLE_ID: /* Time Date Table */
    case TOT_TABLE_ID: /* Time Offset Table */
        {
			/* Get the date */
			/* ------------ */
			/* The following code is applicable between 1900 (01 March) to 2100 (28 February) */
			uint32_t MJD,Y,M,D,K;

			MJD = (Buffer_p[3] << 8) | (Buffer_p[4]);
			Y   = ((MJD * 100) - 1507820) / 36525;
			M   = (((MJD * 100) - 1495610) - (Y * 36525)) / 3060;
			D   = MJD - 14956 - ((Y * 36525) / 100) - (( M * 3060) / 100);
			if ((M==14) || (M==15)) K=1; else K=0;
			Day   = D;
			Month = M -1 - (K * 12);
			Year  = 1900 + Y + K;

			/* Get the time */
			/* ------------ */
			Hour   = ((Buffer_p[5] >> 4) * 10) + (Buffer_p[5] & 0xF);
			Minute = ((Buffer_p[6] >> 4) * 10) + (Buffer_p[6] & 0xF);
			Second = ((Buffer_p[7] >> 4) * 10) + (Buffer_p[7] & 0xF);
			
			/* Display the TDT content */
			/* ======================= */
			mTBOX_PRINT((kTBOX_LF,"%s-Day  : %02d/%02d/%04d\n",
						 ((Header.TableId == TDT_TABLE_ID)? "TDT": (Header.TableId == TOT_TABLE_ID)? "TOT": "T?T"),
						 Day ,Month ,Year));
			mTBOX_PRINT((kTBOX_LF,"%s-Time : %02d:%02d:%02d\n",
						 ((Header.TableId == TDT_TABLE_ID)? "TDT": (Header.TableId == TOT_TABLE_ID)? "TOT": "T?T"),
						 Hour,Minute,Second));
		}
		break;

    default:
        break;
    }
}

