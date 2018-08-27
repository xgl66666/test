///////////////////////////////////////////////////////////////////////////////////////////////////
//
// * Copyright (c) 2006 - 2007 Mstar Semiconductor, Inc.
// This program is free software.
// You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   EMAC.h
/// @author MStar Semiconductor Inc.
/// @brief  EMAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------
// Linux EMAC.h define start
// -----------------------------------------------------------------------------

#ifndef __DRV_EMAC_API_H_
#define __DRV_EMAC_API_H_


#ifdef __cplusplus
extern "C"
{
#endif

/// EMAC DDI return value
typedef MS_U32                                 DRVEMAC_RESULT;
/// @name DRVEMAC_RESULT
/// @ref DRVEMAC_RESULT
/// return value
/// @{
#define DRVEMAC_OK                          0x00000000
#define DRVEMAC_FAIL                        0x00000001

/// EMAC OID
typedef MS_U32                                 EMAC_OID;
enum {
    EOID_LINK_SPEED                       = 0x00000001  ,   // 4B
    EOID_MEDIA_CONNECT_STATUS                           ,   // 4B
    EOID_MAC_ADDRESS                                    ,   // 6B
    EOID_DUPLEX                                         ,   // 4B
    EOID_XMIT_OK                                        ,   // 4B
    EOID_RCV_OK                                         ,   // 4B
    EOID_XMIT_ERROR                                     ,   // 4B
    EOID_RCV_ERROR                                      ,   // 4B
    EOID_RCV_NO_BUFFER                                  ,   // 4B
};
#define EMAC_LINK_SPEED_10                  10
#define EMAC_LINK_SPEED_100                 100
#define EMAC_CONN_STATUS_CONNECTED          1
#define EMAC_CONN_STATUS_DISCONNECTED       0
#define EMAC_DUPLEX_HALF                    1
#define EMAC_DUPLEX_FULL                    2

//EMAC link status
#define EMAC_LS_WAIT   0x80
//Link: 0=unlink, 1=link
#define EMAC_LS_LINK   0x04
//Speed: 0=10Mb, 1=100Mb
#define EMAC_LS_SPEED  0x02
//Duplex: 0=half, 1=full
#define EMAC_LS_DUPLEX 0x01
// Function Declaration -----------------------------------------------------
int CheckEMAC_Initialized(void);

// Four main functions:
DRVEMAC_RESULT  MDrv_EMAC_Init(void);
DRVEMAC_RESULT  QueryInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xread);
DRVEMAC_RESULT  SetInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xwrit);

/* matt.lin(20071018) */
void MDrv_EMAC_ChangeMacAddr(MS_U8*);

MS_U8* drvEMAC_GetMacAddr(void);
void init_all_network_interfaces(void);

// 20071029_CHARLES : Power off mode APIs
void EMAC_PowerOff(void);
void EMAC_PowerOff_Set(void);
void EMAC_PowerOn(void);
void EMAC_PowerOn_Set(void);
void EMAC_Suspend(void);
void EMAC_Resume(void);

// for HB
MS_U8 EMAC_PhyAddrScan(void);
extern MS_BOOL (*PHY_ConnectStatus)(void);

#ifdef __cplusplus
}
#endif

#endif
