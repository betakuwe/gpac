/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2002)
                               All Rights Reserved

   $Id$
   Author(s): Tobias Chalupka

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   CDK_timerdefs.h
    \brief  Timer/PLL definitions for timer initialization and usage
 */
 
#include "syslib_settings.h"

#if !defined(__CDK_TIMERDEFS_H__)
#define __CDK_TIMERDEFS_H__

/*!
 * \def    CDK_CLOCKS_PER_SEC
 *         Architecture-specific timer frequency. If not defined, it is set to CLOCKS_PER_SEC.
 *
 * \def    CDK_PLL_MULTIPLIER
 *         PLL multiplier.
 *
 * \def    CDK_PLL_DIVIDER
 *         PLL divider.
 *
 * \def    CDK_CLKIN_MHZ
 *         DSP input frequency. Used to determine the effective CPU frequency.
 *
 * \def    CDK_CPU_MHZ
 *         Effective CPU frequency.
 */

#if   defined(__XTENSA__)
  /* clock() seems to give back the same values as xthal_get_ccount(),
     and the default CLOCKS_PER_SEC does not fit this. */
  #define CDK_CPU_MHZ        100
  #define CDK_CLOCKS_PER_SEC 100000000

#elif defined(__CC_ARM) && !defined(__TARGET_ARCH_7_R)
  #if   defined(__TARGET_ARCH_7_A)
    /* ISSM */
    #define ARM_USE_CCNT
  #endif

  #ifdef ARM_USE_CCNT
    /* CCNT register based clock */
    #define CDK_CPU_MHZ 100
    #define CDK_CLOCKS_PER_SEC 100000000
  #elif defined(ARM_USE_TIMER)
    #define CDK_CLOCKS_PER_SEC (2000000<<(16-8))
  #else
    /* Use SYS_24MHZ register */
    #ifdef __TARGET_ARCH_7_A
      #define CDK_CPU_MHZ 750
    #else
      #define CDK_CPU_MHZ 210
    #endif
    #define CDK_CLOCKS_PER_SEC 24000000
  #endif /* ARM_USE_CCNT */

#elif defined(__ADSP21469__)
  /* SHARC */
  #define CDK_CPU_MHZ              450
  #define CDK_CLOCKS_PER_SEC 450000000

#elif defined(__ADSP21369__)
  /* SHARC */
  #define CDK_CPU_MHZ              400
  #define CDK_CLOCKS_PER_SEC 400000000
  
#elif defined(__ADSP2158X__)
  /* SHARC */
  #define CDK_CPU_MHZ (CDK_CLOCKS_PER_SEC/1000000)
  
#elif defined(__SDE_MIPS__)
  #define CDK_CLOCKS_PER_SEC 50000000
  #define CDK_CPU_MHZ 100

#elif defined(__linux__)
  /* Nano seconds using clock_gettime() */
  #define CDK_CLOCKS_PER_SEC (1000000000)
  /* Max. frequency of Nvidia Shield Tablet */
  /* #define CDK_CPU_MHZ 2218               */

#endif /* architecture specific clock selector */

#if !defined(CDK_CLOCKS_PER_SEC)
  #include <time.h>
  #if defined(CLOCKS_PER_SEC)
    #define CDK_CLOCKS_PER_SEC CLOCKS_PER_SEC
  #else
    #define CDK_CLOCKS_PER_SEC 1000
  #endif
#endif

#endif /* __CDK_TIMERDEFS_H__ */
