/******************************** MPEG Audio Encoder **************************

                       (C) copyright Fraunhofer-IIS (1999)
                               All Rights Reserved

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

   $Id$
   Initial author:
   contents/description: CRC calculation

******************************************************************************/

#ifndef CDK_CRC_H
#define CDK_CRC_H

#include "CDK_tools_settings.h"


#include "CDK_bitstream.h"


#define MAX_CRC_REGS 3    /*!< Maximal number of overlapping crc region in ADTS channel pair element is two.
                               Select three independent regions preventively. */

/**
 *  This structure describes single crc region used for crc calculation.
 */
typedef struct
{
  UCHAR isActive;
  INT   maxBits;
  UINT  bitBufCntBits;
  UINT  validBits;

} CCrcRegData;

/**
 *  CRC info structure.
 */
typedef struct
{
    CCrcRegData  crcRegData[MAX_CRC_REGS];   /*!< Multiple crc region description. */
    const USHORT       *pCrcLookup;          /*!< Pointer to lookup table filled in CDK_crcInit(). */

    USHORT              crcPoly;             /*!< CRC generator polynom. */
    USHORT              crcMask;             /*!< CRC mask. */
    USHORT              startValue;          /*!< CRC start value. */
    UCHAR               crcLen;              /*!< CRC length. */

    UINT                regStart;            /*!< Start region marker for synchronization. */
    UINT                regStop;             /*!< Stop region marker for synchronization. */

    USHORT              crcValue;            /*!< Crc value to be calculated. */

} CDK_CRCINFO;

/**
 *  CRC info handle.
 */
typedef CDK_CRCINFO* HANDLE_CDK_CRCINFO;


/**
 * \brief  Initialize CRC structure.
 *
 * The function initializes existing crc info structure with denoted configuration.
 *
 * \param hCrcInfo              Pointer to an outlying allocated crc info structure.
 * \param crcPoly               Configure crc polynom.
 * \param crcStartValue         Configure crc start value.
 * \param crcLen                Configure crc length.
 *
 * \return  none
 */
void CDKcrcInit(
        HANDLE_CDK_CRCINFO              hCrcInfo,
        const UINT                      crcPoly,
        const UINT                      crcStartValue,
        const UINT                      crcLen
        );

/**
 * \brief  Reset CRC info structure.
 *
 * This function clears all intern states of the crc structure.
 *
 * \param hCrcInfo              Pointer to crc info stucture.
 *
 * \return  none
 */
void CDKcrcReset(
        HANDLE_CDK_CRCINFO              hCrcInfo
        );


/**
 * \brief  Start CRC region with maximum number of bits.
 *
 * This function marks position in bitstream to be used as start point for crc calculation.
 * Bitstream range for crc calculation can be limited or kept dynamic depending on mBits parameter.
 * The crc region has to be terminated with CDKcrcEndReg() in each case.
 *
 * \param hCrcInfo              Pointer to crc info stucture.
 * \param hBs                   Pointer to current bit buffer structure.
 * \param mBits                 Number of bits in crc region to be calculated.
 *                              - mBits > 0: Zero padding will be used for CRC calculation, if there
 *                                           are less than mBits bits available.
 *                              - mBits < 0: No zero padding is done.
 *                              - mBits = 0: The number of bits used in crc calculation is dynamically,
 *                                           depending on bitstream position between CDKcrcStartReg() and
 *                                           CDKcrcEndReg() call.
 *
 * \return  ID for the created region, -1 in case of an error
 */
INT CDKcrcStartReg(
        HANDLE_CDK_CRCINFO              hCrcInfo,
        const HANDLE_CDK_BITSTREAM      hBs,
        const INT                       mBits
        );


/**
 * \brief  Ends CRC region.
 *
 * This function terminates crc region specified with CDKcrcStartReg(). The number of bits in crc region depends
 * on mBits parameter of CDKcrcStartReg().
 * This function calculates and updates crc in info structure.
 *
 * \param hCrcInfo              Pointer to crc info stucture.
 * \param hBs                   Pointer to current bit buffer structure.
 * \param reg                   Crc region ID created in CDKcrcStartReg().
 *
 * \return  0 on success
 */
INT CDKcrcEndReg(
        HANDLE_CDK_CRCINFO              hCrcInfo,
        const HANDLE_CDK_BITSTREAM      hBs,
        const INT                       reg
        );


/**
 * \brief  This function returns crc value from info struct.
 *
 * \param hCrcInfo              Pointer to crc info stucture.
 *
 * \return  CRC value masked with crc length.
 */
USHORT CDKcrcGetCRC(
        const HANDLE_CDK_CRCINFO        hCrcInfo
        );


#endif /* CDK_CRC_H */
