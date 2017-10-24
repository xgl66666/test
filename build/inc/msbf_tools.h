/**************************************************************************//**
** @file msbf_tools.h
**
** @brief
**  MSBF (Most Significant Byte Fist) tools to set or to get data in/from
**  a buffer 'structure field'.
**
**  File data must be independent from alignment, use MSBF form in NVM file
**  and configuration file
**
** @ingroup DATA FILE MANAGEMENT
**
** @version $Rev: 244 $
**          $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/msbf_tools.h $
**          $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
**
** @author  SmarDTV Rennes - LIPPA
**
** COPYRIGHT:
**   2011 SmarDTV
**
******************************************************************************/

#ifndef _MSBF_TOOLS_H
#define _MSBF_TOOLS_H

/******************************************************************************
* Includes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Defines
******************************************************************************/


/******************************************************************************
* Macros
******************************************************************************/

/**
 *
 * These macros provide support for closely packed structure fields,
 * and offer assignment and retrieval routines.
*/
#define mMSBF_SET08(p,v) { (p)[0] = (uint8_t)(v); }
#define mMSBF_SET16(p,v) { (p)[0] = (uint8_t)((v)>>8);  (p)[1] = (uint8_t)(v); }
#define mMSBF_SET24(p,v) { (p)[0] = (uint8_t)((v)>>16); (p)[1] = (uint8_t)((v)>>8); (p)[2] = (uint8_t)(v); }
#define mMSBF_SET32(p,v) { (p)[0] = (uint8_t)((v)>>24); (p)[1] = (uint8_t)((v)>>16); (p)[2] = (uint8_t)((v)>>8); (p)[3] = (uint8_t)(v); }

#define mMSBF_GET08(p) ( (uint8_t)((p)[0]) )
#define mMSBF_GET16(p) ( (((uint16_t)((p)[0]))<<8)  | (uint16_t)((p)[1]) )
#define mMSBF_GET24(p) ( (((uint32_t)((p)[0]))<<16) | (((uint32_t)((p)[1]))<<8) | (uint32_t)((p)[2]) )
#define mMSBF_GET32(p) ( (((uint32_t)((p)[0]))<<24) | (((uint32_t)((p)[1]))<<16) | (((uint32_t)((p)[2]))<<8) | (uint32_t)((p)[3]) )

/* Idem but with a fix pointer and offset automaticly incremented */
#define mMSBF_SET08_O(p,o,v) {(p)[(o)++]=(uint8_t)(v);}
#define mMSBF_SET16_O(p,o,v) {(p)[(o)++]=(uint8_t)((v)>>8);(p)[(o)++]=(uint8_t)(v);}
#define mMSBF_SET24_O(p,o,v) {(p)[(o)++]=(uint8_t)((v)>>16);(p)[(o)++]=(uint8_t)((v)>>8);(p)[(o)++]=(uint8_t)(v);}
#define mMSBF_SET32_O(p,o,v) {(p)[(o)++]=(uint8_t)((v)>>24);(p)[(o)++]=(uint8_t)((v)>>16);(p)[(o)++]=(uint8_t)((v)>>8);(p)[(o)++]=(uint8_t)(v);}
#define mMSBF_GET08_O(p,o,v) {(v)=mMSBF_GET08((p)+(o));(o)+=1;}
#define mMSBF_GET16_O(p,o,v) {(v)=mMSBF_GET16((p)+(o));(o)+=2;}
#define mMSBF_GET24_O(p,o,v) {(v)=mMSBF_GET24((p)+(o));(o)+=3;}
#define mMSBF_GET32_O(p,o,v) {(v)=mMSBF_GET32((p)+(o));(o)+=4;}

/******************************************************************************
* Typedefs
******************************************************************************/

/******************************************************************************
* Functions
******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* _MSBF_TOOLS_H */

/* msbf_tools.h */

