/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2004)
                               All Rights Reserved

   $Id$
   Author(s):   Manuel Jander

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   CDK_audio.h
 *  \brief  Global audio struct and constant definitions.
 */

#ifndef CDK_AUDIO_H
#define CDK_AUDIO_H

#include "machine_type.h"
#include "genericStds.h"
#include "syslib_channelMapDescr.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * File format identifiers.
 */
typedef enum
{
  FF_UNKNOWN           = -1, /**< Unknown format.        */
  FF_RAW               = 0,  /**< No container, bit stream data conveyed "as is". */

  FF_MP4_3GPP          = 3,  /**< 3GPP file format.      */
  FF_MP4_MP4F          = 4,  /**< MPEG-4 File format.     */

  FF_RAWPACKETS        = 5,  /**< Proprietary raw packet file. */

  FF_DRMCT             = 12, /**< Digital Radio Mondial (DRM30/DRM+) CT proprietary file format. */

  FF_RTPDUMP           = 18  /**< RTPdump file format. */

} FILE_FORMAT;

/**
 * Transport type identifiers.
 */
typedef enum
{
  TT_UNKNOWN           = -1, /**< Unknown format.            */
  TT_MP4_RAW           = 0,  /**< "as is" access units (packet based since there is obviously no sync layer) */
  TT_MP4_ADIF          = 1,  /**< ADIF bitstream format.     */
  TT_MP4_ADTS          = 2,  /**< ADTS bitstream format.     */

  TT_MP4_LATM_MCP1     = 6,  /**< Audio Mux Elements with muxConfigPresent = 1 */
  TT_MP4_LATM_MCP0     = 7,  /**< Audio Mux Elements with muxConfigPresent = 0, out of band StreamMuxConfig */

  TT_MP4_LOAS          = 10, /**< Audio Sync Stream.         */

  TT_DRM               = 12, /**< Digital Radio Mondial (DRM30/DRM+) bitstream format. */

  TT_MP1_L1            = 16, /**< MPEG 1 Audio Layer 1 audio bitstream. */
  TT_MP1_L2            = 17, /**< MPEG 1 Audio Layer 2 audio bitstream. */
  TT_MP1_L3            = 18, /**< MPEG 1 Audio Layer 3 audio bitstream. */
  TT_MHAS              = 20, /**< MPEG-H 3D Audio Stream. */
  TT_MHAS_PACKETIZED   = 24, /**< MPEG-H 3D Audio Packets. */

  TT_RSVD50            = 50, /**< */

  TT_MHA_RAW           = 60 /**< MPEGH 3D Audio */

} TRANSPORT_TYPE;

#define TT_IS_PACKET(x) \
       ( ((x) == TT_MP4_RAW) \
      || ((x) == TT_DRM) \
      || ((x) == TT_MP4_LATM_MCP0) \
      || ((x) == TT_MP4_LATM_MCP1) \
      || ((x) == TT_MHA_RAW) \
      || ((x) == TT_MHAS_PACKETIZED) )

#define TT_CFG_IS_INBAND(x) \
       ( ((x) == TT_MP4_ADTS) \
      || ((x) == TT_MP4_ADIF) \
      || ((x) == TT_MP1_L1) \
      || ((x) == TT_MP1_L2) \
      || ((x) == TT_MP1_L3) \
      || ((x) == TT_RSVD50) \
      || ((x) == TT_MP4_LATM_MCP1) \
      || ((x) == TT_MHAS) \
      || ((x) == TT_MHAS_PACKETIZED) )

/**
 * Audio Object Type definitions.
 */
typedef enum
{
  AOT_NONE             = -1,
  AOT_NULL_OBJECT      = 0,
  AOT_AAC_MAIN         = 1, /**< Main profile                              */
  AOT_AAC_LC           = 2, /**< Low Complexity object                     */
  AOT_AAC_SSR          = 3,
  AOT_AAC_LTP          = 4,
  AOT_SBR              = 5,
  AOT_AAC_SCAL         = 6,
  AOT_TWIN_VQ          = 7,
  AOT_CELP             = 8,
  AOT_HVXC             = 9,
  AOT_RSVD_10          = 10, /**< (reserved)                                */
  AOT_RSVD_11          = 11, /**< (reserved)                                */
  AOT_TTSI             = 12, /**< TTSI Object                               */
  AOT_MAIN_SYNTH       = 13, /**< Main Synthetic object                     */
  AOT_WAV_TAB_SYNTH    = 14, /**< Wavetable Synthesis object                */
  AOT_GEN_MIDI         = 15, /**< General MIDI object                       */
  AOT_ALG_SYNTH_AUD_FX = 16, /**< Algorithmic Synthesis and Audio FX object */
  AOT_ER_AAC_LC        = 17, /**< Error Resilient(ER) AAC Low Complexity    */
  AOT_RSVD_18          = 18, /**< (reserved)                                */
  AOT_ER_AAC_LTP       = 19, /**< Error Resilient(ER) AAC LTP object        */
  AOT_ER_AAC_SCAL      = 20, /**< Error Resilient(ER) AAC Scalable object   */
  AOT_ER_TWIN_VQ       = 21, /**< Error Resilient(ER) TwinVQ object         */
  AOT_ER_BSAC          = 22, /**< Error Resilient(ER) BSAC object           */
  AOT_ER_AAC_LD        = 23, /**< Error Resilient(ER) AAC LowDelay object   */
  AOT_ER_CELP          = 24, /**< Error Resilient(ER) CELP object           */
  AOT_ER_HVXC          = 25, /**< Error Resilient(ER) HVXC object           */
  AOT_ER_HILN          = 26, /**< Error Resilient(ER) HILN object           */
  AOT_ER_PARA          = 27, /**< Error Resilient(ER) Parametric object     */
  AOT_RSVD_28          = 28, /**< might become SSC                          */
  AOT_PS               = 29, /**< PS, Parametric Stereo (includes SBR)      */
  AOT_MPEGS            = 30, /**< MPEG Surround                             */

  AOT_ESCAPE           = 31, /**< Signal AOT uses more than 5 bits          */

  AOT_MP3ONMP4_L1      = 32, /**< MPEG-Layer1 in mp4                        */
  AOT_MP3ONMP4_L2      = 33, /**< MPEG-Layer2 in mp4                        */
  AOT_MP3ONMP4_L3      = 34, /**< MPEG-Layer3 in mp4                        */
  AOT_RSVD_35          = 35, /**< might become DST                          */
  AOT_RSVD_36          = 36, /**< might become ALS                          */
  AOT_AAC_SLS          = 37, /**< AAC + SLS                                 */
  AOT_SLS              = 38, /**< SLS                                       */
  AOT_ER_AAC_ELD       = 39, /**< AAC Enhanced Low Delay                    */

  AOT_USAC             = 42, /**< USAC                                      */
  AOT_SAOC             = 43, /**< SAOC                                      */
  AOT_LD_MPEGS         = 44, /**< Low Delay MPEG Surround                   */

  AOT_RSVD50           = 50,  /**< Interim AOT for Rsvd50                   */

  /* Pseudo AOTs */
  AOT_MP2_AAC_MAIN     = 128, /**< Virtual AOT MP2 Main profile                           */
  AOT_MP2_AAC_LC       = 129, /**< Virtual AOT MP2 Low Complexity profile                 */
  AOT_MP2_AAC_SSR      = 130, /**< Virtual AOT MP2 Scalable Sampling Rate profile         */

  AOT_MP2_SBR          = 132, /**< Virtual AOT MP2 Low Complexity Profile with SBR        */

  AOT_DAB              = 134, /**< Virtual AOT for DAB (Layer2 with scalefactor CRC)      */
  AOT_DABPLUS_AAC_LC   = 135, /**< Virtual AOT for DAB plus AAC-LC                        */
  AOT_DABPLUS_SBR      = 136, /**< Virtual AOT for DAB plus HE-AAC                        */
  AOT_DABPLUS_PS       = 137, /**< Virtual AOT for DAB plus HE-AAC v2                     */

  AOT_PLAIN_MP1        = 140, /**< Virtual AOT for plain mp1                              */
  AOT_PLAIN_MP2        = 141, /**< Virtual AOT for plain mp2                              */
  AOT_PLAIN_MP3        = 142, /**< Virtual AOT for plain mp3                              */

  AOT_DRM_AAC          = 143, /**< Virtual AOT for DRM (ER-AAC-SCAL without SBR)          */
  AOT_DRM_SBR          = 144, /**< Virtual AOT for DRM (ER-AAC-SCAL with SBR)             */
  AOT_DRM_MPEG_PS      = 145, /**< Virtual AOT for DRM (ER-AAC-SCAL with SBR and MPEG-PS) */
  AOT_DRM_SURROUND     = 146, /**< Virtual AOT for DRM Surround (ER-AAC-SCAL (+SBR) +MPS) */
  AOT_DRM_USAC         = 147, /**< Virtual AOT for DRM with USAC                          */

  AOT_MPEGH3DA         = 150, /**< Virtual AOT for MPEG-H 3D audio                        */

  AOT_MPEGD_RESIDUALS  = 256  /**< Virtual AOT for MPEG-D residuals                       */

} AUDIO_OBJECT_TYPE;

#define CAN_DO_PS(aot) \
  ((aot) == AOT_AAC_LC \
|| (aot) == AOT_SBR \
|| (aot) == AOT_PS \
|| (aot) == AOT_ER_BSAC \
|| (aot) == AOT_DRM_AAC)

#define IS_USAC(aot) \
  ((aot) == AOT_USAC \
|| (aot) == AOT_RSVD50 \
|| (aot) == AOT_MPEGH3DA)

#define IS_LOWDELAY(aot) \
  ((aot) == AOT_ER_AAC_LD \
|| (aot) == AOT_ER_AAC_ELD)

/** Channel Mode ( 1-7 equals MPEG channel configurations, others are arbitrary). */
typedef enum {
  MODE_INVALID                  = -1,
  MODE_UNKNOWN                  =  0,
  MODE_1                        =  1,       /**< C */
  MODE_2                        =  2,       /**< L+R */
  MODE_1_2                      =  3,       /**< C, L+R */
  MODE_1_2_1                    =  4,       /**< C, L+R, Rear */
  MODE_1_2_2                    =  5,       /**< C, L+R, LS+RS */
  MODE_1_2_2_1                  =  6,       /**< C, L+R, LS+RS, LFE */
  MODE_1_2_2_2_1                =  7,       /**< C, LC+RC, L+R, LS+RS, LFE */

  MODE_6_1                      = 11,       /**< C, L+R, LS+RS, Crear, LFE */
  MODE_7_1_BACK                 = 12,       /**< C, L+R, LS+RS, Lrear+Rrear, LFE */
  MODE_22_2                     = 13,       /**< */
  MODE_7_1_TOP_FRONT            = 14,       /**< C, L+R, LS+RS, LFE, Ltop+Rtop */

  MODE_1_1                      = 16,       /**< 2 SCEs (dual mono) */
  MODE_1_1_1_1                  = 17,       /**< 4 SCEs */
  MODE_1_1_1_1_1_1              = 18,       /**< 6 SCEs */
  MODE_1_1_1_1_1_1_1_1          = 19,       /**< 8 SCEs */
  MODE_1_1_1_1_1_1_1_1_1_1_1_1  = 20,       /**< 12 SCEs */

  MODE_2_2                      = 21,       /**< 2 CPEs */
  MODE_2_2_2                    = 22,       /**< 3 CPEs */
  MODE_2_2_2_2                  = 23,       /**< 4 CPEs */
  MODE_2_2_2_2_2_2              = 24,       /**< 6 CPEs */

  MODE_2_1                      = 30,       /**< CPE,SCE (ARIB standard B32) */

  MODE_7_1_REAR_SURROUND        = 33,       /**< C, L+R, LS+RS, Lrear+Rrear, LFE */
  MODE_7_1_FRONT_CENTER         = 34,       /**< C, LC+RC, L+R, LS+RS, LFE */

  MODE_212                      = 128       /**< 212 configuration, used in ELDv2 */

} CHANNEL_MODE;

/**
 * Speaker description tags.
 * Do not change the enumeration values unless it keeps the following segmentation:
 * - Bit 0-3: Horizontal postion (0: none, 1: front, 2: side, 3: back, 4: lfe)
 * - Bit 4-7: Vertical position (0: normal, 1: top, 2: bottom)
 */
typedef enum {
  ACT_NONE         = 0x00,
  ACT_FRONT        = 0x01,  /*!< Front speaker position (at normal height) */
  ACT_SIDE         = 0x02,  /*!< Side speaker position (at normal height) */
  ACT_BACK         = 0x03,  /*!< Back speaker position (at normal height) */
  ACT_LFE          = 0x04,  /*!< Low frequency effect speaker postion (front) */

  ACT_TOP          = 0x10,  /*!< Top speaker area (for combination with speaker positions) */
  ACT_FRONT_TOP    = 0x11,  /*!< Top front speaker = (ACT_FRONT|ACT_TOP) */
  ACT_SIDE_TOP     = 0x12,  /*!< Top side speaker  = (ACT_SIDE |ACT_TOP) */
  ACT_BACK_TOP     = 0x13,  /*!< Top back speaker  = (ACT_BACK |ACT_TOP) */

  ACT_BOTTOM       = 0x20,  /*!< Bottom speaker area (for combination with speaker positions) */
  ACT_FRONT_BOTTOM = 0x21,  /*!< Bottom front speaker = (ACT_FRONT|ACT_BOTTOM) */
  ACT_SIDE_BOTTOM  = 0x22,  /*!< Bottom side speaker  = (ACT_SIDE |ACT_BOTTOM) */
  ACT_BACK_BOTTOM  = 0x23   /*!< Bottom back speaker  = (ACT_BACK |ACT_BOTTOM) */

} AUDIO_CHANNEL_TYPE;

typedef enum
{
  SIG_UNKNOWN                 = -1,
  SIG_IMPLICIT                =  0,
  SIG_EXPLICIT_BW_COMPATIBLE  =  1,
  SIG_EXPLICIT_HIERARCHICAL   =  2

} SBR_PS_SIGNALING;

/**
 * Audio Codec flags.
 */
#define AC_ER_VCB11     0x000001  /*!< aacSectionDataResilienceFlag     flag (from ASC): 1 means use virtual codebooks  */
#define AC_ER_RVLC      0x000002  /*!< aacSpectralDataResilienceFlag     flag (from ASC): 1 means use huffman codeword reordering */
#define AC_ER_HCR       0x000004  /*!< aacSectionDataResilienceFlag     flag (from ASC): 1 means use virtual codebooks  */
#define AC_SCALABLE     0x000008  /*!< AAC Scalable*/
#define AC_ELD          0x000010  /*!< AAC-ELD */
#define AC_LD           0x000020  /*!< AAC-LD */
#define AC_ER           0x000040  /*!< ER syntax */
#define AC_BSAC         0x000080  /*!< BSAC */
#define AC_USAC         0x000100  /*!< USAC */
#define AC_MPEGH3DA     0x000200  /*!< MPEG-H 3D audio */
#define AC_HDAAC        0x000400  /*!< HD-AAC */
#define AC_RSVD50       0x004000  /*!< Rsvd50 */
#define AC_SBR_PRESENT  0x008000  /*!< SBR present flag (from ASC)             */
#define AC_SBRCRC       0x010000  /*!< SBR CRC present flag. Only relevant for AAC-ELD for now. */
#define AC_PS_PRESENT   0x020000  /*!< PS present flag (from ASC or implicit)  */
#define AC_MPS_PRESENT  0x040000  /*!< MPS present flag (from ASC or implicit) */
#define AC_DRM          0x080000  /*!< DRM bit stream syntax */
#define AC_INDEP        0x100000  /*!< Independency flag */
#define AC_MPEGD_RES    0x200000  /*!< MPEG-D residual individual channel data. */
#define AC_SAOC_PRESENT 0x400000  /*!< SAOC Present Flag */
#define AC_DAB          0x800000  /*!< DAB bit stream syntax */
#define AC_ELD_DOWNSCALE 0x1000000/*!< ELD Downscaled playout */
#define AC_LD_MPS      0x2000000  /*!< Low Delay MPS. */
#define AC_DRC_PRESENT 0x4000000  /*!< Dynamic Range Control (DRC) data found. */
#define AC_USAC_SCFGI3 0x8000000  /*!< USAC flag: If stereoConfigIndex is 3 the flag is set. */
/**
 * Audio Codec flags (reconfiguration).
 */
#define AC_CM_DET_CFG_CHANGE      0x000001  /*!< Config mode signalizes the callback to work in config change detection mode */
#define AC_CM_ALLOC_MEM           0x000002  /*!< Config mode signalizes the callback to work in memory allocation mode */

/**
 * Audio Codec flags (element specific).
 */
#define AC_EL_USAC_TW             0x000001  /*!< USAC time warped filter bank is active */
#define AC_EL_USAC_NOISE          0x000002  /*!< USAC noise filling is active */
#define AC_EL_USAC_ITES           0x000004  /*!< USAC SBR inter-TES tool is active */
#define AC_EL_USAC_PVC            0x000008  /*!< USAC SBR predictive vector coding tool is active */
#define AC_EL_USAC_MPS212         0x000010  /*!< USAC MPS212 tool is active */
#define AC_EL_USAC_LFE            0x000020  /*!< USAC element is LFE */
#define AC_EL_USAC_CP_POSSIBLE    0x000040  /*!< USAC may use Complex Stereo Prediction in this channel element */
#define AC_EL_ENHANCED_NOISE      0x000080  /*!< Enhanced noise filling*/
#define AC_EL_IGF_AFTER_TNS       0x000100  /*!< IGF after TNS */
#define AC_EL_IGF_INDEP_TILING    0x000200  /*!< IGF independent tiling */
#define AC_EL_IGF_USE_ENF         0x000400  /*!< IGF use enhanced noise filling */
/* plan-B element-flags */
#define AC_EL_FULLBANDLPD         0x000800  /*!< enable fullband LPD tools */
#define AC_EL_LPDSTEREOIDX        0x001000  /*!< LPD-stereo-tool stereo index */
#define AC_EL_LFE                 0x002000  /*!< The element is of type LFE. */

/* CODER_CONFIG::flags */
#define CC_MPEG_ID         0x00100000
#define CC_IS_BASELAYER    0x00200000
#define CC_PROTECTION      0x00400000
#define CC_SBR             0x00800000
#define CC_SBRCRC          0x00010000
#define CC_SAC             0x00020000
#define CC_RVLC            0x01000000
#define CC_VCB11           0x02000000
#define CC_HCR             0x04000000
#define CC_PSEUDO_SURROUND 0x08000000
#define CC_USAC_NOISE      0x10000000
#define CC_USAC_TW         0x20000000
#define CC_USAC_HBE        0x40000000

/** Generic audio coder configuration structure. */
typedef struct {
  AUDIO_OBJECT_TYPE aot;         /**< Audio Object Type (AOT).           */
  AUDIO_OBJECT_TYPE extAOT;      /**< Extension Audio Object Type (SBR). */
  CHANNEL_MODE channelMode;      /**< Channel mode.                      */
  UCHAR      channelConfigZero;  /**< Use channel config zero + pce although a standard channel config could be signaled. */
  INT        samplingRate;       /**< Sampling rate.                     */
  INT        extSamplingRate;    /**< Extended samplerate (SBR).         */
  INT        bitRate;            /**< Average bitrate.                   */
  int        samplesPerFrame;    /**< Number of PCM samples per codec frame and audio channel. */
  int        noChannels;         /**< Number of audio channels.          */
  int        bitsFrame;
  int        nSubFrames;         /**< Amount of encoder subframes. 1 means no subframing.                                     */
  int        BSACnumOfSubFrame;  /**< The number of the sub-frames which are grouped and transmitted in a super-frame (BSAC). */
  int        BSAClayerLength;    /**< The average length of the large-step layers in bytes (BSAC).                            */
  UINT       flags;              /**< flags */
  UCHAR      matrixMixdownA;     /**< Matrix mixdown index to put into PCE. Default value 0 means no mixdown coefficient,
                                      valid values are 1-4 which correspond to matrix_mixdown_idx 0-3. */
  UCHAR      headerPeriod;       /**< Frame period for sending in band configuration buffers in the transport layer. */

  UCHAR      stereoConfigIndex; /**< USAC MPS stereo mode */
  UCHAR      sbrMode;           /**< USAC SBR mode */
  SBR_PS_SIGNALING sbrSignaling;/**< 0: implicit signaling, 1: backwards compatible explicit signaling, 2: hierarcical explicit signaling */

  UCHAR      rawConfig[64];     /**< raw codec specific config as bit stream */
  int        rawConfigBits;     /**< Size of rawConfig in bits */

  UCHAR      sbrPresent;
  UCHAR      psPresent;
} CODER_CONFIG;


#define USAC_ID_BIT 16  /** USAC element IDs start at USAC_ID_BIT */

/** MP4 Element IDs. */
typedef enum
{
  /* mp4 element IDs */
  ID_NONE = -1,   /**< Invalid Element helper ID.             */
  ID_SCE = 0,     /**< Single Channel Element.                */
  ID_CPE = 1,     /**< Channel Pair Element.                  */
  ID_CCE = 2,     /**< Coupling Channel Element.              */
  ID_LFE = 3,     /**< LFE Channel Element.                   */
  ID_DSE = 4,     /**< Currently one Data Stream Element for ancillary data is supported. */
  ID_PCE = 5,     /**< Program Config Element.                */
  ID_FIL = 6,     /**< Fill Element.                          */
  ID_END = 7,     /**< Arnie (End Element = Terminator).      */
  ID_EXT = 8,     /**< Extension Payload (ER only).           */
  ID_SCAL = 9,    /**< AAC scalable element (ER only).        */
  /* USAC element IDs */
  ID_USAC_SCE = 0 + USAC_ID_BIT,     /**< Single Channel Element.                */
  ID_USAC_CPE = 1 + USAC_ID_BIT,     /**< Channel Pair Element.                  */
  ID_USAC_LFE = 2 + USAC_ID_BIT,     /**< LFE Channel Element.                   */
  ID_USAC_EXT = 3 + USAC_ID_BIT,     /**< Extension Element.                     */
  ID_USAC_END = 4 + USAC_ID_BIT,     /**< Arnie (End Element = Terminator).      */
  ID_LAST
} MP4_ELEMENT_ID;

/* usacConfigExtType q.v. ISO/IEC DIS 23008-3 Table 52  and  ISO/IEC FDIS 23003-3:2011(E) Table 74*/
typedef enum
{
    /* USAC and MPEG-H 3DA */
    ID_CONFIG_EXT_FILL = 0,
    /* MPEG-H 3DA */
    ID_CONFIG_EXT_DOWNMIX = 1,
    ID_CONFIG_EXT_LOUDNESS_INFO = 2,
    ID_CONFIG_EXT_AUDIOSCENE_INFO = 3,
    ID_CONFIG_EXT_HOA_MATRIX = 4,
    ID_CONFIG_EXT_SIG_GROUP_INFO = 6
    /* 5-127 => reserved for ISO use */
    /* > 128 => reserved for use outside of ISO scope */
} CONFIG_EXT_ID;

#define IS_CHANNEL_ELEMENT(elementId) \
  ((elementId) == ID_SCE \
|| (elementId) == ID_CPE \
|| (elementId) == ID_LFE \
|| (elementId) == ID_USAC_SCE \
|| (elementId) == ID_USAC_CPE \
|| (elementId) == ID_USAC_LFE)

#define IS_MP4_CHANNEL_ELEMENT(elementId) \
   ((elementId) == ID_SCE \
  || (elementId) == ID_CPE \
  || (elementId) == ID_LFE)

#define EXT_ID_BITS 4  /**< Size in bits of extension payload type tags. */

/** Extension payload types. */
typedef enum {
  EXT_FIL           = 0x00,
  EXT_FILL_DATA     = 0x01,
  EXT_DATA_ELEMENT  = 0x02,
  EXT_DATA_LENGTH   = 0x03,
  EXT_LDSAC_DATA    = 0x09,
  EXT_SAOC_DATA     = 0x0a,
  EXT_DYNAMIC_RANGE = 0x0b,
  EXT_SAC_DATA      = 0x0c,
  EXT_SBR_DATA      = 0x0d,
  EXT_SBR_DATA_CRC  = 0x0e
} EXT_PAYLOAD_TYPE;


#define IS_USAC_CHANNEL_ELEMENT(elementId) \
  ((elementId) == ID_USAC_SCE \
|| (elementId) == ID_USAC_CPE \
|| (elementId) == ID_USAC_LFE)

/** MPEG-D USAC & MPEG-H 3D audio Extension Element Types. */
typedef enum {
  /* usac */
  ID_EXT_ELE_FILL          = 0x00,
  ID_EXT_ELE_MPEGS         = 0x01,
  ID_EXT_ELE_SAOC          = 0x02,
  /* mpegh3da */
  ID_EXT_ELE_AUDIOPREROLL  = 0x03,
  ID_EXT_ELE_UNI_DRC       = 0x04,
  ID_EXT_ELE_OBJ_METADATA  = 0x05,
  ID_EXT_ELE_SAOC_3D       = 0x06,
  ID_EXT_ELE_HOA           = 0x07,
  ID_EXT_ELE_FMT_CNVRTR    = 0x08,
  ID_EXT_ELE_MCT           = 0x09,
  ID_EXT_ELE_ENHANCED_OBJ_METADATA = 0x0d
} USAC_EXT_ELEMENT_TYPE;


/**
 * Proprietary raw packet file configuration data type identifier.
 */
typedef enum
{
  TC_NOTHING = 0,                /* No configuration available -> in-band configuration.   */
  TC_RAW_ASC,                    /* Configuration data field is a raw AudioSpecificConfig. */
  TC_RAW_SMC,                    /* Configuration data field is a raw StreamMuxConfig.     */
  TC_RAW_SDC = 21,               /* Configuration data field is a raw Drm SDC.             */
  TC_RAW_MHAS_PACKETIZED = 24    /* Configuration data field is empty (in-band signaled config).
                                    Payload is has TT_MHAS_PACKETIZED format */

} TP_CONFIG_TYPE;

/*
 * ##############################################################################################
 * Library identification and error handling
 * ##############################################################################################
 */
/* \cond */

typedef enum {
  CDK_NONE   = 0,
  CDK_TOOLS  = 1,
  CDK_SYSLIB = 2,
  CDK_AACDEC = 3,
  CDK_AACENC = 4,
  CDK_SBRDEC = 5,
  CDK_SBRENC = 6,
  CDK_TPDEC  = 7,
  CDK_TPENC  = 8,
  CDK_MPSDEC = 9,
  CDK_MPEGFILEREAD  = 10,
  CDK_MPEGFILEWRITE = 11,
  CDK_MP2DEC = 12,
  CDK_DABDEC = 13,
  CDK_DABPARSE = 14,
  CDK_DRMDEC = 15,
  CDK_DRMPARSE = 16,
  CDK_AACLDENC = 17,
  CDK_MP2ENC = 18,
  CDK_MP3ENC = 19,
  CDK_MP3DEC = 20,
  CDK_MP3HEADPHONE = 21,
  CDK_MP3SDEC = 22,
  CDK_MP3SENC = 23,
  CDK_EAEC    = 24,
  CDK_DABENC  = 25,
  CDK_DMBDEC  = 26,
  CDK_FDREVERB = 27,
  CDK_DRMENC  = 28,
  CDK_METADATATRANSCODER = 29,
  CDK_AC3DEC = 30,
  CDK_PCMDMX = 31,
  CDK_SAOCDEC = 32,
  CDK_HEADPHONE = 33,
  CDK_MPSENC    = 34,
  CDK_TDLIMIT   = 35,
  CDK_ARITH     = 36,
  CDK_AC3ENC    = 37,
  CDK_UNIDRCDEC = 38,

  CDK_MODULE_LAST

} CDK_MODULE_ID;

/* AAC capability flags */
#define CAPF_AAC_LC           0x00000001  /**< Support flag for AAC Low Complexity.                             */
#define CAPF_ER_AAC_LD        0x00000002  /**< Support flag for AAC Low Delay with Error Resilience tools.      */
#define CAPF_ER_AAC_SCAL      0x00000004  /**< Support flag for AAC Scalable.                                   */
#define CAPF_ER_AAC_LC        0x00000008  /**< Support flag for AAC Low Complexity with Error Resilience tools. */
#define CAPF_AAC_480          0x00000010  /**< Support flag for AAC with 480 framelength.  */
#define CAPF_AAC_512          0x00000020  /**< Support flag for AAC with 512 framelength.  */
#define CAPF_AAC_960          0x00000040  /**< Support flag for AAC with 960 framelength.  */
#define CAPF_AAC_1024         0x00000080  /**< Support flag for AAC with 1024 framelength. */
#define CAPF_AAC_HCR          0x00000100  /**< Support flag for AAC with Huffman Codeword Reordering.    */
#define CAPF_AAC_VCB11        0x00000200  /**< Support flag for AAC Virtual Codebook 11.    */
#define CAPF_AAC_RVLC         0x00000400  /**< Support flag for AAC Reversible Variable Length Coding.   */
#define CAPF_AAC_MPEG4        0x00000800  /**< Support flag for MPEG file format.                           */
#define CAPF_AAC_DRC          0x00001000  /**< Support flag for AAC Dynamic Range Control. */
#define CAPF_AAC_CONCEALMENT  0x00002000  /**< Support flag for AAC concealment.           */
#define CAPF_AAC_DRM_BSFORMAT 0x00004000  /**< Support flag for AAC DRM bistream format.                         */
#define CAPF_ER_AAC_ELD       0x00008000  /**< Support flag for AAC Enhanced Low Delay with Error Resilience tools.  */
#define CAPF_ER_AAC_BSAC      0x00010000  /**< Support flag for AAC BSAC.                           */
#define CAPF_AAC_SUPERFRAMING 0x00020000  /**< Support flag for AAC Superframing.                           */
#define CAPF_AAC_ELD_DOWNSCALE 0x00040000 /**< Support flag for AAC-ELD Downscaling           */
#define CAPF_AAC_QMF_RESAMP   0x00080000  /**< Support flag for QMF based internal resampler. */
#define CAPF_AAC_USAC_LP      0x00100000  /**< Support flag for USAC low power mode. */
#define CAPF_AAC_USAC         0x00200000  /**< Support flag for Unified Speech and Audio Coding (USAC). */
#define CAPF_AAC_MPEGH3DA     0x00400000  /**< Support flag for MPEGH 3D Audio */

/* Transport capability flags */
#define CAPF_ADTS             0x00000001  /**< Support flag for ADTS transport format.        */
#define CAPF_ADIF             0x00000002  /**< Support flag for ADIF transport format.        */
#define CAPF_LATM             0x00000004  /**< Support flag for LATM transport format.        */
#define CAPF_LOAS             0x00000008  /**< Support flag for LOAS transport format.        */
#define CAPF_RAWPACKETS       0x00000010  /**< Support flag for RAW PACKETS transport format. */
#define CAPF_DRM              0x00000020  /**< Support flag for DRM/DRM+ transport format.    */
#define CAPF_RSVD50           0x00000040  /**< Support flag for RSVD50 transport format       */

/* SBR capability flags */
#define CAPF_SBR_LP           0x00000001  /**< Support flag for SBR Low Power mode.           */
#define CAPF_SBR_HQ           0x00000002  /**< Support flag for SBR High Quality mode.        */
#define CAPF_SBR_DRM_BS       0x00000004  /**< Support flag for                               */
#define CAPF_SBR_CONCEALMENT  0x00000008  /**< Support flag for SBR concealment.              */
#define CAPF_SBR_DRC          0x00000010  /**< Support flag for SBR Dynamic Range Control.    */
#define CAPF_SBR_PS_MPEG      0x00000020  /**< Support flag for MPEG Parametric Stereo.       */
#define CAPF_SBR_PS_DRM       0x00000040  /**< Support flag for DRM Parametric Stereo.        */
#define CAPF_SBR_ELD_DOWNSCALE 0x00000080 /**< Support flag for ELD reduced delay mode        */
#define CAPF_SBR_HBEHQ        0x00000100  /**< Support flag for HQ HBE                        */

/* MP2 encoder capability flags */
#define CAPF_MP2ENC_SS        0x00000001  /**< Support flag for Seamless Switching.           */
#define CAPF_MP2ENC_DAB       0x00000002  /**< Support flag for Layer2 DAB.                   */

/* MP2 decoder capability flags */
#define CAPF_MP2DEC_DAB       0x00000001  /**< Support flag for Layer2 DAB.                   */
#define CAPF_MP2DEC_CONCEAL   0x00000002  /**< Support flag for Layer2 error concealment.     */
#define CAPF_MP2DEC_SYNC      0x00000004  /**< Support flag for Layer2 stream synchronization.*/
#define CAPF_MP2DEC_MPS       0x00000008  /**< Support flag for Layer2 Surround decoding.     */
#define CAPF_MP2DEC_DRC       0x00000010  /**< Support flag for Layer2 Dynamic Range Control. */
#define CAPF_MP2DEC_BIRDIE    0x00000020  /**< Support flag for Layer2 birdie detector.       */
#define CAPF_MP2DEC_SBFILT    0x00000040  /**< Support flag for Layer2 subband filtering.     */

/* DAB capability flags */
#define CAPF_DAB_MP2          0x00000001  /**< Support flag for Layer2 DAB.                   */
#define CAPF_DAB_AAC          0x00000002  /**< Support flag for DAB+ (HE-AAC v2).             */
#define CAPF_DAB_PAD          0x00000004  /**< Support flag for PAD extraction.               */
#define CAPF_DAB_DRC          0x00000008  /**< Support flag for Dynamic Range Control.        */
#define CAPF_DAB_SURROUND     0x00000010  /**< Support flag for DAB Surround (MPS).           */

/* DMB capability flags */
#define CAPF_DMB_BSAC         0x00000001  /**< Support flag for ER AAC BSAC.                  */
#define CAPF_DMB_DRC          0x00000008  /**< Support flag for Dynamic Range Control.        */
#define CAPF_DMB_SURROUND     0x00000010  /**< Support flag for DMB Surround (MPS).           */

/* DRM capability flags */
#define CAPF_DRM_AAC          0x00000001  /**< Support flag for DRM AAC.                      */
#define CAPF_DRM_HVXC         0x00000002  /**< Support flag for DRM HVXC                      */
#define CAPF_DRM_CELP         0x00000004  /**< Support flag for DRM CELP                      */
#define CAPF_DRM_PLUS         0x00000008  /**< Support flag for DRM +                         */
#define CAPF_DRM_SURROUND     0x00000010  /**< Support flag for DRM Surround.                 */
#define CAPF_DRM_QMF_RESAMP   0x00000020  /**< Support flag for DRM QMF based internal resampler. */
#define CAPF_DRM_USAC_LP      0x00000040  /**< Support flag for DRM USAC low power mode.      */
#define CAPF_DRM_USAC         0x00000080  /**< Support flag for DRM xHE-AAC.                  */

/* PCM utils capability flags */
#define CAPF_DMX_BLIND        0x00000001  /**< Support flag for blind downmixing.             */
#define CAPF_DMX_PCE          0x00000002  /**< Support flag for guided downmix with data from MPEG-2/4 Program Config Elements (PCE). */
#define CAPF_DMX_ARIB         0x00000004  /**< Support flag for PCE guided downmix with slightly different equations and levels to fulfill ARIB standard. */
#define CAPF_DMX_DVB          0x00000008  /**< Support flag for guided downmix with data from DVB ancillary data fields. */
#define CAPF_DMX_CH_EXP       0x00000010  /**< Support flag for simple upmixing by dublicating channels or adding zero channels. */
#define CAPF_DMX_6_CH         0x00000020  /**< Support flag for 5.1 channel configuration (input and output). */
#define CAPF_DMX_8_CH         0x00000040  /**< Support flag for 6 and 7.1 channel configurations (input and output). */
#define CAPF_DMX_24_CH        0x00000080  /**< Support flag for 22.2 channel configuration (input and output). */
#define CAPF_LIMITER          0x00002000  /**< Support flag for signal level limiting. */

/* MPEG Surround capability flags */
#define CAPF_MPS_STD          0x00000001  /**< Support flag for MPEG Surround.           */
#define CAPF_MPS_LD           0x00000002  /**< Support flag for Low Delay MPEG Surround. */
#define CAPF_MPS_USAC         0x00000004  /**< Support flag for USAC MPEG Surround.      */
#define CAPF_MPS_HQ           0x00000010  /**< Support flag indicating if high quality processing is supported */
#define CAPF_MPS_LP           0x00000020  /**< Support flag indicating if partially complex (low power) processing is supported */
#define CAPF_MPS_BLIND        0x00000040  /**< Support flag indicating if blind processing is supported */
#define CAPF_MPS_BINAURAL     0x00000080  /**< Support flag indicating if binaural output is possible */
#define CAPF_MPS_2CH_OUT      0x00000100  /**< Support flag indicating if 2ch output is possible      */
#define CAPF_MPS_6CH_OUT      0x00000200  /**< Support flag indicating if 6ch output is possible      */
#define CAPF_MPS_8CH_OUT      0x00000400  /**< Support flag indicating if 8ch output is possible      */
#define CAPF_MPS_1CH_IN       0x00001000  /**< Support flag indicating if 1ch dmx input is possible   */
#define CAPF_MPS_2CH_IN       0x00002000  /**< Support flag indicating if 2ch dmx input is possible   */
#define CAPF_MPS_6CH_IN       0x00004000  /**< Support flag indicating if 5ch dmx input is possible   */

/* \endcond */


/*
 * ##############################################################################################
 * Library versioning
 * ##############################################################################################
 */

/**
 * Convert each member of version numbers to one single numeric version representation.
 * \param lev0  1st level of version number.
 * \param lev1  2nd level of version number.
 * \param lev2  3rd level of version number.
  */
#define LIB_VERSION(lev0, lev1, lev2) ((lev0<<24 & 0xff000000) | \
                                       (lev1<<16 & 0x00ff0000) | \
                                       (lev2<<8  & 0x0000ff00))

/**
 *  Build text string of version.
 */
#define LIB_VERSION_STRING(info) CDKsprintf((info)->versionStr, "%d.%d.%d", (((info)->version >> 24) & 0xff), (((info)->version >> 16) & 0xff), (((info)->version >> 8 ) & 0xff))

/**
 *  Library information.
 */
typedef struct LIB_INFO
{
  const char* title;
  const char* build_date;
  const char* build_time;
  CDK_MODULE_ID module_id;
  INT version;
  UINT  flags;
  char versionStr[32];
} LIB_INFO;

/** Initialize library info. */
static inline void CDKinitLibInfo( LIB_INFO* info )
{
  int i;

  for (i = 0; i < CDK_MODULE_LAST; i++) {
    info[i].module_id = CDK_NONE;
  }
}

/** Aquire supported features of library. */
static inline UINT CDKlibInfo_getCapabilities( const LIB_INFO* info, CDK_MODULE_ID module_id )
{
  int i;

  for (i=0; i<CDK_MODULE_LAST; i++) {
    if (info[i].module_id == module_id) {
      return info[i].flags;
    }
  }
  return 0;
}

/** Search for next free tab. */
static inline INT CDKlibInfo_lookup( const LIB_INFO* info, CDK_MODULE_ID module_id )
{
  int i = -1;

  for (i = 0; i < CDK_MODULE_LAST; i++) {
    if (info[i].module_id == module_id)
      return -1;
    if (info[i].module_id == CDK_NONE)
      break;
  }
  if (i == CDK_MODULE_LAST)
    return -1;

  return i;
}


/*
 * ##############################################################################################
 * Buffer description
 * ##############################################################################################
 */

/**
 *  I/O buffer descriptor.
 */
typedef struct CDK_bufDescr
{
  void  **ppBase;     /*!< Pointer to an array containing buffer base addresses.
                           Set to NULL for buffer requirement info. */
  UINT   *pBufSize;   /*!< Pointer to an array containing the number of elements that can
                           be placed in the specific buffer. */
  UINT   *pEleSize;   /*!< Pointer to an array containing the element size for each buffer
                           in bytes. That is mostly the number returned by the sizeof()
                           operator for the data type used for the specific buffer. */
  UINT   *pBufType;   /*!< Pointer to an array of bit fields containing a description
                           for each buffer. See XXX below for more details.  */
  UINT    numBufs;    /*!< Total number of buffers. */

} CDK_bufDescr;

/**
 * Buffer type description field.
 */
#define CDK_BUF_TYPE_MASK_IO     ( (UINT)0x03 << 30 )
#define CDK_BUF_TYPE_MASK_DESCR  ( (UINT)0x3F << 16 )
#define CDK_BUF_TYPE_MASK_ID     ( (UINT)0xFF )

#define CDK_BUF_TYPE_INPUT       ( (UINT)0x1 << 30 )
#define CDK_BUF_TYPE_OUTPUT      ( (UINT)0x2 << 30 )

#define CDK_BUF_TYPE_PCM_DATA    ( (UINT)0x1 << 16 )
#define CDK_BUF_TYPE_ANC_DATA    ( (UINT)0x2 << 16 )
#define CDK_BUF_TYPE_BS_DATA     ( (UINT)0x4 << 16 )

#ifdef __cplusplus
}
#endif

#endif /* CDK_AUDIO_H */
