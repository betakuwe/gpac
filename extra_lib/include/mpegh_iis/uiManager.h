/**************************  MPEG-H Decoder UI Manager  **********************

                        (C) Copyright Fraunhofer IIS (2016)
                               All Rights Reserved

   $Id$
   Author(s): Matthias Neusinger
   Description: MPEG-H UI Manager

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "tpdec_lib.h"


#include "common_fix.h"

/* input flags */
#define UI_MANAGER_FORCE_UPDATE         1   /* force output of XML, even if nothing changed */
#define UI_MANAGER_FORCE_RESTART_XML    4   /* force restart of partial XML output */

/* output flags */
#define UI_MANAGER_NO_CHANGE            1   /* nothing changed, no XML written */
#define UI_MANAGER_CONTINUES_XML        2   /* partial XML output, continues output of previous call */
#define UI_MANAGER_INCOMPLETE_XML       4   /* XML output is not complete, further call required */
#define UI_MANAGER_SHORT_OUTPUT         8   /* only minimal XML output was written */

#define UI_MANAGER_USE_DEFAULT_TARGET_LOUDNESS   -128

/* error codes */
typedef enum {
  UI_MANAGER_OK = 0,
  UI_MANAGER_OUT_OF_MEMORY,
  UI_MANAGER_BUFFER_TOO_SMALL,
  UI_MANAGER_INVALID_PARAM,
  UI_MANAGER_NOT_ALLOWED,
  UI_MANAGER_INVALID_STATE
} UI_MANAGER_ERROR;


/* UI state data */
struct USER_INTERACTIVITY_GROUP_STATUS {
  UCHAR groupID;
  UCHAR onOff;
  UCHAR routeToWIRE;
  UCHAR routeToWireID;
  UCHAR changePosition;
  UCHAR azOffset;
  UCHAR elOffset;
  UCHAR distFact;
  UCHAR changeGain;
  UCHAR gain;
};

typedef struct USER_INTERACTIVITY_STATUS {
  UCHAR interactionMode;
  UCHAR numGroups;
  UCHAR groupPresetID;

  USER_INTERACTIVITY_GROUP_STATUS groupData[ASI_MAX_GROUPS];
} USER_INTERACTIVITY_STATUS;

typedef struct {
  SCHAR targetLoudness;
  ULONG drcSelected;
} UI_DRC_LOUDNESS_STATUS;


/* instance handle */
struct UI_MANAGER;
typedef struct UI_MANAGER *HANDLE_UI_MANAGER;

INT UI_Manager_Create(HANDLE_UI_MANAGER *phUiManager);

AUDIO_SCENE_INFO* UI_Manager_GetAsiPointer(HANDLE_UI_MANAGER hUiManager);

UI_MANAGER_ERROR UI_Manager_SetIsActive(HANDLE_UI_MANAGER hUiManager, UCHAR isActive);

UI_MANAGER_ERROR UI_Manager_GetXmlSceneState(
  HANDLE_UI_MANAGER hUiManager,
  char *xmlOut,
  UINT xmlOutSize,
  UINT flagsIn,
  UINT *flagsOut);

UI_MANAGER_ERROR UI_Manager_ApplyXmlAction(
  HANDLE_UI_MANAGER hUiManager,
  const char *xmlIn,
  UINT xmlInSize,
  UINT *flagsOut);

UI_MANAGER_ERROR UI_Manager_GetInteractivityStatus(
  HANDLE_UI_MANAGER hUiManager,
  USER_INTERACTIVITY_STATUS *pUiStatus,
  UCHAR *pSignalChanged);

UI_MANAGER_ERROR UI_Manager_GetDrcLoudnessStatus(
  HANDLE_UI_MANAGER hUiManager,
  UI_DRC_LOUDNESS_STATUS *pDrcLoudnessStatus);

UI_MANAGER_ERROR UI_Manager_GetStatusChanged(
  HANDLE_UI_MANAGER hUiManager,
  UCHAR *pInteractivityStatusChanged,
  UCHAR *pDrcLoudnessStatusChanged
  );

INT UI_Manager_Delete(HANDLE_UI_MANAGER *phUiManager);


#endif
