/*************************************************************************
 *
 *               (C) copyright Fraunhofer - IIS (2012)
 *                        All Rights Reserved
 *
 *   filename: iisIGFSCFDecoder.h
 *   project : IGF scalefactor decoder
 *   author  : Youliy Ninov, youliy.ninov@iis.fraunhofer.de
 *             Florin Ghido, florin.ghido@iis.fraunhofer.de,
 *             Andreas Niedermeier, Andreas.niedermeier@iis.fraunhofer.de
 *   This software and/or program is protected by copyright law and
 *   international treaties. Any reproduction or distribution of this
 *   software and/or program, or any portion of it, may result in severe
 *   civil and criminal penalties, and will be prosecuted to the maximum
 *   extent possible under law.
 *
 *   $Id$
 *************************************************************************/
#ifndef _IIS_IGFSCFDECODER_
#define _IIS_IGFSCFDECODER_

#include "global_functional_range.h"


#include "CDK_bitstream.h"
#include "genericStds.h"

#include "ac_arith_coder.h"

typedef enum igf_scf_win_mode {
  IGF_SCF_LONG,
  IGF_SCF_SHORT,
  IGF_SCF_TCX_MEDIUM,
  IGF_SCF_TCX_LONG,
  IGF_SCF_CODEC_TRANSITION
} IGF_SCF_WIN_MODE;

typedef struct igfscfdec_private_data_struct {
#ifdef NDR_DEBUG
  int signature;                               /* do not remove! */
#endif
  INT               size;                      /* do not remove! */

  HANDLE_CDK_BITSTREAM hBs;
  Tastat st;
  INT               prevBuffer[ 1024 ]; /* TODO: limit this value to acual needed size */
  INT              *prevSB;
  INT              *prevLB;
  INT              *prevTCXmedium;
  INT              *prevTCXlong;
  INT               prevDLB;
  INT               prevDSB;
  INT               prevDTCXmedium;
  INT               prevDTCXlong;
  INT               scfCountLongBlock;
  INT               scfCountShortBlock;
  INT               scfCountTCXBlock;
  INT              *sfe;
  INT               t;
  INT               lastFrameBlockType;

  INT               arith_decode_first_symbol;
  INT               high;
  INT               low;
  INT               bits_to_follow;
  INT               value;
}IGFSCFDEC_PRIVATE_DATA,*IGFSCFDEC_PRIVATE_DATA_HANDLE;


/**********************************************************************//**
definiton of public data for this module
**************************************************************************/
typedef struct igfscfdec_public_data_struct {
  INT     bitsReaded;   /* after a call bitsReaded contains the number of bits consumed by the decoder */
}IGFSCFDEC_INSTANCE, *IGFSCFDEC_INSTANCE_HANDLE;


void iisIGFSCFDecLibInit(
        IGFSCFDEC_PRIVATE_DATA  * hPrivateData,
        INT   scfCountLongBlock,
        INT   scfCountShortBlock,
		INT   scfCountTCXBlock
        );

int iisIGFSCFDecoderGetLastFrameWasShortBlock(
        IGFSCFDEC_PRIVATE_DATA          * hPrivateData        /**< inout: handle to public data or NULL in case there was no instance created */
        );

void iisIGFSCFDecoderSetLastFrameWasShortBlock(
        IGFSCFDEC_PRIVATE_DATA          * hPrivateData,          /**< inout: handle to public data or NULL in case there was no instance created */
        int                                 isShortBlock
        );

/**********************************************************************//**
resets the internal decoder memory (context memory
**************************************************************************/
void iisIGFSCFDecoderReset(
        IGFSCFDEC_PRIVATE_DATA          * hPublicData
        );

/**********************************************************************//**
main decoder function
**************************************************************************/
void iisIGFSCFDecoderDecode(
        IGFSCFDEC_PRIVATE_DATA  * hPrivateData,
        HANDLE_CDK_BITSTREAM                hBStr,
        INT                                *sfe,               /**< out: ptr to an array which will contain the decoded quantized coefficients */
        INT                                 indepFlag,         /**< in: if  1 on input the encoder will be forced to reset,
                                                                  if  0 on input the encodder will be forced to encode without a reset */
        INT                                 isShortBlock,      /**< in: if  1 on input a short block data will be decompressed */
        INT                                 noShortBlock,
        INT                                 sfeReducePrecisionEnabled
        );


#endif

