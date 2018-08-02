#ifndef VECTORS_H
#define VECTORS_H

#include "stdlib.h"
#include <vector>
#include <string.h>
#include <stdio.h>

#include "OEMCrypto.h"

using std::vector;

namespace WV {

class TestVectors
{
public:
    vector<OEMCrypto_UINT8> GetDeviceKey() const;
    vector<OEMCrypto_UINT8> GetEncryptedAssetKey() const;
    vector<OEMCrypto_UINT8> GetClearAssetKey() const;
    vector<OEMCrypto_UINT8> GetEncryptedEcm() const;
    OEMCrypto_UINT32 GetFlags() const;
    vector<OEMCrypto_UINT8> GetClearEcm() const;
    vector<OEMCrypto_UINT8> GetClearContentKey()const ;
    size_t GetVectorCount() const;
    vector<OEMCrypto_UINT8> GetIV(size_t index) const;
    vector<OEMCrypto_UINT8> GetEncryptedData(size_t index) const;
    vector<OEMCrypto_UINT8> GetClearData(size_t index) const;
};

}

#endif
