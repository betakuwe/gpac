/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2005)
                               All Rights Reserved

   $Id$
   Author(s):
   Description:

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef SCRAMBLE_H
#define SCRAMBLE_H

#include "CDK_tools_settings.h"
#include "common_fix.h"

#if defined(__arm__)
#include "arm/scramble.h"

#elif defined(__mips__) && defined(__mips_dsp)
#include "mips/scramble.h"

#elif defined(__ADSP21000__) && defined(__ADSP21469__)
#include "sharc/scramble.h"

#elif defined(__XTENSA__)
#include "xtensa/scramble_xtensa.h"

#endif

/*****************************************************************************

    functionname: scramble
    description:  bitreversal of input data
    returns:
    input:
    output:

*****************************************************************************/
#if !defined(FUNCTION_scramble)

inline void scramble(FIXP_DBL *x, INT length)
{
  C_ALLOC_ALIGNED_CHECK(x);

#ifndef SCRAMBLE
#define SCRAMBLE(x,a,b)     \
  tmp=x[2*(a)+0];  x[2*(a)+0]=x[2*(b)+0];  x[2*(b)+0]=tmp;  \
  tmp=x[2*(a)+1];  x[2*(a)+1]=x[2*(b)+1];  x[2*(b)+1]=tmp;
#endif

  FIXP_DBL tmp;

  switch (length)
  {
#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_8
  	case 8:
  	  SCRAMBLE(x,1,4)  	  SCRAMBLE(x,3,6)
      break;
#endif
#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_16
    case 16:
  	  SCRAMBLE(x, 1, 8)   SCRAMBLE(x, 2, 4)	  SCRAMBLE(x, 3,12)   SCRAMBLE(x, 5,10)
  	  SCRAMBLE(x, 7,14)	  SCRAMBLE(x,11,13)
      break;
#endif
#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_32
/* Note: If scramble_32 is defined, it is called instead of scramble() by caller (e.g. dit_fft) */
#ifndef FUNCTION_scramble_32
  	case 32:
  	  SCRAMBLE(x, 1,16)	  SCRAMBLE(x, 2, 8)	  SCRAMBLE(x, 3,24)	  SCRAMBLE(x, 5,20)
  	  SCRAMBLE(x, 6,12)	  SCRAMBLE(x, 7,28)	  SCRAMBLE(x, 9,18)	  SCRAMBLE(x,11,26)
  	  SCRAMBLE(x,13,22)	  SCRAMBLE(x,15,30)	  SCRAMBLE(x,19,25)	  SCRAMBLE(x,23,29)
      break;
#endif
#endif
#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_64
    case 64:
      {
        /* Note: This version needs on ARM926 only 33% of the cycles compared to the generic loop below */
        /* This table indicates, which entries of the vector should be exchanged. */
        static const UCHAR br_offsets64[28][2] = 
        {
          {  1, 32 }, {  2, 16 }, {  3, 48 }, {  4,  8 }, {  5, 40 }, {  6, 24 }, {  7, 56 }, {  9, 36 },
          { 10, 20 }, { 11, 52 }, { 13, 44 }, { 14, 28 }, { 15, 60 }, { 17, 34 }, { 19, 50 }, { 21, 42 },
          { 22, 26 }, { 23, 58 }, { 25, 38 }, { 27, 54 }, { 29, 46 }, { 31, 62 }, { 35, 49 }, { 37, 41 },
          { 39, 57 }, { 43, 53 }, { 47, 61 }, { 55, 59 }
        };
        int i;
        
        for (i = 28-1; i>=0; i--)
        {
          int src = br_offsets64[i][0];
          int dst = br_offsets64[i][1];
          SCRAMBLE(x,src,dst)
        }
      }
  	  break;
#endif


#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_128
  	case 128:
  	{
      /* In order to bit-reverse an 6-bit value, the lookup   */
      /* table must be used twice:                            */
      /* rev(n) = bitreverse[n&0xF]<<2 + bitreverse[n>>4]>>2  */
      
      static const UCHAR bitreverse[16] = { 0x0,0x8,0x4,0xC,0x2,0xA,0x6,0xE,0x1,0x9,0x5,0xD,0x3,0xB,0x7,0xF  };

    
      /* These bits indicate, if the entry x[n] has to be exchanged or not.  */
      /* The last words are skipped and replaced by hard-coded exchanges at  */
      /* the procedure end. The table was generated with a C-code.           */
      static const ULONG bits [3] = { 0xEEEEFEFE, 0xAAAAEAEA, 0x8888A8A8, /* 0x00008080 */};
  	
  	  int i, j, rev_n;
  	  ULONG bits16;
  	
  	  int n=0;
      for (i = 0; i < 2; i++) /* bits[0,1] */
      {
        bits16 = bits[i];
        for (j=16; j--;  n++)
        {
          rev_n = (bitreverse[n&0xF]<<2) + (bitreverse[n>>4]>>2);
          SCRAMBLE(x,2*n+1, rev_n+64)    
          if (bits16 & 1)
          {
            SCRAMBLE(x,2*n, rev_n)    
          }
          bits16 >>= 2;
        }
      }

      bits16 = bits[i];
      for (j=8; j--;  n++)  /* bits[2] */
      {
        if (bits16 & 2)
        {
          rev_n = (bitreverse[n&0xF]<<2) + (bitreverse[n>>4]>>2);
          SCRAMBLE(x,2*n+1, rev_n+64)    
        }
        n++;
        if (bits16 & 8)
        {
          rev_n = (bitreverse[n&0xF]<<2) + (bitreverse[n>>4]>>2);
          SCRAMBLE(x,2*n+1, rev_n+64)    
        }
        bits16 >>= 4;
      }

      SCRAMBLE(x,103,115)
      SCRAMBLE(x,111,123)
  	}
    break;
#endif




#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_256
  	case 256:
  	{
      /* In order to bit-reverse a 7-bit value, the lookup    */
      /* table must be used twice:                            */
      /* rev_n = (bitreverse[n&0xF]<<3)+(bitreverse[n>>4]>>1) */
      static const UCHAR bitreverse[16] = { 0x0,0x8,0x4,0xC,0x2,0xA,0x6,0xE,0x1,0x9,0x5,0xD,0x3,0xB,0x7,0xF  };

    
      /* These bits indicate, if the entry x[n] has to be exchanged or not.  */
      /* The last words are skipped and replaced by hard-coded exchange at   */
      /* the procedure end. The table was generated with a C-code.           */
      /* Note: The table is the same as for case 512, but packed in LONG     */
      static const ULONG bits[7] = 
  	  {
        0xFEFEFFFE, 0xEEEEFEEE, 0xEAEAEEEA, 0xAAAAEAAA, 
        0xA8A8AAA8, 0x8888A888, 0x80808880,/*0x00008000*/
  	  };
  	
  	  int i, j, rev_n;
  	  const ULONG *bitsPtr = bits;
  	  ULONG bits16;
  	
  	  int n=0;
      for (i=7; i--; )
      {
        bits16 = *bitsPtr++;
        for (j=16; j--;  n++)
        {
          /* If bit #1 is not set, then bit #0 is also not set - see table 'bits' above  */
          if (bits16 & 2)
          {
            rev_n = (bitreverse[n&0xF]<<3) + (bitreverse[n>>4]>>1);
            SCRAMBLE(x,2*n+1, rev_n+128)    
            if (bits16 & 1)
            {
              SCRAMBLE(x,2*n, rev_n)    
            }
          }
          bits16 >>= 2;
        }
      }
      SCRAMBLE(x,239,247)
  	}
    break;
#endif
  	  
#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_512
  	case 512:
  	{
      /* Note: This version needs on ARM926 only 48% of the cycles compared to the generic loop below */
    
      /* A 4-bit bit-reverse is done via lookup-table.      */
      /* Usage: rev(n) = bitreverse[n];  with n = [0..15]   */
      /* In order to bit-reverse an 8-bit value, the lookup */
      /* table must be used for each nibble:                */
      /* rev(n) = bitreverse[n&0xF]<<4 + bitreverse[n>>4];  */
      static const UCHAR bitreverse[16] = { 0x0,0x8,0x4,0xC,0x2,0xA,0x6,0xE,0x1,0x9,0x5,0xD,0x3,0xB,0x7,0xF  };
    
      /* These bits indicate, if the entry x[n] has to be exchanged or not.  */
      /* Each word must be used twice, the table is thus reduced to the half.*/
      /* The last words are skipped and replaced by hard-coded exchanges at  */
      /* the procedure end. The table was generated with a C-code.           */
      static const USHORT bits[14] = 
  	  {
        0xFFFE, 0xFEFE, 0xFEEE, 0xEEEE, 0xEEEA, 0xEAEA, 0xEAAA, 0xAAAA,
        0xAAA8, 0xA8A8, 0xA888, 0x8888, 0x8880, 0x8080/*0x8000, 0x0000 */
  	  };
  	
  	  int i, j, rev_n;
  	  const USHORT *bitsPtr = bits;
  	  ULONG bits16;
  	
  	  int n=0;
      for (i=14; i--; )
      {
        bits16 = (ULONG) *bitsPtr++;
        bits16 = bits16 + (bits16 << 16);    /* duplicate bits16 into high and low part of 32-bit */
        for (j=16; j--;  n++)
        {
          /* If bit #1 is not set, then bit #0 is also not set - see table 'bits' above  */
          if (bits16 & 2)
          {
            rev_n = (bitreverse[n&0xF]<<4) + bitreverse[n>>4];
            SCRAMBLE(x,2*n+1, rev_n+256)    
            if (bits16 & 1)
            {
              SCRAMBLE(x,2*n, rev_n)    
            }
          }
          bits16 >>= 2;
        }
      }
      SCRAMBLE(x,463,487)
      SCRAMBLE(x,479,503)
  	}
    break;
#endif
    default:
      INT m,k,j;
      for (m=1,j=0; m<length-1; m++)
      {
        {for(k=length>>1; (!((j^=k)&k)); k>>=1);}
        if (j>m)
        {
          tmp=x[2*m];
          x[2*m]=x[2*j];
          x[2*j]=tmp;

          tmp=x[2*m+1];
          x[2*m+1]=x[2*j+1];
          x[2*j+1]=tmp;
        }
      }
      break;

#ifdef CDK_TOOLS_ENABLE_FAST_SCRAMBLE_1024
#pragma "Error: scramble length 1024 not implemented!"
#endif
  }
}


#endif /* !defined(FUNCTION_scramble) */

#endif /* SCRAMBLE_H */
