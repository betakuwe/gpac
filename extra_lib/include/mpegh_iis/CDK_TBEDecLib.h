/******************************************************************************

                        (C) Copyright Fraunhofer IIS (2015)
                               All Rights Reserved

   $Id$
   Author(s):   Thomas Blender (fixed-point version)
                Alfonso Pino Garcia (fixed-point version)
                Arthur Tritthart (XTENSA version)
   Description: Time-domain Bandwidth Extension (TBE)

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __CDK_TBEDECLIB_H_
#define __CDK_TBEDECLIB_H_

#include "CDK_core.h"
#include "common_fix.h"

#include "CDK_TBEComLib.h"



/* Temporary switch to decide whether to use the standard TBE-implementation (define) as stated in ISO/IEC JTC1/SC29/WG11 MPEG2015/M36530 
 * or the current float-decoder implementation (undefine). 
 */
#define TBE_STD


#define NUM_Q_LSF 5


typedef struct tbe_data_struct {
    UCHAR tbe_heMode;                     /* 1: TBE decoding of current frame uses low bit rate high efficiency mode. 0: high resolution configuration (tbe_hrConfig) is enabled. */
    UCHAR idxFrameGain;                   /* overall frame gain adjustment. */
    UCHAR idxSubGains;                    /* temporal sub-frame gain shape adjustment. */
    UCHAR idxShbFrGain;                   /* overall high band target gain. only read if tbe_heMode is zero and tbe_hrConfig is 1. */
#ifdef TBE_STD
    UCHAR lsf_idx[2];                     /* LSF data associated with the first [0] / second [1] stage LSFs used to estimate the LSP and then subsequently the interpolated sub-frame LP parameters. */
    UCHAR tbe_hrConfig;                   /* signals whether the current frame uses high resolution configuration. only read from bit stream if tbe_heMode is zero. */
    UCHAR tbe_nlConfig;                   /* signals the NL configuration that is to be used to generate the HE LB excitation. only read from bit stream if tbe_heMode is set to zero. default value is 1, if tbe_heMode is 1. */
    UCHAR idxMixConfig;                   /* data to control HB excitation generation. only read from bit stream if tbe_heMode is zero. */
    UCHAR idxResSubGains;                 /* data for temporal sub-frame residual gain shape adjustment. only read from bit stream if tbe_heMode is zero and tbe_hrConfig is 1. */
    UCHAR idxShbExcResp[2];               /* data to filter the HE excitation. only read from bit stream if tbe_heMode is zero and tbe_hrConfig is 1. */
#else
    INT coder_type[2];                /* bit 0: generic decoder yes(1)/no(0); for non generic decoder (bit 0 = 0) bit 1: voiced(1)/unvoiced(0) */
    UCHAR lsf_q_num_bits[NUM_Q_LSF];  /* lsf_idx */
    INT grid_idx;
    INT m_idx;
    UCHAR idx_res_gs[4];
    INT idx_mixFac;
#endif
} TBE_DATA, *TBE_DATA_HANDLE;



/*************************************************************************/
/*                             module  defines                           */
/*************************************************************************/

#define CDK_TBEDEC_ERROR_FIRST             1000

/*! The error return values of Temporal Bandwidth Extension */
typedef enum {
  CDK_TBEDEC_NO_ERROR                      = 0,
  CDK_TBEDEC_WARNING_1,
  CDK_TBEDEC_WARNING_2,
  CDK_TBEDEC_ERROR_INVALID_HANDLE          = CDK_TBEDEC_ERROR_FIRST,
  CDK_TBEDEC_ERROR_MEMORY,
  CDK_TBEDEC_ERROR_INVALID_SETUP,
  CDK_TBEDEC_ERROR_INVALID_INPUT,
  CDK_TBEDEC_ERROR_UNKNOWN
} CDK_TBEDEC_RETURN;




/*************************************************************************/
/*                             module  data                              */
/*************************************************************************/

typedef struct CDK_tbedec_instance_struct {
  /* TBE synthesis buffer are located at structure start to be aligned */
  FIXP_TBE tbe_synth_buffer[CDK_TBE_L_DIV_1024 - (2*CDK_TBE_TBE_DEC_DELAY)];
  FIXP_DBL tbe_synth[(2 * CDK_TBE_L_DIV_1024) + (2 * CDK_TBE_TBE_DEC_DELAY) + CDK_TBE_TD_RS_DELAY];

  FIXP_TBE old_bwe_exc[2 * CDK_TBE_PIT16k_MAX];
  FIXP_TBE old_bwe_exc_extended[CDK_TBE_NL_BUFF_OFFSET];
  INT      old_bwe_exc_extended_exp;
  FIXP_TBE bwe_non_lin_prev_scale;
  UCHAR tbe_heMode;

} CDK_TBEDEC_INSTANCE, *CDK_TBEDEC_INSTANCE_HANDLE;

void CDK_TBE_dequantizeSHBparams(
                    FIXP_TBE                         *Q_lsf,              /* o  : SHB LSF from de-quantization            */
                    INT                              *Q_lsf_e,            /* o  : SHB LSF from de-quantization exponents  */
                    FIXP_TBE                         *Q_subgain,          /* o  : SHB subframe gains from de-quantization     */
                    FIXP_TBE                         *Q_framegrain,       /* o  : SHB frame gain from de-quantization         */
                    INT                              *Q_framegrain_e,     /* o  : SHB frame gain from de-quantization exp     */
                    FIXP_TBE                         *Q_hbEnerTarget,
                    INT                              *Q_hbEnerTarget_e,
                    FIXP_TBE                         *Q_resSubGain,
                    FIXP_TBE                         *Q_mixFactors,
                    FIXP_TBE                         *Q_ShbExcResp,
                    UCHAR                            *lsf_idx,
                    UCHAR                            idxResSubGains,
              const UCHAR                            idxMixConfig,
              const UCHAR                            idxShbFrGain,
              const UCHAR                            idxSubGains,
              const UCHAR                            idxFrameGain,
              const UCHAR                            *idxShbExcResp,
              const UCHAR                            tbe_hrConfig,
              const UCHAR                            tbe_heMode
);

/*************************************************************************
main TBE deocding function
**************************************************************************/

CDK_TBEDEC_RETURN CDK_TBEDecLibApply(
              CDK_TBEDEC_INSTANCE_HANDLE              hInstance,          /* i/o: instance handle                          */
                    FIXP_TBE                         *bwe_exc_buf,        /* i  : excitation for TBE, exponent SF_EXC (16) */
                    FIXP_TBE                         *bwe_exc_extended,   /* i  : bandwidth extended excitation            */
              const FIXP_TBE                         *voice_factors,      /* i  : voicing factors                          */     /* assume always positiv and lower than 1.0f voice factors */
                    FIXP_TBE                         *synth,              /* o  : SHB synthesis/final synthesis            */
                    short                             first_frame,        /* i  : flag indicating first frame after switch */
              const short                             L_frame             /* i  : ACELP frame length                       */
);

/*************************************************************************
genereate transition
**************************************************************************/
void CDK_TBEDecLibGenTransition(
  CDK_TBEDEC_INSTANCE_HANDLE              hInstance,          /**< inout: instance handle                     */
  short                                   length,             /**< in:  targeted length of transition signal  */
  FIXP_DBL                               *output              /**< out: synthesized transitions signal        */
);

/*************************************************************************
initialization of an instance of this module, pass a ptr to a hInstance
\return returns an error code
**************************************************************************/
int CDK_TBEDecLibNew(
              CDK_TBEDEC_INSTANCE_HANDLE                 *phInstance          /**< inout: instance handle of Temporal Bandwidth Extension */
);

/*************************************************************************
TBE initialization
**************************************************************************/
int CDK_TBEDecLibInit(
  CDK_TBEDEC_INSTANCE_HANDLE                 hInstance          /**< inout: instance handle of Temporal Bandwidth Extension */
);

/*************************************************************************
TBE reset for TCX transistion
**************************************************************************/
int CDK_TBEDecLibReset(
  CDK_TBEDEC_INSTANCE_HANDLE                 hInstance          /**< inout: instance handle of Temporal Bandwidth Extension */
);

/*************************************************************************
free instance of this module
**************************************************************************/
void CDK_TBEDecLibDelete(CDK_TBEDEC_INSTANCE_HANDLE *phInstance);

void CDK_TBEDecLibAssignTBEParams(
              CDK_TBEDEC_INSTANCE_HANDLE      hInstance,          /**< inout: instance handle */
              TBE_DATA_HANDLE                 tbe_data);






#endif  /* __CDK_TBEDECLIB_H_ */
