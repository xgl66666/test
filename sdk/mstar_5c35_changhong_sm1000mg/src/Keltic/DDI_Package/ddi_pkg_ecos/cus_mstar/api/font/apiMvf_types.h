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
//------------------------------------------------------------------------------
//
//    Portions of this software are copyright 2007 The FreeType
//    Project (www.freetype.org).  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
/// @file apiMvf_types.h
/// @brief MStar Vector Font Engine module.
/// @author MStar Semiconductor Inc.
///
///  Vector Font Engine render. Reference "smooth" render of FreeType2 library
///
/// Features:
/// - Providing data type of rendering function
//
////////////////////////////////////////////////////////////////////////////////



#ifndef __API_MVFTYPES_H__
#define __API_MVFTYPES_H__


#ifdef __cplusplus
extern "C"
{
#endif




  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    basic_types                                                        */
  /*                                                                       */
  /* <Title>                                                               */
  /*    Basic Data Types                                                   */
  /*                                                                       */
  /* <Abstract>                                                            */
  /*    The basic data types defined by the library.                       */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This section contains the basic data types defined by FreeType 2,  */
  /*    ranging from simple scalar types to bitmap descriptors.  More      */
  /*    font-specific structures are defined in a different section.       */
  /*                                                                       */
  /* <Order>                                                               */
  /*    MVF_Byte                                                            */
  /*    MVF_Bytes                                                           */
  /*    MVF_Char                                                            */
  /*    MVF_Int                                                             */
  /*    MVF_UInt                                                            */
  /*    MVF_Short                                                           */
  /*    MVF_UShort                                                          */
  /*    MVF_Long                                                            */
  /*    MVF_ULong                                                           */
  /*    MVF_Bool                                                            */
  /*    MVF_Offset                                                          */
  /*    MVF_PtrDist                                                         */
  /*    MVF_String                                                          */
  /*    MVF_Tag                                                             */
  /*    MVF_Error                                                           */
  /*    MVF_Fixed                                                           */
  /*    MVF_Pointer                                                         */
  /*    MVF_Pos                                                             */
  /*    MVF_Vector                                                          */
  /*    MVF_BBox                                                            */
  /*    MVF_Matrix                                                          */
  /*    MVF_FWord                                                           */
  /*    MVF_UFWord                                                          */
  /*    MVF_F2Dot14                                                         */
  /*    MVF_UnitVector                                                      */
  /*    MVF_F26Dot6                                                         */
  /*                                                                       */
  /*                                                                       */
  /*    MVF_Generic                                                         */
  /*    MVF_Generic_Finalizer                                               */
  /*                                                                       */
  /*    MVF_Bitmap                                                          */
  /*    MVF_Pixel_Mode                                                      */
  /*    MVF_Palette_Mode                                                    */
  /*    MVF_Glyph_Format                                                    */
  /*    MVF_IMAGE_TAG                                                       */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Bool                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef of unsigned char, used for simple booleans.
  typedef unsigned char  MVF_Bool;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_FWord                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A signed 16-bit integer used to store a distance in original font units.
  typedef signed short  MVF_FWord;   /* distance in FUnits */


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_UFWord                                                          */
  /*                                                                       */
  /* <Description>                                                         */
  ///   An unsigned 16-bit integer used to store a distance in original font units.
  typedef unsigned short  MVF_UFWord;  /* unsigned distance */


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Char                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A simple typedef for the _signed_ char type.
  typedef signed char  MVF_Char;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Byte                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A simple typedef for the _unsigned_ char type.
  typedef unsigned char  MVF_Byte;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Bytes                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///    A typedef for constant memory areas.
  typedef const MVF_Byte*  MVF_Bytes;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Tag                                                             */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A typedef for 32bit tags (as used in the SFNT format).             */
  /*                                                                       */
//  typedef MVF_UInt32  MVF_Tag;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_String                                                          */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A simple typedef for the char type, usually used for strings.
  typedef char  MVF_String;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Short                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef for signed short.
  typedef signed short  MVF_Short;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_UShort                                                          */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef for unsigned short.
  typedef unsigned short  MVF_UShort;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Int                                                             */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef for the int type.
  typedef signed int  MVF_Int;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_UInt                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef for the unsigned int type.
  typedef unsigned int  MVF_UInt;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Long                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef for signed long.
  typedef signed long  MVF_Long;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_ULong                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A typedef for unsigned long.
  typedef unsigned long  MVF_ULong;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_F2Dot14                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A signed 2.14 fixed float type used for unit vectors.
  typedef signed short  MVF_F2Dot14;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_F26Dot6                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A signed 26.6 fixed float type used for vectorial pixel coordinates.
  typedef signed long  MVF_F26Dot6;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Fixed                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///   This type is used to store 16.16 fixed float values, like scaling values or matrix coefficients.
  typedef signed long  MVF_Fixed;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Error                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  ///   The FreeType error code type.  A value of 0 is always interpreted as a successful operation.                                         */
  typedef int  MVF_Error;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Pointer                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  ///   A simple typedef for a typeless pointer.
  typedef void*  MVF_Pointer;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_Offset                                                          */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This is equivalent to the ANSI C `size_t' type, i.e., the largest  */
  /*    _unsigned_ integer type used to express a file size or position,   */
  /*    or a memory block size.                                            */
  /*                                                                       */
  //typedef size_t  MVF_Offset;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_PtrDist                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This is equivalent to the ANSI C `ptrdiff_t' type, i.e., the       */
  /*    largest _signed_ integer type used to express the distance         */
  /*    between two pointers.                                              */
  /*                                                                       */
  //typedef ft_ptrdiff_t  MVF_PtrDist;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    MVF_UnitVector                                                      */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A simple structure used to store a 2D vector unit vector.  Uses    */
  /*    MVF_F2Dot14 types.                                                  */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    x :: Horizontal coordinate.                                        */
  /*                                                                       */
  /*    y :: Vertical coordinate.                                          */
  /*                                                                       */
  typedef struct  MVF_UnitVector_
  {
    MVF_F2Dot14  x;
    MVF_F2Dot14  y;

  } MVF_UnitVector;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    MVF_Matrix                                                          */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A simple structure used to store a 2x2 matrix.  Coefficients are   */
  /*    in 16.16 fixed float format.  The computation performed is:        */
  /*                                                                       */
  /*       {                                                               */
  /*          x' = x*xx + y*xy                                             */
  /*          y' = x*yx + y*yy                                             */
  /*       }                                                               */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    xx :: Matrix coefficient.                                          */
  /*                                                                       */
  /*    xy :: Matrix coefficient.                                          */
  /*                                                                       */
  /*    yx :: Matrix coefficient.                                          */
  /*                                                                       */
  /*    yy :: Matrix coefficient.                                          */
  /*                                                                       */
  typedef struct  MVF_Matrix_
  {
    MVF_Fixed  xx, xy;
    MVF_Fixed  yx, yy;

  } MVF_Matrix;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    MVF_Data	                                                       */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Read-only binary data represented as a pointer and a length.       */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    pointer :: The data.                                               */
  /*                                                                       */
  /*    length  :: The length of the data in bytes.                        */
  /*                                                                       */
  typedef struct  MVF_Data_
  {
    const MVF_Byte*  pointer;
    MVF_Int          length;

  } MVF_Data;


  /*************************************************************************/
  /*                                                                       */
  /* <FuncType>                                                            */
  /*    MVF_Generic_Finalizer                                               */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Describes a function used to destroy the `client' data of any      */
  /*    FreeType object.  See the description of the @MVF_Generic type for  */
  /*    details of usage.                                                  */
  /*                                                                       */
  /* <Input>                                                               */
  /*    The address of the FreeType object which is under finalization.    */
  /*    Its client data is accessed through its `generic' field.           */
  /*                                                                       */
  ///   Describes a function used to destroy the `client' data of any FreeType object.
  typedef void  (*MVF_Generic_Finalizer)(void*  object);


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    MVF_Generic                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Client applications often need to associate their own data to a    */
  /*    variety of FreeType core objects.  For example, a text layout API  */
  /*    might want to associate a glyph cache to a given size object.      */
  /*                                                                       */
  /*    Most FreeType object contains a `generic' field, of type           */
  /*    MVF_Generic, which usage is left to client applications and font    */
  /*    servers.                                                           */
  /*                                                                       */
  /*    It can be used to store a pointer to client-specific data, as well */
  /*    as the address of a `finalizer' function, which will be called by  */
  /*    FreeType when the object is destroyed (for example, the previous   */
  /*    client example would put the address of the glyph cache destructor */
  /*    in the `finalizer' field).                                         */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    data      :: A typeless pointer to any client-specified data. This */
  /*                 field is completely ignored by the FreeType library.  */
  /*                                                                       */
  /*    finalizer :: A pointer to a `generic finalizer' function, which    */
  /*                 will be called when the object is destroyed.  If this */
  /*                 field is set to NULL, no code will be called.         */
  /*                                                                       */
  typedef struct  MVF_Generic_
  {
    void*                 data;
    MVF_Generic_Finalizer  finalizer;

  } MVF_Generic;


  /*************************************************************************/
  /*                                                                       */
  /* <Macro>                                                               */
  /*    MVF_MAKE_TAG                                                        */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This macro converts four-letter tags which are used to label       */
  /*    TrueType tables into an unsigned long to be used within FreeType.  */
  /*                                                                       */
  /* <Note>                                                                */
  /*    The produced values *must* be 32bit integers.  Don't redefine this */
  /*    macro.                                                             */
  /*                                                                       */
#define MVF_MAKE_TAG( _x1, _x2, _x3, _x4 ) \
          ( ( (MVF_ULong)_x1 << 24 ) |     \
            ( (MVF_ULong)_x2 << 16 ) |     \
            ( (MVF_ULong)_x3 <<  8 ) |     \
              (MVF_ULong)_x4         )


  /*************************************************************************/
  /*************************************************************************/
  /*                                                                       */
  /*                    L I S T   M A N A G E M E N T                      */
  /*                                                                       */
  /*************************************************************************/
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    list_processing                                                    */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_ListNode                                                        */
  /*                                                                       */
  /* <Description>                                                         */
  /*     Many elements and objects in FreeType are listed through an       */
  /*     @MVF_List record (see @MVF_ListRec).  As its name suggests, an      */
  /*     MVF_ListNode is a handle to a single list element.                 */
  /*                                                                       */
  typedef struct MVF_ListNodeRec_*  MVF_ListNode;


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    MVF_List                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A handle to a list record (see @MVF_ListRec).                       */
  /*                                                                       */
  typedef struct MVF_ListRec_*  MVF_List;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    MVF_ListNodeRec                                                     */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A structure used to hold a single list element.                    */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    prev :: The previous element in the list.  NULL if first.          */
  /*                                                                       */
  /*    next :: The next element in the list.  NULL if last.               */
  /*                                                                       */
  /*    data :: A typeless pointer to the listed object.                   */
  /*                                                                       */
  typedef struct  MVF_ListNodeRec_
  {
    MVF_ListNode  prev;
    MVF_ListNode  next;
    void*        data;

  } MVF_ListNodeRec;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    MVF_ListRec                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A structure used to hold a simple doubly-linked list.  These are   */
  /*    used in many parts of FreeType.                                    */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    head :: The head (first element) of doubly-linked list.            */
  /*                                                                       */
  /*    tail :: The tail (last element) of doubly-linked list.             */
  /*                                                                       */
  typedef struct  MVF_ListRec_
  {
    MVF_ListNode  head;
    MVF_ListNode  tail;

  } MVF_ListRec;


  /* */

#define MVF_IS_EMPTY( list )  ( (list).head == 0 )

  /* return base error code (without module-specific prefix) */
#define MVF_ERROR_BASE( x )    ( (x) & 0xFF )

  /* return module error code */
#define MVF_ERROR_MODULE( x )  ( (x) & 0xFF00U )

#define MVF_BOOL( x )  ( (MVF_Bool)( x ) )

#ifdef __cplusplus
}
#endif


#endif /* __API_MVFTYPES_H__ */


/* END */
