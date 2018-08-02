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
//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///@file apiTTX.h
///@brief  Teletext User Interface, header file for release
///@author MStarSemi Inc.
//////////////////////////////////////////////////////////////////////////////
///@section WriteCMD SECTION 1
///@section TT_API_Write  Teletext User Interface Detail Descriptions - Write Commands
///Here providing the interface for sending commands or settings to Teletext
///@par INDEX
///@ref WS1 @ref Digitkey
///@n @ref WS2 @ref UPDOWN
///@n @ref WS3 @ref COLORKEY
///@n @ref WS4 @ref INDEXKEY
///@n @ref WS5 @ref CLOCKMODE
///@n @ref WS6 @ref MIXMODE
///@n @ref WS7 @ref UPDATEMODE
///@n @ref WS8 @ref HOLDMODE
///@n @ref WS9 @ref SIZEMODE
///@n @ref WS10 @ref REVEALMODE
///@n @ref WS11 @ref LISTMODE
///@n @ref WS12 @ref SETLIST
///@n @ref WS13 @ref SUBCODEIN
///@n @ref WS14 @ref SetLANG
///@n @ref WS15 @ref SetOSD
///@n @ref WS16 @ref InitTTX
///@n @ref WS17 @ref ResetTTX
///@n @ref WS18 @ref TTXON
///@n @ref WS19 @ref TTXOFF
///@n @ref WS20 @ref TTXTV
///@n @ref WS21 @ref VIDEOSYSTEM
///@n @ref WS22 @ref SETSOURCE
///@n @ref WS23 @ref CHANGEPAGE
///@n @ref WS24 @ref ACIEnable
///@n @ref WS25 @ref ACIDisable
///@n @ref WS26 @ref ACISelectMenuItem
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///@n
///@n
///@section WS1 SECTION 1-1
///@section Digitkey Digit Keys: Teletext User Command 00 ~ 09
///
///@link #TT_User_Command
///      TT_User_Command @endlink=
///- TTX_CMD_DIGIT0
///- TTX_CMD_DIGIT1
///- TTX_CMD_DIGIT2
///- TTX_CMD_DIGIT3
///- TTX_CMD_DIGIT4
///- TTX_CMD_DIGIT5
///- TTX_CMD_DIGIT6
///- TTX_CMD_DIGIT7
///- TTX_CMD_DIGIT8
///- TTX_CMD_DIGIT9
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Input digit 0 to teletext
///  TT_WriteCommand(TTX_CMD_DIGIT0, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS2 SECTION 1-2
///@section UPDOWN PAGE UP/DOWN: Teletext User Command 10 and 11
///
///@link #TT_User_Command
///      TT_User_Command @endlink=
///- TTX_CMD_PAGEUP : Page up
///- TTX_CMD_PAGEDOWN : Page down
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Teletext Page Up
///  TT_WriteCommand(TTX_CMD_PAGEUP, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS3 SECTION 1-3
///@section COLORKEY Color Key Link: Teletext User Command 12 ~ 15
///-# Link to the specific page.
///-# In "List mode": Link to the specific page defined by user last time or default, and  teletext decoder
///will keep the incoming 3 digits as new link page number for Red/Green/Yellow/Cyon key.
///-# In "Subcode Input mode":
///- TT_User_Command = TTX_CMD_RED : Increase the subcode sequencially. (+)
///- TT_User_Command = TTX_CMD_GREEN : Decrease the subcode sequencially. (-)
///
///@link #TT_User_Command
///      TT_User_Command @endlink=
///- TTX_CMD_RED
///- TTX_CMD_GREEN
///- TTX_CMD_YELLOW
///- TTX_CMD_CYAN
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Link to Red color key
///  TT_WriteCommand(TTX_CMD_RED, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS4 SECTION 1-4
///@section INDEXKEY Index Key: Teletext User Command 16
///-Link to index page (Specific page number will be included in teletext data, Or set to default index page 100.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_INDEX
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Link to index page
///  TT_WriteCommand(TTX_CMD_INDEX, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS5 SECTION 1-5
///@section CLOCKMODE Clock Mode: Teletext User Command 17
///-Enable/Disable Clock mode when \b Teletext \b is \b in \b TV \b MODE
///(When enable "Clock mode, there will be a small clock strip on the screen if Clock information is included in teletext signal.)
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_CLOCK
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable Clock mode
///- TT_DEC_ON : Enable Clock mode
///- TT_DEC_TOGGLE : Toggle Clock mode (Clock mode Enable  -->Disable --> Enable .....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //Enter Clock mode
///  TT_WriteCommand(TTX_CMD_CLOCK, TT_DEC_ON , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS6 SECTION 1-6
///@section MIXMODE MIX Mode: TT_User_Command 18
///-Enable/Disable MIX mode
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_MIX
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable MIX mode
///- TT_DEC_ON : Enable MIX mode
///- TT_DEC_TOGGLE : Toggle MIX mode (MIX mode Enable --> Disable --> Enable .....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //Disable MIX mode
///  TT_WriteCommand(TTX_CMD_MIX, TT_DEC_OFF , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS7 SECTION 1-7
///@section UPDATEMODE Update Mode: TT_User_Command 19
///- Enable/Disable Update mode
///- Entering UPDATE mode: there is only "Page number" displayed in black with a yellow background on
///the left upper corner of the display screen when current page is \b NOT \b updated.
///- Entering UPDATE mode: there is only "Page number" displayed in white with a black background on
///the left upper corner of the display screen when current page is \b updated.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_UPDATE
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable Update mode
///- TT_DEC_ON : Enable Update mode
///- TT_DEC_TOGGLE : Toggle Update mode (Update mode ENABLE--> DISABLE --> ENABLE....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //First time calling with command - Enter UPDATE mode
///  TT_WriteCommand(TTX_CMD_UPDATE, TT_DEC_TOGGLE, 0);
///  //Disable UPDATE mode when it's in UPDATE mode currently
///  TT_WriteCommand(TTX_CMD_UPDATE, TT_DEC_TOGGLE, 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS8 SECTION 1-8
///@section HOLDMODE HOLD Mode: TT_User_Command 20
///-Enable/Disable HOLD mode
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_HOLD
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable HOLD mode
///- TT_DEC_ON : Enable HOLD mode
///- TT_DEC_TOGGLE : Toggle HOLD mode (HOLD mode ENABLE--> DISABLE --> ENABLE....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //Enter HOLD mode
///  TT_WriteCommand(TTX_CMD_HOLD, TT_DEC_ON, 0);
///  //Disable HOLD mode
///  TT_WriteCommand(TTX_CMD_HOLD, TT_DEC_OFF, 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS9 SECTION 1-9
///@section SIZEMODE SIZE Mode: TT_User_Command 21
///-Enable/Disable SIZE mode
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_SIZE
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable Size mode  (Display in Normal Size)
///- TT_DEC_ON : Enable Size mode (Display High half page in double size)
///- TT_DEC_TOGGLE : Toggle Size mode
///@n   (ZOOM In for HIGH HALF page --> ZOOM In for LOW HALF page --> Disable Size mode (Normal Size))
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //First time calling with command - ZOOM In for HIGH HALF page
///  TT_WriteCommand(TTX_CMD_SIZE, TT_DEC_TOGGLE, 0);
///  //Second time calling with command - ZOOM In for LOW HALF page
///  TT_WriteCommand(TTX_CMD_SIZE, TT_DEC_TOGGLE , 0);
///  //3nd time calling with command - Disable SIZE mode (Normal size)
///  TT_WriteCommand(TTX_CMD_HOLD, TT_DEC_TOGGLE , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS10 SECTION 1-10
///@section REVEALMODE REVEAL Mode: TT_User_Command 22
///-Enable/Disable REVEAL mode
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_REVEAL
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable REVEAL mode
///- TT_DEC_ON : Enable REVEAL mode
///- TT_DEC_TOGGLE : Toggle REVEAL mode (REVEAL mode ENABLE--> DISABLE --> ENABLE....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need parameter 2, just set to 0
///
///@par Example
///@code
///  //Disable REVEAL mode
///  TT_WriteCommand(TTX_CMD_REVEAL, TT_DEC_OFF, 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS11 SECTION 1-11
///@section LISTMODE LIST Mode: TT_User_Command 23
///-Enable/Disable LIST mode
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_LIST
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable LIST mode
///- TT_DEC_ON : Enable LIST mode
///- TT_DEC_TOGGLE : Toggle LIST mode (LIST mode ENABLE--> DISABLE --> ENABLE....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need parameter 2, just set to 0
///
///@par Example
///@code
///  //Enter LIST mode
///  TT_WriteCommand(TTX_CMD_LIST, TT_DEC_ON , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS12 SECTION 1-12
///@section SETLIST Set LIST Page: Teletext User Command 24 ~ 27
///-# Set specific page number (or user preference page number) for Color key (R/G/Y/C)
///
///@link #TT_User_Command
///      TT_User_Command @endlink=
///- TTX_CMD_SET_LIST_R: Set "Red key LIST Link page Number"
///- TTX_CMD_SET_LIST_G: Set "Green key LIST Link page Number"
///- TTX_CMD_SET_LIST_Y: Set "Yellow key LIST Link page Number"
///- TTX_CMD_SET_LIST_C: Set "Cyon key LIST Link page Number"
///
///@par TT_PARAM1 = Page number in hex
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //Set page 0x100 to "Green key LIST Link page Number"
///  TT_WriteCommand(TTX_CMD_SET_LIST_G, 0x100 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS13 SECTION 1-13
///@section SUBCODEIN SUBCODE input Mode: TT_User_Command 28
///- Enable/Disable SUBCODE input mode
///- The following image shows the subcode display format
///
///@image html subcode.jpg
///
///
///
///@n
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_SUBCODE
///
///@par TT_PARAM1 =
///- TT_DEC_OFF : Disable SUBCODE input mode
///- TT_DEC_ON : Enable SUBCODE input mode
///- TT_DEC_TOGGLE : Toggle SUBCODE input mode (SUBCODE input mode ENABLE--> DISABLE --> ENABLE....)
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //Enter SUBCODE Input mode
///  TT_WriteCommand(TTX_CMD_SUBCODE, TT_DEC_ON, 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS14 SECTION 1-14
///@section SetLANG Set Teletext Language group: TT_User_Command 29
///- Enable/Disable SUBCODE input mode
///- Below shows the language supports mapping with different language group and Control bits
///
///@image html language_group.jpg
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_SET_LANGUAGE
///
///@par TT_PARAM1 =
///@n TT_Charset_LG_Group_West : Teletext Language group WEST
///@n TT_Charset_LG_Group_East: Teletext Language group EAST,
///@n TT_Charset_LG_Group_Russian: Teletext Language group RUSSIAN,
///@n TT_Charset_LG_Group_Arabic: Teletext Language group ARABIC,
///@n TT_Charset_LG_Group_Farsi: Teletext Language group FARSI
///@see TT_Charset_LG_Group
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 2, just set to 0
///
///@par Example
///@code
///  //Set to Language EAST
///  TT_WriteCommand(TTX_CMD_SET_LANGUAGE, TT_Charset_LG_Group_East, 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS15 SECTION 1-15
///@section SetOSD Set Teletext OSD attributes: TT_User_Command 30
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_OSD
///
///@par TT_PARAM1 = Teletext OSD function types
///- TT_OSD_H_POS: Set Teletext Display Horizontal start Position (X coordinate)
///- TT_OSD_V_POS: Set Teletext Display Vertical start Position (Y coordinate)
///- TT_OSD_H_SCALE: Set Teletext Display Horizontal Scaling ratio
///- TT_OSD_V_SCALE: Set Teletext Display Vertical Scaling ratio
///- TT_FONT_H_RES: Set Teletext Font Horizontal resolution (in Pixels)
///- TT_FONT_V_RES: Set Teletext Font Vertical resolution (in Pixels)
///@see TTOSDType
///
///@par TT_PARAM2 = value for Teletext OSD function types
///
///@par Example
///@code
///
///  //Set Teletext OSD Position at coordinate (8, 0)
///  TT_WriteCommand(TTX_CMD_OSD, TT_OSD_H_POS, 8);
///  TT_WriteCommand(TTX_CMD_OSD, TT_OSD_V_POS, 0);
///
///  //Set Teletext OSD Horizontal Scaling ratio 2
///  TT_WriteCommand(TTX_CMD_OSD, TT_OSD_H_SCALE, 2);
///
///  //Set Teletext OSD FONT Vertical Resolution to 18 pixels
///  TT_WriteCommand(TTX_CMD_OSD, TT_FONT_V_RES, 18);
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS16 SECTION 1-16
///@section InitTTX Teletext Initialization: TT_User_Command 31
///- Enable Teletext decoder
///- Init Memory for teletext (Set memory base address and memory size for TTX)
///- Teletext Decoder create
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_INIT_TTX
///
///@par TT_PARAM1 = Memory base address (starting address) for Teletext
///- If this parameter is set to 0, the TTX library will allocate memory automatically.
///
///@par TT_PARAM2 = Memory size
///- If TT_PARAM1 is not 0, this parameter indicates the size of memory block. This
///  value must be larger or equal to the TTX necessary memory size which can be gotten
///  from read command TTX_CMD_GET_MEM_SIZE.
///
///@par Example
///@code
///  //Init teletext, Memory base 0x18888888, and have block size 0x1000
///  TT_WriteCommand(TTX_CMD_INIT_TTX, 0x18888888 , 0x1000);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS17 SECTION 1-17
///@section ResetTTX Teletext Decoder Reset: TT_User_Command 32
///- Reset Teletext decoder parameters and status
///- Reset Teletext database
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_RESET
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Reset teletext
///  TT_WriteCommand(TTX_CMD_RESET, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS18 SECTION 1-18
///@section TTXON Enable Teletext: TT_User_Command 33
///- Enable Teletext Acquiring, processing data from VBI buffer and Display.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_ON
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Enable teletext
///  TT_WriteCommand(TTX_CMD_ON, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS19 SECTION 1-19
///@section TTXOFF Disable Teletext: TT_User_Command 34
///- Disable Teletext Acquiring, processing data from VBI buffer and Display.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_OFF
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Disable teletext
///  TT_WriteCommand(TTX_CMD_OFF, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS20 SECTION 1-20
///@section TTXTV Disable Teletext: TT_User_Command 35
///- Enable Teletext EnableAcquiring, processing data from VBI buffer.
///- But Disable Teletext display.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_TVMODE
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///@note In this case, we don't need Input parameter 1 and parameter 2, just set to 0
///
///@par Example
///@code
///  //Set teletext to TV Mode
///  TT_WriteCommand(TTX_CMD_TVMODE, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS21 SECTION 1-21
///@section VIDEOSYSTEM Set Video System: TT_User_Command 36
///- Set Video system
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_VIDEO_SYSTEM
///
///@par TT_PARAM1 = Video system
///@n  TT_VIDEO_SYSTEM_PAL : Video system PAL
///@n  TT_VIDEO_SYSTEM_SECAM : Video system SECAM
///@n  TT_VIDEO_SYSTEM_PAL_M : Video system PAL-M
///@n  TT_VIDEO_SYSTEM_PAL_NC : Video system PAL-NC
///@see TT_VIDEO_SYSTEM
///
///@par TT_PARAM2 = 0
///
///@par Example
///@code
///  //Set teletext to PAL
///  TT_WriteCommand(TTX_CMD_VIDEO_SYSTEM, TT_VIDEO_SYSTEM_PAL , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS22 SECTION 1-22
///@section SETSOURCE Set input source and start to acquire packets immediately: TT_User_Command 37
///- Set TTX input source and  start to acquire packets immediately
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_SET_SOURCE
///
///@par TT_PARAM1 =
///@n TT_INPUT_SOURCE_TSP_0 : Teletext input source TSP0
///@n TT_INPUT_SOURCE_TSP_1 : Teletext input source TSP1
///@n TT_INPUT_SOURCE_VD_0 : Teletext input source VD0
///@n TT_INPUT_SOURCE_VD_1 : Teletext input source VD1
///@see TT_INPUT_SOURCE_SET
///@par TT_PARAM2 = TTX PID (if TT_PARAM1 is set to TT_INPUT_SOURCE_TSP_0 or TT_INPUT_SOURCE_TSP_1, this parameter is necessary)
///
///@par Example
///@code
///  //Set teletext input source and PID
///  TT_WriteCommand(TTX_CMD_SET_SOURCE,TT_INPUT_SOURCE_TSP_0, 0x1B7);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS23 SECTION 1-23
///@section CHANGEPAGE Change TTX Page Teletext: TT_User_Command 38
///- Change the current TTX page
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_CHANGE_PAGE
///
///@par TT_PARAM1 = TTX page
///
///@par TT_PARAM2 = subpage number (0xffff=don't care)
///
///@par Example
///@code
///  //Set teletext page to Mag 8, page 99, subpage 1234
///  TT_WriteCommand(TTX_CMD_CHANGE_PAGE, 0x099 , 0x1234);
///
///  //Set teletext page to Mag 8, page 99, don't care subpage
///  TT_WriteCommand(TTX_CMD_CHANGE_PAGE, 0x099 , 0xffff);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS24 SECTION 1-24
///@section ACIEnable Enable ACI feature: TT_User_Command 39
///- Enable the ACI feature
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_ACI_ENABLE
///
///@par TT_PARAM1 = system language in type TT_ACI_LANG
///
///@par TT_PARAM2 = 0
///
///@par Example
///@code
///  //Enable ACI feature and set system language to English
///  TT_WriteCommand(TTX_CMD_ACI_ENABLE, TT_ACI_LANG_ENGLISH , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS25 SECTION 1-25
///@section ACIDisable Diable ACI feature: TT_User_Command 40
///- Disable the ACI feaure.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_ACI_DISABLE
///
///@par TT_PARAM1 = 0
///
///@par TT_PARAM2 = 0
///
///@par Example
///@code
///  //Disable the ACI feature
///  TT_WriteCommand(TTX_CMD_ACI_DISABLE, 0 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section WS26 SECTION 1-26
///@section ACISelectMenuItem Set selected item in ACI menu: TT_User_Command 41
///- Set selected item in ACI menu.
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_ACI_SELECT_MENU_ITEM
///
///@par TT_PARAM1 = item index in menu (index: 1~n, '0' means default item)
///
///@par TT_PARAM2 = 0
///
///@par Example
///@code
///  //Select first item in ACI menu.
///  TT_WriteCommand(TTX_CMD_ACI_SELECT_MENU_ITEM, 1 , 0);
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section ReadCMD SECTION 2
///@section TT_API_READ  Teletext User Interface Detail Descriptions - READ Commands
///Here providing the interface to get information from teletext.
///@par INDEX
///@ref RS1 @ref GETList
///@n @ref RS2 @ref GET830
///@n @ref RS3 @ref GETCLOCK
///@n @ref RS4 @ref GetTTState
///@n @ref RS5 @ref GetMemSize
///@n @ref RS6 @ref GetACIStatus
///@n @ref RS7 @ref GetACIMenuItem
///@n @ref RS8 @ref GetACIChannelInfoCount
///@n @ref RS9 @ref GetACIChannelInfo
///@n
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///@n
///@n
///@section RS1 SECTION 2-1
///@section GETList Get LIST Page: TT_User_Command 39 ~ 42
///- Get List mode page number (or user preference page number) of Color key (R/G/Y/C)
///
///@link #TT_User_Command
///      TT_User_Command @endlink=
///- TTX_CMD_GET_LIST_R: Get "Red key LIST Link page Number"
///- TTX_CMD_GET_LIST_G: Get "Green key LIST Link page Number"
///- TTX_CMD_GET_LIST_Y: Get "Yellow key LIST Link page Number"
///- TTX_CMD_GET_LIST_C: Get "Cyon key LIST Link page Number"
///
///@par *TT_ParamBuf = pointer to a 2-byte buffer for return page number.
///- In this case, TT_ParamBuf will contain "List mode page number" of R/G/Y/C key.
///
///@par TT_PARAM2 = 2 (2 bytes for TT_ParamBuf since "Page Number" takes 2 bytes)
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///  unsigned char retunBuf[2];
///  bool status;
///  //Get List mode Red key page number
///  status = TT_ReadCommand(TTX_CMD_GET_LIST_R, retunBuf , 2);
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS2 SECTION 2-2
///@section GET830 Get CNI From Packet 830 or VPS: TT_User_Command 43 ~ 45
///- Get CNI from Teletext packet 8/30 or VPS
///
///@link #TT_User_Command
///      TT_User_Command @endlink=
///- TTX_CMD_GET_830FMT1: Get Network ID from Packet 8/30 format 1
///- TTX_CMD_GET_830FMT2: Get PDC (CNI) from Packet 8/30 format 2
///- TTX_CMD_GET_VPS_CNI: Get CNI from VPS
///
///@par *TT_ParamBuf = pointer to a 2-bytes buffer for return CNI/PDC data.
///- In this case, TT_ParamBuf will contain "CNI/PDC" data.
///- In case TTX_CMD_GET_VPS_CNI, if gotten CNI is "0x0DC3", it represnets
///  "ARD network". If gotten CNI is "0x1DC3", it represents "ZDF network".
///
///@par TT_PARAM2 = 2 (2 bytes for TT_ParamBuf since "CNI/PDC" takes 2 bytes)
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///  unsigned char retunBuf[2];
///  bool status;
///  //Get CNI from Teletext packet 8/30 formate 1
///  status = TT_ReadCommand(TTX_CMD_GET_830FMT1, retunBuf , 2);
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS3 SECTION 2-3
///@section GETCLOCK Get CLOCK information From teletext: TT_User_Command 46
///- Get Clock information (in ASCII code) if there is clock information contained in teletext signal
///
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_CLOCK_INFO
///
///
///@par *TT_ParamBuf = pointer to a 8 bytes array for returning clock
///- In this case, TT_ParamBuf will contain "CLOCK" string in ASCII code.
///
///@par TT_PARAM2 = 8 (8 bytes for TT_ParamBuf since "Clock string" takes 8 bytes)
///
///@return bool:
///- 0: No Clock information
///- 1: Clock information get
///
///@par Example
///@code
///  unsigned char retunBuf[8];
///  bool status;
///  //Get Clock information from Teletext
///  status = TT_ReadCommand(TTX_CMD_GET_CLOCK_INFO, retunBuf , 8);
///  //if status = FALSE, then "retunBuf" is meaningless
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS4 SECTION 2-4
///@section GetTTState Get Teletext Status: TT_User_Command 47
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_STATUS
///
///@par *TT_ParamBuf = pointer to a 2 bytes array for returning teletext status:
///- NAVI_MODE  \b BIT[0:1] = 00:TOP, 01:FLOF, 10:LIST mode, 11:Proprietary navigation mode
///- HOLD \b BIT2 = 1: Hold mode, 0:Not in Holde mode.
///- MIX  \b BIT3 = 1: MIX mode, 0:Not in MIX mode.
///- UPDATE  \b BIT4 = 1: UPDATE mode, 0:Not in UPDATE mode.
///- CLOCK  \b BIT5 = 1: CLOCK mode, 0:Not in CLOCK mode.
///- SUBCODE  \b BIT6 = 1: SUBCODE Input mode, 0:Not in SUBCODE Input mode.
///- TXT_ON  \b BIT7 = 1: Teletext ON, 0:Teletext is not ON
///- TV_MODE  \b BIT8 = 1: Teletext is in TV Mode, 0:Teletext is not in TV Mode
///- P830_FMT1_VALID  \b BIT9 = 1: Pakcet 8/30 format 1 is Valid, 0:Pakcet 8/30 format 1 is not valid (for CNI)
///- P830_FMT2_VALID  \b BIT10 = 1: Pakcet 8/30 format 2 is Valid, 0:Pakcet 8/30 format 2 is not valid (for CNI)
///- TTX_EXIST  \b BIT11 = 1: Teletext signal with this Channel, 0: NO Teletext signal with this Channel
///- NEWS_SUBTITLE  \b BIT12 = 1: Current page is a newsflash or subtitle page, 0: Current page is NOT a newsflash or subtitle page
///- TOP_IND  \b BIT13 = 1: TOP navigation is indicated, 0: NO TOP navigation
///- FLOF_IND  \b BIT14 = 1: FLOF navigation is indicated, 0: NO FLOF navigation
///
///@par TT_PARAM2 = 2 (2 bytes for TT_ParamBuf since "Teletext status" takes 2 bytes)
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///  unsigned char retunBuf[2];
///  bool status;
///  //Get Teletext status report
///  status = TT_ReadCommand(TTX_CMD_GET_STATUS, retunBuf , 2);
///  //if status = FALSE, then "retunBuf" is meaningless
///
///@endcode
///
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS5 SECTION 2-5
///@section GetMemSize Get Size of Memory used for TTX: TT_User_Command 48
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_MEM_SIZE
///
///@par *TT_ParamBuf = pointer to a 4 bytes buffer for returning the memory size.
///- In this case, TT_ParamBuf will contain a unsigned interger number.
///
///@par TT_PARAM2 = 4 (4 bytes for TT_ParamBuf since "memory size" takes 4 bytes)
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///  MS_U32 memSize;
///  bool status;
///  //Get size of memory used for Teletext
///  status = TT_ReadCommand(TTX_CMD_GET_MEM_SIZE, (MS_U8*)&memSize , 4);
///  //if status = FALSE, then "retunBuf" is meaningless
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS6 SECTION 2-6
///@section GetACIStatus Get ACI status: TT_User_Command 52
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_ACI_STATUS
///
///@par *TT_ParamBuf = pointer to variable with type TT_ACI_STATUS_RESULT
///
///@par TT_PARAM2 = size of type TT_ACI_STATUS_RESULT
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///  TT_ACI_STATUS_RESULT aciStatus;
///  bool status;
///  status = TT_ReadCommand(TT_ACI_STATUS_RESULT, (MS_U8*)&aciStatus , sizeof(TT_ACI_STATUS_RESULT));
///  //if status = FALSE, then "aciStatus" is meaningless
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS7 SECTION 2-7
///@section GetACIMenuItem Get ACI menu items: TT_User_Command 53
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_ACI_MENU_ITEM
///
///@par *TT_ParamBuf = pointer to a buffer which is large enough to keep
///                    all the menu item information with type TT_ACI_MENU_ITEM
///
///@par TT_PARAM2 = sizeof(TT_ACI_MENU_ITEM) X "menu item count"
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
/// TT_ACI_STATUS_RESULT aciStatusResult;
/// TT_ReadCommand(TTX_CMD_GET_ACI_STATUS,(MS_U8*)&aciStatusResult,sizeof(TT_ACI_STATUS_RESULT));
/// TT_ACI_MENU_ITEM* menuPtr;
/// MS_U16 i=0;
/// //have menu, then get the menu items
/// if(aciStatusResult.hasMenu)
/// {
///   //allocate necessary memory
///   menuPtr=MsOS_AllocateMemory(aciStatusResult.menuItemCount*sizeof(TT_ACI_MENU_ITEM),gs32CachedPoolID);
///   TT_ReadCommand(TTX_CMD_GET_ACI_MENU_ITEM,(MS_U8*)menuPtr,aciStatusResult.menuItemCount*sizeof(TT_ACI_MENU_ITEM));
///   MsOS_FreeMemory(menuPtr,gs32CachedPoolID);
/// }
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS8 SECTION 2-8
///@section GetACIChannelInfoCount Get ACI channel info record count: TT_User_Command 54
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD_COUNT
///
///@par *TT_ParamBuf = pointer to a 2 bytes buffer for returning the record count.
///
///@par TT_PARAM2 = 2 (2 bytes for TT_ParamBuf since "record count" takes 2 bytes)
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///    MS_U16 channelCount=0;
///    TT_ReadCommand(TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD_COUNT,(MS_U8*)&channelCount,2);
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n
//////////////////////////////////////////////////////////////////////////////
///@section RS9 SECTION 2-9
///@section GetACIChannelInfo Get ACI channel info: TT_User_Command 55
///@link #TT_User_Command
///      TT_User_Command @endlink= TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD
///
///@par *TT_ParamBuf = pointer to a buffer which is large enough to keep
///                    all the channel information record with type TT_ACI_CHANNEL_INFO_RECORD
///
///@par TT_PARAM2 = sizeof(TT_ACI_CHANNEL_INFO_RECORD) X "channel information record count"
///
///@return bool:
///- 0: Fail
///- 1: Success
///
///@par Example
///@code
///
/// MS_U16 channelCount=0;
/// TT_ReadCommand(TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD_COUNT,(MS_U8*)&channelCount,2);
/// TT_ACI_CHANNEL_INFO_RECORD* itemPtr=MsOS_AllocateMemory(channelCount*sizeof(TT_ACI_CHANNEL_INFO_RECORD),gs32CachedPoolID);
/// TT_ReadCommand(TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD,(MS_U8*)itemPtr,channelCount*sizeof(TT_ACI_CHANNEL_INFO_RECORD));
/// MsOS_FreeMemory(itemPtr,gs32CachedPoolID);
///
///@endcode
///
///@code
///@endcode
///@n
///@n
///@n
///@n
///@n


#ifndef API_TTX_H
#define API_TTX_H

#include "MsTypes.h"
#include "apiDMX.h"
#ifdef __cplusplus
extern "C"
{
#endif

///@section TT_User_Command
///
///Teletext User commands
typedef enum
{
    TTX_CMD_DIGIT0,                 ///< 0: IR input digit 0
    TTX_CMD_DIGIT1,                 ///< 1: IR input digit 1
    TTX_CMD_DIGIT2,                 ///< 2: IR input digit 2
    TTX_CMD_DIGIT3,                 ///< 3: IR input digit 3
    TTX_CMD_DIGIT4,                 ///< 4: IR input digit 4
    TTX_CMD_DIGIT5,                 ///< 5: IR input digit 0
    TTX_CMD_DIGIT6,                 ///< 6: IR input digit 0
    TTX_CMD_DIGIT7,                 ///< 7: IR input digit 0
    TTX_CMD_DIGIT8,                 ///< 8: IR input digit 0
    TTX_CMD_DIGIT9,                 ///< 9: IR input digit 0
    TTX_CMD_PAGEUP,                 ///< 10: IR input Page UP
    TTX_CMD_PAGEDOWN,               ///< 11: IR input Page DOWN
    TTX_CMD_PAGELEFT,               ///< 12: IR input Page LEFT
    TTX_CMD_PAGERIGHT,              ///< 13: IR input Page RIGHT
    TTX_CMD_RED,                    ///< 14: IR input RED key
    TTX_CMD_GREEN,                  ///< 15: IR input GREEN key
    TTX_CMD_YELLOW,                 ///< 16: IR input YELLOW key
    TTX_CMD_CYAN,                   ///< 17: IR input CYAN key
    TTX_CMD_INDEX,                  ///< 18: Go to "INDEX" page
    TTX_CMD_CLOCK,                  ///< 19: Enable "Clock" mode (Show Clock when Teletext is OFF)
    TTX_CMD_MIX,                    ///< 20: Enable "MIX" mode
    TTX_CMD_UPDATE,                 ///< 21: Enable "UPDATE" mode
    TTX_CMD_HOLD,                   ///< 22: Enable "HOLD" mode
    TTX_CMD_SIZE,                   ///< 23: Change Teletext "SIZE" mode (Toggle: Zoom In Uphalf -> Zoom In Lowhalf -> Normal)
    TTX_CMD_REVEAL,                 ///< 24: Enable "Reveal" function
    TTX_CMD_LIST,                   ///< 25: Enable "LIST" mode
    TTX_CMD_SET_LIST_R,             ///< 26: Set "Red key LIST Link page Number"
    TTX_CMD_SET_LIST_G,             ///< 27: Set "Green key LIST Link page Number"
    TTX_CMD_SET_LIST_Y,             ///< 28: Set "Yellow key LIST Link page Number"
    TTX_CMD_SET_LIST_C,             ///< 29: Set "Cyon key LIST Link page Number"
    TTX_CMD_SUBCODE,                ///< 30: Enter "SUBCODE INPUT" mode
    TTX_CMD_SET_LANGUAGE,           ///< 31: Set "Teletext LANGUAGE GROUP"
    TTX_CMD_OSD,                    ///< 32: Set Teletext Display parameters
    TTX_CMD_INIT_TTX,               ///< 33: Initialize Teletext Decoder
    TTX_CMD_RESET,                  ///< 34: "RESET" TTX Decoder
    TTX_CMD_ON,                     ///< 35: Set "Teletext ON"
    TTX_CMD_OFF,                    ///< 36: Set "Teletex OFF"
    TTX_CMD_TVMODE,                 ///< 37: Set "TV Mode" (Teletext processing in background without Teletext display)
    TTX_CMD_VIDEO_SYSTEM,           ///< 38: Set TV video system
    TTX_CMD_SET_SOURCE,             ///< 39: Set TTX input source and  start to acquire packets immediately
    TTX_CMD_CHANGE_PAGE,            ///< 40: change current TTX page

    //for ACI feature
    TTX_CMD_ACI_ENABLE,             ///< 41: Turn on the ACI function.
    TTX_CMD_ACI_DISABLE,            ///< 42: Turn off the ACI function.
    TTX_CMD_ACI_SELECT_MENU_ITEM,   ///< 43: If have ACI menu, set the index of seleted item. (index: 1~n)
    /// If set index as '0', it means using default item in this menu.
    //READ CMD
    TTX_CMD_GET_LIST_R,             ///< 44: Get "RED key LIST Link PAGE Number" in Hex
    TTX_CMD_GET_LIST_G,             ///< 45: Get "Green key LIST Link PAGE Number" in Hex
    TTX_CMD_GET_LIST_Y,             ///< 46: Get "Yellow key LIST Link PAGE Number" in Hex
    TTX_CMD_GET_LIST_C,             ///< 47: Get "Cyon key LIST Link PAGE Number" in Hex
    TTX_CMD_GET_830FMT1,            ///< 48: Get Packet 830 format 1 Network ID
    TTX_CMD_GET_830FMT2,            ///< 49: Get Packet 830 format 2 PDC (CNI)
    TTX_CMD_GET_VPS_CNI,            ///< 50: Get VPS (CNI)
    TTX_CMD_GET_CLOCK_INFO,         ///< 51: Get Clock information from Teletext
    TTX_CMD_GET_STATUS,             ///< 52: Get Teletext Status
    TTX_CMD_GET_MEM_SIZE,           ///< 53: Get size of memory which is necessary for TTX.

    //for ACI feature
    TTX_CMD_GET_ACI_STATUS,                     ///< 54: Query the status about ACI feature.
    TTX_CMD_GET_ACI_MENU_ITEM,                  ///< 55: Get the item information in menu by index number. (index: 1~n)
    TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD_COUNT,  ///< 56: Get the channel information record count.
    TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD,        ///< 57: Get the channel information record by index number. (index: 1~n)

    TTX_CMD_DIRECTLY_ON,                        ///< 58: Set "Teletext ON" by directly calling function instead of by sending message
    TTX_CMD_DIRECTLY_OFF,                       ///< 59: Set "Teletex OFF" by directly calling function instead of by sending message

    TTX_CMD_SUBTITLE_TTX_ON,                    ///< 60: Enter Teletext-Subtitle mode
    TTX_CMD_TTX_MODE,                           ///< 61: Set TTX mode
    TTX_CMD_SET_SYSTEM,                         ///< 62: Set TTX system mode (PAL or NTSC)
    TTX_CMD_VBI_MODE,                           ///< 63: Enable VBI mode or use OSD mode

    TTX_CMD_NUM                                 ///< Total Command numbers

} TT_User_Command;

///Sub-Commands for TT_CMD_OSD
typedef enum
{
    TT_OSD_H_POS,                   ///< 0: Set Teletext Display Horizontal start Position (X coordinate)
    TT_OSD_V_POS,                   ///< 1: Set Teletext Display Vertical start Position (Y coordinate)
    TT_OSD_H_SCALE,                 ///< 2: Set Teletext Display Horizontal Scaling ratio
    TT_OSD_V_SCALE,                 ///< 3: Set Teletext Display Vertical Scaling ratio
    TT_FONT_H_RES,                  ///< 4: Set Teletext Font Horizontal resolution (in Pixels)
    TT_FONT_V_RES,                  ///< 5: Set Teletext Font Vertical resolution (in Pixels)
} TTOSDType;

///Teletext Function States: Teletext decoder function OFF
#define TT_DEC_OFF                  0x0000
///Teletext Function States: Teletext decoder function ON
#define TT_DEC_ON                   0x0001
///Teletext Function States: Teletext decoder function TOGGLE
#define TT_DEC_TOGGLE               0x0002
///Teletext Language Group


typedef enum
{
    TT_Charset_LG_Group_West,       ///< 0: Teletext Language group WEST
    TT_Charset_LG_Group_East,       ///< 1: Teletext Language group EAST,
    TT_Charset_LG_Group_Russian,    ///< 2: Teletext Language group RUSSIAN,
    TT_Charset_LG_Group_Arabic,     ///< 3: Teletext Language group ARABIC,
    TT_Charset_LG_Group_Farsi       ///< 4: Teletext Language group FARSI,
} TT_Charset_LG_Group_Set;

///Sub-Commands for TT_CMD_SET_SOURCE
typedef enum
{
    TT_INPUT_SOURCE_TSP_0, ///< 0: Teletext input source TSP0
    TT_INPUT_SOURCE_TSP_1, ///< 1: Teletext input source TSP1
    TT_INPUT_SOURCE_VD_0,  ///< 2: Teletext input source VD0
    TT_INPUT_SOURCE_VD_1   ///< 3: Teletext input source VD1
} TT_INPUT_SOURCE_SET;

///Video system
typedef enum
{
    TT_VIDEO_SYSTEM_PAL,    ///< 0: Video system PAL
    TT_VIDEO_SYSTEM_SECAM,  ///< 1: Video system SECAM
    TT_VIDEO_SYSTEM_PAL_M,  ///< 2: Video system PAL-M
    TT_VIDEO_SYSTEM_PAL_NC  ///< 3: Video system PAL-NC
} TT_VIDEO_SYSTEM;

///Language setting for ACI
typedef enum
{
    TT_ACI_LANG_NONE,
    TT_ACI_LANG_CZECH,
    TT_ACI_LANG_DANISH,
    TT_ACI_LANG_DUTCH,
    TT_ACI_LANG_ENGLISH,
    TT_ACI_LANG_FINNISH,
    TT_ACI_LANG_FRENCH,
    TT_ACI_LANG_GERMAN,
    TT_ACI_LANG_GREEK,
    TT_ACI_LANG_HUNGARIAN,
    TT_ACI_LANG_ITALIAN,
    TT_ACI_LANG_TURKISH,
    TT_ACI_LANG_NORWEGIAN,
    TT_ACI_LANG_POLISH,
    TT_ACI_LANG_PORTUGUESE,
    TT_ACI_LANG_ROMANIAN,
    TT_ACI_LANG_RUSSIAN,
    TT_ACI_LANG_SPANISH,
    TT_ACI_LANG_SWEDISH
} TT_ACI_LANG;

typedef enum
{
    TT_ACI_TV_SOUND_SYSTEM_UNKNOWN,
    TT_ACI_TV_SOUND_SYSTEM_PAL_SECAM_BG,
    TT_ACI_TV_SOUND_SYSTEM_SECAM_L,
    TT_ACI_TV_SOUND_SYSTEM_PAL_M,
    TT_ACI_TV_SOUND_SYSTEM_NTSC_M,
    TT_ACI_TV_SOUND_SYSTEM_PAL_I,
    TT_ACI_TV_SOUND_SYSTEM_SECAM_DK
} TT_ACI_TV_SOUND_SYSTEM;

typedef enum
{
    TT_ACI_RADIO_SYSTEM_UNKNOWN,
    TT_ACI_RADIO_SYSTEM_FM,
    TT_ACI_RADIO_SYSTEM_DSR,
    TT_ACI_RADIO_SYSTEM_DAB,
    TT_ACI_RADIO_SYSTEM_DCR
} TT_ACI_RADIO_SYSTEM;

//****************************************************************************
// Control mode definitions
//****************************************************************************
typedef enum
{
    TT_Mode_Normal,
    //TT_Mode_Clock,
    TT_Mode_Subtitle,
    //TT_Mode_Status,
} eTELETEXT_MODE;

typedef enum
{
    TT_System_Mode_PAL,
    TT_System_Mode_NTSC
} TT_SYSTEM_MODE;

typedef enum
{
    TT_LIVE_MODE,
    TT_FILEIN_MODE,
    TT_RECORD_MODE
} TT_SET_TSP_MODE;

///for returning result of TTX_CMD_GET_ACI_STATUS
typedef struct
{
    MS_BOOL hasAci;         //have ACI?
    MS_BOOL hasMenu;        //have menu?
    MS_U8   menuItemCount;  //if have menu, how many menu items?
} TT_ACI_STATUS_RESULT;

///for returning reult of TTX_CMD_GET_ACI_MENU_ITEM
typedef struct
{
    MS_U8 numDigit[2];
    MS_U8 text[16];
} TT_ACI_MENU_ITEM;

///for returning result of TTX_CMD_GET_ACI_CHANNEL_INFO_RECORD
typedef struct
{
    MS_U8 presetNum[3];
    MS_U8 freq[5];
    MS_U8 name[6];
    TT_ACI_TV_SOUND_SYSTEM soundSystem;
    TT_ACI_RADIO_SYSTEM radioSystem;
} TT_ACI_CHANNEL_INFO_RECORD;

//for decoder status
typedef struct
{
    MS_BOOL   Mix;
    MS_BOOL   Cancel;
    MS_BOOL   Reveal;
    MS_BOOL   Hold;
    MS_BOOL   User;
    MS_BOOL   Display;
    MS_BOOL   Clock;
    MS_BOOL   StatusDisplay;
    MS_BOOL   List;
    MS_U8    Size;
    MS_BOOL   PageCatch;
    MS_BOOL   TopMenu;
    MS_BOOL   Subtitle;
    MS_BOOL   SubNews;
    MS_BOOL   PageFound;
    MS_BOOL   PageProcessed;
    MS_U8    PageAvailable;
    MS_BOOL   PageUpdateAvailable;
    MS_BOOL   RequestedPageAvailable;
    MS_BOOL   AciFound;
    MS_BOOL   SubpageAvailable;
    MS_BOOL   SubpageBarVisible;
    MS_U8    TopFlofEnable;
    MS_U8    NumCount;
    MS_U16   NumKey;
    MS_U8    ListModeSelection;
    MS_BOOL   SubpageInput;
    MS_U8    SubpageInputNumCount;
    MS_U16   SubpageInputNumKey;
} apiTTX_DecoderStatus;

//////////////////////////////////////////////////////////////////////////////
///Teletext User Interface : Write commands to teletext (Write Command)
///@param TT_CMD \b IN: Teletext User Command @see TT_User_Command
///@param TT_PARAM1 \b IN: Input parameter 1 (see below detail description)
///@param TT_PARAM2 \b IN: Input parameter 2 (see below detail description)
//////////////////////////////////////////////////////////////////////////////
MS_BOOL TT_WriteCommand(TT_User_Command TT_CMD, MS_U32 TT_PARAM1, MS_U32 TT_PARAM2);

//////////////////////////////////////////////////////////////////////////////////////
///Teletext User Interface : Get information from Teletext (Read Command)
///@param TT_CMD \b IN: Teletext User Command (TT_User_Command)
///@param *TT_ParamBuf \b IN: Pointer fo Return data buffer (see below detail description)
///@param TT_ParamLen \b IN: Return data buffer length (see below detail description)
///@return MS_BOOL: Status return (TRUE/FALSE: see below detail description)
//////////////////////////////////////////////////////////////////////////////////////
MS_BOOL TT_ReadCommand(TT_User_Command TT_CMD, MS_U8 *TT_ParamBuf, MS_U32 TT_ParamLen);

//-------------------------------------------------------------------------------------------------
/// Allocate TT Packet Buffer
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
MS_BOOL apiTTX_AllocTTPktBuffer(void);
void apiTTX_SaveGopPalette(void);
void apiTTX_RestoreGopPalette(void);
//-------------------------------------------------------------------------------------------------
/// Disable IP GOP which is used by Subtitle
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Disable_IP_GOP(void);
//-------------------------------------------------------------------------------------------------
/// Enable IP GOP which is used by Subtitle
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Enable_IP_GOP(void);
//-------------------------------------------------------------------------------------------------
/// Start TTX task
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Start_TTX_Task(void);
//-------------------------------------------------------------------------------------------------
/// Pend TTX task
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Pend_TTX_Task(void);

//-------------------------------------------------------------------------------------------------
/// Free memory allocation when TTX exit
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_ExitFreeMemory(void);

//-------------------------------------------------------------------------------------------------
/// allocate memory when enter TTX
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
MS_BOOL apiTTX_AllocMemory(void);

//-------------------------------------------------------------------------------------------------
/// Set TTX controlling mode.
/// @param mode \b IN: TTX control mode
/// -@see eTELETEXT_MODE
/// @return None :
//-------------------------------------------------------------------------------------------------
void MApi_TTX_SetControlMode(eTELETEXT_MODE mode);

//-------------------------------------------------------------------------------------------------
/// Set TTX controlling mode.
/// @param None \b IN:
/// @return eTELETEXT_MODE : TTX control mode
/// -@see eTELETEXT_MODE
//-------------------------------------------------------------------------------------------------
eTELETEXT_MODE MApi_TTX_GetControlMode(void);

//-------------------------------------------------------------------------------------------------
/// Set TTX OSD offset when enter TTX for different output
/// @param x : OSD x pixels
/// @param y : OSD y pixels
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_SetOSDOffset(MS_U16 x, MS_U16 y);

//-------------------------------------------------------------------------------------------------
/// Set TTX data for different tsp mode
/// @param ettTSPMode : set TSP mode
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_SetTSPMode(TT_SET_TSP_MODE ettTSPMode);

//-------------------------------------------------------------------------------------------------
/// Allocate VBI packet buffer memory
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
MS_BOOL apiTTX_AllocVBIPKTMemory(void);

//-------------------------------------------------------------------------------------------------
/// Set enable or disable VBI in backgound
/// @param bEnable: enable or disable set VBI in backgound
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Enable_VBIBackground(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Stop received TTX packet from demux
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Stop_DMX(void);

typedef enum
{
    E_APITTX_GOP_DST_AUTO = 0,
    E_APITTX_GOP_DST_IP,
    E_APITTX_GOP_DST_OP,
} APITTX_GOP_DST_TYPE;

//-------------------------------------------------------------------------------------------------
/// set TTX GOP Sequence
/// @param u32GOPSeq
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Set_GOP_Sequence(MS_U32 u32GOPSeq);
//-------------------------------------------------------------------------------------------------
/// set TTX GOP destination type
/// @param APITTX_GOP_DST_TYPE
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Set_GOP_Dst(APITTX_GOP_DST_TYPE type);
//-------------------------------------------------------------------------------------------------
/// apiTTX lock
/// lock for apiTTX
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Lock(void);

//-------------------------------------------------------------------------------------------------
/// apiTTX unlock
/// unlock for apiTTX
/// @return None
//-------------------------------------------------------------------------------------------------
void apiTTX_Unlock(void);

//-------------------------------------------------------------------------------------------------
/// apiTTX_DecoderGetStatus - TT_DecoderGetStatus
/// A wrapper of TT_DecoderGetStatus, users can get information from the decoder if needed
/// @return MS_BOOL - TRUE is success; otherwise FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL apiTTX_DecoderGetStatus(apiTTX_DecoderStatus *DecStatus);

//-------------------------------------------------------------------------------------------------
// apiTTX_TTXGetStatus -
// @return TRUE: TTX is on
// @return FALSE: TTX is off
//-------------------------------------------------------------------------------------------------
MS_BOOL apiTTX_TTXGetStatus(void);

//-------------------------------------------------------------------------------------------------
// apiTTX_TTXSetStatus -
// TRUE: TTX is on
// FALSE: TTX is off
// @return: none
//-------------------------------------------------------------------------------------------------
void apiTTX_TTXSetStatus(MS_BOOL bIsTTXOn);

typedef void (*MsMW_VDEC_GetDispInfo)(VDEC_EX_DispInfo* info);
extern MsMW_VDEC_GetDispInfo TTX_VDEC_GetDispInfo;

//----------------------------------------------------------------------------------------
//  Delete TTX task and free all teletext resource
//  @param None
//  @return None
//----------------------------------------------------------------------------------------
void apiTTX_Delete_TTX_Task(void);

//-------------------------------------------------------------------------------------------
/// Free section buffer memory when TTX exit
/// @param None
/// @return None
//-------------------------------------------------------------------------------------------
void apiTTX_ExitFreeSecMemory(void);
//-------------------------------------------------------------------------------------------------
// apiTTX_TTXMainControl - a unified control interface for teletext
// @param MB_BOOL bEnableTTX
// @param MB_BOOL bEnableTTX_BG_VBI
// @param TT_INPUT_SOURCE_SET
// @param MS_U16*: a pointer to a U16 which is the teletext pid
// @param MS_U8*: a pointer to a U8 which is magazine number for
// teletext subtitle
// @param MS_U8*: a pointer to a U8 which is page number for
// teletext subtitle
//
// @return: TRUE if success, FALSE if fail
//
// _NOTE_
// If both bEnableTTX and bEnableTTX_BG_VBI are false, all ttx tasks will be
// deleted after this function call.
//-------------------------------------------------------------------------------------------------
MS_BOOL apiTTX_TTXMainControl(MS_BOOL bEnableTTX, MS_BOOL bEnableTTX_BG_VBI,
                              MS_U8 *pU8mag, MS_U8 *pU8page);

void apiTTX_SetDMXFltSrc(DMX_FILTER_TYPE);

#ifdef __cplusplus
}
#endif

#endif
