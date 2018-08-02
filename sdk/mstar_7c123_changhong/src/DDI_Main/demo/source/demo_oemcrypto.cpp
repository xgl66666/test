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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
///
/// @file   appDemo_OEMCrypto.cpp
/// @author MStar Semiconductor Inc.
/// @brief  OEM Crypto demo code
///
/// @name TV App Command Usage
//-------------------------------------------------------------------------------------------------
#if (DEMO_DRM_TEST == 1)
#include "OEMCrypto.h"
#include <iostream>
#include <string.h>

#include "vectors.h"
#include "demo_oemcrypto.h"

using namespace std;
using namespace WV;

static int sFailCount = 0;

static void fail()
{
    sFailCount++;
}

static void DumpTestVector(OEMCrypto_UINT8 const *array, size_t nelem)
{
    for (size_t i = 0; i < nelem; i++)
    {
        char buf[4];
        sprintf(buf, "%02x ", array[i]);
        cout << buf;
    }
    cout << endl;
}

static void TestKeyboxGood()
{
    // Note - you can create a 128-byte binary test keybox from this data with 'xxd -r -p'
    OEMCrypto_UINT8 goodKeybox[] = {
        0x74, 0x65, 0x73, 0x74, 0x69, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xbf, 0x3d, 0xc5, 0x48, 0xcc, 0xdd, 0x1c, 0x71, 0xd0, 0xe2, 0xea, 0x60, 0xdc, 0x0b, 0xc9, 0x81,
        0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4e, 0x84, 0xc0, 0x23, 0x18, 0x89, 0xc2, 0x85, 0xc4,
        0x0a, 0x61, 0x82, 0x67, 0x73, 0xca, 0x3b, 0xaa, 0x47, 0xc3, 0xd0, 0xc7, 0xa2, 0x4d, 0xd5, 0x65,
        0x90, 0x45, 0xf4, 0x1a, 0x85, 0x1d, 0x64, 0xde, 0xb0, 0xf4, 0xb7, 0xb2, 0xaa, 0x39, 0x7f, 0x86,
        0xd8, 0x26, 0xe4, 0x6f, 0xfd, 0x07, 0x89, 0xd0, 0x23, 0xb7, 0xb7, 0xf2, 0xa8, 0x3c, 0xa4, 0x1d,
        0x04, 0xc1, 0x0c, 0x64, 0xd7, 0xf2, 0x2e, 0xe7, 0x6b, 0x62, 0x6f, 0x78, 0xa0, 0x79, 0xbd, 0xaa
    };

    OEMCryptoResult result;

    // Attempt to install the keybox. If this installation fails, we proceed anyway, so that
    // we can test on factory-provisioned devices that don't implement
    // OEMCrypto_InstallKeybox() but still have some way to provision with the test keybox.
    cout << "calling OEMCrypto_InstallKeybox with encrypted good keybox" << endl;
    result = OEMCrypto_InstallKeybox(goodKeybox, sizeof(goodKeybox));
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_EncryptAndStoreKeyBox failed (this is ok for factory-provisioned devices): " << (int)result << endl;
    }

    OEMCryptoResult OEMCrypto_GetKeyData(OEMCrypto_UINT8* keyData, OEMCrypto_UINT32 keyDataLength);
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "Keydata invalid: " << (int)result << endl;
        fail();
    }

    OEMCryptoResult OEMCrypto_GetDeviceID(OEMCrypto_UINT8* deviceID, OEMCrypto_UINT32 idLength);
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_GetDeviceID failed: " << (int)result << endl;
        fail();
    }
    else
    {
        cout << "Keybox is valid" << endl;
    }
}

static void TestDecrypt(const TestVectors &vectors, size_t index, bool inPlace)
{
    cout << "TestDecrypt[" << index << "]" << endl;

    vector<OEMCrypto_UINT8> zero_iv =  vectors.GetIV(0);
    vector<OEMCrypto_UINT8> iv =  vectors.GetIV(index);
    vector<OEMCrypto_UINT8> encryptedData = vectors.GetEncryptedData(index);
    vector<OEMCrypto_UINT8> clearData = vectors.GetClearData(index);

    OEMCryptoResult result;
    OEMCrypto_UINT32 nbytes = encryptedData.size();
    cout << "nbytes=" << nbytes << endl;

    vector<OEMCrypto_UINT8> decryptedData(nbytes);

    // The OEMCrypto API does not allow us to pass in an explicit IV, only
    // a flag indicating whether to reset the IV or not. However, the test
    // data is set up so that each (nonzero) IV is in fact the IV left over
    // from the last decryption.
    OEMCrypto_UINT8 *src = encryptedData.data();
    if (inPlace)
    {
        decryptedData = encryptedData;
        src = decryptedData.data();
    }

    result = OEMCrypto_DecryptCTS(src, decryptedData.data(), encryptedData.size(), iv == zero_iv);
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_DecryptCTS failed: " << (int)result << endl;
        fail();
    }

    if (clearData != decryptedData)
    {
        cerr << "Data mismatch in decryptedData in TestDecrypt(" << index << ")" << endl;
        cerr << "clearData: "; DumpTestVector(clearData.data(), clearData.size());
        cerr << "decryptedData: "; DumpTestVector(decryptedData.data(), decryptedData.size());
        fail();
    }
    else
    {
        cerr << "TestDecrypt(" << index << ") " << "successfully" << endl;
    }
}

static void TestGetDeviceID()
{
    OEMCrypto_UINT8 deviceId[33];
    OEMCryptoResult result;

    // Extra byte to check for running over buffer
    deviceId[32] = 0xee;

    cout << "calling OEMCrypto_GetDeviceID" << endl;

    OEMCrypto_UINT32 deviceIdLength = 0;
    result = OEMCrypto_GetDeviceID(deviceId, &deviceIdLength);
    if (result != OEMCrypto_ERROR_SHORT_BUFFER)
    {
        cerr << "OEMCrypto_GetDeviceID failed to check buffer length" << endl;
        return;
    }
    if (deviceIdLength != 32)
    {
        cerr << "OEMCrypto_GetDeviceID returned wrong buffer size" << endl;
        return;
    }
    result = OEMCrypto_GetDeviceID(deviceId, &deviceIdLength);
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_GetDeviceID failed: " << (int)result << endl;
        fail();
    }
    if (deviceIdLength > sizeof(deviceId))
    {
        cerr << "OEMCrypto_GetDeviceID returned wrong number of bytes" << endl;
        return;
    }
    if (deviceId[32] != 0xee)
    {
        cerr << "OEMCrypto_GetDeviceID wrote past end of buffer" << endl;
        return;
    }
    cout << "Device ID is: " << deviceId << endl;
}

void TestOEMCrypto(const TestVectors &vectors)
{
    cout << "TestOEMCrypto" << endl;

    cout << "calling OEMCrypto_Initialize" << endl;

    OEMCryptoResult result;
    result = OEMCrypto_Initialize();
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_Initialize failed: " << (int)result << endl;
        fail();
    }

    TestKeyboxGood();
    TestGetDeviceID();

    OEMCrypto_UINT8 keyData[72] = {};
    OEMCrypto_UINT32 keyDataLength = sizeof(keyData);
    result = OEMCrypto_GetKeyData(keyData, &keyDataLength);

    if (keyDataLength != sizeof(keyData))
    {
        cerr << "OEMCrypto_GetKeyData returned wrong number of bytes" << endl;
        return;
    }

    cout << "KeyData: " ; DumpTestVector(keyData, sizeof(keyData));

    vector<OEMCrypto_UINT8> encryptedAssetKey = vectors.GetEncryptedAssetKey();

    cout << "calling OEMCrypto_SetEntitlementKey" << endl;

    result = OEMCrypto_SetEntitlementKey(&encryptedAssetKey[0], encryptedAssetKey.size());
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_SetEntitlementKey failed: " << (int)result << endl;
        fail();
    }

    vector<OEMCrypto_UINT8> encryptedEcm = vectors.GetEncryptedEcm();

    cout << "calling OEMCrypto_DeriveControlWord" << endl;

    OEMCrypto_UINT32 flags;
    result = OEMCrypto_DeriveControlWord(&encryptedEcm[0], encryptedEcm.size(), &flags);
    if (result != OEMCrypto_SUCCESS)
    {
        cerr << "OEMCrypto_DeriveControlWord failed: " << (int)result << endl;
        fail();
    }

    if (flags != vectors.GetFlags())
    {
        cerr << "Flags mismatch: 0x" << hex << flags << " expected 0x" << vectors.GetFlags() << dec << endl;
        fail();
    }

    for (size_t i = 0; i < vectors.GetVectorCount(); i++)
    {
        TestDecrypt(vectors, i, false);
    }

    for (size_t i = 0; i < vectors.GetVectorCount(); i++)
    {
        TestDecrypt(vectors, i, true);
    }
}


int Demo_DRM_OEMCryptoTest(void)
{
    WV::TestVectors vectors;
    TestOEMCrypto(vectors);

    if (sFailCount == 0)
    {
        cout << "Test completed successfully!" << endl;
        return 1;
    }
    else
    {
        cout << "Test completed with " << sFailCount << " failures." << endl;
        sFailCount = 0;
        return 0;
    }
}
#endif

