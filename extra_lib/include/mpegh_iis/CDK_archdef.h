/***************************  Fraunhofer IIS  Tools  ***********************

                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved

   $Id$
   Author(s):
   Description:

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __ARCH_H__
#define __ARCH_H__

/* Performance / Quality profile selector */
  #define CDK_HIGH_PERFORMANCE

/* Unify some few toolchain specific defines to avoid having large "or" macro contraptions all over the source code. */

/* Take action against VisualStudio 2005 crosscompile problems. */

/* Use single macro (the GCC built in macro) for architecture identification independent of the particular toolchain */
#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || (defined(_MSC_VER) && defined(_M_IX86)) || (defined(_MSC_VER) && defined(_M_X64)) || defined (__x86_64__)
#define __x86__
#endif

#if (defined(_M_ARM) || defined(__CC_ARM) || defined(__TMS470__)) && !defined(__arm__) || defined(__aarch64__)
#define __arm__
#endif



/* backward compatibility with ARM ADS 1.2 */
#if defined(__CC_ARM) && (__ARMCC_VERSION < 200000)
#ifdef __TARGET_FEATURE_DSPMUL
#define __TARGET_ARCH_ARM 5
#else
#define __TARGET_ARCH_ARM 4
#endif
#endif

#if (__TARGET_ARCH_ARM == 5) || defined(__TARGET_FEATURE_DSPMUL) || (_M_ARM == 5) || defined(__ARM_ARCH_5TEJ__) || defined(__TI_TMS470_V5__) || defined(__ARM_ARCH_7EM__)
/* Define __ARM_ARCH_5TE__ if armv5te features are supported  */
#define __ARM_ARCH_5TE__
#endif

#if (__TARGET_ARCH_ARM == 6) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6ZK__) || defined(__TI_TMS470_V6__)
/* Define __ARM_ARCH_6__ if the armv6 intructions are being supported. */
#define __ARM_ARCH_5TE__
#define __ARM_ARCH_6__
#endif

#if defined(__TARGET_ARCH_7_R) || defined(__ARM_ARCH_7R__) || defined(__TI_TMS470_V7R4__)
/* Define __ARM_ARCH_7_A__ if the armv7 intructions are being supported. */
#define __ARM_ARCH_5TE__
#define __ARM_ARCH_6__
#define __ARM_ARCH_7_R__
#endif

#if defined(__TARGET_ARCH_7_A) || defined(__ARM_ARCH_7A__) || defined(__TI_TMS470_V7A8__)
/* Define __ARM_ARCH_7_A__ if the armv7 intructions are being supported. */
#define __ARM_ARCH_5TE__
#define __ARM_ARCH_6__
#define __ARM_ARCH_7_A__
#endif

#if defined(__TARGET_ARCH_7_M) || defined(__ARM_ARCH_7_M__) || defined(__TI_TMS470_V7M3__)
/* Define __ARM_ARCH_7M__ if the ARMv7-M instructions are being supported, e.g. Cortex-M3. */
#define __ARM_ARCH_7M__
#endif

#if defined(__TARGET_ARCH_7E_M) || defined(__ARM_ARCH_7E_M__)
/* Define __ARM_ARCH_7EM__ if the ARMv7-ME instructions are being supported, e.g. Cortex-M4. */
#define __ARM_ARCH_7EM__
#endif

#if defined (__aarch64__)
#define __ARM_ARCH_8__
#endif

#if (defined(__TARGET_FEATURE_NEON) || defined(__ARM_NEON) ) && !defined(__ARM_NEON__)
/* Detect and unify macros for neon feature. */
#define __ARM_NEON__
#endif
#if defined(__ARM_ARCH_8__) || defined(__ARM_NEON_DISABLE__)
#undef __ARM_NEON__       /* disable use of ARMv7 legacy NEON */
#ifndef __ARM_AARCH64_NEON_DISABLE__
#ifndef __llvm__    /* exclude android clang compiler, something is still wrong for armv8 NEON compiles */
#define __ARM_AARCH64_NEON__
#endif
#endif 
#endif

#ifdef _M_ARM
#include "cmnintrin.h"
#include "armintr.h"
#endif




#if defined(__hexagon__) || defined(__HEXAGON_V4__) || defined(__HEXAGON_V55__) || defined(__qdsp6__) || defined(__QDSP6_V4__) || defined(__QDSP6_V55__)
#define __hgon__
#endif

#if defined(__XTENSA_CONFIG__hifi2_std) || defined(__XTENSA_CONFIG__hifiep_bd5)
#define __XTENSA_CONFIG__DC_D_330HiFi
#endif

#if defined(__XTENSA_CONFIG__hifi3_bd5)
#define __XTENSA_CONFIG__DC_E_330HiFi
#define __XTENSA_CONFIG__DC_D_330HiFi
#endif

#if defined(__CC_ARM) && !(defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__))
/* ARM optimizations do not work for arch 7M, i.e. e.g. Cortex-M3, since it only supports THUMB */
#define CCARM_SUPPORT_ARM_INSTR /* support ARM instruction set */
#endif


/* Define preferred Multiplication type */
#if defined(CDK_HIGH_PERFORMANCE) && !defined(CDK_HIGH_QUALITY) /* CDK_HIGH_PERFORMANCE */

#if defined(__mips__)
#define ARCH_PREFER_MULT_16x16
#undef SINETABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT
#undef WINDOWTABLE_16BIT

#elif defined(__arm__) && defined(__ARM_ARCH_8__)
#define ARCH_PREFER_MULT_32x16
#define SINETABLE_16BIT
#define POW2COEFF_16BIT
#define LDCOEFF_16BIT
#define WINDOWTABLE_16BIT

#elif defined(__arm__) && defined(__ARM_ARCH_5TE__)
#define ARCH_PREFER_MULT_32x16
#define SINETABLE_16BIT
#define POW2COEFF_16BIT
#define LDCOEFF_16BIT
#define WINDOWTABLE_16BIT

#elif defined(__arm__) && defined(__ARM_ARCH_7M__)
#define ARCH_PREFER_MULT_32x16
#define SINETABLE_16BIT
#define POW2COEFF_16BIT
#define LDCOEFF_16BIT
#define WINDOWTABLE_16BIT

#elif defined(__arm__) && defined(__ARM_ARCH_7EM__)
#define ARCH_PREFER_MULT_32x32
#define ARCH_PREFER_MULT_32x16
#define SINETABLE_16BIT
#define POW2COEFF_16BIT
#define LDCOEFF_16BIT
#define WINDOWTABLE_16BIT

#elif defined(__arm__) && !defined(__ARM_ARCH_5TE__)
#define ARCH_PREFER_MULT_16x16
#undef SINETABLE_16BIT
#undef WINDOWTABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT

#elif defined(__ADSP21000__)
  #define ARCH_PREFER_MULT_32x32
  #define SINETABLE_16BIT
  #define WINDOWTABLE_16BIT
  #undef POW2COEFF_16BIT
  #undef LDCOEFF_16BIT

#elif defined(__XTENSA__)
  #define ARCH_PREFER_MULT_32x16
  #define ARCH_PREFER_MULT_16x16
  #define SINETABLE_16BIT
  #define POW2COEFF_16BIT
  #define LDCOEFF_16BIT
  #define WINDOWTABLE_16BIT

#elif defined(__hgon__)
  #define ARCH_PREFER_MULT_32x16
  #define SINETABLE_16BIT
  #define POW2COEFF_16BIT
  #define LDCOEFF_16BIT
  #define WINDOWTABLE_16BIT

#elif defined(__x86__)
#define ARCH_PREFER_MULT_32x16
#define SINETABLE_16BIT
#define WINDOWTABLE_16BIT
#define POW2COEFF_16BIT
#define LDCOEFF_16BIT

#else
  #error  >>>> Please set architecture characterization defines for your platform (CDK_HIGH_PERFORMANCE)! <<<<

#endif /* Architecture switches */

#elif !defined(CDK_HIGH_PERFORMANCE) && defined(CDK_HIGH_QUALITY) /* CDK_HIGH_QUALITY */

#if defined(__mips__)
#define ARCH_PREFER_MULT_32x32
#undef SINETABLE_16BIT
#undef WINDOWTABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT

#elif (defined(_M_ARM) || defined(__arm__) || defined(__CC_ARM)) && defined(__ARM_ARCH_7_A__)
#define ARCH_PREFER_MULT_32x32
#undef SINETABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT
#undef WINDOWTABLE_16BIT

#elif (defined(_M_ARM) || defined(__arm__) || defined(__CC_ARM)) && defined(__ARM_ARCH_5TE__)
#define ARCH_PREFER_MULT_32x32
#undef SINETABLE_16BIT
#undef WINDOWTABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT

#elif (defined(_M_ARM) || defined(__arm__) || defined(__CC_ARM)) && !defined(__ARM_ARCH_5TE__)
#define ARCH_PREFER_MULT_32x32
#undef SINETABLE_16BIT
#undef WINDOWTABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT

#elif defined(__ADSP21000__)
  #define ARCH_PREFER_MULT_32x32
  #define SINETABLE_16BIT
  #define WINDOWTABLE_16BIT
  #undef POW2COEFF_16BIT
  #undef LDCOEFF_16BIT

#elif defined(__XTENSA__)
  #define ARCH_PREFER_MULT_32x16
  #define ARCH_PREFER_MULT_32x32
  #define SINETABLE_32BIT
  #define POW2COEFF_32BIT
  #define LDCOEFF_32BIT
  #define WINDOWTABLE_32BIT

#elif defined(__hgon__)
  #define ARCH_PREFER_MULT_32x16
  #define ARCH_PREFER_MULT_32x32
  #undef SINETABLE_16BIT
  #undef POW2COEFF_16BIT
  #undef LDCOEFF_16BIT
  #undef WINDOWTABLE_16BIT

#elif defined(__x86__)
#define ARCH_PREFER_MULT_32x32
#undef SINETABLE_16BIT
#undef WINDOWTABLE_16BIT
#undef POW2COEFF_16BIT
#undef LDCOEFF_16BIT

#else

  #error  >>>> Please set architecture characterization defines for your platform (CDK_HIGH_QUALITY)! <<<<

#endif /* Architecture switches */

#else /* neither CDK_HIGH_QUALITY or CDK_HIGH_PERFORMANCE */

#error Either set CDK_HIGH_QUALITY or CDK_HIGH_PERFORMANCE, but not both nor none of them.

#endif /* End of quality/complexity tradeoff */

#define CDKTOOLS_PACKED_TABLES

#ifdef CDKTOOLS_PACKED_TABLES

#ifdef SINETABLE_16BIT
  #define FIXP_STB FIXP_SGL     /* STB sinus Tab used in transformation */
  #define FIXP_STP FIXP_SPK
  #define STC(a) (FX_DBL2FXCONST_SGL(a))
#else
  #define FIXP_STB FIXP_DBL
  #define FIXP_STP FIXP_DPK
  #define STC(a) ((FIXP_DBL)(LONG)(a))
#endif /* defined(SINETABLE_16BIT) */

#define STCP(cos,sin) { { STC(cos), STC(sin) } }

#else /* CDKTOOLS_PACKED_TABLES */

#ifdef SINETABLE_16BIT
  #define FIXP_STB FIXP_SGL     /* STB sinus Tab used in transformation */
  #define FIXP_STP FIXP_SGL
  #define STC(a) (FX_DBL2FXCONST_SGL(a))
#else
  #define FIXP_STB FIXP_DBL
  #define FIXP_STP FIXP_DBL
  #define STC(a) ((FIXP_DBL)(LONG)(a))
#endif /* defined(SINETABLE_16BIT) */

#endif /* CDKTOOLS_PACKED_TABLES */

#ifdef WINDOWTABLE_16BIT
  #define FIXP_WTB FIXP_SGL /* single FIXP_SGL values */
  #define FX_DBL2FX_WTB(x) FX_DBL2FX_SGL(x)
  #define FIXP_WTP FIXP_SPK /* packed FIXP_SGL values */
  #define WTC(a) FX_DBL2FXCONST_SGL(a)
#else /* SINETABLE_16BIT */
  #define FIXP_WTB FIXP_DBL
  #define FX_DBL2FX_WTB(x) (x)
  #define FIXP_WTP FIXP_DPK
  #define WTC(a) (FIXP_DBL)(a)
#endif /* SINETABLE_16BIT */

#define WTCP(a,b) { { WTC(a), WTC(b) } }

#endif /* __ARCH_H__ */
