/********************************  CDK_tools  ******************************

                        (C) Copyright Fraunhofer IIS (2011)
                               All Rights Reserved

   $Id$
   Author(s):   Manuel Jander
   Description: LPC related functions

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/
#ifndef CDK_LPC_H
#define CDK_LPC_H

#include "CDK_tools_settings.h"


#include "common_fix.h"

#define LPC_MAX_ORDER 24


typedef FIXP_SGL FIXP_LPC;
#define FX_DBL2FX_LPC(x) FX_DBL2FX_SGL((FIXP_DBL)(x))
#define FX_DBL2FXCONST_LPC(x) FX_DBL2FXCONST_SGL(x)
#define FX_LPC2FX_DBL(x) FX_SGL2FX_DBL(x)
#define FL2FXCONST_LPC(val)   FL2FXCONST_SGL(val)
#define MAXVAL_LPC MAXVAL_SGL

#if defined(CLPC_ANALYSIS_ENABLE)
/**
 * \brief Obtain residual signal through LPC analysis.
 * \param signal pointer to buffer holding signal to be analysed. Residual is returned there (in place)
 * \param signal_size the size of the input data in pData
 * \param lpcCoeff_m the LPC filter coefficient mantissas
 * \param lpcCoeff_e the LPC filter coefficient exponent
 * \param order the LPC filter order (size of coeff)
 * \param filtState Pointer to state buffer of size order
 * \param filtStateIndex pointer to state index storage
 */
void CLpc_Analysis(
        FIXP_DBL signal[],
        const int signal_size,
        const FIXP_LPC lpcCoeff_m[],
        const int lpcCoeff_e,
        const int order,
        FIXP_DBL *filtState,
        int * filtStateIndex
        );
#endif

#if defined(CLPC_ANALYSISLATTICE_ENABLE)
/**
 * \brief Obtain residual signal through LPC analysis using ParCor/ reflection coefficients.
 * \param signal pointer to buffer holding signal to be analysed. Residual is returned there (in place)
 * \param signal_size the size of the input data in pData
 * \param inc buffer traversal increment for pData
 * \param pCoeff the LPC filter coefficients
 * \param order the LPC filter order (size of coeff)
 * \param filtState filter state buffer
 */
void CLpc_AnalysisLattice(
        FIXP_DBL signal[],
        const int signal_size,
        const FIXP_LPC pCoeff[],
        const int order,
        FIXP_DBL *filtState
        );
#endif

#if defined(CLPC_SYNTHESIS_ENABLE)
/**
 * \brief Synthesize signal fom residual through LPC synthesis, using LP coefficients.
 * \param signal pointer to buffer holding the residual signal. The synthesis is returned there (in place)
 * \param signal_size the size of the input data in pData
 * \param inc buffer traversal increment for signal
 * \param coeff the LPC filter coefficients
 * \param coeff_e exponent of coeff
 * \param order the LPC filter order (size of coeff)
 * \param state state buffer of size LPC_MAX_ORDER
 * \param pStateIndex pointer to state index storage
 */
void CLpc_Synthesis (
        FIXP_DBL *signal,
        const int signal_size,
        const int signal_e,
        const int inc,
        const FIXP_LPC coeff [],
        const int coeff_e,
        const int order,
        FIXP_DBL *filtState,
        int *pStateIndex
        );
#endif

#if defined(CLPC_SYNTHESISLATTICE_ENABLE)
/**
 * \brief Synthesize signal fom residual through LPC synthesis, using ParCor coefficients. The algorithm
 *        assumes a filter gain of max 1.0. If the filter gain is higher, this must be accounted into the
 *        values of signal_e and/or signal_e_out to avoid overflows.
 * \param signal pointer to buffer holding the residual signal. The synthesis is returned there (in place)
 * \param signal_size the size of the input data in pData
 * \param inc buffer traversal increment for signal
 * \param coeff the LPC filter coefficients
 * \param coeff_e exponent of coeff
 * \param order the LPC filter order (size of coeff)
 * \param state state buffer of size LPC_MAX_ORDER
 */
void CLpc_SynthesisLattice(
        FIXP_DBL *signal,
        const int signal_size,
        const int signal_e,
        const int signal_e_out,
        const int inc,
        const FIXP_SGL *coeff,
        const int order,
        FIXP_DBL *state
        );

void CLpc_SynthesisLattice(
        FIXP_DBL *signal,
        const int signal_size,
        const int signal_e,
        const int signal_e_out,
        const int inc,
        const FIXP_DBL *coeff,
        const int order,
        FIXP_DBL *state
        );
#endif

#if defined(CLPC_PARCOR_TO_LPC_ENABLE)
/**
 * \brief
 */
INT CLpc_ParcorToLpc(
        const FIXP_LPC reflCoeff[],
        FIXP_LPC LpcCoeff[],
        const int numOfCoeff,
        FIXP_DBL workBuffer[]
        );
#endif

#if defined(CLPC_AUTO_TO_PARCOR_ENABLE)
/** 
 * \brief Calculate ParCor (Partial autoCorrelation, reflection) coefficients from autocorrelation
 *        coefficients using the Schur algorithm (instead of Levinson Durbin).
 * \param acorr order+1 autocorrelation coefficients
 * \param reflCoeff output reflection /ParCor coefficients. The first coefficient which is always 1.0 is ommitted.
 * \param order number of acorr / reflCoeff coefficients.
 * \param pPredictionGain_m prediction gain mantissa
 * \param pPredictionGain_e prediction gain exponent
 */
void CLpc_AutoToParcor(
        FIXP_DBL  acorr[],
        const int acorr_e,
        FIXP_LPC reflCoeff[],
        const int order,
        FIXP_DBL *pPredictionGain_m,
        INT      *pPredictionGain_e
        );
#endif

#if defined(CLPC_AUTOCORR_ENABLE)
/**
 * \brief Calculate the autocorrelation of any order and size
 * \param signal input signal
 * \param acorr pointer to output buffer of size "order" or bigger.
 * \param signal_size the size of the input signal "signal".
 * \param order the order of the autocorrelation to be calculated.
 * \param scale a right shift to be applied to the autocorrelation output.
 * \return energy ( = acorr[0] without normalization).
 */
FIXP_DBL CLpc_AutoCorr(
        const FIXP_DBL *signal,
        const int       signal_e,
        FIXP_DBL       *acorr,
        INT            *pAcorr_e,
        const int       signal_size,
        const int       order
        );
#endif

#if defined(CLPC_AUTO_TO_LPC_ENABLE)
/**
 * \brief Levinson-Durbin algorithm to compute
 *        the LPC parameters from given autocorrelation value.
 * \param lpcCoeff_m pointer to where the mantissas of output LPC coeffcients will be stored into.
 * \param lpcCoeff_e Pointer to where the exponent of lpcCoeff_m will be stored into.
 * \param R pointer to m+1 input autocorrelation coefficients.
 * \param m LPC order
 * \param rc pointer to where reflection will be stored into (currently not implemened).
 */
void CLpc_AutoToLpc(
        FIXP_LPC *lpcCoeff_m,
        INT *lpcCoeff_e,
        const FIXP_DBL *R,
        const INT R_e,
        const int m,
        FIXP_DBL *rc,
        const int is_ltpf
        );

void CLpc_AutoToLpcIGF(
        FIXP_DBL *lpcCoeff_m,
        INT *lpcCoeff_e,
        const FIXP_DBL *R,
        const INT R_e,
        const int m,
        FIXP_DBL *rc
        );
#endif /* defined(CLPC_AUTO_TO_LPC_ENABLE) */


#endif /* CDK_LPC_H */
