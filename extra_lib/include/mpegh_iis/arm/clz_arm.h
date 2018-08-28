/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved

   $Id$
   Author(s):
   Description: fixed point intrinsics

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/
#if defined(__arm__)

#define FUNCTION_fixnormz_D
#if   defined(__CC_ARM)
  /* RealView and ADS  */
  #if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    #ifdef FUNCTION_fixnormz_D
    inline INT fixnormz_D(LONG value)
    {
      INT result;
      result = __clz(value);
      return result;
    }
    #endif /* #ifdef FUNCTION_fixnormz_D */

  #elif defined(__ARM_ARCH_5TE__)
    #define FUNCTION_fixnorm_D
    #define FUNCTION_fixnormz_S

    #ifdef FUNCTION_fixnormz_D
    inline INT fixnormz_D(LONG value)
    {
      INT result;
      __asm {
        clz result, value
      }
      return result;
    }
    #endif /* #ifdef FUNCTION_fixnormz_D */

    #ifdef FUNCTION_fixnorm_D
    inline INT fixnorm_D(LONG value)
    {
      INT result;
      __asm {
        subs result, value, #0
        mvnlt result, result
        clzne  result, result
        subne result, result, #1
      }
      return result;
    }
    #endif /* #ifdef FUNCTION_fixnorm_D */

    #ifdef FUNCTION_fixnormz_S
    inline INT fixnormz_S(SHORT value)
    {
      INT result;
      __asm {
        subs result, value, #0
        clzge  result, result
        movlt result, #0
        subge result, result, #16
      }
      return result;
    }
    #endif /* #ifdef FUNCTION_fixnormz_S */

    #if (__ARMCC_VERSION > 320000)
      #define FUNCTION_fixnorm_S
      #ifdef FUNCTION_fixnorm_S
        inline INT fixnorm_S(SHORT value)
        {
          INT result;
    __asm {
            lsls result, value, #16
            mvnmi result, result
            clzne  result, result
            subne result, result, #1
          }
          return result;
        }
      #endif /* #ifdef FUNCTION_fixnorm_S */
    #endif /* #if (__ARMCC_VERSION > 320000) */

  #endif /* defined (__ARM_ARCH_xx__) */

#elif defined(__GNUC__)
  /* ARM gcc*/

#if defined (__ARM_ARCH_5TE__) || defined(__ARM_ARCH_8__)
#if defined FUNCTION_fixnormz_D
  #define FUNCTION_fixnorm_D
  #define FUNCTION_fixnormz_S
  #define FUNCTION_fixnorm_S
#endif

  #ifdef FUNCTION_fixnormz_D
  inline INT fixnormz_D(LONG value)
  {
    INT result;
    #if defined(__ARM_ARCH_8__)
    asm("clz %w0, %w1 ": "=r"(result) : "r"(value) );
    #else
    asm("clz %0, %1 ": "=r"(result) : "r"(value) );
    #endif
    return result;
  }
  #endif /* #ifdef FUNCTION_fixnormz_D */

  #ifdef FUNCTION_fixnorm_D
  inline INT fixnorm_D(LONG value)
  {
    INT result;
#if defined(__ARM_ARCH_8__)
    if (!value) return 0;
    if (value < 0)
        value = ~value;
    return fixnormz_D(value) - 1;
#else
    asm("subs  %0, %1, #0 \n\t"
        "mvnlt %0, %0     \n\t"
        "clzne %0, %0     \n\t"
        "subne %0, %0, #1 \n\t"
        : "=r"(result) 
        : "r"(value)
        : "cc" );
#endif
    return result;
  }
  #endif /* #ifdef FUNCTION_fixnorm_D */

  #ifdef FUNCTION_fixnormz_S
  inline INT fixnormz_S(SHORT value)
  {
    INT result;
#if defined(__ARM_ARCH_8__)
    result = (LONG) (value<<16);
    if (result == 0)
        result = 16;
    else
        result = fixnormz_D(result);
#else
    asm("subs  %0, %1, #0    \n\t"
        "clzge %0, %0        \n\t"
        "movlt %0, #0        \n\t"
        "subge %0, %0, #16   \n\t"
        : "=r"(result) 
        : "r"(value)
        : "cc" );
#endif
    return result;
  }
#endif /* #ifdef FUNCTION_fixnormz_S */

  #ifdef FUNCTION_fixnorm_S
  inline INT fixnorm_S(SHORT value)
  {
    INT result;
#if defined(__ARM_ARCH_8__)
    LONG lvalue = (LONG) (value << 16);
    if (!lvalue) return 0;
    if (lvalue < 0)
        lvalue = ~lvalue;
    return fixnormz_D(lvalue) - 1;
#else
    asm("lsls  %0, %1, #16 \n\t"
        "mvnlt %0, %0      \n\t"
        "clzne %0, %0      \n\t"
        "subne %0, %0, #1  \n\t"
        : "=r"(result) 
        : "r"(value)
        : "cc" );
#endif
    return result;
  }
#endif /* #ifdef FUNCTION_fixnorm_S */


#endif

#elif defined(__TMS470__)

/* TI ARM Compiler */

#ifdef FUNCTION_fixnormz_D
inline INT fixnormz_D(INT value) {
  return _norm(value);
}
#endif /* #ifdef FUNCTION_fixnormz_D */

#endif /* arm toolchain */

#endif /* __arm__ */

