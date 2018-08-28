/************************  CDK PCM postprocessor module  *********************

                    (C) Copyright Fraunhofer IIS 2008-2014
                              All Rights Reserved

  $Id$
  Author(s):   Matthias Neusinger
  Description: Hard limiter for clipping prevention

  This software and/or program is protected by copyright law and international
  treaties. Any reproduction or distribution of this software and/or program,
  or any portion of it, may result in severe civil and criminal penalties, and
  will be prosecuted to the maximum extent possible under law.

*******************************************************************************/

#ifndef _LIMITER_H_
#define _LIMITER_H_

#include "common_fix.h"
#include "CDK_audio.h"
#include "CDK_tools_settings.h"

#define TDL_ATTACK_DEFAULT_MS      (15)              /* default attack  time in ms */
#define TDL_RELEASE_DEFAULT_MS     (50)              /* default release time in ms */

#define TDL_GAIN_SCALING           (15)              /* scaling of gain value. */

#ifdef __cplusplus
extern "C" {
#endif

struct TDLimiter {
  unsigned int  attack;
  FIXP_DBL      attackConst, releaseConst;
  unsigned int  attackMs, releaseMs, maxAttackMs;
  FIXP_DBL      threshold;
  unsigned int  channels, maxChannels;
  UINT          sampleRate, maxSampleRate;
  FIXP_DBL      cor, max;
  FIXP_DBL*     maxBuf;
  FIXP_DBL*     delayBuf;
  unsigned int  maxBufIdx, delayBufIdx;
  FIXP_DBL      smoothState0;
  FIXP_DBL      minGain;

  FIXP_DBL      additionalGainPrev;
  FIXP_DBL      additionalGainFilterState;
  FIXP_DBL      additionalGainFilterState1;
};


typedef enum {
  TDLIMIT_OK = 0,
  TDLIMIT_UNKNOWN = -1,

  __error_codes_start = -100,

  TDLIMIT_INVALID_HANDLE,
  TDLIMIT_INVALID_PARAMETER,

  __error_codes_end
} TDLIMITER_ERROR;

struct TDLimiter;
typedef struct TDLimiter* TDLimiterPtr;

#define PCM_LIM LONG
#define FIXP_DBL2PCM_LIM(x) (x)
#define PCM_LIM_BITS 32

/******************************************************************************
* pcmLimiter_Reset                                                            *
* limiter: limiter handle                                                     *
* returns: error code                                                         *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_Reset(TDLimiterPtr limiter);


/******************************************************************************
* pcmLimiter_Destroy                                                          *
* limiter: limiter handle                                                     *
* returns: error code                                                         *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_Destroy(TDLimiterPtr limiter);


/******************************************************************************
* pcmLimiter_GetDelay                                                         *
* limiter: limiter handle                                                     *
* returns: exact delay caused by the limiter in samples per channel           *
******************************************************************************/
unsigned int pcmLimiter_GetDelay(TDLimiterPtr limiter);

/******************************************************************************
* pcmLimiter_GetMaxGainReduction                                              *
* limiter: limiter handle                                                     *
* returns: maximum gain reduction in last processed block in dB               *
******************************************************************************/
INT pcmLimiter_GetMaxGainReduction(TDLimiterPtr limiter);

/******************************************************************************
* pcmLimiter_SetNChannels                                                     *
* limiter:   limiter handle                                                   *
* nChannels: number of channels ( <= maxChannels specified on create)         *
* returns:   error code                                                       *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_SetNChannels(TDLimiterPtr limiter, unsigned int nChannels);

/******************************************************************************
* pcmLimiter_SetSampleRate                                                    *
* limiter:    limiter handle                                                  *
* sampleRate: sampling rate in Hz ( <= maxSampleRate specified on create)     *
* returns:    error code                                                      *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_SetSampleRate(TDLimiterPtr limiter, UINT sampleRate);

/******************************************************************************
* pcmLimiter_SetAttack                                                        *
* limiter:    limiter handle                                                  *
* attackMs:   attack time in ms ( <= maxAttackMs specified on create)         *
* returns:    error code                                                      *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_SetAttack(TDLimiterPtr limiter, unsigned int attackMs);

/******************************************************************************
* pcmLimiter_SetRelease                                                       *
* limiter:    limiter handle                                                  *
* releaseMs:  release time in ms                                              *
* returns:    error code                                                      *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_SetRelease(TDLimiterPtr limiter, unsigned int releaseMs);


/******************************************************************************
* pcmLimiter_GetLibInfo                                                       *
* info:       pointer to an allocated and initialized LIB_INFO structure      *
* returns:    error code                                                      *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_GetLibInfo( LIB_INFO *info );

#ifdef __cplusplus
}
#endif

/******************************************************************************
* pcmLimiter_Create                                                           *
* maxAttackMs:   maximum and initial attack/lookahead time in milliseconds    *
* releaseMs:     release time in milliseconds (90% time constant)             *
* threshold:     limiting threshold                                           *
* maxChannels:   maximum and initial number of channels                       *
* maxSampleRate: maximum and initial sampling rate in Hz                      *
* returns:       limiter handle                                               *
******************************************************************************/
TDLimiterPtr pcmLimiter_Create(unsigned int  maxAttackMs,
                           unsigned int  releaseMs,
                           FIXP_DBL      threshold,
                           unsigned int  maxChannels,
                           UINT          maxSampleRate);

/******************************************************************************
* pcmLimiter_SetThreshold                                                     *
* limiter:    limiter handle                                                  *
* threshold:  limiter threshold                                               *
* returns:    error code                                                      *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_SetThreshold(TDLimiterPtr limiter, FIXP_DBL threshold);

/******************************************************************************
* pcmLimiter_Apply                                                            *
* limiter:    limiter handle                                                  *
* pGain :     pointer to gains to be applied to the signal before limiting,   *
*             which are downscaled by TDL_GAIN_SCALING bit.                   *
*             These gains are delayed by gain_delay, and smoothed.            *
*             Smoothing is done by a butterworth lowpass filter with a cutoff *
*             frequency which is fixed with respect to the sampling rate.     *
*             It is a substitute for the smoothing due to windowing and       *
*             overlap/add, if a gain is applied in frequency domain.          *
* gain_scale: pointer to scaling exponents to be applied to the signal before *
*             limiting, without delay and without smoothing                   *
* gain_size:  number of elements in pGain, currently restricted to 1          *
* gain_delay: delay [samples] with which the gains in pGain shall be applied  *
*             gain_delay <= nSamples                                          *
* samples:    input/output buffer containing interleaved samples              *
*             precision of output will be DFRACT_BITS-TDL_GAIN_SCALING bits   *
* nSamples:   number of samples per channel                                   *
* returns:    error code                                                      *
******************************************************************************/
TDLIMITER_ERROR pcmLimiter_Apply(
                 TDLimiterPtr limiter,
                 PCM_LIM*    samplesIn,
                 INT_PCM*     samplesOut,
                 FIXP_DBL*    pGain,
                 const INT*   gain_scale,
                 const UINT   gain_size,
                 const UINT   gain_delay,
                 const UINT   nSamples);

#endif  /* #ifndef _LIMITER_H_ */
