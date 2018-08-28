/**************************  Fraunhofer IIS CDK SysLib  **********************
 
                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved
 
   $Id$
   Author(s):   Manuel Jander

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.
 
******************************************************************************/

/** \file   CDK_workload.h
 *  \brief  Workload information gathering and evaluation.

This component is a flexible tool to measure workload of different code parts. Subsequent calls
of CDKwrkLoadStart() and CDKwrkLoadStop() can add up to a total amount of workload of several
congruent code parts.

The CDKwrkLoadStop() receives as additional information the amount of time the processed data
corresponds to (for example, how many seconds worth of audio data the encompassed program code
processed). That way the workload measurement tool can estimate the percentage of achieved
real-time performance. If a given portion of program code does not yet yield any known amount
of real-time equivalency, the real-time parameter of CDKwrkLoadStop() can be zero, in which
case the processing time will be added to the next CDKwrkLoadStart()-CDKwrkLoadStop()-encompassed
program code interval. Finally the functions CDKwrkLoadPrint() or CDKwrkLoadsPrint() can be used
to generate an appropriate text output of the measurements gathered so far. The built-in
measurements comprise of total average, peak, and sliding window (smoothed) peak values.

*/


#ifndef CDK_WRKLOAD_H
#define CDK_WRKLOAD_H

#include "syslib_settings.h"

#ifdef WORKLOAD_MEASUREMENT_ENABLE

#include "genericStds.h"

#ifdef __linux__
#include <stdio.h>
#endif

#define CDK_WRKLOAD_AVG     0x1         /**< Flag indicating print of average workload consumption over all measured frames. */
#define CDK_WRKLOAD_SLD     0x2         /**< Flag indicating print of sliding window average consumption.                    */
#define CDK_WRKLOAD_PEAK    0x4         /**< Flag indicating print of peak workload consumption.                             */
#define CDK_WRKLOAD_SLDPK   0x8         /**< Flag indicating print of sliding window peak consumption.                       */
#define CDK_WRKLOAD_SUM     0x8000      /**< Flag indicating print of total consumption.                                     */
#define CDK_WRKLOAD_FORCE   0x80000000  /**< Flag indicating print of results, even if not enough samples available.         */
#define CDK_WRKLOAD_ALL     0x7fffffff  /**< Flag indicating print of everything.                                            */

#define CDK_WRKLOAD_PART  (CDK_WRKLOAD_AVG|CDK_WRKLOAD_PEAK|CDK_WRKLOAD_SLD)                                     /**< Describes what to print after CDK_WRKLOAD_NUM_MEASURES frames. */
#define CDK_WRKLOAD_FINAL (CDK_WRKLOAD_AVG|CDK_WRKLOAD_PEAK|CDK_WRKLOAD_SUM|\
                           CDK_WRKLOAD_SLDPK|CDK_WRKLOAD_FORCE)                                                  /**< Describes what to print at the end of processing.              */

#define CDK_WRKLOAD_NUM_MEASURES  5   /**< Number of frames after workload measurement is printed. Defines length of one sliding window. Output controlled by CDK_WRKLOAD_PART.      */
#define CDK_WRKLOAD_LABELSIZE     32  /**< Maximum number of characters used to label one workload measurment module.    */

/* typedef INT64 cdk_clocks_t; */
typedef float cdk_clocks_t;

/** Information field for workload measurement. */
typedef struct {
  UINT clocks_start;                                          /**< Internal clocks start value.     */
  UINT cMeasures;                                             /**< Count of measures.               */
  float measure_avg;                                          /**< Measure average value.           */
  float measure_peak;                                         /**< Measure peak value.              */
  float measure_sw;                                           /**< Measure sliding window value.    */
  float measure_swpeak;                                       /**< Measure sliding window value.    */

  float history[CDK_WRKLOAD_NUM_MEASURES];                    /**< Measurement history buffer.      */
  UINT hist_idx;                                              /**< History index.                   */
  float accu;                                                 /**< Accumulate deltas during accu condition.        */
  int  fAccu;                                                 /**< Flag accu condition (zero realtime).            */
  int fPrint;                                                 /**< Flag indicating that results where printed.     */
  char label[CDK_WRKLOAD_LABELSIZE];                          /**< Brief label for one workload measurment module. */
#ifdef __linux__
  FILE *fp1; /* File for minimum frame time storage. */
#endif
} CDK_WRKLOAD;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Create a workload element instance.
 * \param pWrkLoad  Pointer to workload element pointer.
 * \param label     String describing the workload element. Must not exceed CDK_WRKLOAD_LABELSIZE characters.
 */
void CDKwrkLoadCreate(CDK_WRKLOAD **pWrkLoad, const char *label);

/**
 * \brief          Initialize a workload element.
 * \param wrkload  Pointer to workload element pointer.
 * \param label    String describing the workload element.
 */
void CDKwrkLoadInit(CDK_WRKLOAD **wrkload, const char *label);

/**
 * \brief          Start of workload clock counting.
 * \param wrkload  Pointer to workload element.
 */
void CDKwrkLoadStart(CDK_WRKLOAD *wrkload);

/**
 * \brief          Stop of workload clock counting and update statistics.
 * \param wrkLoad  Pointer to workload element.
 * \param usecds   Microseconds of realtime between start and (this) stop call.
 */
void CDKwrkLoadStop(CDK_WRKLOAD *wrkLoad, UINT usecds);

/**
 * \brief          Print workload measurement results into a string buffer.
 * \param wrkload  Pointer to workload element pointer.
 * \param flags    Flags describing what to print.
 * \param buffer   Target buffer for the workload result string.
 */
void CDKwrkLoadsPrint(CDK_WRKLOAD **wrkload, INT flags, char *buffer);

/**
 * \brief          Print workload measurement results to stdout.
 * \param wrkload  Pointer to workload element pointer.
 * \param flags    Flags describing what to print.
 */
void CDKwrkLoadPrint(CDK_WRKLOAD **wrkload, INT flags);

/**
 * \brief Shut down workload measurement.
 */
void CDKwrkLoadClose(CDK_WRKLOAD *wrkLoad);

/**
 * \brief          Delete a workload element instance.
 * \param wrkLoad  Pointer to workload element pointer.
 */
void CDKwrkLoadDelete(CDK_WRKLOAD **wrkLoad);

#ifdef __cplusplus
}
#endif		
	
#endif /* #ifdef WORKLOAD_MEASUREMENT_ENABLE */

#endif /* #ifndef CDK_WRKLOAD_H */
