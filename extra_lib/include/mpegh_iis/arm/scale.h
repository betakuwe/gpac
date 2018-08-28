/********************************  Fraunhofer IIS  ***************************

                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved

   $Id:
   Author(s):
   Description: ARM scaling operations

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#if defined(__CC_ARM)

#if !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

#if defined(__ARM_ARCH_6__)

inline static INT shiftRightSat(const INT src, const int scale)
{
  INT result;
  
  __asm {
      ssat result,#SAMPLE_BITS,(src>>scale)
  }
      
  return result;
}

  #define SATURATE_INT_PCM_RIGHT_SHIFT(src, scale) shiftRightSat(src, scale)

inline static INT shiftLeftSat(const INT src, const int scale)
{
  INT result;
  
  __asm  {
      ssat result,#SAMPLE_BITS,(src<<scale)
  }
  return result;
}
        
  #define SATURATE_INT_PCM_LEFT_SHIFT(src, scale)  shiftLeftSat(src, scale)

#endif /* __ARM_ARCH_6__ */

#endif /* !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__) */


#elif defined(__GNUC__) /* GCC Compiler */

#if defined(__ARM_ARCH_6__)

inline static INT shiftRightSat(INT src, int scale)
{
  INT result;
  asm(
      "ssat %0,%2,%0;\n"
          
      : "=&r"(result)
      : "r"(src>>scale), "M"(SAMPLE_BITS)
      );
      
  return result;
}

  #define SATURATE_INT_PCM_RIGHT_SHIFT(src, scale) shiftRightSat(src, scale)

inline static INT shiftLeftSat(INT src, int scale)
{
  INT result;
  asm(
      "ssat %0,%2,%0;\n"
          
      : "=&r"(result)
      : "r"(src<<scale), "M"(SAMPLE_BITS)
      );
      
  return result;
}
        
  #define SATURATE_INT_PCM_LEFT_SHIFT(src, scale)  shiftLeftSat(src, scale)

#endif /* __ARM_ARCH_6__ */

#endif /* compiler selection */

#define FUNCTION_scaleValueInPlace
#ifdef FUNCTION_scaleValueInPlace
inline
void scaleValueInPlace(FIXP_DBL *value, /*!< Value */
                       INT scalefactor   /*!< Scalefactor */
                       )
{
  INT newscale;
  if ((newscale = scalefactor) >= 0)
    *value <<= newscale;
  else
    *value >>= -newscale;
}
#endif /* #ifdef FUNCTION_scaleValueInPlace */


  #define SATURATE_RIGHT_SHIFT(src, scale, dBits)                                                        \
      ( (((LONG)(src) ^ ((LONG)(src) >> (DFRACT_BITS-1)))>>(scale)) > (LONG)(((1U)<<((dBits)-1))-1))     \
          ? ((LONG)(src) >> (DFRACT_BITS-1)) ^ (LONG)(((1U)<<((dBits)-1))-1)                             \
          : ((LONG)(src) >> (scale))

  #define SATURATE_LEFT_SHIFT(src, scale, dBits)                                                         \
      ( ((LONG)(src) ^ ((LONG)(src) >> (DFRACT_BITS-1))) > ((LONG)(((1U)<<((dBits)-1))-1) >> (scale)) )  \
          ? ((LONG)(src) >> (DFRACT_BITS-1)) ^ (LONG)(((1U)<<((dBits)-1))-1)                             \
          : ((LONG)(src) << (scale))

#if defined(__ARM_NEON__)
#define FUNCTION_scaleCplxValues
#endif
