/***************************  Fraunhofer IIS CDK Tools  ***********************

                        (C) Copyright Fraunhofer IIS (2005)
                               All Rights Reserved

   $Id$
   Author(s):   M. Lohwasser
   Description: fixed point abs definitions

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#if !defined(__ABS_H__)
#define __ABS_H__


#if   defined(__ADSP21000__)
#include "sharc/abs_sharc.h"

#elif defined(__mips__)
#include "mips/abs_mips.h"

#elif defined(__XTENSA__)
#include "xtensa/abs_xtensa.h"

#elif defined(__x86__)
#include "x86/abs_x86.h"

#endif /* all cores */

/*************************************************************************
 *************************************************************************
    Software fallbacks for missing functions
**************************************************************************
**************************************************************************/

#if !defined(FUNCTION_fixabs_D)
inline FIXP_DBL fixabs_D(FIXP_DBL x) { return ((x) > (FIXP_DBL)(0)) ? (x) : -(x) ; }
#endif

#if !defined(FUNCTION_fixabs_I)
inline INT fixabs_I(INT x)           { return ((x) > (INT)(0))      ? (x) : -(x) ; }
#endif

#if !defined(FUNCTION_fixabs_S)
inline FIXP_SGL fixabs_S(FIXP_SGL x) { return ((x) > (FIXP_SGL)(0)) ? (x) : -(x) ; }
#endif

#endif /* __ABS_H__ */
