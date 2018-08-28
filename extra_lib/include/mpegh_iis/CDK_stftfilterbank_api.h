/*
 *  CDK_stftfilterbank_api.h
 *  Copyright 2015 Fraunhofer IIS. All rights reserved.
 *
 *  Original float version by dirk.mahne@iis.fraunhofer.de , 2014/03/06
 *  CDK (fixed-point) version by Arthur Tritthart, 2015/10/16
 *
 *  $Id$
 */



#ifndef _CDK_STFT_FILTERBANK_API_H_
#define _CDK_STFT_FILTERBANK_API_H_

#include "CDK_tools_settings.h"


#include "common_fix.h"

/** STFT Filterbank handle.
    Handle to a STFT Filterbank instance. */    
typedef struct T_STFT_FILTERBANK *HANDLE_STFT_FILTERBANK;

    
/** STFT Filterbank mode, analysis, synthesis.
    STFT_FILTERBANK_MODE_INVALID         : invalid stft filterbank mode, \n
    STFT_FILTERBANK_MODE_TIME_TO_FREQ    : time to frequency transform, analysis, \n
    STFT_FILTERBANK_MODE_FREQ_TO_TIME    : frequency to time transform, synthesis */
typedef enum
{
    STFT_FILTERBANK_MODE_INVALID = 0,
    STFT_FILTERBANK_MODE_TIME_TO_FREQ,
    STFT_FILTERBANK_MODE_FREQ_TO_TIME        
} STFT_FILTERBANK_MODE;    


/** STFT Filterbank configuration.
    Comprises all input parameters to be handled to ::StftFilterbank_Open().                                \n
    \param stftFilterbankMode       STFT Filterbank mode STFT_FILTERBANK_MODE.                              \n
    \param numChans                 Number of channels.                                                     \n
    \param frameSize                Frame size, number of samples, per channel, per processing call.        \n    
    \param fftSize                  FFT size, at least 2 * frameSize, if greater, zero padding is performed \n        
    \param ph_stft_filterbank A pointer to a handle to STFT Filterbank instance.
    \return Returns 0 on success, otherwise an error code.    */    
typedef struct T_STFT_FILTERBANK_CONFIG
{    
    STFT_FILTERBANK_MODE        stftFilterbankMode;
    UINT                        numChans;
    UINT                        frameSize;
    UINT                        fftSize;
} STFT_FILTERBANK_CONFIG;


/** Open STFT Filterbank.
    Open a STFT Filterbank instance.
    \param stftFilterbankConfig A pointer to STFT Filterbank configuration.
    \param ph_stft_filterbank A pointer to a handle to STFT Filterbank instance.
    \return Returns 0 on success, otherwise an error code.    */    
int StftFilterbank_Open(
                        const STFT_FILTERBANK_CONFIG     *stftFilterbankConfig,
                        HANDLE_STFT_FILTERBANK           *ph_stft_filterbank );
    

/** STFT Filterbank processing.
    STFT Filterbank processing, transform of time signal to frequency signal, or vice versa, dependent on filterbank mode STFT_FILTERBANK_MODE.
    Time signal: multi channel (dependent on numChans), interleaved, \n
    Frequency signal: subsequent channels, Re and Im coefficients interleaved within channel \n
    \param inputBuffer Input signal, time or frequency, dependent on filterbankMode STFT_FILTERBANK_MODE.
    \param outputBuffer Output signal, time or frequency, dependent on filterbankMode STFT_FILTERBANK_MODE. \n
    For STFT synthesis, the scaled output signal is accumulated on outputBuffer.
    \param h_stft_filterbank A handle to STFT Filterbank instance.
    \return none  */        
void StftFilterbank_Process( 
                            FIXP_DBL                * RESTRICT inputBuffer,
                            FIXP_DBL                * RESTRICT outputBuffer,                            
                            HANDLE_STFT_FILTERBANK   h_stft_filterbank );

    
/** Close STFT Filterbank.
    Close STFT Filterbank instance.
    \param ph_stft_filterbank A pointer to a handle to STFT Filterbank instance.
    \return Returns 0 on success, otherwise an error code.    */            
int StftFilterbank_Close( HANDLE_STFT_FILTERBANK *ph_stft_filterbank );


/** Get STFT Filterbank configuration.
    Returns STFT Filterbank configuration.
    \param stftFilterbankConfig A pointer to STFT Filterbank configuration.    
    \param ph_stft_filterbank A pointer to a handle to STFT Filterbank instance.
    \return Returns 0 on success, otherwise an error code.    */            
int StftFilterbank_GetConfig(
                                STFT_FILTERBANK_CONFIG           *stftFilterbankConfig,
                                const HANDLE_STFT_FILTERBANK     h_stft_filterbank );


/** Display STFT Filterbank configuration.
    Display STFT Filterbank configuration.
    \param ph_stft_filterbank A pointer to a handle to STFT Filterbank instance.
    \return Returns 0 on success, otherwise an error code.    */            
int StftFilterbank_DisplayConfig( const HANDLE_STFT_FILTERBANK    h_stft_filterbank );


/** Convert STFT Filterbank error code to info string.
    Returns an information string for a given STFT Filterbank error code.
    \param stftFilterbankErrorCode  A STFT Filterbank error code, returned by API functions.
    \return Returns an info string according to error code.    */
const char *StftFilterbank_ErrorCodeToStr( const int stftFilterbankErrorCode );


#endif /* #ifndef _CDK_STFT_FILTERBANK_API_H_ */


