/**************************  Fraunhofer IIS CDK SysLib  **********************

                    (C) Copyright Fraunhofer IIS (2013-2014)
                               All Rights Reserved

  $Id$
  Author(s):   Thomas Dietzen

  This software and/or program is protected by copyright law and international
  treaties. Any reproduction or distribution of this software and/or program,
  or any portion of it, may result in severe civil and criminal penalties, and
  will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   syslib_channelMapDescr.h
 *  \brief  Function and structure declarations for the channel map descriptor implementation.
 */

#ifndef __SYSLIB_CHANNELMAPDESCR_H__
#define __SYSLIB_CHANNELMAPDESCR_H__

#include "machine_type.h"


/**
 * \brief  Contains information needed for a single channel map.
 */
typedef struct {
  const UCHAR* pChannelMap;  /*!< Actual channel mapping for one single configuration. */
  UCHAR        numChannels;  /*!< The number of channels for the channel map which is the maximum used channel index+1. */
} CHANNEL_MAP_INFO;


/**
 * \brief   This is the main data struct. It contains the mapping for all channel configurations such as administration
 *          information.
 *
 * CAUTION: Do not access this structure directly from a algorithm specific library. Always use one of the API access
 *          functions below!
 */
typedef struct {
  const CHANNEL_MAP_INFO* pMapInfoTab;    /*!< Table of channel maps. */
  UINT                    mapInfoTabLen;  /*!< Length of the channel map table array. */
  UINT                    fPassThrough;   /*!< Flag that defines whether the specified mapping shall be applied
                                               (value: 0) or the input just gets passed through (MPEG mapping). */
} CDK_channelMapDescr;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief  Initialize a given channel map descriptor.
 *
 * \param  pMapDescr      Pointer to a channel map descriptor to be initialized.
 * \param  pMapInfoTab    Table of channel maps to initizalize the descriptor with.
 *                        If a NULL pointer is given a default table for WAV-like mapping will be used.
 * \param  mapInfoTabLen  Length of the channel map table array (pMapInfoTab). If a zero length is given
                          a default table for WAV-like mapping will be used.
 * \param  fPassThrough   If the flag is set the reordering (given by pMapInfoTab) will be bypassed.
 */
void CDK_chMapDescr_init(
        CDK_channelMapDescr* const    pMapDescr,
        const CHANNEL_MAP_INFO* const pMapInfoTab,
        const UINT                    mapInfoTabLen,
        const UINT                    fPassThrough
        );


/**
 * \brief  Change the channel reordering state of a given channel map descriptor.
 *
 * \param  pMapDescr     Pointer to a (initialized) channel map descriptor.
 * \param  fPassThrough  If the flag is set the reordering (given by pMapInfoTab) will be bypassed.
 * \return               Value unequal to zero if set operation was not successful. And zero on success.
 */
int CDK_chMapDescr_setPassThrough(
        CDK_channelMapDescr* const pMapDescr,
        UINT                       fPassThrough
        );


/**
 * \brief  Get the mapping value for a specific channel and map index.
 *
 * \param  pMapDescr  Pointer to channel map descriptor.
 * \param  chIdx      Channel index.
 * \param  mapIdx     Mapping index (corresponding to the channel configuration index).
 * \return            Mapping value.
 */
UCHAR CDK_chMapDescr_getMapValue(
        const CDK_channelMapDescr* const pMapDescr,
        const UCHAR                      chIdx,
        const UINT                       mapIdx
        );


/**
 * \brief  Evaluate whether channel map descriptor is reasonable or not.
 *
 * \param  pMapDescr Pointer to channel map descriptor.
 * \return           Value unequal to zero if descriptor is valid, otherwise zero.
 */
int CDK_chMapDescr_isValid(
        const CDK_channelMapDescr* const pMapDescr
        );


#ifdef __cplusplus
}
#endif

#endif /* !defined(__SYSLIB_CHANNELMAPDESCR_H__) */
