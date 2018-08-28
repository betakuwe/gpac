/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2001)
                               All Rights Reserved

   $Id$
   Author(s):   Josef Hoepfl, DSP Solutions
   Description: Fix point RFFT

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __RFFT_H__
#define __RFFT_H__

#include "CDK_tools_settings.h"


#include "common_fix.h"

/**
 * \brief Perform an inplace real FFT using a complex fft of half length.
 *
 * A FFT of an real input signal with N input samples results (N/2+1) complex output samples.
 * To get the (N/2+1) complex output values the input buffer has to be at least N+2 samples. If the input buffer
 * has only N samples the real part of (N/2+1) complex value is written into imaginary part of first complex
 * output value.
 *
 * \param fftLength             Length of the FFT to be calculated.
 * \param pBuffer               Pointer to Input/Output buffer. The input data must have at least 1 bit scale headroom.
 * \param bufferSize            Size of Input/Output buffer in bytes.
 * \param scalefactor           Pointer to an INT, which contains the current scale of the input data,
 *                              which is updated according to the FFT scale.
 *
 * \return                      0, on success; nonzero, on failure.
 */
INT rfft(
        const UINT                fftLength,
        FIXP_DBL * const          pBuffer,
        const UINT                bufferSize,
        INT * const               scalefactor
        );

/**
 * \brief Perform an inplace real IFFT using a complex fft of half length.
 *
 * The input buffer has to hold (N/2+1) complex spectral values. Because of symmetric issue the real time signal can
 * be calculated with the halve length of complex spectrum. The imaginary part of first and (N/2+1) spectral value must be zero.
 * Alternatively it's possible to feed the IFFT with an input buffer of size N/2 complex spectral values wherein the real
 * part of (N/2+1) spectral value must be placed to the imaginary part of the first complex spectral value.
 *
 * \param fftLength             Length of the FFT to be calculated.
 * \param pBuffer               Pointer to Input/Output buffer. The input data must have at least 1 bit scale headroom.
 * \param bufferSize            Size of Input/Output buffer in bytes.
 * \param scalefactor           Pointer to an INT, which contains the current scale of the input data,
 *                              which is updated according to the FFT scale.
 *
 * \return                      0, on success; nonzero, on failure.
 */
INT irfft(
        const UINT                fftLength,
        FIXP_DBL * const          pBuffer,
        const UINT                bufferSize,
        INT * const               scalefactor
        );


void getSineTab(
        const UINT                length,
        const FIXP_STP**          ppSineTab,
        UINT*                     pStep
        );


#endif /* __RFFT_H__ */
