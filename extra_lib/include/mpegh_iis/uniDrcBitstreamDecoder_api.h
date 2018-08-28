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

 Apple Inc. and Fraunhofer IIS

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

 Apple Inc. and Fraunhofer IIS retains full right to modify and use the code for its
 own purpose, assign or donate the code to a third party and to inhibit third parties
 from using the code for products that do not conform to MPEG-related ITU Recommenda-
 tions and/or ISO/IEC International Standards.

 This copyright notice must be included in all copies or derivative works.

 Copyright (c) ISO/IEC 2014.

 ***********************************************************************************/


#ifndef _UNI_DRC_BITSTREAM_DECODER_API_H_
#define _UNI_DRC_BITSTREAM_DECODER_API_H_

#include "uniDrcCommon_api.h"
#include "uniDrcCommon.h"
#include "uniDrc.h"
#include "CDK_bitstream.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef DRC_RM5_BS_SYNTAX
typedef enum {
  UNI_DRC_BS_DEC_BASE_CHANNEL_COUNT
} UNI_DRC_BS_DEC_USER_PARAM;
#endif /* ! DRC_RM5_BS_SYNTAX */

/* handles */
typedef struct T_UNI_DRC_BS_DEC_STRUCT *HANDLE_UNI_DRC_BS_DEC_STRUCT;

/* open */
int
openUniDrcBitstreamDec(HANDLE_UNI_DRC_BS_DEC_STRUCT *phUniDrcBsDecStruct);

/* init */
int
initUniDrcBitstreamDec(HANDLE_UNI_DRC_BS_DEC_STRUCT hUniDrcBsDecStruct,
                       HANDLE_UNI_DRC_GAIN hUniDrcGain,
                       int audioSampleRate,
                       int audioFrameSize,
#ifndef DRC_RM5_BS_SYNTAX
                       int baseChannelCount,
#endif /* DRC_RM5_BS_SYNTAX */
                       int delayMode);

#ifndef DRC_RM5_BS_SYNTAX
int
setParamUniDrcBitstreamDec(HANDLE_UNI_DRC_BS_DEC_STRUCT hUniDrcBsDecStruct,
                           UNI_DRC_BS_DEC_USER_PARAM requestType,
                           INT requestValue);

#endif /* ! DRC_RM5_BS_SYNTAX */


int
parseMpegh3daUniDrcConfig(HANDLE_CDK_BITSTREAM bitstream,
                          HANDLE_UNI_DRC_BS_DEC_STRUCT hUniDrcBsDecStruct,
                          UniDrcConfig* uniDrcConfig,
                          LoudnessInfoSet* loudnessInfoSet
                          );

int
parseUniDrcGain(HANDLE_CDK_BITSTREAM bitstream,
                HANDLE_UNI_DRC_BS_DEC_STRUCT hUniDrcBsDecStruct,
                HANDLE_UNI_DRC_CONFIG hUniDrcConfig,
                HANDLE_UNI_DRC_GAIN hUniDrcGain);


int
parseMpegh3daLoudnessInfoSet(HANDLE_CDK_BITSTREAM bitstream,
                             LoudnessInfoSet* loudnessInfoSet);


/* close */
int closeUniDrcBitstreamDec(HANDLE_UNI_DRC_BS_DEC_STRUCT *phUniDrcBsDecStruct);

#if DEBUG_STRUCTS
/* plot info */
void printUniDrcGain( HANDLE_UNI_DRC_CONFIG hUniDrcConfig,
                        HANDLE_UNI_DRC_GAIN hUniDrcGain);
void printUniDrcConfig(HANDLE_UNI_DRC_CONFIG uniDrcConfig);
void printLoudnessInfoSet(HANDLE_LOUDNESS_INFO_SET loudnessInfoSet);
#endif

#endif /* _UNI_DRC_BITSTREAM_DECODER_H_ */

#ifdef __cplusplus
}
#endif /* __cplusplus */

