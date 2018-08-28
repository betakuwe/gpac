/****************  Fraunhofer IIS arithmetic coding library  *****************

                   (C) Copyright Fraunhofer IIS (2010-2015)
                             All Rights Reserved

   $Id$
   Author(s):   Oliver Weiss
   Description: Interface for Spectral Noiseless Coding Scheme based on an
                Arithmetic Coder in Conjunction with an Adaptive Context

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __AC_ARITH_CODER_H__
#define __AC_ARITH_CODER_H__

#include "ac_arith_coder_settings.h"

#include "common_fix.h"
#include "CDK_bitstream.h"

#include "CDK_audio.h"


typedef enum {
    ARITH_CODER_OK    = 0,
    ARITH_CODER_ERROR = 5
} ARITH_CODING_ERROR;

typedef struct {
  SHORT m_numberLinesPrev;
  UCHAR c_prev[(1024/2)+4]; /* 2-tuple context of previous frame, 4 bit */
} CArcoData;


typedef struct
{
  int low;
  int high;
  int vobf;
  int bits_to_follow;
} Tastat;

/* prototypes */

CArcoData * CArco_Create(void);

void CArco_Destroy(CArcoData * pArcoData);


/**
 * \brief decode a spectral data element by using an adaptive context dependent arithmetic coding scheme
 * \param hBs bit stream handle
 * \param spectrum pointer to quantized data output.
 * \param lg number of quantized spectral coefficients (output by the arithmetic decoder).
 * \param lg_max max number of quantized spectral coefficients.
 * \param arith_reset_flag flag which indicates if the spectral noiseless context must be reset
 * \return void
 */
ARITH_CODING_ERROR CArco_DecodeArithData(
        CArcoData *pArcoData,
        HANDLE_CDK_BITSTREAM hBs,
        FIXP_DBL *RESTRICT spectrum,
        int lg,
        int lg_max,
        int arith_reset_flag
        );


INT ari_decode_14bits(
        HANDLE_CDK_BITSTREAM hBs,
        Tastat *s,
        const SHORT *c_freq,
        int cfl
        );
INT ari_decode_14bits_IGF(
        HANDLE_CDK_BITSTREAM hBs,
        Tastat *s,
        const SHORT *c_freq,
        int cfl
        );


#endif /* __AC_ARITH_CODER_H__ */
