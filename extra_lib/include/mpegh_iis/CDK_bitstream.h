/***************************  Fraunhofer IIS CDK Tools  ***********************

                        (C) Copyright Fraunhofer IIS (2009)
                               All Rights Reserved

   $Id$
   Author(s):   M. Lohwasser
   Description: bitstream interface to bitbuffer routines

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __CDK_BITSTREAM_H__
#define __CDK_BITSTREAM_H__

#include "CDK_tools_settings.h"


#include "CDK_bitbuffer.h"
#include "machine_type.h"

#include "genericStds.h"

#define CACHE_BITS 32

#define BUFSIZE_DUMMY_VALUE    MAX_BUFSIZE_BYTES

typedef enum {
    BS_READER,
    BS_WRITER
} CDK_BS_CFG;


typedef struct
{
  UINT       CacheWord ;
  UINT       BitsInCache ;
  CDK_BITBUF hBitBuf;
  UINT       ConfigCache ;
} CDK_BITSTREAM;

typedef CDK_BITSTREAM* HANDLE_CDK_BITSTREAM;

/**
 * \brief CreateBitStream Function.
 *
 * Create and initialize bitstream with extern allocated buffer.
 *
 * \param pBuffer  Pointer to BitBuffer array.
 * \param bufSize  Length of BitBuffer array. (awaits size 2^n and <= MAX_BUFSIZE_BYTES)
 * \param config   Initialize BitStream as Reader or Writer.
 */
CDK_INLINE
HANDLE_CDK_BITSTREAM CDKcreateBitStream (UCHAR *pBuffer,
                                      UINT bufSize,
                                      CDK_BS_CFG config = BS_READER)
{
  HANDLE_CDK_BITSTREAM hBitStream = (HANDLE_CDK_BITSTREAM) CDKcalloc(1, sizeof(CDK_BITSTREAM));
  if (hBitStream == NULL) return NULL;
  CDK_InitBitBuffer(&hBitStream->hBitBuf, pBuffer, bufSize, 0) ;

  /* init cache */
  hBitStream->CacheWord = hBitStream->BitsInCache = 0 ;
  hBitStream->ConfigCache = config ;

  return hBitStream ;
}


/**
 * \brief Initialize BistreamBuffer. BitBuffer can point to filled BitBuffer array .
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param pBuffer    Pointer to BitBuffer array.
 * \param bufSize    Length of BitBuffer array in bytes. (awaits size 2^n and <= MAX_BUFSIZE_BYTES)
 * \param validBits  Number of valid BitBuffer filled Bits.
 * \param config     Initialize BitStream as Reader or Writer.
 * \return void
 */
CDK_INLINE
void CDKinitBitStream (HANDLE_CDK_BITSTREAM hBitStream,
                       UCHAR *pBuffer,
                       UINT bufSize,
                       UINT validBits,
                       CDK_BS_CFG config = BS_READER)
{
  CDK_InitBitBuffer(&hBitStream->hBitBuf, pBuffer, bufSize, validBits) ;

  /* init cache */
  hBitStream->CacheWord = hBitStream->BitsInCache = 0 ;
  hBitStream->ConfigCache = config ;
}


/**
 * \brief ResetBitbuffer Function. Reset states in BitBuffer and Cache.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param config     Initialize BitStream as Reader or Writer.
 * \return void
 */
CDK_INLINE void CDKresetBitbuffer( HANDLE_CDK_BITSTREAM hBitStream, CDK_BS_CFG config = BS_READER)
{
  CDK_ResetBitBuffer( &hBitStream->hBitBuf ) ;

  /* init cache */
  hBitStream->CacheWord = hBitStream->BitsInCache = 0 ;
  hBitStream->ConfigCache = config ;
}


/** DeleteBitStream.

    Deletes the in Create Bitstream allocated BitStream and BitBuffer.
*/
CDK_INLINE void CDKdeleteBitStream (HANDLE_CDK_BITSTREAM hBitStream)
{
  CDK_DeleteBitBuffer(&hBitStream->hBitBuf) ;
  CDKfree(hBitStream) ;
}


/**
 * \brief ReadBits Function (forward). This function returns a number of sequential
 *        bits from the input bitstream.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param numberOfBits  The number of bits to be retrieved. ( (0),1 <= numberOfBits <= 32)
 * \return the requested bits, right aligned
 * \return
 */

CDK_INLINE UINT CDKreadBits(HANDLE_CDK_BITSTREAM hBitStream,
                        const UINT numberOfBits)
{
  UINT bits = 0;
  INT missingBits = numberOfBits - hBitStream->BitsInCache;

  CDK_ASSERT(numberOfBits <= 32);
  if (missingBits > 0)
  {
    if (missingBits != 32) bits = hBitStream->CacheWord << missingBits;
    hBitStream->CacheWord = CDK_get32 (&hBitStream->hBitBuf);
    hBitStream->BitsInCache += CACHE_BITS;
  }

  hBitStream->BitsInCache -= numberOfBits;

  return ( bits | (hBitStream->CacheWord >> hBitStream->BitsInCache)) & BitMask[numberOfBits];

}

CDK_INLINE UINT CDKreadBit(HANDLE_CDK_BITSTREAM hBitStream)
{
  if (!hBitStream->BitsInCache)
  {
    hBitStream->CacheWord = CDK_get32 (&hBitStream->hBitBuf);
    hBitStream->BitsInCache = CACHE_BITS-1;
    return hBitStream->CacheWord >> 31;
  }
  hBitStream->BitsInCache--;

  return (hBitStream->CacheWord >> hBitStream->BitsInCache) & 1;
}

/**
 * \brief Read2Bits Function (forward). This function reads 2 sequential
 *        bits from the input bitstream. It is the optimized version
          of CDKreadBits() for reading 2 bits.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \return the requested bits, right aligned
 * \return
 */
CDK_INLINE UINT CDKread2Bits(HANDLE_CDK_BITSTREAM hBitStream)
{
  /* 
  ** Version corresponds to optimized CDKreadBits implementation
  ** calling CDK_get32, that keeps read pointer aligned.
  */
  UINT bits = 0;
  INT missingBits = 2 - hBitStream->BitsInCache;
  if (missingBits > 0)
  {
    bits = hBitStream->CacheWord << missingBits;
    hBitStream->CacheWord = CDK_get32 (&hBitStream->hBitBuf);
    hBitStream->BitsInCache += CACHE_BITS;
  }

  hBitStream->BitsInCache -= 2;

  return ( bits | (hBitStream->CacheWord >> hBitStream->BitsInCache)) & 0x3;
}

/**
 * \brief ReadBits Function (backward). This function returns a number of sequential bits
 *        from the input bitstream.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param numberOfBits  The number of bits to be retrieved.
 * \return the requested bits, right aligned
 */
CDK_INLINE UINT CDKreadBitsBwd(HANDLE_CDK_BITSTREAM hBitStream,
                           const UINT numberOfBits)
{
  const UINT validMask = BitMask [numberOfBits] ;

  if (hBitStream->BitsInCache <= numberOfBits)
  {
    const INT freeBits = (CACHE_BITS-1) - hBitStream->BitsInCache ;

    hBitStream->CacheWord = (hBitStream->CacheWord << freeBits) | CDK_getBwd (&hBitStream->hBitBuf,freeBits) ;
    hBitStream->BitsInCache += freeBits ;
  }

  hBitStream->BitsInCache -= numberOfBits ;

  return (hBitStream->CacheWord >> hBitStream->BitsInCache) & validMask ;
}

/**
 * \brief read an integer value using a varying number of bits from the bitstream
 *
 *        q.v. ISO/IEC FDIS 23003-3  Table 16
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param nBits1  number of bits to read for a small integer value or escape value  
 * \param nBits2  number of bits to read for a medium sized integer value or escape value
 * \param nBits3  number of bits to read for a large integer value
 * \return integer value read from bitstream 
 */
CDK_INLINE UINT escapedValue(
        HANDLE_CDK_BITSTREAM hBitStream,
        int nBits1,
        int nBits2,
        int nBits3
        )
{
  UINT value = CDKreadBits(hBitStream, nBits1);

  if (value == (UINT)(1 << nBits1) - 1) {
    UINT valueAdd = CDKreadBits(hBitStream, nBits2);
    value += valueAdd;
    if (valueAdd == (UINT)(1 << nBits2) - 1) {
      value += CDKreadBits(hBitStream, nBits3);
    }
  }

  return value;
}

/**
 * \brief return a number of bits from the bitBuffer.
 *        You have to know what you do! Cache has to be synchronized before using this
 *        function.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param numBits The number of bits to be retrieved.
 * \return the requested bits, right aligned
 */
CDK_INLINE UINT CDKgetBits (HANDLE_CDK_BITSTREAM hBitStream, UINT numBits)
{
    return CDK_get (&hBitStream->hBitBuf, numBits) ;
}


/**
 * \brief WriteBits Function. This function writes numberOfBits of value into bitstream.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param value         The data to be written
 * \param numberOfBits  The number of bits to be written
 * \return              Number of bits written
 */
CDK_INLINE UCHAR CDKwriteBits(HANDLE_CDK_BITSTREAM hBitStream, UINT value,
                     const UINT numberOfBits)
{
  const UINT validMask = BitMask [numberOfBits];


  if (hBitStream == NULL) {
    return numberOfBits;
  }

  if ((hBitStream->BitsInCache+numberOfBits) < CACHE_BITS)
  {
    hBitStream->BitsInCache += numberOfBits ;
    hBitStream->CacheWord   =  (hBitStream->CacheWord << numberOfBits) | (value & validMask);
  }
  else
  {
    /* Put always 32 bits into memory             */
    /* - fill cache's LSBits with MSBits of value */
    /* - store 32 bits in memory using subroutine */
    /* - fill remaining bits into cache's LSBits  */
    /* - upper bits in cache are don't care       */

    /* Compute number of bits to be filled into cache */
    int missing_bits = CACHE_BITS - hBitStream->BitsInCache;
    int remaining_bits = numberOfBits-missing_bits;
    value = value & validMask;
    /* Avoid shift left by 32 positions */
    UINT CacheWord = (missing_bits == 32) ? 0 : (hBitStream->CacheWord << missing_bits);
    CacheWord |= (value >> (remaining_bits));
    CDK_put(&hBitStream->hBitBuf, CacheWord, 32);

    hBitStream->CacheWord = value;
    hBitStream->BitsInCache = remaining_bits;
  }

 return numberOfBits;
}


/**
 * \brief WriteBits Function (backward). This function writes numberOfBits of value into bitstream.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param value         Variable holds data to be written.
 * \param numberOfBits  The number of bits to be written.
 * \return number of bits written
 */
CDK_INLINE UCHAR CDKwriteBitsBwd(HANDLE_CDK_BITSTREAM hBitStream, UINT value,
                                 const UINT numberOfBits)
{
  const UINT validMask = BitMask [numberOfBits] ;

  if ((hBitStream->BitsInCache+numberOfBits) <= CACHE_BITS)
  {
    hBitStream->BitsInCache += numberOfBits ;
    hBitStream->CacheWord   =  (hBitStream->CacheWord << numberOfBits) | (value & validMask);
  }
  else
  {
    CDK_putBwd(&hBitStream->hBitBuf, hBitStream->CacheWord, hBitStream->BitsInCache) ;
    hBitStream->BitsInCache = numberOfBits ;
    hBitStream->CacheWord   = (value & validMask) ;
  }

 return numberOfBits;
}

/**
 * \brief write an integer value using a varying number of bits from the bitstream
 *
 *        q.v. ISO/IEC FDIS 23003-3  Table 16
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param value   the data to be written
 * \param nBits1  number of bits to write for a small integer value or escape value  
 * \param nBits2  number of bits to write for a medium sized integer value or escape value
 * \param nBits3  number of bits to write for a large integer value
 * \return number of bits written
 */
CDK_INLINE UCHAR CDKwriteEscapedValue(
        HANDLE_CDK_BITSTREAM hBitStream,
        UINT value,
        UINT nBits1,
        UINT nBits2,
        UINT nBits3
        )
{
  UCHAR nbits = 0;
  UINT tmp = (1 << nBits1) - 1;

  if (value < tmp) {
    nbits += CDKwriteBits(hBitStream, value, nBits1);
  }
  else {
    nbits += CDKwriteBits(hBitStream, tmp, nBits1);
    value -= tmp;
    tmp = (1 << nBits2) - 1;

    if (value < tmp) {
      nbits += CDKwriteBits(hBitStream, value, nBits2);
    }
    else {
      nbits += CDKwriteBits(hBitStream, tmp, nBits2);
      value -= tmp;

      nbits += CDKwriteBits(hBitStream, value, nBits3);
    }
  }

  return nbits;
}


/**
 * \brief SyncCache Function. Clear cache after read forward.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \return void
 */
CDK_INLINE void CDKsyncCache (HANDLE_CDK_BITSTREAM hBitStream)
{
  if (hBitStream->ConfigCache == BS_READER)
#ifdef FUNCTION_CDK_pushBackBS_READER
    CDK_pushBackBS_READER (&hBitStream->hBitBuf,hBitStream->BitsInCache);
#else
    CDK_pushBack(&hBitStream->hBitBuf,hBitStream->BitsInCache, hBitStream->ConfigCache);
#endif
  else if (hBitStream->BitsInCache)                       /* BS_WRITER */
    CDK_put(&hBitStream->hBitBuf, hBitStream->CacheWord, hBitStream->BitsInCache) ;

  hBitStream->BitsInCache = 0 ;
  hBitStream->CacheWord   = 0 ;
}


/**
 * \brief SyncCache Function. Clear cache after read backwards.
 *
 * \param  hBitStream HANDLE_CDK_BITSTREAM handle
 * \return void
 */
CDK_INLINE void CDKsyncCacheBwd (HANDLE_CDK_BITSTREAM hBitStream)
{
  if (hBitStream->ConfigCache == BS_READER) {
    CDK_pushForward (&hBitStream->hBitBuf,hBitStream->BitsInCache,hBitStream->ConfigCache) ;
  } else {                      /* BS_WRITER */
    CDK_putBwd (&hBitStream->hBitBuf, hBitStream->CacheWord, hBitStream->BitsInCache) ;
  }

  hBitStream->BitsInCache = 0 ;
  hBitStream->CacheWord   = 0 ;
}


/**
 * \brief Byte Alignment Function.
 *        This function performs the byte_alignment() syntactic function on the input stream,
 *        i.e. some bits will be discarded/padded so that the next bits to be read/written will
 *        be aligned on a byte boundary with respect to the bit position 0.
 *
 * \param  hBitStream HANDLE_CDK_BITSTREAM handle
 * \return void
 */
CDK_INLINE void CDKbyteAlign (HANDLE_CDK_BITSTREAM hBitStream)
{
  CDKsyncCache (hBitStream) ;
  CDK_byteAlign (&hBitStream->hBitBuf, (UCHAR)hBitStream->ConfigCache) ;
}


/**
 * \brief Byte Alignment Function with anchor
 *        This function performs the byte_alignment() syntactic function on the input stream,
 *        i.e. some bits will be discarded so that the next bits to be read/written would be aligned
 *        on a byte boundary with respect to the given alignment anchor.
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param alignmentAnchor bit position to be considered as origin for byte alignment
 * \return void
 */
CDK_INLINE void CDKbyteAlign (HANDLE_CDK_BITSTREAM hBitStream, UINT alignmentAnchor)
{
  CDKsyncCache (hBitStream) ;
  if (hBitStream->ConfigCache == BS_READER) {
    CDK_pushForward (&hBitStream->hBitBuf,
                     (8 - ((alignmentAnchor - CDK_getValidBits(&hBitStream->hBitBuf)) & 0x07)) & 0x07,
                     hBitStream->ConfigCache) ;
  }
  else {
    CDK_put (&hBitStream->hBitBuf,
             0,
             (8 - ((CDK_getValidBits(&hBitStream->hBitBuf)-alignmentAnchor) & 0x07)) & 0x07 );
  }
}


/**
 * \brief Push Back(Cache) / For / BiDirectional Function.
 *        PushBackCache function ungets a number of bits erroneously read/written by the last Get() call.
 *       NB: The number of bits to be stuffed back into the stream may never exceed the
 *       number of bits returned by the immediately preceding Get() call.
 *
 *       PushBack function ungets a number of bits (combines cache and bitbuffer indices)
 *       PushFor  function gets a number of bits (combines cache and bitbuffer indices)
 *       PushBiDirectional gets/ungets number of bits as defined in PusBack/For function
 *       NB: The sign of bits is not known, so the function checks direction and calls
 *        appropriate function. (positive sign pushFor, negative sign pushBack )
 *
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param numberOfBits  The number of bits to be pushed back/for.
 * \return void
 */
CDK_INLINE void CDKpushBackCache (HANDLE_CDK_BITSTREAM hBitStream, const UINT numberOfBits)
{
  CDK_ASSERT ((hBitStream->BitsInCache+numberOfBits)<=CACHE_BITS);
  hBitStream->BitsInCache += numberOfBits ;
}

CDK_INLINE void CDKpushBack (HANDLE_CDK_BITSTREAM hBitStream, const UINT numberOfBits)
{
  if ((hBitStream->BitsInCache+numberOfBits)<CACHE_BITS && (hBitStream->ConfigCache == BS_READER) ) {
    hBitStream->BitsInCache += numberOfBits ;
    CDKsyncCache(hBitStream) ; /* sync cache to avoid invalid cache */
  }
  else {
    CDKsyncCache(hBitStream) ;
    CDK_pushBack(&hBitStream->hBitBuf,numberOfBits,hBitStream->ConfigCache);
  }
}

CDK_INLINE void CDKpushFor (HANDLE_CDK_BITSTREAM hBitStream, const UINT numberOfBits)
{
  if ( (hBitStream->BitsInCache>numberOfBits) && (hBitStream->ConfigCache == BS_READER) ) {
    hBitStream->BitsInCache -= numberOfBits;
  }
  else {
    CDKsyncCache(hBitStream) ;
    CDK_pushForward(&hBitStream->hBitBuf,numberOfBits,hBitStream->ConfigCache);
  }
}

CDK_INLINE void CDKpushBiDirectional (HANDLE_CDK_BITSTREAM hBitStream, const INT numberOfBits)
{
  if(numberOfBits>=0)  CDKpushFor(hBitStream, numberOfBits) ;
  else                 CDKpushBack(hBitStream, -numberOfBits) ;
}


/**
 * \brief GetValidBits Function.  Clear cache and return valid Bits from Bitbuffer.
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \return amount of valid bits that still can be read or were already written.
 *
 */
CDK_INLINE UINT CDKgetValidBits (HANDLE_CDK_BITSTREAM hBitStream)
{
  CDKsyncCache(hBitStream) ;
  return CDK_getValidBits(&hBitStream->hBitBuf) ;
}


/**
 * \brief return amount of unused Bits from Bitbuffer.
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \return amount of free bits that still can be written into the bitstream
 */
CDK_INLINE INT CDKgetFreeBits (HANDLE_CDK_BITSTREAM hBitStream)
{
   return CDK_getFreeBits (&hBitStream->hBitBuf) ;
}

/**
 * \brief reset bitcounter in bitBuffer to zero.
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \return void
 */
CDK_INLINE void CDKresetBitCnt (HANDLE_CDK_BITSTREAM hBitStream)
{
    CDKsyncCache (hBitStream) ;
    CDK_setBitCnt (&hBitStream->hBitBuf, 0) ;
}

/**
 * \brief set bitcoutner in bitBuffer to given value.
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \param value new value to be assigned to the bit counter
 * \return void
 */
CDK_INLINE void CDKsetBitCnt (HANDLE_CDK_BITSTREAM hBitStream, UINT value)
{
    CDKsyncCache (hBitStream) ;
    CDK_setBitCnt (&hBitStream->hBitBuf, value) ;
}

/**
 * \brief get bitcounter state from bitBuffer.
 * \param hBitStream HANDLE_CDK_BITSTREAM handle
 * \return current bit counter value
 */
CDK_INLINE INT  CDKgetBitCnt (HANDLE_CDK_BITSTREAM hBitStream)
{
    CDKsyncCache(hBitStream) ;
    return CDK_getBitCnt(&hBitStream->hBitBuf) ;
}


/**
 * \brief Fill the BitBuffer with a number of input bytes from  external source.
 *        The bytesValid variable returns the number of ramaining valid bytes in extern inputBuffer.
 *
 * \param hBitStream  HANDLE_CDK_BITSTREAM handle
 * \param inputBuffer Pointer to input buffer with bitstream data.
 * \param bufferSize  Total size of inputBuffer array.
 * \param bytesValid  Input: number of valid bytes in inputBuffer. Output: bytes still left unread in inputBuffer.
 * \return void
 */
CDK_INLINE void CDKfeedBuffer (HANDLE_CDK_BITSTREAM hBitStream, const UCHAR inputBuffer [], const UINT bufferSize, UINT *bytesValid)
{
  CDKsyncCache (hBitStream) ;
  CDK_Feed(&hBitStream->hBitBuf, inputBuffer, bufferSize, bytesValid  ) ;
}


/**
 * \brief fill destination BitBuffer with a number of bytes from source BitBuffer. The
 *        bytesValid variable returns the number of ramaining valid bytes in source BitBuffer.
 *
 * \param hBSDst            HANDLE_CDK_BITSTREAM handle to write data into
 * \param hBSSrc            HANDLE_CDK_BITSTREAM handle to read data from
 * \param bytesValid        Input: number of valid bytes in inputBuffer. Output: bytes still left unread in inputBuffer.
 * \return void
 */
CDK_INLINE void CDKcopyBuffer (HANDLE_CDK_BITSTREAM hBSDst, HANDLE_CDK_BITSTREAM hBSSrc, UINT *bytesValid)
{
  CDKsyncCache (hBSSrc) ;
  CDK_Copy (&hBSDst->hBitBuf, &hBSSrc->hBitBuf, bytesValid) ;
}


/**
 * \brief fill the outputBuffer with all valid bytes hold in BitBuffer. The WriteBytes
 *        variable returns the number of written Bytes.
 *
 * \param hBitStream    HANDLE_CDK_BITSTREAM handle
 * \param outputBuffer  Pointer to output buffer.
 * \param writeBytes    Number of bytes write to output buffer.
 * \return void
 */
CDK_INLINE void CDKfetchBuffer(HANDLE_CDK_BITSTREAM hBitStream, UCHAR *outputBuffer, UINT *writeBytes)
{
  CDKsyncCache (hBitStream) ;
  CDK_Fetch(&hBitStream->hBitBuf, outputBuffer, writeBytes);
}


#endif
