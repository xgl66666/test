#include <stdio.h>
#include "error.h"
#include "MsCommon.h"
#include "Board.h"
#include "apiDigiTuner.h"
#include "drvIIC.h"
#include "drvTuner_RDA5815M.h"
#include "drvTuner.h"
#include "drvTunerNull.h"

#if IF_THIS_TUNER_INUSE(TUNER_RDA5815M)

#define RDA5815M_SLA            0x18
#define RDA5815Sleep(x)         MsOS_DelayTask(x)
#define Xtal_27M                1
#define Xtal_30M                0
#define Xtal_24M                0




typedef int INT32;

static MS_BOOL RDA5815WriteReg(MS_U8 u8TunerIndex, MS_U8 paddr, MS_U8 pu8data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Value[2];
    HWI2C_PORT hwi2c_port;

    u8Value[0] = paddr;
    u8Value[1] = pu8data;

    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        bRet &= MDrv_IIC_Write(RDA5815M_SLA, 0, 0, u8Value, 2);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        bRet &= MDrv_IIC1_Write(RDA5815M_SLA, 0, 0, u8Value, 2);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    } 

    if (bRet == FALSE)
    {
        printf(" ERROR in RDA5815WriteReg !!! \n");
    }
    return bRet;
}



void RDA5815Initial(MS_U8 u8TunerIndex)
{
//RDA5815WriteReg(register address,register data);

    RDA5815Sleep(1);//Wait 1ms.
    // Chip register soft reset
    RDA5815WriteReg(u8TunerIndex,0x04, 0x04);
    RDA5815WriteReg(u8TunerIndex,0x04, 0x05);

    // Initial configuration start

    //pll setting
    RDA5815WriteReg(u8TunerIndex,0x1a, 0x13);
    RDA5815WriteReg(u8TunerIndex,0x41, 0x53);
    RDA5815WriteReg(u8TunerIndex,0x38, 0x9B);
    RDA5815WriteReg(u8TunerIndex,0x39, 0x15);
    RDA5815WriteReg(u8TunerIndex,0x3A, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x3B, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x3C, 0x0c);
    RDA5815WriteReg(u8TunerIndex,0x0c, 0xE2);
    RDA5815WriteReg(u8TunerIndex,0x2e, 0x6F);

#if (Xtal_27M == 1)
    RDA5815WriteReg(u8TunerIndex,0x72, 0x07); // v1.1, 1538~1539
    RDA5815WriteReg(u8TunerIndex,0x73, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x74, 0x71);
    RDA5815WriteReg(u8TunerIndex,0x75, 0x06); // v1.1, 1363~1364, 1862~1863
    RDA5815WriteReg(u8TunerIndex,0x76, 0x40);
    RDA5815WriteReg(u8TunerIndex,0x77, 0x89);
    RDA5815WriteReg(u8TunerIndex,0x79, 0x04); // v1.1, 900
    RDA5815WriteReg(u8TunerIndex,0x7A, 0x2A);
    RDA5815WriteReg(u8TunerIndex,0x7B, 0xAA);
    RDA5815WriteReg(u8TunerIndex,0x7C, 0xAB);
#endif
#if (Xtal_30M == 1)
    RDA5815WriteReg(u8TunerIndex,0x72, 0x06); // v1.2, 1544~1545
    RDA5815WriteReg(u8TunerIndex,0x73, 0x60);
    RDA5815WriteReg(u8TunerIndex,0x74, 0x66);
    RDA5815WriteReg(u8TunerIndex,0x75, 0x05); // v1.2, 1364~1365, 1859~1860
    RDA5815WriteReg(u8TunerIndex,0x76, 0xA0);
    RDA5815WriteReg(u8TunerIndex,0x77, 0x7B);
    RDA5815WriteReg(u8TunerIndex,0x79, 0x03); // v1.2, 901
    RDA5815WriteReg(u8TunerIndex,0x7A, 0xC0);
    RDA5815WriteReg(u8TunerIndex,0x7B, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x7C, 0x00);
#endif
#if (Xtal_24M == 1)
    RDA5815WriteReg(u8TunerIndex,0x72, 0x08); // v1.3, 1547~1548
    RDA5815WriteReg(u8TunerIndex,0x73, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x74, 0x80);
    RDA5815WriteReg(u8TunerIndex,0x75, 0x07); // v1.3, 1367~1368, 1859~1860
    RDA5815WriteReg(u8TunerIndex,0x76, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x77, 0x9A);
    RDA5815WriteReg(u8TunerIndex,0x79, 0x04); // v1.3, 901
    RDA5815WriteReg(u8TunerIndex,0x7A, 0xB0);
    RDA5815WriteReg(u8TunerIndex,0x7B, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x7C, 0x00);
#endif

    RDA5815WriteReg(u8TunerIndex,0x2f, 0x57);
    RDA5815WriteReg(u8TunerIndex,0x0d, 0x70);
    RDA5815WriteReg(u8TunerIndex,0x18, 0x4B);
    RDA5815WriteReg(u8TunerIndex,0x30, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0x5c, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0x65, 0x00);
    RDA5815WriteReg(u8TunerIndex,0x70, 0x3F);
    RDA5815WriteReg(u8TunerIndex,0x71, 0x3F);
    RDA5815WriteReg(u8TunerIndex,0x53, 0xA8);
    RDA5815WriteReg(u8TunerIndex,0x46, 0x21);
    RDA5815WriteReg(u8TunerIndex,0x47, 0x84);
    RDA5815WriteReg(u8TunerIndex,0x48, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x49, 0x08);
    RDA5815WriteReg(u8TunerIndex,0x60, 0x80);
    RDA5815WriteReg(u8TunerIndex,0x61, 0x80);
    RDA5815WriteReg(u8TunerIndex,0x6A, 0x08);
    RDA5815WriteReg(u8TunerIndex,0x6B, 0x63);
    RDA5815WriteReg(u8TunerIndex,0x69, 0xF8);
    RDA5815WriteReg(u8TunerIndex,0x57, 0x64);
    RDA5815WriteReg(u8TunerIndex,0x05, 0xaa);
    RDA5815WriteReg(u8TunerIndex,0x06, 0xaa);
    RDA5815WriteReg(u8TunerIndex,0x15, 0xAE);
    RDA5815WriteReg(u8TunerIndex,0x4a, 0x67);
    RDA5815WriteReg(u8TunerIndex,0x4b, 0x77);

    //agc setting

    RDA5815WriteReg(u8TunerIndex,0x4f, 0x40);
    RDA5815WriteReg(u8TunerIndex,0x5b, 0x20);

    RDA5815WriteReg(u8TunerIndex,0x16, 0x0C);
    RDA5815WriteReg(u8TunerIndex,0x18, 0x0C);
    RDA5815WriteReg(u8TunerIndex,0x30, 0x1C);
    RDA5815WriteReg(u8TunerIndex,0x5c, 0x2C);
    RDA5815WriteReg(u8TunerIndex,0x6c, 0x3C);
    RDA5815WriteReg(u8TunerIndex,0x6e, 0x3C);
    RDA5815WriteReg(u8TunerIndex,0x1b, 0x7C);
    RDA5815WriteReg(u8TunerIndex,0x1d, 0xBD);
    RDA5815WriteReg(u8TunerIndex,0x1f, 0xBD);
    RDA5815WriteReg(u8TunerIndex,0x21, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x23, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x25, 0xFE);
    RDA5815WriteReg(u8TunerIndex,0x27, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0x29, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0xb3, 0xFF);
    RDA5815WriteReg(u8TunerIndex,0xb5, 0xFF);

    RDA5815WriteReg(u8TunerIndex,0x17, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x19, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x31, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x5d, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x6d, 0xF0);
    RDA5815WriteReg(u8TunerIndex,0x6f, 0xF1);
    RDA5815WriteReg(u8TunerIndex,0x1c, 0xF5);
    RDA5815WriteReg(u8TunerIndex,0x1e, 0x35);
    RDA5815WriteReg(u8TunerIndex,0x20, 0x79);
    RDA5815WriteReg(u8TunerIndex,0x22, 0x9D);
    RDA5815WriteReg(u8TunerIndex,0x24, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x26, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x28, 0xBE);
    RDA5815WriteReg(u8TunerIndex,0x2a, 0xCF);
    RDA5815WriteReg(u8TunerIndex,0xb4, 0xDF);
    RDA5815WriteReg(u8TunerIndex,0xb6, 0x0F);

    RDA5815WriteReg(u8TunerIndex,0xb7, 0x15); //start
    RDA5815WriteReg(u8TunerIndex,0xb9, 0x6c);
    RDA5815WriteReg(u8TunerIndex,0xbb, 0x63);
    RDA5815WriteReg(u8TunerIndex,0xbd, 0x5a);
    RDA5815WriteReg(u8TunerIndex,0xbf, 0x5a);
    RDA5815WriteReg(u8TunerIndex,0xc1, 0x55);
    RDA5815WriteReg(u8TunerIndex,0xc3, 0x55);
    RDA5815WriteReg(u8TunerIndex,0xc5, 0x47);
    RDA5815WriteReg(u8TunerIndex,0xa3, 0x53);
    RDA5815WriteReg(u8TunerIndex,0xa5, 0x4f);
    RDA5815WriteReg(u8TunerIndex,0xa7, 0x4e);
    RDA5815WriteReg(u8TunerIndex,0xa9, 0x4e);
    RDA5815WriteReg(u8TunerIndex,0xab, 0x54);
    RDA5815WriteReg(u8TunerIndex,0xad, 0x31);
    RDA5815WriteReg(u8TunerIndex,0xaf, 0x43);
    RDA5815WriteReg(u8TunerIndex,0xb1, 0x9f);

    RDA5815WriteReg(u8TunerIndex,0xb8, 0x6c); //end
    RDA5815WriteReg(u8TunerIndex,0xba, 0x92);
    RDA5815WriteReg(u8TunerIndex,0xbc, 0x8a);
    RDA5815WriteReg(u8TunerIndex,0xbe, 0x8a);
    RDA5815WriteReg(u8TunerIndex,0xc0, 0x82);
    RDA5815WriteReg(u8TunerIndex,0xc2, 0x93);
    RDA5815WriteReg(u8TunerIndex,0xc4, 0x85);
    RDA5815WriteReg(u8TunerIndex,0xc6, 0x77);
    RDA5815WriteReg(u8TunerIndex,0xa4, 0x82);
    RDA5815WriteReg(u8TunerIndex,0xa6, 0x7e);
    RDA5815WriteReg(u8TunerIndex,0xa8, 0x7d);
    RDA5815WriteReg(u8TunerIndex,0xaa, 0x6f);
    RDA5815WriteReg(u8TunerIndex,0xac, 0x65);
    RDA5815WriteReg(u8TunerIndex,0xae, 0x43);
    RDA5815WriteReg(u8TunerIndex,0xb0, 0x9f);
    RDA5815WriteReg(u8TunerIndex,0xb2, 0xf0);

    RDA5815WriteReg(u8TunerIndex,0x81, 0x92); //rise
    RDA5815WriteReg(u8TunerIndex,0x82, 0xb4);
    RDA5815WriteReg(u8TunerIndex,0x83, 0xb3);
    RDA5815WriteReg(u8TunerIndex,0x84, 0xac);
    RDA5815WriteReg(u8TunerIndex,0x85, 0xba);
    RDA5815WriteReg(u8TunerIndex,0x86, 0xbc);
    RDA5815WriteReg(u8TunerIndex,0x87, 0xaf);
    RDA5815WriteReg(u8TunerIndex,0x88, 0xa2);
    RDA5815WriteReg(u8TunerIndex,0x89, 0xac);
    RDA5815WriteReg(u8TunerIndex,0x8a, 0xa9);
    RDA5815WriteReg(u8TunerIndex,0x8b, 0x9b);
    RDA5815WriteReg(u8TunerIndex,0x8c, 0x7d);
    RDA5815WriteReg(u8TunerIndex,0x8d, 0x74);
    RDA5815WriteReg(u8TunerIndex,0x8e, 0x9f);
    RDA5815WriteReg(u8TunerIndex,0x8f, 0xf0);

    RDA5815WriteReg(u8TunerIndex,0x90, 0x15); //fall
    RDA5815WriteReg(u8TunerIndex,0x91, 0x39);
    RDA5815WriteReg(u8TunerIndex,0x92, 0x30);
    RDA5815WriteReg(u8TunerIndex,0x93, 0x27);
    RDA5815WriteReg(u8TunerIndex,0x94, 0x29);
    RDA5815WriteReg(u8TunerIndex,0x95, 0x0d);
    RDA5815WriteReg(u8TunerIndex,0x96, 0x10);
    RDA5815WriteReg(u8TunerIndex,0x97, 0x1e);
    RDA5815WriteReg(u8TunerIndex,0x98, 0x1a);
    RDA5815WriteReg(u8TunerIndex,0x99, 0x19);
    RDA5815WriteReg(u8TunerIndex,0x9a, 0x19);
    RDA5815WriteReg(u8TunerIndex,0x9b, 0x32);
    RDA5815WriteReg(u8TunerIndex,0x9c, 0x1f);
    RDA5815WriteReg(u8TunerIndex,0x9d, 0x31);
    RDA5815WriteReg(u8TunerIndex,0x9e, 0x43);

    RDA5815Sleep(10);//Wait 10ms;

    // Initial configuration end
}

/********************************************************************************/
//  Function to Set the RDA5815
//  fPLL:       Frequency           unit: MHz from 250 to 2300
//  fSym:   SymbolRate          unit: KSps from 1000 to 60000
/********************************************************************************/

INT32 RDA5815Set(MS_U8 u8TunerIndex, unsigned long fPLL, unsigned long fSym)
{
    unsigned char buffer;
    unsigned long temp_value = 0;
    unsigned long bw;/*,temp_value1 = 0,temp_value2=0 ;*/
    unsigned char Filter_bw_control_bit;

    RDA5815WriteReg(u8TunerIndex,0x04, 0xc1); //add by rda 2011.8.9,RXON = 0 , change normal working state to idle state
    RDA5815WriteReg(u8TunerIndex,0x2b, 0x95); //clk_interface_27m=0  add by rda 2012.1.12

    //set frequency start
#if (Xtal_27M == 1)     // v1.1
    temp_value = (unsigned long)fPLL * 77672; //((2^21) / RDA5815_XTALFREQ);
#endif
#if (Xtal_30M == 1)     // v1.2
    temp_value = (unsigned long)fPLL * 69905; //((2^21) / RDA5815_XTALFREQ);
#endif
#if (Xtal_24M == 1)     // v1.3
    temp_value = (unsigned long)fPLL * 87381; //((2^21) / RDA5815_XTALFREQ);
#endif

    buffer = ((unsigned char)((temp_value >> 24) & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x07, buffer);
    buffer = ((unsigned char)((temp_value >> 16) & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x08, buffer);
    buffer = ((unsigned char)((temp_value >> 8) & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x09, buffer);
    buffer = ((unsigned char)(temp_value & 0xff));
    RDA5815WriteReg(u8TunerIndex,0x0a, buffer);
    //set frequency end

    // set Filter bandwidth start
    bw = fSym;      //kHz

    Filter_bw_control_bit = (unsigned char)((bw * 135 / 200 + 4000) / 1000);

    if (Filter_bw_control_bit < 4)
        Filter_bw_control_bit = 4;    // MHz
    else if (Filter_bw_control_bit > 40)
        Filter_bw_control_bit = 40;   // MHz

    Filter_bw_control_bit &= 0x3f;
    Filter_bw_control_bit |= 0x40;      //v1.5

    RDA5815WriteReg(u8TunerIndex,0x0b, Filter_bw_control_bit);
    // set Filter bandwidth end

    RDA5815WriteReg(u8TunerIndex,0x04, 0xc3);    //add by rda 2011.8.9,RXON = 0 ,rxon=1,normal working
    RDA5815WriteReg(u8TunerIndex,0x2b, 0x97);    //clk_interface_27m=1  add by rda 2012.1.12
    RDA5815Sleep(5);//Wait 5ms;

    return 1;
}


/********************************************************************************/
// Function: InitTuner
// Description: initialize tuner setting
// Parameter: void *    pData

/********************************************************************************/

MS_BOOL  MDrv_Tuner_Initial(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    printf("RDA5815M v1.5 Tuner Init\n");
    RDA5815Initial(u8TunerIndex);

    return 1;
}


MS_BOOL MDrv_Tuner_SetFreq_S2(MS_U8 u8TunerIndex, MS_U32 dwFreq, MS_U32 ucBw)
{
//    MS_U32 u32BW;
//    printf("RDA5815 Freq:%lu,Bandwidth:%u\n",dwFreq,ucBw);
    //u32BW = (6+ucBw)*1000;
    RDA5815Set(u8TunerIndex, dwFreq, ucBw);

    return 1;
}

#define RDA5815M_CHIP_ID0 0x58
#define RDA5815M_CHIP_ID1 0xf8
MS_BOOL MDrv_Tuner_RDA5815M_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 u8data[2] = {0};
    MS_U8 u8addr = 0;
    HWI2C_PORT hwi2c_port;
    
    hwi2c_port = getI2CPort(u8TunerIndex);
    if (hwi2c_port < E_HWI2C_PORT_1)
    {
        MDrv_IIC_Read(RDA5815M_SLA, &u8addr, 1, u8data, 2);
    }
    else if (hwi2c_port < E_HWI2C_PORT_2)
    {
        MDrv_IIC1_Read(RDA5815M_SLA, &u8addr, 1, u8data, 2);
    }
    else
    {
        TUNER_ERR(("hwi2c_port number exceeds limitation\n"));
        return FALSE;
    } 


    if((u8data[1] != RDA5815M_CHIP_ID1) || (u8data[0] != RDA5815M_CHIP_ID0))
        return FALSE;
    
    if(NULL != pu32channel_cnt)
        *(pu32channel_cnt) += 1;
    
    return TRUE;
}


MS_BOOL RDA5815M_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    return TRUE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_RDA5815M) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_RDA5815M",          // demod system name
    .data               = TUNER_RDA5815M,            // private data value
    .Init               = MDrv_Tuner_Initial,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_RDA5815M_CheckExist,
    .Extension_Function = RDA5815M_Extension_Function,
};



#endif


