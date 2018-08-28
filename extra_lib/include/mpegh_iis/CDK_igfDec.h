/*************************************************************************\
 *
 *               (C) copyright Fraunhofer - IIS (2014)
 *                        All Rights Reserved
 *
 *   filename: iisIGFDecLib.h
 *   project : MPEG-H
 *   author  : Youliy Ninov,nin@iis.fraunhofer.de
 *   contents/description: Intelligent gap filling library
 *
 *   This software and/or program is protected by copyright law and
 *   international treaties. Any reproduction or distribution of this
 *   software and/or program, or any portion of it, may result in severe
 *   civil and criminal penalties, and will be prosecuted to the maximum
 *   extent possible under law.
 *
 *   $Id$
\*************************************************************************/
#ifndef _IIS_IGFDECLIB_H
#define _IIS_IGFDECLIB_H

/* include GFRs temporarily until all plan-b / core experiment phase 2 tools are integrated and rm6 is implemented */
#include "global_functional_range.h"


#include "CDK_bitstream.h"
#include "CDK_igfSCFDecoder.h"


/* temporary switch until plan-b / core experiment phase 2 tools are integrated */
#define IGF_PHASE2_CE 

#define IGF_CLEANING_ACTIVE
#define IGF_CLEAN_THRESHOLD 16


#define IGF_MAX_WIN 8
#define IGF_MAX_TILES 4
#define IGF_MAX_GRIDS 5
#define IGF_MAX_SFB_BANDS 32
#define IGF_MAX_SFB_SHORT 16
#define IGF_MAX_TILE_WIDTH 1024

typedef enum {
  IGF_FRAME_DIVISION_AAC_OR_TCX_LONG = 0, /* AAC short blocks are interleaved and AAC and TCX long can share memory */
  IGF_FRAME_DIVISION_TCX_SHORT_1     = 1,
  IGF_FRAME_DIVISION_TCX_SHORT_2     = 2,
  N_IGF_FRAME_DIVISION               = 3
} IGF_FRAME_DIVISION;


typedef enum  {
  WHITENING_MID,
  WHITENING_OFF,
  WHITENING_STRONG,
  WHITENING_UNUSED
} WHITENING_LEVEL;


typedef enum {
    IGF_GRID_LONG_WINDOW,
    IGF_GRID_SHORT_WINDOW,
    IGF_GRID_TCX10_WINDOW,
    IGF_GRID_TCX20_WINDOW,
    IGF_GRID_UNDEF
} DEC_IGF_GRID;


typedef struct igf_map_info_struct {

  FIXP_DBL         *fSpectrumTab[IGF_MAX_WIN];                       /**< holds the information of the spectrum after dequantization and noisefilling from iSrcStart to (iSrcStart+iWidth), is used as source spectrum for IGF **/    

  SHORT            fSpectrumTab_sfb_exp[IGF_MAX_WIN*IGF_MAX_SFB_SHORT]; /**< sfb scale values **/

  SHORT            fSpectrumScale[IGF_MAX_WIN];  /**< window scale values **/


  WHITENING_LEVEL   iWhiteningLevel;                       /**< whitening level of the tile **/ 
  
  SHORT             SBTab[IGF_MAX_TILE_WIDTH];
  SHORT             iSfbWidthTab[IGF_MAX_SFB_BANDS];                      /**< holds information for the sfb granulation in a tile */
  SHORT             iSfbCnt;                               /**< count of sfbs in a tile */
  SHORT            *iSBTab;
  SHORT             iSBCnt;
  SHORT             iSrcStart;                             /**< defines the first line in source spectrum of the tile **/
  SHORT             iDesStart;                             /**< defines the first line in destination spectrum of the tile **/
  SHORT             iWidth;                                /**< width (or lengt) of the tile **/
  UCHAR             iIdx;                                  /**< tile index, transmitted in the bitstream **/
  UCHAR             iSfbSplit;  /**< When a paired SFB must be split between two tiles */

}IGF_MAP_INFO, *IGF_MAP_INFO_HANDLE;

typedef struct igf_work_memory_struct {

    FIXP_DBL          virtualSpec[1024];
    FIXP_DBL          Spectrum_tab_array[1024];

    FIXP_DBL          fSfbGainTab[IGF_MAX_SFB_BANDS];                       /**< gain to apply on IGF data */
    FIXP_DBL          fSfbDestinEnergyTab[IGF_MAX_SFB_BANDS];               /**< energy levels transmitted and decoded */
    FIXP_DBL          fSfbTileEnergyTab[IGF_MAX_WIN][IGF_MAX_SFB_BANDS];                 /**< energy levels calculated from source region */
    FIXP_DBL          fSfbSurvivedEnergyTab[IGF_MAX_WIN][IGF_MAX_SFB_BANDS];             /**< energy levels calculated from source region */
    FIXP_DBL          sE[IGF_MAX_SFB_BANDS];
    FIXP_DBL          tE[IGF_MAX_SFB_BANDS];

    SCHAR             fSfbGainTab_exp[IGF_MAX_SFB_BANDS];   
    SCHAR             fSfbDestinEnergyTab_exp[IGF_MAX_SFB_BANDS];               /**< energy levels transmitted and decoded */
    SCHAR             fSfbTileEnergyTab_exp[IGF_MAX_WIN][IGF_MAX_SFB_BANDS];
    SCHAR             fSfbSurvivedEnergyTab_exp[IGF_MAX_WIN][IGF_MAX_SFB_BANDS];
    SCHAR             sE_exp[IGF_MAX_SFB_BANDS];
    SCHAR             tE_exp[IGF_MAX_SFB_BANDS];

    SCHAR             intermediate_shift[IGF_MAX_WIN][IGF_MAX_SFB_BANDS];
    SCHAR             Initial_exp[IGF_MAX_WIN][IGF_MAX_SFB_BANDS];

}IGF_WORK_MEMORY, *IGF_WORK_MEMORY_HANDLE;



typedef struct igf_grid_info_struct {

  IGF_MAP_INFO      sIGFMapInfoTab[IGF_MAX_TILES];
  FIXP_DBL         *pCCSpectrumTab[IGF_MAX_WIN]; /**< core coder spectrum pointer */
  SHORT             iIGFStartLine;              /**< start line in IGF range */
  SHORT             iIGFStopLine;               /**< stop line in IGF range */
  SHORT             iIGFMinLine;                /**< minimal line where IGF start to copy from */
  UCHAR             iIGFTileIdxTab[IGF_MAX_TILES]; /**< tile index table used with initialization of sIGFMapInfoTab */
  UCHAR             iIGFNumSfb;                 /**< IGF number of SCF bands in total */
  UCHAR             iIGFStartSfb;               /**< start SBF in IGF range */
  UCHAR             iIGFStopSfb;                /**< stop SFB in IGF range */
  UCHAR             iIGFNumTile;                /**< number of tiles used */

}IGF_GRID_INFO, *IGF_GRID_INFO_HANDLE;


typedef struct igf_private_data_static_struct {

  IGFSCFDEC_PRIVATE_DATA     hIGFSCFDecoder;

  IGF_GRID_INFO              sGridInfoTab[IGF_MAX_GRIDS];

  WHITENING_LEVEL            igfWhiteningLevelPrev[IGF_MAX_TILES];

  INT                        aacFrameLength;
  INT                        igfMinSubbandLB;
  INT                        igfMinSubbandSB;
  INT                        igfMinSubbandTCX10;
  INT                        igfMinSubbandTCX20;
  INT                        igfTilesLB[IGF_MAX_TILES];
  INT                        igfTilesSB[IGF_MAX_TILES];
  INT                        igfTilesTCX10[IGF_MAX_TILES];
  INT                        igfTilesTCX20[IGF_MAX_TILES];
  UCHAR                      prevPatchNum[IGF_MAX_TILES]; /**< indices of the best matching patch */
  SHORT                      shortBlockSize;
  SHORT                      longBlockSize;
  UCHAR                      numSfbLB;
  const SHORT               *sfbOffsetLB;
  UCHAR                      numSfbSB;
  const SHORT               *sfbOffsetSB;
  UCHAR                      enhancedNoiseFilling;
  UCHAR                      igfUseWhitening;
  UCHAR                      igfAfterTnsSynth;
  UCHAR                      igfIndependentTiling;
  UCHAR                      useHighRes;
  UCHAR                      igfStartSfbLB;
  UCHAR                      igfStopSfbLB;
  UCHAR                      igfStartSfbSB;
  UCHAR                      igfStopSfbSB;
  UCHAR                      igfNTilesLB;
  UCHAR                      igfNTilesSB;
  UCHAR                      igfNTilesTCX10;
  UCHAR                      igfNTilesTCX20;

}IGF_PRIVATE_STATIC_DATA,*IGF_PRIVATE_STATIC_DATA_HANDLE;


typedef struct {
  INT                         sfe[256];                             /**< array which will contain the decoded quantized coefficients */
  INT                         iCurrTileIdxTab[IGF_MAX_TILES];
  WHITENING_LEVEL             igfWhiteningLevel[IGF_MAX_TILES];
} IGF_BITSTREAM_DATA;


typedef struct igf_private_data_struct {
  
  IGF_WORK_MEMORY            IGF_WorkingMem[IGF_MAX_TILES];
  IGF_BITSTREAM_DATA         bitstreamData[N_IGF_FRAME_DIVISION];
  UCHAR                      igfUseEnfFlat;
  UCHAR                      tileNum[IGF_MAX_TILES];     /**< indices of the best matching patch */
  UCHAR                      igf_allZero;
  

}IGF_PRIVATE_DATA,*IGF_PRIVATE_DATA_HANDLE;


void iisIGFDecLibInit(
        IGF_PRIVATE_STATIC_DATA       * hPrivateStaticData,     /**< inout: instance handle of MPEG-H */
        IGF_PRIVATE_DATA              * hPrivateData,     /**< inout: instance handle of MPEG-H */
        const UCHAR                     igfStartIndex,
        const UCHAR                     igfStopIndex,
        const UCHAR                     igfUseHighRes,
        const UCHAR                     igfUseWhitening,
        const UINT                      aacSampleRate,
        const INT                       aacFrameLength,
        const SHORT                   * sfb_offset_LB,
        const INT                       len_LB,
        const SHORT                   * sfb_offset_SB,
        const INT                       len_SB,
        const UCHAR                     enhancedNoiseFilling,
        const UCHAR                     igfAfterTnsSynth,
        const UCHAR                     igfIndependentTiling
        );


/**********************************************************************//**
implements the decoding of SCF
**************************************************************************/
void iisIGFDecLibReadSCF(
        IGF_PRIVATE_STATIC_DATA   *       hPrivateStaticData,
        IGF_PRIVATE_DATA          *       hPrivateData,
        HANDLE_CDK_BITSTREAM              hBStr,
        const INT                         indepFlag,         /**< in: if  1 on input the encoder will be forced to reset
                                                         if  0 on input the encodder will be forced to encode without a reset */
        const INT                         window_sequence,   /**< in: 1==SHORT 0==LONG window seq */
        const INT                         winCount,          /**< in: number of windows in the group */
        const INT                         frameType
        );

/**********************************************************************//**
implements the decoding of IGF
**************************************************************************/
int iisIGFDecLibReadIGF(
        IGF_PRIVATE_STATIC_DATA_HANDLE    hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE           hPrivateData,
        HANDLE_CDK_BITSTREAM              hBStr,
        const INT                         window_sequence,        /**< in: 1==SHORT 0==LONG window seq */
        const INT                         bUsacIndependencyFlag,  /**< in: USAC independency flag */
        const INT                         igfUseEnf,
        const INT                         frameType
        );

void iisIGF_Sync_Data(
        IGF_PRIVATE_DATA_HANDLE hPrivateDataL,
        IGF_PRIVATE_DATA_HANDLE hPrivateDataR
        );


void iisIGFDecLibInitGridInfo(
        IGF_WORK_MEMORY_HANDLE                    hWorkMem,
        IGF_GRID_INFO_HANDLE                      hGridInfo,
        const INT                                 aacFrameLength,
        const UINT                                aacSampleRate,
        const INT                                 window_sequence,
        const SHORT                               *sfb_offset,
        const INT                                 sfb_offset_len,
        const INT                                 sfbStart,
        const INT                                 sfbStop,
        const INT                                 useHighRes,
        INT                                      *tile,
        INT                                      *tileIdx
        );



void CIgf_apply(
        IGF_PRIVATE_STATIC_DATA_HANDLE hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE hPrivateData,
        FIXP_DBL               *p2_spectrum,
        SHORT                  *specScale,
        const INT               window_sequence,
        const INT               numOfGroups,
        const INT               NumberOfSpectra,
        const UCHAR            *groupLength,
        ULONG                  *randomSeed,
        UCHAR                  *TNF_mask,
        const UCHAR             flag_INF_active,
        const INT               frameType
        );

void CIgf_apply_stereo(
        IGF_PRIVATE_STATIC_DATA_HANDLE hPrivateStaticDataL,
        IGF_PRIVATE_STATIC_DATA_HANDLE hPrivateStaticDataR,
        IGF_PRIVATE_DATA_HANDLE hPrivateDataL,
        IGF_PRIVATE_DATA_HANDLE hPrivateDataR,
        FIXP_DBL               *p2_spectrumL,
        FIXP_DBL               *p2_spectrumR,
        SHORT                  *specScaleL,
        SHORT                  *specScaleR,
        const INT               window_sequence,
        const INT               numOfGroups,
        const INT               NumberOfSpectra,
        const UCHAR            *groupLength,
        ULONG                  *randomSeedL,
        ULONG                  *randomSeedR,
        const UCHAR            *iUseMSTab,
        UCHAR                  *TNF_maskL,
        UCHAR                  *TNF_maskR,
        const UCHAR             flag_INF_active,
        const INT               frameType
);

void iisIGFDecLibInjectSourceSpectrumNew(
        IGF_PRIVATE_STATIC_DATA_HANDLE        hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE               hPrivateData,
        FIXP_DBL                             *pSpectralData,          /**< ptr to spectral window data       */
        SHORT                                *pSpectralData_exp,
        const INT                             window_sequence,        /**< in: 1==SHORT 0==LONG window seq    */
        const INT                             frameType
        );

void iisIGFDecLibInjectSourceSpectrumTCX(
        IGF_PRIVATE_STATIC_DATA_HANDLE        hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE               hPrivateData,
        FIXP_DBL                             *pSpectralData,
        SHORT                                *specScale,
        UCHAR                                *inf_mask,
        ULONG                                *randomSeed,
        const FIXP_DBL                        noise_level,
        const INT                             startLine,
        const INT                             stopLine,
        const INT                             window_sequence,
        const INT                             win,
        const INT                             frameType,
        const INT                             useINFFlag
        );

FIXP_DBL* iisIGFDecLibAccessSourceSpectrum(
        IGF_PRIVATE_STATIC_DATA_HANDLE        hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE               hPrivateData,
        const INT                             tileIdx,
        const INT                             window_sequence,         /**< in: 1==SHORT 0==LONG window seq */
        const INT                             win
);

SHORT * iisIGFDecLibAccessSourceSpectrum_exponent(
        IGF_PRIVATE_STATIC_DATA_HANDLE        hPrivateStaticData,
        const INT                             tileIdx,
        const INT                             window_sequence         /**< in: 1==SHORT 0==LONG window seq */
        );

SHORT * iisIGFDecLibAccessSourceSpectrum_Scale(
        IGF_PRIVATE_STATIC_DATA_HANDLE        hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE               hPrivateData,
        const INT                             tileIdx,
        const INT                             window_sequence         /**< in: 1==SHORT 0==LONG window seq */
        );

INT iisIGFDecLibGetNumberOfTiles(
        IGF_PRIVATE_STATIC_DATA_HANDLE        hPrivateStaticData,
        const INT                             window_sequence         /**< in: 1==SHORT 0==LONG window seq */
);


/********************************************************
    TNF functions
*********************************************************/
void CIgf_TNF_apply(
        IGF_PRIVATE_STATIC_DATA_HANDLE hPrivateStaticData,
        IGF_PRIVATE_DATA_HANDLE hPrivateData,
        FIXP_DBL               *p2_spectrum,
        SHORT                  *specScale,
        const UCHAR            *TNF_mask,
        const INT               window_sequence
        );


#endif /* _IIS_IGFDECLIB_H */
