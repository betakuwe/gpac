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


#if defined(__CC_ARM)

#if defined(__ARM_ARCH_7M__) /* use software fallbacks */
#endif

#if defined(__ARM_ARCH_7EM__)

  #define FUNCTION_cplxMultDiv2_32x16
  #define FUNCTION_cplxMultDiv2_32x16X2
  #define FUNCTION_cplxMultDiv2_32x32X2

  #define FUNCTION_cplxMultAddDiv2_32x16
  #define FUNCTION_cplxMultAddDiv2_32x16X2
  #define FUNCTION_cplxMultAddDiv2_32x32X2


#ifdef FUNCTION_cplxMultDiv2_32x16
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          FIXP_DBL a_Re,
                          FIXP_DBL a_Im,
                          const FIXP_SPK wpk )
{
   *c_Re = fMultAddDiv2(-fMultDiv2(a_Im,(FIXP_SGL)(LONG)(SHORT)(wpk.w)),a_Re,(FIXP_SGL)((LONG)(SHORT)(wpk.w>>16)));
   *c_Im = fMultAddDiv2( fMultDiv2(a_Re,(FIXP_SGL)(LONG)(SHORT)(wpk.w)),a_Im,(FIXP_SGL)((LONG)(SHORT)(wpk.w>>16)));
}

inline void cplxMultDiv2negRe( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL a_Re,
                          const FIXP_DBL a_Im,
                          const FIXP_SPK wpk )
{
   *c_Re = fMultAddDiv2(-fMultDiv2(a_Re,(FIXP_SGL)((LONG)(SHORT)(wpk.w>>16))),    a_Im,(FIXP_SGL)((LONG)(SHORT)(wpk.w)));
   *c_Im = fMultAddDiv2( fMultDiv2(a_Re,(FIXP_SGL)((LONG)(SHORT)(wpk.w))),a_Im,(FIXP_SGL)((LONG)(SHORT)(wpk.w>>16)));
}

#endif  /* FUNCTION_cplxMultDiv2_32x16 */


#ifdef FUNCTION_cplxMultDiv2_32x16X2
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          FIXP_DBL a_Re,
                          FIXP_DBL a_Im,
                          FIXP_SGL b_Re,
                          FIXP_SGL b_Im )
{
   *c_Re = fMultAddDiv2(-fMultDiv2(a_Im,b_Im),a_Re,b_Re);
   *c_Im = fMultAddDiv2( fMultDiv2(a_Re,b_Im),a_Im,b_Re);
}
#endif  /* FUNCTION_cplxMultDiv2_32x16X2 */

#ifdef FUNCTION_cplxMultDiv2_32x32X2
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          FIXP_DBL a_Re,
                          FIXP_DBL a_Im,
                          FIXP_DBL b_Re,
                          FIXP_DBL b_Im )
{
   *c_Re = fMultSubDiv2(fMultDiv2(a_Re,b_Re),a_Im,b_Im);
   *c_Im = fMultAddDiv2(fMultDiv2(a_Re,b_Im),a_Im,b_Re);
}
#endif  /* FUNCTION_cplxMultDiv2_32x32X2 */


#ifdef FUNCTION_cplxMultAddDiv2_32x16
inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             FIXP_DBL a_Re,
                             FIXP_DBL a_Im,
                             const FIXP_SPK wpk )
{
   *c_Re = fMultAddDiv2(*c_Re-fMultDiv2(a_Im,(FIXP_SGL)(LONG)(SHORT)(wpk.w)),a_Re,(FIXP_SGL)((LONG)(SHORT)(wpk.w>>16)));
   *c_Im = fMultAddDiv2(fMultAddDiv2(*c_Im,a_Re,(FIXP_SGL)(LONG)(SHORT)(wpk.w)),a_Im,(FIXP_SGL)((LONG)(SHORT)(wpk.w>>16)));
}
#endif  /* FUNCTION_cplxMultAddDiv2_32x16 */


#ifdef FUNCTION_cplxMultAddDiv2_32x16X2
inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             FIXP_DBL a_Re,
                             FIXP_DBL a_Im,
                             FIXP_SGL b_Re,
                             FIXP_SGL b_Im )
{
   *c_Re = fMultAddDiv2(*c_Re-fMultDiv2(a_Im,b_Im),a_Re,b_Re);
   *c_Im = fMultAddDiv2(fMultAddDiv2(*c_Im,a_Re,b_Im),a_Im,b_Re);
}
#endif  /* FUNCTION_cplxMultAddDiv2_32x16X2 */

#ifdef FUNCTION_cplxMultAddDiv2_32x32X2
inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             FIXP_DBL a_Re,
                             FIXP_DBL a_Im,
                             FIXP_DBL b_Re,
                             FIXP_DBL b_Im )
{
   *c_Re = fMultSubDiv2(fMultAddDiv2(*c_Re,a_Re,b_Re),a_Im,b_Im);
   *c_Im = fMultAddDiv2(fMultAddDiv2(*c_Im,a_Re,b_Im),a_Im,b_Re);
}
#endif  /* FUNCTION_cplxMultAddDiv2_32x32X2 */

#endif /* defined(__ARM_ARCH_7EM__) */



#if !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

/* ARM ADS */

#if defined(__ARM_ARCH_5TE__)
  #define FUNCTION_cplxMultDiv2_32x16
  #define FUNCTION_cplxMultDiv2_32x16X2
  //#define FUNCTION_cplxMult_32x16
  //#define FUNCTION_cplxMult_32x16X2
#endif

#define FUNCTION_cplxMultDiv2_32x32X2
//#define FUNCTION_cplxMult_32x32X2


#ifdef FUNCTION_cplxMultDiv2_32x16
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          FIXP_DBL a_Re,
                          FIXP_DBL a_Im,
                          const FIXP_SPK wpk )
{
   LONG tmp1,tmp2;
   const LONG w = wpk.w;
    
   __asm { smulwt  tmp1, a_Im, w;
           smulwt  tmp2, a_Re, w;
           rsb     tmp1, tmp1, #0;
           smlawb  a_Re, a_Re, w, tmp1;
           smlawb  a_Im, a_Im, w, tmp2; }

    *c_Re = a_Re;
    *c_Im = a_Im;
}

// Note: This macro is identical to the above one, except
//       that the output real part is negated
         
inline void cplxMultDiv2negRe( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL a_Re,
                          const FIXP_DBL a_Im,
                          const FIXP_SPK wpk )
{
   LONG tmp1,tmp2;
   const LONG w = wpk.w;

   __asm { smulwb tmp1, a_Re, w;
           rsb tmp2,tmp1,#0;
           smlawt tmp1, a_Im, w, tmp2
           smulwt tmp2, a_Re, w;
           smlawb tmp2, a_Im, w, tmp2;
         }

    *c_Re = tmp1;
    *c_Im = tmp2;
}
#endif /* FUNCTION_cplxMultDiv2_32x16 */

#ifdef FUNCTION_cplxMultDiv2_32x16X2
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_SGL b_Re,
                          const FIXP_SGL b_Im)
{
    LONG tmp1, tmp2;

    __asm { smulwb tmp1, a_Im, b_Im;
            rsb tmp2,tmp1,#0;
            smlawb tmp1, a_Re, b_Re, tmp2;
            smulwb tmp2, a_Re, b_Im;
            smlawb tmp2, a_Im, b_Re, tmp2; }

     *c_Re = tmp1;
     *c_Im = tmp2;
}
#endif /* FUNCTION_cplxMultDiv2_32x16X2 */

#ifdef FUNCTION_cplxMultDiv2_32x32X2
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL  a_Re,
                          const FIXP_DBL  a_Im,
                          const FIXP_DBL b_Re,
                          const FIXP_DBL b_Im)
{
#if defined(__ARM_ARCH_6__)
    LONG re, im;
    __asm
    {
       smmul re, a_Re, b_Re;
       smmul im, a_Re, b_Im;
       smmls re, a_Im, b_Im, re;
       smmla im, a_Im, b_Re, im;
    }
    *c_Re = re;
    *c_Im = im;

#else
    LONG d_0, d_1,lo;

    __asm
    {
       smull lo, d_0, a_Im, b_Im;
       rsb d_0,d_0,#0;
       smlal lo, d_0, a_Re, b_Re;
       smull lo, d_1, a_Re, b_Im;
       smlal lo, d_1, a_Im, b_Re;
    }

    *c_Re = d_0;
    *c_Im = d_1;
#endif
}
#endif /* FUNCTION_cplxMultDiv2_32x32X2 */

#endif /* !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__) */

/* ############################################################################# */

#elif defined(__arm__) && defined(__GNUC__)

#if defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_8__)
#define FUNCTION_cplxMultDiv2_32x16
#define FUNCTION_cplxMultDiv2_32x16X2
  //#define FUNCTION_cplxMult_32x16
  //#define FUNCTION_cplxMult_32x16X2
#endif

#define FUNCTION_cplxMultDiv2_32x32X2
//#define FUNCTION_cplxMult_32x32X2
#ifdef FUNCTION_cplxMultDiv2_32x32X2
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL a_Re,
                          const FIXP_DBL a_Im,
                          const FIXP_DBL b_Re,
                          const FIXP_DBL b_Im)
{
    LONG tmp1, tmp2;

#ifdef __ARM_ARCH_8__
    asm(
       "smull  %x0, %w2, %w4;       \n" /* tmp1  = a_Re * b_Re */
       "smull  %x1, %w2, %w5;       \n" /* tmp2  = a_Re * b_Im */
       "smsubl %x0, %w3, %w5, %x0;  \n" /* tmp1 -= a_Im * b_Im */
       "smaddl %x1, %w3, %w4, %x1;  \n" /* tmp2 += a_Im * b_Re */
       "asr    %x0, %x0,  #32       \n"
       "asr    %x1, %x1,  #32       \n"
       : "=&r"(tmp1), "=&r"(tmp2)
       : "r"(a_Re), "r"(a_Im), "r"(b_Re), "r"(b_Im)
       );
#elif defined(__ARM_ARCH_6__)
    asm(
       "smmul %0, %2, %4;\n"     /* tmp1  = a_Re * b_Re */
       "smmls %0, %3, %5, %0;\n" /* tmp1 -= a_Im * b_Im */
       "smmul %1, %2, %5;\n"     /* tmp2  = a_Re * b_Im */
       "smmla %1, %3, %4, %1;\n" /* tmp2 += a_Im * b_Re */
       : "=&r"(tmp1), "=&r"(tmp2)
       : "r"(a_Re), "r"(a_Im), "r"(b_Re), "r"(b_Im)
       );
#else
    LONG discard;
    asm(
       "smull %2, %0, %7, %6;\n" /* tmp1  = -a_Im * b_Im */
       "smlal %2, %0, %3, %5;\n" /* tmp1 +=  a_Re * b_Re */
       "smull %2, %1, %3, %6;\n" /* tmp2  =  a_Re * b_Im */
       "smlal %2, %1, %4, %5;\n" /* tmp2 +=  a_Im * b_Re */
       : "=&r"(tmp1), "=&r"(tmp2), "=&r"(discard)
       : "r"(a_Re), "r"(a_Im), "r"(b_Re), "r"(b_Im), "r"(-a_Im)
       );
 #endif
    *c_Re = tmp1;
    *c_Im = tmp2;
}
#endif /* FUNCTION_cplxMultDiv2_32x32X2 */


#ifdef FUNCTION_cplxMultDiv2_32x16
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL a_Re,
                          const FIXP_DBL a_Im,
                          FIXP_SPK wpk )
{
#ifdef __ARM_ARCH_8__
    FIXP_DBL b_Im = FX_SGL2FX_DBL(wpk.v.im);
    FIXP_DBL b_Re = FX_SGL2FX_DBL(wpk.v.re);
    cplxMultDiv2(c_Re, c_Im, a_Re, a_Im, b_Re, b_Im);
#else
   LONG tmp1,tmp2;
   const LONG w = wpk.w;
   asm("smulwt %0, %3, %4;\n"
       "rsb %1,%0,#0;\n"
       "smlawb %0, %2, %4, %1;\n"
       "smulwt %1, %2, %4;\n"
       "smlawb %1, %3, %4, %1;\n"
       : "=&r"(tmp1), "=&r"(tmp2)
       : "r"(a_Re), "r"(a_Im), "r"(w)
       );
    *c_Re = tmp1;
    *c_Im = tmp2;
#endif
}
#endif /* FUNCTION_cplxMultDiv2_32x16 */

#ifdef FUNCTION_cplxMultDiv2_32x16X2
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          const FIXP_DBL a_Re,
                          const FIXP_DBL a_Im,
                          const FIXP_SGL b_Re,
                          const FIXP_SGL b_Im)
{

#ifdef __ARM_ARCH_8__
    FIXP_DBL b_re = FX_SGL2FX_DBL(b_Re);
    FIXP_DBL b_im = FX_SGL2FX_DBL(b_Im);
    cplxMultDiv2(c_Re, c_Im, a_Re, a_Im, b_re, b_im);
#else
    LONG tmp1, tmp2;

    asm("smulwb %0, %3, %5;\n"     /* %7   = -a_Im * b_Im */
        "rsb %1,%0,#0;\n"
        "smlawb %0, %2, %4, %1;\n" /* tmp1 =  a_Re * b_Re - a_Im * b_Im */
        "smulwb %1, %2, %5;\n"     /* %7   =  a_Re * b_Im */
        "smlawb %1, %3, %4, %1;\n" /* tmp2 =  a_Im * b_Re + a_Re * b_Im */
        : "=&r"(tmp1), "=&r"(tmp2)
        : "r"(a_Re), "r"(a_Im), "r"(b_Re), "r"(b_Im)
        );

     *c_Re = tmp1;
     *c_Im = tmp2;
#endif
}
#endif /* FUNCTION_cplxMultDiv2_32x16X2 */

#ifdef FUNCTION_cplxMultAddDiv2_32x16X2
inline void cplxMultAddDiv2( FIXP_DBL *c_Re,
                             FIXP_DBL *c_Im,
                             const FIXP_DBL  a_Re,
                             const FIXP_DBL  a_Im,
                             const FIXP_SGL b_Re,
                             const FIXP_SGL b_Im)
{
    LONG tmp1, tmp2;

    asm("smulwb %0, %3, %5;\n"
        "rsb %1,%0,#0;\n"
        "smlawb %0, %2, %4, %1;\n"
        "smulwb %1, %2, %5;\n"
        "smlawb %1, %3, %4, %1;\n"
        : "=&r"(tmp1), "=&r"(tmp2)
        : "r"(a_Re), "r"(a_Im), "r"(b_Re), "r"(b_Im)
        );

     *c_Re += tmp1;
     *c_Im += tmp2;
}
#endif /* FUNCTION_cplxMultAddDiv2_32x16X2 */



#elif defined(__TMS470__)

#define FUNCTION_cplxMultDiv2_32x16

// workaround for ti arm compiler bug: "INTERNAL ERROR: no match for ICALL"
inline void cplxMultDiv2( FIXP_DBL *c_Re,
                          FIXP_DBL *c_Im,
                          FIXP_DBL a_Re,
                          FIXP_DBL a_Im,
                          LONG w )
{
  FIXP_SGL b_Re = (short)(w>>16);
  FIXP_SGL b_Im = (short)((w<<16)>>16);

  LONG tmp;

  tmp = fMultDiv2(a_Re,b_Re);
  *c_Re = tmp - fMultDiv2(a_Im,b_Im);
  tmp = fMultDiv2(a_Re,b_Im);
  *c_Im = tmp + fMultDiv2(a_Im,b_Re);

}

#endif

