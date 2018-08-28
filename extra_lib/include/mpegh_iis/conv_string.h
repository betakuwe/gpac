/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2009)
                               All Rights Reserved

   $Id$
   Author(s):

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   conv_string.h
 *  \brief  String conversion functions.
 */

#ifndef _CONV_STRING_H
#define _CONV_STRING_H

#include "syslib_settings.h"

#ifdef CONVERT_STRING_ENABLE

#include "genericStds.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief                Convert a bitbuffer to a hex string.
 * \param string         Hex string.
 * \param charBuf        Input buffer that has to be converted.
 * \param charBufLength  Size of the input buffer.
 * \return               Error code.
 */
INT charBuf2HexString(char *string, UCHAR *charBuf, INT charBufLength);

/**
 * \brief                Convert a hex string to bits.
 * \param string         Hex string that has to be converted.
 * \param charBuf        Output buffer holding the bits, will be filled up with zeros.
 * \param charBufLength  Size of the output buffer.
 * \return               Error code.
 */
INT hexString2CharBuf(const char *string, UCHAR *charBuf, UINT charBufLength);

/**
 * \brief    Convert a character representing a hex sign to the appropriate value.
 * \param c  Character that has to be converted.
 * \return   Converted value (value between 0 and 15), if 16, an error has occured.
 */
UCHAR hexChar2Dec(const char c);

#ifdef __cplusplus
}
#endif

#endif /* CONVERT_STRING_ENABLE */

#endif /* _CONV_STRING_H */
