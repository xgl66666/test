#include "platform_dibcom.h"
#include "dibcom8090.h"
#include "dibcom809x_api_2_layer_simple.h"



MS_U8 g_debugi2cdata = 1;

unsigned char dib_init(void)
{
    unsigned char u8Return = DIB_RETURN_SUCCESS;
    Out(printf("dibcom star\n"));
    dib8090_sip_register();
    dib8090_set_wbd_table_target();
    u8Return = dib8000_reset();
    u8Return = dib0090_reset();

    return u8Return;
}

void dib_tune(unsigned int frequency, unsigned char bandwidth)
{
    struct dibChannel ch;
    struct dibDemodMonitor mon;
	DibZeroMemory(&mon, sizeof(mon));

    //tune ....
    INIT_CHANNEL(&ch, STANDARD_ISDBT);

    ch.RF_kHz = frequency;
    ch.bandwidth_kHz     = 6000;
    //printf("-I- dibcom tune star: freq=%dkHz,Bandwidth=%dKHz\n", ch.RF_kHz, ch.bandwidth_kHz);
    Out(printf("-I- dibcom tune star: freq=%dkHz,Bandwidth=%dKHz\n", ch.RF_kHz, ch.bandwidth_kHz));
    if(ch.RF_kHz<=30000||ch.RF_kHz>=900000)
    {
        printf("-E- frontend input error\n");

        return;
    }
    frontend_tune_diversity_sync(1, &ch);
    DibMSleep(50);
#ifdef DIBCOM_EXTENDED_MONITORING
    // only for debug signal
    do{
        DibMSleep(500);
        demod_get_monitoring(&mon);
        dib7000_print_monitor(&mon, 0);
    }while(0);
#endif
}

unsigned char dib_lockstatus(void)
{
    return dib8000_get_signal_status();
}

unsigned char dib_signal_strength(void)
{
    MS_U16 v = dib8000_get_signal_strength();
	return (MS_U8)(100 - ((v * 100) / 65535));
}

unsigned char dib_signal_quality(void)
{
    /*
    MS_U8 quality = 0;
    if(1 == dib_lockstatus())
    {
        MS_U32 w = dib8000_get_signal_quality();
        printf("\n[w] = %d\n",w);
        if(w < 20000)
        {
            quality = ((20000 - w) * 35) / 20000;
        }
    }
        printf("\n[quality] = %d\n",quality);
    return quality;
    */
    struct dibDemodMonitor mon;
    if(1 == dib_lockstatus())
    {
        demod_get_monitoring(&mon);
    }
    else
    {
        mon.CoN = 0;
    }
    return mon.CoN;

}

