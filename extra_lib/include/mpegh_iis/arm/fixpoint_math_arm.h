/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (1999)
                               All Rights Reserved

   $Id$
   Author(s):   Arthur Tritthart
   Description: Fixed point specific mathematical functions (ARM versions)

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/
#if defined(__ARM_NEON__)
#   include "arm/CDK_neon_funcs.h"
#endif


#if (defined (LDCOEFF_16BIT) && ((LD_PRECISION==5) || (LD_PRECISION==10)))
#define FUNCTION_flog2_func1
#endif

#ifdef FUNCTION_flog2_func1

/* -------------------------------------------------------------------------- */
/* This subroutine replaces the 10x loop in fLog2, a Taylor approximation     */
/* For ARM, loops cost a lot of overhead as well as fMult(). Both issues are  */
/* bypassed in this version, that uses a squared factor, shifted left by 1.   */
/* It works for LD_PRECISION equal to 5 or 10, coefficients are FIXP_SGL.     */
/* If coefficients in FIXP_DBL are required, please adapt accordingly.        */
/* -------------------------------------------------------------------------- */

static inline FIXP_DBL flog2_func1(const FIXP_DBL x2_m)
{
  FIXP_DBL result_m, sqx2_m, px2_m, px3_m;
  FIXP_SGL Coeff2, Coeff3;
  const FIXP_SGL *pldCoeff = &ldCoeff[2];

  result_m  = -(x2_m/2);     /* 1st iteration with factor -0.5*1/1 */
  sqx2_m    = fMultDiv2(x2_m, x2_m)<<2;     /* sqx2_m: x2_m^2<<1 */
  result_m -= sqx2_m/8;    /* 2nd iteration with factor -0.5*1/2 */
  px2_m     = fMultDiv2(x2_m, sqx2_m);      /* px2_m: x2_m^3     */
  px3_m     = fMultDiv2(sqx2_m, sqx2_m)>>1; /* px3_m: x2_m^4     */

  Coeff2 = *pldCoeff++;                     /* factor   -0.5*1/3 */
  Coeff3 = *pldCoeff++;                     /* factor   -0.5*1/4 */
  result_m = fMultAddDiv2(result_m, Coeff2, px2_m);
  px2_m = fMultDiv2(px2_m, sqx2_m);         /* px2_m: x2_m^5     */
  result_m = fMultAddDiv2(result_m, Coeff3, px3_m);
  px3_m = fMultDiv2(px3_m, sqx2_m);         /* px3_m: x2_m^6     */

  Coeff2 = *pldCoeff++;                     /* factor   -0.5*1/5 */
  result_m = fMultAddDiv2(result_m, Coeff2, px2_m);
#if (LD_PRECISION == 10)
  Coeff3 = *pldCoeff++;                     /* factor   -0.5*1/6 */
  px2_m = fMultDiv2(px2_m, sqx2_m);         /* px2_m: x2_m^7     */
  result_m = fMultAddDiv2(result_m, Coeff3, px3_m);
  px3_m = fMultDiv2(px3_m, sqx2_m);         /* px3_m: x2_m^8     */

  Coeff2 = *pldCoeff++;                     /* factor   -0.5*1/7 */
  Coeff3 = *pldCoeff++;                     /* factor   -0.5*1/8 */
  result_m = fMultAddDiv2(result_m, Coeff2, px2_m);
  px2_m = fMultDiv2(px2_m, sqx2_m);         /* px2_m: x2_m^9     */
  result_m = fMultAddDiv2(result_m, Coeff3, px3_m);
  px3_m = fMultDiv2(px3_m, sqx2_m);         /* px3_m: x2_m^10    */

  Coeff2 = *pldCoeff++;                     /* factor   -0.5*1/9 */
  Coeff3 = *pldCoeff++;                     /* factor   -0.5*1/10*/
  result_m = fMultAddDiv2(result_m, Coeff2, px2_m);
  result_m = fMultAddDiv2(result_m, Coeff3, px3_m);
#endif

  return (result_m);
}
#endif



#if defined(__ARM_ARCH_8__) || defined(__ARM_NEON__)
#define FUNCTION_schur_div
#endif


#ifdef FUNCTION_schur_div
#if defined(__ARM_ARCH_8__)
#define USE_ARMV8_64BIT_DIVISION
/* for every instruction, refer to ARM DDI 0487A.g (Reference Manual for ARMv8)
   http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0406c/index.html
   AArch64 SIMD (NEON) arithmetics are IEEE 754-2008 compliant, says above doc */
#ifdef __CC_ARM 
__asm
static CDK_FORCEINLINE FIXP_DBL schur_div(FIXP_DBL num, FIXP_DBL denum, const INT count)
{
#ifdef USE_ARMV8_64BIT_DIVISION
    scvtf d0, w0, #31;   /* C7.3.209 convert fixed-p Q1.31 num to double   */
    scvtf d1, w1, #31;   /* C7.3.209 convert fixed-p Q1.31 denum to double */
    fdiv d0, d0, d1;     /* C7.3.85 double division                        */
    fcvtzs w0, d0, #31;  /* C7.3.79 convert double result to fixed-p Q1.31 */
#else /* USE_ARMV8_64BIT_DIVISION */
    scvtf s0, w0, #31;   /* C7.3.209 convert fixed-p Q1.31 num to float   */
    scvtf s1, w1, #31;   /* C7.3.209 convert fixed-p Q1.31 denum to float */
    fdiv s0, s0, s1;     /* C7.3.85 float division                        */
    fcvtzs w0, s0, #31;  /* C7.3.79 convert float result to fixed-p Q1.31 */
#endif /* USE_ARMV8_64BIT_DIVISION */
    bx lr;
}
#else /* __CC_ARM */
static CDK_FORCEINLINE FIXP_DBL schur_div(FIXP_DBL num, FIXP_DBL denum, const INT count)
{
  FIXP_DBL result;
  __asm__ (
#ifdef USE_ARMV8_64BIT_DIVISION
           "scvtf d0, %w1, #31 \n\t"    /* C7.3.209 convert fixed-p Q1.31 num to double   */
           "scvtf d1, %w2, #31 \n\t"    /* C7.3.209 convert fixed-p Q1.31 denum to double */
           "fdiv d0, d0, d1 \n\t"       /* C7.3.85 double division                        */
           "fcvtzs %w0, d0, #31 \n\t"   /* C7.3.79 convert double result to fixed-p Q1.31 */
#else /* USE_ARMV8_64BIT_DIVISION */
           "scvtf s0, %w1, #31 \n\t"    /* C7.3.209 convert fixed-p Q1.31 num to float   */
           "scvtf s1, %w2, #31 \n\t"    /* C7.3.209 convert fixed-p Q1.31 denum to float */
           "fdiv s0, s0, s1 \n\t"       /* C7.3.85 float division                        */
           "fcvtzs %w0, s0, #31 \n\t"   /* C7.3.79 convert float result to fixed-p Q1.31 */
#endif /* USE_ARMV8_64BIT_DIVISION */
           : "=r" (result)
           :"r" (num), "r" (denum));
  return result;
}
#endif  /* __CC_ARM */
#else /* __ARM_ARCH_8__ */
#ifdef __CC_ARM 
__asm
static CDK_FORCEINLINE FIXP_DBL schur_div(FIXP_DBL num, FIXP_DBL denum, const INT count)
{
    VMOV d0, r0, r1;            // 4.2.3  move num to s0, denum to s1
    VCVT.F32.S32 d0, d0, #31;   // 4.10.6 convert from fixpoint Q1.31 into float
    VDIV.F32 s0, s0, s1;        // 4.10.2 s0 = s0 / s1
    VCVT.S32.F32 s0, s0, #31;   // 4.10.6 convert from float into fixpoint Q1.31
    VMOV r0, s0;
    BX lr;
}
#else /* __CC_ARM */
static CDK_FORCEINLINE FIXP_DBL schur_div(FIXP_DBL num, FIXP_DBL denum, const INT count)
{
  FIXP_DBL result;
  __asm__ (
           "VMOV d0, %1, %2 \n\t"
           "VCVT.F32.S32 d0, d0, #31 \n\t"
           "VDIV.F32 s0, s0, s1 \n\t"
           "VCVT.S32.F32 s0, s0, #31 \n\t"
           "VMOV %0, s0 \n\t"
           : "=r" (result)
           :"r" (num), "r" (denum));
  return result;
}
#endif  /* __CC_ARM */
#endif  /* __ARM_ARCH_8__ */
#endif  /* defined FUNCTION_schur_div */



#if defined(AACENC_SUPPORT_PNS_TOOL)
#if defined(CCARM_SUPPORT_ARM_INSTR) && (FIXP_SPC == FIXP_DBL)
  #define FUNCTION_schur_div_8_square
#endif

#ifdef FUNCTION_schur_div_8_square
/* -------------------------------------------------------------------------- */
/* This subroutine replaces a piece of code in chaosmeasure.cpp:              */
/*      tmp = schur_div(tmp<<leadingBits, center<<leadingBits, 8);            */
/*      tmp = fMult(tmp,tmp);                                                 */
/* It performs a division with 8 bit precision and squares the result.        */
/* -------------------------------------------------------------------------- */


static inline FIXP_DBL schur_div_8_square(FIXP_DBL num, FIXP_DBL denum)
{
  INT div       = 0;                       /* bit #7 is always 0 */
  ULONG L_num   = (ULONG) num << 1;
  ULONG L_denum = (ULONG) denum;
  __asm
  {
    CMP      L_num, L_denum              /* generate C carry flag, if (unsigned) L_num greater/equal L_denum */
    SUBHS    L_num, L_num, L_denum
    ADC      div, div, div               /* bit #6: div = div+div+carry = div<<1 + carry */

    MOV      L_num, L_num, LSL #1
    CMP      L_num, L_denum
    SUBHS    L_num, L_num, L_denum
    ADC      div, div, div               /* bit #5 */

    MOV      L_num, L_num, LSL #1
    CMP      L_num, L_denum
    SUBHS    L_num, L_num, L_denum
    ADC      div, div, div               /* bit #4 */

    MOV      L_num, L_num, LSL #1
    CMP      L_num, L_denum
    SUBHS    L_num, L_num, L_denum
    ADC      div, div, div               /* bit #3 */

    MOV      L_num, L_num, LSL #1
    CMP      L_num, L_denum
    SUBHS    L_num, L_num, L_denum
    ADC      div, div, div               /* bit #2 */

    MOV      L_num, L_num, LSL #1
    CMP      L_num, L_denum
    SUBHS    L_num, L_num, L_denum
    ADC      div, div, div               /* bit #1 */

    MOV      L_num, L_num, LSL #1
    CMP      L_num, L_denum
    ADC      div, div, div               /* bit #0 */
  }
  FIXP_DBL result = fPow2Div2((FIXP_SGL) div) << (DFRACT_BITS-2*8+1);
  return result;
}
#endif /* #ifdef FUNCTION_schur_div_8_square */
#endif /* #if defined(AACENC_SUPPORT_PNS_TOOL) */



#if defined(__ARM_ARCH_8__) || defined(__ARM_NEON__)
#define FUNCTION_sqrtFixp
#endif


/* Square root function for fixed-point data in range 0x0000.0000 to 0x7FFF.FFFF */
/* For other input values, the result is invalid.                                */
#ifdef FUNCTION_sqrtFixp
#ifdef __ARM_ARCH_8__
#define USE_ARMV8_64BIT_SQUAREROOT
/* for every instruction, refer to ARM DDI 0487A.g (Reference Manual for ARMv8)
   http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0406c/index.html 
   AArch64 SIMD (NEON) arithmetics are IEEE 754-2008 compliant, says above doc */
#ifdef __CC_ARM
__asm
static CDK_FORCEINLINE FIXP_DBL sqrtFixp(FIXP_DBL op)
{ 
#ifdef USE_ARMV8_64BIT_SQUAREROOT
    scvtf d0, w0, #31;    /* C7.3.209 convert fixed-p Q1.31 op to double */
    fsqrt d0, d0;         /* C7.3.147 double square root */
    fcvtzs w0, d0, #31;   /* C7.3.79 convert double result to fixed-p Q1.31 */
#else /* USE_ARMV8_64BIT_SQUAREROOT */
    scvtf s0, w0, #31;    /* C7.3.209 convert fixed-p Q1.31 op to float */
    fsqrt s0, s0;         /* C7.3.147 float square root */
    fcvtzs w0, s0, #31;   /* C7.3.79 convert float result to fixed-p Q1.31 */
#endif /* USE_ARMV8_64BIT_SQUAREROOT */
    BX lr;
}
#else /* __CC_ARM */
static CDK_FORCEINLINE FIXP_DBL sqrtFixp(FIXP_DBL op)
{
  FIXP_DBL result;
  __asm__ (
#ifdef USE_ARMV8_64BIT_SQUAREROOT
           "scvtf d0, %w1, #31 \n\t"    /* C7.3.209 convert fixed-p Q1.31 op to double */
           "fsqrt d0, d0 \n\t"          /* C7.3.147 double square root */
           "fcvtzs %w0, d0, #31 \n\t"   /* C7.3.79 convert double result to fixed-p Q1.31 */
#else /* USE_ARMV8_64BIT_SQUAREROOT */
           "scvtf s0, %w1, #31 \n\t"    /* C7.3.209 convert fixed-p Q1.31 op to float */
           "fsqrt s0, s0 \n\t"          /* C7.3.147 float square root */
           "fcvtzs %w0, s0, #31 \n\t"   /* C7.3.79 convert float result to fixed-p Q1.31 */
#endif /* USE_ARMV8_64BIT_SQUAREROOT */
           : "=r" (result)
           :"r" (op));
  return result;
}
#endif /* __CC_ARM */
#else /* __ARM_ARCH_8__ */
#ifdef __CC_ARM
__asm
static CDK_FORCEINLINE FIXP_DBL sqrtFixp(FIXP_DBL op)
{
  VMOV s0, r0;
  VCVT.F32.S32 s0, s0, #31; 
  VSQRT.F32 s0, s0;
  VCVT.S32.F32 s0, s0, #31; 
  VMOV r0, s0;
  BX lr;
}
#else /* __CC_ARM */
static CDK_FORCEINLINE FIXP_DBL sqrtFixp(FIXP_DBL op)
{
  FIXP_DBL result;
  __asm__ (
  "VMOV s0, %1 \n\t"
  "VCVT.F32.S32 s0, s0, #31 \n\t"
  "VSQRT.F32 s0, s0 \n\t"
  "VCVT.S32.F32 s0, s0, #31 \n\t"
  "VMOV %0, s0 \n\t"
    : "=r" (result)
    :"r" (op));
  return result;
}
#endif /* __CC_ARM */
#endif /* __ARM_ARCH_8__ */
#endif /* FUNCTION_sqrtFixp */



#if defined(FUNCTION_schur_div)
#define FUNCTION_invFixp
#endif

#ifdef FUNCTION_invFixp
/**
 * \brief calculate 1.0/op
 * \param op mantissa of the input value, represented as INT (not a FIX_DBL like in the header!).
 * \return mantissa of the result (FIXP_DBL) with implicit exponent of 31
 * \exceptions are provided for op=0,1 setting max. positive value
 */
#define USE_ARMV8_64BIT_INVDIV
#if defined(__ARM_ARCH_8__)
/* for every instruction, refer to ARM DDI 0487A.g (Reference Manual for ARMv8)
   http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0406c/index.html 
   AArch64 SIMD (NEON) arithmetics are IEEE 754-2008 compliant, says above doc */
#ifdef __CC_ARM
__asm
static CDK_FORCEINLINE FIXP_DBL invFixp(FIXP_DBL op)
{
#ifdef USE_ARMV8_64BIT_INVDIV
    fmov d0, #1.00000000;  /* C7.3.115 move f-point value to scalar reg      */
    scvtf d1, w0;          /* C7.3.210 convert integer op to double          */
    fdiv d0, d0, d1;       /* C7.3.85 double division                        */
    fcvtzs w0, d0, #31;    /* C7.3.79 convert double result to fixed-p Q1.31 */
#else
    fmov s0, #1.00000000;  /* C7.3.115 move f-point value to scalar reg     */
    scvtf s1, w0;          /* C7.3.210 convert integer op to float          */
    fdiv s0, s0, s1;       /* C7.3.85 float division                        */
    fcvtzs w0, s0, #31;    /* C7.3.79 convert float result to fixed-p Q1.31 */
#endif /* USE_ARMV8_64BIT_INVDIV */
    bx lr;
}
#else /* __CC_ARM */
static CDK_FORCEINLINE FIXP_DBL invFixp(FIXP_DBL op)
{
  FIXP_DBL result;
  __asm__ (
#ifdef USE_ARMV8_64BIT_INVDIV
           "fmov d0, #1.00000000 \n\t"  /* C7.3.115 move f-point value to scalar reg      */
           "scvtf d1, %w1 \n\t"         /* C7.3.210 convert integer op to double          */
           "fdiv d0, d0, d1 \n\t"       /* C7.3.85 double division                        */
           "fcvtzs %w0, d0, #31 \n\t"   /* C7.3.79 convert double result to fixed-p Q1.31 */
#else
           "fmov s0, #1.00000000 \n\t"  /* C7.3.115 move f-point value to scalar reg     */
           "scvtf s1, %w1 \n\t"         /* C7.3.210 convert integer op to float          */
           "fdiv s0, s0, s1 \n\t"       /* C7.3.85 float division                        */
           "fcvtzs %w0, s0, #31 \n\t"   /* C7.3.79 convert float result to fixed-p Q1.31 */
#endif /* USE_ARMV8_64BIT_INVDIV */
           : "=r" (result)
           :"r" (op));
  return result;
}
#endif /* __CC_ARM */
#else /* __ARM_ARCH_8__ */
/* for every instruction, refer to ARM DDI 0406C.b (Reference Manual for ARMv7)
   http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0406c/index.html */
#ifdef __CC_ARM
__asm
static CDK_FORCEINLINE FIXP_DBL invFixp(FIXP_DBL op)
{
   vmov.f32 s0, #1.00000000;  /* A8.8.102 move float 1.0 to VPF reg       */
   vmov s1, r0;               /* A8.8.343 move op to VPF reg              */
   vcvt.f32.s32 s1, s1;       /* A8.8.306 convert integer op to float     */
   vdiv.f32 s0, s0, s1;       /* A8.8.312 float division                  */
   vcvt.s32.f32 s0, s0, #31;  /* A8.8.308 convert float result to fixed-p */
   vmov r0, s0;               /* A8.8.343 move result to output reg       */
   bx lr;        
}
#else /* __CC_ARM */
static CDK_FORCEINLINE FIXP_DBL invFixp(FIXP_DBL op)
{
  FIXP_DBL result;
  __asm__ (
           "vmov.f32 s0, #1.00000000 \n\t"  /* A8.8.102 move float 1.0 to VPF reg       */
           "vmov s1, %1 \n\t"               /* A8.8.343 move op to VPF reg              */
           "vcvt.f32.s32 s1, s1 \n\t"       /* A8.8.306 convert integer op to float     */
           "vdiv.f32 s0, s0, s1 \n\t"       /* A8.8.312 float division                  */
           "vcvt.s32.f32 s0, s0, #31 \n\t"  /* A8.8.308 convert float result to fixed-p */
           "vmov %0, s0 \n\t"               /* A8.8.343 move result to output reg       */
           : "=r" (result)
           :"r" (op));
  return result;
}
#endif /* __CC_ARM */
#endif /* __ARM_ARCH_8__ */

/**
 * \brief calculate 1.0/(op_m * 2^op_e)
 * \param op_m mantissa of the input value, represented as FIXP_DBL.
 * \param op_e pointer to the exponent of the input value, and where the result will be stored in.
 * \return mantissa of the result
 */
static CDK_FORCEINLINE FIXP_DBL invFixp(FIXP_DBL op_m, int *op_e)
{
    FIXP_DBL result;
    if (op_m <= (FIXP_DBL) 0x00000000)
    {
        *op_e = 31 - *op_e;
        return((LONG)MAXVAL_DBL);
    }
    INT shl = CountLeadingBits(op_m);
    op_m <<= shl;
    {
        *op_e = shl + 1 - *op_e;
        result = schur_div(0x40000000, op_m, 16);
    }
    return result;
}
#endif /* FUNCTION_invFixp */



#if (defined(__ARM_ARCH_8__) || defined(__ARM_NEON__)) && !defined(__CC_ARM)
#define FUNCTION_invSqrtNorm2
#endif

#ifdef FUNCTION_invSqrtNorm2
/**
 * \brief calculate 1.0/sqrt(op)
 * \param op_m mantissa of input value.
 * \param result_e pointer to return the exponent of the result
 * \return mantissa of the result
 */
/*****************************************************************************
  delivers 1/sqrt(op) normalized to .5...1 and the shift value of the OUTPUT,
  i.e. the denormalized result is 1/sqrt(op) = invSqrtNorm(op) * 2^(shift)
*****************************************************************************/
//#define USE_ARMV8_64BIT_INVSQRT
static CDK_FORCEINLINE FIXP_DBL invSqrtNorm2(FIXP_DBL op, INT *shift)
{
    FIXP_DBL result;
    INT exponent;

    if (op == FL2FXCONST_DBL(0.0)) {
        *shift = 16;
        return((LONG)MAXVAL_DBL);  /* maximum positive value */
    }
    
    exponent = fNormz(op) - 1;
    op <<= (exponent & 0xfffffffe);
    *shift = 1 + (exponent>>1);

#ifdef __ARM_ARCH_8__
#ifdef USE_ARMV8_64BIT_INVSQRT
    __asm__ (
             "fmov d1, %x2 \n\t"
             "scvtf d0, %w1, #31 \n\t"
             "fsqrt d0, d0 \n\t"          /* C7.3.147 double square root */
             "fdiv d0, d1, d0 \n\t"
             "fcvtzs %w0, d0, #31 \n\t"   /* C7.3.79 convert double result to fixed-p Q1.31 */
             : "=r" (result)
             :"r" (op), "r" (0.5));
#else /* USE_ARMV8_64BIT_INVSQRT */
    __asm__ (
             "fmov s1, %w2 \n\t"
             "scvtf s0, %w1, #31 \n\t"
             "fsqrt s0, s0 \n\t"          /* C7.3.147 float square root */
             "fdiv s0, s1, s0 \n\t"
             "fcvtzs %w0, s0, #31 \n\t"   /* C7.3.79 convert float result to fixed-p Q1.31 */
             : "=r" (result)
             :"r" (op), "r" (0.5f));
#endif /* USE_ARMV8_64BIT_INVSQRT */
#else /* __ARM_ARCH_8__ */
  __asm__ (
           "vmov s1, %1 \n\t"               /* A8 move op to VPF reg                    */
           "vcvt.f32.s32 s1, s1, #31 \n\t"  /* A8.8.308 convert fixed-p op to float     */
           "vsqrt.f32 s1, s1 \n\t"          /* A8.8.401 float square root               */
           "vmov.f32 s0, #0.50 \n\t"            /* A8.8.343 move float 0.5 to VPF reg       */
           "vdiv.f32 s0, s0, s1 \n\t"       /* A8.8.312 float division                  */
           "vcvt.s32.f32 s0, s0, #31 \n\t"  /* A8.8.308 convert float result to fixed-p */
           "vmov %0, s0 \n\t"               /* A8.8.343 move result to output reg       */
           : "=r" (result)
           :"r" (op));
#endif /* __ARM_ARCH_8__ */

    return result;
}
#endif /* FUNCTION_invSqrtNorm2 */



#if defined(__ARM_ARCH_5TE__)
#define FUNCTION_CDK_preload_L2_cache
#endif

#ifdef FUNCTION_CDK_preload_L2_cache
// This routine is used to preload data into L2 cache in blocks of 2 cache lines (64 bytes) per iteration
// Most profit is reachable on Cortex-A9 platforms
// Description:
// - preload of buffer[0..63] is ordered
// - nbytes is decremented by 64, buffer address is incremented by 64
// - loop breaks, if nbytes goes zero or negative
// Note: Any preload order beyond the buffer's boundary does not harm

#if defined (__CC_ARM)
__asm
static inline void CDK_preload_L2_cache(const SHORT *buffer, INT nbytes)
{
CDK_preload_L2_cache_loop
  PLD [r0, #0];
  PLD [r0, #32];
  SUBS r1, r1, #64;
  ADD  r0, r0, #64;
  BGT CDK_preload_L2_cache_loop;

  BX lr
}
__asm
static inline void CDK_preload_L2_cache(const SHORT *buf1,const SHORT *buf2,INT nbytes)
{
CDK_preload_L2_cache_loop_p2
  PLD [r0, #0];
  PLD [r0, #32];
  PLD [r1, #0];
  PLD [r1, #32];
  SUBS r2, r2, #64;
  ADD  r0, r0, #64;
  ADD  r1, r1, #64;
  BGT CDK_preload_L2_cache_loop_p2;

  BX lr
}
#else
/* GCC, clang version */
static inline void CDK_preload_L2_cache(const SHORT *buffer, INT nbytes)
{
  __asm__ (
"CDK_preload_L2_cache_loop%=:"
    "PLD [%0, #0] \n\t"
    "PLD [%0, #32] \n\t"
    "SUBS %1, %1, #64 \n\t"
    "ADD  %0, %0, #64 \n\t"
    "BGT CDK_preload_L2_cache_loop%= \n\t"
    : "+r" (buffer), "+r" (nbytes) );
  return;
}
static inline void CDK_preload_L2_cache(const SHORT *buf1, const SHORT *buf2, INT nbytes)
{
    __asm__ (
"CDK_preload_L2_cache_loop_p2%=:"
    "PLD [%0, #0] \n\t"
    "PLD [%0, #32] \n\t"
    "PLD [%1, #0] \n\t"
    "PLD [%1, #32] \n\t"
    "SUBS %2, %2, #64 \n\t"
    "ADD  %0, %0, #64 \n\t"
    "ADD  %1, %1, #64 \n\t"
    "BGT CDK_preload_L2_cache_loop_p2%= \n\t"
    : "+r" (buf1), "+r" (buf2), "+r" (nbytes) );
  return;
}
#endif // defined (__CC_ARM)
#endif /* FUNCTION_CDK_preload_L2_cache */

