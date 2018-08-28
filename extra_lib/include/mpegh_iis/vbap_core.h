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

#ifndef __VBAP_CORE_H__
#define __VBAP_CORE_H__

#include "cartesianMath.h"


/* #define ENABLE_NON_UNIFORM_SPREAD */

#define VBAP_PCM FIXP_DBL

#define GVBAPRENDERER_MAX_OBJECTS 32
#define GVBAPRENDERER_MAX_CHANNEL_OUT 12
#define GVBAPRENDERER_MAX_GHOST_SPEAKER 6
#define GVBAPRENDERER_MAX_OAM_FRAMES_PER_CORE_FRAME 16



/* OAM definitions */
#define OAM_NUMBER_COMPONENTS	5	          /* number of OAM components */

#define OAM_BITS_AZI  8                   /* number of quantization bits for the azimuth angle */
#define OAM_BITS_ELE  6                   /* number of quantization bits for the elevation angle */
#define OAM_BITS_RAD  4                   /* number of quantization bits for the radius (logarithmic) */
#define OAM_BITS_GAIN 7                   /* number of quantization bits for the object gain */
#define OAM_BITS_SPREAD 7                 /* number of quantization bits for the spread angle */
#define OAM_BITS_SPREAD_HEIGHT 5          /* number of quantization bits for the spread angle */
#define OAM_BITS_SPREAD_DEPTH 4           /* number of quantization bits for the spread angle */
#define OAM_BITS_OBJ_PRIORITY 3           /* number of quantization bits for the object priority */


typedef enum
{
  AZIMUTH                   = 0,
  ELEVATION                 = 1,
  RADIUS                    = 2,
  GAIN                      = 3,
  SPREAD                    = 4,
  DYNAMIC_OBJECT_PRIORITY   = 5,
  SPREAD_HEIGHT             = 6,
  SPREAD_DEPTH              = 7,
  OAM_NUMBER_MAX_COMPONENTS = 8   /* maximal number of OAM components */
} OAM_COMPONENT;

/* #define OAM_CONCEALMENT */

/* Use CDK function */
void fixp_cos_sin(FIXP_DBL x, int scale, FIXP_DBL *cos, FIXP_DBL *sin);  //TODO


#define MIN_GAIN -42949672      /* -0.01f */


typedef struct _GVBAPRENDERER* HANDLE_GVBAPRENDERER;


typedef enum {
  OBJ_CH_UNKNOWN   = -1,
  OBJ_CH_M_L030    =  0,
  OBJ_CH_M_R030    =  1,
  OBJ_CH_M_000     =  2,
  OBJ_CH_LFE1      =  3,
  OBJ_CH_M_L110    =  4,
  OBJ_CH_M_R110    =  5,
  OBJ_CH_M_L022    =  6,
  OBJ_CH_M_R022    =  7,
  OBJ_CH_M_L135    =  8,
  OBJ_CH_M_R135    =  9,
  OBJ_CH_M_180     = 10,
  OBJ_CH_RES1      = 11,
  OBJ_CH_RES2      = 12,
  OBJ_CH_M_L090    = 13,
  OBJ_CH_M_R090    = 14,
  OBJ_CH_M_L060    = 15,
  OBJ_CH_M_R060    = 16,
  OBJ_CH_U_L030    = 17,
  OBJ_CH_U_R030    = 18,
  OBJ_CH_U_000     = 19,
  OBJ_CH_U_L135    = 20,
  OBJ_CH_U_R135    = 21,
  OBJ_CH_U_180     = 22,
  OBJ_CH_U_L090    = 23,
  OBJ_CH_U_R090    = 24,
  OBJ_CH_T_000     = 25,
  OBJ_CH_LFE2      = 26,
  OBJ_CH_L_L045    = 27,
  OBJ_CH_L_R045    = 28,
  OBJ_CH_L_000     = 29,
  OBJ_CH_U_L110    = 30,
  OBJ_CH_U_R110    = 31,
  OBJ_CH_U_L045    = 32,
  OBJ_CH_U_R045    = 33,
  OBJ_CH_M_L045    = 34,
  OBJ_CH_M_R045    = 35,
  OBJ_CH_LFE3      = 36,
  OBJ_CH_M_LSCR    = 37,
  OBJ_CH_M_RSCR    = 38,
  OBJ_CH_M_LSCH    = 39,
  OBJ_CH_M_RSCH    = 40,
  OBJ_CH_M_L150    = 41,
  OBJ_CH_M_R150    = 42
} CICP2GEOMETRAY_CICP_SPEAKER_INDEX_LABEL;

/* This structure can hold a single OAM sample. */
typedef struct
{
  PointSpherical sph;
  PointCartesian cart;    
  FIXP_DBL gain;         /**< linear gain value.                            Range 0.004 ... 5.957  Exponent = 3     */
  FIXP_DBL spreadAngle;  /**< spread angle or spread width in radian.       Range  0 ... 180    mapped to 0 ... 1.0 */
  FIXP_DBL spreadHeight; /**< spread Height in radian.                      Range  0 ...  90    mapped to 0 ... 0.5 */
  FIXP_DBL spreadDepth;  /**< spread Depth.                                 Range  0 ... 15.5      Exponent = 4     */
} OAM_SAMPLE;



/*
 * @brief Contains all relevant information for one speaker
 * @param xyz       Cartesian speaker coordinates. All Values between -1 and 1 because of the spherical characteristic
 * @param sph       Spherical speaker coordinates
 * @param isLFE     true if this speaker is a LFE, false if not
 * @param isGhost   true if this speaker is a virtual ghost speaker, false if not
 */
typedef struct _SPEAKER
{
  CICP2GEOMETRAY_CICP_SPEAKER_INDEX_LABEL id;
  PointCartesian xyz;
  PointSpherical sph;
  bool isLFE;
  bool isGhost;

} SPEAKER;

/*
 * @brief holds the relevant information for each speaker triangle
 * @param triangle        the indices for the three speakers which build a speaker triangle
 * @param matrix          the matrix which holds the Cartesian speaker coordinates (x,y,z) for the three triangle speakers in each row
 * @param inverseMatrix   the inverse matrix of the matrix above
 */
typedef struct _SPEAKERTRIPLET
{
  INT triangle[3];
  PointCartesian matrix[3];
  PointCartesian inverseMatrix[3];
  INT exponentInverseMatrix;

} SPEAKERTRIPLET;

/*
 * @brief all required information about the speaker setup
 * @param speakerList     a list of all real and LFE speaker with all necessary information
 * @param speakerTriplet  a list of all speaker triangles with matrices for rendering
 * @param coversUpperHalfSphere   true if the loudspeaker setup covers the sphere above 5° elevation
 * @param coversLowerHalfSphere   true if the loudspeaker setup covers the sphere below -5° elevation
 * @param coversLowerHalfSphere   true if the loudspeaker setup has speakers above 21° elevation
 */
typedef struct _SPEAKERSETUP
{
  SPEAKER* speakerList;
  SPEAKERTRIPLET* speakerTriplet;

  int greatestInverseMatrixExponent;

  int speakerListSize;
  int speakerTripletSize;

  int* mapping;

  bool coversUpperHalfSphere;
  bool coversLowerHalfSphere;
  bool hasHeightSpeakers;

} SPEAKERSETUP;



/*
 * @brief gVBAP Handler. Holds all information about the speaker setup, the calculated start and end gains,
 *        information about the output channels and number of objects. Additional it holds some needed cache
 * @param speakerSetup      all information about the speakersetup
 * @param startGains        the calculated start gains
 * @param endGains          the calculated end gains
 * @param outputCache       cache on which all objects for one output channel is rendered. For copy to output buffer
 *                          a left shift with saturation will be done on this framelength * 32 bit cache.
 * @param baseVectors       holds the 19 basevectors which are calculated in spreading
 * @param numChannels       number of real speaker including LFE
 * @param numLFE            number of LFE
 * @param numGhosts         number of virtual speaker
 * @param numObjects        number of objects
 * @param frameLength       length of the current frame
 * @param gainCacheLength   length of the gainCache (it should be numChannels + numGhosts)
 * @param startGainsFilled  indicates whether the startGains are filled from last endGains or not (important for first run)
 */
typedef struct _GVBAPRENDERER
{
  PointCartesian baseVectors[19+1];   /* +1 for xtensa to have the possibility to do two operation at the same time */

  SPEAKERSETUP speakerSetup;

  OAM_SAMPLE* oamStartSample; /* Dim: [objects] */
  OAM_SAMPLE** oamSamples;    /* Dim: [OAM frames][objects] */

  UCHAR metadataPresent[GVBAPRENDERER_MAX_OAM_FRAMES_PER_CORE_FRAME]; /* Dim: [OAM frames] */

  int hasUniformSpread;

#ifdef OAM_CONCEALMENT
  OAM_SAMPLE* oamLastSample;  /* Dim: [objects] Used for concealment */
#endif

  UCHAR flagOamFrameOk;        /* 0: if no complete Frame was read; 1: if complete oam Data was read */

  UCHAR fixed_val[OAM_NUMBER_MAX_COMPONENTS];
  INT OAM_parsed_data[GVBAPRENDERER_MAX_OBJECTS * OAM_NUMBER_MAX_COMPONENTS]; /* 32 bit signed integer */

  FIXP_DBL** startGains; /* Dim: [objects][real channels without LFE] */
  FIXP_DBL** endGains;   /* Dim: [objects][real channels without LFE] */

  FIXP_DBL** stepState;  /* Dim: [objects][real channels without LFE] */
  FIXP_DBL** scaleState; /* Dim: [objects][real channels without LFE] */

  FIXP_DBL* gainCache;   /* Dim: [real channels without LFE but with ghost speakers] */

  FIXP_DBL** downmixMatrix;  /* Dim: [real speakers without LFE][real speaker without LFE + ghost speaker] */

  FIXP_DBL* outputCache;  /* Dim: [frame length] */

  int numChannels;
  int numLFE;
  int numGhosts;
  int numObjects;
  int frameLength;
  int oamFrameLength;
  int numOamFrames;
  int gainCacheLength;
  
  int startGainsFilled;

} GVBAPRENDERER;











/*
 * @brief use Cramer rule to calculate the inverse of the Matrix M
 *        Store the results in transposed order in the gVBAPRenderer Handle.
 * @param hgVBAPRenderer     gVBAPRenderer Handle
 */
int generateInverseMatrices(HANDLE_GVBAPRENDERER hgVBAPRenderer);



/*
 * @brief Some kind of wrapper for calculateVbapGain
 *        It checks whether a downmix matrix exists and if yes it maps the ghosts speakers to real ones
 * @param hgVBAPRenderer  gVBAPRenderer Handle
 * @param source          Cartesian coordinates of object position
 * @param final_gains     Array of gains where the result will be stored. The length of this array is the real number of speaker including LFE
 * @param gainFactor      Is a gain factor from the OAM sample
 * @param spreadAngle     Is a spread angle from the OAM sample
 */
void calculateVbap(HANDLE_GVBAPRENDERER hgVBAPRenderer, OAM_SAMPLE source, FIXP_DBL *final_gains);



/*
 * @brief Calculates gain factors for given object position.
 *         Includes OAM gain factor and OAM spread angle in calculation
 * @param hgVBAPRenderer  gVBAPRenderer Handle
 * @param source          Cartesian coordinates of object position
 * @param final_gains     Array of gains where the result will be stored. The length of this array is the real number of speaker including LFE
 * @param gainFactor      Is a gain factor from the OAM sample
 * @param spreadAngle     Is a spread angle from the OAM sample
 */
void calculateVbapGain(HANDLE_GVBAPRENDERER hgVBAPRenderer, OAM_SAMPLE source);




/*
 * @brief Calculates spread vectors
 * @param hgVBAPRenderer  gVBAPRenderer Handle
 * @param source          Cartesian coordinates of object position
 * @param spreadAngle     Is a spread angle from the OAM sample.        -180° ... 180° is mapped to -1 ... 1
 */
void calcSpreadVectors(HANDLE_GVBAPRENDERER hgVBAPRenderer, OAM_SAMPLE source);


/*
 * @brief Calculates the triangle into which the base vectors points.
 *        At same time it calculates the gains for the three loudspeakers.
 * @param hgVBAPRenderer  gVBAPRenderer Handle
 */
void calculateOneSourcePosition(HANDLE_GVBAPRENDERER hgVBAPRenderer);



void calcSpreadGains(HANDLE_GVBAPRENDERER hgVBAPRenderer, FIXP_DBL spreadAngle);



#endif

