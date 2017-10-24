////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

#ifndef _AUTH_H_
#define _AUTH_H_

void Secure_SPI_Init(void);
void Secure_Init(void);
void Secure_Kernel_Authentication(U32 u32kernelAddr);
void Secure_Application_Authentication(void);

void Secure_USB_KL_Upgrade(void);
void Secure_USB_CK_Upgrade(void);

void Secure_GetSSKey(void);

#endif //_AUTH_H_

