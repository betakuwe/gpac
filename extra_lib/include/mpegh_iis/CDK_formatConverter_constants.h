#ifndef __FORMATCONVERTER_CONSTANTS_H__
#define __FORMATCONVERTER_CONSTANTS_H__

#include "global_functional_range.h"



#include "common_fix.h"
extern const FIXP_DBL f_bands_nrm_71[];

/*-------------------------------------------------------------*/
/*   constants for fft transform size of 512 coefficients      */
/*-------------------------------------------------------------*/
extern const FIXP_DBL f_bands_nrm_stft_256_erb_58[];


#ifdef CDK_IMMERSIVE_FORMAT_CONVERTER

/* The following tables are halfed. Its exponent is 1. */

extern const FIXP_DBL EQVF_StftErb[];

extern const FIXP_DBL EQVB_StftErb[];

extern const FIXP_DBL EQVFC_StftErb[];

extern const FIXP_DBL EQVBC_StftErb[];

extern const FIXP_DBL EQVOG_StftErb[];

extern const FIXP_DBL EQVS_StftErb[];

extern const FIXP_DBL EQBTM_StftErb[];

extern const FIXP_DBL EQVBA_StftErb[];

extern const FIXP_DBL COLOR_060_030_StftErb[];

extern const FIXP_DBL COLOR_090_030_StftErb[];

extern const FIXP_DBL COLOR_060_110_StftErb[];

extern const FIXP_DBL COLOR_090_110_StftErb[];

extern const FIXP_DBL COLOR_135_110_StftErb[];

extern const FIXP_DBL COLOR_180_110_StftErb[];

/* TABLE : INITIAL SPATIAL LOCALIZATION PANNING COEFFICIENTS G_VH. Modifiable by initElevSptlParms. */
extern const FIXP_DBL GVH[13][6];

/* TABLE : INITIAL SPATIAL LOCALIZATION PANNING COEFFICIENTS G_VL. Exponent = 1. Modifiable by initElevSptlParms. */
extern const FIXP_DBL GVL[13][6];

/* TABLE : INITIAL SPATIAL LOCALIZATION PANNING COEFFICIENTS G_VH. Modifiable by initElevSptlParms. */
extern const INT GVH_e[13][6];

/* TABLE : INITIAL SPATIAL LOCALIZATION PANNING COEFFICIENTS G_VL. Exponent = 1. Modifiable by initElevSptlParms. */
extern const INT GVL_e[13][6];

extern const FIXP_DBL fc_StftErb[];

#endif


#endif /*__FORMATCONVERTER_CONSTANTS_H__ __FORMATCONVERTER_CONSTANTS_H__ */
