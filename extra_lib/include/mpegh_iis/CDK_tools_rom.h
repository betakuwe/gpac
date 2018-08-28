/***************************  Fraunhofer IIS CDK Tools  ***********************

                        (C) Copyright Fraunhofer IIS (2008)
                               All Rights Reserved

   $Id$
   Author(s):   Oliver Moser
   Description: ROM tables used by CDK tools

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __CDK_TOOLS_ROM_H__
#define __CDK_TOOLS_ROM_H__

#include "CDK_tools_settings.h"
#include "common_fix.h"
#include "CDK_audio.h"

/* TI FFT twiddles */

#ifdef INCLUDE_w1024
  extern const int w1024[2 * 1020];
#endif
#ifdef INCLUDE_w512
  extern const int w512[2 * 510];
#endif
#ifdef INCLUDE_w256
  extern const int w256[2 * 252];
#endif
#ifdef INCLUDE_w128
  extern const int w128[2 * 126];
#endif
#ifdef INCLUDE_w64
  extern const int w64[2 * 60];
#endif
#ifdef INCLUDE_w32
  extern const int w32[2 * 30];
#endif
#ifdef INCLUDE_w16
  extern const int w16[2 * 12];
#endif

/* MPEGD MDCT2QMF Module */

/* sinetables */

/* None radix2 rotation vectors */
#ifdef USE_PACKED_ROTVECTOR_240
extern RAM_ALIGN const LONG RotVector240[210];
#else
#endif /* USE_PACKED_ROTVECTOR_240 */
#ifdef INCLUDE_RotVectorReal160
extern RAM_ALIGN const FIXP_STB RotVectorReal160[160];
#endif
#ifdef INCLUDE_RotVectorImag160
extern RAM_ALIGN const FIXP_STB RotVectorImag160[160];
#endif
#ifdef INCLUDE_RotVectorReal20
extern RAM_ALIGN const FIXP_STB RotVectorReal20[20];
#endif
#ifdef INCLUDE_RotVectorImag20
extern RAM_ALIGN const FIXP_STB RotVectorImag20[20];
#endif
#ifdef INCLUDE_RotVectorReal320
extern RAM_ALIGN const FIXP_STB RotVectorReal320[320];
#endif
#ifdef INCLUDE_RotVectorImag320
extern RAM_ALIGN const FIXP_STB RotVectorImag320[320];
#endif
#ifdef INCLUDE_RotVectorReal120
extern RAM_ALIGN const FIXP_STB RotVectorReal120[120];
#endif
#ifdef INCLUDE_RotVectorImag120
extern RAM_ALIGN const FIXP_STB RotVectorImag120[120];
#endif
#ifdef INCLUDE_RotVectorReal21
extern const FIXP_STB RotVectorReal21[21];
#endif
#ifdef INCLUDE_RotVectorImag21
extern const FIXP_STB RotVectorImag21[21];
#endif
#ifdef INCLUDE_RotVectorReal168
extern const FIXP_STB RotVectorReal168[168];
#endif
#ifdef INCLUDE_RotVectorImag168
extern const FIXP_STB RotVectorImag168[168];
#endif
#ifdef INCLUDE_RotVectorReal336
extern const FIXP_STB RotVectorReal336[336];
#endif
#ifdef INCLUDE_RotVectorImag336
extern const FIXP_STB RotVectorImag336[336];
#endif


/* Regular sine tables */
extern RAM_ALIGN const FIXP_STP SineTable1024[];
extern RAM_ALIGN const FIXP_STP SineTable512[];
#ifdef INCLUDE_SineTable168
extern const FIXP_STP SineTable168[];
#endif 
#ifdef INCLUDE_SineTable10
extern RAM_ALIGN const FIXP_STP SineTable10[];
#endif

/* AAC-LC windows */
extern RAM_ALIGN const FIXP_WTP SineWindow1024[];
extern RAM_ALIGN const FIXP_WTP KBDWindow1024[];
extern RAM_ALIGN const FIXP_WTP SineWindow128[];
extern RAM_ALIGN const FIXP_WTP KBDWindow128[];


/* AAC-LD windows */
extern RAM_ALIGN const FIXP_WTP SineWindow512[];

/* USAC TCX Window */
extern RAM_ALIGN const FIXP_WTP SineWindow256[256];

/* USAC 8/3 windows */

/* DCT and others */
#ifdef CDK_MPEGH3DA_SINEWIN_ENABLE 
#ifdef INCLUDE_SineWindow160
extern const FIXP_WTP SineWindow160[];
#endif
#ifdef INCLUDE_SineWindow80
extern const FIXP_WTP SineWindow80[];
#endif
#endif
extern RAM_ALIGN const FIXP_WTP SineWindow64[];
extern RAM_ALIGN const FIXP_WTP SineWindow32[];

/**
 * \brief Helper table for window slope mapping. You should prefer the usage of the
 * function CDKgetWindowSlope(), this table is only made public for some optimized
 * access inside dct.cpp.
 */
extern const FIXP_WTP *const windowSlopes[2][4][9];

/**
 * \brief Window slope access helper. Obtain a window of given length and shape.
 * \param length Length of the window slope.
 * \param shape Shape index of the window slope. 0: sine window, 1: Kaiser-Bessel. Any other
 *              value is applied a mask of 1 to, mapping it to either 0 or 1.
 * \param Pointer to window slope or NULL if the requested window slope is not available.
 */
const FIXP_WTP * CDKgetWindowSlope(int length, int shape);

extern const FIXP_WTP sin_twiddle_L64[];



/**
 * Audio bitstream element specific syntax flags:
 */
#define AC_EL_GA_CCE    0x00000001  /*!< GA AAC coupling channel element (CCE) */

/*
 * Raw Data Block list items.
 */
typedef enum {
  element_instance_tag,
  common_window,             /* -> decision for link_sequence */
  global_gain,
  ics_info, /* ics_reserved_bit, window_sequence, window_shape, max_sfb, scale_factor_grouping, predictor_data_present, ltp_data_present, ltp_data */
  max_sfb,
  ms, /* ms_mask_present, ms_used */
  /*predictor_data_present,*/ /* part of ics_info */
  ltp_data_present,
  ltp_data,
  section_data,
  scale_factor_data,
  pulse, /* pulse_data_present, pulse_data  */
  tns_data_present,
  tns_data,
  gain_control_data_present,
  gain_control_data,
  esc1_hcr,
  esc2_rvlc,
  spectral_data,
  enhancedNoiseFilling,

  scale_factor_data_usac,
  core_mode,                 /* -> decision for link_sequence */
  common_tw,
  lpd_channel_stream,
  tw_data,
  noise,
  ac_spectral_data,
  fac_data,
  tns_active, /* introduced in MPEG-D usac CD */
  tns_data_present_usac,
  common_max_sfb,

  coupled_elements,          /* only for CCE parsing */
  gain_element_lists,        /* only for CCE parsing */

  /* Non data list items */
  adtscrc_start_reg1,
  adtscrc_start_reg2,
  adtscrc_end_reg1,
  adtscrc_end_reg2,
  drmcrc_start_reg,
  drmcrc_end_reg,
  next_channel,
  next_channel_loop,
  link_sequence,
  end_of_sequence
} rbd_id_t;

struct element_list {
  const rbd_id_t *id;
  const struct element_list *next[2];
};

typedef struct element_list element_list_t;
/**
 * \brief get elementary stream pieces list for given parameters.
 * \param aot audio object type
 * \param epConfig the epConfig value from the current Audio Specific Config
 * \param nChannels amount of channels contained in the current element.
 * \param layer the layer of the current element.
 * \param elFlags element specific flags.
 * \return element_list_t parser guidance structure.
 */
const element_list_t * getBitstreamElementList(AUDIO_OBJECT_TYPE aot, SCHAR epConfig, UCHAR nChannels, UCHAR layer, UINT elFlags);



typedef enum
{
  /* n.a. */
  CDK_FORMAT_1_0          = 1,    /* mono */
  CDK_FORMAT_2_0          = 2,    /* stereo */
  CDK_FORMAT_3_0_FC       = 3,    /* 3/0.0 */
  CDK_FORMAT_3_1_0        = 4,    /* 3/1.0 */
  CDK_FORMAT_5_0          = 5,    /* 3/2.0 */
  CDK_FORMAT_5_1          = 6,    /* 5.1 */
  CDK_FORMAT_7_1_ALT      = 7,    /* 5/2.1 ALT */
  /* 8 n.a.*/
  CDK_FORMAT_3_0_RC       = 9,    /* 2/1.0 */
  CDK_FORMAT_2_2_0        = 10,   /* 2/2.0 */
  CDK_FORMAT_6_1          = 11,   /* 3/3.1 */
  CDK_FORMAT_7_1          = 12,   /* 3/4.1 */
  CDK_FORMAT_22_2         = 13,   /* 22.2 */
  CDK_FORMAT_5_2_1        = 14,   /* 5/2.1*/
  CDK_FORMAT_5_5_2        = 15,   /* 5/5.2 */
  CDK_FORMAT_9_1          = 16,   /* 5/4.1 */
  CDK_FORMAT_6_5_1        = 17,   /* 6/5.1 */
  CDK_FORMAT_6_7_1        = 18,   /* 6/7.1 */
  CDK_FORMAT_5_6_1        = 19,   /* 5/6.1 */
  CDK_FORMAT_7_6_1        = 20,   /* 7/6.1 */
  CDK_FORMAT_IN_LISTOFCHANNELS = 21,
  CDK_FORMAT_OUT_LISTOFCHANNELS = 22,
  /* 20 formats + In & Out list of channels */
  CDK_NFORMATS            = 23,
  CDK_FORMAT_FAIL         = -1
} CDK_converter_formatid_t;

extern const INT format_nchan[CDK_NFORMATS+9-2];

#endif

