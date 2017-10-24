#include "tester_prefs_dmd.h"
/* structure to redo correctly 
 * for SAT:
 * name + PMT + Video + Audio + PCR
 * for TER
 * name + Video + Audio + PCR */

typedef struct {
	char *name;
	unsigned int frequency; /* Satellite in KHz else in Hz */
	unsigned int symbolRate; /* in Baud */
	tTDAL_DMD_Convolution convolution;
	tTDAL_DMD_LNB_Polarization polarization;
}t_testDemux;

static t_testDemux DemuxChannel[NUMBER_CHANNEL] =
{
#if defined(MEDIUM_SAT)
	{"euroVsport !!!", 400, 410, 420, 410},
	{"euronews", 1034, 2221, 2233, 2221},
	{"rtl", 44, 163, 104, 163},
	{"rtl2", 45, 166, 128, 166},
	{"tv5", 36, 164, 112, 164},
	{"cnn", 3946, 165, 100, 165},
	{"arte", 400, 401, 402, 401},
	{"mosaïc", 40, 175, 120, 175},
	{"3sat", 200, 210, 220, 210},
	{"bvn", 1025, 515, 96 8190},
	{"eins extra", 100, 101, 102, 101},
	{"flirtcafe.tv", 109, 751, 752, 751},
	{"sat1", 106, 1791, 1792, 1791},
	{"satmode teaser", 1110, 1111, 1112, 1111},
	{"sky news", 4131, 305, 306, 131},
	{"sportswin.tv", 34, 308, 256, 308},
	{"super rtl", 45, 165, 120, 165}	
#elif defined(MEDIUM_TER)
{"tf1", 0X78, 0x82, 0x78},
{"france2", 120, 130, 120},
{"m6", 120, 130, 120},
{"canal+", 160, 80, 160},
{"direct8", 160, 80, 160}
#elif defined(MEDIUM_CAB)
	{"internal player", 160, 80, 160}
#endif
};
