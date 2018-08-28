/***********************************************************************************
 
 This software module was originally developed by 
 
 Fraunhofer IIS
 
 in the course of development of the ISO/IEC 23008-3 for reference purposes and its 
 performance may not have been optimized. This software module is an implementation
 of one or more tools as specified by the ISO/IEC 23008-3 standard. ISO/IEC gives 
 you a royalty-free, worldwide, non-exclusive, copyright license to copy, distribute, 
 and make derivative works of this software module or modifications  thereof for use
 in implementations or products claiming conformance to the ISO/IEC 23008-3 standard 
 and which satisfy any specified conformance criteria. Those intending to use this 
 software module in products are advised that its use may infringe existing patents. 
 ISO/IEC have no liability for use of this software module or modifications thereof. 
 Copyright is not released for products that do not conform to the ISO/IEC 23008-3 
 standard.
 
 Fraunhofer IIS retains full right to modify and use the code for its own purpose,
 assign or donate the code to a third party and to inhibit third parties from using 
 the code for products that do not conform to MPEG-related ITU Recommendations and/or 
 ISO/IEC International Standards.
 
 This copyright notice must be included in all copies or derivative works. 
 
 Copyright (c) ISO/IEC 2013.
 
 ***********************************************************************************/

#ifndef __CDK_CICP2GEOMETRY_H__
#define __CDK_CICP2GEOMETRY_H__

#include "machine_type.h"
#include "global_functional_range.h"

/** @mainpage cicp2geometryLib
*
* Converts CICP information to geometric description of the
* reproduction setup or (tries) vice versa.
* Also allows to read and write geometry description from/to
* files.
*
* @author Michael Fischer
**/

/** \file cicp2geometry.h */

#if CDK_GFR_CHANNEL_CONFIG >= CDK_GFR_CH_MPEGH_L4
  #define CICP2GEOMETRY_MPEGH_L4
#endif

#define CICP2GEOMETRY_MAX_LOUDSPEAKERS (32)
#define CICP2GEOMETRY_CICP_LOUDSPEAKER_TABLESIZE (43)

/**********************************************************************//**

**************************************************************************/

typedef enum {
  CICP2GEOMETRY_OK                 = 0,     /**< no error */
  CICP2GEOMETRY_INIT_ERROR         = -999,  /**< initialization failed */
  CICP2GEOMETRY_CONFIG_ERROR       = -998,  /**< bad configuration of the channel CICP2GEOMETRY */
  CICP2GEOMETRY_BAD_POINTER        = -997,  /**< bad pointer or corrupted memory */
  CICP2GEOMETRY_ALLOC_ERROR        = -996,  /**< memory couldn't be allocated */
  CICP2GEOMETRY_INVALID_AZIMITH    = -995,  /**< invalid azimuth angle */
  CICP2GEOMETRY_INVALID_ELEVATION  = -994,  /**< invalid elevation angle */
  CICP2GEOMETRY_INVALID_LFE_FLAG   = -993,  /**< invalid LFE flag */
  CICP2GEOMETRY_INVALID_SCREEN_RELATIVE_FLAG = -992, /**< invalid screen relative flag */
  CICP2GEOMETRY_UNKNOWN_SPEAKER_INDEX        = -991, /**< unknown cicp speaker index */
  CICP2GEOMETRY_INVALID_CICP_INDEX           = -990, /**< invalid cicp index */
  CICP2GEOMETRY_UNSUPPORTED_CHANNEL_COUNT    = -989, /**< unsupported number of channels */
  CICP2GEOMETRY_INVALID_GEO_FILE_SYNTAX      = -988, /**< invalid syntax within geometric description */
  CICP2GEOMETRY_INVALID_FILEHANDLE           = -987  /**< invalid file handle */
} CICP2GEOMETRY_ERROR;

/**********************************************************************//**

**************************************************************************/

typedef enum {
  CICP2GEOMETRY_CICP_INVALID   = -1000,  /**< Invalid CICP index */
  CICP2GEOMETRY_CICP_FROM_FILE = -1, /**< informal signaling- read geometry from file */
  CICP2GEOMETRY_CICP_VALID     = 0,  /**< start of valid range for CICP indices */
  CICP2GEOMETRY_CICP_1_0_0     = 1,  /**< mono geometry */
  CICP2GEOMETRY_CICP_2_0_0     = 2,  /**< stereo geometry */
  CICP2GEOMETRY_CICP_3_0_0     = 3,  /**< n.a. - stereo plus centre front geometry */
  CICP2GEOMETRY_CICP_3_1_0     = 4,  /**< n.a. - stereo plus centre front and rear centre geometry */
  CICP2GEOMETRY_CICP_3_2_0     = 5,  /**< n.a. - 5.1 without LFE geometry */
  CICP2GEOMETRY_CICP_3_2_1     = 6,  /**< 5.1 geometry */
  CICP2GEOMETRY_CICP_5_2_1     = 7,  /**< 7.1 front centre: 5.1 with additional left/right front centre */
  CICP2GEOMETRY_CICP_1_1       = 8,  /**< n.a. */
  CICP2GEOMETRY_CICP_2_1_0     = 9,  /**< n.a. - stereo plus rear centre geometry  */
  CICP2GEOMETRY_CICP_2_2_0     = 10, /**< n.a. - front and surround speakers */
  CICP2GEOMETRY_CICP_3_3_1     = 11, /**< n.a. - 5.1 with additional rear centre */
  CICP2GEOMETRY_CICP_3_4_1     = 12, /**< 7.1 rear surround: 5.1 with additional rear surrounds */
  CICP2GEOMETRY_CICP_11_11_2   = 13, /**< 22.2 channel mapping */
  CICP2GEOMETRY_CICP_5_2_1_ELEVATION = 14, /**< 7.1 front vertical height: 5.1 with additional front vertical height */
  CICP2GEOMETRY_CICP_5_5_2     = 15, /**< 10.2 : 7.1 rear surround with additional front vertical height, rear centre and a second LFE */
  CICP2GEOMETRY_CICP_5_4_1     = 16, /**< 10.1 : 5.1 + front vertical heights + surround vertical heights */
  CICP2GEOMETRY_CICP_6_5_1     = 17, /**< 11.1 : 5.1 + front vertical heights + surround vertical heights + centre front vertical height + top centre surround */
  CICP2GEOMETRY_CICP_6_7_1     = 18, /**< 13.1 : 7.1 rear surround + front vertical heights + surround vertical heights + centre front vertical height + top centre surround */
  CICP2GEOMETRY_CICP_7_4_1     = 19, /**< 11.1 : 7.1 rear surround + front vertical heights + surround vertical heights  */
  CICP2GEOMETRY_CICP_9_4_1     = 20  /**< 13.1 : 9.1 + 4  */

} CICP2GEOMETRY_CICP_INDEX;

/**********************************************************************//**

**************************************************************************/

typedef enum{
  CICP2GEOMETRY_LOUDSPEAKER_KNOWN   = 1, /**< Known CICP loudspeaker */
  CICP2GEOMETRY_LOUDSPEAKER_UNKNOWN = 0, /**< Unknown CICP loudspeaker */
  CICP2GEOMETRY_LOUDSPEAKER_INVALID = -1 /**< Invalid index for a CICP loudspeaker */
} CICP2GEOMETRY_LOUDSPEAKER_TYPE;

/**********************************************************************//**

**************************************************************************/
typedef struct cicp2geometry_channel_geometry {
  int cicpLoudspeakerIndex;                         /**< index specifying cicp loudspeaker. -1 if only geometry is given. */
  int Az;                                           /**< azimuth angle of the channel    */
  int El;                                           /**< elevation angle of the channel  */
  int LFE;                                          /**< flag whether channel is an LFE  */
  int screenRelative;                               /**< flag whether loudspeaker position is screen relative */
  int   hasDistance;                                /**< distance info available for loudspeaker */
  unsigned int distance;                            /**< distance value in cm */
  int   hasLoudspeakerCalibrationGain;              /**< calibration info available for loudspeaker */
  int loudspeakerCalibrationGain;                 /**< linear calibration gain */
  CICP2GEOMETRY_LOUDSPEAKER_TYPE loudspeakerType;   /**<  enum describing the loudspeaker type */
} CICP2GEOMETRY_CHANNEL_GEOMETRY;

/**********************************************************************//**
get_geometry_from_cicp() expects a cicpIndex and will define  the
CICP2GEOMETRY_CHANNEL_GEOMETRY for each channel. Additionally the number
of channels and LFEs is returned.

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_geometry_from_cicp( int              cicpIndex,   /**<  in: cicpIndex indicating the reproduction setup  */
                                            CICP2GEOMETRY_CHANNEL_GEOMETRY AzElLfe[CICP2GEOMETRY_MAX_LOUDSPEAKERS], /**< out: pointer to an array with CICP2GEOMETRY_CHANNEL_GEOMETRYs */
                                                          int *            numChannels, /**< out: number of effective output channels for chosen reproduction setup (i.e. w/o LFEs) */
                                                          int *            numLFEs      /**< out: number of LFEs in chosen reproduction setup  */
                                                        );

/**********************************************************************//**
get_numChannels_from_cicp() expects a cicpIndex and returns the number
of channels.

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_numChannels_from_cicp( int cicpIndex, int * numTotalChannels );


/**********************************************************************//**
cicp2geometry_get_cicpIndex_from_geometry() expects an array of 
CICP2GEOMETRY_CHANNEL_GEOMETRYs and will match these to a cicp loudspeaker
layout index- if there is a match.

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_cicpIndex_from_geometry( CICP2GEOMETRY_CHANNEL_GEOMETRY * AzElLfe,     /**< in: pointer to an array with CICP2GEOMETRY_CHANNEL_GEOMETRYs */
                                                               int                              numChannels, /**< in: number of effective output channels in this reproduction setup (i.e. w/o LFEs) */
                                                               int                              numLFEs,     /**< in: number of LFEs in this reproduction setup  */
                                                               int *                            cicpIndex    /**< out: cicpIndex of this reproduction setup  */
                                                             );

/**********************************************************************//**
cicp2geometry_get_geometry_from_file() expects a file specifying the geometry of the 
reproduction setup either with CICP speaker indices or plain angle description.
Will define CICP2GEOMETRY_CHANNEL_GEOMETRY for each channel. 
Additionally the number of channels and LFEs is returned.

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_geometry_from_file( char*            geometryInputFilename, /**<  in: filename of textfile containing CICP channel indices or geometry data  */
                                            CICP2GEOMETRY_CHANNEL_GEOMETRY AzElLfe[CICP2GEOMETRY_MAX_LOUDSPEAKERS],  /**< out: pointer to an array with CICP2GEOMETRY_CHANNEL_GEOMETRYs */
                                                          int *            numChannels,           /**< out: number of effective output channels for chosen reproduction setup (i.e. w/o LFEs) */
                                                          int *            numLFEs                /**< out: number of LFEs in chosen reproduction setup  */
                                                        );

/**********************************************************************//**
cicp2geometry_get_geometry_from_cicp_loudspeaker_index() expects a CICP
loudspeaker index and writes the according geometric data into the struct
of the given pointer.

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_geometry_from_cicp_loudspeaker_index( int  cicpLoudspeakerIndex, /**<  in: cicp speaker index specifying the geometric position */
                                                CICP2GEOMETRY_CHANNEL_GEOMETRY*  AzElLfe                /**< out:pointer to an array with CICP2GEOMETRY_CHANNEL_GEOMETRYs */
                                                                          );

/**********************************************************************//**
cicp2geometry_write_geometry_to_file() expects a filename  for a file in which 
the geometry of the reproduction setup will be written, either with CICP speaker 
indices or plain angle description. Additionally the number of channels and LFEs 
is written as a first entry in form of a sum.

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_write_geometry_to_file( char*            geometryOutputFilename, /**<  in: filename of textfile containing CICP channel indices or geometry data  */
                                            CICP2GEOMETRY_CHANNEL_GEOMETRY AzElLfe[CICP2GEOMETRY_MAX_LOUDSPEAKERS], /**< in: pointer to an array with CICP2GEOMETRY_CHANNEL_GEOMETRYs */
                                                          int              numChannels,            /**< in: number of effective output channels for chosen reproduction setup (i.e. w/o LFEs) */
                                                          int              numLFEs                 /**< in: number of LFEs in chosen reproduction setup  */
                                                        );

/**********************************************************************//**
cicp2geometry_compare_geometry() compares two CICP2GEOMETRY_CHANNEL_GEOMETRY 
arrays.

\return returns 0 if both geometries are equal.
**************************************************************************/
int cicp2geometry_compare_geometry(CICP2GEOMETRY_CHANNEL_GEOMETRY geoOne[CICP2GEOMETRY_MAX_LOUDSPEAKERS],
                                   unsigned int numChannelsOne,
                                   CICP2GEOMETRY_CHANNEL_GEOMETRY geoTwo[CICP2GEOMETRY_MAX_LOUDSPEAKERS],
                                   unsigned int numChannelsTwo,
                                   unsigned int tolerance);


/*********************************************************************//**
*************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_number_of_lfes(
        CICP2GEOMETRY_CHANNEL_GEOMETRY* AzElLfe, 
        UINT numChannels,
        UINT *numLfes );

/*********************************************************************//**
*************************************************************************/
CICP2GEOMETRY_ERROR cicp2geometry_get_deviation_angles(
        CICP2GEOMETRY_CHANNEL_GEOMETRY    *AzElLfe,
        UINT                              numSpeaker,
        INT                               *azDev,
        INT                               *elDev,
        INT                               *isDeviated,
        CICP2GEOMETRY_CHANNEL_GEOMETRY    *normalizedGeo);

#endif /* __CICP2GEOMETRY_H__ */
