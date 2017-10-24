#include <common.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvIR.h>
#include <sys/drv/drvSAR.h>
#include <IR_MSTAR_DTV.h>

/// MS Return Error
#ifndef MSRET_ERROR
#define MSRET_ERROR     0
#endif
/// MS Return OK
#ifndef MSRET_OK
#define MSRET_OK        1
#endif

#define _PA2VA(x) (MS_U32)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_U32)MsOS_VA2PA((x))

BOOLEAN msAPI_IR_Initialize(void)
{
    static IR_RegCfg tIRRegCfg =
    {
        .u8Ctrl0        = IR_LEADER_CODE_CHECKING_OPTION,
        .u8Ctrl1        = 0x01,
        .u8Clk_mhz      = MST_XTAL_CLOCK_MHZ,
        .u8HdrCode0     = IR_HEADER_CODE0,
        .u8HdrCode1     = IR_HEADER_CODE1,
        .u8CCodeBytes   = 2,
        .u8CodeBits     = 32,  //(32bits==4bytes)
    };

    static IR_TimeBnd tIRTimeBnd =
    {
        .tHdr={
            .s16Time = IR_HEADER_CODE_TIME,
            .s8UpBnd = IR_HEADER_CODE_TIME_UB,
            .s8LoBnd = IR_HEADER_CODE_TIME_LB},
        .tOff={
            .s16Time = IR_OFF_CODE_TIME,
            .s8UpBnd = IR_OFF_CODE_TIME_UB,
            .s8LoBnd = IR_OFF_CODE_TIME_LB},
        .tOffRpt={
            .s16Time = IR_OFF_CODE_RP_TIME,
            .s8UpBnd = IR_OFF_CODE_RP_TIME_UB,
            .s8LoBnd = IR_OFF_CODE_RP_TIME_LB},
        .tLg01Hg={
            .s16Time = IR_LOGI_01H_TIME,
            .s8UpBnd = IR_LOGI_01H_TIME_UB,
            .s8LoBnd = IR_LOGI_01H_TIME_LB},
        .tLg0={
            .s16Time = IR_LOGI_0_TIME,
            .s8UpBnd = IR_LOGI_0_TIME_UB,
            .s8LoBnd = IR_LOGI_0_TIME_LB},
        .tLg1={
            .s16Time = IR_LOGI_1_TIME,
            .s8UpBnd = IR_LOGI_1_TIME_UB,
            .s8LoBnd = IR_LOGI_1_TIME_LB}
    };

    IR_RegCfg* pirRegCfg=&tIRRegCfg;
    IR_TimeBnd* pirTimeBnd=&tIRTimeBnd;

    //(0) For debugging
    //printf("MailBox: MDrv_IR_SetDbgLevel\n");
    //MDrv_IR_SetDbgLevel(E_IR_DBGLVL_INFO);

    //(1) Initialization
    MsOS_Dcache_Flush((MS_U32)pirRegCfg, sizeof(IR_RegCfg));
    if(MDrv_IR_Init((IR_RegCfg*)_VA2PA((MS_U32)pirRegCfg))==E_IR_FAIL)
    {
        printf("MDrv_IR_Init fails...\n");
        return MSRET_ERROR;
    }

    //(2) Configure Time Bounds
    MsOS_Dcache_Flush((MS_U32)pirTimeBnd, sizeof(IR_TimeBnd));
    if(MDrv_IR_Config(pirTimeBnd)==E_IR_FAIL)
    {
        printf("MDrv_IR_Config fails...\n");
        return MSRET_ERROR;
    }

    return MSRET_OK;
}

BOOLEAN msAPI_KeyPad_Initialize(void)
{
    static SAR_RegCfg sarRegCfgCh;
    SAR_RegCfg* psarRegCfgCh=&sarRegCfgCh;

    MS_U8 u8ChEnb[KEYPAD_CHANNEL_SUPPORT]={ENABLE_KPDCHAN_1,ENABLE_KPDCHAN_2,ENABLE_KPDCHAN_3,ENABLE_KPDCHAN_4};
    MS_U8 u8UpBnd[KEYPAD_CHANNEL_SUPPORT]={KEYPAD_CH1_UB,KEYPAD_CH2_UB,KEYPAD_CH3_UB,KEYPAD_CH4_UB};
    MS_U8 u8LoBnd[KEYPAD_CHANNEL_SUPPORT]={KEYPAD_CH1_LB,KEYPAD_CH2_LB,KEYPAD_CH3_LB,KEYPAD_CH4_LB};
    MS_U8 u8ChKey[ADC_KEY_CHANNEL_NUM][4]={
        {ADC_KEY_1_L0_FLAG,ADC_KEY_1_L1_FLAG,ADC_KEY_1_L2_FLAG,ADC_KEY_1_L3_FLAG}, //mapping keys
        {ADC_KEY_2_L0_FLAG,ADC_KEY_2_L1_FLAG,ADC_KEY_2_L2_FLAG,ADC_KEY_2_L3_FLAG} //mapping keys
    };
    MS_U8 k,u8Idx;

    if(MDrv_SAR_Init()==E_SAR_FAIL)
    {
        printf("MDrv_SAR_Init fails\n");
        return MSRET_ERROR;
    }

    for(u8Idx=0,k=0;k<KEYPAD_CHANNEL_SUPPORT;k++)
    {
        if(u8ChEnb[k]==ENABLE)
        {
            psarRegCfgCh->u8SARChID = k;
            psarRegCfgCh->u8UpBnd = u8UpBnd[k];
            psarRegCfgCh->u8LoBnd = u8LoBnd[k];
            psarRegCfgCh->u8KeyLevelNum = ADC_KEY_LEVEL;
            psarRegCfgCh->u8KeyThreshold[0] = ADC_KEY_L0;
            psarRegCfgCh->u8KeyThreshold[1] = ADC_KEY_L1;
            psarRegCfgCh->u8KeyThreshold[2] = ADC_KEY_L2;
            psarRegCfgCh->u8KeyThreshold[3] = ADC_KEY_L3;
            psarRegCfgCh->u8KeyCode[0] = u8ChKey[u8Idx][0];
            psarRegCfgCh->u8KeyCode[1] = u8ChKey[u8Idx][1];
            psarRegCfgCh->u8KeyCode[2] = u8ChKey[u8Idx][2];
            psarRegCfgCh->u8KeyCode[3] = u8ChKey[u8Idx][3];

            // configure keypad channel
            //MsOS_Dcache_Flush((MS_U32)psarRegCfgCh, sizeof(SAR_RegCfg));
            //MsOS_FlushMemory();
            flush_cache((MS_U32)psarRegCfgCh, sizeof(SAR_RegCfg));
            Chip_Flush_Memory();

            if(MDrv_SAR_Config(psarRegCfgCh)==E_SAR_FAIL)
            {
                printf("MDrv_SAR_Config: CH_%d fails\n",u8Idx);
                return MSRET_ERROR;
            }

            if((++u8Idx)>=ADC_KEY_CHANNEL_NUM)
                break;
        }
    }

    if(MDrv_SAR_Enable(TRUE)==E_SAR_FAIL)
    {
        printf("MDrv_SAR_Enable fails\n");
        return MSRET_ERROR;
    }
    return MSRET_OK;
}