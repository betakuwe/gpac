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

#ifndef __CDK_TBEComLib_H_

#define __CDK_TBEComLib_H_

#include "CDK_core.h"
#include "common_fix.h"
#include "CDK_lpc.h"


#define TBE_NL_CONFIG_REFSOFT_ALIGNED /* aligns initialization of NL mode to refSoft/spec */

#define TBE_OVERHAD_FIX


/* Data type for downmix matrix coefficients */
#define FIXP_TBE_BITS     32
#if (FIXP_TBE_BITS == 32)
#define FIXP_TBE              FIXP_DBL
#define FL2FXCONST_TBE(val)   FL2FXCONST_DBL(val)
#define FL2FX_TBE(val)        FL2FX_DBL(val)
#define FX_TBE2FL(val)        FX_DBL2FL(val)
#define FX_DBL2FX_TBE(x)      (x)
#define FX_TBE2FX_DBL(x)      (x)
#define MAXVAL_TBE            MAXVAL_DBL
#define MINVAL_TBE            MINVAL_DBL
#else
#define FIXP_TBE              FIXP_SGL
#define FL2FXCONST_TBE(val)   FL2FXCONST_SGL(val)
#define FL2FX_TBE(val)        FL2FX_SGL(val)
#define FX_TBE2FL(val)        FX_SGL2FL(val)
#define FX_DBL2FX_TBE(x)      FX_DBL2FX_SGL(x)
#define FX_TBE2FX_DBL(x)      FX_SGL2FX_DBL(x)
#define MAXVAL_TBE            MAXVAL_SGL
#endif





/*************************************************************************

**************************************************************************/
void CDK_TBEComLibAutocorr(
    const FIXP_TBE *signal_m, /* i  : input signal                            */
    const INT signal_e,       /* i  : exponent of input signal                */
          FIXP_TBE *r_m,      /* o  : autocorrelations vector (mantissa)      */
          INT *r_e,           /* o  : autocorrelation exponent (single value) */
    const UCHAR m,            /* i  : order of LP filter                      */
    const INT len,            /* i  : window size                             */
    const FIXP_DBL *wind      /* i  : window                                  */
);

/*************************************************************************

**************************************************************************/
void CDK_TBEComLibNonLinearity(
          FIXP_TBE *input,                      /* i  : input signal               */
    const INT       input_exp,                  /* i  : input signal's exponent    */
          FIXP_TBE *output,                     /* o  : output signal              */
          INT *output_exp,                      /* o  : exponent of output signal  */
          FIXP_TBE *old_bwe_exc_extended,       /* i/o: memory buffer              */
          INT      *old_bwe_exc_extended_exp,   /* i/o: memory buffer's exponent   */
    const UINT length,                          /* i  : input length               */
#ifdef TBE_NL_CONFIG_REFSOFT_ALIGNED
          UCHAR idxSubGains,                    /* i  : subgain idx        */
          UCHAR idxMixConfig,                   /* i  : mix idx            */
          UCHAR heMode,                         /* i  : hi-efficiency mode */
#endif
    const UCHAR tbe_nlConfig                    /* i  : NL mode                    */
);

/*************************************************************************

**************************************************************************/
void CDK_TBEComLibSpaceLsfs(
          FIXP_TBE *lsfs,        /* i/o: Line spectral frequencies   */
          INT      *lsfs_e,      /* i/o: Line spectral frequencies   */
    const UINT order             /* i  : order of LP analysis        */
);

/*************************************************************************

**************************************************************************/

FIXP_TBE CDK_TBEComLibSum2(                                         /* o  : sum of all squared vector elements   */
              const FIXP_TBE                   *vec,                /* i  :  input vector                        */
              const INT                         lvec,               /* i  :  length of input vector              */
              INT                              *exponent            /* o  : exponent of result                   */
);

FIXP_TBE CDK_TBEComLibSum2(                                         /* o  : sum of all squared vector elements   */
              const FIXP_TBE                   *vec,                /* i  :  input vector                        */
                    INT                         vec_e,              /* i  :  input vector's exponent             */
              const INT                         lvec,               /* i  :  length of input vector              */
                    INT                        *exponent            /* o  : exponent of result                   */
);


/*************************************************************************
     temporary wrapper for CLpc_AutoToLpc BLD: delete this wrapper later
**************************************************************************/
void CDK_TBEComLibLevDur(
        FIXP_TBE *lpcCoeff_m,
        INT *lpcCoeff_e,
        const FIXP_DBL *R,
        const INT R_e,
        const int m
);


/*************************************************************************

**************************************************************************/
void CDK_TBEComLibGenShbExc(
  FIXP_TBE *exc16kWhtnd,            /* i : Harmonic Exc mantissa             */
  INT       exc16kWhtnd_e,          /* i : Harmonic Exc exponent             */
  FIXP_TBE *White_exc16k,           /* i : Random Exc with exponent 15       */
  INT       White_exc16k_e,         /* i : Random Exc exponent               */
  const FIXP_TBE *voice_factors,    /* i : voice factors                     */
  const FIXP_TBE pow1_m,            /* i : power of Harmonic Exc             */
  const INT pow1_e,                 /* i : exponent of power of Harmonic Exc */
  const FIXP_TBE pow2_m,            /* i : Power of Random Exc               */
  const INT pow2_e,                 /* i : exponent of Power of Random Exc   */
  FIXP_TBE *excTBE_m,               /* o : TBE Excitation                    */
  INT *excTBE_e                     /* o : exponent of TBE Excitation        */
  );


/*************************************************************************
    Mean of the vector elements
**************************************************************************/
FIXP_DBL CDK_TBEComLibMean_4(     /* o  : Mean of vector with exponent 0 */
    const FIXP_DBL *vec           /* i  : input vector                   */
);

/*************************************************************************
    Line spectral frequencies conversion to line spectral pairs
**************************************************************************/
void CDK_TBEComLibLsf2lsp(
    const FIXP_DBL *lsf,    /* i  : lsf[m]   normalized (range: 0<=val<=fs/2)  */
    const INT *lsf_e,       /* i  : lsf_e[m] exponents corresponding to lsf[m] */
    FIXP_DBL *lsp,          /* o  : lsp[m]   (range: -1<=val<1)                */
    const INT m             /* i  : LPC order                                  */
);

/*************************************************************************

**************************************************************************/
void CDK_TBEComLibInterpolateAllpassSteep(
  const FIXP_TBE *in,           /* i  : input array of size N       */
  FIXP_TBE *state,              /* i/o: memory                      */    /* assume states exponent = in exponent + 1 */
  const INT N,                  /* i  : number of input samples     */
  FIXP_TBE *out                 /* o  : output array of size 2*N    */
);

/*************************************************************************
    Line spectral pairs conversion to linear prediction coefficients
**************************************************************************/
void TBE_E_LPC_f_lsp_a_conversion(FIXP_DBL *lsp, FIXP_DBL *a);

/*************************************************************************
LP filter weighting -> inline?
Output filter coefficients ap have the same exponent as input LPC a
**************************************************************************/
void CDK_weight_a(
    const FIXP_TBE *a,     /* i  : LP filter coefficients          */
          FIXP_TBE *ap,    /* o  : weighted LP filter coefficients */
    const FIXP_DBL gamma,  /* i  : weighting factor                */
    const INT m            /* i  : order of LP filter              */
);

/*************************************************************************
Create a random vector
**************************************************************************/
void CDK_create_random_vector(
          FIXP_DBL *output,    /* o  : output random vector      */
    const INT length,          /* i  : length of random vector   */
          SHORT *seed          /* i/o: start seed                */
);

/*************************************************************************
Random seed generator
**************************************************************************/
SHORT CDK_own_random(         /* o  : output random value */
    SHORT *seed               /* i/o: random seed         */
);

/*************************************************************************
     CDK_TBEComLibGenNlExc
**************************************************************************/
void CDK_TBEComLibGenNlExc(
    const FIXP_TBE *bwe_exc_extended,                 /* i  : bandwidth extended excitation */
          FIXP_TBE *exc16k,                           /* o  : harmonic excitation output */
          FIXP_TBE *mem_genSHBexc                     /* i/o: memory */
);

/*************************************************************************
     CDK_TBEComLibFir
     this Version works without dynamic exponent.
     It works only with order of FIR filter lower than 16
     Output exponent is 4
**************************************************************************/
void CDK_TBEComLibFir(
          FIXP_TBE *x,    /* i/o: input/output vector                       */
    const FIXP_TBE *h,    /* i  : impulse response of the FIR filter        */
          FIXP_TBE *mem,  /* i/o: memory of the input signal (L samples)    */
    const UINT L,         /* i  : input vector size                         */
    const UINT K          /* i  : order of the FIR filter (K+1 coefs.)      */
);


void CDK_TBE_GenSHBSynth(
              const FIXP_TBE                        *input_synspeech,            /* i  : input synthesized speech    */
                    FIXP_TBE                        *shb_syn_speech_32k,         /* o  : output highband component with exponent one higher than input exponent  */
                    FIXP_TBE                        *state_lsyn_filt_shb_local   /* i/o: memory                      */
);

/*************************************************************************
    LPC Synthesis (final version)
**************************************************************************/
void CDK_TBEComLibSynFilt(
    const FIXP_TBE *a_m,            /* i  : LP filter coefficients mantissa            */
    const INT       a_e,            /* i  : LP filter coefficients exponent            */
    const FIXP_TBE *x_m,            /* i  : input signal mantissa (input needs to have 1 bit headroom) */
          FIXP_TBE *y,              /* o  : output signal                              */
          FIXP_TBE *mem,            /* i/o: initial filter states                      */
          INT    numSubFrames)      /* i  : number of consecutive subframes to process */
;


/*************************************************************************

**************************************************************************/
void CDK_TBEComLibGenNmodExc (
    const FIXP_TBE *exc16kWhtnd,     /* i   : Harmonic Exc [256] */
          INT       exc16kWhtnd_e,   /* i   : Harmonic Exc exponent */
    const FIXP_TBE *voice_factors,   /* i   : Voice Factors [4] */
          FIXP_TBE *mem_csfilt,      /* i/o : Exc Envelope memory */
          INT      *mem_csfilt_e,    /* i/o : Exc Envelope memory */
          SHORT    *bwe_seed,        /* i/o : Random Seed [2] */
    const FIXP_TBE *lpc_shb_sf,      /* i   : Subframe LPCs [44] with exponent TBE_SF_A_COEFFS (3) */
          FIXP_TBE *wn_ana_mem,      /* i/o : memory [10] */
          FIXP_TBE *wn_syn_mem,      /* i/o : memory [10] */
          FIXP_TBE *White_exc16k,    /* o   : Random Exc output [256] with an exponent of 13 + excNoisyEnv_e[0] * 2*LPC_e */
          INT      *White_exc16k_e   /* o   : Random Exc output exponent */
);

/*************************************************************************
 LPC synth
**************************************************************************/
void CDK_TBEComLibLpcSyn(
    FIXP_TBE *exc16kWhtnd_m,            /* i  : exc mantissa */
    INT *exc16kWhtnd_e,                 /* i/o: exc exponent */
    const FIXP_TBE *lpc_shb_sf_m,       /* i  : Subframe LPCs mantissa with exponent TBE_SF_A_COEFFS (3) */
    const FIXP_TBE hbEnerTarget,        /* i  : TBE frame energy mantissa */
    const INT hbEnerTarget_e,           /* i  : TBE frame energy exponent */
    const short tbe_heMode,             /* i  : TBE HE mode */
    const short tbe_hrConfig,           /* i  : tbe_hrConfig */
          FIXP_TBE *state_lpc_syn,      /* i/o: LPC synthesis memory */
          INT      *state_lpc_syn_e,    /* i/o: LPC synthesis memory exponent */
          FIXP_TBE *excSHB              /* o  : shb synthesis */
);

/*************************************************************************

**************************************************************************/
void CDK_TBEComLibMixPastExc(
          FIXP_TBE  bwe_exc[],
          INT       *error,
    const INT       T0,
    const INT       T0_frac
);

/*************************************************************************

**************************************************************************/
void CDK_TBE_ScaleShapedSHB(
              const unsigned short                    length,             /* i  : SHB overlap length                */
                    FIXP_TBE                         *synSHB_m,           /* i/o: synthesized shb signal mantissa   */
                    INT                              *synSHB_e,           /* i/o: synthesized shb signal exponent   */
                    FIXP_TBE                         *overlap,            /* i/o: buffer for overlap-add            */
                    INT                              *overlap_e,          /* i/o: buffer for overlap-add (exponent) */
              const FIXP_TBE                         *subgain,            /* i  : subframe gain                     */
              const FIXP_TBE                          frame_gain_m,       /* i  : frame gain mantissa               */
              const INT                               frame_gain_e,       /* i  : frame gain exponent               */
#ifndef TBE_OVERHAD_FIX
              const FIXP_TBE                         *win,                /* i  : window                            */
#endif
              const FIXP_TBE                         *subwin              /* i  : subframes window                  */
);

/*************************************************************************

**************************************************************************/
void CDK_TBEComLibPrepTBEExc(
    const short L_frame,            /* i  : length of the frame         */
    const short i_subfr,            /* i  : subframe index              */
    const FIXP_SGL gain_pit,        /* i  : Pitch gain, exponent SF_GAIN_P (1) */
    const FIXP_DBL gain_code,       /* i  : algebraic codebook gain, exponent = SF_GAIN_C (16) */
    const FIXP_SGL *code,           /* i  : algebraic excitation, exponent = SF_CODE (6) */
    const FIXP_DBL voice_fac,       /* i  : voicing factor              */
          FIXP_DBL *voice_factors,  /* o  : TBE voicing factor          */
          FIXP_DBL bwe_exc[]        /* i/o: excitation for TBE, exponent SF_EXC (16) */
);


#define CDK_TBE_L_FRAME_1024                          1024
#define CDK_TBE_L_DIV_1024                            256
#define CDK_TBE_L_SUBFR                               64
#define CDK_TBE_PIT16k_MAX                            434 /*Maximum pitch lag @24kHz; 289->Maximum pitch lag @16kHz*/
#define CDK_TBE_NL_BUFF_OFFSET                        12
#define CDK_TBE_NL_BUFF_OFFSET_MAX                    32
#define CDK_TBE_HIBND_ACB_L_FAC                       2 /* SHB Interpolation Factor */
#define CDK_TBE_NB_SUBFR                              4
#define CDK_TBE_L_FRAME_TBE                           CDK_TBE_L_DIV_1024
#define CDK_TBE_L_SUBFR_TBE                           (CDK_TBE_L_FRAME_TBE/CDK_TBE_NB_SUBFR)
#define CDK_TBE_ALLPASSSECTIONS_STEEP                 3                    /* Size of all pass filters for interpolation and decimation by a factor of 2 */
#ifdef TBE_OVERHAD_FIX
  #define CDK_TBE_L_SHB_LAHEAD                          16                   /* Size of lookahead for SHB */
#else
  #define CDK_TBE_L_SHB_LAHEAD                          20                   /* Size of lookahead for SHB */
#endif
#define CDK_TBE_NUM_SHB_SUBFR                         16
#define CDK_TBE_LPC_SHB_ORDER                         10
#define CDK_TBE_LPC_WHTN_ORDER                        4                    /* Order of whitening filter for SHB excitation */
#define CDK_TBE_SHB_OVERLAP_LEN                       16
#define CDK_TBE_RESP_ORDER                            15
#define CDK_TBE_NL_HARMONIC                           0
#define CDK_TBE_NL_SMOOTH                             1
#define CDK_TBE_NL_HYBRID                             2
#define CDK_TBE_TBE_DEC_DELAY                         (CDK_TBE_L_SHB_LAHEAD + (CDK_TBE_NL_BUFF_OFFSET/2))
#define CDK_TBE_NUM_SHB_SUBGAINS                      4
#define CDK_TBE_NUM_STAGES_LSF                        2
#define CDK_TBE_MAX_LEN_LP                            CDK_TBE_L_FRAME_1024
#define CDK_TBE_SPC                                   FL2FXCONST_TBE(32*0.0234952) /* exp = -5 */
#define CDK_TBE_TD_RS_DELAY                           30


/* ######################################################################*/
/* ########################### module  data  ############################*/
/* ######################################################################*/
/* All pass filter coeffs for interpolation and decimation by a factor of 2 */
static const FIXP_TBE CDK_AP1_STEEP[CDK_TBE_ALLPASSSECTIONS_STEEP] = { FL2FXCONST_TBE(0.06056541924291), FL2FXCONST_TBE(0.42943401549235), FL2FXCONST_TBE(0.80873048306552) };
static const FIXP_TBE CDK_AP2_STEEP[CDK_TBE_ALLPASSSECTIONS_STEEP] = { FL2FXCONST_TBE(0.22063024829630), FL2FXCONST_TBE(0.63593943961708), FL2FXCONST_TBE(0.94151583095682) };
static const FIXP_TBE CDK_interpol_frac_shb[CDK_TBE_NB_SUBFR*2] = { FL2FXCONST_TBE(0.7), FL2FXCONST_TBE(0.3), FL2FXCONST_TBE(0.5), FL2FXCONST_TBE(0.5), FL2FXCONST_TBE(0.3), FL2FXCONST_TBE(0.7), FL2FXCONST_TBE(0.1), FL2FXCONST_TBE(0.9) };

/* ######################################################################*/
/* Common data */
/* ######################################################################*/
#ifndef TBE_OVERHAD_FIX
static const FIXP_TBE CDK_window_shb[CDK_TBE_L_SHB_LAHEAD]= 
{ FL2FXCONST_TBE(4.618346E-02),  FL2FXCONST_TBE(1.381564E-01),  FL2FXCONST_TBE(2.289505E-01),  FL2FXCONST_TBE(3.177914E-01),  FL2FXCONST_TBE(4.039210E-01),  FL2FXCONST_TBE(4.866045E-01),  FL2FXCONST_TBE(5.651364E-01),  FL2FXCONST_TBE(6.388468E-01),  FL2FXCONST_TBE(7.071068E-01),  FL2FXCONST_TBE(7.693340E-01),
  FL2FXCONST_TBE(8.249975E-01),  FL2FXCONST_TBE(8.736224E-01),  FL2FXCONST_TBE(9.147939E-01),  FL2FXCONST_TBE(9.481606E-01),  FL2FXCONST_TBE(9.734381E-01),  FL2FXCONST_TBE(9.904104E-01),  FL2FXCONST_TBE(9.989330E-01),  (FIXP_TBE)MAXVAL_TBE, (FIXP_TBE)MAXVAL_TBE, (FIXP_TBE)MAXVAL_TBE};
#endif

static const FIXP_TBE CDK_subwin_shb[CDK_TBE_SHB_OVERLAP_LEN+1]=
{FL2FXCONST_TBE(0.000000000), FL2FXCONST_TBE(0.007312931), FL2FXCONST_TBE(0.033416940), FL2FXCONST_TBE(0.077119580), FL2FXCONST_TBE(0.136556101), FL2FXCONST_TBE(0.209438491), FL2FXCONST_TBE(0.293364150), FL2FXCONST_TBE(0.385224703),
 FL2FXCONST_TBE(0.481317588), FL2FXCONST_TBE(0.577845599), FL2FXCONST_TBE(0.671497772), FL2FXCONST_TBE(0.758927143), FL2FXCONST_TBE(0.836651580), FL2FXCONST_TBE(0.901448444), FL2FXCONST_TBE(0.951083203), FL2FXCONST_TBE(0.984171147),
 (FIXP_TBE)MAXVAL_TBE};

static const FIXP_TBE CDK_gaus_dico_swb[256] =
{
FL2FXCONST_TBE(0.02164473),  FL2FXCONST_TBE(0.35885197),  FL2FXCONST_TBE(-0.16274954), FL2FXCONST_TBE(-0.08241354), FL2FXCONST_TBE(0.07313631),  FL2FXCONST_TBE(-0.00054929), FL2FXCONST_TBE(-0.13080014), FL2FXCONST_TBE(0.07226136),
FL2FXCONST_TBE(-0.13965981), FL2FXCONST_TBE(-0.04834007), FL2FXCONST_TBE(-0.02745908), FL2FXCONST_TBE(-0.02867859), FL2FXCONST_TBE(0.11216793),  FL2FXCONST_TBE(0.16604294),  FL2FXCONST_TBE(-0.00134274), FL2FXCONST_TBE(0.06818508),
FL2FXCONST_TBE(-0.17387933), FL2FXCONST_TBE(0.09406016),  FL2FXCONST_TBE(-0.08150196), FL2FXCONST_TBE(0.05083200),  FL2FXCONST_TBE(-0.01952806), FL2FXCONST_TBE(-0.10203217), FL2FXCONST_TBE(-0.03067050), FL2FXCONST_TBE(-0.05153965),
FL2FXCONST_TBE(0.06250680),  FL2FXCONST_TBE(0.00859049),  FL2FXCONST_TBE(-0.12008808), FL2FXCONST_TBE(-0.11361376), FL2FXCONST_TBE(0.17176038),  FL2FXCONST_TBE(0.01174004),  FL2FXCONST_TBE(-0.02275130), FL2FXCONST_TBE(-0.09895785),
FL2FXCONST_TBE(-0.10167463), FL2FXCONST_TBE(-0.22059087), FL2FXCONST_TBE(-0.05334539), FL2FXCONST_TBE(-0.00629700), FL2FXCONST_TBE(-0.16706355), FL2FXCONST_TBE(0.07795000),  FL2FXCONST_TBE(0.08731710),  FL2FXCONST_TBE(0.09669208),
FL2FXCONST_TBE(0.15378080),  FL2FXCONST_TBE(0.01794813),  FL2FXCONST_TBE(-0.01549965), FL2FXCONST_TBE(-0.24923822), FL2FXCONST_TBE(0.19985947),  FL2FXCONST_TBE(-0.10477958), FL2FXCONST_TBE(0.06674605),  FL2FXCONST_TBE(-0.11186616),
FL2FXCONST_TBE(-0.17927034), FL2FXCONST_TBE(0.08443811),  FL2FXCONST_TBE(0.25542912),  FL2FXCONST_TBE(0.03167623),  FL2FXCONST_TBE(0.19633667),  FL2FXCONST_TBE(0.19163096),  FL2FXCONST_TBE(0.01907267),  FL2FXCONST_TBE(0.12298489),
FL2FXCONST_TBE(-0.03147158), FL2FXCONST_TBE(0.05562247),  FL2FXCONST_TBE(0.30200079),  FL2FXCONST_TBE(-0.04257871), FL2FXCONST_TBE(0.08275045),  FL2FXCONST_TBE(-0.03386311), FL2FXCONST_TBE(-0.02265750), FL2FXCONST_TBE(0.18742503),
FL2FXCONST_TBE(-0.13598505), FL2FXCONST_TBE(-0.32004824), FL2FXCONST_TBE(-0.00438390), FL2FXCONST_TBE(-0.15576170), FL2FXCONST_TBE(0.06006401),  FL2FXCONST_TBE(-0.00952147), FL2FXCONST_TBE(0.18848655),  FL2FXCONST_TBE(0.06630960),
FL2FXCONST_TBE(0.07121546),  FL2FXCONST_TBE(-0.00733249), FL2FXCONST_TBE(0.08277771),  FL2FXCONST_TBE(0.22764891),  FL2FXCONST_TBE(0.06772452),  FL2FXCONST_TBE(-0.09509693), FL2FXCONST_TBE(-0.00172236), FL2FXCONST_TBE(0.08452052),
FL2FXCONST_TBE(0.17020901),  FL2FXCONST_TBE(-0.03737585), FL2FXCONST_TBE(0.02349647),  FL2FXCONST_TBE(0.10855560),  FL2FXCONST_TBE(0.06854416),  FL2FXCONST_TBE(0.07084806),  FL2FXCONST_TBE(0.09390105),  FL2FXCONST_TBE(0.00124924),
FL2FXCONST_TBE(0.03026483),  FL2FXCONST_TBE(-0.15169589), FL2FXCONST_TBE(0.01347072),  FL2FXCONST_TBE(-0.15377805), FL2FXCONST_TBE(0.14992996),  FL2FXCONST_TBE(0.11630810),  FL2FXCONST_TBE(0.03483583),  FL2FXCONST_TBE(-0.03914850),
FL2FXCONST_TBE(-0.20075595), FL2FXCONST_TBE(0.12728901),  FL2FXCONST_TBE(-0.04495851), FL2FXCONST_TBE(-0.11576717), FL2FXCONST_TBE(-0.15281813), FL2FXCONST_TBE(0.06055827),  FL2FXCONST_TBE(-0.03471978), FL2FXCONST_TBE(-0.03617816),
FL2FXCONST_TBE(0.17230885),  FL2FXCONST_TBE(0.03094525),  FL2FXCONST_TBE(-0.15618153), FL2FXCONST_TBE(0.21792564),  FL2FXCONST_TBE(0.08106838),  FL2FXCONST_TBE(-0.22098514), FL2FXCONST_TBE(-0.10796417), FL2FXCONST_TBE(0.07131225),
FL2FXCONST_TBE(0.22092983),  FL2FXCONST_TBE(-0.01539366), FL2FXCONST_TBE(-0.02876964), FL2FXCONST_TBE(-0.30910203), FL2FXCONST_TBE(0.02143815),  FL2FXCONST_TBE(-0.11630868), FL2FXCONST_TBE(-0.00922897), FL2FXCONST_TBE(0.07431208),
FL2FXCONST_TBE(0.15533504),  FL2FXCONST_TBE(0.11425125),  FL2FXCONST_TBE(0.07125455),  FL2FXCONST_TBE(-0.11914105), FL2FXCONST_TBE(-0.04275274), FL2FXCONST_TBE(-0.05072749), FL2FXCONST_TBE(-0.22143129), FL2FXCONST_TBE(0.19787727),
FL2FXCONST_TBE(-0.20946717), FL2FXCONST_TBE(-0.16564523), FL2FXCONST_TBE(0.05962536),  FL2FXCONST_TBE(-0.22325630), FL2FXCONST_TBE(-0.04333350), FL2FXCONST_TBE(-0.04707248), FL2FXCONST_TBE(0.16608582),  FL2FXCONST_TBE(0.00948954),
FL2FXCONST_TBE(0.11283893),  FL2FXCONST_TBE(-0.04097161), FL2FXCONST_TBE(-0.09076904), FL2FXCONST_TBE(0.26722300),  FL2FXCONST_TBE(0.00987607),  FL2FXCONST_TBE(-0.05807892), FL2FXCONST_TBE(0.07872546),  FL2FXCONST_TBE(0.08040629),
FL2FXCONST_TBE(0.12927419),  FL2FXCONST_TBE(-0.05647410), FL2FXCONST_TBE(0.09603068),  FL2FXCONST_TBE(-0.02356448), FL2FXCONST_TBE(-0.02160797), FL2FXCONST_TBE(-0.11687102), FL2FXCONST_TBE(0.07936122),  FL2FXCONST_TBE(-0.05764586),
FL2FXCONST_TBE(-0.10510305), FL2FXCONST_TBE(-0.02326054), FL2FXCONST_TBE(0.12021790),  FL2FXCONST_TBE(0.09782617),  FL2FXCONST_TBE(-0.22600858), FL2FXCONST_TBE(-0.02555378), FL2FXCONST_TBE(-0.03561033), FL2FXCONST_TBE(-0.01337216),
FL2FXCONST_TBE(0.11311363),  FL2FXCONST_TBE(-0.03096960), FL2FXCONST_TBE(-0.22801498), FL2FXCONST_TBE(0.05643769),  FL2FXCONST_TBE(0.13053033),  FL2FXCONST_TBE(0.04452197),  FL2FXCONST_TBE(-0.09299882), FL2FXCONST_TBE(-0.11475921),
FL2FXCONST_TBE(0.02257649),  FL2FXCONST_TBE(-0.21770498), FL2FXCONST_TBE(-0.11454470), FL2FXCONST_TBE(-0.09435777), FL2FXCONST_TBE(0.00638951),  FL2FXCONST_TBE(-0.36990553), FL2FXCONST_TBE(0.04266735),  FL2FXCONST_TBE(0.06915011),
FL2FXCONST_TBE(0.07644624),  FL2FXCONST_TBE(-0.24336053), FL2FXCONST_TBE(-0.03421960), FL2FXCONST_TBE(-0.10622191), FL2FXCONST_TBE(-0.17223521), FL2FXCONST_TBE(0.04054553),  FL2FXCONST_TBE(0.13831380),  FL2FXCONST_TBE(0.02925055),
FL2FXCONST_TBE(0.16207848),  FL2FXCONST_TBE(-0.12994884), FL2FXCONST_TBE(-0.09751288), FL2FXCONST_TBE(-0.05397306), FL2FXCONST_TBE(-0.09323815), FL2FXCONST_TBE(0.13425350),  FL2FXCONST_TBE(-0.00046960), FL2FXCONST_TBE(0.31072289),
FL2FXCONST_TBE(0.13740718),  FL2FXCONST_TBE(0.05835414),  FL2FXCONST_TBE(-0.04803475), FL2FXCONST_TBE(0.15423043),  FL2FXCONST_TBE(-0.09652353), FL2FXCONST_TBE(0.14896898),  FL2FXCONST_TBE(-0.16368309), FL2FXCONST_TBE(0.05875925),
FL2FXCONST_TBE(-0.03678078), FL2FXCONST_TBE(-0.19627908), FL2FXCONST_TBE(0.07034992),  FL2FXCONST_TBE(-0.27213186), FL2FXCONST_TBE(-0.04338680), FL2FXCONST_TBE(0.01567988),  FL2FXCONST_TBE(-0.09158870), FL2FXCONST_TBE(0.11987700),
FL2FXCONST_TBE(0.07083926),  FL2FXCONST_TBE(0.01099900),  FL2FXCONST_TBE(-0.01084446), FL2FXCONST_TBE(0.04508050),  FL2FXCONST_TBE(-0.10655984), FL2FXCONST_TBE(-0.13945042), FL2FXCONST_TBE(0.05837287),  FL2FXCONST_TBE(0.08458713),
FL2FXCONST_TBE(-0.04212087), FL2FXCONST_TBE(-0.15749574), FL2FXCONST_TBE(0.11632511),  FL2FXCONST_TBE(0.07976698),  FL2FXCONST_TBE(0.06725866),  FL2FXCONST_TBE(-0.09567240), FL2FXCONST_TBE(0.03796997),  FL2FXCONST_TBE(-0.09355708),
FL2FXCONST_TBE(-0.13569611), FL2FXCONST_TBE(-0.19498724), FL2FXCONST_TBE(0.14951572),  FL2FXCONST_TBE(-0.16023041), FL2FXCONST_TBE(0.04185898),  FL2FXCONST_TBE(0.06099325),  FL2FXCONST_TBE(0.03425207),  FL2FXCONST_TBE(0.16211477),
FL2FXCONST_TBE(0.03998571),  FL2FXCONST_TBE(-0.03629408), FL2FXCONST_TBE(-0.10099959), FL2FXCONST_TBE(0.19540504),  FL2FXCONST_TBE(0.11653102),  FL2FXCONST_TBE(0.23601755),  FL2FXCONST_TBE(0.04943547),  FL2FXCONST_TBE(-0.26040605),
FL2FXCONST_TBE(0.02153429),  FL2FXCONST_TBE(0.22880882),  FL2FXCONST_TBE(-0.13646534), FL2FXCONST_TBE(0.03881640),  FL2FXCONST_TBE(-0.02896636), FL2FXCONST_TBE(0.09774253),  FL2FXCONST_TBE(-0.13509314), FL2FXCONST_TBE(-0.08713179),
FL2FXCONST_TBE(0.13485038),  FL2FXCONST_TBE(0.06968338),  FL2FXCONST_TBE(0.19561967),  FL2FXCONST_TBE(0.07884958),  FL2FXCONST_TBE(-0.10365590), FL2FXCONST_TBE(-0.10321335), FL2FXCONST_TBE(-0.09081125), FL2FXCONST_TBE(-0.00147976)
};



#endif /* __CDK_TBEComLib_H_ */


