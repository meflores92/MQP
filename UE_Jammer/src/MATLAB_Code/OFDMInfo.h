/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: OFDMInfo.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

#ifndef OFDMINFO_H
#define OFDMINFO_H

/* Include Files */
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void OFDMInfo(boolean_T varargin_3, double *info_NSubcarriers,
              double *info_Nfft, double *info_SubcarrierSpacing,
              double *info_SampleRate, double info_CyclicPrefixLengths_data[],
              int info_CyclicPrefixLengths_size[2], double *info_SymbolsPerSlot,
              double *info_SlotsPerSubframe, double *info_Resampling_L,
              double *info_Resampling_M, double *info_Resampling_TW,
              double *info_Resampling_AStop);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for OFDMInfo.h
 *
 * [EOF]
 */
