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

  /* ############################################################################# */
  #if   defined(__CC_ARM)
  /* ############################################################################# */

    #if defined(__ARM_ARCH_7M__)   /* Cortex-M3 uses pure C-code, only SMULL, MUL available */

    #endif /* defined(__ARM_ARCH_7M__) */

    #if defined(__ARM_ARCH_7EM__)  /* Cortex-M4 uses pure C-code, allowes DSP extension codes */

      #define FUNCTION_fixmadddiv2_DD
      #define FUNCTION_fixmadddiv2_DS
      #define FUNCTION_fixmadddiv2_SS

      #define FUNCTION_fixmadd_DD
      #define FUNCTION_fixmadd_DS
      #define FUNCTION_fixmadd_SS

      #define FUNCTION_fixmsubdiv2_DD
      #define FUNCTION_fixmsubdiv2_DS
      #define FUNCTION_fixmsubdiv2_SS

      #define FUNCTION_fixmsub_DD
      #define FUNCTION_fixmsub_DS
      #define FUNCTION_fixmsub_SS

      #define FUNCTION_fixpow2adddiv2_D
      #define FUNCTION_fixpow2add_D
      #define FUNCTION_fixpow2adddiv2_S
      #define FUNCTION_fixpow2add_S

      #if defined(FUNCTION_fixmadddiv2_DD)
        inline LONG fixmadddiv2_DD (const INT x, const LONG a, const INT b)
        {
          return ((((long long)x << 32) + (long long)a * b) >> 32);  // SMMLA
        }
      #endif

      #if defined(FUNCTION_fixmadddiv2_DS)
        inline INT fixmadddiv2_DS (const INT x, const INT a, const SHORT b)
        {
          return x + fixmuldiv2_DS(a,b);                           // SMLAWB
        }
      #endif

      #if defined(FUNCTION_fixmadddiv2_SS)
        inline INT fixmadddiv2_SS (const INT x, const SHORT a, const SHORT b)
        {
          return  x + fixmuldiv2_SS(a,b);                          // MLA or SMLABB
        }
      #endif

      #if defined(FUNCTION_fixmadd_DD)
        inline INT fixmadd_DD (const INT x, const INT a, const INT b)
        {
          return fixmadddiv2_DD(x,a,b)<<1;                         // SMMLA, LSL #1
        }
      #endif

      #if defined(FUNCTION_fixmadd_DS)
        inline INT fixmadd_DS (const INT x, const INT a, const SHORT b)
        {
          return fixmadddiv2_DS(x,a,b)<<1;                         // SMLAWB, LSL #1
        }
      #endif

      #if defined(FUNCTION_fixmadd_SS)
        inline INT fixmadd_SS (const INT x, const SHORT a, const SHORT b)
        {
          return  fixmadddiv2_SS(x,a,b)<<1;                        // MLA or SMLABB, LSL #1
        }
      #endif

      #if defined(FUNCTION_fixmsubdiv2_DD)
        inline INT fixmsubdiv2_DD (const INT x, const INT a, const INT b)
        {
          return ((((long long)x << 32) - (long long)a * b) >> 32);  // SMMLA
        }
      #endif

      #if defined(FUNCTION_fixmsubdiv2_DS)
        inline INT fixmsubdiv2_DS (const INT x, const INT a, const SHORT b)
        {
          return x - fixmuldiv2_DS(a,b);                           // SMULWB, SUB
        }
      #endif

      #if defined(FUNCTION_fixmsubdiv2_SS)
        inline INT fixmsubdiv2_SS (const INT x, const SHORT a, const SHORT b)
        {
          return  x - fixmuldiv2_SS(a,b);                          // MLS
        }
      #endif

      #if defined(FUNCTION_fixmsub_DD)
        inline INT fixmsub_DD (const INT x, const INT a, const INT b)
        {
          return fixmsubdiv2_DD(x,a,b)<<1;                         // SMMLS, LSL #1
        }
      #endif

      #if defined(FUNCTION_fixmsub_DS)
        inline INT fixmsub_DS (const INT x, const INT a, const SHORT b)
        {
          return fixmsubdiv2_DS(x,a,b)<<1;                         // SMULWB, SUB, LSL #1
        }
      #endif

      #if defined(FUNCTION_fixmsub_SS)
        inline INT fixmsub_SS (const INT x, const SHORT a, const SHORT b)
        {
          return  fixmsubdiv2_SS(x,a,b)<<1;                        // MLS, LSL #1
        }
      #endif

      #if defined(FUNCTION_fixpow2adddiv2_D)
        inline INT fixpadddiv2_D (const INT x, const INT a)
        {
          return fixmadddiv2_DD(x,a,a);                            // SMMLA
        }
      #endif

      #if defined(FUNCTION_fixpow2add_D)
        inline INT fixpadd_D (const INT x, const INT a)
        {
          return x + (fixmuldiv2_DD(a,a)<<1);                      // SMULL, ADD x, p LSL #1
        }
      #endif

      #if defined(FUNCTION_fixpow2adddiv2_S)
        inline INT fixpadddiv2_S (const INT x, const SHORT a)
        {
          return fixmadddiv2_SS(x,a,a);                            // MLA
        }
      #endif

      #if defined(FUNCTION_fixpow2add_S)
        inline INT fixpadd_S (const INT x, const SHORT a)
        {
          return x + (fixmuldiv2_SS(a,a)<<1);                      // MUL, ADD x, p LSL #1
        }
      #endif

    #endif /* defined(__ARM_ARCH_7EM__) */

    #if !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

      #define FUNCTION_fixmadddiv2_SS_Dual
      #ifdef  FUNCTION_fixmadddiv2_SS_Dual
      #if defined(__ARM_ARCH_6__)
        inline INT fixmadddiv2_SS_Dual (INT x, const INT a, const INT b) {
          INT result;
          __asm {smlad result, a, b, x;}
          return result ;
        }
      #elif  defined(__ARM_ARCH_5TE__)
        inline INT fixmadddiv2_SS_Dual (const INT x, const INT a, const INT b) {
          INT result;
          __asm {smlabb result, a, b, x;
                 smlatt result, a, b, result; }
          return result ;
        }
      #else
        inline INT fixmadddiv2_SS_Dual (const INT x, const INT a, const INT b) {
          INT result;
          result = fixmadddiv2_SS(x, (SHORT) a, (SHORT) b) + 
          result = fixmadddiv2_SS(result, (SHORT) (a>>16), (SHORT) (b>>16));
          return result ;
        }
      #endif
      #endif /* FUNCTION_fixmadddiv2_SS_Dual */

      #define FUNCTION_fixmadddiv2_DD

      #ifdef FUNCTION_fixmadddiv2_DD 
      #if defined(__ARM_ARCH_6__)
        inline FIXP_DBL fixmadddiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
          INT result;
          __asm {smmla result, a, b, x;}
          return result ;
        }
        #define FUNCTION_fixmsubdiv2_DD
        inline FIXP_DBL fixmsubdiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
          INT result;
          __asm {smmls result, a, b, x;}
          return result ;
        }
      #else
        inline FIXP_DBL fixmadddiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b)
        {
          INT discard=0, result = x ;
          __asm {smlal discard, result, a, b}
          return result ;
        }
      #endif /* __ARM_ARCH_6__ */
      #endif /* #ifdef FUNCTION_fixmadddiv2_DD */

    #endif /* !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__) */

    #if defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_6__)

      #define FUNCTION_fixmadddiv2_DS

      #ifdef FUNCTION_fixmadddiv2_DS 
      inline FIXP_DBL fixmadddiv2_DS (FIXP_DBL x, const FIXP_DBL a, const FIXP_SGL b)
      {
        INT result ;
        __asm {smlawb result, a, b, x}
        return result ;
      }
      #endif /* #ifdef FUNCTION_fixmadddiv2_DS */

    #endif /* defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_6__) */

    #define FUNCTION_fixmadddiv2BitExact_DD
    #ifdef FUNCTION_fixmadddiv2BitExact_DD
    #define fixmadddiv2BitExact_DD(a, b, c) fixmadddiv2_DD(a, b, c)
    #endif /* #ifdef FUNCTION_fixmadddiv2BitExact_DD */

    #define FUNCTION_fixmsubdiv2BitExact_DD
    #ifdef FUNCTION_fixmsubdiv2BitExact_DD
    inline FIXP_DBL fixmsubdiv2BitExact_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
      return x - fixmuldiv2BitExact_DD(a, b);
    }
    #endif /* #ifdef FUNCTION_fixmsubdiv2BitExact_DD */

    #define FUNCTION_fixmadddiv2BitExact_DS
    #ifdef FUNCTION_fixmadddiv2BitExact_DS
    #define fixmadddiv2BitExact_DS(a, b, c) fixmadddiv2_DS(a, b, c)
    #endif /* #ifdef FUNCTION_fixmadddiv2BitExact_DS */

    #define FUNCTION_fixmsubdiv2BitExact_DS
    #ifdef FUNCTION_fixmsubdiv2BitExact_DS
    inline FIXP_DBL fixmsubdiv2BitExact_DS (FIXP_DBL x, const FIXP_DBL a, const FIXP_SGL b) {
      return x - fixmuldiv2BitExact_DS(a, b);
    }
    #endif /* #ifdef FUNCTION_fixmsubdiv2BitExact_DS */

  /* ############################################################################# */
  #elif defined(__GNUC__) && defined(__arm__)
  /* ############################################################################# */
    /* ARM GNU GCC */

    #ifdef __ARM_ARCH_8__
      #define FUNCTION_fixmadddiv2_DD
      #ifdef FUNCTION_fixmadddiv2_DD
      inline FIXP_DBL fixmadddiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
        INT64 result;
        asm  ("smull  %x0,  %w1,  %w2;    \n"
              "asr    %x0,  %x0,  #32;    \n"
              "add    %w0, %w3,  %w0;    \n"
              : "=&r" (result)
              : "r" (a), "r" (b), "r"(x) );
        return (INT) result ;
      }
      #endif /* #ifdef FUNCTION_fixmadddiv2_DD */

      #define FUNCTION_fixmsubdiv2_DD
      #ifdef FUNCTION_fixmsubdiv2_DD
      inline FIXP_DBL fixmsubdiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
        INT64 result;
        asm  ("smull  %x0,  %w1, %w2;     \n"
              "asr    %x0,  %x0,  #32;     \n"
              "sub    %w0, %w3, %w0;     \n"
              : "=&r" (result)
              : "r" (a), "r" (b), "r"(x) );
        return (INT) result ;
      }
      #endif /* #ifdef FUNCTION_fixmsubdiv2_DD */

    #elif defined(__ARM_ARCH_6__)
      #define FUNCTION_fixmadddiv2_DD
      #ifdef FUNCTION_fixmadddiv2_DD
      inline FIXP_DBL fixmadddiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
        INT result;
        asm  ("smmla %0, %1, %2, %3;\n"
              : "=r" (result)
              : "r" (a), "r" (b), "r"(x) );
        return result ;
      }
      #endif /* #ifdef FUNCTION_fixmadddiv2_DD */

      #define FUNCTION_fixmsubdiv2_DD
      #ifdef FUNCTION_fixmsubdiv2_DD
      inline FIXP_DBL fixmsubdiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
        INT result;
        asm  ("smmls %0, %1, %2, %3;\n"
              : "=r" (result)
              : "r" (a), "r" (b), "r"(x) );
        return result ;
      }
      #endif /* #ifdef FUNCTION_fixmsubdiv2_DD */

    #else /* __ARM_ARCH_6__ */
      #define FUNCTION_fixmadddiv2_DD
      #ifdef FUNCTION_fixmadddiv2_DD
      inline FIXP_DBL fixmadddiv2_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
        INT discard = 0;
        INT result = x;
        asm  ("smlal %0, %1, %2, %3;\n"
              : "+r" (discard), "+r" (result)
              : "r" (a), "r" (b) );
        return result ;
      }
      #endif /* #ifdef FUNCTION_fixmadddiv2_DD */
    #endif /* __ARM_ARCH_6__ */

    #if defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_6__)

      #define FUNCTION_fixmadddiv2_DS
      #ifdef FUNCTION_fixmadddiv2_DS
      inline FIXP_DBL fixmadddiv2_DS (FIXP_DBL x, const FIXP_DBL a, const FIXP_SGL b) {
        INT result;
        asm("smlawb %0, %1, %2, %3 "
              : "=r" (result)
              : "r" (a), "r" (b), "r" (x) );
        return result ;
      }
      #endif /* #ifdef FUNCTION_fixmadddiv2_DS */

    #endif /* defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_6__) */

    #define FUNCTION_fixmadddiv2BitExact_DD
    #ifdef FUNCTION_fixmadddiv2BitExact_DD
    #define fixmadddiv2BitExact_DD(a, b, c) fixmadddiv2_DD(a, b, c)
    #endif /* #ifdef FUNCTION_fixmadddiv2BitExact_DD */

    #define FUNCTION_fixmsubdiv2BitExact_DD
    #ifdef FUNCTION_fixmsubdiv2BitExact_DD
    inline FIXP_DBL fixmsubdiv2BitExact_DD (FIXP_DBL x, const FIXP_DBL a, const FIXP_DBL b) {
      return x - fixmuldiv2BitExact_DD(a, b);
    }
    #endif /* #ifdef FUNCTION_fixmsubdiv2BitExact_DD */

    #define FUNCTION_fixmadddiv2BitExact_DS
    #ifdef FUNCTION_fixmadddiv2BitExact_DS
    #define fixmadddiv2BitExact_DS(a, b, c) fixmadddiv2_DS(a, b, c)
    #endif /* #ifdef FUNCTION_fixmadddiv2BitExact_DS */

    #define FUNCTION_fixmsubdiv2BitExact_DS
    #ifdef FUNCTION_fixmsubdiv2BitExact_DS
    inline FIXP_DBL fixmsubdiv2BitExact_DS (FIXP_DBL x, const FIXP_DBL a, const FIXP_SGL b) {
      return x - fixmuldiv2BitExact_DS(a, b);
    }
    #endif /* #ifdef FUNCTION_fixmsubdiv2BitExact_DS */
  
  /* ############################################################################# */
  #elif defined(__TMS470__)
  /* ############################################################################# */

    #define FUNCTION_fixmadddiv2_DS
    #ifdef FUNCTION_fixmadddiv2_DS
    inline FIXP_DBL fixmadddiv2_DS (FIXP_DBL x, const FIXP_DBL a, const FIXP_SGL b)
    {
     return _smlawb (a, b, x);
    }
    #endif /* #ifdef FUNCTION_fixmadddiv2_DS */

  /* ############################################################################# */
  #endif /* toolchain */
  /* ############################################################################# */

#endif /* __arm__ */

