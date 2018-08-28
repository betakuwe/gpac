/*****************************  iisFormatConverterLib  ***********************

                         (C) Copyright Fraunhofer IIS (2014)
                                All Rights Reserved

    $Id$
    Author(s):   Achim Kuntz, Alfonso Pino, Thomas Blender
    Description: format converter library

    This software and/or program is protected by copyright law and international
    treaties. Any reproduction or distribution of this software and/or program,
    or any portion of it, may result in severe civil and criminal penalties, and
    will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef __IIS_FORMATCONVERTERLIB_H__
#define __IIS_FORMATCONVERTERLIB_H__

#include "global_functional_range.h"


#include "CDK_cicp2geometry.h"
#include "common_fix.h"
#include "iisDRCDecLib.h"


#define AFC_MAX_IN_CHANNELS            32
#define AFC_MAX_OUT_CHANNELS           32

typedef enum
{
  IIS_FORMATCONVERTER_MODE_INVALID                          = 0,
  IIS_FORMATCONVERTER_MODE_PASSIVE_TIME_DOMAIN              = 1,
  IIS_FORMATCONVERTER_MODE_PASSIVE_FREQ_DOMAIN              = 2, 
  IIS_FORMATCONVERTER_MODE_ACTIVE_FREQ_DOMAIN_PHASE_ALIGN   = 3,
  IIS_FORMATCONVERTER_MODE_CUSTOM_FREQ_DOMAIN_PHASE_ALIGN	= 4,
  IIS_FORMATCONVERTER_MODE_ACTIVE_FREQ_DOMAIN_STFT			= 5,
  IIS_FORMATCONVERTER_MODE_CUSTOM_FREQ_DOMAIN_STFT			= 6
} IIS_FORMATCONVERTER_MODE;

typedef struct 
{
  UINT                               numLocalSpeaker;
  INT                                pas;
  INT                                aes;
  void*                              member;
  UINT                               numSignalsTotal;
  IIS_FORMATCONVERTER_MODE           fCMode;
} IIS_FORMATCONVERTER, *IIS_FORMATCONVERTER_HANDLE;

/* Passive downmix paramaters and data types: */



/* parameters struct for peak filter definition */
typedef struct {
    FIXP_DBL f;                                 /* peak frequency [Hz]     */
    INT f_e;                                    /* peak frequency [Hz] exp */
    FIXP_DBL q;                                 /* peak Q factor           */ 
    INT q_e;                                    /* peak Q factor exp       */
    FIXP_DBL g;                                 /* peak gain [dB]          */ 
    INT g_e;                                    /* peak gain [dB] exp      */ 
} pkFilterParamsStruct;

/* parameters struct for EQ definition */
typedef struct {
    INT nPkFilter;                              /* number of cascaded peak filters */
    FIXP_DBL G;                                 /* global gain [dB]                */
    INT G_e;                                    /* global gain [dB] exp            */
    pkFilterParamsStruct pkFilterParams[15];    /* pointer to nPkFilter-element array of pkFilterParamsStructs */
} eqParamsStruct;

/* parameters struct for the entire EQ configuration */
typedef struct {
  INT numEQs;                                   /* number of EQs */
  eqParamsStruct eqParams[15];                  /* array of EQs of size numEQs */
  INT eqMap[64];                                /* index map for the EQs of size inputCount */
} eqConfigStruct;


/** */
int IIS_FormatConverter_Create(
                               IIS_FORMATCONVERTER_HANDLE* self, 
                               IIS_FORMATCONVERTER_MODE mode, 
                               CICP2GEOMETRY_CHANNEL_GEOMETRY* outGeo, 
                               unsigned int numOutChannels, 
                               unsigned int sampleRate, 
                               unsigned int frameSize
                               );




/**********************************************************************//**
IIS_FormatConverter_Config_SetDeviationAngles() will set the horizontal
and vertical deviation of the output speakers. the local setup has to be known
before this function is called.

\return returns 0 for no error.
**************************************************************************/

/**********************************************************************//**
IIS_FormatConverter_Config_AddInputSetup() will add the geometric information
of another channel signal group with given channels.


\return returns 0 for no error, 1 in case of an error.
**************************************************************************/
int IIS_FormatConverter_Config_AddInputSetup(
                               IIS_FORMATCONVERTER_HANDLE       self, 
                               CICP2GEOMETRY_CHANNEL_GEOMETRY*  geo, 
                               unsigned int                     numChannels,
                               unsigned int                     channelOffset,       /**< in: offset of the first channel of the setup in the buffer handed over to process() */
                               int*                             channelId            /**< in: Array of ids - will be used to identify signals with the correpsonding downmixmatrices (if given)
                                                                                              In MPEG-H context this id is conform with the metaDataElementId. May be NULL if no downmixmatrices are handed over.*/
                               );


void IIS_FormatConverter_Config_SetAES(
                               IIS_FORMATCONVERTER_HANDLE self, 
                               UINT aes
                               );

void IIS_FormatConverter_Config_SetPAS(
                               IIS_FORMATCONVERTER_HANDLE self, 
                               UINT pas
                               );

int IIS_FormatConverter_Config_SetImmersiveDownmixFlag(
                               IIS_FORMATCONVERTER_HANDLE self,
                               UINT immersiveFlag
                               );

int IIS_FormatConverter_Config_SetRendering3DTypeFlag(
                               IIS_FORMATCONVERTER_HANDLE self,
                               UINT rendering3DtypeFlag
                               );

int IIS_FormatConverter_GetDelay( 
                                 IIS_FORMATCONVERTER_HANDLE self, 
                                 UINT* delay
                                 );

int IIS_FormatConverter_GetDmxMtx(
                                  IIS_FORMATCONVERTER_HANDLE self, 
                                  FIXP_DBL **dmxMtx
                                  );

int IIS_FormatConverter_IsOpen( IIS_FORMATCONVERTER_HANDLE self);

/**  Call this open after all _Config_ methods have been called. The FormatConverter will then initialize itself
     acording to those settings.
*/
int IIS_FormatConverter_Open(
                             IIS_FORMATCONVERTER_HANDLE self
                             );


INT IIS_FormatConverter_Process(
        IIS_FORMATCONVERTER_HANDLE self,
        HANDLE_DRC_DECODER hDrcDec,
        FIXP_DBL *deinBuffer,
        FIXP_DBL *deoutBuffer,
        const int timeBufferChannelOffset
        );

/**********************************************************************//**

\return returns 0 for no error, 1 in case of an error.
**************************************************************************/

int IIS_FormatConverter_Close(
                             IIS_FORMATCONVERTER_HANDLE* self
                             );

INT FormatConverterFrame( 
      IIS_FORMATCONVERTER_HANDLE FormatConverter, 
      HANDLE_CDK_BITSTREAM hBitStr
      );


#endif /* __IIS_FORMATCONVERTERLIB_H__ */




