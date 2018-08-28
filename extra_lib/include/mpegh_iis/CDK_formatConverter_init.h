/***********************************************************************************
 
 This software module was originally developed by 
 
 Fraunhofer IIS
 
 in the course of development of the ISO/IEC 23008-3 for reference purposes and its 
 performance may not have been optimized. This software module is an implementation
 of one or more tools as specified by the ISO/IEC 23008-3 standard. ISO/IEC gives 
 you a royalty-free, worldwide, non-exclusive, copyright license to copy, distribute, 
 and make derivative works of this software module or modifications  thereof for use
 in implementations or products claiming conformance to the ISO/IEC 23008-3 standard 
 and which satisfy any specified conformance criteria. Those intending to use this 
 software module in products are advised that its use may infringe existing patents. 
 ISO/IEC have no liability for use of this software module or modifications thereof. 
 Copyright is not released for products that do not conform to the ISO/IEC 23008-3 
 standard.
 
 Fraunhofer IIS retains full right to modify and use the code for 
 their own purpose, assign or donate the code to a third party and to inhibit third 
 parties from using the code for products that do not conform to MPEG-related ITU 
 Recommendations and/or ISO/IEC International Standards.
 
 This copyright notice must be included in all copies or derivative works. 
 
 Copyright (c) ISO/IEC 2013.
 
 ***********************************************************************************/

#ifndef CONVERTER_H
#define CONVERTER_H

#include "global_functional_range.h"


#include "CDK_formatConverter_intern.h"
#include "common_fix.h"


#define DYNAMIC_DMX_MATR

/**

 * Error codes
 * ************************************************************************** */
typedef enum
{
    FORMAT_CONVERTER_STATUS_OK =              0,      /* success */
    FORMAT_CONVERTER_STATUS_FAILED =          -1,     /* generic error */
    FORMAT_CONVERTER_STATUS_MISSING_RULE =    -2,     /* missing downmix rule */
    FORMAT_CONVERTER_STATUS_INFORMAT =        -3,     /* invalid input format index */
    FORMAT_CONVERTER_STATUS_OUTFORMAT =       -4,     /* invalid output format index */
    FORMAT_CONVERTER_STATUS_SFREQ =           -5,     /* invalid sampling frequency */
    FORMAT_CONVERTER_STATUS_BLOCKLENGTH =     -6,     /* invalid block length */
    FORMAT_CONVERTER_STATUS_TRIM =            -7,     /* out-of-range trim values */
    FORMAT_CONVERTER_STATUS_RANDOMIZATION =   -8,     /* randomization out-of-range */
    FORMAT_CONVERTER_STATUS_BANDS =           -9      /* bands out-of-range */
} converter_status_t;


/* public data types / structures
 * ************************************************************************** */

/* API functions
 * ************************************************************************** */

/**
  * Set input format or output format that is not in list of known formats.
  * Each function call either sets input _or_ output format.
  * Allocates memory.
  */
void converter_set_inout_format(
        IIS_FORMATCONVERTER_INTERNAL_HANDLE    fcInt,
        const INT                              inout,           /* in:  0: set input format, 1: set output format */
        const INT                              num_channels,    /* in:  number of channels in setup to define */
        const AFC_FORMAT_CONVERTER_CHANNEL_ID *channel_vector   /* in:  vector containing channel enums */
);


/**
 * Initialize audio processing.
 *
 * No memory is allocated, nothing to release.
 */
converter_status_t
converter_init(
        IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt,
        converter_pr_t                  *params,        /* out: initialized parameters */
        const CDK_converter_formatid_t  input_format,   /* in:  input format id */
        const CDK_converter_formatid_t  output_format,  /* in:  output format id */
        const INT*                      randomization,  /* in:  randomization angles [azi,ele,azi,ele, ... in degrees] */
        const INT                       sfreq_Hz,       /* in:  sampling frequency [Hz] */
        const INT                       blocklength,    /* in:  processing block length [samples] */
        const UINT                      nbands,         /* in:  number of filterbank processing bands */
        const FIXP_DBL*                 bands_nrm       /* in:  filterbank processing bands center frequencies [normalized frequency] */
);


FIXP_DBL peak_filter(
                  const FIXP_DBL f,    /* peak frequency [Hz] */
                  INT            f_e,
                  const FIXP_DBL q,    /* peak Q factor */ 
                  INT            q_e,
                  const FIXP_DBL g,    /* peak gain */
                  INT            g_e,
                  const FIXP_DBL G,    /* gain */
                  INT            G_e,
                  const FIXP_DBL b,    /* band center frequency [Hz] */
                  INT            b_e,
                  INT            *result_e);  

void setActiveDownmixRange (IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt, INT fs);
void setActiveDownmixRange_StftErb (IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt, INT fs);
void normalizePG (IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt);
void initElevSptlParms (IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt, FIXP_DBL elv, const UINT num_band, const FIXP_DBL fs, const INT * randomization, int nchanout );


#endif  /* CONVERTER_H */
