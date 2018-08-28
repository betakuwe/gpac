/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2001)
                               All Rights Reserved

   $Id$
   Author(s):   Josef Hoepfl, DSP Solutions
   Description: Fix point FFT

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __FFT_H__
#define __FFT_H__

#include "CDK_tools_settings.h"
#include "common_fix.h"

/**
 * \brief Perform an inplace complex valued FFT of length 2^n
 *
 * \param length Length of the FFT to be calculated.
 * \param pInput Input/Output data buffer. The input data must have at least 1 bit scale headroom.
 *          The values are interleaved, real/imag pairs.
 * \param scalefactor Pointer to an INT, which contains the current scale of the input data,
 *                    which is updated according to the FFT scale.
 */
void fft(int length, FIXP_DBL *pInput, INT *scalefactor);

/**
 * \brief Perform an inplace complex valued IFFT of length 2^n
 *
 * \param length Length of the FFT to be calculated.
 * \param pInput Input/Output data buffer. The input data must have at least 1 bit scale headroom.
 *          The values are interleaved, real/imag pairs.
 * \param scalefactor Pointer to an INT, which contains the current scale of the input data,
 *                    which is updated according to the IFFT scale.
 */
void ifft(int length, FIXP_DBL *pInput, INT *scalefactor);


/*
 * Frequently used and fixed short length FFTs.
 */


#ifndef FUNCTION_fft_8
LNK_SECTION_CODE_L1
static void CDK_FORCEINLINE fft_8(FIXP_DBL *x)
{
      FIXP_SPK w_PiFOURTH = { { FIXP_SGL(0x5A82), FIXP_SGL(0x5A82) } };

      FIXP_DBL a00, a10, a20, a30;
      FIXP_DBL y[16];

      a00 = (x[0] + x[8])>>1;
      a10 =  x[4] + x[12];
      a20 = (x[1] + x[9])>>1;
      a30 =  x[5] + x[13];

      y[0] = a00 + (a10>>1);
      y[4] = a00 - (a10>>1);
      y[1] = a20 + (a30>>1);
      y[5] = a20 - (a30>>1);

      a00 = a00      - x[8];
      a10 = (a10>>1) - x[12];
      a20 = a20      - x[9];
      a30 = (a30>>1) - x[13];

      y[2] = a00 + a30;
      y[6] = a00 - a30;
      y[3] = a20 - a10;
      y[7] = a20 + a10;

      a00 = (x[2] + x[10])>>1;
      a10 =  x[6] + x[14];
      a20 = (x[3] + x[11])>>1;
      a30 =  x[7] + x[15];

      y[8]  = a00 + (a10>>1);
      y[12] = a00 - (a10>>1);
      y[9]  = a20 + (a30>>1);
      y[13] = a20 - (a30>>1);

      a00 = a00      - x[10];
      a10 = (a10>>1) - x[14];
      a20 = a20      - x[11];
      a30 = (a30>>1) - x[15];

      y[10] = a00 + a30;
      y[14] = a00 - a30;
      y[11] = a20 - a10;
      y[15] = a20 + a10;

      FIXP_DBL vr, vi, ur, ui;

      ur = y[0]>>1;
      ui = y[1]>>1;
      vr = y[8];
      vi = y[9];
      x[0] = ur + (vr>>1);
      x[1] = ui + (vi>>1);
      x[8] = ur - (vr>>1);
      x[9] = ui - (vi>>1);

      ur = y[4]>>1;
      ui = y[5]>>1;
      vi = y[12];
      vr = y[13];
      x[4]  = ur + (vr>>1);
      x[5]  = ui - (vi>>1);
      x[12] = ur - (vr>>1);
      x[13] = ui + (vi>>1);

      ur = y[10];
      ui = y[11];

      cplxMultDiv2(&vi,&vr,ui,ur,w_PiFOURTH);

      ur = y[2];
      ui = y[3];
      x[2]  = (ur>>1) + vr;
      x[3]  = (ui>>1) + vi;
      x[10] = (ur>>1) - vr;
      x[11] = (ui>>1) - vi;

      ur = y[14];
      ui = y[15];

      cplxMultDiv2(&vr,&vi,ui,ur,w_PiFOURTH);

      ur = y[6];
      ui = y[7];
      x[6]  = (ur>>1) + vr;
      x[7]  = (ui>>1) - vi;
      x[14] = (ur>>1) - vr;
      x[15] = (ui>>1) + vi;
}
#endif /* FUNCTION_fft_8 */

#endif
