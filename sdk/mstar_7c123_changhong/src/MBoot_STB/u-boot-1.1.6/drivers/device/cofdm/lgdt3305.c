#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <msAPI_Tuner.h>

#if(FRONTEND_DEMOD_TYPE ==LGDT3305_DEMOD)
#include <device/lgdt3305.h>

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

static MS_BOOL                      bInited     = FALSE;

#define COFDMDMD_STACK_SIZE          (2048)
#define COFDMDMD_MUTEX_TIMEOUT       (2000)
#define COFDMDMD_AUTO_TUN_MAX        (2 * 5)
#define COFDMDMD_AUTO_TUN_RETRY_MAX  (2)

#define DBG_MSB(x) // x

#define DEMOD_TYPE 3 // Digital Demodulator. Parameter  0:64QAM, 1:256QAM, 3:VSB
#define MDrv_IIC_Read(a, b, c, d, e) MDrv_IIC_ReadBytes(a, c, b, e, d)
#define MDrv_IIC_Write(a, b, c, d, e) MDrv_IIC_WriteBytes(a, c, b, e, d)

//unsigned int ChipAddress;
/////////////////////////////////////////
//   STATUS CHECK
//
float Log10Approx(float flt_x)
    {
        MS_U32 u32_temp = 1;
        MS_U8 indx = 0;
        do
        {
            u32_temp = u32_temp << 1;
            if (flt_x < (float)u32_temp)
                break;
        }
        while (++indx < 32);
        // 10*log10(X) ~= 0.3*N, when X ~= 2^N
        return (float)0.3 *indx;
    }

int lgdt3305Carriercheck(int _mode)
{
	int carrerData=0;//, syncData=0, rdData=0, paErr=0;
	int _lockValue=0;

	if(_mode!=3)
	{
		carrerData = RegGetOneRegister(0x011d) & 0x02;   // QAM CARRIER LOCK
		if(carrerData)	 _lockValue=1;                     // after 2nd lock status
		else			 _lockValue=0;
	}
	else
	{
		carrerData = RegGetOneRegister(0x011d) & 0x80;   // VSB CARRIER LOCK
		if(carrerData)	 _lockValue=1;
		else 			 _lockValue=0;
	}
	return _lockValue;
}

int lgdt3305inlockcheck()                             // nSYNCLOCK  0: LOCK // 1: UNLOCK
{
	int syncData=0;
	syncData =(RegGetOneRegister(0x0003)&0x08)>>3;
	return syncData;
}
int lgdt3305Errorcheck()
{
	int cpaErr=0;
	cpaErr =(RegGetOneRegister(0x0003)&0x02)>>1;
	return cpaErr;
}


int lgdt3305trlockcheck()                             // nSYNCLOCK  0: LOCK // 1: UNLOCK
{
	int syncData=0;
//yncData =(RegGetOneRegister(0x021B)&0x06);
	syncData =RegGetOneRegister(0x021B);
// printf("lgdt3305trlockcheck 0x%x\n",syncData);
	syncData =(syncData &0x06);
	return syncData;
}

///////////////////////////////////////////////
// MPEG OUT MODE SET
//
void lgdt3305Mpegout(int _mode)
{
	int rdData=0;

	if(_mode==0)
	{
		rdData=RegGetOneRegister(0x050e);
		rdData=(rdData&~0x20)|0x20;
		RegSetOneRegister(0x050e, rdData);
	}
	else
	{
		rdData=RegGetOneRegister(0x050e);
		rdData=(rdData&~0x20)|0x00;
		RegSetOneRegister(0x050e, rdData);
	}

	RegSetOneRegister(0x0002, 0x08);
	RegSetOneRegister(0x0002, 0x09);
}


/////////////////////////////////////////
// MPEG CLOCK SET
//
void lgdt3305Mpegclk(int _mode)
{
	int rdData=0;

	if(_mode==0)
	{
		rdData=RegGetOneRegister(0x050e);
		rdData=(rdData&~0x80)|0x00;
		RegSetOneRegister(0x050e, rdData);
	}
	else
	{
		rdData=RegGetOneRegister(0x050e);
		rdData=(rdData&~0x80)|0x80;
		RegSetOneRegister(0x050e, rdData);
	}
	RegSetOneRegister(0x0002, 0x08);
	RegSetOneRegister(0x0002, 0x09);
}

///////////////////////////////////////////
// MPEG VALID SET
//
void lgdt3305Mpegval(int _mode)
{
	int rdData=0;

	if(_mode==0)
	{
		rdData=RegGetOneRegister(0x050e);
		rdData=(rdData&~0x01)|0x00;
		RegSetOneRegister(0x050e, rdData);
	}
	else
	{
		rdData=RegGetOneRegister(0x050e);
		rdData=(rdData&~0x01)|0x01;
		RegSetOneRegister(0x050e, rdData);
	}

	RegSetOneRegister(0x0002, 0x08);
	RegSetOneRegister(0x0002, 0x09);
}

///////////////////////////////////////////
// MPEG VALID SET
//
void lgdt3305Mpegs(void)
{
    int rdData=0;

    if (!TS_PARALLEL_OUTPUT)
    {
        rdData=0xF9;  // serial TS
    }
    else
    {
        rdData=0xD9;  
    }
    RegSetOneRegister(0x050e, rdData);

    RegSetOneRegister(0x0002, 0x08);
    RegSetOneRegister(0x0002, 0x09); 
}

///////////////////////////////////////////////////
//  SNR Check
//
float lgdt3305_SignalStatus(int _mode)
{
	float			snr=0;
	unsigned int	data_p;
	unsigned long  	data = 0;
	unsigned long 	dataSignalPower = 0;

	if(_mode==3)
	{
		data_p = RegGetOneRegister(0x0417);
		data = data_p & 0x0F;
		data <<= 16;
		data_p = RegGetOneRegister(0x0418);
		data |= data_p << 8;
		data_p = RegGetOneRegister(0x0419);
		data |= data_p;

		if ((data < 0x0FFFFF) && (data > 0))
		{
			//snr = 10 * log10(double((25 * 32 * 32) / data ));
			snr = 10 * Log10Approx((float)((25 * 32 * 32) / data ));
		}
		else
			snr = 0;

		if (snr < 0)
			snr = 0;
	}
	else
	{
			//----- Assgin Ps(Signal Power) based upon the mode -----//

		if ( ( RegGetOneRegister(0x050A) & 0x0C ) == 0x0C) {
			if (_mode == 0)	dataSignalPower = 688128;
			else if (_mode == 1) dataSignalPower = 696320;
				//----- End of assigning Ps -----//
				data_p = RegGetOneRegister(0x011B);
				data =  data_p << 8;
				data_p = RegGetOneRegister(0x011C);
				data |= data_p;
				if ((data != 0xFFFF) && (data !=0))
				{
					//snr = 10 * log10(double(dataSignalPower / data));					
					snr = 10 * Log10Approx((float)(dataSignalPower / data));
				}
				else snr = 0;
			}
		else snr =0;
		if (snr < 0) snr = 0;
	} 
	return (float)snr;
}



int lgdt3305_MpegError()
{
	int error=0;
	int	data_p;
	int	data = 0;

	data_p = RegGetOneRegister(0x050C);
	data = data_p << 8;
	data_p = RegGetOneRegister(0x050D);
	data |= data_p;
	error = data;
	return error;
}

void lgdt3305QamSpecInv(int _mode)
{
	int rdData=0;

	if(_mode==0)
	{
		rdData=RegGetOneRegister(0x0504);
		rdData=(rdData&~0x02)|0x00;
		RegSetOneRegister(0x0504, rdData);
	}
	else
	{
		rdData=RegGetOneRegister(0x0504);
		rdData=(rdData&~0x02)|0x02;
		RegSetOneRegister(0x0504, rdData);
	}

	RegSetOneRegister(0x0002, 0x08);
	RegSetOneRegister(0x0002, 0x09);
}


double lgdt3305_Ber(){

	unsigned char Data;
	unsigned int err;
	unsigned int nPeriod;

	/*----- Checks BERTPERD							-----*/
	Data = RegGetOneRegister(0x0801);
	Data &= 0x1F;

	/*----- Period = 2^BERTPERD						-----*/
	nPeriod = 1<<Data;

	/*----- Reads 4 registers						-----*/
	err =RegGetOneRegister(0x080A) <<24 | RegGetOneRegister(0x080B)<<16 | RegGetOneRegister(0x080C) <<8 |RegGetOneRegister(0x080D) ;

	return err/nPeriod;
}


void Lgdt3305ResetBER(void)
{
	unsigned char	Data;

	Data = RegGetOneRegister(0x0800);
	Data &= 0xFE;
	RegSetOneRegister(0x0800, (Data|0x01) );
	MsOS_DelayTask(20);//wait(20);
	RegSetOneRegister(0x0800, Data);
}

int lgdt3305_IfAcc(void)
{
	int if_acc=0;
	int	data_p;

	data_p = RegGetOneRegister(0x0318)&0x0F;
	if_acc = data_p << 8;
	data_p = RegGetOneRegister(0x0319);
	if_acc |= data_p;
 
 //  printf("PWR = 0x%x \n",if_acc);
	return if_acc;
}


void lgdt3305Init(int _mode){

	if(_mode==2){


		RegSetOneRegister(0x0002, RegGetOneRegister(0x0002)&0xFE);
		RegSetOneRegister(0x000A, (RegGetOneRegister(0x000A)&0xEF) | 0x00 );
		RegSetOneRegister(0x000B, (RegGetOneRegister(0x000B)&0xEF) | 0x00 );

	}
	else{

		RegSetOneRegister(0x000A, (RegGetOneRegister(0x000A)&0xEF) | 0x10 );
		RegSetOneRegister(0x000B, (RegGetOneRegister(0x000B)&0xEF) | 0x10 );
	}


	if(_mode==3){
		//	1) OPERMODE[1:0] <-- '11'b
		RegSetOneRegister(0x0000, (RegGetOneRegister(0x0000) & 0xFC)| 0x03);
		//	2) DAGCREF[15:0] <-- 0x32C4 (@0x0012,0x0013)
		RegSetOneRegister(0x0012, 0x32);
		RegSetOneRegister(0x0013, 0xC4);
		// 3) LOCKDTEN  <- '0' (@0x0314[1])
		RegSetOneRegister(0x0314, 0xE1);
		// 4) IFBW[15:0] <-- 0x8000 (@0x0308,0x0309)
		RegSetOneRegister(0x0308, 0x80);
		RegSetOneRegister(0x0309, 0x00);
		// 5) ACQEN <- '0'b (@0x030C[2])
		RegSetOneRegister(0x030C, 0x30);
		// 6) QAMFBEXT <- '0'b (@0x043F[2])
		RegSetOneRegister(0x043F, 0xAA);
		//#ifdef	VSB_PLL_SWEEP
		//	RegSetOneRegister(0x012A, 0xC9);
		//#endif
		// 7) GCONCIR <- '0'b (@0x0116[7]
		RegSetOneRegister(0x0116, RegGetOneRegister(0x0116)& 0x7F);
		// 8) Reset
		RegSetOneRegister(0x0002, RegGetOneRegister(0x0002) & 0xFE);
		MsOS_DelayTask(20);       // reset delay
		RegSetOneRegister(0x0002, (RegGetOneRegister(0x0002) & 0xFE) | 0x01);

	}
	else if( _mode==0 || _mode==1 ){

		if(_mode==0){
			//	1) OPERMODE[1:0]
			RegSetOneRegister(0x0000, RegGetOneRegister(0x0000) & 0xFC);
			//	2) DAGCREF[15:0] <-- 0x2A00 (@0x0012,0x0013)
			RegSetOneRegister(0x0012, 0x2A);
			RegSetOneRegister(0x0013, 0x00);
		}
		else{
			RegSetOneRegister(0x0000, (RegGetOneRegister(0x0000) & 0xFC)|0x01);
			RegSetOneRegister(0x0012, 0x2A);
			RegSetOneRegister(0x0013, 0x80);
		}

		// 3) LOCKDTEN  <- '1' (@0x0314[1])
		RegSetOneRegister(0x0314, 0xE3);
		//  4) IFBW[15:0] <-- 0x8888 (@0x0308,0x0309)
		RegSetOneRegister(0x0308, 0x88);
		RegSetOneRegister(0x0309, 0x88);
		//  5) ACQEN <- '1'b (@0x030C[2])
		RegSetOneRegister(0x030C, 0x34);
		// 6) QAMFBEXT <- '1'b (@0x043F[2])
		RegSetOneRegister(0x043F, 0xAE);
		// 6) reset
		RegSetOneRegister(0x0002, RegGetOneRegister(0x0002) & 0xFE);
		MsOS_DelayTask(20);       // reset delay
		RegSetOneRegister(0x0002, (RegGetOneRegister(0x0002) & 0xFE) | 0x01);
	}

}

    
BOOLEAN IIC_Bypass_Mode(BOOLEAN enable)
{
    unsigned char   nData;

    // Add 1ms delay by j.y. won : 2007.08.21 (due to tuner)
    
    MsOS_DelayTask(3);//wait(1);
    nData = RegGetOneRegister(0x0001);
    nData &= 0xDF;

    if (enable == 1)  /*----- I2C Repeater Turn ON	-----*/
    {
        RegSetOneRegister(0x0001, nData);
    }
    else        /*----- I2C Repeater Turn OFF	-----*/
    {
        nData |= 0x20;
        RegSetOneRegister(0x0001, nData);
    }
    MsOS_DelayTask(3);//wait(1);
     return TRUE;
}

void RegSetOneRegister(int reg_id, unsigned char _Data)
{
	unsigned char data[3];//,UCtmp=-1;

	data[0] = (unsigned char)((reg_id>>8)&0xff);
	data[1] = (unsigned char)(reg_id&0xff);
	data[2] = _Data;
	//I2CWrite(ChipAddress,&data[0],3);
	if (MDrv_IIC_Write(LGIT_3305_DEMOD_I2C_ADDR, 0, 0, &data[0],3) == FALSE)
	{
		//HB_printf("%s: LGDT3305 write fail\n", __FUNCTION__);
	}
}
int RegGetOneRegister(int reg_id)
{
	unsigned char    readvalue[3]     ;

	readvalue[0]=(unsigned char)((reg_id>>8)&0xff);
	readvalue[1]=(unsigned char)(reg_id&0xff);
//	I2CWrite(ChipAddress,&readvalue[0],2);
//	I2CRead(ChipAddress,&readvalue[2],1);
	if (MDrv_IIC_Write(LGIT_3305_DEMOD_I2C_ADDR, 0, 0, &readvalue[0],2) == FALSE)
	{
			//HB_printf("%s: LGDT3305 write fail\n", __FUNCTION__);
			return 0;
	}
	
	if (MDrv_IIC_Read(LGIT_3305_DEMOD_I2C_ADDR,0,0,&readvalue[2],1)==FALSE)
	{
		//	HB_printf("%s: LGDT3305 read fail\n", __FUNCTION__);
			return 0;
	}
	
	return(readvalue[2]) ;
}

void Run3305(int system)
{
//	int rdData = 0;
	RegSetOneRegister(0x0002, RegGetOneRegister(0x0002)&0xFE);
	MsOS_DelayTask(20);   // Reset Delay
	RegSetOneRegister(0x0002, RegGetOneRegister(0x0002)|0x01);

	/////////////////////////////////////////////////////////////
/*	rdData=RegGetOneRegister(0x001);	// I2C RPT OFF
	rdData=(rdData&~0x20)|0x20;
	RegSetOneRegister(0x001, rdData); */

	lgdt3305Init(system);

	if(system == 0 || system == 1)
		lgdt3305QamSpecInv(1); //0 : Not inverted 1: Inverted

	RegSetOneRegister(0x0002, RegGetOneRegister(0x0002)&0xFE);
	MsOS_DelayTask(20);   // Reset Delay
	RegSetOneRegister(0x0002, RegGetOneRegister(0x0002)|0x01);

}

int CheckStatus(int system)
{
	double snr, ber;
	int ubc;

	snr=0;
	ber=0;
	ubc=0;

	if(2==system) system=3;

	if(lgdt3305Carriercheck(system))
	{

	}
	else
	{   
		return 0;
	}

	if(!lgdt3305inlockcheck())	
    {
      //  HB_printf("nSync Locked\n");
	}
	else
	{  
	  //  HB_printf("nSync UnLocked\n");		
		return 0;
	}

	if(lgdt3305Errorcheck())
	{
	 //   HB_printf("No Error\n");
	}
	else
	{
	  //  HB_printf("Error\n");		
		return 0;
	}

// signal check if need
#if 0
	snr=lgdt3305_SignalStatus(system);

	HB_printf("SNR = %5.3f dB\n",snr);

	ubc=lgdt3305_MpegError();
	HB_printf("UBC = %d\n",ubc);

	Lgdt3305ResetBER();
	ber= lgdt3305_Ber();

	if(ber> 0.0002)
	{
		HB_printf("Good BER\n");
	}
	else
	{
		HB_printf("BAD BER\n");
	}

	HB_printf("BER = %.3e\n", ber);
#endif

	if(lgdt3305trlockcheck())
	{
		return 1;
	}
	else {
		return 0;
	}
	return 0;
}

MS_BOOL MDrv_Demod_Init(void)
{

    bInited = FALSE;
    DBG_MSB(printf("-----%s start-----\n", __FUNCTION__));
 
     lgdt3305Mpegs();

    bInited = TRUE;
    DBG_MSB(HB_printf("-----%s done-----\n", __FUNCTION__));
    return TRUE;

}

MS_BOOL MDrv_Demod_GetLockStatus(void)
{
    return CheckStatus(DEMOD_TYPE);  //Check LGDT3305 Status. Parameter  0:64QAM, 1:256QAM, 3:VSB
}


MS_BOOL MDrv_Demod_Reset(void)
{

    Run3305(DEMOD_TYPE); //RUN Digital Demodulator. Parameter  0:64QAM, 1:256QAM, 3:VSB

    return TRUE;
}
#endif
