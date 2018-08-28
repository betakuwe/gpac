/***************************  Fraunhofer IIS CDK Tools  **********************

(C) Copyright Fraunhofer IIS (2015)
All Rights Reserved

$Id$
Author(s):   M. Huettenberger
Description: Fixed-point implementation of a factor 1, 1.5, 2 and 3
upsampling LP-interpolator utilizing biquad sections.

This software and/or program is protected by copyright law and international
treaties. Any reproduction or distribution of this software and/or program,
or any portion of it, may result in severe civil and criminal penalties, and
will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef CDK_TD_UPSAMPLER_H
#define CDK_TD_UPSAMPLER_H

#include "CDK_tools_settings.h"


#define TD_ISO_RESAMPLER /* Activate the time domain upsampler as it is specified in MPEG-H DAM3 in clause 5.5.x.5 TD resampler */

#include "common_fix.h"

#define TD_UPSAMPLER_MAX_DELAY 7

/* configuration */
#define TD_UPSAMPLER_FUNCTIONALITY_1_1
#define TD_UPSAMPLER_FUNCTIONALITY_3_2
#define TD_UPSAMPLER_FUNCTIONALITY_2_1
#define TD_UPSAMPLER_FUNCTIONALITY_3_1

#define TD_SECOND_ORDER_SECTIONS   7                                /* number of cascaded biquads e.g. 7 => IIR order: 14 */
#define TD_STATES_MEM_SIZE         (TD_SECOND_ORDER_SECTIONS * 2)   /* size of the states memory */

#ifdef TD_ISO_RESAMPLER
#ifdef TD_ISO_RESAMPLER_DEFAULT
#define TD_RS_STATES_MEM_SIZE      61
#else
#define TD_RS_STATES_MEM_SIZE      31
#endif
#define TD_RS_DELAY                30
#define DOWNSAMP_FILT_MEM_SIZE     60   
#define TD_RS_IO_SCALE              1
#endif

/* enums */
typedef enum {
  DOWNSAMPLE,
  JUSTFILTER
} RESAMPLING;

/**
* Upsampling factor types
*/
typedef enum
{
  TD_FAC_UPSAMPLE_1_1 = 0,              /**< no upsampling         */
  TD_FAC_UPSAMPLE_3_2 = 1,              /**< factor 3/2 upsampling */
  TD_FAC_UPSAMPLE_2_1 = 2,              /**< factor 2/1 upsampling */
  TD_FAC_UPSAMPLE_3_1 = 3               /**< factor 3/1 upsampling */

} TD_FAC_UPSAMPLE;

/* TD-upsampler interface */

/**
* \brief   Zero-initialize the interpolation filter state memory
*
* \param   facUpsample   i  : factor for upsampling
* \param  *states        i/o: filter states
* \return  delay of upsampler at the output sample rate
*/
short TD_upsampler_init(TD_FAC_UPSAMPLE facUpsample, FIXP_DBL *states);

/**
* \brief  Processes time-domain upsampling
*
* \param   facUpsample  i  : factor for upsampling
* \param  *sigIn        i  : (input) signal to be upsampled
* \param   lenIn        i  : length of input sequence (must be even)
* \param  *sigOut       o  : (output) upsampled signal
* \param  *states       i/o: filter states
* \return  length of the upsampled signal
*
*/
short TD_upsampler(
  TD_FAC_UPSAMPLE facUpsample,
  const FIXP_DBL *sigIn,
  short lenIn,
  FIXP_DBL *sigOut,
  FIXP_DBL *states
  );

#ifdef TD_ISO_RESAMPLER
/**
* \brief   Zero-initialize the interpolation filter state memory for either upsampler or downsampler
*
* \param  *states        i/o: filter states
* \return  delay of resampler at the output sample rate
*/
short TD_iso_resampler_init(FIXP_DBL *states);

/**
* \brief  Processes time-domain upsampling
*
* \param  *sigIn        i  : (input) signal to be upsampled
* \param   lenIn        i  : length of input sequence
* \param  *sigOut       o  : (output) upsampled signal
* \param  *states       i/o: filter states
* \return  length of the upsampled signal
*
*/
short TD_iso_upsampler(
  const FIXP_DBL *sigIn,
  short lenIn,
  FIXP_DBL *sigOut,
  FIXP_DBL *states,
  INT No_memory_used
  );

/**
* \brief  Processes time-domain upsampling
*
* \param  *sigIn        i  : (input) signal to be upsampled
* \param   lenIn        i  : length of input sequence
* \param  *sigOut       o  : (output) upsampled signal
* \return  length of the upsampled signal
*
*/
short TD_iso_upsampler_no_states(
  const FIXP_DBL *sigIn,
  short lenIn,
  FIXP_DBL *sigOut
  );

/**
* \brief  Processes time-domain downsampling
*
* \param  *sigIn        i  : (input) signal to be downsampled
* \param   lenIn        i  : length of input sequence (must be even)
* \param  *sigOut       o  : (output) downsampled signal
* \param  *states       i/o: filter states
* \return  length of the downsampled signal
*
*/
short TD_iso_downsampler_or_SyncFilter(
    const FIXP_DBL *sigIn,
    short lenIn,
    FIXP_DBL *sigOut,
    FIXP_DBL *states,
    INT JustFilter,
    INT Save_States
    );

/**
* \brief  Processes time-domain downsampling
*
* \param  *sigIn        i  : (input) signal to be downsampled
* \param   lenIn        i  : length of input sequence (must be even)
* \param  *sigOut       o  : (output) downsampled signal
* \return  length of the downsampled signal
*
*/
short TD_iso_downsampler_no_states(
  const FIXP_DBL *sigIn,
  short lenIn,
  FIXP_DBL *sigOut
  );
#endif /* TD_ISO_RESAMPLER */


#endif /* CDK_TD_UPSAMPLER_H */


