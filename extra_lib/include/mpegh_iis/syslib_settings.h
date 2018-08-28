/**************************  Fraunhofer IIS CDK SysLib  **********************

                        (C) Copyright Fraunhofer IIS (2010)
                               All Rights Reserved

   $Id$
   Author(s):   Oliver Moser

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

/** \file   syslib_settings.h
 *  \brief  CDK sysLib settings to enable or disable certain components.
 */

#ifndef __SYSLIB_SETTINGS_H__
#define __SYSLIB_SETTINGS_H__


  /* #define AUDIO_ENABLE */
  #define CMDL_PARSER_ENABLE
  #define CONVERT_STRING_ENABLE
  /* #define MDMA_ENABLE */
  /* #define MLFILE_ENABLE */
  /* #define UART_ENABLE */
  #define WAV_FILE_ENABLE
  #define FILE_IO_SUPPORT

  /** \def STACKLOAD_MEASUREMENT_ENABLE
   *  Enable CDK stack load measurement. See CDK_stackload.h for details.
   */
  /* #define STACKLOAD_MEASUREMENT_ENABLE */

  /** \def WORKLOAD_MEASUREMENT_ENABLE
   *  Enable CDK work load measurement. See CDK_workload.h for details.
   */
  #define WORKLOAD_MEASUREMENT_ENABLE

/* dependencies */
#ifdef CMDL_PARSER_ENABLE
  #define CONVERT_STRING_ENABLE
#endif

/** \def    CDK_WORKLOAD_MEASUREMENT
 *          Include workload measurement code and make workload measurement globally available.
 *
 *  \def    MEMORY_MEASUREMENT
 *          Perform memory measurement. Every memory allocation or deallocation function counts the
 *          requested sizes and prints the current sum using CDKprintf()
 *
 *  \def    CDK_TESTING_ENABLE
 *          Perform additional runtime tests.
 *
 *  \def    CDK_DEBUG
 *          If defined then a compilation will include:
 *          -- debugging information inside libraries and print it,
 *          -- more sanity checks.
 *
 * \def     CDK_REENTRANT_ENABLE
 *          If defined all CDK libraries become re-entrant capable at the cost
 *          of more memory being required. Note that re-entrant is different to multiple instance capable.
 *
 * \def     EXTERNAL_RESOURCES
 *
 *          If set, all memory alloc operations for all modules using CDK_sysLib must be passed to CDK_feedMemory() first.
 *          Multiple instances of all modules are supported.
 *
 * \def     CDK_STD_MATH_WRAPPER_ENABLE
 *          If set, a variety of mathematical functions defined in standard math library will be provided in genericStds.
 */

/* #define CDK_WORKLOAD_MEASUREMENT */
/* #define MEMORY_MEASUREMENT */
/* #define CDK_DEBUG */
/* #define CDK_REENTRANT_ENABLE */
/* #define CDK_TESTING_ENABLE */
/* #define EXTERNAL_RESOURCES  */
#define CDK_STD_MATH_WRAPPER_ENABLE

  #define CDK_OVERLAYS_ENABLE
  #define CDK_SCRATCHBUFFER_STATIC_ENABLE

/* #define MEMORY_MEASUREMENT */


/* #define _CRT_SECURE_NO_DEPRECATE */

/* Tuning parameters */

/* Make use of the TSCL register for all C64x+ DSPS for cycle counting
 * so that the CPU cycles can be read directly instead of using a timer */


/** \mainpage CDK sysLib

\section SCOPE Scope
This document gives an overview of the features and usage of the CDK (Core Design Kit) System
Integration library developed by the Fraunhofer Institute for Integrated Circuits IIS.

\section GENERAL General
\image latex CDK_sysLib.png "Interaction of CDK sysLib with the rest of CDK world" width=13cm
The CDK sysLib library provides an abstraction level for basic system-related functions handled
differently on various platforms, but with a common generic API. The system integration library is
the only part of the entire CDK software that directly touches any platform dependent software or
hardware. All standard C library calls are wrapped using CDK specific functions. Also some additional
funtions are provided using a single API across all target environments. Depending on the actual
hardware, some features might be available or not. In the latter case dummy stubs will be used in
order to prevent the system integration libraries API from getting broken. Some components are
essential for any member of the CDK software family, and others are optional helpers for the testing
and measuring process.

The advantage of the architecture set forth is its flexibility for adding new platforms, and the
ability to write customized implementations of relevant system functions like malloc(), free(),
memcpy(), or  basic I/O like printf(), fopen(), etc. Any customized implementation can be added
without modifications in the libraries that use CDK sysLib.

Services provided:

- Wrappers for basic RTS functions
- Hardware setup (initialization code)
- Command line parser
- WAVE file reader and writer
- Soundcard I/O (ALSA, winmm, Blackfin EZKit, TI DSK6416)
- Workload measurement
- Heap and stack memory measurement
- Memory management

\section THIRDPARTY_LIBRARIES Third-party Libraries
CDK sysLib contains third-party libraries, that either are optional or required for CDK projects
to build. For example for TI platforms there are:

- Board Support Library (BSL)
- Chip Support Library (CSL)
- TI DSP library

The reason why these libraries are included in CDK sysLib is to ensure deterministic behaviour
for different users and system installations. CDK projects have been tested successfully with
the respective versions therein. Different CDK projects may use different functionalities of
these libraries and should be asked directly at the respective techsupport mail reflector.

*/


#endif /* __SYSLIB_SETTINGS_H__ */
