/***************************  CDK Sequential/Parallel downmix module  ************************

                     (C) Copyright Fraunhofer IIS 2015
                               All Rights Reserved

  $Id$
  Author(s):   Alfonso Pino Garcia
  Description: Declare functions that perform (de)interleaving combined with format change

  This software and/or program is protected by copyright law and international
  treaties. Any reproduction or distribution of this software and/or program,
  or any portion of it, may result in severe civil and criminal penalties, and
  will be prosecuted to the maximum extent possible under law.

*******************************************************************************/

#include "global_functional_range.h"

#include "fixpoint_math.h"

void CDK_dmx_interleave(FIXP_DBL *RESTRICT pIn, LONG  *RESTRICT pOut, UINT channels, UINT frameSize, UINT length);
void CDK_dmx_interleave(FIXP_DBL *RESTRICT pIn, SHORT *RESTRICT pOut, UINT channels, UINT frameSize, UINT length);
void CDK_dmx_interleave(SHORT    *RESTRICT pIn, SHORT *RESTRICT pOut, UINT channels, UINT frameSize, UINT length);

void CDK_dmx_deinterleave(const LONG   *RESTRICT pIn, SHORT *RESTRICT pOut, const UINT channels, const UINT frameSize, const UINT bufferSize);
void CDK_dmx_deinterleave(const LONG   *RESTRICT pIn, LONG  *RESTRICT pOut, const UINT channels, const UINT frameSize, const UINT bufferSize);
void CDK_dmx_deinterleave(const SHORT  *RESTRICT pIn, SHORT *RESTRICT pOut, const UINT channels, const UINT frameSize, const UINT bufferSize);
void CDK_dmx_deinterleave(const SHORT  *RESTRICT pIn, LONG  *RESTRICT pOut, const UINT channels, const UINT frameSize, const UINT bufferSize);
