/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2003)
                               All Rights Reserved

   $Id$
   Author(s): Manuel Jander

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   coresup.h
 *  \brief  System support routines
 */

#ifndef CORESUP_H
#define CORESUP_H

#include "syslib_settings.h"


#include "CDK_timerdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set the whole system into a reasonable state.
 */
void coresup_init(void);

/**
 * Set the status of a LED.
 */
void coresup_LEDset(int led, int en);

/**
 * Toggle the status of a LED.
 */
void coresup_LEDtoggle(int led);

/**
 * Read status of a button or switch.
 */
int  coresup_buttonStatus(int btn);

/**
 * Read all buttons at once. Returns a bit mask. bit 0 is button 0 ...
 */
unsigned int coresup_buttonStatusAll(void);

/**
 * Read timer value. This value increases CDK_CLOCKS_PER_SEC in one second.
 */
int CDKclock(void);

#ifdef __cplusplus
}
#endif


#endif /* CORESUP_H */
