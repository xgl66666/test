/*****************************************************************************
* Tuner sample code
*
* History:
* Date Athor Version Reason
* ============ ============= ========= =================
* 1.Apr.29.2010 Version1.0
*****************************************************************************/
#include <common.h>
#include <frontend_config.h>


#if (FRONTEND_TUNER_TYPE==TUNER_AV2012)
//#include "drvTuner_AV2012.h"
#include <common.h>
#include <sys/common/MsTypes.h>
#include "sys/common/MsOS.h"
#include <sys/drv/drvIIC.h>
#include "device/drvTuner.h"
#include <frontend_config.h>

#define  TUNER_AV2012_SLAVE_ID      0xC0//0xC6
#define  TUNER_CRYSTAL_FREQ         27
static MS_U8 _u8SlaveID = TUNER_AV2012_SLAVE_ID;


MS_U8 TunerInitialSetting[2][42]=
{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x38, 0x00, 0x00, 0x50, 0x1F, 0xA3, 0xFD, 0x00, 0x0E, 0x82, 0x88, 0xB4, 0x96, 0x40, 0x5B,0x6A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80,0x00, 0xF5, 0x7F, 0x4A, 0x9B, 0xE0, 0xE0, 0x36, 0x02, 0xAB, 0x97, 0xC5, 0xA8}
};

MS_BOOL MDrv_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    return MDrv_IIC_WriteBytes(u16BusNumSlaveID,AddrCnt,pu8addr,u16size,pBuf);
}

MS_BOOL MDrv_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    return MDrv_IIC_ReadBytes(u16BusNumSlaveID,AddrCnt,pu8addr,u16size,pBuf);

}

MS_BOOL AV2012_WriteReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Value[2];
    u8Value[0]=u8Addr;
    u8Value[1]=u8Data;
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, u8Value, 2);
    return bRet;
}
MS_BOOL AV2012_ReadReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data)
{
    MS_BOOL bRet=TRUE;
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, &u8Addr, 1);
    bRet&=MDrv_IIC_Read(u8SlaveID, 0, 0, u8Data, 1);
    return bRet;
}

void AV2012_SlaveID_Check(void)
{
     MS_U8 regValue;
     
      _u8SlaveID = 0xC0;
      do
      {
          regValue=(char) (0x38);
          if(AV2012_WriteReg(_u8SlaveID,0,regValue))
          {
               regValue = 0;
               if(AV2012_ReadReg(_u8SlaveID,0,&regValue))
               {
                     if(regValue == 0x38)
                     {
                         break;
                     }
               }
          }
          _u8SlaveID += 0x02;
      }while(_u8SlaveID <= 0xC6);
      if(_u8SlaveID > 0xC6)
      {
           _u8SlaveID = TUNER_AV2012_SLAVE_ID;
      }
}

MS_BOOL MDrv_Tuner_Initial(void)
{
    MS_BOOL bRet=TRUE;
    MS_U8 index;

    AV2012_SlaveID_Check();
    
    for (index=0; index < 12; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][12], TunerInitialSetting[1][12]);
    MsOS_DelayTask(100);
    for (index=0; index < 12; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][12], TunerInitialSetting[1][12]);
    MsOS_DelayTask(50);
    return bRet;
}

MS_BOOL MDrv_Tuner_SetFreq(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bRet=TRUE;
    MS_BOOL bAutoScan=FALSE;
    MS_U32 u32FracN;
    MS_U32 BW;
    MS_U32 BF;
    MS_U8 u8Reg[7];
    printf("\n%s=======herere\n",__func__);
    if (u32SymbolRate_Hz == 0 || u32SymbolRate_Hz == 45000) //auto-scan mode
    {
        printf("\nsybrate=%d\n",u32SymbolRate_Hz);
	    bAutoScan = TRUE;
    }
    MsOS_DelayTask(50);
    u32FracN = (u16CenterFreq + TUNER_CRYSTAL_FREQ/2)/TUNER_CRYSTAL_FREQ;
    if(u32FracN > 0xff)
    {
   	   u32FracN = 0xff;
    }
    u8Reg[0]=(MS_U8) (u32FracN & 0xff);
    u32FracN = (u16CenterFreq<<17)/TUNER_CRYSTAL_FREQ;
    u32FracN = (u32FracN & 0x1ffff);
    u8Reg[1]=(MS_U8) ((u32FracN>>9)&0xff);
    u8Reg[2]=(MS_U8) ((u32FracN>>1)&0xff);
    u8Reg[3]=(MS_U8) (((u32FracN<<7)&0x80) | 0x50); // default is 0x50
    // Channel Filter Bandwidth Setting.
    if(bAutoScan==TRUE)//requested by BB
    {
        u8Reg[5] = 0xA3; //BW=27MHz
    }
    else
    {
   	    // rolloff is 35%
   	    BW = u32SymbolRate_Hz*135/200;
   	    // monsen 20080726, BB request low IF when sym < 6.5MHz
   	    // add 6M when Rs<6.5M,
   	    if(u32SymbolRate_Hz<6500)
   	    {
   	    	BW = BW + 6000;
   	    }
   	    	// add 2M for LNB frequency shifting
   	    BW = BW + 2000;
   	    	// add 8% margin since fc is not very accurate
   	    BW = BW*108/100;
   	    	// Bandwidth can be tuned from 4M to 40M
   	    if( BW< 4000)
   	    {
   	    	BW = 4000;
   	    }
   	    if( BW> 40000)
   	    {
   	    	BW = 40000;
   	    }
   	    BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
   	    u8Reg[5] = (MS_U8)BF;
    }
    // Sequence 4
    // Send Reg0 ->Reg4
    MsOS_DelayTask(5);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x00, u8Reg[0]);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x01, u8Reg[1]);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x02, u8Reg[2]);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x03, u8Reg[3]);
    MsOS_DelayTask(5);
    // Sequence 5
    // Send Reg5
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x05, u8Reg[5]);
    MsOS_DelayTask(5);
    // Fine-tune Function Control
    //Tuner fine-tune gain function block. bit2.
    //not auto-scan case. enable block function. FT_block=1, FT_EN=1
    if (bAutoScan==FALSE)
    {
         u8Reg[6] = 0x06;
         bRet&=AV2012_WriteReg(_u8SlaveID, 0x25, u8Reg[6]);
         MsOS_DelayTask(5);
         //Disable RFLP at Lock Channel sequence after reg[37]
         //RFLP=OFF at Lock Channel sequence
         // RFLP can be Turned OFF, only at Receving mode.
	u8Reg[7] = 0xD6;
         bRet&=AV2012_WriteReg(_u8SlaveID, 0x0C, u8Reg[7]);
    }
    return bRet;
}
MS_BOOL MDrv_Tuner_CheckLock(void)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;

    bRet&=AV2012_ReadReg(_u8SlaveID, 0x0B, &u8Data);
    if (bRet==FALSE)
    {
        return bRet;
    }
    else
    {
        if ((u8Data&0x03)!=0x03)
        {
            bRet=FALSE;
        }
    }
    //printf("Tuner Status 0x%x\n", u8Data);
    return bRet;
}
#endif
