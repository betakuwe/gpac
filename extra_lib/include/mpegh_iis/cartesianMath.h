/******************************************************************************

                        (C) Copyright Fraunhofer IIS (2014)
                               All Rights Reserved

   $Id$
   Author(s):   Thomas Blender (fix-point version)
                Arthur Tritthart (XTENSA, ARM version)
   Description: Math functions for Vector Base Amplitude Panning (VBAP)

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef VBAPRenderer_CartesianMath_h
#define VBAPRenderer_CartesianMath_h

#include "CDK_core.h"
#include "fixpoint_math.h"
#include "CDK_trigFcts.h"

#if defined(__XTENSA_CONFIG__DC_E_330HiFi)
/* Note: [un]commenting the next line controls the use of the compile switch __XTENSA_VBAP__ */
/*       This is compliant to the scripts of the define-switcher toolset.                    */
#define FUNCTION__XTENSA_VBAP__
#endif
#ifdef FUNCTION__XTENSA_VBAP__
#define __XTENSA_VBAP__
#endif

typedef struct _PointSpherical
{
  /*
   * @param a Azimuth in radian
   * @param e Elevation in radian
   *
   * -180 ... 180 is mapped to -1 ... 1
   *  -pi ... pi  is mapped to -1 ... 1
   */

  FIXP_DBL azi;
  FIXP_DBL ele;

} PointSpherical;


/*
 * @brief Point (or vector) in Cartesian coordinates.
 */
typedef struct _PointCartesian
{
  /*
   * @param x Cartesian x coordinate
   * @param y Cartesian y coordinate
   * @param z Cartesian z coordinate
   *
   * Exponent for object data is normally 4 (because of the required radius of maximal 16m)
   * Exponent for speaker position is 0
   */

  FIXP_DBL x;
  FIXP_DBL y;
  FIXP_DBL z;

} PointCartesian;





/************************************************
 *     Function prototypes                      *
 ************************************************/

/*
 * @brief converts a spherical point to a Cartesian point
 * @param sph     spherical point, where azimuth and elevation is mapped -pi ... pi to -1 ... 1
 *
 * @return Cartesian coordinate, where x,y, and z is between -1 ... 1
 */
PointCartesian sphericalToCartesian(PointSpherical sph);

/*
 * @brief calculates the cross product of two Cartesian vectors
 * @param a, b    Cartesian points. Both exponents have to be the same.
 *
 * @return a x b. Result has an exponent of 1. In most cases the vector length don't care.
 */
PointCartesian crossProduct(PointCartesian *a, PointCartesian *b);

/*
 * @brief calculates the scalar product of two Cartesian vectors
 * @param a, b    Cartesian points. Both exponents have to be the same.
 *
 * @return a dot b. Result has a exponent of 2.
 */
FIXP_DBL dotProduct(PointCartesian *a, PointCartesian *b);

#if defined(__XTENSA_VBAP__)
/*
 * @brief calculates the scalar product of two Cartesian vectors (a and b)
 *        where each of them have to be divided by 2 before calling
 *        the function.
 * @param d0.H: a.x     d0.L: a.y
 * @param d1.H: a.z     d1.L: don't care
 * @param d2.H: b.x     d2.L: b.y
 * @param d3.H: b.z     d1.L: don't care
 *
 * @return FIXP_DBL = a dot b. Result has a exponent of 2.
 */
__attribute__((always_inline))
static ae_int32 dotProductDiv2(ae_int32x2 d0, ae_int32x2 d1, ae_int32x2 d2, ae_int32x2 d3)
{
  ae_int32x2 result;
  result = AE_MULFP32X2RAS(d0, d2);         /* result.H   = a.x * b.x         result.L = a.y * b.y */
  result = AE_ADD32_HL_LH(result, result);  /* result.H/L = (a.x * b.x) + (a.y * b.y)              */
  AE_MULAFP32X2RAS(result, d1, d3);         /* result.H   = result.H + (a.z * b.z)                 */
                                            /* result.L   = don't care                             */
  return AE_MOVAD32_H(result);              /* return 32 bit (result.H)                            */
}
#endif

/*
 * @brief calculates the length of a given vector.
 * @param a       is a Cartesian point or vector
 *
 * @return length   of a. Result has a exponent of 1.
 */
FIXP_DBL getLength(PointCartesian a);

/*
 * @brief normalizes the vector a. The entries of a normalized vector are always between -1 and 1
 *        Therefore the results exponent will be set to 0
 * @param a       is a Cartesian point or vector
 * @return 0 if succeed, 1 if error
 */
int normalize(PointCartesian *a);

#if defined(__XTENSA_VBAP__)
/*
 * @brief normalize the Cartesian vector a.
 * @param da0.H: a.x     da0.L: a.y
 * @param da1.H: a.z     da1.L: 0
 *
 */
void normalizeX2(ae_int32x2 &da0, ae_int32x2 &da1);
#endif




/************************************************
 *     Operator overloading                     *
 ************************************************/
PointCartesian operator- (const PointCartesian &lhs, const PointCartesian &rhs);
PointCartesian operator+ (const PointCartesian &lhs, const PointCartesian &rhs);
PointCartesian operator* (const FIXP_DBL &lhs, const PointCartesian &rhs);



#endif

