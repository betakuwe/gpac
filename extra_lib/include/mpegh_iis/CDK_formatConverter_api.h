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

/*
 *  File formatConverter_api.h
 *  of module formatConverter.
 *  
 */    

#ifndef _FORMATCONVERTER_API_H_
#define _FORMATCONVERTER_API_H_


#include "CDK_cicp2geometry.h"
#include "CDK_formatConverter_intern.h"


/** List of channels known to the format coverter DMX matrix generator. 
    NOTE: Keep this list in sync with implementation! */
typedef enum
{
    /* horizontal channels */
    API_CH_M_000 =      0,
    API_CH_M_L015 =     1,
    API_CH_M_R015 =     2,
    API_CH_M_L030 =     3,
    API_CH_M_R030 =     4,
    API_CH_M_L045 =     5,
    API_CH_M_R045 =     6,
    API_CH_M_L060 =     7,
    API_CH_M_R060 =     8,
    API_CH_M_L090 =     9,
    API_CH_M_R090 =     10,
    API_CH_M_L110 =     11,
    API_CH_M_R110 =     12,
    API_CH_M_L135 =     13,
    API_CH_M_R135 =     14,
    API_CH_M_L150 =     15,
    API_CH_M_R150 =     16,
    API_CH_M_180 =      17,
    /* height channels */
    API_CH_U_000 =      18,
    API_CH_U_L045 =     19,
    API_CH_U_R045 =     20,
    API_CH_U_L030 =     21,
    API_CH_U_R030 =     22,
    API_CH_U_L090 =     23,
    API_CH_U_R090 =     24,
    API_CH_U_L110 =     25,
    API_CH_U_R110 =     26,
    API_CH_U_L135 =     27,
    API_CH_U_R135 =     28,
    API_CH_U_180 =      29,
    /* top channel */
    API_CH_T_000 =      30,
    /* low channels */
    API_CH_L_000 =      31,
    API_CH_L_L045 =     32,
    API_CH_L_R045 =     33,
    /* low frequency effects */
    API_CH_LFE1 =       34,
    API_CH_LFE2 =       35,
    /* empty channel */
    API_CH_EMPTY =      -1
} api_chid_t;



      
/** Supported Format Converter input formats (IIS channel order).
 FORMAT_CONVERTER_INPUT_FORMAT_INVALID = 0       : invalid input format. \n
 FORMAT_CONVERTER_INPUT_FORMAT_5_1               : 5.1 input format (default channel order).   \n
 FORMAT_CONVERTER_INPUT_FORMAT_5_2_1 \n
 FORMAT_CONVERTER_INPUT_FORMAT_7_1 \n
 FORMAT_CONVERTER_INPUT_FORMAT_7_1_ALT \n
 FORMAT_CONVERTER_INPUT_FORMAT_10_1 \n
 FORMAT_CONVERTER_INPUT_FORMAT_22_2              : 22.2 input format (IIS channel order).  \n
 FORMAT_CONVERTER_INPUT_FORMAT_14_0              : 14.0 input format (IIS channel order).  \n
 FORMAT_CONVERTER_INPUT_FORMAT_9_1 \n
 FORMAT_CONVERTER_INPUT_FORMAT_9_0               : 9.0 input format (IIS channel order).   \n
 FORMAT_CONVERTER_INPUT_FORMAT_11_1              : 11.1 input format (IIS channel order).  \n
 FORMAT_CONVERTER_INPUT_FORMAT_4_4_0 \n
 FORMAT_CONVERTER_INPUT_FORMAT_4_4_T_0 \n
 FORMAT_CONVERTER_INPUT_FORMAT_3_0_FC \n
 FORMAT_CONVERTER_INPUT_FORMAT_3_0_RC \n
 FORMAT_CONVERTER_INPUT_FORMAT_4_0 \n
 FORMAT_CONVERTER_INPUT_FORMAT_5_0 \n
 FORMAT_CONVERTER_INPUT_FORMAT_6_1  \n
 FORMAT_CONVERTER_INPUT_FORMAT_LISTOFCHANNELS \n    
 FORMAT_CONVERTER_INPUT_FORMAT_GENERIC \n*/    



/**
   Match input/output channel configurations given by channel geometry data to channel labels 
   known to the format converter DMX matrix generator (= format converter initialization).
   \param chConfig geometry definition of setup to match
  */
INT formatConverterMatchChConfig2Channels(
        const CICP2GEOMETRY_CHANNEL_GEOMETRY     *chConfig,       /* in:   channel configuration */
        const INT                                inout,           /* in:   0: input config, 1: output config */
        const INT                                numChannels,     /* in:   number of channels in channel config */
        AFC_FORMAT_CONVERTER_CHANNEL_ID          *channel_vec,   /* out:  vector containing channel enums */
        UINT                                     *numUnknownCh,   /* out:  number of unknown channels */
        INT                                      *unknownCh_vec, /* out:  vector containing indices(=position in setup) of channels unknown to the format converter */
        INT                                      *azOffset_vec,  /* out:  vector containing azimuth offsets in degrees */
        INT                                      *elOffset_vec   /* out:  vector containing elevation offsets in degrees */
);


/**
  * Set input format or output format that is not in list of known formats.
  * Each function call either sets input _or_ output format.
  */
void formatConverterSetInOutFormat(
        IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt,
        const INT                             inout,           /* in:  0: set input format, 1: set output format */
        const INT                             num_channels,    /* in:  number of channels in setup to define */
        const AFC_FORMAT_CONVERTER_CHANNEL_ID *channel_vector  /* in:  vector containing channel enums */
);

/** Open Format Converter.
    Allocates Format Converter structs.
    \param formatConverterMode Format Converter processing mode.
    \param formatConverterInputFormat Format Converter input format.
    \param formatConverterOutputFormat Format Converter output format.
    \param samplingRate The sampling rate [Hz].
    \param frameSize format fonverter frameSize in samples for time AND frequency domain processing.
    \param numFreqBands num frequency bands for freq domain processing.
    \param numInputChans Returns/reads number of input channels. Reads in case of generic input format.
    \param numOutputChans Returns/reads number of output channels. Reads in case of generic output format.
    \param delaySamples returns the format converter delay in samples.
    \param phFormatConverter_params A pointer to a handle to Format Converter parameter struct
    \param phFormatConverter_state A pointer to a handle to Format Converter state struct.
    \return Returns 0 on success, otherwise 1.    */
INT  formatConverterOpen(    
                        const IIS_FORMATCONVERTER_MODE          formatConverterMode,  
                        IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt);


/** Initialize Format Converter.
    Perform initialization of Format Converter with given parameters.
    \param centerFrequeciesNormalized required vector with normalized center frequencies of filterbank used for frequency domain processing.
    \param azimuthDeviation optional vector with azimuth deviations of output format speakers from specified default output format speakers. In [deg]. Disable with NULL ptr.
    \param elevationDeviation optional vector with elevation deviations of output format speakers from specified default output format speakers. In [deg]. Disable with NULL ptr.
    \param distance optional vector with distances of output format speakers from sweet spot position = Radius in [m]. Disable with NULL ptr.    
    \param hFormatConverter_params A handle to Format Converter parameter struct
    \param hFormatConverter_state A handle to Format Converter state struct.    
    \return Returns 0 on success, otherwise an error code.    */
INT  formatConverterInit( IIS_FORMATCONVERTER_INTERNAL_HANDLE fcInt,
                         const FIXP_DBL                   *centerFrequeciesNormalized);


/** Format Converter processing in time domain.
    Performs time domain processing of format converter for one frame.
    \param inputBuffer inputBuffer with format (numInputChans x frameSize)
    \param outputBuffer outputBuffer with format (numOutputChans x frameSize).    
    \param fcInt A handle to Format Converter parameter struct.   
    \return Returns 0 on success, otherwise 1.    */
INT  formatConverterProcess_timeDomain( 
                                        IIS_FORMATCONVERTER_INTERNAL_HANDLE   fcInt,
                                        const DMXH_PCM *RESTRICT               inputBuffer,
                                        DMXH_PCM *RESTRICT                     outputBuffer );

/** Close Format Converter.
    Deallocates Format Converter structs.
    \param phFormatConverter_params A pointer to a handle to Format Converter parameter struct
    \param phFormatConverter_state A pointer to a handle to Format Converter state struct.  */
void  formatConverterClose(
                        IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt );

/** Postprocess DMX gains matrix. 
    Applies a threshold to MTX gains to avoid excessive use of phantom sources.
    Normalizes gains to preserve the same energy as before post-processing the matrix.
    \param dmxMtx DMX Matrix in format (numInputChans x numOutputChans).    
    \param numInputChans Number of input channels
    \param numOutputChans Number of output cannels
    \return Returns 0 on success, otherwise 1.    */
INT formatConverterPostprocessDmxMtx(
                                FIXP_DMX_H **dmxMtx,
                                const UINT numInputChans,
                                const UINT numOutputChans);

/** Add postprocessed DMX gains matrix to the downmix matrix. 
    \param dmxMtx DMX Matrix in format (numInputChans x numOutputChans).    
    \param fcInt A pointer to a handle to Format Converter state struct. */
INT  formatConverterAddDmxMtx(
                              FIXP_DMX_H **dmxMtx,
                              IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt);

/** Set Sorted DMX Matrix of Format Converter based on its DMX Matrix.
    Overwrite internal DMX Matrix with external DMX Matrix.
    \param dmxMtx DMX Matrix in format (numInputChans x numOutputChans).    
    \param hFormatConverter_params A handle to Format Converter parameter struct
    \param hFormatConverter_state A handle to Format Converter state struct.         
    \return Returns 0 on success, otherwise 1.    */
INT  formatConverterSetDmxMtx(
                                /* FIXP_DMX_H **dmxMtx, */
                                IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt);

/** Set parsed DMX Matrix to Format Converter DMX Matrix.
    Overwrite internal DMX Matrix with external DMX Matrix.
    \param dmxMtx DMX Matrix in format (numInputChans x numOutputChans).    
    \param fcInt A pointer to a handle to Format Converter state struct.    */
void formatConverterAddParsedDmxMtx(
                              FIXP_DMX_H *dmxMtxIn,
                              FIXP_DMX_H *dmxMtxOut,
                              INT numChannelsIn,
                              INT numChannelsOut);

/** Set EQs of Format Converter.
    \param eqIndex_sorted eq index for the active downmix process
    \param numEQs number of EQ definitions
    \param eqParams pointer to numEQs eqParamsStructs
    \param eqMap vector (length: numInputChans) indicating which EQ to apply to which input, 0 = no EQ
    \param grp Curent signal group
    \param bsNumSignalGroups total number of signal groups
    \param hFcInt A handle to the internal Format Converter state struct.
    \return Returns 0 on success, otherwise 1.    */
INT formatConverterSetEQs(
                          INT                                  *eqIndex_sorted,
                          UINT                                 numEQs,
                          eqParamsStruct                       *eqParams,
                          INT                                  *eqMap,
                          UINT                                 grp,
                          UINT                                 bsNumSignalGroups,
                          IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt
                          );

    
/** Return processing delay of Format Converter.
    \param mode format converter processing mode.
    \return Returns delay in samples on success, otherwise -1.  */    
INT formatConverterGetDelaySamples( IIS_FORMATCONVERTER_MODE mode );
    
/** Display Format Converter parameters.
    Displays certain Format Converter parameters.
    \param mode mode of format converter display (0: nearly silent, 1: print minimal information, 2: print all information).
    \param hFormatConverter_params A handle to Format Converter parameter struct.
    \return Returns 0 on success, otherwise 1.  */    
INT formatConverterDisplayParams( INT mode, IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt );


/** Get build info of Format Converter.
    Build date and time of Format Converter.
    \return Returns string of build date and time.  */    
const char *formatConverterGetInfo( void );   


INT setCustomDownmixParameters( IIS_FORMATCONVERTER_INTERNAL_HANDLE  fcInt, INT adaptiveEQStrength, INT phaseAlignStrength);

#endif



