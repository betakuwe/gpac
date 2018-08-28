/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2005)
                               All Rights Reserved

   $Id$
   Author(s):
   Description: Scaling operations

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef SCALE_H
#define SCALE_H

#include "CDK_tools_settings.h"
#include "common_fix.h"
#include "genericStds.h"
#include "fixminmax.h"

  #define SCALE_INLINE


#if   defined(__arm__)
#include "arm/scale.h"

#elif defined(__mips__)
#include "mips/scale.h"

#elif defined(__ADSP21000__)
#include "sharc/scale.h"

#elif defined(__XTENSA__)
#include "xtensa/scale_xtensa.h"

#elif defined(__hgon__)
#include "hexagon/scale_hexagon.h"

#endif


SCALE_INLINE void scaleValues(FIXP_SGL *vector, INT len, INT scalefactor);
SCALE_INLINE void scaleValues(FIXP_DBL *vector, INT len, INT scalefactor);
SCALE_INLINE void scaleValues(FIXP_DBL *dst, const FIXP_DBL *src, INT len, INT scalefactor);
#if (SAMPLE_BITS == 16)
SCALE_INLINE void scaleValues(FIXP_PCM *dst, const FIXP_DBL *src, INT len, INT scalefactor);
#endif
SCALE_INLINE void scaleValues(FIXP_PCM *dst, const FIXP_SGL *src, INT len, INT scalefactor);
SCALE_INLINE void scaleCplxValues(FIXP_DBL *r_dst, FIXP_DBL *i_dst, const FIXP_DBL *r_src, const FIXP_DBL *i_src, INT len, INT scalefactor);
SCALE_INLINE void scaleValuesWithFactor(FIXP_DBL *vector, FIXP_DBL factor, INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(FIXP_DBL *vector,             INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(FIXP_DBL *dst, FIXP_DBL *src, INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(FIXP_SGL *dst, FIXP_DBL *src, INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(INT_PCM  *dst, FIXP_DBL *src, INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(FIXP_SGL *vector,             INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(FIXP_SGL *dst, FIXP_SGL *src, INT len, INT scalefactor);
SCALE_INLINE void scaleValuesSaturate(INT_PCM *dst, INT_PCM *src, INT len, INT scalefactor);
SCALE_INLINE INT getScalefactorShort(const SHORT *vector, INT len);
SCALE_INLINE INT getScalefactorPCM(const INT_PCM *vector, INT len, INT stride);
SCALE_INLINE INT getScalefactor(const FIXP_DBL *vector, INT len);
SCALE_INLINE INT getScalefactor(const FIXP_SGL *vector, INT len);

#ifndef FUNCTION_scaleValue
/*!
 *
 *  \brief Multiply input by \f$ 2^{scalefactor} \f$
 *
 *  \return Scaled input
 *
 */
#define FUNCTION_scaleValue
inline
FIXP_DBL scaleValue(const FIXP_DBL value, /*!< Value */
                    INT scalefactor   /*!< Scalefactor */
                   )
{
  if(scalefactor > 0)
    return (value<<scalefactor);
  else
    return (value>>(-scalefactor));
}
inline
FIXP_SGL scaleValue(const FIXP_SGL value, /*!< Value */
                    INT scalefactor   /*!< Scalefactor */
                   )
{
  if(scalefactor > 0)
    return (value<<scalefactor);
  else
    return (value>>(-scalefactor));
}
#endif

#ifndef FUNCTION_scaleValueSaturate
/*!
 *
 *  \brief Multiply input by \f$ 2^{scalefactor} \f$
 *  \param value The value to be scaled.
 *  \param the shift amount
 *  \return \f$ value * 2^scalefactor \f$
 *
 */
#define FUNCTION_scaleValueSaturate
inline
FIXP_DBL scaleValueSaturate(
        const FIXP_DBL value,
        INT scalefactor               /* in range -31 ... +31 */
        )
{
  int headroom = fixnormz_D((INT) value ^ (INT) ((value>>31)));  /* headroom in range 1...32 */
  if(scalefactor >= 0) 
  {
    /* shift left: saturate in case of headroom less/equal scalefactor */
    if (headroom <= scalefactor) 
    {
      if (value > (FIXP_DBL) 0)
        return (FIXP_DBL)MAXVAL_DBL;                  /* 0x7FFF.FFFF */
      else
        return (FIXP_DBL) MINVAL_DBL + (FIXP_DBL) 1;  /* 0x8000.0001 */
    } 
    else 
    {
      return fMax((value<<scalefactor), (FIXP_DBL) MINVAL_DBL + (FIXP_DBL) 1);
    }
  } 
  else 
  {
    scalefactor = -scalefactor;
    /* shift right: clear in case of 32-headroom greater/equal -scalefactor */
    if ((DFRACT_BITS-headroom) <= scalefactor)
    {
      return (FIXP_DBL) 0;
    }
    else
    {
      return fMax((value>>scalefactor), (FIXP_DBL) MINVAL_DBL + (FIXP_DBL) 1);
    }
  }
}
#endif

#ifndef FUNCTION_scaleValueInPlace
/*!
 *
 *  \brief Multiply input by \f$ 2^{scalefactor} \f$ in place
 *
 *  \return void
 *
 */
#define FUNCTION_scaleValueInPlace
inline
void scaleValueInPlace(
        FIXP_DBL *value, /*!< Value */
        INT scalefactor   /*!< Scalefactor */
        )
{
  INT newscale;
  /* Note: The assignment inside the if conditional allows combining a load with the compare to zero (on ARM and maybe others) */
  if ((newscale = (scalefactor)) >= 0) {
    *(value) <<= newscale;
  } else {
    *(value) >>= -newscale;
  }
}
#endif

/*!
 *
 *  \brief  Scale input value by 2^{scale} and saturate output to 2^{dBits-1}
 *  \return scaled and saturated value
 *
 *  This macro scales src value right or left and applies saturation to (2^dBits)-1
 *  maxima output.
 */

#ifndef SATURATE_RIGHT_SHIFT
  #define SATURATE_RIGHT_SHIFT(src, scale, dBits)                                                      \
            ( (((LONG)(src)>>(scale)) > (LONG)(((1U)<<((dBits)-1))-1))      ? (LONG)(((1U)<<((dBits)-1))-1)    \
              : (((LONG)(src)>>(scale)) < ~((LONG)(((1U)<<((dBits)-1))-1))) ? ~((LONG)(((1U)<<((dBits)-1))-1)) \
              : ((LONG)(src) >> (scale)) )
#endif

#ifndef SATURATE_LEFT_SHIFT
  #define SATURATE_LEFT_SHIFT(src, scale, dBits)                                                       \
            ( ((LONG)(src) > ((LONG)(((1U)<<((dBits)-1))-1)>>(scale)))    ? (LONG)(((1U)<<((dBits)-1))-1)      \
              : ((LONG)(src) < ~((LONG)(((1U)<<((dBits)-1))-1)>>(scale))) ? ~((LONG)(((1U)<<((dBits)-1))-1))   \
              : ((LONG)(src) << (scale)) )
#endif

#ifndef SATURATE_SHIFT
#define SATURATE_SHIFT(src, scale, dBits)               \
     ( ((scale) < 0)                                      \
      ? SATURATE_LEFT_SHIFT((src), -(scale), (dBits))   \
      : SATURATE_RIGHT_SHIFT((src), (scale), (dBits)) )
#endif

/*
 * Alternative shift and saturate left, saturates to -0.99999 instead of -1.0000
 * to avoid problems when inverting the sign of the result.
 */
#ifndef SATURATE_LEFT_SHIFT_ALT
#define SATURATE_LEFT_SHIFT_ALT(src, scale, dBits)                                                     \
            ( ((LONG)(src) > ((LONG)(((1U)<<((dBits)-1))-1)>>(scale)))    ? (LONG)(((1U)<<((dBits)-1))-1)      \
              : ((LONG)(src) < ~((LONG)(((1U)<<((dBits)-1))-2)>>(scale))) ? ~((LONG)(((1U)<<((dBits)-1))-2))   \
              : ((LONG)(src) << (scale)) )
#endif

#ifndef SATURATE_RIGHT_SHIFT_ALT
  #define SATURATE_RIGHT_SHIFT_ALT(src, scale, dBits)                                                  \
            ( (((LONG)(src)>>(scale)) > (LONG)(((1U)<<((dBits)-1))-1))      ? (LONG)(((1U)<<((dBits)-1))-1)    \
              : (((LONG)(src)>>(scale)) < ~((LONG)(((1U)<<((dBits)-1))-2))) ? ~((LONG)(((1U)<<((dBits)-1))-2)) \
              : ((LONG)(src) >> (scale)) )
#endif

/*
  Interface:
  src  : FIXP_DBL  : input value to be rounded/shifted right
  scale: INT       : shift right factor in range [0..31]
  dBits: const INT : number of data bits of result, e.g. SAMPLE_BITS, DFRACT_BITS, SAMPLE_BITS_QMFOUT
  Description:
    The result is created by adding a rounding constant to src and shifting the sum right by scale.
    The shift-result is then compared to the maximum positive/negative values representable in dBits 
    format and limited accordingly, if exceeding.
 */
#ifndef SATURATE_RIGHT_SHIFT_RND
#define SATURATE_RIGHT_SHIFT_RND(src, scale, dBits)  (scale ? SATURATE_RIGHT_SHIFT(src+(FIXP_DBL)(1<<(scale-1)), scale, dBits) : SATURATE_RIGHT_SHIFT(src, scale, dBits) )
#endif


#ifndef SATURATE_INT_PCM_RIGHT_SHIFT
#define SATURATE_INT_PCM_RIGHT_SHIFT(src, scale) SATURATE_RIGHT_SHIFT(src, scale, SAMPLE_BITS)
#endif

#ifndef SATURATE_INT_PCM_LEFT_SHIFT
#define SATURATE_INT_PCM_LEFT_SHIFT(src, scale) SATURATE_LEFT_SHIFT(src, scale, SAMPLE_BITS)
#endif

#endif /* #ifndef SCALE_H */
