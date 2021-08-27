/***************************************************************************** 
Copyright 2011, AltoBeam Inc. All rights reserved.
     
File Name: atbmdvbc.c
   
Version: 1.4
Released 2011-5-29
******************************************************************************/

#include <frontend_config.h>

#if (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x)

#include "../../../../include/prana2/stdio.h"
//#include <math.h>
#include "atbm885x.h"
#include <sys/common/MsCommon.h>

#define ATBM_DEBUG_DVBC   0      /*default no debug output*/

#define CMS0022_COARSE_CARRIER_ACQ_SWEEP_STEP    (0.05) 
static double CMS0022_MAX_SYMB_RATE_RATIO = (1.0/4);         
static double CMS0022_CIC2_DELAY         = (3.0);
static double CMS0022_CIC3_DELAY         = (0.0);

#define DVBC_SAMPLE_RATE_ADDR            0x210
#define DVBC_SAMPLE_RATE_RECIP_ADDR      0x214
#define DVBC_OUTPUT_SHIFT_ADDR           0x128
#define DVBC_DECIMATION_FACTOR_ADDR      0x124
#define DVBC_SLOW_CONTROL_TC_ADDR        0x3BC
#define DVBC_CARRIER_LOCK_ACQUIRE_TIMEOUT_ADDR 0x348
#define DVBC_PL_CARRIER_FREQUENCY_RANGE_ADDR   0x38C
#define DVBC_PL_CARRIER_STEP_FREQUENCY_ADDR    0x388
#define DVBC_COARSE_FREQUENCY_OFFSET_ADDR  0x118
#define DVBC_SEARCH_STEP_ADDR              0x3B0 
#define DVBC_SEARCH_RANGE_ADDR             0x3B4  
#define DVBC_BITSYNC_DETECT_TIMEOUT_ADDR   0x364 
#define DVBC_AUTO_EQU_SEARCH_ADDR          0x3CC
extern u_int8  ATBMSlaveAddr;
static double dvbc_osc_freq;
static double dvbc_symbol_rate;

static double log10(double flt_x)
{
 MS_U32      u32_temp = 1;
 MS_U8       indx = 0;

 do {
     u32_temp = u32_temp << 1;
     if (flt_x < (double)u32_temp)
         break;
 }while (++indx < 32);

 // 10*log10(X) ~= 0.3*N, when X ~= 2^N
 return (double)0.3 * indx;
}
static double pow( double d, int n)
{
    int s;
    double r = 1;

    if( n == 0 )
        return 1;

    if( n < 0 )
        s = 1 , n = -n;
    else
        s = 0;

    while( n != 1 )
    {
        if( n&1 ) r*=d;
        n /= 2;
        d *= d;
    }

    r *= d;
    return s?1/r:r;
}

static float  fabs(float x)
{
    float  s;

    if (x < 0)
        s = -x;
    else
        s = x;

    return s;
}
static int floor(double n)
{
    return (int)fabs(n);
}



/****************************************************************************
Function:		ATBM_DVBC_Init
Parameters:   custom_config              
Return: 		None

Description:
	Call this function to configure all DVBC related registers.		 
******************************************************************************/
void ATBM_DVBC_Init(struct custom_config_t custom_config)
{

	u_int32 RifControl;
	double OSC_Freq = 0;
	double IF_Freq = 0;
	double	dSymbolRateM;
	int	iItbCoarseFrequencyOffset;	

	ATBM_DVBC_Write(0x04, 0x1);  //DVBC Module Reset

	RifControl = 0x01; //IF mode
	ATBM_DVBC_Write(0x100, RifControl); 
	ATBM_DVBC_Write(0x104,0x34);  //gain control 

	dSymbolRateM = (double)(custom_config.dvbc_params.symbol_rate*1.0)/1000; 
	dvbc_symbol_rate = dSymbolRateM;
	OSC_Freq = custom_config.sample_clk_Frequency;		
	dvbc_osc_freq = OSC_Freq;	

	IF_Freq = custom_config.tuner_config.IF_Frequency;	
	iItbCoarseFrequencyOffset = (int)(134217728*(OSC_Freq - IF_Freq)/OSC_Freq);
	ATBM_DVBC_Write(DVBC_COARSE_FREQUENCY_OFFSET_ADDR,iItbCoarseFrequencyOffset);

	ATBM_DVBC_SetCarrier(OSC_Freq,	dSymbolRateM);	 	
	ATBM_DVBC_SetSymbolRate(OSC_Freq, dSymbolRateM);		 
	ATBM_DVBC_SetQAM();	 

	ATBM_DVBC_Write(0x20c,0x10); 
	ATBM_DVBC_Write(0x204,0x400); //DVB-C symbol rate offset related	
	ATBM_DVBC_Write(0x04, 0x00);  //DVBC Module start processing
}

/****************************************************************************
Function:		ATBM_DVBC_SetSymbolRate
Parameters:   OSC_Freq: OSC sampling frequency, 30.4, in MHz   
                     dSymbolRateM, symbol rate, in Msbps
Return: 		None

Description:
	Call this function to configure all DVB-C  symbol rate related registers.			 
******************************************************************************/
void ATBM_DVBC_SetSymbolRate(double OSC_Freq, double dSymbolRateM)
{
	unsigned long   uLog2DecFactor;		
	double		    dSmpsPerSymb;		
	double		    dCicDcGain;			
	unsigned long	uDecCicOutputShift; 
	unsigned long	uAfcSlowControlTC;	

	unsigned long	uTrlSampleRate;
	unsigned long	uTrlSampleRateRecip;

	// Calculate factor by which decimation filter must reduce rate

	uLog2DecFactor = (unsigned long)(floor(log10(((CMS0022_MAX_SYMB_RATE_RATIO*OSC_Freq)/dSymbolRateM))/log10(2.0))); 

	// Samples per symbol
	if(uLog2DecFactor > 3)
	{
		uLog2DecFactor=3;
	}

	dSmpsPerSymb = OSC_Freq/dSymbolRateM;

	uTrlSampleRate = (unsigned long)((1<<16)*2/(dSmpsPerSymb/(1<<uLog2DecFactor))+0.5);
	ATBM_DVBC_Write(DVBC_SAMPLE_RATE_ADDR, uTrlSampleRate);

	uTrlSampleRateRecip = (int)(((1<<11)*(dSmpsPerSymb/(1<<uLog2DecFactor))/2)+0.5);
	ATBM_DVBC_Write(DVBC_SAMPLE_RATE_RECIP_ADDR, uTrlSampleRateRecip);
	// Calculate CIC gain
	dCicDcGain  = pow(2.0*(1<<(uLog2DecFactor-1)),(int)CMS0022_CIC2_DELAY);
	//Calculate CIC gain *=
	dCicDcGain *= pow(3.0*(1<<(uLog2DecFactor-1)),(int)CMS0022_CIC3_DELAY);

	uDecCicOutputShift = (uLog2DecFactor > 0) ? ((unsigned)((log10(dCicDcGain)/log10(2.0))+0.5)+1) : 1;

	ATBM_DVBC_Write(DVBC_OUTPUT_SHIFT_ADDR, uDecCicOutputShift);
	ATBM_DVBC_Write(DVBC_DECIMATION_FACTOR_ADDR,uLog2DecFactor);

	// Program AFC tracking time constant as function of symbol rate
	uAfcSlowControlTC = (1<<(uLog2DecFactor+2));
	ATBM_DVBC_Write(DVBC_SLOW_CONTROL_TC_ADDR,uAfcSlowControlTC);

}

/****************************************************************************
Function:	ATBM_DVBC_SetCarrier
Parameters:   OSC_Freq: OSC sampling frequency, 30.4, in MHz   
                     dSymbolRateM, symbol rate. 
Return: 	None

Description:
	Call this function to configure DVBC registers according to OSC frequency  
	and symbol rate.
******************************************************************************/
void   ATBM_DVBC_SetCarrier(	double	OSC_Freq, double dSymbolRateM)
{

	double  dIfSearchStartFoOvrFsamp;   

	double  dIfSearchStepFstpOvrFsamp;  


	int			    iIfSrchStartFreq;
	int			    iIfSrchStepFreq;

	unsigned long	uTimSearchLockTimeout;
	unsigned long	uPlCarrierFrequencyRange;
	unsigned long	uCarrierLockAcquireTimeout;

	// Calculate parameters for coarse stepped carrier search

	double dWinEndFreqMHz = 0.15*dSymbolRateM;

	dIfSearchStartFoOvrFsamp = dWinEndFreqMHz/OSC_Freq;

	if(dWinEndFreqMHz != 0.0)
	{
		dIfSearchStepFstpOvrFsamp = CMS0022_COARSE_CARRIER_ACQ_SWEEP_STEP*
		dSymbolRateM/OSC_Freq;
	}
	else
	{
		dIfSearchStepFstpOvrFsamp = 0;

	}

	iIfSrchStartFreq = (int)(((dIfSearchStartFoOvrFsamp)*(1<<27))+0.5); 

	ATBM_DVBC_Write(DVBC_SEARCH_RANGE_ADDR,iIfSrchStartFreq);

	iIfSrchStepFreq = (int)(((dIfSearchStepFstpOvrFsamp)*(1<<27))+0.5);
	ATBM_DVBC_Write(DVBC_SEARCH_STEP_ADDR,iIfSrchStepFreq);
	
	// Program parameters for fine stepped carrier search
	// Step size
	uTimSearchLockTimeout = 20;	// 20 for 1% step;
	ATBM_DVBC_Write(DVBC_PL_CARRIER_STEP_FREQUENCY_ADDR,uTimSearchLockTimeout);

	// Search range
	uPlCarrierFrequencyRange = 39;// 39 for +/-15% limits
	ATBM_DVBC_Write(DVBC_PL_CARRIER_FREQUENCY_RANGE_ADDR,uPlCarrierFrequencyRange);


	// Standard times and thresholds for all other QAM levels
	uCarrierLockAcquireTimeout = 4;   	
	ATBM_DVBC_Write(DVBC_CARRIER_LOCK_ACQUIRE_TIMEOUT_ADDR, uCarrierLockAcquireTimeout);


}


/****************************************************************************
Function:	ATBM_DVBC_SetQAM
Parameters:   none                
Return: 	None

Description:
    Call this function to configure DVBC QAM related registers:			 
******************************************************************************/
void ATBM_DVBC_SetQAM()
{

	u_int32	uBitSyncDetectTimeout;     

	ATBM_DVBC_Write(DVBC_AUTO_EQU_SEARCH_ADDR,0x3f8); //Atuo QAM detect

	uBitSyncDetectTimeout = 60000/(256*16); 

	ATBM_DVBC_Write(DVBC_BITSYNC_DETECT_TIMEOUT_ADDR,uBitSyncDetectTimeout);

}

/*****************************************************************************
Function:    ATBM_DVBC_SignalStrength
Parameters:  none
Return:      XgcGain

Description:
    This function checks DVB-C external IF gain value used by the XGC. The function 
    can be used for DVB-C signal strength evaluation.
    Different Tuner have different XgcGain Range. It is needed to test XgcGain value
    on different signal strength level to make a Tuner's private signal strength evaluation function.
    Refer to ATBM_DVBC_SignalStrength_TDAC2_C02A().
*****************************************************************************/
u_int16 ATBM_DVBC_SignalStrength()
{
	u_int16 XgcGain;
	u_int32 XgcGain32; 

	XgcGain32 = ATBM_DVBC_Read(0x114);	
	XgcGain = (u_int16)(XgcGain32&0xfff);	

	printf(" XgcGain = 0x%x\n", XgcGain);
	return XgcGain;
}

/*****************************************************************************
Function:    ATBM_DVBC_SignalStrength_TDAC2_C02A
Parameters:  none
Return:      signal_percent

Description:
    This function checks DVB-C external IF gain value used by the XGC. 
    The function can be used for ALPS TDAC2-C02A Tuner DVB-C signal strength evaluation.  
    Range:  20--90. This is for reference only. For other Tuner the AGC range may be changed.
    Test the result of 0x114 register value(TDAC2-C02A) and give the reference evaluation:
    256QAM, symbol rate 6.875M
*****************************************************************************/
u_int16 ATBM_DVBC_SignalStrength_TDAC2_C02A()
{
	u_int32 XgcGain; 
	u_int16 signal_percent;

	XgcGain = ATBM_DVBC_Read(0x114);	
	XgcGain = XgcGain&0xfff;	
	if(XgcGain > 0x500)
	{	
		signal_percent = 20 + ((XgcGain - 0x500)*70)/(0xa53-0x500);  
	}else
	{
		signal_percent = 20; 
	}        
	if(signal_percent>90)
	{
		signal_percent = 90;
	}
	printf(" XgcGain = 0x%x, signal_percent = %d\n", XgcGain, signal_percent);
	return signal_percent;
}

/****************************************************************************
Function:   ATBM_DVBC_SNR
Parameters:   None                    
Return:    DVBC signal-to-noise ratio in dB format

Description:
    Calculate  DVB-C SNR:			 
******************************************************************************/
double   ATBM_DVBC_SNR()
{
	double dvbc_snr;
	u_int32 dtmp;
	u_int32 tmp; 				

	dvbc_snr = 0;
	dtmp = ATBM_DVBC_Read(0x3C4);
	tmp = ATBM_DVBC_Read(0x3d0);

	dtmp = dtmp&0xffff;
	tmp=(tmp&0x7)+1; //pow(2,tmp) means QAM		
    
	if(dtmp ==0)
	{    	
		dtmp = 1;
	}

	if((tmp==4) ||(tmp==6) ||(tmp==8))
	{
		if(dtmp!=0)
		{
			dvbc_snr = -20*log10(dtmp/65536.0)-0.8;//QAM=16,64,256
		}
	}
	else if( (tmp==5) ||(tmp==7))
	{
		if(dtmp !=0)
		{
			dvbc_snr = -20*log10(dtmp/65536.0)-4.0;//QAM=32,128
		}
	}
	else
	{
		dvbc_snr=0;

	}	
	//  printf("DVBC SNR = %f dB\n",dvbc_snr);	
	return dvbc_snr;

}

/****************************************************************************
Function:   ATBM_DVBC_BER
Parameters:   BerExponent: scientific notation exponent                    
Return:   BerCoeffient: RS corrected Bit error rate real number.

Description:
	Calculate DVB-C RS bit error rate. Measure window is 0x8000 MPEG packets. 
	Here we read number of bit errors in the last measure window.

Note: measure window length is configurable, but default to 0x8000 packets.   
      0x8000 packets takes about 960ms in 256QAM(6.875Msbps)
******************************************************************************/
double   ATBM_DVBC_BER(u_int32 *BerExponent)
{
	double BerCoeffient =0;
	u_int32 temp;
	u_int32 TotalBits = 53477376; //SampleSize: 32768*204*8
	u_int32 e_para = 0;
	u_int32 i;	
	unsigned char data[2];
	u_int32 CorrectedBerBits = 0;	

	data[1] = ATBMRead(0x15, 0x23);
	data[0] = ATBMRead(0x15, 0x22);
	CorrectedBerBits = data[1]*256 + data[0];
	data[1] = ATBMRead(0x15, 0x21);
	data[0] = ATBMRead(0x15, 0x20);
	TotalBits = (data[1]*256 + data[0])*204*8;		
	if(CorrectedBerBits ==0)
	{
		BerCoeffient = 0;
		*BerExponent = 0;			
		printf("PreRsBer = %f E- %d\n",BerCoeffient, *BerExponent);			
		return BerCoeffient;
	}	
	for (i=0;i<12;i++)
	{
		temp = CorrectedBerBits/TotalBits;
		if (temp)
		{
			BerCoeffient = (CorrectedBerBits*1.0)/TotalBits;
			break;
		}
		else
		{
			e_para +=1;
			CorrectedBerBits *=10;
		}
	}	
	*BerExponent  = e_para;
	printf("PreRsBer = %f E- %d\n",BerCoeffient, *BerExponent);		
	return BerCoeffient;

}

/****************************************************************************
Function:   ATBM_DVBC_UncorrectablePER
Parameters: PktsExponent: scientific notation exponent                   
Return:   PktsCoeffient: error packets rate real number.

Description:
	This function counts DVB-C data packet which contains uncorrected error and calculate packet error rates.
	Measure window is 0x8000 MPEG packets. 
	Here we read the counted number of uncorrected packets in the last measure window.
	
Note: measure window length is configurable, but default to 0x8000 packets.   
      0x8000 packets takes about 960ms in 256QAM(6.875Msbps)
******************************************************************************/
double   ATBM_DVBC_UncorrectablePER(u_int32 *PktsExponent )
{
	double PktsCoeffient = 0;
	u_int32 temp;
	u_int32 TotalPkts = 32768; //SampleSize: 0x8000 packets
	u_int32 e_para = 0;
	u_int32 i;
	unsigned char data[2];
	u_int32 UnCorrectedPackets = 0;	

	data[1] = ATBMRead(0x15, 0x27);
	data[0] = ATBMRead(0x15, 0x26);
	UnCorrectedPackets = data[1]*256 + data[0];
	data[1] = ATBMRead(0x15, 0x21);
	data[0] = ATBMRead(0x15, 0x20);
	TotalPkts = data[1]*256 + data[0];		

	if(UnCorrectedPackets ==0)
	{
		PktsCoeffient = 0;
		*PktsExponent = 0;			
		printf("Pks Error = %f E- %d\n",PktsCoeffient, *PktsExponent);			
		return PktsCoeffient;
	}	
	for (i=0;i<12;i++)
	{
		temp = UnCorrectedPackets/TotalPkts;
		if (temp)
		{
			PktsCoeffient = (UnCorrectedPackets*1.0)/TotalPkts;
			break;
		}
		else
		{
			e_para +=1;
			UnCorrectedPackets *=10;
		}
	}	
	*PktsExponent  = e_para;
	printf("Pks Error = %f E- %d\n",PktsCoeffient, *PktsExponent);	
	return PktsCoeffient;
}

/****************************************************************************
Function:      ATBM_DVBC_CarrierOffset
Parameters:    none
Return:        DVBC_CarrierOffset(KHz)

Description:
	This function checks DVB-C signal carrier offset on field testing environment.
	This function is not meant to be used by customers. It's a debug utility to be used by FAEs.
****************************************************************************/
double   ATBM_DVBC_CarrierOffset()
{  
	int tmp;
	double dtmp;
	double ItbCarFreqOffset_stat,ItbCoarseFrequencyOffset,ChannelCentreFrequency;
	double DVBC_CarrierOffset;

	//-------Carrier frequency offset, reg ItbCarFreqOffset_stat---------------
	tmp = ATBM_DVBC_Read(0x3C0); //32bit signed
	ItbCarFreqOffset_stat = tmp/pow((double)2,(double)29)*dvbc_symbol_rate*1000;//khz

	//-----ChannelCentreFrequency , reg ItbFrequencyStatus------------------------
	tmp  = ATBM_DVBC_Read(0x11c); //27bit signed      
	tmp = tmp&0x7ffffff;
	dtmp = tmp;
	if(tmp&(0x1<<26))
	{
		dtmp=-(0x7ffffff-dtmp+1);
	}
	ChannelCentreFrequency = dtmp*dvbc_osc_freq/pow((double)2,(double)27)*1000; //khz

	//------ItbCoarseFrequencyOffset--------------------------------------------
	tmp = ATBM_DVBC_Read(0x118); //32bit signed      
	ItbCoarseFrequencyOffset = tmp*dvbc_osc_freq/pow((double)2,(double)27)*1000; //khz

	//-----------  calculate overall_CFO -------------
	if(ChannelCentreFrequency<0)
	{
		DVBC_CarrierOffset =  ChannelCentreFrequency  + fabs(ItbCoarseFrequencyOffset)+ ItbCarFreqOffset_stat;
	}
	else
	{
		DVBC_CarrierOffset =  ChannelCentreFrequency  - fabs(ItbCoarseFrequencyOffset)+ ItbCarFreqOffset_stat;
	}
	printf("DVB-C signal Carrier Offset = %f KHz\n", DVBC_CarrierOffset);        
	return DVBC_CarrierOffset;		
}

/****************************************************************************
Function:   ATBM_DVBC_GetQAM
Parameters:   None                    
Return:    QAM_index

Description:
	Get DVB-C QAM index			
	4: 16QAM
	5: 32QAM
	6: 64QAM
	7: 128QAM
	8: 256QAM
******************************************************************************/
u_int8   ATBM_DVBC_GetQAM()
{
	u_int32 tmp;
	u_int8 QAM_index; 				

	tmp = ATBM_DVBC_Read(0x3d0);

	QAM_index = (u_int8)((tmp&0x7)+1); //pow(2,tmp) means QAM		
	printf("DVB-C signal QAM index = %d \n", QAM_index);       
	return QAM_index;
}

/****************************************************************************
Function:   ATBM_DVBC_Write
Parameters: uAddress: Register physical address 
u32Data - 32bit data 
Return:    None

Description:
	For DVB-C register write, It is an encapsulated function by calling ATBMTransWrite function.			 
******************************************************************************/
void ATBM_DVBC_Write(u_int32 uAddress, u_int32 u32Data)
{

	u_int8 w_data[4];
	u_int8 addr_l, addr_h;

	//Add mutex lock here if there is multi-thread operation of demodulator's I2C
	//the following I2C operations will not be interrupted. 

	addr_l = uAddress&0xff;
	addr_h = (uAddress>>8)&0xff;
        
	w_data[0]=u32Data&0xff,w_data[1]=(u32Data>>8)&0xff,w_data[2]=(u32Data>>16)&0xff,w_data[3]=(u32Data>>24)&0xff;

	ATBMTransWrite(0x15, 0x03, 0x0);		
	ATBMTransWrite(0x15, 0x01, addr_l);
	ATBMTransWrite(0x15, 0x02, addr_h);

	ATBMTransWrite(0x15, 0x04, w_data[0]);
	ATBMTransWrite(0x15, 0x05, w_data[1]);
	ATBMTransWrite(0x15, 0x06, w_data[2]);	
	ATBMTransWrite(0x15, 0x07, w_data[3]);

	ATBMTransWrite(0x15, 0x03, 0x01);	

	ATBMTransWrite(0x15, 0x03, 0x0);
	//Release mutex lock.
	printf(" %x %x\n", uAddress, u32Data);
}


/****************************************************************************
Function:      ATBM_DVBC_Read
Parameters:    uAddress: Register physical address 
Return:        rdata: 32-bit data read from target register  

Description:
	For DVB-C register read, It is an encapsulated function by calling
	ATBMTransWrite and ATBMTransRead functions.
******************************************************************************/
u_int32 ATBM_DVBC_Read(u_int32  uAddress)
{

	u_int8 ro_latch_on=1,ro_latch_off=0;
	u_int32 rdata;
	u_int8 data[4];
	u_int8 addr_l, addr_h;

	//Add mutex lock here if there is multi-thread operation of demodulator's I2C
	//the following I2C operations will not be interrupted. 

	addr_l = uAddress&0xff;
	addr_h = (uAddress>>8)&0xff;
	ATBMTransWrite(0x08, 0x4d, ro_latch_off);
	ATBMTransWrite(0x15, 0x03, 0x0);		
	ATBMTransWrite(0x15, 0x01, addr_l);
	ATBMTransWrite(0x15, 0x02, addr_h);

	ATBMTransWrite(0x08, 0x4d, ro_latch_on);
	data[0]= ATBMTransRead(0x15, 0x08);
	data[1]= ATBMTransRead(0x15, 0x09);
	data[2]= ATBMTransRead(0x15, 0x0a);
	data[3]= ATBMTransRead(0x15, 0x0b);
	ATBMTransWrite(0x08, 0x4d, ro_latch_off);

	rdata = (data[3]<<24) + (data[2]<<16) + (data[1]<<8) +data[0];	
        //Release mutex lock.
	return rdata;
}


/****************************************************************************
Function:    ATBMTransRead
Parameters:  base_address, register_address
Return:      reg_data[0]

Description: 
     ATBM884x demodulator register Reading API, decoder platform independent. 
     It calls I2Cwrite and i2CRead functions that should be implemented by 
     customers based on customers' platforms.
     
     This function is used for DVB-C transfer. it use two step I2C operations
     as ATBMWRead. 
     **Should not add I2C mutex lock in this API. refer ATBM_DVBC_Write and ATBM_DVBC_Read
****************************************************************************/
u_int8  ATBMTransRead(u_int8 base_addr, u_int8 register_addr)
{ 
	int retval = 0; 
	u_int8 addr_data[2];
	u_int8 reg_data[1];

	addr_data[0] = base_addr;
	addr_data[1] = register_addr;

	retval = I2CWrite(ATBMSlaveAddr, addr_data, 2);
	if(!retval)
	{
		return 0;
	}

	retval = I2CRead(ATBMSlaveAddr, reg_data, 1);
	if(!retval)
	{
		return 0;
	}
			
	return(reg_data[0]);
}


/****************************************************************************
Function:        ATBMTransWrite
Parameters:  base_address, register_address, data
Return:      1:success, 0:fail  

Description: 
     ATBM884x demodulator register Writing API, decoder platform independent. 
     It calls I2Cwrite and I2CRead functions that should be implemented by 
     customers based on customers' platforms.
     
     This function is used for DVB-C transfer. it use two step I2C operations
     as ATBMWrite. 
     **Should not add I2C mutex lock in this API. refer ATBM_DVBC_Write and ATBM_DVBC_Read
****************************************************************************/
int  ATBMTransWrite(u_int8 base_addr, u_int8 register_addr, u_int8 data)
{ 
	int retval = 0; 
	u_int8 addr_data[2];
	u_int8 reg_data[1];

	addr_data[0] = base_addr;
	addr_data[1] = register_addr;

	retval = I2CWrite(ATBMSlaveAddr, addr_data, 2);
	if(!retval)
	{
		return 0;
	}

	reg_data[0] = data;
	retval = I2CWrite(ATBMSlaveAddr, reg_data, 1);
	if(!retval)
	{
		return 0;
	} 	
	//printf("%x %x %x\n", base_addr, register_addr, data);		
	return(retval);
}

#endif

