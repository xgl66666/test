/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_SerdesApi.c
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 11/19/2011
 *
 * DESCRIPTION        : Implementation file for MxL_HRCLS SerDes APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL_HRCLS_Common.h"

#ifdef _MXL_HRCLS_SERDES_ENABLED_
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_PhyCtrl.h"

#define MXL_HRCLS_SERDES_CHANTUNE_TIMEOUT_MS 100
#define MXL_HRCLS_SERDES_DEFAULT_TUNE_BANDWIDTH 6
#define MXL_HRCLS_SERDES_DEFAULT_TUNE_FREQUENCY_HZ 666000000

typedef struct
{
  UINT8 d2a_tx_slice_pch_nch_en;
  UINT8 d2a_rx_term;
} RCAL_LOOKUP_TABLE;

#ifdef _MXL_HRCLS_SERDES_USING_RCAL_LUT_FOR_SETTING_IO_TERMINATION_
static RCAL_LOOKUP_TABLE serdesRCalLookupTable[MXL_HRCLS_SERDES_RCAL_CODE_MAX] = 
{
  {0x18, 0x0},
  {0x17, 0x1},
  {0x16, 0x2},
  {0x16, 0x2},
  {0x15, 0x3},
  {0x15, 0x3},
  {0x14, 0x4},
  {0x14, 0x4},
  {0x13, 0x4},
  {0x13, 0x5},
  {0x12, 0x5},
  {0x12, 0x5},
  {0x11, 0x6},
  {0x11, 0x6},
  {0x11, 0x6},
  {0x10, 0x7}
};
#endif

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_UpdateSerdesRegisterField
--| 
--| AUTHOR        : Sailesh Kandula 
--|
--| DATE CREATED  : 10/20/2011
--|                  
--| DESCRIPTION   : Reads and updates field 
--|                 
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE if error
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serdesId, 
                                                            UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, 
                                                            UINT16 newValue)
{
  regAddr = (serdesId == MXL_HRCLS_SERDES_LANE1) ? (regAddr + 0x800) : regAddr;
  return MxL_HRCLS_Ctrl_UpdateRegisterField(devId, regAddr, lsbPos, fieldWidth, newValue);
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Toggle field value -> 1 -> 0  (for Serdes)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serdesId, 
                                                                  UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
   regAddr = (serdesId == MXL_HRCLS_SERDES_LANE1) ? (regAddr + 0x800) : regAddr;
   return(MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero( devId, regAddr, bitLocation, bitWidth));
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 12/29/2011
--|
--| DESCRIPTION   : Toggle field value -> 1 -> 0  (for Serdes)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serdesId, 
                                                                  UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth)
{
   regAddr = (serdesId == MXL_HRCLS_SERDES_LANE1) ? (regAddr + 0x800) : regAddr;
   return(MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne( devId, regAddr, bitLocation, bitWidth));
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SerdesTuneSlot2
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 03/15/2012
--|
--| DESCRIPTION   : Tune DFE channel that goes to slot 2 of serdes 
--| 
--| NOTE: 5/06/2014: This sequence can be used on the other Serdes types as well.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_SerdesTuneSlot2(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  MXL_STATUS_E status;
  MXL_HRCLS_RX_PWR_ACCURACY_E accuracy;
  
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("SerDesId=%d, DFE channel=%d\n", serDesId, devContextPtr->serDesSlot2Chan[(UINT8) serDesId]););
  status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], NULL, &accuracy);
  if ((status == MXL_SUCCESS) && (accuracy == MXL_HRCLS_PWR_INVALID))
  {
    UINT64 timeNow, timeout;
    status = MxL_HRCLS_Ctrl_CfgTunerChanTune(devContextPtr, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], 
                              MXL_HRCLS_SERDES_DEFAULT_TUNE_BANDWIDTH,
                              MXL_HRCLS_SERDES_DEFAULT_TUNE_FREQUENCY_HZ);
    if (status == MXL_SUCCESS)
    {
      MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
      timeout = timeNow + ((UINT64) MXL_HRCLS_SERDES_CHANTUNE_TIMEOUT_MS * 1000);
      do
      {
        MxLWare_HRCLS_OEM_DelayUsec(10*1000); /* Average channel tunning time */ 
        status = MxL_HRCLS_Ctrl_ReqTunerRxPwr(devContextPtr, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], NULL, &accuracy);
        MxLWare_HRCLS_OEM_GetCurrTimeInUsec(&timeNow);
      } while ((accuracy == MXL_HRCLS_PWR_INVALID) && (status == MXL_SUCCESS) && (timeNow < timeout));
      status = ((status == MXL_SUCCESS) && (accuracy != MXL_HRCLS_PWR_INVALID))?MXL_SUCCESS:MXL_FAILURE;
    }
    MXLERR(
    if (status != MXL_SUCCESS)
    {
      MxL_HRCLS_ERROR("%s. Cannot tune channel %d for serDes lane %d, slot 2 (status %d, accuracy %d, timeout %dms)\n", __FUNCTION__, 
                    devContextPtr->serDesSlot2Chan[(UINT8) serDesId], serDesId, status, accuracy,
                    MXL_HRCLS_SERDES_CHANTUNE_TIMEOUT_MS);
    }
    )
  }
MXLDBG2(  
  else
  {
    if (accuracy != MXL_HRCLS_PWR_INVALID)
    {
      MxL_HRCLS_DEBUG("DFE channel %d already tuned (state=%d). Skipping\n", devContextPtr->serDesSlot2Chan[(UINT8) serDesId], accuracy);
    }
  }
  )
  MXLEXITSTR(status);
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SerdesEnableClock
--| 
--| AUTHOR        : Mariusz Murawski
--|
--| DATE CREATED  : 03/15/2012
--|
--| DESCRIPTION   : Perform 2A sequence (PG).
--| 
--| NOTE: 5/06/2014: This sequence can be used on the other Serdes types as well.
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_SerdesEnableClock(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  UINT8 status = MXL_SUCCESS;
  UINT32 clockVal;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  status |= MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_SERDES0_CLK_ENA, 1);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DIG_ANA_SERDES1_CLK_ENA, 1);
  if (serDesId == MXL_HRCLS_SERDES_LANE0)
  {
    clockVal |= (UINT32) (((UINT32) 1 << 10) | ((UINT32) 1 << 18) | ((UINT32) 1 << 19));
  }
  else
  {
    clockVal |= (UINT32) (((UINT32) 1 << 10) | ((UINT32) 1 << 20) | ((UINT32) 1 << 21));
  }
  status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
  if (serDesId == MXL_HRCLS_SERDES_LANE0)
  {
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 1);
    status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DFE_USRESAMP_RESET);
  }
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_WBCLK_EN, 1);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_UPX_CLK_EN, 1);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_WBCLK_RESET, 0);
  status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_UPX_RESET, 0);

  if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3)
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_CLR_DP_NBAND_TITAN, 0);
  else
    status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_CLR_DP_NBAND, 0);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_InitPhySerdes
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 10/17/2014
--|
--| DESCRIPTION   : Perform PHY Initialize for Serdes Type 2/3.  This needs to be done ONLY ONCE.
--|
--| NOTE: This is used if serDesType == MXL_HRCLS_SERDES_TYPE_2 OR 
--|                               serDesType == MXL_HRCLS_SERDES_TYPE_3 only.  (MxL Serdes: Atlas/Titan)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_InitPhySerdes(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;
  UINT16 phy0_d2a_refsx_ena = 0, phy1_d2a_refsx_ena = 0;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, serDesType=%d\n", devId, devContextPtr->serDesType););

  if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
  {
    if ((MXL_SUCCESS == MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_REFSX_ENA, &phy0_d2a_refsx_ena)) &&
        (MXL_SUCCESS == MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_REFSX_ENA, &phy1_d2a_refsx_ena)) &&
        ((phy0_d2a_refsx_ena & 0x1) == 0) && ((phy1_d2a_refsx_ena & 0x1) == 0))
    {
      // PHY Init and should happen ONLY ONCE.
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_ENA, 1);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_ENA, 1);
			  
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_START_TUNE, 0);
		
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_START_TUNE, 0);
    } else MXLERR(MxL_HRCLS_DEBUG("EITHER Readback fail OR phyx_d2a_refsx_ena already set: %04x %04x\n", phy0_d2a_refsx_ena, phy1_d2a_refsx_ena););
  }
  else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3)
  {
    if ((MXL_SUCCESS == MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_REFSX_ENA_TITAN, &phy0_d2a_refsx_ena)) &&
        (MXL_SUCCESS == MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_REFSX_ENA_TITAN, &phy1_d2a_refsx_ena)) &&
        ((phy0_d2a_refsx_ena & 0x1) == 0) && ((phy1_d2a_refsx_ena & 0x1) == 0))
    {
      // PHY Init and should happen ONLY ONCE.
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_ENA_TITAN, 1);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_ENA_TITAN, 1);
  
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_START_TUNE_TITAN, 0);

      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_START_TUNE_TITAN, 0);
    } else MXLERR(MxL_HRCLS_DEBUG("EITHER Readback fail OR phyx_d2a_refsx_ena already set: %04x %04x\n", phy0_d2a_refsx_ena, phy1_d2a_refsx_ena););
  } else status = MXL_NOT_SUPPORTED;

  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_PowerdownSerdes
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 08/26/2014
--|
--| DESCRIPTION   : Perform Serdes power down section on Type 2/3
--|
--| NOTE: This is used if serDesType == MXL_HRCLS_SERDES_TYPE_2 or  
--|                                serDesType == MXL_HRCLS_SERDES_TYPE_3 only.  (MxL Serdes: Atlas/Titan)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

static MXL_STATUS_E MxL_HRCLS_Ctrl_PowerdownSerdes(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;
	
  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, serDesType=%d\n", devId, devContextPtr->serDesType););

  if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
  {
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_PI_RESETBAR, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_RESETBAR, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CNTR_RESETBAR, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_RESETBAR, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_RESETBAR, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CKGEN_RESETBAR, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CLK_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_EQ_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_SIGDET_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_COMP_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CLK_EN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_START_TUNE, 0);
  }
  else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3)
  {
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_PI_RESETBAR_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_RESETBAR_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CNTR_RESETBAR_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_RESETBAR_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_RESETBAR_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CKGEN_RESETBAR_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CLK_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_EQ_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_SIGDET_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_COMP_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CLK_EN_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA_TITAN, 0);
    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_START_TUNE_TITAN, 0);
  }
  else
    status = MXL_NOT_SUPPORTED;
  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SerdesOverwriteDefaults
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 05/01/2014
--|
--| DESCRIPTION   : Step 10: overwrite default values in registers of each serdes (PG)
--|
--| NOTE: This is used if serDesType == MXL_HRCLS_SERDES_TYPE_2/3 only.  (MxL Serdes: Atlas/Titan)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_SerdesOverwriteDefaults(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, serDesType=%d, serDesDSLanesCnt = %d\n", devId, devContextPtr->serDesType, devContextPtr->serDesDSLanesCnt););
  
  if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2 || devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3)
  {
#ifdef _MXL_HRCLS_SERDES_USING_TX_SLICE_NCH_EN_VALUE_
    UINT16 dataRb;
#endif
    
    if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
    {
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_LEAK_REG, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_SX_REG_AMP, 0x4);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CLKDIST_LEAK_REG, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_LEAK_REG, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_REG_AMP, 0x4);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_GAIN, 0x14);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_EXT_CMP_VTH_MAX, 0x7);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_EXT_CMP_VTH_MIN, 0x2);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_COMP_THRESH, 0x17);

#ifdef _MXL_HRCLS_SERDES_USING_TX_SLICE_NCH_EN_VALUE_
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN, &dataRb);
      else
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_SLICE_NCH_EN, &dataRb);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN, dataRb & 0x1F);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN, dataRb & 0x1F);   
#endif
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_TEMP_INFO, 0x2);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN, 0x0C);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN, 0x10);   
    }
    else
    {
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_LEAK_REG_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_SX_REG_AMP_TITAN, 0x4);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CLKDIST_LEAK_REG_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_LEAK_REG_TITAN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_REG_AMP_TITAN, 0x4);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_GAIN_TITAN, 0x14);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_EXT_CMP_VTH_MAX_TITAN, 0x7);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_EXT_CMP_VTH_MIN_TITAN, 0x2);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_COMP_THRESH_TITAN, 0x17);

#ifdef _MXL_HRCLS_SERDES_USING_TX_SLICE_NCH_EN_VALUE_
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN_TITAN, &dataRb);
      else
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_SLICE_NCH_EN_TITAN, &dataRb);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, dataRb & 0x1F);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, dataRb & 0x1F);   
#endif
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_TEMP_INFO_TITAN, 0x2);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, 0x0C);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, 0x10);   
    }
  }
  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_SetSerdesIOTermination
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 05/01/2014
--|
--| DESCRIPTION   : Step 9: input and output terminations based on stored R-CAL value and lookup table 
--|                           (Atlas_V1 Radio PG)
--|
--| NOTE: This is used if serDesType == MXL_HRCLS_SERDES_TYPE_2/3 only.  (MxL Serdes: Atlas/Titan)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxL_HRCLS_Ctrl_SetSerdesIOTermination(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_SERDES_LANE_ID_E serDesId, MXL_HRCLS_SERDES_RCAL_CODE_E rcalCode)
{
#ifdef _MXL_HRCLS_SERDES_USING_RCAL_LUT_FOR_SETTING_IO_TERMINATION_
  UINT8 txSliceVal, rxTerm;
#endif
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, rcalCode = %d\n", devId, rcalCode););

  if (rcalCode < MXL_HRCLS_SERDES_RCAL_CODE_MAX)
  {
#ifdef _MXL_HRCLS_SERDES_USING_RCAL_LUT_FOR_SETTING_IO_TERMINATION_
    txSliceVal = serdesRCalLookupTable[rcalCode].d2a_tx_slice_pch_nch_en;
    rxTerm = serdesRCalLookupTable[rcalCode].d2a_rx_term;
#endif
    if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
    {
#ifdef _MXL_HRCLS_SERDES_USING_RCAL_LUT_FOR_SETTING_IO_TERMINATION_
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_PCH_EN, txSliceVal);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN, txSliceVal);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM, rxTerm);
#endif
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_PCH_EN, 0x12);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN, 0x12);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM, 0x05);
    }
    else
    {
#ifdef _MXL_HRCLS_SERDES_USING_RCAL_LUT_FOR_SETTING_IO_TERMINATION_
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_PCH_EN_TITAN, txSliceVal);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN_TITAN, txSliceVal);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM_TITAN, rxTerm);
#endif
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_PCH_EN_TITAN, 0x12);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN_TITAN, 0x12);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM_TITAN, 0x05);
    }
  }
  else
    status = MXL_INVALID_PARAMETER;
    
  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

#ifdef _MXL_HRCLS_SERDES_ENABLE_CONFIG_IO_TERMINATION_
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerdesIOTermination
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 05/01/2014
--|
--| DESCRIPTION   : Step 9: input and output terminations based on stored R-CAL value and lookup table 
--|                           (Atlas_V1 Radio PG)
--|
--| NOTE: This is used if serDesType == MXL_HRCLS_SERDES_TYPE_2/3 only.  (MxL Serdes: Atlas/Titan)
--|
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerdesIOTermination(UINT8 devId,  MXL_HRCLS_SERDES_LANE_ID_E serDesId, MXL_HRCLS_SERDES_RCAL_CODE_E rcalCode)
{
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  UINT8 status = MXL_SUCCESS;
  UINT16 dataRb;
  
  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("devId = %d, rcalCode=%d\n", devId, rcalCode););

  MXL_HRCLS_SERDES_SWAP(serDesId);
  if (devContextPtr && (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2 || devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3))
  {
    status = MxL_HRCLS_Ctrl_SetSerdesIOTermination(devContextPtr, serDesId, rcalCode);
    if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
    {
#ifdef _MXL_HRCLS_SERDES_USING_TX_SLICE_NCH_EN_VALUE_
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN, &dataRb);
      else
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_SLICE_NCH_EN, &dataRb);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN, dataRb & 0x1F);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN, dataRb & 0x1F);
#endif
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN, 0x0C);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN, 0x10);
    }
    else
    {
#ifdef _MXL_HRCLS_SERDES_USING_TX_SLICE_NCH_EN_VALUE_
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN_TITAN, &dataRb);
      else
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_SLICE_NCH_EN_TITAN, &dataRb);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, dataRb & 0x1F);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, dataRb & 0x1F); 
#endif
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, 0x0C);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, 0x10)
    }
  }
  else status = MXL_INVALID_PARAMETER;
  
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E)status;
}
#endif

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgSerDesType1Mode
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure line rate for serdes phy (type 1). According to PG 12/01/2012
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_CfgSerDesType1Mode(
    MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
    MXL_HRCLS_SERDES_LANE_ID_E serDesId,
    MXL_HRCLS_SERDES_LINERATE_E lineRate,
    MXL_HRCLS_SERDES_MODE_E mode
    )
{
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;
  UINT16 dataRb = 0;
  MXL_HRCLS_SERDES_LANE_ID_E otherSerdesLane;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, serDesId=%d, lineRate=%d, mode=%d\n", devId, serDesId, lineRate, mode););

  otherSerdesLane = (serDesId == MXL_HRCLS_SERDES_LANE1) ? MXL_HRCLS_SERDES_LANE0 : MXL_HRCLS_SERDES_LANE1;
  if(mode == MXL_HRCLS_SERDES_MODE_DISABLED)  //Disable SERDES
  {
    UINT32 clockVal = 0;
    // Serdes
    status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_IN_EN_INT, 0);
    status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 1);
    status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_OUT_EN_INT, 0);
    if (serDesId == MXL_HRCLS_SERDES_LANE0)
    {
      status |=  MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_0_RX_RESET, 1);
    }
    else
    {
      status |=  MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_1_RX_RESET, 1);
    }

    // Clock and Phy
    if ((status == MXL_SUCCESS) 
         && (MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal) == MXL_SUCCESS))
    {
      if(serDesId == MXL_HRCLS_SERDES_LANE1) 
      {
        clockVal &= (UINT32) (~(1 << 21));  // bit 21
      }
      else
      {
        clockVal &= (UINT32) (~(1 << 19));  // bit 19
      }
      status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal); 
    } else status = MXL_FAILURE;

    if (status == MXL_SUCCESS)
    {
      status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_RESET_ALL, 1);
      status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_REF_SSP_EN, 0);
      status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_TEST_POWERDOWN, 1);
    }

    if (status == MXL_SUCCESS)
    {
      if(serDesId == MXL_HRCLS_SERDES_LANE1)
      {
        clockVal &= (UINT32) (~(1 << 20)); /* Bit 20, serdes1_tx_clk */
      }
      else
      {
        clockVal &= (UINT32) (~(1 << 18)); /* Bit 18, serdes0_tx_clk */
      }
      status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
    }

    /* Disable resampler only if in HALF mode and both serdes are disabled or
     *  if the current serdes is in full mode */

    if ((status == MXL_SUCCESS) && (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_DISABLED))
    {
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_RESET, 1);
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 0 );
      clockVal &= (UINT32) (~(1 << 10)); /* serdes_dfe */
      status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
    }
  }
  else  /* enable the serdes in either full mode or half mode*/
  {
    status = MxL_HRCLS_Ctrl_SerdesTuneSlot2(devContextPtr, serDesId);

    if (status == MXL_SUCCESS)
    {
      UINT16 eco8_value = 0;
      MXL_BOOL_E mixed_mode = MXL_FALSE;

      // 1A
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_MODEA_MODE, 1);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_MODEA_HALF_MODE_INT, (mode == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?1:0);

      // 1B
      status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_RATE, 1);

      // 2A
      status |= MxL_HRCLS_Ctrl_SerdesEnableClock(devId, serDesId);

      // 2B
      mixed_mode = (((mode == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE) && (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE))
          || ((mode == MXL_HRCLS_SERDES_MODE_ENABLED_FULLRATE) && (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)))?MXL_TRUE:MXL_FALSE;
     
      MXLDBG3(
        if (mixed_mode == MXL_TRUE)
        {
          MxL_HRCLS_DEBUG("Serdes mixed A mode detected\n");
        }
      )

      status |= MxLWare_HRCLS_OEM_ReadRegister(devId, EXTRACT_ADDR(ECO_8), &eco8_value);
      if (mixed_mode == MXL_TRUE)
      {
        eco8_value |= (UINT16) (0x01);
      }
      else
      {
        eco8_value &= (UINT16) (~(0x01));
      }
      status |= MxLWare_HRCLS_OEM_WriteRegister(devId, EXTRACT_ADDR(ECO_8), eco8_value);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_MPLL_MULTIPLIER, (mode == MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)?50:100);

      // 2C Program PHY 
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_TEST_POWERDOWN, 0);
      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_REF_SSP_EN, 1);
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
      {
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_0_TX_RESET, 0);
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_0_RX_RESET, 0);
      }
      else
      {
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_1_TX_RESET, 0);
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES_1_RX_RESET, 0);
      }

      status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_PHY_RESET_ALL, 0);

      MxLWare_HRCLS_OEM_DelayUsec(10*1000); /* Delay for 10 ms */

      if (serDesId ==  MXL_HRCLS_SERDES_LANE0)
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES0_PHY_STATUS_READY, &dataRb);
      else
        status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES1_PHY_STATUS_READY, &dataRb);

      if ((status == MXL_SUCCESS) && (dataRb != 0))
      {
        // 3A
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_DOUT_SEL, 0);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 0);
      }
      else
      {
        MXLERR(MxL_HRCLS_ERROR("Serdes PHY is not ready !!!\n"););
        status = MXL_FAILURE;
      }
    }
    else
    {
      MXLERR(MxL_HRCLS_ERROR("Cannot tune DFE channel %d for serdes slot 2\n", devContextPtr->serDesSlot2Chan[(UINT8) serDesId]););
    }
  }

  MXLEXITSTR(status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgSerDesType2Mode
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 05/02/2014
--|                  
--| DESCRIPTION   : Configure serdes phy (type 2). According to PG 05/01/2014
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_CfgSerDesType2Mode(
    MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
    MXL_HRCLS_SERDES_LANE_ID_E serDesId,
    MXL_HRCLS_SERDES_MODE_E mode
    )
{
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;
  UINT16 dataRb = 0;
  MXL_HRCLS_SERDES_LANE_ID_E otherSerdesLane = (serDesId == MXL_HRCLS_SERDES_LANE1) ? MXL_HRCLS_SERDES_LANE0 : MXL_HRCLS_SERDES_LANE1;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, serDesId=%d, mode=%d\n", devId, serDesId, mode););

  status = MxL_HRCLS_Ctrl_InitPhySerdes(devContextPtr);

  if (MXL_SUCCESS == status)
  {
    if(mode == MXL_HRCLS_SERDES_MODE_DISABLED)  //Disable SERDES
    {
      // Serdes Power Down section

      UINT32 clockVal = 0;
      // 1. Serdes Link:
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
      {
        status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_IN_EN_INT, 0);
      }
      status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 1);
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
      {
        status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_OUT_EN_INT, 0);
      }

      // 3. Clock and Phy
      if ((status == MXL_SUCCESS) 
           && (MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal) == MXL_SUCCESS))
      {
        if(serDesId == MXL_HRCLS_SERDES_LANE1) 
        {
          clockVal &= (UINT32) (~(1 << 21));  // bit 21 serdes1_rx_clk
        }
        else
        {
          clockVal &= (UINT32) (~(1 << 19));  // bit 19 serdes0_rx_clk
        }
        status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal); 
      } else status = MXL_FAILURE;

      if (status == MXL_SUCCESS)
      {
        status |= MxL_HRCLS_Ctrl_PowerdownSerdes(devContextPtr, serDesId);
      }
      
      if (status == MXL_SUCCESS)
      {
        if(serDesId == MXL_HRCLS_SERDES_LANE1)
        {
          clockVal &= (UINT32) (~(1 << 20)); /* Bit 20, serdes1_tx_clk */
        }
        else
        {
          clockVal &= (UINT32) (~(1 << 18)); /* Bit 18, serdes0_tx_clk */
        }
        status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
      }
  
      // 1. Tx Resamp:
      if (status == MXL_SUCCESS)
      {
        if (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_DISABLED)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_RESET, 1);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 0);
          clockVal &= (UINT32) (~(1 << 10)); /* serdes_dfe */
          status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
          
          // Write 0 to the registers so that we can bring both lanes up later.
//          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_ENA, 0);
//          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_ENA, 0);
        }
      }
    }
    else
    {
      /* Enable slot 2 */
      status = MxL_HRCLS_Ctrl_SerdesTuneSlot2(devContextPtr, serDesId);
  
      status |= MxL_HRCLS_Ctrl_SetSerdesIOTermination(devContextPtr, serDesId, MXL_HRCLS_SERDES_RCAL_CODE_6);
      status |= MxL_HRCLS_Ctrl_SerdesOverwriteDefaults(devContextPtr, serDesId);
  
      if (status == MXL_SUCCESS && mode != MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)
      {
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_MODEA_MODE, 1);

        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_RATE, 1);
  
        status |= MxL_HRCLS_Ctrl_SerdesEnableClock(devId, serDesId);
  
        // 13.c PHY Enable
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_ENA, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA, 1);
        if ((serDesId == MXL_HRCLS_SERDES_LANE0) && (status == MXL_SUCCESS))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_EQ_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_SIGDET_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DAC_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_COMP_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CLK_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_EN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_EN, 1);
        }
  
        // 13.c.iii/iv/v: Wait 30us (>20us as said)
        MxLWare_HRCLS_OEM_DelayUsec(30);      
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_START_TUNE, 1);
  
        // 13.c.vi: Wait 1ms 
        MxLWare_HRCLS_OEM_DelayUsec(5000);
        if (serDesId == MXL_HRCLS_SERDES_LANE0)
        {
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_A2D_REFSX_SX_RDY, &dataRb);
          if (MXL_SUCCESS != status || dataRb == 0)
            MXLERR(MxL_HRCLS_DEBUG("status = %d, SERDES_PHY0_A2D_REFSX_SX_RDY = %d\n", status, dataRb););
        }
        else
        {
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_A2D_REFSX_SX_RDY, &dataRb);
          if (MXL_SUCCESS != status || dataRb == 0)
            MXLERR(MxL_HRCLS_DEBUG("status = %d, SERDES_PHY1_A2D_REFSX_SX_RDY = %d\n", status, dataRb););
        }
  
        // 13.c.vii/viii
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CLK_EN, 1);
  
        // 13.c.ix/x: Wait 1us (>5ns as said)
        MxLWare_HRCLS_OEM_DelayUsec(1);
        if ((serDesId == MXL_HRCLS_SERDES_LANE0) && (status == MXL_SUCCESS))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_RESETBAR, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_RESETBAR, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CNTR_RESETBAR, 0x7);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_RESETBAR, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_PI_RESETBAR, 1);
        }
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CKGEN_RESETBAR, 1);
  
        // 13.d: Wait for PHY ready
        MxLWare_HRCLS_OEM_DelayUsec(10*1000); /* Delay for 10 ms */
        dataRb = 0; 
        if (serDesId ==  MXL_HRCLS_SERDES_LANE0)
        {
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES0_PHY_STATUS_READY, &dataRb);
        }
        else
        {
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES1_PHY_STATUS_TX_READY, &dataRb);
        }
  
        if ((status == MXL_SUCCESS) && (dataRb != 0))
        {
          // 7.a.i - vi:
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_DOUT_SEL, 0);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 0);
        }
        else
        {
          MXLERR(MxL_HRCLS_ERROR("Serdes PHY is not ready !!!\n"););
          status = MXL_FAILURE;
        }
      }
      else
      {
        MXLERR(MxL_HRCLS_ERROR("[%d] Tune DFE channel %d for serdes slot 2 status %d, mode: %d\n", __LINE__, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], status, mode););
      }
    }
  } else MXLERR(MxL_HRCLS_DEBUG("MxL_HRCLS_Ctrl_InitPhySerdes failed, status %d\n", status););
    
  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}
  
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxL_HRCLS_Ctrl_CfgSerDesType3Mode
--| 
--| AUTHOR        : Chau Dang
--|
--| DATE CREATED  : 05/02/2014
--|                  
--| DESCRIPTION   : Configure serdes phy (type 3).  Similar to Atlas (type 2), but regs names are suffixed
--|                           _TITAN
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
static MXL_STATUS_E MxL_HRCLS_Ctrl_CfgSerDesType3Mode(
    MXL_HRCLS_DEV_CONTEXT_T * devContextPtr,
    MXL_HRCLS_SERDES_LANE_ID_E serDesId,
    MXL_HRCLS_SERDES_MODE_E mode
    )
{
  UINT8 status = MXL_SUCCESS;
  UINT8 devId = devContextPtr->devId;
  UINT16 dataRb = 0;
  MXL_HRCLS_SERDES_LANE_ID_E otherSerdesLane = (serDesId == MXL_HRCLS_SERDES_LANE1) ? MXL_HRCLS_SERDES_LANE0 : MXL_HRCLS_SERDES_LANE1;

  MXLENTERSTR;
  MXLENTER(MxL_HRCLS_DEBUG("devId = %d, serDesId=%d, mode=%d\n", devId, serDesId, mode););

  status = MxL_HRCLS_Ctrl_InitPhySerdes(devContextPtr);

  if (MXL_SUCCESS == status)
  {
    if(mode == MXL_HRCLS_SERDES_MODE_DISABLED)  //Disable SERDES
    {
      // Serdes Power Down section

      UINT32 clockVal = 0;
      // 1. Serdes Link:
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
      {
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES0_REG_US_IN_EN_TITAN, 0);
      }

      if (serDesId == MXL_HRCLS_SERDES_LANE0)
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_FRAMER_CLEAR_TITAN, 1);
      else
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_FRAMER_CLEAR_TITAN, 1);
        
      if (serDesId == MXL_HRCLS_SERDES_LANE0)
      {
        status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_US_OUT_EN_TITAN, 0);
      }

      // 3. Clock and Phy
      if ((status == MXL_SUCCESS) 
           && (MxL_HRCLS_Ctrl_ReadClockRegister(devId, &clockVal) == MXL_SUCCESS))
      {
        if(serDesId == MXL_HRCLS_SERDES_LANE1) 
        {
          clockVal &= (UINT32) (~(1 << 21));  // bit 21 serdes1_rx_clk
        }
        else
        {
          clockVal &= (UINT32) (~(1 << 19));  // bit 19 serdes0_rx_clk
        }
        status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal); 
      } else status = MXL_FAILURE;

      if (status == MXL_SUCCESS)
      {
        status |= MxL_HRCLS_Ctrl_PowerdownSerdes(devContextPtr, serDesId);
      }

      if (status == MXL_SUCCESS)
      {
        if(serDesId == MXL_HRCLS_SERDES_LANE1)
        {
          clockVal &= (UINT32) (~(1 << 20)); /* Bit 20, serdes1_tx_clk */
        }
        else
        {
          clockVal &= (UINT32) (~(1 << 18)); /* Bit 18, serdes0_tx_clk */
        }
        status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
      }

      // 1. Tx Resamp:
      if (status == MXL_SUCCESS)
      {
        if (devContextPtr->serDesMode[(UINT8) otherSerdesLane] == MXL_HRCLS_SERDES_MODE_DISABLED)
        {
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 0);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_RESET, 1);
          status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 0);
          clockVal &= (UINT32) (~(1 << 10)); /* serdes_dfe */
          status |= MxL_HRCLS_Ctrl_WriteClockRegister(devId, clockVal);
          
          // Write 0 to the registers so that we can bring both lanes up later.
//          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_REFSX_ENA_TITAN, 0);
//          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES_PHY0_D2A_REFSX_ENA_TITAN, 0);
        }
      }
    }
    else
    {
      /* Enable slot 2 */
      status = MxL_HRCLS_Ctrl_SerdesTuneSlot2(devContextPtr, serDesId);

      status |= MxL_HRCLS_Ctrl_SetSerdesIOTermination(devContextPtr, serDesId, MXL_HRCLS_SERDES_RCAL_CODE_6);
      status |= MxL_HRCLS_Ctrl_SerdesOverwriteDefaults(devContextPtr, serDesId);

      if (status == MXL_SUCCESS && mode != MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE)
      {
        status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_RATE, 1); /* Check */

        status |= MxL_HRCLS_Ctrl_SerdesEnableClock(devId, serDesId);

        // 13.c PHY Enable
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_ENA_TITAN, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CHP_REG_ENA_TITAN, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_CLKDIST_REG_ENA_TITAN, 1);
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_VCO_REG_ENA_TITAN, 1);

        MxLWare_HRCLS_OEM_DelayUsec(30);	  
        if ((serDesId == MXL_HRCLS_SERDES_LANE0) && (status == MXL_SUCCESS))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_TERM_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_EQ_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_SIGDET_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DAC_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_COMP_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CLK_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_EN_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_EN_TITAN, 1);
        }

        // 13.c.iii/iv/v: Wait 30us (>20us as said)
        MxLWare_HRCLS_OEM_DelayUsec(30);      
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_REFSX_START_TUNE_TITAN, 1);

        // 13.c.vi: Wait 1ms 
        MxLWare_HRCLS_OEM_DelayUsec(5000);
        if (serDesId == MXL_HRCLS_SERDES_LANE0)
        {
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_A2D_REFSX_SX_RDY_TITAN, &dataRb);
          if (MXL_SUCCESS != status || dataRb == 0)
            MXLERR(MxL_HRCLS_DEBUG("status = %d, SERDES_PHY0_A2D_REFSX_SX_RDY = %d\n", status, dataRb););
        }
        else
        {
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_A2D_REFSX_SX_RDY_TITAN, &dataRb);
          if (MXL_SUCCESS != status || dataRb == 0)
            MXLERR(MxL_HRCLS_DEBUG("status = %d, SERDES_PHY1_A2D_REFSX_SX_RDY = %d\n", status, dataRb););
        }

        // 13.c.vii/viii
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CLK_EN_TITAN, 1);

        // 13.c.ix/x: Wait 1us (>5ns as said)
        MxLWare_HRCLS_OEM_DelayUsec(1);
        if ((serDesId == MXL_HRCLS_SERDES_LANE0) && (status == MXL_SUCCESS))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_RESETBAR_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_BB_RESETBAR_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_CNTR_RESETBAR_TITAN, 0x7);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_RESETBAR_TITAN, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_RX_PI_RESETBAR_TITAN, 1);
        }
        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesId, SERDES_PHY0_D2A_TX_CKGEN_RESETBAR_TITAN, 1);

        // 13.d: Wait for PHY ready
        MxLWare_HRCLS_OEM_DelayUsec(10*1000); /* Delay for 10 ms */
        dataRb = 0; 
        if (serDesId ==  MXL_HRCLS_SERDES_LANE0)
        {
          // status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES0_PHY_STATUS_READY, &dataRb);
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_PHY_STATUS_TX_READY_TITAN, &dataRb);
          if (dataRb != 0)
          {
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_PHY_STATUS_RX_READY_TITAN, &dataRb);
          }
        }
        else
        {
          // status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES1_PHY_STATUS_TX_READY, &dataRb);
          status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_PHY_STATUS_TX_READY_TITAN, &dataRb);
        }

        if ((status == MXL_SUCCESS) && (dataRb != 0))
        {
          // 7.a.i - vi:
          if (serDesId == MXL_HRCLS_SERDES_LANE0)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_LSB_FIRST_OUT_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_DOUT_SEL_TITAN, 0);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_FRAMER_CLEAR_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_FRAMER_CLEAR_TITAN, 0);
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_LSB_FIRST_OUT_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_DOUT_SEL_TITAN, 0);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_FRAMER_CLEAR_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_FRAMER_CLEAR_TITAN, 0);
          }
        
        }
        else
        {
          MXLERR(MxL_HRCLS_ERROR("Serdes PHY is not ready !!!\n"););
          status = MXL_FAILURE;
        }
      }
      else
      {
        MXLERR(MxL_HRCLS_ERROR("[%d] Tune DFE channel %d for serdes slot 2 status %d, mode: %d\n", __LINE__, devContextPtr->serDesSlot2Chan[(UINT8) serDesId], status, mode););
      }
    }
  } else MXLERR(MxL_HRCLS_DEBUG("MxL_HRCLS_Ctrl_InitPhySerdes failed, status %d\n", status););
  
  MXLEXITSTR(status);
  return (MXL_STATUS_E)status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesMode
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure line rate for serdes phy. According to PG 12/01/2012
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesMode(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesId,
    MXL_HRCLS_SERDES_LINERATE_E lineRate,
    MXL_HRCLS_SERDES_MODE_E mode
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("devId=%d, serDesId=%d, lineRate=%d, mode=%d\n", devId, serDesId, lineRate, mode););

  MXL_HRCLS_SERDES_SWAP(serDesId);
  MXLDBG3(MxL_HRCLS_DEBUG("serDesId after swap=%d\n", serDesId););

  if ((devContextPtr) && (serDesId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if ((mode != MXL_HRCLS_SERDES_MODE_ENABLED_HALFRATE) && (lineRate == MXL_HRCLS_SERDES_LINERATE_5400MHZ))
      {
        devContextPtr->serDesMode[(UINT8) serDesId] = mode;
        devContextPtr->serDesLineRate = lineRate;

        switch (devContextPtr->serDesType)
        {
          case MXL_HRCLS_SERDES_TYPE_1:
          default:
            status = MxL_HRCLS_Ctrl_CfgSerDesType1Mode(devContextPtr, serDesId, lineRate, mode);
            break;

          case MXL_HRCLS_SERDES_TYPE_2:
            status = MxL_HRCLS_Ctrl_CfgSerDesType2Mode(devContextPtr, serDesId, mode);
            break;
              
          case MXL_HRCLS_SERDES_TYPE_3:
            status = MxL_HRCLS_Ctrl_CfgSerDesType3Mode(devContextPtr, serDesId, mode);
            break;
        }
      } 
      else 
      {
        status = MXL_NOT_SUPPORTED;
        MXLERR(MxL_HRCLS_ERROR("Only 5400;fullrate mode is supported\n"););
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes parameters
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsParams(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_HRCLS_SERDES_CODING_E dsCoding,
    MXL_BOOL_E scramblerEnable
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("serDesDsId=%d, dsCoding=%d, scramblerEnable=%d\n", serDesDsId, dsCoding, scramblerEnable););

  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      switch (devContextPtr->serDesType)
      {
        case MXL_HRCLS_SERDES_TYPE_1:
        {
          UINT16 useCrc = (dsCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_RS_USE_CRC, useCrc);

          break;
        }
        
        case MXL_HRCLS_SERDES_TYPE_2:
        {
          UINT16 useCrc = (dsCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_RS_USE_CRC, useCrc);
          // 14.a: SerDesEnOut.
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_DOUT_SEL, 0);
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_FRAMER_CLEAR_INT, 0);

          break;
        }

        case MXL_HRCLS_SERDES_TYPE_3:
        {
          UINT16 useCrc = (dsCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;

          if (serDesDsId == MXL_HRCLS_SERDES_LANE0)
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_RS_USE_CRC_TITAN, useCrc);
            // 14.a: SerDesEnOut.
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_LSB_FIRST_OUT_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_DOUT_SEL_TITAN, 0);

            
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_FRAMER_CLEAR_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES0_REG_DS_FRAMER_CLEAR_TITAN, 0);
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_RS_USE_CRC_TITAN, useCrc);
            // 14.a: SerDesEnOut.
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_LSB_FIRST_OUT_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_DOUT_SEL_TITAN, 0);

            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_FRAMER_CLEAR_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, SERDES1_REG_DS_FRAMER_CLEAR_TITAN, 0);
          }
          break;
        }      
      }
    }else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}


/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsPrbsParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes scrambler parameters
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPrbsParams(
    UINT8     devId,
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_BOOL_E  enable,
    MXL_HRCLS_SERDES_PRBS_MODE_E dsPrbsMode
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt) && 
         ((dsPrbsMode == MXL_HRCLS_SERDES_PRBS31) || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS23) 
       || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS15) || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS7) || (dsPrbsMode == MXL_HRCLS_SERDES_PRBS22)))
  {
    if (devContextPtr->driverInitialized)
    {
      switch (devContextPtr->serDesType)
      {
        case MXL_HRCLS_SERDES_TYPE_1:
        {
          // Re-arranged according to tickets #120, #124
          if (enable == MXL_ENABLE)
          {
            if (dsPrbsMode != MXL_HRCLS_SERDES_PRBS22)
            {
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_DOUT_SEL, 1);

              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_PRBS_MODEA_RST, 0);
          
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_PRBS_MODEA_SEL, (UINT16) dsPrbsMode); /* Program PRBS mode */
            } else status = MXL_INVALID_PARAMETER;
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_PRBS_MODEA_RST, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_LSB_FIRST_OUT, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_DS_DOUT_SEL, 0);
          }
          break;
        }

        case MXL_HRCLS_SERDES_TYPE_2:
          if (enable == MXL_ENABLE)
          {
            if (dsPrbsMode != MXL_HRCLS_SERDES_PRBS22)
            {
              // 14.a 
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_LSB_FIRST_OUT, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_DOUT_SEL, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_RST, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_SEL, (UINT16) dsPrbsMode);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_RST, 0);
            }
            else
            {
              status = MXL_NOT_SUPPORTED;
            }
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_LSB_FIRST_OUT, 0);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_DOUT_SEL, 0);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_RST, 1);
          }
          break;

        case MXL_HRCLS_SERDES_TYPE_3:
        {
          if (enable == MXL_ENABLE)
          {
            if (dsPrbsMode != MXL_HRCLS_SERDES_PRBS22)
            {
              // 14.a 
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_LSB_FIRST_OUT_TITAN, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_DOUT_SEL_TITAN, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_RST_TITAN, 1);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_SEL_TITAN, (UINT16) dsPrbsMode);
              status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_RST_TITAN, 0);
            }
            else
            {
              status = MXL_NOT_SUPPORTED;
            }
          }
          else
          {
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_LSB_FIRST_OUT_TITAN, 0);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_DOUT_SEL_TITAN, 0);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_PRBS_MODEA_RST_TITAN, 1);
          }
          break;
        }
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsPreEmphasis 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes pre-emphasis
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPreEmphasis(
    UINT8     devId,                
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,  
    UINT8     preEmphasisSetting          
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      UINT16 dataRb;

      switch (devContextPtr->serDesType)
      {
        case MXL_HRCLS_SERDES_TYPE_1:
        default:
          status =  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_PHY_TX_PREEMPH_GEN3, (UINT16) preEmphasisSetting);
          break;

        case MXL_HRCLS_SERDES_TYPE_2:
          if (serDesDsId == MXL_HRCLS_SERDES_LANE0)
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_DEMPH_EN, &dataRb);
          else
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_DEMPH_EN, &dataRb);
          MXLDBG1(MxL_HRCLS_DEBUG("txAmpEn=%02x, preEmphasisSetting=%02x, writeVal=%02x\n", 
                                   dataRb, preEmphasisSetting, (dataRb & 0x1F) - ((preEmphasisSetting >> 3) & 0x7)););
          status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_D2A_TX_AMP_EN, (dataRb & 0x1F) - ((preEmphasisSetting >> 3) & 0x7));
          break;
        
        case MXL_HRCLS_SERDES_TYPE_3:
          if (serDesDsId == MXL_HRCLS_SERDES_LANE0)
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, &dataRb);
          else
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_DEMPH_EN_TITAN, &dataRb);
          MXLDBG1(MxL_HRCLS_DEBUG("txAmpEn=%02x, preEmphasisSetting=%02x, writeVal=%02x\n", 
                                   dataRb, preEmphasisSetting, (dataRb & 0x1F) - ((preEmphasisSetting >> 3) & 0x7)););
          status |=  MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, (dataRb & 0x1F) - ((preEmphasisSetting >> 3) & 0x7));
          break;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsAmplitude 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes signal amplitude
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsAmplitude(
    UINT8     devId,                     
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    UINT8     dsAmplitude                 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    UINT16 txAmpEn;
    UINT16 txDemphEn;
    UINT16 txSliceNchEn;
    UINT16 writeVal;
    
    if (devContextPtr->driverInitialized)
    {
      switch (devContextPtr->serDesType)
      {
        case MXL_HRCLS_SERDES_TYPE_1:
        default:
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_PHY_TX_AMPLITUDE_GEN3, (UINT16) dsAmplitude);
          break;

        case MXL_HRCLS_SERDES_TYPE_2:
          if (serDesDsId == MXL_HRCLS_SERDES_LANE0)
          {
            status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_AMP_EN, &txAmpEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_DEMPH_EN, &txDemphEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN, &txSliceNchEn);
          }
          else
          {
            status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_AMP_EN, &txAmpEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_DEMPH_EN, &txDemphEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_SLICE_NCH_EN, &txSliceNchEn);
          }

          writeVal = (txAmpEn & 0x1F) - (txDemphEn & 0x1F) +  (txSliceNchEn & 0x1F) - 7 + ((dsAmplitude >> 4) & 0x7);
          MXLDBG1(MxL_HRCLS_DEBUG("txAmpEn=%02x, txDemphEn=%02x, txSliceNchEn=%02x, dsAmplitude=%02x, writeVal=%02x\n", 
                                   txAmpEn, txDemphEn, txSliceNchEn, dsAmplitude, writeVal););
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_D2A_TX_AMP_EN, (UINT16) writeVal);
          writeVal = (txSliceNchEn & 0x1F) - 7 + ((dsAmplitude >> 4) & 0x7);
          MXLDBG1(MxL_HRCLS_DEBUG("writeVal=%02x\n", writeVal););
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_D2A_TX_DEMPH_EN, (UINT16) writeVal);
          break;

        case MXL_HRCLS_SERDES_TYPE_3:
          if (serDesDsId == MXL_HRCLS_SERDES_LANE0)
          {
            status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, &txAmpEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, &txDemphEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY0_D2A_TX_SLICE_NCH_EN_TITAN, &txSliceNchEn);
          }
          else
          {
            status = MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_AMP_EN_TITAN, &txAmpEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_DEMPH_EN_TITAN, &txDemphEn);
            status |= MxL_HRCLS_Ctrl_ReadRegisterField(devId, SERDES_PHY1_D2A_TX_SLICE_NCH_EN_TITAN, &txSliceNchEn);
          }

          writeVal = (txAmpEn & 0x1F) - (txDemphEn & 0x1F) +	(txSliceNchEn & 0x1F) - 7 + ((dsAmplitude >> 4) & 0x7);
          MXLDBG1(MxL_HRCLS_DEBUG("txAmpEn=%02x, txDemphEn=%02x, txSliceNchEn=%02x, dsAmplitude=%02x, writeVal=%02x\n", 
                                                   txAmpEn, txDemphEn, txSliceNchEn, dsAmplitude, writeVal););
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_D2A_TX_AMP_EN_TITAN, (UINT16) writeVal);
          writeVal = (txSliceNchEn & 0x1F) - 7 + ((dsAmplitude >> 4) & 0x7);
          MXLDBG1(MxL_HRCLS_DEBUG("writeVal=%02x\n", writeVal););
          status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_D2A_TX_DEMPH_EN_TITAN, (UINT16) writeVal);
          break;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsCrossBar 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure Downstream serdes crossbar setting
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsCrossBar(
    UINT8     devId,          
    MXL_HRCLS_SERDES_LANE_ID_E serDesDsId,
    MXL_HRCLS_SERDES_SLOT_ID_E serDesSlotId,                
    MXL_HRCLS_CHAN_ID_E chanId                      
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("serDesDsId=%d, serDesSlotId=%d, chanId=%d\n", serDesDsId, serDesSlotId, chanId););

  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if (devContextPtr)
  {
    if ((serDesDsId < devContextPtr->serDesDSLanesCnt)
      && (serDesSlotId < devContextPtr->serDesSlotsPerLane[(UINT8) serDesDsId]) 
      && ((chanId < devContextPtr->dfeChanCount || chanId == MXL_HRCLS_CHAN_NB)))
    {
      if (devContextPtr->driverInitialized)
      {
        {
          MXL_HRCLS_CHAN_ID_E phyChanId;
          phyChanId = MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(devContextPtr, chanId);

          if (phyChanId == MXL_HRCLS_CHAN_NB) phyChanId = MXL_HRCLS_CHAN0;

          switch (devContextPtr->serDesType)
          {
            case MXL_HRCLS_SERDES_TYPE_1:
            case MXL_HRCLS_SERDES_TYPE_2:
            default:
            {
              status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(
                  devId, 
                  serDesDsId,
                  EXTRACT_ADDR(SERDES0_REG_NB0_SEL) + ((UINT16) serDesSlotId >> 1)*4, 
                  ((UINT16) serDesSlotId & 1) ? 8:0 /* lsbPos: 0 for even slots and 8 for odd slots */, 
                  EXTRACT_NBITS(SERDES0_REG_NB0_SEL),  
                  phyChanId);
              break;
            }
            
            case MXL_HRCLS_SERDES_TYPE_3:
            {
              if (serDesDsId == MXL_HRCLS_SERDES_LANE0)
              {
                status = MxL_HRCLS_Ctrl_UpdateRegisterField(
                    devId, 
                    EXTRACT_ADDR(SERDES0_REG_NB0_SEL_TITAN) + ((UINT16) serDesSlotId >> 1)*4, 
                    ((UINT16) serDesSlotId & 1) ? 8 : 0 /* lsbPos: 0 for even slots and 8 for odd slots */, 
                    EXTRACT_NBITS(SERDES0_REG_NB0_SEL_TITAN),  
                    phyChanId);
              }
              else
              {
                status = MxL_HRCLS_Ctrl_UpdateRegisterField(
                    devId, 
                    EXTRACT_ADDR(SERDES1_REG_NB0_SEL_TITAN) + ((UINT16) serDesSlotId >> 1)*4, 
                    ((UINT16) serDesSlotId & 1) ? 8 : 0 /* lsbPos: 0 for even slots and 8 for odd slots */, 
                    EXTRACT_NBITS(SERDES1_REG_NB0_SEL_TITAN),  
                    phyChanId);

              }
              break;
            }
          }
              
          if ((status == MXL_SUCCESS) && (serDesSlotId == MXL_HRCLS_SERDES_SLOT2)) 
          {
            devContextPtr->serDesSlot2Chan[(UINT8) serDesDsId] = phyChanId;
          }

          MXLDBG1(MxL_HRCLS_DEBUG("Channel %d connected to slot %d of SerDes%d\n", chanId, serDesSlotId, serDesDsId););
        }
      } else status = MXL_NOT_INITIALIZED;
    }
    else
    {
      status = MXL_INVALID_PARAMETER;
      MXLERR(
        MxL_HRCLS_ERROR("Invalid parameter in Xbar setting: ");
        if (serDesDsId < devContextPtr->serDesDSLanesCnt)
        {
          if (serDesSlotId < devContextPtr->serDesSlotsPerLane[(UINT8) serDesDsId])
          {
            MxL_HRCLS_ERROR("chanId should be less than %d or exactly %d\n", devContextPtr->dfeChanCount, MXL_HRCLS_CHAN_NB);
          } else MxL_HRCLS_ERROR("serDesSlotId should be less than %d\n", devContextPtr->serDesSlotsPerLane[(UINT8) serDesDsId]);
        } else MxL_HRCLS_ERROR("serDesDsId should be less than %d\n", devContextPtr->serDesDSLanesCnt);
      )
    }
  } 
  else 
  {
    status = MXL_INVALID_PARAMETER;
  }
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesDsPhyPolarity 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/18/2012
--|                  
--| DESCRIPTION   : Invert DS phy
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesDsPhyPolarity(UINT8 devId, 
        MXL_HRCLS_SERDES_LANE_ID_E serDesDsId, 
        MXL_BOOL_E phyInverted)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("serDesDsId=%d, DS phyInverted=%d\n", serDesDsId, (phyInverted==MXL_TRUE)?1:0););
  MXLENTERAPI(MxL_HRCLS_DEBUG("devContextPtr=%p, serDesDSLanesCnt=%d\n", devContextPtr, (devContextPtr ? devContextPtr->serDesDSLanesCnt : (-1))););
  MXL_HRCLS_SERDES_SWAP(serDesDsId);
  if ((devContextPtr) && (serDesDsId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        switch (devContextPtr->serDesType)
        {
          case MXL_HRCLS_SERDES_TYPE_1:
          default:
            status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES0_REG_PHY_TX_INVERT, (phyInverted == MXL_ENABLE)?1:0);
            break;

          case MXL_HRCLS_SERDES_TYPE_2:
            status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_POLAR_INV, (phyInverted == MXL_ENABLE)?1:0);
            break;

          case MXL_HRCLS_SERDES_TYPE_3:
            status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, serDesDsId, SERDES_PHY0_REG_DS_POLAR_INV_TITAN, (phyInverted == MXL_ENABLE)?1:0);
            break;
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } 
  else 
  {
    status = MXL_INVALID_PARAMETER;
  }
  MXLEXITAPISTR(devId,status);
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream serdes settings
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsParams(
    UINT8     devId,   
    MXL_HRCLS_SERDES_CODING_E usCoding,   
    MXL_BOOL_E  usRsCorrectEnable,
    MXL_BOOL_E  descramblerEnable 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("coding=%s, RS correction=%d, descrambler=%d (ignored)\n", (usCoding == MXL_HRCLS_SERDES_CODING_CRC)?"CRC":"RS", (usRsCorrectEnable == MXL_ENABLE)?1:0, (descramblerEnable == MXL_ENABLE)?1:0););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        switch (devContextPtr->serDesType)
        {
          case  MXL_HRCLS_SERDES_TYPE_1:
          {
            UINT16 useCrc = (usCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
            devContextPtr->serDesUsPrbsMode = MXL_FALSE;
            
            /* Set Us encoding for both serdes 0 and serdes 1 */
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_USE_CRC, useCrc);

            /* Enable/Disable correction */
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DISABLE_CORRECTION, (usRsCorrectEnable == MXL_ENABLE)?0:1);

            // 3B
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_LSB_FIRST_IN, 1);
          
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_IN_EN_INT, 1);
          
            status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR);
            status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 1);
            status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DFE_USRESAMP_RESET);

            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_OUT_EN_INT, 1);
            break;
          }
          
          case MXL_HRCLS_SERDES_TYPE_2:
          {
            UINT16 regData = 0;
            UINT16 useCrc = (usCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
            devContextPtr->serDesUsPrbsMode = MXL_FALSE;

            /* Set Us encoding for both serdes 0 and serdes 1 */
            status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_USE_CRC, useCrc);
            /* Enable/Disable correction */
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DISABLE_CORRECTION, (usRsCorrectEnable == MXL_ENABLE)?0:1);

            // 14.a: SerDesEnOut.
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_LSB_FIRST_IN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_IN_EN_INT, 1);              
            status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR);
            status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 1);
            status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DFE_USRESAMP_RESET);

            // 14.b.vi: Wait 2ms for SERDES0_US2APB_SYNC = 1
            MxLWare_HRCLS_OEM_DelayUsec(5000);
            if ((status == MXL_SUCCESS) && (MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US2APB_SYNC), &regData) == MXL_SUCCESS))
            {
              if (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_US2APB_SYNC)))
              {
                status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 1);
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_OUT_EN_INT, 1);
                status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 1);
              }
              else
              {
                MXLERR(MxL_HRCLS_DEBUG("SERDES0_US2APB_SYNC = %04x\n", regData););
                status = MXL_NOT_READY;
              }
            }
            else
            {
              MXLERR(MxL_HRCLS_DEBUG("Reading SERDES0_US2APB_SYNC failed\n"););
              status = MXL_FAILURE;
            }
            break;
          }

          case MXL_HRCLS_SERDES_TYPE_3:
          {
            UINT16 regData = 0;
            UINT16 useCrc = (usCoding == MXL_HRCLS_SERDES_CODING_CRC)?1:0;
            devContextPtr->serDesUsPrbsMode = MXL_FALSE;

            /* Set Us encoding for both serdes 0 and serdes 1 */
            status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_USE_CRC_TITAN, useCrc);
            /* Enable/Disable correction */
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DISABLE_CORRECTION_TITAN, (usRsCorrectEnable == MXL_ENABLE)?0:1);

            // 14.a: SerDesEnOut.
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_LSB_FIRST_IN_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_IN_EN_TITAN, 1);
            status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR_TITAN);
            status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_DEFRAME_CLEAR_TITAN);
            status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_USRESAMP_CLK_EN, 1);
            status |= MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(devId, DFE_USRESAMP_RESET);

            // 14.b.vi: Wait 2ms for SERDES0_US2APB_SYNC = 1
            MxLWare_HRCLS_OEM_DelayUsec(5000);
#ifndef _MXL_DIAG_ENABLED_
            if ((status == MXL_SUCCESS) && (MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US2APB_SYNC_TITAN), &regData) == MXL_SUCCESS))
            {
              if (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_US2APB_SYNC_TITAN)))
              {
#endif
                status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_OP_EN, 1);
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_US_OUT_EN_TITAN, 1);
                status |= MxL_HRCLS_Ctrl_UpdateRegisterField(devId, DFE_TX_RESAMP_DOUT_EN, 1);
#ifndef _MXL_DIAG_ENABLED_
              }
              else
              {
                MXLERR(MxL_HRCLS_DEBUG("SERDES0_US2APB_SYNC = %04x\n", regData););
                status = MXL_NOT_READY;
              }
            }
            else
            {
              MXLERR(MxL_HRCLS_DEBUG("Reading SERDES0_US2APB_SYNC failed: %d\n", status););
              status = MXL_FAILURE;
            }
#endif
            break;
          }
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsSyncParams 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream synchronization settings
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsSyncParams(
    UINT8     devId,
    UINT8     S1,
    UINT8     S2,
    UINT8     S3
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        UINT16 regValue = 0;
        UINT8 shift, mask;

        switch (devContextPtr->serDesType)
        {
          case MXL_HRCLS_SERDES_TYPE_1:
          case MXL_HRCLS_SERDES_TYPE_2:
          default:
            mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNC_NUM));
            if (S1 > mask)
            {
              status = MXL_INVALID_PARAMETER;
              MXLERR(MxL_HRCLS_ERROR("S1 should be less than or equal %d\n", mask););
            }
            shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNC_NUM);
            regValue = (S1 & mask) << shift;

            mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNC_NOT_DET_NUM)); 
            if (S2 > mask)
            {
              status = MXL_INVALID_PARAMETER;
              MXLERR(MxL_HRCLS_ERROR("S2 should be less than or equal %d\n", mask););
            }
            shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNC_NOT_DET_NUM);
            regValue |= (S2 & mask) << shift;
            mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNCLOST_RANGE_NUM)); 
            if (S3 > mask)
            {
              status = MXL_INVALID_PARAMETER;
              MXLERR(MxL_HRCLS_ERROR("S3 should be less than or equal %d\n", mask););
            }
            shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNCLOST_RANGE_NUM);
            regValue |= (S3 & mask) << shift;

            if (status == MXL_SUCCESS)
            {
              status = MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_REG_US_MODEA_SYNC_NUM), regValue);
              if (status == MXL_SUCCESS)
              {
                status = MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES1_REG_US_MODEA_SYNC_NUM), regValue);
              }
            }            
            break;

          case MXL_HRCLS_SERDES_TYPE_3:
            mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNC_NUM_TITAN));
            if (S1 > mask)
            {
              status = MXL_INVALID_PARAMETER;
              MXLERR(MxL_HRCLS_ERROR("S1 should be less than or equal %d\n", mask););
            }
            shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNC_NUM_TITAN);
            regValue = (S1 & mask) << shift;
		
            mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNC_NOT_DET_NUM_TITAN)); 
            if (S2 > mask)
            {
              status = MXL_INVALID_PARAMETER;
              MXLERR(MxL_HRCLS_ERROR("S2 should be less than or equal %d\n", mask););
            }
            shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNC_NOT_DET_NUM_TITAN);
            regValue |= (S2 & mask) << shift;
            mask = MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_US_MODEA_SYNCLOST_RANGE_NUM_TITAN)); 
            if (S3 > mask)
            {
              status = MXL_INVALID_PARAMETER;
              MXLERR(MxL_HRCLS_ERROR("S3 should be less than or equal %d\n", mask););
            }
            shift = MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_REG_US_MODEA_SYNCLOST_RANGE_NUM_TITAN);
            regValue |= (S3 & mask) << shift;
		
            if (status == MXL_SUCCESS)
            {
              status = MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_REG_US_MODEA_SYNC_NUM_TITAN), regValue);
              if (status == MXL_SUCCESS)
              {
                status = MxLWare_HRCLS_OEM_WriteRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES1_REG_US_MODEA_SYNC_NUM_TITAN), regValue);
              }
            }
            break;
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsEqualizer 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream equalizer settings
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsEqualizer(
    UINT8     devId,     
    UINT8     equalizerSetting
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1 && (equalizerSetting <= MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES0_REG_PHY_RX_EQ))))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_PHY_RX_EQ, (UINT16) equalizerSetting);
        }
        else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2 && equalizerSetting <= MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES_PHY0_D2A_RX_EQ)))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_RX_EQ, (UINT16) equalizerSetting);
        }
        else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3 && equalizerSetting <= MXL_HRCLS_LSB_MASK(MxL_HRCLS_Ctrl_ExtractRegNumBits(SERDES_PHY0_D2A_RX_EQ_TITAN)))
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_D2A_RX_EQ_TITAN, (UINT16) equalizerSetting);
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsPrbs 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Configure upstream serdes PRBS 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPrbs(
    UINT8     devId,                        
    MXL_BOOL_E  usPrbsCheckerEnable,         
    MXL_HRCLS_SERDES_PRBS_MODE_E usPrbsMode 
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) &&
      ((usPrbsMode == MXL_HRCLS_SERDES_PRBS7) || (usPrbsMode == MXL_HRCLS_SERDES_PRBS15)
      || (usPrbsMode == MXL_HRCLS_SERDES_PRBS23) || (usPrbsMode == MXL_HRCLS_SERDES_PRBS31)
      || (usPrbsMode == MXL_HRCLS_SERDES_PRBS22)) )

  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        devContextPtr->serDesUsPrbsMode = usPrbsCheckerEnable;
        switch (devContextPtr->serDesType)
        {
          case MXL_HRCLS_SERDES_TYPE_1:
          {
            status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_PHY_RX0_ALIGN_EN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_PHY_RX0_ALIGN_EN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
            /* Configure Us parameters of both SERDES0 and SERDES1 */
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_MODEA_RST, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_RST, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_LSB_FIRST_IN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_LSB_FIRST_IN, (usPrbsCheckerEnable == MXL_ENABLE)?0:1);
//        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_RST, (usPrbsCheckerEnable == MXL_ENABLE)?0:1) ;

            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);
//        status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE1, SERDES0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);
            break;
          }

          case MXL_HRCLS_SERDES_TYPE_2:          
            if (usPrbsMode != MXL_HRCLS_SERDES_PRBS22)
            {
              if (usPrbsCheckerEnable == MXL_ENABLE)
              {
                UINT16 regData = 0;

                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_LSB_FIRST_IN, 1);
                /* 14.c.iii/iv: Enable PRBS */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_RST, 1);
                /* 14.c.v Clear error counter */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR, 1);
                /* 14.c.vi: Set PRBS mode */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_SEL, (UINT16) usPrbsMode);

                /* 14.c.vii/viii: Wait for SERDES_PHYn_A2D_RX_DATA_VLD = 1 */
                MxLWare_HRCLS_OEM_DelayUsec(1000);
                if (MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_A2D_RX_DATA_VLD), &regData) == MXL_SUCCESS)
                {
                  if (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES_PHY0_A2D_RX_DATA_VLD)))
                  {
                    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR, 0);
                    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_RST, 0);
                  }
                  else
                    MXLERR(MxL_HRCLS_ERROR("[%d]SERDES_PHY0_A2D_RX_DATA_VLD is not set\n", __LINE__););
                }
                else
                  MXLERR(MxL_HRCLS_ERROR("[%d]Error reading SERDES_PHY0_A2D_RX_DATA_VLD\n", __LINE__););
              }
              else
              {
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_LSB_FIRST_IN, 0);

                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR, 0);
                /* 14.c.iii/iv: Disable PRBS */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_RST, 1);
              }
            }
            else
            {
              MXLERR(MxL_HRCLS_ERROR("[%d]Atlas US Serdes does not support MXL_HRCLS_SERDES_PRBS22\n", __LINE__););
              status = MXL_NOT_SUPPORTED;
            }
            break;
            
          case MXL_HRCLS_SERDES_TYPE_3:
          {
            if (usPrbsMode != MXL_HRCLS_SERDES_PRBS22)
            {
              if (usPrbsCheckerEnable == MXL_ENABLE)
              {
                UINT16 regData = 0;

                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_LSB_FIRST_IN_TITAN, 1);
                /* 14.c.iii/iv: Enable PRBS */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_RST_TITAN, 1);
                /* 14.c.v Clear error counter */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR_TITAN, 1);
                /* 14.c.vi: Set PRBS mode */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_SEL_TITAN, (UINT16) usPrbsMode);

                /* 14.c.vii/viii: Wait for SERDES_PHYn_A2D_RX_DATA_VLD = 1 */
                MxLWare_HRCLS_OEM_DelayUsec(1000);
                if (MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_A2D_RX_DATA_VLD_TITAN), &regData) == MXL_SUCCESS)
                {
                  if (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES_PHY0_A2D_RX_DATA_VLD_TITAN)))
                  {
                    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR_TITAN, 0);
                    status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_RST_TITAN, 0);
                  }
                  else
                    MXLERR(MxL_HRCLS_ERROR("[%d]SERDES_PHY0_A2D_RX_DATA_VLD_TITAN is not set\n", __LINE__););
                }
                else
                  MXLERR(MxL_HRCLS_ERROR("[%d]Error reading SERDES_PHY0_A2D_RX_DATA_VLD_TITAN\n", __LINE__););
              }
              else
              {
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_LSB_FIRST_IN_TITAN, 0);

                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR_TITAN, 0);
                /* 14.c.iii/iv: Disable PRBS */
                status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_RST_TITAN, 1);
              }
            }
            else
            {
              MXLERR(MxL_HRCLS_ERROR("[%d]Atlas US Serdes does not support MXL_HRCLS_SERDES_PRBS22\n", __LINE__););
              status = MXL_NOT_SUPPORTED;
            }
            break;
          }
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqSerDesUsErrCnt 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Request serdes upstream error counters 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsErrCnt(
    UINT8     devId,
    MXL_HRCLS_SERDES_US_ERRCNT_T* errCntPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (errCntPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesUsPrbsMode == MXL_FALSE)
        {
          UINT16 correctedAddrLo, correctedAddrHi, uncorrectedAddrLo, uncorrectedAddrHi;
          UINT16 receivedAddrHi, receivedAddrLo;

          UINT16 readDataHi = 0, readDataLo = 0;

          if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1 || devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
          {
            correctedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_CORRECTED_PACKETS_LSB); 
            correctedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_CORRECTED_PACKETS_MSB);
            uncorrectedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_UNCORRECTED_PACKETS_LSB);
            uncorrectedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_UNCORRECTED_PACKETS_MSB);
            receivedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_TOTAL_PACKETS_LSB);
            receivedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_TOTAL_PACKETS_MSB);
          }
          else
          {
            correctedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_CORRECTED_PACKETS_LSB_TITAN); 
            correctedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_CORRECTED_PACKETS_MSB_TITAN);
            uncorrectedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_UNCORRECTED_PACKETS_LSB_TITAN);
            uncorrectedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_UNCORRECTED_PACKETS_MSB_TITAN);
            receivedAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_TOTAL_PACKETS_LSB_TITAN);
            receivedAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_TOTAL_PACKETS_MSB_TITAN);
          }

          if ((MxLWare_HRCLS_OEM_ReadRegister(devId, correctedAddrLo, &readDataLo) == MXL_SUCCESS) && 
              (MxLWare_HRCLS_OEM_ReadRegister(devId, correctedAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            errCntPtr->cntCorrected = (readDataLo | (readDataHi << 16));
          } else status = MXL_FAILURE;

          if ((status == MXL_SUCCESS) &&
              (MxLWare_HRCLS_OEM_ReadRegister(devId, uncorrectedAddrLo, &readDataLo) == MXL_SUCCESS) && 
              (MxLWare_HRCLS_OEM_ReadRegister(devId, uncorrectedAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            errCntPtr->cntUncorrected = (readDataLo | (readDataHi << 16));
          } else status = MXL_FAILURE;

          if ((status == MXL_SUCCESS) &&
              (MxLWare_HRCLS_OEM_ReadRegister(devId, receivedAddrLo, &readDataLo) == MXL_SUCCESS) && 
              (MxLWare_HRCLS_OEM_ReadRegister(devId, receivedAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            errCntPtr->cntReceived = (readDataLo | (readDataHi << 16));
          } else status = MXL_FAILURE;
        }
        else
        {
          errCntPtr->cntCorrected = 0; 
          errCntPtr->cntUncorrected = 0; 
          errCntPtr->cntReceived = 0; 
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
 } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsErrCntClear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/29/2011
--|                  
--| DESCRIPTION   : Clear upstream error counter 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsErrCntClear(
    UINT8     devId
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr) 
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1 || devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_CLEAR_CNT); 
        else
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_RS_CLEAR_CNT_TITAN); 
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsPrbsErrCntClear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/29/2011
--|                  
--| DESCRIPTION   : Clear upstream error counter 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPrbsErrCntClear(
    UINT8     devId
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1)
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_RX_CLR); 
        else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_RX_CLR); 
        else
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_PRBS_RX_CLR_TITAN); 
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqSerDesUsPrbsErrCnt 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Request serdes upstream error counters in PRBS mode 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsPrbsErrCnt(
    UINT8   devId,
    UINT32* errCntPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && (errCntPtr))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesUsPrbsMode == MXL_TRUE)
        {
          UINT16 regAddrHi, regAddrLo;
          UINT16 readDataHi =  0, readDataLo = 0, maskHi = 0xffff;

          if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1)
          {
            regAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US_PRBS_MODEA_ERR_CNT_MSB);
            regAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US_PRBS_MODEA_ERR_CNT_LSB);
          }
          else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
          {
            regAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_US_PRBS_MODEA_ERR_CNT_HI);
            regAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_US_PRBS_MODEA_ERR_CNT_LO);
          }
          else
          {
            regAddrHi = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_US_PRBS_MODEA_ERR_CNT_HI_TITAN);
            regAddrLo = MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_US_PRBS_MODEA_ERR_CNT_LO_TITAN);
          }
          maskHi = 0xffff;

          // 14.d: Extra step for Atlas
          if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_LATCH_CNT, 1);

          // 14.d: Extra step for Titan
          if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3)
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_LATCH_CNT_TITAN, 1);
          
          if ((MxLWare_HRCLS_OEM_ReadRegister(devId, regAddrLo, &readDataLo) == MXL_SUCCESS) &&
              (MxLWare_HRCLS_OEM_ReadRegister(devId, regAddrHi, &readDataHi) == MXL_SUCCESS))
          {
            *errCntPtr = (readDataLo | ((readDataHi & maskHi) << 16));
          }                                                                     

          // 14.d: Extra step for Atlas
          if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_LATCH_CNT, 0);

          // 14.d: Extra step for Titan
          if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_3)
            status |= MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_PRBS_MODEA_LATCH_CNT_TITAN, 0);

        } 
        else
        {
          *errCntPtr = 0;
        }
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_ReqSerDesUsSyncLockStatus 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/20/2011
--|                  
--| DESCRIPTION   : Request serdes upstream synchronization status 
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_ReqSerDesUsSyncLockStatus(
    UINT8   devId,
    MXL_BOOL_E* syncLockStatusPtr,
    UINT8*    syncLostCntPtr
    )
{
  UINT8 status = MXL_SUCCESS;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if ((devContextPtr) && ((syncLockStatusPtr) || (syncLostCntPtr)))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        UINT16 regData = 0;

        switch (devContextPtr->serDesType)
        {
          case MXL_HRCLS_SERDES_TYPE_1:
            if (devContextPtr->serDesUsPrbsMode == MXL_FALSE)
            {
              status = MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US2APB_SYNC), &regData);
              if (syncLockStatusPtr)
              {
                *syncLockStatusPtr = (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_US2APB_SYNC)))?MXL_TRUE:MXL_FALSE;
              }
              if (syncLostCntPtr)
              {
                *syncLostCntPtr = (UINT8) MxL_HRCLS_Ctrl_ExtractFieldFromRegister(regData, SERDES0_US_SYNCLOST_CNT);
              }
            }
            else
            {
              if (syncLockStatusPtr) *syncLockStatusPtr = MXL_FALSE;
              if (syncLostCntPtr) *syncLostCntPtr = 0;
            }
            break;

          case MXL_HRCLS_SERDES_TYPE_2:
            if (devContextPtr->serDesUsPrbsMode == MXL_FALSE)
            {
              status = MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US2APB_SYNC), &regData);
              if (syncLockStatusPtr)
              {
                *syncLockStatusPtr = (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_US2APB_SYNC)))?MXL_TRUE:MXL_FALSE;
              }
              if (syncLostCntPtr)
              {
                *syncLostCntPtr = (UINT8) MxL_HRCLS_Ctrl_ExtractFieldFromRegister(regData, SERDES0_US_SYNCLOST_CNT);
              }
            }
            else
            {
              if (MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_A2D_RX_DATA_VLD), &regData) == MXL_SUCCESS)
              {
                if (syncLockStatusPtr)
                {
                  if (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES_PHY0_A2D_RX_DATA_VLD)))
                  {
                    *syncLockStatusPtr = MXL_TRUE;
                  }
                  else
                  {
                    MXLERR(MxL_HRCLS_ERROR("[%d]SERDES_PHY0_A2D_RX_DATA_VLD is not set\n", __LINE__););
                    *syncLockStatusPtr = MXL_FALSE;
                  }
                }
              }
              else MXLERR(MxL_HRCLS_ERROR("[%d]Error reading SERDES_PHY0_A2D_RX_DATA_VLD\n", __LINE__););
            }
            break;

          case MXL_HRCLS_SERDES_TYPE_3:
            if (devContextPtr->serDesUsPrbsMode == MXL_FALSE)
            {
              status = MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES0_US2APB_SYNC_TITAN), &regData);
              if (syncLockStatusPtr)
              {
                *syncLockStatusPtr = (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES0_US2APB_SYNC_TITAN)))?MXL_TRUE:MXL_FALSE;
              }
              if (syncLostCntPtr)
              {
                *syncLostCntPtr = (UINT8) MxL_HRCLS_Ctrl_ExtractFieldFromRegister(regData, SERDES0_US_SYNCLOST_CNT_TITAN);
              }
            }
            else
            {
              /* Read SERDES_PHY0_A2D_RX_DATA_VLD_TITAN and perform similar operation as in MXL_HRCLS_SERDES_TYPE_2 */
              if (MxLWare_HRCLS_OEM_ReadRegister(devId, MxL_HRCLS_Ctrl_ExtractRegAddr(SERDES_PHY0_A2D_RX_DATA_VLD_TITAN), &regData) == MXL_SUCCESS)
              {
                if (syncLockStatusPtr)
                {
                  if (regData & (UINT16) (1 << MxL_HRCLS_Ctrl_ExtractRegLsbLoc(SERDES_PHY0_A2D_RX_DATA_VLD_TITAN)))
                  {
                    *syncLockStatusPtr = MXL_TRUE;
                  }
                  else
                  {
                    MXLERR(MxL_HRCLS_ERROR("[%d]SERDES_PHY0_A2D_RX_DATA_VLD_TITAN is not set\n", __LINE__););
                    *syncLockStatusPtr = MXL_FALSE;
                  }
                }
              }
              else MXLERR(MxL_HRCLS_ERROR("[%d]Error reading SERDES_PHY0_A2D_RX_DATA_VLD_TITAN\n", __LINE__););
            }
            break;
        }
        MXLDBG1(MxL_HRCLS_DEBUG("[HRCLS] Serdes US sync status: %s\nSync lost cnt: %d\n", (syncLockStatusPtr)?((*syncLockStatusPtr == MXL_TRUE)?"OK":"LOST"):"----", 
                                                                                            (syncLostCntPtr)?(*syncLostCntPtr):(-1)););
      }
      else
      {
        status = MXL_NOT_SUPPORTED;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return (MXL_STATUS_E) status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsSyncLostCntClear
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 12/29/2011
--|                  
--| DESCRIPTION   : Clear sync lost counter
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsSyncLostCntClear(
    UINT8   devId
    )
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1 || devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
          /* Atlas has same registers with Minos */
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_SYNCLOST_CNT_CLR);
        else
          status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_US_SYNCLOST_CNT_CLR_TITAN);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_API_CfgSerDesUsPhyPolarity 
--| 
--| AUTHOR        : Mariusz Murawski 
--|
--| DATE CREATED  : 04/18/2012
--|                  
--| DESCRIPTION   : Invert US phy
--|                 
--| RETURN VALUE  : MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
--|
--|-------------------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesUsPhyPolarity(UINT8 devId, MXL_BOOL_E phyInverted)
{
  MXL_STATUS_E status;
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("US phyInverted=%d\n", (phyInverted==MXL_TRUE)?1:0););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1)
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES0_REG_PHY_RX_INVERT, (phyInverted == MXL_ENABLE)?1:0);
        else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
        {
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_POLAR_INV, (phyInverted == MXL_ENABLE)?1:0);
        }
        else
          status = MxL_HRCLS_Ctrl_UpdateSerdesRegisterField(devId, MXL_HRCLS_SERDES_LANE0, SERDES_PHY0_REG_US_POLAR_INV_TITAN, (phyInverted == MXL_ENABLE)?1:0);
      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);
  return status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  enable Enable or disable upstream hardware
 *
 *  @apibrief   This API configures up stream
 *
 *  @usage      TBD
 *
 *  @equ261     None
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgDevUpStreamParams(
    UINT8     devId,
    MXL_HRCLS_TXDAC_STATE_E  enable,
    MXL_HRCLS_TXDAC_OUT_SWING_E outputSwing
    )
{
  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);
  UINT8 status = MXL_SUCCESS;

  MXLENTERAPISTR(devId);
  MXLENTERAPI(MxL_HRCLS_DEBUG("enable=%d, swing=%d\n", (UINT8) enable, (UINT8) outputSwing););
  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesDSLanesCnt)
      {
        HOST_COMMAND_T cfgDevUsParams;
        UINT8 enableHw;
        UINT8 outputSwingHw;

        enableHw = (enable == MXL_HRCLS_TXDAC_DISABLE)?MXL_HRCLS_HW_TXDAC_DISABLE:
                   (enable == MXL_HRCLS_TXDAC_ENABLE)?MXL_HRCLS_HW_TXDAC_ENABLE:MXL_HRCLS_HW_TXDAC_EXTERNAL;
        outputSwingHw = (outputSwing == MXL_HRCLS_TXDAC_OUT_MIN)?MXL_HRCLS_HW_TXDAC_SWING_MIN:
                        (outputSwing == MXL_HRCLS_TXDAC_OUT_MAX)?MXL_HRCLS_HW_TXDAC_SWING_MAX:MXL_HRCLS_HW_TXDAC_SWING_DEFAULT;

        /* Form command payload */
        cfgDevUsParams.data[0]    = (((UINT8)enableHw      << 0) & 0x03) 
                                  | (((UINT8)outputSwingHw << 2) & 0x0C);
        cfgDevUsParams.payloadLen = 1;
        cfgDevUsParams.syncChar   = 0;
        cfgDevUsParams.commandId  = 0;
        cfgDevUsParams.payloadCheckSum = 0;

        /* Send host command */
        status = MxL_HRCLS_Ctrl_SendHostCommand(devId, &cfgDevUsParams, HOST_CMD_CFG_DEV_UPSTREAM_PARAMS, MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM);

        /* Wait for some time (20ms) for host command to complete */
        MxLWare_HRCLS_OEM_DelayUsec(20*1000);

      } else status = MXL_NOT_SUPPORTED;
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E) status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  serDesId Serdes lane ID that should be reset 
 *
 *  @apibrief   This API reset serdes PHY 
 *
 *  @usage      Should be called only if there is a problem with establish stable 
 *              connection between Hercules device and backend.
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgSerDesPhyReset(UINT8 devId, MXL_HRCLS_SERDES_LANE_ID_E serDesId)
{
  UINT8 status = MXL_SUCCESS;

  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);
  MXL_HRCLS_SERDES_SWAP(serDesId);
  if ((devContextPtr) && (serDesId < devContextPtr->serDesDSLanesCnt))
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_1)
        status = MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, serDesId, SERDES0_REG_PHY_BPR_RESET);
      else if (devContextPtr->serDesType == MXL_HRCLS_SERDES_TYPE_2)
      {
#ifdef _MXL_HRCLS_SERDES_ENABLE_RX_CDR_RESET_
        status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_RESETBAR);
        status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitOneZero(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_RESETBAR);
        status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(devId, serDesId, SERDES_PHY0_D2A_RX_CDR_RESETBAR);
        status |= MxL_HRCLS_Ctrl_ToggleSerdesRegisterBitZeroOne(devId, serDesId, SERDES_PHY0_D2A_RX_DSRLZR_RESETBAR);
#endif
      }
      else
      {
#ifdef _MXL_HRCLS_SERDES_ENABLE_RX_CDR_RESET_
        // 10/22/2014: ChauD.  PHY reset should be removed.
//        status |= MxL_HRCLS_Ctrl_PowerdownSerdes(devContextPtr, serDesId);
#endif
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;
  MXLEXITAPISTR(devId,status);
  return (MXL_STATUS_E)status;
}

/**
 *****************************************************************************************
 *  @param[in]  devId MxL device id
 *  @param[in]  usBandEdge - Specified the upstream band edge frequency.
 *
 *  @apibrief   This API sets the Upstream band frequency edge used for optimal AGC performance.
 *
 *  @usage      This should be called any time after firmware download before channel tune.
 *
 *  @return     MXL_SUCCESS, MXL_FAILURE, MXL_NOT_INITIALIZED, MXL_INVALID_PARAMETER
 ****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_API_CfgUsBandEdge(UINT8 devId,  MXL_HRCLS_UPSTREAM_BANDEDGE_E usBandEdge)
{
  UINT8 status = MXL_SUCCESS;

  MXL_HRCLS_DEV_CONTEXT_T * devContextPtr = MxL_HRCLS_Ctrl_GetDeviceContext(devId);

  MXLENTERAPISTR(devId);

  if (devContextPtr)
  {
    if (devContextPtr->driverInitialized)
    {
      if (devContextPtr->firmwareStatus == FIRMWARE_STATUS_LOADED)
      {
        switch(usBandEdge) {
          case MXL_HRCLS_UPSTREAM_BANDEDGE_42MHZ:
            status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_DOCSIS_US_BAND_EDGE_IN_MHZ, 42);
            break;
          case MXL_HRCLS_UPSTREAM_BANDEDGE_65MHZ:
            status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_DOCSIS_US_BAND_EDGE_IN_MHZ, 65);
            break;
          case MXL_HRCLS_UPSTREAM_BANDEDGE_85MHZ:
            status |= MxLWare_HRCLS_OEM_WriteRegister(devId, MAILBOX_REG_DOCSIS_US_BAND_EDGE_IN_MHZ, 85);
            break;
          default:
            MXLERR(MxL_HRCLS_ERROR("Invalid Upstream Frequency Band Edge Specified\n");); 
            status = MXL_INVALID_PARAMETER;
            break;
        }
      } else {
        MXLERR(MxL_HRCLS_ERROR("Upstream Band Edge Configuration is not supported without firmware loaded. Load firmware first\n");); 
        status = MXL_NOT_SUPPORTED;
      }
    } else status = MXL_NOT_INITIALIZED;
  } else status = MXL_INVALID_PARAMETER;

  MXLEXITAPISTR(devId, status);
  return (MXL_STATUS_E)status;
}

#endif // _MXL_HRCLS_SERDES_ENABLED_

