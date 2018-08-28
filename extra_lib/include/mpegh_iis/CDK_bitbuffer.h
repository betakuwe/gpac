/***************************  Fraunhofer IIS CDK Tools  ***********************
 
                        (C) Copyright Fraunhofer IIS (2005)
                               All Rights Reserved
 
   $Id$
   Author(s):   M. Lohwasser
   Description: common bitbuffer read/write routines

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.
 
******************************************************************************/

#ifndef __CDK_BITBUFFER_H__
#define __CDK_BITBUFFER_H__

#include "CDK_tools_settings.h"


#include "CDK_archdef.h"
#include "machine_type.h"


/* leave 3 bits headroom so MAX_BUFSIZE can be represented in bits as well. */
#define MAX_BUFSIZE_BYTES   (0x10000000)

#ifdef __XTENSA_CONFIG__DC_E_330HiFi
#define CDK_XTENSA_HIFI3_BITREADER_ENABLE
#endif

typedef struct
{
  UINT ValidBits;
  UINT ReadOffset;
  UINT WriteOffset;
  UINT BitCnt;
  UINT BitNdx;

  UCHAR *Buffer;
  UINT   bufSize;
  UINT   bufBits;
#ifdef CDK_XTENSA_HIFI3_BITREADER_ENABLE
  USHORT    *CDKreadBitsHiFi3_ptr;
#endif
} CDK_BITBUF;

typedef CDK_BITBUF *HANDLE_CDK_BITBUF;

#ifdef __cplusplus
extern "C"
{
#endif

extern const UINT BitMask [32+1];

/**  The BitBuffer Functions are called straight from CDK_bitstream Interface.
     For Functions functional survey look there.
*/

void CDK_CreateBitBuffer (HANDLE_CDK_BITBUF *hBitBuffer, UCHAR *pBuffer, UINT bufSize) ;

void CDK_InitBitBuffer   (HANDLE_CDK_BITBUF hBitBuffer, UCHAR *pBuffer,
                          UINT bufSize, UINT validBits) ;

void CDK_ResetBitBuffer  (HANDLE_CDK_BITBUF hBitBuffer) ;

void CDK_DeleteBitBuffer (HANDLE_CDK_BITBUF hBitBuffer) ;

#ifdef INLINE_CDK_get

#define FUNCTION_CDK_get

CDK_INLINE INT CDK_get (HANDLE_CDK_BITBUF hBitBuf, const UINT numberOfBits)
{
  UINT byteOffset = hBitBuf->BitNdx >> 3 ;
  UINT bitOffset  = hBitBuf->BitNdx & 0x07 ;

  hBitBuf->BitNdx     = (hBitBuf->BitNdx + numberOfBits) & (hBitBuf->bufBits - 1) ;
  hBitBuf->BitCnt    +=  numberOfBits ;
  hBitBuf->ValidBits -=  numberOfBits ;

  UINT byteMask = hBitBuf->bufSize - 1 ;

  UINT tx = (hBitBuf->Buffer [ byteOffset    & byteMask] << 24) |
            (hBitBuf->Buffer [(byteOffset+1) & byteMask] << 16) |
            (hBitBuf->Buffer [(byteOffset+2) & byteMask] <<  8) |
             hBitBuf->Buffer [(byteOffset+3) & byteMask];

  tx <<= bitOffset;
  tx |= hBitBuf->Buffer [(byteOffset+4) & byteMask] >> (8-bitOffset);
  return (tx >> (32 - numberOfBits)) ;
}
#else
INT  CDK_get             (HANDLE_CDK_BITBUF hBitBuffer, const UINT numberOfBits) ;
#endif /* #ifdef INLINE_CDK_get */

INT  CDK_get32           (HANDLE_CDK_BITBUF hBitBuffer) ;

void CDK_put             (HANDLE_CDK_BITBUF hBitBuffer, UINT value, const UINT numberOfBits) ;

INT  CDK_getBwd          (HANDLE_CDK_BITBUF hBitBuffer, const UINT numberOfBits);
void CDK_putBwd          (HANDLE_CDK_BITBUF hBitBuffer, UINT value, const UINT numberOfBits) ;

void CDK_pushBack        (HANDLE_CDK_BITBUF hBitBuffer, const UINT numberOfBits, UCHAR config) ;

#ifdef INLINE_CDK_pushBackBS_READER

#define FUNCTION_CDK_pushBackBS_READER

static inline void CDK_pushBackBS_READER (HANDLE_CDK_BITBUF hBitBuf, const UINT numberOfBits)
{
  hBitBuf->BitCnt    -= numberOfBits ;
  hBitBuf->ValidBits += numberOfBits;
  hBitBuf->BitNdx     = (hBitBuf->BitNdx - numberOfBits) & (hBitBuf->bufBits - 1) ;
}
#else
void CDK_pushBackBS_READER (HANDLE_CDK_BITBUF hBitBuf, const UINT numberOfBits);
#endif /* #ifdef INLINE_CDK_pushBackBS_READER */

void CDK_pushForward     (HANDLE_CDK_BITBUF hBitBuffer, const UINT numberOfBits, UCHAR config) ;

void CDK_byteAlign       (HANDLE_CDK_BITBUF hBitBuffer, UCHAR config) ;

#ifdef INLINE_CDK_getValidBits

#define FUNCTION_CDK_getValidBits

static inline UINT CDK_getValidBits (HANDLE_CDK_BITBUF hBitBuffer) { return hBitBuffer->ValidBits; }
#else
UINT CDK_getValidBits    (HANDLE_CDK_BITBUF hBitBuffer);
#endif /* #ifdef INLINE_CDK_getValidBits */

INT  CDK_getFreeBits     (HANDLE_CDK_BITBUF hBitBuffer) ;

void CDK_setBitCnt       (HANDLE_CDK_BITBUF hBitBuffer, const UINT value) ;
INT  CDK_getBitCnt       (HANDLE_CDK_BITBUF hBitBuffer) ;

void CDK_Feed            (HANDLE_CDK_BITBUF hBitBuffer, const UCHAR inputBuffer [],
                          const UINT bufferSize, UINT *bytesValid) ;

void CDK_Copy            (HANDLE_CDK_BITBUF hBitBufDst, HANDLE_CDK_BITBUF hBitBufSrc, UINT *bytesValid) ;

void CDK_Fetch           (HANDLE_CDK_BITBUF hBitBuffer, UCHAR outBuf[], UINT *writeBytes);

#ifdef CDK_XTENSA_HIFI3_BITREADER_ENABLE
void CDKinitBitStream_HiFi3 (HANDLE_CDK_BITBUF hBitBuf);
void CDKpushBackHiFi3(HANDLE_CDK_BITBUF hBitBuf, INT nbits);
void CDKdeinitBitStream_HiFi3(HANDLE_CDK_BITBUF hBitBuf, UINT ConfigCache);
#endif /* #if defined (__XTENSA_CONFIG__DC_E_330HiFi) && defined(CDK_XTENSA_HIFI3_BITREADER_ENABLE) */

#ifdef __cplusplus
}
#endif


#endif
