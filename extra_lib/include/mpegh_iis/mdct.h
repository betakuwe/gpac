/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2015)
                               All Rights Reserved

   $Id$
   Author(s):   Manuel Jander, Josef Hoepfl, Youliy Ninov
   Description: MDCT/MDST routines

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __MDCT_H__
#define __MDCT_H__

#include "CDK_tools_settings.h"


#include "common_fix.h"

#define MDCT_OUT_HEADROOM     2 /* Output additional headroom */
#define PCM_OUT_BITS DFRACT_BITS
#define PCM_OUT_HEADROOM 8 /* Must have the same values as DMXH_HEADROOM */

#define MDCT_OUTPUT_SCALE    (-MDCT_OUT_HEADROOM+PCM_OUT_HEADROOM+(DFRACT_BITS-PCM_OUT_BITS))
/* Define and determine rounding constant in case, we shift to the right */
#if (MDCT_OUTPUT_SCALE > 1) && (PCM_OUT_HEADROOM == 0)
#define MDCT_OUTPUT_RND_VAL ((FIXP_DBL) (1<< (MDCT_OUTPUT_SCALE-1)) )
#else
#undef MDCT_OUTPUT_RND_VAL
#endif
/* Refer to "Output word length" in ISO/IEC 14496-3:2008(E) 23.2.3.6 */
#define MDCT_OUTPUT_GAIN      16

#if (MDCT_OUTPUT_SCALE >= 0)
#define IMDCT_SCALE(x) SATURATE_RIGHT_SHIFT(x, MDCT_OUTPUT_SCALE, PCM_OUT_BITS)
#else
#define IMDCT_SCALE(x) SATURATE_LEFT_SHIFT(x, -MDCT_OUTPUT_SCALE, PCM_OUT_BITS)
#endif
#define IMDCT_SCALE_DBL(x) (FIXP_DBL)(x)

#define MLT_FLAG_CURR_ALIAS_SYMMETRY  1

typedef enum {
  BLOCK_LONG = 0, /* normal long block */
  BLOCK_START,    /* long start block */
  BLOCK_SHORT,    /* 8 short blocks sequence */
  BLOCK_STOP      /* long stop block*/
} BLOCK_TYPE;

typedef enum {
  SHAPE_SINE = 0,
  SHAPE_KBD,
  SHAPE_LOL
} WINDOW_SHAPE;

/**
 * \brief MDCT persistent data
 */
typedef struct {
  union {
    FIXP_DBL *freq;
    FIXP_DBL *time;
  } overlap;                       /**< Pointer to overlap memory */

  const FIXP_WTP *prev_wrs;        /**< pointer to previous right window slope  */
  int prev_tl;                     /**< previous transform length */
  int prev_nr;                     /**< previous right window offset */
  int prev_fr;                     /**< previous right window slope length */
  int ov_offset;                   /**< overlap time data fill level */
  int ov_size;                     /**< Overlap buffer size in words */

  int prevAliasSymmetry;
  int prevPrevAliasSymmetry;

  FIXP_DBL *pFacZir;
  FIXP_DBL *pAsymOvlp;            /**< pointer to asymmetric overlap (used for stereo LPD transition) */
} mdct_t;

typedef mdct_t* H_MDCT;

/**
 * \brief Initialize as valid MDCT handle
 *
 * \param hMdct handle of an allocated MDCT handle.
 * \param overlap pointer to FIXP_DBL overlap buffer.
 * \param overlapBufferSize size in FIXP_DBLs of the given overlap buffer.
 */
void mdct_init( H_MDCT hMdct,
                FIXP_DBL *overlap,
                INT overlapBufferSize );

/**
 * \brief perform MDCT transform (time domain to frequency domain) with given parameters.
 *
 * \param hMdct handle of an allocated MDCT handle.
 * \param pTimeData pointer to input time domain signal
 * \param noInSamples number of input samples
 * \param mdctData pointer to where the resulting MDCT spectrum will be stored into.
 * \param nSpec number of spectra
 * \param pMdctData_e pointer to the input data exponent. Updated accordingly on return for output data.
 * \return number of input samples processed.
 */
INT mdct_block (
        H_MDCT hMdct,
        const INT_PCM * pTimeData,
        const INT noInSamples,
        FIXP_DBL *RESTRICT mdctData,
        const INT nSpec,
        const INT tl,
        const FIXP_WTP *pRightWindowPart,
        const INT fr,
        SHORT *pMdctData_e
       );

/**
 * \brief perform MDST transform (time domain to frequency domain) with given parameters.
 *
 * \param hMdct handle of an allocated MDCT handle.
 * \param pTimeData pointer to input time domain signal
 * \param noInSamples number of input samples
 * \param mdctData pointer to where the resulting MDCT spectrum will be stored into.
 * \param nSpec number of spectra
 * \param pMdctData_e pointer to the input data exponent. Updated accordingly on return for output data.
 * \return number of input samples processed.
 */
INT mdst_block (
        H_MDCT hMdct,
        const INT_PCM * pTimeData,
        const INT noInSamples,
        FIXP_DBL *RESTRICT mdctData,
        const INT nSpec,
        const INT tl,
        const FIXP_WTP *pRightWindowPart,
        INT fr,
        SHORT *pMdctData_e
       );


/**
 * \brief add/multiply 2/N transform gain and MPEG4 part 3 defined output gain (see definition
 *        of MDCT_OUTPUT_GAIN) to given mantissa factor and exponent.
 * \param pGain pointer to the mantissa of a gain factor to be applied to IMDCT data.
 * \param pExponent pointer to the exponent of a gain factor to be applied to IMDCT data.
 * \param tl length of the IMDCT where the gain *pGain * (2 ^ *pExponent) will be applied to.
 */
void imdct_gain(
        FIXP_DBL *pGain,
        int *pExponent,
        int tl
        );

/**
 * \brief drain buffered output samples into given buffer. Changes the MDCT state.
 */
INT imdct_drain(
        H_MDCT hMdct,
        FIXP_DBL * pTimeData,
        INT nrSamplesRoom
        );


/**
 * \brief Copy overlap time domain data to given buffer. Does not change the MDCT state.
 * \return number of actually copied samples (ov + nr).
 */
INT imdct_copy_ov_and_nr(
        H_MDCT hMdct,
        FIXP_DBL * pTimeData,
        INT nrSamples
        );

INT imdct_copy_full(
        H_MDCT hMdct,
        FIXP_DBL * pTimeData,
        INT nrSamples
        );

/**
 * \brief Adapt MDCT parameters for non-matching window slopes.
 * \param hMdct handle of an allocated MDCT handle.
 * \param pfl pointer to left overlap window side length.
 * \param pnl pointer to length of the left n part of the window.
 * \param tl transform length.
 * \param wls pointer to the left side overlap window coefficients.
 * \param noOutSamples desired number of output samples.
 */
void imdct_adapt_parameters(H_MDCT hMdct, int *pfl, int *pnl, int tl, const FIXP_WTP *wls, int noOutSamples);

/**
 * \brief perform several inverse MLT transforms (frequency domain to time domain) with given parameters.
 *
 * \param hMdct handle of an allocated MDCT handle.
 * \param output pointer to where the output time domain signal will be stored into.
 * \param spectrum pointer to the input MDCT spectra.
 * \param scalefactors exponents of the input spectrum.
 * \param nSpec number of MDCT spectrums.
 * \param noOutSamples desired number of output samples.
 * \param tl transform length.
 * \param wls pointer to the left side overlap window coefficients.
 * \param fl left overlap window side length.
 * \param wrs pointer to the right side overlap window coefficients of all individual IMDCTs.
 * \param fr right overlap window side length of all individual IMDCTs.
 * \param gain factor to apply to output samples (if != 0).
 * \param flags flags controlling the type of transform
 * \return number of output samples returned.
 */
INT  imlt_block(
        H_MDCT hMdct,
        FIXP_DBL *output,
        FIXP_DBL *spectrum,
        const SHORT scalefactor[],
        const INT nSpec,
        const INT noOutSamples,
        const INT tl,
        const FIXP_WTP *wls,
        INT fl,
        const FIXP_WTP *wrs,
        const INT fr,
        FIXP_DBL gain,
        int flags);

/**
 * \brief perform several inverse MDCT transforms (frequency domain to time domain) with given parameters.
 *
 * \param hMdct handle of an allocated MDCT handle.
 * \param output pointer to where the output time domain signal will be stored into.
 * \param spectrum pointer to the input MDCT spectra.
 * \param scalefactor exponents of the input spectrum.
 * \param nSpec number of MDCT spectra.
 * \param noOutSamples desired number of output samples.
 * \param tl transform length.
 * \param wls pointer to the left side overlap window coefficients.
 * \param fl left overlap window side length.
 * \param wrs pointer to the right side overlap window coefficients of all individual IMDCTs.
 * \param fr right overlap window side length of all individual IMDCTs.
 * \param gain factor to apply to output samples (if != 0).
 * \return number of output samples returned.
 */
inline INT imdct_block(
        H_MDCT hMdct,
        FIXP_DBL *output,
        FIXP_DBL *spectrum,
        const SHORT scalefactor[],
        const INT nSpec,
        const INT noOutSamples,
        const INT tl,
        const FIXP_WTP *wls,
        INT fl,
        const FIXP_WTP *wrs,
        const INT fr,
        FIXP_DBL gain)
{
  hMdct->prevAliasSymmetry = 0;
  hMdct->prevPrevAliasSymmetry = 0;
  return imlt_block(hMdct, output, spectrum, scalefactor, nSpec, noOutSamples, tl, wls, fl, wrs, fr, gain, 0);
}

/**
 * \brief perform several inverse MDST transforms (frequency domain to time domain) with given parameters.
 *
 * \param hMdct handle of an allocated MDCT handle.
 * \param output pointer to where the output time domain signal will be stored into.
 * \param spectrum pointer to the input MDCT spectra.
 * \param scalefactor exponents of the input spectrum.
 * \param nSpec number of MDCT spectra.
 * \param noOutSamples desired number of output samples.
 * \param tl transform length.
 * \param wls pointer to the left side overlap window coefficients.
 * \param fl left overlap window side length.
 * \param wrs pointer to the right side overlap window coefficients of all individual IMDCTs.
 * \param fr right overlap window side length of all individual IMDCTs.
 * \param gain factor to apply to output samples (if != 0).
 * \return number of output samples returned.
 */
inline INT imdst_block(
        H_MDCT hMdct,
        FIXP_DBL *output,
        FIXP_DBL *spectrum,
        const SHORT scalefactor[],
        const INT nSpec,
        const INT noOutSamples,
        const INT tl,
        const FIXP_WTP *wls,
        INT fl,
        const FIXP_WTP *wrs,
        const INT fr,
        FIXP_DBL gain )
{
  hMdct->prevAliasSymmetry = 1;
  hMdct->prevPrevAliasSymmetry = 1;
  return imlt_block(hMdct, output, spectrum, scalefactor, nSpec, noOutSamples, tl, wls, fl, wrs, fr, gain, MLT_FLAG_CURR_ALIAS_SYMMETRY);
}


#endif /* __MDCT_H__ */
