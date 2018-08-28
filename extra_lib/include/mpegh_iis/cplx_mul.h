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

#include "CDK_tools_settings.h"
#include "common_fix.h"

#if !defined(__CPLX_Mult_H__)
#define __CPLX_Mult_H__

#if   defined(__CC_ARM) || defined(__arm__) || defined(_M_ARM)
#include "arm/cplx_mul.h"

#elif defined(__GNUC__) && defined(__mips__)
#include "mips/cplx_mul.h"

#elif defined(__XTENSA__)
#include "xtensa/cplx_mul_xtensa.h"

#elif defined(__hgon__)
#include "hexagon/cplx_mul_hexagon.h"

#endif /* #if defined all cores: bfin, arm, etc. */

/* ############################################################################# */

/* Fallback generic implementations */

#if !defined(FUNCTION_cplxMultDiv2_32x16X2)
#define FUNCTION_cplxMultDiv2_32x16X2

inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_SGL  b_Re,
                          const FIXP_SGL  b_Im)
{
  *c_Re = fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im = fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}
#endif

#if !defined(FUNCTION_cplxMultDiv2_16x16X2)
#define FUNCTION_cplxMultDiv2_16x16X2

inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_SGL  a_Re,
                          const FIXP_SGL  a_Im,
                          const FIXP_SGL  b_Re,
                          const FIXP_SGL  b_Im)
{
  *c_Re = fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im = fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}

inline void cplxMultDiv2( FIXP_SGL *c_Re,
                          FIXP_SGL *c_Im,
                          const FIXP_SGL  a_Re,
                          const FIXP_SGL  a_Im,
                          const FIXP_SGL  b_Re,
                          const FIXP_SGL  b_Im)
{
  *c_Re = FX_DBL2FX_SGL(fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im));
  *c_Im = FX_DBL2FX_SGL(fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re));
}
#endif


#if !defined(FUNCTION_cplxMultDiv2_32x16)
#define FUNCTION_cplxMultDiv2_32x16

inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL a_Re,
                          const FIXP_DBL a_Im,
                          const FIXP_SPK w )
{
  cplxMultDiv2(c_Re, c_Im, a_Re, a_Im, w.v.re, w.v.im);
}
#endif

#if !defined(FUNCTION_cplxMultDiv2_16x16)
#define FUNCTION_cplxMultDiv2_16x16

inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_SGL a_Re,
                          const FIXP_SGL a_Im,
                          const FIXP_SPK w )
{
  cplxMultDiv2(c_Re, c_Im, a_Re, a_Im, w.v.re, w.v.im);
}

inline void cplxMultDiv2( FIXP_SGL *c_Re,
                          FIXP_SGL *c_Im,
                          const FIXP_SGL a_Re,
                          const FIXP_SGL a_Im,
                          const FIXP_SPK w )
{
  cplxMultDiv2(c_Re, c_Im, a_Re, a_Im, w.v.re, w.v.im);
}
#endif

#if !defined(FUNCTION_cplxMultAddDiv2_32x16X2)
#define FUNCTION_cplxMultAddDiv2_32x16X2

inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_SGL  b_Re,
                          const FIXP_SGL  b_Im)
{
  *c_Re += fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im += fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}
#endif

#if !defined(FUNCTION_cplxMultAddDiv2_32x16)
#define FUNCTION_cplxMultAddDiv2_32x16

inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_SPK  w)
{
  *c_Re += fMultDiv2(a_Re,w.v.re) - fMultDiv2(a_Im,w.v.im);
  *c_Im += fMultDiv2(a_Re,w.v.im) + fMultDiv2(a_Im,w.v.re);
}
#endif

#if !defined(FUNCTION_cplxMultSubDiv2_32x16X2)
#define FUNCTION_cplxMultSubDiv2_32x16X2

inline void cplxMultSubDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             const FIXP_DBL  a_Re,
                             const FIXP_DBL  a_Im,
                             const FIXP_SGL  b_Re,
                             const FIXP_SGL  b_Im)
{
  *c_Re -= fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im -= fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}
#endif

#if !defined(FUNCTION_cplxMultDiv2_32x32X2)
#define FUNCTION_cplxMultDiv2_32x32X2

inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_DBL  b_Re,
                          const FIXP_DBL  b_Im)
{
  *c_Re = fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im = fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}
#endif

#if !defined(FUNCTION_cplxMultDiv2_32x32)
#define FUNCTION_cplxMultDiv2_32x32

inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_DPK  w)
{
  cplxMultDiv2(c_Re, c_Im, a_Re, a_Im, w.v.re, w.v.im);
}
#endif

#if !defined(FUNCTION_cplxMultSubDiv2_32x32X2)
#define FUNCTION_cplxMultSubDiv2_32x32X2

inline void cplxMultSubDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             const FIXP_DBL  a_Re,
                             const FIXP_DBL  a_Im,
                             const FIXP_DBL  b_Re,
                             const FIXP_DBL  b_Im)
{
  *c_Re -= fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im -= fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}
#endif

#if !defined(FUNCTION_cplxMultAddDiv2_32x32X2)
#define FUNCTION_cplxMultAddDiv2_32x32X2

inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             const FIXP_DBL  a_Re,
                             const FIXP_DBL  a_Im,
                             const FIXP_DBL  b_Re,
                             const FIXP_DBL  b_Im)
{
  *c_Re += fMultDiv2(a_Re,b_Re) - fMultDiv2(a_Im,b_Im);
  *c_Im += fMultDiv2(a_Re,b_Im) + fMultDiv2(a_Im,b_Re);
}
#endif

/* ############################################################################# */

#if !defined(FUNCTION_cplxMult_32x16X2)
#define FUNCTION_cplxMult_32x16X2

inline void cplxMult( FIXP_DBL *c_Re,
                      FIXP_DBL *c_Im,
                      const FIXP_DBL  a_Re,
                      const FIXP_DBL  a_Im,
                      const FIXP_SGL  b_Re,
                      const FIXP_SGL  b_Im)
{
  *c_Re = fMult(a_Re,b_Re) - fMult(a_Im,b_Im);
  *c_Im = fMult(a_Re,b_Im) + fMult(a_Im,b_Re);
}
inline void cplxMult( FIXP_SGL *c_Re,
                      FIXP_SGL *c_Im,
                      const FIXP_SGL  a_Re,
                      const FIXP_SGL  a_Im,
                      const FIXP_SGL  b_Re,
                      const FIXP_SGL  b_Im)
{
  *c_Re = FX_DBL2FX_SGL(fMult(a_Re,b_Re) - fMult(a_Im,b_Im));
  *c_Im = FX_DBL2FX_SGL(fMult(a_Re,b_Im) + fMult(a_Im,b_Re));
}
#endif

#if !defined(FUNCTION_cplxMult_32x16)
#define FUNCTION_cplxMult_32x16

inline void cplxMult( FIXP_DBL *c_Re,
                      FIXP_DBL *c_Im,
                      const FIXP_DBL a_Re,
                      const FIXP_DBL a_Im,
                      const FIXP_SPK w )
{
  cplxMult(c_Re, c_Im, a_Re, a_Im, w.v.re, w.v.im);
}
#endif

#if !defined(FUNCTION_cplxMult_32x32X2)
#define FUNCTION_cplxMult_32x32X2

inline void cplxMult( FIXP_DBL *c_Re,
                      FIXP_DBL *c_Im,
                      const FIXP_DBL  a_Re,
                      const FIXP_DBL  a_Im,
                      const FIXP_DBL  b_Re,
                      const FIXP_DBL  b_Im)
{
  *c_Re = fMult(a_Re,b_Re) - fMult(a_Im,b_Im);
  *c_Im = fMult(a_Re,b_Im) + fMult(a_Im,b_Re);
}
#endif

#if !defined(FUNCTION_cplxMult_32x32)
#define FUNCTION_cplxMult_32x32
inline void cplxMult( FIXP_DBL *c_Re,
                      FIXP_DBL *c_Im,
                      const FIXP_DBL  a_Re,
                      const FIXP_DBL  a_Im,
                      const FIXP_DPK  w)
{
  cplxMult(c_Re, c_Im, a_Re, a_Im, w.v.re, w.v.im);
}
#endif

#if !defined(FUNCTION_cplxMult_nIm)
#define FUNCTION_cplxMult_nIm

/* Same as cplxMult, but
   a_Im must be negated, when used
   c_re must be negated, when output
*/
inline void cplxMult_nIm( FIXP_DBL *c_Re,
                      FIXP_DBL *c_Im,
                      const FIXP_DBL a_Re,
                      const FIXP_DBL a_Im,
                      const FIXP_SPK w )
{
  *c_Re = -(fMult(a_Re,w.v.re) + fMult(a_Im,w.v.im));
  *c_Im =   fMult(a_Re,w.v.im) - fMult(a_Im,w.v.re);
}

inline void cplxMult_nIm( FIXP_DBL *c_Re,
                      FIXP_DBL *c_Im,
                      const FIXP_DBL a_Re,
                      const FIXP_DBL a_Im,
                      const FIXP_DPK w )
{
  *c_Re = -(fMult(a_Re,w.v.re) + fMult(a_Im,w.v.im));
  *c_Im =   fMult(a_Re,w.v.im) - fMult(a_Im,w.v.re);
}
#endif

/* ############################################################################# */

#endif /* __CPLX_Mult_H__ */

