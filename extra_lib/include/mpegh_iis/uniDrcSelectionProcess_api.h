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


#ifndef _UNI_DRC_SELECTION_PROCESS_API_H_
#define _UNI_DRC_SELECTION_PROCESS_API_H_

#include "uniDrcCommon_api.h"
#include "uniDrcCommon.h"

typedef enum {
  UNI_DRC_SEL_PROC_LOUDNESS_NORMALIZATION_ON,
  UNI_DRC_SEL_PROC_TARGET_LOUDNESS,
  UNI_DRC_SEL_PROC_EFFECT_TYPE,
  UNI_DRC_SEL_PROC_EFFECT_TYPE_FALLBACK_CODE,
  UNI_DRC_SEL_PROC_LOUDNESS_MEASUREMENT_METHOD,
  UNI_DRC_SEL_PROC_DOWNMIX_ID,
  UNI_DRC_SEL_PROC_BASE_CHANNEL_COUNT,
  UNI_DRC_SEL_PROC_BOOST,
  UNI_DRC_SEL_PROC_COMPRESS
} UNI_DRC_SEL_PROC_USER_PARAM;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* handles */
typedef struct T_UNI_DRC_SEL_PROC_STRUCT *HANDLE_UNI_DRC_SEL_PROC_STRUCT;
typedef struct T_UNI_DRC_SEL_PROC_PARAMS *HANDLE_UNI_DRC_SEL_PROC_PARAMS;

/* open */
int
openUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT *phUniDrcSelProcStruct);

int
setCodecModeUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT hUniDrcSelProcStruct,
                                   DRC_DEC_CODEC_MODE codecMode);

#if UNIDRC_TEST_FRAMEWORK
/* init */
int
initUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT hUniDrcSelProcStruct,
                           HANDLE_UNI_DRC_SEL_PROC_PARAMS hUniDrcSelProcParams,
                           HANDLE_UNI_DRC_INTERFACE hUniDrcInterface);
#endif

/* setParam */
int
setParamUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT hUniDrcSelProcStruct,
                               UNI_DRC_SEL_PROC_USER_PARAM requestType,
                               FIXP_DBL requestValue);

/* set MPEG-H specific params */
int
setMpeghParamsUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT hUniDrcSelProcStruct,
                                       int numGroupIdsRequested,
                                       int *groupIdRequested,
                                       int numGroupPresetIdsRequested,
                                       int *groupPresetIdRequested,
                                       int *numMembersGroupPresetIdsRequested,
                                       int groupPresetIdRequestedPreference);

/* process */
int
processUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT hUniDrcSelProcStruct,
                              HANDLE_UNI_DRC_CONFIG hUniDrcConfig,
                              HANDLE_LOUDNESS_INFO_SET hLoudnessInfoSet,
                              HANDLE_UNI_DRC_SEL_PROC_OUTPUT hUniDrcSelProcOutput);

#if UNIDRC_TEST_FRAMEWORK
int
getMultiBandDrcPresent(HANDLE_UNI_DRC_CONFIG hUniDrcConfig,  /* from refsoft RM5. Similar functionality as bitstreamContainsMultibandDrc */
                       int                   numSets[3],
                       int                   multiBandDrcPresent[3]);
#endif

int
bitstreamContainsMultibandDrc(HANDLE_UNI_DRC_CONFIG hUniDrcConfig, int downmixId);

int
getUniDrcConfigHasChanged(HANDLE_UNI_DRC_SEL_PROC_STRUCT hUniDrcSelProcStruct);

/* close */
int closeUniDrcSelectionProcess(HANDLE_UNI_DRC_SEL_PROC_STRUCT *phUniDrcSelProcStruct);


#endif /* _UNI_DRC_SELECTION_PROCESS_API_H_ */

#ifdef __cplusplus
}
#endif /* __cplusplus */

