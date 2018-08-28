/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved

   $Id$
   Author(s):   M. Lohwasser, M. Gayer
   Description: min/max inline functions and defines

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef FIXMINMAX__H
#define FIXMINMAX__H

#include "CDK_archdef.h"
#include "machine_type.h"

/* Inline Function to determine the smaller/bigger value of two values with same type. */

#if defined(__ADSP21000__)
#include "sharc/fixminmax_sharc.h"

#else

template <class T> inline T fixmin (T a, T b)
{
	return (a < b ? a : b);
}

template <class T> inline T fixmax (T a, T b)
{
	return (a > b ? a : b);
}

#define fixmax_D(a,b) fixmax(a,b)
#define fixmin_D(a,b) fixmin(a,b)
#define fixmax_S(a,b) fixmax(a,b)
#define fixmin_S(a,b) fixmin(a,b)
#define fixmax_I(a,b) fixmax(a,b)
#define fixmin_I(a,b) fixmin(a,b)
#define fixmax_UI(a,b) fixmax(a,b)
#define fixmin_UI(a,b) fixmin(a,b)


#endif

#endif
