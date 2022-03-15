/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: OFDMInfo.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "OFDMInfo.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : boolean_T varargin_3
 *                double *info_NSubcarriers
 *                double *info_Nfft
 *                double *info_SubcarrierSpacing
 *                double *info_SampleRate
 *                double info_CyclicPrefixLengths_data[]
 *                int info_CyclicPrefixLengths_size[2]
 *                double *info_SymbolsPerSlot
 *                double *info_SlotsPerSubframe
 *                double *info_Resampling_L
 *                double *info_Resampling_M
 *                double *info_Resampling_TW
 *                double *info_Resampling_AStop
 * Return Type  : void
 */
void OFDMInfo(boolean_T varargin_3, double *info_NSubcarriers,
              double *info_Nfft, double *info_SubcarrierSpacing,
              double *info_SampleRate, double info_CyclicPrefixLengths_data[],
              int info_CyclicPrefixLengths_size[2], double *info_SymbolsPerSlot,
              double *info_SlotsPerSubframe, double *info_Resampling_L,
              double *info_Resampling_M, double *info_Resampling_TW,
              double *info_Resampling_AStop)
{
  double N_CP;
  int L;
  int i;
  if (varargin_3) {
    L = 12;
    N_CP = 512.0;
  } else {
    L = 14;
    N_CP = 144.0;
  }
  N_CP = N_CP / 2048.0 * 1024.0;
  info_CyclicPrefixLengths_size[0] = 1;
  info_CyclicPrefixLengths_size[1] = L;
  for (i = 0; i < L; i++) {
    info_CyclicPrefixLengths_data[i] = N_CP;
  }
  info_CyclicPrefixLengths_data[0] =
      (15360.0 - ((double)L * 1024.0 + ((double)L - 2.0) * N_CP)) / 2.0;
  info_CyclicPrefixLengths_data[(int)((double)L / 2.0 + 1.0) - 1] =
      (15360.0 - ((double)L * 1024.0 + ((double)L - 2.0) * N_CP)) / 2.0;
  *info_NSubcarriers = 624.0;
  *info_Nfft = 1024.0;
  *info_SubcarrierSpacing = 15000.0;
  *info_SampleRate = 1.536E+7;
  *info_SymbolsPerSlot = L;
  *info_SlotsPerSubframe = 1.0;
  *info_Resampling_L = 1.0;
  *info_Resampling_M = 1.0;
  *info_Resampling_TW = 1.0;
  *info_Resampling_AStop = 70.0;
}

/*
 * File trailer for OFDMInfo.c
 *
 * [EOF]
 */
