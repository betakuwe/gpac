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

#if   defined(__CC_ARM)
  /* ARM ADS/RVCT */
#if defined(__ARM_ARCH_7M__)   /* Cortex-M3 uses pure C-code, only SMULL, MUL available */

#endif /* defined(__ARM_ARCH_7M__) */

#if defined(__ARM_ARCH_7EM__)  /* Cortex-M4 uses pure C-code, allows DSP extension codes */

  #define FUNCTION_fixmuldiv2_DD
  #define FUNCTION_fixmuldiv2_DS
  #define FUNCTION_fixmuldiv2_SS
  #define FUNCTION_fixmuldiv2BitExact_DD

  #define FUNCTION_fixmul_DD
  #define FUNCTION_fixmul_DS
  #define FUNCTION_fixmul_SS
  #define FUNCTION_fixmulBitExact_DD

  #define FUNCTION_fixpow2div2_D
  #define FUNCTION_fixpow2div2_S
  #define FUNCTION_fixpow2_D
  #define FUNCTION_fixpow2_S


#if defined(FUNCTION_fixmuldiv2_DD)
inline INT fixmuldiv2_DD (const INT a, const INT b)
{
  return (((long long)a * b) >> 32);                       /* SMULL */
}
#endif

#if defined(FUNCTION_fixmuldiv2_DS)
inline INT fixmuldiv2_DS (const INT a, const SHORT b)
{
  return (((long long)a * b) >> 16);                       /* SMULWB */ 
}
#endif

#if defined(FUNCTION_fixmuldiv2_SS)
inline INT fixmuldiv2_SS (const SHORT a, const SHORT b)
{
  return ((long)a * b);                                    /* SMULBB or MUL */
}
#endif

#if defined(FUNCTION_fixmuldiv2BitExact_DD)
inline INT fixmuldiv2BitExact_DD (const INT a, const INT b)
{
  return fixmuldiv2_DD(a,b);                               /* SMULL */
}
#endif

#if defined(FUNCTION_fixmul_DD)
inline INT fixmul_DD (const INT a, const INT b)
{
  return fixmuldiv2_DD(a,b) << 1;                          /* SMULL, LSL #1 */
}
#endif

#if defined(FUNCTION_fixmul_DS)
inline INT fixmul_DS (const INT a, const SHORT b)
{
  return fixmuldiv2_DS(a,b) << 1;                          /* SMULWB, LSL #1 */
}
#endif

#if defined(FUNCTION_fixmul_SS)
inline INT fixmul_SS (const SHORT a, const SHORT b)
{
  return fixmuldiv2_SS(a,b) << 1;                          /* SMULBB, LSL #1 */
}
#endif

#if defined(FUNCTION_fixmulBitExact_DD)
inline INT fixmulBitExact_DD (const INT a, const INT b)
{
  return fixmul_DD(a,b);                                   /* SMULL, LSL #1 */
}
#endif

#if defined(FUNCTION_fixpow2div2_D)
inline INT fixpow2div2_D (const INT a)
{
  return fixmuldiv2_DD(a,a);                               /* SMULL */
}
#endif

#if defined(FUNCTION_fixpow2div2_S)
inline INT fixpow2div2_S (const SHORT a)
{
  return fixmuldiv2_SS(a,a);                               /* SMULBB */
}
#endif

#if defined(FUNCTION_fixpow2_D)
inline INT fixpow2_D (const INT a)
{
  return fixmuldiv2_DD(a,a) << 1;                          /* SMULL, LSL #1 */
}
#endif

#if defined(FUNCTION_fixpow2_S)
inline INT fixpow2_S (const SHORT a)
{
  return fixmuldiv2_SS(a,a) << 1;                          /* SMULBB, LSL #1 */
}
#endif

#endif /* defined(__ARM_ARCH_7EM__) */


#if !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

#define FUNCTION_fixmuldiv2_DD
#define FUNCTION_fixmul_DD

#define FUNCTION_fixmuldiv2BitExact_DD
#ifdef FUNCTION_fixmuldiv2BitExact_DD
  #define fixmuldiv2BitExact_DD(a,b) fixmuldiv2_DD(a,b)
#endif /* #ifdef FUNCTION_fixmuldiv2BitExact_DD */

#define FUNCTION_fixmulBitExact_DD
#ifdef FUNCTION_fixmulBitExact_DD
  #define fixmulBitExact_DD(a,b) fixmul_DD(a,b)
#endif /* #ifdef FUNCTION_fixmulBitExact_DD */

#define FUNCTION_fixmuldiv2BitExact_DS
#ifdef FUNCTION_fixmuldiv2BitExact_DS
  #define fixmuldiv2BitExact_DS(a,b) fixmuldiv2_DS(a,b)
#endif /* #ifdef FUNCTION_fixmuldiv2BitExact_DS */

#define FUNCTION_fixmulBitExact_DS
#ifdef FUNCTION_fixmulBitExact_DS
  #define fixmulBitExact_DS(a,b) fixmul_DS(a,b)
#endif /* #ifdef FUNCTION_fixmulBitExact_DS */

#ifdef FUNCTION_fixmuldiv2_DD
#ifdef __ARM_ARCH_6__
static inline INT fixmuldiv2_DD (const INT a, const INT b)
{
  INT result ;
  __asm { smmul result, a, b }
return result ;
}
#else
inline INT fixmuldiv2_DD (const INT a, const INT b)
{
  INT discard, result ;
  __asm { smull discard, result, a, b }
  return result ;
}
#endif
#endif /* #ifdef FUNCTION_fixmuldiv2_DD */

#if defined(__ARM_ARCH_5TE__)
#define FUNCTION_fixmuldiv2_SD
#ifdef FUNCTION_fixmuldiv2_SD
inline INT fixmuldiv2_SD (const SHORT a, const INT b)
{
  INT result ;
  __asm { smulwb result, b, a }
  return result ;
}
#endif /* #ifdef FUNCTION_fixmuldiv2_SD */
#endif /* #if defined(__ARM_ARCH_5TE__) */

#ifdef FUNCTION_fixmul_DD
inline INT fixmul_DD(const INT a, const INT b)
{
    return (fixmuldiv2_DD(a,b)<<1);
}
#endif /* #ifdef FUNCTION_fixmul_DD */
#endif /* !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__) */

#elif defined(__GNUC__) && defined(__arm__)
/* ARM with GNU compiler */

#define FUNCTION_fixmuldiv2_DD

#define FUNCTION_fixmuldiv2BitExact_DD
#ifdef FUNCTION_fixmuldiv2BitExact_DD
  #define fixmuldiv2BitExact_DD(a,b) fixmuldiv2_DD(a,b)
#endif /* #ifdef FUNCTION_fixmuldiv2BitExact_DD */

#define FUNCTION_fixmulBitExact_DD
#ifdef FUNCTION_fixmulBitExact_DD
  #define fixmulBitExact_DD(a,b) (fixmuldiv2BitExact_DD(a,b)<<1)
#endif /* #ifdef FUNCTION_fixmulBitExact_DD */

#define FUNCTION_fixmuldiv2BitExact_DS
#ifdef FUNCTION_fixmuldiv2BitExact_DS
  #define fixmuldiv2BitExact_DS(a,b) fixmuldiv2_DS(a,b)
#endif /* #ifdef FUNCTION_fixmuldiv2BitExact_DS */

#define FUNCTION_fixmulBitExact_DS
#ifdef FUNCTION_fixmulBitExact_DS
  #define fixmulBitExact_DS(a,b) fixmul_DS(a,b)
#endif /* #ifdef FUNCTION_fixmulBitExact_DS */

#ifdef FUNCTION_fixmuldiv2_DD
inline INT fixmuldiv2_DD (const INT a, const INT b)
{
  INT result ;
#if defined(__ARM_ARCH_8__)
  INT64 result64;
  __asm__ ("smull %x0, %w1, %w2;\n"
           "asr %x0, %x0, #32;     " : "=r" (result64)
                                   :  "r" (a), "r" (b)) ;
  result = (INT) result64;
#elif defined(__ARM_ARCH_6__) || defined(__TARGET_ARCH_7E_M)
  __asm__ ("smmul %0, %1, %2" : "=r" (result)
                              : "r" (a), "r" (b)) ;
#else
  INT discard;
  __asm__ ("smull %0, %1, %2, %3" : "=&r" (discard), "=r" (result)
                                  : "r" (a), "r" (b)) ;
#endif
  return result ;
}
#endif /* #ifdef FUNCTION_fixmuldiv2_DD */

#if defined(__ARM_ARCH_8__)
#define FUNCTION_fixmuldiv2_SD
#ifdef FUNCTION_fixmuldiv2_SD
inline INT fixmuldiv2_SD (const SHORT a, const INT b)
{
  return fixmuldiv2_DD((INT)(a<<16), b);
}
#endif /* #ifdef FUNCTION_fixmuldiv2_SD */
#elif defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_6__)
#define FUNCTION_fixmuldiv2_SD
#ifdef FUNCTION_fixmuldiv2_SD
inline INT fixmuldiv2_SD (const SHORT a, const INT b)
{
  INT result ;
  __asm__ ("smulwb %0, %1, %2"
    : "=r" (result)
    : "r" (b), "r" (a)) ;
  return result ;
}
#endif /* #ifdef FUNCTION_fixmuldiv2_SD */
#endif

#define FUNCTION_fixmul_DD
#ifdef FUNCTION_fixmul_DD
#if defined(__ARM_ARCH_8__)
inline INT fixmul_DD (const INT a, const INT b)
{
  INT64 result64;

  __asm__ ("smull %x0, %w1, %w2;\n"
           "asr %x0, %x0, #31;     " : "=r" (result64)
                                   :  "r" (a), "r" (b)) ;
  return (INT) result64;
}
#else
inline INT fixmul_DD(const INT a, const INT b)
{
    return (fixmuldiv2_DD(a,b)<<1);
}
#endif /* __ARM_ARCH_8__ */
#endif /* #ifdef FUNCTION_fixmul_DD */

#elif defined(__TMS470__)

/* TI ARM Compiler using V5E (ARM9) C Intrinsics */
#define FUNCTION_fixmuldiv2_DD
#define FUNCTION_fixmuldiv2_SD
#define FUNCTION_fixmuldiv2_SS

#ifdef FUNCTION_fixmuldiv2_DD
inline FIXP_DBL fixmuldiv2_DD (const FIXP_DBL a, const FIXP_DBL b)
{
   INT64 result = (b*(INT64)a) >> 32;
   return (FIXP_DBL)result;
}
#endif /* #ifdef FUNCTION_fixmuldiv2_DD */

#ifdef FUNCTION_fixmuldiv2_SD
inline FIXP_DBL fixmuldiv2_SD (const FIXP_SGL a, const FIXP_DBL b)
{
  return _smulwb(b, a);
}
#endif /* #ifdef FUNCTION_fixmuldiv2_SD */

#ifdef FUNCTION_fixmuldiv2_SS
#define fixmuldiv2_SS _smulbb
#endif /* #ifdef FUNCTION_fixmuldiv2_SS */

#endif /* defined(__GNUC__) && defined(__arm__) */

#endif /* __arm__ */

