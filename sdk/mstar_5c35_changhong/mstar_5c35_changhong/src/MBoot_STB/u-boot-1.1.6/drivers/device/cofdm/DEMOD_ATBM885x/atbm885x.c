/***************************************************************************** 
Copyright 2011, AltoBeam Inc. All rights reserved.
     
File Name: atbm885x.c
   
Version: 1.4
Released 2011-5-29

Description: Example programming interface for ATBM885x	

Important struct:

custom_config
           All custom_config parameters should be customized according to system           

Important functions:

ATBMInit_DTMB_Mode()
           This function sets the demodulator working in DTMB mode. and calls ATBMInit
           function. it can be used in mode switching between DTBM and DVB-C
           
ATBMInit_DVBC_Mode()
           This function sets the demodulator working in DVB-C mode.and calls ATBMInit
           function. it can be used in mode switching between DTBM and DVB-C

ATBMInit() 
           It is called by ATBMInit_DTMB_Mode or ATBMInit_DVBC_Mode. This function initializes
           the demodulator in DTMB mode or DVB-C mode, all related configurations are included. 

ATBMLockedFlag()
           This function checks the demodulator locking status.          

ATBMSignalQuality()
           This function evaluates signal quality.                
           
******************************************************************************/

#include <frontend_config.h>

#if (FRONTEND_DEMOD_TYPE == DEMOD_ATBM885x)

//#include <stdio.h>
#include "../../../../include/prana2/stdio.h"

#include <sys/common/MsCommon.h>
#include <sys/drv/drvHWI2C.h>
#include <sys/drv/drvIIC.h>
#include <sys/drv/drvGPIO.h>

#include "atbm885x.h"

//#include <math.h>
//#include "MsCommon.h"

//#include "drvIIC.h"

//#include "drvGPIO.h"



#define ATBM_DEBUG   0      /*default no debug output*/

u_int8  ATBMSlaveAddr = 0x80;   /*either 0x80 or 0x88 */
u_int8  ATBM_SOFTWARE_VERSION = 14; /*SW guide version*/

static u_int8 DTMB_Internal[]=
{		
	0x00, 0x16, 0x1d,
	0x09, 0x88, 0x08,
	0x09, 0x89, 0x0c,
	0x09, 0x9a, 0x40,
	0x09, 0x35, 0x14,
	0x0a, 0xf9, 0x00,
	0x0e, 0x01, 0x20,
	0x08, 0x52, 0x28,
	0x0c, 0x24, 0x0a,
	0x0c, 0x26, 0x0a
};
static u_int8 DVBC_Internal[]=
{			
	0x00, 0x0a, 0x00,
	0x00, 0x09, 0x00,
	0x00, 0x13, 0x01,
	0x15, 0x18, 0x01,
	0x15, 0x15, 0x01,
	0x15, 0x20, 0x00,
	0x15, 0x21, 0x80,
	0x15, 0x2e, 0x01
};
u_int8 ATBM_ChipID;
struct custom_config_t custom_config;

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
static  double pow( double d, int n)
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
 Function:    ATBMInit_DTMB_Mode
 Parameters:  none
 Return:      none  
 
 Description:
	This function initializes ATBM885x in DTMB mode, selects DTMB receiving mode,
	then calls ATBMInit function.	  
 ******************************************************************************/
 void ATBMInit_DTMB_Mode(void)
 {		 
	 custom_config.dtmb_dvbc_mode = ATBM_DTMB_MODE;

	 /* Configure the demod with IF frequency of the tuner, according to the manual of the tuner.
	 * Options:
	 *       36, 36.1667 is typically used (MHz), ATBM885x support other tuner IF frequency either.
	 *       0 (used for ZIF tuner)  
	 */     

#if (FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER) //Mapping to Tuner setting..
//#if (FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER) //Mapping to Tuner setting..
	custom_config.tuner_config.IF_Frequency = 4.57;
#else
	custom_config.tuner_config.IF_Frequency = 36.0;
#endif
	 ATBMInit();
 }

 /****************************************************************************
 Function:    ATBMInit_DVBC_Mode
 Parameters:  none
 Return:      none  
 
 Description:
	  This function initializes ATBM885x in DVB-C mode, selects DVB-C receiving mode,
	  then calls ATBMInit function	  
 ******************************************************************************/
 void ATBMInit_DVBC_Mode(void)
 {	 
 
	 custom_config.dtmb_dvbc_mode = ATBM_DVBC_MODE;

	 /* Configure the demod with IF frequency of the tuner, according to the manual of the tuner.
	 * Options:
	 *     36, 36.125 or other tuner IF frequrency (MHz)
	 *     0  used for ZIF tuner
	 */
	 custom_config.tuner_config.IF_Frequency = 36.0;

	 ATBMInit();  
 }

 /****************************************************************************
Function:     ATBMInit
Parameters:   none
Return:       none  

Description:      
    Initializes ATBM885x demodulator and configures all necessary registers. 
    The demodulator will start processing incoming data after this function is called.

    This function includes Software Reset and demodulator configuration:   
       1. Demodulator reset
       2. Demodulator configuration
       3. Configure done confirmation                         
******************************************************************************/
 void ATBMInit()
 {    
	 u_int8 config_done = 0x01;	

	 /***********************************************************/
	 /*           Customize begin                                                                 */    
	 /***********************************************************/

	 /* Related with tuner IF signal. 
	 * The choice between the two options below depends on the hardware design if ZIF Tuner is used.
	 * Options:NO_SWAP_IQ, SWAP_IQ, default: NO_SWAP_IQ
	 */
#if (FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER)
	 custom_config.tuner_config.IQ_mode = SWAP_IQ;  
#else   
	 custom_config.tuner_config.IQ_mode = NO_SWAP_IQ;       

#endif

	 /* Configure the TS stream interface of the demod.
	 * Select the TS stream output in parallel or serial mode.
	 * Options:TS_PARALLEL_MODE, TS_SERIAL_MODE,	
	 */  					     
	 custom_config.ts_mode.TS_Transfer_Type = TS_SERIAL_MODE;//TS_PARALLEL_MODE;	 


	 /* Select the TS stream data output alignment with clock edge: on rising edge or on falling edge.
	 * if TS_OUTPUT_FALLING_EDGE is selected, MPEG decoder should use Rising edge
	 * if TS_OUTPUT_RISING_EDGE  is selected, MPEG decoder should use Falling edge
	 * Options:TS_OUTPUT_FALLING_EDGE, TS_OUTPUT_RISING_EDGE  
	 */
	 custom_config.ts_mode.output_edge = TS_OUTPUT_FALLING_EDGE;    


	 /* Configure the output of the TS stream clock pattern: free-running, or running only when data are valid.
	 * Options: TS_CLOCK_CONST_OUTPUT, TS_CLOCK_VALID_OUTPUT
	 */ 
	 custom_config.ts_mode.SPI_clock_constant_output = TS_CLOCK_CONST_OUTPUT;


	 /*Select TS SPI data output MSB, valid for parallel TS interface.
	 *if TS_SPI_MSB_ON_DATA_BIT7 is selected, TS data MSB on data[7], bit7 Pin 
	 *if TS_SPI_MSB_ON_DATA_BIT0 is selected, TS data MSB on data[0], bit0 Pin 
	 */
	 custom_config.ts_mode.TS_SPI_MSB_Selection = TS_SPI_MSB_ON_DATA_BIT7;

	 /*Select TS SSI data output pin for Serial TS output, valid for serial TS interface.
	 *if TS_SSI_OUTPUT_ON_DATA_BIT0 is selected, serial data output on data[0], bit0 Pin.
	 *if TS_SSI_OUTPUT_ON_DATA_BIT7 is selected, serial data output on data[7], bit7 Pin.
	 */
	 custom_config.ts_mode.TS_SSI_Output_Selection = TS_SSI_OUTPUT_ON_DATA_BIT0;


	 /* Configure the clock source of the demod.	 
	 * Select  whether a crystal or an oscillator is used for the clock source of the demod.
	 * Options: CRYSTAL , OSCILLATOR  
	 */  
	 custom_config.CRYSTAL_OR_OSCILLATOR = CRYSTAL;   


	 /* Select the clock source frequency for the demod.
	 * Options: 30.4 (MHz)
	 *   please contact FAE if need to support other frequencies.
	 */           
	 custom_config.sample_clk_Frequency = 30.4;   

	 /* Configure DVB-C Symbol rate, valid for DVB-C receiving*/	  
	 custom_config.dvbc_params.symbol_rate = 6875;//default: 6875K, 6.875M 	 

	 /***********************************************************/
	 /*           Customize end                                 */
	 /***********************************************************/
  
	 ATBMReset(custom_config.CRYSTAL_OR_OSCILLATOR); //Check hardware state	 
	 Delayms(10); /*10 ms*/  
	 ATBMWrite(0x00, 0x04, 0x00);	
	 ATBMWrite(0x01, 0x0c, 0x01);
	 ATBMWrite(0x10, 0xf7, 0xe8);	
	 ATBMWrite(0x10, 0xfb, 0x07);	
	 ATBMConfig( custom_config);
	 if(custom_config.dtmb_dvbc_mode == ATBM_DVBC_MODE) 
	 {
		 ATBMWriteRegArray(DVBC_Internal,sizeof(DVBC_Internal)/sizeof(unsigned char)); 
		 ATBMWrite(0x15, 0x11, 0x01); 
		 ATBMWrite(0x15, 0x12, 0x01); 
		 ATBMWrite(0x00, 0x16, 0x00);	//IF mode	 
		 ATBM_DVBC_Init(custom_config);                     
	 }
	 else //DTMB mode
	 {
		  ATBMWrite(0x00, 0x0a, 0x01); 
		  ATBMWrite(0x00, 0x09, 0x01);  	     
		  ATBMWrite(0x00, 0x13, 0x00); //DTMB mode
		  ATBMWrite(0x15, 0x18, 0x00);			
		  ATBMWrite(0x15, 0x15, 0x00); 	

		  ATBMWrite(0x15, 0x11, 0x00); 
		  ATBMWrite(0x15, 0x12, 0x00);
	 
		 ATBMWriteRegArray(DTMB_Internal,sizeof(DTMB_Internal)/sizeof(unsigned char)); 			
	 } 
	 ATBMWrite(0x00, 0x04, config_done);	
#if ATBM_DEBUG
	 ATBMDebugRegister();
#endif  
 }
 
/****************************************************************************
Function:     ATBMChipID
Parameters:   none
Return:       ChipID  

Description:
     This function checks chip ID,The chip ID of ATBM885x is 0x30.     
******************************************************************************/
u_int8 ATBMChipID()
{
	u_int8 chipID;
	chipID = ATBMRead(0x00, 0x00);
	printf("Chip ID= 0x%x, chipID ATBM885x:0x30\n",chipID);
	return chipID;
}
 
 
 /****************************************************************************
Function:    ATBMConfig
Parameters:  custom_config
Return:      none  

Description:  
     Set registers according to customer's configuration, 
     This function configures ATBM885x. 
 ******************************************************************************/
void ATBMConfig(struct custom_config_t custom_config)
{

	/*Demodulator default clock 30.4MHz; Tuner IF 36.166Mhz*/
	ATBMSetOSC(custom_config.tuner_config,custom_config.sample_clk_Frequency);		

	/*Configures MPEG TS output  mode*/		 	
	ATBMSetTSMode(custom_config.ts_mode); 
	ATBMWrite(0x01, 0x02, 1);	 //I2C related
	ATBMWrite(0x02, 0x0c, ATBM_SOFTWARE_VERSION);
}

/****************************************************************************
Function:    ATBM_Check_Demod_Status
Parameters:  none
Return:      PLL lock status  

Description:  
   Call this function when crystal is used. if PLL locked, hardware reset is not needed.  
   This function is called before configuring demodulator registers.
   This function check demodulator status after power on or hardware reset 
   If 1 is returned, demodulator in ready status.
   If 0 is returned, demodulator need hardware reset or power on.
******************************************************************************/
u_int8 ATBM_Check_Demod_Status(void)
{	
	u_int8 check_times = 0;
	u_int8 analog, PLL_state, tmp, hardware_state;
	u_int8 step = 0x20; 
	hardware_state = ATBMRead(0x06, 0x07);
	if(hardware_state!=5)
	{	
		return 0;	
	}
	PLL_state = ATBM_Check_PLL_Status();  
	if(PLL_state == 1)
	{ 
		return 1;
	}		
	tmp = ATBMRead(0x02, 0x14);	
	while(check_times<5)
	{
		if((tmp&0x02) == 0x02) 
		{		
			ATBMWrite(0x06, 0x06, 0x02); 	
		}else
		{
			ATBMWrite(0x06, 0x06, 0x00); 
		}
		analog = check_times*step;	
		if(check_times == 4)
		{
			analog = 0x60;	
		}
		ATBMWrite(0x06, 0x0d, analog);
		if(check_times == 4)
		{
			ATBMWrite(0x06, 0x05, 0x00);
		}
		Delayms(10);			
		PLL_state = ATBM_Check_PLL_Status(); 
		if(PLL_state == 1)
		{         
			return 1; 
		}	
		check_times = check_times + 1;
	}
	return 0;
} 
/****************************************************************************
Function:    PLL_Lock_Flag
Parameters:  none
Return:      demodulator PLL lock status  

Description: 
	 when internal PLL is locked, the returned value is 1.
******************************************************************************/
 u_int8 PLL_Lock_Flag()
 {
	 int i = 0;
	 u_int8 flag;
	 while(i<20)
	 {
		 flag = ATBMRead(0x06, 0x11);
		 if((flag&0x01) != 0x01)
		 {
			 return 0;
		 }
		 i++;
	 }
	 return 1;
 }
/****************************************************************************
Function:    ATBM_Check_PLL_Status
Parameters:  none
Return:      PLL status  

Description: 
	 When demodulator PLL locked and stable, the returned value is 1.
******************************************************************************/
 u_int8 ATBM_Check_PLL_Status(void)
 {
	 u_int8 pll_state, tmp;	

	 pll_state = PLL_Lock_Flag(); 
	 if(pll_state == 1)
	 {
		 tmp = ATBMRead(0x02, 0x14);
		 if((tmp&0x02) != 0x02)
		 {		
			 ATBMWrite(0x06, 0x06, 0x02); 
		 }
		 else
		 {
			 ATBMWrite(0x06, 0x06, 0x00); 
		 }
	 }
	 else
	 {
		 return 0;
	 }
	 Delayms(10);
	 pll_state = PLL_Lock_Flag();
	 if(pll_state == 1)
	 {
		 return 1;
	 }
	 return 0;
 }

/****************************************************************************
Function:    ATBMReset
Parameters:  cry_or_osc
Return:      none  

Description:
      This function resets ATBM885x demodulator if PLL is not locked. this is a
	  compatible function used in ATBMInit or be called after suspend.
	  for example, transfer from  suspend mode to standby mode.
 ******************************************************************************/
 void ATBMReset(u_int8 cry_or_osc)
 {

	 u_int8 HardwareState, check_result = 0;
	 u_int8 loop, tmp;	

	 /*
	 * After hardware power on properly or reset correctly,	  
	 * HardwareState value should be 0x05 when using crystal, 3.3V PLL 
	 * HardwareState value should be 0x07 when using oscillator, 3.3V PLL 
	 */	
	 if(cry_or_osc==CRYSTAL)
	 {
		 //Software checking, when power on failed, it will return 0 because I2C error or PLL error
		 check_result = ATBM_Check_Demod_Status();
		 if(check_result == 1)
		 {
			 return;
		 }	
		 //Hardware checking, usually used after demodulator resume from suspend mode
		 for(loop=0; loop<2; loop++)
		 {
			 if(check_result != 1)
			 {
				 printf("I2C Error or it is not properly power up, need hardware reset\n");
				 DemodHardwareReset();    //hardware check
				 Delayms(50);	
				 check_result = ATBM_Check_Demod_Status(); 		
			 }
			 else
			 {
				 return;
			 }	 
		 }
	 }	

	 if(cry_or_osc==OSCILLATOR)
	 {
		 HardwareState = ATBMRead(0x06, 0x07);
		 for(loop=0; loop<10; loop++)
		 {
			 if(HardwareState!=0x07)
			 {		            
				 printf("I2C Error or it is not properly power up, need hardware reset\n");
				 DemodHardwareReset();
				 Delayms(100);					
				 HardwareState  =ATBMRead(0x06, 0x07);
			 }
			 else
			 {
				 tmp = ATBMRead(0x02, 0x14);
				 if((tmp&0x02) != 0x02)
				 {		
					 ATBMWrite(0x06, 0x06, 0x02); 
				 }
				 else
				 {
					 ATBMWrite(0x06, 0x06, 0x00); 
				 }
				 break;
			 }	 
		 }
	 }

 }


/****************************************************************************
Function:    ATBMSetTSMode
Parameters:  ts_mode
Return:      none  

Description:
      This function configures Transport stream interface mode.   		 
******************************************************************************/
void ATBMSetTSMode(struct MPEG_TS_mode_t ts_mode)
{
	u_int8 SerialEnabled =0x0;/*parallel clock out */
	//u_int8 SSI_sync_hold_cycle = 0x00;
	u_int8 TSOutputEdegeMode =ts_mode.output_edge;  
	u_int8 TsClockMode = ts_mode.SPI_clock_constant_output;  

	/*0x00: SPI data align with rising edge of TSBCLK; 0x01: SPI data align with falling edge of TSBCLK*/
	ATBMWrite(0x03, 0x01, TSOutputEdegeMode); 	
	ATBMWrite(0x03, 0x06, TsClockMode);  /*either free running or only active when data are valid*/      

	if(ts_mode.TS_Transfer_Type==TS_SERIAL_MODE)
	{    
		SerialEnabled = 0x01;    /*enable serial TS mode */   

		//SSI_sync_hold_cycle = 0x00; //0x00:1 bit sync;0x01: 8 bit sync
		/*0: serial sync  holds 1 cycle; 1: serial sync holds 8 cycles*/
		//ATBMWrite(0x03,0x0f, SSI_sync_hold_cycle);  

		/*0:disable serial mode; 1: enable serial mode */  
		ATBMWrite(0x03, 0x05, SerialEnabled);   
		if(ts_mode.TS_SSI_Output_Selection == TS_SSI_OUTPUT_ON_DATA_BIT0)
		{
			ATBMWrite(0x03, 0x1f, 0x00); 
		}else
		{
			ATBMWrite(0x03, 0x1f, 0x01); 
		}
	}
	else	
	{	  
		SerialEnabled =0x0; /*parallel clock output */
		/*0: disable serial mode; 1:enable serial mode */	
		ATBMWrite(0x03, 0x05, SerialEnabled); 
		if(ts_mode.TS_SPI_MSB_Selection == TS_SPI_MSB_ON_DATA_BIT7)
		{
			ATBMWrite(0x03, 0x1f, 0x00); 
		}else
		{
			ATBMWrite(0x03, 0x1f, 0x01); 
		}
	}
}

/****************************************************************************
Function:    ATBMSetOSC
Parameters:  tuner_config, sample_clk_Frequency
Return:      1 for success,0 for error   

Description:
     This function configures ATBM885x according to external Clock source 
     and  Tuner IF frequency.
 ******************************************************************************/
int ATBMSetOSC(struct tuner_config_t tuner_config, double sample_clk_Frequency) 
{
	/* 
	* Demodulator default registers are for 30.4 MHz crystal, no need to list here.
	*/
	u_int8  u8RXFLT[6]; 
	double tuner_if_frequency = tuner_config.IF_Frequency;	   
	double CLK_Freq = sample_clk_Frequency;
	u_int32 freq_lif_tmp;
	u_int32 rate_ratio_tmp;
	u_int8  IF_tuner_on= 0;
	u_int8 ADC_config= 0x16;	
	u_int8 swap_iq;	

	if(tuner_config.IQ_mode == SWAP_IQ)
	{	 	
		swap_iq = 0x03;   
		ATBMWrite(0x0c, 0x01, swap_iq); 
	}

	if((tuner_if_frequency > -0.0001)&&(tuner_if_frequency < 0.0001)) //0:ZIF tuner
	{
		/*Tuner selection. 0: ZIF tuner; 1: IF tuner*/  
		ATBMWrite(0x00, 0x01, IF_tuner_on);  
		
		/*ADC config*/
		ATBMWrite(0x06, 0x02, ADC_config); 
	}
	else
	{
		/*
		* 0x0A03~0x0A05 are only related to sampling frequency; 
		* 0x0A00~0x0A02 are related to sampling frequency and tuner IF frequency.
		*/	
		if(tuner_if_frequency>CLK_Freq) 
		{
			freq_lif_tmp=(int)floor(2*PI*(tuner_if_frequency-CLK_Freq)*(1<<22)/CLK_Freq);
		}
		else //low IF
		{
			freq_lif_tmp=(int)floor(2*PI*(tuner_if_frequency)*(1<<22)/CLK_Freq);
		}
		u8RXFLT[0] = (u_int8)(freq_lif_tmp&0xff);
		u8RXFLT[1] = (u_int8)((freq_lif_tmp>>8)&0xff);
		u8RXFLT[2] = (u_int8)((freq_lif_tmp>>16)&0xff);

		ATBMWrite(0x0A, 0x00, u8RXFLT[0]);
		ATBMWrite(0x0A, 0x01, u8RXFLT[1]);
		ATBMWrite(0x0A, 0x02, u8RXFLT[2]);   
		//printf("IF freq_lif: 0x%x 0x%x  0x%x\n", u8RXFLT[0], u8RXFLT[1], u8RXFLT[2]);
	}

	rate_ratio_tmp=(int)floor(2*CLK_Freq*(1<<19)/30.4 + 0.5); //32bit
	u8RXFLT[3] = (u_int8)(rate_ratio_tmp&0xff);
	u8RXFLT[4] = (u_int8)((rate_ratio_tmp>>8)&0xff);
	u8RXFLT[5] = (u_int8)((rate_ratio_tmp>>16)&0x1f);
  
	ATBMWrite(0x0A, 0x03, u8RXFLT[3]);
	ATBMWrite(0x0A, 0x04, u8RXFLT[4]);
	ATBMWrite(0x0A, 0x05, u8RXFLT[5]);   	
	//printf("ADC rate ratio: 0x%x 0x%x  0x%x\n", u8RXFLT[3], u8RXFLT[4], u8RXFLT[5]);	
	return 1;			
}

/****************************************************************************
Function:    ATBM_I2CByPassOn
Parameters:  none
Return:      none  

Description:      
      Enable demod to pass I2C commands between tuner and host.  
 ****************************************************************************/
 void ATBM_I2CByPassOn()
 {
	 u_int8 i2c_bybass_mode = 1;
	 /* 1: Enable demod to pass I2C commands between tuner and host. */
	 ATBMWrite(0x01, 0x03, i2c_bybass_mode); 
 }


 /****************************************************************************
Function:    ATBM_I2CByPassOff
Parameters:  none
Return:      none  

Description:      
     Disable the I2C pass-through. Tuner is disconnected from the I2C BUS after this
     function is called. 
 ****************************************************************************/
 void ATBM_I2CByPassOff()
 {
	 u_int8 i2c_bybass_mode = 0;
	 
	 /*0: Disable demod to pass I2C commands between tuner and host*/
	 Delayms(10);   //Some tuners need this. 
	 ATBMWrite(0x01, 0x03, i2c_bybass_mode); 
 }
 
 
 /****************************************************************************
 Function:    ATBMLockedFlag
 Parameters:  none
 Return:      1: locked, 0:unlocked  
 
   Description:
   This function checks ATBM885x Demod locking status, i.e., whether demod has 
  decoded DTMB or DVB-C signal.   		 
 ****************************************************************************/
 int ATBMLockedFlag()
 {
	 u_int8 dtmb_fec = 0;
	 u_int32 dvbc_status = 0; 	 
	 u_int8 cur_receiving_mode;
	 
	 cur_receiving_mode = ATBMRead(0x00, 0x13); //0:DTMB, 1:DVB-C
	 if(cur_receiving_mode==0x00) //DTMB
	 {
		 dtmb_fec = ATBMRead(0x0b, 0x08);
		 if((dtmb_fec&0x01)==0x01) //FER locked:bit0
		 {
			 return 1;
		 }
	 }
	 if(cur_receiving_mode==0x01)
	 {
		 dvbc_status = ATBM_DVBC_Read(0x08);   //read DVB-C internal register        
		 if(dvbc_status&0x20) //bit 5
		 {                           
			 return 1;   //DVB-C FEC locked
		 } 	 
	 }
	 return 0;
 }

/****************************************************************************
Function:    ATBMLockedLoopCheck()
Parameters:  none
Return:      1: locked, 0:unlocked  

Description:
      This function keeps checking ATBM885x signal locking status until it's Locked or time out. 
      recommended timeout value
      DTMB: 2000 milliseconds, usually locked in 300-400 Milliseconds if DTMB signal is detected
      DVB-C: 600 milliseconds, usually locked in 100-200 Milliseconds if DVB-C signal is detected.
****************************************************************************/
int ATBMLockedLoopCheck()
{
	unsigned char dtmb_fec = 0;
	unsigned int dvbc_status = 0; 	
	int i;
	unsigned char cur_receiving_mode;
	int check_times;
	unsigned int dtmb_time_out = 2000;  //2000 millisecond
	unsigned int dvbc_time_out = 600;

    cur_receiving_mode = ATBMRead(0x00, 0x13); //0:DTMB, 1:DVB-C
	if(cur_receiving_mode == 0) //DTMB
	{
		check_times = dtmb_time_out/50;
		for(i=0; i<check_times; i++)
		{
			dtmb_fec = ATBMRead(0x0b, 0x08);			
			if(dtmb_fec&0x01) //bit 0
			{                           
				return 1;           //DTMB FEC locked
			}                 
			Delayms(50);
		}
	}
	if(cur_receiving_mode == 1) //DVB-C
	{
		check_times = dvbc_time_out/50;
		for(i=0; i<check_times; i++)
		{
			dvbc_status = ATBM_DVBC_Read(0x08);       //read DVB-C internal register        
			if(dvbc_status&0x20) //bit 5
			{                           
				return 1;           //DVB-C FEC locked
			}                 
			Delayms(50);
		}
	}
	return 0; //FEC not locked
}

/****************************************************************************
Function:    ATBMFrameErrorRatio
Parameters:  none
Return:      frame_error_ratio 

Description:    
	This function counts pre-BCH error frames in specified frame window size
	(Default 2048 frames), then print and return pre-BCH frames error ratio.   
****************************************************************************/
double ATBMFrameErrorRatio()
{

	u_int8 frame_error0, frame_error1, LockValue;
	u_int8 frame_count_max[2];
	double frame_error_ratio;

	frame_count_max[0] = ATBMRead(0x0B, 0x09); 
	frame_count_max[1] = ATBMRead(0x0B, 0x0A);	

	LockValue =1; 
	ATBMWrite(0x08, 0x4d, LockValue);  /*latch read only registers*/
	frame_error0 = ATBMRead(0x0B, 0x14);  
	frame_error1 = ATBMRead(0x0b, 0x15);
	LockValue = 0;
	ATBMWrite(0x08, 0x4d, LockValue);

	frame_error_ratio = ((frame_error1&0x7f)*256 + frame_error0)*1.0/((frame_count_max[1]&0x7f)*256 + frame_count_max[0]);	
	//printf("DTMB Pre-BCH frames error ratio = %f\n",frame_error_ratio);
	return  frame_error_ratio; 

}

/****************************************************************************
Function:       ATBMSignalStrength
Parameters:     none
Return:         agc_value, in the range of 0--1023

Description:
     This function checks ATBM885x DTMB signal strength related agc_value: 
     when signal is weak agc_value will be bigger; The smaller the agc_value
     is, the stronger the signal strength detected.	 	
 ****************************************************************************/
u_int16 ATBMSignalStrength()	
{
    u_int8 agc0, agc1,LockValue;
    u_int16 agc_value;
  
    LockValue =1; 
    ATBMWrite(0x08, 0x4d, LockValue); /*latch read only registers*/
	
    agc0 = ATBMRead(0x10, 0x28);  
    agc1 = ATBMRead(0x10, 0x29);
	
    LockValue = 0;
    ATBMWrite(0x08, 0x4d, LockValue);

    agc_value = (u_int8)(((agc1&0x03)<<8)|agc0);
    //printf("signal strength agc_value = %d\n", agc_value);
    return agc_value;
}

/****************************************************************************
Function:           ATBMSignalNoiseRatio
Parameters:      none
Return:             double (dB) 

Description:
     This function checks ATBM885x signal-to-noise ratio.
     Can be used for signal quality evaluation.   		 
 ****************************************************************************/
 double ATBMSignalNoiseRatio()	
 {
	 u_int8 LockValue;
	 
	 u_int8 pn_value,PN;   
	 u_int8 tmp_noise; 
	 u_int8 is_sc;
	 double SNR = 0;   
	 
	 u_int8 signal_h, signal_m, signal_l;
	 u_int8 noise_h,noise_m, noise_l;   
	 u_int32 signal_power = 0;
	 u_int32 noise_power = 0;
	 
	 LockValue =1; /*lock register value*/
	 ATBMWrite(0x08, 0x4d, LockValue);
	 
	 pn_value = ATBMRead(0x08, 0x2d);   
	 PN = (u_int8)(pn_value&0x03);
	 
	 tmp_noise = ATBMRead(0x14, 0xAF); 	
	 is_sc = ATBMRead(0x0d, 0x0f); 
	 if(is_sc!=1)
	 {
		 signal_l = ATBMRead(0x0f, 0x1d);
		 signal_m = ATBMRead(0x0f, 0x1e);
		 signal_h = ATBMRead(0x0f, 0x1f); 
		 noise_l = ATBMRead(0x0f, 0x1a);
		 noise_m = ATBMRead(0x0f, 0x1b);   
		 noise_h = ATBMRead(0x0f, 0x1c);
		 signal_power = ((signal_h&0x0f)<<16)+(signal_m<<8)+signal_l;
		 noise_power  = ((noise_h&0x07)<<16)+(noise_m<<8)+noise_l; 				 
		 if(signal_power ==0)
		 {
			 signal_power =1; //signal_power should >0 in log calculation;
		 }		
		 SNR = (double) (10*(log10(signal_power*1.0/(noise_power+1))));	    
	 }	 
	 if(is_sc==1)
	 {
		 if(PN==2) //PN595
		 {   	 
			 signal_l = ATBMRead(0x14, 0xdc);
			 signal_m = ATBMRead(0x14, 0xdd);
			 signal_h = ATBMRead(0x14, 0xde);  
			 signal_power = ((signal_h&0x0f)<<16)+(signal_m<<8)+signal_l;
			 signal_power = signal_power/16;
			 if(tmp_noise!=3)
			 {
				 noise_l = ATBMRead(0x14, 0xf8);		
				 noise_h = ATBMRead(0x14, 0xf9);
				 noise_power = ((noise_h&0x3f)<<8) + noise_l;
			 }
			 else
			 {                    
				 noise_l = ATBMRead(0x13, 0x40);		
				 noise_h = ATBMRead(0x13, 0x41);                       
				 noise_power = ((noise_h&0x3f)<<8) + noise_l;
			 }	
			 if(signal_power ==0)
			 {
				 signal_power =1; //signal_power should >0 in log calculation;
			 }		
			 SNR = (double) (10*(log10(signal_power*1.0/(noise_power+1))));	    
			 
		 }
		 
		 if( (PN==1)||(PN==3) ) //PN420 & PN945
		 {   	   	
			 
			 signal_l = ATBMRead(0x09, 0xcc);	
			 signal_h = ATBMRead(0x09, 0xcd);  
			 signal_power = (signal_h<<8) + signal_l;
			 noise_l = ATBMRead(0x09, 0xed);
			 noise_h = ATBMRead(0x09, 0xee);
			 noise_power = ((noise_h&0x3f)<<8) + noise_l;						
			 
			 if(signal_power ==0)
			 {
				 signal_power =1; //signal_power >0;
			 }			 
			 if(noise_power==0)
			 {
				 // printf("noise_power is 0,  signal error!");
				 SNR = (double)(10*(log10(signal_power*1.0/(noise_power + 1))));
			 }
			 else
			 {	
				 SNR = (double)(10*(log10(signal_power*1.0/noise_power)));	    
			 }    
		 }
	 }
	 LockValue = 0; /*cancel the lock*/
	 ATBMWrite(0x08,0x4d, LockValue);
	 return SNR;
 }
/****************************************************************************
Function:    ATBMSignalQuality
Parameters:  none
Return:      SNR100per, the signal quality evaluated value. 

Description:
     This function checks ATBM885x signal-to-noise ratio and evaluated in range of 30--100.
     Can be used for signal quality evaluation:
     max value:99, min:30
 ****************************************************************************/
int ATBMSignalQuality()
{   
 	u_int8 tps_code, ts_sync;  
	double frame_error;
	u_int32 SNR100per;   
	double SNR_dB = 0;    
	u_int32 SNR = 0;	
	SNR_dB = ATBMSignalNoiseRatio(); //ATBM885x
	SNR = (u_int32)(pow(10, SNR_dB/10) + 0.5);	
   		
	tps_code = ATBMGetTPS();
	ts_sync = ATBMRead(0x03, 0x0d);  
	if(ts_sync !=1)
	{
		ts_sync = ATBMRead(0x03, 0x0d);  
	}
	frame_error = ATBMFrameErrorRatio();

	SNR100per = 20;  
     if((tps_code <11)&&(tps_code>2))/*4QAM FEC:0.6 0.8*/
   	{
   	 if(SNR>100)
   	  	{
   	  	SNR100per = 90+ SNR/100;
   	  	}
   	  else if(SNR>60)
   	  	{
   	  	  SNR100per = 80 + (SNR-60)/4 ;
   	  	}   	 
	  else if(SNR>20)
	 	{
	 	 SNR100per = 60 + (SNR-20)/2;
	 	}
	  else if(SNR>5)
	  	{	  	
	         SNR100per = 40 + SNR;
	  	}
	  else
	    {
	     SNR100per = 40;
	    }
   	}
  
   if((tps_code <17)&&(tps_code>10)) /*For 16QAM FEC:0.6 0.8*/
   	{
   	  if(SNR>100)
   	  	{
   	  	SNR100per = 90+ SNR/100;
   	  	}
   	  else if(SNR>70)
   	  	{
   	  	  SNR100per = 80 + (SNR-70)/3 ;
   	  	}   	 
	  else if(SNR>30)
	 	{
	 	 SNR100per = 60 + (SNR-30)/2;
	 	}
	  else if(SNR>10)
	  	{	  	
	         SNR100per = 50 + (SNR-10)/2;
	  	}
	  else
	    {
	     SNR100per = 40;
	    }
   	}   
   if((tps_code <25)&&(tps_code>16))/*64QAM FEC:0.6; 32QAM 0.8*/
   	{
      
	   if(SNR>100)
   	  	{
   	  	SNR100per = 90+ SNR/100;
   	  	}
   	  else if(SNR>70)
   	  	{
   	  	 SNR100per = 80 + (SNR-70)/3 ;
   	  	}   	 
	  else if(SNR>30)
	 	{
	 	 SNR100per = 60 + (SNR-30)/2;
	 	}
	  else if(SNR>20)
	  	{	  	
	         SNR100per = 30+ SNR;
	  	}
	  else
	    {
	     SNR100per = 40;
	    }
   	}
   
    //re-evaluation   
	if((ts_sync ==1)&&(frame_error < 0.0001)) //no frame error
   		{
   		//default:5, you can change this value form 0--10, for evaluation	
   		SNR100per +=5; 	
   		}
	else if((ts_sync ==1)&&(frame_error > 0.0001))
	 	{
	 	  SNR100per = SNR100per - 10; 
	 	}
	else
		{
		  SNR100per = 30;
		}
   if(SNR100per>99)
	{
	  SNR100per = 99;
	}   
	printf("TPS =%d, SNR = %d SNR100per = %d\n",tps_code, SNR, SNR100per);   
   return SNR100per;
}


/***************************************************************************** 
Function:    ATBMSignalStrength_Sharp2093
Parameters:  none
Return:      signal_percent

Description:
     This function checks ATBM885x IF AGC power. It can be used for Sharp2093 Tuner
     signal strength evaluation.	
     This is reference code for Sharp2093 DTT Can tuner.
     Range:  20--80.This is for reference only. For specific Tuner the AGC range may be changed.
*****************************************************************************/
u_int16 ATBMSignalStrength_Sharp2093()	
{
	u_int8 agc0, agc1,LockValue;
	u_int16 agc_value;
	u_int16 signal_percent;

	LockValue =1; /*lock register value*/
	ATBMWrite(0x08,0x4d, LockValue);

	agc0 = ATBMRead(0x10, 0x28);  
	agc1 = ATBMRead(0x10, 0x29);

	LockValue = 0;
	ATBMWrite(0x08,0x4d, LockValue);
	agc_value = (u_int8)(((agc1&0x03)<<8)|agc0);

	/*Sharp 2093 Reference*/
	if(agc_value <352)
	{
		signal_percent = 80;	
	}                    
	else if(agc_value <492)
	{   	
		signal_percent = (u_int16)((532 - agc_value)/2);	
	}
	else
	{
		signal_percent = 20;	
	}
	/*************************************/
	return signal_percent;
}

/***************************************************************************** 
Function:    ATBMSignalStrength_TDAC2_C02A
Parameters:  none
Return:      signal_percent

Description:
     This function checks ATBM885x IF AGC power. It can be used for ALPS TDAC2-C02A Tuner
     signal strength evaluation.	
     This is reference code for ALPS TDAC2-C02A DTMB(DVB-C) Can tuner.
     Range:  20--90.This is for reference only. For specific Tuner the AGC range may be changed.
*****************************************************************************/
u_int16 ATBMSignalStrength_TDAC2_C02A()	
{
	u_int8 agc0, agc1,LockValue;
	u_int16 agc_value;
	u_int16 signal_percent;

	LockValue =1; /*lock register value*/
	ATBMWrite(0x08,0x4d, LockValue);

	agc0 = ATBMRead(0x10, 0x28);  
	agc1 = ATBMRead(0x10, 0x29);

	LockValue = 0;
	ATBMWrite(0x08,0x4d, LockValue);
	agc_value = (u_int16)(((agc1&0x03)<<8)|agc0);

	/*ALPS TDAC2-C02A tuner Reference evaluation*/
	if(agc_value <=380)
	{
		signal_percent = 90;	
	}                    
	else if(agc_value <=870)
	{   	
		signal_percent = (u_int16)((870 - agc_value)/7 + 20);	
	}
	else
	{
		signal_percent = 20;	
	}
	/*************************************/
	return signal_percent;
}

/*****************************************************************************
Function:    ATBMSignalStrength_Maxim2165
Parameters:  none
Return:      signal_percent

Description:
      This function checks ATBM885x IF AGC power. It can be used for Maxim2165 signal 
      strength evaluation.	
      This is reference code for maxim silicon tuner(maxim2165).
      Range:  20--80. This is for reference only. For specific Tuner the range may be changed
*****************************************************************************/
u_int16 ATBMSignalStrength_Maxim2165()	
{
	u_int8 agc0, agc1,LockValue;
	u_int16 agc_value;
	u_int16 signal_percent;

	LockValue =1; /*lock register value*/
	ATBMWrite(0x08,0x4d, LockValue);

	agc0 = ATBMRead(0x10, 0x28);  
	agc1 = ATBMRead(0x10, 0x29);

	LockValue = 0;
	ATBMWrite(0x08, 0x4d, LockValue);
	agc_value = (u_int16)(((agc1&0x03)<<8)|agc0);

	/*maxim2165 Reference*/
	if(agc_value <235)
	{
		signal_percent = 80;	 
	}			 
	else if(agc_value <655)
	{		 
		signal_percent = (u_int16)((775 - agc_value) /6);  
	}
	else
	{
		signal_percent = 20;	 
	}

	return signal_percent;
}


/****************************************************************************
Function:    ATBMGetTPS
Parameters:  none
Return:      TPS_index

Description:
      This function checks signal TPS. The return value is TPS index in GB20600-2006 		 
 ****************************************************************************/
u_int8 ATBMGetTPS()
{
    u_int8 TPS_index;
   
    TPS_index = ATBMRead(0x0d, 0x04);
   
    return TPS_index;
}

/****************************************************************************
Function:    ATBM_Standby
Parameters:  none
Return:      none

Description:
      This function put demodulator in standby mode, it can cut off part power,
	  the demodulator can wake up at once by calling ATBM_Standby_WakeUp.
	  two goals:
	  1, cut off part power when demodulator put in standby mode
	  2, put TS output pin in high-z state.
 ****************************************************************************/
void ATBM_Standby()
{
	u_int8 adc_pd;    
	u_int8 ts_edge;	  
	u_int8 ref_stdby;
	u_int8 adc_rfv;
	u_int8 gpo_reg;
	ref_stdby = ATBMRead(0x06, 0x00); 
	adc_pd = ATBMRead(0x06, 0x02);
	ts_edge = ATBMRead(0x03, 0x01);
	adc_rfv = ATBMRead(0x06, 0x0d);
	gpo_reg = ATBMRead(0x10, 0xf7);

	ref_stdby|= 0x02; //bit1 set 1
	adc_pd|=0x01;     //bit0 set 1
	ts_edge|= 0x02;   //bit1 set 1 
	adc_rfv|=0x08;    
	gpo_reg|=0x01;  //bit0 set 1
	ATBMWrite(0x00, 0x05, 0x01); 
	ATBMWrite(0x06, 0x0d, adc_rfv);
	ATBMWrite(0x06, 0x00, ref_stdby);
	ATBMWrite(0x06, 0x02, adc_pd); 
	ATBMWrite(0x15, 0x00, 0x00); 
	ATBMWrite(0x03, 0x01, ts_edge); //all TS output PINs will be high-z
	ATBMWrite(0x10, 0xf7, gpo_reg); 
	ATBMWrite(0x00, 0x19, 0x00); 
}

/****************************************************************************
Function:    ATBM_Standby_WakeUp
Parameters:  none
Return:      none

Description:
Put demodulator in previous normal working mode from standby mode 		 
****************************************************************************/
void ATBM_Standby_WakeUp()
{
	u_int8 adc_pd;    
	u_int8 ts_edge;
	u_int8 ref_stdby;
	u_int8 adc_rfv;
	u_int8 gpo_reg;
	ref_stdby = ATBMRead(0x06, 0x00);
	adc_pd = ATBMRead(0x06, 0x02);
	ts_edge = ATBMRead(0x03, 0x01);
	adc_rfv = ATBMRead(0x06, 0x0d);
	gpo_reg = ATBMRead(0x10, 0xf7);
	ref_stdby&=0xfd;  //bit1 set 0
	adc_pd&=0xfe;     //bit0 set 0
	ts_edge&=0xfd;    //bit1 set 0	
	adc_rfv&=0xf7;
	gpo_reg&=0xfe; //bit 0 set 0
	ATBMWrite(0x06, 0x00, ref_stdby);
	ATBMWrite(0x06, 0x02, adc_pd); 
	ATBMWrite(0x15, 0x00, 0x01); 
	ATBMWrite(0x03, 0x01, ts_edge); //all TS output PINs will be in normal mode	
	ATBMWrite(0x10, 0xf7, gpo_reg); 
	ATBMWrite(0x06, 0x0d, adc_rfv);
	ATBMWrite(0x00, 0x19, 0x01); 
	ATBMWrite(0x00, 0x05, 0x00); 
}

/****************************************************************************
Function:    ATBM_Suspend
Parameters:  none
Return:      none

Description:
    Put ATBM885x demodulator in suspend mode, Hardware reset is needed to wake up
    the demodulator. if external oscillator is used Hardware pull down is used 
    to enter suspend mode.
****************************************************************************/
void ATBM_Suspend()
{	
	ATBM_Standby();	
	ATBMWrite(0x06, 0x0e, 0x02); //shut down crystal clock
}


/****************************************************************************
Function:    ATBMDebugRegister
Parameters:  none
Return:      none

Description:
      This function checks Demodulator registers,  used for debug purpose 
 ****************************************************************************/
void ATBMDebugRegister()
{
   /*common register*/
	u_int8 software_version;	
	
	u_int8  demod_state;	
	u_int8  config_done;
 	u_int8  freq_offset; 	
 	u_int8  dsp_reset;	
	u_int8  i2c_bybass_state;
	u_int8  CAN_Tuner;
	u_int8  u8RXFLT[6];
	/*silicon tuner config*/
	u_int8  ADC_config;	
	u_int8  LNA_on_off;
	u_int8 fatal_rst, weak_rst;
	/*TS interface configuration*/
	u_int8 SamplingEdegeMode;   
	u_int8 SerialEnabled;
	u_int8 ClockOutSelect;
	
	software_version = ATBMRead(0x02, 0x0c);	
	demod_state = ATBMRead(0x06, 0x07);	
	config_done = ATBMRead(0x00, 0x04);	
	dsp_reset = ATBMRead(0x00, 0x0A);	
	printf("Current software version: %d\n", software_version);	
	printf("demod_state:0x%x configure done:0x%x dsp_reset:0x%x\n",demod_state, config_done,dsp_reset);
	
	/*Demod passing I2C commands between host and tuner. 0:disable 1:enable.*/	
	i2c_bybass_state = ATBMRead(0x01, 0x03); 	
   	CAN_Tuner = ATBMRead(0x00, 0x01);	
	printf("bypass state:0x%x; CAN Tuner used(silicon tuner=0):0x%x\n", i2c_bybass_state, CAN_Tuner);
	   	
	ADC_config = ATBMRead(0x06, 0x02);	
	LNA_on_off = ATBMRead(0x10, 0x30);
	printf("For silicon tuner ADC_config: 0x%x; LNA_on_off: 0x%x\n", ADC_config, LNA_on_off);
	
   	freq_offset = ATBMRead(0x08, 0x00);	
	printf("freq_offset 0x08_0x00: 0x%x\n", freq_offset);
	freq_offset = ATBMRead(0x08, 0x01);	
	printf("freq_offset 0x08_0x01: 0x%x\n", freq_offset);
    
	u8RXFLT[0] = ATBMRead(0x0A, 0x00);
	u8RXFLT[1] = ATBMRead(0x0A, 0x01);
	u8RXFLT[2] = ATBMRead(0x0A, 0x02);
	u8RXFLT[3] = ATBMRead(0x0A, 0x03);
	u8RXFLT[4] = ATBMRead(0x0A, 0x04);
	u8RXFLT[5] = ATBMRead(0x0A, 0x05);
	printf("IF freq_lif: 0x%x 0x%x  0x%x\n", u8RXFLT[0], u8RXFLT[1], u8RXFLT[2]);
	printf("ADC rate ratio: 0x%x 0x%x  0x%x\n", u8RXFLT[3], u8RXFLT[4], u8RXFLT[5]);    	
	
	fatal_rst = ATBMRead(0x00, 0x09);	
	weak_rst = ATBMRead(0x00, 0x0a);
	printf("DSP reset options fatal_rst = 0x%x, weak_rst =  0x%x\n", fatal_rst, weak_rst);

	SamplingEdegeMode = ATBMRead(0x03, 0x01);/*0x00: rising edge TS output; 0x01: falling edge TS output*/	  	                                                                                                                          
	SerialEnabled = ATBMRead(0x03, 0x05);    /*0: disable serial mode; 1: enable serial mode */
	ClockOutSelect = ATBMRead(0x03, 0x0b);   /*0: SPI clock output; 1: TS serial clock output */  	

	printf("TS interface SerialEnabled= %d SamplingEdegeMode =%d, clock select ClockOutSelect%d\n",\
		SerialEnabled,SamplingEdegeMode, ClockOutSelect);		    
  
}

/****************************************************************************
Function:    ATBMWriteRegArray
Parameters:  *reg_table, table_len
Return:      none

Description:
      This function writes a group of registers to demodulator	 	 
 ****************************************************************************/
void ATBMWriteRegArray(u_int8 *reg_table, u_int32 table_len)
{
   u_int32 i;
   for(i=0;i<table_len;i+=3)
   {
         ATBMWrite(reg_table[i],reg_table[i+1],reg_table[i+2]);
         //printf("%x %x %x\n", reg_table[i],reg_table[i+1], reg_table[i+2]); 		
   }
}


/****************************************************************************
Function:    ATBMDebugRegArray
Parameters:  *reg_table, table_len
Return:      none

Description:
     This function reads a group of registers from demodulator and prints them
     for debug purposes.	 
 ****************************************************************************/
void ATBMDebugRegArray(u_int8 *reg_table, u_int32 table_len)
{
   u_int32 i;
   u_int8 reg_data;
   for(i=0;i<table_len;i+=3)
   {
        reg_data = ATBMRead(reg_table[i],reg_table[i+1]);
		
        printf("%x %x  %x\n", reg_table[i], reg_table[i+1], reg_data); 		
   }
}
/****************************************************************************
Function:    ATBMRead
Parameters:  base_address, register_address
Return:      reg_data[0]

Description: 
     ATBM885x demodulator register Reading API, decoder platform independent. 
     It calls I2Cwrite and i2CRead functions that should be implemented by 
     customers based on customers' platforms.

****************************************************************************/
u_int8  ATBMRead(u_int8 base_addr, u_int8 register_addr)
{ 
	//int retval = 0; 
	u_int8 addr_data[2];
	u_int8 reg_data[1];

	addr_data[0] = base_addr;
	addr_data[1] = register_addr;
    
    if(!MDrv_IIC_ReadBytes(ATBMSlaveAddr, 2, addr_data, 1, reg_data))
     {
         printf("i2c read error!\n");
     }
#if 0
	retval = I2CWrite(ATBMSlaveAddr, addr_data, 2);
	if(!retval)
	{
		//Release mutex lock.
		return 0;
	}

	retval = I2CRead(ATBMSlaveAddr, reg_data, 1);
	if(!retval)
	{
		//Release mutex lock.
		return 0;
	}
#endif
	return(reg_data[0]);
}

/****************************************************************************
Function:        ATBMWrite
Parameters:  base_address, register_address, data
Return:      1:success, 0:fail  

Description: 
     ATBM885x demodulator register Writing API, decoder platform independent. 
     It calls I2Cwrite and i2CRead functions that should be implemented by 
     customers based on customers' platforms.
 
****************************************************************************/
int  ATBMWrite(u_int8 base_addr, u_int8 register_addr, u_int8 data)
{ 
	int retval = 0; 
	u_int8 addr_data[2];
	//u_int8 reg_data[1];
	//Add mutex lock here if there is multi-thread operation of demodulator's I2C
	//the following I2C operations will not be interrupted. 
	addr_data[0] = base_addr;
	addr_data[1] = register_addr;

	if(MDrv_IIC_WriteBytes( ATBMSlaveAddr, 0, NULL, 2, addr_data))
	{
		retval = 1;
	}
	else
	{
		retval = 0;
		printf("i2c write error!\n");
	}
	if(MDrv_IIC_WriteBytes(ATBMSlaveAddr, 0, NULL, 1, &data))
	{
		retval = retval&1;
	}
	else
	{
		printf("i2c write error!\n");

		retval = 0;
	}

#if 0
	retval = I2CWrite(ATBMSlaveAddr, addr_data, 2);
	if(!retval)
	{
		//Release mutex lock.
		return 0;
	}
	reg_data[0] = data;
	retval = I2CWrite(ATBMSlaveAddr, reg_data, 1);
	if(!retval)
	{
		//Release mutex lock.
		return 0;
	} 	
	//Release mutex lock
	printf("%x %x %x\n", base_addr, register_addr, data);		
 #endif
	return(retval);
}


/****************************************************************************

   The following are APIs users need to implement based on their platform.
                
****************************************************************************/
extern int I2CWrite(u_int8 I2CSlaveAddr, u_int8 *data, int length)
 {
   /* I2C write, please port this function*/
   if(MDrv_IIC_WriteBytes(I2CSlaveAddr, 0, NULL, length, data))
   //if(MDrv_IIC_Write(I2CSlaveAddr,NULL,0,data,length))
      return 1;
   else
   {
       printf("i2c write error!\n");

      return 0;
   }

 }
 
extern int  I2CRead(u_int8 I2CSlaveAddr, u_int8 *data, int length)
 {
     /* I2C read, please port this function*/
     if(MDrv_IIC_ReadBytes(I2CSlaveAddr, 0, NULL, length, data))
     //if(MDrv_IIC_Read(I2CSlaveAddr,NULL,0,data,length))
        return 1;
     else
     {
         printf("i2c read error!\n");

        return 0;
     }

 }


extern void DemodHardwareReset(void) {

    //extern void MApi_Demod_HWReset(MS_BOOL bReset);
    //printf("---> Hw Rest for DEMOD \n");
    
    mdrv_gpio_set_low(6);
    Delayms(80);
    mdrv_gpio_set_high(6);
     Delayms(400);
}


extern void Delayms (int ms) {

 MsOS_DelayTask(ms);

}

#endif

