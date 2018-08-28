/******************************************************************************

                        (C) Copyright Fraunhofer IIS (2014)
                               All Rights Reserved

   $Id$

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/***********************************************************************************

 This software module was originally developed by

 Apple Inc.

 in the course of development of the ISO/IEC 23003-4 for reference purposes and its
 performance may not have been optimized. This software module is an implementation
 of one or more tools as specified by the ISO/IEC 23003-4 standard. ISO/IEC gives
 you a royalty-free, worldwide, non-exclusive, copyright license to copy, distribute,
 and make derivative works of this software module or modifications  thereof for use
 in implementations or products claiming conformance to the ISO/IEC 23003-4 standard
 and which satisfy any specified conformance criteria. Those intending to use this
 software module in products are advised that its use may infringe existing patents.
 ISO/IEC have no liability for use of this software module or modifications thereof.
 Copyright is not released for products that do not conform to the ISO/IEC 23003-4
 standard.

 Apple Inc. retains full right to modify and use the code for its own purpose,
 assign or donate the code to a third party and to inhibit third parties from using
 the code for products that do not conform to MPEG-related ITU Recommendations and/or
 ISO/IEC International Standards.

 This copyright notice must be included in all copies or derivative works.

 Copyright (c) ISO/IEC 2014.

 ***********************************************************************************/

#ifndef _UNI_DRC_COMMON_H_
#define _UNI_DRC_COMMON_H_

#include "common_fix.h"
#include "global_functional_range.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef CDK_GFR_MPEGD_USAC_UNIDRC
#error : inserted by coan: "#define DRCDEC_MPEG_D_DRC_ENABLE" contradicts -U or --implicit at /home/amm-er/hil/MPEG-H/b_eval_mpegh_2016-04-14_test/CDK_heaacdec/TelecomParisTech_MPEGH_eval_20171023/libDRCGainDec/include/uniDrcCommon.h(57)
#endif

#if (CDK_GFR_CHANNEL_CONFIG == CDK_GFR_CH_MPEGH_L3)
#define LC_PROFILE_LEVEL_3              1   /* reduce MPEG-H Low Complexity profile to level 3 */
#else
#define LC_PROFILE_LEVEL_3              0   /* support MPEG-H Low Complexity profile level 4 */
#endif
#define TARGET_LOUDNESS                 -24.0f
#define DRC_RM5_BS_SYNTAX

#define DEBUG_NODES                     0
#define DEBUG_BITSTREAM                 0   /* print bitstream bits written and read */
#define DEBUG_DRC_SELECTION             0   /* display information about selected DRC sets */
#define DEBUG_DRC_ERRORS                0   /* print warnings and errors */
#define DEBUG_STRUCTS                   0   /* print structs */

#if LC_PROFILE_LEVEL_3
//#define CHANNEL_COUNT_MAX               16
#define CHANNEL_COUNT_MAX               24
#define CHANNEL_COUNT_MAX_INCL_HOA      147 /* 3 signal groups with HOA order 6 (49 coefficient channels each) */
#define BAND_COUNT_MAX                  4
#define SEQUENCE_COUNT_MAX              32
#define GAIN_ELEMENT_COUNT_MAX          32
#define CHANNEL_GROUP_COUNT_MAX         16
#else
#define CHANNEL_COUNT_MAX               28
#define CHANNEL_COUNT_MAX_INCL_HOA      147 /* 3 signal groups with HOA order 6 (49 coefficient channels each) */
#define BAND_COUNT_MAX                  4
#define SEQUENCE_COUNT_MAX              48
#define GAIN_ELEMENT_COUNT_MAX          48
#define CHANNEL_GROUP_COUNT_MAX         28
#endif

#define MEASUREMENT_COUNT_MAX           16
#define DRC_COEFFICIENTS_COUNT_MAX      4
#define DRC_INSTRUCTIONS_COUNT_MAX      32
#define DOWNMIX_INSTRUCTION_COUNT_MAX   32
#define DRC_INSTRUCTIONS_COUNT_PLUS_MAX (DRC_INSTRUCTIONS_COUNT_MAX + 1)
#define LOUDNESS_INFO_COUNT_MAX         32
#define NODE_COUNT_MAX                  32

#define SUB_DRC_COUNT                   4
#define SEL_DRC_COUNT                   3
#define ADDITIONAL_DOWNMIX_ID_MAX       8
#define BUFFER_FOR_INTERPOLATION_SIZE   4

#define DELAY_MODE_REGULAR_DELAY        0
#define DELAY_MODE_LOW_DELAY            1
#define DELAY_MODE_DEFAULT              DELAY_MODE_REGULAR_DELAY

#define FEATURE_REQUEST_COUNT_MAX       10
#define EFFECT_TYPE_REQUEST_COUNT_MAX   10

#define PROC_COMPLETE                   1
#define UNEXPECTED_ERROR                2
#define PARAM_ERROR                     3
#define EXTERNAL_ERROR                  4
#define MEMORY_ERROR                    5

#define UNDEFINED_LOUDNESS_VALUE        (FIXP_DBL)MAXVAL_DBL
#define ID_FOR_BASE_LAYOUT              0x0
#define ID_FOR_ANY_DOWNMIX              0x7F
#define ID_FOR_NO_DRC                   0x0
#define ID_FOR_ANY_DRC                  0x3F

/* uniDrcInterface */
#define MAX_NUM_DOWNMIX_ID_REQUESTS        15
#define MAX_NUM_DRC_FEATURE_REQUESTS       7
#define MAX_NUM_DRC_EFFECT_TYPE_REQUESTS   15
#define MAX_SIGNATURE_DATA_LENGTH_PLUS_ONE 256

/* extensions */
#define EXT_COUNT_MAX                   2
#define UNIDRCGAINEXT_TERM              0x0
#define UNIDRCLOUDEXT_TERM              0x0
#define UNIDRCCONFEXT_TERM              0x0
#define UNIDRCINTERFACEEXT_TERM         0x0

/* MPEG-H */
#define MAX_NUM_GROUP_ID_REQUESTS        15
#define MAX_NUM_GROUP_PRESET_ID_REQUESTS 15
#define MAX_NUM_MEMBERS_GROUP_PRESET     15

#define MAXPACKETLOSSTIME               5 /* 2.5 seconds */
#define MAXMISSINGCONFIGTIME            5 /* 2.5 seconds */

#define SLOPE_FACTOR_DB_TO_LINEAR   FL2FXCONST_DBL(0.1151f * (float)(1<<3))               /* ln(10) / 20 */

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef enum {
#if UNIDRC_TEST_FRAMEWORK
  DRC_DEC_TEST_TIME_DOMAIN = -100,
  DRC_DEC_TEST_QMF_DOMAIN,
  DRC_DEC_TEST_STFT_DOMAIN,
#endif
  DRC_DEC_CODEC_MODE_UNDEFINED = -1,
  DRC_DEC_MPEG_D_USAC,
  DRC_DEC_MPEG_H_3DA
} DRC_DEC_CODEC_MODE;

typedef enum {
  EFFECT_TYPE_REQUESTED_NONE,
  EFFECT_TYPE_REQUESTED_NIGHT,
  EFFECT_TYPE_REQUESTED_NOISY,
  EFFECT_TYPE_REQUESTED_LIMITED,
  EFFECT_TYPE_REQUESTED_LOWLEVEL,
  EFFECT_TYPE_REQUESTED_DIALOG,
  EFFECT_TYPE_REQUESTED_GENERAL_COMPR,
  EFFECT_TYPE_REQUESTED_EXPAND,
  EFFECT_TYPE_REQUESTED_ARTISTIC,

  EFFECT_TYPE_REQUESTED_COUNT
} EFFECT_TYPE_REQUEST;

typedef enum {
  MATCH_EFFECT_TYPE,
  MATCH_DYNAMIC_RANGE,
  MATCH_DRC_CHARACTERISTIC
} DRC_FEATURE_REQUEST_TYPE;

typedef struct T_UNI_DRC_SEL_PROC_PARAMS{
    /* system parameters */
    int   baseChannelCount; /* optional */
    int   baseLayout; /* optional */
    int   targetConfigRequestType;
    int   numDownmixIdRequests;
    int   downmixIdRequested[MAX_NUM_DOWNMIX_ID_REQUESTS];
    int   targetLayoutRequested;
    int   targetChannelCountRequested;

    /* loudness normalization parameters */
    int   loudnessNormalizationOn;
    FIXP_DBL targetLoudness; /* e = 7 */
    int   albumMode;
    int   peakLimiterPresent;
    int   loudnessDeviationMax; /* only 1 dB steps */
    FIXP_DBL loudnessNormalizationGainDbMax; /* e = 7 */
    FIXP_DBL loudnessNormalizationGainModificationDb; /* e = 7 */
    FIXP_DBL outputPeakLevelMax; /* e = 7 */

    /* dynamic range control parameters */
    int   numBandsSupported;
    int   dynamicRangeControlOn;
    int   numDrcFeatureRequests;
    DRC_FEATURE_REQUEST_TYPE drcFeatureRequestType[MAX_NUM_DRC_FEATURE_REQUESTS];
    int   numDrcEffectTypeRequests[MAX_NUM_DRC_FEATURE_REQUESTS];
    int   numDrcEffectTypeRequestsDesired[MAX_NUM_DRC_FEATURE_REQUESTS];
    EFFECT_TYPE_REQUEST drcEffectTypeRequest[MAX_NUM_DRC_FEATURE_REQUESTS][MAX_NUM_DRC_EFFECT_TYPE_REQUESTS];

    /* MPEG-H parameters */
    int numGroupIdsRequested;
    int groupIdRequested[MAX_NUM_GROUP_ID_REQUESTS];
    int numGroupPresetIdsRequested;
    int groupPresetIdRequested[MAX_NUM_GROUP_PRESET_ID_REQUESTS];
    int numMembersGroupPresetIdsRequested[MAX_NUM_MEMBERS_GROUP_PRESET];
    int groupPresetIdRequestedPreference;
    
    /* other */
    FIXP_SGL boost; /* e = 1 */
    FIXP_SGL compress; /* e = 1 */
    int drcCharacteristicTarget;
} UniDrcSelProcParams;

typedef struct T_UNI_DRC_SEL_PROC_OUTPUT{   
    FIXP_DBL outputPeakLevelDb; /* e = 7 */
    FIXP_DBL loudnessNormalizationGainDb; /* e = 7 */
    FIXP_DBL outputLoudness; /* e = 7 */
        
    int selectedDrcSetIds[SEL_DRC_COUNT];
    int selectedDownmixIds[SEL_DRC_COUNT];
    int numSelectedDrcSets;
    
    int activeDownmixId;
    int baseChannelCount;
    int targetChannelCount;
    int targetLayout;
    int downmixMatrixPresent;

    FIXP_SGL boost;
    FIXP_SGL compress;
    int drcCharacteristicTarget;
    
    int groupIdLoudnessCount;
    int groupId[LOUDNESS_INFO_COUNT_MAX];
    FIXP_DBL groupIdLoudness[LOUDNESS_INFO_COUNT_MAX];
    
} UniDrcSelProcOutput;

#ifdef __cplusplus
}
#endif
#endif
