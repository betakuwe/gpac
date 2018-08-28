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

 Fraunhofer IIS

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

 Fraunhofer IIS retains full right to modify and use the code for its own purpose,
 assign or donate the code to a third party and to inhibit third parties from using
 the code for products that do not conform to MPEG-related ITU Recommendations and/or
 ISO/IEC International Standards.

 This copyright notice must be included in all copies or derivative works.

 Copyright (c) ISO/IEC 2014.

 ***********************************************************************************/


#ifndef _DRC_TOOL_API_H_
#define _DRC_TOOL_API_H_

#include "uniDrcCommon_api.h"
#include "uniDrcCommon.h"
#include "uniDrc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SUBBAND_DOMAIN_MODE_OFF         0
#define SUBBAND_DOMAIN_MODE_QMF64       1
#define SUBBAND_DOMAIN_MODE_QMF71       2
#define SUBBAND_DOMAIN_MODE_STFT256     3

    /* QMF64 */
#define AUDIO_CODEC_SUBBAND_COUNT_QMF64                     64
#define AUDIO_CODEC_SUBBAND_DOWNSAMPLING_FACTOR_QMF64       64
#define AUDIO_CODEC_SUBBAND_ANALYSE_DELAY_QMF64             320

    /* QMF71 (according to ISO/IEC 23003-1:2007) */
#define AUDIO_CODEC_SUBBAND_COUNT_QMF71                     71
#define AUDIO_CODEC_SUBBAND_DOWNSAMPLING_FACTOR_QMF71       64
#define AUDIO_CODEC_SUBBAND_ANALYSE_DELAY_QMF71             320+384

    /* STFT256 (according to ISO/IEC 23008-3:2015/AMD3) */
#define AUDIO_CODEC_SUBBAND_COUNT_STFT256                   256
#define AUDIO_CODEC_SUBBAND_DOWNSAMPLING_FACTOR_STFT256     256
#define AUDIO_CODEC_SUBBAND_ANALYSE_DELAY_STFT256           256

/* Handles to parameter structs */
typedef struct T_UNI_DRC_GAIN_DEC_STRUCTS     *HANDLE_UNI_DRC_GAIN_DEC_STRUCTS;

/* init functions */
int drcDecOpen(HANDLE_UNI_DRC_GAIN_DEC_STRUCTS *phUniDrcGainDecStructs);

int drcDecInit(   HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                  const int                        audioFrameSize,
                  const int                        audioSampleRate,
                  const int                        delayMode,
                  const int                        timeDomainSupported,
                  const int                        subbandDomainSupported);

int drcDecInitAfterConfig(    HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                              HANDLE_UNI_DRC_CONFIG            hUniDrcConfig,
                              const int                        numSetsProcessed,
                              const int*                       drcSetIdProcessed,
                              const int*                       drcDownmixIdProcessed,
                              FIXP_DBL                         loudnessNormalizationGainDb);

/* close functions */
int drcDecClose( HANDLE_UNI_DRC_GAIN_DEC_STRUCTS *phUniDrcGainDecStructs,
                 HANDLE_UNI_DRC_CONFIG hUniDrcConfig);

/* process functions for iisDRCDecLib */

/* call drcDecInterpolate first */
int drcDecInterpolate(HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                      HANDLE_UNI_DRC_CONFIG            hUniDrcConfig,
                      HANDLE_UNI_DRC_GAIN              hUniDrcGain,
                      const FIXP_DBL                   loudnessNormalizationGainDb,
                      const FIXP_SGL                   boost,
                      const FIXP_SGL                   compress,
                      const int                        drcCharacteristicTarget);  /* not supported */

/* Then call one of drcDecProcessTime, drcDecProcessFreq */
int drcDecProcessTime( HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                       HANDLE_UNI_DRC_CONFIG            hUniDrcConfig,
                       int delaySamples,
                       int decDownmixId,
                       int channelOffset,
                       int drcChannelOffset,
                       int numChannelsProcessed,
                       FIXP_DBL*                        audioIOBuffer,
                       const int timeDataChannelOffest);

int drcDecProcessFreq( HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                       HANDLE_UNI_DRC_CONFIG            hUniDrcConfig,
                       int delaySamples,
                       int decDownmixId,
                       int channelOffset,
                       int drcChannelOffset,
                       int numChannelsProcessed,
                       int processSingleTimeslot,
                       FIXP_DBL*                        audioIOBufferReal[],
                       FIXP_DBL*                        audioIOBufferImag[]);

int drcDecApplyLoudnessNormalization( HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                                      FIXP_DBL                         loudnessNormalizationGainDb);

#if UNIDRC_TEST_FRAMEWORK
int drcDecGetChannelCount(HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs,
                          HANDLE_UNI_DRC_CONFIG            hUniDrcConfig,
                          int decDownmixId);
#endif

void drcDecSetChannelGains(HANDLE_UNI_DRC_GAIN_DEC_STRUCTS  hUniDrcGainDecStructs, int numChannels, int frameSize, FIXP_DBL* channelGainDb, FIXP_DBL* audioBuffer, const int audioBufferChannelOffset);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DRC_TOOL_API_H_ */

