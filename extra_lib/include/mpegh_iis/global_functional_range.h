/***************************  Fraunhofer IIS CDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (1999)
                               All Rights Reserved

   $Id$
   Author(s):
   Description: Global functional range configuration. The macros in this file
                describe the supported functionality range for all libraries.
                Macros defined here should not be used in any object files
                directly.

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/
#ifndef __GLOBAL_FUNCTIONAL_RANGE_H__
#define __GLOBAL_FUNCTIONAL_RANGE_H__

/******************************* Audio Formats *******************************/

/*************************** Channel Configuration ***************************/
/*
  * In-/Output channels vs. core channels
  The channel configuration sets the number of encoder and decoder core
  channels. The number of in-/output channels is typically the same as the
  number of core channels.
  Exception: When Mpeg-D surround is enabled, the number of in-/output channels
  is set according to the Mpeg-D surround settings. (default: 6 channels).

  * Backwards compatibility
  Channel configurations include other channel configurations as pictured in
  the table below.

  CDK_GFR_CHANNEL_CONFIG | 22.2  | 7.1   | 5.1   | stereo| mono  
  -----------------------+-------+-------+-------+-------+-------
  CDK_GFR_CH_CFG_MONO    |       |       |       |       |   x   
  CDK_GFR_CH_CFG_STEREO  |       |       |       |   x   |   x   
  CDK_GFR_CH_CFG_5_1     |       |       |   x   |   x   |   x   
  CDK_GFR_CH_CFG_7_1     |       |   x   |   x   |   x   |   x   
  CDK_GFR_CH_CFG_22_2    |   x   |   x   |   x   |   x   |   x   

  CDK_GFR_CH_MPEGH_L3: MPEG-H profile level 3
*/

#define CDK_GFR_CH_CFG_MONO     0
#define CDK_GFR_CH_CFG_STEREO   1
#define CDK_GFR_CH_CFG_5_1      2
#define CDK_GFR_CH_CFG_7_1      3
#define CDK_GFR_CH_MPEGH_L3     4
#define CDK_GFR_CH_CFG_22_2     5
#define CDK_GFR_CH_MPEGH_L4     6

#define CDK_GFR_CHANNEL_CONFIG CDK_GFR_CH_MPEGH_L3


/********************************* Modifiers *********************************/
/* #define CDK_GFR_EVAL_PACKAGE */

/* #define CDK_GFR_FL_10MS */  /* 10ms raster frame length (960, 480) */
/* #define CDK_GFR_FL_3Q */    /* 3/4 frame length as used e.g. in USAC/RSVD50 */


#endif /* __GLOBAL_FUNCTIONAL_RANGE_H__ */

