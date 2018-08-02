
code U8 MST_HSPRule_IP_Index_Main[PQ_HSPRule_IP_NUM_Main]=
{
    PQ_IP_HSP_Y_Main,
    PQ_IP_HSP_C_Main,
    PQ_IP_SRAM2_Main,
    PQ_IP_C_SRAM2_Main,
    PQ_IP_VE_HSD_SRAM_Main,
    PQ_IP_Peaking_Main,
};


code U8 MST_HSPRule_Array_Main[PQ_HSPRule_NUM_Main][PQ_HSPRule_IP_NUM_Main]=
{
    {//PreV_ScalingDown_Interlace, 0
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//PreV_ScalingDown_Progressive, 1
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_00x_YUV, 2
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc30Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_00x_RGB, 3
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc30Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_01x_YUV, 4
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc30Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_01x_RGB, 5
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc30Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_02x_YUV, 6
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc30Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_02x_RGB, 7
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc30Apass0001Astop40_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_03x_YUV, 8
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc35Apass0001Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_03x_RGB, 9
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc35Apass0001Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_04x_YUV, 10
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc45Apass0001Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_04x_RGB, 11
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc45Apass0001Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_05x_YUV, 12
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc57Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_05x_RGB, 13
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc57Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_06x_YUV, 14
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_06x_RGB, 15
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_07x_YUV, 16
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_07x_RGB, 17
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc67Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_08x_YUV, 18
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc94Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_08x_RGB, 19
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc94Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_09x_YUV, 20
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc94Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_09x_RGB, 21
        PQ_IP_HSP_Y_SRAM_2_6Tap_Main, PQ_IP_HSP_C_C_SRAM_2_Main, PQ_IP_SRAM2_InvSinc6Tc4p4Fc94Apass01Astop60_Main, PQ_IP_C_SRAM2_C121_Main, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_10x_YUV, 22
        PQ_IP_HSP_Y_Bypass_Main, PQ_IP_HSP_C_Bypass_Main, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_10x_RGB, 23
        PQ_IP_HSP_Y_Bypass_Main, PQ_IP_HSP_C_Bypass_Main, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_NULL, 
    },
    {//ScalingDown_15x_YUV, 24
        PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_Peaking_DeJaggy_Main, 
    },
    {//ScalingDown_15x_RGB, 25
        PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_Peaking_DeJaggy_Main, 
    },
    {//ScalingDown_20x_YUV, 26
        PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_Peaking_DeJaggy_Main, 
    },
    {//ScalingDown_20x_RGB, 27
        PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_Peaking_DeJaggy_Main, 
    },
    {//ScalingDown_40x_YUV, 28
        PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_Peaking_DeJaggy_Main, 
    },
    {//ScalingDown_40x_RGB, 29
        PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, PQ_IP_NULL, 
        PQ_IP_Peaking_DeJaggy_Main, 
    },
};
