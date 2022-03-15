/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: SyncFrameInit.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 20-Feb-2022 08:55:56
 */

/* Include Files */
#include "SyncFrameInit.h"
#include "nrTimingEstimate.h"
#include "operation_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal32_T rxWaveform[921600]
 * Return Type  : double
 */
double SyncFrameInit(const creal32_T rxWaveform[921600])
{
  double refGrid[480];
  int i;
  memset(&refGrid[0], 0, 480U * sizeof(double));
  for (i = 0; i < 127; i++) {
    refGrid[i + 296] = iv6[i];
  }
  /*  Second OFDM symbol for correct CP length */
  /*  Timing estimation. This is the timing offset to the OFDM symbol prior to
   */
  /*  the detected SSB due to the content of the reference grid */
  /* rxSampleRate = 46.08e6; */
  /* samplesPerSymbol = round(rxSampleRate*(0.001/2)/14); */
  return nrTimingEstimate(rxWaveform, refGrid) - 1644.0;
  /* timingOffset = nrTimingEstimateOpt(rxWaveform,refGrid,0,rxSampleRate) -
   * samplesPerSymbol; */
}

/*
 * File trailer for SyncFrameInit.c
 *
 * [EOF]
 */
