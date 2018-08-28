/*****************************************************************************

                         (C) Copyright Fraunhofer IIS (2014)
                                All Rights Reserved

    $Id$
    Author(s):   Bernhard Neugebauer
    Description: MPEG-D DRC Decoder

    This software and/or program is protected by copyright law and international
    treaties. Any reproduction or distribution of this software and/or program,
    or any portion of it, may result in severe civil and criminal penalties, and
    will be prosecuted to the maximum extent possible under law.

******************************************************************************/ 

#ifndef iis_DRC_GainDecoder_h
#define iis_DRC_GainDecoder_h

#include "uniDrcCommon.h"
#include "CDK_bitstream.h"
#include "CDK_audio.h"

#if defined __cplusplus
extern "C" {
#endif

struct sDrcDecParams;
typedef struct sDrcDecParams DrcDecParams, *HANDLE_DRC_DECODER;

typedef enum {
  DRC_DEC_SELECTION = 0x1,    /* DRC decoder instance for DRC set selection only */
  DRC_DEC_GAIN      = 0x2,    /* DRC decoder instance for applying DRC only */
  DRC_DEC_ALL       = 0x3     /* DRC decoder with full functionality */
} DRC_DEC_FUNCTIONAL_RANGE;

typedef enum {
  /* get and set userparams */
  DRC_DEC_BOOST,
  DRC_DEC_COMPRESS,
  /* set only userparams */
  DRC_DEC_LOUDNESS_NORMALIZATION_ON,
  DRC_DEC_TARGET_LOUDNESS,                      /**< Set target loudness in dB as FIXP_DBL with exponent equal to 7 */
  DRC_DEC_EFFECT_TYPE,
  DRC_DEC_EFFECT_TYPE_FALLBACK_CODE,
  DRC_DEC_LOUDNESS_MEASUREMENT_METHOD,
  /* set only system (not user) parameters */
  DRC_DEC_DOWNMIX_ID, 
  DRC_DEC_BASE_CHANNEL_COUNT,
  /* get only system parameters */
  DRC_DEC_IS_MULTIBAND_DRC_1,
  DRC_DEC_IS_MULTIBAND_DRC_2
} DRC_DEC_USERPARAM;

typedef enum {
  DRC_DEC_OK = 0,

  DRC_DEC_NOT_OK = -10000,
  DRC_DEC_OUT_OF_MEMORY,
  DRC_DEC_NOT_OPENED,
  DRC_DEC_NOT_READY,
  DRC_DEC_PARAM_OUT_OF_RANGE,
  DRC_DEC_INVALID_PARAM
} DRC_DEC_ERROR;

DRC_DEC_ERROR
iisDrcDec_Open(HANDLE_DRC_DECODER* phDrcDec,
               DRC_DEC_FUNCTIONAL_RANGE functionalRange);

DRC_DEC_ERROR
iisDrcDec_SetCodecMode(HANDLE_DRC_DECODER hDrcDec,
                       DRC_DEC_CODEC_MODE codecMode);

DRC_DEC_ERROR
iisDrcDec_Init(HANDLE_DRC_DECODER hDrcDec,
               int audioFrameSize,
               int sampleRate,
               int baseChannelCount);

DRC_DEC_ERROR
iisDrcDec_Close(HANDLE_DRC_DECODER* phDrcDec);

/* set single user request */
DRC_DEC_ERROR
iisDrcDec_SetParam(HANDLE_DRC_DECODER hDrcDec,
                   DRC_DEC_USERPARAM requestType,
                   FIXP_DBL requestValue);

DRC_DEC_ERROR
iisDrcDec_GetParam(HANDLE_DRC_DECODER hDrcDec,
                   DRC_DEC_USERPARAM requestType,
                   FIXP_DBL* pRequestValue);

DRC_DEC_ERROR
iisDrcDec_SetSelectionProcessParameters(HANDLE_DRC_DECODER hDrcDec,
                                        UniDrcSelProcParams uniDrcSelProcParams);

DRC_DEC_ERROR
iisDrcDec_SetSelectionProcessMpeghParameters(HANDLE_DRC_DECODER hDrcDec,
                                             int numGroupIdsRequested,
                                             int *groupIdsRequested,
                                             int numGroupPresetIdsRequested,
                                             int *groupPresetIdsRequested,
                                             int *numMembersGroupPresetIdsRequested,
                                             int groupPresetIdRequestedPreference);

DRC_DEC_ERROR
iisDrcDec_SetSelectionProcessMpeghParameters_simple(HANDLE_DRC_DECODER hDrcDec,
                                                    int groupPresetIdRequested,
                                                    int numGroupIdsRequested,
                                                    int *groupIdsRequested);

DRC_DEC_ERROR
iisDrcDec_SetDownmixInstructions(
        HANDLE_DRC_DECODER hDrcDec,
        const int numDowmixId,
        int* downmixId,
        int* targetLayout,
        int* targetChannelCount);

void
iisDrcDec_SetSelectionProcessOutput(HANDLE_DRC_DECODER hDrcDec,
                                    UniDrcSelProcOutput uniDrcSelProcOutput);

UniDrcSelProcOutput*
iisDrcDec_GetSelectionProcessOutput(HANDLE_DRC_DECODER hDrcDec);

void
iisDrcDec_SetChannelGains(HANDLE_DRC_DECODER hDrcDec,
                          const int numChannels,
                          const int frameSize,
                          FIXP_DBL* channelGainDb,
                          FIXP_DBL *audioBuffer,
                          const int audioBufferChannelOffset);

DRC_DEC_ERROR
iisDrcDec_ReadUniDrcConfig(HANDLE_DRC_DECODER hDrcDec,
        HANDLE_CDK_BITSTREAM hBitstream);

DRC_DEC_ERROR
iisDrcDec_ReadLoudnessInfoSet(HANDLE_DRC_DECODER hDrcDec,
        HANDLE_CDK_BITSTREAM hBitstream);

DRC_DEC_ERROR
iisDrcDec_ReadUniDrcGain(HANDLE_DRC_DECODER hDrcDec,
                         HANDLE_CDK_BITSTREAM hBitstream);


/* calling sequence:
   iisDrcDec_Read...()
   iisDrcDec_SetChannelGains()
   iisDrcDec_Preprocess()
   iisDrcDec_Process...() */

DRC_DEC_ERROR
iisDrcDec_Preprocess(HANDLE_DRC_DECODER hDrcDec);

DRC_DEC_ERROR
iisDrcDec_ProcessTime(HANDLE_DRC_DECODER hDrcDec,
                      int delaySamples,
                      int decDownmixId,
                      int channelOffset,
                      int drcChannelOffset,
                      int numChannelsProcessed,
                      FIXP_DBL* realBuffer,
                      const int timeDataChannelOffest);

DRC_DEC_ERROR
iisDrcDec_ProcessFreq(HANDLE_DRC_DECODER hDrcDec,
                      int delaySamples,
                      int decDownmixId,
                      int channelOffset,
                      int drcChannelOffset,
                      int numChannelsProcessed,
                      int processSingleTimeslot,
                      FIXP_DBL** realBuffer,
                      FIXP_DBL** imagBuffer);

/* Get library info for this module. */
DRC_DEC_ERROR
iisDrcDec_GetLibInfo( LIB_INFO *info );


#if defined __cplusplus
}
#endif
#endif

