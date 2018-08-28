/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (1999)
                               All Rights Reserved

   $Id$
   Author(s):   Marc Gayer
   Description: fixed point intrinsics

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#if !defined(__CLZ_H__)
#define __CLZ_H__

#include "CDK_tools_settings.h"
#include "CDK_archdef.h"
#include "machine_type.h"

#if defined(__arm__)
#include "arm/clz_arm.h"

#elif defined(__mips__)
#include "mips/clz_mips.h"

#elif defined(__XTENSA__)
#include "xtensa/clz_xtensa.h"

#elif defined(__hgon__)
#include "hexagon/clz_hexagon.h"

#elif defined(__ADSP21000__)
#include "sharc/clz_sharc.h"

#elif defined(__x86__)
#include "x86/clz_x86.h"

#endif /* all cores */


/*************************************************************************
 *************************************************************************
    Software fallbacks for missing functions.
**************************************************************************
**************************************************************************/

#if !defined(FUNCTION_fixnormz_S)
#ifdef FUNCTION_fixnormz_D
inline INT fixnormz_S (SHORT a)
{
  if ( a < 0 ) {
    return 0;
  }
  return fixnormz_D((INT)(a)) - 16;
}
#else
inline INT fixnormz_S (SHORT a)
{
    int leadingBits = 0;
    a = ~a;
    while(a & 0x8000) {
      leadingBits++;
      a <<= 1;
    }

    return (leadingBits);
}
#endif
#endif

#if !defined(FUNCTION_fixnormz_D)
inline INT fixnormz_D (LONG a)
{
    INT leadingBits = 0;
    a = ~a;
    while(a & 0x80000000) {
      leadingBits++;
      a <<= 1;
    }

    return (leadingBits);
}
#endif


/*****************************************************************************

    functionname: fixnorm_D
    description:  Count leading ones or zeros of operand val for dfract/LONG INT values.
                  Return this value minus 1. Return 0 if operand==0.
*****************************************************************************/
#if !defined(FUNCTION_fixnorm_S)
#ifdef FUNCTION_fixnorm_D
inline INT fixnorm_S(FIXP_SGL val)
{
  if (val == (FIXP_SGL)0) {
    return 0;
  }
  return fixnorm_D((INT)(val)) - 16;
}
#else
inline INT fixnorm_S(FIXP_SGL val)
{
    INT leadingBits = 0;
    if ( val != (FIXP_SGL)0 ) {
        if ( val < (FIXP_SGL)0 ) {
            val = ~val;
        }
        leadingBits = fixnormz_S(val) - 1;
    }
    return (leadingBits);
}
#endif
#endif

#if !defined(FUNCTION_fixnorm_D)
inline INT fixnorm_D(FIXP_DBL val)
{
    INT leadingBits = 0;
    if ( val != (FIXP_DBL)0 ) {
        if ( val < (FIXP_DBL)0 ) {
            val = ~val;
        }
        leadingBits = fixnormz_D(val) - 1;
    }
    return (leadingBits);
}
#endif

#endif /* __CLZ_H__ */
