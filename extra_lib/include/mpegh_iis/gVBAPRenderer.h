/******************************************************************************

                        (C) Copyright Fraunhofer IIS (2014)
                               All Rights Reserved

   $Id$
   Author(s):   Thomas Blender (fix-point version)
                Arthur Tritthart (XTENSA, ARM version)
   Description: Vector Base Amplitude Panning (VBAP)

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __GVBAPRENDERER_H__
#define __GVBAPRENDERER_H__

#include "global_functional_range.h"

#include "CDK_core.h"
#include "CDK_matrixCalloc.h"
#include "CDK_cicp2geometry.h"
#include "quickHull.h"
#include "vbap_core.h"

#include "CDK_bitstream.h"




int objectMetadataFrame(HANDLE_GVBAPRENDERER hgVBAPRenderer,
                        HANDLE_CDK_BITSTREAM bs,
                        int usacExtElementPayloadLength,
                        UCHAR lowDelayMetadataCoding,
                        UINT num_objects,
                        UCHAR hasDynamicObjectPriority,
                        UCHAR hasUniformSpread);

int objectMetadataFrameEnhanced(
        HANDLE_GVBAPRENDERER hgVBAPRenderer,
        HANDLE_CDK_BITSTREAM bs,
        UINT _num_objects,
        int independencyFlag,
        int hasDiffuseness,
        int hasCommonGroupDiffuseness,
        int hasCommonGroupExcludedSectors,
        const UCHAR useOnlyPredefinedSectors[],
        int numAlreadyAddedObjects
        );

/*
 * @brief Allocates memory for gVBAPRenderer Handle.
 *        Copy some initial data for speaker setup.
 * @param phgVBAPRenderer       Pointer to gVBAPRenderer handle
 * @param numObjects            Number of Objects which have to be rendered
 * @param frameLength           Audio frame length
 * @param oamFrameLength        Frame length of the OAM data
 * @param outGeometryInfo       Loudspeaker geometry information for output setting
 * @param outChannels           Number of real speaker including number of LFE
 */
int gVBAPRenderer_Open(HANDLE_GVBAPRENDERER* phgVBAPRenderer,
                       int numObjects,
                       int frameLength,
                       int oamFrameLength,
                       CICP2GEOMETRY_CHANNEL_GEOMETRY* outGeometryInfo,
                       int outChannels);

/*
 * @brief Render one frame with all objects from old oamStopSample to new oamStopSample.
 *        Start position is always old stop position, which calculated gains are stored in the handle
 *        First start position is the same as stop position
 * @param phgVBAPRenderer       Pointer to gVBAPRenderer handle
 * @param inputBuffer           Input PCM samples
 * @param outputBuffer          Output PCM samples
 * @param oamStopSample         Stop position of all objects
 * @param inputBufferChannelOffset Amount of samples for each channel in inputBuffer
 */
int gVBAPRenderer_RenderFrame_Time(HANDLE_GVBAPRENDERER hgVBAPRenderer,
                                   VBAP_PCM* inputBuffer,
                                   VBAP_PCM* outputBuffer,
                                   const INT startSamplePosition,
                                   const int inputBufferChannelOffset);

/*
 * @brief Free memory for gVBAPRenderer Handle.
 * @param phgVBAPRenderer       Pointer to gVBAPRenderer handle
 */
int gVBAPRenderer_Close(HANDLE_GVBAPRENDERER hgVBAPRenderer);



#endif /* __GVBAPRENDERER_H__ */
