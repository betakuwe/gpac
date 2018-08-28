

#ifndef __IIS_FORMATCONVERTER_INTERN_H__
#define __IIS_FORMATCONVERTER_INTERN_H__

#include "global_functional_range.h"


#include "CDK_cicp2geometry.h"
#include "CDK_formatConverterLib.h"
#include "CDK_stftfilterbank_api.h"
#include "CDK_tools_rom.h"

/* MPEG-H profile level 3: MAX_INPUT_CHANNELS = 16, MAX_OUTPUT_CHANNELS = 12 */
#if (CDK_GFR_CHANNEL_CONFIG == CDK_GFR_CH_MPEGH_L3)
#define CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS (16)
#define CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS       (16)
#define CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS      (12)
#elif (CDK_GFR_CHANNEL_CONFIG == CDK_GFR_CH_MPEGH_L4)
#define CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS (28)
#define CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS       (28)
#define CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS      (24)
#else
#define CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS (32)
#define CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS       (24)
#define CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS      (24)
#endif

/**
 * Converter parameters
 */
#define     IN_OUT_N        (60)
#define     MAXBANDS        (58)
#define     NCHANOUT_MAX    (CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS)
#define     NCHANIN_MAX     (CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS)
#define     CDK_FORMAT_CONVERTER_MAX_LFE (2)

/* Downmix Module data types and definitions */

/* Data type for downmix matrix coefficients */
#define FIXP_DMX_H_BITS     16
#if (FIXP_DMX_H_BITS == 32)
#define FIXP_DMX_H              FIXP_DBL
#define FL2FXCONST_DMX_H(val)   FL2FXCONST_DBL(val)
#define FX_DBL2FX_DMX_H(x)      (x)
#define MAXVAL_DMX_H            MAXVAL_DBL
#define MINVAL_DMX_H            MINVAL_DBL
#else
#define FIXP_DMX_H              FIXP_SGL
#define FL2FXCONST_DMX_H(val)   FL2FXCONST_SGL(val)
#define FL2FX_DMX_H(val)        FL2FX_SGL(val)
#define FX_DBL2FX_DMX_H(x)      FX_DBL2FX_SGL(x)
#define FX_DMX_H2FX_DBL(x)      FX_SGL2FX_DBL(x)
#define MAXVAL_DMX_H            MAXVAL_SGL
#define MINVAL_DMX_H            MINVAL_SGL
#endif

/* Data type for equalizer coefficients */
#define FIXP_EQ_H_BITS     32
#if (FIXP_EQ_H_BITS == 32)
#define FIXP_0_dot_5           0x40000000
#define FIXP_0_dot_25          0x20000000
#define FIXP_EQ_H              FIXP_DBL
#define FL2FXCONST_EQ_H(val)   FL2FXCONST_DBL(val)
#define FX_DBL2FX_EQ_H(x)      (x)
#define MAXVAL_EQ_H            MAXVAL_DBL
#define MINVAL_EQ_H            MINVAL_DBL
#else
#define FIXP_0_dot_5           0x4000
#define FIXP_0_dot_25          0x2000
#define FIXP_EQ_H              FIXP_SGL
#define FL2FXCONST_EQ_H(val)   FL2FXCONST_SGL(val)
#define FL2FX_EQ_H(val)        FL2FX_SGL(val)
#define FX_DBL2FX_EQ_H(x)      FX_DBL2FX_SGL(x)
#define MAXVAL_EQ_H            MAXVAL_SGL
#define MINVAL_EQ_H            MINVAL_SGL
#endif

#define EQ_H_EXP                           1             /* default EQs exponent */
#define FIXP_EQ_H_FORMAT_1_dot_0           FIXP_EQ_H(FIXP_0_dot_5)
#define EQ_BITSTREAM_H_EXP                 2             /* bitstream parsed EQs exponent */
#define FIXP_EQ_BITSTREAM_H_FORMAT_1_dot_0 FIXP_EQ_H(FIXP_0_dot_25)

/* Data type for downmix input/output data */
#define DMXH_PCM            FIXP_DBL
#define DMXH_PCMF           FIXP_DBL
#define DMXH_HEADROOM       8 /* PCM output headroom, must have the same values as PCM_OUT_HEADROOM */
#define FX_DBL2FX_DMXH(x)   (x)


#define DMX_MAX_IN_CHANNELS  24
#define DMX_MAX_OUT_CHANNELS 16

/*    
 *    Flag definition for function CDK_dmx:
 *    Bit 0: Indicates frame start, if set, otherwise frame continuation
 *           Note: this flag triggers, if set, a clearing of the output buffer (pOut[0..noOutChannels*frameSize-1])
 */
#define DMX_FLAG_START               (1<<0)

#ifndef MPEGH_DMX
#define MPEGH_DMX

#define FX_PI         FL2FXCONST_DBL(M_PI/4)
#define FIX_ANGLE(x) (x/360)

#ifndef min
#define min(a,b) (( (a) < (b) ) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (( (a) > (b) ) ? (a) : (b))
#endif

#endif



/** Handle to Format Converter parameter struct.
    Holds Format Converter parameter data. */
typedef struct T_FORMAT_CONVERTER_PARAMS    *HANDLE_FORMAT_CONVERTER_PARAMS;

/** Handle to Format Converter state struct.
    Holds Format Converter state data. */
typedef struct T_FORMAT_CONVERTER_STATE    *HANDLE_FORMAT_CONVERTER_STATE;




/* downmix processing rules */

typedef enum
{
    /* external rules (indices 0..10) */
    RULE_NOPROC   = 0,  /* no processing channel copy with gain */
    RULE_EQ1      = 1,  /* eq for front up-median downmix */
    RULE_EQ2      = 2,  /* eq for surround up-median downmix */
    RULE_EQ3      = 3,  /* eq for top-up downmix */
    RULE_EQ4      = 4,  /* eq for top-median downmix */
    RULE_EQ5      = 5,  /* eq for horizontal channel that is displaced to height */    
#ifdef CDK_IMMERSIVE_FORMAT_CONVERTER
    /* New equalizers */
    RULE_EQVF   = 7,  /* eq for CH_U_L030, CH_U_R030, CH_U_L045, CH_U_R045 */
    RULE_EQVB   = 8,  /* eq for CH_U_L135, CH_U_R135 */
    RULE_EQVFC = 9,  /* eq for CH_U_000 */
    RULE_EQVBC = 10, /* eq for CH_U_180 */
    RULE_EQVOG = 11,  /* eq for CH_T_000 */
    RULE_EQVS  = 12, /* eq for CH_U_L090, CH_U_R110 */
    RULE_EQBTM = 13, /* eq for CH_L_000, CH_L_L045, CH_L_R045 */
    RULE_EQVBA = 14, /* eq for CH_U_L110, CH_U_R110 */

    RULE_06_03 = 15, /* eq for CH_M_L060 and CH_M_R060 to CH_M_L030 and CH_M_R030 */
    RULE_09_03 = 16, /* eq for CH_M_L090 and CH_M_R090 to CH_M_L030 and CH_M_R030 */
    RULE_06_11 = 17, /* eq for CH_M_L060 and CH_M_R060 to CH_M_L110 and CH_M_R110 */
    RULE_09_11 = 18, /* eq for CH_M_L090 and CH_M_R090 to CH_M_L110 and CH_M_R110 */
    RULE_13_11 = 19, /* eq for CH_M_L135 and CH_M_R135 to CH_M_L110 and CH_M_R110 */
    RULE_18_11 = 20, /* eq for CH_M_180          to CH_M_L110 and CH_M_R110 */
    RULE_REQ   = 21, /* first eq for random setups */
    N_EQ       = 40, /* number of eqs, the value of N_EQ is 40 in case of randomization, otherwise 20 */
    /* rules only used within converter only */
    RULE_PANNING  = 100, /* manual amplitude panning (specifying alpha0, alpha) */
    RULE_TOP2ALLU = 101, /* top channel to all upper channels */
    RULE_TOP2ALLM = 102, /* top channel to all horizontal channels */
    RULE_AUTOPAN  = 103, /* automatic amplitude panning (alpha0 defined by destimation */
    RULE_VIRTUAL  = 104
#else
    RULE_REQ      = 6,   /* first eq for random setups */
    N_EQ          = 12,  /* number of eqs */
    /* rules only used within converter only */
    RULE_PANNING  = 100, /* manual amplitude panning (specifying alpha0, alpha) */
    RULE_TOP2ALLU = 101, /* top channel to all upper channels */
    RULE_TOP2ALLM = 102, /* top channel to all horizontal channels */
    RULE_AUTOPAN  = 103  /* automatic amplitude panning (alpha0 defined by destimation */
#endif
                         /* channels, alpha defined by source channels) */
} converter_dmxrulesid_t;



typedef struct converter_pr
{
    UINT        nchanin;                    /* number of input channels */
    UINT        nchanout;                   /* number of output channels */
    INT         in_out_src[IN_OUT_N];       /* in-out conversion source */
    INT         in_out_dst[IN_OUT_N];       /* in-out conversion destination */
    FIXP_DMX_H  in_out_gain[IN_OUT_N];      /* in-out conversion gain [lin] */
    INT         in_out_proc[IN_OUT_N];      /* in-out conversion processing */

    FIXP_EQ_H   eq[N_EQ][MAXBANDS];         /* equalizers [lin gains] */ /* TODO dynamic allocation */
    INT         eq_e[N_EQ];                 /* equalizers exp term [lin gains] */ /* TODO dynamic allocation */

#ifdef CDK_IMMERSIVE_FORMAT_CONVERTER
    FIXP_DMX_H  in_out_gainL[IN_OUT_N];     /* in-out conversion gain, G_vL [lin] */

    /* variables for the timbral elevation rendering */
    INT         in_out_src2[IN_OUT_N];      /* in-out conversion source */
    INT         in_out_dst2[IN_OUT_N];      /* in-out conversion destination */
    FIXP_DMX_H  in_out_gain2[IN_OUT_N];     /* in-out conversion gain [lin] */
    INT         in_out_proc2[IN_OUT_N];     /* in-out conversion processing */
    FIXP_DMX_H  in_out_gaini[IN_OUT_N];     /* in-out mixing gain [lin] */
#endif

} converter_pr_t;


#ifdef CDK_IMMERSIVE_FORMAT_CONVERTER
/* INDEX : FOR THE CONVENIENCE FOR ONLY HEIGHT CHANNELS */
typedef enum
{
  TFC=0,
  TFL,
  TFR,
  TFLA,
  TFRA,
  TSL,
  TSR,
  TBLA,
  TBRA,
  TBL,
  TBR,
  TBC,
  VOG
} H;

/* INDEX : FOR THE 5.1 CHANNEL SYSTEM */
typedef enum
{
  FL=0,
  FR,
  FC,
  SW,
  SL,
  SR
} C;

#endif

/*
IMPORTANT/WARNING:
    -the converter implementation assumes that all horizontal channels are:
        CH_M_000 <= i <= CH_M_180
    -the converter implementation assumes that all height channels are:
        CH_U_000 <= i <= CH_U_180
    Should this be changed, then converter.c needs to be modified!
*/

typedef enum
{
    /* horizontal channels */
    CH_M_000 =      0,
    CH_M_L022 =     1,
    CH_M_R022 =     2,
    CH_M_L030 =     3,
    CH_M_R030 =     4,
    CH_M_L045 =     5,
    CH_M_R045 =     6,
    CH_M_L060 =     7,
    CH_M_R060 =     8,
    CH_M_L090 =     9,
    CH_M_R090 =     10,
    CH_M_L110 =     11,
    CH_M_R110 =     12,
    CH_M_L135 =     13,
    CH_M_R135 =     14,
    CH_M_L150 =     15,
    CH_M_R150 =     16,
    CH_M_180 =      17,
    /* height channels */
    CH_U_000 =      18,
    CH_U_L045 =     19,
    CH_U_R045 =     20,
    CH_U_L030 =     21,
    CH_U_R030 =     22,
    CH_U_L090 =     23,
    CH_U_R090 =     24,
    CH_U_L110 =     25,
    CH_U_R110 =     26,
    CH_U_L135 =     27,
    CH_U_R135 =     28,
    CH_U_180 =      29,
    /* top channel */
    CH_T_000 =      30,
    /* low channels */
    CH_L_000 =      31,
    CH_L_L045 =     32,
    CH_L_R045 =     33,
    /* low frequency effects */
    CH_LFE1 =       34,
    CH_LFE2 =       35,
    CH_LFE3 =       36,
    /* SCR */
    CH_M_LSCR =     37,
    CH_M_RSCR =     38,
    /* empty channel */
    CH_EMPTY =      -1
} AFC_FORMAT_CONVERTER_CHANNEL_ID;

typedef enum
{
    CDK_FORMAT_CONVERTER_INPUT_FORMAT_INVALID      = 0,
    CDK_FORMAT_CONVERTER_INPUT_FORMAT_LISTOFCHANNELS,
    CDK_FORMAT_CONVERTER_INPUT_FORMAT_GENERIC
} CDK_FORMAT_CONVERTER_INPUT_FORMAT;

typedef enum
{
    CDK_FORMAT_CONVERTER_OUTPUT_FORMAT_INVALID      = 0,
    CDK_FORMAT_CONVERTER_OUTPUT_FORMAT_LISTOFCHANNELS,
    CDK_FORMAT_CONVERTER_OUTPUT_FORMAT_GENERIC
} CDK_FORMAT_CONVERTER_OUTPUT_FORMAT;

typedef struct T_FORMAT_CONVERTER_PARAMS
{
    
  /** Sampling rate. */    
  INT     samplingRate;

  /** Needs to be set, if the IIS_FORMATCONVERTER_MODE == IIS_FORMATCONVERTER_MODE_CUSTOM_FREQ_DOMAIN */
  FIXP_DBL   phaseAlignStrength;
  FIXP_DBL   adaptiveEQStrength;

  /** format converter input format of internal init structure */
  CDK_converter_formatid_t formatConverterInputFormat_internal;
  /** format converter output format of internal init structure */
  CDK_converter_formatid_t formatConverterOutputFormat_internal;
    
  /** handle to internal init structure*/
  converter_pr_t *formatConverterParams_internal;
  
  /** flag signalling generic input,output formats */    
  INT genericIOFmt;

  /** dmx matrix */
  FIXP_DMX_H **dmxMtx;
  FIXP_DMX_H *dmxMtx_sorted;
  /* equalizer index vector [in][out]*/
  INT **eqIndexVec;
  INT *eqIndexVec_sorted;

#ifdef CDK_IMMERSIVE_FORMAT_CONVERTER
  /** dmx matrix L */
  FIXP_DMX_H **dmxMtxL;
  FIXP_DMX_H *dmxMtxL_sorted;
  /** dmx matrix 2 */
  FIXP_DMX_H **dmxMtx2;
  FIXP_DMX_H *dmxMtx2_sorted;
  /* equalizer index vector 2 [in][out]*/
  INT **eqIndexVec2;
  INT *eqIndexVec2_sorted;

  /* Immersive mode control */
  INT immersiveMode;
  INT Mode3Drendering;
  INT Mode2Drendering;
  INT rendering3DTypeFlag_internal;

  FIXP_DMX_H *dmxMatrixL_CDK;
  FIXP_DMX_H *dmxMatrixH_CDK;
  INT *eqIndex_CDK;

  INT chOut_count[NCHANOUT_MAX];
  INT chOut_exp[NCHANOUT_MAX];
  INT dmx_iterations;

#endif


    /** flag signalling the DMX matrix coefficients have been set */
    INT dmxMtxIsSet;
    
    /** formatConverterDelay */
    INT formatConverterDelay;
    

    /** center frequencies */
    FIXP_DBL *centerFrequenciesNormalized;
    
    
    /** azimuthElevationDeviation */
    INT *azimuthElevationDeviation;

    /** distance*/
    INT *distance;
    
    
    /** eqLimitMax */
    FIXP_DBL eqLimitMax;    /* value could be nearly 16 at most -> exponent = 4 */
    
    /** eqLimitMin */
    FIXP_DBL eqLimitMin;    /* value could be -16 at least -> exponent = 4 */
    
    
    /** applyEqFilters */
    INT applyEqFilters;
    
    /* randomFlag */
    INT randomFlag;
    
} FORMAT_CONVERTER_PARAMS;

/**********************************************************************************************************************************/

typedef struct T_FORMAT_CONVERTER_STATE
{
    /** handle for params and states of active dmx in stft domain */
    void *handleActiveDmxStft;

} FORMAT_CONVERTER_STATE;


typedef struct 
{
  UINT                                  fcNumFreqBands;
  const FIXP_DBL*                       fcCenterFrequencies;

  /* Deprecated, use inputChannelGeo below and merge setups as soon as they are added. */
  CICP2GEOMETRY_CHANNEL_GEOMETRY        *inputSetups[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS];

  CICP2GEOMETRY_CHANNEL_GEOMETRY        inputChannelGeo[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS];
  
  UINT                                  numInputChannels[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS];
  UINT                                  inputChannelGroupOffsets[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS];
  UINT                                  numInputChannelGroups;

  UINT                                  dmxMatrixValid;
  UINT                                  amountOfAddedDmxMatricesAndEqualizers;
  /* EqualizerConfig variables. Parsed and decoded bitstream EQs. */
  FIXP_EQ_H                             **eqGains;
  INT                                   *eqGains_e;

  UINT                                  numTotalInputChannels;


  UINT                                  frameSize;
  UINT                                  numTimeSlots;
  UINT                                  samplingRate;

  UINT                                  immersiveDownmixFlag;
  UINT                                  rendering3DTypeFlag;

  /* Candidates */
  IIS_FORMATCONVERTER_MODE              mode;
  CDK_FORMAT_CONVERTER_INPUT_FORMAT     fcInputFormat;
  CDK_FORMAT_CONVERTER_OUTPUT_FORMAT    fcOutputFormat;
  INT                                   aes;
  INT                                   pas;
  UINT                                  numOutputChannels;
  CICP2GEOMETRY_CHANNEL_GEOMETRY        outChannelGeo[CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS];
  INT                                   cicpLayoutIndex;
#ifdef CDK_FC_MISPLACED_SPEAKER_ENABLE
  INT                                   lsAzimuthDeviation[CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS];
  INT                                   lsElevationDeviation[CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS];
#endif
  UINT                                  outChannelVbapFlag;
  UINT                                  inChannelVbapFlag;

  UINT                                  fcDelay;
  UINT                                  totalDelay;


  HANDLE_FORMAT_CONVERTER_PARAMS        fcParams;
  HANDLE_FORMAT_CONVERTER_STATE         fcState;

  /***************Buffers and handles for STFT processing *************/
  FIXP_DBL                              **inputBufferStft;    /*real, imag interleaved, N/2+1 coefficient stored in imaginary part of first coefficient-> stft of length N results in N interleaved coefficients*/
  FIXP_DBL                              **prevInputBufferStft;
  FIXP_DBL                              **outputBufferStft;
  HANDLE_STFT_FILTERBANK                stftFilterbankAnalysis[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNEL_GROUPS];
  HANDLE_STFT_FILTERBANK                stftFilterbankSynthesis[CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS];
 
  STFT_FILTERBANK_CONFIG                stftFilterbankConfigAnalysis;
  STFT_FILTERBANK_CONFIG                stftFilterbankConfigSynthesis;
  UINT                                  stftFrameSize;
  UINT                                  stftLength;
  UINT                                  stftNumErbBands;
  UINT                                  *stftErbFreqIdx;

  AFC_FORMAT_CONVERTER_CHANNEL_ID       *channelVec;
  int                                   *unknownChannelVec;

  int                                   inputChannelIds[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS];
  int                                   downmixMatrixChannelIds[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS];

  AFC_FORMAT_CONVERTER_CHANNEL_ID       format_in_listOfChannels[CDK_FORMAT_CONVERTER_MAX_INPUT_CHANNELS];
  int                                   format_in_listOfChannels_nchan;
  AFC_FORMAT_CONVERTER_CHANNEL_ID       format_out_listOfChannels[CDK_FORMAT_CONVERTER_MAX_OUTPUT_CHANNELS];
  int                                   format_out_listOfChannels_nchan;
  int                                   openSuccess;

  FIXP_DBL                              **deinBuffer;
  FIXP_DBL                              **deoutBuffer;

  FIXP_DBL        GVH[13][6];
  INT             GVH_e[13][6];
  FIXP_DBL        GVL[13][6];
  INT             GVL_e[13][6];

  UINT            is4GVH_StftErb[58];
  INT             topIn[13];
  UINT            midOut[6];
  INT             elv;
  INT             D2;

  UINT            erb_is4GVH_L;
  UINT            erb_is4GVH_H;

} IIS_FORMATCONVERTER_INTERNAL, *IIS_FORMATCONVERTER_INTERNAL_HANDLE;


#define MAX_CHANNELS 33
#define HOPSIZE 32
 
/* For memory allocation of temporary vectors that are used in the processing */
#define NUMTEMPVECTORS 4

#ifndef MAX_BANDS
#define MAX_BANDS 77
#endif

typedef struct {
    FIXP_DBL *re;
    FIXP_DBL *im;
} compexVector;

#define AFC_H_EPSILON         ((FIXP_DBL) 1)   /* Used as denumerator minimum instead of division by zero */


#endif /* _FORMATCONVERTER_INTERN_H__ */

