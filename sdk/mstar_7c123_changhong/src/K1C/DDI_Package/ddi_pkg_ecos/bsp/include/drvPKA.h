//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvPKA.h
/// @brief  PKA Driver Interface
/// @author MStar Semiconductor,Inc.
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVPKA_H_
#define _DRVPKA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#define PKA_MAX_BIGVALUE_ARRAY_SIZE     (130)
#define PKA_MAX_POINTCURVE_ARRAY_SIZE   (24)

/** PKA related functions */
typedef struct DLL_PACKED
{
    MS_U32 au32Value[PKA_MAX_BIGVALUE_ARRAY_SIZE]; // little endian
    MS_U32 u32Size;        // number of elements used in value
}DrvPKA_PKAValue;

typedef struct DLL_PACKED
{
    MS_U32 au32X[PKA_MAX_POINTCURVE_ARRAY_SIZE];      // little endian
    MS_U32 au32Y[PKA_MAX_POINTCURVE_ARRAY_SIZE];      // little endian
    MS_U32 u32Size;        // number of elements used in x/y, must not exceed prime modulus p's size
}DrvPKA_ECCPoint;

/* an elliptic curve is defined by p,a,b */
typedef struct DLL_PACKED
{
    MS_U32 au32P[PKA_MAX_POINTCURVE_ARRAY_SIZE];            // little endian, prime modulus p which > 2^63, the highest word indicated by u32Size may not be zero
    MS_U32 au32A[PKA_MAX_POINTCURVE_ARRAY_SIZE];            // little endian, a < p
    MS_U32 au32B[PKA_MAX_POINTCURVE_ARRAY_SIZE];            // little endian, b < p
    MS_U32 au32Order[PKA_MAX_POINTCURVE_ARRAY_SIZE];        // order
    MS_U32 au32Gx[PKA_MAX_POINTCURVE_ARRAY_SIZE];           // base point x
    MS_U32 au32Gy[PKA_MAX_POINTCURVE_ARRAY_SIZE];           // base point y
    MS_U32 u32Size;              // number of elements used in p
}DrvPKA_ECCCurve;

typedef struct DLL_PACKED
{
    MS_U32 au32R[PKA_MAX_BIGVALUE_ARRAY_SIZE];     // little endian
    MS_U32 au32S[PKA_MAX_BIGVALUE_ARRAY_SIZE];     // little endian
    MS_U32 u32Size;        // number of elements used in R/S
}DrvPKA_ECCSignature;

MS_BOOL MDrv_PKA_Init(void);

// Operation: Elliptic curve affine point addition in prime domain
// P1_xy + P2_xy -> P0_xy, prime curve: y^2 = x^3 + ax + b (mod p)
// Neither P1_xy nor P2_xy can be the "point-at-infinity"
// P1_xy and P2_xy must be on the curve
MS_BOOL MDrv_PKA_ECCPointAdd(DrvPKA_ECCCurve* pstCurve, DrvPKA_ECCPoint* pstPoint1, DrvPKA_ECCPoint* pstPoint2, DrvPKA_ECCPoint* pstResult);

// Operation: Elliptic curve affine point multiplication in prime domain
// k * P1_xy -> P0_xy, optional randomization factor Rz, prime curve: y^2 = x^3 + ax + b (mod p)
// 1 <= Rz < p
// 1 < k <= n (curve's order)
// P1_xy cannot be the "point-at-infinity"
// P1_xy must be on the curve
// p, k < 2^768
MS_BOOL MDrv_PKA_ECCPointMul(DrvPKA_ECCCurve* pstCurve, DrvPKA_ECCPoint* pstPoint, DrvPKA_PKAValue* pstScalar, DrvPKA_PKAValue* pstRandFact, DrvPKA_ECCPoint* pstResult);


// Operation: modular exponentiation
MS_BOOL MDrv_PKA_ModExp(DrvPKA_PKAValue* pstBase, DrvPKA_PKAValue* pstExponent, DrvPKA_PKAValue* pstModulus, MS_U32 pstShift, DrvPKA_PKAValue* pstResult);

// Operation: modular exponentiation with Chinese Remainders Theorem
MS_BOOL MDrv_PKA_ModExpCRT(DrvPKA_PKAValue* pstExpP, DrvPKA_PKAValue* pstExpQ, DrvPKA_PKAValue* pstModP, DrvPKA_PKAValue* pstModQ, DrvPKA_PKAValue* pstQInv, DrvPKA_PKAValue* pstInput, MS_U32 pstShift, DrvPKA_PKAValue* pstResult);

// Operation: modular inversion
// A: input, N: modulus, R: result
// 1 / A (mod N) -> R
// N must be odd
// N != 1
// The highest word of the modulus may not be zero
// input, N < 2^4160
MS_BOOL MDrv_PKA_ModInv(DrvPKA_PKAValue* pstInput, DrvPKA_PKAValue* pstModulus, DrvPKA_PKAValue* pstResult);

// Operation: Modulo
// A: dividend, B: modulus, R: result
// A mod B -> R
// size of A must larger than size of B
// The highest word of the modulus may not be zero
// A < 2^4160
MS_BOOL MDrv_PKA_Mod(DrvPKA_PKAValue* pstDividend, DrvPKA_PKAValue* pstModulus, DrvPKA_PKAValue* pstResult);

// Operation: Big number addition
// A + B - C -> D
// A, B: addend , C: subtrahend, D: result
// A, B, C < 2^4160, A + B must >= C
MS_BOOL MDrv_PKA_AddSub(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstOpnd3, DrvPKA_PKAValue* pstResult);

// Operation: Big number addition
// A + B -> C
// A, B: addend , C: result
// A, B < 2^4160
MS_BOOL MDrv_PKA_Add(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstResult);

// Operation: Big number subtraction
// A - B -> C
// A: minuend, B: subtrahend, C: result
// A, B < 2^4160
// A must >= B
MS_BOOL MDrv_PKA_Sub(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstResult);

// Operation: Big number multiplication
// A * B -> C
// A: multiplicand, B: multiplier, C: result
// C < 2^4160
MS_BOOL MDrv_PKA_Mul(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstResult);

// Operation: Big number multiplication
// A / B -> Q, R
// A: Dividend, B: Divisor, Q: Quotient, R: Remainder
// size of A must larger than size of B
// The highest word of the divisor may not be zero
// A < 2^4160
MS_BOOL MDrv_PKA_Div(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstQuotient, DrvPKA_PKAValue* pstReminder);

// Operation: Elliptic Curve Digital Signature generation
// (curve, G, n, Rb, priKey, HASH(msg), Rz) -> (r, s)
// curve: defined by p, a, b
// G: base point
// n: integer order of G, means that n * G = O
MS_BOOL MDrv_PKA_ECDSASign(DrvPKA_ECCCurve* pstCurve, DrvPKA_PKAValue* pstBaseRandomizer, DrvPKA_PKAValue* pstPrivateKey, DrvPKA_PKAValue* pstMsgHash, DrvPKA_PKAValue* pstRandomFactor, DrvPKA_ECCSignature* pstSignature);

// Operation: Elliptic Curve Digital Signature verification
// (curve, G, n, Rb, pubKey, Rpub, HASH(msg), (r, s)) -> (r', s')
// curve: defined by p, a, b
// G: base point
// n: integer order of G, means that n * G = O
// Output: *bValid = TRUE if signature is valid, elase FALSE
MS_BOOL MDrv_PKA_ECDSAVerify(DrvPKA_ECCCurve* pstCurve, DrvPKA_PKAValue* pstBaseRandomizer, DrvPKA_ECCPoint* pstPublicKey, DrvPKA_PKAValue* pstKeyRandomizer, DrvPKA_PKAValue* pstMsgHash, DrvPKA_ECCSignature* pstSignature, MS_BOOL* bValid);

// Operation: Digital Signature generation
MS_BOOL MDrv_PKA_DSASign(DrvPKA_PKAValue* pstBase, DrvPKA_PKAValue* pstModP, DrvPKA_PKAValue* pstModN, DrvPKA_PKAValue* pstPrivateKey, DrvPKA_PKAValue* pstMsgHash, DrvPKA_PKAValue* pstRandomFactor, MS_U32 pstShift, DrvPKA_ECCSignature* pstSignature);

// Operation: Digital Signature verification
MS_BOOL MDrv_PKA_DSAVerify(DrvPKA_PKAValue* pstBase, DrvPKA_PKAValue* pstModP, DrvPKA_PKAValue* pstModN, DrvPKA_PKAValue* pstPublicKey, DrvPKA_PKAValue* pstMsgHash, MS_U32 pstShift, DrvPKA_ECCSignature* pstSignature, MS_BOOL* bValid);

#ifdef __cplusplus
}
#endif


#endif // _DRVPKA_H_
