/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2003)
                               All Rights Reserved

   $Id$
   Author(s):   M. Lohwasser, M. Gayer
   Description:

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef _FFT_RAD2_H
#define _FFT_RAD2_H

#include "CDK_tools_settings.h"

#include "common_fix.h"

/**
 * \brief Performe an inplace  complex valued FFT of 2^n length
 *
 * \param x Input/Output data buffer. The input data must have at least 1 bit scale headroom.
 *          The values are interleaved, real/imag pairs.
 * \param ldn log2 of FFT length
 * \param trigdata Pointer to a sinetable of a length of at least (2^ldn)/2 sine values.
 * \param trigDataSize length of the sinetable "trigdata".
 */
void dit_fft(FIXP_DBL *x, const INT ldn, const FIXP_STP *trigdata, const INT trigDataSize) ;


/**
 * \brief Performe an inplace complex valued inverse FFT of 2^n length
 *
 * \param x Input/Output data buffer. The input data must have at least 1 bit scale headroom.
 *          The values are interleaved, real/imag pairs.
 * \param ldn log2 of FFT length
 * \param trigdata Pointer to a sinetable of a length of at least (2^ldn)/2 sine values.
 * \param trigDataSize length of the sinetable "trigdata".
 */
void dit_ifft(FIXP_DBL *x, const INT ldn, const FIXP_STP *trigdata, const INT trigDataSize) ;


#endif /* _FFT_RAD2_H */

