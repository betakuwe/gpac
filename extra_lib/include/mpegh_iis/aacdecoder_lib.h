/*****************************  MPEG-4 AAC Decoder  **************************
 
                      (C) Copyright Fraunhofer IIS 1999-2008
                               All Rights Reserved
 
   $Id$
   Author(s):   Manuel Jander

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.
 
******************************************************************************/

/**
 * \file   aacdecoder_lib.h
 * \brief  CDK AAC decoder library interface header file.
 *

\page INTRO Introduction


\section SCOPE Scope

This document describes the high-level application interface and usage of the ISO/MPEG-2/4 AAC Decoder
library developed by the Fraunhofer Institute for Integrated Circuits (IIS).
Depending on the library configuration, decoding of AAC-LC (Low-Complexity),
HE-AAC (High-Efficiency AAC v1 and v2), AAC-LD (Low-Delay) and AAC-ELD (Enhanced Low-Delay) is implemented
for high performance, resource restricted embedded applications.

All references to SBR (Spectral Band Replication) are only applicable to HE-AAC and AAC-ELD
configurations of the CDK library. All references to PS (Parametric Stereo) are only applicable to
HE-AAC v2 decoder configuration of the library.

\section DecoderBasics Decoder Basics

This document can only give a rough overview about the ISO/MPEG-2, ISO/MPEG-4 AAC audio, MPEG-D USAC
and MPEG-H coding standards. To understand all details referenced in this document, you are encouraged to read
the following documents.

- ISO/IEC 13818-7 (MPEG-2 AAC) Standard, defines the syntax of MPEG-2 AAC audio bitstreams.
- ISO/IEC 14496-3 (MPEG-4 AAC, subpart 1 and 4) Standard, defines the syntax of MPEG-4 AAC audio bitstreams.
- ISO/IEC 23003-3 (MPEG-D USAC), defines MPEG-D USAC unified speech and audio codec.
- ISO-IEC 23008-3 (MPEG-H), defines MPEG-H Low Complexity profile
- Lutzky, Schuller, Gayer, Kr&auml;mer, Wabnik, "A guideline to audio codec delay", 116th AES Convention, May 8, 2004

In short, MPEG Advanced Audio Coding is based on a time-to-frequency mapping of the signal. The signal
is partitioned into overlapping time portions and transformed into frequency domain. The spectral
components are then quantized and coded using a highly efficient coding scheme.\n
Encoded MPEG-2 and MPEG-4 AAC audio bitstreams are composed of frames. Contrary to MPEG-1/2 Layer-3 (mp3),
the length of individual frames is not restricted to a fixed number of bytes, but can take any length between 1 and 768 bytes.

In addition to the above mentioned frequency domain coding mode, MPEG-D USAC also employs a time domain Algebraic
Code-Excited Linear Prediction (ACELP) speech coder core. This operating mode
is selected by the encoder in order to achieve the optimum audio quality for different content type. Several enhancements
allow achieving higher quality at lower bit rates compared to MPEG-4 HE-AAC.
 
MPEG-H comprises an audio codec based on MPEG-D USAC and a rendering solution for 3D Audio with a large number of audio signals,
but with a lower worst case computational complexity per channel. The audio signals are categorized into channel beds,
objects and HOA (higher order ambisonic) signal groups. These are rendered to any given loudspeaker configuration with
advanced loudness management and optional user interactivity.

\page LIBUSE Library Usage


\section InterfaceDescritpion API Description

All API header files are located in the folder /include of the release package. The contents of each file is described in
detail in this document. All header files are provided for usage in specific C/C++ programs. The main AAC decoder library
API functions are located in aacdecoder_lib.h header file.

In binary releases the decoder core resides in statically linkable libraries, for example libAACdec.a.


\section Calling_Sequence Calling Sequence

The following sequence is necessary for proper decoding of ISO/MPEG-2/4 AAC, HE-AAC v2 or MPEG-H LC bitstreams. In the
following description, input stream read and output write function details are left out, since they may be 
implemented in a variety of configurations depending on the user's specific requirements. The example implementation in
main.cpp uses file-based input/output, and in such case one may call mpegFileRead_Open() to open an input file and
to allocate memory for the required structures, and the corresponding mpegFileRead_Close() to close opened
files and to de-allocate associated structures. mpegFileRead_Open() will attempt to detect the bitstream format and
in case of MPEG-4 file format or Raw Packets file format (a proprietary Fraunhofer IIS file format suitable only for testing) 
it will read the Audio Specific Config data (ASC). An unsuccessful attempt to recognize the bitstream format requires the user to
provide this information manually (see \ref CommandLineUsage). For any other bitstream formats that are
usually applicable in streaming applications, the decoder itself will try to synchronize and parse the given
bitstream fragment using the CDK transport library. Hence, for streaming applications (without file access)
this step is not necessary.


-# Call aacDecoder_Open() to open and retrieve a handle to a new AAC decoder instance.
\dontinclude main.cpp
\skipline aacDecoder_Open
-# If out-of-band config data (Audio Specific Config (ASC) or Stream Mux Config (SMC)) is available, call
aacDecoder_ConfigRaw() to pass this data to the decoder before beginning the decoding process. If this data is 
not available in advance, the decoder will configure itself while decoding, during the aacDecoder_DecodeFrame()
function call.
-# Begin decoding loop.
\skipline do {
-# Read data from bitstream file or stream buffer in to the driver program working memory (a client-supplied input buffer "inBuffer" in main.cpp).
This buffer will be used to load AAC bitstream data to the decoder.  Only when all data in this buffer has been processed will the decoder signal
an empty buffer. For file-based input, you may invoke mpegFileRead_Read() to acquire new bitstream data.
-# Call aacDecoder_Fill() to fill the decoder's internal bitstream input buffer with the client-supplied bitstream input buffer. Note, if the 
data loaded in to the internal buffer is not sufficient to decode a frame, aacDecoder_DecodeFrame() will return ::AAC_DEC_NOT_ENOUGH_BITS 
until a sufficient amount of data is loaded in to the internal buffer. For streaming formats (ADTS, LOAS), it is acceptable 
to load more than one frame to the decoder. However, for RAW file format (Fraunhofer IIS proprietary format), only one frame may be loaded to the decoder per
aacDecoder_DecodeFrame() call. For least amount of communication delay, fill and decode should be performed on a frame by frame basis.
\skipline aacDecoder_Fill
-# Call aacDecoder_DecodeFrame(). This function decodes one frame and writes decoded PCM audio data to a client-supplied buffer. It is the
client's responsibility to allocate a buffer which is large enough to hold the decoded output data.
\skipline aacDecoder_DecodeFrame
If the bitstream configuration (number of channels, sample rate, frame size) is not known a priori, you may
call aacDecoder_GetStreamInfo() to retrieve a structure that contains this information. You may use this data to initialize an audio
output device. In the example main.cpp program, if the number of channels or the sample rate has changed since program
start or the previously decoded frame, the audio output device is then re-initialized. If WAVE file output
is chosen, a new WAVE file for each new stream configuration is be created.
\skipline aacDecoder_GetStreamInfo
If you support MPEG-H user interactivity, call aacDecoder_UI_GetXmlSceneState() to retrieve an XML description of
available interactivity settings and their current state. You may then call aacDecoder_UI_ApplyXmlAction() one or more
times to send interactivity commands as XML strings to the decoder. For more details see the section \ref MpegHUserInteractivity.
-# Repeat steps 5 to 7 until no data is available to decode any more, or in case of error.
\skipline } while
-# Call aacDecoder_Close() to de-allocate all AAC decoder and transport layer structures.
\skipline aacDecoder_Close

\image latex decode.png "Decode calling sequence" width=11cm

\image latex change_source.png "Change data source sequence" width 5cm

\image latex conceal.png "Error concealment sequence" width=14cm

\section BufferSystem Buffer System

There are three main buffers in an AAC decoder application. One external input buffer to hold bitstream
data from file I/O or elsewhere, one decoder-internal input buffer, and one to hold the decoded output
PCM sample data. In resource limited applications, the output buffer may be reused as an external input buffer
prior to the subsequence aacDecoder_Fill() function call.

The external input buffer is set in the example program main.cpp and its size is defined by ::IN_BUF_SIZE.
You may freely choose different buffer sizes. To feed the data to the decoder-internal input buffer, use the
function aacDecoder_Fill(). This function returns important information regarding the number of bytes in the
external input buffer that have not yet been copied into the internal input buffer (variable bytesValid).
Once the external buffer has been fully copied, it can be completely re-filled again.
In case you wish to refill the buffer while there are unprocessed bytes (bytesValid is unequal 0), you
should preserve the unconsumed data. However, we recommend to refill the buffer only when bytesValid returns 0.

The bytesValid parameter is an input and output parameter to the CDK decoder. As an input, it signals how many valid 
bytes are available in the external buffer. After consumption of the external buffer using aacDecoder_Fill() function,
the bytesValid parameter indicates if any of the bytes in the external buffer were not consumed.

\image latex dec_buffer.png "Life cycle of the external input buffer" width=9cm

The size of the decoder-internal input buffer is set in tpdec_settings.h (see define ::TRANSPORTDEC_INBUF_SIZE).
You may choose a smaller size under the following considerations:

- each input channel requires 768 bytes
- the whole buffer must be of size 2^n

So for example a stereo decoder:

\f[
TRANSPORTDEC\_INBUF\_SIZE = 2 * 768 = 1536 => 2048
\f]

Please note that tpdec_settings.h and TRANSPORTDEC_INBUF_SIZE are not part of the decoder's library application interface. Therefore
only source-code clients may change this setting. If you received a library release and desire to have a different buffer setting, 
please contact us so we may change this setting in order to meet your memory requirements.

\section MPEG_H_API MPEG-H Applicable API Functionality

This section gives a brief overview of API functionality which is relevant for decoding MPEG-H bit streams.

\subsection AACDEC_PARAM AACDEC_PARAM Interface

The ::AAC_TARGET_LAYOUT_CICP parameter is required to set the desired speaker layout according to table 95 of ISO/IEC 23008-3.
Note that this parameter has a significant impact on the computational complexity of the decoder. The complexity increases with the number of rendered output channels.
In case the device (e.g. a TV set) has only stereo speakers, and no means of providing output to any other device with more than 2 speakers,
then a stereo target layout is sufficient for the worst case complexity assessment.
If the device has 5.1 speaker connectors or an output e.g. via HDMI or HDMI-ARC to a connected soundbar or AVR, then a 5.1 channel or even 7.1 channel target layout is the limit.
This means that significantly less speakers need to be rendered than the overall theoretical worst case number of channels depending on the Profile Level supported by the decoder.
E.g. for MPEG-H Low Complexity Profile Level 3 the maximum number of supported output channels by the decoder is 12.

The ::AAC_UNIDRC_TARGET_LOUDNESS parameter is required to set the desired target loudness. The value depends on the device. For TV sets its -24dB FS, for AVR -31dB FS and for mobile devices -16dB FS.

The parameters ::AAC_CONCEAL_FADEOUT_SLOPE, ::AAC_CONCEAL_FADEIN_SLOPE and ::AAC_CONCEAL_MUTE_RELEASE are applicable. The default values are recommended.

All other parameters are not applicable for MPEG-H.

\subsection CStreamInfo CStreamInfo

The variables sampleRate, frameSize and numChannels define the decoders PCM output. The variable mpeghAUSize may be required by the decoder framework.
pChannelType and pChannelIndices are not applicable to MPEG-H.

All other variables have informational character and might be undefined for the MPEG-H decoding context.

\subsection Functions Functions

aacDecoder_AncDataInit() and aacDecoder_AncDataGet() are not applicable.

aacDecoder_SetParam() and aacDecoder_SetParamVector() are applicable with a subset of the parameters listed above.

The following API functions are generic and also apply to MPEG-H decoding: 

aacDecoder_GetFreeBytes()

aacDecoder_Open()            relevant TRANSPORT_TYPE: TT_MHAS, TT_MHAS_PACKETIZED, TT_MHA_RAW

aacDecoder_ConfigRaw()       relevant for TT_MHA_RAW

aacDecoder_GetFlushStatus()

aacDecoder_Fill()

aacDecoder_DecodeFrame()

aacDecoder_Close()

aacDecoder_GetStreamInfo()

aacDecoder_GetLibInfo()

aacDecoder_UI_GetXmlSceneState() and aacDecoder_UI_ApplyXmlAction() are MPEG-H specific functions for user interaction (communication with GUI).

\page MemoryConsiderations Memory Considerations

In resource constrained environments it is necessary to keep CPU, RAM and ROM usage to a minimum.
If you require the decoder to consume as few resources as possible, there are several possibilities to
reduce the overall memory consumption.

\section DecoderBelongings What Belongs To The Decoder?

First it is important to understand that the entire libSYS does not belong to the core decoder, as it is
merely a handy library for system integration, debugging and a residence for a few global symbols such as
in CDK_audio.h.

In genericStds.h there are some references to cos()/sin()/log()/... functions which the linker might not
always be able to identify them as unnecessary and schedule for truncation. Hence, please ensure that the binary
you build does not contain any of such routines.

Additionally, with libSYS you may measure the stack and workload consumption of any decoder configuration
you choose. This information can help you adapt your stack and heap memory settings accordingly.

Furthermore, libMpegFileRead is not part of the core decoder. It is responsible for file access for testing
and demo.

\section ChoosingTheOptimalSettings Choosing The Optimal Settings

Each CDK library contains a settings file, which allow to exclude certain features and code sections that are
not required by a certain project. These settings are used for compile-time configuration only and cannot
be modified in library releases. If you received object code, then these settings have already been
optimized according to the delivery agreement specification.

CDK_tools_settings.h: contains the configuration of libCDK (for example defines CDK_TOOLS_AAC_LC_DEC to
only include libCDK functions and data required for an AAC LC decoder).
Here we have an AND relation which means several of these defines may be set to include the
respective code parts. Multiple definitions may occur, although this has no negative side effect.

aacdec_settings.h: choose the type of decoder (for example define ::DECODER_TYPE as AAC_LC to build a
decoder only capable of decoding AAC LC). Here we have an OR relation which means ::DECODER_TYPE
can only have one value.

tpdec_settings.h: specify the desired support for bitstream formats. If you choose only Raw Packets
File Format (simple concatenation of Access Units and a preceding size field) you can save some code
size by removing the following defines:

\verbatim
#define TPDEC_ADIF_ENABLE
#define TPDEC_ADTS_ENABLE
#define TPDEC_CRC_ENABLE
#define TPDEC_LOAS_ENABLE
#define TPDEC_LATM_ENABLE
#define TPDEC_DRM_ENABLE
\endverbatim

Additionally the support for MPEG-4 File Format can also be removed by undefining ::MP4FF_ENABLE in
mpegFileRead.h.


\subsection SUPPFRAMM Supported Frame lengths

For MPEG-4 AAC-LC and HE-AAC decoders, two frame lengths are available (1024 and 960). If you do not need frame length 960 support, 
you can save 20 kB of ROM in libCDK and about 3 kB of code in the decoder library. To do so,
undefine the following defines in CDK_tools_settings.h:

\verbatim
#define MDCT120_ENABLE
#define MDCT960_ENABLE
#define INCLUDE_KBDWindow120
#define INCLUDE_KBDWindow960
\endverbatim

and in aacdec_settings.h undefine ::USE_960_TABLES.

For MPEG-H decoders, the core codec only supports frame length 1024. Due to internal resampling a larger output frame size can occur. 

Again this is only available in source-code releases.

\page OutputFormat Decoder audio output

\section OutputFormatObtaining Obtaining channel mapping information

The decoded audio output format is indicated by a set of variables of the CStreamInfo structure.
While the struct members sampleRate, frameSize and numChannels might be self explanatory,
pChannelType and pChannelIndices require some further explanation.

These two arrays indicate the configuration of channel data within the output buffer. Both arrays have
CStreamInfo::numChannels number of cells. Each cell of pChannelType indicates the channel type, which is described in
the enum ::AUDIO_CHANNEL_TYPE (defined in CDK_audio.h). The cells of pChannelIndices indicate the sub index
among the channels starting with 0 among channels of the same audio channel type.

The indexing scheme is structured as defined in MPEG-2/4 Standards. Indices start from the front
direction (a center channel if available, will always be index 0) and increment, starting 
with the left side, pairwise (e.g. L, R) and from front to back (Front L, Front R, Surround L, Surround R). 
For detailed explanation, please refer to ISO/IEC 13818-7:2005(E), chapter 8.5.3.2.

In case a Program Config is included in the audio configuration, the channel mapping described within
it will be adopted.

In case of MPEG-D Surround the channel mapping will follow the same criteria described in ISO/IEC 13818-7:2005(E),
but adding corresponding top channels (if available) to the channel types in order to avoid ambiguity. 
The examples below explain these aspects in detail.

\section OutputFormatChange Changing the audio output format

For MPEG-4 audio the channel order can be changed at runtime through the parameter 
::AAC_PCM_OUTPUT_CHANNEL_MAPPING. See the description of those
parameters and the decoder library function aacDecoder_SetParam() for more detail.

\section OutputFormatExample Channel mapping examples

The following examples illustrate the location of individual audio samples in the audio buffer that
is passed to aacDecoder_DecodeFrame() and the expected data in the CStreamInfo structure which can be obtained
by calling aacDecoder_GetStreamInfo().

\subsection ExamplesStereo Stereo

In case of ::AAC_PCM_OUTPUT_CHANNEL_MAPPING set to 1,
a AAC-LC bit stream which has channelConfiguration = 2 in its audio specific config would lead
to the following values in CStreamInfo:

CStreamInfo::numChannels = 2

CStreamInfo::pChannelType = { ::ACT_FRONT, ::ACT_FRONT }

CStreamInfo::pChannelIndices = { 0, 1 }

The output buffer will be formatted as follows:

\verbatim
  <left sample 0>  <left sample 1>  <left sample 2>  ... <left sample N>
  <right sample 0> <right sample 1> <right sample 2> ... <right sample N>
\endverbatim

Where N equals to CStreamInfo::frameSize .

\subsection ExamplesSurround Surround 5.1

In case of ::AAC_PCM_OUTPUT_CHANNEL_MAPPING set to 1,
a AAC-LC bit stream which has channelConfiguration = 6 in its audio specific config, would lead
to the following values in CStreamInfo:

CStreamInfo::numChannels = 6

CStreamInfo::pChannelType = { ::ACT_FRONT, ::ACT_FRONT, ::ACT_FRONT, ::ACT_LFE, ::ACT_BACK, ::ACT_BACK }

CStreamInfo::pChannelIndices = { 1, 2, 0, 0, 0, 1 }

Since ::AAC_PCM_OUTPUT_CHANNEL_MAPPING is 1, WAV file channel ordering will be used. For a 5.1 channel
scheme, thus the channels would be: front left, front right, center, LFE, surround left, surround right.
Thus the third channel is the center channel, receiving the index 0. The other front channels are
front left, front right being placed as first and second channels with indices 1 and 2 correspondingly.
There is only one LFE, placed as the fourth channel and index 0. Finally both surround
channels get the type definition ACT_BACK, and the indices 0 and 1.

The output buffer will be formatted as follows:

\verbatim
<front left sample 0> <front right sample 0>
<center sample 0> <LFE sample 0>
<surround left sample 0> <surround right sample 0>

<front left sample 1> <front right sample 1>
<center sample 1> <LFE sample 1>
<surround left sample 1> <surround right sample 1>

...

<front left sample N> <front right sample N>
<center sample N> <LFE sample N>
<surround left sample N> <surround right sample N>
\endverbatim

Where N equals to CStreamInfo::frameSize .

\subsection ExamplesArib ARIB coding mode 2/1

In case of ::AAC_PCM_OUTPUT_CHANNEL_MAPPING set to 1,
in case of a ARIB bit stream using coding mode 2/1 as described in ARIB STD-B32 Part 2 Version 2.1-E1, page 61,
would lead to the following values in CStreamInfo:

CStreamInfo::numChannels = 3

CStreamInfo::pChannelType = { ::ACT_FRONT, ::ACT_FRONT, ::ACT_BACK }

CStreamInfo::pChannelIndices = { 0, 1, 0 }

The audio channels will be placed as follows in the audio output buffer:

\verbatim
<front left sample 0> <front right sample 0>  <mid surround sample 0>

<front left sample 1> <front right sample 1> <mid surround sample 1>

...

<front left sample N> <front right sample N> <mid surround sample N>

Where N equals to CStreamInfo::frameSize .

\endverbatim

\section MpegHUserInteractivity MPEG-H User Interactivity

MPEG-H supports user interactivity, e.g. turning on/off or changing gain of certain audio tracks.
The decoder library includes functions to query information about the current audio scene and available
interactivity settings, and to send interactivity commands to the decoder. These functions make use of
XML strings for exchanging UI data with the decoder. The XML format is described in detail separately.

The function aacDecoder_UI_GetXmlSceneState() retrieves an XML description of the current audio scene and
all available interactivity parameters as defined in the bitstream. This function has to be called in the
decoding thread regularly after each call of aacDecoder_DecodeFrame(). The client has to supply a buffer
to which the XML string is written. If no change has occurred since the last call of the function, an
empty string is returned and the flag ::AACDEC_UI_NO_CHANGE is set in the output flags (the input flag
::AACDEC_UI_FORCE_UPDATE can be used to force output of the XML description). If the size of the buffer
is not sufficient to hold the full XML description, as much as possible is written and further calls to
the function are required to retrieve the remaining parts. The output flags ::AACDEC_UI_CONTINUES_XML and
::AACDEC_UI_INCOMPLETE_XML indicate if the returned XML string is a continuation of previous output and
if the output is not yet complete. As certain elements of the XML description are always written as
entities, the buffer has to have a minimum size of 1024 characters to ensure success of the function.
The returned XML description can then be used to present an interface (e.g. an on-screen display) with
interactivity options to the user.
The first call after a configuration change generates only minimal XML output (containing only content ID
and configuration change flag) and indicates this by setting the output flag ::AACDEC_UI_SHORT_OUTPUT.
This allows to react to the configuration change and apply further actions before processing the full
scene description. A further call of the function generates the full XML scene description.


The function aacDecoder_UI_ApplyXmlAction() sends a single interactivity command to the decoder. The
command has to be sent as an XML string conforming to the specified format. For each command a separate
call of the function is required. Sending commands using this function allows to react to user actions.
It can also be used to restore previously saved interactivity settings by sending an appropriate
sequence of commands.

*/

#ifndef AACDECODER_LIB_H
#define AACDECODER_LIB_H

#include "machine_type.h"
#include "CDK_audio.h"

#include "genericStds.h"

/**
 * \brief  AAC decoder error codes.
 */
typedef enum {
  AAC_DEC_OK                             = 0x0000,  /*!< No error occurred. Output buffer is valid and error free. */
  AAC_DEC_INTERMEDIATE_OK                = 0x0001,  /*!< No error occurred. Output samples are not available yet. */
  AAC_DEC_OUT_OF_MEMORY                  = 0x0002,  /*!< Heap returned NULL pointer. Output buffer is invalid. */
  AAC_DEC_UNKNOWN                        = 0x0005,  /*!< Error condition is of unknown reason, or from a another module. Output buffer is invalid. */

  /* Synchronization errors. Output buffer is invalid. */
  aac_dec_sync_error_start               = 0x1000,
  AAC_DEC_TRANSPORT_SYNC_ERROR           = 0x1001,  /*!< The transport decoder had synchronization problems. Do not exit decoding. Just feed new
                                                         bitstream data. */
  AAC_DEC_NOT_ENOUGH_BITS                = 0x1002,  /*!< The input buffer ran out of bits. */
  aac_dec_sync_error_end                 = 0x1FFF,

  /* Initialization errors. Output buffer is invalid. */
  aac_dec_init_error_start               = 0x2000,
  AAC_DEC_INVALID_HANDLE                 = 0x2001,  /*!< The handle passed to the function call was invalid (NULL). */
  AAC_DEC_UNSUPPORTED_AOT                = 0x2002,  /*!< The AOT found in the configuration is not supported. */
  AAC_DEC_UNSUPPORTED_FORMAT             = 0x2003,  /*!< The bitstream format is not supported.  */
  AAC_DEC_UNSUPPORTED_ER_FORMAT          = 0x2004,  /*!< The error resilience tool format is not supported. */
  AAC_DEC_UNSUPPORTED_EPCONFIG           = 0x2005,  /*!< The error protection format is not supported. */
  AAC_DEC_UNSUPPORTED_MULTILAYER         = 0x2006,  /*!< More than one layer for AAC scalable is not supported. */
  AAC_DEC_UNSUPPORTED_CHANNELCONFIG      = 0x2007,  /*!< The channel configuration (either number or arrangement) is not supported. */
  AAC_DEC_UNSUPPORTED_SAMPLINGRATE       = 0x2008,  /*!< The sample rate specified in the configuration is not supported. */
  AAC_DEC_INVALID_SBR_CONFIG             = 0x2009,  /*!< The SBR configuration is not supported. */
  AAC_DEC_SET_PARAM_FAIL                 = 0x200A,  /*!< The parameter could not be set. Either the value was out of range or the parameter does
                                                         not exist. */
  AAC_DEC_NEED_TO_RESTART                = 0x200B,  /*!< The decoder needs to be restarted, since the required configuration change cannot be
                                                         performed. */
  AAC_DEC_OUTPUT_BUFFER_TOO_SMALL        = 0x200C,  /*!< The provided output buffer is too small. */
  AAC_DEC_INVALID_FORMATCONVERTERCONFIG  = 0x200D,  /*!< Invalid configuration for the format converter. */
  aac_dec_init_error_end                 = 0x2FFF,

  /* Decode errors. Output buffer is valid but concealed. */
  aac_dec_decode_error_start             = 0x4000,
  AAC_DEC_TRANSPORT_ERROR                = 0x4001,  /*!< The transport decoder encountered an unexpected error. */
  AAC_DEC_PARSE_ERROR                    = 0x4002,  /*!< Error while parsing the bitstream. Most probably it is corrupted, or the system crashed. */
  AAC_DEC_UNSUPPORTED_EXTENSION_PAYLOAD  = 0x4003,  /*!< Error while parsing the extension payload of the bitstream. The extension payload type
                                                         found is not supported. */
  AAC_DEC_DECODE_FRAME_ERROR             = 0x4004,  /*!< The parsed bitstream value is out of range. Most probably the bitstream is corrupt, or
                                                         the system crashed. */
  AAC_DEC_CRC_ERROR                      = 0x4005,  /*!< The embedded CRC did not match. */
  AAC_DEC_INVALID_CODE_BOOK              = 0x4006,  /*!< An invalid codebook was signaled. Most probably the bitstream is corrupt, or the system
                                                         crashed. */
  AAC_DEC_UNSUPPORTED_PREDICTION         = 0x4007,  /*!< Predictor found, but not supported in the AAC Low Complexity profile. Most probably the
                                                         bitstream is corrupt, or has a wrong format. */
  AAC_DEC_UNSUPPORTED_CCE                = 0x4008,  /*!< A CCE element was found which is not supported. Most probably the bitstream is corrupt, or
                                                         has a wrong format. */
  AAC_DEC_UNSUPPORTED_LFE                = 0x4009,  /*!< A LFE element was found which is not supported. Most probably the bitstream is corrupt, or
                                                         has a wrong format. */
  AAC_DEC_UNSUPPORTED_GAIN_CONTROL_DATA  = 0x400A,  /*!< Gain control data found but not supported. Most probably the bitstream is corrupt, or has
                                                         a wrong format. */
  AAC_DEC_UNSUPPORTED_SBA                = 0x400B,  /*!< SBA found, but currently not supported in the BSAC profile. */
  AAC_DEC_TNS_READ_ERROR                 = 0x400C,  /*!< Error while reading TNS data. Most probably the bitstream is corrupt or the system
                                                         crashed. */
  AAC_DEC_RVLC_ERROR                     = 0x400D,  /*!< Error while decoding error resilient data. */
  aac_dec_decode_error_end               = 0x4FFF,

  /* User interactivity errors */
  aac_dec_ui_error_start                 = 0x5000,
  AAC_DEC_UI_XML_PARSE_ERROR             = 0x5001,  /*!< The XML string could not be parsed. The command was not executed. */
  AAC_DEC_UI_INVALID_PARAM               = 0x5002,  /*!< A function call or XML string contained an invalid parameter. */
  AAC_DEC_UI_NOT_ALLOWED                 = 0x5003,  /*!< The UI command is not allowed and was not executed. */
  aac_dec_ui_error_end                   = 0x50FF,  

  /* Ancillary data errors. Output buffer is valid. */
  aac_dec_anc_data_error_start           = 0x8000,
  AAC_DEC_ANC_DATA_ERROR                 = 0x8001,  /*!< Non severe error concerning the ancillary data handling. */
  AAC_DEC_TOO_SMALL_ANC_BUFFER           = 0x8002,  /*!< The registered ancillary data buffer is too small to receive the parsed data. */
  AAC_DEC_TOO_MANY_ANC_ELEMENTS          = 0x8003,  /*!< More than the allowed number of ancillary data elements should be written to buffer. */
  aac_dec_anc_data_error_end             = 0x8FFF

} AAC_DECODER_ERROR;


/** Macro to identify initialization errors. */
#define IS_INIT_ERROR(err)   ( (((err)>=aac_dec_init_error_start)   && ((err)<=aac_dec_init_error_end))   ? 1 : 0)
/** Macro to identify decode errors. */
#define IS_DECODE_ERROR(err) ( (((err)>=aac_dec_decode_error_start) && ((err)<=aac_dec_decode_error_end)) ? 1 : 0)
/** Macro to identify if the audio output buffer contains valid samples after calling aacDecoder_DecodeFrame(). */
#define IS_OUTPUT_VALID(err) ( ((err) == AAC_DEC_OK) || IS_DECODE_ERROR(err) )

/*! \enum  AAC_MD_PROFILE
 *  \brief The available metadata profiles which are mostly related to downmixing. The values define the arguments
 *         for the use with parameter ::AAC_METADATA_PROFILE.
 */
typedef enum
{
  AAC_MD_PROFILE_MPEG_STANDARD    = 0,  /*!< The standard profile creates a mixdown signal based on the advanced
                                             downmix metadata (from a DSE). The equations and default values are
                                             defined in ISO/IEC 14496:3 Ammendment 4. Any other (legacy) downmix
                                             metadata will be ignored. No other parameter will be modified.         */
  AAC_MD_PROFILE_MPEG_LEGACY      = 1,  /*!< This profile behaves identical to the standard profile if advanced
                                             downmix metadata (from a DSE) is available. If not, the matrix_mixdown
                                             information embedded in the program configuration element (PCE) will be
                                             applied. If neither is the case, the module creates a mixdown using the
                                             default coefficients as defined in ISO/IEC 14496:3 AMD 4. The profile
                                             can be used to support legacy digital TV (e.g. DVB) streams.           */
  AAC_MD_PROFILE_MPEG_LEGACY_PRIO = 2,  /*!< Similar to the ::AAC_MD_PROFILE_MPEG_LEGACY profile but if both the
                                             advanced (ISO/IEC 14496:3 AMD 4) and the legacy (PCE) MPEG downmix
                                             metadata are available the latter will be applied.                     */
  AAC_MD_PROFILE_ARIB_JAPAN       = 3   /*!< Downmix creation as described in ABNT NBR 15602-2. But if advanced
                                             downmix metadata (ISO/IEC 14496:3 AMD 4) is available it will be
                                             preferred because of the higher resolutions. In addition the metadata
                                             expiry time will be set to the value defined in the ARIB standard
                                             (see ::AAC_METADATA_EXPIRY_TIME).                                      */
} AAC_MD_PROFILE;

/*! \enum  AAC_DRC_DEFAULT_PRESENTATION_MODE_OPTIONS
 *  \brief Options for handling of DRC parameters, if presentation mode is not indicated in bitstream
 */
typedef enum
{
  AAC_DRC_PARAMETER_HANDLING_DISABLED      = -1,  /*!< DRC parameter handling disabled, all parameters are applied as requested. */
  AAC_DRC_PARAMETER_HANDLING_ENABLED       =  0,  /*!< Apply changes to requested DRC parameters to prevent clipping. */
  AAC_DRC_PRESENTATION_MODE_1_DEFAULT      =  1,  /*!< Use DRC presentation mode 1 as default (e.g. for Nordig) */
  AAC_DRC_PRESENTATION_MODE_2_DEFAULT      =  2   /*!< Use DRC presentation mode 2 as default (e.g. for DTG DBook) */
} AAC_DRC_DEFAULT_PRESENTATION_MODE_OPTIONS;


/**
 * \brief AAC decoder setting parameters
 */
typedef enum
{
  AAC_PCM_OUTPUT_CHANNEL_MAPPING          = 0x0003,  /*!< Output buffer channel ordering. 0: MPEG PCE style order, 1: WAV file channel order (default). */
  AAC_PCM_LIMITER_ENABLE                  = 0x0010,  /*!< Enable signal level limiting. \n
                                                          -1: Auto-config. Enable limiter for all non-lowdelay configurations by default. \n
                                                           0: Disable limiter in general. \n
                                                           1: Enable limiter always.
                                                          It is recommended to call the decoder with a AACDEC_CLRHIST flag to reset all states when
                                                          the limiter switch is changed explicitly. */
  AAC_PCM_LIMITER_ATTACK_TIME             = 0x0011,  /*!< Signal level limiting attack time in ms.
                                                          Default configuration is 15 ms. Adjustable range from 1 ms to 15 ms. */
  AAC_PCM_LIMITER_RELEAS_TIME             = 0x0012,  /*!< Signal level limiting release time in ms.
                                                          Default configuration is 50 ms. Adjustable time must be larger than 0 ms. */

  AAC_METADATA_PROFILE                    = 0x0020,  /*!< See ::AAC_MD_PROFILE for all available values. */
  AAC_METADATA_EXPIRY_TIME                = 0x0021,  /*!< Defines the time in ms after which all the bitstream associated meta-data (DRC, downmix
                                                          coefficients, ...) will be reset to default if no update has been received. Negative values
                                                          disable the feature. */

  AAC_CONCEAL_METHOD                      = 0x0100,  /*!< Error concealment: Processing method. \n
                                                          0: Spectral muting. \n
                                                          1: Noise substitution (see ::CONCEAL_NOISE). \n
                                                          2: Energy interpolation (adds additional signal delay of one frame, see ::CONCEAL_INTER). \n */
  AAC_CONCEAL_FADEOUT_SLOPE               = 0x0101,  /*!< Error concealment: Length of fade-out slope (in frames). Default is 5 frames. */
  AAC_CONCEAL_FADEIN_SLOPE                = 0x0102,  /*!< Error concealment: Length of fade-in slope (in frames). Default is 5 frames. */
  AAC_CONCEAL_MUTE_RELEASE                = 0x0103,  /*!< Error concealment: Number of error free frames before signal fades in again. Default is 0
                                                          frames. */
  AAC_CONCEAL_COMFORT_NOISE_LEVEL         = 0x0104,  /*!< Error concealment: Level of comfort noise inserted after fade-out (0.25 dB steps). The
                                                          value -1 switches off comfort noise; */
  AAC_CONCEAL_FADEOUT_ATTENUATION_VECTOR  = 0x0105,  /*!< Error concealment: Vector of attenuation gain factors for fade-out in dB, unsigned 8
                                                          significant bits stored in INT. */
  AAC_CONCEAL_FADEIN_ATTENUATION_VECTOR   = 0x0106,  /*!< Error concealment: Vector of attenuation gain factors for fade-in in dB, unsigned 8
                                                          significant bits stored in INT. */

  AAC_DRC_BOOST_FACTOR                    = 0x0200,  /*!< Dynamic Range Control: Scaling factor for boosting gain values.
                                                          Defines how the boosting DRC factors (conveyed in the bitstream) will be applied to the
                                                          decoded signal. The valid values range from 0 (don't apply boost factors) to 127 (fully
                                                          apply boost factors). Default value is 0. */
  AAC_DRC_ATTENUATION_FACTOR              = 0x0201,  /*!< Dynamic Range Control: Scaling factor for attenuating gain values. Same as
                                                          ::AAC_DRC_BOOST_FACTOR but for attenuating DRC factors. */
  AAC_DRC_REFERENCE_LEVEL                 = 0x0202,  /*!< Dynamic Range Control: Target reference level. Defines the level below full-scale
                                                          (quantized in steps of 0.25dB) to which the output audio signal will be normalized to by
                                                          the DRC module. The valid values range from 0 (full-scale) to 127 (31.75 dB below
                                                          full-scale). Any value smaller than 0 switches off normalization. By default, normalization 
                                                          is switched off. */
  AAC_DRC_HEAVY_COMPRESSION               = 0x0203,  /*!< Dynamic Range Control: En-/Disable DVB specific heavy compression (aka RF mode).
                                                          If set to 1, the decoder will apply the compression values from the DVB specific ancillary
                                                          data field. At the same time the MPEG-4 Dynamic Range Control tool will be disabled. By
                                                          default, heavy compression is disabled. */
  AAC_DRC_DEFAULT_PRESENTATION_MODE       = 0x0204,  /*!< Dynamic Range Control: Default presentation mode (DRC parameter handling). \n
                                                          Defines the handling of the DRC parameters boost factor, attenuation factor
                                                          and heavy compression, if no presentation mode is indicated in the bitstream.\n
                                                          For options, see ::AAC_DRC_DEFAULT_PRESENTATION_MODE_OPTIONS.\n 
                                                          Default: ::AAC_DRC_PARAMETER_HANDLING_DISABLED */
  AAC_DRC_ENC_TARGET_LEVEL                = 0x0205,  /*!< Dynamic Range Control: Encoder target level for light (i.e. not heavy) compression.\n
                                                          If known, this declares the target reference level that was assumed at the encoder for calculation 
                                                          of limiting gains. The valid values range from 0 (full-scale) to 127 (31.75 dB below full-scale).
                                                          This parameter is used only with ::AAC_DRC_PARAMETER_HANDLING_ENABLED and ignored otherwise.\n
                                                          Default: 127 (worst-case assumption).\n */
  AAC_QMF_LOWPOWER                        = 0x0300,  /*!< Quadrature Mirror Filter (QMF) Bank processing mode. \n
                                                          -1: Use internal default. Implies MPEG Surround partially complex accordingly. \n
                                                           0: Use complex QMF data mode. \n
                                                           1: Use real (low power) QMF data mode. \n */
  AAC_QMF_RESAMPLER                       = 0x0301,  /*!< Quadrature Mirror Filter (QMF) Resampler. \n
                                                           1: Active resampler for only core bitstreams (resampler standalone), core + SBR, core + MPS + SBR or core + SBR + MPS \n
                                                           0: Inactive resampler. \n */

  AAC_SAOC_ENABLE                         = 0x0400,  /*!< MPEG-D SAOC: enable payload decoding */

  AAC_MPEGS_ENABLE                        = 0x0500,  /*!< MPEG Surround: Allow/Disable decoding of MPS content. Available only for decoders with MPEG
                                                          Surround support. */

  AAC_MPEGS_BINAURAL_ENABLE               = 0x0501,  /*!< MPEG Surround: En-/disable binaural processing of MPS content for headphone playback.
                                                          Available only for decoders with MPEG Surround support.
                                                          NOTE: If set to 1 the mix-down (::AAC_PCM_MAX_OUTPUT_CHANNELS) and the channel extension
                                                                feature (::AAC_PCM_MIN_OUTPUT_CHANNELS) will be disabled automatically. */
  AAC_MPEGS_BINAURAL_QUALITY              = 0x0504,  /*!< MPEG Surround: Binaural quality (0: Low Complexity, 1: High Quality, only in combination
                                                          with complex QMF data mode) */
  AAC_MPEGS_BINAURAL_DISTANCE             = 0x0505,  /*!< MPEG Surround Binaural: Perceived distance of virtual speakers. */
  AAC_MPEGS_BINAURAL_DIALOG_CLARITY       = 0x0506,  /*!< MPEG Surround Binaural: Dialog clarity. */
  AAC_MPEGS_BINAURAL_FRONT_ANGLE          = 0x0507,  /*!< MPEG Surround Binaural: Angle of virtual front speakers. */
  AAC_MPEGS_BINAURAL_BACK_ANGLE           = 0x0508,  /*!< MPEG Surround Binaural: Angle of virtual rear speaker pair. */
  AAC_MPEGS_BINAURAL_PRESET               = 0x0509,  /*!< MPEG Surround Binaural: Virtual speaker setup and dialog clarity presets.\n
                                                          1: dry room\n
                                                          2: living room (default)\n
                                                          3: cinema\n */
  AAC_MPEGS_BLIND_MODE_ENABLE             = 0x0510,  /*!< MPEG Surround: En-/disable enhanced matrix (aka blind) mode to get multi-channel audio for
                                                          streams without MPS payload. */

  AAC_TPDEC_PARAM_MINIMIZE_DELAY          = 0x0600,  /*!< Minimize delay flag of transport decoder. Only meaningful for stream formats.
                                                          If set, the last frame of the input buffer will be used to start synchronization.  
                                                          Any previous frames will be skipped. Once synchronization was successful, no
                                                          future frames will be skipped. */
  AAC_TPDEC_PARAM_IGNORE_BUFFERFULLNESS   = 0x0601,  /*!< Applicable to LOAS/LATM or ADTS streams. It allows to ignore the buffer fullness parameter
                                                          that is part of the header there.\n
                                                          For streaming, the decoder makes use the buffer fullness parameter to make sure that the
                                                          input buffer never runs empty. The decoder will only start decoding after it has received
                                                          enough bytes to ensure that the stream is never interrupted due to a buffer underrun.\n
                                                          For packet-based scenarios, this mechanism is not necessary. Additionally, it may be
                                                          unwanted, since it may cause an additional startup delay, i.e. the decoder may return
                                                          ::AAC_DEC_NOT_ENOUGH_BITS for the first frames. To disable this behavior,
                                                          ::AAC_TPDEC_PARAM_IGNORE_BUFFERFULLNESS has to be set to 1. Therefore, the decoder will
                                                          start decoding immediately. */
  AAC_TPDEC_PARAM_EARLY_CONFIG            = 0x0602,  /*!< If the decoder is not configured, search for in band config in future frames and use that. */
  AAC_TPDEC_CLEAR_BUFFER                  = 0x0603,  /*!< Clear internal bit stream buffer of transport layers. The decoder will start decoding
                                                          at new data passed after this event and any previous data is discarded. */
  AAC_TPDEC_PARAM_SET_BITRATE             = 0x0604,  /*!< Set the bit rate, required for some internal operations (amount of lost frame estimation). */
  AAC_TPDEC_PARAM_SET_BURSTPERIOD         = 0x0605,  /*!< Set optional amount of audio data time (in milliseconds) expected for each burst data block.
                                                          This is required to calculate the decode start up (see also
                                                          ::AAC_TPDEC_PARAM_IGNORE_BUFFERFULLNESS) and the amount of lost frames in case of bit stream
                                                          errors. Set to 0 if data rate is continuous. */
  AAC_ELD_DOWNSCALE_FACTOR                = 0x0700,  /*!< Downscaled AAC-ELD decoding. In order to reduce CPU workload, the decoder offers a downscaled
                                                          decoding meaning that the output sample rate is the sample rate signaled inside
                                                          the bit stream, divided by the downscale factor. In an ideal configuration both encoder and
                                                          decoder should run with the same downscale factor. Possible values for the downscale factor
                                                          are 1, 2 or 4. */

  AAC_USAC_LP_FLAGS                       = 0x0800,  /*!<  Special non-standard conform USAC low power mode (bit field). This mode is not recommended if not absolutely required.\n
                                                          0x01 SBR low power harmonic QMF transposer.\n
                                                          0x02 Reserved bit.\n
                                                          0x04 Reserved bit.\n
                                                          0x08 Reserved bit.\n
                                                          0x10 Reserved bit.\n */

  AAC_TARGET_LAYOUT_CICP                  = 0x0900,  /*!< Target Layout index for audio output based on table 95 of ISO/IEC 23008-3.
                                                          This parameter must be set before any aacDecoder_ConfigRaw or aacDecoder_DecodeFrame call. By default the output is rendered to
                                                          the common 5.1 speaker layout.\n
                                                          The following lists all possible parameter values, the resulting number of output channels and a brief description.
                                                          Note that only a sub-set of these indices may be available depending on the supported MPEG-H Low Complexity Profile Level.
                                                          E.g. all indices resulting in more than 12 output channels require the decoder configured to support at least Profile Level 4.\n
                                                          Index | Output Channels | Description
                                                          ----: | --------------: | :-----------------
                                                          -1    |                 | output core decoder signal without rendering (for testing purposes only)
                                                           1    |  1              | mono
                                                           2    |  2              | stereo
                                                           3    |  3              | stereo plus center front
                                                           4    |  4              | stereo plus center front and rear center
                                                           5    |  5              | 5.1 without LFE
                                                           6    |  6              | 5.1
                                                           7    |  8              | 7.1 front center: 5.1 with additional left/right front center
                                                           9    |  3              | stereo plus rear center
                                                          10    |  4              | front and surround speakers
                                                          11    |  7              | 5.1 with additional rear center
                                                          12    |  8              | 7.1 rear surround: 5.1 with additional rear surrounds
                                                          13    | 24              | 22.2 channel mapping
                                                          14    |  8              | 7.1 front vertical height: 5.1 with additional front vertical height
                                                          15    | 12              | 10.2 : 7.1 rear surround with additional front vertical height, rear center and a second LFE
                                                          16    | 10              | 10.1 : 5.1 + front vertical heights + surround vertical heights
                                                          17    | 12              | 11.1 : 5.1 + front vertical heights + surround vertical heights + center front vertical height + top center
                                                          18    | 14              | 13.1 : 7.1 rear surround + front vertical heights + surround vertical heights + center front vertical
                                                          19    | 12              | 11.1 : 7.1 rear surround + front vertical heights + surround vertical heights
                                                          20    | 14              | 13.1 : 9.1 + 4
                                                          */

  AAC_TARGET_LAYOUT_GEOMETRY_VECTOR       = 0x0906,  /*!<  Target Layout defined as a vector of azimuth/elevation angle pairs in degrees and distance in centimeter.
                                                           This parameter must be set before any aacDecoder_ConfigRaw or aacDecoder_DecodeFrame call.
                                                           The data format is a series of 16 bit signed values:\n
                                                           [number of speakers N]\n
                                                           [azimuth speaker 1] [elevation speaker 1] [distance speaker 1]\n
                                                           [azimuth speaker 2] [elevation speaker 2] [distance speaker 2]\n
                                                            ...\n
                                                           [azimuth speaker N] [elevation speaker N] [distance speaker N] */
  
  AAC_UNIDRC_LOUDNESS_NORMALIZATION_ON    = 0x0901,  /*!<  MPEG-D DRC: Enable loudness normalisation. 0: Disabled, 1: Enabled (default) */
  AAC_UNIDRC_TARGET_LOUDNESS              = 0x0902,  /*!<  MPEG-D DRC: Target loudness for loudness normalisation of the audio output, e.g.:\n
                                                           -31 for audio/video receivers (AVR) or other devices allowing audio playback with high dynamic range,\n
                                                           -24 for TV sets or equivalent devices (default),\n
                                                           -16 for mobile devices where the dynamic range of audio playback is restricted.\n
                                                           Must be set before the first call of aacDecoder_DecodeFrame.*/

  AAC_UI_MANAGER_ENABLE                   = 0x0907    /*!<  Embedded UI manager:\n
                                                           1: enable embedded UI manager functionality and ignore MHAS packets\n
                                                           MHA_PACTYP_USERINTERACTION and MHA_PACTYP_LOUDNESS_DRC\n
                                                           0: Detached UI manager: disable embedded UI manager functionality and apply parameters from
                                                           MHA_PACTYP_USERINTERACTION and MHA_PACTYP_LOUDNESS_DRC MHAS packets.\n
                                                           -1: Automatically select embedded UI manager or MHAS UI control. MHAS UI control higher priority. */
} AACDEC_PARAM;

/**
 * \brief This structure gives information about the currently decoded audio data.
 *        All fields are read-only.
 */
typedef struct
{
  /* These five members are the only really relevant ones for the user.                                                            */
  INT               sampleRate;          /*!< The sample rate in Hz of the decoded PCM audio signal.                               */
  INT               frameSize;           /*!< The frame size of the decoded PCM audio signal. \n
                                              Typically this is: \n
                                              1024 or 960 for AAC-LC \n
                                              2048 or 1920 for HE-AAC (v2) \n
                                              512 or 480 for AAC-LD and AAC-ELD \n
                                              768, 1024, 2048 or 4096 for USAC \n
                                              1024, 1536, 2048 or 3072 for MPEG-H \n
                                              Other values are possible if an decoder internal resampling is performed or \n
                                              if only a fraction of a frame is available (e.g. possible for MPEG-H decoding).       */
  INT               mpeghAUSize;         /*!< The full frame size in samples of the last decoded MPEG-H access unit. This is typically
                                              1024, 1536, 2048 or 3072 depending on the core sampling rate. Other shorter sizes occur
                                              if the frame has been cut by an audio truncation message. 
                                              If CStreamInfo.frameSize is smaller than CStreamInfo.mpeghAUSize the current access unit
                                              is completed with samples from a subsequent call to aacDecoder_DecodeFrame().
                                              -1 means undefined (e.g. if the decoder returned with AAC_DEC_NOT_ENOUGH_BITS). */
  INT               numChannels;         /*!< The number of output audio channels before the rendering module (original channel configuration). */
  AUDIO_CHANNEL_TYPE *pChannelType;      /*!< Audio channel type of each output audio channel.                                     */
  UCHAR             *pChannelIndices;    /*!< Audio channel index for each output audio channel.
                                               See ISO/IEC 13818-7:2005(E), 8.5.3.2 Explicit channel mapping using a program_config_element() */
  /* Decoder internal members. */
  INT               aacSampleRate;       /*!< Sampling rate in Hz without SBR (from configuration info).                           */
  INT               profile;             /*!< MPEG-2 profile (from file header) (-1: not applicable (e. g. MPEG-4)).               */
  AUDIO_OBJECT_TYPE aot;                 /*!< Audio Object Type (from ASC): is set to the appropriate value for MPEG-2 bitstreams (e. g. 2 for AAC-LC). */
  INT               channelConfig;       /*!< Channel configuration (0: PCE defined, 1: mono, 2: stereo, ...                       */
  INT               bitRate;             /*!< Instantaneous bit rate.                   */
  INT               aacSamplesPerFrame;  /*!< Samples per frame for the AAC core (from ASC). \n
                                              1024 or 960 for AAC-LC \n
                                              512 or 480 for AAC-LD and AAC-ELD         */
  INT               aacNumChannels;      /*!< The number of audio channels after AAC core processing (before PS or MPS processing).
                                              CAUTION: This are not the final number of output channels! */
  AUDIO_OBJECT_TYPE extAot;              /*!< Extension Audio Object Type (from ASC)   */
  INT               extSamplingRate;     /*!< Extension sampling rate in Hz (from ASC) */

  UINT              outputDelay;         /*!< The number of samples the output is additionally delayed by.the decoder. */
  UINT              flags;               /*!< Copy of internal flags. Only to be written by the decoder, and only to be read externally. */

  SCHAR             epConfig;            /*!< epConfig level (from ASC): only level 0 supported, -1 means no ER (e. g. AOT=2, MPEG-2 AAC, etc.)  */

  /* Statistics */
  INT               numLostAccessUnits;  /*!< This integer will reflect the estimated amount of lost access units in case aacDecoder_DecodeFrame()
                                              returns AAC_DEC_TRANSPORT_SYNC_ERROR. It will be < 0 if the estimation failed. */

  UINT              numTotalBytes;       /*!< This is the number of total bytes that have passed through the decoder. */
  UINT              numBadBytes;         /*!< This is the number of total bytes that were considered with errors from numTotalBytes. */
  UINT              numTotalAccessUnits; /*!< This is the number of total access units that have passed through the decoder. */
  UINT              numBadAccessUnits;   /*!< This is the number of total access units that were considered with errors from numTotalBytes. */

  /* Metadata */
  SCHAR             drcProgRefLev;       /*!< DRC program reference level. Defines the reference level below full-scale.
                                              It is quantized in steps of 0.25dB. The valid values range from 0 (0 dBFS) to 127 (-31.75 dBFS).
                                              It is used to reflect the average loudness of the audio in LKFS according to ITU-R BS 1770.
                                              If no level has been found in the bitstream the value is -1. */
  SCHAR             drcPresMode;         /*!< DRC presentation mode. According to ETSI TS 101 154, this field indicates whether
                                              light (MPEG-4 Dynamic Range Control tool) or heavy compression (DVB heavy compression)
                                              dynamic range control shall take priority on the outputs.
                                              For details, see ETSI TS 101 154, table C.33. Possible values are: \n
                                              -1: No corresponding metadata found in the bitstream \n
                                               0: DRC presentation mode not indicated \n
                                               1: DRC presentation mode 1 \n
                                               2: DRC presentation mode 2 \n
                                               3: Reserved */
  SCHAR             pceMatrixMixdownIdx;     /*!< The 2 bit matrix mixdown index extracted from PCE. */
  SCHAR             pcePseudoSurroundEnable; /*!< Pseudo Surround flag extracted from PCE */ 

} CStreamInfo;


typedef struct AAC_DECODER_INSTANCE *HANDLE_AACDECODER;  /*!< Pointer to a AAC decoder instance. */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Initialize ancillary data buffer.
 *
 * \param self    AAC decoder handle.
 * \param buffer  Pointer to (external) ancillary data buffer.
 * \param size    Size of the buffer pointed to by buffer.
 * \return        Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_AncDataInit ( HANDLE_AACDECODER self,
                         UCHAR            *buffer,
                         int               size );

/**
 * \brief Get one ancillary data element.
 *
 * \param self   AAC decoder handle.
 * \param index  Index of the ancillary data element to get.
 * \param ptr    Pointer to a buffer receiving a pointer to the requested ancillary data element.
 * \param size   Pointer to a buffer receiving the length of the requested ancillary data element.
 * \return       Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_AncDataGet ( HANDLE_AACDECODER self,
                        int               index,
                        UCHAR           **ptr,
                        int              *size );

/**
 * \brief Set one single decoder parameter.
 *
 * \param self   AAC decoder handle.
 * \param param  Parameter to be set.
 * \param value  Parameter value.
 * \return       Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_SetParam ( const HANDLE_AACDECODER  self,
                      const AACDEC_PARAM       param,
                      const INT                value );

/**
 * \brief Set one vector of decoder parameters. Applies to AACDEC_PARAM with _VECTOR suffix.
 *
 * \param self    AAC decoder handle.
 * \param param   Parameter vector to be set.
 * \param pValue  Pointer to vector holding parameter values.
 * \return        Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_SetParamVector ( const HANDLE_AACDECODER  self,
                            const AACDEC_PARAM       param,
                            const void              *pValue );
/**
 * \brief              Get free bytes inside decoder internal buffer
 * \param self         Handle of AAC decoder instance
 * \param pFreeBytes   Pointer to variable receiving amount of free bytes inside decoder internal buffer
 * \return             Error code
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_GetFreeBytes ( const HANDLE_AACDECODER  self,
                                            UINT *pFreeBytes);

/**
 * \brief               Open an AAC decoder instance
 * \param transportFmt  The transport type to be used
 * \return              AAC decoder handle
 */
LINKSPEC_H HANDLE_AACDECODER 
aacDecoder_Open ( TRANSPORT_TYPE transportFmt, UINT nrOfLayers );

/**
 * \brief Explicitly configure the decoder by passing a raw AudioSpecificConfig (ASC) or a StreamMuxConfig (SMC),
 *  contained in a binary buffer. This is required for MPEG-4 and Raw Packets file format bitstreams
 *  as well as for LATM bitstreams with no in-band SMC. If the transport format is LATM with or without
 *  LOAS, configuration is assumed to be an SMC, for all other file formats an ASC.
 *
 * \param self    AAC decoder handle.
 * \param conf    Pointer to an unsigned char buffer containing the binary configuration buffer (either ASC or SMC).
 * \param length  Length of the configuration buffer in bytes.
 * \return        Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_ConfigRaw ( HANDLE_AACDECODER self,
                       UCHAR            *conf[],
                       const UINT        length[] );

/**
 * \brief Get flushing status of decoder.
 *  The function returns the flushing status of the decoder.
 *  If flushing is activ the return value is 1 otherwise it is 0.
 *
 * \param self    AAC decoder handle.
 *
 * \return        Flush status.
 */
LINKSPEC_H INT 
aacDecoder_GetFlushStatus (
        HANDLE_AACDECODER   self
        );

/**
 * \brief Fill AAC decoder's internal input buffer with bitstream data from the external input buffer.
 *  The function only copies such data as long as the decoder-internal input buffer is not full.
 *  So it grabs whatever it can from pBuffer and returns information (bytesValid) so that at a
 *  subsequent call of %aacDecoder_Fill(), the right position in pBuffer can be determined to
 *  grab the next data.
 *
 * \param self        AAC decoder handle.
 * \param pBuffer     Pointer to external input buffer.
 * \param bufferSize  Size of external input buffer. This argument is required because decoder-internally
 *                    we need the information to calculate the offset to pBuffer, where the next
 *                    available data is, which is then fed into the decoder-internal buffer (as much
 *                    as possible). Our example framework implementation fills the buffer at pBuffer
 *                    again, once it contains no available valid bytes anymore (meaning bytesValid equal 0).
 * \param bytesValid  Number of bitstream bytes in the external bitstream buffer that have not yet been
 *                    copied into the decoder's internal bitstream buffer by calling this function.
 *                    The value is updated according to the amount of newly copied bytes.
 * \return            Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_Fill ( HANDLE_AACDECODER  self,
                  UCHAR             *pBuffer[],
                  const UINT         bufferSize[],
                  UINT              *bytesValid );

#define AACDEC_CONCEAL  1 /*!< Flag for aacDecoder_DecodeFrame(): Trigger the built-in error concealment module
                                 to generate a substitute signal for one lost frame. New input data will not be
                                 considered. */
#define AACDEC_FLUSH    2 /*!< Flag for aacDecoder_DecodeFrame(): Flush all filterbanks to get all delayed audio
                                 without having new input data. Thus new input data will not be considered.*/
#define AACDEC_INTR     4 /*!< Flag for aacDecoder_DecodeFrame(): Signal an input bit stream data discontinuity.
                                 Resync any internals as necessary. */
#define AACDEC_CLRHIST  8 /*!< Flag for aacDecoder_DecodeFrame(): Clear all signal delay lines and history buffers.
                                 CAUTION: This can cause discontinuities in the output signal. */
#define AACDEC_PREROLL 16 /*!< Flag indicating pre-roll frame */

/**
 * \brief            Decode one audio frame
 *
 * \param self       AAC decoder handle.
 * \param pTimeData  Pointer to external output buffer where the decoded PCM samples will be stored into.
 * \param flags      Bit field with flags for the decoder: \n
 *                   (flags & AACDEC_CONCEAL) == 1: Do concealment. \n
 *                   (flags & AACDEC_FLUSH) == 2: Discard input data. Flush filter banks (output delayed audio). \n
 *                   (flags & AACDEC_INTR) == 4: Input data is discontinuous. Resynchronize any internals as necessary.
 * \return           Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_DecodeFrame ( HANDLE_AACDECODER  self,
                         INT_PCM           *pTimeData,
                         const INT          timeDataSize,
                         UINT               flags );

/**
 * \brief       De-allocate all resources of an AAC decoder instance.
 *
 * \param self  AAC decoder handle.
 * \return      void
 */
LINKSPEC_H void aacDecoder_Close ( HANDLE_AACDECODER self );

/**
 * \brief       Get CStreamInfo handle from decoder.
 *
 * \param self  AAC decoder handle.
 * \return      Reference to requested CStreamInfo.
 */
LINKSPEC_H CStreamInfo* aacDecoder_GetStreamInfo( HANDLE_AACDECODER self );

/**
 * \brief       Get decoder library info.
 *
 * \param info  Pointer to an allocated LIB_INFO structure.
 * \return      0 on success
 */
LINKSPEC_H INT aacDecoder_GetLibInfo( LIB_INFO *info );



#define AACDEC_UI_FORCE_UPDATE          1   /*!< Flag for aacDecoder_UI_GetXmlSceneState(): Force output of XML
                                                 description, even if nothing has changed since the last call. */
#define AACDEC_UI_FORCE_RESTART_XML     4   /*!< Flag for aacDecoder_UI_GetXmlSceneState(): Force restart of
                                                 XML output if incomplete output was returned by previous call. */

#define AACDEC_UI_NO_CHANGE             1   /*!< Flag returned by aacDecoder_UI_GetXmlSceneState(): Nothing has
                                                 changed since the last call, no XML output was generated. */
#define AACDEC_UI_CONTINUES_XML         2   /*!< Flag returned by aacDecoder_UI_GetXmlSceneState(): XML output
                                                 is a continuation of incomplete output of the previous call. */
#define AACDEC_UI_INCOMPLETE_XML        4   /*!< Flag returned by aacDecoder_UI_GetXmlSceneState(): XML output
                                                 is not complete, at least one further call of the function is
                                                 required to get the complete XML output. */
#define AACDEC_UI_SHORT_OUTPUT          8   /*!< Flag returned by aacDecoder_UI_GetXmlSceneState(): only minimal 
                                                 XML output was generated, a further call of the function will
                                                 return the full XML scene description. */

/**
 * \brief            Get XML description of audio scene and available user interactivity parameters.\n
 *                   See \ref MpegHUserInteractivity
 *
 * \param self       AAC decoder handle.
 * \param xmlOut     Pointer to external output buffer where the XML string will be stored.
 * \param xmlOutSize Size of the output buffer (number of chars). Has to be at least 1024 to ensure success of the function.
 * \param flagsIn    Bit field with flags for the decoder: \n
 *                   (flags & ::AACDEC_UI_FORCE_UPDATE) != 0: Force output of XML description even if nothing has changed \n
 *                   (flags & ::AACDEC_UI_FORCE_RESTART_XML) != 0: Restart XML output from the beginning (if incomplete 
 *                   output was returned by previous call). \n
 * \param flagsOut   Pointer to bit field with flags returned by the decoder: \n
 *                   (flags & ::AACDEC_UI_NO_CHANGE) != 0: Nothing has changed since the previous call of this
 *                   function, no XML output was written. \n
 *                   (flags & ::AACDEC_UI_CONTINUES_XML) != 0: The XML output is a continuation of incomplete output of
 *                   the previous call and has to be appended to the output of the previous call. \n
 *                   (flags & ::AACDEC_UI_INCOMPLETE_XML) != 0: The XML output is not yet complete,
 *                   at least one further call of the function is required to get the remaining part. \n
 *                   (flags & ::AACDEC_UI_SHORT_OUTPUT) != 0: Only minimal XML output was generated, a further call of
                     the function will return the full scene description. \n
 * \return           Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_UI_GetXmlSceneState( HANDLE_AACDECODER self,
                                char *xmlOut,
                                UINT xmlOutSize,
                                UINT flagsIn,
                                UINT *flagsOut);

/**
 * \brief            Send XML description of UI command to the decoder.\n
 *                   See \ref MpegHUserInteractivity
 *
 * \param self       AAC decoder handle.
 * \param xmlIn      Pointer to a buffer containing the XML command string.
 * \param xmlInSize  Size of the XML string (number of chars).
 * \param flagsOut   Pointer to bit field with flags returned by the decoder, currently not used. \n
 * \return           Error code.
 */
LINKSPEC_H AAC_DECODER_ERROR
aacDecoder_UI_ApplyXmlAction( HANDLE_AACDECODER self,
                              const char *xmlIn,
                              UINT xmlInSize,
                              UINT *flagsOut);

#ifdef __cplusplus
}
#endif

#endif /* AACDECODER_LIB_H */
