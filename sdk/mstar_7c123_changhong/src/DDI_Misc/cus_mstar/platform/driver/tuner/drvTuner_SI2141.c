#include "MsCommon.h"
#include "Board.h"
#include "drvIIC.h"

#if IF_THIS_TUNER_INUSE(TUNER_SI2141)

#include <string.h>
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "Si2141_44_24_L1_API.h"
#include "si2144_41_i2c_api.h"

/* Si2141_44_24 API Specific Includes */
#include "Si2141_44_24_L2_API.h"               /* Include file for this code */
#define SI2141_I2C_ADDR   0xC0

#define DEFAULT_DVB_TYPE ISDBT
#define C_Si2141_44_24_IF_FREQUENCY         (5000)  /* kHz */
#define C_Si2141_44_24_LO_ADDRESS           (SI2141_I2C_ADDR)
#define TUNER_DTV_IQ_SWAP               0 // iq swap


static MS_U8 SI2141_Address = 0xC0;
static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM];
static L1_Si2141_44_24_Context *pstSI2141_API = NULL;
static MS_BOOL _u8TunerInited[MAX_FRONTEND_NUM];
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_Network_Type_DVBT =1,
    E_Network_Type_DVBC ,
    E_Network_Type_Analog_T ,
    E_Network_Type_Analog_C ,
    E_Network_Type_DVBT2,
    E_Network_Type_ATSC,
    E_Network_Type_ISDBT,
    E_Network_Type_ISDBC,
    E_Network_Type_DVBS,
    E_Network_Type_DTMB,
    E_Network_Type_QAM_US,
} Network_Type;

/* Setup properties to switch standards. */
static void SI2141_SetIfDemod(TUNER_EN_TER_BW_MODE eBandWidth, Network_Type eNetworktype)
{
    // set a global here to save the video standard,  because it's not always available from the caller.
        switch (eNetworktype)
        {
           case E_Network_Type_DVBT:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_DVBT;
                if(eBandWidth == TUNER_BW_MODE_6MHZ)
                    pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_6MHZ;
                else if (eBandWidth == TUNER_BW_MODE_7MHZ)
                    pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_7MHZ;
                else
                    pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_8MHZ;

                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_INVERTED;

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 0;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=17 to get IF_AGC=1.24Vrms in DVB-T when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =20;
                printf("***[SI2141_SetIfDemod_DVB-T] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }
                pstSI2141_API->prop->dtv_lif_freq.offset =C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }
                break;
           case E_Network_Type_DVBT2:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_DVBT;
                if(eBandWidth == TUNER_BW_MODE_8MHZ)
                    pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_8MHZ;
                else if (eBandWidth == TUNER_BW_MODE_7MHZ)
                    pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_7MHZ;
                else
                    pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_6MHZ;
                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_INVERTED;

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_39;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_4;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 210;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=32 to get IF_AGC=1.24Vrms in DVB-T2 when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =25;
                printf("***[SI2141_SetIfDemod_DVB-T2] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }
                pstSI2141_API->prop->dtv_lif_freq.offset =C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }
                break;
           case E_Network_Type_DVBC:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_DVBC;
                pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_8MHZ;
                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_INVERTED;

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 0;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=14 to get IF_AGC=1.20Vrms in DVB-C when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =20;
                printf("***[SI2141_SetIfDemod_DVB-C] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }
                pstSI2141_API->prop->dtv_lif_freq.offset =C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }
                break;
           case E_Network_Type_ISDBT:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_ISDBT;
                pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_6MHZ;
                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 0;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=55 to get IF_AGC 1.0v in MSB1400 on 149B-C01A when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =37;
                printf("***[SI2141_SetIfDemod_ISDBT] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }

                pstSI2141_API->prop->dtv_lif_freq.offset =C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }
                break;
           case E_Network_Type_ATSC:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_ATSC;
                pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_6MHZ;
                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL;

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 0;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=17 to get IF_AGC=1.24Vrms in DVB-T when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =20;
                printf("***[SI2141_SetIfDemod_ATSC] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }
                pstSI2141_API->prop->dtv_lif_freq.offset = C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }
                /*  api->prop->tuner_return_loss.mode = Si2141_44_24_TUNER_RETURN_LOSS_PROP_RETURNLOSS_DEFAULT;
                 if (Si2141_44_24_L1_SetProperty2(api, Si2141_44_24_TUNER_RETURN_LOSS_PROP_CODE) != 0)
                {
                    mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_TUNER_RETURN_LOSS_PROP_CODE\n"));
                }
                */
                pstSI2141_API->prop->dtv_pga_limits.max = 56;
                pstSI2141_API->prop->dtv_pga_limits.min = 24;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API,Si2141_44_24_DTV_PGA_LIMITS_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_PGA_LIMITS_PROP\n"));
                }

                break;
           case E_Network_Type_QAM_US:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_QAM_US;
                pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_6MHZ;
                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL;

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 0;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=17 to get IF_AGC=1.24Vrms in DVB-T when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =20;
                 printf("***[SI2141_SetIfDemod_QAM_US] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }
                pstSI2141_API->prop->dtv_lif_freq.offset = C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }

                /*api->prop->tuner_return_loss.mode = Si2141_44_24_TUNER_RETURN_LOSS_PROP_MODE_TERRESTRIAL;
                 if (Si2141_44_24_L1_SetProperty2(api, Si2141_44_24_TUNER_RETURN_LOSS_PROP_CODE) != 0)
                {
                    mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_TUNER_RETURN_LOSS_PROP_CODE\n"));
                }*/
                pstSI2141_API->prop->dtv_pga_limits.max = 56;
                pstSI2141_API->prop->dtv_pga_limits.min = 24;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API,Si2141_44_24_DTV_PGA_LIMITS_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_PGA_LIMITS_PROP\n"));
                }
                break;
           case E_Network_Type_DTMB:
                pstSI2141_API->prop->dtv_mode.modulation = Si2141_44_24_DTV_MODE_PROP_MODULATION_DTMB;
                pstSI2141_API->prop->dtv_mode.bw = Si2141_44_24_DTV_MODE_PROP_BW_BW_8MHZ;
                pstSI2141_API->prop->dtv_mode.invert_spectrum = Si2141_44_24_DTV_MODE_PROP_INVERT_SPECTRUM_INVERTED;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_MODE_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_MODE_PROP\n"));
                }
                pstSI2141_API->prop->dtv_agc_speed.if_agc_speed = Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
                pstSI2141_API->prop->dtv_agc_speed.agc_decim = Si2141_44_24_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_AGC_SPEED_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_AGC_SPEED_PROP\n"));
                }
                pstSI2141_API->prop->dtv_initial_agc_speed_period.period = 0;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_INITIAL_AGC_SPEED_PERIOD_PROP\n"));
                }
                /* AMP=55 to get IF_AGC 1.0v in MSB1400 on 149B-C01A when AMP is controlled by demod AGC (DTV_AGC_SOURCE=DLIF_AGC_3DB) */
                pstSI2141_API->prop->dtv_lif_out.amp =37;
                printf("***[SI2141_SetIfDemod_DTMB] dtv_lif_out.amp = %d ",pstSI2141_API->prop->dtv_lif_out.amp);

                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_OUT_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_OUT_PROP\n"));
                }

                pstSI2141_API->prop->dtv_lif_freq.offset =C_Si2141_44_24_IF_FREQUENCY;
                if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_DTV_LIF_FREQ_PROP) != 0)
                {
                    TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_DTV_LIF_FREQ_PROP\n"));
                }
                break;
           case E_Network_Type_DVBS:
           default:
                printf("[Error]%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);
                break;
        }
}

static Network_Type Dtv_system_enum_conversion(MS_U32 u32BroadCastType)
{
    Network_Type ret_dtv_sys = E_Network_Type_ISDBT;

    switch (u32BroadCastType)
    {
        case ISDBT:
            ret_dtv_sys = E_Network_Type_ISDBT;
            break;
        case DVBT:
            ret_dtv_sys = E_Network_Type_DVBT;
            break;
        case DVBC:
            ret_dtv_sys = E_Network_Type_DVBC;
            break;
        case DVBS:
            ret_dtv_sys = E_Network_Type_DVBS;
            break;
        case DTMB:
            ret_dtv_sys = E_Network_Type_DTMB;
            break;
        case ATSC:
            ret_dtv_sys = E_Network_Type_ATSC;
            break;
        case DVBT2:
            ret_dtv_sys = E_Network_Type_DVBT2;
            break;
        default:
            ret_dtv_sys = E_Network_Type_DVBT;
            break;
    }
    return ret_dtv_sys;
}

static MS_BOOL Si2141_44_24_GetRSSILevel(float *strength_dBm)
{
    float rssi=0;

    if(pstSI2141_API==NULL)
    {
        TUNER_ERR(("Tuner not inited\n"));
        return FALSE;
    }

    if (Si2141_44_24_L1_TUNER_STATUS(pstSI2141_API)!=0) {
        TUNER_ERR((" [Silabs]: ERROR_READING_COMMAND Si2141_44_24_TUNER_STATUS\n"));
        return FALSE;
    }

    rssi=(float) pstSI2141_API->rsp->tuner_status.rssi;
    *strength_dBm = rssi;
    return TRUE;
}

MS_BOOL MDrv_Tuner_SI2141_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam)
{
    MS_BOOL bRet = TRUE;
    int error;
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(u8TunerIndex);

    printf("Tuner SI2141 Init...\n");
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        u8TunerIndex = 0; //means I2C port
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        u8TunerIndex = 1;
    }
    else
    {
        printf("hwi2c_port number exceeds limitation\n");
        return FALSE;
    }

    if(pParam->pCur_Broadcast_type == NULL)
    {
        TUNER_ERR(("[%s][%d]Error parameter\n",__FUNCTION__,__LINE__));
        return FALSE;
    }
    else
    {
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;
    }

    if(pstSI2141_API==NULL)
    {
        TUNER_ERR(("[%s][%d]pstSI2141_API not init\n",__FUNCTION__,__LINE__));
        bRet = FALSE;
    }
    else
    { 
        if(_u8TunerInited[u8TunerIndex]==TRUE)
        {
            return TRUE;
        }
        /* Software Init */
        Si2141_44_24_L1_API_Init(pstSI2141_API, C_Si2141_44_24_LO_ADDRESS);
         /*** below power up setting is for successfully excuting Si2141_44_24_LoadFirmware_16 */
        pstSI2141_API->cmd->power_up.clock_mode =  Si2141_44_24_POWER_UP_CMD_CLOCK_MODE_XTAL;
        pstSI2141_API->cmd->power_up.en_xout    =  Si2141_44_24_POWER_UP_CMD_EN_XOUT_EN_XOUT;


        if (Si2141_44_24_Init(pstSI2141_API) != NO_Si2141_44_24_ERROR )
        {
          printf("\n");
          printf("ERROR ----------------------------------\n");
          printf("ERROR initializing the Si2141_44_24!\n");
          printf("ERROR ----------------------------------\n");
          printf("\n");

          bRet = FALSE;
        }
        else
        {
            /*** Enable external AGC1 control pin*/
            error=Si2141_44_24_L1_CONFIG_PINS (pstSI2141_API,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_GPIO1_MODE_NO_CHANGE,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_GPIO2_MODE_NO_CHANGE,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_AGC1_MODE_DTV_AGC,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_AGC1_READ_DO_NOT_READ,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_AGC2_MODE_NO_CHANGE,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_AGC2_READ_DO_NOT_READ,
                                                                 Si2141_44_24_CONFIG_PINS_CMD_XOUT_MODE_NO_CHANGE);
            printf ("Si2141_44_24_L1_CONFIG_PINS, ATV_DTV_AGC1, error 0x%02x: %s\n", error, Si2141_44_24_L1_API_ERROR_TEXT(error));
            _u8TunerInited[u8TunerIndex]=TRUE;
        }
    }

    return bRet;
}

MS_BOOL MDrv_Tuner_SI2141_SetTuner(MS_U8 u8TunerIndex,MS_U32 u32Freq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MS_BOOL bRet = FALSE;
    MS_U32 start_time = 0;
    MS_U32 timeout = 0;
    MS_U32 u32FreqHz = 0;
    Network_Type eNetworktype = E_Network_Type_ISDBT;
    static TUNER_EN_TER_BW_MODE ePreBW = TUNER_BW_MODE_6MHZ;
    unsigned char previous_dtv_mode = 0;
    unsigned char previous_agc_speed = 0;

    if(pstSI2141_API==NULL)
    {
        TUNER_ERR(("Tuner not inited\n"));
        return bRet;
    }

    TUNER_DBG(("[SI2141]Freq=%lu KHz, bw=%d MHz, mode=%ld\n", u32Freq, ucBw, *InitParam[u8TunerIndex].pCur_Broadcast_type));
    eNetworktype = Dtv_system_enum_conversion(*InitParam[u8TunerIndex].pCur_Broadcast_type);

    previous_dtv_mode=pstSI2141_API->prop->dtv_mode.modulation;
    previous_agc_speed=pstSI2141_API->prop->dtv_agc_speed.if_agc_speed;

    //TUNER_DBG(("Si2141_44_24_TunerSetFreq freq = %d; Band = %d Network = %d\n", (U16)Freq,eBandWidth, eNetworktype));
    pstSI2141_API->prop->tuner_lo_injection.band_1      = Si2141_44_24_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE;
    pstSI2141_API->prop->tuner_lo_injection.band_2      = Si2141_44_24_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE;
    pstSI2141_API->prop->tuner_lo_injection.band_3      = Si2141_44_24_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE;
    if (Si2141_44_24_L1_SetProperty2(pstSI2141_API, Si2141_44_24_TUNER_LO_INJECTION_PROP) != 0)
    {
        TUNER_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2141_44_24_TUNER_LO_INJECTION_PROP\n"));
    }

    switch (eNetworktype)
    {
       case E_Network_Type_DVBT:
            if ( (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_DVBT)
                || (previous_agc_speed!=Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO)
                || (ucBw != ePreBW)
                )
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_DVBT2:
            if ( (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_DVBT)
                || (previous_agc_speed!=Si2141_44_24_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_39)
                || (ucBw != ePreBW)
               )
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_DVBC:
            if (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_DVBC)
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_ISDBT:
            if (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_ISDBT)
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_ISDBC:
            if (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_ISDBC)
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_DTMB:
            if (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_DTMB)
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_ATSC:
            if (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_ATSC)
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_QAM_US:
            if (previous_dtv_mode != Si2141_44_24_DTV_MODE_PROP_MODULATION_QAM_US)
                SI2141_SetIfDemod(ucBw, eNetworktype);
            break;
       case E_Network_Type_Analog_T:
       case E_Network_Type_Analog_C:
       case E_Network_Type_DVBS:
       default:
            printf("[Error]%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);
            break;
    }

    u32FreqHz = u32Freq*1000;
    if (u32FreqHz < Si2141_44_24_TUNER_TUNE_FREQ_CMD_FREQ_MIN || u32FreqHz > Si2141_44_24_TUNER_TUNE_FREQ_CMD_FREQ_MAX)
    {
        printf("[Silabs]:<= Frequency out of Range\n");
        TUNER_ERR((" [Silabs]: <= Frequency out of Range\n"));
        return FALSE;
    }

    if (Si2141_44_24_L1_TUNER_TUNE_FREQ(pstSI2141_API, Si2141_44_24_TUNER_TUNE_FREQ_CMD_MODE_DTV,  u32FreqHz) != 0)
    {
        printf("[Silabs]:Error Si2141_44_24_L1_TUNER_TUNE_FREQ\n");
        TUNER_ERR((" [Silabs]: Error Si2141_44_24_L1_TUNER_TUNE_FREQ\n"));
        return FALSE;
    }

    /* wait for TUNINT, timeout is 140ms */
    timeout = 140;
    start_time = MsOS_GetSystemTime();
    while( MsOS_Timer_DiffTimeFromNow(start_time) < timeout )
    {
        if (Si2141_44_24_L1_CheckStatus(pstSI2141_API) != 0)
        {
            TUNER_ERR(("%s:%d, SI2141 init failed\n",__FUNCTION__,__LINE__));
            return FALSE;
        }
        if (pstSI2141_API->status->tunint)
        {
            bRet = TRUE;
            TUNER_DBG(("[SI2141]TUNER LOCK!!!!!\n"));
            break;
        }
    }

    /* wait for DTVINT, timeout is 40ms */
    timeout = 40;
    start_time = MsOS_GetSystemTime();
    while(MsOS_Timer_DiffTimeFromNow(start_time) < timeout )
    {
        if (Si2141_44_24_L1_CheckStatus(pstSI2141_API) != 0)
        {
            TUNER_ERR(("%s:%d, SI2141 init failed\n",__FUNCTION__,__LINE__));
            return FALSE;
        }
        if (pstSI2141_API->status->dtvint)
        {
            TUNER_DBG(("[SI2141]DTV LOCK!!!!!!\n"));
            bRet = TRUE;
            break;
        }
    }

    if (bRet == TRUE)
        ePreBW = ucBw;

    return bRet;
}

MS_BOOL MDrv_Tuner_SI2141_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_BOOL bRet = FALSE;
    MS_U8  buffer1[2] = {0xff, 0x00};
    MS_U8  buffer2[2] = {0xc0, 0x00};
    MS_U8  buffer3[2] = {0xfe, 0x00};
    MS_U8  replybyte   = 0;

    memset(_u8TunerInited, 0, sizeof(_u8TunerInited));

    TUNER_DBG(("[SI2141]CheckExist\n"));
    if(pu32channel_cnt==NULL)
    {
        TUNER_ERR(("Error parameter\n"));
        return bRet;
    }
    if(pstSI2141_API==NULL)
    {
        pstSI2141_API = (L1_Si2141_44_24_Context *)malloc(sizeof(L1_Si2141_44_24_Context));
        if(pstSI2141_API==NULL)
        {
            TUNER_ERR(("Error parameter\n"));
            return bRet;
        }
    }
    InitParam[u8TunerIndex].u8SlaveID = C_Si2141_44_24_LO_ADDRESS;    
    SI214X_ucI2cSetSlaveAddr(u8TunerIndex, SI2141_Address);
    SI214X_ucI2cWriteOnly(0xFF, buffer1, sizeof(buffer1));
    SI214X_ucI2cWriteOnly(0xFF, buffer2, sizeof(buffer2));
    SI214X_ucI2cReadOnly(0xFF, &replybyte, 1);
    SI214X_ucI2cWriteOnly(0xFF, buffer3, sizeof(buffer3));
    printf("[SI2141]replybyte=0x%x\n", replybyte);
    if (replybyte == 0x1D)
        {
        bRet = TRUE;     
        }
    else if(replybyte == 0x0D)//rework for doing "reboot" command after tuner init
    {
        if(_u8TunerInited[u8TunerIndex]==FALSE)
        {
            TUNER_MS_INIT_PARAM stParam;
            MS_U32 u32Cur_Broadcast_type = DEFAULT_DVB_TYPE;
            memset(&stParam, 0, sizeof(stParam));
            stParam.pCur_Broadcast_type = &u32Cur_Broadcast_type;
            if(MDrv_Tuner_SI2141_Init(u8TunerIndex, &stParam)==TRUE)
            {
                _u8TunerInited[u8TunerIndex]=TRUE;
            bRet = TRUE;
            }
        }
    }

    if(bRet==TRUE)
    {
            *(pu32channel_cnt) += 1;
    }

    return bRet;

}

MS_BOOL MDrv_Tuner_SI2141_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* u32IF_Freq)
{
    u8TunerIndex = u8TunerIndex;
    *u32IF_Freq = C_Si2141_44_24_IF_FREQUENCY;
    return TRUE;
}

MS_BOOL MDrv_Tuner_SI2141_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bResult = FALSE;

    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_LOOP_THROUGH:
            TUNER_ERR(("Not implement\n"));
            bResult = TRUE;
            break;

        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            bResult = Si2141_44_24_GetRSSILevel(data);
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
    }

    return bResult;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_SI2141) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_SI2141",          // demod system name
    .data               = TUNER_SI2141,            // private data value
    .Init               = MDrv_Tuner_SI2141_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_SI2141_SetTuner,
    .CheckExist         = MDrv_Tuner_SI2141_CheckExist,
    .GetTunerIF         = MDrv_Tuner_SI2141_GetTunerIF,
    .Extension_Function = MDrv_Tuner_SI2141_Extension_Function
};

#endif


