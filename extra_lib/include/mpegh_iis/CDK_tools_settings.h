/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2007)
                               All Rights Reserved

   $Id$
   Author(s):   Oliver Moser
   Description: CDK tools settings

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __CDK_TOOLS_SETTINGS_H__
#define __CDK_TOOLS_SETTINGS_H__

#include "global_functional_range.h"

/* stuff which is always enabled until good mapping solution is found */

/* Sine table for the PCM evaluation beep */


/* huffman decoder for MPEG-D configuration */





/* huffman decoder for MPEG-D configuration end */

/* upsampler for MPEG-H CE phase 2 */
  #define INCLUDE_KBDWindow64
/* upsampler for MPEG-H CE phase 2 end */

/**** Math dependencies on functionality ****/






  /* Encoder only */



  /* utilized in CE phase2 decoder */
  #define CDK_MPEGH3DA_SINEWIN_ENABLE    

  #define RFFT672_ENABLE
  #ifdef RFFT672_ENABLE
    #define FFT336_ENABLE                 /* FFT-length for stereo-LPD tool is 672 but since this tool utilizes the rfft() the necessary FFT-length is N/2 = 672/2 = 336. */
    #define INCLUDE_SineTable168          /* After the rfft() called the FFT 336 a sine-table of length 168 is used for post sorting. */
  #endif

  #define RFFT336_ENABLE                  /* for stereo LPD without fullband LPD */
  #ifdef RFFT336_ENABLE
    #define FFT168_ENABLE
  #endif

  #define INCLUDE_SineWindow80
  #define INCLUDE_SineWindow160

/* select LPC-tools */
  /* LPC for TNS */

  #define CLPC_ANALYSIS_ENABLE 
  #define CLPC_ANALYSISLATTICE_ENABLE 
  #define CLPC_SYNTHESIS_ENABLE
  #define CLPC_SYNTHESISLATTICE_ENABLE
  #define CLPC_PARCOR_TO_LPC_ENABLE 
  #define CLPC_AUTO_TO_PARCOR_ENABLE
  #define CLPC_AUTOCORR_ENABLE
  #define CLPC_AUTO_TO_LPC_ENABLE


/* all applications which imply usage of SBR */



/* Configuration of decorrelator was moved to CDK_decor_settings.h */


/* Configuration of matixCalloc START */
/* #define MATRIX_CALLOC_4D_ENABLE */
/* #define MATRIX_CALLOC_5D_ENABLE */
/* Configuration of matixCalloc END */






/***** Bit stream sequencer dependencies to global functionality presets ****/

#if ( CDK_GFR_CHANNEL_CONFIG != CDK_GFR_CH_CFG_MONO )
#endif










/***** MDCT length dependencies to global functionality presets ****/

/* 3/4 raster frame length */

/* 10ms raster frame length */

/* radix 2 raster frame length */

    #define USE_CORE_FL_1024
    /* #define INCLUDE_KBDWindow512 (not required for TCX) */



/**** QMF functionality preset dependencies ****/








/* Enables QMF based resampling */

#if defined(USE_CORE_FL_1024)
  #define COREFRAMELENGTH 1024
#elif defined(USE_CORE_FL_960)
  #define COREFRAMELENGTH 960
#elif defined(USE_CORE_FL_768)
  #define COREFRAMELENGTH 768
#elif defined(USE_CORE_FL_512)
  #define COREFRAMELENGTH 512
#elif defined(USE_CORE_FL_480)
  #define COREFRAMELENGTH 480
#endif

  /* => no SBR */
  #define FRAMELENGTH            COREFRAMELENGTH

#define TD_UPSAMPLER_MAX_UP_RATIO 3

/* Framelength including possible resampling in QMF domain */
  #define FRAMELENGTH2 (FRAMELENGTH*TD_UPSAMPLER_MAX_UP_RATIO)

#if FRAMELENGTH > 1024
  #define MAX_FRAMELENGTH        1024
#else
  #define MAX_FRAMELENGTH        FRAMELENGTH
#endif

/**** FAC  functionality preset dependencies ****/


  /* FD <-> ACELP FAC */



/*
 * Filter coefficient type definition
 */
#define FIXP_QMF FIXP_DBL



/***** Performance, memory, tuning specific configuration macros ******/

/* NOTE: This macro is duplicated here for the partial processing to work. */
#define CDKTOOLS_PACKED_TABLES

/* The following HQ-switches enable 32x32 bit multiplications inside the fft */
/* resulting in a higher accuracy of the fft on costs of higher complexity   */
/* The compile switches do not affect the format of the rotation tables.     */
/* Note: 32-bit factors are not implemented for all hardware platforms       */
/* By default, the switches are not active, a prefix of "no" is added        */

#if !defined(__ADSP21000__) && !defined(__mips__)
#define FFT8_UNROLLED_ENABLE
#define FFT16_UNROLLED_ENABLE
#endif
#if  !defined(__ADSP21000__) &&!defined(__ARM_NEON__)  &&!defined(__XTENSA__) && !defined(__mips__)
#define FFT32_UNROLLED_ENABLE
#endif
/* This switch improves the quality and speed of the FFT12, FFT48, FFT192 and FFT384 */

#ifndef FFT8_UNROLLED_ENABLE
  #define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_8
#endif
#ifndef FFT16_UNROLLED_ENABLE
  #define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_16
#endif
#ifndef FFT32_UNROLLED_ENABLE
  #define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_32
#endif
#define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_64
#define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_128
#define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_256
#define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_512
/*#define CDK_TOOLS_ENABLE_FAST_SCRAMBLE_1024*/


/***** Misc function dependencies *****/

/* #define LPC_AUTOCORRELATION_NORMALIZE_ENABLE */

/***** Trigonometric function dependencies *****/

/***** Sqrt function dependencies *****/

/***** Inverse logarithm function dependencies *****/

/***** MDCT2QMF dependencies *****/


/***** MDCT dependencies *****/


/* The low overlap tables are mapped to identical tables. */

/***** QMF dependencies *****/















/***** DCT dependencies *****/



/* select the smallest sinewindow which serves all enabled DCTIV lengths */


/* DCTIV 10ms raster lengths */








/* DCTIV 3/4 radix 2 raster lengths */






/* DCTIV radix 2 length */



  #define DCTIV512_SINETABLE         SineTable1024
  #define DCTIV512_SINETABLE_SIZE    1024
  #define DCTIV512_SINETABLE_SIZE_LD 10















/* DCTIV 5/16 radix 2 length */














/* The SineWindow tables are orthogonal, they are not the same with different strides
   as the Sinetables are. */

/* 5/16 Radix 2 */


/* 10ms raster */


/* 3/4 Radix 2 */
/* Radix 2 */
  #define DCTIV512_SINEWINDOW SineWindow512

/***** Dependency FFTs *****/
#if defined(FFT336_ENABLE)
  #define INCLUDE_RotVectorReal336
  #define INCLUDE_RotVectorImag336
  #define FFT21_ENABLE
#endif
#if defined(FFT168_ENABLE)
  #define INCLUDE_RotVectorReal168
  #define INCLUDE_RotVectorImag168
  #define FFT21_ENABLE
#endif
#if defined(FFT21_ENABLE)
  #define INCLUDE_RotVectorReal21
  #define INCLUDE_RotVectorImag21
#endif


/* Within the next few lines, the smallest sinetable that serves all enabled FFT lengths is automatically selected. */

/* FFT radix 2 length */








  #define FFT128_SINETABLE         SineTable512
  #define FFT128_SINETABLE_SIZE    512
  #define FFT128_SINETABLE_SIZE_LD 9




#if  !defined(FFT32_UNROLLED_ENABLE)
  #define FFT32_SINETABLE           SineTable512
  #define FFT32_SINETABLE_SIZE      512
  #define FFT32_SINETABLE_SIZE_LD   9
#endif

#if  !defined(FFT16_UNROLLED_ENABLE)
  #define FFT16_SINETABLE           SineTable512
  #define FFT16_SINETABLE_SIZE      512
  #define FFT16_SINETABLE_SIZE_LD   9
#endif

#if  !defined(FFT8_UNROLLED_ENABLE)
  #define FFT8_SINETABLE            SineTable512
  #define FFT8_SINETABLE_SIZE       512
  #define FFT8_SINETABLE_SIZE_LD    9
#endif







/***** Hardware specific backend switches *****/

/* Twiddle factors for TI DSP FFT */
#ifdef TIC6400_DSP_LIB_AVAILABLE
    #define INCLUDE_w16
    #define INCLUDE_w32
    #define INCLUDE_w64
    #define INCLUDE_w128
    #define INCLUDE_w256
    #define INCLUDE_w512
#elif defined(TIC6700_DSP_LIB_AVAILABLE)
    #define INCLUDE_w16f
    #define INCLUDE_w32f
    #define INCLUDE_w64f
    #define INCLUDE_w128f
    #define INCLUDE_w256f
    #define INCLUDE_w512f
#endif

/* Optimization switches */
/* #define FFT_TWO_STAGE_MACRO_ENABLE */
/* #define FFT_APPLY_ROT_VECTOR_HQ */

#endif /* __CDK_TOOLS_SETTINGS_H__ */
