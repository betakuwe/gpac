/**************************  Fraunhofer IIS CDK HOA Decoder  *****************

                        (C) Copyright Fraunhofer IIS (2015)
                               All Rights Reserved

   $Id$
   Author(s):   Matthias Neusinger
   Description: HOA decoder

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __CDK_HOA_DEC_LIB_H__
#define __CDK_HOA_DEC_LIB_H__

#include "common_fix.h"
#include "CDK_bitstream.h"

#define RENDER_MATRIX_ESD        -1
#define RENDER_MATRIX_CICP_1      1
#define RENDER_MATRIX_CICP_2      2
#define RENDER_MATRIX_CICP_3      3
#define RENDER_MATRIX_CICP_4      4
#define RENDER_MATRIX_CICP_5      5
#define RENDER_MATRIX_CICP_6      6
#define RENDER_MATRIX_CICP_7      7
#define RENDER_MATRIX_CICP_9      9
#define RENDER_MATRIX_CICP_10     10
#define RENDER_MATRIX_CICP_11     11
#define RENDER_MATRIX_CICP_12     12
#define RENDER_MATRIX_CICP_13     13
#define RENDER_MATRIX_CICP_14     14
#define RENDER_MATRIX_CICP_15     15
#define RENDER_MATRIX_CICP_16     16
#define RENDER_MATRIX_CICP_17     17
#define RENDER_MATRIX_CICP_18     18
#define RENDER_MATRIX_CICP_19     19

// HOA decoder errors
typedef enum {
  HOADEC_OK = 0,
  HOADEC_INVALID_PARAM,
  HOADEC_INVALID_CALL,
  HOADEC_UNKNOWN_ERROR
} HOA_DECODER_ERROR;

// HOA config
struct HOAConfig {
  USHORT numHOATransportChannels;
  USHORT hoaOrder;
  USHORT numOfHoaCoeffs;
  SHORT minAmbHoaOrder;
  USHORT minNumOfCoeffsForAmbHOA;
  USHORT numOfAdditionalCoders;
  UCHAR singleLayer;
  USHORT maxNoOfDirSigsForPrediction;
  USHORT noOfBitsPerScalefactor;
  USHORT codedSpatialInterpolationTime;
  USHORT spatialInterpolationMethod;
  USHORT codedVVecLength;
  USHORT maxGainCorrAmpExp;
  USHORT hoaFrameLengthIndicator;
  USHORT ambAsignmBits;
  USHORT activePredIdsBits;
  USHORT numActivePredIdsBits;
  USHORT gainCorrPrevAmpExpBits;
  USHORT maxHOAOrderToBeTransmitted;
  USHORT numVVecVqElementsBits;
};

struct HOA_DECODER;
typedef struct HOA_DECODER *HANDLE_HOA_DECODER;

HOA_DECODER_ERROR HOA_Decoder_Create(HANDLE_HOA_DECODER *hHoaDec);

HOA_DECODER_ERROR HOA_Decoder_Configure(HANDLE_HOA_DECODER hHoaDec,
                                        const HOAConfig *config,
                                        UINT coreFrameLength,
                                        UINT transpSignalDelay,
                                        INT renderMatrixId);

HOA_DECODER_ERROR HOA_Decoder_ParseFrame(HANDLE_HOA_DECODER hHoaDec,
                                         HANDLE_CDK_BITSTREAM hBitStr,
                                         UCHAR errorConcealmentFlag);

HOA_DECODER_ERROR HOA_Decoder_DecodeFrame(HANDLE_HOA_DECODER hHoaDec,
                                          FIXP_DBL **inputTransportChannels,
                                          FIXP_DBL **output);

USHORT HOA_Decoder_GetNumOutputs(HANDLE_HOA_DECODER hHoaDec);

USHORT HOA_Decoder_GetNumOfHoaCoeffs(HANDLE_HOA_DECODER hHoaDec);

HOA_DECODER_ERROR HOA_Decoder_Delete(HANDLE_HOA_DECODER *hHoaDec);

#endif
