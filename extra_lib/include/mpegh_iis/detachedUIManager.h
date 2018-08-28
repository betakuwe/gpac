/**************************  MPEG-H Decoder UI Manager  **********************

                        (C) Copyright Fraunhofer IIS (2016)
                               All Rights Reserved

   $Id$
   Author(s): Matthias Neusinger
   Description: MPEG-H Detached UI Manager

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __DETACHED_UI_MANAGER_H__
#define __DETACHED_UI_MANAGER_H__

#include "machine_type.h"
#include "CDK_audio.h"

#include "genericStds.h"


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct DETACHED_UI_MANAGER *HANDLE_DETACHED_UI_MANAGER;

typedef enum {
  DETACHED_UI_OK = 0,
  DETACHED_UI_OUT_OF_MEMORY,
  DETACHED_UI_BUFFER_TOO_SMALL,
  DETACHED_UI_INVALID_PARAM,
  DETACHED_UI_NOT_ALLOWED,
  DETACHED_UI_INVALID_STATE,
  DETACHED_UI_PARSE_ERROR
} DETACHED_UI_ERROR;

#define DETACHED_UI_FORCE_UPDATE        1   /*!< Flag for detached_UI_GetXmlSceneState(): Force output of XML
                                                 description, even if nothing has changed since the last call. */
#define DETACHED_UI_FORCE_RESTART_XML   4   /*!< Flag for detached_UI_GetXmlSceneState(): Force restart of
                                                 XML output if incomplete output was returned by previous call. */

#define DETACHED_UI_NO_CHANGE           1   /*!< Flag returned by detached_UI_GetXmlSceneState(): Nothing has
                                                 changed since the last call, no XML output was generated. */
#define DETACHED_UI_CONTINUES_XML       2   /*!< Flag returned by detached_UI_GetXmlSceneState(): XML output
                                                 is a continuation of incomplete output of the previous call. */
#define DETACHED_UI_INCOMPLETE_XML      4   /*!< Flag returned by detached_UI_GetXmlSceneState(): XML output
                                                 is not complete, at least one further call of the function is
                                                 required to get the complete XML output. */


/**
 * \brief           Open a detached UI manager instance
 * \return          UI manager handle
 */
LINKSPEC_H HANDLE_DETACHED_UI_MANAGER 
detached_UI_Manager_Open();

/**
 * \brief           De-allocate all resources of a detached UI manager instance.
 *
 * \param self      UI manager handle.
 * \return          void
 */
LINKSPEC_H void
detached_UI_Manager_Close(HANDLE_DETACHED_UI_MANAGER self);

/**
 * \brief            Get XML description of audio scene and available user interactivity parameters
 *
 * \param self       UI manager handle.
 * \param xmlOut     Pointer to external output buffer where the XML string will be stored.
 * \param xmlOutSize Size of the output buffer (number of chars). Has to be at least 1024 to ensure success of the function.
 * \param flagsIn    Bit field with flags for the UI manager: \n
 *                   (flags & ::AACDEC_UI_FORCE_UPDATE) != 0: Force output of XML description even if nothing has changed \n
 *                   (flags & ::AACDEC_UI_FORCE_RESTART_XML) != 0: Restart XML output from the beginning (if incomplete 
 *                   output was returned by previous call). \n
 * \param flagsOut   Pointer to bit field with flags returned by the UI manager: \n
 *                   (flags & ::AACDEC_UI_NO_CHANGE) != 0: Nothing has changed since the previous call of this
 *                   function, no XML output was written. \n
 *                   (flags & ::AACDEC_UI_CONTINUES_XML) != 0: The XML output is a continuation of incomplete output of
 *                   the previous call and has to be appended to the output of the previous call. \n
 *                   (flags & ::AACDEC_UI_INCOMPLETE_XML) != 0: The XML output is not yet complete,
 *                   at least one further call of the function is required to get the remaining part. \n
 * \return           Error code.
 */
LINKSPEC_H DETACHED_UI_ERROR
detached_UI_GetXmlSceneState( HANDLE_DETACHED_UI_MANAGER self,
                              char *xmlOut,
                              UINT xmlOutSize,
                              UINT flagsIn,
                              UINT *flagsOut);

/**
 * \brief            Send XML description of UI command to the UI manager
 *
 * \param self       UI manager handle.
 * \param xmlIn      Pointer to a buffer containing the XML command string.
 * \param xmlInSize  Size of the XML string (number of chars).
 * \param flagsOut   Pointer to bit field with flags returned by the UI manager, currently not used. \n
 * \return           Error code.
 */
LINKSPEC_H DETACHED_UI_ERROR
detached_UI_ApplyXmlAction( HANDLE_DETACHED_UI_MANAGER self,
                            const char *xmlIn,
                            UINT xmlInSize,
                            UINT *flagsOut);

LINKSPEC_H DETACHED_UI_ERROR
detached_UI_FeedMHAS( HANDLE_DETACHED_UI_MANAGER self,
                      UCHAR *mhasBuffer,
                      UINT mhasLength);

LINKSPEC_H DETACHED_UI_ERROR
detached_UI_UpdateMHAS( HANDLE_DETACHED_UI_MANAGER self,
                        UCHAR *mhasBuffer,
                        UINT mhasBufferLength,
                        UINT *mhasLength);

#ifdef __cplusplus
}
#endif

#endif
