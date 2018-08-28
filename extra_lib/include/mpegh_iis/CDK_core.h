/***************************  Fraunhofer IIS CDK Tools  ***********************
 
                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved
 
   $Id$
   Author(s):   Manuel Jander
   Description: CDK tools versioning support

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.
 
******************************************************************************/

#ifndef CDK_CORE_H
#define CDK_CORE_H

#include "CDK_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get CDK_tools library information.
 *  @return     Return 0 on success and a negative errorcode on failure (see errorcodes.h).
 */
int CDK_toolsGetLibInfo(LIB_INFO *info);

#ifdef __cplusplus
}
#endif

#endif
